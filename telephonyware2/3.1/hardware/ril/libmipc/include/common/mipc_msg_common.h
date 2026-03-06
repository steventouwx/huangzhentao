#ifndef __MIPC_MSG_COMMON_CONST_H__
#define __MIPC_MSG_COMMON_CONST_H__

enum MIPC_MSG_enum {
    MIPC_MSG_NONE = 0,
    MIPC_REQ_BEGIN                                          = 0,

    MIPC_IND_BEGIN                                          = 0x4000,

    MIPC_CMD_BEGIN                                          = 0x8000,

    MIPC_NTF_BEGIN                                          = 0xC000,

};

enum MIPC_COMMON_TLV_enum {
    MIPC_COMMON_TLV_NONE = 0,
    /*  */
    MIPC_T_RESULT                                           = 0,
    /*  */
    MIPC_T_MSG_VERSION                                      = 1,
};

enum mipc_boolean_const_enum {
    mipc_boolean_const_NONE = 0,
    /* BOOLEAN */
    MIPC_BOOLEAN_FALSE                                      = 0,
    MIPC_BOOLEAN_TRUE                                       = 1,
};
typedef enum mipc_boolean_const_enum mipc_boolean_const_enum;

enum mipc_sys_sim_ps_const_enum {
    mipc_sys_sim_ps_const_NONE = 0,
    /* SYS_SIM_PS */
    MIPC_SYS_SIM_PS_SIM0                                    = 0x01,
    MIPC_SYS_SIM_PS_SIM1                                    = 0x02,
    MIPC_SYS_SIM_PS_SIM2                                    = 0x04,
    MIPC_SYS_SIM_PS_SIM3                                    = 0x08,
    MIPC_SYS_SIM_PS_PS0                                     = 0x10,
    MIPC_SYS_SIM_PS_PS1                                     = 0x20,
    MIPC_SYS_SIM_PS_PS2                                     = 0x40,
    MIPC_SYS_SIM_PS_PS3                                     = 0x80,
    MIPC_SYS_SIM_PS_ALL                                     = 0xFF,
};
typedef enum mipc_sys_sim_ps_const_enum mipc_sys_sim_ps_const_enum;

enum mipc_sys_reboot_mode_const_enum {
    mipc_sys_reboot_mode_const_NONE = 0,
    /* SYS_REBOOT_MODE */
    /* normal reboot mode */
    MIPC_SYS_REBOOT_MODE_NORMAL                             = 0,
    MIPC_SYS_REBOOT_MODE_DOWNLOAD                           = 1,
    MIPC_SYS_REBOOT_MODE_POWEROFF                           = 2,
    MIPC_SYS_REBOOT_MODE_PRESHUTDOWN                        = 3,
    MIPC_SYS_REBOOT_MODE_DOWNLOAD_NONRST                    = 4,
    MIPC_SYS_REBOOT_MODE_SBP_CHANGE                         = 5,
    MIPC_SYS_REBOOT_MODE_FAST_DOWNLOAD                      = 6,
};
typedef enum mipc_sys_reboot_mode_const_enum mipc_sys_reboot_mode_const_enum;

enum mipc_sys_cap_cellular_class_const_enum {
    mipc_sys_cap_cellular_class_const_NONE = 0,
    /* SYS_CAP_CELLULAR_CLASS */
    MIPC_SYS_CAP_CELLULAR_CLASS_GSM                         = 0x00000001,
    MIPC_SYS_CAP_CELLULAR_CLASS_CDMA                        = 0x00000002,
};
typedef enum mipc_sys_cap_cellular_class_const_enum mipc_sys_cap_cellular_class_const_enum;

enum mipc_sys_voice_class_const_enum {
    mipc_sys_voice_class_const_NONE = 0,
    /* SYS_VOICE_CLASS */
    MIPC_SYS_VOICE_CLASS_UNKNOWN                            = 0,
    MIPC_SYS_VOICE_CLASS_NO_VOICE                           = 1,
    MIPC_SYS_VOICE_CLASS_SEPARATE_VOICE_DATA                = 2,
    MIPC_SYS_VOICE_CLASS_SIMULTANEOUS_VOICE_DATA            = 3,
};
typedef enum mipc_sys_voice_class_const_enum mipc_sys_voice_class_const_enum;

enum mipc_sys_sim_class_const_enum {
    mipc_sys_sim_class_const_NONE = 0,
    /* SYS_SIM_CLASS */
    MIPC_SYS_SIM_CLASS_UNKNOWN                              = 0,
    MIPC_SYS_SIM_CLASS_LOGICAL                              = 1,
    MIPC_SYS_SIM_CLASS_REMOVABLE                            = 2,
};
typedef enum mipc_sys_sim_class_const_enum mipc_sys_sim_class_const_enum;

enum mipc_sys_cap_data_const_enum {
    mipc_sys_cap_data_const_NONE = 0,
    /* SYS_CAP_DATA */
    MIPC_SYS_CAP_DATA_NONE                                  = 0x00000000,
    MIPC_SYS_CAP_DATA_GPRS                                  = 0x00000001,
    MIPC_SYS_CAP_DATA_EDGE                                  = 0x00000002,
    MIPC_SYS_CAP_DATA_UMTS                                  = 0x00000004,
    MIPC_SYS_CAP_DATA_HSDPA                                 = 0x00000008,
    MIPC_SYS_CAP_DATA_HSUPA                                 = 0x00000010,
    MIPC_SYS_CAP_DATA_LTE                                   = 0x00000020,
    MIPC_SYS_CAP_DATA_5G_NSA                                = 0x00000040,
    MIPC_SYS_CAP_DATA_5G_SA                                 = 0x00000080,
    MIPC_SYS_CAP_DATA_1XRTT                                 = 0x00010000,
    MIPC_SYS_CAP_DATA_1XEVDO                                = 0x00020000,
    MIPC_SYS_CAP_DATA_1XEVDO_REVA                           = 0x00040000,
    MIPC_SYS_CAP_DATA_1XEVDV                                = 0x00080000,
    MIPC_SYS_CAP_DATA_3XRTT                                 = 0x00100000,
    MIPC_SYS_CAP_DATA_1XEVDO_REVB                           = 0x00200000,
    MIPC_SYS_CAP_DATA_UMB                                   = 0x00400000,
    MIPC_SYS_CAP_DATA_HSPA_PLUS                             = 0x40000000,
    MIPC_SYS_CAP_DATA_CUSTOM                                = 0x80000000,
};
typedef enum mipc_sys_cap_data_const_enum mipc_sys_cap_data_const_enum;

enum mipc_sys_cap_sms_const_enum {
    mipc_sys_cap_sms_const_NONE = 0,
    /* SYS_CAP_SMS */
    MIPC_SYS_CAP_SMS_NONE                                   = 0x00000000,
    MIPC_SYS_CAP_SMS_PDU_RECEIVE                            = 0x00000001,
    MIPC_SYS_CAP_SMS_PDU_SEND                               = 0x00000002,
    MIPC_SYS_CAP_SMS_TEXT_RECEIVE                           = 0x00000004,
    MIPC_SYS_CAP_SMS_TEXT_SEND                              = 0x00000008,
};
typedef enum mipc_sys_cap_sms_const_enum mipc_sys_cap_sms_const_enum;

enum mipc_sys_cap_ctrl_const_enum {
    mipc_sys_cap_ctrl_const_NONE = 0,
    /* SYS_CAP_CTRL */
    MIPC_SYS_CAP_CTRL_NONE                                  = 0x00000000,
    /* Indicates whether device can select network maually */
    MIPC_SYS_CAP_CTRL_REG_MANUAL                            = 0x00000001,
    MIPC_SYS_CAP_CTRL_HW_RADIO_SWITCH                       = 0x00000002,
    MIPC_SYS_CAP_CTRL_CDMA_MOBILE_IP                        = 0x00000004,
    MIPC_SYS_CAP_CTRL_CDMA_SIMPLE_IP                        = 0x00000008,
    MIPC_SYS_CAP_CTRL_MULTI_CARRIER                         = 0x00000010,
    MIPC_SYS_CAP_CTRL_ESIM                                  = 0x00000020,
    MIPC_SYS_CAP_CTRL_UE_POLICY_ROUTE_SELECTION             = 0x00000040,
    MIPC_SYS_CAP_CTRL_SIM_HOT_SWAP_CAPABLE                  = 0x00000080,
};
typedef enum mipc_sys_cap_ctrl_const_enum mipc_sys_cap_ctrl_const_enum;

enum mipc_sys_auth_algo_const_enum {
    mipc_sys_auth_algo_const_NONE = 0,
    /* SYS_AUTH_ALGO */
    MIPC_SYS_AUTH_ALGO_NONE                                 = 0,
    MIPC_SYS_AUTH_ALGO_SIM                                  = 1,
    /* Extensible Authentication Protocol Method for 3rd Generation Authentication and Key Agreement, refer to 3GPP TS 33.102 */
    MIPC_SYS_AUTH_ALGO_AKA                                  = 2,
    /* Improved Extensible Authentication Protocol Method for 3rd Generation Authentication and Key Agreement, refer to 3GPP TS 33.402 and RFC 5448 */
    MIPC_SYS_AUTH_ALGO_AKAP                                 = 3,
};
typedef enum mipc_sys_auth_algo_const_enum mipc_sys_auth_algo_const_enum;

enum mipc_sys_cap_service_const_enum {
    mipc_sys_cap_service_const_NONE = 0,
    /* SYS_CAP_SERVICE */
    MIPC_SYS_CAP_SERVICE_NONE                               = 0x00000000,
    MIPC_SYS_CAP_SERVICE_LTE_ATTACH                         = 0x00000001,
    MIPC_SYS_CAP_SERVICE_CONTEXT_MGMT                       = 0x00000002,
    MIPC_SYS_CAP_SERVICE_MULTI_SIM                          = 0x00000004,
    MIPC_SYS_CAP_SERVICE_EX_SAR                             = 0x00000008,
    MIPC_SYS_CAP_SERVICE_NETWORK_BLACKLIST                  = 0x00000010,
    MIPC_SYS_CAP_SERVICE_5G_NSA                             = 0x00000020,
};
typedef enum mipc_sys_cap_service_const_enum mipc_sys_cap_service_const_enum;

enum mipc_sys_cap_gsm_band_const_enum {
    mipc_sys_cap_gsm_band_const_NONE = 0,
    /* SYS_CAP_GSM_BAND */
    MIPC_SYS_CAP_GSM_BAND_900                               = 0x00000002,
    MIPC_SYS_CAP_GSM_BAND_DCS_1800                          = 0x00000008,
    MIPC_SYS_CAP_GSM_BAND_PCS_1900                          = 0x00000010,
    MIPC_SYS_CAP_GSM_BAND_PCS_850                           = 0x00000080,
};
typedef enum mipc_sys_cap_gsm_band_const_enum mipc_sys_cap_gsm_band_const_enum;

enum mipc_sys_cap_umts_band_const_enum {
    mipc_sys_cap_umts_band_const_NONE = 0,
    /* SYS_CAP_UMTS_BAND */
    MIPC_SYS_CAP_UMTS_BAND_I                                = 0x00000001,
    MIPC_SYS_CAP_UMTS_BAND_II                               = 0x00000002,
    MIPC_SYS_CAP_UMTS_BAND_III                              = 0x00000004,
    MIPC_SYS_CAP_UMTS_BAND_IV                               = 0x00000008,
    MIPC_SYS_CAP_UMTS_BAND_V                                = 0x00000010,
    MIPC_SYS_CAP_UMTS_BAND_VI                               = 0x00000020,
    MIPC_SYS_CAP_UMTS_BAND_VII                              = 0x00000040,
    MIPC_SYS_CAP_UMTS_BAND_VIII                             = 0x00000080,
    MIPC_SYS_CAP_UMTS_BAND_IX                               = 0x00000100,
    MIPC_SYS_CAP_UMTS_BAND_X                                = 0x00000200,
};
typedef enum mipc_sys_cap_umts_band_const_enum mipc_sys_cap_umts_band_const_enum;

enum mipc_sys_cap_cdma_band_const_enum {
    mipc_sys_cap_cdma_band_const_NONE = 0,
    /* SYS_CAP_CDMA_BAND */
    MIPC_SYS_CAP_CDMA_BAND_0                                = 0x00000001,
    MIPC_SYS_CAP_CDMA_BAND_1                                = 0x00000002,
    MIPC_SYS_CAP_CDMA_BAND_2                                = 0x00000004,
    MIPC_SYS_CAP_CDMA_BAND_3                                = 0x00000008,
    MIPC_SYS_CAP_CDMA_BAND_4                                = 0x00000010,
    MIPC_SYS_CAP_CDMA_BAND_5                                = 0x00000020,
    MIPC_SYS_CAP_CDMA_BAND_6                                = 0x00000040,
    MIPC_SYS_CAP_CDMA_BAND_7                                = 0x00000080,
    MIPC_SYS_CAP_CDMA_BAND_8                                = 0x00000100,
    MIPC_SYS_CAP_CDMA_BAND_9                                = 0x00000200,
    MIPC_SYS_CAP_CDMA_BAND_10                               = 0x00000400,
    MIPC_SYS_CAP_CDMA_BAND_11                               = 0x00000800,
    MIPC_SYS_CAP_CDMA_BAND_12                               = 0x00001000,
    MIPC_SYS_CAP_CDMA_BAND_13                               = 0x00002000,
    MIPC_SYS_CAP_CDMA_BAND_14                               = 0x00004000,
    MIPC_SYS_CAP_CDMA_BAND_15                               = 0x00008000,
    MIPC_SYS_CAP_CDMA_BAND_16                               = 0x00010000,
    MIPC_SYS_CAP_CDMA_BAND_17                               = 0x00020000,
    MIPC_SYS_CAP_CDMA_BAND_18                               = 0x00040000,
    MIPC_SYS_CAP_CDMA_BAND_19                               = 0x00080000,
    MIPC_SYS_CAP_CDMA_BAND_20                               = 0x00100000,
};
typedef enum mipc_sys_cap_cdma_band_const_enum mipc_sys_cap_cdma_band_const_enum;

enum mipc_sys_cap_tds_band_const_enum {
    mipc_sys_cap_tds_band_const_NONE = 0,
    /* SYS_CAP_TDS_BAND */
    MIPC_SYS_CAP_TDS_BAND_A                                 = 0x00000001,
    MIPC_SYS_CAP_TDS_BAND_B                                 = 0x00000002,
    MIPC_SYS_CAP_TDS_BAND_C                                 = 0x00000004,
    MIPC_SYS_CAP_TDS_BAND_D                                 = 0x00000008,
    MIPC_SYS_CAP_TDS_BAND_E                                 = 0x00000010,
    MIPC_SYS_CAP_TDS_BAND_F                                 = 0x00000020,
};
typedef enum mipc_sys_cap_tds_band_const_enum mipc_sys_cap_tds_band_const_enum;

enum mipc_sys_product_type_const_enum {
    mipc_sys_product_type_const_NONE = 0,
    /* SYS_PRODUCT_TYPE */
    MIPC_SYS_PRODUCT_TYPE_SMARTPHONE                        = 0x00,
    MIPC_SYS_PRODUCT_TYPE_DATACARD                          = 0x01,
};
typedef enum mipc_sys_product_type_const_enum mipc_sys_product_type_const_enum;

enum mipc_sys_fcc_lock_mode_const_enum {
    mipc_sys_fcc_lock_mode_const_NONE = 0,
};
typedef enum mipc_sys_fcc_lock_mode_const_enum mipc_sys_fcc_lock_mode_const_enum;

enum mipc_sys_fcc_lock_state_const_enum {
    mipc_sys_fcc_lock_state_const_NONE = 0,
};
typedef enum mipc_sys_fcc_lock_state_const_enum mipc_sys_fcc_lock_state_const_enum;

enum mipc_sys_config_class_const_enum {
    mipc_sys_config_class_const_NONE = 0,
    /* SYS_CONFIG_CLASS */
    /*  */
    MIPC_SYS_CONFIG_CLASS_GENERAL_MD_CONFIG                 = 0,
    /* for AT+EGMC support */
    MIPC_SYS_CONFIG_CLASS_LEGACY_MD_CONFIG                  = 1,
    /*  */
    MIPC_SYS_CONFIG_CLASS_HOST_RELAY_DIPC_CONFIG            = 2,
    /* for AT+EWOCFGSET support */
    MIPC_SYS_CONFIG_CLASS_N3X_CONFIG                        = 3,
    /* for AT+EIWLCFGSET support */
    MIPC_SYS_CONFIG_CLASS_IWLAN_CONFIG                      = 4,
};
typedef enum mipc_sys_config_class_const_enum mipc_sys_config_class_const_enum;

enum mipc_ims_config_class_const_enum {
    mipc_ims_config_class_const_NONE = 0,
    /* IMS_CONFIG_CLASS */
    /* for AT+EIMSCFG support */
    MIPC_IMS_CONFIG_CLASS_IMS_FEATURE                       = 0,
    /* for AT+ECFGSET support */
    MIPC_IMS_CONFIG_CLASS_IMS_CONFIG                        = 1,
    /* for AT+EPVSSET support */
    MIPC_IMS_CONFIG_CLASS_IMS_PROVISION                     = 2,
    /*  */
    MIPC_IMS_CONFIG_CLASS_IMS_CAPABILITY                    = 3,
};
typedef enum mipc_ims_config_class_const_enum mipc_ims_config_class_const_enum;

enum mipc_ims_config_ind_reason_const_enum {
    mipc_ims_config_ind_reason_const_NONE = 0,
    /* IMS_CONFIG_IND_REASON */
    MIPC_IMS_CONFIG_IND_REASON_IMS_READY                    = 0,
    MIPC_IMS_CONFIG_IND_REASON_USER_CHANGE                  = 1,
};
typedef enum mipc_ims_config_ind_reason_const_enum mipc_ims_config_ind_reason_const_enum;

enum mipc_ims_ind_type_const_enum {
    mipc_ims_ind_type_const_NONE = 0,
    /* IMS_IND_TYPE */
    MIPC_IMS_IND_TYPE_NOTIFY                                = 0,
    MIPC_IMS_IND_TYPE_INIT                                  = 1,
    MIPC_IMS_IND_TYPE_REJECT                                = 2,
    MIPC_IMS_IND_TYPE_MAX                                   = 0x7F,
};
typedef enum mipc_ims_ind_type_const_enum mipc_ims_ind_type_const_enum;

enum mipc_ims_state_ind_event_const_enum {
    mipc_ims_state_ind_event_const_NONE = 0,
    /* IMS_STATE_IND_EVENT */
    MIPC_IMS_STATE_IND_EVENT_REG_STATE                      = 0,
    MIPC_IMS_STATE_IND_EVENT_WFC                            = 1,
    MIPC_IMS_STATE_IND_EVENT_REG_URI                        = 2,
    MIPC_IMS_STATE_IND_EVENT_REG_RESP                       = 3,
};
typedef enum mipc_ims_state_ind_event_const_enum mipc_ims_state_ind_event_const_enum;

enum mipc_ims_state_const_enum {
    mipc_ims_state_const_NONE = 0,
    /* IMS_STATE */
    MIPC_IMS_STATE_UNREGISTERED                             = 0,
    MIPC_IMS_STATE_REGISTERED                               = 1,
    MIPC_IMS_STATE_UNREGISTERING                            = 2,
    MIPC_IMS_STATE_REGISTERING                              = 3,
    MIPC_IMS_STATE_REREGISTERING                            = 4,
};
typedef enum mipc_ims_state_const_enum mipc_ims_state_const_enum;

enum mipc_ims_reg_type_const_enum {
    mipc_ims_reg_type_const_NONE = 0,
    /* IMS_REG_TYPE */
    MIPC_IMS_REG_TYPE_NORMAL_SERVICE                        = 0,
    MIPC_IMS_REG_TYPE_EMERGENCY_SERVICE                     = 1,
};
typedef enum mipc_ims_reg_type_const_enum mipc_ims_reg_type_const_enum;

enum mipc_ims_reg_err_code_const_enum {
    mipc_ims_reg_err_code_const_NONE = 0,
    /* IMS_REG_ERR_CODE */
    MIPC_IMS_REG_ERR_CODE_SIP_OK                            = 0,
    MIPC_IMS_REG_ERR_CODE_SIP_NO_RESOURCE                   = 1,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_UA_ID                 = 2,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_DIALOG_ID             = 3,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_UA_STATE              = 4,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_REQ_ID                = 5,
    MIPC_IMS_REG_ERR_CODE_SIP_UNSUPPORTED_METHOD            = 6,
    MIPC_IMS_REG_ERR_CODE_SIP_BEARER_ERROR                  = 8,
    MIPC_IMS_REG_ERR_CODE_SIP_SOCKET_ERROR                  = 9,
    MIPC_IMS_REG_ERR_CODE_SIP_BEARER_TIMEOUT_ERROR          = 10,
    MIPC_IMS_REG_ERR_CODE_SIP_WOULDBLOCK                    = 17,
    MIPC_IMS_REG_ERR_CODE_SIP_HEADER_NOT_FOUND              = 18,
    MIPC_IMS_REG_ERR_CODE_SIP_NO_MEMORY                     = 19,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_PARAM                 = 20,
    MIPC_IMS_REG_ERR_CODE_SIP_NO_SIP_MSG                    = 21,
    MIPC_IMS_REG_ERR_CODE_SIP_NO_UA_DISPATCHER              = 22,
    MIPC_IMS_REG_ERR_CODE_SIP_NO_URI                        = 23,
    MIPC_IMS_REG_ERR_CODE_SIP_OP_ALREADY                    = 24,
    MIPC_IMS_REG_ERR_CODE_SIP_OP_NOT_ALLOW                  = 25,
    MIPC_IMS_REG_ERR_CODE_SIP_FS_ERROR                      = 26,
    MIPC_IMS_REG_ERR_CODE_SIP_DNS_ERROR                     = 27,
    MIPC_IMS_REG_ERR_CODE_SIP_EARLY_NOTIFY                  = 28,
    MIPC_IMS_REG_ERR_CODE_SIP_DHCP_ERROR                    = 29,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_CONN_ID               = 30,
    MIPC_IMS_REG_ERR_CODE_SIP_INVALID_DNS_ID                = 31,
    MIPC_IMS_REG_ERR_CODE_SIP_SOCKET_BIND_ERROR             = 32,
    MIPC_IMS_REG_ERR_CODE_SIP_TOO_LONG_MESSAGE              = 33,
    MIPC_IMS_REG_ERR_CODE_SIP_UNSPECIFIED_ERROR             = 99,
    MIPC_IMS_REG_ERR_CODE_SIP_100_TRYING                    = 100,
    MIPC_IMS_REG_ERR_CODE_SIP_180_RINGING                   = 180,
    MIPC_IMS_REG_ERR_CODE_SIP_181_CALL_FORWARDED            = 181,
    MIPC_IMS_REG_ERR_CODE_SIP_182_QUEUED                    = 182,
    MIPC_IMS_REG_ERR_CODE_SIP_183_SESSION_PROGRESS          = 183,
    MIPC_IMS_REG_ERR_CODE_SIP_199_EARLY_DIALOG_TERMINATED   = 199,
    MIPC_IMS_REG_ERR_CODE_SIP_200_OK                        = 200,
    MIPC_IMS_REG_ERR_CODE_SIP_202_ACCEPTED                  = 202,
    MIPC_IMS_REG_ERR_CODE_SIP_300_MULTIPLE_CHOICES          = 300,
    MIPC_IMS_REG_ERR_CODE_SIP_301_MOVED_PERMANENTLY         = 301,
    MIPC_IMS_REG_ERR_CODE_SIP_302_MOVED_TEMPORARILY         = 302,
    MIPC_IMS_REG_ERR_CODE_SIP_305_USE_PROXY                 = 305,
    MIPC_IMS_REG_ERR_CODE_SIP_380_ALTERNATIVE_SERVICE       = 380,
    MIPC_IMS_REG_ERR_CODE_SIP_400_BAD_REQUEST               = 400,
    MIPC_IMS_REG_ERR_CODE_SIP_401_UNAUTHORIZED              = 401,
    MIPC_IMS_REG_ERR_CODE_SIP_402_PAYMENT_REQUIRED          = 402,
    MIPC_IMS_REG_ERR_CODE_SIP_403_FORBIDDEN                 = 403,
    MIPC_IMS_REG_ERR_CODE_SIP_404_NOT_FOUND                 = 404,
    MIPC_IMS_REG_ERR_CODE_SIP_405_METHOD_NOT_ALLOW          = 405,
    MIPC_IMS_REG_ERR_CODE_SIP_406_NOT_ACCEPTABLE            = 406,
    MIPC_IMS_REG_ERR_CODE_SIP_407_PROXY_AUTH_REQUIRED       = 407,
    MIPC_IMS_REG_ERR_CODE_SIP_408_REQUEST_TIMEOUT           = 408,
    MIPC_IMS_REG_ERR_CODE_SIP_409_CONFLICT                  = 409,
    MIPC_IMS_REG_ERR_CODE_SIP_410_GONE                      = 410,
    MIPC_IMS_REG_ERR_CODE_SIP_411_LENGTH_REQUIRED           = 411,
    MIPC_IMS_REG_ERR_CODE_SIP_412_CONDITIONAL_REQ_FAIL      = 412,
    MIPC_IMS_REG_ERR_CODE_SIP_413_REQUEST_ENTITY_TOO_LARGE  = 413,
    MIPC_IMS_REG_ERR_CODE_SIP_414_REQUEST_URI_TOO_LARGE     = 414,
    MIPC_IMS_REG_ERR_CODE_SIP_415_UNSUPPORTED_MEDIA_TYPE    = 415,
    MIPC_IMS_REG_ERR_CODE_SIP_416_UNSUPPORTED_SCHEME        = 416,
    MIPC_IMS_REG_ERR_CODE_SIP_420_BAD_EXTENSION             = 420,
    MIPC_IMS_REG_ERR_CODE_SIP_421_EXTENSION_REQUIRED        = 421,
    MIPC_IMS_REG_ERR_CODE_SIP_422_INTERVAL_TOO_BRIEF        = 422,
    MIPC_IMS_REG_ERR_CODE_SIP_423_INTERVAL_TOO_BRIEF        = 423,
    MIPC_IMS_REG_ERR_CODE_SIP_429_PROVIDE_REFERRER_ID       = 429,
    MIPC_IMS_REG_ERR_CODE_SIP_433_ANONYMITY_DISALLOWED      = 433,
    MIPC_IMS_REG_ERR_CODE_SIP_480_TEMP_UNAVAILABLE          = 480,
    MIPC_IMS_REG_ERR_CODE_SIP_481_CALL_TRANS_NOT_EXIST      = 481,
    MIPC_IMS_REG_ERR_CODE_SIP_482_LOOP_DETECTED             = 482,
    MIPC_IMS_REG_ERR_CODE_SIP_483_TOO_MANY_HOPS             = 483,
    MIPC_IMS_REG_ERR_CODE_SIP_484_ADDRESS_INCOMPLETE        = 484,
    MIPC_IMS_REG_ERR_CODE_SIP_485_AMBIGUOUS                 = 485,
    MIPC_IMS_REG_ERR_CODE_SIP_486_BUSY_HERE                 = 486,
    MIPC_IMS_REG_ERR_CODE_SIP_487_REQ_TERMINATED            = 487,
    MIPC_IMS_REG_ERR_CODE_SIP_488_NOT_ACCEPTABLE_HERE       = 488,
    MIPC_IMS_REG_ERR_CODE_SIP_489_BAD_EVENT                 = 489,
    MIPC_IMS_REG_ERR_CODE_SIP_491_REQUEST_PENDING           = 491,
    MIPC_IMS_REG_ERR_CODE_SIP_493_UNDECIPHERABLE            = 493,
    MIPC_IMS_REG_ERR_CODE_SIP_494_SECURITY_AGREEMENT_REQUIRED = 494,
    MIPC_IMS_REG_ERR_CODE_SIP_499_CLIENT_CLOSED_REQUEST     = 499,
    MIPC_IMS_REG_ERR_CODE_SIP_500_SERVER_INT_ERROR          = 500,
    MIPC_IMS_REG_ERR_CODE_SIP_501_NOT_IMPLEMENTED           = 501,
    MIPC_IMS_REG_ERR_CODE_SIP_502_BAD_GATEWAY               = 502,
    MIPC_IMS_REG_ERR_CODE_SIP_503_SERVICE_UNAVAILABLE       = 503,
    MIPC_IMS_REG_ERR_CODE_SIP_504_SERVER_TIMEOUT            = 504,
    MIPC_IMS_REG_ERR_CODE_SIP_505_SIP_VERSION_NOT_SUPPORTED = 505,
    MIPC_IMS_REG_ERR_CODE_SIP_513_MESSAGE_TOO_LARGE         = 513,
    MIPC_IMS_REG_ERR_CODE_SIP_580_PRECONDITION_FAILURE      = 580,
    MIPC_IMS_REG_ERR_CODE_SIP_600_BUSY_EVERYWHERE           = 600,
    MIPC_IMS_REG_ERR_CODE_SIP_603_DECLINE                   = 603,
    MIPC_IMS_REG_ERR_CODE_SIP_604_DOES_NOT_EXIST_ANYWHERE   = 604,
    MIPC_IMS_REG_ERR_CODE_SIP_605_DECLINE_EVERYWHERE        = 605,
    MIPC_IMS_REG_ERR_CODE_SIP_606_NOT_ACCEPTABLE            = 606,
    MIPC_IMS_REG_ERR_CODE_SIP_607_UNWANTED                  = 607,
    MIPC_IMS_REG_ERR_CODE_SIP_608_REJECTED                  = 608,
};
typedef enum mipc_ims_reg_err_code_const_enum mipc_ims_reg_err_code_const_enum;

enum mipc_ims_pdn_err_cause_const_enum {
    mipc_ims_pdn_err_cause_const_NONE = 0,
    /* IMS_PDN_ERR_CAUSE */
    MIPC_IMS_PDN_ERR_CAUSE_ERR_CAUSE_NONE                   = 1,
    MIPC_IMS_PDN_ERR_CAUSE_OPERATOR_DETERMINED_BARRING      = 8,
    MIPC_IMS_PDN_ERR_CAUSE_INSUFFICIENT_RESOURCES           = 26,
    MIPC_IMS_PDN_ERR_CAUSE_UNKNOWN_OR_MISSING_APN           = 27,
    MIPC_IMS_PDN_ERR_CAUSE_UNKNOWN_PDN_TYPE                 = 28,
    MIPC_IMS_PDN_ERR_CAUSE_USER_AUTHENTICATION_FAILED       = 29,
    MIPC_IMS_PDN_ERR_CAUSE_REQUEST_REJECTED_BY_SERVING_GW_OR_PDN_GW = 30,
    MIPC_IMS_PDN_ERR_CAUSE_REQUEST_REJECTED_UNSPECIFIED     = 31,
    MIPC_IMS_PDN_ERR_CAUSE_SERVICE_OPTION_NOT_SUPPORTED     = 32,
    MIPC_IMS_PDN_ERR_CAUSE_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED = 33,
    MIPC_IMS_PDN_ERR_CAUSE_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER = 34,
    MIPC_IMS_PDN_ERR_CAUSE_PTI_ALREADY_IN_USE               = 35,
    MIPC_IMS_PDN_ERR_CAUSE_REGULAR_DEACTIVATION             = 36,
    MIPC_IMS_PDN_ERR_CAUSE_EPS_QOS_NOT_ACCEPTED             = 37,
    MIPC_IMS_PDN_ERR_CAUSE_NETWORK_FAILURE                  = 38,
    MIPC_IMS_PDN_ERR_CAUSE_REACTIVATION_REQUESTED           = 39,
    MIPC_IMS_PDN_ERR_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION = 41,
    MIPC_IMS_PDN_ERR_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION = 42,
    MIPC_IMS_PDN_ERR_CAUSE_INVALID_EPS_BEARER_OR_PDU_SESSION_IDENTITY = 43,
    MIPC_IMS_PDN_ERR_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER = 44,
    MIPC_IMS_PDN_ERR_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTER = 45,
    MIPC_IMS_PDN_ERR_CAUSE_OUT_OF_LAND_SERVICE_AREA         = 46,
    MIPC_IMS_PDN_ERR_CAUSE_PTI_MISMATCH                     = 47,
    MIPC_IMS_PDN_ERR_CAUSE_LAST_PDN_DISCONNECTION_NOT_ALLOWED = 49,
    MIPC_IMS_PDN_ERR_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED       = 50,
    MIPC_IMS_PDN_ERR_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED       = 51,
    MIPC_IMS_PDN_ERR_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED = 52,
    MIPC_IMS_PDN_ERR_CAUSE_ESM_INFORMATION_NOT_RECEIVED     = 53,
    MIPC_IMS_PDN_ERR_CAUSE_PDN_CONNECTION_OR_PDU_SESSION_DOES_NOT_EXIST = 54,
    MIPC_IMS_PDN_ERR_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED = 55,
    MIPC_IMS_PDN_ERR_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST = 56,
    MIPC_IMS_PDN_ERR_CAUSE_PDN_TYPE_IPV4V6_ONLY_ALLOWED     = 57,
    MIPC_IMS_PDN_ERR_CAUSE_PDN_TYPE_NON_IP_ONLY_ALLOWED     = 58,
    MIPC_IMS_PDN_ERR_CAUSE_UNSUPPORTED_QCI_VALUE            = 59,
    MIPC_IMS_PDN_ERR_CAUSE_BEARER_HANDLING_NOT_SUPPORTED    = 60,
    MIPC_IMS_PDN_ERR_CAUSE_MAXIMUM_NUM_OF_EPS_BEARERS_REACHED = 65,
    MIPC_IMS_PDN_ERR_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION = 66,
    MIPC_IMS_PDN_ERR_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN = 67,
    MIPC_IMS_PDN_ERR_CAUSE_NOT_SUPPORTED_SSC_MODE           = 68,
    MIPC_IMS_PDN_ERR_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE = 69,
    MIPC_IMS_PDN_ERR_CAUSE_MISSING_OR_UNKNOWN_DNN_IN_A_SLICE = 70,
    MIPC_IMS_PDN_ERR_CAUSE_INVALID_PTI_VALUE                = 81,
    MIPC_IMS_PDN_ERR_CAUSE_MAX_DATA_RATE_PER_UE_FOR_USER_PLANE_INTGRITY_PROTECTION_IS_TO_LOW = 82,
    MIPC_IMS_PDN_ERR_CAUSE_SEMANTIC_ERROR_IN_THE_QOS_OPERATION = 83,
    MIPC_IMS_PDN_ERR_CAUSE_SYNTACTICAL_ERROR_IN_THE_QOS_OPERATION = 84,
    MIPC_IMS_PDN_ERR_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE   = 95,
    MIPC_IMS_PDN_ERR_CAUSE_INVALID_MANDATORY_INFORMATION    = 96,
    MIPC_IMS_PDN_ERR_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED = 97,
    MIPC_IMS_PDN_ERR_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 98,
    MIPC_IMS_PDN_ERR_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED = 99,
    MIPC_IMS_PDN_ERR_CAUSE_CONDITIONAL_IE_ERROR             = 100,
    MIPC_IMS_PDN_ERR_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 101,
    MIPC_IMS_PDN_ERR_CAUSE_PROTOCO_ERROR_UNSPECIFIED        = 111,
    MIPC_IMS_PDN_ERR_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_EPS_BEARER_CONTEXT = 112,
    MIPC_IMS_PDN_ERR_CAUSE_MULTIPLE_ACCESSES_TO_PDN_CONNECTION_NOT_ALLOWED = 113,
};
typedef enum mipc_ims_pdn_err_cause_const_enum mipc_ims_pdn_err_cause_const_enum;

enum mipc_ims_ecc_ind_rat_const_enum {
    mipc_ims_ecc_ind_rat_const_NONE = 0,
    /* IMS_ECC_IND_RAT */
    MIPC_IMS_ECC_IND_RAT_GSM                                = 0,
    MIPC_IMS_ECC_IND_RAT_WCDMA                              = 1,
    MIPC_IMS_ECC_IND_RAT_TD_SCDMA                           = 2,
    MIPC_IMS_ECC_IND_RAT_FDD_LTE                            = 3,
    MIPC_IMS_ECC_IND_RAT_TDD_LTE                            = 4,
};
typedef enum mipc_ims_ecc_ind_rat_const_enum mipc_ims_ecc_ind_rat_const_enum;

enum mipc_ims_report_status_const_enum {
    mipc_ims_report_status_const_NONE = 0,
    /* IMS_REPORT_STATUS */
    MIPC_IMS_REPORT_STATUS_DISABLE_STATUS                   = 0,
    MIPC_IMS_REPORT_STATUS_ENABLE_STATUS                    = 1,
    MIPC_IMS_REPORT_STATUS_MAX                              = 2,
};
typedef enum mipc_ims_report_status_const_enum mipc_ims_report_status_const_enum;

enum mipc_ims_addition_service_const_enum {
    mipc_ims_addition_service_const_NONE = 0,
    /* IMS_ADDITION_SERVICE */
    MIPC_IMS_ADDITION_SERVICE_CALL_COMPOSER                 = 0,
    MIPC_IMS_ADDITION_SERVICE_INVALID_SERVICE               = 16,
};
typedef enum mipc_ims_addition_service_const_enum mipc_ims_addition_service_const_enum;

enum mipc_nw_ims_vops_status_const_enum {
    mipc_nw_ims_vops_status_const_NONE = 0,
    /* NW_IMS_VOPS_STATUS */
    MIPC_NW_IMS_VOPS_STATUS_NOT_REV_FROM_NW                 = 0,
    MIPC_NW_IMS_VOPS_STATUS_REV_FROM_NW                     = 1,
    MIPC_NW_IMS_VOPS_STATUS_MAX                             = 2,
};
typedef enum mipc_nw_ims_vops_status_const_enum mipc_nw_ims_vops_status_const_enum;

enum mipc_sys_config_change_reason_const_enum {
    mipc_sys_config_change_reason_const_NONE = 0,
    /* SYS_CONFIG_CHANGE_REASON */
    MIPC_SYS_CONFIG_CHANGE_REASON_SIM_READY                 = 0,
    MIPC_SYS_CONFIG_CHANGE_REASON_USER_CHANGE               = 1,
};
typedef enum mipc_sys_config_change_reason_const_enum mipc_sys_config_change_reason_const_enum;

enum mipc_sys_adpclk_state_const_enum {
    mipc_sys_adpclk_state_const_NONE = 0,
    /* SYS_ADPCLK_STATE */
    /*  */
    MIPC_SYS_ADPCLK_STATE_DISABLE                           = 0,
    /*  */
    MIPC_SYS_ADPCLK_STATE_ENABLE                            = 1,
};
typedef enum mipc_sys_adpclk_state_const_enum mipc_sys_adpclk_state_const_enum;

enum mipc_sys_mcf_op_const_enum {
    mipc_sys_mcf_op_const_NONE = 0,
    /* SYS_MCF_OP */
    MIPC_SYS_MCF_OP_SET_OTA_AND_OPOTA_FILE_PATH             = 0,
    MIPC_SYS_MCF_OP_SET_OTA_FILE_PATH                       = 1,
    MIPC_SYS_MCF_OP_SET_OPOTA_FILE_PATH                     = 2,
    MIPC_SYS_MCF_OP_SET_FILE_PATH_IN_MCF_DSBP_ACTIVE_MODE   = 3,
    MIPC_SYS_MCF_OP_GET_APPLIED_FILE_PATH                   = 4,
    MIPC_SYS_MCF_OP_DUMP_LID_DATA                           = 5,
    MIPC_SYS_MCF_OP_SET_FILE_PATH_AND_AUTO_SELECT_BIN       = 6,
    MIPC_SYS_MCF_OP_UPDATE_OPOTA_FILE                       = 7,
    MIPC_SYS_MCF_OP_INI_OPERATION                           = 8,
    MIPC_SYS_MCF_OP_LID_VARIABLE_OPERATION                  = 9,
    MIPC_SYS_MCF_OP_QUERY_VARIABLE_VALUE                    = 10,
    MIPC_SYS_MCF_OP_ASSIGN_COMBINED_PATH                    = 11,
};
typedef enum mipc_sys_mcf_op_const_enum mipc_sys_mcf_op_const_enum;

enum mipc_sys_mcf_config_type_const_enum {
    mipc_sys_mcf_config_type_const_NONE = 0,
    /* SYS_MCF_CONFIG_TYPE */
    MIPC_SYS_MCF_CONFIG_TYPE_DEFAULT_BIN                    = 0,
    MIPC_SYS_MCF_CONFIG_TYPE_CARRIER_BIN                    = 1,
    MIPC_SYS_MCF_CONFIG_TYPE_GENERAL_CARRIER_BIN            = 2,
    MIPC_SYS_MCF_CONFIG_TYPE_RAW_BIN                        = 3,
    MIPC_SYS_MCF_CONFIG_TYPE_NETWORK_BIN                    = 4,
    MIPC_SYS_MCF_CONFIG_TYPE_GENERAL_NETWORK_BIN            = 5,
    MIPC_SYS_MCF_CONFIG_TYPE_MAX                            = 0x7F,
};
typedef enum mipc_sys_mcf_config_type_const_enum mipc_sys_mcf_config_type_const_enum;

enum mipc_sys_mcf_path_type_const_enum {
    mipc_sys_mcf_path_type_const_NONE = 0,
    /* SYS_MCF_PATH_TYPE */
    MIPC_SYS_MCF_PATH_TYPE_OTA                              = 0,
    MIPC_SYS_MCF_PATH_TYPE_RUNTIME                          = 1,
    MIPC_SYS_MCF_PATH_TYPE_MANUAL                           = 2,
    MIPC_SYS_MCF_PATH_TYPE_MAX                              = 0x7F,
};
typedef enum mipc_sys_mcf_path_type_const_enum mipc_sys_mcf_path_type_const_enum;

enum mipc_sys_sar_mode_const_enum {
    mipc_sys_sar_mode_const_NONE = 0,
    /* SYS_SAR_MODE */
    MIPC_SYS_SAR_MODE_BY_DEVICE                             = 0x00,
    MIPC_SYS_SAR_MODE_BY_HOST                               = 0x01,
};
typedef enum mipc_sys_sar_mode_const_enum mipc_sys_sar_mode_const_enum;

enum mipc_sys_account_id_const_enum {
    mipc_sys_account_id_const_NONE = 0,
    /* SYS_ACCOUNT_ID */
    MIPC_SYS_ACCOUNT_ID_SIM1_NORMAL                         = 0,
    MIPC_SYS_ACCOUNT_ID_SIM1_EMERGENCY                      = 1,
    MIPC_SYS_ACCOUNT_ID_SIM2_NORMAL                         = 2,
    MIPC_SYS_ACCOUNT_ID_SIM2_EMERGENCY                      = 3,
    MIPC_SYS_ACCOUNT_ID_FROM_IWLAN                          = 100,
};
typedef enum mipc_sys_account_id_const_enum mipc_sys_account_id_const_enum;

enum mipc_sys_wakeup_reason_const_enum {
    mipc_sys_wakeup_reason_const_NONE = 0,
    /* SYS_WAKEUP_REASON */
    /* Initialize value. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_INVALID            = 0,
    /* AP sleep timer expired. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_SCHEDULED          = 1,
    /* Wake up when it received SMS from network. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_SMS                = 2,
    /* Wake up when ecall incoming call is received. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_MT_INBAND_CALL     = 3,
    /* Wake up when losing network for 15 minutes. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_NO_SERVICE         = 4,
    /* Wake up AP when whiltelist IP pakcet is received. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_IP_PACKET          = 5,
    /* Wake up AP when previous data call status is changed. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_PKT_SRVC_STATUS    = 12,
    /* Wake up AP when previous data call IP status is changed. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_EXTENDED_IP_CONFIG = 13,
    /* Wake up when it received permanent reject from network. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_NAS                = 14,
    /* Wake up indication when it received an ecall from network. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_VOICE              = 15,
    /* SIM removal, SIM refresh event notification to AP. */
    MIPC_SYS_WAKEUP_REASON_WAKEUP_REASON_IND_UIM            = 16,
};
typedef enum mipc_sys_wakeup_reason_const_enum mipc_sys_wakeup_reason_const_enum;

enum mipc_md_init_id_const_enum {
    mipc_md_init_id_const_NONE = 0,
    /* MD_INIT_ID */
    MIPC_MD_INIT_ID_SMS_READY                               = 1,
    MIPC_MD_INIT_ID_PHB_READY                               = 2,
    MIPC_MD_INIT_ID_CHANGE_EONS_FILE                        = 8,
    MIPC_MD_INIT_ID_INVALID_SIM                             = 16,
    MIPC_MD_INIT_ID_PHB_NOT_READY                           = 32,
    MIPC_MD_INIT_ID_TCM_READY                               = 64,
};
typedef enum mipc_md_init_id_const_enum mipc_md_init_id_const_enum;

enum mipc_sys_dsbp_mode_const_enum {
    mipc_sys_dsbp_mode_const_NONE = 0,
    /* SYS_DSBP_MODE */
    MIPC_SYS_DSBP_MODE_DISABLE                              = 0,
    MIPC_SYS_DSBP_MODE_ENABLE_IMS                           = 1,
    MIPC_SYS_DSBP_MODE_ENABLE_ALL                           = 2,
    MIPC_SYS_DSBP_MODE_DYNAMIC_LEVEL                        = 3,
    MIPC_SYS_DSBP_MODE_MAX                                  = 0x7F,
};
typedef enum mipc_sys_dsbp_mode_const_enum mipc_sys_dsbp_mode_const_enum;

enum mipc_sys_sbp_mode_const_enum {
    mipc_sys_sbp_mode_const_NONE = 0,
    /* SYS_SBP_MODE */
    MIPC_SYS_SBP_MODE_SET_SBP_ID                            = 0,
    MIPC_SYS_SBP_MODE_SET_CONFIG_WITH_FEATURE_INT           = 1,
    MIPC_SYS_SBP_MODE_SET_DATA_WITH_FEATURE_INT             = 2,
    MIPC_SYS_SBP_MODE_GET_CONFIG_WITH_FEATURE_INT           = 3,
    MIPC_SYS_SBP_MODE_GET_DATA_WITH_FEATURE_INT             = 4,
    MIPC_SYS_SBP_MODE_SET_CONFIG_WITH_FEATURE_STR           = 5,
    MIPC_SYS_SBP_MODE_SET_DATA_WITH_FEATURE_STR             = 6,
    MIPC_SYS_SBP_MODE_GET_CONFIG_WITH_FEATURE_STR           = 7,
    MIPC_SYS_SBP_MODE_GET_DATA_WITH_FEATURE_STR             = 8,
};
typedef enum mipc_sys_sbp_mode_const_enum mipc_sys_sbp_mode_const_enum;

enum mipc_sys_sbp_set_mode_const_enum {
    mipc_sys_sbp_set_mode_const_NONE = 0,
    /* SYS_SBP_SET_MODE */
    MIPC_SYS_SBP_SET_MODE_SET_SBP_ID                        = 0,
    MIPC_SYS_SBP_SET_MODE_SET_CONFIG_WITH_FEATURE_INT       = 1,
    MIPC_SYS_SBP_SET_MODE_SET_DATA_WITH_FEATURE_INT         = 2,
    MIPC_SYS_SBP_SET_MODE_SET_CONFIG_WITH_FEATURE_STR       = 5,
    MIPC_SYS_SBP_SET_MODE_SET_DATA_WITH_FEATURE_STR         = 6,
};
typedef enum mipc_sys_sbp_set_mode_const_enum mipc_sys_sbp_set_mode_const_enum;

enum mipc_sys_sbp_get_mode_const_enum {
    mipc_sys_sbp_get_mode_const_NONE = 0,
    /* SYS_SBP_GET_MODE */
    MIPC_SYS_SBP_GET_MODE_GET_CONFIG_WITH_FEATURE_INT       = 3,
    MIPC_SYS_SBP_GET_MODE_GET_DATA_WITH_FEATURE_INT         = 4,
    MIPC_SYS_SBP_GET_MODE_GET_CONFIG_WITH_FEATURE_STR       = 7,
    MIPC_SYS_SBP_GET_MODE_GET_DATA_WITH_FEATURE_STR         = 8,
};
typedef enum mipc_sys_sbp_get_mode_const_enum mipc_sys_sbp_get_mode_const_enum;

enum mipc_sys_log_flush_trigger_result_const_enum {
    mipc_sys_log_flush_trigger_result_const_NONE = 0,
    /* SYS_LOG_FLUSH_TRIGGER_RESULT */
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_SUCCESS               = 0,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_PENDING               = 1,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_NO_DATA               = 2,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_HAS_MORE_DATA_TO_HANDLE = 3,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_ERROR                 = 4,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_SERVICE_NOT_SUPPORT   = 5,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_SERVICE_NOT_READY     = 6,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_SERVICE_OPENED        = 7,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_SERVICE_EXIST         = 8,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_RESOURCE_NOT_ENOUGH   = 9,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_INTERNAL_ERROR        = 10,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_INPUT_DATA_INVAID     = 11,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_PROCEDURE_IS_ONGOING  = 12,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_DATA_FLOW_OR_CHANNEL_UNAVAILABLE = 13,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_LOG_MODE_INVALID      = 14,
    MIPC_SYS_LOG_FLUSH_TRIGGER_RESULT_TIMEOUT               = 15,
};
typedef enum mipc_sys_log_flush_trigger_result_const_enum mipc_sys_log_flush_trigger_result_const_enum;

enum mipc_sys_md_log_result_const_enum {
    mipc_sys_md_log_result_const_NONE = 0,
    /* SYS_MD_LOG_RESULT */
    MIPC_SYS_MD_LOG_RESULT_SUCCESS                          = 0,
    MIPC_SYS_MD_LOG_RESULT_PENDING                          = 1,
    MIPC_SYS_MD_LOG_RESULT_NO_DATA                          = 2,
    MIPC_SYS_MD_LOG_RESULT_HAS_MORE_DATA_TO_HANDLE          = 3,
    MIPC_SYS_MD_LOG_RESULT_ERROR                            = 4,
    MIPC_SYS_MD_LOG_RESULT_SERVICE_NOT_IMPLEMENT            = 5,
    MIPC_SYS_MD_LOG_RESULT_SERVICE_NOT_READY                = 6,
    MIPC_SYS_MD_LOG_RESULT_SERVICE_OPENED                   = 7,
    MIPC_SYS_MD_LOG_RESULT_SERVICE_EXIST                    = 8,
    MIPC_SYS_MD_LOG_RESULT_RESOURCE_NOT_ENOUGH              = 9,
    MIPC_SYS_MD_LOG_RESULT_INTERNAL_ERROR                   = 10,
    MIPC_SYS_MD_LOG_RESULT_INPUT_DATA_INVAID                = 11,
    MIPC_SYS_MD_LOG_RESULT_PROCEDURE_IS_ONGOING             = 12,
    MIPC_SYS_MD_LOG_RESULT_DATA_FLOW_OR_CHANNEL_UNAVAILABLE = 13,
    MIPC_SYS_MD_LOG_RESULT_LOG_MODE_INVALID                 = 14,
    MIPC_SYS_MD_LOG_RESULT_TIMEOUT                          = 15,
    MIPC_SYS_MD_LOG_RESULT_SERVICE_NOT_SUPPORT              = 16,
};
typedef enum mipc_sys_md_log_result_const_enum mipc_sys_md_log_result_const_enum;

enum mipc_sys_hba_ctrl_mode_const_enum {
    mipc_sys_hba_ctrl_mode_const_NONE = 0,
    /* SYS_HBA_CTRL_MODE */
    MIPC_SYS_HBA_CTRL_MODE_PAUSE                            = 0,
    MIPC_SYS_HBA_CTRL_MODE_STOP                             = 1,
    MIPC_SYS_HBA_CTRL_MODE_INVALID                          = 0x7F,
};
typedef enum mipc_sys_hba_ctrl_mode_const_enum mipc_sys_hba_ctrl_mode_const_enum;

enum mipc_sys_set_recording_tx_cond_iq_dump_req_capture_mode_const_enum {
    mipc_sys_set_recording_tx_cond_iq_dump_req_capture_mode_const_NONE = 0,
    /* SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE */
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE_RA = 0,
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE_SR = 1,
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE_PUSCH = 2,
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE_ACK = 3,
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE_SRS = 4,
};
typedef enum mipc_sys_set_recording_tx_cond_iq_dump_req_capture_mode_const_enum mipc_sys_set_recording_tx_cond_iq_dump_req_capture_mode_const_enum;

enum mipc_sys_spv_control_type_const_enum {
    mipc_sys_spv_control_type_const_NONE = 0,
    /* SYS_SPV_CONTROL_TYPE */
    /* System suggestion suite framework control. */
    MIPC_SYS_SPV_CONTROL_TYPE_SSS                           = 0,
    /* SS profiling framework control. */
    MIPC_SYS_SPV_CONTROL_TYPE_SSPROF                        = 1,
    /* EMI latency profiling HW control. */
    MIPC_SYS_SPV_CONTROL_TYPE_ELM                           = 2,
    /* Bus monitoring/profiling HW control. */
    MIPC_SYS_SPV_CONTROL_TYPE_BUSMON                        = 3,
    /* MCU core profiling HW control. */
    MIPC_SYS_SPV_CONTROL_TYPE_PDAMON                        = 4,
    /* SWLA control. */
    MIPC_SYS_SPV_CONTROL_TYPE_SWLA                          = 5,
};
typedef enum mipc_sys_spv_control_type_const_enum mipc_sys_spv_control_type_const_enum;

enum mipc_sys_pmic_type_const_enum {
    mipc_sys_pmic_type_const_NONE = 0,
    /* SYS_PMIC_TYPE */
    MIPC_SYS_PMIC_TYPE_DCL_BUCK_CPU                         = 0,
    MIPC_SYS_PMIC_TYPE_DCL_BUCK_GPU                         = 1,
    MIPC_SYS_PMIC_TYPE_DCL_BUCK_MD                          = 2,
    MIPC_SYS_PMIC_TYPE_DCL_BUCK_RF                          = 3,
    MIPC_SYS_PMIC_TYPE_DCL_MAIN_PMIC                        = 4,
    MIPC_SYS_PMIC_TYPE_DCL_BUCK_VPU                         = 5,
    MIPC_SYS_PMIC_TYPE_DCL_SUB_PMIC                         = 6,
    MIPC_SYS_PMIC_TYPE_DCL_CLOCK_PMIC                       = 7,
    MIPC_SYS_PMIC_TYPE_DCL_SECOND_PMIC                      = 8,
    MIPC_SYS_PMIC_TYPE_DCL_SECOND_PMIC_P                    = 9,
    MIPC_SYS_PMIC_TYPE_DCL_NR                               = 10,
};
typedef enum mipc_sys_pmic_type_const_enum mipc_sys_pmic_type_const_enum;

enum mipc_sys_idc_period_type_const_enum {
    mipc_sys_idc_period_type_const_NONE = 0,
    /* SYS_IDC_PERIOD_TYPE */
    MIPC_SYS_IDC_PERIOD_TYPE_DOWNLINK                       = 0x00,
    MIPC_SYS_IDC_PERIOD_TYPE_UPLINK                         = 0x01,
    MIPC_SYS_IDC_PERIOD_TYPE_BI_DIRECTIONAL                 = 0x02,
    MIPC_SYS_IDC_PERIOD_TYPE_GUARD_PERIOD                   = 0x03,
};
typedef enum mipc_sys_idc_period_type_const_enum mipc_sys_idc_period_type_const_enum;

enum mipc_sys_idc_channel_type_const_enum {
    mipc_sys_idc_channel_type_const_NONE = 0,
    /* SYS_IDC_CHANNEL_TYPE */
    MIPC_SYS_IDC_CHANNEL_TYPE_TDD                           = 0x00,
    MIPC_SYS_IDC_CHANNEL_TYPE_FDD                           = 0x01,
};
typedef enum mipc_sys_idc_channel_type_const_enum mipc_sys_idc_channel_type_const_enum;

enum mipc_sys_forward_at_req_forward_type_const_enum {
    mipc_sys_forward_at_req_forward_type_const_NONE = 0,
    /* SYS_FORWARD_AT_REQ_FORWARD_TYPE */
    MIPC_SYS_FORWARD_AT_REQ_FORWARD_TYPE_URC                = 0,
    MIPC_SYS_FORWARD_AT_REQ_FORWARD_TYPE_RSP                = 1,
};
typedef enum mipc_sys_forward_at_req_forward_type_const_enum mipc_sys_forward_at_req_forward_type_const_enum;

enum mipc_mspm_session_type_const_enum {
    mipc_mspm_session_type_const_NONE = 0,
    /* MSPM_SESSION_TYPE */
    MIPC_MSPM_SESSION_TYPE_SESSION_START                    = 0,
    MIPC_MSPM_SESSION_TYPE_SESSION_START_NTF                = 1,
    MIPC_MSPM_SESSION_TYPE_SESSION_END_NTF                  = 2,
};
typedef enum mipc_mspm_session_type_const_enum mipc_mspm_session_type_const_enum;

enum mipc_mspm_block_mode_const_enum {
    mipc_mspm_block_mode_const_NONE = 0,
    /* MSPM_BLOCK_MODE */
    MIPC_MSPM_BLOCK_MODE_MSPM_BLOCK_NONE                    = 0,
    MIPC_MSPM_BLOCK_MODE_MSPM_BLOCK                         = 1,
};
typedef enum mipc_mspm_block_mode_const_enum mipc_mspm_block_mode_const_enum;

enum mipc_apn_pdp_type_const_enum {
    mipc_apn_pdp_type_const_NONE = 0,
    /* APN_PDP_TYPE */
    MIPC_APN_PDP_TYPE_DEFAULT                               = 0,
    MIPC_APN_PDP_TYPE_IPV4                                  = 1,
    MIPC_APN_PDP_TYPE_IPV6                                  = 2,
    MIPC_APN_PDP_TYPE_IPV4V6                                = 3,
    MIPC_APN_PDP_TYPE_IPV4_AND_IPV6                         = 4,
    MIPC_APN_PDP_TYPE_ETHERNET                              = 5,
};
typedef enum mipc_apn_pdp_type_const_enum mipc_apn_pdp_type_const_enum;

enum mipc_deact_reason_enum_const_enum {
    mipc_deact_reason_enum_const_NONE = 0,
    /* DEACT_REASON_ENUM */
    MIPC_DEACT_REASON_ENUM_DONT_CARE                        = 0,
    MIPC_DEACT_REASON_ENUM_DEACT_NORMAL                     = 1,
    MIPC_DEACT_REASON_ENUM_DEACT_NO_PCSCF                   = 3,
    MIPC_DEACT_REASON_ENUM_DEACT_FORCE_TO_LOCAL_RELEASE     = 6,
    MIPC_DEACT_REASON_ENUM_DEACT_AOSP_SHUTDOWN              = 7,
    MIPC_DEACT_REASON_ENUM_DEACT_AOSP_HANDOVER              = 8,
    MIPC_DEACT_REASON_ENUM_DEACT_APN_CHANGED                = 9,
    MIPC_DEACT_REASON_ENUM_DEACT_IMS_HANDOVER               = 10,
    MIPC_DEACT_REASON_ENUM_DEACT_TEMP_DATA_SWITCH           = 11,
    MIPC_DEACT_REASON_ENUM_DEACT_IPCA                       = 20,
    MIPC_DEACT_REASON_ENUM_MAX                              = 0x7F,
};
typedef enum mipc_deact_reason_enum_const_enum mipc_deact_reason_enum_const_enum;

enum mipc_apn_enabled_type_const_enum {
    mipc_apn_enabled_type_const_NONE = 0,
    /* APN_ENABLED_TYPE */
    MIPC_APN_ENABLED_TYPE_DISABLED                          = 0,
    MIPC_APN_ENABLED_TYPE_ENABLED                           = 1,
    MIPC_APN_ENABLED_TYPE_MAX                               = 2,
};
typedef enum mipc_apn_enabled_type_const_enum mipc_apn_enabled_type_const_enum;

enum mipc_packet_filter_ip_type_const_enum {
    mipc_packet_filter_ip_type_const_NONE = 0,
    /* PACKET_FILTER_IP_TYPE */
    MIPC_PACKET_FILTER_IP_TYPE_IPV4                         = 0,
    MIPC_PACKET_FILTER_IP_TYPE_IPV6                         = 1,
    MIPC_PACKET_FILTER_IP_TYPE_IP_ALL                       = 2,
    MIPC_PACKET_FILTER_IP_TYPE_MAX                          = 0x7F,
};
typedef enum mipc_packet_filter_ip_type_const_enum mipc_packet_filter_ip_type_const_enum;

enum mipc_packet_filter_protocol_type_const_enum {
    mipc_packet_filter_protocol_type_const_NONE = 0,
    /* PACKET_FILTER_PROTOCOL_TYPE */
    MIPC_PACKET_FILTER_PROTOCOL_TYPE_TCP                    = 0,
    MIPC_PACKET_FILTER_PROTOCOL_TYPE_UDP                    = 1,
    MIPC_PACKET_FILTER_PROTOCOL_TYPE_ICMP                   = 2,
    MIPC_PACKET_FILTER_PROTOCOL_TYPE_PROTOCOL_ALL           = 3,
    MIPC_PACKET_FILTER_PROTOCOL_TYPE_MAX                    = 0x7F,
};
typedef enum mipc_packet_filter_protocol_type_const_enum mipc_packet_filter_protocol_type_const_enum;

enum mipc_ia_md_prefer_type_const_enum {
    mipc_ia_md_prefer_type_const_NONE = 0,
    /* IA_MD_PREFER_TYPE */
    MIPC_IA_MD_PREFER_TYPE_DISABLE_MD_PREFER                = 0,
    MIPC_IA_MD_PREFER_TYPE_ENABLE_MD_PREFER                 = 1,
    MIPC_IA_MD_PREFER_TYPE_MAX                              = 2,
};
typedef enum mipc_ia_md_prefer_type_const_enum mipc_ia_md_prefer_type_const_enum;

enum mipc_eif_address_type_const_enum {
    mipc_eif_address_type_const_NONE = 0,
    /* EIF_ADDRESS_TYPE */
    MIPC_EIF_ADDRESS_TYPE_DEFAULT                           = 0,
    MIPC_EIF_ADDRESS_TYPE_IPV4                              = 1,
    MIPC_EIF_ADDRESS_TYPE_IPV6                              = 2,
    MIPC_EIF_ADDRESS_TYPE_IPV4V6                            = 3,
};
typedef enum mipc_eif_address_type_const_enum mipc_eif_address_type_const_enum;

enum mipc_mbs_op_const_enum {
    mipc_mbs_op_const_NONE = 0,
    /* MBS_OP */
    MIPC_MBS_OP_INVALID                                     = 0,
    MIPC_MBS_OP_JOIN_MBS_SESSION                            = 1,
    MIPC_MBS_OP_LEAVE_MBS_SESSION                           = 2,
};
typedef enum mipc_mbs_op_const_enum mipc_mbs_op_const_enum;

enum mipc_mbs_decision_const_enum {
    mipc_mbs_decision_const_NONE = 0,
    /* MBS_DECISION */
    MIPC_MBS_DECISION_INVALID                               = 0,
    MIPC_MBS_DECISION_SERVICE_AREA_UPDATE                   = 1,
    MIPC_MBS_DECISION_JOIN_ACCEPTED                         = 2,
    MIPC_MBS_DECISION_JOIN_REJECTED                         = 3,
    MIPC_MBS_DECISION_REMOVE_UE_FROM_MBS_SESSION            = 4,
    MIPC_MBS_DECISION_VGSM_AREA_CHECK_FAIL                  = 0xA0,
    MIPC_MBS_DECISION_VGSM_TIMER_CHECK_FAIL                 = 0xA1,
};
typedef enum mipc_mbs_decision_const_enum mipc_mbs_decision_const_enum;

enum mipc_mbs_reject_cause_const_enum {
    mipc_mbs_reject_cause_const_NONE = 0,
    /* MBS_REJECT_CAUSE */
    MIPC_MBS_REJECT_CAUSE_NONE                              = 0,
    MIPC_MBS_REJECT_CAUSE_INSUFFICIENT_RESOURCES            = 1,
    MIPC_MBS_REJECT_CAUSE_USER_NOT_AUTHORIZED_TO_USE_SRV    = 2,
    MIPC_MBS_REJECT_CAUSE_SESSION_NOT_STARTED_OR_WILL_NOT_START_SOON = 3,
    MIPC_MBS_REJECT_CAUSE_USER_OUTSIDE_MBS_AREA             = 4,
    MIPC_MBS_REJECT_CAUSE_SESSION_CONTEXT_NOT_FOUND         = 5,
    MIPC_MBS_REJECT_CAUSE_SESSION_IS_RELEASED               = 6,
};
typedef enum mipc_mbs_reject_cause_const_enum mipc_mbs_reject_cause_const_enum;

enum mipc_mbs_session_id_type_const_enum {
    mipc_mbs_session_id_type_const_NONE = 0,
    /* MBS_SESSION_ID_TYPE */
    MIPC_MBS_SESSION_ID_TYPE_INVALID                        = 0,
    MIPC_MBS_SESSION_ID_TYPE_TMGI                           = 1,
    MIPC_MBS_SESSION_ID_TYPE_IPV4                           = 2,
    MIPC_MBS_SESSION_ID_TYPE_IPV6                           = 3,
};
typedef enum mipc_mbs_session_id_type_const_enum mipc_mbs_session_id_type_const_enum;

enum mipc_apn_auth_type_const_enum {
    mipc_apn_auth_type_const_NONE = 0,
    /* APN_AUTH_TYPE */
    MIPC_APN_AUTH_TYPE_NONE                                 = 0,
    MIPC_APN_AUTH_TYPE_PAP                                  = 1,
    MIPC_APN_AUTH_TYPE_CHAP                                 = 2,
    MIPC_APN_AUTH_TYPE_PAP_OR_CHAP                          = 3,
};
typedef enum mipc_apn_auth_type_const_enum mipc_apn_auth_type_const_enum;

enum mipc_apn_type_const_enum {
    mipc_apn_type_const_NONE = 0,
    /* APN_TYPE */
    MIPC_APN_TYPE_UNKNOWN                                   = 0x00000000,
    MIPC_APN_TYPE_DEFAULT                                   = 0x00000001,
    MIPC_APN_TYPE_IMS                                       = 0x00000002,
    MIPC_APN_TYPE_MMS                                       = 0x00000004,
    MIPC_APN_TYPE_SUPL                                      = 0x00000008,
    MIPC_APN_TYPE_DUN                                       = 0x00000010,
    MIPC_APN_TYPE_HIPRI                                     = 0x00000020,
    MIPC_APN_TYPE_FOTA                                      = 0x00000040,
    MIPC_APN_TYPE_CBS                                       = 0x00000080,
    MIPC_APN_TYPE_EMERGENCY                                 = 0x00000100,
    MIPC_APN_TYPE_IA                                        = 0x00000200,
    MIPC_APN_TYPE_DM                                        = 0x00000400,
    MIPC_APN_TYPE_WAP                                       = 0x00000800,
    MIPC_APN_TYPE_NET                                       = 0x00001000,
    MIPC_APN_TYPE_CMMAIL                                    = 0x00002000,
    MIPC_APN_TYPE_TETHERING                                 = 0x00004000,
    MIPC_APN_TYPE_RCSE                                      = 0x00008000,
    MIPC_APN_TYPE_XCAP                                      = 0x00010000,
    MIPC_APN_TYPE_RCS                                       = 0x00020000,
    MIPC_APN_TYPE_RCS_PCSCF                                 = 0x00040000,
    MIPC_APN_TYPE_VSIM                                      = 0x00080000,
    MIPC_APN_TYPE_BIP                                       = 0x00100000,
    MIPC_APN_TYPE_NW_ASSIGNED                               = 0x80000000,
};
typedef enum mipc_apn_type_const_enum mipc_apn_type_const_enum;

enum mipc_apn_compression_const_enum {
    mipc_apn_compression_const_NONE = 0,
    /* APN_COMPRESSION */
    MIPC_APN_COMPRESSION_NONE                               = 0,
    MIPC_APN_COMPRESSION_ENABLE                             = 1,
};
typedef enum mipc_apn_compression_const_enum mipc_apn_compression_const_enum;

enum mipc_apn_change_reason_const_enum {
    mipc_apn_change_reason_const_NONE = 0,
    /* APN_CHANGE_REASON */
    /* Default change reason of setting initial attach information such as scenario of SIM change. */
    MIPC_APN_CHANGE_REASON_DEFAULT                          = 0,
    /* The control point can configures re-attach IA information using this request as well for scenarios like IMS handover to Wi-Fi. TLV MIPC_APN_SET_IA_REQ_T_CHANGE_REASON=MIPC_APN_CHANGE_REASON_IMS_HANDOVER indicates this IA information is for re-attach scenario only. Modem does not store re-attach IA information and the control point has to configure every time. Across modem reboot, modem uses original IA configuration to perform initial attach instead of re-attach configuration. */
    MIPC_APN_CHANGE_REASON_IMS_HANDOVER                     = 1,
};
typedef enum mipc_apn_change_reason_const_enum mipc_apn_change_reason_const_enum;

enum mipc_apn_profile_mode_const_enum {
    mipc_apn_profile_mode_const_NONE = 0,
    /* APN_PROFILE_MODE */
    MIPC_APN_PROFILE_MODE_HOST_NOT_SET_APN_PROFILE          = 0,
    MIPC_APN_PROFILE_MODE_HOST_SET_APN_PROFILE              = 1,
};
typedef enum mipc_apn_profile_mode_const_enum mipc_apn_profile_mode_const_enum;

enum mipc_data_fallback_type_const_enum {
    mipc_data_fallback_type_const_NONE = 0,
    /* DATA_FALLBACK_TYPE */
    MIPC_DATA_FALLBACK_TYPE_DISABLE                         = 0,
    MIPC_DATA_FALLBACK_TYPE_IPV4_FIRST                      = 1,
    MIPC_DATA_FALLBACK_TYPE_IPV6_FIRST                      = 2,
    MIPC_DATA_FALLBACK_TYPE_MAX                             = 0x7F,
};
typedef enum mipc_data_fallback_type_const_enum mipc_data_fallback_type_const_enum;

enum mipc_mipc_cid_act_state_const_enum {
    mipc_mipc_cid_act_state_const_NONE = 0,
    /* MIPC_CID_ACT_STATE */
    MIPC_MIPC_CID_ACT_STATE_DEACTIVATED                     = 0,
    MIPC_MIPC_CID_ACT_STATE_ACTIVATED                       = 1,
};
typedef enum mipc_mipc_cid_act_state_const_enum mipc_mipc_cid_act_state_const_enum;

enum mipc_data_reuse_type_const_enum {
    mipc_data_reuse_type_const_NONE = 0,
    /* DATA_REUSE_TYPE */
    MIPC_DATA_REUSE_TYPE_DEFAULT                            = 0,
    MIPC_DATA_REUSE_TYPE_USER_SETTING_FIRST                 = 1,
    MIPC_DATA_REUSE_TYPE_NOT_REUSE                          = 2,
    MIPC_DATA_REUSE_TYPE_REUSE_ONLY                         = 3,
};
typedef enum mipc_data_reuse_type_const_enum mipc_data_reuse_type_const_enum;

enum mipc_data_config_type_const_enum {
    mipc_data_config_type_const_NONE = 0,
    /* DATA_CONFIG_TYPE */
    MIPC_DATA_CONFIG_TYPE_DISABLE                           = 0,
    MIPC_DATA_CONFIG_TYPE_ENABLE                            = 1,
    MIPC_DATA_CONFIG_TYPE_NO_CHANGE                         = 2,
};
typedef enum mipc_data_config_type_const_enum mipc_data_config_type_const_enum;

enum mipc_data_retry_mode_const_enum {
    mipc_data_retry_mode_const_NONE = 0,
    /* DATA_RETRY_MODE */
    MIPC_DATA_RETRY_MODE_QUERY_TIMER                        = 0,
    MIPC_DATA_RETRY_MODE_RESET_TIMER                        = 1,
    MIPC_DATA_RETRY_MODE_RETRY_AFTER_TIMER                  = 2,
};
typedef enum mipc_data_retry_mode_const_enum mipc_data_retry_mode_const_enum;

enum mipc_data_retry_type_const_enum {
    mipc_data_retry_type_const_NONE = 0,
    /* DATA_RETRY_TYPE */
    MIPC_DATA_RETRY_TYPE_RETRY_TYPE_NO_SUGGEST              = 0,
    MIPC_DATA_RETRY_TYPE_RETRY_TYPE_NO_RETRY                = 1,
    MIPC_DATA_RETRY_TYPE_RETRY_TYPE_WITH_SUGGEST            = 2,
};
typedef enum mipc_data_retry_type_const_enum mipc_data_retry_type_const_enum;

enum mipc_data_lce_mode_const_enum {
    mipc_data_lce_mode_const_NONE = 0,
    /* DATA_LCE_MODE */
    MIPC_DATA_LCE_MODE_DISABLE                              = 0,
    MIPC_DATA_LCE_MODE_ENABLE                               = 1,
    MIPC_DATA_LCE_MODE_SET                                  = 2,
};
typedef enum mipc_data_lce_mode_const_enum mipc_data_lce_mode_const_enum;

enum mipc_data_keepalive_status_code_const_enum {
    mipc_data_keepalive_status_code_const_NONE = 0,
    /* DATA_KEEPALIVE_STATUS_CODE */
    MIPC_DATA_KEEPALIVE_STATUS_CODE_ACTIVE                  = 0,
    MIPC_DATA_KEEPALIVE_STATUS_CODE_INACTIVE                = 1,
    MIPC_DATA_KEEPALIVE_STATUS_CODE_PENDING                 = 2,
};
typedef enum mipc_data_keepalive_status_code_const_enum mipc_data_keepalive_status_code_const_enum;

enum mipc_data_keepalive_type_const_enum {
    mipc_data_keepalive_type_const_NONE = 0,
    /* DATA_KEEPALIVE_TYPE */
    MIPC_DATA_KEEPALIVE_TYPE_NATT_IPV4                      = 0,
    MIPC_DATA_KEEPALIVE_TYPE_NATT_IPV6                      = 1,
    MIPC_DATA_KEEPALIVE_TYPE_IPV4_TCP                       = 2,
    MIPC_DATA_KEEPALIVE_TYPE_IPV4_UDP                       = 3,
    MIPC_DATA_KEEPALIVE_TYPE_IPV6_TCP                       = 4,
    MIPC_DATA_KEEPALIVE_TYPE_IPV6_UDP                       = 5,
};
typedef enum mipc_data_keepalive_type_const_enum mipc_data_keepalive_type_const_enum;

enum mipc_data_lgdcont_req_type_const_enum {
    mipc_data_lgdcont_req_type_const_NONE = 0,
    /* DATA_LGDCONT_REQ_TYPE */
    MIPC_DATA_LGDCONT_REQ_TYPE_NEW_PDP_OR_HANDOVER          = 0,
    MIPC_DATA_LGDCONT_REQ_TYPE_EMERGENCY_PDP                = 1,
    MIPC_DATA_LGDCONT_REQ_TYPE_NEW_PDP                      = 2,
    MIPC_DATA_LGDCONT_REQ_TYPE_HANDOVER                     = 3,
    MIPC_DATA_LGDCONT_REQ_TYPE_EMERGENCY_HANDOVER           = 4,
};
typedef enum mipc_data_lgdcont_req_type_const_enum mipc_data_lgdcont_req_type_const_enum;

enum mipc_data_lgdcont_rat_type_const_enum {
    mipc_data_lgdcont_rat_type_const_NONE = 0,
    /* DATA_LGDCONT_RAT_TYPE */
    MIPC_DATA_LGDCONT_RAT_TYPE_All_RAT                      = 0,
    MIPC_DATA_LGDCONT_RAT_TYPE_RAT_23G                      = 1,
    MIPC_DATA_LGDCONT_RAT_TYPE_RAT_4G                       = 2,
};
typedef enum mipc_data_lgdcont_rat_type_const_enum mipc_data_lgdcont_rat_type_const_enum;

enum mipc_data_bind_type_const_enum {
    mipc_data_bind_type_const_NONE = 0,
    /* DATA_BIND_TYPE */
    MIPC_DATA_BIND_TYPE_NOT_BIND_RNDIS                      = 0,
    MIPC_DATA_BIND_TYPE_BIND_RNDIS                          = 1,
};
typedef enum mipc_data_bind_type_const_enum mipc_data_bind_type_const_enum;

enum mipc_data_mod_event_type_const_enum {
    mipc_data_mod_event_type_const_NONE = 0,
    /* DATA_MOD_EVENT_TYPE */
    MIPC_DATA_MOD_EVENT_TYPE_INFORMATIONAL_EVENT            = 0,
    MIPC_DATA_MOD_EVENT_TYPE_ACKNOWLEDGEMENT_REQUIRED       = 1,
};
typedef enum mipc_data_mod_event_type_const_enum mipc_data_mod_event_type_const_enum;

enum mipc_data_paging_restrictions_const_enum {
    mipc_data_paging_restrictions_const_NONE = 0,
    /* DATA_PAGING_RESTRICTIONS */
    /*  */
    MIPC_DATA_PAGING_RESTRICTIONS_NOT_RESTRICTED            = 0,
    /*  */
    MIPC_DATA_PAGING_RESTRICTIONS_ALL_RESTRICTED            = 1,
    /*  */
    MIPC_DATA_PAGING_RESTRICTIONS_RESTRICTED_EXCEPT_VOICE   = 2,
    /*  */
    MIPC_DATA_PAGING_RESTRICTIONS_RESTRICTED_EXCEPT_SPECIFIED_PDX_CONN = 3,
    /*  */
    MIPC_DATA_PAGING_RESTRICTIONS_RESTRICTED_EXCEPT_VOICE_AND_SPECIFIED_PDX_CONN = 4,
};
typedef enum mipc_data_paging_restrictions_const_enum mipc_data_paging_restrictions_const_enum;

enum mipc_data_paging_restrict_result_const_enum {
    mipc_data_paging_restrict_result_const_NONE = 0,
    /* DATA_PAGING_RESTRICT_RESULT */
    /*  */
    MIPC_DATA_PAGING_RESTRICT_RESULT_REJECTED               = 0,
    /*  */
    MIPC_DATA_PAGING_RESTRICT_RESULT_ACCEPTED               = 1,
};
typedef enum mipc_data_paging_restrict_result_const_enum mipc_data_paging_restrict_result_const_enum;

enum mipc_data_ims_bearer_state_const_enum {
    mipc_data_ims_bearer_state_const_NONE = 0,
    /* DATA_IMS_BEARER_STATE */
    MIPC_DATA_IMS_BEARER_STATE_START                        = 0,
    MIPC_DATA_IMS_BEARER_STATE_STOP                         = 1,
};
typedef enum mipc_data_ims_bearer_state_const_enum mipc_data_ims_bearer_state_const_enum;

enum mipc_data_ims_bearer_type_const_enum {
    mipc_data_ims_bearer_type_const_NONE = 0,
    /* DATA_IMS_BEARER_TYPE */
    MIPC_DATA_IMS_BEARER_TYPE_SIP                           = 0,
    MIPC_DATA_IMS_BEARER_TYPE_VOICE                         = 2,
    MIPC_DATA_IMS_BEARER_TYPE_VIDEO                         = 3,
};
typedef enum mipc_data_ims_bearer_type_const_enum mipc_data_ims_bearer_type_const_enum;

enum mipc_data_ims_bearer_pdp_type_const_enum {
    mipc_data_ims_bearer_pdp_type_const_NONE = 0,
    /* DATA_IMS_BEARER_PDP_TYPE */
    MIPC_DATA_IMS_BEARER_PDP_TYPE_IPV4                      = 1,
    MIPC_DATA_IMS_BEARER_PDP_TYPE_IPV6                      = 2,
};
typedef enum mipc_data_ims_bearer_pdp_type_const_enum mipc_data_ims_bearer_pdp_type_const_enum;

enum mipc_mipc_eiwlpl_priority_type_const_enum {
    mipc_mipc_eiwlpl_priority_type_const_NONE = 0,
    /* MIPC_EIWLPL_PRIORITY_TYPE */
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_START                    = 0,
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_NULL                     = 1,
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_CELLULAR                 = 2,
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_WIFI                     = 3,
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_CELLULAR_GREATER_THAN_WIFI = 4,
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_WIFI_GREATER_THAN_CELLULAR = 5,
    MIPC_MIPC_EIWLPL_PRIORITY_TYPE_END                      = 6,
};
typedef enum mipc_mipc_eiwlpl_priority_type_const_enum mipc_mipc_eiwlpl_priority_type_const_enum;

enum mipc_internal_eif_req_cmd_const_enum {
    mipc_internal_eif_req_cmd_const_NONE = 0,
    /* INTERNAL_EIF_REQ_CMD */
    MIPC_INTERNAL_EIF_REQ_CMD_IFUP                          = 0,
    MIPC_INTERNAL_EIF_REQ_CMD_IFDOWN                        = 1,
    MIPC_INTERNAL_EIF_REQ_CMD_IPUPDATE                      = 2,
    MIPC_INTERNAL_EIF_REQ_CMD_IFQUERY                       = 3,
    MIPC_INTERNAL_EIF_REQ_CMD_NO_RA_INITIAL                 = 4,
    MIPC_INTERNAL_EIF_REQ_CMD_NO_RA_REFRESH                 = 5,
    MIPC_INTERNAL_EIF_REQ_CMD_IPADD                         = 6,
    MIPC_INTERNAL_EIF_REQ_CMD_IPDEL                         = 7,
    MIPC_INTERNAL_EIF_REQ_CMD_IPCHG                         = 8,
};
typedef enum mipc_internal_eif_req_cmd_const_enum mipc_internal_eif_req_cmd_const_enum;

enum mipc_internal_eif_ind_cmd_const_enum {
    mipc_internal_eif_ind_cmd_const_NONE = 0,
    /* INTERNAL_EIF_IND_CMD */
    MIPC_INTERNAL_EIF_IND_CMD_IFUP                          = 0,
    MIPC_INTERNAL_EIF_IND_CMD_IFDOWN                        = 1,
    MIPC_INTERNAL_EIF_IND_CMD_IPCHG                         = 4,
    MIPC_INTERNAL_EIF_IND_CMD_IFCHANGE                      = 5,
    MIPC_INTERNAL_EIF_IND_CMD_MTU                           = 6,
};
typedef enum mipc_internal_eif_ind_cmd_const_enum mipc_internal_eif_ind_cmd_const_enum;

enum mipc_internal_eipport_action_const_enum {
    mipc_internal_eipport_action_const_NONE = 0,
    /* INTERNAL_EIPPORT_ACTION */
    MIPC_INTERNAL_EIPPORT_ACTION_ALLOC                      = 0,
    MIPC_INTERNAL_EIPPORT_ACTION_FREE                       = 1,
};
typedef enum mipc_internal_eipport_action_const_enum mipc_internal_eipport_action_const_enum;

enum mipc_internal_eipport_result_const_enum {
    mipc_internal_eipport_result_const_NONE = 0,
    /* INTERNAL_EIPPORT_RESULT */
    MIPC_INTERNAL_EIPPORT_RESULT_FAILURE                    = 0,
    MIPC_INTERNAL_EIPPORT_RESULT_SUCCESS                    = 1,
};
typedef enum mipc_internal_eipport_result_const_enum mipc_internal_eipport_result_const_enum;

enum mipc_internal_eipspi_action_const_enum {
    mipc_internal_eipspi_action_const_NONE = 0,
    /* INTERNAL_EIPSPI_ACTION */
    MIPC_INTERNAL_EIPSPI_ACTION_ALLOC                       = 0,
    MIPC_INTERNAL_EIPSPI_ACTION_FREE                        = 1,
};
typedef enum mipc_internal_eipspi_action_const_enum mipc_internal_eipspi_action_const_enum;

enum mipc_internal_ho_progress_const_enum {
    mipc_internal_ho_progress_const_NONE = 0,
    /* INTERNAL_HO_PROGRESS */
    MIPC_INTERNAL_HO_PROGRESS_START                         = 0,
    MIPC_INTERNAL_HO_PROGRESS_STOP_SUCCESS                  = 1,
    MIPC_INTERNAL_HO_PROGRESS_STOP_FAILED                   = 0xFF,
    MIPC_INTERNAL_HO_PROGRESS_END                           = 1,
    MIPC_INTERNAL_HO_PROGRESS_IP_READY                      = 2,
    MIPC_INTERNAL_HO_PROGRESS_MAX                           = 0x7F,
};
typedef enum mipc_internal_ho_progress_const_enum mipc_internal_ho_progress_const_enum;

enum mipc_dsda_allowed_type_const_enum {
    mipc_dsda_allowed_type_const_NONE = 0,
    /* DSDA_ALLOWED_TYPE */
    MIPC_DSDA_ALLOWED_TYPE_NOT_ALLOWED                      = 0,
    MIPC_DSDA_ALLOWED_TYPE_ALLOWED                          = 1,
    MIPC_DSDA_ALLOWED_TYPE_MAX                              = 0x7F,
};
typedef enum mipc_dsda_allowed_type_const_enum mipc_dsda_allowed_type_const_enum;

enum mipc_dsda_state_type_const_enum {
    mipc_dsda_state_type_const_NONE = 0,
    /* DSDA_STATE_TYPE */
    MIPC_DSDA_STATE_TYPE_ACTIVE                             = 0,
    MIPC_DSDA_STATE_TYPE_POSSIBLE                           = 1,
    MIPC_DSDA_STATE_TYPE_NOT_POSSIBLE                       = 2,
    MIPC_DSDA_STATE_TYPE_MAX                                = 0x7F,
};
typedef enum mipc_dsda_state_type_const_enum mipc_dsda_state_type_const_enum;

enum mipc_dr_dsda_type_const_enum {
    mipc_dr_dsda_type_const_NONE = 0,
    /* DR_DSDA_TYPE */
    MIPC_DR_DSDA_TYPE_DSDA                                  = 0,
    MIPC_DR_DSDA_TYPE_DR_DSDA                               = 1,
    MIPC_DR_DSDA_TYPE_MAX                                   = 0x7F,
};
typedef enum mipc_dr_dsda_type_const_enum mipc_dr_dsda_type_const_enum;

enum mipc_dr_dsds_type_const_enum {
    mipc_dr_dsds_type_const_NONE = 0,
    /* DR_DSDS_TYPE */
    MIPC_DR_DSDS_TYPE_DSDA                                  = 0,
    MIPC_DR_DSDS_TYPE_DR_DSDS                               = 1,
    MIPC_DR_DSDS_TYPE_MAX                                   = 0x7F,
};
typedef enum mipc_dr_dsds_type_const_enum mipc_dr_dsds_type_const_enum;

enum mipc_psi_action_enum_const_enum {
    mipc_psi_action_enum_const_NONE = 0,
    /* PSI_ACTION_ENUM */
    MIPC_PSI_ACTION_ENUM_RELEASE                            = 0,
    MIPC_PSI_ACTION_ENUM_OCCUPY                             = 1,
    MIPC_PSI_ACTION_ENUM_QUERY                              = 2,
    MIPC_PSI_ACTION_ENUM_INJECT                             = 3,
    MIPC_PSI_ACTION_ENUM_REQUIRE                            = 4,
    MIPC_PSI_ACTION_ENUM_MAX                                = 0x7F,
};
typedef enum mipc_psi_action_enum_const_enum mipc_psi_action_enum_const_enum;

enum mipc_ssc_mode_enum_const_enum {
    mipc_ssc_mode_enum_const_NONE = 0,
    /* SSC_MODE_ENUM */
    MIPC_SSC_MODE_ENUM_NONE                                 = 0,
    MIPC_SSC_MODE_ENUM_SSC_MODE_1                           = 1,
    MIPC_SSC_MODE_ENUM_SSC_MODE_2                           = 2,
    MIPC_SSC_MODE_ENUM_SSC_MODE_3                           = 3,
    MIPC_SSC_MODE_ENUM_MAX                                  = 0x7F,
};
typedef enum mipc_ssc_mode_enum_const_enum mipc_ssc_mode_enum_const_enum;

enum mipc_ursp_route_supp_profile_type_enum_const_enum {
    mipc_ursp_route_supp_profile_type_enum_const_NONE = 0,
    /* URSP_ROUTE_SUPP_PROFILE_TYPE_ENUM */
    MIPC_URSP_ROUTE_SUPP_PROFILE_TYPE_ENUM_REQ              = 1,
    MIPC_URSP_ROUTE_SUPP_PROFILE_TYPE_ENUM_MAX              = 0x7FFFFFFF,
};
typedef enum mipc_ursp_route_supp_profile_type_enum_const_enum mipc_ursp_route_supp_profile_type_enum_const_enum;

enum mipc_ursp_reeval_type_enum_const_enum {
    mipc_ursp_reeval_type_enum_const_NONE = 0,
    /* URSP_REEVAL_TYPE_ENUM */
    MIPC_URSP_REEVAL_TYPE_ENUM_PERIODIC                     = 1,
    MIPC_URSP_REEVAL_TYPE_ENUM_PDU_REL                      = 2,
    MIPC_URSP_REEVAL_TYPE_ENUM_PCF_UPDATE                   = 3,
    MIPC_URSP_REEVAL_TYPE_ENUM_INTER_SYS                    = 4,
    MIPC_URSP_REEVAL_TYPE_ENUM_REG                          = 5,
    MIPC_URSP_REEVAL_TYPE_ENUM_WLAN_EST                     = 6,
    MIPC_URSP_REEVAL_TYPE_ENUM_WLAN_REL                     = 7,
    MIPC_URSP_REEVAL_TYPE_ENUM_ALLOWED_NSSAI                = 8,
    MIPC_URSP_REEVAL_TYPE_ENUM_LADN                         = 9,
    MIPC_URSP_REEVAL_TYPE_ENUM_MAX                          = 0x7FFFFFFF,
};
typedef enum mipc_ursp_reeval_type_enum_const_enum mipc_ursp_reeval_type_enum_const_enum;

enum mipc_nssai_type_enum_const_enum {
    mipc_nssai_type_enum_const_NONE = 0,
    /* NSSAI_TYPE_ENUM */
    MIPC_NSSAI_TYPE_ENUM_DEFAULT_CONFIGURED_NSSAI           = 0,
    MIPC_NSSAI_TYPE_ENUM_DEFAULT_CONFIGURED_AND_REJECT_NSSAI = 1,
    MIPC_NSSAI_TYPE_ENUM_DEFAULT_CONFIGURED_AND_REJECT_AND_CONFIGURED_NSSAI = 2,
    MIPC_NSSAI_TYPE_ENUM_DEFAULT_CONFIGURED_AND_REJECT_AND_CONFIGURED_AND_ALLOWED_NSSAI = 3,
    MIPC_NSSAI_TYPE_ENUM_MAX                                = 0x7F,
};
typedef enum mipc_nssai_type_enum_const_enum mipc_nssai_type_enum_const_enum;

enum mipc_nw_radio_state_const_enum {
    mipc_nw_radio_state_const_NONE = 0,
    /* NW_RADIO_STATE */
    MIPC_NW_RADIO_STATE_OFF                                 = 0,
    MIPC_NW_RADIO_STATE_ON                                  = 1,
};
typedef enum mipc_nw_radio_state_const_enum mipc_nw_radio_state_const_enum;

enum mipc_nw_radio_state_cause_const_enum {
    mipc_nw_radio_state_cause_const_NONE = 0,
    /* NW_RADIO_STATE_CAUSE */
    MIPC_NW_RADIO_STATE_CAUSE_UNSPECIFIED                   = 0,
    MIPC_NW_RADIO_STATE_CAUSE_DUPLEX_MODE_CHANGE            = 1,
    MIPC_NW_RADIO_STATE_CAUSE_POWER_OFF                     = 2,
    MIPC_NW_RADIO_STATE_CAUSE_SIM_SWITCH                    = 3,
    MIPC_NW_RADIO_STATE_CAUSE_POWER_EPOF                    = 128,
    MIPC_NW_RADIO_STATE_CAUSE_ENTER_AOC50                   = 129,
    MIPC_NW_RADIO_STATE_CAUSE_GRACEFUL_POWER_OFF            = 130,
};
typedef enum mipc_nw_radio_state_cause_const_enum mipc_nw_radio_state_cause_const_enum;

enum mipc_nw_provider_state_const_enum {
    mipc_nw_provider_state_const_NONE = 0,
    /* NW_PROVIDER_STATE */
    /*  */
    MIPC_NW_PROVIDER_STATE_UNKNOWN                          = 0,
    /*  */
    MIPC_NW_PROVIDER_STATE_AVAILABLE                        = 1,
    /*  */
    MIPC_NW_PROVIDER_STATE_CURRENT                          = 2,
    /*  */
    MIPC_NW_PROVIDER_STATE_FORBIDDEN                        = 3,
};
typedef enum mipc_nw_provider_state_const_enum mipc_nw_provider_state_const_enum;

enum mipc_nw_register_mode_const_enum {
    mipc_nw_register_mode_const_NONE = 0,
    /* NW_REGISTER_MODE */
    /*  */
    MIPC_NW_REGISTER_MODE_AUTOMATIC                         = 0,
    /*  */
    MIPC_NW_REGISTER_MODE_MANUAL                            = 1,
    /*  */
    MIPC_NW_REGISTER_MODE_DEREGISTER                        = 2,
    /* This command will do manual selection first. If it failed, it will try automatic selection and keep the selection mode in automatic. */
    MIPC_NW_REGISTER_MODE_MANUAL_EXT                        = 4,
    /* This command also provides semi-auto selection, which can trigger one manual selection procedure but keep the selection mode in automatic. */
    MIPC_NW_REGISTER_MODE_SEMI_AUTO                         = 5,
    MIPC_NW_REGISTER_MODE_SPECIFY_SEARCH                    = 7,
};
typedef enum mipc_nw_register_mode_const_enum mipc_nw_register_mode_const_enum;

enum mipc_nw_register_state_const_enum {
    mipc_nw_register_state_const_NONE = 0,
    /* NW_REGISTER_STATE */
    MIPC_NW_REGISTER_STATE_NOT_REGISTERED                   = 0,
    MIPC_NW_REGISTER_STATE_HOME                             = 1,
    MIPC_NW_REGISTER_STATE_SEARCHING                        = 2,
    MIPC_NW_REGISTER_STATE_DENIED                           = 3,
    MIPC_NW_REGISTER_STATE_UNKNOWN                          = 4,
    MIPC_NW_REGISTER_STATE_ROAMING                          = 5,
    MIPC_NW_REGISTER_STATE_HOME_SMS_ONLY                    = 6,
    MIPC_NW_REGISTER_STATE_ROAMING_SMS_ONLY                 = 7,
    MIPC_NW_REGISTER_STATE_ATTACHED_ECC_ONLY                = 8,
    MIPC_NW_REGISTER_STATE_HOME_CSFB_NOT_PREF               = 9,
    MIPC_NW_REGISTER_STATE_ROAMING_CSFB_NOT_PREF            = 10,
};
typedef enum mipc_nw_register_state_const_enum mipc_nw_register_state_const_enum;

enum mipc_nw_ps_const_enum {
    mipc_nw_ps_const_NONE = 0,
    /* NW_PS */
    MIPC_NW_PS_DETACH                                       = 0,
    MIPC_NW_PS_ATTACH                                       = 1,
};
typedef enum mipc_nw_ps_const_enum mipc_nw_ps_const_enum;

enum mipc_nw_ia_const_enum {
    mipc_nw_ia_const_NONE = 0,
    /* NW_IA */
    MIPC_NW_IA_DETACH                                       = 0,
    MIPC_NW_IA_ATTACH                                       = 1,
};
typedef enum mipc_nw_ia_const_enum mipc_nw_ia_const_enum;

enum mipc_nw_fast_dormancy_const_enum {
    mipc_nw_fast_dormancy_const_NONE = 0,
    /* NW_FAST_DORMANCY */
    MIPC_NW_FAST_DORMANCY_DISABLE                           = 0,
    MIPC_NW_FAST_DORMANCY_ENABLE                            = 1,
    MIPC_NW_FAST_DORMANCY_SET_TIMER                         = 2,
    MIPC_NW_FAST_DORMANCY_SCREEN_STATUS                     = 3,
};
typedef enum mipc_nw_fast_dormancy_const_enum mipc_nw_fast_dormancy_const_enum;

enum mipc_nw_blacklist_type_const_enum {
    mipc_nw_blacklist_type_const_NONE = 0,
    /* NW_BLACKLIST_TYPE */
    MIPC_NW_BLACKLIST_TYPE_SIM                              = 0,
    MIPC_NW_BLACKLIST_TYPE_NETWORK                          = 1,
};
typedef enum mipc_nw_blacklist_type_const_enum mipc_nw_blacklist_type_const_enum;

enum mipc_nw_cell_type_const_enum {
    mipc_nw_cell_type_const_NONE = 0,
    /* NW_CELL_TYPE */
    MIPC_NW_CELL_TYPE_NONE                                  = 0,
    MIPC_NW_CELL_TYPE_GSM                                   = 1,
    MIPC_NW_CELL_TYPE_CDMA                                  = 2,
    MIPC_NW_CELL_TYPE_LTE                                   = 3,
    MIPC_NW_CELL_TYPE_UMTS                                  = 4,
    MIPC_NW_CELL_TYPE_TD_SCDMA                              = 5,
    MIPC_NW_CELL_TYPE_NR                                    = 6,
    /* Just valid when in NSA case, this means the cell info has LTE and NR. */
    MIPC_NW_CELL_TYPE_NSA_EXT                               = 7,
};
typedef enum mipc_nw_cell_type_const_enum mipc_nw_cell_type_const_enum;

enum mipc_nw_ch_lock_const_enum {
    mipc_nw_ch_lock_const_NONE = 0,
    /* NW_CH_LOCK */
    MIPC_NW_CH_LOCK_CANCEL                                  = 0,
    MIPC_NW_CH_LOCK_ENABLE_CH_LK_EM                         = 1,
    MIPC_NW_CH_LOCK_ENABLE_CH_LK_CBRS                       = 2,
    MIPC_NW_CH_LOCK_CONFIG_CH_LK_CBRS                       = 3,
};
typedef enum mipc_nw_ch_lock_const_enum mipc_nw_ch_lock_const_enum;

enum mipc_nw_ch_lock_mode_const_enum {
    mipc_nw_ch_lock_mode_const_NONE = 0,
    /* NW_CH_LOCK_MODE */
    MIPC_NW_CH_LOCK_MODE_IDLE_MODE_ONLY                     = 0,
    MIPC_NW_CH_LOCK_MODE_IDLE_MR                            = 1,
    MIPC_NW_CH_LOCK_MODE_IDLE_HOCCO                         = 2,
    MIPC_NW_CH_LOCK_MODE_IDLE_AND_CONNECTED                 = 3,
    MIPC_NW_CH_LOCK_MODE_UNCHANGED                          = 255,
};
typedef enum mipc_nw_ch_lock_mode_const_enum mipc_nw_ch_lock_mode_const_enum;

enum mipc_nw_data_speed_const_enum {
    mipc_nw_data_speed_const_NONE = 0,
    /* NW_DATA_SPEED */
    MIPC_NW_DATA_SPEED_NONE_SPECIFIED                       = 0x0000,
    MIPC_NW_DATA_SPEED_GPRS                                 = 0x0001,
    MIPC_NW_DATA_SPEED_EDGE                                 = 0x0002,
    MIPC_NW_DATA_SPEED_UMTS                                 = 0x0004,
    MIPC_NW_DATA_SPEED_HSDPA                                = 0x0008,
    MIPC_NW_DATA_SPEED_HSUPA                                = 0x0010,
    MIPC_NW_DATA_SPEED_HSDPA_UPA                            = 0x0018,
    MIPC_NW_DATA_SPEED_HSDPAP                               = 0x0020,
    MIPC_NW_DATA_SPEED_HSDPAP_UPA                           = 0x0030,
    MIPC_NW_DATA_SPEED_HSUPAP                               = 0x0040,
    MIPC_NW_DATA_SPEED_HSUPAP_DPA                           = 0x0048,
    MIPC_NW_DATA_SPEED_HSPAP                                = 0x0060,
    MIPC_NW_DATA_SPEED_DC_DPA                               = 0x0088,
    MIPC_NW_DATA_SPEED_DC_DPA_UPA                           = 0x0098,
    MIPC_NW_DATA_SPEED_DC_HSDPAP                            = 0x00a0,
    MIPC_NW_DATA_SPEED_DC_HSDPAP_UPA                        = 0x00b0,
    MIPC_NW_DATA_SPEED_DC_HSUPAP_DPA                        = 0x00c8,
    MIPC_NW_DATA_SPEED_DC_HSPAP                             = 0x00e0,
    MIPC_NW_DATA_SPEED_1XRTT                                = 0x0100,
    MIPC_NW_DATA_SPEED_HRPD                                 = 0x0200,
    MIPC_NW_DATA_SPEED_EHRPD                                = 0x0400,
    MIPC_NW_DATA_SPEED_LTE                                  = 0x1000,
    MIPC_NW_DATA_SPEED_LTE_CA                               = 0x2000,
    MIPC_NW_DATA_SPEED_ENDC                                 = 0x4000,
    MIPC_NW_DATA_SPEED_NR                                   = 0x8000,
    MIPC_NW_DATA_SPEED_NOT_CHANGE                           = 0xffff,
};
typedef enum mipc_nw_data_speed_const_enum mipc_nw_data_speed_const_enum;

enum mipc_nw_signal_type_const_enum {
    mipc_nw_signal_type_const_NONE = 0,
    /* NW_SIGNAL_TYPE */
    MIPC_NW_SIGNAL_TYPE_GSM                                 = 0,
    MIPC_NW_SIGNAL_TYPE_UMTS                                = 1,
    MIPC_NW_SIGNAL_TYPE_LTE                                 = 2,
    MIPC_NW_SIGNAL_TYPE_NR                                  = 3,
    MIPC_NW_SIGNAL_TYPE_NR_NSA                              = 4,
    MIPC_NW_SIGNAL_TYPE_CDMA                                = 5,
};
typedef enum mipc_nw_signal_type_const_enum mipc_nw_signal_type_const_enum;

enum mipc_nw_cscon_mode_const_enum {
    mipc_nw_cscon_mode_const_NONE = 0,
    /* NW_CSCON_MODE */
    MIPC_NW_CSCON_MODE_IDLE                                 = 0,
    MIPC_NW_CSCON_MODE_CONNECTED                            = 1,
    MIPC_NW_CSCON_MODE_UNKNOWN                              = 255,
};
typedef enum mipc_nw_cscon_mode_const_enum mipc_nw_cscon_mode_const_enum;

enum mipc_nw_cscon_state_const_enum {
    mipc_nw_cscon_state_const_NONE = 0,
    /* NW_CSCON_STATE */
    MIPC_NW_CSCON_STATE_UTRAN_URA_PCH                       = 0,
    MIPC_NW_CSCON_STATE_UTRAN_CELL_PCH                      = 1,
    MIPC_NW_CSCON_STATE_UTRAN_CELL_FACH                     = 2,
    MIPC_NW_CSCON_STATE_UTRAN_CELL_DCH                      = 3,
    MIPC_NW_CSCON_STATE_GERAN_CS_CONNECTED                  = 4,
    MIPC_NW_CSCON_STATE_GERAN_PS_CONNECTED                  = 5,
    MIPC_NW_CSCON_STATE_GERAN_CS_PS_CONNECTED               = 6,
    MIPC_NW_CSCON_STATE_EUTRAN_CONNECTED                    = 7,
    MIPC_NW_CSCON_STATE_NR_RAN_CONNECTED                    = 8,
    MIPC_NW_CSCON_STATE_NR_RAN_INACTIVE                     = 8,
    MIPC_NW_CSCON_STATE_UNKNOWN                             = 255,
};
typedef enum mipc_nw_cscon_state_const_enum mipc_nw_cscon_state_const_enum;

enum mipc_nw_cscon_access_const_enum {
    mipc_nw_cscon_access_const_NONE = 0,
    /* NW_CSCON_ACCESS */
    MIPC_NW_CSCON_ACCESS_GERAN                              = 0,
    MIPC_NW_CSCON_ACCESS_UTRAN_TDD                          = 1,
    MIPC_NW_CSCON_ACCESS_UTRAN_FDD                          = 2,
    MIPC_NW_CSCON_ACCESS_EUTRAN_TDD                         = 3,
    MIPC_NW_CSCON_ACCESS_EUTRAN_FDD                         = 4,
    MIPC_NW_CSCON_ACCESS_NR                                 = 5,
    MIPC_NW_CSCON_ACCESS_UNKNOWN                            = 255,
};
typedef enum mipc_nw_cscon_access_const_enum mipc_nw_cscon_access_const_enum;

enum mipc_nw_cscon_core_network_const_enum {
    mipc_nw_cscon_core_network_const_NONE = 0,
    /* NW_CSCON_CORE_NETWORK */
    MIPC_NW_CSCON_CORE_NETWORK_EPC                          = 0,
    MIPC_NW_CSCON_CORE_NETWORK_5GCN                         = 1,
    MIPC_NW_CSCON_CORE_NETWORK_UNKNOWN                      = 255,
};
typedef enum mipc_nw_cscon_core_network_const_enum mipc_nw_cscon_core_network_const_enum;

enum mipc_nw_prefer_rat_type_const_enum {
    mipc_nw_prefer_rat_type_const_NONE = 0,
    /* NW_PREFER_RAT_TYPE */
    MIPC_NW_PREFER_RAT_TYPE_GSM                             = 1,
    MIPC_NW_PREFER_RAT_TYPE_UMTS                            = 2,
    MIPC_NW_PREFER_RAT_TYPE_LTE                             = 4,
    MIPC_NW_PREFER_RAT_TYPE_NR                              = 128,
};
typedef enum mipc_nw_prefer_rat_type_const_enum mipc_nw_prefer_rat_type_const_enum;

enum mipc_nw_lte_carrier_arrregation_switch_const_enum {
    mipc_nw_lte_carrier_arrregation_switch_const_NONE = 0,
    /* NW_LTE_CARRIER_ARRREGATION_SWITCH */
    MIPC_NW_LTE_CARRIER_ARRREGATION_SWITCH_TURNOFF          = 0,
    MIPC_NW_LTE_CARRIER_ARRREGATION_SWITCH_TURNON           = 1,
};
typedef enum mipc_nw_lte_carrier_arrregation_switch_const_enum mipc_nw_lte_carrier_arrregation_switch_const_enum;

enum mipc_nw_ps_cs_reg_state_const_enum {
    mipc_nw_ps_cs_reg_state_const_NONE = 0,
    /* NW_PS_CS_REG_STATE */
    MIPC_NW_PS_CS_REG_STATE_IN_SERVICE                      = 0,
    MIPC_NW_PS_CS_REG_STATE_OUT_OF_SERVICE                  = 1,
    MIPC_NW_PS_CS_REG_STATE_EMERGENCY_ONLY                  = 2,
    MIPC_NW_PS_CS_REG_STATE_POWER_OFF                       = 3,
};
typedef enum mipc_nw_ps_cs_reg_state_const_enum mipc_nw_ps_cs_reg_state_const_enum;

enum mipc_nw_ps_cs_roaming_type_const_enum {
    mipc_nw_ps_cs_roaming_type_const_NONE = 0,
    /* NW_PS_CS_ROAMING_TYPE */
    MIPC_NW_PS_CS_ROAMING_TYPE_NOT_ROAMING                  = 0,
    MIPC_NW_PS_CS_ROAMING_TYPE_UNKNOWN                      = 1,
    MIPC_NW_PS_CS_ROAMING_TYPE_DOMESTIC                     = 2,
    MIPC_NW_PS_CS_ROAMING_TYPE_INTERNATIONAL                = 3,
};
typedef enum mipc_nw_ps_cs_roaming_type_const_enum mipc_nw_ps_cs_roaming_type_const_enum;

enum mipc_nw_ril_ps_cs_reg_state_const_enum {
    mipc_nw_ril_ps_cs_reg_state_const_NONE = 0,
    /* NW_RIL_PS_CS_REG_STATE */
    MIPC_NW_RIL_PS_CS_REG_STATE_NOT_REGISTERED_AND_NOT_SEARCHING = 0,
    MIPC_NW_RIL_PS_CS_REG_STATE_HOME_NETWORK                = 1,
    MIPC_NW_RIL_PS_CS_REG_STATE_NOT_REGISTERED_AND_SEARCHING = 2,
    MIPC_NW_RIL_PS_CS_REG_STATE_REG_DENIED                  = 3,
    MIPC_NW_RIL_PS_CS_REG_STATE_UNKNOWN                     = 4,
    MIPC_NW_RIL_PS_CS_REG_STATE_ROAMING                     = 5,
    MIPC_NW_RIL_PS_CS_REG_STATE_GOOGLE_UNDEFINED1           = 6,
    MIPC_NW_RIL_PS_CS_REG_STATE_GOOGLE_UNDEFINED2           = 7,
    MIPC_NW_RIL_PS_CS_REG_STATE_GOOGLE_UNDEFINED3           = 8,
    MIPC_NW_RIL_PS_CS_REG_STATE_GOOGLE_UNDEFINED4           = 9,
    MIPC_NW_RIL_PS_CS_REG_STATE_NOT_REGISTERED_AND_NOT_SEARCHING_EMERGENCY_CALL_ENABLED = 10,
    MIPC_NW_RIL_PS_CS_REG_STATE_GOOGLE_UNDEFINED5           = 11,
    MIPC_NW_RIL_PS_CS_REG_STATE_NOT_REGISTERED_AND_SEARCHING_EMERGENCY_CALL_ENABLED = 12,
    MIPC_NW_RIL_PS_CS_REG_STATE_REG_DENIED_EMERGENCY_CALL_ENABLED = 13,
    MIPC_NW_RIL_PS_CS_REG_STATE_UNKNOWN_EMERGENCY_CALL_ENABLED = 14,
    MIPC_NW_RIL_PS_CS_REG_STATE_GOOGLE_UNDEFINED6           = 15,
};
typedef enum mipc_nw_ril_ps_cs_reg_state_const_enum mipc_nw_ril_ps_cs_reg_state_const_enum;

enum mipc_nw_cellmeasurement_scan_type_const_enum {
    mipc_nw_cellmeasurement_scan_type_const_NONE = 0,
    /* NW_CELLMEASUREMENT_SCAN_TYPE */
    MIPC_NW_CELLMEASUREMENT_SCAN_TYPE_ONLINE_CUS_DEP        = 0,
    MIPC_NW_CELLMEASUREMENT_SCAN_TYPE_OFFLINE               = 1,
    MIPC_NW_CELLMEASUREMENT_SCAN_TYPE_ONLINE_NETWORK_DEP    = 2,
};
typedef enum mipc_nw_cellmeasurement_scan_type_const_enum mipc_nw_cellmeasurement_scan_type_const_enum;

enum mipc_nw_cellmeasurement_bit_rat_const_enum {
    mipc_nw_cellmeasurement_bit_rat_const_NONE = 0,
    /* NW_CELLMEASUREMENT_BIT_RAT */
    MIPC_NW_CELLMEASUREMENT_BIT_RAT_LTE                     = 0x0004,
    MIPC_NW_CELLMEASUREMENT_BIT_RAT_NR                      = 0x0080,
};
typedef enum mipc_nw_cellmeasurement_bit_rat_const_enum mipc_nw_cellmeasurement_bit_rat_const_enum;

enum mipc_nw_band_option_const_enum {
    mipc_nw_band_option_const_NONE = 0,
    /* NW_BAND_OPTION */
    /* Current setting bands */
    MIPC_NW_BAND_OPTION_CURRENT_SETTING                     = 0,
    /* The bands supported by modem system */
    MIPC_NW_BAND_OPTION_SYSTEM_SUPPORT                      = 1,
};
typedef enum mipc_nw_band_option_const_enum mipc_nw_band_option_const_enum;

enum mipc_nw_signal_threshold_mode_const_enum {
    mipc_nw_signal_threshold_mode_const_NONE = 0,
    /* NW_SIGNAL_THRESHOLD_MODE */
    MIPC_NW_SIGNAL_THRESHOLD_MODE_MAP_VALUE                 = 0,
    MIPC_NW_SIGNAL_THRESHOLD_MODE_RAW                       = 1,
};
typedef enum mipc_nw_signal_threshold_mode_const_enum mipc_nw_signal_threshold_mode_const_enum;

enum mipc_nw_roaming_mode_const_enum {
    mipc_nw_roaming_mode_const_NONE = 0,
    /* NW_ROAMING_MODE */
    MIPC_NW_ROAMING_MODE_DIS_NONE                           = 0x0,
    MIPC_NW_ROAMING_MODE_DIS_INTERNATIONAL                  = 0x1,
    MIPC_NW_ROAMING_MODE_DIS_NATIONAL                       = 0x2,
    MIPC_NW_ROAMING_MODE_DIS_H_P_PLMN_SEARCH                = 0x4,
};
typedef enum mipc_nw_roaming_mode_const_enum mipc_nw_roaming_mode_const_enum;

enum mipc_nw_antenna_platform_const_enum {
    mipc_nw_antenna_platform_const_NONE = 0,
    /* NW_ANTENNA_PLATFORM */
    MIPC_NW_ANTENNA_PLATFORM_VZ_6290                        = 0,
    MIPC_NW_ANTENNA_PLATFORM_VZ_6292                        = 1,
    MIPC_NW_ANTENNA_PLATFORM_VZ_6294                        = 2,
};
typedef enum mipc_nw_antenna_platform_const_enum mipc_nw_antenna_platform_const_enum;

enum mipc_nw_cell_connection_state_const_enum {
    mipc_nw_cell_connection_state_const_NONE = 0,
    /* NW_CELL_CONNECTION_STATE */
    MIPC_NW_CELL_CONNECTION_STATE_NONE_SERVING              = 0,
    MIPC_NW_CELL_CONNECTION_STATE_PRIMARY_SERVING           = 1,
    MIPC_NW_CELL_CONNECTION_STATE_SECONDARY_SERVING         = 2,
};
typedef enum mipc_nw_cell_connection_state_const_enum mipc_nw_cell_connection_state_const_enum;

enum mipc_nw_rat_switch_mode_const_enum {
    mipc_nw_rat_switch_mode_const_NONE = 0,
    /* NW_RAT_SWITCH_MODE */
    MIPC_NW_RAT_SWITCH_MODE_NONE                            = 0,
    MIPC_NW_RAT_SWITCH_MODE_NSA_SCG                         = 1,
    MIPC_NW_RAT_SWITCH_MODE_NSA_SCG_ALLOW                   = 2,
    MIPC_NW_RAT_SWITCH_MODE_NSA_TAU                         = 3,
    MIPC_NW_RAT_SWITCH_MODE_SA_ONLY                         = 4,
    MIPC_NW_RAT_SWITCH_MODE_SA_TAU                          = 5,
    MIPC_NW_RAT_SWITCH_MODE_BOTH                            = 6,
    MIPC_NW_RAT_SWITCH_MODE_BOTH_TAU                        = 7,
    MIPC_NW_RAT_SWITCH_MODE_ALL                             = 8,
    MIPC_NW_RAT_SWITCH_MODE_RRC_STATE                       = 9,
};
typedef enum mipc_nw_rat_switch_mode_const_enum mipc_nw_rat_switch_mode_const_enum;

enum mipc_nw_ind_type_const_enum {
    mipc_nw_ind_type_const_NONE = 0,
    /* NW_IND_TYPE */
    MIPC_NW_IND_TYPE_ECELL                                  = 1,
};
typedef enum mipc_nw_ind_type_const_enum mipc_nw_ind_type_const_enum;

enum mipc_nw_ps_ctrl_mode_const_enum {
    mipc_nw_ps_ctrl_mode_const_NONE = 0,
    /* NW_PS_CTRL_MODE */
    MIPC_NW_PS_CTRL_MODE_CGATT                              = 0,
    MIPC_NW_PS_CTRL_MODE_EGTYPE                             = 1,
};
typedef enum mipc_nw_ps_ctrl_mode_const_enum mipc_nw_ps_ctrl_mode_const_enum;

enum mipc_nw_act_type_const_enum {
    mipc_nw_act_type_const_NONE = 0,
    /* NW_ACT_TYPE */
    MIPC_NW_ACT_TYPE_GSM                                    = 0,
    MIPC_NW_ACT_TYPE_UMTS                                   = 2,
    MIPC_NW_ACT_TYPE_EGPRS                                  = 3,
    MIPC_NW_ACT_TYPE_HSDPA                                  = 4,
    MIPC_NW_ACT_TYPE_HSUPA                                  = 5,
    MIPC_NW_ACT_TYPE_HSDPA_HSPUPA                           = 6,
    MIPC_NW_ACT_TYPE_LTE                                    = 7,
    MIPC_NW_ACT_TYPE_NR                                     = 11,
    MIPC_NW_ACT_TYPE_NG_RAN                                 = 12,
    MIPC_NW_ACT_TYPE_ENDC                                   = 13,
    MIPC_NW_ACT_TYPE_INVALID                                = 0xFF,
};
typedef enum mipc_nw_act_type_const_enum mipc_nw_act_type_const_enum;

enum mipc_nw_ps_test_mode_const_enum {
    mipc_nw_ps_test_mode_const_NONE = 0,
    /* NW_PS_TEST_MODE */
    MIPC_NW_PS_TEST_MODE_NONE                               = 0,
    MIPC_NW_PS_TEST_MODE_CTA                                = 1,
    MIPC_NW_PS_TEST_MODE_FTA                                = 2,
    MIPC_NW_PS_TEST_MODE_IOT                                = 3,
    MIPC_NW_PS_TEST_MODE_OPERATOR                           = 4,
    MIPC_NW_PS_TEST_MODE_FACTORY                            = 5,
};
typedef enum mipc_nw_ps_test_mode_const_enum mipc_nw_ps_test_mode_const_enum;

enum mipc_nw_signal_measurement_type_const_enum {
    mipc_nw_signal_measurement_type_const_NONE = 0,
    /* NW_SIGNAL_MEASUREMENT_TYPE */
    /* Received Signal Strength Indication. Used RAT: GSM, C2K */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_RSSI                    = 1,
    /* Received Signal Code Power. Used RAT: UMTS */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_RSCP                    = 2,
    /* Reference Signal Received Power. Used RAT: LTE */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_RSRP                    = 3,
    /* Reference Signal Received Quality. Used RAT: LTE */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_RSRQ                    = 4,
    /* Reference Signal Signal-to-Noise Ratio. Used RAT: LTE */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_RSSNR                   = 5,
    /* 5G SS Reference Signal Received Power. Used RAT: NR */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_SSRSRP                  = 6,
    /* 5G SS Reference Signal Received Qulity. Used RAT: NR */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_SSRSRQ                  = 7,
    /* 5G SS Signal-to-Noise and Interference Ratio.  Used RAT: NR */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_SSSINR                  = 8,
    /* EcN0 value.  Used RAT: UMTS */
    MIPC_NW_SIGNAL_MEASUREMENT_TYPE_ECN0                    = 9,
};
typedef enum mipc_nw_signal_measurement_type_const_enum mipc_nw_signal_measurement_type_const_enum;

enum mipc_nw_signal_rat_const_enum {
    mipc_nw_signal_rat_const_NONE = 0,
    /* NW_SIGNAL_RAT */
    MIPC_NW_SIGNAL_RAT_GSM                                  = 1,
    MIPC_NW_SIGNAL_RAT_UMTS                                 = 2,
    MIPC_NW_SIGNAL_RAT_LTE                                  = 3,
    MIPC_NW_SIGNAL_RAT_C2K                                  = 4,
    MIPC_NW_SIGNAL_RAT_NR                                   = 5,
};
typedef enum mipc_nw_signal_rat_const_enum mipc_nw_signal_rat_const_enum;

enum mipc_nw_bit_rat_const_enum {
    mipc_nw_bit_rat_const_NONE = 0,
    /* NW_BIT_RAT */
    MIPC_NW_BIT_RAT_GSM_BIT                                 = 1,
    MIPC_NW_BIT_RAT_UMTS_BIT                                = 2,
    MIPC_NW_BIT_RAT_LTE_BIT                                 = 4,
    MIPC_NW_BIT_RAT_NR_BIT                                  = 8,
    MIPC_NW_BIT_RAT_C2K_BIT                                 = 16,
};
typedef enum mipc_nw_bit_rat_const_enum mipc_nw_bit_rat_const_enum;

enum mipc_nw_bit_act_const_enum {
    mipc_nw_bit_act_const_NONE = 0,
    /* NW_BIT_ACT */
    /* Bit0. This bit represents GSM access technology, 0: access technology not selected, 1: access technology selected */
    MIPC_NW_BIT_ACT_GSM_ACTN_BIT                            = 1,
    /* Bit1. This bit represents GSM compact access technology, 0: access technology not selected, 1: access technology selected */
    MIPC_NW_BIT_ACT_GSM_COMPACT_ACTN_BIT                    = 2,
    /* Bit2. This bit represents UTRAN access technology, 0: access technology not selected, 1: access technology selected */
    MIPC_NW_BIT_ACT_UTRAN_ACTN_BIT                          = 4,
    /* Bit3. This bit represents E-UTRAN access technology, 0: access technology not selected, 1: access technology selected */
    MIPC_NW_BIT_ACT_E_UTRAN_ACTN_BIT                        = 8,
    /* Bit4. This bit represents NG-RAN access technology, 0: access technology not selected, 1: access technology selected */
    MIPC_NW_BIT_ACT_NG_RAN_ACTN_BIT                         = 16,
};
typedef enum mipc_nw_bit_act_const_enum mipc_nw_bit_act_const_enum;

enum mipc_nw_iwlan_status_const_enum {
    mipc_nw_iwlan_status_const_NONE = 0,
    /* NW_IWLAN_STATUS */
    MIPC_NW_IWLAN_STATUS_NOT_READY                          = 0,
    MIPC_NW_IWLAN_STATUS_READY                              = 1,
};
typedef enum mipc_nw_iwlan_status_const_enum mipc_nw_iwlan_status_const_enum;

enum mipc_nw_ca_mode_const_enum {
    mipc_nw_ca_mode_const_NONE = 0,
    /* NW_CA_MODE */
    MIPC_NW_CA_MODE_TURN_OFF                                = 0,
    MIPC_NW_CA_MODE_TURN_ON                                 = 1,
};
typedef enum mipc_nw_ca_mode_const_enum mipc_nw_ca_mode_const_enum;

enum mipc_nw_clear_nssai_type_const_enum {
    mipc_nw_clear_nssai_type_const_NONE = 0,
    /* NW_CLEAR_NSSAI_TYPE */
    MIPC_NW_CLEAR_NSSAI_TYPE_CLEAR_NSSAI_CONFIG_ALL         = 0,
    MIPC_NW_CLEAR_NSSAI_TYPE_CLEAR_PREFERRED_NON3GPP_NSSAI_CONFIG = 1,
    MIPC_NW_CLEAR_NSSAI_TYPE_CLEAR_PREFERRED_3GPP_NSSAI_CONFIG = 2,
    MIPC_NW_CLEAR_NSSAI_TYPE_CLEAR_PREFERRED_ALL_NSSAI_CONFIG = 3,
    MIPC_NW_CLEAR_NSSAI_TYPE_CLEAR_DEFAULT_NSSAI_CONFIG     = 4,
};
typedef enum mipc_nw_clear_nssai_type_const_enum mipc_nw_clear_nssai_type_const_enum;

enum mipc_rx_mimo_ctrl_operation_const_enum {
    mipc_rx_mimo_ctrl_operation_const_NONE = 0,
    /* RX_MIMO_CTRL_OPERATION */
    MIPC_RX_MIMO_CTRL_OPERATION_CTRL_OPERATION_RESET        = 0,
    MIPC_RX_MIMO_CTRL_OPERATION_CTRL_OPERATION_FORCED_1RX   = 1,
    MIPC_RX_MIMO_CTRL_OPERATION_CTRL_OPERATION_FORCED_2RX   = 2,
    MIPC_RX_MIMO_CTRL_OPERATION_CTRL_OPERATION_FORCED_4RX   = 3,
};
typedef enum mipc_rx_mimo_ctrl_operation_const_enum mipc_rx_mimo_ctrl_operation_const_enum;

enum mipc_rx_mimo_release_level_const_enum {
    mipc_rx_mimo_release_level_const_NONE = 0,
    /* RX_MIMO_RELEASE_LEVEL */
    MIPC_RX_MIMO_RELEASE_LEVEL_RELEASE_LEVEL_NO_NEED_RELEASE = 0,
    MIPC_RX_MIMO_RELEASE_LEVEL_RELEASE_LEVEL_ATTEMP_RELEASE = 1,
    MIPC_RX_MIMO_RELEASE_LEVEL_RELEASE_LEVEL_FORCED_RELEASE = 2,
};
typedef enum mipc_rx_mimo_release_level_const_enum mipc_rx_mimo_release_level_const_enum;

enum mipc_lte_rrc_state_const_enum {
    mipc_lte_rrc_state_const_NONE = 0,
    /* LTE_RRC_STATE */
    MIPC_LTE_RRC_STATE_NULL                                 = 0,
    MIPC_LTE_RRC_STATE_IDLE                                 = 1,
    MIPC_LTE_RRC_STATE_ATMPT_CONNECTION                     = 2,
    MIPC_LTE_RRC_STATE_CONNECTED                            = 3,
    MIPC_LTE_RRC_STATE_ENDING                               = 4,
};
typedef enum mipc_lte_rrc_state_const_enum mipc_lte_rrc_state_const_enum;

enum mipc_cell_list_type_const_enum {
    mipc_cell_list_type_const_NONE = 0,
    /* CELL_LIST_TYPE */
    /* Get LTE available 1xRTT cell lists */
    MIPC_CELL_LIST_TYPE_AVAILABLE                           = 0,
    /* Get LTE unavailable 1xRTT cell lists */
    MIPC_CELL_LIST_TYPE_UNAVAILABLE                         = 1,
};
typedef enum mipc_cell_list_type_const_enum mipc_cell_list_type_const_enum;

enum mipc_ca_link_mode_const_enum {
    mipc_ca_link_mode_const_NONE = 0,
    /* CA_LINK_MODE */
    /* Get CA downlink capability */
    MIPC_CA_LINK_MODE_DOWNLINK                              = 0,
    /* Get CA downlink and uplink capability */
    MIPC_CA_LINK_MODE_DOWNLIINK_AND_UPLINK                  = 1,
};
typedef enum mipc_ca_link_mode_const_enum mipc_ca_link_mode_const_enum;

enum mipc_ca_link_enable_mode_const_enum {
    mipc_ca_link_enable_mode_const_NONE = 0,
    /* CA_LINK_ENABLE_MODE */
    /* Get DL CA enable status for specified band_combo. */
    MIPC_CA_LINK_ENABLE_MODE_DOWNLINK                       = 2,
    /* Get UL CA enable status for specified band_combo. */
    MIPC_CA_LINK_ENABLE_MODE_UPLINK                         = 3,
};
typedef enum mipc_ca_link_enable_mode_const_enum mipc_ca_link_enable_mode_const_enum;

enum mipc_band_combo_enable_status_const_enum {
    mipc_band_combo_enable_status_const_NONE = 0,
    /* BAND_COMBO_ENABLE_STATUS */
    /* The band_combo is disable. */
    MIPC_BAND_COMBO_ENABLE_STATUS_DISABLE                   = 0,
    /* The band_combo is enable. */
    MIPC_BAND_COMBO_ENABLE_STATUS_ENABLE                    = 1,
};
typedef enum mipc_band_combo_enable_status_const_enum mipc_band_combo_enable_status_const_enum;

enum mipc_tm9_enable_status_const_enum {
    mipc_tm9_enable_status_const_NONE = 0,
    /* TM9_ENABLE_STATUS */
    /* The TM9 is disable. */
    MIPC_TM9_ENABLE_STATUS_DISABLE                          = 0,
    /* The TM9 is enable. */
    MIPC_TM9_ENABLE_STATUS_ENABLE                           = 1,
};
typedef enum mipc_tm9_enable_status_const_enum mipc_tm9_enable_status_const_enum;

enum mipc_tm9_setting_type_const_enum {
    mipc_tm9_setting_type_const_NONE = 0,
    /* TM9_SETTING_TYPE */
    /* The FDD TM9. */
    MIPC_TM9_SETTING_TYPE_FDD                               = 0,
    /* The TDD TM9. */
    MIPC_TM9_SETTING_TYPE_TDD                               = 1,
};
typedef enum mipc_tm9_setting_type_const_enum mipc_tm9_setting_type_const_enum;

enum mipc_omadm_node_type_const_enum {
    mipc_omadm_node_type_const_NONE = 0,
    /* OMADM_NODE_TYPE */
    /* Autonomous gap */
    MIPC_OMADM_NODE_TYPE_AUTONOMOUS_GAP                     = 0,
    /* MPSR duration */
    MIPC_OMADM_NODE_TYPE_MPSR_DURATION                      = 1,
    /* SCAN duration */
    MIPC_OMADM_NODE_TYPE_SCAN_DURATION                      = 2,
    /* Sleep duration */
    MIPC_OMADM_NODE_TYPE_SLEEP_DURATION                     = 3,
    /* MPSR max duration */
    MIPC_OMADM_NODE_TYPE_MPSR_MAX_DURATION                  = 4,
    /* MPSR max duration */
    MIPC_OMADM_NODE_TYPE_LONG_MPSR_DURATION                 = 5,
    /* TPLMN barring timer */
    MIPC_OMADM_NODE_TYPE_TPLMN_BARRING_TIMER                = 6,
    /* LTE HPUE */
    MIPC_OMADM_NODE_TYPE_LTE_HPUE                           = 7,
};
typedef enum mipc_omadm_node_type_const_enum mipc_omadm_node_type_const_enum;

enum mipc_ca_comb_list_type_const_enum {
    mipc_ca_comb_list_type_const_NONE = 0,
    /* CA_COMB_LIST_TYPE */
    /* White list. */
    MIPC_CA_COMB_LIST_TYPE_WHITE                            = 0,
    /* Black list. */
    MIPC_CA_COMB_LIST_TYPE_BLACK                            = 1,
};
typedef enum mipc_ca_comb_list_type_const_enum mipc_ca_comb_list_type_const_enum;

enum mipc_nw_set_ca_rat_const_enum {
    mipc_nw_set_ca_rat_const_NONE = 0,
    /* NW_SET_CA_RAT */
    /* The CA of all rat */
    MIPC_NW_SET_CA_RAT_ALL                                  = 0,
    /* The LTE CA */
    MIPC_NW_SET_CA_RAT_LTE                                  = 1,
};
typedef enum mipc_nw_set_ca_rat_const_enum mipc_nw_set_ca_rat_const_enum;

enum mipc_nitz_ind_type_const_enum {
    mipc_nitz_ind_type_const_NONE = 0,
    /* NITZ_IND_TYPE */
    /* Sib16 time, AT+CIEV: 11 */
    MIPC_NITZ_IND_TYPE_CIEV                                 = 0,
    /* Report time zone, AT+CTZEU */
    MIPC_NITZ_IND_TYPE_CTZEU                                = 1,
};
typedef enum mipc_nitz_ind_type_const_enum mipc_nitz_ind_type_const_enum;

enum mipc_nw_tuw_id_const_enum {
    mipc_nw_tuw_id_const_NONE = 0,
    /* NW_TUW_ID */
    MIPC_NW_TUW_ID_TUW1                                     = 1,
    MIPC_NW_TUW_ID_TUW2                                     = 2,
    MIPC_NW_TUW_ID_TUW3                                     = 3,
};
typedef enum mipc_nw_tuw_id_const_enum mipc_nw_tuw_id_const_enum;

enum mipc_nw_ch_info_rat_const_enum {
    mipc_nw_ch_info_rat_const_NONE = 0,
    /* NW_CH_INFO_RAT */
    MIPC_NW_CH_INFO_RAT_GSM                                 = 1,
    MIPC_NW_CH_INFO_RAT_UMTS                                = 2,
    MIPC_NW_CH_INFO_RAT_LTE                                 = 4,
    MIPC_NW_CH_INFO_RAT_NR                                  = 128,
};
typedef enum mipc_nw_ch_info_rat_const_enum mipc_nw_ch_info_rat_const_enum;

enum mipc_nw_allowed_mcc_list_action_const_enum {
    mipc_nw_allowed_mcc_list_action_const_NONE = 0,
    /* NW_ALLOWED_MCC_LIST_ACTION */
    MIPC_NW_ALLOWED_MCC_LIST_ACTION_DISABLE                 = 0,
    MIPC_NW_ALLOWED_MCC_LIST_ACTION_ENABLE                  = 1,
};
typedef enum mipc_nw_allowed_mcc_list_action_const_enum mipc_nw_allowed_mcc_list_action_const_enum;

enum mipc_nw_vg_option_operation_const_enum {
    mipc_nw_vg_option_operation_const_NONE = 0,
    /* NW_VG_OPTION_OPERATION */
    MIPC_NW_VG_OPTION_OPERATION_VG_OPTION_OPERATION_NONE    = 0,
    MIPC_NW_VG_OPTION_OPERATION_VG_OPTION_OPERATION_LOCAL_RELEASE = 1,
    MIPC_NW_VG_OPTION_OPERATION_VG_OPTION_OPERATION_LOCAL_RELEASE_NR_SEARCH = 2,
};
typedef enum mipc_nw_vg_option_operation_const_enum mipc_nw_vg_option_operation_const_enum;

enum mipc_lte_event_type_const_enum {
    mipc_lte_event_type_const_NONE = 0,
    /* LTE_EVENT_TYPE */
    MIPC_LTE_EVENT_TYPE_A1                                  = 1,
    MIPC_LTE_EVENT_TYPE_A2                                  = 2,
    MIPC_LTE_EVENT_TYPE_A3                                  = 3,
    MIPC_LTE_EVENT_TYPE_A4                                  = 4,
    MIPC_LTE_EVENT_TYPE_A5                                  = 5,
    MIPC_LTE_EVENT_TYPE_B1_GSM                              = 6,
    MIPC_LTE_EVENT_TYPE_B2_GSM                              = 7,
    MIPC_LTE_EVENT_TYPE_B1_UMTS                             = 8,
    MIPC_LTE_EVENT_TYPE_B2_UMTS                             = 9,
    MIPC_LTE_EVENT_TYPE_B1_NR                               = 10,
    MIPC_LTE_EVENT_TYPE_B2_NR                               = 11,
};
typedef enum mipc_lte_event_type_const_enum mipc_lte_event_type_const_enum;

enum mipc_signal_quality_type_const_enum {
    mipc_signal_quality_type_const_NONE = 0,
    /* SIGNAL_QUALITY_TYPE */
    MIPC_SIGNAL_QUALITY_TYPE_RSRP                           = 1,
    MIPC_SIGNAL_QUALITY_TYPE_RSRQ                           = 2,
    MIPC_SIGNAL_QUALITY_TYPE_SINR                           = 3,
    MIPC_SIGNAL_QUALITY_TYPE_RSSI                           = 4,
    MIPC_SIGNAL_QUALITY_TYPE_RSCP                           = 5,
    MIPC_SIGNAL_QUALITY_TYPE_ECNO                           = 6,
};
typedef enum mipc_signal_quality_type_const_enum mipc_signal_quality_type_const_enum;

enum mipc_nw_scan_action_const_enum {
    mipc_nw_scan_action_const_NONE = 0,
    /* NW_SCAN_ACTION */
    /* Start network scanning */
    MIPC_NW_SCAN_ACTION_START                               = 1,
    /* Stop network scanning */
    MIPC_NW_SCAN_ACTION_STOP                                = 2,
};
typedef enum mipc_nw_scan_action_const_enum mipc_nw_scan_action_const_enum;

enum mipc_nw_service_scan_type_const_enum {
    mipc_nw_service_scan_type_const_NONE = 0,
    /* NW_SERVICE_SCAN_TYPE */
    MIPC_NW_SERVICE_SCAN_TYPE_NORMAL                        = 1,
};
typedef enum mipc_nw_service_scan_type_const_enum mipc_nw_service_scan_type_const_enum;

enum mipc_nw_scan_rat_const_enum {
    mipc_nw_scan_rat_const_NONE = 0,
    /* NW_SCAN_RAT */
    MIPC_NW_SCAN_RAT_GSM                                    = 1,
    MIPC_NW_SCAN_RAT_UMTS                                   = 2,
    MIPC_NW_SCAN_RAT_LTE                                    = 4,
    MIPC_NW_SCAN_RAT_C2K                                    = 16,
    MIPC_NW_SCAN_RAT_NR                                     = 128,
};
typedef enum mipc_nw_scan_rat_const_enum mipc_nw_scan_rat_const_enum;

enum mipc_nw_scan_type_const_enum {
    mipc_nw_scan_type_const_NONE = 0,
    /* NW_SCAN_TYPE */
    MIPC_NW_SCAN_TYPE_ONE_SHOT                              = 0,
    MIPC_NW_SCAN_TYPE_PERIODIC                              = 1,
};
typedef enum mipc_nw_scan_type_const_enum mipc_nw_scan_type_const_enum;

enum mipc_nw_incremental_results_const_enum {
    mipc_nw_incremental_results_const_NONE = 0,
    /* NW_INCREMENTAL_RESULTS */
    /* disable (one final result) */
    MIPC_NW_INCREMENTAL_RESULTS_DISABLE                     = 0,
    /* enable (multiple partial result) */
    MIPC_NW_INCREMENTAL_RESULTS_ENABLE                      = 1,
};
typedef enum mipc_nw_incremental_results_const_enum mipc_nw_incremental_results_const_enum;

enum mipc_nw_scan_status_const_enum {
    mipc_nw_scan_status_const_NONE = 0,
    /* NW_SCAN_STATUS */
    MIPC_NW_SCAN_STATUS_PARTIAL                             = 0,
    MIPC_NW_SCAN_STATUS_COMPLETE                            = 1,
};
typedef enum mipc_nw_scan_status_const_enum mipc_nw_scan_status_const_enum;

enum mipc_nw_plmn_status_const_enum {
    mipc_nw_plmn_status_const_NONE = 0,
    /* NW_PLMN_STATUS */
    MIPC_NW_PLMN_STATUS_INVALID                             = 0,
    MIPC_NW_PLMN_STATUS_AVAILABLE                           = 1,
    MIPC_NW_PLMN_STATUS_CURRENT                             = 2,
    MIPC_NW_PLMN_STATUS_FORBIDDEN                           = 3,
};
typedef enum mipc_nw_plmn_status_const_enum mipc_nw_plmn_status_const_enum;

enum mipc_nw_register_status_const_enum {
    mipc_nw_register_status_const_NONE = 0,
    /* NW_REGISTER_STATUS */
    MIPC_NW_REGISTER_STATUS_NOT_REGISTERED                  = 0,
    MIPC_NW_REGISTER_STATUS_REGISTERED                      = 1,
};
typedef enum mipc_nw_register_status_const_enum mipc_nw_register_status_const_enum;

enum mipc_nw_srv_cell_status_const_enum {
    mipc_nw_srv_cell_status_const_NONE = 0,
    /* NW_SRV_CELL_STATUS */
    /*  */
    MIPC_NW_SRV_CELL_STATUS_NONE                            = 0,
    /* primary serving cell */
    MIPC_NW_SRV_CELL_STATUS_PRIMARY                         = 1,
    /* secondary serving cell */
    MIPC_NW_SRV_CELL_STATUS_SECONDARY                       = 2,
};
typedef enum mipc_nw_srv_cell_status_const_enum mipc_nw_srv_cell_status_const_enum;

enum mipc_nw_bw_check_const_enum {
    mipc_nw_bw_check_const_NONE = 0,
    /* NW_BW_CHECK */
    /* disable bandwidth check */
    MIPC_NW_BW_CHECK_DISABLE                                = 0,
    /* enable bandwidth check */
    MIPC_NW_BW_CHECK_ENABLE                                 = 1,
};
typedef enum mipc_nw_bw_check_const_enum mipc_nw_bw_check_const_enum;

enum mipc_nw_5guc_state_const_enum {
    mipc_nw_5guc_state_const_NONE = 0,
    /* NW_5GUC_STATE */
    /* don't display 5G UC */
    MIPC_NW_5GUC_STATE_NOT_DISPLAY                          = 0,
    /* display 5G UC */
    MIPC_NW_5GUC_STATE_DISPLAY                              = 1,
};
typedef enum mipc_nw_5guc_state_const_enum mipc_nw_5guc_state_const_enum;

enum mipc_nw_uc_band_const_enum {
    mipc_nw_uc_band_const_NONE = 0,
    /* NW_UC_BAND */
    /* not on UC band */
    MIPC_NW_UC_BAND_NOT_ON                                  = 0,
    /* on UC band */
    MIPC_NW_UC_BAND_ON                                      = 1,
};
typedef enum mipc_nw_uc_band_const_enum mipc_nw_uc_band_const_enum;

enum mipc_nw_edrx_access_technology_enum_const_enum {
    mipc_nw_edrx_access_technology_enum_const_NONE = 0,
    /* NW_EDRX_ACCESS_TECHNOLOGY_ENUM */
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_NOT_USED            = 0,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_EC_GSM_IOT          = 1,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_GSM             = 2,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_UTRAN           = 3,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_EUTRAN_WB       = 4,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_EUTRAN_NB       = 5,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_SATELLITE_EUTRAN_NB = 6,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_SATELLITE_EUTRAN_WB = 7,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_NGRAN           = 8,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_SATELLITE_NGRAN = 9,
    /*  */
    MIPC_NW_EDRX_ACCESS_TECHNOLOGY_ENUM_ACT_INVALID         = 0xFF,
};
typedef enum mipc_nw_edrx_access_technology_enum_const_enum mipc_nw_edrx_access_technology_enum_const_enum;

enum mipc_nw_psm_mode_enum_const_enum {
    mipc_nw_psm_mode_enum_const_NONE = 0,
    /* NW_PSM_MODE_ENUM */
    /*  */
    MIPC_NW_PSM_MODE_ENUM_POWER_SAVING_MODE_DISABLE         = 0,
    /*  */
    MIPC_NW_PSM_MODE_ENUM_POWER_SAVING_MODE_ENABLE          = 1,
    /*  */
    MIPC_NW_PSM_MODE_ENUM_POWER_SAVING_MODE_DISABLE_AND_DISCARD_PARAM = 2,
};
typedef enum mipc_nw_psm_mode_enum_const_enum mipc_nw_psm_mode_enum_const_enum;

enum mipc_n3_power_state_const_enum {
    mipc_n3_power_state_const_NONE = 0,
    /* N3_POWER_STATE */
    MIPC_N3_POWER_STATE_DISABLE                             = 0,
    MIPC_N3_POWER_STATE_ENABLE                              = 1,
};
typedef enum mipc_n3_power_state_const_enum mipc_n3_power_state_const_enum;

enum mipc_n3_reg_state_const_enum {
    mipc_n3_reg_state_const_NONE = 0,
    /* N3_REG_STATE */
    MIPC_N3_REG_STATE_NOT_REGISTERED                        = 0,
    MIPC_N3_REG_STATE_REGISTERED                            = 1,
    MIPC_N3_REG_STATE_SEARCHING                             = 2,
    MIPC_N3_REG_STATE_REG_DENIED                            = 3,
    MIPC_N3_REG_STATE_UNKNOWN                               = 4,
    MIPC_N3_REG_STATE_EMERGENCY_ONLY                        = 5,
};
typedef enum mipc_n3_reg_state_const_enum mipc_n3_reg_state_const_enum;

enum mipc_n3_device_type_const_enum {
    mipc_n3_device_type_const_NONE = 0,
    /* N3_DEVICE_TYPE */
    /*  */
    MIPC_N3_DEVICE_TYPE_NONE                                = 0,
    /* Wireless LAN */
    MIPC_N3_DEVICE_TYPE_WLAN                                = 1,
    /* Internet Data of SIM1 */
    MIPC_N3_DEVICE_TYPE_DATA_1                              = 2,
    /* Internet Data of SIM2 */
    MIPC_N3_DEVICE_TYPE_DATA_2                              = 3,
    /* Internet Data of SIM3 */
    MIPC_N3_DEVICE_TYPE_DATA_3                              = 4,
    /* Internet Data of SIM4 */
    MIPC_N3_DEVICE_TYPE_DATA_4                              = 5,
    /* Bluetooth */
    MIPC_N3_DEVICE_TYPE_BLUETOOTH                           = 6,
    /* Wireline */
    MIPC_N3_DEVICE_TYPE_WIRE                                = 7,
    /*  */
    MIPC_N3_DEVICE_TYPE_UNKNOWN                             = 8,
};
typedef enum mipc_n3_device_type_const_enum mipc_n3_device_type_const_enum;

enum mipc_nw_basement_status_const_enum {
    mipc_nw_basement_status_const_NONE = 0,
    /* NW_BASEMENT_STATUS */
    /* enter basement */
    MIPC_NW_BASEMENT_STATUS_ENTER                           = 0,
    /* leave basement */
    MIPC_NW_BASEMENT_STATUS_LEAVE                           = 1,
    /* unknown status */
    MIPC_NW_BASEMENT_STATUS_UNKNOWN                         = 2,
};
typedef enum mipc_nw_basement_status_const_enum mipc_nw_basement_status_const_enum;

enum mipc_nw_cag_info_format_const_enum {
    mipc_nw_cag_info_format_const_NONE = 0,
    /* NW_CAG_INFO_FORMAT */
    /*  */
    MIPC_NW_CAG_INFO_FORMAT_ALPHANUMERIC                    = 0,
    /*  */
    MIPC_NW_CAG_INFO_FORMAT_NUMERIC                         = 1,
};
typedef enum mipc_nw_cag_info_format_const_enum mipc_nw_cag_info_format_const_enum;

enum mipc_nw_cag_mode_const_enum {
    mipc_nw_cag_mode_const_NONE = 0,
    /* NW_CAG_MODE */
    /* Automatic CAG selection mode */
    MIPC_NW_CAG_MODE_AUTOMATIC                              = 0,
    /* Manual CAG selection mode */
    MIPC_NW_CAG_MODE_MANUAL                                 = 1,
};
typedef enum mipc_nw_cag_mode_const_enum mipc_nw_cag_mode_const_enum;

enum mipc_nw_cellular_id_type_const_enum {
    mipc_nw_cellular_id_type_const_NONE = 0,
    /* NW_CELLULAR_ID_TYPE */
    MIPC_NW_CELLULAR_ID_TYPE_ID_TYPE_INVALID                = 0,
    MIPC_NW_CELLULAR_ID_TYPE_ID_TYPE_IMSI                   = 1,
    MIPC_NW_CELLULAR_ID_TYPE_ID_TYPE_IMEI                   = 2,
    MIPC_NW_CELLULAR_ID_TYPE_ID_TYPE_UNCIPHERED_SUCI        = 3,
};
typedef enum mipc_nw_cellular_id_type_const_enum mipc_nw_cellular_id_type_const_enum;

enum mipc_nw_nas_msg_type_const_enum {
    mipc_nw_nas_msg_type_const_NONE = 0,
    /* NW_NAS_MSG_TYPE */
    MIPC_NW_NAS_MSG_TYPE_NAS_MSG_TYPE_INVALID               = 0,
    MIPC_NW_NAS_MSG_TYPE_ATTACH_REQUEST                     = 1,
    MIPC_NW_NAS_MSG_TYPE_IDENTITY_RESPONSE                  = 2,
    MIPC_NW_NAS_MSG_TYPE_DETACH_REQUEST                     = 3,
    MIPC_NW_NAS_MSG_TYPE_TRACKING_AREA_UPDATE_REQUEST       = 4,
    MIPC_NW_NAS_MSG_TYPE_LOCATION_UPDATE_REQUEST            = 5,
    MIPC_NW_NAS_MSG_TYPE_AUTHENTICATION_AND_CIPHERING_RESPONSE = 6,
    MIPC_NW_NAS_MSG_TYPE_REGISTRATION_REQUEST               = 7,
    MIPC_NW_NAS_MSG_TYPE_DEREGISTRATION_REQUEST             = 8,
    MIPC_NW_NAS_MSG_TYPE_CM_REESTABLISHMENT_REQUEST         = 9,
    MIPC_NW_NAS_MSG_TYPE_CM_SERVICE_REQUEST                 = 10,
    MIPC_NW_NAS_MSG_TYPE_IMSI_DETACH_INDICATION             = 11,
};
typedef enum mipc_nw_nas_msg_type_const_enum mipc_nw_nas_msg_type_const_enum;

enum mipc_nw_config_result_const_enum {
    mipc_nw_config_result_const_NONE = 0,
    /* NW_CONFIG_RESULT */
    MIPC_NW_CONFIG_RESULT_SUCCESS                           = 0,
    MIPC_NW_CONFIG_RESULT_BUSY                              = 1,
    MIPC_NW_CONFIG_RESULT_FAILURE                           = 2,
    MIPC_NW_CONFIG_RESULT_NOT_SUPPORT                       = 3,
};
typedef enum mipc_nw_config_result_const_enum mipc_nw_config_result_const_enum;

enum mipc_nw_connection_event_const_enum {
    mipc_nw_connection_event_const_NONE = 0,
    /* NW_CONNECTION_EVENT */
    MIPC_NW_CONNECTION_EVENT_CS_SIGNALING_GSM               = 0,
    MIPC_NW_CONNECTION_EVENT_PS_SIGNALING_GPRS              = 1,
    MIPC_NW_CONNECTION_EVENT_CS_SIGNALING_UMTS              = 2,
    MIPC_NW_CONNECTION_EVENT_PS_SIGNALING_UMTS              = 3,
    MIPC_NW_CONNECTION_EVENT_NAS_SIGNALING_LTE              = 4,
    MIPC_NW_CONNECTION_EVENT_AS_SIGNALING_LTE               = 5,
    MIPC_NW_CONNECTION_EVENT_VOLTE_SIP                      = 6,
    MIPC_NW_CONNECTION_EVENT_VOLTE_SIP_SOS                  = 7,
    MIPC_NW_CONNECTION_EVENT_VOLTE_RTP                      = 8,
    MIPC_NW_CONNECTION_EVENT_VOLTE_RTP_SOS                  = 9,
    MIPC_NW_CONNECTION_EVENT_NAS_SIGNALING_5G               = 10,
    MIPC_NW_CONNECTION_EVENT_AS_SIGNALING_5G                = 11,
    MIPC_NW_CONNECTION_EVENT_VONR_SIP                       = 12,
    MIPC_NW_CONNECTION_EVENT_VONR_SIP_SOS                   = 13,
    MIPC_NW_CONNECTION_EVENT_VONR_RTP                       = 14,
    MIPC_NW_CONNECTION_EVENT_VONR_RTP_SOS                   = 15,
    MIPC_NW_CONNECTION_EVENT_INVALID_CONNECTION_EVENT       = 255,
};
typedef enum mipc_nw_connection_event_const_enum mipc_nw_connection_event_const_enum;

enum mipc_nw_integrity_algo_const_enum {
    mipc_nw_integrity_algo_const_NONE = 0,
    /* NW_INTEGRITY_ALGO */
    MIPC_NW_INTEGRITY_ALGO_GSM_IA0                          = 14,
    MIPC_NW_INTEGRITY_ALGO_GSM_IA1                          = 15,
    MIPC_NW_INTEGRITY_ALGO_GSM_IA2                          = 16,
    MIPC_NW_INTEGRITY_ALGO_GSM_IA3                          = 17,
    MIPC_NW_INTEGRITY_ALGO_GSM_IA4                          = 18,
    MIPC_NW_INTEGRITY_ALGO_GSM_IA5                          = 19,
    MIPC_NW_INTEGRITY_ALGO_UMTS_IA0                         = 29,
    MIPC_NW_INTEGRITY_ALGO_UMTS_IA1                         = 30,
    MIPC_NW_INTEGRITY_ALGO_UMTS_IA2                         = 31,
    MIPC_NW_INTEGRITY_ALGO_LTE_IA0                          = 41,
    MIPC_NW_INTEGRITY_ALGO_LTE_IA1                          = 42,
    MIPC_NW_INTEGRITY_ALGO_LTE_IA2                          = 43,
    MIPC_NW_INTEGRITY_ALGO_LTE_IA3                          = 44,
    MIPC_NW_INTEGRITY_ALGO_NR_IA0                           = 55,
    MIPC_NW_INTEGRITY_ALGO_NR_IA1                           = 56,
    MIPC_NW_INTEGRITY_ALGO_NR_IA2                           = 57,
    MIPC_NW_INTEGRITY_ALGO_NR_IA3                           = 58,
    MIPC_NW_INTEGRITY_ALGO_SIP_NO_IPSEC_INTEGRITY_ALGO      = 66,
    MIPC_NW_INTEGRITY_ALGO_SIP_NULL_INTEGRITY               = 68,
    MIPC_NW_INTEGRITY_ALGO_SIP_AES_GMAC                     = 70,
    MIPC_NW_INTEGRITY_ALGO_SIP_HMAC_SHA1_96                 = 74,
    MIPC_NW_INTEGRITY_ALGO_SIP_HMAC_MD5_96                  = 75,
    MIPC_NW_INTEGRITY_ALGO_RTP                              = 85,
    MIPC_NW_INTEGRITY_ALGO_SRTP_NULL                        = 86,
    MIPC_NW_INTEGRITY_ALGO_SRTP_HMAC_SHA1                   = 89,
    MIPC_NW_INTEGRITY_ALGO_INVALID_INTEGRITY_ALGO           = 255,
};
typedef enum mipc_nw_integrity_algo_const_enum mipc_nw_integrity_algo_const_enum;

enum mipc_nw_encryption_algo_const_enum {
    mipc_nw_encryption_algo_const_NONE = 0,
    /* NW_ENCRYPTION_ALGO */
    MIPC_NW_ENCRYPTION_ALGO_GSM_A5_0                        = 0,
    MIPC_NW_ENCRYPTION_ALGO_GSM_A5_1                        = 1,
    MIPC_NW_ENCRYPTION_ALGO_GSM_A5_2                        = 2,
    MIPC_NW_ENCRYPTION_ALGO_GSM_A5_3                        = 3,
    MIPC_NW_ENCRYPTION_ALGO_GSM_A5_4                        = 4,
    MIPC_NW_ENCRYPTION_ALGO_GSM_EA0                         = 14,
    MIPC_NW_ENCRYPTION_ALGO_GSM_EA1                         = 15,
    MIPC_NW_ENCRYPTION_ALGO_GSM_EA2                         = 16,
    MIPC_NW_ENCRYPTION_ALGO_GSM_EA3                         = 17,
    MIPC_NW_ENCRYPTION_ALGO_GSM_EA4                         = 18,
    MIPC_NW_ENCRYPTION_ALGO_GSM_EA5                         = 19,
    MIPC_NW_ENCRYPTION_ALGO_UMTS_EA0                        = 29,
    MIPC_NW_ENCRYPTION_ALGO_UMTS_EA1                        = 30,
    MIPC_NW_ENCRYPTION_ALGO_UMTS_EA2                        = 31,
    MIPC_NW_ENCRYPTION_ALGO_LTE_EA0                         = 41,
    MIPC_NW_ENCRYPTION_ALGO_LTE_EA1                         = 42,
    MIPC_NW_ENCRYPTION_ALGO_LTE_EA2                         = 43,
    MIPC_NW_ENCRYPTION_ALGO_LTE_EA3                         = 44,
    MIPC_NW_ENCRYPTION_ALGO_NR_EA0                          = 55,
    MIPC_NW_ENCRYPTION_ALGO_NR_EA1                          = 56,
    MIPC_NW_ENCRYPTION_ALGO_NR_EA2                          = 57,
    MIPC_NW_ENCRYPTION_ALGO_NR_EA3                          = 58,
    MIPC_NW_ENCRYPTION_ALGO_SIP_NO_IPSEC_ENCRYPTION_CONFIG  = 66,
    MIPC_NW_ENCRYPTION_ALGO_IMS_NULL                        = 67,
    MIPC_NW_ENCRYPTION_ALGO_SIP_NULL_ENCRYPTION             = 68,
    MIPC_NW_ENCRYPTION_ALGO_SIP_AES_GCM                     = 69,
    MIPC_NW_ENCRYPTION_ALGO_SIP_AES_GMAC                    = 70,
    MIPC_NW_ENCRYPTION_ALGO_SIP_AES_CBC                     = 71,
    MIPC_NW_ENCRYPTION_ALGO_SIP_DES_EDE3_CBC                = 72,
    MIPC_NW_ENCRYPTION_ALGO_AES_EDE3_CBC                    = 73,
    MIPC_NW_ENCRYPTION_ALGO_HMAC_SHA1_96                    = 74,
    MIPC_NW_ENCRYPTION_ALGO_HMAC_MD5_96                     = 75,
    MIPC_NW_ENCRYPTION_ALGO_RTP                             = 85,
    MIPC_NW_ENCRYPTION_ALGO_SRTP_NULL                       = 86,
    MIPC_NW_ENCRYPTION_ALGO_SRTP_AES_COUNTER                = 87,
    MIPC_NW_ENCRYPTION_ALGO_SRTP_AES_F8                     = 88,
    MIPC_NW_ENCRYPTION_ALGO_SRTP_HMAC_SHA1                  = 89,
    MIPC_NW_ENCRYPTION_ALGO_ENCR_AES_GCM_16                 = 99,
    MIPC_NW_ENCRYPTION_ALGO_ENCR_AES_CBC                    = 100,
    MIPC_NW_ENCRYPTION_ALGO_AUTH_HMAC_SHA2_256_128          = 101,
    MIPC_NW_ENCRYPTION_ALGO_UNKNOWN_ENCRYPTION_ALGO         = 113,
    MIPC_NW_ENCRYPTION_ALGO_OTHER_ENCRYPTION_ALGO           = 114,
    MIPC_NW_ENCRYPTION_ALGO_OPYX_ENCRYPTION_ALGO            = 124,
    MIPC_NW_ENCRYPTION_ALGO_INVALID_ENCRYPTION_ALGO         = 255,
};
typedef enum mipc_nw_encryption_algo_const_enum mipc_nw_encryption_algo_const_enum;

enum mipc_release_preference_uai_report_status_const_enum {
    mipc_release_preference_uai_report_status_const_NONE = 0,
    /* RELEASE_PREFERENCE_UAI_REPORT_STATUS */
    /* NW configured */
    MIPC_RELEASE_PREFERENCE_UAI_REPORT_STATUS_NW_CONFIGURED = 1,
    /* NW not configured */
    MIPC_RELEASE_PREFERENCE_UAI_REPORT_STATUS_NW_NOT_CONFIGURED = 2,
    /* Report sent */
    MIPC_RELEASE_PREFERENCE_UAI_REPORT_STATUS_REPORT_SENT   = 3,
    /* Report Not Sent while Prohibit timer is running */
    MIPC_RELEASE_PREFERENCE_UAI_REPORT_STATUS_REPORT_NOT_SENT_TIMER = 4,
    /* Report Not Sent due to Same as previous info */
    MIPC_RELEASE_PREFERENCE_UAI_REPORT_STATUS_REPORT_NOT_SENT_SAME = 5,
    /* Report Not Sent with other cause */
    MIPC_RELEASE_PREFERENCE_UAI_REPORT_STATUS_REPORT_NOT_SENT_OTHER = 6,
};
typedef enum mipc_release_preference_uai_report_status_const_enum mipc_release_preference_uai_report_status_const_enum;

enum mipc_ue_preferred_rrc_state_const_enum {
    mipc_ue_preferred_rrc_state_const_NONE = 0,
    /* UE_PREFERRED_RRC_STATE */
    /* Idle */
    MIPC_UE_PREFERRED_RRC_STATE_IDLE                        = 0,
    /* inactive */
    MIPC_UE_PREFERRED_RRC_STATE_INACTIVE                    = 1,
    /* connected */
    MIPC_UE_PREFERRED_RRC_STATE_CONNECTED                   = 2,
    /* out Of Connected */
    MIPC_UE_PREFERRED_RRC_STATE_OUT_OF_CONNECTED            = 3,
};
typedef enum mipc_ue_preferred_rrc_state_const_enum mipc_ue_preferred_rrc_state_const_enum;

enum mipc_nrrc_preference_report_status_const_enum {
    mipc_nrrc_preference_report_status_const_NONE = 0,
    /* NRRC_PREFERENCE_REPORT_STATUS */
    /* Invalid */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_INVALID              = 0,
    /* NW configured */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_NW_CONFIGURED        = 1,
    /* NW not configured */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_NW_NOT_CONFIGURED    = 2,
    /* Report sent */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_REPORT_SENT          = 3,
    /* Report Not Sent while Prohibit timer is running */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_REPORT_NOT_SENT_TIMER = 4,
    /* Report Not Sent due to Same as previous info */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_REPORT_NOT_SENT_SAME = 5,
    /* Report Not Sent with other cause */
    MIPC_NRRC_PREFERENCE_REPORT_STATUS_REPORT_NOT_SENT_OTHER = 6,
};
typedef enum mipc_nrrc_preference_report_status_const_enum mipc_nrrc_preference_report_status_const_enum;

enum mipc_overheating_uai_report_status_const_enum {
    mipc_overheating_uai_report_status_const_NONE = 0,
    /* OVERHEATING_UAI_REPORT_STATUS */
    /* NW configured */
    MIPC_OVERHEATING_UAI_REPORT_STATUS_NW_CONFIGURED        = 1,
    /* NW not configured */
    MIPC_OVERHEATING_UAI_REPORT_STATUS_NW_NOT_CONFIGURED    = 2,
    /* Report sent */
    MIPC_OVERHEATING_UAI_REPORT_STATUS_REPORT_SENT          = 3,
    /* Report Not Sent while Prohibit timer is running */
    MIPC_OVERHEATING_UAI_REPORT_STATUS_REPORT_NOT_SENT_TIMER = 4,
    /* Report Not Sent due to Same as previous info */
    MIPC_OVERHEATING_UAI_REPORT_STATUS_REPORT_NOT_SENT_SAME = 5,
    /* Report Not Sent with other cause */
    MIPC_OVERHEATING_UAI_REPORT_STATUS_REPORT_NOT_SENT_OTHER = 6,
};
typedef enum mipc_overheating_uai_report_status_const_enum mipc_overheating_uai_report_status_const_enum;

enum mipc_nrrc_timer_name_const_enum {
    mipc_nrrc_timer_name_const_NONE = 0,
    /* NRRC_TIMER_NAME */
    /* T300 */
    MIPC_NRRC_TIMER_NAME_T300                               = 0,
    /* T301 */
    MIPC_NRRC_TIMER_NAME_T301                               = 1,
    /* T302 */
    MIPC_NRRC_TIMER_NAME_T302                               = 2,
    /* T304 */
    MIPC_NRRC_TIMER_NAME_T304                               = 3,
    /* T311 */
    MIPC_NRRC_TIMER_NAME_T311                               = 4,
    /* T319 */
    MIPC_NRRC_TIMER_NAME_T319                               = 5,
    /* T320 */
    MIPC_NRRC_TIMER_NAME_T320                               = 6,
    /* T321 */
    MIPC_NRRC_TIMER_NAME_T321                               = 7,
    /* T325 */
    MIPC_NRRC_TIMER_NAME_T325                               = 8,
    /* T380 */
    MIPC_NRRC_TIMER_NAME_T380                               = 9,
    /* T390 */
    MIPC_NRRC_TIMER_NAME_T390                               = 10,
    /* T304_SCG */
    MIPC_NRRC_TIMER_NAME_T304_SCG                           = 11,
    /* T346c_MCG */
    MIPC_NRRC_TIMER_NAME_T346C_MCG                          = 12,
    /* T346c_SCG */
    MIPC_NRRC_TIMER_NAME_T346C_SCG                          = 13,
    /* T346d_MCG */
    MIPC_NRRC_TIMER_NAME_T346D_MCG                          = 14,
    /* T346d_SCG */
    MIPC_NRRC_TIMER_NAME_T346D_SCG                          = 15,
    /* T346f */
    MIPC_NRRC_TIMER_NAME_T346F                              = 16,
    /* T342 */
    MIPC_NRRC_TIMER_NAME_T342                               = 17,
    /* T345 */
    MIPC_NRRC_TIMER_NAME_T345                               = 18,
    /* T346a_MCG */
    MIPC_NRRC_TIMER_NAME_T346A_MCG                          = 19,
    /* T346a_SCG */
    MIPC_NRRC_TIMER_NAME_T346A_SCG                          = 20,
    /* T346b_MCG */
    MIPC_NRRC_TIMER_NAME_T346B_MCG                          = 21,
    /* T346b_SCG */
    MIPC_NRRC_TIMER_NAME_T346B_SCG                          = 22,
    /* T346e_MCG */
    MIPC_NRRC_TIMER_NAME_T346E_MCG                          = 23,
    /* T346e_SCG */
    MIPC_NRRC_TIMER_NAME_T346E_SCG                          = 24,
};
typedef enum mipc_nrrc_timer_name_const_enum mipc_nrrc_timer_name_const_enum;

enum mipc_nrrc_timer_status_const_enum {
    mipc_nrrc_timer_status_const_NONE = 0,
    /* NRRC_TIMER_STATUS */
    /* Timer is Started */
    MIPC_NRRC_TIMER_STATUS_STARTED                          = 0,
    /* Timer is Stopped */
    MIPC_NRRC_TIMER_STATUS_STOPPED                          = 1,
    /* Timer is Expired */
    MIPC_NRRC_TIMER_STATUS_EXPIRED                          = 2,
    /* Timer is Configured */
    MIPC_NRRC_TIMER_STATUS_CONFIGURED                       = 3,
};
typedef enum mipc_nrrc_timer_status_const_enum mipc_nrrc_timer_status_const_enum;

enum mipc_sim_status_const_enum {
    mipc_sim_status_const_NONE = 0,
    /* SIM_STATUS */
    /* SIM is not inserted */
    MIPC_SIM_STATUS_NOT_INSERT                              = 0,
    /* CPIN responded BUSY */
    MIPC_SIM_STATUS_BUSY                                    = 1,
    /* CPIN responded READY */
    MIPC_SIM_STATUS_READY                                   = 2,
    /* MT is waiting for SIM PIN to be provided */
    MIPC_SIM_STATUS_SIM_PIN                                 = 3,
    /* MT is waiting for SIM PUK to be provided */
    MIPC_SIM_STATUS_SIM_PUK                                 = 4,
    /* MT is waiting for phone-to-SIM-card password to be provided */
    MIPC_SIM_STATUS_PH_SIM_PIN                              = 5,
    /* MT is waiting for phone-to-very-first-SIM-card password to be provided */
    MIPC_SIM_STATUS_PH_FSIM_PIN                             = 6,
    /* MT is waiting for phone-to-very-first-SIM-card unblocking password to be provided */
    MIPC_SIM_STATUS_PH_FSIM_PUK                             = 7,
    /* MT is waiting for SIM PIN2 to be provided */
    MIPC_SIM_STATUS_SIM_PIN2                                = 8,
    /* MT is waiting for SIM PUK2 to be provided */
    MIPC_SIM_STATUS_SIM_PUK2                                = 9,
    /* MT is waiting for network personalization password to be provided */
    MIPC_SIM_STATUS_PH_NET_PIN                              = 10,
    /* MT is waiting for network personalization unblocking password to be provided */
    MIPC_SIM_STATUS_PH_NET_PUK                              = 11,
    /* MT is waiting for network subset personalization password to be provided */
    MIPC_SIM_STATUS_PH_NETSUB_PIN                           = 12,
    /* MT is waiting for network subset personalization unblocking password to be provided */
    MIPC_SIM_STATUS_PH_NETSUB_PUK                           = 13,
    /* MT is waiting for service provider personalization password to be provided */
    MIPC_SIM_STATUS_PH_SP_PIN                               = 14,
    /* MT is waiting for service provider personalization unblocking password to be provided */
    MIPC_SIM_STATUS_PH_SP_PUK                               = 15,
    /* MT is waiting for corporate personalization password to be provided */
    MIPC_SIM_STATUS_PH_CORP_PIN                             = 16,
    /* MT is waiting for corporate personalization unblocking password to be provided */
    MIPC_SIM_STATUS_PH_CORP_PUK                             = 17,
    /* CARD is rebooted */
    MIPC_SIM_STATUS_CARD_REBOOT                             = 18,
    /* CARD is restricted */
    MIPC_SIM_STATUS_CARD_RESTRICTED                         = 19,
    /* eUICC without profile */
    MIPC_SIM_STATUS_EMPT_EUICC                              = 20,
    /* SIM initialization is done after CPIN is ready */
    MIPC_SIM_STATUS_COMPLETE_READY                          = 21,
};
typedef enum mipc_sim_status_const_enum mipc_sim_status_const_enum;

enum mipc_sim_state_const_enum {
    mipc_sim_state_const_NONE = 0,
    /* SIM_STATE */
    MIPC_SIM_STATE_UNKNOWN                                  = 0,
    MIPC_SIM_STATE_OFFEMPTY                                 = 1,
    MIPC_SIM_STATE_OFF                                      = 2,
    MIPC_SIM_STATE_EMPTY                                    = 3,
    MIPC_SIM_STATE_NOTREADY                                 = 4,
    MIPC_SIM_STATE_ACTIVE                                   = 5,
    MIPC_SIM_STATE_ERROR                                    = 6,
    MIPC_SIM_STATE_ACTIVE_ESIM                              = 7,
    MIPC_SIM_STATE_ACTIVE_ESIM_NOPROFILE                    = 8,
};
typedef enum mipc_sim_state_const_enum mipc_sim_state_const_enum;

enum mipc_sim_card_present_state_const_enum {
    mipc_sim_card_present_state_const_NONE = 0,
    /* SIM_CARD_PRESENT_STATE */
    MIPC_SIM_CARD_PRESENT_STATE_ABSENT                      = 0,
    MIPC_SIM_CARD_PRESENT_STATE_PRESENT                     = 1,
    MIPC_SIM_CARD_PRESENT_STATE_ERROR                       = 2,
    MIPC_SIM_CARD_PRESENT_STATE_RESTRICTED                  = 3,
};
typedef enum mipc_sim_card_present_state_const_enum mipc_sim_card_present_state_const_enum;

enum mipc_sim_sub_state_const_enum {
    mipc_sim_sub_state_const_NONE = 0,
    /* SIM_SUB_STATE */
    MIPC_SIM_SUB_STATE_UNKNOWN                              = 0,
    MIPC_SIM_SUB_STATE_PLUG_IN                              = 1,
    MIPC_SIM_SUB_STATE_PLUG_OUT                             = 2,
    MIPC_SIM_SUB_STATE_RECOVERY_START                       = 3,
    MIPC_SIM_SUB_STATE_RECOVERY_EN                          = 4,
};
typedef enum mipc_sim_sub_state_const_enum mipc_sim_sub_state_const_enum;

enum mipc_sim_pin_type_const_enum {
    mipc_sim_pin_type_const_NONE = 0,
    /* SIM_PIN_TYPE */
    MIPC_SIM_PIN_TYPE_NONE                                  = 0,
    MIPC_SIM_PIN_TYPE_PIN1                                  = 1,
    MIPC_SIM_PIN_TYPE_PIN2                                  = 2,
    MIPC_SIM_PIN_TYPE_PUK1                                  = 3,
    MIPC_SIM_PIN_TYPE_PUK2                                  = 4,
    MIPC_SIM_PIN_TYPE_NW                                    = 5,
    MIPC_SIM_PIN_TYPE_SUB_NW                                = 6,
    MIPC_SIM_PIN_TYPE_SP                                    = 7,
    MIPC_SIM_PIN_TYPE_CORP                                  = 8,
    MIPC_SIM_PIN_TYPE_SIM                                   = 9,
    MIPC_SIM_PIN_TYPE_NS_SP                                 = 10,
    MIPC_SIM_PIN_TYPE_SIM_C                                 = 11,
    MIPC_SIM_PIN_TYPE_PUK_N                                 = 12,
    MIPC_SIM_PIN_TYPE_PUK_NS                                = 13,
    MIPC_SIM_PIN_TYPE_PUK_SP                                = 14,
    MIPC_SIM_PIN_TYPE_PUK_C                                 = 15,
    MIPC_SIM_PIN_TYPE_PUK_SIM                               = 16,
    MIPC_SIM_PIN_TYPE_PUK_NS_SP                             = 17,
    MIPC_SIM_PIN_TYPE_PUK_SIM_C                             = 18,
};
typedef enum mipc_sim_pin_type_const_enum mipc_sim_pin_type_const_enum;

enum mipc_sim_pin_protection_const_enum {
    mipc_sim_pin_protection_const_NONE = 0,
    /* SIM_PIN_PROTECTION */
    MIPC_SIM_PIN_PROTECTION_DISABLE                         = 0,
    MIPC_SIM_PIN_PROTECTION_ENABLE                          = 1,
};
typedef enum mipc_sim_pin_protection_const_enum mipc_sim_pin_protection_const_enum;

enum mipc_sim_pin_mode_const_enum {
    mipc_sim_pin_mode_const_NONE = 0,
    /* SIM_PIN_MODE */
    MIPC_SIM_PIN_MODE_NOT_SUPPORT                           = 0,
    MIPC_SIM_PIN_MODE_ENABLE                                = 1,
    MIPC_SIM_PIN_MODE_DISABLE                               = 2,
};
typedef enum mipc_sim_pin_mode_const_enum mipc_sim_pin_mode_const_enum;

enum mipc_sim_pin_format_const_enum {
    mipc_sim_pin_format_const_NONE = 0,
    /* SIM_PIN_FORMAT */
    MIPC_SIM_PIN_FORMAT_UNKNOWN                             = 0,
    MIPC_SIM_PIN_FORMAT_NUMERIC                             = 1,
    MIPC_SIM_PIN_FORMAT_ALPHA_NUMERIC                       = 2,
};
typedef enum mipc_sim_pin_format_const_enum mipc_sim_pin_format_const_enum;

enum mipc_sim_pin_state_const_enum {
    mipc_sim_pin_state_const_NONE = 0,
    /* SIM_PIN_STATE */
    MIPC_SIM_PIN_STATE_UNLOCKED                             = 0,
    MIPC_SIM_PIN_STATE_LOCKED                               = 1,
};
typedef enum mipc_sim_pin_state_const_enum mipc_sim_pin_state_const_enum;

enum mipc_sim_pin_operation_const_enum {
    mipc_sim_pin_operation_const_NONE = 0,
    /* SIM_PIN_OPERATION */
    MIPC_SIM_PIN_OPERATION_ENTER                            = 0,
    MIPC_SIM_PIN_OPERATION_ENABLE                           = 1,
    MIPC_SIM_PIN_OPERATION_DISABLE                          = 2,
    MIPC_SIM_PIN_OPERATION_CHANGE                           = 3,
};
typedef enum mipc_sim_pin_operation_const_enum mipc_sim_pin_operation_const_enum;

enum mipc_sim_pass_through_mode_const_enum {
    mipc_sim_pass_through_mode_const_NONE = 0,
    /* SIM_PASS_THROUGH_MODE */
    MIPC_SIM_PASS_THROUGH_MODE_DISABLE                      = 0,
    MIPC_SIM_PASS_THROUGH_MODE_ENABLE                       = 1,
};
typedef enum mipc_sim_pass_through_mode_const_enum mipc_sim_pass_through_mode_const_enum;

enum mipc_sim_app_type_const_enum {
    mipc_sim_app_type_const_NONE = 0,
    /* SIM_APP_TYPE */
    /* unknown type */
    MIPC_SIM_APP_TYPE_UNKNOWN                               = 0,
    /* legacy SIM directories rooted at the MF */
    MIPC_SIM_APP_TYPE_MF                                    = 1,
    /* legacy SIM directories rooted at the DF_GSM */
    MIPC_SIM_APP_TYPE_MFSIM                                 = 2,
    /* legacy SIM directories rooted at the DF_CDMA */
    MIPC_SIM_APP_TYPE_MFRUIM                                = 3,
    /* USIM application */
    MIPC_SIM_APP_TYPE_USIM                                  = 4,
    /* CSIM application */
    MIPC_SIM_APP_TYPE_CSIM                                  = 5,
    /* ISIM application */
    MIPC_SIM_APP_TYPE_ISIM                                  = 6,
};
typedef enum mipc_sim_app_type_const_enum mipc_sim_app_type_const_enum;

enum mipc_sim_app_type_ex_const_enum {
    mipc_sim_app_type_ex_const_NONE = 0,
    /* SIM_APP_TYPE_EX */
    /* ISIM application */
    MIPC_SIM_APP_TYPE_EX_ISIM                               = 0,
    /* USIM application */
    MIPC_SIM_APP_TYPE_EX_USIM                               = 1,
    /* CSIM application */
    MIPC_SIM_APP_TYPE_EX_CSIM                               = 2,
    /* SIM application */
    MIPC_SIM_APP_TYPE_EX_SIM                                = 3,
    /* UIM application */
    MIPC_SIM_APP_TYPE_EX_UIM                                = 4,
    /* unknown type */
    MIPC_SIM_APP_TYPE_EX_UNKNOWN                            = 255,
};
typedef enum mipc_sim_app_type_ex_const_enum mipc_sim_app_type_ex_const_enum;

enum mipc_sim_access_command_const_enum {
    mipc_sim_access_command_const_NONE = 0,
    /* SIM_ACCESS_COMMAND */
    MIPC_SIM_ACCESS_COMMAND_READ_BINARY                     = 176,
    MIPC_SIM_ACCESS_COMMAND_READ_RECORD                     = 178,
    MIPC_SIM_ACCESS_COMMAND_GET_RESPONSE                    = 192,
    MIPC_SIM_ACCESS_COMMAND_UPDATE_BINARY                   = 214,
    MIPC_SIM_ACCESS_COMMAND_UPDATE_RECORD                   = 220,
    MIPC_SIM_ACCESS_COMMAND_STATUS                          = 242,
    MIPC_SIM_ACCESS_COMMAND_RETRIEVE_DATA                   = 203,
    MIPC_SIM_ACCESS_COMMAND_SET_DATA                        = 219,
};
typedef enum mipc_sim_access_command_const_enum mipc_sim_access_command_const_enum;

enum mipc_sim_file_accessibility_const_enum {
    mipc_sim_file_accessibility_const_NONE = 0,
    /* SIM_FILE_ACCESSIBILITY */
    MIPC_SIM_FILE_ACCESSIBILITY_UNKNOWN                     = 0,
    MIPC_SIM_FILE_ACCESSIBILITY_NOTSHAREABLE                = 1,
    MIPC_SIM_FILE_ACCESSIBILITY_SHAREABLE                   = 2,
};
typedef enum mipc_sim_file_accessibility_const_enum mipc_sim_file_accessibility_const_enum;

enum mipc_sim_file_type_const_enum {
    mipc_sim_file_type_const_NONE = 0,
    /* SIM_FILE_TYPE */
    MIPC_SIM_FILE_TYPE_UNKNOWN                              = 0,
    MIPC_SIM_FILE_TYPE_WORKING_EF                           = 1,
    MIPC_SIM_FILE_TYPE_INTERNAL_EF                          = 2,
    MIPC_SIM_FILE_TYPE_DF_OR_ADF                            = 3,
};
typedef enum mipc_sim_file_type_const_enum mipc_sim_file_type_const_enum;

enum mipc_sim_file_structure_const_enum {
    mipc_sim_file_structure_const_NONE = 0,
    /* SIM_FILE_STRUCTURE */
    MIPC_SIM_FILE_STRUCTURE_UNKNOWN                         = 0,
    MIPC_SIM_FILE_STRUCTURE_TRANSPARENT                     = 1,
    MIPC_SIM_FILE_STRUCTURE_CYCLIC                          = 2,
    MIPC_SIM_FILE_STRUCTURE_LINEAR                          = 3,
    MIPC_SIM_FILE_STRUCTURE_BERTLV                          = 4,
};
typedef enum mipc_sim_file_structure_const_enum mipc_sim_file_structure_const_enum;

enum mipc_pin_state_const_enum {
    mipc_pin_state_const_NONE = 0,
    /* PIN_STATE */
    MIPC_PIN_STATE_UNKNOWN                                  = 0,
    MIPC_PIN_STATE_ENABLED_NOT_VERIFIED                     = 1,
    MIPC_PIN_STATE_ENABLED_VERIFIED                         = 2,
    MIPC_PIN_STATE_DISABLED                                 = 3,
    MIPC_PIN_STATE_ENABLED_BLOCKED                          = 4,
    MIPC_PIN_STATE_ENABLED_PERM_BLOCKED                     = 5,
};
typedef enum mipc_pin_state_const_enum mipc_pin_state_const_enum;

enum mipc_sim_app_state_const_enum {
    mipc_sim_app_state_const_NONE = 0,
    /* SIM_APP_STATE */
    MIPC_SIM_APP_STATE_UNKNOWN                              = 0,
    MIPC_SIM_APP_STATE_DETECTED                             = 1,
    MIPC_SIM_APP_STATE_PIN                                  = 2,
    MIPC_SIM_APP_STATE_PUK                                  = 3,
    MIPC_SIM_APP_STATE_SUBSTATE                             = 4,
    MIPC_SIM_APP_STATE_READY                                = 5,
};
typedef enum mipc_sim_app_state_const_enum mipc_sim_app_state_const_enum;

enum mipc_sim_app_sub_status_const_enum {
    mipc_sim_app_sub_status_const_NONE = 0,
    /* SIM_APP_SUB_STATUS */
    MIPC_SIM_APP_SUB_STATUS_UNKNOWN                         = 0,
    MIPC_SIM_APP_SUB_STATUS_IN_PROGRESS                     = 1,
    MIPC_SIM_APP_SUB_STATUS_READY                           = 2,
    MIPC_SIM_APP_SUB_STATUS_NW                              = 3,
    MIPC_SIM_APP_SUB_STATUS_NW_PUK                          = 4,
    MIPC_SIM_APP_SUB_STATUS_SUB_NW                          = 5,
    MIPC_SIM_APP_SUB_STATUS_SUB_NW_PUK                      = 6,
    MIPC_SIM_APP_SUB_STATUS_CORP                            = 7,
    MIPC_SIM_APP_SUB_STATUS_CORP_PUK                        = 8,
    MIPC_SIM_APP_SUB_STATUS_SP                              = 9,
    MIPC_SIM_APP_SUB_STATUS_SP_PUK                          = 10,
    MIPC_SIM_APP_SUB_STATUS_IMSI                            = 11,
    MIPC_SIM_APP_SUB_STATUS_IMSI_PUK                        = 12,
    MIPC_SIM_APP_SUB_STATUS_LINK_NS_SP                      = 13,
    MIPC_SIM_APP_SUB_STATUS_LINK_NS_SP_PUK                  = 14,
    MIPC_SIM_APP_SUB_STATUS_LINK_SIM_C                      = 15,
    MIPC_SIM_APP_SUB_STATUS_LINK_SIM_C_PUK                  = 16,
};
typedef enum mipc_sim_app_sub_status_const_enum mipc_sim_app_sub_status_const_enum;

enum mipc_sim_cause_const_enum {
    mipc_sim_cause_const_NONE = 0,
    /* SIM_CAUSE */
    MIPC_SIM_CAUSE_CARD_REMOVED                             = 0,
    MIPC_SIM_CAUSE_ACCESS_ERROR                             = 1,
    MIPC_SIM_CAUSE_SIM_REFRESH                              = 2,
    MIPC_SIM_CAUSE_SIM_REFRESH_POWER_OFF                    = 3,
    MIPC_SIM_CAUSE_SIM_PUK1                                 = 4,
    MIPC_SIM_CAUSE_SIM_ACCESS_PROFILE_ON                    = 5,
    MIPC_SIM_CAUSE_SIM_ACCESS_PROFILE_OFF                   = 6,
    MIPC_SIM_CAUSE_DUAIL_DISCONNECTED                       = 7,
    MIPC_SIM_CAUSE_DUAIL_CONNECTED                          = 8,
    MIPC_SIM_CAUSE_SIM_VSIM_ON                              = 9,
    MIPC_SIM_CAUSE_SIM_VSIM_OFF                             = 10,
    MIPC_SIM_CAUSE_SIM_PLUG_OUT                             = 11,
    MIPC_SIM_CAUSE_SIM_PLUG_IN                              = 12,
    MIPC_SIM_CAUSE_SIM_RECOVERY_START                       = 13,
    MIPC_SIM_CAUSE_SIM_RECOVERY_END                         = 14,
    MIPC_SIM_CAUSE_SIM_IMEI_LOCK_FAIL                       = 15,
    MIPC_SIM_CAUSE_SIM_OP09_LOCK_FAIL                       = 16,
    MIPC_SIM_CAUSE_SIM_CARD_TECHNICAL_PROBLEM               = 17,
    MIPC_SIM_CAUSE_SIM_POWER_OFF_BY_L4                      = 18,
    MIPC_SIM_CAUSE_SIM_OP20_IMEI_LOCK                       = 19,
    MIPC_SIM_CAUSE_SIM_PLUG_IN_REPLACED                     = 20,
    MIPC_SIM_CAUSE_SIM_OP01_LOCK                            = 21,
    MIPC_SIM_CAUSE_SIM_OP01_UNLOCK                          = 22,
    MIPC_SIM_CAUSE_SIM_FAST_RECOVERY_START                  = 23,
    MIPC_SIM_CAUSE_SIM_FAST_RECOVERY_SUCCESS                = 24,
    MIPC_SIM_CAUSE_SIM_TRAY_PLUG_OUT                        = 25,
    MIPC_SIM_CAUSE_SIM_PLUG_IN_NO_INIT                      = 26,
    MIPC_SIM_CAUSE_SIM_PRESENCE_DETECTION_FAIL              = 27,
    MIPC_SIM_CAUSE_SIM_STATE_DISABLE                        = 28,
    MIPC_SIM_CAUSE_SIM_STATE_ENABLE                         = 29,
};
typedef enum mipc_sim_cause_const_enum mipc_sim_cause_const_enum;

enum mipc_sim_additional_cause_const_enum {
    mipc_sim_additional_cause_const_NONE = 0,
    /* SIM_ADDITIONAL_CAUSE */
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_NAA_INIT_AND_FULL_FCN = 0,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_FCN          = 1,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_NAA_INIT_AND_FCN = 2,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_NAA_INIT     = 3,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_SIM_RESET    = 4,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_NAA_APP_RESET = 5,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_NAA_SESSION_RESET = 6,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_STEERING_OF_ROAMING = 7,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_STEERING_OF_ROAMING_FOR_IWLAN = 8,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_TYPE_EUICC_PROFILE = 9,
    MIPC_SIM_ADDITIONAL_CAUSE_SIM_REFRESH_END               = 255,
};
typedef enum mipc_sim_additional_cause_const_enum mipc_sim_additional_cause_const_enum;

enum mipc_sim_vsim_type_const_enum {
    mipc_sim_vsim_type_const_NONE = 0,
    /* SIM_VSIM_TYPE */
    MIPC_SIM_VSIM_TYPE_SIM_TYPE_LOCAL_SIM                   = 0,
    MIPC_SIM_VSIM_TYPE_SIM_TYPE_REMOTE_SIM                  = 1,
};
typedef enum mipc_sim_vsim_type_const_enum mipc_sim_vsim_type_const_enum;

enum mipc_sim_vsim_auth_mode_const_enum {
    mipc_sim_vsim_auth_mode_const_NONE = 0,
    /* SIM_VSIM_AUTH_MODE */
    MIPC_SIM_VSIM_AUTH_MODE_RELEASING_RF                    = 0,
    MIPC_SIM_VSIM_AUTH_MODE_OCCUPING_RF                     = 1,
};
typedef enum mipc_sim_vsim_auth_mode_const_enum mipc_sim_vsim_auth_mode_const_enum;

enum mipc_sim_power_state_const_enum {
    mipc_sim_power_state_const_NONE = 0,
    /* SIM_POWER_STATE */
    MIPC_SIM_POWER_STATE_OFF                                = 0,
    MIPC_SIM_POWER_STATE_ON                                 = 1,
};
typedef enum mipc_sim_power_state_const_enum mipc_sim_power_state_const_enum;

enum mipc_sim_crrst_state_const_enum {
    mipc_sim_crrst_state_const_NONE = 0,
    /* SIM_CRRST_STATE */
    MIPC_SIM_CRRST_STATE_SIM_CRRST_STATE_NULL               = 0,
    MIPC_SIM_CRRST_STATE_SIM_CRRST_STATE_LOCKED             = 1,
    MIPC_SIM_CRRST_STATE_SIM_CRRST_STATE_UNLOCKED           = 2,
    MIPC_SIM_CRRST_STATE_SIM_CRRST_STATE_DISABLED           = 3,
};
typedef enum mipc_sim_crrst_state_const_enum mipc_sim_crrst_state_const_enum;

enum mipc_sim_carrier_restriction_status_const_enum {
    mipc_sim_carrier_restriction_status_const_NONE = 0,
    /* SIM_CARRIER_RESTRICTION_STATUS */
    MIPC_SIM_CARRIER_RESTRICTION_STATUS_SIM_CARRIER_RESTRICTION_STATUS_UNKNOWN = 0,
    MIPC_SIM_CARRIER_RESTRICTION_STATUS_SIM_CARRIER_RESTRICTION_STATUS_NOT_RESTRICTED = 1,
    MIPC_SIM_CARRIER_RESTRICTION_STATUS_SIM_CARRIER_RESTRICTION_STATUS_RESTRICTED = 2,
};
typedef enum mipc_sim_carrier_restriction_status_const_enum mipc_sim_carrier_restriction_status_const_enum;

enum mipc_sim_sml_category_const_enum {
    mipc_sim_sml_category_const_NONE = 0,
    /* SIM_SML_CATEGORY */
    MIPC_SIM_SML_CATEGORY_CAT_N                             = 0,
    MIPC_SIM_SML_CATEGORY_CAT_NS                            = 1,
    MIPC_SIM_SML_CATEGORY_CAT_SP                            = 2,
    MIPC_SIM_SML_CATEGORY_CAT_CORP                          = 3,
    MIPC_SIM_SML_CATEGORY_CAT_SIM                           = 4,
    MIPC_SIM_SML_CATEGORY_CAT_NS_SP                         = 5,
    MIPC_SIM_SML_CATEGORY_CAT_SIM_CORP                      = 6,
    MIPC_SIM_SML_CATEGORY_CAT_SIZE                          = 7,
    MIPC_SIM_SML_CATEGORY_CAT_DUAL_SIM_LOCK_CONTROL         = 8,
    MIPC_SIM_SML_CATEGORY_CAT_MASTER_KEY                    = 9,
    MIPC_SIM_SML_CATEGORY_CAT_UNKNOWN                       = 255,
};
typedef enum mipc_sim_sml_category_const_enum mipc_sim_sml_category_const_enum;

enum mipc_sim_sml_operation_const_enum {
    mipc_sim_sml_operation_const_NONE = 0,
    /* SIM_SML_OPERATION */
    MIPC_SIM_SML_OPERATION_UNLOCK                           = 0,
    MIPC_SIM_SML_OPERATION_LOCK                             = 1,
    MIPC_SIM_SML_OPERATION_ADD                              = 2,
    MIPC_SIM_SML_OPERATION_REMOVE                           = 3,
    MIPC_SIM_SML_OPERATION_DISABLE                          = 4,
    MIPC_SIM_SML_OPERATION_UPDATE_AUTOLOCK_COUNT            = 5,
};
typedef enum mipc_sim_sml_operation_const_enum mipc_sim_sml_operation_const_enum;

enum mipc_sim_sml_rsu_operation_const_enum {
    mipc_sim_sml_rsu_operation_const_NONE = 0,
    /* SIM_SML_RSU_OPERATION */
    MIPC_SIM_SML_RSU_OPERATION_INIT                         = 1,
    MIPC_SIM_SML_RSU_OPERATION_UPDATE_LOCK_DATA             = 2,
    MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_STATUS              = 3,
    MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER                 = 4,
    MIPC_SIM_SML_RSU_OPERATION_GET_SHARED_KEY               = 5,
    MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_VERSION             = 6,
    MIPC_SIM_SML_RSU_OPERATION_GET_LOCK_DATA                = 7,
    MIPC_SIM_SML_RSU_OPERATION_SET_LOCK_DATA_MORE           = 8,
    MIPC_SIM_SML_RSU_OPERATION_SET_LOCK_DATA_DONE           = 9,
    MIPC_SIM_SML_RSU_OPERATION_GET_ALLOWED_LOCK_DATA        = 8,
    MIPC_SIM_SML_RSU_OPERATION_GET_EXCLUDED_LOCK_DATA       = 9,
    MIPC_SIM_SML_RSU_OPERATION_GET_MD_CONFIG                = 10,
    MIPC_SIM_SML_RSU_OPERATION_GET_API_VERSION              = 11,
    MIPC_SIM_SML_RSU_OPERATION_GET_SIM_SLOTS                = 12,
    MIPC_SIM_SML_RSU_OPERATION_RESET_LOCK_DATA              = 13,
    MIPC_SIM_SML_RSU_OPERATION_VERIFY_TF_STATUS             = 14,
    MIPC_SIM_SML_RSU_OPERATION_GET_IMEI                     = 15,
    MIPC_SIM_SML_RSU_OPERATION_GET_SIGNED_TOKEN             = 16,
    MIPC_SIM_SML_RSU_OPERATION_RELOCK                       = 17,
};
typedef enum mipc_sim_sml_rsu_operation_const_enum mipc_sim_sml_rsu_operation_const_enum;

enum mipc_sml_rsu_request_type_const_enum {
    mipc_sml_rsu_request_type_const_NONE = 0,
    /* SML_RSU_REQUEST_TYPE */
    MIPC_SML_RSU_REQUEST_TYPE_STOP                          = 0,
    MIPC_SML_RSU_REQUEST_TYPE_START                         = 1,
};
typedef enum mipc_sml_rsu_request_type_const_enum mipc_sml_rsu_request_type_const_enum;

enum mipc_sim_fcp_convert_const_enum {
    mipc_sim_fcp_convert_const_NONE = 0,
    /* SIM_FCP_CONVERT */
    /* Not convert USIM FCP to SIM format when CMD receives response */
    MIPC_SIM_FCP_CONVERT_DISABLE                            = 0,
    /* Convert USIM FCP to SIM format when CMD receives response */
    MIPC_SIM_FCP_CONVERT_ENABLE                             = 1,
};
typedef enum mipc_sim_fcp_convert_const_enum mipc_sim_fcp_convert_const_enum;

enum mipc_sim_event_const_enum {
    mipc_sim_event_const_NONE = 0,
    /* SIM_EVENT */
    MIPC_SIM_EVENT_NULL                                     = 0,
    MIPC_SIM_EVENT_APP_INIT                                 = 1,
    MIPC_SIM_EVENT_APP_LIST                                 = 2,
    MIPC_SIM_EVENT_DATA_ICON_READY                          = 3,
    MIPC_SIM_EVENT_FCP_CACHE_READY                          = 4,
    MIPC_SIM_EVENT_DATA_CACHE_READY                         = 5,
};
typedef enum mipc_sim_event_const_enum mipc_sim_event_const_enum;

enum mipc_sim_mep_mode_const_enum {
    mipc_sim_mep_mode_const_NONE = 0,
    /* SIM_MEP_MODE */
    /* There is no jointly supported MEP mode, set supported MEP mode to NONE. */
    MIPC_SIM_MEP_MODE_NONE                                  = 0,
    /* The ISD-R is selected on eSIM port 0 only and profiles are selected on eSIM ports 1 and higher, with the eSIM port being assigned by the LPA or platform. */
    MIPC_SIM_MEP_MODE_MEP_A1                                = 1,
    /* The ISD-R is selected on eSIM port 0 only and profiles are selected on eSIM ports 1 and higher, with the eSIM port being assigned by the eUICC. */
    MIPC_SIM_MEP_MODE_MEP_A2                                = 2,
    /* Profiles are selected on eSIM ports 0 and higher, with the ISD-R being selectable on any of these eSIM ports. */
    MIPC_SIM_MEP_MODE_MEP_B                                 = 3,
};
typedef enum mipc_sim_mep_mode_const_enum mipc_sim_mep_mode_const_enum;

enum mipc_sms_state_const_enum {
    mipc_sms_state_const_NONE = 0,
    /* SMS_STATE */
    /* Some SMS related operations would be banned because SMS module is not initialized */
    MIPC_SMS_STATE_NOT_INITIALIZED                          = 0,
    /* SMS module is initialized, and SMS operations are permitted now */
    MIPC_SMS_STATE_INITIALIZED                              = 1,
};
typedef enum mipc_sms_state_const_enum mipc_sms_state_const_enum;

enum mipc_sms_format_const_enum {
    mipc_sms_format_const_NONE = 0,
    /* SMS_FORMAT */
    /* for cfg use only */
    MIPC_SMS_FORMAT_PDU                                     = 0,
    /* for cfg use only; MIPC dont support TEXT mode now */
    MIPC_SMS_FORMAT_TEXT                                    = 1,
    /* [PHASE OUT] */
    MIPC_SMS_FORMAT_CDMA                                    = 2,
    /* for MIPC other than CFG */
    MIPC_SMS_FORMAT_PDU_3GPP                                = 3,
    /* for MIPC other than CFG */
    MIPC_SMS_FORMAT_PDU_3GPP2                               = 4,
    /* query the CFG */
    MIPC_SMS_FORMAT_QUERY                                   = 0xFD,
    /* same as not carrying this TLV */
    MIPC_SMS_FORMAT_NULL                                    = 0xFE,
    /* invalid value */
    MIPC_SMS_FORMAT_INVALID                                 = 0xFF,
};
typedef enum mipc_sms_format_const_enum mipc_sms_format_const_enum;

enum mipc_sms_cbm_type_const_enum {
    mipc_sms_cbm_type_const_NONE = 0,
    /* SMS_CBM_TYPE */
    /* ETWS primary message */
    MIPC_SMS_CBM_TYPE_ETWS_PRIMARY                          = 0x00000001,
    /* CBM with msg_id in 4352-4356 */
    MIPC_SMS_CBM_TYPE_ETWS_SECONDARY                        = 0x00000002,
    /* CBM with msg_id in 4370-4400 */
    MIPC_SMS_CBM_TYPE_CMAS                                  = 0x00000004,
    /* for MIPC other than CFG */
    MIPC_SMS_CBM_TYPE_OTHERS                                = 0x80000000,
};
typedef enum mipc_sms_cbm_type_const_enum mipc_sms_cbm_type_const_enum;

enum mipc_sms_etws_primary_type_const_enum {
    mipc_sms_etws_primary_type_const_NONE = 0,
    /* SMS_ETWS_PRIMARY_TYPE */
    /* Normal ETWS primary message will be indicated */
    MIPC_SMS_ETWS_PRIMARY_TYPE_ETWS_PRIMARY                 = 0x01,
    /* ETWS primary message with security check will be indicated */
    MIPC_SMS_ETWS_PRIMARY_TYPE_ETWS_PRIMARY_WITH_SECURITY_CHECK = 0x02,
    /* ETWS primary message with msg_id = 4355 will be indicated */
    MIPC_SMS_ETWS_PRIMARY_TYPE_ETWS_PRIMARY_FOR_TEST        = 0x04,
};
typedef enum mipc_sms_etws_primary_type_const_enum mipc_sms_etws_primary_type_const_enum;

enum mipc_sms_flag_const_enum {
    mipc_sms_flag_const_NONE = 0,
    /* SMS_FLAG */
    MIPC_SMS_FLAG_ALL                                       = 0,
    MIPC_SMS_FLAG_NEW                                       = 1,
    MIPC_SMS_FLAG_OLD                                       = 2,
    MIPC_SMS_FLAG_SENT                                      = 3,
    MIPC_SMS_FLAG_DRAFT                                     = 4,
    MIPC_SMS_FLAG_INDEX                                     = 5,
    MIPC_SMS_FLAG_INVALID                                   = 0xFF,
};
typedef enum mipc_sms_flag_const_enum mipc_sms_flag_const_enum;

enum mipc_sms_status_const_enum {
    mipc_sms_status_const_NONE = 0,
    /* SMS_STATUS */
    MIPC_SMS_STATUS_REC_UNREAD                              = 0,
    MIPC_SMS_STATUS_REC_READ                                = 1,
    MIPC_SMS_STATUS_STO_UNSENT                              = 2,
    MIPC_SMS_STATUS_STO_SENT                                = 3,
    MIPC_SMS_STATUS_INVALID                                 = 0xFF,
};
typedef enum mipc_sms_status_const_enum mipc_sms_status_const_enum;

enum mipc_sms_class_const_enum {
    mipc_sms_class_const_NONE = 0,
    /* SMS_CLASS */
    MIPC_SMS_CLASS_0                                        = 0,
    MIPC_SMS_CLASS_1                                        = 1,
    MIPC_SMS_CLASS_2                                        = 2,
    MIPC_SMS_CLASS_3                                        = 3,
    MIPC_SMS_CLASS_NONE                                     = 0xFE,
    MIPC_SMS_CLASS_INVALID                                  = 0xFF,
};
typedef enum mipc_sms_class_const_enum mipc_sms_class_const_enum;

enum mipc_sms_store_flag_const_enum {
    mipc_sms_store_flag_const_NONE = 0,
    /* SMS_STORE_FLAG */
    MIPC_SMS_STORE_FLAG_NONE                                = 0x00000000,
    /* [3GPP]storage full */
    MIPC_SMS_STORE_FLAG_STORE_FULL                          = 0x00000001,
    /* [3GPP]new SMS IND, only valid in INDICATION mode */
    MIPC_SMS_STORE_FLAG_NEW_MESSAGE                         = 0x00000002,
    /* [3GPP]SIM storage full */
    MIPC_SMS_STORE_FLAG_STORE_FULL_SIM                      = 0x00000004,
    /* [3GPP]NVRAM or HOST/TE storage full */
    MIPC_SMS_STORE_FLAG_STORE_FULL_ME                       = 0x00000008,
    /* [3GPP2]UIM storage full */
    MIPC_SMS_STORE_FLAG_STORE_FULL_UIM                      = 0x00000010,
    /* [3GPP2]new c2k_sms IND, only valid in INDICATION mode */
    MIPC_SMS_STORE_FLAG_NEW_SMS_C2K                         = 0x00000020,
};
typedef enum mipc_sms_store_flag_const_enum mipc_sms_store_flag_const_enum;

enum mipc_sms_storage_const_enum {
    mipc_sms_storage_const_NONE = 0,
    /* SMS_STORAGE */
    /* SIM */
    MIPC_SMS_STORAGE_SIM                                    = 0,
    /* NVRAM */
    MIPC_SMS_STORAGE_ME                                     = 1,
    /* Only used in the field "PREFER_STORAGE" of MIPC_SMS_CFG_REQ and MIPC_SMS_CFG_CNF (not saved in SIM/UIM/NVRAM, just relay short message to host) */
    MIPC_SMS_STORAGE_TE                                     = 2,
    /* UIM */
    MIPC_SMS_STORAGE_UIM                                    = 3,
    /* SIM first, followed by NVRAM */
    MIPC_SMS_STORAGE_MT                                     = 4,
    MIPC_SMS_STORAGE_QUERY                                  = 0xFD,
    MIPC_SMS_STORAGE_NULL                                   = 0xFE,
    MIPC_SMS_STORAGE_INVALID                                = 0xFF,
};
typedef enum mipc_sms_storage_const_enum mipc_sms_storage_const_enum;

enum mipc_sms_ack_const_enum {
    mipc_sms_ack_const_NONE = 0,
    /* SMS_ACK */
    MIPC_SMS_ACK_NONEED                                     = 0,
    MIPC_SMS_ACK_NEEDED                                     = 1,
    MIPC_SMS_ACK_QUERY                                      = 0xFD,
    MIPC_SMS_ACK_NULL                                       = 0xFE,
    MIPC_SMS_ACK_INVALID                                    = 0xFF,
};
typedef enum mipc_sms_ack_const_enum mipc_sms_ack_const_enum;

enum mipc_new_sms_ack_const_enum {
    mipc_new_sms_ack_const_NONE = 0,
    /* NEW_SMS_ACK */
    MIPC_NEW_SMS_ACK_RP_ACK                                 = 0,
    MIPC_NEW_SMS_ACK_RP_ERROR                               = 1,
    MIPC_NEW_SMS_ACK_INVALID                                = 0xFF,
};
typedef enum mipc_new_sms_ack_const_enum mipc_new_sms_ack_const_enum;

enum mipc_sms_send_save_const_enum {
    mipc_sms_send_save_const_NONE = 0,
    /* SMS_SEND_SAVE */
    MIPC_SMS_SEND_SAVE_NO_SAVE                              = 0,
    MIPC_SMS_SEND_SAVE_SAVE_IN_SIM                          = 1,
    MIPC_SMS_SEND_SAVE_INVALID                              = 0xFF,
};
typedef enum mipc_sms_send_save_const_enum mipc_sms_send_save_const_enum;

enum mipc_sms_more_msg_to_send_const_enum {
    mipc_sms_more_msg_to_send_const_NONE = 0,
    /* SMS_MORE_MSG_TO_SEND */
    /*  */
    MIPC_SMS_MORE_MSG_TO_SEND_DISABLE                       = 0,
    /* Modem will be disable after a period of time; refer AT+CMMS=1 */
    MIPC_SMS_MORE_MSG_TO_SEND_ENABLE_ONCE                   = 1,
    /* Modem will not be disabled automatically; refer AT+CMMS=2 */
    MIPC_SMS_MORE_MSG_TO_SEND_ENABLE_ALWAYS                 = 2,
    /* NULL value; no action needs to be performed by modem */
    MIPC_SMS_MORE_MSG_TO_SEND_NULL                          = 0xFE,
    /* INVALID value */
    MIPC_SMS_MORE_MSG_TO_SEND_INVALID                       = 0xFF,
};
typedef enum mipc_sms_more_msg_to_send_const_enum mipc_sms_more_msg_to_send_const_enum;

enum mipc_sms_cbm_cfg_type_const_enum {
    mipc_sms_cbm_cfg_type_const_NONE = 0,
    /* SMS_CBM_CFG_TYPE */
    /* List in corresponding CFG IE will be provided for USER */
    MIPC_SMS_CBM_CFG_TYPE_ALLOW                             = 1,
    /* List in corresponding CFG IE will not be provided for USER */
    MIPC_SMS_CBM_CFG_TYPE_BAN                               = 2,
    /* Query DEVICE's value of corresponding CFG */
    MIPC_SMS_CBM_CFG_TYPE_QUERY                             = 0xFD,
};
typedef enum mipc_sms_cbm_cfg_type_const_enum mipc_sms_cbm_cfg_type_const_enum;

enum mipc_sms_scbm_status_const_enum {
    mipc_sms_scbm_status_const_NONE = 0,
    /* SMS_SCBM_STATUS */
    MIPC_SMS_SCBM_STATUS_EXIT                               = 0,
    MIPC_SMS_SCBM_STATUS_ENTERED                            = 1,
    MIPC_SMS_SCBM_STATUS_DISABLED                           = 2,
    MIPC_SMS_SCBM_STATUS_REINITED                           = 3,
};
typedef enum mipc_sms_scbm_status_const_enum mipc_sms_scbm_status_const_enum;

enum mipc_sms_c2k_err_class_const_enum {
    mipc_sms_c2k_err_class_const_NONE = 0,
    /* SMS_C2K_ERR_CLASS */
    MIPC_SMS_C2K_ERR_CLASS_NO_ERR                           = 0,
    MIPC_SMS_C2K_ERR_CLASS_TEMP                             = 2,
    MIPC_SMS_C2K_ERR_CLASS_PERMANENT                        = 3,
    MIPC_SMS_C2K_ERR_CLASS_INVALID                          = 0xFF,
};
typedef enum mipc_sms_c2k_err_class_const_enum mipc_sms_c2k_err_class_const_enum;

enum mipc_sms_text_mode_param_action_const_enum {
    mipc_sms_text_mode_param_action_const_NONE = 0,
    /* SMS_TEXT_MODE_PARAM_ACTION */
    MIPC_SMS_TEXT_MODE_PARAM_ACTION_SET                     = 1,
    MIPC_SMS_TEXT_MODE_PARAM_ACTION_QUERY                   = 0xFD,
    MIPC_SMS_TEXT_MODE_PARAM_ACTION_NULL                    = 0xFE,
    MIPC_SMS_TEXT_MODE_PARAM_ACTION_INVALID                 = 0xFF,
};
typedef enum mipc_sms_text_mode_param_action_const_enum mipc_sms_text_mode_param_action_const_enum;

enum mipc_sms_c2k_err_code_const_enum {
    mipc_sms_c2k_err_code_const_NONE = 0,
    /* SMS_C2K_ERR_CODE */
    MIPC_SMS_C2K_ERR_CODE_FDN_CHECK                         = 0x0E,
    MIPC_SMS_C2K_ERR_CODE_RUIM_ABSENT                       = 0x6B,
};
typedef enum mipc_sms_c2k_err_code_const_enum mipc_sms_c2k_err_code_const_enum;

enum mipc_sms_handle_mode_const_enum {
    mipc_sms_handle_mode_const_NONE = 0,
    /* SMS_HANDLE_MODE */
    /* MD would relay the raw SMS pdu to AP */
    MIPC_SMS_HANDLE_MODE_RELAY                              = 1,
    /* MD would save the SMS pdu in MD */
    MIPC_SMS_HANDLE_MODE_SAVE                               = 2,
};
typedef enum mipc_sms_handle_mode_const_enum mipc_sms_handle_mode_const_enum;

enum mipc_sms_md_storage_const_enum {
    mipc_sms_md_storage_const_NONE = 0,
    /* SMS_MD_STORAGE */
    /* [3GPP]SIM */
    MIPC_SMS_MD_STORAGE_SIM                                 = 1,
    /* [3GPP]NVRAM */
    MIPC_SMS_MD_STORAGE_NV                                  = 2,
    /* [3GPP]try SIM first, then NVRAM */
    MIPC_SMS_MD_STORAGE_SIM_NV                              = 3,
    /* [3GPP2]UIM */
    MIPC_SMS_MD_STORAGE_UIM                                 = 4,
};
typedef enum mipc_sms_md_storage_const_enum mipc_sms_md_storage_const_enum;

enum mipc_sms_ack_mode_const_enum {
    mipc_sms_ack_mode_const_NONE = 0,
    /* SMS_ACK_MODE */
    /* prefer HOST do the ack action */
    MIPC_SMS_ACK_MODE_HOST                                  = 1,
    /* prefer Modem do the ack action */
    MIPC_SMS_ACK_MODE_MD                                    = 2,
};
typedef enum mipc_sms_ack_mode_const_enum mipc_sms_ack_mode_const_enum;

enum mipc_sms_standard_const_enum {
    mipc_sms_standard_const_NONE = 0,
    /* SMS_STANDARD */
    /* 3gpp format */
    MIPC_SMS_STANDARD_3GPP                                  = 1,
    /* 3gpp2 format */
    MIPC_SMS_STANDARD_3GPP2                                 = 2,
};
typedef enum mipc_sms_standard_const_enum mipc_sms_standard_const_enum;

enum mipc_sms_action_const_enum {
    mipc_sms_action_const_NONE = 0,
    /* SMS_ACTION */
    /* set relate params */
    MIPC_SMS_ACTION_SET                                     = 1,
    /* get relate params */
    MIPC_SMS_ACTION_GET                                     = 2,
};
typedef enum mipc_sms_action_const_enum mipc_sms_action_const_enum;

enum mipc_sms_cgsms_const_enum {
    mipc_sms_cgsms_const_NONE = 0,
    /* SMS_CGSMS */
    MIPC_SMS_CGSMS_PS_DOMAIN                                = 0,
    MIPC_SMS_CGSMS_CS_DOMAIN                                = 1,
    MIPC_SMS_CGSMS_PS_PREFER                                = 2,
    MIPC_SMS_CGSMS_CS_PREFER                                = 3,
};
typedef enum mipc_sms_cgsms_const_enum mipc_sms_cgsms_const_enum;

enum mipc_sms_ims_cfg_const_enum {
    mipc_sms_ims_cfg_const_NONE = 0,
    /* SMS_IMS_CFG */
    /* no SMS delivery via IMS */
    MIPC_SMS_IMS_CFG_NO_IMS                                 = 0,
    /* SMS might be provided via IMS */
    MIPC_SMS_IMS_CFG_IMS_ALLOW                              = 1,
};
typedef enum mipc_sms_ims_cfg_const_enum mipc_sms_ims_cfg_const_enum;

enum mipc_em_id_type_const_enum {
    mipc_em_id_type_const_NONE = 0,
    /* EM_ID_TYPE */
    MIPC_EM_ID_TYPE_UNKNOWN_SERVICE                         = 0,
    MIPC_EM_ID_TYPE_NOT_EMERGENCY_SERVICE                   = 1,
    MIPC_EM_ID_TYPE_EMERGENCY_SERVICE                       = 2,
    MIPC_EM_ID_TYPE_MAX                                     = 3,
};
typedef enum mipc_em_id_type_const_enum mipc_em_id_type_const_enum;

enum mipc_pdis_method_type_const_enum {
    mipc_pdis_method_type_const_NONE = 0,
    /* PDIS_METHOD_TYPE */
    MIPC_PDIS_METHOD_TYPE_NONE                              = 0,
    MIPC_PDIS_METHOD_TYPE_SIM                               = 1,
    MIPC_PDIS_METHOD_TYPE_MO                                = 2,
    MIPC_PDIS_METHOD_TYPE_PCO                               = 3,
    MIPC_PDIS_METHOD_TYPE_DHCPV4                            = 4,
    MIPC_PDIS_METHOD_TYPE_DHCPV6                            = 5,
    MIPC_PDIS_METHOD_TYPE_MANUL                             = 6,
};
typedef enum mipc_pdis_method_type_const_enum mipc_pdis_method_type_const_enum;

enum mipc_scm_application_enum_const_enum {
    mipc_scm_application_enum_const_NONE = 0,
    /* SCM_APPLICATION_ENUM */
    MIPC_SCM_APPLICATION_ENUM_MMTEL_VOICE                   = 0,
    MIPC_SCM_APPLICATION_ENUM_MMTEL_VIDEO                   = 1,
    MIPC_SCM_APPLICATION_ENUM_SMS_OVER_IP                   = 2,
    MIPC_SCM_APPLICATION_ENUM_REG_SIG                       = 3,
    MIPC_SCM_APPLICATION_ENUM_MAX                           = 0x7F,
};
typedef enum mipc_scm_application_enum_const_enum mipc_scm_application_enum_const_enum;

enum mipc_scm_indication_enum_const_enum {
    mipc_scm_indication_enum_const_NONE = 0,
    /* SCM_INDICATION_ENUM */
    MIPC_SCM_INDICATION_ENUM_START_INDICATION               = 1,
    MIPC_SCM_INDICATION_ENUM_END_INDICATION                 = 2,
    MIPC_SCM_INDICATION_ENUM_MAX                            = 0x7F,
};
typedef enum mipc_scm_indication_enum_const_enum mipc_scm_indication_enum_const_enum;

enum mipc_exims_service_type_enum_const_enum {
    mipc_exims_service_type_enum_const_NONE = 0,
    /* EXIMS_SERVICE_TYPE_ENUM */
    MIPC_EXIMS_SERVICE_TYPE_ENUM_NORMAL_VOICE_CALL          = 1,
    MIPC_EXIMS_SERVICE_TYPE_ENUM_NORMAL_VIDEO_CALL          = 2,
    MIPC_EXIMS_SERVICE_TYPE_ENUM_SMS                        = 4,
    MIPC_EXIMS_SERVICE_TYPE_ENUM_REG_SIG                    = 8,
    MIPC_EXIMS_SERVICE_TYPE_ENUM_MAX                        = 0x7F,
};
typedef enum mipc_exims_service_type_enum_const_enum mipc_exims_service_type_enum_const_enum;

enum mipc_exims_service_status_enum_const_enum {
    mipc_exims_service_status_enum_const_NONE = 0,
    /* EXIMS_SERVICE_STATUS_ENUM */
    MIPC_EXIMS_SERVICE_STATUS_ENUM_START_INDICATION         = 0,
    MIPC_EXIMS_SERVICE_STATUS_ENUM_STOP_INDICATION          = 1,
    MIPC_EXIMS_SERVICE_STATUS_ENUM_MAX                      = 0x7F,
};
typedef enum mipc_exims_service_status_enum_const_enum mipc_exims_service_status_enum_const_enum;

enum mipc_exims_uac_cause_enum_const_enum {
    mipc_exims_uac_cause_enum_const_NONE = 0,
    /* EXIMS_UAC_CAUSE_ENUM */
    MIPC_EXIMS_UAC_CAUSE_ENUM_BARRED                        = 0,
    MIPC_EXIMS_UAC_CAUSE_ENUM_NO_COVERAGE                   = 1,
    MIPC_EXIMS_UAC_CAUSE_ENUM_DEREGISTERED                  = 2,
    MIPC_EXIMS_UAC_CAUSE_ENUM_GEMINI_SUSPEND                = 3,
    MIPC_EXIMS_UAC_CAUSE_ENUM_MAX                           = 0x7F,
};
typedef enum mipc_exims_uac_cause_enum_const_enum mipc_exims_uac_cause_enum_const_enum;

enum mipc_act_call_ntf_enum_const_enum {
    mipc_act_call_ntf_enum_const_NONE = 0,
    /* ACT_CALL_NTF_ENUM */
    MIPC_ACT_CALL_NTF_ENUM_USE_FB_DATA_CALL                 = 0,
    MIPC_ACT_CALL_NTF_ENUM_UNUSE_FB_DATA_CALL               = 1,
    MIPC_ACT_CALL_NTF_ENUM_DEACT_DATA_CALL                  = 2,
    MIPC_ACT_CALL_NTF_ENUM_MAX                              = 0x7F,
};
typedef enum mipc_act_call_ntf_enum_const_enum mipc_act_call_ntf_enum_const_enum;

enum mipc_ipc_packet_route_app_id_enum_const_enum {
    mipc_ipc_packet_route_app_id_enum_const_NONE = 0,
    /* IPC_PACKET_ROUTE_APP_ID_ENUM */
    MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_NONE = 0,
    MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_WIFI = 1,
    MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_IMS = 2,
};
typedef enum mipc_ipc_packet_route_app_id_enum_const_enum mipc_ipc_packet_route_app_id_enum_const_enum;

enum mipc_ims_reg_state_type_const_enum {
    mipc_ims_reg_state_type_const_NONE = 0,
    /* IMS_REG_STATE_TYPE */
    MIPC_IMS_REG_STATE_TYPE_NOT_REGISTERED                  = 0,
    MIPC_IMS_REG_STATE_TYPE_REGISTERED                      = 1,
};
typedef enum mipc_ims_reg_state_type_const_enum mipc_ims_reg_state_type_const_enum;

enum mipc_ims_dereg_cause_type_const_enum {
    mipc_ims_dereg_cause_type_const_NONE = 0,
    /* IMS_DEREG_CAUSE_TYPE */
    MIPC_IMS_DEREG_CAUSE_TYPE_UNSPECIFIED                   = 0,
    MIPC_IMS_DEREG_CAUSE_TYPE_POWER_OFF                     = 1,
    MIPC_IMS_DEREG_CAUSE_TYPE_RF_OFF                        = 2,
};
typedef enum mipc_ims_dereg_cause_type_const_enum mipc_ims_dereg_cause_type_const_enum;

enum mipc_ims_retry_type_const_enum {
    mipc_ims_retry_type_const_NONE = 0,
    /* IMS_RETRY_TYPE */
    MIPC_IMS_RETRY_TYPE_NOT_RETRY                           = 0,
    MIPC_IMS_RETRY_TYPE_RETRY                               = 1,
};
typedef enum mipc_ims_retry_type_const_enum mipc_ims_retry_type_const_enum;

enum mipc_ims_rat_type_const_enum {
    mipc_ims_rat_type_const_NONE = 0,
    /* IMS_RAT_TYPE */
    MIPC_IMS_RAT_TYPE_LTE                                   = 0,
    MIPC_IMS_RAT_TYPE_WIFI                                  = 1,
    MIPC_IMS_RAT_TYPE_EHRPD                                 = 2,
    MIPC_IMS_RAT_TYPE_GSM                                   = 3,
    MIPC_IMS_RAT_TYPE_UMTS                                  = 4,
};
typedef enum mipc_ims_rat_type_const_enum mipc_ims_rat_type_const_enum;

enum mipc_ims_uri_type_const_enum {
    mipc_ims_uri_type_const_NONE = 0,
    /* IMS_URI_TYPE */
    MIPC_IMS_URI_TYPE_URI_MSISDN                            = 0,
    MIPC_IMS_URI_TYPE_URI_IMSI                              = 1,
};
typedef enum mipc_ims_uri_type_const_enum mipc_ims_uri_type_const_enum;

enum mipc_ims_evodata_en_enum_const_enum {
    mipc_ims_evodata_en_enum_const_NONE = 0,
    /* IMS_EVODATA_EN_ENUM */
    MIPC_IMS_EVODATA_EN_ENUM_VODATA_OFF                     = 0,
    MIPC_IMS_EVODATA_EN_ENUM_VODATA_ON                      = 1,
};
typedef enum mipc_ims_evodata_en_enum_const_enum mipc_ims_evodata_en_enum_const_enum;

enum mipc_ims_evodata_mode_enum_const_enum {
    mipc_ims_evodata_mode_enum_const_NONE = 0,
    /* IMS_EVODATA_MODE_ENUM */
    MIPC_IMS_EVODATA_MODE_ENUM_DEFAULT                      = 0,
    MIPC_IMS_EVODATA_MODE_ENUM_OOS_MODE                     = 1,
};
typedef enum mipc_ims_evodata_mode_enum_const_enum mipc_ims_evodata_mode_enum_const_enum;

enum mipc_ims_evodata_allowrat_enum_const_enum {
    mipc_ims_evodata_allowrat_enum_const_NONE = 0,
    /* IMS_EVODATA_ALLOWRAT_ENUM */
    MIPC_IMS_EVODATA_ALLOWRAT_ENUM_DEFAULT                  = 0,
    MIPC_IMS_EVODATA_ALLOWRAT_ENUM_LTE_NR_3G                = 1,
};
typedef enum mipc_ims_evodata_allowrat_enum_const_enum mipc_ims_evodata_allowrat_enum_const_enum;

enum mipc_ims_vops_n3gpp_enum_const_enum {
    mipc_ims_vops_n3gpp_enum_const_NONE = 0,
    /* IMS_VOPS_N3GPP_ENUM */
    MIPC_IMS_VOPS_N3GPP_ENUM_IMS_VOPS_NOT_SUPPORT_OVER_NON_3GPP_ACCESS = 0,
    MIPC_IMS_VOPS_N3GPP_ENUM_IMS_VOPS_SUPPORT_OVER_NON_3GPP_ACCESS = 1,
    MIPC_IMS_VOPS_N3GPP_ENUM_IMS_VOPS_OVER_NON_3GPP_ACCESS_NOT_UPDATED = 2,
};
typedef enum mipc_ims_vops_n3gpp_enum_const_enum mipc_ims_vops_n3gpp_enum_const_enum;

enum mipc_wfc_signal_type_const_enum {
    mipc_wfc_signal_type_const_NONE = 0,
    /* WFC_SIGNAL_TYPE */
    MIPC_WFC_SIGNAL_TYPE_LTE_RSRP                           = 0,
    MIPC_WFC_SIGNAL_TYPE_LTE_RS_SNR                         = 1,
    MIPC_WFC_SIGNAL_TYPE_UMTS_RSCP                          = 2,
    MIPC_WFC_SIGNAL_TYPE_UMTS_ECNO                          = 3,
    MIPC_WFC_SIGNAL_TYPE_LTE_RSRQ                           = 4,
    MIPC_WFC_SIGNAL_TYPE_NR_SS_RSRP                         = 5,
    MIPC_WFC_SIGNAL_TYPE_INVALID                            = 0xFF,
};
typedef enum mipc_wfc_signal_type_const_enum mipc_wfc_signal_type_const_enum;

enum mipc_epdg_screen_state_const_enum {
    mipc_epdg_screen_state_const_NONE = 0,
    /* EPDG_SCREEN_STATE */
    MIPC_EPDG_SCREEN_STATE_UNLOCK                           = 0,
    MIPC_EPDG_SCREEN_STATE_LOCK                             = 1,
    MIPC_EPDG_SCREEN_STATE_OPEN_KEYGUARD_ON                 = 2,
    MIPC_EPDG_SCREEN_STATE_INVALID                          = 0xFF,
};
typedef enum mipc_epdg_screen_state_const_enum mipc_epdg_screen_state_const_enum;

enum mipc_wifi_type_const_enum {
    mipc_wifi_type_const_NONE = 0,
    /* WIFI_TYPE */
    MIPC_WIFI_TYPE_802_11                                   = 0,
    MIPC_WIFI_TYPE_802_11a                                  = 1,
    MIPC_WIFI_TYPE_802_11b                                  = 2,
    MIPC_WIFI_TYPE_802_11g                                  = 3,
    MIPC_WIFI_TYPE_802_11n                                  = 4,
    MIPC_WIFI_TYPE_802_11ac                                 = 5,
    MIPC_WIFI_TYPE_INVALID                                  = 0xFF,
};
typedef enum mipc_wifi_type_const_enum mipc_wifi_type_const_enum;

enum mipc_wifi_extend_state_const_enum {
    mipc_wifi_extend_state_const_NONE = 0,
    /* WIFI_EXTEND_STATE */
    /* it is default value, Indicates that the above status is not satisfied */
    MIPC_WIFI_EXTEND_STATE_DEFAULT_VALUE                    = 0,
    /* meaning has wifi transport capability && net capability validated */
    MIPC_WIFI_EXTEND_STATE_WIFI_TRANSFER_CAPABILITY         = 1,
};
typedef enum mipc_wifi_extend_state_const_enum mipc_wifi_extend_state_const_enum;

enum mipc_wifi_conn_state_const_enum {
    mipc_wifi_conn_state_const_NONE = 0,
    /* WIFI_CONN_STATE */
    /* wifi dis-associated */
    MIPC_WIFI_CONN_STATE_DIS_ASSOCIATED                     = 0,
    /* wifi associated */
    MIPC_WIFI_CONN_STATE_ASSOCIATED                         = 1,
};
typedef enum mipc_wifi_conn_state_const_enum mipc_wifi_conn_state_const_enum;

enum mipc_wifi_apm_state_const_enum {
    mipc_wifi_apm_state_const_NONE = 0,
    /* WIFI_APM_STATE */
    MIPC_WIFI_APM_STATE_WIFI_TURN_OFF_IN_APM                = 0,
    MIPC_WIFI_APM_STATE_WIFI_REMAIN_ON_IN_APM               = 1,
};
typedef enum mipc_wifi_apm_state_const_enum mipc_wifi_apm_state_const_enum;

enum mipc_wfc_prefer_const_enum {
    mipc_wfc_prefer_const_NONE = 0,
    /* WFC_PREFER */
    MIPC_WFC_PREFER_CELL_ONLY                               = 0,
    MIPC_WFC_PREFER_WIFI_PREFER                             = 1,
    MIPC_WFC_PREFER_CELL_PREFER                             = 2,
    MIPC_WFC_PREFER_WIFI_ONLY                               = 3,
    MIPC_WFC_PREFER_INVALID                                 = 0xFF,
};
typedef enum mipc_wfc_prefer_const_enum mipc_wfc_prefer_const_enum;

enum mipc_wfc_rat_const_enum {
    mipc_wfc_rat_const_NONE = 0,
    /* WFC_RAT */
    MIPC_WFC_RAT_3GPP                                       = 1,
    MIPC_WFC_RAT_WIFI                                       = 2,
    MIPC_WFC_RAT_INVALID                                    = 0xFF,
};
typedef enum mipc_wfc_rat_const_enum mipc_wfc_rat_const_enum;

enum mipc_wfc_pdn_type_const_enum {
    mipc_wfc_pdn_type_const_NONE = 0,
    /* WFC_PDN_TYPE */
    MIPC_WFC_PDN_TYPE_IMS                                   = 0,
    MIPC_WFC_PDN_TYPE_SMS                                   = 1,
    MIPC_WFC_PDN_TYPE_MMS                                   = 2,
    MIPC_WFC_PDN_TYPE_INVALID                               = 0xFF,
};
typedef enum mipc_wfc_pdn_type_const_enum mipc_wfc_pdn_type_const_enum;

enum mipc_wfc_pdn_ho_status_const_enum {
    mipc_wfc_pdn_ho_status_const_NONE = 0,
    /* WFC_PDN_HO_STATUS */
    MIPC_WFC_PDN_HO_STATUS_START                            = 0,
    MIPC_WFC_PDN_HO_STATUS_SUCCESS                          = 1,
    MIPC_WFC_PDN_HO_STATUS_FAIL                             = 0xFF,
};
typedef enum mipc_wfc_pdn_ho_status_const_enum mipc_wfc_pdn_ho_status_const_enum;

enum mipc_wfc_pdn_oos_state_const_enum {
    mipc_wfc_pdn_oos_state_const_NONE = 0,
    /* WFC_PDN_OOS_STATE */
    MIPC_WFC_PDN_OOS_STATE_OOS_END_PDN_DIS                  = 0,
    MIPC_WFC_PDN_OOS_STATE_OOS_START                        = 1,
    MIPC_WFC_PDN_OOS_STATE_OOS_END_PDN_RESUME               = 2,
    MIPC_WFC_PDN_OOS_STATE_FAIL                             = 0xFF,
};
typedef enum mipc_wfc_pdn_oos_state_const_enum mipc_wfc_pdn_oos_state_const_enum;

enum mipc_wfc_wifi_pdn_state_const_enum {
    mipc_wfc_wifi_pdn_state_const_NONE = 0,
    /* WFC_WIFI_PDN_STATE */
    MIPC_WFC_WIFI_PDN_STATE_NOT_REGISTERED                  = 0,
    MIPC_WFC_WIFI_PDN_STATE_REGISTERED                      = 1,
    MIPC_WFC_WIFI_PDN_STATE_NOT_REGISTERED_WITH_LOCK        = 2,
    MIPC_WFC_WIFI_PDN_STATE_INVALID                         = 0xFF,
};
typedef enum mipc_wfc_wifi_pdn_state_const_enum mipc_wfc_wifi_pdn_state_const_enum;

enum mipc_ss_ussd_const_enum {
    mipc_ss_ussd_const_NONE = 0,
    /* SS_USSD */
    MIPC_SS_USSD_NO_ACTION_REQUIRED                         = 0,
    MIPC_SS_USSD_ACTION_REQUIRED                            = 1,
    MIPC_SS_USSD_TERMINATED_BY_NW                           = 2,
    MIPC_SS_USSD_OTHER_LOCAL_CLIENT                         = 3,
    MIPC_SS_USSD_OPERATION_NOT_SUPPORTED                    = 4,
    MIPC_SS_USSD_NW_TIME_OUT                                = 5,
};
typedef enum mipc_ss_ussd_const_enum mipc_ss_ussd_const_enum;

enum mipc_ss_session_const_enum {
    mipc_ss_session_const_NONE = 0,
    /* SS_SESSION */
    MIPC_SS_SESSION_NEW                                     = 0,
    MIPC_SS_SESSION_EXISTING                                = 1,
};
typedef enum mipc_ss_session_const_enum mipc_ss_session_const_enum;

enum mipc_ss_call_waiting_const_enum {
    mipc_ss_call_waiting_const_NONE = 0,
    /* SS_CALL_WAITING */
    MIPC_SS_CALL_WAITING_DISABLE                            = 0,
    MIPC_SS_CALL_WAITING_ENABLE                             = 1,
};
typedef enum mipc_ss_call_waiting_const_enum mipc_ss_call_waiting_const_enum;

enum mipc_ss_set_call_forward_operation_code_const_enum {
    mipc_ss_set_call_forward_operation_code_const_NONE = 0,
    /* SS_SET_CALL_FORWARD_OPERATION_CODE */
    MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_ACTIVATE     = 0,
    MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_DEACTIVATE   = 1,
    MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_REGISTRATION = 3,
    MIPC_SS_SET_CALL_FORWARD_OPERATION_CODE_SS_ERASURE      = 4,
};
typedef enum mipc_ss_set_call_forward_operation_code_const_enum mipc_ss_set_call_forward_operation_code_const_enum;

enum mipc_ss_call_forward_reason_const_enum {
    mipc_ss_call_forward_reason_const_NONE = 0,
    /* SS_CALL_FORWARD_REASON */
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_UNCONDITIONAL  = 21,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_BUSY           = 67,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_NO_REPLY       = 61,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_NOT_REACHABLE  = 62,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_ALL            = 2,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_ALL_CONDITIONAL = 4,
    MIPC_SS_CALL_FORWARD_REASON_CALL_FORWARD_NOT_REGISTER   = 68,
};
typedef enum mipc_ss_call_forward_reason_const_enum mipc_ss_call_forward_reason_const_enum;

enum mipc_ss_active_status_const_enum {
    mipc_ss_active_status_const_NONE = 0,
    /* SS_ACTIVE_STATUS */
    MIPC_SS_ACTIVE_STATUS_NOT_ACTIVE                        = 0,
    MIPC_SS_ACTIVE_STATUS_ACTIVE                            = 1,
};
typedef enum mipc_ss_active_status_const_enum mipc_ss_active_status_const_enum;

enum mipc_ss_type_of_address_const_enum {
    mipc_ss_type_of_address_const_NONE = 0,
    /* SS_TYPE_OF_ADDRESS */
    MIPC_SS_TYPE_OF_ADDRESS_NORMAL                          = 0,
    MIPC_SS_TYPE_OF_ADDRESS_INTERNATIONAL                   = 1,
};
typedef enum mipc_ss_type_of_address_const_enum mipc_ss_type_of_address_const_enum;

enum mipc_ss_service_class_const_enum {
    mipc_ss_service_class_const_NONE = 0,
    /* SS_SERVICE_CLASS */
    MIPC_SS_SERVICE_CLASS_NONE                              = 0,
    MIPC_SS_SERVICE_CLASS_VOICE                             = 1,
    MIPC_SS_SERVICE_CLASS_DATA                              = 2,
    MIPC_SS_SERVICE_CLASS_FAX                               = 4,
    MIPC_SS_SERVICE_CLASS_SMS                               = 8,
    MIPC_SS_SERVICE_CLASS_DATA_CIRCUIT_SYNC                 = 16,
    MIPC_SS_SERVICE_CLASS_DATA_CIRCUIT_ASYNC                = 32,
    MIPC_SS_SERVICE_CLASS_DEDICATED_PACKET_ACCESS           = 64,
    MIPC_SS_SERVICE_CLASS_DEDICATED_PAD_ACCESS              = 128,
    MIPC_SS_SERVICE_CLASS_MTK_LINE2                         = 256,
    MIPC_SS_SERVICE_CLASS_MTK_VIDEO                         = 512,
    MIPC_SS_SERVICE_CLASS_MAX_VALUE                         = 0x3FF,
};
typedef enum mipc_ss_service_class_const_enum mipc_ss_service_class_const_enum;

enum mipc_ss_call_barring_lock_const_enum {
    mipc_ss_call_barring_lock_const_NONE = 0,
    /* SS_CALL_BARRING_LOCK */
    MIPC_SS_CALL_BARRING_LOCK_UNLOCK                        = 0,
    MIPC_SS_CALL_BARRING_LOCK_LOCK                          = 1,
};
typedef enum mipc_ss_call_barring_lock_const_enum mipc_ss_call_barring_lock_const_enum;

enum mipc_ss_call_barring_fac_const_enum {
    mipc_ss_call_barring_fac_const_NONE = 0,
    /* SS_CALL_BARRING_FAC */
    MIPC_SS_CALL_BARRING_FAC_AO                             = 0,
    MIPC_SS_CALL_BARRING_FAC_OI                             = 1,
    MIPC_SS_CALL_BARRING_FAC_OX                             = 2,
    MIPC_SS_CALL_BARRING_FAC_AI                             = 3,
    MIPC_SS_CALL_BARRING_FAC_IR                             = 4,
    MIPC_SS_CALL_BARRING_FAC_AB                             = 5,
    MIPC_SS_CALL_BARRING_FAC_AG                             = 6,
    MIPC_SS_CALL_BARRING_FAC_AC                             = 7,
    MIPC_SS_CALL_BARRING_FAC_ACR                            = 8,
    MIPC_SS_CALL_BARRING_FAC_MT                             = 9,
};
typedef enum mipc_ss_call_barring_fac_const_enum mipc_ss_call_barring_fac_const_enum;

enum mipc_ss_cnap_state_const_enum {
    mipc_ss_cnap_state_const_NONE = 0,
    /* SS_CNAP_STATE */
    MIPC_SS_CNAP_STATE_DISABLE                              = 0,
    MIPC_SS_CNAP_STATE_ENABLE                               = 1,
};
typedef enum mipc_ss_cnap_state_const_enum mipc_ss_cnap_state_const_enum;

enum mipc_ss_ecfu_icon_status_const_enum {
    mipc_ss_ecfu_icon_status_const_NONE = 0,
    /* SS_ECFU_ICON_STATUS */
    MIPC_SS_ECFU_ICON_STATUS_HIDE_CFU_ICON                  = 0,
    MIPC_SS_ECFU_ICON_STATUS_SHOW_CFU_ICON                  = 1,
};
typedef enum mipc_ss_ecfu_icon_status_const_enum mipc_ss_ecfu_icon_status_const_enum;

enum mipc_spi_status_enum_const_enum {
    mipc_spi_status_enum_const_NONE = 0,
    /* SPI_STATUS_ENUM */
    MIPC_SPI_STATUS_ENUM_SUCCESS                            = 0,
    MIPC_SPI_STATUS_ENUM_FAILURE                            = 1,
    MIPC_SPI_STATUS_ENUM_ENCAP_NO_RULE                      = 2,
    MIPC_SPI_STATUS_ENUM_DECAP_NO_RULE                      = 2,
    MIPC_SPI_STATUS_ENUM_DECAP_PACKET_SA_UNMATCH            = 3,
    MIPC_SPI_STATUS_ENUM_SPI_FREE_FAIL_SA_EXISTANT          = 4,
    MIPC_SPI_STATUS_ENUM_ENCAP_SA_SUSPEND                   = 5,
    MIPC_SPI_STATUS_ENUM_MAX                                = 6,
};
typedef enum mipc_spi_status_enum_const_enum mipc_spi_status_enum_const_enum;

enum mipc_ipsec_proto_enum_const_enum {
    mipc_ipsec_proto_enum_const_NONE = 0,
    /* IPSEC_PROTO_ENUM */
    MIPC_IPSEC_PROTO_ENUM_ESP                               = 50,
    MIPC_IPSEC_PROTO_ENUM_AH                                = 51,
    MIPC_IPSEC_PROTO_ENUM_MAX                               = 0x7F,
};
typedef enum mipc_ipsec_proto_enum_const_enum mipc_ipsec_proto_enum_const_enum;

enum mipc_ipsec_mode_enum_const_enum {
    mipc_ipsec_mode_enum_const_NONE = 0,
    /* IPSEC_MODE_ENUM */
    MIPC_IPSEC_MODE_ENUM_transport                          = 0,
    MIPC_IPSEC_MODE_ENUM_tunnel                             = 1,
    MIPC_IPSEC_MODE_ENUM_MAX                                = 0x7F,
};
typedef enum mipc_ipsec_mode_enum_const_enum mipc_ipsec_mode_enum_const_enum;

enum mipc_stk_not_handled_const_enum {
    mipc_stk_not_handled_const_NONE = 0,
    /* STK_NOT_HANDLED */
    MIPC_STK_NOT_HANDLED_BY_FUNCTION_CANNOT_BE_HANDLED_BY_HOST = 0,
    MIPC_STK_NOT_HANDLED_By_FUNCTION_MAY_BE_HANDLED_BY_HOST = 1,
};
typedef enum mipc_stk_not_handled_const_enum mipc_stk_not_handled_const_enum;

enum mipc_stk_handled_const_enum {
    mipc_stk_handled_const_NONE = 0,
    /* STK_HANDLED */
    MIPC_STK_HANDLED_BY_FUNCTION_ONLY_TRANSPARENT_TO_HOST   = 2,
    MIPC_STK_HANDLED_BY_FUNCTION_NOTIFICATION_TO_HOST_POSSIBLE = 3,
    MIPC_STK_HANDLED_BY_FUNCTION_NOTIFICATIONS_TO_HOST_ENABLE = 4,
    MIPC_STK_HANDLED_BY_FUNCTION_CAN_BE_OVERRIDDEN_BY_HOST  = 5,
    MIPC_STK_HANDLED_BY_HOST_FUNCTION_NOT_ABLE_TO_HANDLE    = 6,
    MIPC_STK_HANDLED_BY_HOST_FUNCTION_ABLE_TO_HANDLE        = 7,
};
typedef enum mipc_stk_handled_const_enum mipc_stk_handled_const_enum;

enum mipc_stk_pac_type_const_enum {
    mipc_stk_pac_type_const_NONE = 0,
    /* STK_PAC_TYPE */
    MIPC_STK_PAC_TYPE_PROACTIVE_COMMAND                     = 0,
    MIPC_STK_PAC_TYPE_NOTIFICATION                          = 1,
    MIPC_STK_PAC_TYPE_SESSION_END                           = 2,
};
typedef enum mipc_stk_pac_type_const_enum mipc_stk_pac_type_const_enum;

enum mipc_sim_refresh_result_type_const_enum {
    mipc_sim_refresh_result_type_const_NONE = 0,
    /* SIM_REFRESH_RESULT_TYPE */
    MIPC_SIM_REFRESH_RESULT_TYPE_SIM_FILE_UPDATE            = 0,
    MIPC_SIM_REFRESH_RESULT_TYPE_SIM_INIT                   = 1,
    MIPC_SIM_REFRESH_RESULT_TYPE_SIM_RESET                  = 2,
    MIPC_SIM_REFRESH_RESULT_TYPE_APP_INIT                   = 3,
    MIPC_SIM_REFRESH_RESULT_TYPE_SIM_INIT_FULL_FILE_CHANGE  = 4,
    MIPC_SIM_REFRESH_RESULT_TYPE_SIM_INIT_FILE_CHANGE       = 5,
    MIPC_SIM_REFRESH_RESULT_TYPE_SESSION_RESET              = 6,
};
typedef enum mipc_sim_refresh_result_type_const_enum mipc_sim_refresh_result_type_const_enum;

enum mipc_ran_const_enum {
    mipc_ran_const_NONE = 0,
    /* RAN */
    MIPC_RAN_UNKNOWN                                        = 0,
    MIPC_RAN_CELL                                           = 1,
    MIPC_RAN_WIFI                                           = 2,
    MIPC_RAN_3GPP2                                          = 3,
    MIPC_RAN_DATA1                                          = 4,
    MIPC_RAN_DATA2                                          = 5,
    MIPC_RAN_DATA3                                          = 6,
    MIPC_RAN_DATA4                                          = 7,
};
typedef enum mipc_ran_const_enum mipc_ran_const_enum;

enum mipc_call_state_const_enum {
    mipc_call_state_const_NONE = 0,
    /* CALL_STATE */
    MIPC_CALL_STATE_IDLE                                    = 0,
    MIPC_CALL_STATE_ESTABLISHING                            = 1,
    MIPC_CALL_STATE_ACTIVE                                  = 2,
    MIPC_CALL_STATE_HELD                                    = 3,
    MIPC_CALL_STATE_TERMINATED                              = 4,
};
typedef enum mipc_call_state_const_enum mipc_call_state_const_enum;

enum mipc_call_clcc_state_const_enum {
    mipc_call_clcc_state_const_NONE = 0,
    /* CALL_CLCC_STATE */
    MIPC_CALL_CLCC_STATE_ACTIVE                             = 0,
    MIPC_CALL_CLCC_STATE_HELD                               = 1,
    MIPC_CALL_CLCC_STATE_DIALING                            = 2,
    MIPC_CALL_CLCC_STATE_ALERTING                           = 3,
    MIPC_CALL_CLCC_STATE_INCOMING                           = 4,
    MIPC_CALL_CLCC_STATE_WAITING                            = 5,
};
typedef enum mipc_call_clcc_state_const_enum mipc_call_clcc_state_const_enum;

enum mipc_call_direction_const_enum {
    mipc_call_direction_const_NONE = 0,
    /* CALL_DIRECTION */
    MIPC_CALL_DIRECTION_MO_CALL                             = 0,
    MIPC_CALL_DIRECTION_MT_CALL                             = 1,
};
typedef enum mipc_call_direction_const_enum mipc_call_direction_const_enum;

enum mipc_sdp_direction_const_enum {
    mipc_sdp_direction_const_NONE = 0,
    /* SDP_DIRECTION */
    MIPC_SDP_DIRECTION_INACTIVE                             = 0,
    MIPC_SDP_DIRECTION_SEND_ONLY                            = 1,
    MIPC_SDP_DIRECTION_RECV_ONLY                            = 2,
    MIPC_SDP_DIRECTION_SEND_RECV                            = 3,
};
typedef enum mipc_sdp_direction_const_enum mipc_sdp_direction_const_enum;

enum mipc_speech_rat_const_enum {
    mipc_speech_rat_const_NONE = 0,
    /* SPEECH_RAT */
    MIPC_SPEECH_RAT_GSM                                     = 1,
    MIPC_SPEECH_RAT_UTMS                                    = 2,
    MIPC_SPEECH_RAT_UMTS                                    = 2,
    MIPC_SPEECH_RAT_IMS                                     = 4,
    MIPC_SPEECH_RAT_LTE                                     = 4,
    MIPC_SPEECH_RAT_COM_GSM                                 = 8,
    MIPC_SPEECH_RAT_C2K                                     = 16,
    MIPC_SPEECH_RAT_1xRTT                                   = 32,
    MIPC_SPEECH_RAT_HRPD                                    = 64,
    MIPC_SPEECH_RAT_NR                                      = 128,
};
typedef enum mipc_speech_rat_const_enum mipc_speech_rat_const_enum;

enum mipc_call_rat_const_enum {
    mipc_call_rat_const_NONE = 0,
    /* CALL_RAT */
    MIPC_CALL_RAT_UNKNOWN                                   = 0,
    MIPC_CALL_RAT_CS                                        = 1,
    MIPC_CALL_RAT_VoLTE                                     = 2,
    MIPC_CALL_RAT_WFC                                       = 3,
    MIPC_CALL_RAT_VoNR                                      = 4,
};
typedef enum mipc_call_rat_const_enum mipc_call_rat_const_enum;

enum mipc_call_type_const_enum {
    mipc_call_type_const_NONE = 0,
    /* CALL_TYPE */
    /*  */
    MIPC_CALL_TYPE_MPTY                                     = 0x00000001,
    /*  */
    MIPC_CALL_TYPE_EMERGENCY                                = 0x00000010,
    /*  */
    MIPC_CALL_TYPE_VIDEO                                    = 0x00000100,
    /*  */
    MIPC_CALL_TYPE_RTT                                      = 0x00001000,
    /*  */
    MIPC_CALL_TYPE_IN_BAND_TONE                             = 0x00010000,
    /*  */
    MIPC_CALL_TYPE_IN_BAND_TONE_VIDEO                       = 0x00020000,
    /*  */
    MIPC_CALL_TYPE_IN_BAND_TONE_VIDEO_VOICE                 = 0x00030000,
    /*  */
    MIPC_CALL_TYPE_TCH                                      = 0x00100000,
};
typedef enum mipc_call_type_const_enum mipc_call_type_const_enum;


enum mipc_call_mode_const_enum {
    mipc_call_mode_const_NONE = 0,
    /* CALL_MODE */
    MIPC_CALL_MODE_VOICE                                    = 0,
    MIPC_CALL_MODE_DATA                                     = 1,
    MIPC_CALL_MODE_VFD_VOICE                                = 3,
    MIPC_CALL_MODE_AVD_VOICE                                = 4,
    MIPC_CALL_MODE_AVF_VOICE                                = 5,
    MIPC_CALL_MODE_VFD_DATA                                 = 6,
    MIPC_CALL_MODE_AVD_DATA                                 = 7,
    MIPC_CALL_MODE_UNKNOWN                                  = 9,
    MIPC_CALL_MODE_IMS_VOICE_CALL                           = 20,
    MIPC_CALL_MODE_IMS_VIDEO_CALL                           = 21,
    MIPC_CALL_MODE_IMS_VOICE_CONFERENCE                     = 22,
    MIPC_CALL_MODE_IMS_VIDEO_CONFERENCE                     = 23,
    MIPC_CALL_MODE_IMS_VOICE_CONFERENCE_PARTICIPANT         = 24,
    MIPC_CALL_MODE_IMS_VIDEO_CONFERENCE_PARTICIPANT         = 25,
    MIPC_CALL_MODE_C2K_VOICE_CALL                           = 40,
    MIPC_CALL_MODE_C2K_OTASP_CALL_STD                       = 41,
    MIPC_CALL_MODE_C2K_OTASP_CALL_NO_STD                    = 42,
    MIPC_CALL_MODE_C2K_ECC_CALL                             = 43,
};
typedef enum mipc_call_mode_const_enum mipc_call_mode_const_enum;

enum mipc_call_dial_address_type_const_enum {
    mipc_call_dial_address_type_const_NONE = 0,
    /* CALL_DIAL_ADDRESS_TYPE */
    MIPC_CALL_DIAL_ADDRESS_TYPE_NONE                        = 0,
    MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI                     = 1,
    MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER                      = 2,
};
typedef enum mipc_call_dial_address_type_const_enum mipc_call_dial_address_type_const_enum;

enum mipc_call_audio_codec_const_enum {
    mipc_call_audio_codec_const_NONE = 0,
    /* CALL_AUDIO_CODEC */
    MIPC_CALL_AUDIO_CODEC_NONE                              = 0x0000,
    MIPC_CALL_AUDIO_CODEC_QCELP13K                          = 0x0001,
    MIPC_CALL_AUDIO_CODEC_EVRC                              = 0x0002,
    MIPC_CALL_AUDIO_CODEC_EVRC_B                            = 0x0003,
    MIPC_CALL_AUDIO_CODEC_EVRC_WB                           = 0x0004,
    MIPC_CALL_AUDIO_CODEC_EVRC_NW                           = 0x0005,
    MIPC_CALL_AUDIO_CODEC_AMR_NB                            = 0x0006,
    MIPC_CALL_AUDIO_CODEC_AMR_WB                            = 0x0007,
    MIPC_CALL_AUDIO_CODEC_GSM_EFR                           = 0x0008,
    MIPC_CALL_AUDIO_CODEC_GSM_FR                            = 0x0009,
    MIPC_CALL_AUDIO_CODEC_GSM_HR                            = 0x000a,
    MIPC_CALL_AUDIO_CODEC_EVS_NB                            = 0x0017,
    MIPC_CALL_AUDIO_CODEC_EVS_WB                            = 0x0018,
    MIPC_CALL_AUDIO_CODEC_EVS_SW                            = 0x0019,
    MIPC_CALL_AUDIO_CODEC_EVS_FB                            = 0x0020,
    MIPC_CALL_AUDIO_CODEC_EVS_AWB                           = 0x0021,
};
typedef enum mipc_call_audio_codec_const_enum mipc_call_audio_codec_const_enum;

enum mipc_sdp_audio_codec_const_enum {
    mipc_sdp_audio_codec_const_NONE = 0,
    /* SDP_AUDIO_CODEC */
    MIPC_SDP_AUDIO_CODEC_AMR                                = 1,
    MIPC_SDP_AUDIO_CODEC_AMR_WB                             = 2,
    MIPC_SDP_AUDIO_CODEC_EVS                                = 17,
};
typedef enum mipc_sdp_audio_codec_const_enum mipc_sdp_audio_codec_const_enum;

enum mipc_call_event_const_enum {
    mipc_call_event_const_NONE = 0,
    /* CALL_EVENT */
    /*  */
    MIPC_CALL_EVENT_MT_REJECTED                             = 1,
    /*  */
    MIPC_CALL_EVENT_MT_REDIRECT                             = 2,
    /*  */
    MIPC_CALL_EVENT_SRVCC                                   = 3,
    /*  */
    MIPC_CALL_EVENT_AUDIO_CODEC_CHANGE                      = 4,
    /*  */
    MIPC_CALL_EVENT_SPEECH_ATTACH                           = 5,
    /*  */
    MIPC_CALL_EVENT_CRING                                   = 6,
    /*  */
    MIPC_CALL_EVENT_RAT                                     = 7,
    /*  */
    MIPC_CALL_EVENT_NO_CARRIER                              = 8,
    /*  */
    MIPC_CALL_EVENT_ECONFSRVCC                              = 9,
};
typedef enum mipc_call_event_const_enum mipc_call_event_const_enum;

enum mipc_gwsd_event_const_enum {
    mipc_gwsd_event_const_NONE = 0,
    /* GWSD_EVENT */
    /*  */
    MIPC_GWSD_EVENT_ECRFCPY                                 = 1,
    /*  */
    MIPC_GWSD_EVENT_EICPGU                                  = 2,
    /*  */
    MIPC_GWSD_EVENT_EICPGRES                                = 3,
};
typedef enum mipc_gwsd_event_const_enum mipc_gwsd_event_const_enum;

enum mipc_call_msg_type_const_enum {
    mipc_call_msg_type_const_NONE = 0,
    /* CALL_MSG_TYPE */
    MIPC_CALL_MSG_TYPE_MT_CALL                              = 0,
    MIPC_CALL_MSG_TYPE_DISCONNECT                           = 1,
    MIPC_CALL_MSG_TYPE_ALERT                                = 2,
    MIPC_CALL_MSG_TYPE_CALL_PROCESS                         = 3,
    MIPC_CALL_MSG_TYPE_SYNC                                 = 4,
    MIPC_CALL_MSG_TYPE_PROGRESS                             = 5,
    MIPC_CALL_MSG_TYPE_CONNECTED                            = 6,
    MIPC_CALL_MSG_TYPE_ALL_CALLS_DISC                       = 129,
    MIPC_CALL_MSG_TYPE_CALL_ID_ASSIGN                       = 130,
    MIPC_CALL_MSG_TYPE_STATE_CHANGE_HELD                    = 131,
    MIPC_CALL_MSG_TYPE_STATE_CHANGE_ACTIVE                  = 132,
    MIPC_CALL_MSG_TYPE_STATE_CHANGE_DISCONNECTED            = 133,
    MIPC_CALL_MSG_TYPE_STATE_CHANGE_MO_DISCONNECTING        = 134,
    MIPC_CALL_MSG_TYPE_STATE_HELD_BY_REMOTE                 = 135,
    MIPC_CALL_MSG_TYPE_STATE_ACTIVE_BY_REMOTE               = 136,
};
typedef enum mipc_call_msg_type_const_enum mipc_call_msg_type_const_enum;

enum mipc_call_disconnected_cause_const_enum {
    mipc_call_disconnected_cause_const_NONE = 0,
    /* CALL_DISCONNECTED_CAUSE */
    /* MTK defined cause. Ask AP to redial the call as an emergency call. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_NON_UE_DETECT_EMERG = 380,
    /* MTK defined cause. ECT Timeout. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_ECT_TIMEOUT = 899,
    /* MTK defined cause. The IMS MO call was not responded and timeout. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_DEDICATED_BEARER_TIMEOUT = 1000,
    /* MTK defined cause. IMS suggest UE to retry the call over CS domain. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_GENERIC_RETRY_CS = 1001,
    /* MTK defined cause. IMS suggest UE not to retry the call. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_GENERIC_NO_RETRY = 1002,
    /* MTK defined cause. IMS suggest UE to retry the call over IMS first, then CSFB. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_GENERIC_RETRY_IMS_CS = 1003,
    /* MTK defined cause. Timer B (as per RFC 3261) timeout for IMS MO call. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_TIMER_B_EXPIRY = 1004,
    /* MTK defined cause. IMS suggest UE to retry the video call as voice call. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_GENERIC_RETRY_IMS_VIDEO_TO_VOICE = 1005,
    /* MTK defined cause. IMS suggest UE not to retry the call and bar IMS for a while. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_NO_RETRY_BAR_IMS = 1006,
    /* MTK defined cause. The IMS call cancelled during redirection failure (e.g., EPSFB fail). */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_REDIRECTION_FAILURE = 1007,
    /* MTK defined cause. IMS suggest UE to redial 3GPP2. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_SILENT_REDIAL_3GPP2 = 2001,
    /* MTK defined cause. IMS suggest UE to dial a 1xRTT call directly. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_DIAL_1XRTT_DIRECTLY = 2002,
    /* MTK defined cause. It requires global mode for the emergency call. */
    MIPC_CALL_DISCONNECTED_CAUSE_MS_EXT_ERROR_REPORT_NEED_GLOBAL_MODE_FOR_EMERG = 2003,
    /* MTK defined cause. SSAC barred. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_SSAC_BAR = 2004,
    /* MTK defined cause. The IMS emergency RTT call over network fails. */
    MIPC_CALL_DISCONNECTED_CAUSE_IMS_EXT_ERROR_REPORT_RTT_EMC_F = 2005,
};
typedef enum mipc_call_disconnected_cause_const_enum mipc_call_disconnected_cause_const_enum;

enum mipc_call_reject_reason_const_enum {
    mipc_call_reject_reason_const_NONE = 0,
    /* CALL_REJECT_REASON */
    MIPC_CALL_REJECT_REASON_UNDEFINED                       = 0,
};
typedef enum mipc_call_reject_reason_const_enum mipc_call_reject_reason_const_enum;

enum mipc_call_dial_type_const_enum {
    mipc_call_dial_type_const_NONE = 0,
    /* CALL_DIAL_TYPE */
    MIPC_CALL_DIAL_TYPE_VOICE                               = 1,
    MIPC_CALL_DIAL_TYPE_VIDEO                               = 2,
    MIPC_CALL_DIAL_TYPE_EMERGENCY                           = 3,
    MIPC_CALL_DIAL_TYPE_RTT                                 = 4,
};
typedef enum mipc_call_dial_type_const_enum mipc_call_dial_type_const_enum;

enum mipc_call_conference_dial_type_const_enum {
    mipc_call_conference_dial_type_const_NONE = 0,
    /* CALL_CONFERENCE_DIAL_TYPE */
    MIPC_CALL_CONFERENCE_DIAL_TYPE_VOICE                    = 1,
    MIPC_CALL_CONFERENCE_DIAL_TYPE_VIDEO                    = 2,
};
typedef enum mipc_call_conference_dial_type_const_enum mipc_call_conference_dial_type_const_enum;

enum mipc_call_dial_domain_const_enum {
    mipc_call_dial_domain_const_NONE = 0,
    /* CALL_DIAL_DOMAIN */
    /* Automatic - NR(IMS), LTE(IMS), WG(CS), 1x(C2K) */
    MIPC_CALL_DIAL_DOMAIN_AUTO                              = 0,
    /* CS only - WG(CS) */
    MIPC_CALL_DIAL_DOMAIN_CS_ONLY                           = 1,
    /* 3GPP only - NR(IMS), LTE(IMS), WG(CS) */
    MIPC_CALL_DIAL_DOMAIN_3GPP_ONLY                         = 2,
    /* 3GPP2 only - 1x(C2K) */
    MIPC_CALL_DIAL_DOMAIN_3GPP2_ONLY                        = 3,
    /* IMS and 3GPP2 only - NR(IMS), LTE(IMS), 1x(C2K) */
    MIPC_CALL_DIAL_DOMAIN_IMS_AND_3GPP2_ONLY                = 4,
    /* 3GPP CS and 3GPP2 only - WG(CS), 1x(C2K) */
    MIPC_CALL_DIAL_DOMAIN_3GPP_CS_AND_3GPP2_ONLY            = 5,
    /* IMS only - NR(IMS), LTE(IMS) */
    MIPC_CALL_DIAL_DOMAIN_IMS_ONLY                          = 6,
};
typedef enum mipc_call_dial_domain_const_enum mipc_call_dial_domain_const_enum;

enum mipc_call_clir_mode_const_enum {
    mipc_call_clir_mode_const_NONE = 0,
    /* CALL_CLIR_MODE */
    /* subscription default */
    MIPC_CALL_CLIR_MODE_DEFAULT                             = 0,
    /* invocation mode */
    MIPC_CALL_CLIR_MODE_INVOCATION                          = 1,
    /* suppression mode */
    MIPC_CALL_CLIR_MODE_SUPPRESSION                         = 2,
};
typedef enum mipc_call_clir_mode_const_enum mipc_call_clir_mode_const_enum;

enum mipc_call_ss_action_const_enum {
    mipc_call_ss_action_const_NONE = 0,
    /* CALL_SS_ACTION */
    /* release all held calls, or set User-Determined User Busy for a waiting call */
    MIPC_CALL_SS_ACTION_RELEASE_ALL_HELD_OR_WAITING_CALL    = 0,
    /* release all active calls and accept the other(waiting or held) call */
    MIPC_CALL_SS_ACTION_RELEASE_ALL_ACTIVE_AND_ACCEPT_CALL  = 1,
    /* places all active call on hold and accept the other(held or waiting) call */
    MIPC_CALL_SS_ACTION_PLACE_ALL_ACTIVE_CALL_ON_HOLD_AND_ACCEPT_CALL = 2,
    /* Connects two calls and siconnects the subscriber from both calls */
    MIPC_CALL_SS_ACTION_EXPLICIT_CALL_AND_TRANSFER          = 4,
    /* Activate the Completion of Calls to Busy Subscriber Request. (CCBS) */
    MIPC_CALL_SS_ACTION_COMPLETION_CALL_BUSY_SUBSCRIBER     = 5,
    /* change a specific call to on-hold (only the IMS call support this feature) */
    MIPC_CALL_SS_ACTION_HOLD_CALL                           = 131,
    /* change a specific call to active (only the IMS call support this feature) */
    MIPC_CALL_SS_ACTION_RESUME_CALL                         = 132,
};
typedef enum mipc_call_ss_action_const_enum mipc_call_ss_action_const_enum;

enum mipc_call_conf_action_const_enum {
    mipc_call_conf_action_const_NONE = 0,
    /* CALL_CONF_ACTION */
    /* merges the call to the conversation */
    MIPC_CALL_CONF_ACTION_MERGE                             = 0,
    /* adds a call to the conversation */
    MIPC_CALL_CONF_ACTION_ADD_PARTICIPANT                   = 1,
    /* remove a call from the conversation */
    MIPC_CALL_CONF_ACTION_REMOVE_PARTICIPANT                = 2,
    /* split a party from the CS MTPY(conference), for cs only */
    MIPC_CALL_CONF_ACTION_SPLIT                             = 3,
    /* manage 3-way calls, for cdma only */
    MIPC_CALL_CONF_ACTION_CDMA_FLASH                        = 4,
};
typedef enum mipc_call_conf_action_const_enum mipc_call_conf_action_const_enum;

enum mipc_set_gwsd_mode_action_const_enum {
    mipc_set_gwsd_mode_action_const_NONE = 0,
    /* SET_GWSD_MODE_ACTION */
    /* set DSDA-like mode */
    MIPC_SET_GWSD_MODE_ACTION_SET_DSDA_LIKE_MODE            = 1,
    /* set single card GWSD mode(CSFB) */
    MIPC_SET_GWSD_MODE_ACTION_SET_SINGLE_CARD_GWSD_MODE     = 2,
};
typedef enum mipc_set_gwsd_mode_action_const_enum mipc_set_gwsd_mode_action_const_enum;

enum mipc_call_hangup_mode_const_enum {
    mipc_call_hangup_mode_const_NONE = 0,
    /* CALL_HANGUP_MODE */
    /*  */
    MIPC_CALL_HANGUP_MODE_HANGUP                            = 0,
    /*  */
    MIPC_CALL_HANGUP_MODE_HANGUP_ALL                        = 1,
    /*  */
    MIPC_CALL_HANGUP_MODE_FORCE_HANGUP                      = 2,
};
typedef enum mipc_call_hangup_mode_const_enum mipc_call_hangup_mode_const_enum;

enum mipc_call_hangup_cause_const_enum {
    mipc_call_hangup_cause_const_NONE = 0,
    /* CALL_HANGUP_CAUSE */
    MIPC_CALL_HANGUP_CAUSE_USER_HANGUP                      = 0,
    MIPC_CALL_HANGUP_CAUSE_IMS_NO_COVERAGE                  = 1,
    MIPC_CALL_HANGUP_CAUSE_IS_LOW_BATTERY                   = 2,
    MIPC_CALL_HANGUP_CAUSE_IMS_FORWARD                      = 3,
    MIPC_CALL_HANGUP_CAUSE_IMS_SPECIAL_HUNGUP               = 4,
    MIPC_CALL_HANGUP_CAUSE_IMS_607_UNWANTED                 = 5,
    MIPC_CALL_HANGUP_CAUSE_CS_INCOMING_REJECTED_NO_FORWARD  = 6,
};
typedef enum mipc_call_hangup_cause_const_enum mipc_call_hangup_cause_const_enum;

enum mipc_call_answer_mode_const_enum {
    mipc_call_answer_mode_const_NONE = 0,
    /* CALL_ANSWER_MODE */
    /*  */
    MIPC_CALL_ANSWER_MODE_DEFAULT                           = 0,
    /* accepts as audio call */
    MIPC_CALL_ANSWER_MODE_AUDIO_CALL                        = 1,
    /* accepts as one way only video call (Rx) */
    MIPC_CALL_ANSWER_MODE_Rx_VIDEO_CALL                     = 2,
    /* accepts as one way only video call (Tx) */
    MIPC_CALL_ANSWER_MODE_Tx_VIDEO_CALL                     = 3,
};
typedef enum mipc_call_answer_mode_const_enum mipc_call_answer_mode_const_enum;

enum mipc_call_ss_code1_const_enum {
    mipc_call_ss_code1_const_NONE = 0,
    /* CALL_SS_CODE1 */
    /* unconditional call forwarding is active */
    MIPC_CALL_SS_CODE1_UCF_ACTIVE                           = 0,
    /* some of the conditional call forwardings are active */
    MIPC_CALL_SS_CODE1_CCF_ACTIVE                           = 1,
    /* call has been forwarded */
    MIPC_CALL_SS_CODE1_FORWARDED_CALL                       = 2,
    /* call is waiting */
    MIPC_CALL_SS_CODE1_WAITING_CALL                         = 3,
    /* this is a CUG call (also <index> present) */
    MIPC_CALL_SS_CODE1_CUG_CALL                             = 4,
    /* outgoing calls are barred */
    MIPC_CALL_SS_CODE1_OUTGOING_CALLS_ARE_BARRED            = 5,
    /* incoming calls are barred */
    MIPC_CALL_SS_CODE1_INCOMING_CALLS_ARE_BARRED            = 6,
    /* CLIR suppression rejected */
    MIPC_CALL_SS_CODE1_CLIR_SUPPRESSION_REJECTED            = 7,
    /* call has been deflected */
    MIPC_CALL_SS_CODE1_DEFLECTED_CALL                       = 8,
    /* called party is idle for video ringtone */
    MIPC_CALL_SS_CODE1_CALLED_PARTY_IS_IDLE_FOR_VIDEO_RINGTONE = 9,
    /* china mobile online service */
    MIPC_CALL_SS_CODE1_CMOS                                 = 12,
};
typedef enum mipc_call_ss_code1_const_enum mipc_call_ss_code1_const_enum;

enum mipc_call_ss_code2_const_enum {
    mipc_call_ss_code2_const_NONE = 0,
    /* CALL_SS_CODE2 */
    /* this is a forwarded call (MT call setup) */
    MIPC_CALL_SS_CODE2_FORWARDED_CALL                       = 0,
    /* this is a CUG call (also <index> present) (MT call setup) */
    MIPC_CALL_SS_CODE2_CUG_CALL                             = 1,
    /* call has been put on hold (during a voice call) */
    MIPC_CALL_SS_CODE2_HOLD_CALL                            = 2,
    /* call has been retrieved (during a voice call) */
    MIPC_CALL_SS_CODE2_RETRIEVED_CALL                       = 3,
    /* multiparty call entered (during a voice call) */
    MIPC_CALL_SS_CODE2_MULTIPARTY_CALL                      = 4,
    /* call on hold has been released (this is not a SS notification) (during a voice call) */
    MIPC_CALL_SS_CODE2_HOLD_CALL_HAS_BEEN_RELEASEED         = 5,
    /* forward check SS message received (can be received whenever) */
    MIPC_CALL_SS_CODE2_FORWARD_CHECK_SS_MSG_RECEIVED        = 6,
    /* all is being connected (alerting) with the remote party in alerting state in explicit call transfer operation (during a voice call) */
    MIPC_CALL_SS_CODE2_CONNECTED_CALL_WITH_REMOTE_PARTY_IN_ALERTING = 7,
    /* call has been connected with the other remote party in explicit call transfer operation (also number and subaddress parameters may be present) (during a voice call or MT call setup) */
    MIPC_CALL_SS_CODE2_CONNECTED_CALL_WITH_REMOTE_PARTY_IN_EXPLICIT_CALL_TRANSFER = 8,
    /* this is a deflected call (MT call setup) */
    MIPC_CALL_SS_CODE2_DEFLECTED_CALL                       = 9,
    /* additional incoming call forwarded */
    MIPC_CALL_SS_CODE2_ADDITIONAL_INCOMING_CALL_FORWARDED   = 10,
    /* Call Forwarded */
    MIPC_CALL_SS_CODE2_CALL_FORWARDED                       = 11,
    /* Call Forwarded Unconditional */
    MIPC_CALL_SS_CODE2_CALL_FORWARDED_UNCONDITIONAL         = 12,
    /* Call Forwarded Conditional */
    MIPC_CALL_SS_CODE2_CALL_FORWARDED_CONDITIONAL           = 13,
    /* Call Busy Forwarded */
    MIPC_CALL_SS_CODE2_CALL_BUSY_FORWARDED                  = 14,
    /* Call Forwarded on No Reply */
    MIPC_CALL_SS_CODE2_CALL_FORWARDED_ON_NO_REPLY           = 15,
    /* Call Forwarded on Not Reachable */
    MIPC_CALL_SS_CODE2_CALL_FORWARDED_ON_NOT_REACHABLE      = 16,
};
typedef enum mipc_call_ss_code2_const_enum mipc_call_ss_code2_const_enum;

enum mipc_emergency_call_s1_support_const_enum {
    mipc_emergency_call_s1_support_const_NONE = 0,
    /* EMERGENCY_CALL_S1_SUPPORT */
    MIPC_EMERGENCY_CALL_S1_SUPPORT_S1_MODE_NOT_SUPPORT      = 0,
    MIPC_EMERGENCY_CALL_S1_SUPPORT_S1_MODE_SUPPORT          = 1,
};
typedef enum mipc_emergency_call_s1_support_const_enum mipc_emergency_call_s1_support_const_enum;

enum mipc_emergency_call_rat_const_enum {
    mipc_emergency_call_rat_const_NONE = 0,
    /* EMERGENCY_CALL_RAT */
    MIPC_EMERGENCY_CALL_RAT_GSM                             = 0,
    MIPC_EMERGENCY_CALL_RAT_WCDMA                           = 1,
    MIPC_EMERGENCY_CALL_RAT_TD_SCDMA                        = 2,
    MIPC_EMERGENCY_CALL_RAT_FDD_LTE                         = 3,
    MIPC_EMERGENCY_CALL_RAT_TDD_LTE                         = 4,
};
typedef enum mipc_emergency_call_rat_const_enum mipc_emergency_call_rat_const_enum;

enum mipc_emergency_call_support_emc_const_enum {
    mipc_emergency_call_support_emc_const_NONE = 0,
    /* EMERGENCY_CALL_SUPPORT_EMC */
    /* Current cell does not support emergency service */
    MIPC_EMERGENCY_CALL_SUPPORT_EMC_NOT_SUPPORT             = 0,
    /* Current cell support emergency servicee */
    MIPC_EMERGENCY_CALL_SUPPORT_EMC_SUPPORT                 = 1,
};
typedef enum mipc_emergency_call_support_emc_const_enum mipc_emergency_call_support_emc_const_enum;

enum mipc_emergency_call_emb_iu_supp_const_enum {
    mipc_emergency_call_emb_iu_supp_const_NONE = 0,
    /* EMERGENCY_CALL_EMB_IU_SUPP */
    MIPC_EMERGENCY_CALL_EMB_IU_SUPP_NOT_SUPPORT_LU_MODE_AND_A_GB_MODE = 0,
    MIPC_EMERGENCY_CALL_EMB_IU_SUPP_ONLY_SUPPORT_LU_MODE    = 1,
};
typedef enum mipc_emergency_call_emb_iu_supp_const_enum mipc_emergency_call_emb_iu_supp_const_enum;

enum mipc_emergency_call_ems_5g_supp_const_enum {
    mipc_emergency_call_ems_5g_supp_const_NONE = 0,
    /* EMERGENCY_CALL_EMS_5G_SUPP */
    MIPC_EMERGENCY_CALL_EMS_5G_SUPP_EMC_NOT_SUPPORT         = 0,
    MIPC_EMERGENCY_CALL_EMS_5G_SUPP_EMC_SUPPORT_NR_5GCN_ONLY = 1,
    MIPC_EMERGENCY_CALL_EMS_5G_SUPP_EMC_SUPPORT_EUTRA_5GCN_ONLY  = 2,
    MIPC_EMERGENCY_CALL_EMS_5G_SUPP_EMC_SUPPORT_NR_5GCN_EUTRA_5GCN = 3,
};
typedef enum mipc_emergency_call_ems_5g_supp_const_enum mipc_emergency_call_ems_5g_supp_const_enum;

enum mipc_emergency_call_emf_5g_supp_const_enum {
    mipc_emergency_call_emf_5g_supp_const_NONE = 0,
    /* EMERGENCY_CALL_EMF_5G_SUPP */
    MIPC_EMERGENCY_CALL_EMF_5G_SUPP_EMC_FALLBACK_NOT_SUPPORT = 0,
    MIPC_EMERGENCY_CALL_EMF_5G_SUPP_EMC_FALLBACK_SUPPORT_NR_5GCN_ONLY = 1,
    MIPC_EMERGENCY_CALL_EMF_5G_SUPP_EMC_FALLBACK_SUPPORT_EUTRA_5GCN_ONLY  = 2,
    MIPC_EMERGENCY_CALL_EMF_5G_SUPP_EMC_FALLBACK_SUPPORT_NR_5GCN_EUTRA_5GCN = 3,
};
typedef enum mipc_emergency_call_emf_5g_supp_const_enum mipc_emergency_call_emf_5g_supp_const_enum;

enum mipc_call_uis_info_type_const_enum {
    mipc_call_uis_info_type_const_NONE = 0,
    /* CALL_UIS_INFO_TYPE */
    MIPC_CALL_UIS_INFO_TYPE_BOTTON_CLICKING                 = 1,
};
typedef enum mipc_call_uis_info_type_const_enum mipc_call_uis_info_type_const_enum;

enum mipc_call_uis_info_result_const_enum {
    mipc_call_uis_info_result_const_NONE = 0,
    /* CALL_UIS_INFO_RESULT */
    MIPC_CALL_UIS_INFO_RESULT_SUCCESS                       = 0,
    MIPC_CALL_UIS_INFO_RESULT_FAILURE                       = 1,
};
typedef enum mipc_call_uis_info_result_const_enum mipc_call_uis_info_result_const_enum;

enum mipc_call_uis_info_cause_const_enum {
    mipc_call_uis_info_cause_const_NONE = 0,
    /* CALL_UIS_INFO_CAUSE */
    MIPC_CALL_UIS_INFO_CAUSE_NONE                           = 0,
    MIPC_CALL_UIS_INFO_CAUSE_CALL_NOT_EXIST                 = 1,
    MIPC_CALL_UIS_INFO_CAUSE_DATA_IS_NULL                   = 2,
    MIPC_CALL_UIS_INFO_CAUSE_LACL_OF_MEMORY                 = 3,
    MIPC_CALL_UIS_INFO_CAUSE_SIP_ERROR                      = 4,
};
typedef enum mipc_call_uis_info_cause_const_enum mipc_call_uis_info_cause_const_enum;

enum mipc_call_prefer_set_state_const_enum {
    mipc_call_prefer_set_state_const_NONE = 0,
    /* CALL_PREFER_SET_STATE */
    MIPC_CALL_PREFER_SET_STATE_DATA_PREFER                  = 0,
    MIPC_CALL_PREFER_SET_STATE_CALL_PREFER                  = 1,
};
typedef enum mipc_call_prefer_set_state_const_enum mipc_call_prefer_set_state_const_enum;

enum mipc_call_prefer_set_monitor_mode_const_enum {
    mipc_call_prefer_set_monitor_mode_const_NONE = 0,
    /* CALL_PREFER_SET_MONITOR_MODE */
    /* DO NOT monitor PCH of other SIM when this SIM is in data transfer */
    MIPC_CALL_PREFER_SET_MONITOR_MODE_NOT_MONITOR           = 0,
    /* monitor PCH of other SIM when this SIM in data transfer */
    MIPC_CALL_PREFER_SET_MONITOR_MODE_MONITOR               = 1,
};
typedef enum mipc_call_prefer_set_monitor_mode_const_enum mipc_call_prefer_set_monitor_mode_const_enum;

enum mipc_call_reject_cause_const_enum {
    mipc_call_reject_cause_const_NONE = 0,
    /* CALL_REJECT_CAUSE */
    MIPC_CALL_REJECT_CAUSE_NORMAL                           = 0,
    MIPC_CALL_REJECT_CAUSE_ABNORMAL                         = 1,
    MIPC_CALL_REJECT_CAUSE_NONUMBER                         = 2,
    MIPC_CALL_REJECT_CAUSE_UNREGISTERED                     = 3,
    MIPC_CALL_REJECT_CAUSE_DISCONNECT                       = 4,
    MIPC_CALL_REJECT_CAUSE_BECANCEL                         = 5,
    MIPC_CALL_REJECT_CAUSE_BEREJECT                         = 6,
    MIPC_CALL_REJECT_CAUSE_BEREJECT_380                     = 7,
    MIPC_CALL_REJECT_CAUSE_BEREJECT_380_EMERGENCY_TO_CS     = 8,
    MIPC_CALL_REJECT_CAUSE_BEREJECT_380_EMERGENCY_REREG     = 9,
    MIPC_CALL_REJECT_CAUSE_BEREJECT_480                     = 10,
    MIPC_CALL_REJECT_CAUSE_BEREJECT_503                     = 11,
    MIPC_CALL_REJECT_CAUSE_TX_TIMEOUT                       = 12,
    MIPC_CALL_REJECT_CAUSE_NO_PRIVACY                       = 13,
    MIPC_CALL_REJECT_CAUSE_PRECONDFAIL                      = 14,
    MIPC_CALL_REJECT_CAUSE_SECONDCALL                       = 15,
    MIPC_CALL_REJECT_CAUSE_NOMEDIACONTENT                   = 16,
    MIPC_CALL_REJECT_CAUSE_INVALIDMSG                       = 17,
    MIPC_CALL_REJECT_CAUSE_INVALIDCMD                       = 18,
    MIPC_CALL_REJECT_CAUSE_INVALIDIDX                       = 19,
    MIPC_CALL_REJECT_CAUSE_SRVCC                            = 20,
    MIPC_CALL_REJECT_CAUSE_FACILITY_REJECTED                = 21,
    MIPC_CALL_REJECT_CAUSE_DEDICATED_BEARER_TIMEOUT         = 22,
    MIPC_CALL_REJECT_CAUSE_UA_MAX                           = 23,
    MIPC_CALL_REJECT_CAUSE_UNAVAILABLE_OF_CALLS             = 24,
    MIPC_CALL_REJECT_CAUSE_IMS_CC_CAUSE_MAX                 = 25,
    MIPC_CALL_REJECT_CAUSE_UNKNOWN                          = 65535,
};
typedef enum mipc_call_reject_cause_const_enum mipc_call_reject_cause_const_enum;

enum mipc_conf_participant_status_const_enum {
    mipc_conf_participant_status_const_NONE = 0,
    /* CONF_PARTICIPANT_STATUS */
    MIPC_CONF_PARTICIPANT_STATUS_PENDING                    = 0,
    MIPC_CONF_PARTICIPANT_STATUS_DIALING_OUT                = 1,
    MIPC_CONF_PARTICIPANT_STATUS_DIALING_IN                 = 2,
    MIPC_CONF_PARTICIPANT_STATUS_ALERTING                   = 3,
    MIPC_CONF_PARTICIPANT_STATUS_ON_HOLD                    = 4,
    MIPC_CONF_PARTICIPANT_STATUS_CONNECTED                  = 5,
    MIPC_CONF_PARTICIPANT_STATUS_DISCONNECTING              = 6,
    MIPC_CONF_PARTICIPANT_STATUS_DISCONNECTED               = 7,
    MIPC_CONF_PARTICIPANT_STATUS_MUTED_VIA_FOCUS            = 8,
    MIPC_CONF_PARTICIPANT_STATUS_CONNECT_FAIL               = 9,
};
typedef enum mipc_conf_participant_status_const_enum mipc_conf_participant_status_const_enum;

enum mipc_sip_direction_const_enum {
    mipc_sip_direction_const_NONE = 0,
    /* SIP_DIRECTION */
    MIPC_SIP_DIRECTION_SEND                                 = 0,
    MIPC_SIP_DIRECTION_RECEIVE                              = 1,
};
typedef enum mipc_sip_direction_const_enum mipc_sip_direction_const_enum;

enum mipc_sip_msg_type_const_enum {
    mipc_sip_msg_type_const_NONE = 0,
    /* SIP_MSG_TYPE */
    MIPC_SIP_MSG_TYPE_REQUEST                               = 0,
    MIPC_SIP_MSG_TYPE_RESPONSE                              = 1,
};
typedef enum mipc_sip_msg_type_const_enum mipc_sip_msg_type_const_enum;

enum mipc_sip_method_const_enum {
    mipc_sip_method_const_NONE = 0,
    /* SIP_METHOD */
    MIPC_SIP_METHOD_INVITE                                  = 1,
    MIPC_SIP_METHOD_REFER                                   = 2,
    MIPC_SIP_METHOD_UPDATE                                  = 3,
    MIPC_SIP_METHOD_CANCEL                                  = 4,
    MIPC_SIP_METHOD_MESSAGE                                 = 5,
    MIPC_SIP_METHOD_ACK                                     = 6,
    MIPC_SIP_METHOD_BYE                                     = 7,
    MIPC_SIP_METHOD_OPTIONS                                 = 8,
    MIPC_SIP_METHOD_SUBSCRIBE                               = 9,
    MIPC_SIP_METHOD_NOTIFY                                  = 10,
    MIPC_SIP_METHOD_PUBLISH                                 = 11,
    MIPC_SIP_METHOD_INFO                                    = 12,
    MIPC_SIP_METHOD_PRACK                                   = 13,
};
typedef enum mipc_sip_method_const_enum mipc_sip_method_const_enum;

enum mipc_ims_event_package_type_const_enum {
    mipc_ims_event_package_type_const_NONE = 0,
    /* IMS_EVENT_PACKAGE_TYPE */
    MIPC_IMS_EVENT_PACKAGE_TYPE_CONFERENCE                  = 1,
    MIPC_IMS_EVENT_PACKAGE_TYPE_DIALOG                      = 2,
    MIPC_IMS_EVENT_PACKAGE_TYPE_MWI                         = 3,
    MIPC_IMS_EVENT_PACKAGE_TYPE_DTMF_GEN                    = 100,
};
typedef enum mipc_ims_event_package_type_const_enum mipc_ims_event_package_type_const_enum;

enum mipc_ecc_info_type_const_enum {
    mipc_ecc_info_type_const_NONE = 0,
    /* ECC_INFO_TYPE */
    MIPC_ECC_INFO_TYPE_HOST                                 = 0,
    MIPC_ECC_INFO_TYPE_SIM                                  = 1,
    MIPC_ECC_INFO_TYPE_MCF                                  = 2,
    MIPC_ECC_INFO_TYPE_NW                                   = 3,
    MIPC_ECC_INFO_TYPE_SPEC                                 = 4,
    MIPC_ECC_INFO_TYPE_N3GPP_IKEV2                          = 5,
    MIPC_ECC_INFO_TYPE_N3GPP_DNS                            = 6,
    MIPC_ECC_INFO_TYPE_NW_EXT                               = 7,
};
typedef enum mipc_ecc_info_type_const_enum mipc_ecc_info_type_const_enum;

enum mipc_dtmf_mode_const_enum {
    mipc_dtmf_mode_const_NONE = 0,
    /* DTMF_MODE */
    /*  */
    MIPC_DTMF_MODE_START                                    = 0,
    /*  */
    MIPC_DTMF_MODE_STOP                                     = 1,
    /*  */
    MIPC_DTMF_MODE_SINGLE_TONE                              = 2,
    /* this mode is only used in CDMA call */
    MIPC_DTMF_MODE_BURST_TONE                               = 3,
};
typedef enum mipc_dtmf_mode_const_enum mipc_dtmf_mode_const_enum;

enum mipc_recv_dtmf_mode_const_enum {
    mipc_recv_dtmf_mode_const_NONE = 0,
    /* RECV_DTMF_MODE */
    /*  */
    MIPC_RECV_DTMF_MODE_START                               = 0,
    /*  */
    MIPC_RECV_DTMF_MODE_STOP                                = 1,
    /*  */
    MIPC_RECV_DTMF_MODE_STOOP_ALL                           = 2,
};
typedef enum mipc_recv_dtmf_mode_const_enum mipc_recv_dtmf_mode_const_enum;

enum mipc_call_ecbm_mode_const_enum {
    mipc_call_ecbm_mode_const_NONE = 0,
    /* CALL_ECBM_MODE */
    /* ecbm mode off */
    MIPC_CALL_ECBM_MODE_ECBM_OFF                            = 0,
    /* ecbm mode on */
    MIPC_CALL_ECBM_MODE_ECBM_ON                             = 1,
    /* no ecbm mode */
    MIPC_CALL_ECBM_MODE_NO_ECBM                             = 2,
};
typedef enum mipc_call_ecbm_mode_const_enum mipc_call_ecbm_mode_const_enum;

enum mipc_call_cli_validity_const_enum {
    mipc_call_cli_validity_const_NONE = 0,
    /* CALL_CLI_VALIDITY */
    /* CLI VALID */
    MIPC_CALL_CLI_VALIDITY_CLI_VALID                        = 0,
    /* CLI has been with held by the originator */
    MIPC_CALL_CLI_VALIDITY_CLI_WITHHELD                     = 1,
    /* CLI is not valid due to interworking problems or limitations of originating network */
    MIPC_CALL_CLI_VALIDITY_CLI_INTERWORK                    = 2,
    /* CLI is not valid due to calling party being of type payphone */
    MIPC_CALL_CLI_VALIDITY_CLI_PAYPHONE                     = 3,
    /* CLI is not valid due to other reasons */
    MIPC_CALL_CLI_VALIDITY_CLI_OTHERS                       = 4,
};
typedef enum mipc_call_cli_validity_const_enum mipc_call_cli_validity_const_enum;

enum mipc_call_cni_validity_const_enum {
    mipc_call_cni_validity_const_NONE = 0,
    /* CALL_CNI_VALIDITY */
    /* CNI VALID */
    MIPC_CALL_CNI_VALIDITY_CNI_VALID                        = 0,
    /* CNI has been with held by the originator */
    MIPC_CALL_CNI_VALIDITY_CNI_WITHHELD                     = 1,
    /* CNI is not valid due to interworking problems or limitations of originating network */
    MIPC_CALL_CNI_VALIDITY_CNI_INTERWORK                    = 2,
    /* CNI is not valid due to calling party being of type payphone */
    MIPC_CALL_CNI_VALIDITY_CNI_PAYPHONE                     = 3,
    /* CNI is not valid due to other reasons */
    MIPC_CALL_CNI_VALIDITY_CNI_OTHERS                       = 4,
};
typedef enum mipc_call_cni_validity_const_enum mipc_call_cni_validity_const_enum;

enum mipc_crss_type_const_enum {
    mipc_crss_type_const_NONE = 0,
    /* CRSS_TYPE */
    /* 3GPP Call Waiting  */
    MIPC_CRSS_TYPE_CRSS_CALL_WAITING                        = 0,
    /* 3GPP Calling line dentification presentation */
    MIPC_CRSS_TYPE_CRSS_CALLING_LINE_ID_PRESET              = 1,
    /* 3GPP Called line identification presentation */
    MIPC_CRSS_TYPE_CRSS_CALLED_LINE_ID_PRESET               = 2,
    /* 3GPP connected line dentification presentation */
    MIPC_CRSS_TYPE_CRSS_CONNECTED_LINE_ID_PRESET            = 3,
    /* 3GPP2 Call Waiting  */
    MIPC_CRSS_TYPE_CDMA_CALL_WAITING                        = 4,
    /* 3GPP2 Calling line dentification presentation */
    MIPC_CRSS_TYPE_CDMA_CALLING_LINE_ID_PRESET              = 5,
};
typedef enum mipc_crss_type_const_enum mipc_crss_type_const_enum;

enum mipc_number_presentation_const_enum {
    mipc_number_presentation_const_NONE = 0,
    /* NUMBER_PRESENTATION */
    /* alowed */
    MIPC_NUMBER_PRESENTATION_ALLOWED                        = 0,
    /* restricted */
    MIPC_NUMBER_PRESENTATION_RESTRICTED                     = 1,
    /* unknown or not specified */
    MIPC_NUMBER_PRESENTATION_UNKNOWN                        = 2,
};
typedef enum mipc_number_presentation_const_enum mipc_number_presentation_const_enum;

enum mipc_number_type_const_enum {
    mipc_number_type_const_NONE = 0,
    /* NUMBER_TYPE */
    /* unknown */
    MIPC_NUMBER_TYPE_UNKNOWN                                = 0,
    /* international call */
    MIPC_NUMBER_TYPE_INTERNATIONAL                          = 1,
    /* national call */
    MIPC_NUMBER_TYPE_NATIONAL                               = 2,
};
typedef enum mipc_number_type_const_enum mipc_number_type_const_enum;

enum mipc_call_ect_type_const_enum {
    mipc_call_ect_type_const_NONE = 0,
    /* CALL_ECT_TYPE */
    /* NVRAM default configuration is used, maybe IMS_BLIND_ECT or IMS_ASSURED_ECT */
    MIPC_CALL_ECT_TYPE_IMS_DEFAULT                          = 0,
    /* triggers IMS explicit call transfer with ECT type blind */
    MIPC_CALL_ECT_TYPE_IMS_BLIND_ECT                        = 1,
    /* triggers IMS explicit call transfer with ECT type assured */
    MIPC_CALL_ECT_TYPE_IMS_ASSURED_ECT                      = 2,
    /* Connects two calls and dsiconnects the subscriber from both calls */
    MIPC_CALL_ECT_TYPE_CONSULTATIVE_ECT                     = 3,
};
typedef enum mipc_call_ect_type_const_enum mipc_call_ect_type_const_enum;

enum mipc_call_cipher_on_status_const_enum {
    mipc_call_cipher_on_status_const_NONE = 0,
    /* CALL_CIPHER_ON_STATUS */
    MIPC_CALL_CIPHER_ON_STATUS_NO_CIPHER                    = 0,
    MIPC_CALL_CIPHER_ON_STATUS_CIPHER                       = 1,
    MIPC_CALL_CIPHER_ON_STATUS_UNKNOWN                      = 255,
};
typedef enum mipc_call_cipher_on_status_const_enum mipc_call_cipher_on_status_const_enum;

enum mipc_call_additional_info_mode_const_enum {
    mipc_call_additional_info_mode_const_NONE = 0,
    /* CALL_ADDITIONAL_INFO_MODE */
    MIPC_CALL_ADDITIONAL_INFO_MODE_ORIGINAL_IMS_CALL        = 1,
    MIPC_CALL_ADDITIONAL_INFO_MODE_CLIENT_API               = 2,
    MIPC_CALL_ADDITIONAL_INFO_MODE_ENRICHED_CALLING         = 3,
};
typedef enum mipc_call_additional_info_mode_const_enum mipc_call_additional_info_mode_const_enum;

enum mipc_call_additional_info_type_const_enum {
    mipc_call_additional_info_type_const_NONE = 0,
    /* CALL_ADDITIONAL_INFO_TYPE */
    MIPC_CALL_ADDITIONAL_INFO_TYPE_HEADER                   = 1,
    MIPC_CALL_ADDITIONAL_INFO_TYPE_LOCATION                 = 2,
};
typedef enum mipc_call_additional_info_type_const_enum mipc_call_additional_info_type_const_enum;

enum mipc_call_peer_rtt_modify_result_const_enum {
    mipc_call_peer_rtt_modify_result_const_NONE = 0,
    /* CALL_PEER_RTT_MODIFY_RESULT */
    MIPC_CALL_PEER_RTT_MODIFY_RESULT_ACCEPT                 = 0,
    MIPC_CALL_PEER_RTT_MODIFY_RESULT_REJECT                 = 1,
};
typedef enum mipc_call_peer_rtt_modify_result_const_enum mipc_call_peer_rtt_modify_result_const_enum;

enum mipc_call_local_rtt_modify_op_const_enum {
    mipc_call_local_rtt_modify_op_const_NONE = 0,
    /* CALL_LOCAL_RTT_MODIFY_OP */
    MIPC_CALL_LOCAL_RTT_MODIFY_OP_DOWNGRADE                 = 0,
    MIPC_CALL_LOCAL_RTT_MODIFY_OP_UPGRADE                   = 1,
};
typedef enum mipc_call_local_rtt_modify_op_const_enum mipc_call_local_rtt_modify_op_const_enum;

enum mipc_call_rtt_mode_op_const_enum {
    mipc_call_rtt_mode_op_const_NONE = 0,
    /* CALL_RTT_MODE_OP */
    /* disable IMS RTT capability */
    MIPC_CALL_RTT_MODE_OP_DISABLE                           = 0,
    /* enable IMS RTT capability with Automatic RTT Operation Mode.  */
    MIPC_CALL_RTT_MODE_OP_AUTOMATIC                         = 1,
    /* enable IMS RTT capability with Upon Request RTT Operation Mode */
    MIPC_CALL_RTT_MODE_OP_UPON_REQUEST                      = 2,
};
typedef enum mipc_call_rtt_mode_op_const_enum mipc_call_rtt_mode_op_const_enum;

enum mipc_call_rtt_audio_type_const_enum {
    mipc_call_rtt_audio_type_const_NONE = 0,
    /* CALL_RTT_AUDIO_TYPE */
    MIPC_CALL_RTT_AUDIO_TYPE_SILENCE                        = 0,
    MIPC_CALL_RTT_AUDIO_TYPE_SPEECH                         = 1,
};
typedef enum mipc_call_rtt_audio_type_const_enum mipc_call_rtt_audio_type_const_enum;

enum mipc_call_rcs_state_const_enum {
    mipc_call_rcs_state_const_NONE = 0,
    /* CALL_RCS_STATE */
    MIPC_CALL_RCS_STATE_DISABLE                             = 0,
    MIPC_CALL_RCS_STATE_ENABLE                              = 1,
    /* RCS needs IMS doing re-registration without RCS feature tag or with temporary RCS feature tag */
    MIPC_CALL_RCS_STATE_RE_REGISTRATION                     = 2,
};
typedef enum mipc_call_rcs_state_const_enum mipc_call_rcs_state_const_enum;

enum mipc_call_rcs_feature_const_enum {
    mipc_call_rcs_feature_const_NONE = 0,
    /* CALL_RCS_FEATURE */
    MIPC_CALL_RCS_FEATURE_NONE                              = 0,
    MIPC_CALL_RCS_FEATURE_SESSION                           = 0x01,
    MIPC_CALL_RCS_FEATURE_FILETRANSFER                      = 0x02,
    MIPC_CALL_RCS_FEATURE_MSG                               = 0x04,
    MIPC_CALL_RCS_FEATURE_LARGEMSG                          = 0x08,
    MIPC_CALL_RCS_FEATURE_GEOPUSH                           = 0x10,
    MIPC_CALL_RCS_FEATURE_GEOPULL                           = 0x20,
    MIPC_CALL_RCS_FEATURE_GEOPULLFT                         = 0x40,
    MIPC_CALL_RCS_FEATURE_IMDN_AGGREGATION                  = 0x80,
    MIPC_CALL_RCS_FEATURE_GEOSMS                            = 0x100,
    MIPC_CALL_RCS_FEATURE_FTHTTP                            = 0x200,
    MIPC_CALL_RCS_FEATURE_CALLCOMPOSER                      = 0x400,
    MIPC_CALL_RCS_FEATURE_CHATBOT                           = 0x800,
    MIPC_CALL_RCS_FEATURE_CHATBOT_SA                        = 0x1000,
    MIPC_CALL_RCS_FEATURE_CHATBOT_VERSION                   = 0x2000,
};
typedef enum mipc_call_rcs_feature_const_enum mipc_call_rcs_feature_const_enum;

enum mipc_call_voice_domain_const_enum {
    mipc_call_voice_domain_const_NONE = 0,
    /* CALL_VOICE_DOMAIN */
    /* CS Voice only */
    MIPC_CALL_VOICE_DOMAIN_CS_ONLY                          = 1,
    /* CS Voice preferred, IMS PS Voice as secondary */
    MIPC_CALL_VOICE_DOMAIN_CS_PREFERRED                     = 2,
    /* IMS PS Voice preferred, CS Voice as secondary */
    MIPC_CALL_VOICE_DOMAIN_PS_PREFERRED                     = 3,
    /* IMS PS Voice only */
    MIPC_CALL_VOICE_DOMAIN_PS_ONLY                          = 4,
};
typedef enum mipc_call_voice_domain_const_enum mipc_call_voice_domain_const_enum;

enum mipc_call_pull_type_const_enum {
    mipc_call_pull_type_const_NONE = 0,
    /* CALL_PULL_TYPE */
    MIPC_CALL_PULL_TYPE_VOICE                               = 0,
    MIPC_CALL_PULL_TYPE_VIDEO                               = 1,
};
typedef enum mipc_call_pull_type_const_enum mipc_call_pull_type_const_enum;

enum mipc_call_tty_mode_const_enum {
    mipc_call_tty_mode_const_NONE = 0,
    /* CALL_TTY_MODE */
    /* Disable TTY call  */
    MIPC_CALL_TTY_MODE_OFF                                  = 0,
    /* Enable TTY call */
    MIPC_CALL_TTY_MODE_FULL                                 = 1,
    /* Enable TTY call with HCO (Hearing Carry-Over) */
    MIPC_CALL_TTY_MODE_HCO                                  = 2,
    /* Enable TTY call with VCO (Voice Carry-Over) */
    MIPC_CALL_TTY_MODE_VCO                                  = 3,
};
typedef enum mipc_call_tty_mode_const_enum mipc_call_tty_mode_const_enum;

enum mipc_call_ims_call_mode_const_enum {
    mipc_call_ims_call_mode_const_NONE = 0,
    /* CALL_IMS_CALL_MODE */
    /* original IMS call mode  */
    MIPC_CALL_IMS_CALL_MODE_ORIGINAL_IMS                    = 1,
    /* Client API call mode */
    MIPC_CALL_IMS_CALL_MODE_CLIENT_API                      = 2,
};
typedef enum mipc_call_ims_call_mode_const_enum mipc_call_ims_call_mode_const_enum;

enum mipc_ecall_type_const_enum {
    mipc_ecall_type_const_NONE = 0,
    /* ECALL_TYPE */
    MIPC_ECALL_TYPE_TEST                                    = 0,
    MIPC_ECALL_TYPE_RECOFIG                                 = 1,
    MIPC_ECALL_TYPE_MANUAL                                  = 2,
    MIPC_ECALL_TYPE_AUTO                                    = 3,
};
typedef enum mipc_ecall_type_const_enum mipc_ecall_type_const_enum;

enum mipc_ecall_msd_format_const_enum {
    mipc_ecall_msd_format_const_NONE = 0,
    /* ECALL_MSD_FORMAT */
    MIPC_ECALL_MSD_FORMAT_BINARY                            = 1,
};
typedef enum mipc_ecall_msd_format_const_enum mipc_ecall_msd_format_const_enum;

enum mipc_phb_ef_file_type_const_enum {
    mipc_phb_ef_file_type_const_NONE = 0,
    /* PHB_EF_FILE_TYPE */
    /* EF_ANR */
    MIPC_PHB_EF_FILE_TYPE_EF_ANR                            = 0,
    /* CNI has been with held by the originator */
    MIPC_PHB_EF_FILE_TYPE_EF_EMAIL                          = 1,
    /* EF_EMAIL */
    MIPC_PHB_EF_FILE_TYPE_EF_SNE                            = 2,
    /* EF_AAS */
    MIPC_PHB_EF_FILE_TYPE_EF_AAS                            = 3,
    /* EF_GAS */
    MIPC_PHB_EF_FILE_TYPE_EF_GAS                            = 4,
    /* EF_GRP */
    MIPC_PHB_EF_FILE_TYPE_EF_GRP                            = 5,
};
typedef enum mipc_phb_ef_file_type_const_enum mipc_phb_ef_file_type_const_enum;

enum mipc_phb_encode_method_const_enum {
    mipc_phb_encode_method_const_NONE = 0,
    /* PHB_ENCODE_METHOD */
    MIPC_PHB_ENCODE_METHOD_ENCODE_IRA                       = 0,
    MIPC_PHB_ENCODE_METHOD_ENCODE_UCS2                      = 1,
    MIPC_PHB_ENCODE_METHOD_ENCODE_UCS2_81                   = 2,
    MIPC_PHB_ENCODE_METHOD_ENCODE_UCS2_82                   = 3,
    MIPC_PHB_ENCODE_METHOD_ENCODE_UCS2_GSM7BIT              = 4,
    MIPC_PHB_ENCODE_METHOD_ENCODE_UNKNOWN                   = 5,
};
typedef enum mipc_phb_encode_method_const_enum mipc_phb_encode_method_const_enum;

enum mipc_phb_stroage_type_const_enum {
    mipc_phb_stroage_type_const_NONE = 0,
    /* PHB_STROAGE_TYPE */
    /* MT phonebook */
    MIPC_PHB_STROAGE_TYPE_ME                                = 0,
    /* SIM/UICC phonebook */
    MIPC_PHB_STROAGE_TYPE_SM                                = 1,
    /* last-dialling phonebook */
    MIPC_PHB_STROAGE_TYPE_LD                                = 2,
    /* MT missed calls list */
    MIPC_PHB_STROAGE_TYPE_MC                                = 3,
    /* MT received calls list */
    MIPC_PHB_STROAGE_TYPE_RC                                = 4,
    /* MT dialled call list */
    MIPC_PHB_STROAGE_TYPE_DC                                = 5,
    /* SIM/UICC fixdialling-phonebook */
    MIPC_PHB_STROAGE_TYPE_FD                                = 6,
    /* SIMown numbers list. */
    MIPC_PHB_STROAGE_TYPE_ON                                = 7,
};
typedef enum mipc_phb_stroage_type_const_enum mipc_phb_stroage_type_const_enum;

enum mipc_sys_lpwr_mode_const_enum {
    mipc_sys_lpwr_mode_const_NONE = 0,
    /* SYS_LPWR_MODE */
    /* Disable the LPWR Assist Mode */
    MIPC_SYS_LPWR_MODE_DISABLE                              = 0x00000000,
    /* bit 0 ~ bit 7 (bit 0 : option1, bit 1 : option2,..., bit 7 : option8). For each option bit will be defined by each user */
    MIPC_SYS_LPWR_MODE_USER_1_CONFIG                        = 0x000000FF,
    /* bit 8 ~ bit 15 (bit 8 : option1, bit 9 : option2,..., bit 15 : option8). For each option bit will be defined by each user */
    MIPC_SYS_LPWR_MODE_USER_2_CONFIG                        = 0x0000FF00,
    /* bit 16 ~ bit 23 (bit 16 : option1, bit 17 : option2,..., bit 23 : option8). For each option bit will be defined by each user */
    MIPC_SYS_LPWR_MODE_USER_3_CONFIG                        = 0x00FF0000,
    /* bit 24 ~ bit 31 (bit 24 : option1, bit 25 : option2,..., bit 31 : option8. For each option bit will be defined by each user) */
    MIPC_SYS_LPWR_MODE_USER_4_CONFIG                        = 0xFF000000,
};
typedef enum mipc_sys_lpwr_mode_const_enum mipc_sys_lpwr_mode_const_enum;

enum mipc_sys_thermal_actuator_id_const_enum {
    mipc_sys_thermal_actuator_id_const_NONE = 0,
    /* SYS_THERMAL_ACTUATOR_ID */
    MIPC_SYS_THERMAL_ACTUATOR_ID_UL_THROTTLING              = 1,
    MIPC_SYS_THERMAL_ACTUATOR_ID_TX_POWER                   = 2,
    MIPC_SYS_THERMAL_ACTUATOR_ID_CC_CTRL                    = 3,
    MIPC_SYS_THERMAL_ACTUATOR_ID_HEADSWITCH                 = 4,
    MIPC_SYS_THERMAL_ACTUATOR_ID_RAS                        = 5,
    MIPC_SYS_THERMAL_ACTUATOR_ID_FR2_OFF                    = 6,
    MIPC_SYS_THERMAL_ACTUATOR_ID_FLIGHT_MODE                = 7,
    MIPC_SYS_THERMAL_ACTUATOR_ID_CHARGER                    = 8,
};
typedef enum mipc_sys_thermal_actuator_id_const_enum mipc_sys_thermal_actuator_id_const_enum;

enum mipc_sys_factory_mode_const_enum {
    mipc_sys_factory_mode_const_NONE = 0,
    /* SYS_FACTORY_MODE */
    /* Factory mode */
    MIPC_SYS_FACTORY_MODE_FACTORY                           = 0,
    /* Normal mode */
    MIPC_SYS_FACTORY_MODE_NORMAL                            = 1,
    /* USB mode, system boot up when USB charger connect. This value is invalid for MIPC_SYS_SET_FACTORY_MODE_REQ. */
    MIPC_SYS_FACTORY_MODE_USB                               = 2,
    /* Firmware update engine mode. This value is invalid for MIPC_SYS_SET_FACTORY_MODE_REQ. */
    MIPC_SYS_FACTORY_MODE_FUE                               = 3,
};
typedef enum mipc_sys_factory_mode_const_enum mipc_sys_factory_mode_const_enum;

enum mipc_sys_chip_diagnosis_operation_const_enum {
    mipc_sys_chip_diagnosis_operation_const_NONE = 0,
    /* SYS_CHIP_DIAGNOSIS_OPERATION */
    /* Trigger DVFS to check related HW status. First, it will enable DVFS and RF enable to do memory access and system test. After DVFS is enabled, DVFS will check related HW is OK. Final, disable DVFS and return CNF message to user. */
    MIPC_SYS_CHIP_DIAGNOSIS_OPERATION_DVFS_TEST             = 0,
};
typedef enum mipc_sys_chip_diagnosis_operation_const_enum mipc_sys_chip_diagnosis_operation_const_enum;

enum mipc_ecall_address_priority_class_const_enum {
    mipc_ecall_address_priority_class_const_NONE = 0,
    /* ECALL_ADDRESS_PRIORITY_CLASS */
    /* eCall URI set by customer, including test eCall and reconfiguration eCall */
    MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_URI      = 1,
    /* eCall URI in the USIM, including test eCall and reconfiguration eCall */
    MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_URI        = 2,
    /* eCall number set by customer, including test eCall and reconfiguration eCall */
    MIPC_ECALL_ADDRESS_PRIORITY_CLASS_CUSTOM_ECALL_NUM      = 3,
    /* eCall number set by customer, including test eCall and reconfiguration eCall */
    MIPC_ECALL_ADDRESS_PRIORITY_CLASS_USIM_ECALL_NUM        = 4,
};
typedef enum mipc_ecall_address_priority_class_const_enum mipc_ecall_address_priority_class_const_enum;

enum mipc_ecall_sim_type_const_enum {
    mipc_ecall_sim_type_const_NONE = 0,
    /* ECALL_SIM_TYPE */
    /* normal sim card */
    MIPC_ECALL_SIM_TYPE_NORMAL                              = 0,
    /* eCall only sim card */
    MIPC_ECALL_SIM_TYPE_ECALL_ONLY                          = 1,
    /* eCall and normal sim card */
    MIPC_ECALL_SIM_TYPE_ECALL_AND_NORMAL                    = 2,
};
typedef enum mipc_ecall_sim_type_const_enum mipc_ecall_sim_type_const_enum;

enum mipc_ecall_status_const_enum {
    mipc_ecall_status_const_NONE = 0,
    /* ECALL_STATUS */
    /* IVS received START signal from PSAP. */
    MIPC_ECALL_STATUS_START                                 = 1,
    /* IVS is sending MSD to PSAP. */
    MIPC_ECALL_STATUS_SENDING_MSD                           = 2,
    /* IVS received LLACK from PSAP. */
    MIPC_ECALL_STATUS_LLACK                                 = 3,
    /* IVS received ALACK POSITIVE from PSAP. */
    MIPC_ECALL_STATUS_ALACK_POSITIVE                        = 4,
    /* IVS received ALACK CLEARDOWN from PSAP. */
    MIPC_ECALL_STATUS_ALACK_CLEARDOWN                       = 5,
    /* eCall call id assign. */
    MIPC_ECALL_STATUS_CALL_ID_ASSIGN                        = 9,
    /* eCall alert. */
    MIPC_ECALL_STATUS_ALERT                                 = 10,
    /* CS eCall connected. */
    MIPC_ECALL_STATUS_CONNECTED                             = 11,
    /* CS eCall disconnected. */
    MIPC_ECALL_STATUS_DISCONNECTED                          = 12,
    /* IMS eCall connected. */
    MIPC_ECALL_STATUS_IMS_CONNECTED                         = 13,
    /* IMS eCall disconnected. */
    MIPC_ECALL_STATUS_IMS_DISCONNECTED                      = 14,
    /* CS eCall disconnected abnormally. */
    MIPC_ECALL_STATUS_ABNORMAL_HANGUP                       = 15,
    /* IMS eCall, IVS received ACK from PSAP. */
    MIPC_ECALL_STATUS_IMS_MSD_ACK                           = 20,
    /* IMS eCall, IVS received update MSD request from PSAP. */
    MIPC_ECALL_STATUS_IMS_UPDATE_MSD                        = 21,
    /* IMS eCall, IVS will transfer MSD by in band modem. */
    MIPC_ECALL_STATUS_IMS_IN_BAND_TRANSFER                  = 22,
    /* IMS eCall, IVS received NACK from PSAP. */
    MIPC_ECALL_STATUS_IMS_MSD_NACK                          = 23,
    /* IMS eCall, SRVCC hanppened. modem will transfer MSD by in band modem (CS domain). */
    MIPC_ECALL_STATUS_IMS_SRVCC                             = 24,
    /* IVS (eCall only sim) enters eCall inactivity procedure */
    MIPC_ECALL_STATUS_ECALL_ONLY_DEREG                      = 31,
    /* PSAP callback, CS call, IVS received START from PSAP. */
    MIPC_ECALL_STATUS_PSAP_CALLBACK_START                   = 40,
    /* PSAP callback, IMS call, received update MSD request from PSAP. */
    MIPC_ECALL_STATUS_PSAP_CALLBACK_IMS_UPDATE_MSD          = 41,
    /* T2 timeout */
    MIPC_ECALL_STATUS_ECALL_T2_TIMEOUT                      = 52,
    /* T5 timeout */
    MIPC_ECALL_STATUS_ECALL_T5_TIMEOUT                      = 55,
    /* T6 timeout */
    MIPC_ECALL_STATUS_ECALL_T6_TIMEOUT                      = 56,
    /* T7 timeout */
    MIPC_ECALL_STATUS_ECALL_T7_TIMEOUT                      = 57,
    /* unspecified ecall state. */
    MIPC_ECALL_STATUS_UNSPECIFIED                           = 255,
};
typedef enum mipc_ecall_status_const_enum mipc_ecall_status_const_enum;

enum mipc_ecall_mode_type_const_enum {
    mipc_ecall_mode_type_const_NONE = 0,
    /* ECALL_MODE_TYPE */
    /* Can not get current eCall mode. */
    MIPC_ECALL_MODE_TYPE_ECALL_NONE                         = 0,
    /* In eCall_only mode, IVS doesn't register to network until eCall triggered. And IVS will deregister from network after PSAP callback timer timeout. */
    MIPC_ECALL_MODE_TYPE_ECALL_ONLY                         = 1,
    /* In eCall_and_normal mode, IVS will always register to network whether eCall triggered or not. */
    MIPC_ECALL_MODE_TYPE_ECALL_AND_NORMAL                   = 2,
    /* For eCall test simulation. */
    MIPC_ECALL_MODE_TYPE_ECALL_TEST_DISABLE                 = 3,
};
typedef enum mipc_ecall_mode_type_const_enum mipc_ecall_mode_type_const_enum;

enum mipc_ecall_t10_status_const_enum {
    mipc_ecall_t10_status_const_NONE = 0,
    /* ECALL_T10_STATUS */
    /* T10 timer is not running. */
    MIPC_ECALL_T10_STATUS_ECALL_T10_NOT_RUN                 = 0,
    /* T10 timer is running. */
    MIPC_ECALL_T10_STATUS_ECALL_T10_RUNNING                 = 1,
};
typedef enum mipc_ecall_t10_status_const_enum mipc_ecall_t10_status_const_enum;

enum mipc_gps_status_control_enum_const_enum {
    mipc_gps_status_control_enum_const_NONE = 0,
    /* GPS_STATUS_CONTROL_ENUM */
    MIPC_GPS_STATUS_CONTROL_ENUM_GPS_OPEN                   = 0,
    MIPC_GPS_STATUS_CONTROL_ENUM_GPS_CLOSE                  = 1,
    MIPC_GPS_STATUS_CONTROL_ENUM_MNL_REBOOT                 = 2,
    MIPC_GPS_STATUS_CONTROL_ENUM_OPEN_GSP_DONE              = 3,
    MIPC_GPS_STATUS_CONTROL_ENUM_CLOSE_GPS_DONE             = 4,
    MIPC_GPS_STATUS_CONTROL_ENUM_RESET_GPS_DONE             = 5,
};
typedef enum mipc_gps_status_control_enum_const_enum mipc_gps_status_control_enum_const_enum;

enum mipc_gps_time_sync_operation_enum_const_enum {
    mipc_gps_time_sync_operation_enum_const_NONE = 0,
    /* GPS_TIME_SYNC_OPERATION_ENUM */
    MIPC_GPS_TIME_SYNC_OPERATION_ENUM_TIME_SYNC_REQ         = 0,
    MIPC_GPS_TIME_SYNC_OPERATION_ENUM_TIME_SYNC_RSP         = 1,
    MIPC_GPS_TIME_SYNC_OPERATION_ENUM_TIME_SYNC_INFO_RSP    = 2,
};
typedef enum mipc_gps_time_sync_operation_enum_const_enum mipc_gps_time_sync_operation_enum_const_enum;

enum mipc_lbs_em_msg_enum_const_enum {
    mipc_lbs_em_msg_enum_const_NONE = 0,
    /* LBS_EM_MSG_ENUM */
    MIPC_LBS_EM_MSG_ENUM_CMD_START_LISTEN                   = 0,
    MIPC_LBS_EM_MSG_ENUM_CMD_STOP_LISTEN                    = 1,
};
typedef enum mipc_lbs_em_msg_enum_const_enum mipc_lbs_em_msg_enum_const_enum;

enum mipc_lbs_debug_req_enum_const_enum {
    mipc_lbs_debug_req_enum_const_NONE = 0,
    /* LBS_DEBUG_REQ_ENUM */
    MIPC_LBS_DEBUG_REQ_ENUM_ACTION_DEBUG_MSG_REQ            = 0,
    MIPC_LBS_DEBUG_REQ_ENUM_ACTION_DEBUG_EXTRA_REQ          = 1,
};
typedef enum mipc_lbs_debug_req_enum_const_enum mipc_lbs_debug_req_enum_const_enum;

enum mipc_lbs_debug_ind_enum_const_enum {
    mipc_lbs_debug_ind_enum_const_NONE = 0,
    /* LBS_DEBUG_IND_ENUM */
    /* Acknowledgement of AP's request */
    MIPC_LBS_DEBUG_IND_ENUM_ACTION_DEBUG_REQ_ACK            = 0,
    /* Inform modem reboot */
    MIPC_LBS_DEBUG_IND_ENUM_ACTION_UPDATE_REBOOT            = 1,
    /* Inform current modem session information */
    MIPC_LBS_DEBUG_IND_ENUM_ACTION_UPDATE_SESSION_INFO      = 2,
    /* Inform extra information which requested by customer */
    MIPC_LBS_DEBUG_IND_ENUM_ACTION_EXTRA_INFO               = 3,
};
typedef enum mipc_lbs_debug_ind_enum_const_enum mipc_lbs_debug_ind_enum_const_enum;

enum mipc_lppe_protocol_enum_const_enum {
    mipc_lppe_protocol_enum_const_NONE = 0,
    /* LPPE_PROTOCOL_ENUM */
    MIPC_LPPE_PROTOCOL_ENUM_PROTOCOL_WLAN                   = 0,
    MIPC_LPPE_PROTOCOL_ENUM_PROTOCOL_BT                     = 1,
    MIPC_LPPE_PROTOCOL_ENUM_PROTOCOL_SENSOR                 = 2,
    MIPC_LPPE_PROTOCOL_ENUM_PROTOCOL_NETWORK                = 3,
    MIPC_LPPE_PROTOCOL_ENUM_PROTOCOL_IPADDR                 = 4,
    MIPC_LPPE_PROTOCOL_ENUM_PROTOCOL_LBS                    = 5,
};
typedef enum mipc_lppe_protocol_enum_const_enum mipc_lppe_protocol_enum_const_enum;

enum mipc_lppe_msg_action_enum_const_enum {
    mipc_lppe_msg_action_enum_const_NONE = 0,
    /* LPPE_MSG_ACTION_ENUM */
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_REQUEST_CAPABILITIES   = 0,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_CAPABILITIES   = 1,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_START_MEASUREMENT      = 2,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_STOP_MEASUREMENT       = 3,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_MEASUREMENT    = 4,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_MEASUREMENT_FINISHED = 5,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_REQUEST_INFORMATION    = 6,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_INFORMATION    = 7,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_REQUEST_CIVIC_ADDRESS  = 8,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_CIVIC_ADDRESS  = 9,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_UPDATE_BATTAERY_INFO = 10,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_UPDATE_NLP_STATUS = 11,
    MIPC_LPPE_MSG_ACTION_ENUM_ACTION_PROVIDE_UPDATE_VOWIFI_REG = 12,
};
typedef enum mipc_lppe_msg_action_enum_const_enum mipc_lppe_msg_action_enum_const_enum;

enum mipc_lppe_supl_req_action_enum_const_enum {
    mipc_lppe_supl_req_action_enum_const_NONE = 0,
    /* LPPE_SUPL_REQ_ACTION_ENUM */
    MIPC_LPPE_SUPL_REQ_ACTION_ENUM_ACTION_AGPS_NI_RESPONSE  = 0,
};
typedef enum mipc_lppe_supl_req_action_enum_const_enum mipc_lppe_supl_req_action_enum_const_enum;

enum mipc_lppe_supl_ind_action_enum_const_enum {
    mipc_lppe_supl_ind_action_enum_const_NONE = 0,
    /* LPPE_SUPL_IND_ACTION_ENUM */
    MIPC_LPPE_SUPL_IND_ACTION_ENUM_ACTION_ACQUIRE_WAKE_LOCK = 0,
    MIPC_LPPE_SUPL_IND_ACTION_ENUM_ACTION_RELEASE_WAKE_LOCK = 1,
    MIPC_LPPE_SUPL_IND_ACTION_ENUM_ACTION_REQUEST_GPS_ICON  = 2,
    MIPC_LPPE_SUPL_IND_ACTION_ENUM_ACTION_REMOVE_GPS_ICON   = 3,
    MIPC_LPPE_SUPL_IND_ACTION_ENUM_ACTION_AGPS_NI_NOTIFY    = 4,
};
typedef enum mipc_lppe_supl_ind_action_enum_const_enum mipc_lppe_supl_ind_action_enum_const_enum;

enum mipc_lbs_aidl_req_action_enum_const_enum {
    mipc_lbs_aidl_req_action_enum_const_NONE = 0,
    /* LBS_AIDL_REQ_ACTION_ENUM */
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_GPS_INIT           = 0,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_GPS_CLEANUP        = 1,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_UPDATE_GNSS_CONFIG = 2,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SET_NFW_ACCESS     = 3,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SET_POSITION_MODE  = 4,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SET_SUPL_SERVER    = 5,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SET_INSTALL_CERT   = 6,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SET_REVOKE_CERT    = 7,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_UPDATE_NETWORK_STATE = 8,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SYNC_LOCATION      = 9,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_GET_SUPL_CERTI     = 10,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_DELET_CERT_ALL     = 11,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_SYNC_TIME          = 12,
    MIPC_LBS_AIDL_REQ_ACTION_ENUM_ACTION_UPDATE_CUST_CONFIG = 13,
};
typedef enum mipc_lbs_aidl_req_action_enum_const_enum mipc_lbs_aidl_req_action_enum_const_enum;

enum mipc_lbs_aidl_ind_action_enum_const_enum {
    mipc_lbs_aidl_ind_action_enum_const_NONE = 0,
    /* LBS_AIDL_IND_ACTION_ENUM */
    /* modem request SUPL certification from AP */
    MIPC_LBS_AIDL_IND_ACTION_ENUM_ACTION_GET_SUPL_CERTI     = 0,
    /* inform AP when modem reboot */
    MIPC_LBS_AIDL_IND_ACTION_ENUM_ACTION_MODEM_REBOOT       = 1,
};
typedef enum mipc_lbs_aidl_ind_action_enum_const_enum mipc_lbs_aidl_ind_action_enum_const_enum;

enum mipc_gps_asn_info_action_enum_const_enum {
    mipc_gps_asn_info_action_enum_const_NONE = 0,
    /* GPS_ASN_INFO_ACTION_ENUM */
    MIPC_GPS_ASN_INFO_ACTION_ENUM_ACTION_REQUEST_ASN1_ASSIST_DATA = 0,
    MIPC_GPS_ASN_INFO_ACTION_ENUM_ACTION_PROVIDE_ASN1_LOCATION_INFORMATION = 1,
    MIPC_GPS_ASN_INFO_ACTION_ENUM_ACTION_ASN_ACK            = 2,
    MIPC_GPS_ASN_INFO_ACTION_ENUM_ACTION_AGPS_VERSION       = 3,
    MIPC_GPS_ASN_INFO_ACTION_ENUM_ACTION_LSP_GNSS_INFO      = 4,
};
typedef enum mipc_gps_asn_info_action_enum_const_enum mipc_gps_asn_info_action_enum_const_enum;

enum mipc_gps_tsx_data_action_enum_const_enum {
    mipc_gps_tsx_data_action_enum_const_NONE = 0,
    /* GPS_TSX_DATA_ACTION_ENUM */
    MIPC_GPS_TSX_DATA_ACTION_ENUM_TSX_DATA_SYNC_REQ         = 0,
    MIPC_GPS_TSX_DATA_ACTION_ENUM_TSX_DATA_INFO_RSP         = 1,
};
typedef enum mipc_gps_tsx_data_action_enum_const_enum mipc_gps_tsx_data_action_enum_const_enum;

enum mipc_agps_status_cmd_enum_const_enum {
    mipc_agps_status_cmd_enum_const_NONE = 0,
    /* AGPS_STATUS_CMD_ENUM */
    MIPC_AGPS_STATUS_CMD_ENUM_AGPS_REBOOT                   = 0,
    MIPC_AGPS_STATUS_CMD_ENUM_AGPS_OPEN_GPS_REQ             = 1,
    MIPC_AGPS_STATUS_CMD_ENUM_AGPS_OPEN_GPS_REJECT          = 2,
    MIPC_AGPS_STATUS_CMD_ENUM_AGPS_CLOSE_GPS_REQ            = 3,
    MIPC_AGPS_STATUS_CMD_ENUM_AGPS_RESET_GPS_REQ            = 4,
};
typedef enum mipc_agps_status_cmd_enum_const_enum mipc_agps_status_cmd_enum_const_enum;

enum mipc_mnl_agps_open_type_enum_const_enum {
    mipc_mnl_agps_open_type_enum_const_NONE = 0,
    /* MNL_AGPS_OPEN_TYPE_ENUM */
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_UNKNOWN = 0,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_C2K     = 0x10,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_SUPL    = 0x20,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_CP_NILR = 0x30,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_CP_MTLR = 0x31,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_CP_MOLR = 0x32,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_CP_QUERY = 0x33,
    MIPC_MNL_AGPS_OPEN_TYPE_ENUM_MNL_AGPS_OPEN_TYPE_CP_MLR  = 0x34,
};
typedef enum mipc_mnl_agps_open_type_enum_const_enum mipc_mnl_agps_open_type_enum_const_enum;

enum mipc_mnl_agps_open_requestor_enum_const_enum {
    mipc_mnl_agps_open_requestor_enum_const_NONE = 0,
    /* MNL_AGPS_OPEN_REQUESTOR_ENUM */
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_UNKNOWN = 0,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_CARRIER = 1,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_OEM    = 2,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_MODEM_CHIPSET_VENDOR = 3,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_GNSS_CHIPSET_VENDOR = 4,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_OTHER_CHIPSET_VENDOR = 5,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_AUTOMOBILE_CLIENT = 6,
    MIPC_MNL_AGPS_OPEN_REQUESTOR_ENUM_OPEN_REQUESTOR_OTHER_REQUESTOR = 7,
};
typedef enum mipc_mnl_agps_open_requestor_enum_const_enum mipc_mnl_agps_open_requestor_enum_const_enum;

enum mipc_mnl_agps_ni_notify_type_enum_const_enum {
    mipc_mnl_agps_ni_notify_type_enum_const_NONE = 0,
    /* MNL_AGPS_NI_NOTIFY_TYPE_ENUM */
    /* NI notify without encoding */
    MIPC_MNL_AGPS_NI_NOTIFY_TYPE_ENUM_NI_NOTIFY             = 0,
    /* NI notify with encoding */
    MIPC_MNL_AGPS_NI_NOTIFY_TYPE_ENUM_NI_NOTIFY2            = 1,
};
typedef enum mipc_mnl_agps_ni_notify_type_enum_const_enum mipc_mnl_agps_ni_notify_type_enum_const_enum;

enum mipc_mnl_agps_ni_type_enum_const_enum {
    mipc_mnl_agps_ni_type_enum_const_NONE = 0,
    /* MNL_AGPS_NI_TYPE_ENUM */
    MIPC_MNL_AGPS_NI_TYPE_ENUM_NI_TYPE_VOICE                = 1,
    MIPC_MNL_AGPS_NI_TYPE_ENUM_NI_TYPE_UTMS_SUPL            = 2,
    MIPC_MNL_AGPS_NI_TYPE_ENUM_NI_TYPE_UTMS_CTRL_PLANE      = 3,
    MIPC_MNL_AGPS_NI_TYPE_ENUM_NI_TYPE_EMERGENCY_SUPL       = 4,
};
typedef enum mipc_mnl_agps_ni_type_enum_const_enum mipc_mnl_agps_ni_type_enum_const_enum;

enum mipc_mnl_agps_ni_encoding_type_enum_const_enum {
    mipc_mnl_agps_ni_encoding_type_enum_const_NONE = 0,
    /* MNL_AGPS_NI_ENCODING_TYPE_ENUM */
    MIPC_MNL_AGPS_NI_ENCODING_TYPE_ENUM_NI_ENCODING_TYPE_NONE = 0,
    MIPC_MNL_AGPS_NI_ENCODING_TYPE_ENUM_NI_ENCODING_TYPE_GSM7 = 1,
    MIPC_MNL_AGPS_NI_ENCODING_TYPE_ENUM_NI_ENCODING_TYPE_UTF8 = 2,
    MIPC_MNL_AGPS_NI_ENCODING_TYPE_ENUM_NI_ENCODING_TYPE_UCS2 = 3,
};
typedef enum mipc_mnl_agps_ni_encoding_type_enum_const_enum mipc_mnl_agps_ni_encoding_type_enum_const_enum;

enum mipc_mnl_agps_location_control_enum_const_enum {
    mipc_mnl_agps_location_control_enum_const_NONE = 0,
    /* MNL_AGPS_LOCATION_CONTROL_ENUM */
    MIPC_MNL_AGPS_LOCATION_CONTROL_ENUM_AGPS_LOCATION       = 0,
    MIPC_MNL_AGPS_LOCATION_CONTROL_ENUM_AGPS_C2K_CELL_LOCATION = 1,
};
typedef enum mipc_mnl_agps_location_control_enum_const_enum mipc_mnl_agps_location_control_enum_const_enum;

enum mipc_mnl_agps_location_type_enum_const_enum {
    mipc_mnl_agps_location_type_enum_const_NONE = 0,
    /* MNL_AGPS_LOCATION_TYPE_ENUM */
    /* From C2K AFLT flow */
    MIPC_MNL_AGPS_LOCATION_TYPE_ENUM_LOC_TYPE_AFLT          = 0,
    /* From C2K Cell */
    MIPC_MNL_AGPS_LOCATION_TYPE_ENUM_LOC_TYPE_CDMA_CELL     = 1,
    /* For CP MOLR after UE provides MSA result, the network will send CISS_FACILITY to provide UE a MSA location. Refer to 3GPP TS37.571-2 7.2.2.2 */
    MIPC_MNL_AGPS_LOCATION_TYPE_ENUM_LOC_TYPE_MOLR_BEGIN_RSP = 2,
    /* From the SUPL END in a supl session. It may be the result of MSA, E-CID, OTDOA, or their hybrid. */
    MIPC_MNL_AGPS_LOCATION_TYPE_ENUM_LOC_TYPE_SUPL_END      = 3,
    /* For Reference Location assistance data in SUPL flow (equivalent to PMTK713 in SUPL flow) */
    MIPC_MNL_AGPS_LOCATION_TYPE_ENUM_LOC_TYPE_SUPL_REF_LOC  = 4,
    /* For Reference Location assistance data in CP AGPS flow (equivalent to PMTK713 in CP AGPS flow) */
    MIPC_MNL_AGPS_LOCATION_TYPE_ENUM_LOC_TYPE_CP_REF_LOC    = 5,
};
typedef enum mipc_mnl_agps_location_type_enum_const_enum mipc_mnl_agps_location_type_enum_const_enum;

enum mipc_agps_md_time_action_enum_const_enum {
    mipc_agps_md_time_action_enum_const_NONE = 0,
    /* AGPS_MD_TIME_ACTION_ENUM */
    MIPC_AGPS_MD_TIME_ACTION_ENUM_MD_TIME_SYNC_IND          = 0,
    MIPC_AGPS_MD_TIME_ACTION_ENUM_MD_TIME_SYNC_CNF          = 1,
};
typedef enum mipc_agps_md_time_action_enum_const_enum mipc_agps_md_time_action_enum_const_enum;

enum mipc_agps_tsx_data_action_enum_const_enum {
    mipc_agps_tsx_data_action_enum_const_NONE = 0,
    /* AGPS_TSX_DATA_ACTION_ENUM */
    MIPC_AGPS_TSX_DATA_ACTION_ENUM_MD_TSX_SYNC_IND          = 0,
    MIPC_AGPS_TSX_DATA_ACTION_ENUM_MD_TSX_SYNC_CNF          = 1,
};
typedef enum mipc_agps_tsx_data_action_enum_const_enum mipc_agps_tsx_data_action_enum_const_enum;

enum mipc_agps_asn_data_type_enum_const_enum {
    mipc_agps_asn_data_type_enum_const_NONE = 0,
    /* AGPS_ASN_DATA_TYPE_ENUM */
    MIPC_AGPS_ASN_DATA_TYPE_ENUM_ASN_DATA_TYPE_MAGNSS       = 0,
    MIPC_AGPS_ASN_DATA_TYPE_ENUM_ASN_DATA_TYPE_LSP          = 1,
};
typedef enum mipc_agps_asn_data_type_enum_const_enum mipc_agps_asn_data_type_enum_const_enum;

enum mipc_agps_version_action_enum_const_enum {
    mipc_agps_version_action_enum_const_NONE = 0,
    /* AGPS_VERSION_ACTION_ENUM */
    MIPC_AGPS_VERSION_ACTION_ENUM_AGPS_VERSION_SYNC         = 0,
    MIPC_AGPS_VERSION_ACTION_ENUM_AGPS_VERSION_QUERY        = 1,
};
typedef enum mipc_agps_version_action_enum_const_enum mipc_agps_version_action_enum_const_enum;

enum mipc_result_const_enum {
    mipc_result_const_NONE = 0,
    /* RESULT */
    /* The operation succeeded */
    MIPC_RESULT_SUCCESS                                     = 0,
    /* The operation failed because the device is busy. In the absence of any explicit information from the function to clear this condition, the host can use subsequent actions by the function (e.g., notifications or command completions) as a hint to retry the failed operation. */
    MIPC_RESULT_BUSY                                        = 1,
    /* The operation failed (a generic failure). */
    MIPC_RESULT_FAILURE                                     = 2,
    /* The operation failed because the SIM card was not fully inserted in to the device. */
    MIPC_RESULT_SIM_NOT_INSERTED                            = 3,
    /* The operation failed because the SIM card is bad and cannot be used any further. */
    MIPC_RESULT_BAD_SIM                                     = 4,
    /* The operation failed because a PIN must be entered to proceed. */
    MIPC_RESULT_PIN_REQUIRED                                = 5,
    /* The operation failed because the PIN is disabled. */
    MIPC_RESULT_PIN_DISABLED                                = 6,
    /* The operation failed because the device is not registered with any network. */
    MIPC_RESULT_NOT_REGISTERED                              = 7,
    /* The operation failed because no network providers could be found. */
    MIPC_RESULT_PROVIDERS_NOT_FOUND                         = 8,
    /* The operation failed because the device does not support the operation. */
    MIPC_RESULT_NO_DEVICE_SUPPORT                           = 9,
    /* The operation failed because the service provider is not currently visible. */
    MIPC_RESULT_PROVIDER_NOT_VISIBLE                        = 10,
    /* The operation failed because the requested data-class was not available. */
    MIPC_RESULT_DATA_CLASS_NOT_AVAILABLE                    = 11,
    /* The operation failed because the packet service is detached. */
    MIPC_RESULT_PACKET_SERVICE_DETACHED                     = 12,
    /* The operation failed because the maximum number of activated contexts has been reached. */
    MIPC_RESULT_MAX_ACTIVATED_CONTEXTS                      = 13,
    /* The operation failed because the device is in the process of initializing. */
    MIPC_RESULT_NOT_INITIALIZED                             = 14,
    /* The operation failed because a voice call is in progress. */
    MIPC_RESULT_VOICE_CALL_IN_PROGRESS                      = 15,
    /* The operation failed because the context is not activated. */
    MIPC_RESULT_CONTEXT_NOT_ACTIVATED                       = 16,
    /* The operation failed because service is not activated. */
    MIPC_RESULT_SERVICE_NOT_ACTIVATED                       = 17,
    /* The operation failed because the access string is invalid. */
    MIPC_RESULT_INVALID_ACCESS_STRING                       = 18,
    /* The operation failed because the username and/or password supplied are invalid. */
    MIPC_RESULT_INVALID_USERID_PASSWORD                     = 19,
    /* The operation failed because the radio is currently powered off. */
    MIPC_RESULT_RADIO_POWER_OFF                             = 20,
    /* The operation failed because of invalid parameters. */
    MIPC_RESULT_INVALID_PARAMETERS                          = 21,
    /* The operation failed because of a read failure. */
    MIPC_RESULT_READ_FAILURE                                = 22,
    /* The operation failed because of a write failure. */
    MIPC_RESULT_WRITE_FAILURE                               = 23,
    /* Reserved. */
    MIPC_RESULT_Reserved_24                                 = 24,
    /* The phonebook operation failed because there is no phone book. */
    MIPC_RESULT_NO_PHONEBOOK                                = 25,
    /* A parameter with dynamic size is larger than the function can handle */
    MIPC_RESULT_PARAMETER_TOO_LONG                          = 26,
    /* The SIM Toolkit application on the SIM card is busy and the command could not be processed */
    MIPC_RESULT_STK_BUSY                                    = 27,
    /* The operation failed because the operation is not allowed. */
    MIPC_RESULT_OPERATION_NOT_ALLOWED                       = 28,
    /* The phonebook or sms operation failed because the because of device or SIM memory failure. */
    MIPC_RESULT_MEMORY_FAILURE                              = 29,
    /* The phonebook or sms operation failed because of an invalid memory index. */
    MIPC_RESULT_INVALID_MEMORY_INDEX                        = 30,
    /* The phonebook or sms operation failed because the device or SIM memory is full. */
    MIPC_RESULT_MEMORY_FULL                                 = 31,
    /* The phonebook or sms operation failed because the filter type is not supported. */
    MIPC_RESULT_FILTER_NOT_SUPPORTED                        = 32,
    /* Attempt to open a device service failed because the number of opened streams has reached the device service instance limit for this service. */
    MIPC_RESULT_DSS_INSTANCE_LIMIT                          = 33,
    /* The device service operation attempted is invalid. */
    MIPC_RESULT_INVALID_DEVICE_SERVICE_OPERATION            = 34,
    /* The device sets this error on an AKA or AKAPrime challenge response when the AKA or AKAPrime challenge sent has incorrect AUTN. */
    MIPC_RESULT_AUTH_INCORRECT_AUTN                         = 35,
    /* The device sets this error on a an AKA or AKAPrime challenge response when the AKA or AKAPrime challenge sent has synchronization failure. When this error code is returned the AUTS field would be set. */
    MIPC_RESULT_AUTH_SYNC_FAILURE                           = 36,
    /* The device sets this error on an AKAPrime challenge response when the AKAPrime challenge sent does not have the AMF bit set to 1. */
    MIPC_RESULT_AUTH_AMF_NOT_SET                            = 37,
    /* The operation failed because it could not support the type of context identified by ContextType. */
    MIPC_RESULT_CONTEXT_NOT_SUPPORTED                       = 38,
    /* The operation failed because sim is missing resource(ex. not enough memory space) . */
    MIPC_RESULT_MISSING_RESOURCE                            = 39,
    /* The operation failed because not found . */
    MIPC_RESULT_NOT_FOUND                                   = 40,
    /* The operation failed because PHB FDN is blocked. */
    MIPC_RESULT_FDN_BLOCKED                                 = 41,
    /* The SMS operation failed because the service center address is either invalid or unknown. */
    MIPC_RESULT_SMS_UNKNOWN_SMSC_ADDRESS                    = 100,
    /* The SMS operation failed because of a network timeout. */
    MIPC_RESULT_SMS_NETWORK_TIMEOUT                         = 101,
    /* The SMS operation failed because the SMS language is not supported.This applies to CDMA based devices only. */
    MIPC_RESULT_SMS_LANG_NOT_SUPPORTED                      = 102,
    /* The SMS operation failed because the SMS encoding is not supported. This applies to CDMA based device only. */
    MIPC_RESULT_SMS_ENCODING_NOT_SUPPORTED                  = 103,
    /* The SMS operation failed because the SMS format is not supported. */
    MIPC_RESULT_SMS_FORMAT_NOT_SUPPORTED                    = 104,
    /* The  operation failed because the operation is not supported. */
    MIPC_RESULT_OPERATION_NOT_SUPPORTED                     = 105,
    /* The operation failed because of not found. There may be no relate logic to handle this MIPC_MSG. */
    MIPC_RESULT_ERROR_NOT_FOUND                             = 106,
    /* The  operation failed and no reason given. */
    MIPC_RESULT_ERROR_UNKOWN                                = 107,
    /*  */
    MIPC_RESULT_COMMON_END                                  = 0x3FFFF,
    /* CAT module extension result begin */
    MIPC_RESULT_CAT_EXT_BEGIN                               = 0x00080000,
    /* CAT module extension result end */
    MIPC_RESULT_CAT_EXT_END                                 = 0x000BFFFF,
    /* CC module extension result begin */
    MIPC_RESULT_CC_EXT_BEGIN                                = 0x000C0000,
    /* this call id is force hangup before */
    MIPC_RESULT_CC_CALL_FORCE_RELEASED_BEFORE               = 0x000C0001,
    /* hold call failed cause no call is exist*/
    MIPC_RESULT_CC_CALL_IS_NOT_EXIST                        = 0x000C0008,
    /* CC module extension result end */
    MIPC_RESULT_CC_EXT_END                                  = 0x000FFFFF,
    /* NW module extension result begin */
    MIPC_RESULT_NW_EXT_BEGIN                                = 0x00100000,
    /* NW module extension result end */
    MIPC_RESULT_NW_EXT_END                                  = 0x0013FFFF,
    /* PDN module extension result begin */
    MIPC_RESULT_PDN_EXT_BEGIN                               = 0x00140000,
    /* PDN module network error begin */
    MIPC_RESULT_PDN_EXT_NETWORK_ERROR_BEGIN                 = 0x00140000,
    /* PDN module cme error begin */
    MIPC_RESULT_PDN_EXT_CME_ERROR_BEGIN                     = 0x00140064,
    /* Insufficient resources (#26) */
    MIPC_RESULT_PDN_EXT_CME_INSUFFICIENT_RESOURCES_26       = 0x0014007E,
    /* Missing or unknown DNN (#27) */
    MIPC_RESULT_PDN_EXT_CME_MISSING_OR_UNKNOWN_DNN_27       = 0x0014007F,
    /* Unknown PDU session type (#28) */
    MIPC_RESULT_PDN_EXT_CME_UNKNOWN_PDU_SESSION_TYPE_28     = 0x00140080,
    /* User authentication or authorization failed (#29) */
    MIPC_RESULT_PDN_EXT_CME_USER_AUTHENTICATION_OR_AUTHORIZATION_FAILED_29 = 0x00140081,
    /* Activation rejected by Serving GW or PDN GW (#30) */
    MIPC_RESULT_PDN_EXT_CME_ACTIVATION_REJECTED_BY_SERVING_GW_OR_PDN_GW_30 = 0x00140082,
    /* Request rejected, unspecified (#31) */
    MIPC_RESULT_PDN_EXT_CME_REQUEST_REJECTED_UNSPECIFIED_31 = 0x00140083,
    /* Service option not supported (#32) */
    MIPC_RESULT_PDN_EXT_CME_SERVICE_OPTION_NOT_SUPPORTED_32 = 0x00140084,
    /* Requested service option not subscribed (#33) */
    MIPC_RESULT_PDN_EXT_CME_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED_33 = 0x00140085,
    /* Service option temporarily out of order (#34) */
    MIPC_RESULT_PDN_EXT_CME_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER_34 = 0x00140086,
    /* PTI already in use (#35) */
    MIPC_RESULT_PDN_EXT_CME_PTI_ALREADY_IN_USE_35           = 0x00140087,
    /* Feature not supported (#40) */
    MIPC_RESULT_PDN_EXT_CME_FEATURE_NOT_SUPPORTED_40        = 0x0014008C,
    /* Semantic error in the TFT operation (#41) */
    MIPC_RESULT_PDN_EXT_CME_SEMANTIC_ERROR_IN_THE_TFT_OPERATION_41 = 0x0014008D,
    /* Syntactical error in the TFT operation (#42) */
    MIPC_RESULT_PDN_EXT_CME_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION_42 = 0x0014008E,
    /* Invalid PDU session identity (#43) */
    MIPC_RESULT_PDN_EXT_CME_INVALID_PDU_SESSION_IDENTITY_43 = 0x0014008F,
    /* Semantic errors in packet filter(s) (#44) */
    MIPC_RESULT_PDN_EXT_CME_SEMANTIC_ERRORS_IN_PACKET_FILTERS_44 = 0x00140090,
    /* Syntactical errors in packet filter(s) (#45) */
    MIPC_RESULT_PDN_EXT_CME_SYNTACTICAL_ERRORS_IN_PACKET_FILTERS_45 = 0x00140091,
    /* PDP context without TFT already activated (#46) */
    MIPC_RESULT_PDN_EXT_CME_PDP_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED_46 = 0x00140092,
    /* APN restriction value incompatible with active EPS bearer context. */
    MIPC_RESULT_PDN_EXT_CME_PDP_AUTHENTICATION_FAILURE_112  = 0x00140095,
    /* Last PDN disconnection not allowed (#49) */
    MIPC_RESULT_PDN_EXT_CME_LAST_PDN_DISCONNECTION_NOT_ALLOWED_49 = 0x001400AB,
    /* Protocol error, unspecified (#111) */
    MIPC_RESULT_PDN_EXT_CME_PROTOCOL_ERROR_UNSPECIFIED_111  = 0x001400B0,
    /* Operator determined barring (#8) */
    MIPC_RESULT_PDN_EXT_CME_OPERATOR_DETERMINED_BARRING_8   = 0x001400B1,
    /* Maximum number of EPS bearers reached (#65) */
    MIPC_RESULT_PDN_EXT_CME_MAXIMUM_NUMBER_OF_EPS_BEARERS_REACHED_65 = 0x001400B2,
    /* Requested APN not supported in current RAT and PLMN combination (#66) */
    MIPC_RESULT_PDN_EXT_CME_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION_66 = 0x001400B3,
    /* Request rejected, bearer control mode violation (#48) */
    MIPC_RESULT_PDN_EXT_CME_REQUEST_REJECTED_BEARER_CONTROL_MODE_VIOLATION_48 = 0x001400B4,
    /* unsupported QCI value (#59) */
    MIPC_RESULT_PDN_EXT_CME_UNSUPPORTED_QCI_VALUE_59        = 0x001400B5,
    /* PDN module cme error end */
    MIPC_RESULT_PDN_EXT_CME_ERROR_END                       = 0x001400C8,
    /* PDN module network SM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_BEGIN              = 0x00140C00,
    /* there is no such cause in SM spec, just for default */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_EMPTY              = 0x00140C01,
    /* OPERATOR DETERMINED BARRING as spec */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_OPERATOR_DETERMINED_BARRING = 0x00140C08,
    /* LLC_SND_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_LLC_SND_FAILURE    = 0x00140C19,
    /* INSUFF_RESOURCE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_INSUFF_RESOURCE    = 0x00140C1A,
    /* UNKNOWN_APN */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNKNOWN_APN        = 0x00140C1B,
    /* UNKNOWN_PDP_ADDR_OR_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNKNOWN_PDP_ADDR_OR_TYPE = 0x00140C1C,
    /* AUTHENTICATION_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_AUTHENTICATION_FAILURE = 0x00140C1D,
    /* ACTIVATION_REJ_GGSN */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_ACTIVATION_REJ_GGSN = 0x00140C1E,
    /* ACTIVATION_REJ_UNSPECIFIED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_ACTIVATION_REJ_UNSPECIFIED = 0x00140C1F,
    /* UNSUPPORTED_SERVICE_OPTION */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNSUPPORTED_SERVICE_OPTION = 0x00140C20,
    /* UNSUBSCRIBED_SERVICE_OPTION */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNSUBSCRIBED_SERVICE_OPTION = 0x00140C21,
    /* SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER = 0x00140C22,
    /* PTI_ALREADY_USED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_PTI_ALREADY_USED   = 0x00140C23,
    /* REGULAR_DEACTIVATION */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_REGULAR_DEACTIVATION = 0x00140C24,
    /* QOS_NOT_ACCEPTED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_QOS_NOT_ACCEPTED   = 0x00140C25,
    /* NETWORK_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_NETWORK_FAIL       = 0x00140C26,
    /* REACTIVATION_REQD */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_REACTIVATION_REQD  = 0x00140C27,
    /* UNSUPPORTED_NW_CONTEXT_ACTIVATION */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNSUPPORTED_NW_CONTEXT_ACTIVATION = 0x00140C28,
    /* SEMANTIC_ERROR_IN_TFT_OP */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SEMANTIC_ERROR_IN_TFT_OP = 0x00140C29,
    /* SYNTACTICAL_ERROR_IN_TFT_OP */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SYNTACTICAL_ERROR_IN_TFT_OP = 0x00140C2A,
    /* UNKNOWN_PDP_CONTEXT */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNKNOWN_PDP_CONTEXT = 0x00140C2B,
    /* SEMANTIC_ERROR_IN_PACKET_FILTER */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SEMANTIC_ERROR_IN_PACKET_FILTER = 0x00140C2C,
    /* SYNTAX_ERROR_IN_PACKET_FILTER */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SYNTAX_ERROR_IN_PACKET_FILTER = 0x00140C2D,
    /* PDP_CONTEXT_WO_TFT_ALREADY_ACT */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_PDP_CONTEXT_WO_TFT_ALREADY_ACT = 0x00140C2E,
    /* PTI_MISMATCH */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_PTI_MISMATCH       = 0x00140C2F,
    /* ACTIVATION_REJ_BCM_VIOLATION */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_ACTIVATION_REJ_BCM_VIOLATION = 0x00140C30,
    /* LAST_PDN_DISC_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_LAST_PDN_DISC_NOT_ALLOWED = 0x00140C31,
    /* PDP_TYPE_IPV4_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_PDP_TYPE_IPV4_ONLY_ALLOWED = 0x00140C32,
    /* PDP_TYPE_IPV6_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_PDP_TYPE_IPV6_ONLY_ALLOWED = 0x00140C33,
    /* SINGLE_ADDR_BEARERS_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SINGLE_ADDR_BEARERS_ONLY_ALLOWED = 0x00140C34,
    /* ESM_INFORMATION_NOT_RECEIVED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_ESM_INFORMATION_NOT_RECEIVED = 0x00140C35,
    /* PDN_CONNENCTION_NOT_EXIST */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_PDN_CONNENCTION_NOT_EXIST = 0x00140C36,
    /* MULTIPLE_PDN_APN_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_MULTIPLE_PDN_APN_NOT_ALLOWED = 0x00140C37,
    /* COLLISION_WITH_NW_INITIATED_REQ */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_COLLISION_WITH_NW_INITIATED_REQ = 0x00140C38,
    /* UNSUPPORTED_QCI_VALUE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_UNSUPPORTED_QCI_VALUE = 0x00140C3B,
    /* MAXIMUM_NUM_OF_PDP_CONTEXTS_REACHED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_MAXIMUM_NUM_OF_PDP_CONTEXTS_REACHED = 0x00140C41,
    /* REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION = 0x00140C42,
    /* INVALID_TI */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_INVALID_TI         = 0x00140C51,
    /* SM_SEMANTICALLY_INCORRECT_MSG */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SM_SEMANTICALLY_INCORRECT_MSG = 0x00140C5F,
    /* INVALID_MAND_INFO */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_INVALID_MAND_INFO  = 0x00140C60,
    /* SM_MSG_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SM_MSG_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED = 0x00140C61,
    /* SM_MSG_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SM_MSG_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 0x00140C62,
    /* IE_NON_EXISTENCE_OR_NOT_IMPLEMENTED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_IE_NON_EXISTENCE_OR_NOT_IMPLEMENTED = 0x00140C63,
    /* CONDITIONAL_IE_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_CONDITIONAL_IE_ERROR = 0x00140C64,
    /* SM_MSG_NOT_COMPATIBLE_WITH_PROTOCOL_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SM_MSG_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 0x00140C65,
    /* SM_PROTOCOL_ERROR_UNSPECIFIED */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_SM_PROTOCOL_ERROR_UNSPECIFIED = 0x00140C6F,
    /* APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_PDP_CTX */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_PDP_CTX = 0x00140C70,
    /* PDN module network SM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_SM_CAUSE_END                = 0x00140C71,
    /* PDN module network ESM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_BEGIN             = 0x00140D00,
    /* there is no spec,default value */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_NO_CAUSE          = 0x00140D01,
    /* ESM_OPERATOR_DETERMINED_BARRING */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_OPERATOR_DETERMINED_BARRING = 0x00140D08,
    /* ESM_INSUFFICIENT_RESOURCES */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_INSUFFICIENT_RESOURCES = 0x00140D1A,
    /* ESM_UNKNOWN_OR_MISSING_APN */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_UNKNOWN_OR_MISSING_APN = 0x00140D1B,
    /* ESM_UNKNOWN_PDN_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_UNKNOWN_PDN_TYPE  = 0x00140D1C,
    /* USER_AUTH_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_USER_AUTH_FAILED  = 0x00140D1D,
    /* REQUEST_REJECTED_BY_SGW_OR_PDNGW */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_REQUEST_REJECTED_BY_SGW_OR_PDNGW  = 0x00140D1E,
    /* REQUEST_REJECTED_UNSPECIFIED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_REQUEST_REJECTED_UNSPECIFIED = 0x00140D1F,
    /* SERVICE_OPT_NOT_SUPPORTED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SERVICE_OPT_NOT_SUPPORTED = 0x00140D20,
    /* REQ_SERVICE_NOT_SUBSCRIBED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_REQ_SERVICE_NOT_SUBSCRIBED = 0x00140D21,
    /* ESM_SERVICE_TEMP_OUT_OF_ORDER */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SERVICE_TEMP_OUT_OF_ORDER = 0x00140D22,
    /* PTI_ALREADY_USED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_PTI_ALREADY_USED  = 0x00140D23,
    /* REGULAR_DEACTIVATION */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_REGULAR_DEACTIVATION = 0x00140D24,
    /* EPS_QOS_NOT_ACCEPTED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_EPS_QOS_NOT_ACCEPTED = 0x00140D25,
    /* NETWORK_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_NETWORK_FAILURE   = 0x00140D26,
    /* REACTIVATION_REQUESTED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_REACTIVATION_REQUESTED = 0x00140D27,
    /* SEMANTIC_ERROR_IN_TFT */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SEMANTIC_ERROR_IN_TFT = 0x00140D29,
    /* SYNTACTIC_ERROR_IN_TFT  */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SYNTACTIC_ERROR_IN_TFT = 0x00140D2A,
    /* INVALID_EPS_BEARER_IDENTITY  */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY = 0x00140D2B,
    /* SEMANTIC_ERROR_IN_PACKET_FILTERS  */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SEMANTIC_ERROR_IN_PACKET_FILTERS = 0x00140D2C,
    /* SYNTACTIC_ERROR_IN_PACKET_FILTERS  */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SYNTACTIC_ERROR_IN_PACKET_FILTERS = 0x00140D2D,
    /* EPSB_CTXT_WITHOUT_TFT_ACTIVATED  */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_EPSB_CTXT_WITHOUT_TFT_ACTIVATED = 0x00140D2E,
    /* PTI_MISMATCH */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_PTI_MISMATCH      = 0x00140D2F,
    /* LAST_PDN_DISC_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_LAST_PDN_DISC_NOT_ALLOWED = 0x00140D31,
    /* PDN_TYPE_IPV4_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED = 0x00140D32,
    /* PDN_TYPE_IPV6_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED = 0x00140D33,
    /* SINGLE_ADDRESS_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SINGLE_ADDRESS_ONLY_ALLOWED = 0x00140D34,
    /* ESM_INFORMATION_NOT_RECEIVED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED = 0x00140D35,
    /* PDN_CONNENCTION_NOT_EXIST */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_PDN_CONNENCTION_NOT_EXIST = 0x00140D36,
    /* MULTIPLE_PDN_APN_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_MULTIPLE_PDN_APN_NOT_ALLOWED = 0x00140D37,
    /* COLLISION_WITH_NW_INIT_REQUEST */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_COLLISION_WITH_NW_INIT_REQUESTD = 0x00140D38,
    /* UNSUPPORTED_QCI_VALUE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_UNSUPPORTED_QCI_VALUE = 0x00140D3B,
    /* MAXIMUM_NUM_OF_EPS_BEARERS_REACHED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_MAXIMUM_NUM_OF_EPS_BEARERS_REACHED = 0x00140D41,
    /* ESM_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_ESM_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION = 0x00140D42,
    /* INVALID_PTI_VALUE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_INVALID_PTI_VALUE = 0x00140D51,
    /* SEMANTIC_INCORRECT_MSG */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_SEMANTIC_INCORRECT_MSG = 0x00140D5F,
    /* INVALID_MANDATORY_IE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_INVALID_MANDATORY_IE = 0x00140D60,
    /* MSG_TYPE_NON_EXISTENT */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_MSG_TYPE_NON_EXISTENT = 0x00140D61,
    /* MSG_TYPE_NOT_COMPATIBLE_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_MSG_TYPE_NOT_COMPATIBLE_STATE = 0x00140D62,
    /* IE_NON_EXISTENT_NOT_IMPLEMENTED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_IE_NON_EXISTENT_NOT_IMPLEMENTED = 0x00140D63,
    /* CONDITIONAL_IE_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_CONDITIONAL_IE_ERROR = 0x00140D64,
    /* MSG_NOT_COMPATIBLE_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_MSG_NOT_COMPATIBLE_STATE = 0x00140D65,
    /* PROTOCOL_ERROR_UNSPECIFIED */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED = 0x00140D6F,
    /* APN_RESTRICT_VALUE_INCOMPATIBLE  */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_APN_RESTRICT_VALUE_INCOMPATIBLE  = 0x00140D70,
    /* PDN module network ESM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_ESM_CAUSE_END               = 0x00140D71,
    /* PDN module network TCM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_BEGIN             = 0x00140F00,
    /* TCM_L4C_CHECKED_ACT_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_CID_UNEXPECTED = 0x00140F01,
    /* TCM_L4C_CHECKED_ACT_WITH_CID_NOT_DEFINED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_CID_NOT_DEFINED = 0x00140F02,
    /* TCM_L4C_CHECKED_ACT_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_FSM_UNEXPECTED = 0x00140F03,
    /* TCM_L4C_CHECKED_ACT_WITH_CID_SECONDARY_WITHOUT_TFT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_CID_SECONDARY_WITHOUT_TFT = 0x00140F04,
    /* TCM_L4C_CHECKED_ACT_WITH_PRIMARY_IS_NOT_ACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_PRIMARY_IS_NOT_ACTIVATED = 0x00140F05,
    /* TCM_L4C_CHECKED_DEACT_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_DEACT_WITH_CID_UNEXPECTED = 0x00140F06,
    /* TCM_L4C_CHECKED_DEACT_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_DEACT_WITH_FSM_UNEXPECTED = 0x00140F07,
    /* TCM_L4C_CHECKED_DEACT_WITH_ACTIVATED_FROM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_DEACT_WITH_ACTIVATED_FROM_UNEXPECTED = 0x00140F08,
    /* TCM_L4C_CHECKED_MODIFY_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_MODIFY_WITH_CID_UNEXPECTED = 0x00140F09,
    /* TCM_L4C_CHECKED_MODIFY_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_MODIFY_WITH_FSM_UNEXPECTED = 0x00140F0A,
    /* TCM_L4C_CHECKED_MODIFY_WITH_NO_QOS_OR_TFT_SET */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_MODIFY_WITH_NO_QOS_OR_TFT_SET = 0x00140F0B,
    /* TCM_L4C_CHECKED_CGDATA_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_CGDATA_WITH_CID_UNEXPECTED = 0x00140F0C,
    /* TCM_L4C_CHECKED_CGDATA_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_CGDATA_WITH_FSM_UNEXPECTED = 0x00140F0D,
    /* TCM_L4C_DCONT_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_CID_UNEXPECTED = 0x00140F0E,
    /* TCM_L4C_DCONT_NOT_ALLOW_FOR_PPP_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_NOT_ALLOW_FOR_PPP_TYPE = 0x00140F0F,
    /* TCM_L4C_DCONT_INVALID_PARAMETER */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_INVALID_PARAMETER = 0x00140F10,
    /* TCM_L4C_DCONT_INVALID_PDP_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_INVALID_PDP_TYPE = 0x00140F11,
    /* TCM_L4C_DCONT_CID_ALREADY_IN_USE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_CID_ALREADY_IN_USE = 0x00140F12,
    /* TCM_L4C_DCONT_INVALID_APN */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_INVALID_APN = 0x00140F13,
    /* TCM_L4C_DSCONT_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DSCONT_CID_UNEXPECTED = 0x00140F14,
    /* TCM_L4C_DSCONT_INVALID_PARAMETER */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DSCONT_INVALID_PARAMETER = 0x00140F15,
    /* TCM_L4C_DSCONT_CID_PRIMARY_IS_NOT_IN_USE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DSCONT_CID_PRIMARY_IS_NOT_IN_USE = 0x00140F16,
    /* TCM_L4C_DSCONT_CID_PRIMARY_IS_NOT_ACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DSCONT_CID_PRIMARY_IS_NOT_ACTIVATED = 0x00140F17,
    /* TCM_L4C_DSCONT_NOT_ALLOW_FOR_PPP_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DSCONT_NOT_ALLOW_FOR_PPP_TYPE = 0x00140F18,
    /* TCM_L4C_DSCONT_CID_ALREADY_IN_USE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DSCONT_CID_ALREADY_IN_USE = 0x00140F19,
    /* TCM_L4C_PRCO_CID_ALREADY_IN_USE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_PRCO_CID_ALREADY_IN_USE = 0x00140F1A,
    /* TCM_L4C_PRCO_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_PRCO_CID_UNEXPECTED = 0x00140F1B,
    /* TCM_L4C_ACT_NOT_SUPPORT_EMERGENCY_BEARER_ACTIVATION */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACT_NOT_SUPPORT_EMERGENCY_BEARER_ACTIVATION = 0x00140F1C,
    /* TCM_L4C_ACT_ALLOC_NSAPI_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACT_ALLOC_NSAPI_FAIL = 0x00140F1D,
    /* TCM_L4C_DEACT_IND_BY_TIMER_EXPIRY */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DEACT_IND_BY_TIMER_EXPIRY = 0x00140F1E,
    /* TCM_L4C_DEACT_IND_BY_NO_USER_RESPONSE_TILL_RAT_CHANGE_COMPLETE_IND */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DEACT_IND_BY_NO_USER_RESPONSE_TILL_RAT_CHANGE_COMPLETE_IND = 0x00140F1F,
    /* TCM_L4C_MOD_PRESERVED_PDP_CONTEXT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_MOD_PRESERVED_PDP_CONTEXT = 0x00140F20,
    /* TCM_L4C_MOD_CHECK_TFT_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_MOD_CHECK_TFT_FAIL = 0x00140F21,
    /* TCM_L4C_MOD_REJ_DUE_TO_DEACT_IND */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_MOD_REJ_DUE_TO_DEACT_IND = 0x00140F22,
    /* TCM_L4C_SET_MULTI_PDN_SUCCESS */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_SET_MULTI_PDN_SUCCESS = 0x00140F23,
    /* TCM_L4C_SET_MULTI_PDN_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_SET_MULTI_PDN_CID_UNEXPECTED = 0x00140F24,
    /* TCM_L4C_SET_MULTI_PDN_PARA_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_SET_MULTI_PDN_PARA_UNEXPECTED = 0x00140F25,
    /* TCM_L4C_SET_MULTI_PDN_INVALID_PDP_TYPE_AND_LEN */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_SET_MULTI_PDN_INVALID_PDP_TYPE_AND_LEN = 0x00140F26,
    /* TCM_L4C_SET_MULTI_PDN_INVALID_APN */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_SET_MULTI_PDN_INVALID_APN = 0x00140F27,
    /* TCM_PPP_CHECKED_ACT_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_ACT_WITH_CID_UNEXPECTED = 0x00140F28,
    /* TCM_PPP_CHECKED_ACT_WITH_PDP_TYPE_LEN_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_ACT_WITH_PDP_TYPE_LEN_UNEXPECTED = 0x00140F29,
    /* TCM_PPP_CHECKED_ACT_WITH_INVALID_CONFIG_PROTOCOL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_ACT_WITH_INVALID_CONFIG_PROTOCOL = 0x00140F2A,
    /* TCM_PPP_CHECKED_ACT_WITH_CID_NOT_DEFINED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_ACT_WITH_CID_NOT_DEFINED = 0x00140F2B,
    /* TCM_PPP_CHECKED_ACT_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_ACT_WITH_FSM_UNEXPECTED = 0x00140F2C,
    /* TCM_PPP_CHECKED_DEACT_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_DEACT_WITH_CID_UNEXPECTED = 0x00140F2D,
    /* TCM_PPP_CHECKED_DEACT_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_DEACT_WITH_FSM_UNEXPECTED = 0x00140F2E,
    /* TCM_PPP_CHECKED_DEACT_WITH_ACTIVATED_FROM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_DEACT_WITH_ACTIVATED_FROM_UNEXPECTED = 0x00140F2F,
    /* TCM_PPP_CHECKED_CGDATA_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_CGDATA_WITH_CID_UNEXPECTED = 0x00140F30,
    /* TCM_PPP_CHECKED_CGDATA_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_PPP_CHECKED_CGDATA_WITH_FSM_UNEXPECTED = 0x00140F31,
    /* TCM_DISPATCH_PDP_ACT_CHECK_TFT_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_DISPATCH_PDP_ACT_CHECK_TFT_FAIL = 0x00140F32,
    /* TCM_UPCM_BIND_CNF_BEARER_HAS_BEEN_DEACTIVATED_ALREADY */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UPCM_BIND_CNF_BEARER_HAS_BEEN_DEACTIVATED_ALREADY = 0x00140F33,
    /* TCM_UPCM_UNBIND_CNF_BEARER_HAS_BEEN_DEACTIVATED_ALREADY */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UPCM_UNBIND_CNF_BEARER_HAS_BEEN_DEACTIVATED_ALREADY = 0x00140F34,
    /* TCM_ACL_APN_NOT_EXIST_IN_ACL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_APN_NOT_EXIST_IN_ACL = 0x00140F35,
    /* TCM_ACL_ACTION_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_ACTION_NOT_ALLOWED = 0x00140F36,
    /* TCM_ACL_SIM_FILE_FULL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_SIM_FILE_FULL = 0x00140F37,
    /* TCM_ACL_ADD_ENTRY_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_ADD_ENTRY_FAILED = 0x00140F38,
    /* TCM_ACL_DEL_ENTRY_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_DEL_ENTRY_FAILED = 0x00140F39,
    /* TCM_ACL_SET_ENTRY_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_SET_ENTRY_FAILED = 0x00140F3A,
    /* TCM_ACL_SIM_READ_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_SIM_READ_FAILED = 0x00140F3B,
    /* TCM_ACL_SIM_WRITE_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ACL_SIM_WRITE_FAILED = 0x00140F3C,
    /* TCM_ESM_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_CAUSE_START = 0x00140F3D,
    /* TCM_ESM_FAIL_CAUSE_EMPTY */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_EMPTY = 0x00140F3E,
    /* TCM_ESM_USER_CANCEL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_USER_CANCEL = 0x00140F3F,
    /* TCM_ESM_CID_ALREADY_CONNECT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_CID_ALREADY_CONNECT = 0x00140F40,
    /* TCM_ESM_PDN_NOT_ACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_PDN_NOT_ACTIVATED = 0x00140F41,
    /* TCM_ESM_BEARER_NOT_ACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_BEARER_NOT_ACTIVATED = 0x00140F42,
    /* TCM_ESM_RMV_LAST_PDN_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_RMV_LAST_PDN_NOT_ALLOWED = 0x00140F43,
    /* TCM_ESM_LOCAL_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_LOCAL_RELEASE = 0x00140F44,
    /* TCM_ESM_DETACH */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_DETACH    = 0x00140F45,
    /* TCM_ESM_IE_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_IE_ERROR  = 0x00140F46,
    /* TCM_ESM_TIMER_TIMEOUT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_TIMER_TIMEOUT = 0x00140F47,
    /* TCM_ESM_TX_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_TX_FAILURE = 0x00140F48,
    /* TCM_ESM_FAIL_CAUSE_RAT_CHANGE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_RAT_CHANGE = 0x00140F49,
    /* TCM_ESM_FAIL_CAUSE_PS_SWITCH */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_PS_SWITCH = 0x00140F4A,
    /* TCM_ESM_ACTION_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_ACTION_NOT_ALLOWED = 0x00140F4B,
    /* TCM_ESM_FAIL_CAUSE_T3346_CONGESTION */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_T3346_CONGESTION = 0x00140F4C,
    /* TCM_ESM_FAIL_CAUSE_THROTTLING_RUNNING */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_THROTTLING_RUNNING = 0x00140F4D,
    /* TCM_ESM_FAIL_CAUSE_SERIOUS_ERROR_AT_LOW_LAYER */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_SERIOUS_ERROR_AT_LOW_LAYER = 0x00140F4E,
    /* TCM_ESM_FAIL_CAUSE_OTHERS */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_OTHERS = 0x00140F4F,
    /* TCM_ESM_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_CAUSE_END = 0x00140F50,
    /* RAT_TCM_RETRY_NEEDED_AFTER_IRAT_IF_IN_3GPP */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_RETRY_NEEDED_AFTER_IRAT_IF_IN_3GPP = 0x00140F51,
    /* RAT_TCM_LOCAL_REJECTED_DUE_TO_PS_SWITCH */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_LOCAL_REJECTED_DUE_TO_PS_SWITCH = 0x00140F52,
    /* AP_RETRY_NEEDED_AFTER_LTE_TO_C2K_IRAT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_AP_RETRY_NEEDED_AFTER_LTE_TO_C2K_IRAT = 0x00140F53,
    /* RAT_TCM_DEACT_SUCCESS_DUE_TO_CONTEXT_NEVER_ACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_DEACT_SUCCESS_DUE_TO_CONTEXT_NEVER_ACTIVATED = 0x00140F54,
    /* RAT_TCM_MODIFY_FAIL_DUE_TO_CONTEXT_DEACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_MODIFY_FAIL_DUE_TO_CONTEXT_DEACTIVATED = 0x00140F55,
    /* RAT_TCM_MODIFY_FAIL_DUE_TO_CONTEXT_NOT_ACTIVATED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_MODIFY_FAIL_DUE_TO_CONTEXT_NOT_ACTIVATED = 0x00140F56,
    /* RAT_TCM_DEACTIVATED_DUE_TO_NAS_TFT_TRIGGERED_DEACTIVATION */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_DEACTIVATED_DUE_TO_NAS_TFT_TRIGGERED_DEACTIVATION = 0x00140F57,
    /* RAT_TCM_23G_MT_PRIMARY_PDP_IS_ABORTED_DUE_TO_CHANGE_TO_4G */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_23G_MT_PRIMARY_PDP_IS_ABORTED_DUE_TO_CHANGE_TO_4G = 0x00140F58,
    /* RAT_TCM_DEACTIVATE_SUCCESS_IMMEDIATELY_BECAUSE_ACTIVATION_IS_NOT_PROCESSED_BY_TCM_YET */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_DEACTIVATE_SUCCESS_IMMEDIATELY_BECAUSE_ACTIVATION_IS_NOT_PROCESSED_BY_TCM_YET = 0x00140F59,
    /* RAT_TCM_EGACT_ACTIVATE_REJ_DUE_TO_ERROR_RAT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_EGACT_ACTIVATE_REJ_DUE_TO_ERROR_RAT = 0x00140F5A,
    /* RAT_TCM_EGACT_DEACTIVATE_REJ_DUE_TO_ERROR_RAT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_EGACT_DEACTIVATE_REJ_DUE_TO_ERROR_RAT = 0x00140F5B,
    /* RAT_TCM_DEACTIVATED_DUE_TO_INACTIVITY_TIMER_TIMEOUT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_DEACTIVATED_DUE_TO_INACTIVITY_TIMER_TIMEOUT = 0x00140F5C,
    /* TCM_ETCM_CID_NOT_DEFINE_TFT_QOS */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ETCM_CID_NOT_DEFINE_TFT_QOS = 0x00140F5D,
    /* TCM_ETCM_CID_IS_NOT_PRIMARY */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ETCM_CID_IS_NOT_PRIMARY = 0x00140F5E,
    /* TCM_ETCM_TFT_VALIDATION_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ETCM_TFT_VALIDATION_ERROR = 0x00140F5F,
    /* TCM_ETCM_PTI_IS_FULL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ETCM_PTI_IS_FULL = 0x00140F60,
    /* TCM_L4C_CHECKED_EGLD_WITH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_EGLD_WITH_CID_UNEXPECTED = 0x00140F61,
    /* TCM_L4C_CHECKED_EGLD_WITH_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_EGLD_WITH_FSM_UNEXPECTED = 0x00140F62,
    /* TCM_L4C_CHECKED_EAPNSYNC_WRONG_APN_IDX */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_EAPNSYNC_WRONG_APN_IDX = 0x00140F63,
    /* TCM_L4C_CHECKED_EAPNSYNC_APN_TOO_LONG */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_EAPNSYNC_APN_TOO_LONG = 0x00140F64,
    /* TCM_L4C_CHECKED_EAPNSYNC_TOO_MUCH_APN_SETTING */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_EAPNSYNC_TOO_MUCH_APN_SETTING = 0x00140F65,
    /* TCM_L4C_MOD_PRIMARY_QOS_IS_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_MOD_PRIMARY_QOS_IS_NOT_ALLOWED = 0x00140F66,
    /* TCM_L4C_MOD_QOS_FOR_NO_MS_PF_IS_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_MOD_QOS_FOR_NO_MS_PF_IS_NOT_ALLOWED = 0x00140F67,
    /* RAT_TCM_ACT_FAIL_DUE_TO_IRAT_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_ACT_FAIL_DUE_TO_IRAT_ONGOING = 0x00140F6B,
    /* RAT_TCM_MODIFY_FAIL_DUE_TO_IRAT_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_MODIFY_FAIL_DUE_TO_IRAT_ONGOING = 0x00140F6C,
    /* RAT_TCM_DEACT_FAIL_DUE_TO_IRAT_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_DEACT_FAIL_DUE_TO_IRAT_ONGOING = 0x00140F6D,
    /* RAT_TCM_LOCAL_REJECTED_DUE_TO_FSM_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_LOCAL_REJECTED_DUE_TO_FSM_UNEXPECTED = 0x00140F6E,
    /* RAT_TCM_LOCAL_REJECTED_DUE_TO_GUARANTEE_RSPONSE_TIME_TIMEOUT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_LOCAL_REJECTED_DUE_TO_GUARANTEE_RSPONSE_TIME_TIMEOUT = 0x00140F6F,
    /* TCM_REGION_2_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_2_CAUSE_START = 0x00140F70,
    /* TCM_L4C_CHECKED_CID_INVALID */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_CID_INVALID = 0x00140F71,
    /* TCM_L4C_CHECKED_CID_ALREADY_IN_ACTION */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_CID_ALREADY_IN_ACTION = 0x00140F72,
    /* TCM_L4C_CHECKED_ACT_WITH_UNEXPECTED_LTE_ATTACH_PDN_IN_23G */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_UNEXPECTED_LTE_ATTACH_PDN_IN_23G = 0x00140F73,
    /* TCM_UGTCM_DEACT_DUE_TO_USER_FORCE_TO_LOCAL_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UGTCM_DEACT_DUE_TO_USER_FORCE_TO_LOCAL_RELEASE = 0x00140F74,
    /* TCM_L4C_ACT_NOT_SUPPORT_HANDOVER_OF_EMERGENCY_BEARER */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACT_NOT_SUPPORT_HANDOVER_OF_EMERGENCY_BEARER = 0x00140F75,
    /* TCM_UGTCM_DEACT_DUE_TO_AOSP_SHUTDOWN */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UGTCM_DEACT_DUE_TO_AOSP_SHUTDOWN = 0x00140F76,
    /* TCM_UGTCM_DEACT_DUE_TO_AOSP_HANDOVER */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UGTCM_DEACT_DUE_TO_AOSP_HANDOVER = 0x00140F77,
    /* TCM_ESM_FAIL_CAUSE_GEMINI_SUSPEND */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_GEMINI_SUSPEND = 0x00140F78,
    /* TCM_ESM_FAIL_CAUSE_GEMINI_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_GEMINI_FAILURE = 0x00140F79,
    /* TCM_ESM_FAIL_CAUSE_GEMINI_SUSPEND_DUE_TO_CALL_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_GEMINI_SUSPEND_DUE_TO_CALL_ONGOING = 0x00140F7A,
    /* TCM_UGTCM_DEACT_DUE_TO_APN_CHANGED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UGTCM_DEACT_DUE_TO_APN_CHANGED = 0x00140F7B,
    /* TCM_ESM_DETACH_REATTACH */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_DETACH_REATTACH = 0x00140F7C,
    /* TCM_VGTCM_DEACT_DUE_TO_AOSP_SHUTDOWN */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_VGTCM_DEACT_DUE_TO_AOSP_SHUTDOWN = 0x00140F7D,
    /* TCM_ESM_FAIL_CAUSE_REQ_RES_APN_MISMATCH */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_FAIL_CAUSE_REQ_RES_APN_MISMATCH = 0x00140F7E,
    /* TCM_UGTCM_DEACT_DUE_TO_IMS_HANDOVER */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_UGTCM_DEACT_DUE_TO_IMS_HANDOVER = 0x00140F7F,
    /* TCM_ESM_DEACT_DUE_TO_DRB_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_DEACT_DUE_TO_DRB_RELEASE = 0x00140F8E,
    /* TCM_REGION_2_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_2_CAUSE_END = 0x00140F8F,
    /* TCM_REGION_3_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_3_CAUSE_START = 0x00140F90,
    /* TCM_L4C_DCONT_PSI_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_DCONT_PSI_UNEXPECTED = 0x00140F91,
    /* TCM_L4C_CHECKED_ACT_WITH_UNEXPECTED_LTE_ATTACH_PDN_IN_5G */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_UNEXPECTED_LTE_ATTACH_PDN_IN_5G = 0x00140F92,
    /* TCM_L4C_CHECKED_ACT_WITH_CID_SECONDARY_NOT_SUPPORT_IN_5G */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHECKED_ACT_WITH_CID_SECONDARY_NOT_SUPPORT_IN_5G = 0x00140F93,
    /* TCM_VGTCM_DEACT_DUE_TO_USER_FORCE_TO_LOCAL_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_VGTCM_DEACT_DUE_TO_USER_FORCE_TO_LOCAL_RELEASE = 0x00140F94,
    /* TCM_L4C_EGPCO_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_EGPCO_CID_UNEXPECTED = 0x00140F95,
    /* TCM_L4C_EGPCO_PSI_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_EGPCO_PSI_UNEXPECTED = 0x00140F96,
    /* TCM_L4C_EGDCONT_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_EGDCONT_CID_UNEXPECTED = 0x00140F97,
    /* TCM_L4C_EGDCONT_PSI_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_EGDCONT_PSI_UNEXPECTED = 0x00140F98,
    /* TCM_L4C_EGDCONT_INVALID_PDP_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_EGDCONT_INVALID_PDP_TYPE = 0x00140F99,
    /* RAT_TCM_HANDOVER_REESTABLISHMENT_NEEDED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_HANDOVER_REESTABLISHMENT_NEEDED = 0x00140F9A,
    /* RAT_TCM_HANDOVER_REESTABLISHMENT_FAIL_DUE_TO_NO_WITHOUT_N26_INFO */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_HANDOVER_REESTABLISHMENT_FAIL_DUE_TO_NO_WITHOUT_N26_INFO = 0x00140F9B,
    /* TCM_VGTCM_DEACT_DUE_TO_INTERRAT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_VGTCM_DEACT_DUE_TO_INTERRAT = 0x00140F9C,
    /* RAT_TCM_LOCAL_REJECTED_DUE_TO_ONGOING_PROCESS */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_RAT_TCM_LOCAL_REJECTED_DUE_TO_ONGOING_PROCESS = 0x00140F9D,
    /* TCM_REGION_3_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_3_CAUSE_END = 0x00140FAF,
    /* TCM_REGION_4_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_4_CAUSE_START = 0x00140FB0,
    /* TCM_L4C_PSI_NOT_DEFINED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_PSI_NOT_DEFINED = 0x00140FB1,
    /* TCM_L4C_VGTCM_PROCEDURE_ONGOING_ON_CORRESPONDING_CID */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_VGTCM_PROCEDURE_ONGOING_ON_CORRESPONDING_CID = 0x00140FB2,
    /* TCM_L4C_CHANGE_PSI_FOR_ACTIVE_CID_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CHANGE_PSI_FOR_ACTIVE_CID_NOT_ALLOWED = 0x00140FB3,
    /* TCM_L4C_CGAUTH_CID_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CGAUTH_CID_UNEXPECTED = 0x00140FB4,
    /* TCM_L4C_CGAUTH_PSI_UNEXPECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_CGAUTH_PSI_UNEXPECTED = 0x00140FB5,
    /* TCM_L4C_VGTCM_UNEXPECTED_FIC_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_VGTCM_UNEXPECTED_FIC_TYPE = 0x00140FB6,
    /* TCM_L4C_PEER_TO_PEER_MODIFY_WITHOUT_P_CSCF */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_PEER_TO_PEER_MODIFY_WITHOUT_P_CSCF = 0x00140FB7,
    /* TCM_L4C_PEER_TO_PEER_MODIFY_WITH_P_CSCF */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_PEER_TO_PEER_MODIFY_WITH_P_CSCF = 0x00140FB8,
    /* TCM_L4C_MODIFY_DUE_TO_INTERRAT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_MODIFY_DUE_TO_INTERRAT = 0x00140FB9,
    /* TCM_L4C_ACTIVATE_FAIL_DUE_TO_SAT_DATA_CONNECTION_CTRL_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACTIVATE_FAIL_DUE_TO_SAT_DATA_CONNECTION_CTRL_FAIL = 0x00140FBA,
    /* TCM_L4C_ACTIVATE_FAIL_DUE_TO_SAT_NOT_ALLOW */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACTIVATE_FAIL_DUE_TO_SAT_NOT_ALLOW = 0x00140FBB,
    /* TCM_L4C_ACTIVATE_FAIL_DUE_TO_SAT_SIM_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACTIVATE_FAIL_DUE_TO_SAT_SIM_ERROR = 0x00140FBC,
    /* TCM_L4C_ACTIVATE_FAIL_DUE_TO_DEACTIVATE_ABORT */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_L4C_ACTIVATE_FAIL_DUE_TO_DEACTIVATE_ABORT = 0x00140FBD,
    /* TCM_REGION_4_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_4_CAUSE_END = 0x00140FCF,
    /* TCM_REGION_5_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_5_CAUSE_START = 0x00140FD0,
    /* TCM_ESM_REGION_2_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_REGION_2_CAUSE_START = 0x00140FD0,
    /* TCM_ESM_4G5_EPS_BEARER_MAPPED_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_4G5_EPS_BEARER_MAPPED_FAILED = 0x00140FD1,
    /* TCM_ESM_ATTACH_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_ATTACH_FAILURE = 0x00140FD2,
    /* TCM_ESM_REGION_2_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_ESM_REGION_2_CAUSE_END = 0x00140FE0,
    /* TCM_REGION_5_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_TCM_REGION_5_CAUSE_END = 0x00140FFF,
    /* PDN module network TCM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_TCM_CAUSE_END               = 0x00140FFF,
    /* PDN module network PAM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_BEGIN             = 0x00141200,
    /* PAM_PDN_ACCESS_GRANTED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_GRANTED = 0x00141201,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_08 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_08 = 0x00141208,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_26 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_26 = 0x0014121A,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_27 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_27 = 0x0014121B,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_28 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_28 = 0x0014121C,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_29 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_29 = 0x0014121D,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_30 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_30 = 0x0014121E,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_31 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_31 = 0x0014121F,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_32 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_32 = 0x00141220,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_33 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_33 = 0x00141221,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_34 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_34 = 0x00141222,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_35 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_35 = 0x00141223,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_36 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_36 = 0x00141224,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_37 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_37 = 0x00141225,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_38 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_38 = 0x00141226,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_39 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_39 = 0x00141227,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_41 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_41 = 0x00141229,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_42 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_42 = 0x0014122A,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_43 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_43 = 0x0014122B,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_44 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_44 = 0x0014122C,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_45 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_45 = 0x0014122D,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_46 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_46 = 0x0014122E,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_47 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_47 = 0x0014122F,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_49 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_49 = 0x00141231,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_50 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_50 = 0x00141232,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_51 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_51 = 0x00141233,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_52 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_52 = 0x00141234,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_53 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_53 = 0x00141235,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_54 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_54 = 0x00141236,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_55 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_55 = 0x00141237,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_56 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_56 = 0x00141238,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_59 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_59 = 0x0014123B,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_65 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_65 = 0x00141241,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_66 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_66 = 0x00141242,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_81 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_81 = 0x00141251,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_95 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_95 = 0x0014125F,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_96 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_96 = 0x00141260,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_97 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_97 = 0x00141261,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_98 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_98 = 0x00141262,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_99 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_99 = 0x00141263,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_100 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_100 = 0x00141264,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_101 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_101 = 0x00141265,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_111 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_111 = 0x0014126F,
    /* PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_112 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_DUE_TO_CAUSE_112 = 0x00141270,
    /* PAM_PDN_ACCESS_REJECT_NOT_IN_ACL */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_NOT_IN_ACL = 0x001412C8,
    /* PAM_PDN_ACCESS_REJECT_READ_EF_ACL_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_PAM_PDN_ACCESS_REJECT_READ_EF_ACL_ONGOING = 0x001412C9,
    /* PDN module network PAM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_CAUSE_END               = 0x001412FF,
    /* PDN module network PAM OP12 error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_BEGIN        = 0x00141300,
    /* PAM_OP12_PDN_ACCESS_GRANTED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_GRANTED = 0x00141301,
    /* PAM_OP12_PDN_ACCESS_REJECT_DISABLE_IN_APN_TABLE */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_DISABLE_IN_APN_TABLE = 0x00141302,
    /* PAM_OP12_PDN_ACCESS_REJECT_ASK_APN_CHANGE_NOT_READY */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_ASK_APN_CHANGE_NOT_READY = 0x00141303,
    /* PAM_OP12_PDN_ACCESS_REJECT_CHANGE_APN_CLASS */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_CHANGE_APN_CLASS = 0x00141304,
    /* PAM_OP12_PDN_ACCESS_REJECT_CHANGE_APN_CLASS_AND_THROTTLE */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_CHANGE_APN_CLASS_AND_THROTTLE = 0x00141305,
    /* PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_FOREVER */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_FOREVER = 0x00141306,
    /* PAM_OP12_PDN_ACCESS_REJECT_THROTTLE */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_THROTTLE = 0x00141307,
    /* PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_WITH_BACKOFF_TIMER */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_WITH_BACKOFF_TIMER = 0x00141308,
    /* PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_FOREVER_CANDIDATE */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_FOREVER_CANDIDATE = 0x00141309,
    /* PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_NO_RSP */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_NO_RSP = 0x0014130A,
    /* PAM_OP12_PDN_ACCESS_REJECT_NOT_IN_ACL */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_NOT_IN_ACL = 0x0014130B,
    /* PAM_OP12_PDN_ACCESS_REJECT_STOP_ATTACH_FOR_CLASS1_AND_CLASS2_NOT_ENABLED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_STOP_ATTACH_FOR_CLASS1_AND_CLASS2_NOT_ENABLED = 0x0014130C,
    /* PAM_OP12_PDN_ACCESS_REJECT_WAIT_TIME_NOT_EXPIRED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_WAIT_TIME_NOT_EXPIRED = 0x0014130D,
    /* PAM_OP12_PDN_ACCESS_REJECT_EXCEED_MAX_PDN_CONN_TRIES */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_EXCEED_MAX_PDN_CONN_TRIES = 0x0014130E,
    /* PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_BY_MAX_PDN_CONN_TRIES */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_THROTTLE_BY_MAX_PDN_CONN_TRIES = 0x0014130F,
    /* PAM_OP12_PDN_ACCESS_REJECT_NO_AVAILABLE_CLASS_CAN_BE_USED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_NO_AVAILABLE_CLASS_CAN_BE_USED = 0x00141310,
    /* PAM_OP12_PDN_ACCESS_REJECT_APN_IN_USE */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_APN_IN_USE = 0x00141311,
    /* PAM_OP12_PDN_ACCESS_REJECT_VZ_REQ_LTEDATA_39683_FEB_2016 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_VZ_REQ_LTEDATA_39683_FEB_2016 = 0x00141312,
    /* PAM_OP12_PDN_ACCESS_REJECT_VZ_REQ_LTEDATA_39684_FEB_2016 */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_VZ_REQ_LTEDATA_39684_FEB_2016 = 0x00141313,
    /* PAM_OP12_PDN_ACCESS_REJECT_ALLOC */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_ALLOC = 0x00141314,
    /* PAM_OP12_PDN_ACCESS_REJECT_MOD */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDN_ACCESS_REJECT_MOD = 0x00141315,
    /* PAM_OP12_PDU_ACCESS_GRANTED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_GRANTED = 0x00141320,
    /* PAM_OP12_PDU_ACCESS_REJECT_THROTTLE_FOREVER */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_REJECT_THROTTLE_FOREVER = 0x00141321,
    /* PAM_OP12_PDU_ACCESS_REJECT_THROTTLE */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_REJECT_THROTTLE = 0x00141322,
    /* PAM_OP12_PDU_ACCESS_REJECT_THROTTLE_WITH_BACKOFF_TIMER */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_REJECT_THROTTLE_WITH_BACKOFF_TIMER = 0x00141323,
    /* PAM_OP12_PDU_ACCESS_REJECT_THROTTLE_NO_RSP */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_REJECT_THROTTLE_NO_RSP = 0x00141324,
    /* PAM_OP12_PDU_ACCESS_REJECT_MOD */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_REJECT_MOD = 0x00141325,
    /* PAM_OP12_PDU_ACCESS_REJECT_EST */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_REJECT_EST = 0x00141326,
    /* PAM_OP12_PDU_ACCESS_THROTTLE_EXEMPTION */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_PAM_OP12_PDU_ACCESS_THROTTLE_EXEMPTION = 0x00141327,
    /* PDN module network PAM OP12 error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_OP12_CAUSE_END          = 0x001413FF,
    /* PDN module network PAM AT&T error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_ATT_CAUSE_BEGIN         = 0x00141400,
    /* PAM_ATT_PDN_ACCESS_GRANTED */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_ATT_CAUSE_PAM_ATT_PDN_ACCESS_GRANTED = 0x00141401,
    /* PAM_ATT_PDN_ACCESS_REJECT_IMS_PDN_BLOCK_TEMP */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_ATT_CAUSE_PAM_ATT_PDN_ACCESS_REJECT_IMS_PDN_BLOCK_TEMP = 0x00141402,
    /* PAM_ATT_PDN_ACCESS_REJECT_IMS_PDN_BLOCK_FOREVER */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_ATT_CAUSE_PAM_ATT_PDN_ACCESS_REJECT_IMS_PDN_BLOCK_FOREVER = 0x00141403,
    /* PDN module network PAM AT&T error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_PAM_ATT_CAUSE_END           = 0x001414FF,
    /* PDN module network L4BPDN error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_BEGIN          = 0x00141500,
    /* L4BPDN_AT_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_OK   = 0x00141501,
    /* L4BPDN_AT_ERROR_CMD_MODE_NOT_SUPPORT */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CMD_MODE_NOT_SUPPORT = 0x00141502,
    /* L4BPDN_AT_ERROR_CURRENT_RAT_UNKNOWN */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CURRENT_RAT_UNKNOWN = 0x00141503,
    /* L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_CGCONTRDP */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_CGCONTRDP = 0x00141504,
    /* L4BPDN_AT_ERROR_CID_IS_NOT_VALID */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_NOT_VALID = 0x00141505,
    /* L4BPDN_AT_ERROR_IE_PARSE_RESULT_IS_NOT_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_IE_PARSE_RESULT_IS_NOT_OK = 0x00141506,
    /* L4BPDN_AT_ERROR_PCO_ID_IS_NOT_VALID */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_PCO_ID_IS_NOT_VALID = 0x00141507,
    /* L4BPDN_AT_ERROR_PCO_ID_IS_NOT_SUPPORTED */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_PCO_ID_IS_NOT_SUPPORTED = 0x00141508,
    /* L4BPDN_AT_ERROR_PCO_ID_SUPPORTED_IS_NOT_PRESENT */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_PCO_ID_SUPPORTED_IS_NOT_PRESENT = 0x00141509,
    /* L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_BINDING */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_BINDING = 0x0014150A,
    /* L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_UNBINDING */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_UNBINDING = 0x0014150B,
    /* L4BPDN_AT_ERROR_CMD_NOT_SUPPORT_IN_CURRENT_DOMAIN */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CMD_NOT_SUPPORT_IN_CURRENT_DOMAIN = 0x0014150C,
    /* L4BPDN_AT_ERROR_CID_IS_ALREADY_BINDED */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_ALREADY_BINDED = 0x0014150D,
    /* L4BPDN_AT_ERROR_CID_IS_ALREADY_UNBINDED */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_ALREADY_UNBINDED = 0x0014150E,
    /* L4BPDN_AT_ERROR_CMD_NOT_SUPPORT */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CMD_NOT_SUPPORT = 0x0014150F,
    /* L4BPDN_AT_ERROR_CGCMOD_NOT_SUPPORT_IN_C2K */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CGCMOD_NOT_SUPPORT_IN_C2K = 0x00141510,
    /* L4BPDN_AT_ERROR_EGACT_NOT_SUPPORT_IN_C2K */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_EGACT_NOT_SUPPORT_IN_C2K = 0x00141511,
    /* L4BPDN_AT_ERROR_CID_IS_ACTIVE_BUT_NOT_PRIMARY_PDP */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_ACTIVE_BUT_NOT_PRIMARY_PDP = 0x00141512,
    /* L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_SENDING_DATA */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_CID_IS_NOT_ACTIVE_FOR_SENDING_DATA = 0x00141513,
    /* L4BPDN_CGEV_IND_FROM_CVAL_WITHOUT_CAUSE */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_CGEV_IND_FROM_CVAL_WITHOUT_CAUSE = 0x00141514,
    /* L4BPDN_CGEV_IND_NO_CAUSE */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_CGEV_IND_NO_CAUSE = 0x00141515,
    /* L4BPDN_AT_ERROR_EGACT_UNEXPECTED_RAT_IN_C2K */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_EGACT_UNEXPECTED_RAT_IN_C2K = 0x00141516,
    /* L4BPDN_AT_ERROR_C2K_LTE_DEACT_RETRY_NEEDED */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_AT_ERROR_C2K_LTE_DEACT_RETRY_NEEDED = 0x00141517,
    /* L4BPDN_LEISIM_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_CAUSE_START = 0x00141580,
    /* L4BPDN_LEISIM_AT_ERROR_CID_IS_ALREADY_BINDED */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_AT_ERROR_CID_IS_ALREADY_BINDED = 0x00141581,
    /* L4BPDN_LEISIM_AT_ERROR_CID_PDN_TRANSFER_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_AT_ERROR_CID_PDN_TRANSFER_ONGOING = 0x00141582,
    /* L4BPDN_LEISIM_OTHER_DIRECTION_IRAT_STARTED_ABORT_REVIVE */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_OTHER_DIRECTION_IRAT_STARTED_ABORT_REVIVE = 0x00141583,
    /* L4BPDN_LEISIM_EHRPD_ALL_3_ROUNDS_REVIVE_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_EHRPD_ALL_3_ROUNDS_REVIVE_FAIL = 0x00141584,
    /* L4BPDN_LEISIM_DEACT_ABORT_REVIVE */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_DEACT_ABORT_REVIVE = 0x00141585,
    /* L4BPDN_LEISIM_EHRPD_ONE_PDN_REVIVE_SUCCESS_NO_NEED_REVIVE_OTHER */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_EHRPD_ONE_PDN_REVIVE_SUCCESS_NO_NEED_REVIVE_OTHER = 0x00141586,
    /* L4BPDN_LEISIM_OTHER_DIRECTION_IRAT_STARTED_ABORT_EHRPD_2ND_REVIVE */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_OTHER_DIRECTION_IRAT_STARTED_ABORT_EHRPD_2ND_REVIVE = 0x00141587,
    /* L4BPDN_LEISIM_OTHER_DIRECTION_IRAT_STARTED_ABORT_EHRPD_3ND_REVIVE */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_L4BPDN_LEISIM_OTHER_DIRECTION_IRAT_STARTED_ABORT_EHRPD_3ND_REVIVE = 0x00141588,
    /* PDN module network L4BPDN error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_L4BPDN_CAUSE_END            = 0x001415FF,
    /* PDN module network D2 error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_BEGIN              = 0x00141600,
    /* D2AT_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AT_OK            = 0x00141601,
    /* D2AT_ERROR_EIF_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AT_ERROR_EIF_ERROR = 0x00141602,
    /* D2AT_ERROR_EIF_FORMAT_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AT_ERROR_EIF_FORMAT_ERROR = 0x00141603,
    /* D2AT_ERROR_INVALID_INTERFACE_ID */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AT_ERROR_INVALID_INTERFACE_ID = 0x00141604,
    /* D2AT_ERROR_INVALID_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AT_ERROR_INVALID_STATE = 0x00141605,
    /* D2AT_ERROR_DSCONT_WITHOUT_PRIMARY */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AT_ERROR_DSCONT_WITHOUT_PRIMARY = 0x00141606,
    /* D2_NETIF_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2_NETIF_OK        = 0x00141640,
    /* D2_NETIF_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2_NETIF_ERROR     = 0x00141641,
    /* D2_NETIF_INVALID_INTERFACE_ID */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2_NETIF_INVALID_INTERFACE_ID = 0x00141642,
    /* D2_NETIF_POOL_FULL */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2_NETIF_POOL_FULL = 0x00141643,
    /* D2_NETIF_INVALID_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2_NETIF_INVALID_STATE = 0x00141644,
    /* D2_NETIF_INVALID_PARAMETER */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2_NETIF_INVALID_PARAMETER = 0x00141645,
    /* D2AM_NW_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_NW_CAUSE_START = 0x00141650,
    /* D2AM_IA_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_CAUSE_START = 0x00141660,
    /* D2AM_IA_APN_NOT_SET */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_APN_NOT_SET = 0x00141661,
    /* D2AM_IA_APN_NOT_FOUND */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_APN_NOT_FOUND = 0x00141662,
    /* D2AM_IA_APN_CID_ALLOC_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_APN_CID_ALLOC_FAIL = 0x00141663,
    /* D2AM_IA_TARGET_DETACH */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_TARGET_DETACH = 0x00141664,
    /* D2AM_IA_RF_OFF */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_RF_OFF     = 0x00141665,
    /* D2AM_IA_APN_ON_WIFI */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_APN_ON_WIFI = 0x00141666,
    /* D2AM_IA_ICCID_NOT_GET */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_ICCID_NOT_GET = 0x00141667,
    /* D2AM_IA_IMS_ON_WIFI */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_IMS_ON_WIFI = 0x00141668,
    /* D2AM_IA_UNBIND_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IA_UNBIND_ONGOING = 0x00141669,
    /* D2AM_OTHER_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_OTHER_CAUSE_START = 0x00141670,
    /* D2AM_DATA_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_DATA_NOT_ALLOWED = 0x00141671,
    /* D2AM_IMPORTANT_USER_USING */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IMPORTANT_USER_USING = 0x00141672,
    /* D2AM_IMS_CALLING_ON_OTHER_SIM */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_IMS_CALLING_ON_OTHER_SIM = 0x00141673,
    /* D2AM_DUAL_MDALLOW */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_DUAL_MDALLOW  = 0x00141674,
    /* D2AM_MDALLOW_NOT_SUPPORT */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_MDALLOW_NOT_SUPPORT = 0x00141675,
    /* D2AM_PS_MODE_UNKNOWN */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_PS_MODE_UNKNOWN = 0x00141676,
    /* D2AM_CS_CALLING_ON_OTHER_SIM */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_CS_CALLING_ON_OTHER_SIM = 0x00141677,
    /* D2AM_OP12_DATA_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_OP12_DATA_NOT_ALLOWED = 0x00141678,
    /* D2AM_DSBP_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_DSBP_ONGOING  = 0x00141679,
    /* D2AM_SIM_NOT_READY */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2AM_SIM_NOT_READY = 0x0014167A,
    /* D2CPM_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_OK           = 0x001416D0,
    /* D2CPM_INVAL_ARG */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_INVAL_ARG    = 0x001416D1,
    /* D2CPM_NOT_CONN */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_NOT_CONN     = 0x001416D2,
    /* D2CPM_ALREADY_PROG */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_ALREADY_PROG = 0x001416D3,
    /* D2CPM_CONN_ABORT */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_CONN_ABORT   = 0x001416D4,
    /* D2CPM_NO_ADDR */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_NO_ADDR      = 0x001416D5,
    /* D2CPM_NOT_PERM */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_NOT_PERM     = 0x001416D6,
    /* D2CPM_IN_USE */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_IN_USE       = 0x001416D7,
    /* D2CPM_CONFLICT */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_CONFLICT     = 0x001416D8,
    /* D2CPM_UNRECOV */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_UNRECOV      = 0x001416D9,
    /* D2CPM_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CPM_CAUSE_END    = 0x001416DF,
    /* D2CM_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CM_CAUSE_START   = 0x001416E0,
    /* D2CM_CAUSE_END */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2CM_CAUSE_END     = 0x001416EF,
    /* D2PM_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2PM_CAUSE_START   = 0x001416F0,
    /* D2PM_D2RM_REJ */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2PM_D2RM_REJ      = 0x001416F1,
    /* D2PM_NW_UNKNOWN */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_D2PM_NW_UNKNOWN    = 0x001416F2,
    /* PDN module network D2 error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_D2_CAUSE_END                = 0x001416FF,
    /* PDN module network DDM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_BEGIN             = 0x00141700,
    /* DDM_AT_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_AT_OK         = 0x00141701,
    /* DDM_DEFINE_ATTACH_PDN_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_DEFINE_ATTACH_PDN_FAIL = 0x00141702,
    /* DDM_BLOCK_ACT_DATA_CALL */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_BLOCK_ACT_DATA_CALL = 0x00141703,
    /* DDM_REUSE_PDP_TYPE_NOT_MATCH */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_REUSE_PDP_TYPE_NOT_MATCH = 0x00141704,
    /* DDM_IA_IS_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_IA_IS_ONGOING = 0x00141705,
    /* DDM_ACT_IS_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_ACT_IS_ONGOING = 0x00141706,
    /* DDM_DEACT_IS_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_DDM_DEACT_IS_ONGOING = 0x00141707,
    /* PDN module network DDM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_DDM_CAUSE_END               = 0x001417FF,
    /* PDN module network 5GSM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_BEGIN            = 0x00141A00,
    /* default value, there is no spec */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_NO_CAUSE         = 0x00141A01,
    /* OPERATOR_DETERMINED_BARRING */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_OPERATOR_DETERMINED_BARRING = 0x00141A08,
    /* INSUFFICIENT_RESOURCES */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INSUFFICIENT_RESOURCES = 0x00141A1A,
    /* MISSING_OR_UNKNOWN_DNN */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN = 0x00141A1B,
    /* UNKNOWN_PDU_SESSION_TYPE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_UNKNOWN_PDU_SESSION_TYPE = 0x00141A1C,
    /* USER_AUTH_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_USER_AUTH_FAILED = 0x00141A1D,
    /* REQUEST_REJECTED_BY_SGW_OR_PDNGW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_REQUEST_REJECTED_BY_SGW_OR_PDNGW = 0x00141A1E,
    /* REQUEST_REJECTED_UNSPECIFIED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_REQUEST_REJECTED_UNSPECIFIED = 0x00141A1F,
    /* SERVICE_OPT_NOT_SUPPORTED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SERVICE_OPT_NOT_SUPPORTED = 0x00141A20,
    /* REQ_SERVICE_NOT_SUBSCRIBED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_REQ_SERVICE_NOT_SUBSCRIBED = 0x00141A21,
    /* SERVICE_OPT_TEMP_OUT_OF_ORDER */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SERVICE_OPT_TEMP_OUT_OF_ORDER = 0x00141A22,
    /* PTI_ALREADY_USED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PTI_ALREADY_USED = 0x00141A23,
    /* REGULAR_DEACTIVATION */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_REGULAR_DEACTIVATION = 0x00141A24,
    /* EPS_QOS_NOT_ACCEPTED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_EPS_QOS_NOT_ACCEPTED = 0x00141A25,
    /* NETWORK_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_NETWORK_FAILURE  = 0x00141A26,
    /* REACTIVATION_REQUESTED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_REACTIVATION_REQUESTED = 0x00141A27,
    /* SEMANTIC_ERROR_IN_TFT */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SEMANTIC_ERROR_IN_TFT = 0x00141A29,
    /* SYNTACTIC_ERROR_IN_TFT */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SYNTACTIC_ERROR_IN_TFT = 0x00141A2A,
    /* INVALID_PDU_SESSION_IDENTITY */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INVALID_PDU_SESSION_IDENTITY = 0x00141A2B,
    /* SEMANTIC_ERROR_IN_PACKET_FILTERS */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SEMANTIC_ERROR_IN_PACKET_FILTERS = 0x00141A2C,
    /* SYNTACTIC_ERROR_IN_PACKET_FILTERS */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SYNTACTIC_ERROR_IN_PACKET_FILTERS = 0x00141A2D,
    /* OUT_OF_LADN_SERVICE_AREA */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_OUT_OF_LADN_SERVICE_AREA = 0x00141A2E,
    /* PTI_MISMATCH */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PTI_MISMATCH     = 0x00141A2F,
    /* LAST_PDN_DISC_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_LAST_PDN_DISC_NOT_ALLOWED = 0x00141A31,
    /* PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED = 0x00141A32,
    /* PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED = 0x00141A33,
    /* SINGLE_ADDRESS_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SINGLE_ADDRESS_ONLY_ALLOWED = 0x00141A34,
    /* PDU_SESSION_NOT_EXIST */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PDU_SESSION_NOT_EXIST = 0x00141A36,
    /* MULTIPLE_PDN_APN_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MULTIPLE_PDN_APN_NOT_ALLOWED = 0x00141A37,
    /* COLLISION_WITH_NW_INIT_REQUEST */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_COLLISION_WITH_NW_INIT_REQUEST = 0x00141A38,
    /* PDU_SESSION_TYPE_IPV4V6_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PDU_SESSION_TYPE_IPV4V6_ONLY_ALLOWED = 0x00141A39,
    /* PDU_SESSION_TYPE_UNSTRUCTURED_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PDU_SESSION_TYPE_UNSTRUCTURED_ONLY_ALLOWED = 0x00141A3A,
    /* UNSUPPORTED_QCI_VALUE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_UNSUPPORTED_QCI_VALUE = 0x00141A3B,
    /* PDU_SESSION_TYPE_ETHERNET_ONLY_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PDU_SESSION_TYPE_ETHERNET_ONLY_ALLOWED = 0x00141A3D,
    /* MAXIMUM_NUM_OF_EPS_BEARERS_REACHED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MAXIMUM_NUM_OF_EPS_BEARERS_REACHED = 0x00141A41,
    /* REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION = 0x00141A42,
    /* INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN = 0x00141A43,
    /* NOT_SUPPORTED_SSC_MODE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_NOT_SUPPORTED_SSC_MODE = 0x00141A44,
    /* INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE = 0x00141A45,
    /* MISSING_OR_UNKNOWN_DNN_IN_A_SLICE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN_IN_A_SLICE = 0x00141A46,
    /* INVALID_PTI_VALUE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INVALID_PTI_VALUE = 0x00141A51,
    /* MAX_DATA_RATE_PER_UE_FOR_UPLANE_INTEGRITY_PROTECTION_IS_TOO_LOW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MAX_DATA_RATE_PER_UE_FOR_UPLANE_INTEGRITY_PROTECTION_IS_TOO_LOW = 0x00141A52,
    /* SEMANTIC_ERROR_IN_QOS_OPERATION */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SEMANTIC_ERROR_IN_QOS_OPERATION = 0x00141A53,
    /* SYNTACTIC_ERROR_IN_QOS_OPERATION */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SYNTACTIC_ERROR_IN_QOS_OPERATION = 0x00141A54,
    /* INVALID_MAPPED_EPS_BEARER_IDENTITY */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INVALID_MAPPED_EPS_BEARER_IDENTITY = 0x00141A55,
    /* SEMANTICLLY_INCORRECT_MSG */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_SEMANTICLLY_INCORRECT_MSG = 0x00141A5F,
    /* INVALID_MANDATORY_IE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_INVALID_MANDATORY_IE = 0x00141A60,
    /* MSG_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENT */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MSG_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENT = 0x00141A61,
    /* MSG_TYPE_NOT_COMPATIBLE_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MSG_TYPE_NOT_COMPATIBLE_STATE = 0x00141A62,
    /* IE_NON_EXISTENT_NOT_IMPLEMENTED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_IE_NON_EXISTENT_NOT_IMPLEMENTED = 0x00141A63,
    /* CONDITIONAL_IE_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_CONDITIONAL_IE_ERROR = 0x00141A64,
    /* MSG_NOT_COMPATIBLE_STATE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_MSG_NOT_COMPATIBLE_STATE = 0x00141A65,
    /* PROTOCOL_ERROR_UNSPECIFIED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED = 0x00141A6F,
    /* PROPRIETARY_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_START = 0x00141A9B,
    /* PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_TO_NESTED_SAME_REQ */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_TO_NESTED_SAME_REQ = 0x00141A9C,
    /* PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_TO_INVALID_PSI */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_TO_INVALID_PSI = 0x00141A9D,
    /* PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_VGSM_INIT_MOD_ONGOING */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_VGSM_INIT_MOD_ONGOING = 0x00141A9E,
    /* PROPRIETARY_CAUSE_RESPONDER_PROCESSING_TIME_NOT_ENOUGH */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_RESPONDER_PROCESSING_TIME_NOT_ENOUGH = 0x00141A9F,
    /* PROPRIETARY_CAUSE_RESPONDER_GUARANTEE_RESPONSE_TIMER_TIME_OUT */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_RESPONDER_GUARANTEE_RESPONSE_TIMER_TIME_OUT = 0x00141AA0,
    /* PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_TO_VZ_REQ_5GNRSA_10_2_3 */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_RESPONDER_REJ_REQ_DUE_TO_VZ_REQ_5GNRSA_10_2_3 = 0x00141AA1,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_PS_SUSPENDED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_PS_SUSPENDED = 0x00141AAA,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_ERROR_OCCURED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_ERROR_OCCURED = 0x00141AAB,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_HURRY_UP_REQ */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_HURRY_UP_REQ = 0x00141AAC,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_UAC_BARRING */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_UAC_BARRING = 0x00141AAD,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_FATAL_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_FATAL_FAIL = 0x00141AAE,
    /* PROPRIETARY_CAUSE_CONN_ABORT_PROC_DUE_TO_POSSIBLE_LONG_GEMINI_SUSPEND */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_ABORT_PROC_DUE_TO_POSSIBLE_LONG_GEMINI_SUSPEND = 0x00141AAF,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_TRANSMISSION_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_TRANSMISSION_FAILURE = 0x00141AB0,
    /* PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_S_NSSAI_REJECTED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CONN_REJ_REQ_DUE_TO_S_NSSAI_REJECTED = 0x00141AB1,
    /* PROPRIETARY_CAUSE_CORE_SYSTEM_STATE_CHECK_FAIL */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_SYSTEM_STATE_CHECK_FAIL = 0x00141AB4,
    /* PROPRIETARY_CAUSE_CORE_REQ_INFO_ERROR */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_REQ_INFO_ERROR = 0x00141AB5,
    /* PROPRIETARY_CAUSE_CORE_TX_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_TX_FAILURE = 0x00141AB6,
    /* PROPRIETARY_CAUSE_CORE_TIMER_TIMEOUT */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_TIMER_TIMEOUT = 0x00141AB7,
    /* PROPRIETARY_CAUSE_CORE_LOCAL_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_LOCAL_RELEASE = 0x00141AB8,
    /* PROPRIETARY_CAUSE_CORE_CANCLE_BY_RELEASE_FROM_UPPER_LAYER */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_CANCLE_BY_RELEASE_FROM_UPPER_LAYER = 0x00141AB9,
    /* PROPRIETARY_CAUSE_CORE_NW_RELEASE_ABORT_ORIGINAL_PROCEDURE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_NW_RELEASE_ABORT_ORIGINAL_PROCEDURE = 0x00141ABA,
    /* PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_ROUTING_FAILURE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_ROUTING_FAILURE = 0x00141ABB,
    /* PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_DNN_NOT_SUPPORT_IN_SLICE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_DNN_NOT_SUPPORT_IN_SLICE = 0x00141ABC,
    /* PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_PLMN_MAX_NUM_PDU_SESSION_REACH */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_PLMN_MAX_NUM_PDU_SESSION_REACH = 0x00141ABD,
    /* PROPRIETARY_CAUSE_CORE_MT_MOD_ABORT_ORIGINAL_PROCEDURE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MT_MOD_ABORT_ORIGINAL_PROCEDURE = 0x00141ABE,
    /* PROPRIETARY_CAUSE_CORE_MT_MOD_ONGOING_MO_MOD_NOT_ALLOW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MT_MOD_ONGOING_MO_MOD_NOT_ALLOW = 0x00141ABF,
    /* PROPRIETARY_CAUSE_CORE_SYNC_PDUS_STATUS_WITH_NW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_SYNC_PDUS_STATUS_WITH_NW = 0x00141AC0,
    /* PROPRIETARY_CAUSE_CORE_UE_DEREGISTER_FROM_NW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_UE_DEREGISTER_FROM_NW = 0x00141AC1,
    /* PROPRIETARY_CAUSE_CORE_5G23_DO_LOCAL_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_5G23_DO_LOCAL_RELEASE = 0x00141AC2,
    /* PROPRIETARY_CAUSE_CORE_THIS_PDU_WAS_REVIVED_IN_4G */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_THIS_PDU_WAS_REVIVED_IN_4G = 0x00141AC3,
    /* PROPRIETARY_CAUSE_CORE_INTERSYSTEM_CHANGE_ABORT */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_INTERSYSTEM_CHANGE_ABORT = 0x00141AC4,
    /* PROPRIETARY_CAUSE_CORE_PDU_SESSION_ALREADY_DEACTIVATE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_PDU_SESSION_ALREADY_DEACTIVATE = 0x00141AC5,
    /* PROPRIETARY_CAUSE_CORE_5G4_MAP_FAILED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_5G4_MAP_FAILED = 0x00141AC6,
    /* PROPRIETARY_CAUSE_CORE_TRIGGERED_BY_RQOS */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_TRIGGERED_BY_RQOS = 0x00141AC7,
    /* PROPRIETARY_CAUSE_CORE_THIS_PDUS_CANNOT_TO_4G */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_THIS_PDUS_CANNOT_TO_4G = 0x00141AC8,
    /* PROPRIETARY_CAUSE_CORE_SYSTEM_STATE_UNSYNC_WITH_UPPER_LAYER */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_SYSTEM_STATE_UNSYNC_WITH_UPPER_LAYER = 0x00141AC9,
    /* PROPRIETARY_CAUSE_CORE_UPPER_LAYER_REQ_FORCE_TO_LOCAL_REL_PDUS */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_UPPER_LAYER_REQ_FORCE_TO_LOCAL_REL_PDUS = 0x00141ACA,
    /* PROPRIETARY_CAUSE_CORE_SYNC_EPSB_STATUS_WITH_NW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_SYNC_EPSB_STATUS_WITH_NW = 0x00141ACB,
    /* PROPRIETARY_CAUSE_CORE_PAM_REJECT_REQ */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_PAM_REJECT_REQ = 0x00141ACC,
    /* PROPRIETARY_CAUSE_CORE_SECOND_EMC_PDUS_NOT_ALLOW */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_SECOND_EMC_PDUS_NOT_ALLOW = 0x00141ACD,
    /* PROPRIETARY_CAUSE_CORE_EST_NON_EMC_PDUS_NOT_ALLOW_WHEN_ENTER_EMC_REGISTERED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_EST_NON_EMC_PDUS_NOT_ALLOW_WHEN_ENTER_EMC_REGISTERED = 0x00141ACE,
    /* PROPRIETARY_CAUSE_CORE_THIS_MOD_REQ_ON_EMC_PDUS_IS_NOT_ALLOWED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_THIS_MOD_REQ_ON_EMC_PDUS_IS_NOT_ALLOWED = 0x00141ACF,
    /* PROPRIETARY_CAUSE_CORE_SNSSAI_NOT_IN_NSSAI */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_SNSSAI_NOT_IN_NSSAI = 0x00141AD0,
    /* PROPRIETARY_CAUSE_CORE_DUE_TO_DETACH_WITH_REATTACH_REQUIRED */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_DUE_TO_DETACH_WITH_REATTACH_REQUIRED = 0x00141AD1,
    /* PROPRIETARY_CAUSE_CORE_MSG_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENT_INDICATED_BY_5GSM_STATUS */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MSG_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENT_INDICATED_BY_5GSM_STATUS = 0x00141AD2,
    /* PROPRIETARY_CAUSE_CORE_TRIGGERED_BY_SNSSAI_UPDATE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_TRIGGERED_BY_SNSSAI_UPDATE = 0x00141AD3,
    /* PROPRIETARY_CAUSE_CORE_VGMM_ENTER_EMC_TO_LOCAL_RELEASE */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_VGMM_ENTER_EMC_TO_LOCAL_RELEASE = 0x00141AD4,
    /* PROPRIETARY_CAUSE_CORE_PROCESS_NW_MOD_COMMAND_OK */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_PROCESS_NW_MOD_COMMAND_OK = 0x00141AD5,
    /* PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_RESTRICTED_SERVICE_AREA */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_PROPRIETARY_CAUSE_CORE_MSG_NOT_FORWARD_RESTRICTED_SERVICE_AREA = 0x00141AD6,
    /* PDN module network 5GSM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_5GSM_CAUSE_END              = 0x00141AFF,
    /* PDN module network EXT_PAM error cause begin */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_BEGIN         = 0x00141B00,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_8 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_8 = 0x00141B08,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_26 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_26 = 0x00141B1A,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_27 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_27 = 0x00141B1B,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_28 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_28 = 0x00141B1C,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_29 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_29 = 0x00141B1D,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_30 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_30 = 0x00141B1E,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_31 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_31 = 0x00141B1F,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_32 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_32 = 0x00141B20,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_33 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_33 = 0x00141B21,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_34 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_34 = 0x00141B22,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_35 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_35 = 0x00141B23,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_36 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_36 = 0x00141B24,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_37 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_37 = 0x00141B25,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_38 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_38 = 0x00141B26,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_39 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_39 = 0x00141B27,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_41 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_41 = 0x00141B29,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_42 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_42 = 0x00141B2A,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_43 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_43 = 0x00141B2B,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_44 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_44 = 0x00141B2C,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_45 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_45 = 0x00141B2D,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_46 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_46 = 0x00141B2E,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_47 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_47 = 0x00141B2F,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_49 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_49 = 0x00141B31,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_50 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_50 = 0x00141B32,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_51 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_51 = 0x00141B33,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_54 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_54 = 0x00141B36,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_55 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_55 = 0x00141B37,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_56 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_56 = 0x00141B38,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_57 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_57 = 0x00141B39,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_59 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_59 = 0x00141B3B,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_58 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_58 = 0x00141B3A,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_61 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_61 = 0x00141B3D,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_65 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_65 = 0x00141B41,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_66 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_66 = 0x00141B42,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_67 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_67 = 0x00141B43,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_68 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_68 = 0x00141B44,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_69 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_69 = 0x00141B45,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_70 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_70 = 0x00141B46,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_81 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_81 = 0x00141B51,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_82 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_82 = 0x00141B52,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_83 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_83 = 0x00141B53,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_84 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_84 = 0x00141B54,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_85 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_85 = 0x00141B55,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_95 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_95 = 0x00141B5F,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_96 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_96 = 0x00141B60,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_97 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_97 = 0x00141B61,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_98 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_98 = 0x00141B62,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_99 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_99 = 0x00141B63,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_100 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_100 = 0x00141B64,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_101 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_101 = 0x00141B65,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_111 */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_CAUSE_111 = 0x00141B6F,
    /* PAM_5G_PDU_SESSION_NOT_FORWARDED_CAUSE_START */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_NOT_FORWARDED_CAUSE_START = 0x00141B80,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_DNN_NOT_SUPPORT_IN_SLICE */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_DNN_NOT_SUPPORT_IN_SLICE = 0x00141B81,
    /* PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_MAX_PDU_SESSION_REACHED */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_5G_PDU_SESSION_ACCESS_REJECT_DUE_TO_MAX_PDU_SESSION_REACHED = 0x00141B82,
    /* PAM_VGSM_REJECT_DUE_TO_LADN_OUT_OF_SERVICE_AREA */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_VGSM_REJECT_DUE_TO_LADN_OUT_OF_SERVICE_AREA = 0x00141B83,
    /* PAM_VGSM_REJECT_DUE_TO_LADN_TABLE_VERSION_NOT_UNSYNC */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_PAM_VGSM_REJECT_DUE_TO_LADN_TABLE_VERSION_NOT_UNSYNC = 0x00141B84,
    /* PDN module network EXT_PAM error cause end */
    MIPC_RESULT_PDN_EXT_NETWORK_EXT_PAM_CAUSE_END           = 0x00141BFF,
    /* PDN module network error end */
    MIPC_RESULT_PDN_EXT_NETWORK_ERROR_END                   = 0x0014FFFF,
    /* PDN mipc messages error begin */
    MIPC_RESULT_PDN_EXT_LOCAL_MIPC_ERROR_BEGIN              = 0x00150000,
    /* There is no enough information in MIPC message */
    MIPC_RESULT_PDN_EXT_LOCAL_MIPC_ERROR_NO_ENOUGH_INFO     = 0x00150001,
    /* PDN mipc messages error end */
    MIPC_RESULT_PDN_EXT_LOCAL_MIPC_ERROR_END                = 0x001500FF,
    /* PDN module parameters error begin */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_BEGIN              = 0x00150100,
    /* parameters for PDN is invalid */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_INVALID_PARA       = 0x00150101,
    /* APN is not found to activate */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_NOT_FIND_APN_TO_ACT = 0x00150102,
    /* APN is already activated or no apn info to abort */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_NO_APN_INFO_TO_ABORT = 0x00150103,
    /* APN is disbaled by host */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_APN_PROFILE_IS_DISABLED = 0x00150104,
    /* APN call info is not found */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_NOT_FIND_CALL_INFO = 0x00150105,
    /* max user reached */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_REACH_MAX_USER     = 0x00150106,
    /* APN profile count is wrong */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_PROFILE_COUNT_ERROR = 0x00150107,
    /* APN type or APN is wrong */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_WRONG_APN_TYPE_OR_NO_APN = 0x00150108,
    /* Get URSP plmn list error */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_URSP_PLMN_LIST_ERROR = 0x00150109,
    /* PDN module parameters error end */
    MIPC_RESULT_PDN_EXT_LOCAL_PARA_ERROR_END                = 0x001501FF,
    /* PDN IP FALLBACK RESPONSE start */
    MIPC_RESULT_PDN_EXT_LOCAL_FALLBACK_RES_START            = 0x00150200,
    /* PDN IP FALLBACK RESPONSE fail abort */
    MIPC_RESULT_PDN_EXT_LOCAL_FALLBACK_RES_FAIL_ABORT       = 0x00150201,
    /* PDN IP FALLBACK RESPONSE success abort */
    MIPC_RESULT_PDN_EXT_LOCAL_FALLBACK_RES_SUCC_ABORT       = 0x00150202,
    /* PDN IP FALLBACK RESPONSE fail continue */
    MIPC_RESULT_PDN_EXT_LOCAL_FALLBACK_RES_FAIL_CONT        = 0x00150203,
    /* PDN IP FALLBACK RESPONSE success continue */
    MIPC_RESULT_PDN_EXT_LOCAL_FALLBACK_RES_SUCC_CONT        = 0x00150204,
    /* PDN IP FALLBACK RESPONSE end */
    MIPC_RESULT_PDN_EXT_LOCAL_FALLBACK_RES_END              = 0x0015020F,
    /* PDN module extension result end */
    MIPC_RESULT_PDN_EXT_END                                 = 0x0017FFFF,
    /* RF module extension result begin */
    MIPC_RESULT_RF_EXT_BEGIN                                = 0x00180000,
    /* RF module extension result end */
    MIPC_RESULT_RF_EXT_END                                  = 0x001BFFFF,
    /* SIM module extension result begin */
    MIPC_RESULT_SIM_EXT_BEGIN                               = 0x001C0000,
    /* MT is waiting  PUK1 code */
    MIPC_RESULT_SIM_EXT_PUK1_REQUIRED                       = 0x001C0001,
    /* MT is waiting  PIN2 code */
    MIPC_RESULT_SIM_EXT_PIN2_REQUIRED                       = 0x001C0002,
    /* MT is waiting  PUK2 code */
    MIPC_RESULT_SIM_EXT_PUK2_REQUIRED                       = 0x001C0003,
    /* SIM failure  */
    MIPC_RESULT_SIM_EXT_SIM_FAILURE                         = 0x001C0004,
    /* Incoorect password returned from UICC  */
    MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD                 = 0x001C0005,
    /* Network personalization PIN required.Related 3GPP SPEC:22.022.  */
    MIPC_RESULT_SIM_EXT_NW_PERSON_PIN_REQUIRED              = 0x001C0006,
    /* Network personalization PUK required.  */
    MIPC_RESULT_SIM_EXT_NW_PERSON_PUK_REQUIRED              = 0x001C0007,
    /* Network subset personalization PIN required. Related 3GPP SPEC:22.022.  */
    MIPC_RESULT_SIM_EXT_NW_SUB_PERSON_PIN_REQUIRED          = 0x001C0008,
    /* Network subset personalization PUK required.  */
    MIPC_RESULT_SIM_EXT_NW_SUB_PERSON_PUK_REQUIRED          = 0x001C0009,
    /* Service Provider personalization PIN required. Related 3GPP SPEC:22.022. */
    MIPC_RESULT_SIM_EXT_SP_PERSON_PIN_REQUIRED              = 0x001C000A,
    /* Service Provider personalization PUK required. */
    MIPC_RESULT_SIM_EXT_SP_PERSON_PUK_REQUIRED              = 0x001C000B,
    /* Corporate personalization PIN required.Related 3GPP SPEC:22.022. */
    MIPC_RESULT_SIM_EXT_CORP_PERSON_PIN_REQUIRED            = 0x001C000C,
    /* Corporate personalization PUK required. */
    MIPC_RESULT_SIM_EXT_CORP_PERSON_PUK_REQUIRED            = 0x001C000D,
    /* IMSI personalization PIN required.Related 3GPP SPEC:22.022. */
    MIPC_RESULT_SIM_EXT_IMSI_PERSON_PIN_REQUIRED            = 0x001C000E,
    /* IMSI personalization PUK required. */
    MIPC_RESULT_SIM_EXT_IMSI_PERSON_PUK_REQUIRED            = 0x001C000F,
    /* Link network and service provider personalization pin required. */
    MIPC_RESULT_SIM_EXT_LINK_NS_SP_PERSON_PIN_REQUIRED      = 0x001C0010,
    /* Link network and service provider personalization puk required. */
    MIPC_RESULT_SIM_EXT_LINK_NS_SP_PERSON_PUK_REQUIRED      = 0x001C0011,
    /* Link SIM and corporate personalization pin required. */
    MIPC_RESULT_SIM_EXT_LINK_SIM_C_PERSON_PIN_REQUIRED      = 0x001C0012,
    /* Link SIM and corporate personalization puk required. */
    MIPC_RESULT_SIM_EXT_LINK_SIM_C_PERSON_PUK_REQUIRED      = 0x001C0013,
    /* BT SAP related error. */
    MIPC_RESULT_SIM_EXT_BT_SAP_UNDEFINED                    = 0x001C0014,
    /* BT SAP not accessible. */
    MIPC_RESULT_SIM_EXT_BT_SAP_NOT_ACCESSIBLE               = 0x001C0015,
    /* BT SAP error card removed. */
    MIPC_RESULT_SIM_EXT_BT_SAP_CARD_REMOVED                 = 0x001C0016,
    /* BT SAP error card already powered off. */
    MIPC_RESULT_SIM_EXT_BT_SAP_CARD_ALREADY_POWERED_OFF     = 0x001C0017,
    /* BT SAP error card already powered on. */
    MIPC_RESULT_SIM_EXT_BT_SAP_CARD_ALREADY_POWERED_ON      = 0x001C0018,
    /* Operation failed because of sim technical problem. */
    MIPC_RESULT_SIM_EXT_TECHNICAL_PROBLEM                   = 0x001C0019,
    /* TMO rsu verifycation result */
    MIPC_RESULT_SIM_EXT_VERIFYCATION_FAILED                 = 0x001C001A,
    /* TMO rsu verifycation result */
    MIPC_RESULT_SIM_EXT_REBOOT_REQUIRED                     = 0x001C001B,
    /* sim state is off */
    MIPC_RESULT_SIM_EXT_SIM_OFF                             = 0x001C001C,
    /* SIM module extension result end */
    MIPC_RESULT_SIM_EXT_END                                 = 0x001FFFFF,
    /* SMS module extension result begin */
    MIPC_RESULT_SMS_EXT_BEGIN                               = 0x00200000,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_WRONG_MSG_ID_RANGE_FORMAT   = 0x00200001,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_WRONG_MSG_ID_SINGLE_FORMAT  = 0x00200002,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_WRONG_DCS_RANGE_FORMAT      = 0x00200003,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_WRONG_DCS_SINGLE_FORMAT     = 0x00200004,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_TOO_MORE_RANGES_OR_SINGLES  = 0x00200005,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_MSG_ID_LOGIC_ERROR          = 0x00200006,
    /*  */
    MIPC_RESULT_SMS_EXT_CBM_CFG_DCS_LOGIC_ERROR             = 0x00200007,
    /* Something wrong with storage parameter, like unsync/uninit... */
    MIPC_RESULT_SMS_EXT_NEED_SET_PREFER_STORAGE_AGAIN       = 0x00200008,
    /* Something wrong with c2k operation */
    MIPC_RESULT_SMS_EXT_C2K_ERROR                           = 0x00200009,
    /* bellow should be old AT error, could use offset to judge AT error cause */
    MIPC_RESULT_SMS_EXT_AT_ERROR_BEGIN                      = 0x00210000,
    /* at error cause 226 would transfer this */
    MIPC_RESULT_SMS_EXT_CM_SMS_CONNECTION_BROKEN            = 0x002100E2,
    /* at error cause 2601 would transfer this */
    MIPC_RESULT_SMS_EXT_MTK_FDN_CHECK_FAILURE               = 0x00210A29,
    /* at error cause 6145 would transfer this */
    MIPC_RESULT_SMS_EXT_MTK_REQ_RETRY                       = 0x00211801,
    /* upper should be old AT error, could use offset to judge AT error cause */
    MIPC_RESULT_SMS_EXT_AT_ERROR_END                        = 0x0021FFFF,
    /* SMS module extension result end */
    MIPC_RESULT_SMS_EXT_END                                 = 0x0023FFFF,
    /* SS module extension result begin */
    MIPC_RESULT_SS_EXT_BEGIN                                = 0x00240000,
    /* cat module extension result end */
    MIPC_RESULT_SS_EXT_END                                  = 0x0027FFFF,
    /* SWITCH module extension result begin */
    MIPC_RESULT_SWITCH_EXT_BEGIN                            = 0x00280000,
    /* SWITCH module extension result end */
    MIPC_RESULT_SWITCH_EXT_END                              = 0x002BFFFF,
    /* SYS module extension result begin */
    MIPC_RESULT_SYS_EXT_BEGIN                               = 0x002C0000,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_PROCESS_OK                      = 0x002C0001,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_NUM_ALREADY_MAX                 = 0x002C0002,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_PROXY_ALREADY_EXIST             = 0x002C0003,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_INTERNAL_ABNORMAL               = 0x002C0004,
    /* when rcv resume, pause, stop */
    MIPC_RESULT_SYS_EXT_HBA_PROXY_NOT_EXIST                 = 0x002C0005,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_HW_FILTER_EXHAUST               = 0x002C0006,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_PASUE_ERR                       = 0x002C0007,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_RESUME_ERR                      = 0x002C0008,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_STOP_ERR                        = 0x002C0009,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_SEDN_TIME_OUT                   = 0x002C000A,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_ACK_TIME_OUT                    = 0x002C000B,
    /*  */
    MIPC_RESULT_SYS_EXT_HBA_RESULT_INVALID                  = 0x002C00FF,
    /*  */
    MIPC_RESULT_SYS_EXT_NVRAM_READ_DISABLE                  = 0x002C0100,
    /*  */
    MIPC_RESULT_SYS_EXT_NVRAM_WRITE_DISABLE                 = 0x002C0101,
    /* SYS module extension result end */
    MIPC_RESULT_SYS_EXT_END                                 = 0x002FFFFF,
    /* IMS module extension result begin */
    MIPC_RESULT_IMS_EXT_BEGIN                               = 0x00300000,
    /* IMS module extension result end */
    MIPC_RESULT_IMS_EXT_END                                 = 0x0033FFFF,
    /* PHB module extension result begin */
    MIPC_RESULT_PHB_EXT_BEGIN                               = 0x00340000,
    /* PHB module PIN2 is required */
    MIPC_RESULT_PHB_EXT_PIN2_REQUIRED                       = 0x00340001,
    /* PHB module PUK2 is required */
    MIPC_RESULT_PHB_EXT_PUK2_REQUIRED                       = 0x00340002,
    /* PHB module text string is too long */
    MIPC_RESULT_PHB_EXT_TEXT_STRING_TOO_LONG                = 0x00340003,
    /* PHB module dial string is too long */
    MIPC_RESULT_PHB_EXT_DIAL_STRING_TOO_LONG                = 0x00340004,
    /* PHB module extension result end */
    MIPC_RESULT_PHB_EXT_END                                 = 0x0034FFFF,
    /*  */
    MIPC_RESULT_AUTH_REQUIRED                               = 0xFFFFFFFE,
    /*  */
    MIPC_RESULT_TIMEOUT                                     = 0xFFFFFFFF,
};
typedef enum mipc_result_const_enum mipc_result_const_enum;

enum mipc_exims_action_type_const_enum {
    mipc_exims_action_type_const_NONE = 0,
    /* EXIMS_ACTION_TYPE */
    MIPC_EXIMS_ACTION_TYPE_CALL                             = 0,
    MIPC_EXIMS_ACTION_TYPE_SMS                              = 1,
    /* emergency call */
    MIPC_EXIMS_ACTION_TYPE_ECALL                            = 2,
    /* emergency SMS */
    MIPC_EXIMS_ACTION_TYPE_ESMS                             = 3,
    /* emergency call callback mode */
    MIPC_EXIMS_ACTION_TYPE_ECBM                             = 4,
    /* emergency SMS callback mode */
    MIPC_EXIMS_ACTION_TYPE_SCBM                             = 5,
};
typedef enum mipc_exims_action_type_const_enum mipc_exims_action_type_const_enum;

enum mipc_exims_session_status_const_enum {
    mipc_exims_session_status_const_NONE = 0,
    /* EXIMS_SESSION_STATUS */
    MIPC_EXIMS_SESSION_STATUS_END                           = 0,
    MIPC_EXIMS_SESSION_STATUS_START                         = 1,
};
typedef enum mipc_exims_session_status_const_enum mipc_exims_session_status_const_enum;

enum mipc_exims_ho_status_const_enum {
    mipc_exims_ho_status_const_NONE = 0,
    /* EXIMS_HO_STATUS */
    MIPC_EXIMS_HO_STATUS_END                                = 0,
    MIPC_EXIMS_HO_STATUS_START                              = 1,
};
typedef enum mipc_exims_ho_status_const_enum mipc_exims_ho_status_const_enum;

enum mipc_exims_call_type_const_enum {
    mipc_exims_call_type_const_NONE = 0,
    /* EXIMS_CALL_TYPE */
    MIPC_EXIMS_CALL_TYPE_VOICE                              = 0,
    MIPC_EXIMS_CALL_TYPE_VIDEO                              = 1,
    MIPC_EXIMS_CALL_TYPE_EMERGENCY                          = 2,
};
typedef enum mipc_exims_call_type_const_enum mipc_exims_call_type_const_enum;

enum mipc_exims_call_status_const_enum {
    mipc_exims_call_status_const_NONE = 0,
    /* EXIMS_CALL_STATUS */
    MIPC_EXIMS_CALL_STATUS_INVITE                           = 0,
    MIPC_EXIMS_CALL_STATUS_PROGRESS                         = 1,
    MIPC_EXIMS_CALL_STATUS_ALERTING                         = 2,
    MIPC_EXIMS_CALL_STATUS_CONNECTED                        = 3,
    MIPC_EXIMS_CALL_STATUS_DISCONNECTED                     = 4,
    MIPC_EXIMS_CALL_STATUS_FAILURE                          = 5,
};
typedef enum mipc_exims_call_status_const_enum mipc_exims_call_status_const_enum;

enum mipc_exims_call_failure_cause_const_enum {
    mipc_exims_call_failure_cause_const_NONE = 0,
    /* EXIMS_CALL_FAILURE_CAUSE */
    /* Illegal Operation; Modem will return RAT_NONE in the next domain selection request. */
    MIPC_EXIMS_CALL_FAILURE_CAUSE_ILLEGAL_OPERATION         = 0x0,
    /* IMS core failure (e.g. 403 Forbidden, 501 Not implemented...) */
    MIPC_EXIMS_CALL_FAILURE_CAUSE_IMS_CORE_FAILURE          = 0x1,
    /* Temporarily Error (e.g. OOS, RLF...) */
    MIPC_EXIMS_CALL_FAILURE_CAUSE_TEMPORARILY_ERROR         = 0x2,
    /* Network error (e.g. PDU rejecte, SR rejected, Attach failed...) */
    MIPC_EXIMS_CALL_FAILURE_CAUSE_NETWORK_ERROR             = 0x3,
    /* Unspecified */
    MIPC_EXIMS_CALL_FAILURE_CAUSE_UNSPECIFIED               = 0xFF,
};
typedef enum mipc_exims_call_failure_cause_const_enum mipc_exims_call_failure_cause_const_enum;

enum mipc_exims_sms_domain_const_enum {
    mipc_exims_sms_domain_const_NONE = 0,
    /* EXIMS_SMS_DOMAIN */
    MIPC_EXIMS_SMS_DOMAIN_NAS                               = 0,
    MIPC_EXIMS_SMS_DOMAIN_IMS                               = 1,
};
typedef enum mipc_exims_sms_domain_const_enum mipc_exims_sms_domain_const_enum;

enum mipc_exims_sms_status_const_enum {
    mipc_exims_sms_status_const_NONE = 0,
    /* EXIMS_SMS_STATUS */
    MIPC_EXIMS_SMS_STATUS_END                               = 0,
    MIPC_EXIMS_SMS_STATUS_START                             = 1,
    MIPC_EXIMS_SMS_STATUS_FAIL                              = 2,
};
typedef enum mipc_exims_sms_status_const_enum mipc_exims_sms_status_const_enum;

enum mipc_exims_reg_state_const_enum {
    mipc_exims_reg_state_const_NONE = 0,
    /* EXIMS_REG_STATE */
    MIPC_EXIMS_REG_STATE_NOT_REGISTERED                     = 0,
    MIPC_EXIMS_REG_STATE_REGISTERED                         = 1,
    MIPC_EXIMS_REG_STATE_REGISTRATION_START                 = 2,
    MIPC_EXIMS_REG_STATE_DEREGISTRATION_START               = 3,
};
typedef enum mipc_exims_reg_state_const_enum mipc_exims_reg_state_const_enum;

enum mipc_exims_ext_info_const_enum {
    mipc_exims_ext_info_const_NONE = 0,
    /* EXIMS_EXT_INFO */
    MIPC_EXIMS_EXT_INFO_RTP_VOICE                           = 0x0001,
    MIPC_EXIMS_EXT_INFO_RTP_TEXT                            = 0x0002,
    MIPC_EXIMS_EXT_INFO_SMS_IMS                             = 0x0004,
    MIPC_EXIMS_EXT_INFO_RTP_VIDEO                           = 0x0008,
};
typedef enum mipc_exims_ext_info_const_enum mipc_exims_ext_info_const_enum;

enum mipc_exims_dereg_cause_const_enum {
    mipc_exims_dereg_cause_const_NONE = 0,
    /* EXIMS_DEREG_CAUSE */
    MIPC_EXIMS_DEREG_CAUSE_UNSPECIFIED                      = 0,
    MIPC_EXIMS_DEREG_CAUSE_POWER_OFF                        = 1,
    MIPC_EXIMS_DEREG_CAUSE_RF_OFF                           = 2,
    MIPC_EXIMS_DEREG_CAUSE_SIM_NOT_READY                    = 7,
};
typedef enum mipc_exims_dereg_cause_const_enum mipc_exims_dereg_cause_const_enum;

enum mipc_exims_retry_type_const_enum {
    mipc_exims_retry_type_const_NONE = 0,
    /* EXIMS_RETRY_TYPE */
    MIPC_EXIMS_RETRY_TYPE_NO_RETRY                          = 0,
    MIPC_EXIMS_RETRY_TYPE_RETRY                             = 1,
};
typedef enum mipc_exims_retry_type_const_enum mipc_exims_retry_type_const_enum;

enum mipc_exims_uri_type_const_enum {
    mipc_exims_uri_type_const_NONE = 0,
    /* EXIMS_URI_TYPE */
    MIPC_EXIMS_URI_TYPE_MSISMDN                             = 0,
    MIPC_EXIMS_URI_TYPE_IMSI                                = 1,
};
typedef enum mipc_exims_uri_type_const_enum mipc_exims_uri_type_const_enum;

enum mipc_exims_config_type_const_enum {
    mipc_exims_config_type_const_NONE = 0,
    /* EXIMS_CONFIG_TYPE */
    MIPC_EXIMS_CONFIG_TYPE_DISABLE                          = 0,
    MIPC_EXIMS_CONFIG_TYPE_LTE_ENABLE                       = 1,
    MIPC_EXIMS_CONFIG_TYPE_LTE_NR_ENABLE                    = 3,
};
typedef enum mipc_exims_config_type_const_enum mipc_exims_config_type_const_enum;

enum mipc_exims_reg_type_const_enum {
    mipc_exims_reg_type_const_NONE = 0,
    /* EXIMS_REG_TYPE */
    MIPC_EXIMS_REG_TYPE_NORMAL                              = 0,
    MIPC_EXIMS_REG_TYPE_EMERGENCY                           = 1,
};
typedef enum mipc_exims_reg_type_const_enum mipc_exims_reg_type_const_enum;

enum mipc_exims_emergency_support_const_enum {
    mipc_exims_emergency_support_const_NONE = 0,
    /* EXIMS_EMERGENCY_SUPPORT */
    /* Emergency service is not supported in 5G. */
    MIPC_EXIMS_EMERGENCY_SUPPORT_NOT_SUPPORT                = 0,
    /* Emergency service is supported in NR connected to 5GC only. */
    MIPC_EXIMS_EMERGENCY_SUPPORT_NR_5GCN_ONLY               = 1,
    /* Emergency service is supported in E-UTRA connected to 5GC only. */
    MIPC_EXIMS_EMERGENCY_SUPPORT_EUTRA_5GCN_ONLY            = 2,
    /* Emergency service is supported in NR and E-UTRA connected to 5GC. */
    MIPC_EXIMS_EMERGENCY_SUPPORT_NR_5GVN__EUTRA_5GCN        = 3,
};
typedef enum mipc_exims_emergency_support_const_enum mipc_exims_emergency_support_const_enum;

enum mipc_exims_rat_const_enum {
    mipc_exims_rat_const_NONE = 0,
    /* EXIMS_RAT */
    MIPC_EXIMS_RAT_NONE                                     = 0x0000,
    MIPC_EXIMS_RAT_LTE                                      = 0x0004,
    MIPC_EXIMS_RAT_NR                                       = 0x0008,
    MIPC_EXIMS_RAT_WIFI                                     = 0x0020,
};
typedef enum mipc_exims_rat_const_enum mipc_exims_rat_const_enum;

enum mipc_exims_call_domain_cause_const_enum {
    mipc_exims_call_domain_cause_const_NONE = 0,
    /* EXIMS_CALL_DOMAIN_CAUSE */
    /* Indicating OOS, registration reject...; Used in normal call only */
    MIPC_EXIMS_CALL_DOMAIN_CAUSE_NOT_REGISTERED             = 0x1,
    /* Returned when modem can't find an available network during search time period */
    MIPC_EXIMS_CALL_DOMAIN_CAUSE_NO_AVAILABLE_NW            = 0x2,
    /* Returned when the camped network is barred (w/o more service scan) */
    MIPC_EXIMS_CALL_DOMAIN_CAUSE_BARRING_IN_CURRENT_NW      = 0x3,
    /* Returned when all attempt count has been exhausted and no more search is allowed */
    MIPC_EXIMS_CALL_DOMAIN_CAUSE_EXHAUST_ATTEMPT_COUNT      = 0x4,
    /* Returned when user RAT setting is not maximum but current RAT can't make emergency call; Used in emergency call only */
    MIPC_EXIMS_CALL_DOMAIN_CAUSE_GLOBAL_RAT_MODE_REQUIRED   = 0x5,
    MIPC_EXIMS_CALL_DOMAIN_CAUSE_UNSPECIFIED                = 0xFF,
};
typedef enum mipc_exims_call_domain_cause_const_enum mipc_exims_call_domain_cause_const_enum;

enum mipc_exims_call_bar_const_enum {
    mipc_exims_call_bar_const_NONE = 0,
    /* EXIMS_CALL_BAR */
    MIPC_EXIMS_CALL_BAR_SSAC_VOICE                          = 0x0001,
    MIPC_EXIMS_CALL_BAR_SSAC_VIDEO                          = 0x0002,
    MIPC_EXIMS_CALL_BAR_UAC_VOICE                           = 0x0004,
    MIPC_EXIMS_CALL_BAR_UAC_VIDEO                           = 0x0008,
    MIPC_EXIMS_CALL_BAR_UAC_EMERGENCY_CALL                  = 0x0010,
};
typedef enum mipc_exims_call_bar_const_enum mipc_exims_call_bar_const_enum;

enum mipc_exims_sms_bar_const_enum {
    mipc_exims_sms_bar_const_NONE = 0,
    /* EXIMS_SMS_BAR */
    MIPC_EXIMS_SMS_BAR_UAC_SMS                              = 0x0001,
};
typedef enum mipc_exims_sms_bar_const_enum mipc_exims_sms_bar_const_enum;

enum mipc_exims_reg_bar_const_enum {
    mipc_exims_reg_bar_const_NONE = 0,
    /* EXIMS_REG_BAR */
    MIPC_EXIMS_REG_BAR_UAC_REG                              = 0x0001,
};
typedef enum mipc_exims_reg_bar_const_enum mipc_exims_reg_bar_const_enum;

enum mipc_sms_direction_const_enum {
    mipc_sms_direction_const_NONE = 0,
    /* SMS_DIRECTION */
    MIPC_SMS_DIRECTION_MO                                   = 0,
    MIPC_SMS_DIRECTION_MT                                   = 1,
};
typedef enum mipc_sms_direction_const_enum mipc_sms_direction_const_enum;

enum mipc_sms_type_const_enum {
    mipc_sms_type_const_NONE = 0,
    /* SMS_TYPE */
    MIPC_SMS_TYPE_REGULAR_SMS                               = 0,
    MIPC_SMS_TYPE_STK_SMS                                   = 1,
    MIPC_SMS_TYPE_EMERGENCY_SMS                             = 2,
};
typedef enum mipc_sms_type_const_enum mipc_sms_type_const_enum;

enum mipc_nw_data_bearer_capability_const_enum {
    mipc_nw_data_bearer_capability_const_NONE = 0,
    /* NW_DATA_BEARER_CAPABILITY */
    MIPC_NW_DATA_BEARER_CAPABILITY_NONE_ACTIVATE            = 0,
    MIPC_NW_DATA_BEARER_CAPABILITY_LTE_CAPABBILITY          = 18,
    MIPC_NW_DATA_BEARER_CAPABILITY_LTE_CA_CAPABILITY        = 19,
};
typedef enum mipc_nw_data_bearer_capability_const_enum mipc_nw_data_bearer_capability_const_enum;

enum mipc_nw_signal_modulation_const_enum {
    mipc_nw_signal_modulation_const_NONE = 0,
    /* NW_SIGNAL_MODULATION */
    MIPC_NW_SIGNAL_MODULATION_BPSK                          = 0,
    MIPC_NW_SIGNAL_MODULATION_QPSK                          = 1,
    MIPC_NW_SIGNAL_MODULATION_8PSK                          = 2,
    MIPC_NW_SIGNAL_MODULATION_16QAM                         = 3,
    MIPC_NW_SIGNAL_MODULATION_32QAM                         = 4,
    MIPC_NW_SIGNAL_MODULATION_64QAM                         = 5,
};
typedef enum mipc_nw_signal_modulation_const_enum mipc_nw_signal_modulation_const_enum;

enum mipc_nw_rat_mode_const_enum {
    mipc_nw_rat_mode_const_NONE = 0,
    /* NW_RAT_MODE */
    MIPC_NW_RAT_MODE_LTE                                    = 3,
    MIPC_NW_RAT_MODE_LTE_NR                                 = 19,
};
typedef enum mipc_nw_rat_mode_const_enum mipc_nw_rat_mode_const_enum;

enum mipc_nw_prefer_rat_const_enum {
    mipc_nw_prefer_rat_const_NONE = 0,
    /* NW_PREFER_RAT */
    MIPC_NW_PREFER_RAT_NO_PREFER                            = 0,
    MIPC_NW_PREFER_RAT_GSM_PREFER                           = 1,
    MIPC_NW_PREFER_RAT_UMTS_PREFER                          = 2,
    MIPC_NW_PREFER_RAT_LTE_PREFER                           = 4,
    MIPC_NW_PREFER_RAT_C2K_PREFER                           = 16,
    MIPC_NW_PREFER_RAT_NR_PREFER                            = 128,
};
typedef enum mipc_nw_prefer_rat_const_enum mipc_nw_prefer_rat_const_enum;

enum mipc_nw_access_tech_const_enum {
    mipc_nw_access_tech_const_NONE = 0,
    /* NW_ACCESS_TECH */
    /* E-UTRAN */
    MIPC_NW_ACCESS_TECH_EUTRAN                              = 7,
    /* E-UTRAN (NB-S1 mode) */
    MIPC_NW_ACCESS_TECH_EUTRAN_NBS1_MODE                    = 9,
    /* E-UTRAN connected to 5GCN */
    MIPC_NW_ACCESS_TECH_EUTRAN_5GCN                         = 10,
    /* NR connected to 5GCN */
    MIPC_NW_ACCESS_TECH_NR_5GCN                             = 11,
    /* NR connected to EPS core */
    MIPC_NW_ACCESS_TECH_NR_EPS                              = 12,
    /* NR connected to EPS core */
    MIPC_NW_ACCESS_TECH_NG_RAN                              = 13,
    /* E-UTRA-NR dual connectivity */
    MIPC_NW_ACCESS_TECH_EUTRA_NR_DUAL                       = 14,
};
typedef enum mipc_nw_access_tech_const_enum mipc_nw_access_tech_const_enum;

enum mipc_nw_rat_lock_const_enum {
    mipc_nw_rat_lock_const_NONE = 0,
    /* NW_RAT_LOCK */
    /* It indicates RAT can be set. */
    MIPC_NW_RAT_LOCK_UNLOCK                                 = 0,
    /* It indicates RAT cannot be set. */
    MIPC_NW_RAT_LOCK_LOCK                                   = 1,
};
typedef enum mipc_nw_rat_lock_const_enum mipc_nw_rat_lock_const_enum;

enum mipc_nw_mipi_op_const_enum {
    mipc_nw_mipi_op_const_NONE = 0,
    /* NW_MIPI_OP */
    MIPC_NW_MIPI_OP_READ                                    = 0,
    MIPC_NW_MIPI_OP_WRITE                                   = 1,
};
typedef enum mipc_nw_mipi_op_const_enum mipc_nw_mipi_op_const_enum;

enum mipc_nw_mipi_rw_type_const_enum {
    mipc_nw_mipi_rw_type_const_NONE = 0,
    /* NW_MIPI_RW_TYPE */
    MIPC_NW_MIPI_RW_TYPE_MIPI_RW                            = 0,
    MIPC_NW_MIPI_RW_TYPE_MIPI_EXTRW_1BYTE                   = 1,
};
typedef enum mipc_nw_mipi_rw_type_const_enum mipc_nw_mipi_rw_type_const_enum;

enum mipc_nw_mipi_cause_const_enum {
    mipc_nw_mipi_cause_const_NONE = 0,
    /* NW_MIPI_CAUSE */
    MIPC_NW_MIPI_CAUSE_MIPI_PORT_INVALID                    = 1,
};
typedef enum mipc_nw_mipi_cause_const_enum mipc_nw_mipi_cause_const_enum;

enum mipc_nw_bpi_op_const_enum {
    mipc_nw_bpi_op_const_NONE = 0,
    /* NW_BPI_OP */
    MIPC_NW_BPI_OP_READ                                     = 0,
    MIPC_NW_BPI_OP_WRITE                                    = 1,
};
typedef enum mipc_nw_bpi_op_const_enum mipc_nw_bpi_op_const_enum;

enum mipc_nw_endc_deactivate_mode_const_enum {
    mipc_nw_endc_deactivate_mode_const_NONE = 0,
    /* NW_ENDC_DEACTIVATE_MODE */
    MIPC_NW_ENDC_DEACTIVATE_MODE_DISABLE                    = 0,
    MIPC_NW_ENDC_DEACTIVATE_MODE_ENABLE                     = 1,
    MIPC_NW_ENDC_DEACTIVATE_MODE_DEACTIVATE_WITHOUT_SEND_SCG_FAILURE = 2,
};
typedef enum mipc_nw_endc_deactivate_mode_const_enum mipc_nw_endc_deactivate_mode_const_enum;

enum mipc_nw_sa_slience_mode_const_enum {
    mipc_nw_sa_slience_mode_const_NONE = 0,
    /* NW_SA_SLIENCE_MODE */
    MIPC_NW_SA_SLIENCE_MODE_OFF                             = 0,
    MIPC_NW_SA_SLIENCE_MODE_ON                              = 1,
    MIPC_NW_SA_SLIENCE_MODE_ENABLE_HANDOVER_ONLY            = 2,
    MIPC_NW_SA_SLIENCE_MODE_REST_ONLY                       = 3,
};
typedef enum mipc_nw_sa_slience_mode_const_enum mipc_nw_sa_slience_mode_const_enum;

enum mipc_nw_sa_blocking_mode_const_enum {
    mipc_nw_sa_blocking_mode_const_NONE = 0,
    /* NW_SA_BLOCKING_MODE */
    MIPC_NW_SA_BLOCKING_MODE_NO_BLOCKING                    = 0,
    MIPC_NW_SA_BLOCKING_MODE_BLOCKING                       = 1,
};
typedef enum mipc_nw_sa_blocking_mode_const_enum mipc_nw_sa_blocking_mode_const_enum;

enum mipc_nw_tx_rat_mode_const_enum {
    mipc_nw_tx_rat_mode_const_NONE = 0,
    /* NW_TX_RAT_MODE */
    MIPC_NW_TX_RAT_MODE_LTE                                 = 0,
    MIPC_NW_TX_RAT_MODE_NR                                  = 1,
    MIPC_NW_TX_RAT_MODE_NR_FR1                              = 2,
    MIPC_NW_TX_RAT_MODE_NR_FR2                              = 3,
    MIPC_NW_TX_RAT_MODE_LTE_NR_FR1                          = 4,
    MIPC_NW_TX_RAT_MODE_LTE_NR_FR2                          = 5,
    MIPC_NW_TX_RAT_MODE_LTE_NR                              = 6,
    MIPC_NW_TX_RAT_MODE_GSM                                 = 7,
    MIPC_NW_TX_RAT_MODE_WCDMA                               = 8,
    MIPC_NW_TX_RAT_MODE_C2K                                 = 9,
};
typedef enum mipc_nw_tx_rat_mode_const_enum mipc_nw_tx_rat_mode_const_enum;

enum mipc_nw_cg_type_const_enum {
    mipc_nw_cg_type_const_NONE = 0,
    /* NW_CG_TYPE */
    MIPC_NW_CG_TYPE_MCG                                     = 0,
    MIPC_NW_CG_TYPE_SCG                                     = 1,
    MIPC_NW_CG_TYPE_ALL_CG                                  = 2,
    MIPC_NW_CG_TYPE_INVALID                                 = 0xFF,
};
typedef enum mipc_nw_cg_type_const_enum mipc_nw_cg_type_const_enum;

enum mipc_nw_overheating_status_const_enum {
    mipc_nw_overheating_status_const_NONE = 0,
    /* NW_OVERHEATING_STATUS */
    MIPC_NW_OVERHEATING_STATUS_FAIL                         = 0,
    MIPC_NW_OVERHEATING_STATUS_SUCCESS                      = 1,
};
typedef enum mipc_nw_overheating_status_const_enum mipc_nw_overheating_status_const_enum;

enum mipc_nw_uai_op_const_enum {
    mipc_nw_uai_op_const_NONE = 0,
    /* NW_UAI_OP */
    MIPC_NW_UAI_OP_LEAVE                                    = 0,
    MIPC_NW_UAI_OP_ENTER                                    = 1,
};
typedef enum mipc_nw_uai_op_const_enum mipc_nw_uai_op_const_enum;

enum mipc_nw_sase_cause_const_enum {
    mipc_nw_sase_cause_const_NONE = 0,
    /* NW_SASE_CAUSE */
    MIPC_NW_SASE_CAUSE_NONE                                 = 0,
    MIPC_NW_SASE_CAUSE_NOT_SUPPORTED                        = 1,
    MIPC_NW_SASE_CAUSE_T345_PENDING                         = 2,
    MIPC_NW_SASE_CAUSE_UNDER_LV_SESSION                     = 3,
    MIPC_NW_SASE_CAUSE_NO_NEED_TO_SEND                      = 4,
    MIPC_NW_SASE_CAUSE_CONNECTED_NOT_ALLOWED                = 5,
};
typedef enum mipc_nw_sase_cause_const_enum mipc_nw_sase_cause_const_enum;

enum mipc_nw_mimo_fr_dl_const_enum {
    mipc_nw_mimo_fr_dl_const_NONE = 0,
    /* NW_MIMO_FR_DL */
    MIPC_NW_MIMO_FR_DL_MIMO_LAYERS_DL2                      = 0,
    MIPC_NW_MIMO_FR_DL_MIMO_LAYERS_DL4                      = 1,
    MIPC_NW_MIMO_FR_DL_MIMO_LAYERS_DL8                      = 2,
};
typedef enum mipc_nw_mimo_fr_dl_const_enum mipc_nw_mimo_fr_dl_const_enum;

enum mipc_nw_mimo_fr_ul_const_enum {
    mipc_nw_mimo_fr_ul_const_NONE = 0,
    /* NW_MIMO_FR_UL */
    MIPC_NW_MIMO_FR_UL_MIMO_LAYERS_UL1                      = 0,
    MIPC_NW_MIMO_FR_UL_MIMO_LAYERS_UL2                      = 1,
    MIPC_NW_MIMO_FR_UL_MIMO_LAYERS_UL4                      = 2,
};
typedef enum mipc_nw_mimo_fr_ul_const_enum mipc_nw_mimo_fr_ul_const_enum;

enum mipc_nw_uai_power_saving_type_const_enum {
    mipc_nw_uai_power_saving_type_const_NONE = 0,
    /* NW_UAI_POWER_SAVING_TYPE */
    MIPC_NW_UAI_POWER_SAVING_TYPE_MAX_CC                    = 0,
    MIPC_NW_UAI_POWER_SAVING_TYPE_MAX_MIMO                  = 1,
    MIPC_NW_UAI_POWER_SAVING_TYPE_DRX                       = 2,
    MIPC_NW_UAI_POWER_SAVING_TYPE_MAX_BW                    = 3,
    MIPC_NW_UAI_POWER_SAVING_TYPE_MIN_SCHED                 = 4,
};
typedef enum mipc_nw_uai_power_saving_type_const_enum mipc_nw_uai_power_saving_type_const_enum;

enum mipc_nw_uai_req_op_const_enum {
    mipc_nw_uai_req_op_const_NONE = 0,
    /* NW_UAI_REQ_OP */
    MIPC_NW_UAI_REQ_OP_UAI_NOT_SUPPORT                      = 0,
    MIPC_NW_UAI_REQ_OP_UAI_ALREADY_SENT_ENTER_REQ           = 1,
    MIPC_NW_UAI_REQ_OP_UAI_ALREADY_SENT_LEAVE_REQ           = 2,
    MIPC_NW_UAI_REQ_OP_UAI_ALREADY_PENDING_ENTER_REQ        = 3,
    MIPC_NW_UAI_REQ_OP_UAI_ALREADY_PENDING_LEAVE_REQ        = 4,
};
typedef enum mipc_nw_uai_req_op_const_enum mipc_nw_uai_req_op_const_enum;

enum mipc_nw_rrc_release_op_const_enum {
    mipc_nw_rrc_release_op_const_NONE = 0,
    /* NW_RRC_RELEASE_OP */
    MIPC_NW_RRC_RELEASE_OP_RRC_IDLE                         = 0,
    MIPC_NW_RRC_RELEASE_OP_RRC_INACTIVE                     = 1,
    MIPC_NW_RRC_RELEASE_OP_RRC_OUT_OF_CONNECTED             = 2,
    MIPC_NW_RRC_RELEASE_OP_RRC_CONNECTED                    = 3,
};
typedef enum mipc_nw_rrc_release_op_const_enum mipc_nw_rrc_release_op_const_enum;

enum mipc_nw_fake_ri_band_const_enum {
    mipc_nw_fake_ri_band_const_NONE = 0,
    /* NW_FAKE_RI_BAND */
    MIPC_NW_FAKE_RI_BAND_FR1                                = 0,
    MIPC_NW_FAKE_RI_BAND_FR2                                = 1,
    MIPC_NW_FAKE_RI_BAND_INVALID                            = 0xFF,
};
typedef enum mipc_nw_fake_ri_band_const_enum mipc_nw_fake_ri_band_const_enum;

enum mipc_nw_fake_ri_ctrl_rank_const_enum {
    mipc_nw_fake_ri_ctrl_rank_const_NONE = 0,
    /* NW_FAKE_RI_CTRL_RANK */
    MIPC_NW_FAKE_RI_CTRL_RANK_RANK1                         = 0,
    MIPC_NW_FAKE_RI_CTRL_RANK_RANK2                         = 1,
    MIPC_NW_FAKE_RI_CTRL_RANK_RANK4                         = 2,
};
typedef enum mipc_nw_fake_ri_ctrl_rank_const_enum mipc_nw_fake_ri_ctrl_rank_const_enum;

enum mipc_nw_fake_ri_rat_const_enum {
    mipc_nw_fake_ri_rat_const_NONE = 0,
    /* NW_FAKE_RI_RAT */
    MIPC_NW_FAKE_RI_RAT_LTE                                 = 1,
    MIPC_NW_FAKE_RI_RAT_NR                                  = 2,
};
typedef enum mipc_nw_fake_ri_rat_const_enum mipc_nw_fake_ri_rat_const_enum;

enum mipc_nw_ftai_attr_all_with_type_const_enum {
    mipc_nw_ftai_attr_all_with_type_const_NONE = 0,
    /* NW_FTAI_ATTR_ALL_WITH_TYPE */
    MIPC_NW_FTAI_ATTR_ALL_WITH_TYPE_FTAI_ATTR_NONE          = 0,
    MIPC_NW_FTAI_ATTR_ALL_WITH_TYPE_FTAI_ATTR_ALL_WITH_INTEGRITY = 1,
    MIPC_NW_FTAI_ATTR_ALL_WITH_TYPE_FTAI_ATTR_ALL_WITH_NON_INTEGRITY = 2,
};
typedef enum mipc_nw_ftai_attr_all_with_type_const_enum mipc_nw_ftai_attr_all_with_type_const_enum;

enum mipc_nw_epsfb_state_enum_const_enum {
    mipc_nw_epsfb_state_enum_const_NONE = 0,
    /* NW_EPSFB_STATE_ENUM */
    MIPC_NW_EPSFB_STATE_ENUM_STARTED                        = 0,
    MIPC_NW_EPSFB_STATE_ENUM_SUCCESSFUL                     = 1,
    MIPC_NW_EPSFB_STATE_ENUM_FAILURE                        = 2,
};
typedef enum mipc_nw_epsfb_state_enum_const_enum mipc_nw_epsfb_state_enum_const_enum;

enum mipc_nw_epsfb_type_enum_const_enum {
    mipc_nw_epsfb_type_enum_const_NONE = 0,
    /* NW_EPSFB_TYPE_ENUM */
    MIPC_NW_EPSFB_TYPE_ENUM_HANDOVER                        = 0,
    MIPC_NW_EPSFB_TYPE_ENUM_REDIRECT                        = 1,
};
typedef enum mipc_nw_epsfb_type_enum_const_enum mipc_nw_epsfb_type_enum_const_enum;

enum mipc_nw_tau_fail_cause_enum_const_enum {
    mipc_nw_tau_fail_cause_enum_const_NONE = 0,
    /* NW_TAU_FAIL_CAUSE_ENUM */
    MIPC_NW_TAU_FAIL_CAUSE_ENUM_DEFAULT                     = 0,
};
typedef enum mipc_nw_tau_fail_cause_enum_const_enum mipc_nw_tau_fail_cause_enum_const_enum;

enum mipc_sys_coex_uart_mode_op_const_enum {
    mipc_sys_coex_uart_mode_op_const_NONE = 0,
    /* SYS_COEX_UART_MODE_OP */
    MIPC_SYS_COEX_UART_MODE_OP_Disable                      = 0,
    MIPC_SYS_COEX_UART_MODE_OP_Enable                       = 1,
};
typedef enum mipc_sys_coex_uart_mode_op_const_enum mipc_sys_coex_uart_mode_op_const_enum;

enum mipc_sys_nvram_cause_const_enum {
    mipc_sys_nvram_cause_const_NONE = 0,
    /* SYS_NVRAM_CAUSE */
    /* NVRAM LID access is successful */
    MIPC_SYS_NVRAM_CAUSE_OK                                 = 0,
    /* NVRAM LID access failed */
    MIPC_SYS_NVRAM_CAUSE_FAIL                               = 1,
    /* NVRAM module is not initialized */
    MIPC_SYS_NVRAM_CAUSE_NOT_READY                          = 2,
    /* NVRAM found an invalid pointer */
    MIPC_SYS_NVRAM_CAUSE_INVALID_PTR                        = 3,
    /* NVRAM found an invalid LID */
    MIPC_SYS_NVRAM_CAUSE_INVALID_ENTITY_ID                  = 4,
    /* NVRAM find an invalid record's index of the LID */
    MIPC_SYS_NVRAM_CAUSE_INVALID_REC_INDEX                  = 5,
    /* NVRAM find an invalid record's amount of the LID */
    MIPC_SYS_NVRAM_CAUSE_INVALID_REC_AMOUNT                 = 6,
    /* NVRAM find an invalid lock type */
    MIPC_SYS_NVRAM_CAUSE_INVALID_LOCK_TYPE                  = 7,
    /* NVRAM find an invalid buffer pointer */
    MIPC_SYS_NVRAM_CAUSE_INVALID_BUF                        = 8,
    /* NVRAM find an invalid buffer size */
    MIPC_SYS_NVRAM_CAUSE_INVALID_BUF_SIZE                   = 9,
    /* NVRAM find an invalid factory rest or rebuilds a command */
    MIPC_SYS_NVRAM_CAUSE_INVALID_CMD                        = 10,
    /* NVRAM find an invalid read type */
    MIPC_SYS_NVRAM_CAUSE_INVALID_CHKSUM_TYPE                = 11,
    /* NVRAM find an invalid rest category in the reset LID message */
    MIPC_SYS_NVRAM_CAUSE_INVALID_RESET_CATEGORY             = 12,
    /* NVRAM is software locked and user writes protected LIDs */
    MIPC_SYS_NVRAM_CAUSE_WRITE_LOCKED                       = 13,
    /* NVRAM security check fails */
    MIPC_SYS_NVRAM_CAUSE_ACCESS_DENIED                      = 14,
    /* NVRAM for customer sensitive LID security check fails */
    MIPC_SYS_NVRAM_CAUSE_ACCESS_CUSTOM_DENIED               = 15,
    /* NVRAM LID signature check fails */
    MIPC_SYS_NVRAM_CAUSE_SIG_CHK_FAIL                       = 16,
    /* NVRAM monitors pool overflow */
    MIPC_SYS_NVRAM_CAUSE_MON_POOL_OVERFLOW                  = 17,
};
typedef enum mipc_sys_nvram_cause_const_enum mipc_sys_nvram_cause_const_enum;

enum mipc_sys_mddbg_control_type_const_enum {
    mipc_sys_mddbg_control_type_const_NONE = 0,
    /* SYS_MDDBG_CONTROL_TYPE */
    MIPC_SYS_MDDBG_CONTROL_TYPE_MDDBG_CONTROL_TYPE_MMV1     = 0,
    MIPC_SYS_MDDBG_CONTROL_TYPE_MDDBG_CONTROL_TYPE_DIAG     = 1,
    MIPC_SYS_MDDBG_CONTROL_TYPE_MDDBG_CONTROL_TYPE_WP       = 2,
    MIPC_SYS_MDDBG_CONTROL_TYPE_MDDBG_CONTROL_TYPE_BP       = 3,
};
typedef enum mipc_sys_mddbg_control_type_const_enum mipc_sys_mddbg_control_type_const_enum;

enum mipc_encoding_scheme_const_enum {
    mipc_encoding_scheme_const_NONE = 0,
    /* ENCODING_SCHEME */
    /* Unknown encoding scheme. */
    MIPC_ENCODING_SCHEME_UNKNOWN                            = 0,
    /* PLMN name encoding scheme is ASCII. */
    MIPC_ENCODING_SCHEME_ASCII                              = 1,
    /* PLMN name encoding scheme is UCS2. */
    MIPC_ENCODING_SCHEME_UCS2                               = 2,
    /* PLMN name encoding scheme is UTF8. */
    MIPC_ENCODING_SCHEME_UTF8                               = 3,
};
typedef enum mipc_encoding_scheme_const_enum mipc_encoding_scheme_const_enum;

enum mipc_plmn_name_source_const_enum {
    mipc_plmn_name_source_const_NONE = 0,
    /* PLMN_NAME_SOURCE */
    /* Name corresponding to the PLMN id is not found in SIM card, network messages or modem list */
    MIPC_PLMN_NAME_SOURCE_NOT_FOUND                         = 0,
    /* Name source is the SIM card */
    MIPC_PLMN_NAME_SOURCE_SIM                               = 1,
    /* Name source is from NITZ. The corresponding PLMN name was not found in SIM */
    MIPC_PLMN_NAME_SOURCE_NETWORK                           = 2,
    /* Name source is maintained by the modem TS25. The corresponding PLMN name was not found in SIM or network messages */
    MIPC_PLMN_NAME_SOURCE_TS25                              = 3,
};
typedef enum mipc_plmn_name_source_const_enum mipc_plmn_name_source_const_enum;



#define MIPC_MAX_AT_URC_LEN                                (128) /* Maximum AT URC length */
#define MIPC_MAX_AT_CMD_LEN                                (128) /* Maximum AT command length */
#define MIPC_MAX_AT_CMD_RSP_LEN                            (1024) /* Maximum AT command response length */
#define MIPC_MAX_AT_GENERAL_LEN                            (2048) /* Maximum general AT command length */
#define MIPC_MAX_REG_UNREG_NUM_IN_ONE_MIPC_REQ             (256) /* Maximum number of mipc_msgs that could be reg/unreg in one REG_REQ/UNREG_REQ */
#define MIPC_MAX_APN_LEN                                   (100) /* Maximum APN Name length */
#define MIPC_MAX_DEL_SPI_NUM                               (50) /* Maximum SPI delete count, SPI is uint32 */
#define MIPC_MAX_APN_BEARER_LEN                            (10) /* Maximum APN bearer length */
#define MIPC_MAX_APN_LIST_NUM                              (10) /* The number of APN LIST */
#define MIPC_MAX_CID_LIST_LEN                              (200) /* cid is 0~199 */
#define MIPC_MAX_RULE_LIST_LEN                             (8) /*  */
#define MIPC_MAX_USERID_LEN                                (64) /* Maximum User Name length */
#define MIPC_MAX_PASSWORD_LEN                              (64) /* Maximum Password length */
#define MIPC_MAX_PROFILE_LIST_COUNT                        (20) /* Maximum profile list count */
#define MIPC_MAX_PACKET_FILTER_LIST_COUNT                  (10) /* Maximum packet filter count */
#define MIPC_MAX_EIWLAN_CMD_LEN                            (8) /* Maximum IWLAN PRIORITY command length */
#define MIPC_MAX_EIWLAN_TYPE_LEN                           (16) /* Maximum IWLAN PRIORITY type length */
#define MIPC_MAX_EIWLAN_DESCRIPTION_LEN                    (32) /* Maximum IWLAN PRIORITY description length */
#define MIPC_FIX_CALL_BARRING_PASSWORD_LEN                 (4) /* Maximum Call barring Password length */
#define MIPC_MAX_OPER_LEN                                  (16) /* Maximum operator name length */
#define MIPC_MAX_USSD_LEN                                  (160) /* Maximum USSD length */
#define MIPC_MAX_USSD_LEN_EX                               (10000) /* Maximum USSD payload external length */
#define MIPC_MAX_PAC_IND_LEN                               (512) /* Maximum pac ind length */
#define MIPC_MAX_TERMINAL_RESPONSE_LEN                     (261) /* Maximum terminal response length */
#define MIPC_MAX_AID_LEN                                   (256) /* Maximum aid length in refresh */
#define MIPC_MAX_ENVELOP_LEN                               (261) /* Maximum envelope length */
#define MIPC_MAX_BIP_LEN                                   (32) /* Maximum stk bip command length */
#define MIPC_MAX_S_NSSAI_NUM                               (64) /* Maximum s-nssai count in s-nssai list */
#define MIPC_MAX_CLIENT_NAME_LEN                           (160) /* Maximum client name length */
#define MIPC_SIM_FIX_ICCID_LEN                             (20) /* Maximum ICCID length */
#define MIPC_SIM_FIX_ICCID_STR_LEN                         (21) /* Maximum ICCID length, including string ending */
#define MIPC_MAX_IMSI_LEN                                  (20) /* Maximum IMSI length */
#define MIPC_MAX_ATR_LEN                                   (80) /* Maximum ATR string length */
#define MIPC_MAX_ATR_STR_LEN                               (80) /* Maximum ATR string length, including string ending */
#define MIPC_MAX_DEVICEID_LEN                              (16) /*  */
#define MIPC_MAX_MANUFACTURER_LEN                          (32) /*  */
#define MIPC_MAX_FIRMWARE_INFO_LEN                         (64) /*  */
#define MIPC_MAX_HARDWAREWARE_INFO_LEN                     (30) /*  */
#define MIPC_FIX_ESN_LEN                                   (8) /* Fixed length for ESN */
#define MIPC_FIX_IMEISV_LEN                                (2) /* Fixed length for IMEISV */
#define MIPC_FIX_MEID_LEN                                  (14) /* Fixed length for MEID */
#define MIPC_FIX_HARDWARE_ID_LEN                           (16) /* Fixed length for harware id */
#define MIPC_MAX_DUMP_LIDS_LEN                             (1024) /* max length for mcf dump lids */
#define MIPC_MAX_MCF_VALUE_LEN                             (512) /* max length for mcf value */
#define MIPC_MAX_MCF_CFG_LEN                               (1024) /* max length for mcf config */
#define MIPC_MAX_MSPM_PROCEDURE_STR_LEN                    (50) /* Maximum MSPM session procedure length */
#define MIPC_MAX_NETWORK_NAME_LEN                          (60) /*  */
#define MIPC_MAX_ROAMING_TEXT_LEN                          (64) /*  */
#define MIPC_FIX_GSM_AUTH_RAND_LEN                         (16) /* Fixed length for RAND */
#define MIPC_FIX_GSM_AUTH_RSP_KC_LEN                       (8) /* Fixed length for KC */
#define MIPC_FIX_GSM_AUTH_RSP_SRES_LEN                     (4) /* Fixed length for SRES */
#define MIPC_MAX_EXT_AUTH_CMD_DATA_LEN                     (256) /* Maximum command data length of extention authenticate command */
#define MIPC_MAX_EXT_AUTH_RSP_DATA_LEN                     (256) /* Maximum command response length of extention authenticate command */
#define MIPC_MAX_TERMINAL_CAPABILITY_DATA_LEN              (256) /* Maximum data length of terminal capability */
#define MIPC_MAX_SIM_APP_LABEL_LEN                         (32) /* Maximum data length of application label */
#define MIPC_MAX_SIM_AID_BYTE_LEN                          (20) /* Maximum data length of application id */
#define MIPC_MAX_SIM_AID_STR_LEN                           (40) /* Maximum string length of application id */
#define MIPC_MAX_SMS_PDU_LEN                               (256) /* Maximum length of SMS PDU(include the sca) */
#define MIPC_MAX_CBM_PDU_LEN                               (1246) /* Maximum length of CBM PDU((82+1)*15+1) */
#define MIPC_MAX_SMS_SCA_LEN                               (32) /* Maximum length of SMS SCA(include the NULL-ending) */
#define MIPC_FIX_SMS_ETWS_SECUR_INFO_LEN                   (50) /* fixed length of ETWS security info */
#define MIPC_MAX_SMS_NUM_LEN                               (32) /* Maximum length of SMS number */
#define MIPC_MAX_SMS_CBM_MSGID_RANGE_NUM                   (30) /*  */
#define MIPC_MAX_SMS_CBM_MSGID_SINGLE_NUM                  (60) /*  */
#define MIPC_MAX_SMS_CBM_DCS_RANGE_NUM                     (30) /*  */
#define MIPC_MAX_SMS_CBM_DCS_SINGLE_NUM                    (60) /*  */
#define MIPC_MAX_SMS_CBM_LANGUAGE_SINGLE_NUM               (60) /*  */
#define MIPC_MAX_SMS_EPSI_LEN                              (256) /*  */
#define MIPC_MAX_SMS_SEG_NUM                               (15) /*  */
#define MIPC_MAX_SMS_DUP_PDU_LEN                           (256) /* Maximum length of SMS PDU */
#define MIPC_MAX_SMS_NUM                                   (1024) /* Maximum sms numbers in group action */
#define MIPC_MAX_SMS_CBM_RANGE_NUM                         (30) /* Maximum sms range numbers in cbm group action */
#define MIPC_MAX_SMS_CBM_SINGLE_NUM                        (60) /* Maximum sms single numbers in cbm group action */
#define MIPC_MAX_WFC_IFNAME_LEN                            (128) /* max length of WFC ifname(like wlan0) */
#define MIPC_MAX_WFC_THRESHOLD_NUM                         (16) /* max length of WFC threshold */
#define MIPC_MAX_WFC_IP_LEN                                (16) /* 16 for IPV6, 4 for IPv4 */
#define MIPC_FIX_WFC_EMC_AID_LEN                           (20) /* fixed length of WFC emergency address id */
#define MIPC_MAX_WFC_SSID_LEN                              (33) /* max length of WFC ssid */
#define MIPC_FIX_WFC_MAC_LEN                               (6) /* fix length of WFC mac */
#define MIPC_FIX_WFC_IPV4_LEN                              (4) /* fix length of WFC ipv4 */
#define MIPC_FIX_WFC_IPV6_LEN                              (16) /* fix length of WFC ipv6 */
#define MIPC_MAX_WFC_DNS_NUM                               (10) /* max wfc dns num */
#define MIPC_MAX_WFC_WIFI_TYPE_LEN                         (32) /* max length of WFC wifi type */
#define MIPC_MAX_SS_DIAL_NUMBER_LEN                        (182) /* Maximum dail number in ss service */
#define MIPC_MAX_SS_DIAL_NUMBER_FOR_ALIGN_LEN              (183) /* Maximum dail number in ss service for align */
#define MIPC_MAX_SS_DIAL_COUNT                             (10) /* Maximum dail number count in ss call barring */
#define MIPC_MAX_DIAL_ADDRESS_LEN                          (180) /* Maximum dail address length */
#define MIPC_MAX_CALL_NUMBER_LEN                           (128) /* Maximum call number length */
#define MIPC_MAX_CALL_PAU_LEN                              (512) /* Maximum call pau length */
#define MIPC_MAX_CALL_ECC_NUMBER_LEN                       (10) /* Maximum ecc call number length */
#define MIPC_MAX_CALL_CONFERENCE_PARTICIPANT_NUM           (40) /* Maximum conference call paticipant number */
#define MIPC_MAX_CALL_NAME_LEN                             (80) /* Maximum call name length */
#define MIPC_MAX_CALL_EIMSEVTPKG_URI_LEN                   (180) /* Maximum call +eimsevtpkg uri length */
#define MIPC_MAX_CALL_DISPLAY_NAME_LEN                     (64) /* Maximum call display name length */
#define MIPC_MAX_CALL_VERSTAT_LEN                          (64) /* Maximum call verstat length */
#define MIPC_MAX_CALL_PRIVACY_LEN                          (64) /* Maximum call privacy length */
#define MIPC_MAX_CALL_SS_SUBADDR_LEN                       (64) /*  */
#define MIPC_MAX_SIM_PATH_BYTE_LEN                         (12) /* Maximum byte length of sim path for byte_array type */
#define MIPC_MAX_SIM_PATH_STR_LEN                          (24) /* Maximum string length of sim path */
#define MIPC_MAX_SIM_APDU_BYTE_LEN                         (261) /* Maximum length of APDU byte */
#define MIPC_MAX_SIM_APDU_STRING_LEN                       (522) /* Maximum length of APDU string */
#define MIPC_MAX_SIM_LONG_BIN_DATA_LEN                     (65535) /* Maximum length of command or response data for LONG APDU operation */
#define MIPC_MAX_SIM_PIN_CODE_LEN                          (16) /* Maximum pin code length for sim card */
#define MIPC_MAX_SIM_PUK_CODE_LEN                          (16) /* Maximum puk code length for sim card */
#define MIPC_MAX_SIM_CMD_DATA_STR_LEN                      (510) /* Maximum command data string length of APDU */
#define MIPC_MAX_SIM_RSP_DATA_STR_LEN                      (510) /* Maximum response data string length of APDU */
#define MIPC_MAX_SIM_CMD_DATA_BYTE_LEN                     (255) /* Maximum command data byte length of APDU */
#define MIPC_MAX_SIM_CMD_EXTENDED_DATA_BYTE_LEN            (65535) /* Maximum command data byte length of extended APDU */
#define MIPC_MAX_SIM_RSP_DATA_BYTE_LEN                     (255) /* Maximum response data  byte length of APDU */
#define MIPC_MAX_SIM_NUM                                   (2) /* Maximum SIM number that user can use to get normal service */
#define MIPC_MAX_PHY_SLOT_NUM                              (4) /* Maximum physical slot number of this device, including embedded SIM */
#define MIPC_MAX_SIM_EID_LEN                               (32) /* Maximum eid string length of euicc */
#define MIPC_MAX_SIM_EID_STR_LEN                           (33) /* Maximum eid string length of euicc, including string ending */
#define MIPC_FIX_SIM_EID_BYTE_LEN                          (16) /* Fixed size for the eid byte array */
#define MIPC_MAX_SIM_FACILITY_LEN                          (3) /* maximum string length of SIM facility string */
#define MIPC_MAX_SIM_MSISDN_LEN                            (81) /*  */
#define MIPC_MAX_MSISDN_NUM                                (10) /*  */
#define MIPC_MAX_SIM_SML_HCK_STR_LEN                       (128) /* maximum string length of SIM sml key string */
#define MIPC_MAX_CALL_FINISH_REASON_LEN                    (128) /*  */
#define MIPC_MAX_DTMF_DIGIT_LEN                            (128) /* Maximum call dtmf digit length. when DTMF_MODE is SINGLE_TONE, MAX_DTMF_DIGIT_LEN is 2 */
#define MIPC_MAX_RECV_DTMF_DIGIT_LEN                       (2) /* Maximum call dtmf digit length. */
#define MIPC_MAX_CALL_ECC_LIST_RECORD_NUM                  (128) /* Maximum record of ecc list record  */
#define MIPC_MAX_CALL_SIP_HEADER_VALUE_PAIR_LEN            (232) /*  */
#define MIPC_MAX_CALL_SIP_HEADER_VALUE_LEN                 (1950) /* Maximum call sip header value length */
#define MIPC_MAX_CALL_ADDITIONAL_INFO_LEN                  (1950) /* Maximum call additinal info length */
#define MIPC_MAX_CALL_MT_SIP_INVITE_LEN                    (1950) /* Maximum MT SIP INVITE length */
#define MIPC_MAX_CALL_RCS_DIGITS_LINE_LEN                  (128) /* Maximum call rcs digits line info string length */
#define MIPC_MAX_CALL_RTT_TEXT_LEN                         (1024) /* Maximum call send Rtt Text length */
#define MIPC_MAX_CALL_PULL_URI_LEN                         (128) /* Maximum call pull uri length */
#define MIPC_MAX_CALL_DISPLAY_AND_SIGNALS_INFO_DISPLAY_LEN (64) /*  */
#define MIPC_MAX_CALL_EXTENDED_DISPLAY_INFO_LEN            (64) /*  */
#define MIPC_MAX_ECALL_MSD_DATA_LEN                        (140) /* Maximum number of bytes of eCall MSD */
#define MIPC_MAX_SIP_REASON_TEXT_LEN                       (120) /*  */
#define MIPC_IMS_MAX_URI_LEN                               (256) /*  */
#define MIPC_IMS_MAX_ERROR_MESSAGE_LEN                     (256) /*  */
#define MIPC_MAX_IMS_EVENT_PACKAGE_DATA_LEN                (64000) /*  */
#define MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN                 (64000) /*  */
#define MIPC_MAX_CALL_NUM                                  (7) /* Maximum call count */
#define MIPC_MAX_PCO_COUNT                                 (16) /* Maximum pco struct count of pco list */
#define MIPC_MAX_MBS_SESSION_INFO_NUM                      (16) /* Maximum MBS session info count */
#define MIPC_MAX_MBS_SERVICE_AREA_NUM                      (16) /*  */
#define MIPC_MAX_SBP_STR_LEN                               (256) /* Maximum length of sbp string */
#define MIPC_MAX_PLMN_ID_LEN                               (7) /* Maximum length of plmn id(mcc + mnc) */
#define MIPC_MAX_BAND_COMBO_LEN                            (16) /* Each band Combo max length */
#define MIPC_OMADM_VALUE_LEN                               (64) /* Maximum OMA-DM configured value length */
#define MIPC_MAX_SYS_THERMAL_ACTUATOR_NAME                 (40) /* maximum length of thermal actuator name */
#define MIPC_MAX_ALPHAID_LEN                               (128) /* Maximum length of alphaid */
#define MIPC_MAX_SUB_ADDRESS_LEN                           (128) /* Maximum length of subaddress */
#define MIPC_MAX_CHANGE_BARRING_PWD_LEN                    (9) /* Maximum length of password in change barring request */
#define MIPC_MAX_SYS_STR_LEN                               (256) /* Maximum length of string in run GBA request */
#define MIPC_MAX_LCE_THRESHOLD_NUMBER                      (60) /* Maximum number(sizeof(int) * 15 = 60 bytes) of LCE threshold */
#define MIPC_MAX_TIME_LEN                                  (16) /* Maximum length of string in data TIME_IND */
#define MIPC_MAX_SET_LOCATION_LEN                          (128) /* Maximum length of string in set location info */
#define MIPC_MAX_DATA_SNSSAI_LEN                           (16) /*  */
#define MIPC_MAX_SPN_LEN                                   (71) /* Maximum length of SPN */
#define MIPC_MAX_GID1_NUM                                  (41) /* Maximum gid1 num */
#define MIPC_MAX_IMPI_LEN                                  (149) /* Maximum length of  IMPI */
#define MIPC_MAX_SS_ECMCCSS_RAW_LEN                        (256) /* Maximum length of ss ecmccss raw string */
#define MIPC_MAX_XCAP_USER_AGENT_STR_LEN                   (128) /* Maximum length of setup xcap user agent string */
#define MIPC_MAX_SS_XUI_INFO_LEN                           (512) /* Maximum length of ss xui info */
#define MIPC_MAX_SS_COMM_INFO_LEN                          (128) /* Maximum length of ss comm info */
#define MIPC_MAX_SS_USSD_LANG_LEN                          (32) /* Maximum length of ss ussd lang */
#define MIPC_MAX_IMS_STRING_INFO_LEN                       (128) /* Maximum length of ims string info */
#define MIPC_MAX_GRP_ID_NUM                                (10) /* Maximum number of grp list. */
#define MIPC_MAX_CHANNEL_LOCK_NUM                          (32) /* Maximum number of grp list. */
#define MIPC_MAX_SYS_THERMAL_SENSOR_NUM                    (8) /* max_sys_thermal_sensor_num */
#define MIPC_MAX_SYS_THERMAL_ACTUATOR_NUM                  (32) /* max_sys_thermal_actuator_num */
#define MIPC_MAX_SYS_THERMAL_TRIP_POINT_NUM_IN_ONE_SENSOR  (16) /* max_sys_thermal_trip_point_num_in_one_sensor */
#define MIPC_MAX_SYS_THERMAL_SENSOR_ALARM_SETTING_NUM      (4) /* One sensor has two alarms. MIPC commond support maximum set four alarms per MIPC command. */
#define MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM              (15) /* Maximum number of physical channel configs list. */
#define MIPC_MAX_CELL_ID_LEN                               (32) /* Max cell id len */
#define MIPC_MAX_NWSCN_RECORD_NUM                          (8) /* Max network scan record num */
#define MIPC_MAX_NWSCN_PLMN_NUM                            (20) /* Max network scan plmn num */
#define MIPC_MAX_NWSCN_RESULT_NUM                          (20) /* Max network scan result num */
#define MIPC_MAX_MCCMNC_LEN                                (4) /* Max mcc/mnc length */
#define MIPC_MAX_PLMN_CAG_INFO_NUM                         (1920) /* Maximum number of cag configs list. */
#define MIPC_UE_OS_ID_LEN                                  (16) /* According to RFC 4122,OS_ID is defined as UUID(Universally Unique IDentifier)with length 128bits(16bytes) */
#define MIPC_MAX_UE_OS_ID_NUM                              (15) /* According to 24.501 D.6.6,UE could carry maximum 15 OS ID to NW */
#define MIPC_MAX_CELL_INFO_NUM                             (33) /* Maximum number of cell info. */
#define MIPC_MAX_UMTS_SERVICE_NUM                          (4) /* Maximum nember of UMTS service cell */
#define MIPC_MAX_BAND_COMBO_NUM                            (600) /* Maximun number of up band combo information + down band combo information */
#define MIPC_MAX_TUW_NUM                                   (3) /* Maximun number of TUW information */
#define MIPC_MAX_SIM_APP_NUM                               (20) /*  */
#define MIPC_MAX_SIM_MSISDN_NUM                            (7) /*  */
#define MIPC_FIXED_SIM_SML_CATEGORY_CAT_SIZE               (7) /*  */
#define MIPC_MAX_FILTER_LIST_NUM                           (16) /* Maximum number of packet filter list */
#define MIPC_MAX_PCO_LIST_NUM                              (20) /* Maximum number of protocol configuration options list */
#define MIPC_MAX_IPV4_NUM                                  (4) /* Maximum number of IPv4 address */
#define MIPC_MAX_IPV6_NUM                                  (4) /* Maximum number of IPv6 address */
#define MIPC_MAX_DMF_RAWDATA_LEN                           (2048) /* Maximum length of DMF raw data. */
#define MIPC_MAX_DMF_DATA_NUM                              (10) /* Maximum number of DMF data. */
#define MIPC_MAX_SYS_DMF_INFO_LEN                          (16384) /* Maximum length of sys dmf info ind */
#define MIPC_MAX_EDRX_ACT_NUM                              (6) /* Maximum number of eDRX access technology. */
#define MIPC_MAX_LTE_CC_MEAS_INFO_NUM                      (5) /* Maximum lte_cc_meas_info count in lte_cc_meas_list */
#define MIPC_MAX_NR_CC_MEAS_INFO_NUM                       (8) /* Maximum nr_cc_meas_info count in nr_cc_meas_list */
#define MIPC_MAX_RAN_CELL_SIZE                             (8) /* Maximum lte/nr spectral info count in spectra_list */
#define MIPC_MAX_PLMN_NUM                                  (12) /* Maximum PLMN number of PLMN array */
#define MIPC_MAX_MCIF_LEN                                  (3072) /* Maximum MCIF payload length */
#define MIPC_MAX_RAW_SIGNAL_NUM                            (2) /* Maximum raw signal number */
#define MIPC_MAX_SERVING_CELL_NUM                          (32) /* Maximum serving cell number */
#define MIPC_MAX_PACKET_FILTER_NUM                         (16) /* Maximum packet filter list number */
#define MIPC_MAX_IA_LIST_NUM                               (3) /* Maximum initial attach list number */
#define MIPC_MAX_EMSLU_SESSION_NUM                         (435) /* Maximum MBMS sessions list number */
#define MIPC_MAX_SAI_LIST_NUM                              (16) /* Maximum service area identifier list number */
#define MIPC_MAX_UINT8_VALUE                               (0xFF) /* Maximum value of uint8 */
#define MIPC_MAX_UINT16_VALUE                              (0xFFFF) /* Maximum value of uint16 */
#define MIPC_MAX_UINT32_VALUE                              (0xFFFFFFFF) /* Maximum value of uint32 */
#define MIPC_MAX_MSISDN_COUNT                              (7) /* Maximum size of MSISDN TLV array */
#define MIPC_MAX_SIM_CARRIER_NUM                           (25) /* Maximum number of the carriers */
#define MIPC_MAX_SID_NID_NUM                               (20) /* Maximum number of SID_NID_TLV_ARRAY */
#define MIPC_MAX_SIM_FILE_NUM                              (10) /* Maximum number of SIM files */
#define MIPC_MAX_CALL_WAITING_NUM                          (7) /* Maximum call waiting number */
#define MIPC_MAX_CALL_FORWARD_NUM                          (10) /* Maximum call forward number */
#define MIPC_MAX_SWITCH_SIM_MAPPING_NUM                    (16) /* Maximum switch SIM mapping list number */
#define MIPC_MAX_CAG_INFO_LEN                              (67) /* Maximum length of CAG info */
#define MIPC_MAX_CID_NUM                                   (50) /* Maximum cid number */
#define MIPC_MAX_MBS_SERVICE_SESSION_INFO_LENGTH           (128) /* Maximum length of service session info */
#define MIPC_MAX_MBS_CELL_NUM                              (128) /* Maximum length of mbs cell */
#define MIPC_MAX_TSN_PMIC_LEN                              (8192) /* Maximum length of TSN PMIC */
#define MIPC_MAX_BC_SRC_LEN                                (35) /* Maximum length of Band Combination string, include ending character */
#define MIPC_MAX_BC_LIST_COUNT                             (1000) /* Maximum size of total Band Combinations */
#define MIPC_MAX_I2C_DATA_LEN                              (256) /* Maximum I2C data length */
#define MIPC_MAX_IDC_FRAME_CFG_PERIOD_NUM                  (40) /* Maximum number of IDC frame configuration period. */
#define MIPC_MAX_IDC_SCAN_FREQ_NUM                         (32) /* Maximum number of scan frequency. */
#define MIPC_MAX_SIM_PSISMSC_LEN                           (256) /* Maximum SIM PSISMSC length */
#define MIPC_MAX_SIM_RECORD_LEN                            (20) /* Maximum SIM record number */
#define MIPC_MAX_SIM_SMSP_LEN                              (64) /* Maximum SIM SMSP length */
#define MIPC_MAX_SIM_SERVICE_TABLE_RESP_LEN                (256) /* Maximum service table size */
#define MIPC_MAX_SIM_GID_RESP_LEN                          (20) /* Maximum GID length */
#define MIPC_MAX_SIM_IMPI_RESP_LEN                         (256) /* Maximum IMPI length */
#define MIPC_MAX_SIM_IMPU_RESP_LEN                         (256) /* Maximum IMPU length */
#define MIPC_MAX_SIM_DOMAIN_RESP_LEN                       (256) /* Maximum DOMAIN length */
#define MIPC_MAX_SIM_PCSCF_RESP_LEN                        (256) /* Maximum PCSCF length */
#define MIPC_NW_FTAI_LIST_MAX_SIZE                         (40) /* Maximum ftai_info count in ftai_list */
#define MIPC_MAX_READ_COEX_UART_LEN                        (128) /* Maximum length read data from coex uart */
#define MIPC_MAX_WRITE_COEX_UART_LEN                       (8) /* Maximum length write data to coex uart */
#define MIPC_MAX_BAND_POWER_SIZE                           (10) /* Maximum band&power list size */
#define MIPC_MAX_NVRAM_DATA_LEN                            (64000) /* Maximum NVRAM data length */
#define MIPC_MAX_TRACK_INUSE_NUM                           (256) /* Maximum number of inused track item */
#define MIPC_MAX_CONGESTION_CFG_SIZE                       (12) /* Maximum congestion config list size */
#define MIPC_MAX_PLMN_NAME_LEN                             (512) /* Maximum PLMN Name length */
#define MIPC_FIX_DOUBLE_LEN                                (8) /*  */
#define MIPC_FIX_FLOAT_LEN                                 (4) /*  */
#define MIPC_FIX_UNSIGNED_LONG_LONG_LEN                    (8) /*  */
#define MIPC_FIX_LONG_LONG_LEN                             (8) /*  */
#define MIPC_MAX_GNSS_DATA_LEN                             (5120) /*  */
#define MIPC_MAX_PMTK_LEN                                  (512) /*  */
#define MIPC_MAX_LOCATION_EM_DATA_SIZE                     (1000) /*  */
#define MIPC_MAX_GNSS_DEBUG_SIZE                           (1500) /*  */
#define MIPC_MAX_LBS_CONFIG_RAW_DATA_SIZE                  (4300) /*  */
#define MIPC_MAX_LPPE_MSG_DATA_LEN                         (4300) /*  */
#define MIPC_MAX_LBS_FRAMEWORK_DATA_LEN                    (5120) /*  */
#define MIPC_MAX_GNSS_BIG_DATA_LEN                         (32000) /*  */
#define MIPC_MAX_AGPS_REQUESTOR_ID_LEN                     (512) /*  */
#define MIPC_MAX_AGPS_CLIENT_NAME_LEN                      (512) /*  */
#define MIPC_VOLTE_MAX_TLS_ID_LENGTH                       (256) /* DTLS association ID, tls-id-value = 20*(tls-id-char)  [RFC 8842] */
#define MIPC_VOLTE_MAX_DCMAP_NUM_PER_CONFIG                (10) /* max DCMAP numbers per config */
#define MIPC_VOLTE_MAX_BDC_CONFIG_NUM                      (2) /* MAX Bootstrap DC config number */
#define MIPC_VOLTE_MAX_BDC_DCMAP_NUM                       (20) /* VOLTE_MAX_BDC_CONFIG_NUM*VOLTE_MAX_DCMAP_NUM_PER_CONFIG */
#define MIPC_VOLTE_MAX_ADC_CONFIG_NUM                      (4) /* MAX Application DC config number */
#define MIPC_VOLTE_MAX_ADC_DCMAP_NUM                       (40) /* VOLTE_MAX_ADC_CONFIG_NUM*VOLTE_MAX_DCMAP_NUM_PER_CONFIG */
#define MIPC_VOLTE_MAX_DC_CONFIG_NUM                       (6) /* 1 BDC + 2 UE-Network ADC (GBR, non-GBR) + 2 UE-UE ADC (GBR, non-GBR) + 1 reserved */
#define MIPC_VOLTE_MAX_DC_DCMAP_NUM                        (60) /* VOLTE_MAX_DC_CONFIG_NUM*VOLTE_MAX_DCMAP_NUM_PER_CONFIG */
#define MIPC_VOLTE_IPADDR_LENGTH                           (16) /* IP address length */
#define MIPC_VOLTE_MAX_IF_NAME_LENGTH                      (16) /* interface name is used by AP to get the network_id */
#define MIPC_VOLTE_MAX_FINGERPRINT_LENGTH                  (192) /* Secure Hash Standard (SHS) */
#define MIPC_VOLTE_MAX_QOS_HINT_LENGTH                     (64) /* REF: 3GPP TS 26.114, Example: a=3gpp-qos-hint: loss=0.00001;latency=300 */
#define MIPC_VOLTE_MAX_DCMAP_LABEL_LENGTH                  (80) /* Max string length for label of dcmap */
#define MIPC_VOLTE_MAX_DCMAP_SUBPROTOCOL_LENGTH            (10) /* Max string length for subprotocol of dcmap */

#pragma pack (push)
#pragma pack (1)

typedef struct mipc_v4_full_addr_struct4 {
    uint32_t                                           mtu; /* 0 means not present */
    uint8_t                                            addr[4]; /* N/A */
    uint8_t                                            mask[4]; /* N/A */
} mipc_v4_full_addr_struct4 ;

typedef struct mipc_v6_full_addr_struct4 {
    uint32_t                                           mtu; /* 0 means not present */
    uint8_t                                            addr[16]; /* N/A */
    uint32_t                                           prefix; /* N/A */
} mipc_v6_full_addr_struct4 ;

typedef struct mipc_addr_struct4 {
    uint32_t                                           addr_len; /* N/A */
    uint8_t                                            addr[16]; /* N/A */
} mipc_addr_struct4 ;

typedef struct mipc_full_addr_struct4 {
    uint32_t                                           addr_len; /* N/A */
    uint8_t                                            addr[16]; /* N/A */
    uint32_t                                           mtu; /* 0 means not present */
    uint8_t                                            mask[4]; /* N/A */
    uint32_t                                           prefix; /* N/A */
} mipc_full_addr_struct4 ;

typedef struct mipc_apn_ia_struct4 {
    char                                               apn[MIPC_MAX_APN_LEN]; /* Access Point Name */
    uint32_t                                           apn_idx; /* APN index */
    char                                               userid[MIPC_MAX_USERID_LEN]; /* Authentication username */
    char                                               password[MIPC_MAX_PASSWORD_LEN]; /* Authentication password */
    uint32_t                                           bearer_bitmask; /* RAT bitmask:
            bit0 : Unknown
            bit14 : LTE
            bit19 : LTE_CA
            bit20 : NR
 */
    uint8_t                                            pdp_type; /* The IP address type */
    uint8_t                                            roaming_type; /* The IP address type for roaming */
    uint8_t                                            auth_type; /* Authentication type for the data call */
    uint8_t                                            compression; /* Compression */
} mipc_apn_ia_struct4 ;

typedef struct mipc_apn_profile_struct4 {
    uint32_t                                           id; /* APN profile ID */
    uint8_t                                            plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* PLMN of operator network */
    uint8_t                                            reserved; /* Reserved */
    char                                               apn[MIPC_MAX_APN_LEN]; /* Access Point Name */
    uint32_t                                           apn_idx; /* APN index */
    char                                               userid[MIPC_MAX_USERID_LEN]; /* Authentication username */
    char                                               password[MIPC_MAX_PASSWORD_LEN]; /* Authentication password */
    uint32_t                                           bearer_bitmask; /* RAT bitmask:
            bit0 : Unknown
            bit14 : LTE
            bit19 : LTE_CA
            bit20 : NR
 */
    uint32_t                                           apn_type; /* The data connection type for APN */
    uint8_t                                            pdp_type; /* The IP address type */
    uint8_t                                            roaming_type; /* The IP address type for roaming */
    uint8_t                                            auth_type; /* Authentication type for the data call */
    uint8_t                                            compression; /* Compression */
    uint8_t                                            reserve1; /* Reserved for implementing field */
    uint8_t                                            reserve2; /* Reserved for implementing field */
    uint8_t                                            reserve3; /* Reserved for implementing field */
    uint8_t                                            enabled; /* It indicates whether APN profile is enabled or not; MAX is not set */
} mipc_apn_profile_struct4 ;

typedef struct mipc_apn_profile_v2_struct4 {
    uint32_t                                           id; /* APN profile ID, 0x7FFFFFFF is the invalid value indicating that host does not need profile ID */
    uint8_t                                            plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* PLMN of operator network */
    uint8_t                                            reserved; /* Reserved */
    char                                               apn[MIPC_MAX_APN_LEN]; /* Access Point Name */
    uint32_t                                           apn_idx; /* It indicates different APNs which have same APN; 0x7FFFFFFF is the invalid value indicating that the APN index is maintained by modem (not by host) */
    char                                               userid[MIPC_MAX_USERID_LEN]; /* Authentication username */
    char                                               password[MIPC_MAX_PASSWORD_LEN]; /* Authentication password */
    uint32_t                                           bearer_bitmask; /* RAT bitmask:
            bit0 : Unknown
            bit14 : LTE
            bit19 : LTE_CA
            bit20 : NR
 */
    uint32_t                                           apn_type; /* The data connection type for APN */
    uint8_t                                            pdp_type; /* The IP address type */
    uint8_t                                            roaming_type; /* The IP address type for roaming */
    uint8_t                                            auth_type; /* Authentication type for the data call */
    uint8_t                                            compression; /* Compression */
    uint8_t                                            enabled; /* It indicates whether APN profile is enabled or not; MAX is not set */
    uint8_t                                            padding[3]; /* Padding */
    uint32_t                                           max_conns; /* It indicates max connection count for OP12 APN */
    uint32_t                                           max_conn_t; /* It indicates max connection time for OP12 APN */
    uint32_t                                           wait_time; /* It indicates wait time for OP12 APN */
    uint32_t                                           inact_timer; /* It indicates inact timer for Sprint APN */
    uint32_t                                           v4_mtu; /* IPv4 MTU, 0x7FFFFFFF is the invalid value indicating that host does not need to set MTU */
    uint32_t                                           v6_mtu; /* IPv6 MTU, 0x7FFFFFFF is the invalid value indicating that host does not need to set MTU */
} mipc_apn_profile_v2_struct4 ;

typedef struct mipc_md_apn_profile_struct4 {
    uint32_t                                           id; /* APN profile ID */
    uint8_t                                            plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* PLMN of operator network */
    uint8_t                                            act_state; /* APN activated state */
    char                                               apn[MIPC_MAX_APN_LEN]; /* Access Point Name */
    uint32_t                                           apn_idx; /* APN index */
    char                                               userid[MIPC_MAX_USERID_LEN]; /* Authentication username */
    char                                               password[MIPC_MAX_PASSWORD_LEN]; /* Authentication password */
    uint32_t                                           bearer_bitmask; /* N/A */
    uint32_t                                           apn_type; /* The data connection types for APN */
    uint8_t                                            pdp_type; /* The IP address type */
    uint8_t                                            roaming_type; /* The IP address type for roaming */
    uint8_t                                            auth_type; /* Authentication type for the data call */
    uint8_t                                            reserve; /* Reserved */
} mipc_md_apn_profile_struct4 ;

typedef struct mipc_op12_apn_profile_legacy_struct4 {
    char                                               apn[MIPC_MAX_APN_LEN]; /* apn name */
    char                                               apnb[MIPC_MAX_APN_BEARER_LEN]; /* apn bearer */
    uint8_t                                            pdp_type; /* pdp type (IPv4, IPv6, IPv4v6 at home) */
    uint8_t                                            id; /* ID */
    uint8_t                                            apnclass; /* APN class */
    uint8_t                                            enabled; /* apn is enabled or not, MAX is not set */
    uint8_t                                            max_conn; /* max connection count */
    uint8_t                                            max_conn_t; /* max connection  */
    uint32_t                                           wait_time; /* wait time by seconds */
    uint8_t                                            act_state; /* APN activated state */
    uint8_t                                            reserve1; /* reserve */
    uint8_t                                            reserve2; /* reserve */
    uint8_t                                            reserve3; /* reserve */
} mipc_op12_apn_profile_legacy_struct4 ;

typedef mipc_op12_apn_profile_legacy_struct4 mipc_vzw_apn_profile_struct4;

typedef struct mipc_op12_apn_profile_struct4 {
    char                                               apn[MIPC_MAX_APN_LEN]; /* apn name */
    char                                               apn_bear[MIPC_MAX_APN_BEARER_LEN]; /* apn bearer */
    uint8_t                                            pdp_type; /* pdp type (IPv4, IPv6, IPv4v6 at home) */
    uint8_t                                            act_state; /* APN activated state */
    uint32_t                                           id; /* ID */
    uint32_t                                           apn_class; /* APN class */
    uint32_t                                           enabled; /* apn is enabled or not, MAX is not set */
    uint32_t                                           max_conn; /* max connection count */
    uint32_t                                           max_conn_t; /* max connection time */
    uint32_t                                           wait_time; /* wait time by seconds */
} mipc_op12_apn_profile_struct4 ;

typedef mipc_op12_apn_profile_struct4 mipc_verizon_apn_profile_struct4;

typedef struct mipc_data_nitz_info_struct4 {
    uint32_t                                           year; /* Year as an integer. */
    uint32_t                                           month; /* N/A */
    uint32_t                                           day; /* N/A */
    uint32_t                                           hour; /* N/A */
    uint32_t                                           minute; /* N/A */
    uint32_t                                           second; /* N/A */
} mipc_data_nitz_info_struct4 ;

typedef struct mipc_data_v4_addr_struct4 {
    uint8_t                                            addr[4]; /* N/A */
} mipc_data_v4_addr_struct4 ;

typedef struct mipc_data_v6_addr_struct4 {
    uint8_t                                            addr[16]; /* N/A */
} mipc_data_v6_addr_struct4 ;

typedef struct mipc_data_pco_ie_struct4 {
    char                                               ie[5]; /* It indicates protocol ID, refer to 3GPP 24.008 clause 10.5.6.3 */
    uint8_t                                            reserved[3]; /* Reserved */
    char                                               content[80]; /* It indicates protocol ID contents, refer to  3GPP 24.008 clause 10.5.6.3 */
} mipc_data_pco_ie_struct4 ;

typedef struct mipc_tmgi_struct_struct4 {
    uint8_t                                            plmn_id[8]; /* PLMN consists of MCC and MNC, Only the first 6 bytes are used for MCC and MNC, the last 2 is padding.
            If PLMN length is 5 ( < 6 ), please set 0x0F for plmn_id[5], 0x0F is invalid value.
 */
    uint8_t                                            service_id[8]; /* Only the first 3 bytes are used for service_id, the last 5 is padding */
} mipc_tmgi_struct_struct4 ;

typedef struct mipc_mbs_session_info_struct4 {
    uint16_t                                           op; /* It indicates to join or leave MBS session */
    uint16_t                                           sesson_id_type; /* It indicates the type of MBS session ID */
    mipc_tmgi_struct_struct4                           tmgi; /* TMGI(Temporary Mobile Group Identity) information */
    uint8_t                                            src_ip_addr[16]; /* First 4 bytes of ipv4 */
    uint8_t                                            dest_ip_addr[16]; /* First 4 bytes of ipv4 */
} mipc_mbs_session_info_struct4 ;

typedef struct mipc_mbs_session_update_ind_struct4 {
    mipc_tmgi_struct_struct4                           tmgi; /* TMGI(Temporary Mobile Group Identity) information */
    uint16_t                                           psi; /* PDU session ID */
    uint16_t                                           ip_addr_present; /* It indicates whether source IP and destination IP are present. */
    uint8_t                                            src_ip_addr[16]; /* First 4 bytes of ipv4 */
    uint8_t                                            dest_ip_addr[16]; /* First 4 bytes of ipv4 */
    uint16_t                                           mbs_decison; /* It indicates whether MBS join is accepted or rejected */
    uint16_t                                           reject_cause; /* It indicates MBS rejected cause from netowrk(3GPP 24501) */
    uint16_t                                           mbs_sec_container_raw_present; /* It indicates whether MBS second container raw data is present. */
    uint8_t                                            mbs_sec_container_raw[38]; /* MBS second container raw data */
} mipc_mbs_session_update_ind_struct4 ;

typedef struct mipc_cell_global_id_struct_struct4 {
    uint8_t                                            plmn_id[8]; /* PLMN consists of MCC and MNC, Only the first 6 bytes are used for MCC and MNC, the last 2 is padding.
            If PLMN length is 5 ( < 6 ), please set 0x0F for plmn_id[5], 0x0F is invalid value.
 */
    uint8_t                                            cell_id[8]; /* Only the first 5 bytes are used for cell ID, the last 3 is padding. */
} mipc_cell_global_id_struct_struct4 ;

typedef struct mipc_tac_struct_struct4 {
    uint8_t                                            plmn_id[8]; /* PLMN consists of MCC and MNC, Only the first 6 bytes are used for MCC and MNC, the last 2 is padding.
            If PLMN length is 5 ( < 6 ), please set 0x0F for plmn_id[5], 0x0F is invalid value.
 */
    uint8_t                                            ta_code[8]; /* Only the first 5 bytes are used for tracking area code, the last 3 is padding. */
} mipc_tac_struct_struct4 ;

typedef struct mipc_data_packet_filter_struct4 {
    uint8_t                                            len; /* Length of packet filter */
    uint8_t                                            reserved[3]; /* Reserved */
    uint8_t                                            pattern[192]; /* Pattern of packet filter indicating which packet's pattern will be sent to host */
    uint8_t                                            mask[192]; /* It indicates mask of filter; for example, if (pattern & mask) == (filter & mask), this packet will be sent to host */
} mipc_data_packet_filter_struct4 ;

typedef struct mipc_data_secondary_pdp_context_info_struct4 {
    uint8_t                                            cid; /* ID indicating PDN connection */
    uint8_t                                            p_cid; /* Primary CID of same APN */
    uint8_t                                            bearer_id; /* Network ESM/5GSM bearer ID */
    uint8_t                                            IM_CN_Signalling_Flag; /* IM CN signalling flag from network; it indicates EPSbearer dedicated for IMS signalling, refer to 3GPP 23.228 annex E.2.1.2 */
    uint8_t                                            WLAN_Offload; /* It indicates whether traffic can be offloaded via a WLAN or not, refer to 3GPP 24.008 clause 10.5.6.20 */
    uint8_t                                            PDU_session_id; /* PDU session ID for NR; refer to 3GPP 24.501 */
    uint16_t                                           QFI; /* QoS Flow Identifier for NR; refer to 3GPP 24.501 */
} mipc_data_secondary_pdp_context_info_struct4 ;

typedef struct mipc_data_qos_info_struct4 {
    uint16_t                                           cid; /* Context identifier */
    uint16_t                                           QCI; /* QoS Flow Identifier; the value of 0 means QCI is selected by network; the other values can refer to 3GPP 23.203 clause 6.1.7.2 */
    uint32_t                                           DL_GBR; /* Downlink Guaranteed Bit Rate of data connection */
    uint32_t                                           UL_GBR; /* Uplink Guaranteed Bit Rate of data connection */
    uint32_t                                           DL_MBR; /* Downlink Maximum Bit Rate of data connection */
    uint32_t                                           UL_MBR; /* Uplink Maximum Bit Rat of data connection */
    uint32_t                                           DL_AMBR; /* Downlink Aggregate Maximum Bit Rate of data connection */
    uint32_t                                           UL_AMBR; /* Uplink Aggregate Maximum Bit Rate of data connection */
} mipc_data_qos_info_struct4 ;

typedef struct mipc_data_5gqos_info_struct4 {
    uint16_t                                           cid; /* Context identifier */
    uint16_t                                           VQI; /* 5QI, 5G QoS Indication */
    uint64_t                                           DL_GFBR; /* Downlink Guaranteed Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           UL_GFBR; /* Uplink Guaranteed Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           DL_MFBR; /* Downlink Maximum Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           UL_MFBR; /* Uplink Maximum Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           DL_SAMBR; /* Downlink session Aggregate Maximum Bit Rate; the unit is kbit/s */
    uint64_t                                           UL_SAMBR; /* Uplink session Aggregate Maximum Bit Rate; the unit is kbit/s */
    uint32_t                                           Averaging_window; /* It represents the duration over which the Guaranteed Flow Bit Rate (GFBR) shall be calculated; the unit is milliseconds */
} mipc_data_5gqos_info_struct4 ;

typedef struct mipc_data_tft_info_struct4 {
    uint8_t                                            cid; /* Context identifier */
    uint8_t                                            packet_filter_identifier; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when packet_filter_identifier_present = True this value is valid. */
    uint8_t                                            evaluation_precedence_index; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when evaluation_precedence_index_present = True this value is valid. */
    uint8_t                                            remote_v4_addr_present; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v6_addr_present; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            protocol_number_next_header; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when protocol_number_next_header_present = True this value is valid. */
    uint8_t                                            tos_traffic_class; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when tos_traffic_class_and_mask_present = True this value is valid. */
    uint8_t                                            tos_mask; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            direction; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when direction_present = True this value is valid. */
    uint8_t                                            NW_packet_filter_identifier; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when NW_packet_filter_Identifier_present = True this value is valid. */
    uint8_t                                            local_v4_addr_present; /* It indicates whether local_v4_addr is present or not. */
    uint8_t                                            local_v6_addr_present; /* It indicates whether local_v6_addr is present or not. */
    uint8_t                                            remote_v4_addr[4]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v4_subnet_mask[4]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v6_addr[16]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v6_subnet_mask[16]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           local_port_low; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           local_port_high; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           remote_port_low; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           remote_port_high; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint32_t                                           ipsec_spi; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when ipsec_spi_present = True this value is valid. */
    uint32_t                                           flow_label; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when flow_label_present = True this value is valid. */
    uint8_t                                            local_v4_addr[4]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            local_v4_subnet_mask[4]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            local_v6_addr[16]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            local_v6_subnet_mask[16]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint32_t                                           QRI; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when QRI_present = True this value is valid. */
    uint16_t                                           dest_MAC_addr_present; /* It indicates whether dest_MAC_addr is present or not. */
    uint16_t                                           src_MAC_addr_present; /* It indicates whether src_MAC_addr is present or not. */
    uint8_t                                            dest_MAC_addr[6]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            src_MAC_addr[6]; /* Follow 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           C_TAG_VID; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when C_TAG_VID_present = True this value is valid. */
    uint16_t                                           S_TAG_VID; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when S_TAG_VID_present = True this value is valid. */
    uint8_t                                            C_TAG_PCP_DEI; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when C_TAG_PCP_DEI_present = True this value is valid. */
    uint8_t                                            S_TAG_PCP_DEI; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when S_TAG_PCP_DEI_present = True this value is valid. */
    uint16_t                                           ethertype; /* Follow 3GPP definition in 27.007 subclause 10.1.25. Only when ethertype_present = True this value is valid. */
    uint8_t                                            packet_filter_identifier_present; /* It indicates whether packet_filter_identifier is present or not. */
    uint8_t                                            evaluation_precedence_index_present; /* It indicates whether evaluation_precedence_index is present or not. */
    uint8_t                                            protocol_number_next_header_present; /* It indicates whether protocol_number_next_header is present or not. */
    uint8_t                                            ipsec_spi_present; /* It indicates whether ipsec_spi_present is present or not. */
    uint8_t                                            tos_traffic_class_and_mask_present; /* It indicates whether tos_traffic_class_and_mask is present or not. */
    uint8_t                                            flow_label_present; /* It indicates whether flow_label is present or not. */
    uint8_t                                            direction_present; /* It indicates whether direction is present or not. */
    uint8_t                                            NW_packet_filter_Identifier_present; /* It indicates whether NW_packet_filter_Identifier is present or not. */
    uint8_t                                            QRI_present; /* It indicates whether QRI is present or not. */
    uint8_t                                            C_TAG_VID_present; /* It indicates whether C_TAG_VID is present or not. */
    uint8_t                                            S_TAG_VID_present; /* It indicates whether S_TAG_VID is present or not. */
    uint8_t                                            C_TAG_PCP_DEI_present; /* It indicates whether C_TAG_PCP_DEI is present or not. */
    uint8_t                                            S_TAG_PCP_DEI_present; /* It indicates whether S_TAG_PCP_DEI is present or not. */
    uint8_t                                            ethertype_present; /* It indicates whether ethertype is present or not. */
    uint8_t                                            padding[2]; /* N/A */
} mipc_data_tft_info_struct4 ;

typedef struct mipc_data_tft_info_v1_struct4 {
    uint8_t                                            cid; /* Context identifier */
    uint8_t                                            packet_filter_identifier_present; /* It indicates whether packet_filter_identifier is present or not. */
    uint8_t                                            packet_filter_identifier; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when packet_filter_identifier_present = True this value is valid. */
    uint8_t                                            evaluation_precedence_index_present; /* It indicates whether evaluation_precedence_index is present or not. */
    uint8_t                                            evaluation_precedence_index; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when evaluation_precedence_index_present = True this value is valid. */
    uint8_t                                            remote_v4_addr_present; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v4_addr[4]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v4_subnet_mask[4]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v6_addr_present; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v6_addr[16]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            remote_v6_prefix[16]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            protocol_number_next_header_present; /* It indicates whether protocol_number_next_header is present or not. */
    uint8_t                                            protocol_number_next_header; /* Follows 3GPP definition in 27.007 subclause 10.1.25.  Only when protocol_number_next_header_present = True this value is valid. */
    uint8_t                                            padding1; /* padding. */
    uint16_t                                           local_port_low; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           local_port_high; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           remote_port_low; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           remote_port_high; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            ipsec_spi_present; /* It indicates whether ipsec_spi_present is present or not. */
    uint8_t                                            padding2; /* padding. */
    uint32_t                                           ipsec_spi; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when ipsec_spi_present = True this value is valid. */
    uint8_t                                            tos_traffic_class_and_mask_present; /* It indicates whether tos_traffic_class_and_mask is present or not. */
    uint8_t                                            tos_traffic_class; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when tos_traffic_class_and_mask_present = True this value is valid. */
    uint8_t                                            tos_mask; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when tos_traffic_class_and_mask_present = True this value is valid. */
    uint8_t                                            flow_label_present; /* It indicates whether flow_label is present or not. */
    uint32_t                                           flow_label; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when flow_label_present = True this value is valid. */
    uint8_t                                            direction_present; /* It indicates whether direction is present or not. */
    uint8_t                                            direction; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when direction_present = True this value is valid. */
    uint8_t                                            NW_packet_filter_Identifier_present; /* It indicates whether NW_packet_filter_Identifier is present or not. */
    uint8_t                                            NW_packet_filter_identifier; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when NW_packet_filter_Identifier_present = True this value is valid. */
    uint8_t                                            local_v4_addr_present; /* It indicates whether local_v4_addr is present or not. */
    uint8_t                                            local_v4_addr[4]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            local_v4_subnet_mask[4]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            local_v6_addr_present; /* It indicates whether local_v6_addr is present or not. */
    uint8_t                                            local_v6_addr[16]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            local_v6_prefix[16]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            QRI_present; /* It indicates whether QRI is present or not. */
    uint8_t                                            padding3; /* padding. */
    uint32_t                                           QRI; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when QRI_present = True this value is valid. */
    uint16_t                                           dest_MAC_addr_present; /* It indicates whether dest_MAC_addr is present or not. */
    uint8_t                                            dest_MAC_addr[6]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint16_t                                           src_MAC_addr_present; /* It indicates whether src_MAC_addr is present or not. */
    uint8_t                                            src_MAC_addr[6]; /* Follows 3GPP definition in 27.007 subclause 10.1.25. */
    uint8_t                                            C_TAG_VID_present; /* It indicates whether C_TAG_VID is present or not. */
    uint8_t                                            padding4; /* padding. */
    uint16_t                                           C_TAG_VID; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when C_TAG_VID_present = True this value is valid. */
    uint8_t                                            S_TAG_VID_present; /* It indicates whether S_TAG_VID is present or not. */
    uint8_t                                            padding5; /* padding. */
    uint16_t                                           S_TAG_VID; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when S_TAG_VID_present = True this value is valid. */
    uint8_t                                            C_TAG_PCP_DEI_present; /* It indicates whether C_TAG_PCP_DEI is present or not. */
    uint8_t                                            C_TAG_PCP_DEI; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when C_TAG_PCP_DEI_present = True this value is valid. */
    uint8_t                                            S_TAG_PCP_DEI_present; /* It indicates whether S_TAG_PCP_DEI is present or not. */
    uint8_t                                            S_TAG_PCP_DEI; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when S_TAG_PCP_DEI_present = True this value is valid. */
    uint8_t                                            ethertype_present; /* It indicates whether ethertype is present or not. */
    uint8_t                                            padding6; /* padding. */
    uint16_t                                           ethertype; /* Follows 3GPP definition in 27.007 subclause 10.1.25. Only when ethertype_present = True this value is valid. */
} mipc_data_tft_info_v1_struct4 ;

typedef struct mipc_data_start_keepalive_request_struct4 {
    uint32_t                                           type; /* Type of keep alive packet */
    uint32_t                                           sourcePort; /* Source port if applicable or 0x7FFFFFFF; the maximum value is 65535 */
    uint32_t                                           destinationPort; /* Destination port if applicable or 0x7FFFFFFF; the maximum value is 65535 */
    uint32_t                                           maxKeepaliveIntervalMillis; /* Maximum milliseconds between two packets */
    uint32_t                                           cid; /* Context identifier for starting keep alive */
    char                                               sourceAddress[68]; /* Source address in network-byte order */
    char                                               destinationAddress[68]; /* Destination address in network-byte order */
} mipc_data_start_keepalive_request_struct4 ;

typedef struct mipc_s_nssai_struct_struct4 {
    uint8_t                                            sst_present; /* It indicates whether SST is present or not */
    uint8_t                                            sst; /* Slice/Service Type, defined in 3GPP TS 23.501 clause 5.15.2.1
            1: Enhanced Mobile Broadband
            2: Ultra-reliable low latency communications
            3: Massive IoT
 */
    uint8_t                                            sd_present; /* It indicates whether SD is present or not */
    uint8_t                                            padding; /* N/A */
    uint32_t                                           sd; /* Slice Differentiator, optional information that complements the Slice/Service type(s) to differentiate amongst multiple Network Slices; it is defined in 3GPP TS 23.501 clause 5.15.2.1 */
    uint8_t                                            mapped_sst_present; /* It indicates whether the mapped SST is present or not */
    uint8_t                                            mapped_sst; /* Mapped Slice/Service Type */
    uint8_t                                            mapped_sd_present; /* It indicates whether the mapped SD is present or not */
    uint8_t                                            padding2; /* N/A */
    uint32_t                                           mapped_sd; /* Mapped Slice Differentiator */
} mipc_s_nssai_struct_struct4 ;

typedef struct mipc_ursp_route_sel_desc_struct_struct4 {
    uint8_t                                            p_val; /* The precedence value of Route Selection Descriptor */
    uint8_t                                            ssc_mode; /* The Session and Service Continuity Mode */
    uint8_t                                            dnn_len; /* The length of DNN */
    uint8_t                                            s_nssai_present; /* It indicates whether SST is present or not */
    mipc_s_nssai_struct_struct4                        s_nssai; /* S-NSSAI content, follow 3GPP definition in 24.501 subclause 9.11.2.8 */
    char                                               dnn[MIPC_MAX_APN_LEN]; /* dnn name, MAX length 100 */
    uint8_t                                            pdu_session_type; /* follow 3GPP definition, in subclause 9.11.4.11 of TS 3GPP 24.501 */
    uint8_t                                            pref_access_type; /* follow 3GPP definition, in subclause 9.11.2.1A of TS 3GPP 24.501 */
    uint8_t                                            padding2[2]; /* N/A */
} mipc_ursp_route_sel_desc_struct_struct4 ;

typedef struct mipc_ursp_traffic_desc_struct_struct4 {
    uint8_t                                            is_match_all; /* The traffic descriptor match-all type; 1: Match-all; 0: Not match-all */
    uint8_t                                            dnn_len; /* The length of DNN */
    uint8_t                                            reserved1[2]; /* Reserved */
    char                                               dnn[MIPC_MAX_APN_LEN]; /* Data Network Name */
    uint8_t                                            os_id[16]; /* OS UUID */
    uint8_t                                            app_id_len; /* The length of OS application ID */
    uint8_t                                            reserved2[3]; /* Reserved */
    uint8_t                                            app_id[255]; /* OS application ID */
    uint8_t                                            reserved3; /* Reserved */
    uint8_t                                            ipv4_addr[4]; /* IPv4 address */
    uint8_t                                            ipv4_mask[4]; /* IPv4 address mask */
    uint8_t                                            ipv6_addr[16]; /* IPv6 address */
    uint8_t                                            ipv6_prefix_len; /* IPv6 address prefix length */
    uint8_t                                            reserved4[3]; /* Reserved */
    uint16_t                                           port_num; /* The single remote port type to specify */
    uint16_t                                           min_port_num; /* The low limit for remote port range type */
    uint16_t                                           max_port_num; /* The high limit for remote port range type */
    uint8_t                                            prot_id_next_hdr; /* Protocol identifier/next header type; specify the IPv4 protocol identifier or IPv6 next header; as specification, this field should be 0xFF if it is not be specified */
    uint8_t                                            dst_fqdn_len; /* The length of destination FQDN */
    uint8_t                                            dst_fqdn[255]; /* The value of destination FQDN; it should be encoded as defined in 3GPP TS 23.003 clause 28.3.2.1 */
    uint8_t                                            padding; /* N/A */
} mipc_ursp_traffic_desc_struct_struct4 ;

typedef struct mipc_ursp_ue_local_conf_struct_struct4 {
    uint8_t                                            ssc_mode; /* The Session and Service Continuity Mode; refer to 3GPP TS 23.501 clause 5.6.9.2 */
    uint8_t                                            dnn_len; /* The length of DNN */
    uint8_t                                            reserved; /* Reserved */
    uint8_t                                            s_nssai_present; /* Specify whether S-NSSAI is present or not */
    mipc_s_nssai_struct_struct4                        s_nssai; /* The S-NSSAI contents, defined in 3GPP 24.501 clause 9.11.2.8 */
    char                                               dnn[MIPC_MAX_APN_LEN]; /* Data Network Name */
    uint8_t                                            pdu_session_type; /* It indicates type of PDU session; refer to 3GPP 24.501 clause 9.11.4.11 */
    uint8_t                                            pref_access_type; /* It indicates access type which user data is pending to be sent to the UE, refer to 3GPP 24.501 clause 9.11.2.1A */
    uint8_t                                            padding[2]; /* N/A */
} mipc_ursp_ue_local_conf_struct_struct4 ;

typedef struct mipc_ursp_get_route_supp_profile_ind_struct_struct4 {
    uint32_t                                           cid; /* PDN/PDU Context ID, uniquely identifies this call */
    uint32_t                                           supp_profile_type; /* type of supp profile, only REQ now */
    mipc_ursp_traffic_desc_struct_struct4              traffic_desc; /* traff descriptor in 3GPP URSP */
    mipc_ursp_ue_local_conf_struct_struct4             ue_local_conf; /* UE local configuration */
    uint8_t                                            is_match_all_disallowed; /* is match all or not, 1 means match all is disallowed, 0 means match all is allowed */
    uint8_t                                            padding[3]; /* N/A */
} mipc_ursp_get_route_supp_profile_ind_struct_struct4 ;

typedef struct mipc_ursp_rule_struct_struct4 {
    uint8_t                                            p_val; /* precedence of URSP rule */
    uint8_t                                            reserved1[3]; /* Reserved */
    mipc_ursp_traffic_desc_struct_struct4              traffic_desc; /* traff descriptor in 3GPP URSP */
    uint8_t                                            num_of_rsd; /* number of RSD */
    uint8_t                                            reserved2[3]; /* Reserved */
    mipc_ursp_route_sel_desc_struct_struct4            route_sel_desc_list[8]; /* RSD list */
} mipc_ursp_rule_struct_struct4 ;

typedef struct mipc_ursp_ue_policy_plmn_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
} mipc_ursp_ue_policy_plmn_struct4 ;

typedef struct mipc_plmn_specific_s_nssai_struct_struct4 {
    uint8_t                                            plmn_id[8]; /* PLMN, it consists of MCC and MNC;
            Only the first 6 length are used for mcc and mnc, the last 2 is padding
 */
    uint8_t                                            sst_present; /* It indicates whether SST is present or not */
    uint8_t                                            sst; /* Slice/Service Type, defined in 3GPP TS 23.501 clause 5.15.2.1
            1: Enhanced Mobile Broadband
            2: Ultra-reliable low latency communications
            3: Massive IoT
 */
    uint8_t                                            sd_present; /* It indicates whether SD is present or not */
    uint8_t                                            padding; /* N/A */
    uint32_t                                           sd; /* Slice Differentiator, optional information that complements the Slice/Service type(s) to differentiate amongst multiple Network Slices; it is defined in 3GPP TS 23.501 clause 5.15.2.1 */
    uint8_t                                            mapped_sst_present; /* It indicates whether the mapped SST is present or not */
    uint8_t                                            mapped_sst; /* Mapped Slice/Service Type */
    uint8_t                                            mapped_sd_present; /* It indicates whether the mapped SD is present or not */
    uint8_t                                            padding2; /* N/A */
    uint32_t                                           mapped_sd; /* Mapped Slice Differentiator in 3GPP */
} mipc_plmn_specific_s_nssai_struct_struct4 ;

typedef struct mipc_rejected_s_nssai_struct_struct4 {
    uint8_t                                            cause_value; /* The rejected cause of S-NSSAI
            0: S-NSSAI not available in the current PLMN or SNPN
            1: S-NSSAI not available in the current registration area
            2: S-NSSAI not available due to the failed or revoked network slicepecific authentication and authorization
 */
    uint8_t                                            sst_present; /* It indicates SST is present or not */
    uint8_t                                            sst; /* Slice/Service Type, defined in 3GPP TS 23.501 clause 5.15.2.1
            1: Enhanced Mobile Broadband
            2: Ultra-reliable low latency communications
            3: Massive IoT
 */
    uint8_t                                            sd_present; /* It indicates SD is present or not */
    uint32_t                                           sd; /* Slice Differentiator, optional information that complements the Slice/Service type(s) to differentiate amongst multiple Network Slices; it is defined in 3GPP TS 23.501 clause 5.15.2.1 */
} mipc_rejected_s_nssai_struct_struct4 ;

typedef struct mipc_lte_cc_meas_info_struct4 {
    char                                               lte_cc_type[12]; /* Indicates the type of cell */
    uint16_t                                           path_loss; /* Indicates the measurement result of path loss, unit in dBm, 0~255 */
    uint16_t                                           rank_indicator; /* Indicates the measurement result of rank indicator; n: rank n; 0 is invalid */
    int32_t                                            serv_rsrp; /* Indicates RSRP of serving cell (see 3GPP TS 36.133 subclause 9.1.4). */
    int32_t                                            serv_rsrq; /* Indicates RSRQ of serving cell (see 3GPP TS 36.133 subclause 9.1.7). */
    int32_t                                            serv_snr; /* Indicates SNR of serving cell (see 3GPP TS 36.133 subclause 9.1.17). */
    int32_t                                            serv_rssi; /* Indicates the latest RSSI value of serving cell, unit in dBm, -255 is invalid. */
} mipc_lte_cc_meas_info_struct4 ;

typedef struct mipc_lte_cc_meas_info_v1_struct4 {
    char                                               lte_cc_type[12]; /* Indicates the type of cell */
    uint16_t                                           cell_index; /* Cell index in the cell list, PCC cell is 0, SCC cell index starts from 1. */
    uint16_t                                           path_loss; /* Indicates the measurement result of path loss, unit in dBm, 0~255 */
    uint16_t                                           rank_indicator; /* Indicates the measurement result of rank indicator; n: rank n; 0 is invalid */
    uint8_t                                            padding[2]; /* N/A */
    int32_t                                            serv_rsrp; /* Indicates RSRP of serving cell (see 3GPP TS 36.133 subclause 9.1.4). */
    int32_t                                            serv_rsrq; /* Indicates RSRQ of serving cell (see 3GPP TS 36.133 subclause 9.1.7). */
    int32_t                                            serv_snr; /* Indicates SNR of serving cell (see 3GPP TS 36.133 subclause 9.1.17). */
    int32_t                                            serv_rssi; /* Indicates the latest RSSI value of serving cell, unit in dBm, -255 is invalid. */
    int32_t                                            measbw_rssi; /* Indicates the latest RSSI value of measurement bandwidth, unit in dBm, -255 is invalid. */
} mipc_lte_cc_meas_info_v1_struct4 ;

typedef struct mipc_nr_cc_meas_info_struct4 {
    char                                               nr_cc_type[12]; /* Indicates the type of NR cell */
    uint16_t                                           path_loss; /* Indicates the measurement result of path loss, unit in dBm, 0~255 */
    uint16_t                                           rank_indicator; /* Indicates the measurement result of rank indicator.n: rank n; 0 is invalid */
    int32_t                                            brsrp; /* Indicates the measurement result of the best beam RSRP in current cell, unit in dBm. */
    int32_t                                            serv_rsrp; /* Indicates RSRP of serving cell (see 3GPP TS 38.133 subclause 10.1.6). */
    int32_t                                            serv_rsrq; /* Indicates RSRQ of serving cell (see 3GPP TS 38.133 subclause 10.1.11). */
    uint32_t                                           ssb_best_beam_id; /* Indicates the SSB beam index with the best RSRP, 0~63, 255 is invalid */
    int32_t                                            ssb_snr_avg; /* Indicates the average SNR of SSB measurement, unit in dB, -255 is invalid */
    int32_t                                            ssb_rsrp_avg; /* Indicates the average RSRP of SSB measurement, unit in dBm, -255 is invalid */
    int32_t                                            trs_snr_avg; /* Indicates the average SNR of TRS measurement, unit in dB, -255 is invalid */
    int32_t                                            trs_rsrp_avg; /* Indicates the average RSRP of TRS measurement, unit in dBm, -255 is invalid */
    int32_t                                            serv_snr; /* Indicates SNR of serving cell (see 3GPP TS 38.133 subclause 10.1.16). */
    int32_t                                            serv_rssi; /* Indicates the latest RSSI value of serving cell, unit in dBm, -255 is invalid. */
} mipc_nr_cc_meas_info_struct4 ;

typedef struct mipc_nr_cc_meas_info_v1_struct4 {
    char                                               nr_cc_type[12]; /* Indicates the type of NR cell */
    uint16_t                                           cell_index; /* Cell index in the cell list, PCC cell is 0, SCC cell index starts from 1. */
    uint16_t                                           path_loss; /* Indicates the measurement result of path loss, unit in dBm, 0~255 */
    uint16_t                                           rank_indicator; /* Indicates the measurement result of rank indicator.n: rank n; 0 is invalid */
    uint8_t                                            padding[2]; /* N/A */
    int32_t                                            brsrp; /* Indicates the measurement result of the best beam RSRP in current cell, unit in dBm. */
    int32_t                                            serv_rsrp; /* Indicates RSRP of serving cell (see 3GPP TS 38.133 subclause 10.1.6). */
    int32_t                                            serv_rsrq; /* Indicates RSRQ of serving cell (see 3GPP TS 38.133 subclause 10.1.11). */
    uint32_t                                           ssb_best_beam_id; /* Indicates the SSB beam index with the best RSRP, 0~63, 255 is invalid */
    int32_t                                            ssb_snr_avg; /* Indicates the average SNR of SSB measurement, unit in dB, -255 is invalid */
    int32_t                                            ssb_rsrp_avg; /* Indicates the average RSRP of SSB measurement, unit in dBm, -255 is invalid */
    int32_t                                            trs_snr_avg; /* Indicates the average SNR of TRS measurement, unit in dB, -255 is invalid */
    int32_t                                            trs_rsrp_avg; /* Indicates the average RSRP of TRS measurement, unit in dBm, -255 is invalid */
    int32_t                                            serv_snr; /* Indicates SNR of serving cell (see 3GPP TS 38.133 subclause 10.1.16). */
    int32_t                                            serv_rssi; /* Indicates the latest RSSI value of serving cell, unit in dBm, -255 is invalid. */
    int32_t                                            bwp_rssi; /* Indicates the latest RSSI value of active BWP bandwidth, unit in dBm, -255 is invalid. */
} mipc_nr_cc_meas_info_v1_struct4 ;

typedef struct mipc_wcdma_current_cell_info_struct4 {
    uint32_t                                           band; /* The band of WCDMA cell */
    uint32_t                                           pci; /* The physical cell id of WCDMA cell */
    uint32_t                                           uarfcn; /* The UARFCN of WCDMA cell */
    uint16_t                                           dl_bandwidth; /* DL bandaidth of WCDMA cell, always be 0 for 5MHz */
    uint16_t                                           ul_bandwidth; /* UL bandaidth of WCDMA cell, always be 0 for 5MHz */
} mipc_wcdma_current_cell_info_struct4 ;

typedef struct mipc_current_cell_info_struct4 {
    uint8_t                                            scc_state; /* The state of secondary cell;1:cell deactived,2:activated, 255: invalid for pcell */
    uint8_t                                            scc_ul_configured; /* The UL configuartion of secondary cell;0:disable, 1:enable, 255: invalid for pcell */
    uint16_t                                           cc_band; /* The band of cell */
    uint32_t                                           cc_pci; /* The physical cell id of cell; CC_PCI:0~1024 */
    uint32_t                                           cc_arfcn; /* EARFCN for LTE or NARFCN for NR */
    uint16_t                                           cc_dl_bandwidth; /* Cell bandwidth:for lte 0:1.4MHz 1:3MHz 2:5MHz 3:10MHz 4:15MHz 5:20MHz; for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    uint16_t                                           cc_ul_bandwidth; /* Cell bandwidth:for lte 0:1.4MHz 1:3MHz 2:5MHz 3:10MHz 4:15MHz 5:20MHz; for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    char                                               cc_dl_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    char                                               cc_ul_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    uint8_t                                            cc_dl_mimo; /* The UL/DL layer number of pcell/scell */
    uint8_t                                            cc_ul_mimo; /* The UL/DL layer number of pcell/scell */
    uint8_t                                            cc_dl_modulation; /* The UL/DL modulation of pcell/scell; 0: BPSK, 1: QPSK, 2: 16QAM, 3: 64QAM, 4: 256QAM */
    uint8_t                                            cc_ul_modulation; /* The UL/DL modulation of pcell/scell; 0: BPSK, 1: QPSK, 2: 16QAM, 3: 64QAM, 4: 256QAM */
    uint8_t                                            cc_dl_rb; /* The UL/DL RB number of pcell/scell */
    uint8_t                                            cc_ul_rb; /* The UL/DL RB number of pcell/scell */
    uint8_t                                            cc_dl_mcs; /* The UL/DL modulation and coding scheme index,0~31 */
    uint8_t                                            cc_ul_mcs; /* The UL/DL modulation and coding scheme index,0~31 */
    int32_t                                            cc_pucch_tx_pwr; /* The TX power of PUCCH ,dBm */
    int32_t                                            cc_pusch_tx_pwr; /* The TX power of PUSCH ,dBm */
    uint16_t                                           cc_lte_dl_tm; /* The UL/DL transmition mode of LTE; for cc_lte_dl_tm:1~10,0 is invalid; for cc_lte_ul_tm:1~2,0 is invalid */
    uint16_t                                           cc_lte_ul_tm; /* The UL/DL transmition mode of LTE; for cc_lte_dl_tm:1~10,0 is invalid; for cc_lte_ul_tm:1~2,0 is invalid */
    uint32_t                                           cc_dl_tput; /* The DL/UL throughput of Pcell/Scell for lte/nr; uint in kbytes/s */
    uint32_t                                           cc_ul_tput; /* The DL/UL throughput of Pcell/Scell for lte/nr; uint in kbytes/s */
    uint16_t                                           cc_dl_bwp_bandwidth; /* Cell bandwidth:just for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    uint16_t                                           cc_ul_bwp_bandwidth; /* Cell bandwidth:just for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    char                                               cc_dl_bwp_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    char                                               cc_ul_bwp_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    uint32_t                                           cc_dl_spectral_efficiency; /* The spectral_efficiency of the cell lte/nr,The value calc is (tput/bandwidth) (bit/s/Hz)* 100; So is 100 times larger than the real value;bit/s/Hz */
    uint32_t                                           cc_ul_spectral_efficiency; /* The spectral_efficiency of the cell lte/nr,The value calc is (tput/bandwidth) (bit/s/Hz)* 100; So is 100 times larger than the real value;bit/s/Hz */
} mipc_current_cell_info_struct4 ;

typedef struct mipc_current_cell_info_v1_struct4 {
    uint8_t                                            scc_state; /* The state of secondary cell;1:cell deactived,2:activated, 255: invalid for pcell */
    uint8_t                                            padding; /* N/A */
    uint16_t                                           cell_index; /* Cell index in the cell list, PCC cell is 0, SCC cell index starts from 1. */
    uint8_t                                            scc_ul_configured; /* The UL configuartion of secondary cell;0:disable, 1:enable, 255: invalid for pcell */
    uint8_t                                            padding1; /* N/A */
    uint16_t                                           cc_band; /* The band of cell */
    uint32_t                                           cc_pci; /* The physical cell id of cell; CC_PCI:0~1024 */
    uint32_t                                           cc_arfcn; /* EARFCN for LTE or NARFCN for NR */
    uint16_t                                           cc_dl_bandwidth; /* Cell bandwidth:for lte 0:1.4MHz 1:3MHz 2:5MHz 3:10MHz 4:15MHz 5:20MHz; for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    uint16_t                                           cc_ul_bandwidth; /* Cell bandwidth:for lte 0:1.4MHz 1:3MHz 2:5MHz 3:10MHz 4:15MHz 5:20MHz; for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    char                                               cc_dl_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    char                                               cc_ul_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    uint8_t                                            cc_dl_mimo; /* The UL/DL layer number of pcell/scell */
    uint8_t                                            cc_ul_mimo; /* The UL/DL layer number of pcell/scell */
    uint8_t                                            cc_dl_modulation; /* The UL/DL modulation of pcell/scell; 0: BPSK, 1: QPSK, 2: 16QAM, 3: 64QAM, 4: 256QAM */
    uint8_t                                            cc_ul_modulation; /* The UL/DL modulation of pcell/scell; 0: BPSK, 1: QPSK, 2: 16QAM, 3: 64QAM, 4: 256QAM */
    uint8_t                                            cc_dl_rb; /* The UL/DL RB number of pcell/scell */
    uint8_t                                            cc_ul_rb; /* The UL/DL RB number of pcell/scell */
    uint8_t                                            cc_dl_mcs; /* The UL/DL modulation and coding scheme index,0~31 */
    uint8_t                                            cc_ul_mcs; /* The UL/DL modulation and coding scheme index,0~31 */
    int32_t                                            cc_pucch_tx_pwr; /* The TX power of PUCCH ,dBm */
    int32_t                                            cc_pusch_tx_pwr; /* The TX power of PUSCH ,dBm */
    uint16_t                                           cc_lte_dl_tm; /* The UL/DL transmition mode of LTE; for cc_lte_dl_tm:1~10,0 is invalid; for cc_lte_ul_tm:1~2,0 is invalid */
    uint16_t                                           cc_lte_ul_tm; /* The UL/DL transmition mode of LTE; for cc_lte_dl_tm:1~10,0 is invalid; for cc_lte_ul_tm:1~2,0 is invalid */
    uint32_t                                           cc_dl_tput; /* The DL/UL throughput of Pcell/Scell for lte/nr; uint in kbytes/s */
    uint32_t                                           cc_ul_tput; /* The DL/UL throughput of Pcell/Scell for lte/nr; uint in kbytes/s */
    uint16_t                                           cc_dl_bwp_bandwidth; /* Cell bandwidth:just for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    uint16_t                                           cc_ul_bwp_bandwidth; /* Cell bandwidth:just for nr 0:5MHz 1:10MHz 2:15MHz 3:20MHz 4:25MHz 5:30MHz 6:40MHz 7:50MHz 8:60MHz 9:80MHz 10:90MHz 11:100MHz 12:200MHz 13:400MHz 14:70MHz */
    char                                               cc_dl_bwp_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    char                                               cc_ul_bwp_bandwidth_str[8]; /* cell bandwidth str unit: MHz */
    uint32_t                                           cc_dl_spectral_efficiency; /* The spectral_efficiency of the cell lte/nr,The value calc is (tput/bandwidth) (bit/s/Hz)* 100; So is 100 times larger than the real value;bit/s/Hz */
    uint32_t                                           cc_ul_spectral_efficiency; /* The spectral_efficiency of the cell lte/nr,The value calc is (tput/bandwidth) (bit/s/Hz)* 100; So is 100 times larger than the real value;bit/s/Hz */
    uint32_t                                           cc_dl_bwp_central_freq; /* Indicates the central frequency of DL active BWP for NR cell, unit in 5KHz, 0 is invalid */
    uint32_t                                           cc_ul_bwp_central_freq; /* Indicates the central frequency of UL active BWP for NR cell, unit in 5KHz, 0 is invalid */
    int32_t                                            cc_lte_target_pwr; /* Indicates the target Tx power of LTE cell, dBm, -255 is invalid */
    uint32_t                                           cc_lte_meas_bandwidth; /* Indicates the measurement bandwidth of LTE cell, refer to <cc_bandwidth> in <dl_cc_info_list>, 255 is invalid */
    uint32_t                                           central_arfcn; /* NR DL central arfcn, unit in 5KHz. */
} mipc_current_cell_info_v1_struct4 ;

typedef struct mipc_nw_provider_struct4 {
    uint32_t                                           index; /* Network index in the SIM Preferred network list */
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* PLMN id, format: ASCII (Number: 0-9) */
    uint8_t                                            reserved[1]; /* Reserved */
    char                                               network_name[60]; /* Network Name, format: ASCII */
    char                                               network_short_name[60]; /* Network short Name, format: ASCII */
    uint32_t                                           act; /* Bit 0: GSM_AcTn, 0: not selected, 1: selected;
            Bit 1: GSM_Compact_AcTn, 0: not selected, 1: selected;
            Bit 2: UTRAN_AcTn, 0: not selected, 1: selected;
            Bit 3: E-UTRAN_AcTn, 0: not selected, 1: selected;
            0: GSM
            1: GSM Compact
            2: UTRAN
            3: GSM w/EGPRS (see NOTE 1)
            4: UTRAN w/HSDPA (see NOTE 2)
            5: UTRAN w/HSUPA (see NOTE 2)
            6: UTRAN w/HSDPA and HSUPA (see NOTE 2)
            7: E-UTRAN
            8: EC-GSM-IoT (A/Gb mode) (see NOTE 3)
            9: E-UTRAN (NB-S1 mode) (see NOTE 4)
            10: E-UTRA connected to a 5GCN (see NOTE 5)
            11: NR connected to a 5GCN (see NOTE 5)
            12: NG-RAN
            13: E-UTRA-NR dual connectivity
 */
    uint32_t                                           act_supported; /* Bitmap of which access technology is selected */
    uint8_t                                            state; /* 0x0:unknown, 0x1:home, 0x2:forbidden, 0x4:preferred, 0x8:visible, 0x10: registered */
    uint8_t                                            rssi; /* RSSI: range 0-31 and 99 for invalid */
    uint8_t                                            error_rate; /* ErrorRate: range 0-7 and 99 for invalid */
    uint8_t                                            padding[1]; /* N/A */
} mipc_nw_provider_struct4 ;

typedef struct mipc_nw_nitz_info_struct4 {
    uint32_t                                           year; /* Year as an integer. */
    uint32_t                                           month; /* N/A */
    uint32_t                                           day; /* N/A */
    uint32_t                                           hour; /* N/A */
    uint32_t                                           minute; /* N/A */
    uint32_t                                           second; /* N/A */
    int32_t                                            time_zone_offset_minutes; /* This value includes any adjustment for the current state of daylight saving time */
    int32_t                                            daylight_saving_offset_minutes; /* This value indicates whether time zone includes daylight savings adjustment; for example, 0 means no adjustment for daylight saving time */
} mipc_nw_nitz_info_struct4 ;

typedef struct mipc_nw_anbr_info_struct4 {
    uint8_t                                            anbrq_config; /* N/A */
    uint8_t                                            ebi; /* N/A */
    uint8_t                                            is_ul; /* N/A */
    uint8_t                                            beare_id; /* qos flow identifier. */
    uint16_t                                           bitrate; /* bitrate value. */
    uint8_t                                            pdu_session_id; /* pdu session id. */
    uint8_t                                            ext_param; /* reserved for future usage. */
} mipc_nw_anbr_info_struct4 ;

typedef struct mipc_nw_irat_info_struct4 {
    uint8_t                                            irat_status; /* N/A */
    uint8_t                                            is_successful; /* present only when irat_status is 0. 0:inter_rat procedure failed , 1:inter_rat procedure is successful, 255: invalid value */
} mipc_nw_irat_info_struct4 ;

typedef struct mipc_nw_reg_state_struct4 {
    uint8_t                                            ps_state; /* It indicates packet service register state
            0: detached
            1: attached, home network
            2: not registered, but MT is currently trying to attach or searching for an operator for registration
            3: registration denied
            4: detached
            5: attached, roaming network
 */
    uint8_t                                            cs_state; /* Indicate circuit service register state. */
    char                                               plmn[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint8_t                                            padding[1]; /* N/A */
    char                                               cs_plmn[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint8_t                                            padding2[1]; /* N/A */
    uint32_t                                           lac; /* N/A */
    uint32_t                                           cs_lac; /* N/A */
    uint32_t                                           ci; /* N/A */
    uint32_t                                           cs_ci; /* N/A */
    uint8_t                                            is_roaming; /* N/A */
    uint8_t                                            cs_is_roaming; /* N/A */
    uint8_t                                            padding3[2]; /* N/A */
} mipc_nw_reg_state_struct4 ;

typedef struct mipc_nw_reg_state_v1_struct4 {
    uint8_t                                            ps_state; /* It indicates packet service register state
            0: detached
            1: attached, home network
            2: not registered, but MT is currently trying to attach or searching for an operator for registration
            3: registration denied
            4: detached
            5: attached, roaming network
 */
    uint8_t                                            cs_state; /* Indicate circuit service register state. */
    char                                               plmn[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    char                                               cs_plmn[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint32_t                                           lac; /* N/A */
    uint32_t                                           cs_lac; /* N/A */
    uint32_t                                           ci; /* N/A */
    uint32_t                                           cs_ci; /* N/A */
    uint8_t                                            is_roaming; /* N/A */
    uint8_t                                            cs_is_roaming; /* N/A */
    uint8_t                                            padding[2]; /* Padding */
} mipc_nw_reg_state_v1_struct4 ;

typedef struct mipc_nw_reg_info_struct4 {
    uint8_t                                            state; /* It indicates network registration status
            0: Unregistered, modem isn't currently searching a new operator to register to
            1: Registered, home network
            2: Not registered, but MT is currently trying to attach or searching for an operator for registration
            3: Registration denied
            4: Out of E-UTRAN coverage
            5: Registered, roaming network
 */
    char                                               plmn[MIPC_MAX_PLMN_ID_LEN]; /* PLMN ID */
    uint32_t                                           lac; /* Location area code */
    uint32_t                                           ci; /* Cell ID */
} mipc_nw_reg_info_struct4 ;

typedef struct mipc_nw_location_info_struct4 {
    uint32_t                                           location_area_code; /* GSM/UMTS area code of the current location */
    uint32_t                                           tracking_area_code; /* LTE tracking area code of the current location */
    uint32_t                                           cell_id; /* ID of the cellular tower */
} mipc_nw_location_info_struct4 ;

typedef struct mipc_nw_gsm_cell_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint32_t                                           lac; /* Location area code */
    uint32_t                                           cid; /* Cell Identity */
    uint32_t                                           ta; /* Timing advance */
    uint32_t                                           arfcn; /* The Absolute Radio Frequency Channel Number of the cell; use 0xFFFFFFFF when this information is not available */
    uint32_t                                           base_station_id; /* The Base Station ID for the cell */
    uint32_t                                           rx_level; /* The received signal strength of the cell */
    uint32_t                                           bitErrorRate; /* Transport Channel BER as defined in TS 25.225 5.2.5; valid values are (0-7, 99) as defined in TS 27.007 8.5 */
    uint32_t                                           registered; /* The registered state of the cell */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Long provider name */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Short provider name */
} mipc_nw_gsm_cell_struct4 ;

typedef struct mipc_nw_umts_cell_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint32_t                                           lac; /* Location area code */
    uint32_t                                           cid; /* Cell Identity */
    uint32_t                                           uarfcn; /* The UTRA Absolute Radio Frequency Channel Number of the cell. Use 0xFFFFFFFF when this information is not available */
    uint32_t                                           psc; /* The Primary Scrambling Code of the cell */
    int32_t                                            rscp; /* The received signal code power of the cell */
    int32_t                                            ecno; /* The signal to noise ratio of the cell */
    uint32_t                                           registered; /* The registered state of the cell */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* long provider name */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* short provider name */
} mipc_nw_umts_cell_struct4 ;

typedef struct mipc_nw_lte_cell_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint32_t                                           cid; /* Cell Identity */
    uint32_t                                           earfcn; /* The Radio Frequency Channel Number of the cell; use 0xFFFFFFFF when this information is not available */
    uint32_t                                           physical_cell_id; /* The Physical Cell ID; use 0xFFFFFFFF when this information is not available */
    uint32_t                                           tac; /* The Tracking Area Code */
    int32_t                                            rsrp; /* The Average Reference signal Received Power (RSRP) */
    int32_t                                            rsrq; /* Average Reference signal Received Quality (RSRQ) */
    uint32_t                                           ta; /* Timing advance */
    int32_t                                            rsrp_in_qdbm; /* The Average Reference Signal Received Power (RSRP) in qdbm */
    int32_t                                            rsrq_in_qdbm; /* Average Reference Signal Received Quality (RSRQ) in qdbm */
    int32_t                                            rssnr; /* The current RSSNR in 0.1 dB units; range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB); INT_MAX: 0x7FFFFFFF denotes invalid value */
    uint32_t                                           cqi; /* The current Channel Quality Indicator; range: 0 to 15; INT_MAX : 0x7FFFFFFF denotes invalid value */
    uint32_t                                           dl_freq_band; /* Downlink frequency band used by the cell */
    uint32_t                                           registered; /* The registered state of the cell */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Long provider name */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Short provider name */
} mipc_nw_lte_cell_struct4 ;

typedef struct mipc_nw_lte_cell_v1_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint32_t                                           cid; /* Cell Identity. */
    uint32_t                                           earfcn; /* The Radio Frequency Channel Number of the cell. Use 0xFFFFFFFF when this information is not available. */
    uint32_t                                           physical_cell_id; /* The Physical Cell ID. Use 0xFFFFFFFF when this information is not available. */
    uint32_t                                           tac; /* The Tracking Area Code. */
    int32_t                                            rsrp; /* The Average Reference signal Received Power. */
    int32_t                                            rsrq; /* Average Reference signal Received Quality. */
    uint32_t                                           ta; /* timing advance. */
    int32_t                                            rsrp_in_qdbm; /* The Average Reference signal Received Power in qdbm. */
    int32_t                                            rsrq_in_qdbm; /* Average Reference signal Received Quality in qdbm. */
    int32_t                                            rssnr; /* The current reference signal signal-to-noise ratio in 0.1 dB units.Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).INT_MAX : 0x7FFFFFFF denotes invalid value. */
    uint32_t                                           cqi; /* The current Channel Quality Indicator.Range: 0 to 15,INT_MAX : 0x7FFFFFFF denotes invalid value. */
    uint32_t                                           dl_freq_band; /* downlink freq band used by the cell. */
    uint32_t                                           registered; /* The registered state of the cell. */
    uint8_t                                            cqi_table_index; /* CQI table index.0xFF denotes invalid value. */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* long provider name. */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* short provider name. */
} mipc_nw_lte_cell_v1_struct4 ;

typedef struct mipc_nw_lte_cell_v2_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint32_t                                           cid; /* Cell Identity. */
    uint32_t                                           earfcn; /* The Radio Frequency Channel Number of the cell. Use 0xFFFFFFFF when this information is not available. */
    uint32_t                                           physical_cell_id; /* The Physical Cell ID. Use 0xFFFFFFFF when this information is not available. */
    uint32_t                                           tac; /* The Tracking Area Code. */
    int32_t                                            rsrp; /* The Average Reference signal Received Power. */
    int32_t                                            rsrq; /* Average Reference signal Received Quality. */
    uint32_t                                           ta; /* timing advance. */
    int32_t                                            rsrp_in_qdbm; /* The Average Reference signal Received Power in qdbm. */
    int32_t                                            rsrq_in_qdbm; /* Average Reference signal Received Quality in qdbm. */
    int32_t                                            rssnr; /* The current reference signal signal-to-noise ratio in 0.1 dB units.Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).INT_MAX : 0x7FFFFFFF denotes invalid value. */
    uint32_t                                           cqi; /* The current Channel Quality Indicator.Range: 0 to 15,INT_MAX : 0x7FFFFFFF denotes invalid value. */
    uint32_t                                           dl_freq_band; /* downlink freq band used by the cell. */
    uint32_t                                           registered; /* The registered state of the cell. */
    uint8_t                                            cqi_table_index; /* CQI table index.0xFF denotes invalid value. */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* long provider name. */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* short provider name. */
    uint8_t                                            padding[3]; /* Padding */
} mipc_nw_lte_cell_v2_struct4 ;

typedef struct mipc_nw_nr_cell_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* Compose of Mobile Country Code (MCC) and Mobile Network Code (MNC) */
    uint64_t                                           cid; /* Cell Identity; 0x000000000FFFFFFF: Invalid value */
    uint32_t                                           physical_cell_id; /* The Physical Cell ID; 0x0FFFFFFF: Invalid value */
    uint32_t                                           nr_arfcn; /* Absolute RF channel number of the serving cell */
    uint32_t                                           tac; /* The Tracking Area Code */
    int32_t                                            rsrp; /* The Average Reference Signal Received Power (RSRP); the valid range is 0~97; 255: Not known or not detectable */
    int32_t                                            rsrq; /* Average Reference Signal Received Quality (RSRQ); the valid range is 0~34; 255: Not known or not detectable */
    int32_t                                            sinr; /* SS Signal-to-Noise and Interference Ratio */
    uint32_t                                           ta; /* Timing advance; 0x0FFFFFFF: Invalid value */
    uint32_t                                           csirsrp; /* CSI Reference Signal Received Power (RSRP) */
    uint32_t                                           csirsrq; /* CSI Reference Signal Received Quality (RSRQ) */
    uint32_t                                           csisinr; /* CSI Signal-to-Noise and Interference Ratio (SINR) */
    uint32_t                                           dl_freq_band; /* Downlink frequency band used by the cell */
    uint32_t                                           registered; /* The registered state of the cell; 0: Not registered; 1: Registered */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* long provider name. */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* short provider name. */
} mipc_nw_nr_cell_struct4 ;

typedef struct mipc_nw_nr_cell_v1_struct4 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    uint64_t                                           cid; /* Cell Identity. */
    uint32_t                                           physical_cell_id; /* The Physical Cell ID. */
    uint32_t                                           nr_arfcn; /* Absolute RF channel number of the serving cell. */
    uint32_t                                           tac; /* The Tracking Area Code. */
    int32_t                                            rsrp; /* The Average Reference signal Received Power. */
    int32_t                                            rsrq; /* Average Reference signal Received Quality. */
    int32_t                                            sinr; /* SS signal-to-noise and interference ratio. */
    uint32_t                                           ta; /* timing advance. */
    uint32_t                                           csirsrp; /* CSI reference signal received power. */
    uint32_t                                           csirsrq; /* CSI reference signal received quality. */
    uint32_t                                           csisinr; /* CSI signal-to-noise and interference ratio. */
    uint32_t                                           dl_freq_band; /* downlink freq band used by the cell. */
    uint32_t                                           registered; /* The registered state of the cell. */
    uint8_t                                            csi_cqi_table_index; /* CSI cqi table index */
    uint8_t                                            csi_cqi_report_num; /* CSI cqi report num */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Long provider name */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Short provider name */
    uint8_t                                            csi_cqi_report_list[19]; /* CSI cqi report info */
} mipc_nw_nr_cell_v1_struct4 ;

typedef struct mipc_nw_cdma_cell_struct4 {
    uint32_t                                           networkId; /* Network Id 0..65535, 0xffffffff means invalid */
    uint32_t                                           systemId; /* CDMA System Id 0..32767, 0xffffffff means invalid. */
    uint32_t                                           basestationId; /* Base Station Id 0..65535, 0xffffffff means invalid */
    uint32_t                                           longitude; /* Longitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.It is represented in units of 0.25 seconds and ranges from -2592000 to 2592000, both values inclusive (corresponding to a range of -180 to +180 degrees). 0xffffffff 0xffffffff means invalid . */
    uint32_t                                           latitude; /* Latitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.It is represented in units of 0.25 seconds and ranges from -1296000 to 1296000, both values inclusive (corresponding to a range of -90 to +90 degrees). 0xffffffff 0xffffffff means invalid . */
    uint32_t                                           dbm_cdma; /* This value is the actual RSSI value. */
    uint32_t                                           ecio_cdma; /* This value is the actual Ec/Io. */
    uint32_t                                           dbm_evdo; /* This value is the actual RSSI value. */
    uint32_t                                           ecio_evdo; /* This value is the actual Ec/Io. */
    uint32_t                                           snr_evdo; /* signal to noise ratio. */
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    uint8_t                                            registered; /* The registered state of the cell. */
} mipc_nw_cdma_cell_struct4 ;

typedef struct mipc_nw_cdma_cell_v1_struct4 {
    uint32_t                                           networkId; /* Network Id 0..65535, 0xffffffff means invalid */
    uint32_t                                           systemId; /* CDMA System Id 0..32767, 0xffffffff means invalid. */
    uint32_t                                           basestationId; /* Base Station Id 0..65535, 0xffffffff means invalid */
    uint32_t                                           longitude; /* Longitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.It is represented in units of 0.25 seconds and ranges from -2592000 to 2592000, both values inclusive (corresponding to a range of -180 to +180 degrees). 0xffffffff 0xffffffff means invalid . */
    uint32_t                                           latitude; /* Latitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.It is represented in units of 0.25 seconds and ranges from -1296000 to 1296000, both values inclusive (corresponding to a range of -90 to +90 degrees). 0xffffffff 0xffffffff means invalid . */
    uint32_t                                           dbm_cdma; /* This value is the actual RSSI value. */
    uint32_t                                           ecio_cdma; /* This value is the actual Ec/Io. */
    uint32_t                                           dbm_evdo; /* This value is the actual RSSI value. */
    uint32_t                                           ecio_evdo; /* This value is the actual Ec/Io. */
    uint32_t                                           snr_evdo; /* signal to noise ratio. */
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    uint8_t                                            registered; /* The registered state of the cell. */
    uint8_t                                            padding[2]; /* Padding */
} mipc_nw_cdma_cell_v1_struct4 ;

typedef struct mipc_nw_tdscdma_cell_struct4 {
    int32_t                                            TBD; /* N/A */
} mipc_nw_tdscdma_cell_struct4 ;

typedef struct mipc_nw_reg_change_info_struct4 {
    uint8_t                                            stat; /* Network register status. */
    uint32_t                                           lac_tac; /* Location area code. */
    uint64_t                                           cell_id; /* Cell id, 32 bit for non-5G and 36 bit for 5G. */
    uint16_t                                           eact; /* Radio access technology. */
    uint8_t                                            rac; /* Routing area code. */
    uint8_t                                            nw_existence; /* Indicate if nw exist, only valid when eact belongs to C2K. */
    uint8_t                                            roam_indicator; /* Roam indicator, only valid when eact belongs to C2K. */
    uint8_t                                            cause_type; /* Indicates the type of reject_cause. */
    uint16_t                                           reject_cause; /* The cause of the failed registration. */
    uint8_t                                            dcnr_restricted; /* Indicate if dcnr is restricted. */
    uint8_t                                            endc_sib_status; /* Indicates endc sib status. */
    uint8_t                                            endc_available; /* Indicate if endc is available. */
} mipc_nw_reg_change_info_struct4 ;

typedef struct mipc_nw_cs_reg_info_struct4 {
    uint8_t                                            stat; /* Network register status. */
    uint16_t                                           rat; /* The radio technology, please refer to eAct of EREG. */
    uint8_t                                            css; /* Network register status, 0: Concurrent service not support; 1: Concurrent service support. */
    uint8_t                                            roaming_ind; /* If register on CDMA or EVDO. */
    int8_t                                             is_in_prl; /* indicates whether the current system is in the PRL if registered on a CDMA or EVDO system or -1 if not. 0=not in the PRL, 1=in the PRL. */
    int16_t                                            def_roaming_ind; /* default Roaming Indicator from the PRL, if registered on a CDMA or EVDO system or -1 if not. Valid values are 0-255. */
    uint16_t                                           reason_for_denial; /* Registeration denied reason. See 3GPP TS 24.008. */
} mipc_nw_cs_reg_info_struct4 ;

typedef struct mipc_nw_cs_reg_info_v1_struct4 {
    uint8_t                                            stat; /* Network register status. */
    uint8_t                                            padding1; /* Padding */
    uint16_t                                           rat; /* The radio technology, please refer to eAct of EREG. */
    uint8_t                                            css; /* Network register status, 0: Concurrent service not support; 1: Concurrent service support. */
    uint8_t                                            roaming_ind; /* If register on CDMA or EVDO. */
    int8_t                                             is_in_prl; /* indicates whether the current system is in the PRL if registered on a CDMA or EVDO system or -1 if not. 0=not in the PRL, 1=in the PRL. */
    uint8_t                                            padding2; /* Padding */
    int16_t                                            def_roaming_ind; /* default Roaming Indicator from the PRL, if registered on a CDMA or EVDO system or -1 if not. Valid values are 0-255. */
    uint16_t                                           reason_for_denial; /* Registeration denied reason. See 3GPP TS 24.008. */
} mipc_nw_cs_reg_info_v1_struct4 ;

typedef struct mipc_nw_ps_reg_info_struct4 {
    uint8_t                                            stat; /* It indicates packet service register state */
    uint16_t                                           rat; /* The radio technology of serving cell */
    uint16_t                                           reason_for_denial; /* Registration denied reason; refer to 3GPP TS 24.008 */
    uint16_t                                           max_data_calls; /* The maximum number of simultaneous data calls; it can be removed because it is always 0 in RIL */
    int32_t                                            is_vops_supported; /* It indicates if camped network supports VoLTE services */
    int32_t                                            is_emc_bearer_supported; /* It indicates if camped network supports VoLTE emergency bearers */
    uint8_t                                            is_endc_available; /* It indicates if E-UTRA-NR Dual Connectivity(EN-DC) is supported by the primary serving cell */
    uint8_t                                            is_dcnr_restricted; /* It indicates if dual connectivity with NR is restricted */
    uint8_t                                            is_nr_available; /* It indicates if NR is available */
    uint8_t                                            roaming_ind; /* It indicates if network is registered on CDMA or EVDO */
    int8_t                                             is_in_prl; /* It indicates whether the current system is included in the PRL if network is registered on a CDMA or EVDO system;
            -1: network is not registered on a CDMA or EVDO system;
            0: network is registered on a CDMA or EVDO system but is not included in the PRL;
            1: network is registered on a CDMA or EVDO system and is included in the PRL
 */
    int16_t                                            def_roaming_ind; /* Default Roaming Indicator from the PRL if network is registered on a CDMA or EVDO system;
            -1: network is not registered on a CDMA or EVDO system;
            valid values are 0-255
 */
    uint64_t                                           cell_id; /* Cell ID */
    uint8_t                                            cache_endc_connect; /* Cache ENDC connection for a specified period of time (the timer is configured by API) */
    uint32_t                                           lac_tac; /* Location area code */
} mipc_nw_ps_reg_info_struct4 ;

typedef struct mipc_nw_channel_lock_info_struct4 {
    uint8_t                                            opt; /* The mode of channel lock; 0: cancel channel lock; 1: enable channel lock for EM; 2: enable channel lock for CBRS; 3: configure channel lock for CBRS */
    uint8_t                                            act; /* 0: GSM; 2: UTRAN; 7: LTE; 11: NR */
    uint32_t                                           band_info[4]; /* Bitmap for supported band information: band_info[0]'s bit0~bit31 indicating band1~band32, band_info[1]'s bit0~bit31 indicating band33~band64, and so on. */
    uint8_t                                            channel_num; /* The real length for arfcn_list */
    uint32_t                                           arfcn_list[32]; /* The ARFCN list expected to be locked */
    uint32_t                                           cell_id; /* It indicates the cell identifier; once the cell_id is specified, the channel_num will be considered as 1 */
    uint8_t                                            lock_mode; /* Lock channel mode */
} mipc_nw_channel_lock_info_struct4 ;

typedef struct mipc_nw_channel_lock_info_v1_struct4 {
    uint32_t                                           opt; /* The mode of channel lock;
            0: Cancel channel lock;
            1: Enable channel lock for EM;
            2: Enable channel lock for CBRS;
            3: Configure channel lock for CBRS
 */
    uint32_t                                           act; /* 0: GSM; 2: UTRAN; 7: LTE; 11: NR */
    uint32_t                                           band_info[32]; /* Bitmap for supported band information: band_info[0]'s bit0~bit31 indicating band1~band32, band_info[1]'s bit0~bit31 indicating band33~band64, and so on. */
    uint32_t                                           channel_num; /* The real length for arfcn_list */
    uint32_t                                           arfcn_list[32]; /* The ARFCN list expected to be locked */
    uint32_t                                           cell_id; /* It indicates the cell identifier; once the cell_id is specified, the channel_num will be considered as 1 */
    uint32_t                                           lock_mode; /* Lock channel mode */
} mipc_nw_channel_lock_info_v1_struct4 ;

typedef struct mipc_nw_gsm_signal_strength_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99) as defined in TS27.007 8.5 */
    int32_t                                            bit_error_rate; /* Valid values are (0-7, 99) as defined in TS27.007 8.5 */
    int32_t                                            timing_advance; /* Timing Advance in bit periods. 1 bit period = 48/13 us */
} mipc_nw_gsm_signal_strength_struct4 ;

typedef struct mipc_nw_umts_signal_strength_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99), as defined in TS27.007 8.5 */
    int32_t                                            bit_error_rate; /* Valid values are (0-7, 99), as defined in TS27.007 8.5 */
    int32_t                                            rscp; /* CPICH RSCP as defined in TS 25.215 5.1.1, valid value are (0-96, 255) */
    int32_t                                            ecno; /* Ec/No value as defined in TS 25.215 5.1.5. Valid values are (0-49, 255) */
} mipc_nw_umts_signal_strength_struct4 ;

typedef struct mipc_nw_lte_signal_strength_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0~31, 99), as defined in TS27.007 8.5 */
    int32_t                                            rsrp; /* The current RSRP in dBm, multiplied by -1; range: 44~140 dBm */
    int32_t                                            rsrq; /* The current RSRQ in dB, multiplied by -1; range: 20~3 dB */
    int32_t                                            rssnr; /* The current RSSNR in 0.1 dB units; range: -200 to +300 */
    int32_t                                            cqi; /* The current CQI; range: 0~15 */
    int32_t                                            timing_advance; /* Timing advance in micro seconds (from cell to device); range: 0~0x7ffffffe */
} mipc_nw_lte_signal_strength_struct4 ;

typedef struct mipc_nw_lte_signal_strength_v1_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99) as defined in TS27.007 8.5. */
    int32_t                                            rsrp; /* The current Reference Signal Receive Power in dBm multipled by -1. Range: 44-140 dBm. */
    int32_t                                            rsrq; /* The current Reference Signal Receive Quality in dB multiplied by -1. (20 - 3 dB). */
    int32_t                                            rssnr; /* The current reference signal signal-to-noise ratio in 0.1 dB units. Range: -200 to + 300 */
    int32_t                                            cqi; /* The current Channel Quality Indicator, range(0-15). */
    int32_t                                            timing_advance; /* Timing advance in micro seconds for a one way trip from cell to device. Range(0 - 0x7ffffffe) */
    uint8_t                                            cqi_table_index; /* CQI table index. 0xff means invalid. */
} mipc_nw_lte_signal_strength_v1_struct4 ;

typedef struct mipc_nw_lte_signal_strength_v2_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99) as defined in TS27.007 8.5. */
    int32_t                                            rsrp; /* The current Reference Signal Receive Power in dBm multipled by -1. Range: 44-140 dBm. */
    int32_t                                            rsrq; /* The current Reference Signal Receive Quality in dB multiplied by -1. (20 - 3 dB). */
    int32_t                                            rssnr; /* The current reference signal signal-to-noise ratio in 0.1 dB units. Range: -200 to + 300 */
    int32_t                                            cqi; /* The current Channel Quality Indicator, range(0-15). */
    int32_t                                            timing_advance; /* Timing advance in micro seconds for a one way trip from cell to device. Range(0 - 0x7ffffffe) */
    uint8_t                                            cqi_table_index; /* CQI table index. 0xff means invalid. */
    uint8_t                                            padding[3]; /* Padding */
} mipc_nw_lte_signal_strength_v2_struct4 ;

typedef struct mipc_nw_nr_signal_strength_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0~31, 99), as defined in TS27.007 8.5 */
    int32_t                                            ss_rsrp; /* SS RSRP, multiplied by -1; range: 44~140 */
    int32_t                                            ss_rsrq; /* SS RSRQ, multiplied by -1; range: 3~20 */
    int32_t                                            ss_sinr; /* SS SINR; range: -23~40 */
    int32_t                                            csi_rsrp; /* CSI RSRP, multiplied by -1; range: 44~140 */
    int32_t                                            csi_rsrq; /* CSI RSRQ, multiplied by -1; range: 3~20 */
    int32_t                                            csi_sinr; /* CSI SINR; range: -23~40 */
} mipc_nw_nr_signal_strength_struct4 ;

typedef struct mipc_nw_nr_signal_strength_v1_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99) as defined in TS27.007 8.5. */
    int32_t                                            ss_rsrp; /* SS reference signal received power, multipled by -1, Range[44, 140]. */
    int32_t                                            ss_rsrq; /* SS reference signal received quality, multipled by -1, Range[3, 20]. */
    int32_t                                            ss_sinr; /* SS signal-to-noise and interference ratio. Range[-23, 40]. */
    int32_t                                            csi_rsrp; /* CSI reference signal received power, multipled by -1. Range[44, 140] */
    int32_t                                            csi_rsrq; /* CSI reference signal received quality, multipled by -1. Range[3, 20] */
    int32_t                                            csi_sinr; /* CSI signal-to-noise and interference ratio. Range[-23, 40] */
    uint8_t                                            csi_cqi_table_index; /* CSI cqi table index */
    uint8_t                                            csi_cqi_report_num; /* CSI cqi report num */
    uint8_t                                            csi_cqi_report_list[19]; /* CSI cqi report info */
} mipc_nw_nr_signal_strength_v1_struct4 ;

typedef struct mipc_nw_nr_signal_strength_v3_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99) as defined in TS27.007 8.5. */
    int32_t                                            ss_rsrp; /* SS reference signal received power, multipled by -1, Range[44, 140]. */
    int32_t                                            ss_rsrq; /* SS reference signal received quality, multipled by -1, Range[3, 20]. */
    int32_t                                            ss_sinr; /* SS signal-to-noise and interference ratio. Range[-23, 40]. */
    int32_t                                            csi_rsrp; /* CSI reference signal received power, multipled by -1. Range[44, 140] */
    int32_t                                            csi_rsrq; /* CSI reference signal received quality, multipled by -1. Range[3, 20] */
    int32_t                                            csi_sinr; /* CSI signal-to-noise and interference ratio. Range[-23, 40] */
    uint8_t                                            csi_cqi_table_index; /* CSI cqi table index */
    uint8_t                                            csi_cqi_report_num; /* CSI cqi report num */
    uint8_t                                            csi_cqi_report_list[19]; /* CSI cqi report info */
    uint8_t                                            padding[3]; /* Padding */
    int32_t                                            timing_advance; /* Timing advance in micro seconds for a one way trip from cell to device. Range(0 - 0x7ffffffe) */
} mipc_nw_nr_signal_strength_v3_struct4 ;

typedef struct mipc_nw_cdma_signal_strength_struct4 {
    int32_t                                            dbm; /* cdma actual RSSI value. */
    int32_t                                            ecio; /* cdma actual Ec/Io value. */
    int32_t                                            snr; /* cdma signal noise ratio. */
} mipc_nw_cdma_signal_strength_struct4 ;

typedef struct mipc_nw_cscon_status_struct4 {
    uint8_t                                            mode; /* It indicates the signaling connection status */
    uint8_t                                            state; /* It indicates the PS state while in GERAN and the RRC state information if the MT is in connected mode while in UTRAN, E-UTRAN and NG-RAN */
    uint8_t                                            access; /* It indicates the current radio access type */
    uint8_t                                            core_network; /* It indicates the core network type which the UE is connected to */
} mipc_nw_cscon_status_struct4 ;

typedef struct mipc_nw_pol_info_struct4 {
    uint32_t                                           start_index; /* It indicates the start index for preferred network list */
    uint32_t                                           end_index; /* It indicates the end index for preferred network list */
    uint8_t                                            format_first; /* It indicates the start format code for network name */
    uint8_t                                            format_last; /* It indicates the stop format code for network name */
} mipc_nw_pol_info_struct4 ;

typedef struct mipc_nw_pol_info_v1_struct4 {
    uint32_t                                           start_index; /* It indicates the start index for preferred network list */
    uint32_t                                           end_index; /* It indicates the end index for preferred network list */
    uint8_t                                            format_first; /* It indicates the start format code for network name */
    uint8_t                                            format_last; /* It indicates the stop format code for network name */
    uint8_t                                            padding[2]; /* Padding */
} mipc_nw_pol_info_v1_struct4 ;

typedef struct mipc_nw_ps_cs_reg_roaming_info_struct4 {
    uint8_t                                            m_voice_reg_state; /* CS state:0 1 2 3 */
    uint8_t                                            m_data_reg_state; /* PS state:0 1 2 3 */
    uint8_t                                            m_voice_roaming_type; /* CS roaming type:0 1 2 3 */
    uint8_t                                            m_data_roaming_type; /* PS roaming type:0 1 2 3 */
    uint8_t                                            m_ril_voice_reg_state; /* CS registration state for GSM,UMTS and CDMA 0~15 */
    uint8_t                                            m_ril_data_reg_state; /* PS registration state for GSM,UMTS and CDMA 0~15 */
} mipc_nw_ps_cs_reg_roaming_info_struct4 ;

typedef struct mipc_nw_cellmeasurement_info_struct4 {
    uint8_t                                            rat; /* Cell RAT info: 7: LTE; 13 : NG-RAN */
    uint32_t                                           arfcn; /* The ARFCN information for this cell */
    uint32_t                                           pci; /* The physical cell ID for this cell */
    uint32_t                                           rsrp; /* The signal information in qdbm for this cell */
    uint32_t                                           rsrq; /* The signal information in qdbm for this cell */
    uint32_t                                           snr; /* The signal information in qdb for this cell */
    uint64_t                                           cid; /* The cell ID for this cell */
} mipc_nw_cellmeasurement_info_struct4 ;

typedef struct mipc_nw_cell_band_bandwidth_struct4 {
    uint16_t                                           cell_band; /* Cell band: 1~1024 */
    uint32_t                                           cell_bandwidth; /* Cell bandwidth: 0~400000 kHz */
} mipc_nw_cell_band_bandwidth_struct4 ;

typedef struct mipc_nw_cell_band_bandwidth_v1_struct4 {
    uint16_t                                           cell_band; /* Cell band: 1~1024 */
    uint8_t                                            padding[2]; /* Padding */
    uint32_t                                           cell_bandwidth; /* Cell bandwidth: 0~400000 kHz */
} mipc_nw_cell_band_bandwidth_v1_struct4 ;

typedef struct mipc_nw_lte_nr_ca_info_struct4 {
    uint32_t                                           cell_index; /* ServCell index */
    uint8_t                                            cell_state; /* 1: SCELL_STATUS_NOACTIVE; 2: SCELL_STATUS_ACTIVE */
    uint8_t                                            cc_cw0_cqi; /* Range: 0~15 */
    uint8_t                                            cc_cw1_cqi; /* Range: 0~15 */
    uint8_t                                            cell_bandwidth; /* Cell bandwidth for LTE:
            0: 1.4MHz
            1: 3MHz
            2: 5MHz
            3: 10MHz
            4: 15MHz
            5: 20MHz
            Cell bandwidth for NR:
            0: 5MHz
            1: 10MHz
            2: 15MHz
            3: 20MHz
            4: 25MHz
            5: 30MHz
            6: 40MHz
            7: 50MHz
            8: 60MHz
            9: 80MHz
            10: 90MHz
            11: 100MHz
            12: 200MHz
            13: 400MHz
            14: 70MHz
 */
    uint16_t                                           cell_band; /* Range: 1~1024 */
    uint16_t                                           cc_pci; /* Range: 0~1024 */
    uint32_t                                           cc_arfcn; /* Channel information: EARFCN for LTE and NR-ARFCN for NR5G */
    char                                               cell_bandwidth_str[8]; /* Cell bandwidth string; unit: MHz */
} mipc_nw_lte_nr_ca_info_struct4 ;

typedef struct mipc_nw_umts_cell_frequency_info_struct4 {
    uint32_t                                           ul_arfcn; /* UMTS upload ARFCN. */
    uint32_t                                           dl_arfcn; /* UMTS download ARFCN. */
    uint32_t                                           band; /* UMTS band information. */
} mipc_nw_umts_cell_frequency_info_struct4 ;

typedef struct mipc_cell_plmn_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    char                                               plmn_name[32]; /* N/A */
} mipc_cell_plmn_struct4 ;

typedef struct mipc_nw_raw_signal_info_struct4 {
    int32_t                                            sig1; /* 0 ~ 63: This field is identical to <rscp> in +CESQ when camping on GSM service;
            0 ~ 96: This field is identical to <rscp> in +CESQ when camping on UMTS service;
            0 ~ 34: This field is identical to <rsrq> in +CESQ when camping on LTE network;
            255: It indicates invalid value
 */
    int32_t                                            sig2; /* 0 ~ 7: BER when camping on GSM;
            0 ~ 49: EcNo when camping on UMTS;
            0 ~ 97: RSRP when camping on LTE
 */
    int32_t                                            rssi_in_qdbm; /* Negative integer; received signal strength raw data in quarter dbm; 1: invalid */
    int32_t                                            rscp_in_qdbm; /* Negative integer; RSCP in quarter dbm; it is available only when camping on UMTS network; 1: invalid */
    int32_t                                            ecn0_in_qdbm; /* Negative integer; EcNo in quarter dbm; it is available only when camping on UMTS network; 1: invalid */
    int32_t                                            rsrq_in_qdbm; /* Negative integer;
            GSM/UMTS/LTE/C2K: RSRQ in quarter dB; it is available only when camping on LTE network; 1: invalid;
            NR: SS-RSRQ in qdb; 0x7FFF: invalid
 */
    int32_t                                            rsrp_in_qdbm; /* Negative integer;
            GSM/UMTS/LTE/C2K: RSRP in quarter dbm; it is available only when camping on LTE network; 1: invalid;
            NR: SS-RSRP in qdbm; 0x7FFF: invalid
 */
    int32_t                                            eact; /*  */
    int32_t                                            sig3; /* GSM/UMTS/LTE/C2K: RSSNR in quarter dB; it is available only when camping on LTE network; 0x7FFF: invalid;
            NR: SS-SINR in qdb; 0x7FFF: invalid
 */
    int32_t                                            serv_band; /* Serving LTE band */
    int32_t                                            second_rsrq_in_qdbm; /* CSI-RSRQ in qdb; it is available only when camping on NR network; 0x7FFF: invalid; invalid value is reported by default */
    int32_t                                            second_rsrp_in_qdbm; /* CSI-RSRP in qdbm; it is available only when camping on NR network; 0x7FFF: invalid; invalid value is reported by default */
    int32_t                                            second_sig3; /* CSI-SINR in qdb; it is available only when camping on NR network; 0x7FFF: invalid; invalid value is reported by default */
    uint8_t                                            signal_type; /* N/A */
} mipc_nw_raw_signal_info_struct4 ;

typedef struct mipc_nw_raw_signal_info_v1_struct4 {
    int32_t                                            sig1; /* 0 ~ 63: This field is identical to <rscp> in +CESQ when camping on GSM service;
            0 ~ 96: This field is identical to <rscp> in +CESQ when camping on UMTS service;
            0 ~ 34: This field is identical to <rsrq> in +CESQ when camping on LTE network;
            255: It indicates invalid value
 */
    int32_t                                            sig2; /* 0 ~ 7: BER when camping on GSM;
            0 ~ 49: EcNo when camping on UMTS;
            0 ~ 97: RSRP when camping on LTE
 */
    int32_t                                            rssi_in_qdbm; /* Negative integer; received signal strength raw data in quarter dbm; 1: invalid */
    int32_t                                            rscp_in_qdbm; /* Negative integer; RSCP in quarter dbm; it is available only when camping on UMTS network; 1: invalid */
    int32_t                                            ecn0_in_qdbm; /* Negative integer; EcNo in quarter dbm; it is available only when camping on UMTS network; 1: invalid */
    int32_t                                            rsrq_in_qdbm; /* Negative integer;
            GSM/UMTS/LTE/C2K: RSRQ in quarter dB; it is available only when camping on LTE network; 1: invalid;
            NR: SS-RSRQ in qdb; 0x7FFF: invalid
 */
    int32_t                                            rsrp_in_qdbm; /* Negative integer;
            GSM/UMTS/LTE/C2K: RSRP in quarter dbm; it is available only when camping on LTE network; 1: invalid;
            NR: SS-RSRP in qdbm; 0x7FFF: invalid
 */
    int32_t                                            eact; /*  */
    int32_t                                            sig3; /* GSM/UMTS/LTE/C2K: RSSNR in quarter dB; it is available only when camping on LTE network; 0x7FFF: invalid;
            NR: SS-SINR in qdb; 0x7FFF: invalid
 */
    int32_t                                            serv_band; /* Serving LTE band */
    int32_t                                            second_rsrq_in_qdbm; /* CSI-RSRQ in qdb; it is available only when camping on NR network; 0x7FFF: invalid; invalid value is reported by default */
    int32_t                                            second_rsrp_in_qdbm; /* CSI-RSRP in qdbm; it is available only when camping on NR network; 0x7FFF: invalid; invalid value is reported by default */
    int32_t                                            second_sig3; /* CSI-SINR in qdb; it is available only when camping on NR network; 0x7FFF: invalid; invalid value is reported by default */
    uint8_t                                            signal_type; /* N/A */
    uint8_t                                            padding[3]; /* Padding */
} mipc_nw_raw_signal_info_v1_struct4 ;

typedef struct mipc_physical_channel_info_struct4 {
    uint32_t                                           rat; /* RAT info. */
    uint8_t                                            status; /* UE and CELL connection status. */
    int32_t                                            cell_bandwidth_downlink; /* Downlink bandwidth for cell. */
    int32_t                                            channel_number; /*  */
    int32_t                                            num_cids; /*  */
    uint32_t                                           physical_cell_id; /*  */
} mipc_physical_channel_info_struct4 ;

typedef struct mipc_physical_channel_info_v1_struct4 {
    uint32_t                                           rat; /* RAT info. */
    uint8_t                                            status; /* UE and CELL connection status. */
    int32_t                                            cell_bandwidth_downlink; /* Downlink bandwidth for cell. */
    int32_t                                            cell_bandwidth_uplink; /* Uplink bandwidth for cell. */
    int32_t                                            downlink_channel_number; /* downlink channel number */
    int32_t                                            uplink_channel_number; /* uplink channel number */
    int32_t                                            num_cids; /*  */
    uint32_t                                           physical_cell_id; /*  */
    uint32_t                                           band; /*  */
    uint32_t                                           frequency_range; /*  */
} mipc_physical_channel_info_v1_struct4 ;

typedef struct mipc_physical_channel_info_v2_struct4 {
    uint32_t                                           rat; /* RAT info. */
    uint8_t                                            status; /* UE and CELL connection status. */
    uint8_t                                            padding[3]; /* Padding */
    int32_t                                            cell_bandwidth_downlink; /* Downlink bandwidth for cell. */
    int32_t                                            cell_bandwidth_uplink; /* Uplink bandwidth for cell. */
    int32_t                                            downlink_channel_number; /* downlink channel number */
    int32_t                                            uplink_channel_number; /* uplink channel number */
    int32_t                                            num_cids; /*  */
    uint32_t                                           physical_cell_id; /*  */
    uint32_t                                           band; /*  */
    uint32_t                                           frequency_range; /*  */
} mipc_physical_channel_info_v2_struct4 ;

typedef struct mipc_nw_name_pair_struct4 {
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* N/A */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* N/A */
} mipc_nw_name_pair_struct4 ;

typedef struct mipc_nw_band_power_pair_struct4 {
    uint32_t                                           band; /* band value */
    uint32_t                                           power; /* power value */
} mipc_nw_band_power_pair_struct4 ;

typedef struct mipc_nw_suggested_struct4 {
    uint8_t                                            state; /* 0: unknown, 1: available, 2: current, 3: forbidden */
    char                                               network_long_name[60]; /* Network long Name, format: ASCII */
    char                                               network_short_name[60]; /* Network short Name, format: ASCII */
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* PLMN id, format: ASCII (Number: 0-9) */
    char                                               lac[10]; /* Location area code. */
    uint32_t                                           act; /* Bit 0: GSM_AcTn; 0, not selected; 1 selected;
            Bit 1: GSM_Compact_AcTn; 0, not selected; 1 selected;
            Bit 2: UTRAN_AcTn; 0, not selected; 1 selected;
            Bit 3: E-UTRAN_AcTn; 0, not selected; 1 selected;
            0 GSM
            1 GSM Compact
            2 UTRAN
            3 GSM w/EGPRS (see NOTE 1)
            4 UTRAN w/HSDPA (see NOTE 2)
            5 UTRAN w/HSUPA (see NOTE 2)
            6 UTRAN w/HSDPA and HSUPA (see NOTE 2)
            7 E-UTRAN
            8 EC-GSM-IoT (A/Gb mode) (see NOTE 3)
            9 E-UTRAN (NB-S1 mode) (see NOTE 4)
            10 E-UTRA connected to a 5GCN (see NOTE 5)
            11 NR connected to a 5GCN (see NOTE 5)
            12 NG-RAN
            13 E-UTRA-NR dual connectivity
 */
} mipc_nw_suggested_struct4 ;

typedef struct mipc_nw_suggested_v1_struct4 {
    uint8_t                                            state; /* 0: unknown, 1: available, 2: current, 3: forbidden */
    char                                               network_long_name[60]; /* Network long Name, format: ASCII */
    char                                               network_short_name[60]; /* Network short Name, format: ASCII */
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* PLMN id, format: ASCII (Number: 0-9) */
    char                                               lac[10]; /* Location area code. */
    uint8_t                                            padding[2]; /* Padding */
    uint32_t                                           act; /* Bit 0: GSM_AcTn; 0, not selected; 1 selected;
            Bit 1: GSM_Compact_AcTn; 0, not selected; 1 selected;
            Bit 2: UTRAN_AcTn; 0, not selected; 1 selected;
            Bit 3: E-UTRAN_AcTn; 0, not selected; 1 selected;
            0 GSM
            1 GSM Compact
            2 UTRAN
            3 GSM w/EGPRS (see NOTE 1)
            4 UTRAN w/HSDPA (see NOTE 2)
            5 UTRAN w/HSUPA (see NOTE 2)
            6 UTRAN w/HSDPA and HSUPA (see NOTE 2)
            7 E-UTRAN
            8 EC-GSM-IoT (A/Gb mode) (see NOTE 3)
            9 E-UTRAN (NB-S1 mode) (see NOTE 4)
            10 E-UTRA connected to a 5GCN (see NOTE 5)
            11 NR connected to a 5GCN (see NOTE 5)
            12 NG-RAN
            13 E-UTRA-NR dual connectivity
 */
} mipc_nw_suggested_v1_struct4 ;

typedef struct mipc_nw_arfcn_struct4 {
    uint32_t                                           arfcn; /* N/A */
} mipc_nw_arfcn_struct4 ;

typedef struct mipc_nw_femtocell_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* ID of the PLMN */
    char                                               oper_long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Operator long name of the femtocell */
    char                                               oper_short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Operator short name of the femtocell */
    uint8_t                                            act; /* The act of the femtocell; 0: GSM; 2: UMTS; 7: LTE */
    uint8_t                                            num_csg; /* The num_csg of the femtocell */
    uint32_t                                           csg_id; /* The CSG ID of the femtocell */
    uint8_t                                            csg_type; /* The CSG type of the femtocell */
    char                                               hnb_name[50]; /* The hnb_name of the femtocell */
    char                                               sig[7]; /* The signal strength of the femtocell */
} mipc_nw_femtocell_info_struct4 ;

typedef struct mipc_nw_femtocell_info_v1_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* ID of the PLMN */
    char                                               oper_long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Operator long name of the femtocell */
    char                                               oper_short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Operator short name of the femtocell */
    uint8_t                                            act; /* The act of the femtocell; 0: GSM; 2: UMTS; 7: LTE */
    uint8_t                                            num_csg; /* The num_csg of the femtocell */
    uint8_t                                            padding1[3]; /* Padding */
    uint32_t                                           csg_id; /* The CSG ID of the femtocell */
    uint8_t                                            csg_type; /* The CSG type of the femtocell */
    char                                               hnb_name[50]; /* The hnb_name of the femtocell */
    char                                               sig[7]; /* The signal strength of the femtocell */
    uint8_t                                            padding2[2]; /* Padding */
} mipc_nw_femtocell_info_v1_struct4 ;

typedef struct mipc_nw_pseudocell_info_struct4 {
    uint8_t                                            type; /* the type of the pseudocell. */
    char                                               plmn[MIPC_MAX_PLMN_ID_LEN]; /* the PLMN id of the pseudocell. */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* the PLMN long name of the pseudocell. */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* the plmn short name of the pseudocell. */
    uint32_t                                           lac; /* the LAC of the pseudocell. */
    uint64_t                                           cell_id; /* the cell id of the pseudocell. */
    uint16_t                                           arfcn; /* the ARFCN of the pseudocell. */
    uint8_t                                            bsic; /* the bsic of the pseudocell. */
    char                                               si3_raw_data[50]; /* the si3_raw_data of the pseudocell. */
} mipc_nw_pseudocell_info_struct4 ;

typedef struct mipc_nw_barring_info_struct4 {
    uint16_t                                           service_type; /* indicates the service type of the barring service. */
    uint8_t                                            barring_type; /* indicates the type of barring applied to the service. 0: NONE, 1: CONDITIONAL, 2: UNCONDITIONAL. */
    uint8_t                                            factor; /* indicates the barring factor as a percentage 0-100. */
    uint16_t                                           time_seconds; /* indicates the number of seconds between re-evaluations of barring. */
    uint8_t                                            is_barred; /* indicates whether barring is currently being applied. */
} mipc_nw_barring_info_struct4 ;

typedef struct mipc_nw_barring_info_v1_struct4 {
    uint16_t                                           service_type; /* indicates the service type of the barring service. */
    uint8_t                                            barring_type; /* indicates the type of barring applied to the service. 0: NONE, 1: CONDITIONAL, 2: UNCONDITIONAL. */
    uint8_t                                            factor; /* indicates the barring factor as a percentage 0-100. */
    uint16_t                                           time_seconds; /* indicates the number of seconds between re-evaluations of barring. */
    uint8_t                                            is_barred; /* indicates whether barring is currently being applied. */
    uint8_t                                            padding; /* Padding */
} mipc_nw_barring_info_v1_struct4 ;

typedef struct mipc_nw_threshold_array_struct4 {
    uint8_t                                            num_of_threshold; /* Threshold number; maximum: 10; 0: disable the use of threshold for reporting */
    int32_t                                            threshold_dbm[10]; /* An array of triggered thresholds in dBm */
} mipc_nw_threshold_array_struct4 ;

typedef struct mipc_nw_threshold_array_v1_struct4 {
    uint8_t                                            num_of_threshold; /* Threshold number; maximum: 10; 0: disable the use of threshold for reporting */
    uint8_t                                            padding[3]; /* Padding */
    int32_t                                            threshold_dbm[10]; /* An array of triggered thresholds in dBm */
} mipc_nw_threshold_array_v1_struct4 ;

typedef struct mipc_nw_extend_provider_struct4 {
    uint32_t                                           index; /* Network index in the SIM Preferred network list */
    uint32_t                                           stat; /* 0: unknown
            1: available
            2: current
            3: forbidden
 */
    char                                               oper_long_name[60]; /* Operator long name, format: ASCII */
    char                                               oper_short_name[60]; /* Operator short name, format: ASCII */
    char                                               oper_numeric_name[MIPC_MAX_PLMN_ID_LEN]; /* Operator numeric name, format: ASCII */
    uint8_t                                            reserved[1]; /* Reserved */
    char                                               lac[20]; /* Location information, format: ASCII */
    uint32_t                                           act; /* Refer to NW_ACT_TYPE */
    uint32_t                                           csq_rssi; /* Coded value */
    uint32_t                                           register_state; /* Register state */
    uint32_t                                           timestamp_type; /* Timestamp type */
    int32_t                                            timestamp; /* Timestamp */
    uint32_t                                           connection_stat; /* Connection state */
    char                                               cell_id[20]; /* Cell ID */
    uint32_t                                           freq; /* Frequency */
    int32_t                                            bsic_psc_cpid_pci; /* BSIC/PSC/CPID/PCI */
    int32_t                                            sig2; /*  */
    int32_t                                            sig3; /*  */
    int32_t                                            sig4; /*  */
    int32_t                                            sig5; /*  */
    uint32_t                                           bit_error_rat; /* Bit Error Ratio */
    uint32_t                                           timing_advance; /* Timing advance */
    uint32_t                                           cqi; /* CQI */
} mipc_nw_extend_provider_struct4 ;

typedef struct mipc_nw_ecainfo_struct4 {
    uint32_t                                           ca_info; /* indicate the CA state. 0: CA CONFIGURED, 1: CA NOT CONFIGURED, 2: CA ACTIVE, 3: CA DEACTIVATED */
    uint32_t                                           pcell_bw; /* indicate pcell bandwidth */
    uint32_t                                           scell_bw1; /* indicate first scell bandwidth */
    uint32_t                                           scell_bw2; /* indicate second scell bandwidth */
    uint32_t                                           scell_bw3; /* indicate third scell bandwidth */
    uint32_t                                           scell_bw4; /* indicate fourth scell bandwidth */
} mipc_nw_ecainfo_struct4 ;

typedef struct mipc_1xrtt_cell_info_struct4 {
    int32_t                                            sid; /*  */
    int32_t                                            nid; /*  */
    int32_t                                            bsid; /*  */
} mipc_1xrtt_cell_info_struct4 ;

typedef struct mipc_band_combo_info_struct4 {
    char                                               band_combo[MIPC_MAX_BAND_COMBO_LEN]; /* band combo */
} mipc_band_combo_info_struct4 ;

typedef struct mipc_nw_ca_band_struct4 {
    uint32_t                                           ca_band_class[8]; /* CA band: bitmask, ca_band_class[0]'s bit0~bit31 indicating band1~band32, ca_band_class[1]'s bit0~bit31 band33~band64 and so on */
} mipc_nw_ca_band_struct4 ;

typedef struct mipc_nw_tuw_info_struct4 {
    uint8_t                                            tuw_id; /* 1: TUW1, 2: TUW2, 3: TUW3 */
    uint32_t                                           tuw_length; /* TUW1: 0~30 seconds, default setting of 3 seconds.  TUW2: 0~120 seconds, default setting of 60 seconds.  TUW3: 0~30 seconds, default setting of 2 seconds. */
} mipc_nw_tuw_info_struct4 ;

typedef struct mipc_nw_tuw_info_v1_struct4 {
    uint8_t                                            tuw_id; /* 1: TUW1, 2: TUW2, 3: TUW3 */
    uint8_t                                            padding[3]; /* Padding */
    uint32_t                                           tuw_length; /* TUW1: 0~30 seconds, default setting of 3 seconds.  TUW2: 0~120 seconds, default setting of 60 seconds.  TUW3: 0~30 seconds, default setting of 2 seconds. */
} mipc_nw_tuw_info_v1_struct4 ;

typedef struct mipc_nr_ca_band_struct4 {
    int32_t                                            band[10]; /* the nr ca band. Pcell always at band[0] if 2 or more bands present. */
} mipc_nr_ca_band_struct4 ;

typedef struct mipc_lte_bound_info_struct4 {
    uint32_t                                           sbp_id; /* 0: The first set of NVRAM parameters; 1: The second one; 2: The third one. */
    uint32_t                                           event_type; /* LTE Events. */
    uint32_t                                           threshold; /* 0: threshold1; 1: threshold2. */
    uint32_t                                           signal_quality_type; /* Signal quality type. */
    int32_t                                            upper_bound; /* upper bound. */
    int32_t                                            lower_bound; /* lower bound. */
} mipc_lte_bound_info_struct4 ;

typedef struct mipc_nw_allowed_mcc_list_struct4 {
    uint32_t                                           allowed_mcc_number; /* The number of allowed mcc. */
    uint16_t                                           allowed_mcc[20]; /* allowed mcc */
} mipc_nw_allowed_mcc_list_struct4 ;

typedef struct mipc_os_id_info_struct4 {
    uint8_t                                            os_id[MIPC_UE_OS_ID_LEN]; /* the os_id info of the update or set. */
} mipc_os_id_info_struct4 ;

typedef struct mipc_nw_congestion_info_v1_struct4 {
    uint32_t                                           mode; /* 0: ul_data_stall; 1: dl_data_stall; 2: high bler; 3: high interference. */
    uint32_t                                           op; /* 0: Feature OFF; 1: smartHO when data stall; 2: SmartReEst when data stall; 3: both feature ON. */
    uint32_t                                           serv_type; /* 0: Internet bearer; 1.IMS bearer */
    uint32_t                                           rat; /* 0: LTE; 1. NR */
    uint32_t                                           det_time; /* uint:s; range:0~0xFF */
    uint32_t                                           duration; /* uint:s; range:0~0xFF */
    uint32_t                                           bar_duration; /* uint:s */
    uint32_t                                           cond_grant_bytes; /* unit:bytes; range:0~0xFFFFFFFF */
    uint32_t                                           cond_bsr; /* unit:BSR level, LTE 0~63, NR 0~254. */
    uint32_t                                           cond_pdcp_discard_rate; /* unit:%, 0~100 */
    int32_t                                            cond_phr; /* unit:dB, LTE -23~40, NR -32~40 */
    int32_t                                            cond_s_rsrp; /* unit:dBm, LTE:-150~-30 , NR:-156~-31 */
    int32_t                                            cond_s_sinr; /* unit:dB, LTE:-20~40, NR: -20~40 */
    int32_t                                            cond_n_rsrp; /* unit:dBm, LTE:-150~-30 , NR:-156~-31 */
    int32_t                                            cond_n_sinr; /* unit:dB, LTE:-20~40, NR: -20~40 */
    uint32_t                                           cond_ul_bler; /* Uplink BLER */
    uint32_t                                           cond_dl_bler; /* Downlink BLER */
    uint32_t                                           cond_ul_tput; /* Uplink T-put */
    uint32_t                                           cond_dl_tput; /* Downlink T-put */
    uint32_t                                           cond_ul_alloc_rate; /* UL alloction rate */
    uint32_t                                           cond_dl_alloc_rate; /* DL alloction rate */
    uint32_t                                           cond_ul_pending_data; /* Total uplink pending data, unit: bytes. */
} mipc_nw_congestion_info_v1_struct4 ;

typedef struct mipc_nw_record_info_struct4 {
    uint32_t                                           rat; /* 1: GSM, 2: UMTS, 4: LTE, 128: NR, refer to NW_SCAN_RAT */
    uint32_t                                           num_band; /* 0~8, 0 means search all supported bands */
    uint32_t                                           band[8]; /* band, 0~1024 */
    uint32_t                                           num_channel; /* 0~32, 0 means search all channels */
    uint32_t                                           channel[32]; /* arfcn, 0~3279165 */
} mipc_nw_record_info_struct4 ;

typedef struct mipc_nw_plmn_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* plmn id */
} mipc_nw_plmn_info_struct4 ;

typedef struct mipc_nw_nr_signal_strength_v2_struct4 {
    int32_t                                            signal_strength; /* Valid values are (0-31, 99) as defined in TS27.007 8.5. */
    int32_t                                            ss_rsrp; /* SS reference signal received power, multipled by -1, Range[44, 140]. */
    int32_t                                            ss_rsrq; /* SS reference signal received quality, multipled by -1, Range[3, 20]. */
    int32_t                                            ss_sinr; /* SS signal-to-noise and interference ratio. Range[-23, 40]. */
    int32_t                                            csi_rsrp; /* CSI reference signal received power, multipled by -1. Range[44, 140] */
    int32_t                                            csi_rsrq; /* CSI reference signal received quality, multipled by -1. Range[3, 20] */
    int32_t                                            csi_sinr; /* CSI signal-to-noise and interference ratio. Range[-23, 40] */
    uint8_t                                            csi_cqi_table_index; /* CSI cqi table index */
    uint8_t                                            csi_cqi_report_num; /* CSI cqi report num */
    uint8_t                                            csi_cqi_report_list[19]; /* CSI cqi report info */
    int32_t                                            timing_advance; /* Timing advance in micro seconds for a one way trip from cell to device. Range(0 - 0x7ffffffe) */
} mipc_nw_nr_signal_strength_v2_struct4 ;

typedef struct mipc_nw_scan_info_struct4 {
    uint32_t                                           rat; /* 1: GSM, 2: UMTS, 4: LTE, 128: NR, refer to NW_SCAN_RAT */
    int32_t                                            xarfcn; /* arfcn for GSM, uarfcn for UMTS, earfcn for LTE, nrarfcn for NR */
    int32_t                                            pcid; /* BSIC for GSM, PSC(Primary scrambling code) for UMTS, PCI(Physical cell ID) for LTE and NR */
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* plmn id */
    char                                               cell_id[MIPC_MAX_CELL_ID_LEN]; /*  cell ID */
    char                                               lac_or_tac[20]; /* LAC for GSM and UMTS, TAC for LTE and NR */
    int32_t                                            plmn_status; /* 0: invalid, 1: available, 2: current, 3: forbidden, refer to NW_PLMN_STATUS */
    int32_t                                            registered; /* 0: not registered, 1: registered, refer to NW_REGISTER_STATUS */
    int32_t                                            srv_cell_status; /* 0: none, 1: primary serving cell, 2: secondary serving cell, refer to NW_SRV_CELL_STATUS */
    int32_t                                            sig1; /* GSM: rxlev, WCDMA: rxlev, TDSCDMA : rssi in dbm, LTE: rxlev, NR: ss-rsrp in rxlev */
    int32_t                                            sig2; /* GSM: rssi in qdbm, UMTS: rssi in qdbm, LTE: rssi in qdbm, NR: ss-rsrq in rxlev */
    int32_t                                            sig3; /* GSM: N/A, WCDMA: rscp, TDSCDMA: N/A, LTE: rsrp in qdbm, NR: ss-rsrp in qdbm */
    int32_t                                            sig4; /* GSM: N/A, WCDMA: ecno, TDSCDMA: N/A, LTE: rsrq in qdbm, NR: ss-rsrq in qdbm */
    int32_t                                            sig5; /* GSM: N/A, UMTS: N/A, LTE: snr in qdbm, NR: ss-snr in qdbm */
} mipc_nw_scan_info_struct4 ;

typedef struct mipc_sim_msisdn_struct4 {
    uint8_t                                            msisdn[24]; /* N/A */
} mipc_sim_msisdn_struct4 ;

typedef struct mipc_sim_pin_desc_struct4 {
    uint8_t                                            pin_mode; /* PIN mode */
    uint8_t                                            pin_format; /* PIN format */
    uint8_t                                            pin_len_min; /* Minimum PIN length */
    uint8_t                                            pin_len_max; /* Maximum PIN length */
} mipc_sim_pin_desc_struct4 ;

typedef struct mipc_sim_app_info_struct4 {
    uint8_t                                            app_type; /* The type of UICC application */
    uint8_t                                            app_id_len; /* The length of UICC application ID */
    uint8_t                                            sim_app_id[MIPC_MAX_SIM_AID_BYTE_LEN]; /* UICC application ID */
    uint8_t                                            name_len; /* The length of application name */
    uint8_t                                            name[32]; /* Application name */
    uint8_t                                            num_of_pins; /* Number of PINs; only support PIN1; not support UPIN; maximum number is 8 */
    uint8_t                                            pin_ref[8]; /* PIN reference; not support different application keys for different applications; maximum size is 8 */
} mipc_sim_app_info_struct4 ;

typedef struct mipc_sim_slots_info_struct4 {
    char                                               card_state[20]; /* Response of AT+CPIN? */
    uint8_t                                            slots_state; /* Slots state of physical slots: 0 means inactive; 1 means active */
    uint8_t                                            logical_idx; /* Logical slots index of physical slot */
    uint8_t                                            reserved[2]; /* Reserved */
    char                                               atr[MIPC_MAX_ATR_LEN]; /* The ATR of the card */
    char                                               eid[MIPC_MAX_SIM_EID_LEN]; /* The EID of the card */
    char                                               iccid[MIPC_SIM_FIX_ICCID_LEN]; /* SIM ICCID of the card */
} mipc_sim_slots_info_struct4 ;

typedef struct mipc_eid_byte_struct_struct4 {
    uint8_t                                            is_valid; /* A flag indicates whether the EID is valid. 0: EID is invalid, 1: EID is valid */
    uint8_t                                            eid[MIPC_FIX_SIM_EID_BYTE_LEN]; /* The EID in Octet16 format (GSMA SGP.22 spec) */
} mipc_eid_byte_struct_struct4 ;

typedef struct mipc_port_info_struct4 {
    char                                               iccid[MIPC_SIM_FIX_ICCID_STR_LEN]; /* For MEP sim, it is ICCID string of the LSI; for a lagecy sim or eSIM which does not support MEP, it means the iccid of the physical sim */
    char                                               reserved1[3]; /* Reserved */
    uint8_t                                            lsi; /* For MEP sim, it means logical SE interface of physical slot;  for a lagecy sim or eSIM which does not support MEP it is always 0 */
    uint8_t                                            port_active; /* For MEP sim, it means the logical SE interface is active or not;for a lagecy sim or eSIM which does not support MEP, it means this physical slot is used or not */
    char                                               reserved2[2]; /* Reserved */
} mipc_port_info_struct4 ;

typedef struct mipc_app_status_desc_struct4 {
    uint8_t                                            app_type; /* Application type */
    uint8_t                                            app_state; /* Application state */
    uint8_t                                            pin_status; /* Same as CPIN state */
    uint8_t                                            sub_status; /* Application sub-status */
    uint8_t                                            pin1_replaced; /* PIN1 applied to all applications */
    uint8_t                                            pin1_state; /* PIN1 state */
    uint8_t                                            pin2_state; /* PIN2 state */
    uint8_t                                            reserved1; /* Reserved */
    char                                               aid[41]; /* AID (MAX_SIM_AID_STR_LEN+1) */
    uint8_t                                            reserved2[3]; /* Reserved */
    char                                               app_label[33]; /* Application label (MAX_SIM_APP_LABEL_LEN+1) */
    uint8_t                                            reserved3[3]; /* Reserved */
} mipc_app_status_desc_struct4 ;

typedef struct mipc_file_list_struct4 {
    char                                               path[9]; /* Path ID + File ID */
    char                                               data[50]; /* SW1SW2 + data string */
} mipc_file_list_struct4 ;

typedef struct mipc_sid_nid_list_struct4 {
    char                                               sid[6]; /* sid */
    uint8_t                                            reserved[2]; /* Reserved */
    char                                               nid[6]; /* nid */
} mipc_sid_nid_list_struct4 ;

typedef struct mipc_sim_carrier_struct_struct4 {
    char                                               mcc[4]; /* MCC */
    char                                               mnc[4]; /* MNC */
    uint8_t                                            match_cat; /* Carrier match type */
    char                                               match_data[21]; /* SPN/IMSI Prefix/GID1/GID2 data; data is null when carrier match type is 0 */
} mipc_sim_carrier_struct_struct4 ;

typedef struct mipc_sms_pdu_struct4 {
    uint16_t                                           message_index; /* Message index */
    uint8_t                                            pdu_len; /* The length of TPDU (without SCA) */
    uint8_t                                            status; /* Message status */
    uint8_t                                            sms_class; /* Message class */
    uint8_t                                            reserved[3]; /* Reserved */
    uint8_t                                            pdu[256]; /* PDU contains SCA and TPDU */
} mipc_sms_pdu_struct4 ;

typedef struct mipc_ss_call_forward_struct4 {
    uint8_t                                            call_forward_status; /* call forward status, active or not active */
    char                                               dial_number[MIPC_MAX_SS_DIAL_NUMBER_FOR_ALIGN_LEN]; /* dial number */
    uint16_t                                           service_class; /* service class */
    uint16_t                                           type_of_address; /* type of address, normal or international */
    uint8_t                                            reserved1[3]; /* Reserved */
    uint8_t                                            timer; /* timer, 1-30 */
    uint8_t                                            reason; /* call forward reason to query from req */
    uint8_t                                            reserved2[3]; /* Reserved */
    char                                               time_slot_begin[MIPC_MAX_SS_COMM_INFO_LEN]; /* time slot begin of call forward */
    char                                               time_slot_end[MIPC_MAX_SS_COMM_INFO_LEN]; /* time slot end of call forward */
} mipc_ss_call_forward_struct4 ;

typedef struct mipc_ss_call_waiting_struct4 {
    uint8_t                                            call_waiting_status; /* call waiting status, active or not active */
    uint8_t                                            padding; /* padding for align */
    uint16_t                                           service_class; /* call waiting service class */
} mipc_ss_call_waiting_struct4 ;

typedef struct mipc_ss_cb_dialnumber_struct4 {
    uint8_t                                            number_status; /* the call barring number status, active or not active */
    char                                               dial_number[MIPC_MAX_SS_DIAL_NUMBER_FOR_ALIGN_LEN]; /* dial number */
} mipc_ss_cb_dialnumber_struct4 ;

typedef struct mipc_sys_mapping_struct4 {
    uint32_t                                           ps_id; /* N/A */
} mipc_sys_mapping_struct4 ;

typedef struct mipc_sys_lte_band_struct4 {
    uint32_t                                           lte_band_class[8]; /* LTE band: bitmask, lte_band_class[0]'s bit0~bit31 indicating band1~band32, lte_band_class[1]'s bit0~bit31 indicating band33~band64, and so on */
} mipc_sys_lte_band_struct4 ;

typedef struct mipc_sys_nr_band_struct4 {
    uint32_t                                           nr_band_class[32]; /* Bands for NR: bitmask, nr_band_class[0]'s bit0~bit31 indicating band1~band32, nr_band_class[1]'s bit0~bit31 indicating band33~band64 and so on */
} mipc_sys_nr_band_struct4 ;

typedef struct mipc_sys_thermal_sensor_config_struct4 {
    uint32_t                                           enable; /* 0: disable, others: enable. */
    uint32_t                                           sensor_id; /* Sensor ID (start from zero). */
    uint32_t                                           alarm_id; /* 0, 1 (support 2 alarms per sensor). */
    int32_t                                            threshold; /* Threshold temperature for compare and notify (unit: /10 degC). Value must < {warn,max,hw_shutdown}_temp. Value must > min_temp. */
    uint32_t                                           hysteresis; /* If rising = (threshold - hysteresis) or falling = (threshold + hysteresis), the polling interval of the sensor will be cut to the interval value shown in field below (unit: /10 degC). */
    uint32_t                                           interval; /* SW sampling interval (value must >= min_sampling_period) (unit: ms). */
    uint32_t                                           alarm_type; /* 0: rising alarm, 1: falling alarm, others: disable. */
} mipc_sys_thermal_sensor_config_struct4 ;

typedef struct mipc_sys_thermal_sensor_config_e_struct4 {
    uint32_t                                           enable; /* 0: disable this trip point, 1: enable this trip point */
    uint32_t                                           sensor_id; /* Sensor ID (start from zero). */
    uint32_t                                           alarm_id; /* 0, 1 (support 2 alarms per sensor). */
    int32_t                                            threshold; /* Threshold temperature for compare and notify (unit: /10 degC). Value must < {warn,max,hw_shutdown}_temp. Value must > min_temp. */
    uint32_t                                           hysteresis; /* If rising = (threshold - hysteresis) or falling = (threshold + hysteresis), the polling interval of the sensor will be cut to the interval value shown in field below (unit: /10 degC). */
    uint32_t                                           interval; /* SW sampling interval (value must >= min_sampling_period) (unit: ms). */
    uint32_t                                           alarm_type; /* 0: rising alarm, 1: falling alarm, others: disable. */
} mipc_sys_thermal_sensor_config_e_struct4 ;

typedef struct mipc_sys_thermal_actuator_state_info_struct4 {
    uint32_t                                           actuator_id; /* Actuator ID (start from one). */
    uint32_t                                           num_throttling_state; /* Number of throttling state, state range is from 0 to num_throttling_state. */
    uint32_t                                           current_throttling_state; /* Current throttling state. */
    char                                               actuator_name[MIPC_MAX_SYS_THERMAL_ACTUATOR_NAME]; /* Actuator name. */
} mipc_sys_thermal_actuator_state_info_struct4 ;

typedef struct mipc_sys_thermal_sensor_info_struct4 {
    uint32_t                                           sensor_id; /* Sensor ID */
    int32_t                                            temperature; /* Current temperature */
} mipc_sys_thermal_sensor_info_struct4 ;

typedef struct mipc_sys_thermal_sensor_info_e_struct4 {
    uint32_t                                           sensor_id; /* Sensor ID (start from zero). */
    int32_t                                            temperature; /* Current temperature (unit: /10 degC). */
} mipc_sys_thermal_sensor_info_e_struct4 ;

typedef struct mipc_sys_thermal_trip_map_struct4 {
    uint32_t                                           val_trip_map; /* 0: disable this trip point, 1: enable this trip point */
    int32_t                                            threshold_value; /* Threshold temperature for compare and notify (unit: /10 degC). Value must < {warn,max,hw_shutdown}_temp. Value must > min_temp. */
    uint32_t                                           hysteresis_value; /* If rising = (threshold - hysteresis) or falling = (threshold + hysteresis), the polling interval of the sensor will be cut to the interval value shown in field below (unit: /10 degC). */
    uint32_t                                           sampling_period; /* SW sampling interval (value must >= min_sampling_period) (unit: ms). */
    uint32_t                                           sensor_alarm_type; /* 0: rising alarm, 1: falling alarm (support 2 alarms per sensor). */
    uint32_t                                           throttle_level; /* The throttle level coressponding to this trip point. */
} mipc_sys_thermal_trip_map_struct4 ;

typedef struct mipc_sys_thermal_trip_change_struct4 {
    uint32_t                                           sensor_id; /* Target sensor ID (start from zero). */
    int32_t                                            before_trip_temp; /* Trip point temperature (before modify). */
    int32_t                                            after_trip_temp; /* Trip point temperature (after modify). */
    uint32_t                                           save_cfg_to_nv; /* Save modified trip point temperature information to NVRAM. 0: no; others: yes */
} mipc_sys_thermal_trip_change_struct4 ;

typedef struct mipc_sys_mia_metrics_struct4 {
    uint32_t                                           type; /* N/A */
    uint32_t                                           latency; /* N/A */
    uint32_t                                           ul_tput; /* N/A */
    uint32_t                                           dl_tput; /* N/A */
} mipc_sys_mia_metrics_struct4 ;

typedef struct mipc_call_detail_info_struct4 {
    uint8_t                                            number_present; /* This parameter can provide details why <number> does not contain a calling party BCD number (refer 3GPP TS 24.008 [8] subclause 10.5.4.30). */
    uint8_t                                            reserved1[3]; /* Reserved */
    char                                               number[MIPC_MAX_CALL_NUMBER_LEN]; /* call number */
    uint8_t                                            name_present; /* this field don't support MIPC_CALL_CNI_VALIDITY_CNI_PAYPHONE and MPIC_CALL_CNI_VALIDITY_CNI_OTHERS */
    uint8_t                                            reserved2[3]; /* Reserved */
    char                                               name[MIPC_MAX_CALL_NAME_LEN]; /* call name */
    char                                               display_name[MIPC_MAX_CALL_DISPLAY_NAME_LEN]; /* display name */
    char                                               verstat[MIPC_MAX_CALL_VERSTAT_LEN]; /* verstat */
    char                                               privacy[MIPC_MAX_CALL_PRIVACY_LEN]; /* privacy */
} mipc_call_detail_info_struct4 ;

typedef struct mipc_call_video_cap_struct4 {
    uint8_t                                            local_video_cap_present; /* This field will be always TRUE currently */
    uint8_t                                            local_video_cap; /* local video capability */
    uint8_t                                            remote_video_cap_present; /* this field will be TRUE after IMS call connected  */
    uint8_t                                            remote_video_cap; /* 1 for remote UE video-capable, 0 for remote UE no video-capble. If video feature tag is included in received SIP messages, then this value is set to 1 */
} mipc_call_video_cap_struct4 ;

typedef struct mipc_ecc_info_struct4 {
    uint8_t                                            is_fake_ecc; /* This field indicates that call ECC number through normal call */
    uint8_t                                            reserved1[3]; /* Reserved */
    uint16_t                                           category; /* A bitmap indicating the Emergency Service Category Value according to 3GPP TS 24.008 */
    uint16_t                                           type; /* the type of ecc number */
    char                                               number[MIPC_MAX_CALL_ECC_NUMBER_LEN]; /* the ecc number */
    uint8_t                                            reserved2[2]; /* Reserved */
} mipc_ecc_info_struct4 ;

typedef struct mipc_nw_srxlev_info_struct4 {
    uint16_t                                           srxlev_in_qdb; /* srxlev in qdb value */
    uint16_t                                           squal_in_qdb; /* squal in qdb value */
} mipc_nw_srxlev_info_struct4 ;

typedef struct mipc_phb_name_str_struct4 {
    char                                               name_str[168]; /* phb name string. */
    uint32_t                                           encode_method; /* phb name string encode method. */
} mipc_phb_name_str_struct4 ;

typedef struct mipc_phb_email_str_struct4 {
    char                                               name_str[248]; /* phb name string. */
    uint32_t                                           encode_method; /* phb name string encode method. */
} mipc_phb_email_str_struct4 ;

typedef struct mipc_phb_anr_entry_struct4 {
    uint32_t                                           type; /* type of the entry,refer to storage type. */
    uint32_t                                           index; /* the stored index of the entry */
    uint32_t                                           ton; /* type of the number. */
    char                                               number[40]; /* number string. */
} mipc_phb_anr_entry_struct4 ;

typedef struct mipc_phb_entry_struct4 {
    uint32_t                                           index; /* the stored index of the entry */
    uint32_t                                           ton; /* type of the number. */
    char                                               number[40]; /* number string. */
    char                                               alphaID[168]; /* alphaID string. */
    char                                               adnumber[40]; /* additional number string. */
    uint32_t                                           adtype; /* additional number type. */
    char                                               email[248]; /* email string. */
    uint32_t                                           encode_method; /* encode method type. */
} mipc_phb_entry_struct4 ;

typedef struct mipc_embms_area_id_info_struct4 {
    uint8_t                                            num_area_ids; /* The number of MBSFN area IDs available. Range is from 1 to 8. */
    uint8_t                                            reserved[3]; /* Reserved */
    uint8_t                                            area_id[8]; /* MBSFN area ID. Range is from 0 to 255. */
} mipc_embms_area_id_info_struct4 ;

typedef struct mipc_embms_session_info_struct4 {
    uint16_t                                           index; /* The index of MBMS session. */
    uint8_t                                            reserved1[2]; /* Reserved */
    char                                               tmgi[13]; /* TMGI(Temporary Mobile Group Identity)in position x in the list. It is represented in the form of a string of 11 or 12 hex digits. */
    uint8_t                                            reserved2[3]; /* Reserved */
    char                                               session_id[3]; /* Session ID in position x in the list. It is represented in the form of a string of 2 hex digits. */
    uint8_t                                            status; /* Status of the MBMS session. 0:MBMS session available(TMGI/Session ID present in MCCH), 1:MBMS session activated(modem receiving MTCH data). */
} mipc_embms_session_info_struct4 ;

typedef struct mipc_embms_nb_req_info_struct4 {
    uint8_t                                            num_nf; /* The number of neighboring frequencies. Range is from 0 to 8. */
    uint8_t                                            index; /* The index of neighboring frequency. Range is from 1 to 8. */
    uint8_t                                            padding[2]; /* padding. */
    uint32_t                                           nf; /* The neighboring frequency. */
    uint8_t                                            num_bands_nf; /* The number of neighboring frequency band in the list. Range is from 0 to 8. */
    uint8_t                                            reserved1[3]; /* Reserved */
    uint16_t                                           band_nf[8]; /* The neighboring frequency band. Range is from 0 to 256. */
    uint8_t                                            num_sais_nf; /* The number of SAIs in the list for neighbor frequency. Range is from 0 to 64? */
    uint8_t                                            reserved2[3]; /* Reserved */
    uint16_t                                           sai_nf[64]; /* The SAIs in SAI list for neighbor frequency. Range is from 0 to 65535. */
} mipc_embms_nb_req_info_struct4 ;

typedef struct mipc_embms_sai_cf_info_struct4 {
    uint8_t                                            num_intra_freq; /* The number of intra frequencies. Range is from 0 to 2. */
    uint8_t                                            padding[3]; /* padding. */
    uint32_t                                           intra_freq[2]; /* Intra frequencys. Range is from 0 to 262143. */
    uint8_t                                            num_sais; /* The number of SAIs in the list. Range is from 1 to 64. */
    uint8_t                                            reserved[3]; /* Reserved */
    uint16_t                                           sais[64]; /* The SAIs in SAI list. Range is from 0 to 65535. */
} mipc_embms_sai_cf_info_struct4 ;

typedef struct mipc_embms_sai_nf_info_struct4 {
    uint8_t                                            index; /* The index of neighboring. Range is from 1 to 8. */
    uint8_t                                            padding[3]; /* padding. */
    uint32_t                                           nf; /* The neighboring frequency . Range is from 0 to 262143. */
    uint8_t                                            num_bands_nf; /* The number of neighboring frequency band in the list. Range is from 0 to 8. */
    uint8_t                                            reserved1[3]; /* Reserved */
    uint16_t                                           band_nf[8]; /* The SAIs in SAI list. Range is from 0 to 256. */
    uint8_t                                            num_sais_nf; /* The number of SAIs in the list. Range is from 0 to 64. */
    uint8_t                                            reserved2[3]; /* Reserved */
    uint16_t                                           sai_nf[64]; /* The SAIs in the SAI list. Range is from 0 to 65535. */
} mipc_embms_sai_nf_info_struct4 ;

typedef struct mipc_internal_set_filter_req_struct4 {
    uint8_t                                            nccmni_net_if; /* N/A */
    uint8_t                                            reserve1; /* N/A */
    uint16_t                                           nccmni_seq; /* N/A */
    uint32_t                                           valid_field; /* mandatory, a bitmap; refer to IPC_FILTER_BY_XXX */
    uint8_t                                            ip_type; /* IPC_IP_TYPE_IPV4 or IPC_IP_TYPE_IPV6 */
    uint8_t                                            ctrl_protocol; /* IPC_HDR_PROT_XXX */
    uint16_t                                           src_port; /* N/A */
    uint16_t                                           dst_port; /* N/A */
    uint16_t                                           tcp_flags; /* IPC_HDR_TCP_FLAG_XXX */
    uint32_t                                           spi; /* N/A */
    uint8_t                                            src_v4_addr[4]; /* Source IPv4 address to filter. */
    uint8_t                                            dst_v4_addr[4]; /* Destination IPv4 address to filter. */
    uint8_t                                            src_v6_addr[16]; /* Source IPv6 address to filter. */
    uint8_t                                            dst_v6_addr[16]; /* Destination IPv6 address to filter. */
    uint8_t                                            icmpv4_type; /* Type field of ICMPv4 header to filter. */
    uint8_t                                            icmpv6_type; /* Type field of ICMPv6 header to filter. */
    uint16_t                                           reserve; /* N/A */
    uint32_t                                           features; /* N/A */
} mipc_internal_set_filter_req_struct4 ;

typedef struct mipc_internal_set_filter_cnf_struct4 {
    uint8_t                                            nccmni_net_if; /* N/A */
    uint8_t                                            reserve1; /* N/A */
    uint16_t                                           nccmni_seq; /* N/A */
    int32_t                                            filter_id; /* >=0: filter id, <0: set failed. */
} mipc_internal_set_filter_cnf_struct4 ;

typedef struct mipc_internal_reset_filter_req_struct4 {
    uint8_t                                            nccmni_net_if; /* N/A */
    uint8_t                                            reserve1; /* N/A */
    uint16_t                                           nccmni_seq; /* N/A */
    int32_t                                            is_deregister_all_filter; /* 0: deregister the filter id only, 1: deregister all filter */
    int32_t                                            filter_id; /* filter id to deregister */
} mipc_internal_reset_filter_req_struct4 ;

typedef struct mipc_internal_reset_filter_cnf_struct4 {
    uint8_t                                            nccmni_net_if; /* N/A */
    uint8_t                                            reserve1; /* N/A */
    uint16_t                                           nccmni_seq; /* N/A */
    int32_t                                            is_success; /* 1: success */
} mipc_internal_reset_filter_cnf_struct4 ;

typedef struct mipc_nw_tx_struct4 {
    uint32_t                                           tx[5]; /* N/A */
} mipc_nw_tx_struct4 ;

typedef struct mipc_plmn_cag_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* the PLMN ID of the cag list info. */
    uint8_t                                            reserved; /* Reserved */
    uint8_t                                            act; /* the act of the cag list info. */
    uint8_t                                            only_via_cag; /* the plmn can be access only via cag cell.0/1 */
    uint8_t                                            cag_id_info_count; /* the count of the cag id info */
    uint8_t                                            padding1; /* padding1 */
    uint32_t                                           cag_id; /* the cag id of the cag_info */
    char                                               hrnn[48]; /* the hrnn of the cag_info */
    uint8_t                                            cag_status; /* the cag status of the cag_info */
    uint8_t                                            padding2[3]; /* padding2 */
} mipc_plmn_cag_info_struct4 ;

typedef struct mipc_nw_congestion_info_struct4 {
    uint32_t                                           op; /* 0: Feature OFF; 1: smartHO when data stall; 2: SmartReEst when data stall; 3: both feature ON. */
    uint32_t                                           serv_type; /* 0: Internet bearer; 1.IMS bearer */
    uint32_t                                           rat; /* 0: LTE; 1. NR */
    uint32_t                                           det_time; /* uint:s; range:0~0xFFFFFFFF */
    uint32_t                                           duration; /* uint:s; range:0~0xFFFF */
    uint32_t                                           cond_grant_bytes; /* unit:bytes; range:0~0xFFFFFFFF */
    uint32_t                                           cond_bsr; /* unit:BSR level, LTE 0~63, NR 0~254. */
    uint32_t                                           cond_pdcp_discard_rate; /* unit:%, 0~100 */
    int32_t                                            cond_phr; /* unit:dB, LTE -23~40, NR -32~40 */
    int32_t                                            cond_s_rsrp; /* unit:dBm, LTE:-150~-30 , NR:-156~-31 */
    int32_t                                            cond_s_sinr; /* unit:dB, LTE:-20~40, NR: -20~40 */
    int32_t                                            cond_n_rsrp; /* unit:dBm, LTE:-150~-30 , NR:-156~-31 */
    int32_t                                            cond_n_sinr; /* unit:dB, LTE:-20~40, NR: -20~40 */
} mipc_nw_congestion_info_struct4 ;

typedef struct mipc_mbms_session_status_struct4 {
    uint8_t                                            is_activate; /* The status of the session */
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* The PLMN ID of the session tmgi info. */
    uint8_t                                            service_id[3]; /* The service id of the session tmgi info. */
} mipc_mbms_session_status_struct4 ;

typedef struct mipc_mbms_freq_info_struct4 {
    uint32_t                                           freq; /* The mbs freq info. */
} mipc_mbms_freq_info_struct4 ;

typedef struct mipc_mbms_tmgi_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* The PLMN ID of the session tmgi info. */
    uint8_t                                            service_id[3]; /* The service id of the session tmgi info. */
} mipc_mbms_tmgi_info_struct4 ;

typedef struct mipc_mbms_usd_fsai_info_struct4 {
    uint16_t                                           num_of_nrarfcn; /* N/A */
    uint16_t                                           num_of_fsai; /* N/A */
    uint32_t                                           nrarfcn[5]; /* The nrarfcn of the session tmgi info. */
    uint16_t                                           fsai[64]; /* The fsai of the session tmgi info. */
} mipc_mbms_usd_fsai_info_struct4 ;

typedef struct mipc_mbms_fsai_neighbor_info_struct4 {
    uint32_t                                           num_of_fsai; /* N/A */
    uint16_t                                           fsai[64]; /* The fsai. */
} mipc_mbms_fsai_neighbor_info_struct4 ;

typedef struct mipc_mbms_tac_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* The PLMN ID of the session tmgi info. */
    uint8_t                                            ta_code[5]; /* The tac code. */
} mipc_mbms_tac_info_struct4 ;

typedef struct mipc_mbms_ip_address_info_struct4 {
    uint8_t                                            src_ip_address[16]; /* The first 4 bytes is for ipv4. */
    uint8_t                                            dest_ip_address[16]; /* The first 4 bytes is for ipv4. */
} mipc_mbms_ip_address_info_struct4 ;

typedef struct mipc_mbms_cell_id_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* The plmn of the cell global id. */
    uint8_t                                            cell_id[5]; /* The cell id of the struct. */
} mipc_mbms_cell_id_info_struct4 ;

typedef struct mipc_data_tsn_info_struct4 {
    uint8_t                                            port_mac_present; /* It indicates whether port_mac is present or not. */
    uint8_t                                            port_mac[6]; /* DS-TT ethernet port MAC address. */
    uint8_t                                            res_time_present; /* It indicates whether res_time is present or not. */
    uint8_t                                            res_time[8]; /* DS-TT residence time. This value is the correction measured in nanoseconds and multiplied by 2^16. LSB bit of the DS-TT residence time included in bit 1 of first octet. If the UE-DS-TT residence time is too big to be represented, all bits of octets 0 to 7 shall be coded as "1" except the MSB bit of octet 7. Refer to IEEE 1588-2088 clause 13.3.2.7. */
    uint8_t                                            pmic_present; /* It indicates whether PMIC is present or not. */
    uint8_t                                            padding[1]; /* Padding */
    uint16_t                                           pmic_len; /* It indicates length of PMIC. */
    uint8_t                                            pmic[8192]; /* Port management information container. */
} mipc_data_tsn_info_struct4 ;

typedef struct mipc_data_tsn_time_struct4 {
    uint32_t                                           day; /* Days of reference time */
    uint32_t                                           second; /* Seconds of reference time */
    uint32_t                                           milli_second; /* Milliseconds of reference time */
    uint32_t                                           nano_second; /* Nanoseconds of reference time */
    uint32_t                                           uncertainly; /* Refer to 3GPP TS 38.311 */
    uint32_t                                           timeInfoType; /* Refer to 3GPP TS 38.311 */
    uint32_t                                           referenceSFN; /* Refer to 3GPP TS 38.311 */
} mipc_data_tsn_time_struct4 ;

typedef struct mipc_ftai_info_struct4 {
    char                                               plmn_id[MIPC_MAX_PLMN_ID_LEN]; /* The plmn of the ftai_info. */
    uint8_t                                            padding[1]; /* Padding */
    uint16_t                                           TAC; /* the tac of the ftai_info. */
} mipc_ftai_info_struct4 ;

typedef struct mipc_sys_idc_gpio_cfg_struct4 {
    uint8_t                                            gpio_0; /* GPIO 0 */
    uint8_t                                            gpio_1; /* GPIO 1 */
    uint8_t                                            gpio_2; /* GPIO 2 */
} mipc_sys_idc_gpio_cfg_struct4 ;

typedef struct mipc_track_item_struct4 {
    uint16_t                                           track_index; /* track_index for modem debug */
    uint16_t                                           mipc_msg_id; /* the pending mipc_msg_id. 0 is invalid, track_not_for_mipc or mipc_msg_parse_fail */
    uint32_t                                           pending_time; /* the pending time in second */
    uint16_t                                           mipc_txid; /* the txid of pending mipc_msg */
    uint16_t                                           mipc_id; /* the mipc_id for modem debug */
} mipc_track_item_struct4 ;

typedef struct mipc_wfp_filter_struct_struct4 {
    uint32_t                                           filter_config; /* Enable flag for applied filters */
    uint32_t                                           operation_config; /* Operation type for each config wifiProxy_filter_operations_e */
    uint8_t                                            priority; /* filter priority: low priority filter routed through AP */
    uint8_t                                            ip_ver; /* IP Versionv=vIPV4 or IPV6 */
    uint8_t                                            protocol; /* IP Protocol C IANA IP Protocol numbers */
    uint8_t                                            icmp_type; /* IANA ICMP parameter types */
    uint8_t                                            icmp_code; /* IANA ICMP code fields */
    uint8_t                                            reserved[3]; /* Reserved */
    uint8_t                                            ip_src[16]; /* IP Src: 4bytes / 16 bytes (IPV4/IPV6) */
    uint8_t                                            ip_src_mask; /* IP mask in CIDR notation */
    uint8_t                                            reserved2[3]; /* Reserved */
    uint8_t                                            ip_dest[16]; /* IP Src: 4bytes / 16 bytes (IPV4/IPV6) */
    uint8_t                                            ip_dest_mask; /* IP mask in CIDR notation */
    uint8_t                                            reserved3[3]; /* Reserved */
    uint16_t                                           src_port; /* TCP UDP Src Port number */
    uint16_t                                           src_port_mask; /* TCP UDP Src Port number mask */
    uint16_t                                           dst_port; /* TCP UDP Dst port number */
    uint16_t                                           dst_port_mask; /* TCP UDP Dst Port number mask */
    uint16_t                                           tcp_flags; /* TCP flags */
    uint16_t                                           tcp_flags_mask; /* TCP flags mask */
    uint16_t                                           tcp_flags_operation; /* TCP flags operation */
    uint8_t                                            reserved4[2]; /* Reserved */
    uint16_t                                           icmp_src_ip[16]; /* ICMP error message (type 3 and 11) */
    uint16_t                                           icmp_src_port; /* ICMP error message (type 3 and 11) */
    uint16_t                                           icmp_src_mask; /* Mask for src port in ICMP error resp */
    uint32_t                                           esp_spi; /* Security parameter Index for ESP packets */
    uint32_t                                           esp_spi_mask; /* SPI mask */
} mipc_wfp_filter_struct_struct4 ;

typedef struct mipc_sys_modem_struct8 {
    uint64_t                                           modem_id; /* Constant across reboots or device removal and insertion */
    uint32_t                                           executor_number; /* The number of executor */
    uint32_t                                           sim_number; /* The number of SIM slot */
    uint32_t                                           concurrency; /* 1: Dual standby; 2: Dual active  */
    uint8_t                                            padding[4]; /* N/A */
} mipc_sys_modem_struct8 ;

typedef struct mipc_sys_adpclk_freq_info_struct8 {
    uint64_t                                           center_frequency; /*  */
    uint32_t                                           frequency_spread; /*  */
    uint32_t                                           noise_power; /*  */
    uint32_t                                           relative_signal_strength; /*  */
    uint32_t                                           connect_status; /*  */
} mipc_sys_adpclk_freq_info_struct8 ;

typedef struct mipc_data_5gqos_info_v1_struct8 {
    uint16_t                                           cid; /* Context identifier */
    uint16_t                                           VQI; /* 5QI, 5G QoS Indication */
    uint32_t                                           Averaging_window; /* It represents the duration over which the Guaranteed Flow Bit Rate (GFBR) shall be calculated; the unit is milliseconds */
    uint64_t                                           DL_GFBR; /* Downlink Guaranteed Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           UL_GFBR; /* Uplink Guaranteed Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           DL_MFBR; /* Downlink Maximum Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           UL_MFBR; /* Uplink Maximum Flow Bit Rate; the unit is kbit/s */
    uint64_t                                           DL_SAMBR; /* Downlink session Aggregate Maximum Bit Rate; the unit is kbit/s */
    uint64_t                                           UL_SAMBR; /* Uplink session Aggregate Maximum Bit Rate; the unit is kbit/s */
} mipc_data_5gqos_info_v1_struct8 ;

typedef struct mipc_nw_nr_cell_v2_struct8 {
    uint8_t                                            state; /* 1: serving; 0: neighboring */
    char                                               provider_id[MIPC_MAX_PLMN_ID_LEN]; /* N/A */
    uint64_t                                           cid; /* Cell Identity. */
    uint32_t                                           physical_cell_id; /* The Physical Cell ID. */
    uint32_t                                           nr_arfcn; /* Absolute RF channel number of the serving cell. */
    uint32_t                                           tac; /* The Tracking Area Code. */
    int32_t                                            rsrp; /* The Average Reference signal Received Power. */
    int32_t                                            rsrq; /* Average Reference signal Received Quality. */
    int32_t                                            sinr; /* SS signal-to-noise and interference ratio. */
    uint32_t                                           ta; /* timing advance. */
    uint32_t                                           csirsrp; /* CSI reference signal received power. */
    uint32_t                                           csirsrq; /* CSI reference signal received quality. */
    uint32_t                                           csisinr; /* CSI signal-to-noise and interference ratio. */
    uint32_t                                           dl_freq_band; /* downlink freq band used by the cell. */
    uint32_t                                           registered; /* The registered state of the cell. */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Long provider name */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* Short provider name */
    uint8_t                                            csi_cqi_table_index; /* CSI cqi table index */
    uint8_t                                            csi_cqi_report_num; /* CSI cqi report num */
    uint8_t                                            csi_cqi_report_list[19]; /* CSI cqi report info */
    uint8_t                                            padding[3]; /* Padding */
} mipc_nw_nr_cell_v2_struct8 ;

typedef struct mipc_nw_reg_change_info_v1_struct8 {
    uint8_t                                            stat; /* Network register status. */
    uint8_t                                            padding1[3]; /* Padding */
    uint32_t                                           lac_tac; /* Location area code. */
    uint64_t                                           cell_id; /* Cell id, 32 bit for non-5G and 36 bit for 5G. */
    uint16_t                                           eact; /* Radio access technology. */
    uint8_t                                            rac; /* Routing area code. */
    uint8_t                                            nw_existence; /* Indicate if nw exist, only valid when eact belongs to C2K. */
    uint8_t                                            roam_indicator; /* Roam indicator, only valid when eact belongs to C2K. */
    uint8_t                                            cause_type; /* Indicates the type of reject_cause. */
    uint16_t                                           reject_cause; /* The cause of the failed registration. */
    uint8_t                                            dcnr_restricted; /* Indicate if dcnr is restricted. */
    uint8_t                                            endc_sib_status; /* Indicates endc sib status. */
    uint8_t                                            endc_available; /* Indicate if endc is available. */
    uint8_t                                            padding2[5]; /* Padding */
} mipc_nw_reg_change_info_v1_struct8 ;

typedef struct mipc_nw_ps_reg_info_v1_struct8 {
    uint8_t                                            stat; /* It indicates packet service register state */
    uint8_t                                            padding1; /* Padding */
    uint16_t                                           rat; /* The radio technology of serving cell */
    uint16_t                                           reason_for_denial; /* Registration denied reason; refer to 3GPP TS 24.008 */
    uint16_t                                           max_data_calls; /* The maximum number of simultaneous data calls; it can be removed because it is always 0 in RIL */
    int32_t                                            is_vops_supported; /* It indicates if camped network supports VoLTE services */
    int32_t                                            is_emc_bearer_supported; /* It indicates if camped network supports VoLTE emergency bearers */
    uint8_t                                            is_endc_available; /* It indicates if E-UTRA-NR Dual Connectivity(EN-DC) is supported by the primary serving cell */
    uint8_t                                            is_dcnr_restricted; /* It indicates if dual connectivity with NR is restricted */
    uint8_t                                            is_nr_available; /* It indicates if NR is available */
    uint8_t                                            roaming_ind; /* It indicates if network is registered on CDMA or EVDO */
    int8_t                                             is_in_prl; /* It indicates whether the current system is included in the PRL if network is registered on a CDMA or EVDO system;
            -1: network is not registered on a CDMA or EVDO system;
            0: network is registered on a CDMA or EVDO system but is not included in the PRL;
            1: network is registered on a CDMA or EVDO system and is included in the PRL
 */
    uint8_t                                            padding2; /* Padding */
    int16_t                                            def_roaming_ind; /* Default Roaming Indicator from the PRL if network is registered on a CDMA or EVDO system;
            -1: network is not registered on a CDMA or EVDO system;
            valid values are 0-255
 */
    uint64_t                                           cell_id; /* Cell ID */
    uint8_t                                            cache_endc_connect; /* Cache ENDC connection for a specified period of time (the timer is configured by API) */
    uint8_t                                            padding3[3]; /* Padding */
    uint32_t                                           lac_tac; /* Location area code */
} mipc_nw_ps_reg_info_v1_struct8 ;

typedef struct mipc_nw_cellmeasurement_info_v1_struct8 {
    uint8_t                                            rat; /* Cell RAT info: 7: LTE; 13 : NG-RAN */
    uint8_t                                            padding[3]; /* Padding */
    uint32_t                                           arfcn; /* The ARFCN information for this cell */
    uint32_t                                           pci; /* The physical cell ID for this cell */
    uint32_t                                           rsrp; /* The signal information in qdbm for this cell */
    uint32_t                                           rsrq; /* The signal information in qdbm for this cell */
    uint32_t                                           snr; /* The signal information in qdb for this cell */
    uint64_t                                           cid; /* The cell ID for this cell */
} mipc_nw_cellmeasurement_info_v1_struct8 ;

typedef struct mipc_nw_pseudocell_info_v1_struct8 {
    uint8_t                                            type; /* the type of the pseudocell. */
    char                                               plmn[MIPC_MAX_PLMN_ID_LEN]; /* the PLMN id of the pseudocell. */
    char                                               long_name[MIPC_MAX_NETWORK_NAME_LEN]; /* the PLMN long name of the pseudocell. */
    char                                               short_name[MIPC_MAX_NETWORK_NAME_LEN]; /* the plmn short name of the pseudocell. */
    uint32_t                                           lac; /* the LAC of the pseudocell. */
    uint8_t                                            padding1[4]; /* Padding */
    uint64_t                                           cell_id; /* the cell id of the pseudocell. */
    uint16_t                                           arfcn; /* the ARFCN of the pseudocell. */
    uint8_t                                            bsic; /* the bsic of the pseudocell. */
    char                                               si3_raw_data[50]; /* the si3_raw_data of the pseudocell. */
    uint8_t                                            padding2[3]; /* Padding */
} mipc_nw_pseudocell_info_v1_struct8 ;

#pragma pack (pop)


#endif /* __MIPC_MSG_COMMON_CONST_H__ */
