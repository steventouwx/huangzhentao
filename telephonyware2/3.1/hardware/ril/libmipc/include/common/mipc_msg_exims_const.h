#ifndef __MIPC_MSG_EXIMS_CONST_H__
#define __MIPC_MSG_EXIMS_CONST_H__

enum MIPC_EXIMS_MSG_enum {
    MIPC_EXIMS_MSG_NONE = 0,
    /*
      When user wants to start or end the session, user should update the session status with session type to modem
    */
    MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ                    = 4609,
    MIPC_EXIMS_UPDATE_SESSION_STATUS_CNF                    = 4610,

    /* This command is used to acquire call domain suggestion from modem. After modem received MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ, modem will perform service check and pre-process (e.g. UAC/SSAC) and choose a domain for user. */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ                   = 4611,
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_CNF                   = 4612,

    /* This command is used to update call status changed event to modem. Modem could make more appropriate domain selection through these information next time. */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ                 = 4613,
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_CNF                 = 4614,

    /*
      This command is used to inform IMS handover status along with source RAT and target RAT, and result to modem.
      After modem received this command, modem would update the call context if IMS handover occurs when active calls exists. And also delay domain selection determination if modem receive MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ during IMS handover ongoing.
    */
    MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ               = 4615,
    MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_CNF               = 4616,

    /* This command is used to acquire SMS domain suggestion from modem. After modem received MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ, modem will perform service check and pre-process to ensure its readiness and choose a domain for user. */
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ                    = 4621,
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_CNF                    = 4622,

    /*
      This command is used to update SMS status changed event to modem. Modem will take the corresponding action according to the status of SMS.
      For example, when the status of SMS is 'MIPC_EXIMS_SMS_STATUS_START', modem will perform UAC and SCM; when the status of SMS is 'MIPC_EXIMS_SMS_STATUS_STOP', modem will stop UAC and SCM, and clear all SMS instance; when the status of SMS is 'MIPC_EXIMS_SMS_STATUS_FAIL', modem will stop UAC and SCM, but not to clear SMS instance for retry determination next time.
    */
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ                  = 4623,
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_CNF                  = 4624,

    /* This command is used to update the IMS registration status to modem. */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ                        = 4625,
    MIPC_EXIMS_UPDATE_REG_STATUS_CNF                        = 4626,

    /* This command is used to update the external IMS related capability and functionality at the same time. */
    MIPC_EXIMS_UPDATE_CONFIG_REQ                            = 4627,
    MIPC_EXIMS_UPDATE_CONFIG_CNF                            = 4628,

    /* This command is used to get ECC list. */
    MIPC_EXIMS_GET_ECC_LIST_REQ                             = 4629,
    MIPC_EXIMS_GET_ECC_LIST_CNF                             = 4630,

    /* to set Smart Congestion Mitigation, only for specific modem branch */
    MIPC_EXIMS_SET_SCM_REQ                                  = 4631,
    MIPC_EXIMS_SET_SCM_CNF                                  = 4632,

    /* to set IMS service session status,only for specific modem branch */
    MIPC_EXIMS_SET_SERVICE_SESSION_REQ                      = 4633,
    MIPC_EXIMS_SET_SERVICE_SESSION_CNF                      = 4634,

    /* to set IMS UAC status,only for specific modem branch */
    MIPC_EXIMS_SET_UAC_REQ                                  = 4635,
    MIPC_EXIMS_SET_UAC_CNF                                  = 4636,

    /* This command is used to report whether IMS Voice Over PS sessions (IMSVOPS) supported indication from the network. */
    MIPC_EXIMS_IMSVOPS_IND                                  = 20993,

    /* This command is used to indicate the emergency service support status in the current cell. */
    MIPC_EXIMS_EMERGENCY_SERVICE_SUPPORT_IND                = 20994,

    /* This command is used to reporting of changes in the emergency bearer services support indicators and emergency service fallback indicator. */
    MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND                 = 20995,

    /* This command will be reported when ECC list changed. */
    MIPC_EXIMS_ECC_LIST_CHANGE_IND                          = 20996,

    /* This command will be reported when the bar status of call is changed. */
    MIPC_EXIMS_UNLOCK_CALL_BAR_IND                          = 20997,

    /* This command will be reported when the bar status of SMS is changed. */
    MIPC_EXIMS_UNLOCK_SMS_BAR_IND                           = 20998,

    /* This command will be reported when the bar status of registration is changed. */
    MIPC_EXIMS_UNLOCK_REG_BAR_IND                           = 20999,


};

    /* MIPC_MSG.EXIMS_UPDATE_SESSION_STATUS_REQ */
enum mipc_exims_update_session_status_req_tlv_enum {
    mipc_exims_update_session_status_req_tlv_NONE = 0,
    /* This field indicates which session type would be start or end. */
    /* type = uint8_t, refer to EXIMS_ACTION_TYPE */
    MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_SESSION_TYPE     = 0x100,
    /* This field indicates the session is start or end. */
    /* type = uint8_t, refer to EXIMS_SESSION_STATUS */
    MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_STATUS           = 0x101,
    /* The callback mode RAT with bit format; User should carry this field when user is going to start an ECBM session or SCBM session to let modem known the current callback mode is activated at which RAT; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_CALLBACK_MODE_RAT = 0x102,
    /*
      This field is mandatory when user is going to start an emergency call or emergency SMS to notify modem that the emergency is initialed w/o or w/ airplane mode.
      True: Emergency is initialed w/ airplane mode.
      False: Emergency is initialed w/o airplane mode.
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_SESSION_STATUS_REQ_T_IS_AIRPLANE_MODE_EMERGENCY = 0x103,
};

    /* MIPC_MSG.EXIMS_UPDATE_SESSION_STATUS_CNF */
enum mipc_exims_update_session_status_cnf_tlv_enum {
    mipc_exims_update_session_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_GET_CALL_DOMAIN_SELECT_REQ */
enum mipc_exims_get_call_domain_select_req_tlv_enum {
    mipc_exims_get_call_domain_select_req_tlv_NONE = 0,
    /* Unique identifier of call instances in the same call session; The range of this field is 1~7. */
    /* type = uint8_t */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_CALL_ID         = 0x100,
    /* Type of the call which would like to acquire call domain suggestion */
    /* type = uint8_t, refer to EXIMS_CALL_TYPE */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_CALL_TYPE       = 0x101,
    /* This field is used for user to provide the allowed-to-select RAT combination with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_AVAILABLE_RAT   = 0x102,
    /* This field is used for user to provide the prefer-to-select RAT combination with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_REQ_T_PREFER_RAT      = 0x103,
};

    /* MIPC_MSG.EXIMS_GET_CALL_DOMAIN_SELECT_CNF */
enum mipc_exims_get_call_domain_select_cnf_tlv_enum {
    mipc_exims_get_call_domain_select_cnf_tlv_NONE = 0,
    /* The recommended RAT which is ready for user to initiate call procedure with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_CNF_T_RAT             = 0x100,
    /* This field is used to specify the cause if modem can't find any RAT. */
    /* type = uint16_t, refer to EXIMS_CALL_DOMAIN_CAUSE */
    MIPC_EXIMS_GET_CALL_DOMAIN_SELECT_CNF_T_CAUSE           = 0x101,
};

    /* MIPC_MSG.EXIMS_UPDATE_CALL_STATUS_EVENT_REQ */
enum mipc_exims_update_call_status_event_req_tlv_enum {
    mipc_exims_update_call_status_event_req_tlv_NONE = 0,
    /* Unique identifier of call instances in the same call session; The range of this field is 1~7. */
    /* type = uint8_t */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CALL_ID       = 0x100,
    /* Type of the call */
    /* type = uint8_t, refer to EXIMS_CALL_TYPE */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CALL_TYPE     = 0x101,
    /* Status of the call */
    /* type = uint32_t, refer to EXIMS_CALL_STATUS */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CALL_STATUS   = 0x102,
    /* The reason of call failure. This field is mandatory when the value of 'CALL_STATUS' is 5 (MIPC_EXIMS_CALL_STATUS_FAILURE) or the default value of this field would be 0XFF which means unspecified. */
    /* type = uint16_t, refer to EXIMS_CALL_FAILURE_CAUSE */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_CAUSE         = 0x103,
    /* This field indicates whether the call is release by NW or not. This field is mandatory when the value of 'CALL_STATUS' is 4 (MIPC_EXIMS_CALL_STATUS_DISCONNECTED). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_IS_FAR_END    = 0x104,
    /* This field indicates this call is MT call or MO call. */
    /* type = uint8_t, refer to CALL_DIRECTION */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_DIR           = 0x105,
    /* Call RAT where call is currently performed at with bit format. */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_RAT           = 0x106,
    /* The call number or SIP URI according to the field 'ADDRESS_TYPE'; This field is mandatory when the value of 'CALL_STATUS' is 0 (MIPC_EXIMS_CALL_STATUS_INVITE). */
    /* type = string */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_ADDRESS       = 0x107,
    /* The type of the field 'DIAL_ADDRESS' */
    /* type = uint32_t, refer to CALL_DIAL_ADDRESS_TYPE */
    MIPC_EXIMS_UPDATE_CALL_STATUS_EVENT_REQ_T_ADDRESS_TYPE  = 0x108,
};

    /* MIPC_MSG.EXIMS_UPDATE_CALL_STATUS_EVENT_CNF */
enum mipc_exims_update_call_status_event_cnf_tlv_enum {
    mipc_exims_update_call_status_event_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ */
enum mipc_exims_update_ims_ho_status_event_req_tlv_enum {
    mipc_exims_update_ims_ho_status_event_req_tlv_NONE = 0,
    /* This field indicates IMS handover status is start or end. */
    /* type = uint8_t, refer to EXIMS_HO_STATUS */
    MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_STATUS      = 0x100,
    /* The handover source RAT with bit format */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_SOURCE_RAT  = 0x101,
    /* The handover target RAT with bit format */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_TARGET_RAT  = 0x102,
    /* The result of handover; this field is mandatory when the value of 'STATUS' is 0 (MIPC_EXIMS_HO_STATUS_END). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_IMS_HO_STATUS_EVENT_REQ_T_RESULT      = 0x103,
};

    /* MIPC_MSG.EXIMS_UPDATE_IMS_HO_STATUS_EVENT_CNF */
enum mipc_exims_update_ims_ho_status_event_cnf_tlv_enum {
    mipc_exims_update_ims_ho_status_event_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_GET_SMS_DOMAIN_SELECT_REQ */
enum mipc_exims_get_sms_domain_select_req_tlv_enum {
    mipc_exims_get_sms_domain_select_req_tlv_NONE = 0,
    /* This field indicates which kind of SMS is requested. */
    /* type = uint8_t, refer to SMS_TYPE */
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ_T_SMS_TYPE         = 0x100,
    /* This field is used for user to provide the allowed-to-select RAT combination with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ_T_AVAILABLE_RAT    = 0x101,
    /* This field is used for user to provide the preferred-to-select domain */
    /* type = uint32_t, refer to EXIMS_SMS_DOMAIN */
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_REQ_T_PREFER_DOMAIN    = 0x102,
};

    /* MIPC_MSG.EXIMS_GET_SMS_DOMAIN_SELECT_CNF */
enum mipc_exims_get_sms_domain_select_cnf_tlv_enum {
    mipc_exims_get_sms_domain_select_cnf_tlv_NONE = 0,
    /* The recommended RAT that is ready for AP to initiate SMS procedure; if modem can't find any, 0 would be returned */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_CNF_T_AVAILABLE_RAT    = 0x100,
    /* This field is used to specify the domain of specified RAT */
    /* type = uint32_t, refer to EXIMS_SMS_DOMAIN */
    MIPC_EXIMS_GET_SMS_DOMAIN_SELECT_CNF_T_SMS_DOMAIN       = 0x101,
};

    /* MIPC_MSG.EXIMS_UPDATE_SMS_STATUS_EVENT_REQ */
enum mipc_exims_update_sms_status_event_req_tlv_enum {
    mipc_exims_update_sms_status_event_req_tlv_NONE = 0,
    /* This field indicates the direction of SMS is MT or MO. */
    /* type = uint8_t, refer to SMS_DIRECTION */
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_DIR            = 0x100,
    /* SMS RAT where SMS is currently performed at with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_RAT            = 0x101,
    /* This field indicates the transmitting SMS domain. */
    /* type = uint32_t, refer to EXIMS_SMS_DOMAIN */
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_SMS_DOMAIN     = 0x102,
    /* This field indicates which kind of SMS is updated. */
    /* type = uint8_t, refer to SMS_TYPE */
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_SMS_TYPE       = 0x103,
    /* This field indicates the current SMS status. */
    /* type = uint8_t, refer to EXIMS_SMS_STATUS */
    MIPC_EXIMS_UPDATE_SMS_STATUS_EVENT_REQ_T_STATUS         = 0x104,
};

    /* MIPC_MSG.EXIMS_UPDATE_SMS_STATUS_EVENT_CNF */
enum mipc_exims_update_sms_status_event_cnf_tlv_enum {
    mipc_exims_update_sms_status_event_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_UPDATE_REG_STATUS_REQ */
enum mipc_exims_update_reg_status_req_tlv_enum {
    mipc_exims_update_reg_status_req_tlv_NONE = 0,
    /* The state of IMS registration */
    /* type = uint8_t, refer to EXIMS_REG_STATE */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_STATE                = 0x100,
    /* This field indicates that the type of IMS registration is a normal service or an emergency service. */
    /* type = uint8_t, refer to EXIMS_REG_TYPE */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_TYPE                 = 0x101,
    /*
      IMS registration extension information with bit format; each bit represents a particular IMS capability of the MT; this field should not present when the value of 'STATE' is 0 (MIPC_EXIMS_REG_STATE_NOT_REGISTERED), or modem will ignore the value of this field.
      Bit0: RTP-based transfer of voice according to MMTEL; refer to 3GPP TS 24.173
      Bit1: RTP-based transfer of text according to MMTEL; refer to 3GPP TS 24.173
      Bit2: SMS using IMS functionality; refer to 3GPP TS 24.341
      Bit3: RTP-based transfer of video according to MMTEL; refer to 3GPP TS 24.173
    */
    /* type = bitmap, refer to EXIMS_EXT_INFO */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_EXT_INFO             = 0x102,
    /* The cause of IMS deregistration */
    /* type = uint8_t, refer to EXIMS_DEREG_CAUSE */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_DEREG_CAUSE          = 0x103,
    /* The RAT where IMS is currently performed at with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_RAT                  = 0x104,
    /* The type of IMS SIP URI */
    /* type = uint8_t, refer to EXIMS_URI_TYPE */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_SIP_URI_TYPE         = 0x105,
    /* This field indicates IMS is retry or not */
    /* type = uint8_t, refer to EXIMS_RETRY_TYPE */
    MIPC_EXIMS_UPDATE_REG_STATUS_REQ_T_IMS_RETRY            = 0x106,
};

    /* MIPC_MSG.EXIMS_UPDATE_REG_STATUS_CNF */
enum mipc_exims_update_reg_status_cnf_tlv_enum {
    mipc_exims_update_reg_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_UPDATE_CONFIG_REQ */
enum mipc_exims_update_config_req_tlv_enum {
    mipc_exims_update_config_req_tlv_NONE = 0,
    /*
      This field indicates IMS functionality is enable or disable. After modem bootup/reset and before receiving MIPC_EXIMS_UPDATE_CONFIG_REQ, this field will be regarded as Disable.
      False: Disable
      True: Enable
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_IMS_CFG_ENABLE        = 0x100,
    /*
      This field indicates IMS Voice is over Cellular RAT functionality. This value would be enable only when the field 'IS_IMS_CFG_ENABLE' indicates enable.
      0: Disable
      1: Voice over LTE enable (EPS Fallback supported for 5G)
      3: Voice over LTE and NR enable
    */
    /* type = uint16_t, refer to EXIMS_CONFIG_TYPE */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_Vo3GPP_CFG               = 0x101,
    /*
      This field indicates IMS Video is over Cellular RAT Capability. This value would be enable only when the field 'Vo3GPP_CFG' indicates enable.
      0: Disable
      1: Video over LTE enable
      3: Video over LTE and NR enable
    */
    /* type = uint16_t, refer to EXIMS_CONFIG_TYPE */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_Vi3GPP_CFG               = 0x102,
    /*
      This field indicates WiFi calling functionality is enable or disable. This value would be enable only when the field 'IS_IMS_CFG_ENABLE' indicates enable.
      False: Disable
      True: Enable
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_VoWiFi_ENABLE         = 0x103,
    /*
      This field indicates IMS Video over WiFi capability is enable or disable. This value would be enable only when the field 'IS_VoWiFi_ENABLE' indicates enable.
      False: Disable
      True: Enable
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_ViWiFi_ENABLE         = 0x104,
    /*
      This field indicates IMS SMS capability is enable or disable. This value would be enable only when the field 'IS_VoWiFi_ENABLE' or the field 'Vo3GPP_CFG' indicates enable.
      False: Disable
      True: Enable
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_IS_IMS_SMS_ENABLE        = 0x105,
    /* This field indicates the timer to guard emergency PDX's establishment */
    /* type = uint32_t */
    MIPC_EXIMS_UPDATE_CONFIG_REQ_T_EMERGENCY_PDX_GUARD_TIMER = 0x106,
};

    /* MIPC_MSG.EXIMS_UPDATE_CONFIG_CNF */
enum mipc_exims_update_config_cnf_tlv_enum {
    mipc_exims_update_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_GET_ECC_LIST_REQ */
enum mipc_exims_get_ecc_list_req_tlv_enum {
    mipc_exims_get_ecc_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_GET_ECC_LIST_CNF */
enum mipc_exims_get_ecc_list_cnf_tlv_enum {
    mipc_exims_get_ecc_list_cnf_tlv_NONE = 0,
    /* The information of ECC number */
    /* type = struct, refer to ecc_info */
    MIPC_EXIMS_GET_ECC_LIST_CNF_T_INFO_TLV_ARRAY            = 0x100,
};

    /* MIPC_MSG.EXIMS_SET_SCM_REQ */
enum mipc_exims_set_scm_req_tlv_enum {
    mipc_exims_set_scm_req_tlv_NONE = 0,
    /* Smart Congestion Mitigation application */
    /* type = uint8_t, refer to SCM_APPLICATION_ENUM */
    MIPC_EXIMS_SET_SCM_REQ_T_APPLICATION                    = 0x100,
    /* start or end */
    /* type = uint8_t, refer to SCM_INDICATION_ENUM */
    MIPC_EXIMS_SET_SCM_REQ_T_INDICATION                     = 0x101,
};

    /* MIPC_MSG.EXIMS_SET_SCM_CNF */
enum mipc_exims_set_scm_cnf_tlv_enum {
    mipc_exims_set_scm_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_SET_SERVICE_SESSION_REQ */
enum mipc_exims_set_service_session_req_tlv_enum {
    mipc_exims_set_service_session_req_tlv_NONE = 0,
    /* IMS service session status */
    /* type = uint8_t, refer to EXIMS_SERVICE_TYPE_ENUM */
    MIPC_EXIMS_SET_SERVICE_SESSION_REQ_T_SERVICE_TYPE       = 0x100,
    /* start or end */
    /* type = uint8_t, refer to EXIMS_SERVICE_STATUS_ENUM */
    MIPC_EXIMS_SET_SERVICE_SESSION_REQ_T_SERVICE_STATUS     = 0x101,
};

    /* MIPC_MSG.EXIMS_SET_SERVICE_SESSION_CNF */
enum mipc_exims_set_service_session_cnf_tlv_enum {
    mipc_exims_set_service_session_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_SET_UAC_REQ */
enum mipc_exims_set_uac_req_tlv_enum {
    mipc_exims_set_uac_req_tlv_NONE = 0,
    /* IMS service session status */
    /* type = uint8_t, refer to EXIMS_SERVICE_TYPE_ENUM */
    MIPC_EXIMS_SET_UAC_REQ_T_SERVICE_TYPE                   = 0x100,
    /* start or end */
    /* type = uint8_t, refer to EXIMS_SERVICE_STATUS_ENUM */
    MIPC_EXIMS_SET_UAC_REQ_T_SERVICE_STATUS                 = 0x101,
};

    /* MIPC_MSG.EXIMS_SET_UAC_CNF */
enum mipc_exims_set_uac_cnf_tlv_enum {
    mipc_exims_set_uac_cnf_tlv_NONE = 0,
    /* Failure info for UAC start */
    /* type = uint8_t, refer to EXIMS_UAC_CAUSE_ENUM */
    MIPC_EXIMS_SET_UAC_CNF_T_CAUSE                          = 0x100,
};

    /* MIPC_MSG.EXIMS_IMSVOPS_IND */
enum mipc_exims_imsvops_ind_tlv_enum {
    mipc_exims_imsvops_ind_tlv_NONE = 0,
    /*
      This field is used to notify AP whether IMSVOPS supported indication.
      True: IMSVOPS support indication received from network is positive.
      False: IMSVOPS support indication is negative or not received from network.
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_IMSVOPS_IND_T_IS_SUPPORT_IMSVOPS             = 0x100,
    /* The IMS VOPS supported indication N3GPP value */
    /* type = uint8_t, refer to IMS_VOPS_N3GPP_ENUM */
    MIPC_EXIMS_IMSVOPS_IND_T_NWIMSVOPS_N3GPP                = 0x101,
};

    /* MIPC_MSG.EXIMS_EMERGENCY_SERVICE_SUPPORT_IND */
enum mipc_exims_emergency_service_support_ind_tlv_enum {
    mipc_exims_emergency_service_support_ind_tlv_NONE = 0,
    /* This field is used to indicate the RAT of emergency bearer support in current cell with bit format; The bit of Wifi is unused in this case */
    /* type = bitmap, refer to EXIMS_RAT */
    MIPC_EXIMS_EMERGENCY_SERVICE_SUPPORT_IND_T_RAT          = 0x100,
    /*
      This field is used to notify AP whether the current cell supports emergency service or not.
      True: Current cell supports emergency service.
      False: Current cell does not support emergency service.
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_EMERGENCY_SERVICE_SUPPORT_IND_T_IS_SUPPORT_EMC = 0x101,
};

    /* MIPC_MSG.EXIMS_EMERGENCY_BEARER_SUPPORT_IND */
enum mipc_exims_emergency_bearer_support_ind_tlv_enum {
    mipc_exims_emergency_bearer_support_ind_tlv_NONE = 0,
    /*
      It indicates whether emergency bearer services indicator in S1 mode is supported or not; refer to 3GPP TS24.301 clause 9.9.3.12A
      True: Emergency bearer service in S1 mode is supported.
      False: Emergency bearer service in S1 mode is not supported.
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_IS_SUPPORT_S1 = 0x100,
    /* This field is used to notify AP the emergency services support status in 5GS for 3GPP access. When the emergency service indicator is not signaled from the network or if no network is available, the value of this field would be 0 which means emergency services is not supported in 5G. Refer to 3GPP TS 24.501 clause 9.11.3.5 */
    /* type = uint8_t, refer to EXIMS_EMERGENCY_SUPPORT */
    MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_EMS_5G_SUPP   = 0x101,
    /* This field is used to notify AP the emergency services fallback support status in 5GS for 3GPP access. When the emergency service fallback indicator is not signaled from the network or if no network is available, the value of this field would be 0 which means emergency services fallback is not supported in 5G. Refer to 3GPP TS 24.501 clause 9.11.3.5 */
    /* type = uint8_t, refer to EXIMS_EMERGENCY_SUPPORT */
    MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_EMF_5G_SUPP   = 0x102,
    /* This field is used to notify AP the emergency bearer service support indicator for IU mode */
    /* type = uint8_t, refer to EMERGENCY_CALL_EMB_IU_SUPP */
    MIPC_EXIMS_EMERGENCY_BEARER_SUPPORT_IND_T_EMB_IU_SUPP   = 0x103,
};

    /* MIPC_MSG.EXIMS_ECC_LIST_CHANGE_IND */
enum mipc_exims_ecc_list_change_ind_tlv_enum {
    mipc_exims_ecc_list_change_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.EXIMS_UNLOCK_CALL_BAR_IND */
enum mipc_exims_unlock_call_bar_ind_tlv_enum {
    mipc_exims_unlock_call_bar_ind_tlv_NONE = 0,
    /* The unlocked bar with bit format; if the bar is unlocked, the bit will be set to 1 */
    /* type = bitmap, refer to EXIMS_CALL_BAR */
    MIPC_EXIMS_UNLOCK_CALL_BAR_IND_T_BAR                    = 0x100,
};

    /* MIPC_MSG.EXIMS_UNLOCK_SMS_BAR_IND */
enum mipc_exims_unlock_sms_bar_ind_tlv_enum {
    mipc_exims_unlock_sms_bar_ind_tlv_NONE = 0,
    /* The unlocked bar with bit format; if the bar is unlocked, the bit will be set to 1 */
    /* type = bitmap, refer to EXIMS_SMS_BAR */
    MIPC_EXIMS_UNLOCK_SMS_BAR_IND_T_BAR                     = 0x100,
};

    /* MIPC_MSG.EXIMS_UNLOCK_REG_BAR_IND */
enum mipc_exims_unlock_reg_bar_ind_tlv_enum {
    mipc_exims_unlock_reg_bar_ind_tlv_NONE = 0,
    /* The unlocked bar with bit format; if the bar is unlocked, the bit will be set to 1 */
    /* type = bitmap, refer to EXIMS_REG_BAR */
    MIPC_EXIMS_UNLOCK_REG_BAR_IND_T_BAR                     = 0x100,
};




#endif /* __MIPC_MSG_EXIMS_CONST_H__ */
