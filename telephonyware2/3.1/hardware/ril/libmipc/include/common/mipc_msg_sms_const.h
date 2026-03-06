#ifndef __MIPC_MSG_SMS_CONST_H__
#define __MIPC_MSG_SMS_CONST_H__

enum MIPC_SMS_MSG_enum {
    MIPC_SMS_MSG_NONE = 0,
    /* This command is used to set or acquire SMS configuration. */
    MIPC_SMS_CFG_REQ                                        = 1537,
    MIPC_SMS_CFG_CNF                                        = 1538,

    /* This command is used to send short message. */
    MIPC_SMS_SEND_REQ                                       = 1541,
    MIPC_SMS_SEND_CNF                                       = 1542,

    /* This command is used to read short message. */
    MIPC_SMS_READ_REQ                                       = 1543,
    MIPC_SMS_READ_CNF                                       = 1544,

    /* This command is used to delete short message. */
    MIPC_SMS_DELETE_REQ                                     = 1545,
    MIPC_SMS_DELETE_CNF                                     = 1546,

    /* This command is used to acquire SMS storage status. */
    MIPC_SMS_GET_STORE_STATUS_REQ                           = 1547,
    MIPC_SMS_GET_STORE_STATUS_CNF                           = 1548,

    /* This command is used to write short message to device (modem or SIM). */
    MIPC_SMS_WRITE_REQ                                      = 1549,
    MIPC_SMS_WRITE_CNF                                      = 1550,

    /* This command is used to configure CBM, and the field of cfg_type must exist when the corresponding option is carried. Currently, some permutations are not supported. */
    MIPC_SMS_CBM_CFG_REQ                                    = 1551,
    MIPC_SMS_CBM_CFG_CNF                                    = 1552,

    /* This command is used to acquire the related feature of SCBM. */
    MIPC_SMS_SCBM_REQ                                       = 1553,
    MIPC_SMS_SCBM_CNF                                       = 1554,

    /* This command is used to set/acquire the related feature of SMS domain select. */
    MIPC_SMS_DOMAIN_REQ                                     = 1555,
    MIPC_SMS_DOMAIN_CNF                                     = 1556,

    /* This command is used to configure the sca/handle_mode/ack_mode of diffirent types of SMS. */
    MIPC_SMS_CFG_V2_REQ                                     = 1557,
    MIPC_SMS_CFG_V2_CNF                                     = 1558,

    /* This command is used to send short message. */
    MIPC_SMS_SEND_V2_REQ                                    = 1559,
    MIPC_SMS_SEND_V2_CNF                                    = 1560,

    /* This command is used to read short message and saved in modem. */
    MIPC_SMS_READ_V2_REQ                                    = 1561,
    MIPC_SMS_READ_V2_CNF                                    = 1562,

    /* This command is used to delete short message in modem. */
    MIPC_SMS_DELETE_V2_REQ                                  = 1563,
    MIPC_SMS_DELETE_V2_CNF                                  = 1564,

    /* This command is used to acquire SMS storage status. */
    MIPC_SMS_GET_STORE_STATUS_V2_REQ                        = 1565,
    MIPC_SMS_GET_STORE_STATUS_V2_CNF                        = 1566,

    /* This command is used to write short message to device (modem or SIM). */
    MIPC_SMS_WRITE_V2_REQ                                   = 1567,
    MIPC_SMS_WRITE_V2_CNF                                   = 1568,

    /* This command is used to configure CBM, and the field of cfg_type must exist when the corresponding option is carried. 3GPP's CH/LAN only support enable combination. */
    MIPC_SMS_CBM_CFG_V2_REQ                                 = 1569,
    MIPC_SMS_CBM_CFG_V2_CNF                                 = 1570,

    /* This command will be reported when SMS configuration is changed (e.g., SIM plug-out and plug-in). */
    MIPC_SMS_CFG_IND                                        = 17921,

    /* This command is used to report new short message when the user prefers not to reply ACK and save the short message in TE. Refer to +CMT and +EC2KCMT for the description of this command. */
    MIPC_SMS_NEW_SMS_IND                                    = 17922,

    /* This command is used to report the SMS storage status changed. If the new short message is saved in device, device will trigger this command. Refer to +CMTI for the description of this command. */
    MIPC_SMS_STORE_STATUS_IND                               = 17923,

    /* This command is used to report new status when user prefers not to reply ACK. */
    MIPC_SMS_NEW_STATUS_REPORT_IND                          = 17926,

    /* This command is used to report the new CBM message, including ETWS primary message, ETWS secondary message, CMAS message, and other CBM messages. */
    MIPC_SMS_NEW_CBM_IND                                    = 17927,

    /* This command is used to report SCBM feature. */
    MIPC_SMS_SCBM_IND                                       = 17928,

    /* other information about SMS to indication; refer +EPSI&+ECARDESNME */
    MIPC_SMS_EXT_INFO_IND                                   = 17929,

    /* MD would indicate with this MIPC_IND when new sms comes, whatever in which ack_mode or storage_mode(only used for special apps like AGPSD) */
    MIPC_SMS_DUP_NEW_SMS_IND                                = 17930,

    /* when modem SMS change status(such as init_done), would notify HOST a MIPC_IND. */
    MIPC_SMS_CFG_V2_IND                                     = 17931,

    /* When received SMS(in SAVE_MODE or MD_ACK_MODE), MD would notify HOST with this MIPC_IND. There would be two combinations for diffirent HANDLE_MODE: [SAVE_MODE]STORAGE+MSG_IDX; [RELAY_MODE]PDU.This is v2 for SMS:NEW_SMS_IND. */
    MIPC_SMS_NEW_SMS_V2_IND                                 = 17932,

    /* SMS storage status changed indication. */
    MIPC_SMS_STORE_STATUS_V2_IND                            = 17933,

    /* This command is used to report new status when user prefers not to reply ACK. */
    MIPC_SMS_NEW_STATUS_REPORT_V2_IND                       = 17934,

    /* This command is used to report the new CBM message, including ETWS primary message, ETWS secondary message, CMAS message, and other CBM messages. */
    MIPC_SMS_NEW_CBM_V2_IND                                 = 17935,

    /* MD would indicate with this MIPC_IND when new sms comes, whatever in which ack_mode or storage_mode(only used for special apps like AGPSD) */
    MIPC_SMS_DUP_NEW_SMS_V2_IND                             = 17936,

    /* This command is used to report new short message when user prefers to reply ACK and save the short message in TE. */
    MIPC_SMS_NEW_SMS_CMD                                    = 34305,
    MIPC_SMS_NEW_SMS_RSP                                    = 34306,

    /* This command is used to report new status when user prefers to reply ACK. */
    MIPC_SMS_NEW_STATUS_REPORT_CMD                          = 34307,
    MIPC_SMS_NEW_STATUS_REPORT_RSP                          = 34308,

    /* This command is used to report new short message when user prefers to reply ACK and save the short message in TE. As avoid conflict issue, user needs to register the same message version for MIPC_SMS_NEW_SMS_CMD and MIPC_SMS_NEW_STATU_REPORT_CMD, or user will get a register error */
    MIPC_SMS_NEW_SMS_V2_CMD                                 = 34309,
    MIPC_SMS_NEW_SMS_V2_RSP                                 = 34310,

    /* This command is used to report new status when user prefers to reply ACK. */
    MIPC_SMS_NEW_STATUS_REPORT_V2_CMD                       = 34311,
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP                       = 34312,


};

    /* MIPC_MSG.SMS_CFG_REQ */
enum mipc_sms_cfg_req_tlv_enum {
    mipc_sms_cfg_req_tlv_NONE = 0,
    /* Set or acquire DEVICE SMS format setting, refer to AT+CMGF */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_CFG_REQ_T_FORMAT                               = 0x100,
    /* Set SCA (SMS Center Address), refer to AT+CSCA */
    /* type = string */
    MIPC_SMS_CFG_REQ_T_SET_SCA                              = 0x8101,
    /* Acquire SCA, refer to AT+CSCA; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CFG_REQ_T_GET_SCA                              = 0x102,
    /* [PHASE OUT]. no need set, DEVICE always return SMS state now */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_GET_SMS_STATE                        = 0x103,
    /* [PHASE OUT]. USER PREFER_STORAGE(query) to get store status */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_GET_STORE_STATUS                     = 0x104,
    /* Set or acquire prefer_ack; True: user replies ACK to TE; if message is saved in SIM card: modem replies ACK to TE; refer AT+CSMS */
    /* type = uint8_t, refer to SMS_ACK */
    MIPC_SMS_CFG_REQ_T_PREFER_ACK                           = 0x105,
    /* Set or acquire prefer_storage; prefer_storage indicates the user's preferred storage used to save MT SMS; it also indicates the setting for MT SMS is save mode (save in modem) or relay mode (relay to HOST); some special SMS would not follow this prefer (like class0) */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_REQ_T_PREFER_STORAGE                       = 0x106,
    /* [PHASE OUT]indicate if need get all the setting: 0->no_need, other->need. not recommend */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_GET_ALL_CAN_GET                      = 0x107,
    /* Host notifies modem that the SMS storage in HOST is valid (true) or invalid (false), so modem can reply ACK error to network quickly in relay mode, refer to AT+EMEMS in MTK_AT */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CFG_REQ_T_SET_HOST_MEM_AVAILABLE               = 0x108,
    /* [3GPP] mipc not support Text mode, but can set text mode parameter as AT+CSMP */
    /* type = uint8_t, refer to SMS_TEXT_MODE_PARAM_ACTION */
    MIPC_SMS_CFG_REQ_T_TEXT_MODE_PARAM_ACTION               = 0x109,
    /* [3GPP] refer TS27.005 3.3.2 <fo>; Default value is 0 */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_TEXT_MODE_FO                         = 0x10A,
    /* [3GPP] refer TS27.005 3.3.2 <vp>; Default value is 0 */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_TEXT_MODE_VP                         = 0x10B,
    /* [3GPP] refer TS27.005 3.3.2 <pid>; Default value is 0 */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_TEXT_MODE_PID                        = 0x10C,
    /* [3GPP] refer TS27.005 3.3.2 <dcs>; Default value is 0 */
    /* type = uint8_t */
    MIPC_SMS_CFG_REQ_T_TEXT_MODE_DCS                        = 0x10D,
    /* [PHASE OUT] */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_REQ_T_PREFER_STORAGE_C2K                   = 0x10E,
    /* It indicates the location for performing read/write/delete operation for SMS in 3GPP format; the 3GPP default setting is MT; If PREFER_STORAGE is TE, this TLV can be configured; if PREFER_STORAGE is not TE, W_R_D_STORAGE is same as PREFER_STORAGE; this field is not necessary for 3GPP2 because 3GPP2 is always UIM and cannot be changed, refer to AT+CPMS */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_REQ_T_W_R_D_STORAGE                        = 0x10F,
    /* Save the settings to non-volatile memory; refer to CSAS; only the save setting of SCA is supported currently; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CFG_REQ_T_SAVE_SETTING                         = 0x110,
};

    /* MIPC_MSG.SMS_CFG_CNF */
enum mipc_sms_cfg_cnf_tlv_enum {
    mipc_sms_cfg_cnf_tlv_NONE = 0,
    /* It indicates the format configured for this protocol; this field is parsed from the result of CMGF */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_CFG_CNF_T_FORMAT                               = 0x100,
    /* It indicate the SCA (SMS Center Address) configured for this protocol */
    /* type = string */
    MIPC_SMS_CFG_CNF_T_SCA                                  = 0x8101,
    /* It indicates if the SMS module is initialized */
    /* type = uint8_t, refer to SMS_STATE */
    MIPC_SMS_CFG_CNF_T_SMS_STATE                            = 0x102,
    /* It indicates the maximum number of messages can be saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_CFG_CNF_T_MAX_MESSAGE                          = 0x103,
    /* True: user replies ACK to network; False: modem replies ACK to network; if message is saved in SIM card: modem replies ACK to network */
    /* type = uint8_t, refer to SMS_ACK */
    MIPC_SMS_CFG_CNF_T_PREFER_ACK                           = 0x104,
    /* It indicates the user's preferred storage used to save MT SMS; TE: New MT message is denoted by MIPC_SMS_NEW_SMS_IND(PREFER_ACK=FALSE) or MIPC_SMS_NEW_SMS_CMD(PREFER_ACK=TRUE); SIM: New MT message is denoted by MIPC_SMS_STORE_STATUS_IND; this configuration option is only a recommendation (currently, CBM and STATUS_REPORT cannot be saved in SIM) */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_CNF_T_PREFER_STORAGE                       = 0x105,
    /* It indicates the number of used messages saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_CFG_CNF_T_USED_MESSAGE                         = 0x106,
    /* It indicates the total 3GPP format messages number that can be saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_CFG_CNF_T_TOTAL_MESSAGE                        = 0x107,
    /* [3GPP] refer TS27.005 3.3.2 <fo> */
    /* type = uint8_t */
    MIPC_SMS_CFG_CNF_T_TEXT_MODE_FO                         = 0x10A,
    /* [3GPP] refer TS27.005 3.3.2 <vp> */
    /* type = uint8_t */
    MIPC_SMS_CFG_CNF_T_TEXT_MODE_VP                         = 0x10B,
    /* [3GPP] refer TS27.005 3.3.2 <pid> */
    /* type = uint8_t */
    MIPC_SMS_CFG_CNF_T_TEXT_MODE_PID                        = 0x10C,
    /* [3GPP] refer TS27.005 3.3.2 <dcs> */
    /* type = uint8_t */
    MIPC_SMS_CFG_CNF_T_TEXT_MODE_DCS                        = 0x10D,
    /* [PHASE OUT] */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_CNF_T_PREFER_STORAGE_C2K                   = 0x10E,
    /* [PHASE OUT] */
    /* type = uint16_t */
    MIPC_SMS_CFG_CNF_T_USED_MESSAGE_C2K                     = 0x10F,
    /* [PHASE OUT] */
    /* type = uint16_t */
    MIPC_SMS_CFG_CNF_T_MAX_MESSAGE_C2K                      = 0x110,
    /* It indicates the location for performing read/write/delete operation for SMS in 3GPP format; if PREFER_STORAGE is TE, this TLV can be configured; if PREFER_STORAGE is not TE, W_R_D_STORAGE is same as PREFER_STORAGE; this field is not necessary for 3GPP2 because 3GPP2 is always UIM and cannot be changed */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_CNF_T_W_R_D_STORAGE                        = 0x111,
};

    /* MIPC_MSG.SMS_SEND_REQ */
enum mipc_sms_send_req_tlv_enum {
    mipc_sms_send_req_tlv_NONE = 0,
    /* It indicates the format of the message to be sent; Default value is PDU_3GPP */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_SEND_REQ_T_FORMAT                              = 0x100,
    /* PDU contains SCA; refer to the PDU parameter in CMGS; this field is only for 3GPP */
    /* type = byte_array */
    MIPC_SMS_SEND_REQ_T_PDU                                 = 0x8101,
    /* [PHASE OUT]the length of TPDU(without SCA) in PDU IE */
    /* type = uint8_t */
    MIPC_SMS_SEND_REQ_T_PDU_LEN                             = 0x102,
    /* Save the message which is sent to device or SIM; default is no_save */
    /* type = uint8_t, refer to SMS_SEND_SAVE */
    MIPC_SMS_SEND_REQ_T_SAVE                                = 0x103,
    /* It indicates if there is more SMS to be sent, enable to keep link opened; default is NULL */
    /* type = uint8_t, refer to SMS_MORE_MSG_TO_SEND */
    MIPC_SMS_SEND_REQ_T_MORE_MSG_TO_SEND                    = 0x104,
    /* [PDU_3GPP2] refer to the 2nd parmameter of EC2KCMGS */
    /* type = byte_array */
    MIPC_SMS_SEND_REQ_T_PDU_C2K                             = 0x8105,
    /* [PDU_3GPP2]  CDMA dest_address in decimal number string(length contains the /0); refer to the 3rd parmameter of EC2KCMGS */
    /* type = string */
    MIPC_SMS_SEND_REQ_T_NUM_C2K                             = 0x8106,
};

    /* MIPC_MSG.SMS_SEND_CNF */
enum mipc_sms_send_cnf_tlv_enum {
    mipc_sms_send_cnf_tlv_NONE = 0,
    /* Message reference of sent message, this field is for 3GPP */
    /* type = uint8_t */
    MIPC_SMS_SEND_CNF_T_MR                                  = 0x103,
    /* If the short message needs to be saved, this field will carry the valid message index in storage */
    /* type = uint16_t */
    MIPC_SMS_SEND_CNF_T_MESSAGE_INDEX                       = 0x104,
    /* [PDU_3GPP2] error class of C2K operation */
    /* type = uint8_t, refer to SMS_C2K_ERR_CLASS */
    MIPC_SMS_SEND_CNF_T_ERR_CLASS_C2K                       = 0x105,
    /* It indicates the format sent */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_SEND_CNF_T_FORMAT                              = 0x106,
    /* [PDU_3GPP2] C2K error code */
    /* type = uint16_t, refer to SMS_C2K_ERR_CODE */
    MIPC_SMS_SEND_CNF_T_ERR_CODE_C2K                        = 0x107,
    /* As define in PDU_3GPP, refer to the <ackpdu> in CMGS; it is currently not supported */
    /* type = byte_array */
    MIPC_SMS_SEND_CNF_T_ACK_PDU                             = 0x8108,
    /* [PDU_3GPP2] message id(refer CS0015 4.5.1 MESSAGE_ID) of sent message */
    /* type = uint16_t */
    MIPC_SMS_SEND_CNF_T_MSGID_C2K                           = 0x109,
};

    /* MIPC_MSG.SMS_READ_REQ */
enum mipc_sms_read_req_tlv_enum {
    mipc_sms_read_req_tlv_NONE = 0,
    /* It indicates the format to be read; Default value is PDU_3GPP */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_READ_REQ_T_FORMAT                              = 0x100,
    /* It indicates the type of short message to be read */
    /* type = uint8_t, refer to SMS_FLAG */
    MIPC_SMS_READ_REQ_T_FLAG                                = 0x101,
    /* It indicates the short message to be read and is valid only when flag is INDEX; if flag is INDEX and the MIPC command carries this field, this field should be 0; Default value is 0 */
    /* type = uint16_t */
    MIPC_SMS_READ_REQ_T_MESSAGE_INDEX                       = 0x102,
    /* Keep the status (refer to SMS_STATUS) unchanged; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_READ_REQ_T_STATUS_UNCHANGE                     = 0x103,
    /* [3GPP]Which storage to read; The default value would be the PREFER_STORAGE(if PREFER_STORAGE is not TE) or W_R_D_STORAGE(if PREFER_STORAGE is TE) in MIPC_SMS_CFG_REQ */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_READ_REQ_T_STORAGE                             = 0x104,
};

    /* MIPC_MSG.SMS_READ_CNF */
enum mipc_sms_read_cnf_tlv_enum {
    mipc_sms_read_cnf_tlv_NONE = 0,
    /* The format of short message to be read */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_READ_CNF_T_FORMAT                              = 0x100,
    /* The count of short message to be read */
    /* type = uint16_t */
    MIPC_SMS_READ_CNF_T_PDU_COUNT                           = 0x103,
    /* The list of short message to be read */
    /* type = struct_array, refer to sms_pdu */
    MIPC_SMS_READ_CNF_T_PDU_LIST                            = 0x8104,
    /* [PDU_3GPP2] read sms */
    /* type = byte_array */
    MIPC_SMS_READ_CNF_T_PDU_C2K                             = 0x8105,
};

    /* MIPC_MSG.SMS_DELETE_REQ */
enum mipc_sms_delete_req_tlv_enum {
    mipc_sms_delete_req_tlv_NONE = 0,
    /* It indicates the type of short message to be deleted */
    /* type = uint8_t, refer to SMS_FLAG */
    MIPC_SMS_DELETE_REQ_T_FLAG                              = 0x100,
    /* It indicates the short message to be deleted; it is valid only when flag is INDEX; if flag is INDEX and the command carries this field, the value of this field should be 0 */
    /* type = uint16_t */
    MIPC_SMS_DELETE_REQ_T_MESSAGE_INDEX                     = 0x101,
    /* It indicates the format to deleted, default is PDU_3GPP */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_DELETE_REQ_T_FORMAT                            = 0x102,
    /* [3GPP]Which storage to delete; The default value would be the PREFER_STORAGE(if PREFER_STORAGE is not TE) or W_R_D_STORAGE(if PREFER_STORAGE is TE) in MIPC_SMS_CFG_REQ */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_DELETE_REQ_T_STORAGE                           = 0x103,
};

    /* MIPC_MSG.SMS_DELETE_CNF */
enum mipc_sms_delete_cnf_tlv_enum {
    mipc_sms_delete_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SMS_GET_STORE_STATUS_REQ */
enum mipc_sms_get_store_status_req_tlv_enum {
    mipc_sms_get_store_status_req_tlv_NONE = 0,
    /* [PHASE OUT] use format to indicate */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_GET_STORE_STATUS_REQ_T_STORAGE                 = 0x100,
    /* PDU_3GPP: Acquire status of W_R_D_STORAGE; PDU_3GPP2: Acquire status of UIM; Default value is PDU_3GPP */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_GET_STORE_STATUS_REQ_T_FORMAT                  = 0x101,
};

    /* MIPC_MSG.SMS_GET_STORE_STATUS_CNF */
enum mipc_sms_get_store_status_cnf_tlv_enum {
    mipc_sms_get_store_status_cnf_tlv_NONE = 0,
    /* It indicates whether the storage is full or not */
    /* type = bitmap, refer to SMS_STORE_FLAG */
    MIPC_SMS_GET_STORE_STATUS_CNF_T_FLAG                    = 0x100,
    /* [PHASE OUT] */
    /* type = uint16_t */
    MIPC_SMS_GET_STORE_STATUS_CNF_T_MESSAGE_INDEX           = 0x101,
    /* It indicates the maximum number of messages that can be saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_GET_STORE_STATUS_CNF_T_MAX_MESSAGE             = 0x102,
    /* It indicate the number of short messages stored in this protocol */
    /* type = uint16_t */
    MIPC_SMS_GET_STORE_STATUS_CNF_T_USED_MESSAGE            = 0x103,
    /* [PHASE OUT] */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_GET_STORE_STATUS_CNF_T_STORAGE                 = 0x104,
};

    /* MIPC_MSG.SMS_WRITE_REQ */
enum mipc_sms_write_req_tlv_enum {
    mipc_sms_write_req_tlv_NONE = 0,
    /* It indicates the format of message to be written to device; defaut is PDU_FORMAT */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_WRITE_REQ_T_FORMAT                             = 0x100,
    /* PDU in bytearray format */
    /* type = byte_array */
    MIPC_SMS_WRITE_REQ_T_PDU                                = 0x8101,
    /* [PHASE OUT]the length of TPDU(without SCA) */
    /* type = uint8_t */
    MIPC_SMS_WRITE_REQ_T_PDU_LEN                            = 0x102,
    /* [3GPP] Which storage to write; If not carry this, the default value would be the CFG_REQ:PREFER_STORAGE(if this is not TE), or CFG_REQ:W_R_D_STORAGE */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_WRITE_REQ_T_STORAGE                            = 0x103,
    /* Message status */
    /* type = uint8_t, refer to SMS_STATUS */
    MIPC_SMS_WRITE_REQ_T_STATUS                             = 0x104,
    /* [PHASE OUT] could use PDU now */
    /* type = byte_array */
    MIPC_SMS_WRITE_REQ_T_PDU_C2K                            = 0x8105,
};

    /* MIPC_MSG.SMS_WRITE_CNF */
enum mipc_sms_write_cnf_tlv_enum {
    mipc_sms_write_cnf_tlv_NONE = 0,
    /* The message index of the written short message */
    /* type = uint16_t */
    MIPC_SMS_WRITE_CNF_T_MESSAGE_INDEX                      = 0x100,
};

    /* MIPC_MSG.SMS_CBM_CFG_REQ */
enum mipc_sms_cbm_cfg_req_tlv_enum {
    mipc_sms_cbm_cfg_req_tlv_NONE = 0,
    /* For 3GPP format CBM only; 0: Disable 3GPP CBM; others: Enable 3GPP CBM */
    /* type = bitmap, refer to SMS_CBM_TYPE */
    MIPC_SMS_CBM_CFG_REQ_T_OPEN_CBM_TYPE                    = 0x100,
    /* Set configuration type of MSG_ID */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_CFG_TYPE                  = 0x101,
    /* Allow or ban msg_id range configuration (can be array format, 30 elements at most); the most significant 16 bits indicate the smallest number of msg_id in this range; the least significant 16 bits indicate the largest number of msg_id in this range; a range contains start msg_id and end msg_id */
    /* type = uint32_t */
    MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_RANGE                     = 0x102,
    /* Allow or ban msg_id configuration (can be array format, 60 elements at most) */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_REQ_T_MSG_ID_SINGLE                    = 0x103,
    /* Set configuration type of DCS */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_REQ_T_DCS_CFG_TYPE                     = 0x104,
    /* Allow or ban DCS range configuration (can be array format, 30 elements at most); the most significant 8 bits indicate the smallest number of DCS in this range; the least significant 8 bits indicate the largest number of DCS in this range; a range contains start and end DCS */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_REQ_T_DCS_RANGE                        = 0x105,
    /* Allow or ban DCS range configuration (can be array format, 60 elements at most), similar to MSG_ID_SINGLE */
    /* type = uint8_t */
    MIPC_SMS_CBM_CFG_REQ_T_DCS_SINGLE                       = 0x106,
    /* For 3GPP format CBM only; For AT+ETWS compatible */
    /* type = bitmap, refer to SMS_ETWS_PRIMARY_TYPE */
    MIPC_SMS_CBM_CFG_REQ_T_ETWS_PRIMARY_CFG                 = 0x107,
    /* It indicates the format for configuration; it can only be pdu_3gpp or pdu_3gpp2; Default value is PDU_3GPP */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_CBM_CFG_REQ_T_FORMAT                           = 0x108,
    /* [3GPP2] for compatible with AT+ECSCB, default is on */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CBM_CFG_REQ_T_C2K_CBM_ENABLE                   = 0x109,
    /* For 3GPP CBM, follow the format of +CBM URC to enable/disable the PDU_3GPP_SEG in MIPC_SMS_NEW_CBM_IND */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CBM_CFG_REQ_T_PDU_3GPP_SEG_ENABLE              = 0x10A,
};

    /* MIPC_MSG.SMS_CBM_CFG_CNF */
enum mipc_sms_cbm_cfg_cnf_tlv_enum {
    mipc_sms_cbm_cfg_cnf_tlv_NONE = 0,
    /* For 3GPP format CBM only; 0: Disable 3GPP CBM; others: Enable 3GPP CBM */
    /* type = bitmap, refer to SMS_CBM_TYPE */
    MIPC_SMS_CBM_CFG_CNF_T_OPEN_CBM_TYPE                    = 0x100,
    /* It indicates <MSG_ID_CFG_TYPE> saved in device; this field may exist when MSG_ID_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_CFG_TYPE                  = 0x101,
    /* It indicates <MSG_ID_RANGE> saved in device; this field may exist when MSG_ID_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists */
    /* type = uint32_t */
    MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_RANGE                     = 0x102,
    /* It indicates <MSG_ID_SINGLE> saved in device; this field may exist when MSG_ID_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_CNF_T_MSG_ID_SINGLE                    = 0x103,
    /* It indicates <DCS_CFG_TYPE> saved in device; this field may exist when DCS_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_CNF_T_DCS_CFG_TYPE                     = 0x104,
    /* It indicates <DCS_RANGE> saved in device; this field may exist when DCS_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_CNF_T_DCS_RANGE                        = 0x105,
    /* It indicates <DCS_SINGLE> saved in device; this field may exist when DCS_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists */
    /* type = uint8_t */
    MIPC_SMS_CBM_CFG_CNF_T_DCS_SINGLE                       = 0x106,
    /* For 3GPP format CBM only; It indicates the language configuration for DCS saved in device; this field may exist when DCS_CFG_TYPE of MIPC_SMS_CBM_CFG_REQ exists; follow DCS configuration to ban or allow this setting */
    /* type = uint8_t */
    MIPC_SMS_CBM_CFG_CNF_T_LANGUAGE_SINGLE                  = 0x107,
    /* It indicates the format for configuration CNF, can only be pdu_3gpp or pdu_3gpp2 */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_CBM_CFG_CNF_T_FORMAT                           = 0x108,
    /* [3GPP2] for compatible with AT+ECSCB, when FORMAT=3GPP2, would carry this option */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CBM_CFG_CNF_T_C2K_CBM_ENABLE                   = 0x109,
};

    /* MIPC_MSG.SMS_SCBM_REQ */
enum mipc_sms_scbm_req_tlv_enum {
    mipc_sms_scbm_req_tlv_NONE = 0,
    /* Device will notify host when the device enters SCBM mode using MIPC_SMS_SCBM_IND; host uses this value to decide whether device should be quitted immediately (or device will be auto quitted 5 minutes later); Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_SCBM_REQ_T_QUIT_SCBM_MODE                      = 0x100,
};

    /* MIPC_MSG.SMS_SCBM_CNF */
enum mipc_sms_scbm_cnf_tlv_enum {
    mipc_sms_scbm_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SMS_DOMAIN_REQ */
enum mipc_sms_domain_req_tlv_enum {
    mipc_sms_domain_req_tlv_NONE = 0,
    /* CGSMS configuration */
    /* type = uint8_t, refer to SMS_ACTION */
    MIPC_SMS_DOMAIN_REQ_T_CGSMS_ACTION                      = 0x137,
    /* Must carry this tlv if CGSMS_ACTION is _SET; This TLV would be ignored if CGSMS_ACTION is not _SET */
    /* type = uint8_t, refer to SMS_CGSMS */
    MIPC_SMS_DOMAIN_REQ_T_CGSMS_VALUE                       = 0x118,
    /* SMS_IMS configuration */
    /* type = uint8_t, refer to SMS_ACTION */
    MIPC_SMS_DOMAIN_REQ_T_SMS_IMS_ACTION                    = 0x138,
    /* Must carry this tlv if SMS_IMS_ACTION is _SET; This TLV would be ignored if SMS_IMS_ACTION is not _SET */
    /* type = uint8_t, refer to SMS_IMS_CFG */
    MIPC_SMS_DOMAIN_REQ_T_SMS_IMS_VALUE                     = 0x119,
};

    /* MIPC_MSG.SMS_DOMAIN_CNF */
enum mipc_sms_domain_cnf_tlv_enum {
    mipc_sms_domain_cnf_tlv_NONE = 0,
    /* CGSMS configuration; Exist when CGSMS_ACTION is _SET/_GET in _REQ */
    /* type = uint8_t, refer to SMS_CGSMS */
    MIPC_SMS_DOMAIN_CNF_T_CGSMS_VALUE                       = 0x118,
    /* SMS_IMS configuration; Exist when SMS_IMS_ACTION is _SET/_GET in _REQ */
    /* type = uint8_t, refer to SMS_IMS_CFG */
    MIPC_SMS_DOMAIN_CNF_T_SMS_IMS_VALUE                     = 0x119,
};

    /* MIPC_MSG.SMS_CFG_V2_REQ */
enum mipc_sms_cfg_v2_req_tlv_enum {
    mipc_sms_cfg_v2_req_tlv_NONE = 0,
    /* [3GPP] Refer Figure 8.5/3GPP TS24.011, dont contain the iei&len byte. */
    /* type = byte_array */
    MIPC_SMS_CFG_V2_REQ_T_SET_SCA                           = 0x8100,
    /* [3GPP] Carry this flag as true would trigger a AT+CSAS, mainly save SCA; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CFG_V2_REQ_T_SAVE_SET                          = 0x101,
    /* [3GPP] If user needs to query the SCA, carry this flag as true; ignore this field if SET_SCA exist; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CFG_V2_REQ_T_GET_SCA                           = 0x102,
    /* [3GPP] Handle mode for 3gpp MT SMS. RELAY_MODE:MD would relay the raw_pdu to HOST via MIPC_IND/CMD; SAVE_MODE:MD would save the raw_pdu in MD, and notify msg_idx to HOST via MIPC_IND, some special case<e.g. class0> would force to RELAY_MODE */
    /* type = uint8_t, refer to SMS_HANDLE_MODE */
    MIPC_SMS_CFG_V2_REQ_T_MT_HANDLE_MODE                    = 0x103,
    /* [3GPP] Set default MD SMS storage */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_CFG_V2_REQ_T_SAVE_STORAGE                      = 0x104,
    /* [3GPP] When 3gpp_mt_sms in RELAY_MODE, this would determine who trigger the ack to NW. HOST_ACK: notify HOST with MIPC_CMD(HOST need ack a MIPC_RSP then); MD_ACK: motify HOST with MIPC_IND */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_REQ_T_MT_ACK_MODE                       = 0x105,
    /* [3GPP] When 3gpp_status_report in RELAY_MODE(MD only support RELAY_MODE for status report), this would determine who trigger the ack to NW */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_REQ_T_SR_ACK_MODE                       = 0x106,
    /* [3GPP2] When 3gpp2_mt_sms in RELAY_MODE(MD only support RELAY_MODE for 3gpp2_mt_sms), this would determine who trigger the ack to NW(sometimes NW dont need UE perform ack<NW dont carry bearer_reply_option refer 3.4.3.5 in 3GPP2 C.S0015-C v1.0>, that would be also notified with MIPC_IND even set HOST_ACK) */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_REQ_T_MT_ACK_MODE_3GPP2                 = 0x107,
    /* [3GPP2] When 3gpp2_status_report in RELAY_MODE(MD only support RELAY_MODE for 3gpp2_status_report), this would determine who trigger the ack to NW(sometimes NW dont need UE perform ack<NW dont carry bearer_reply_option refer 3.4.3.5 in 3GPP2 C.S0015-C v1.0>, that would be also notified with MIPC_IND even set HOST_ACK) */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_REQ_T_SR_ACK_MODE_3GPP2                 = 0x108,
};

    /* MIPC_MSG.SMS_CFG_V2_CNF */
enum mipc_sms_cfg_v2_cnf_tlv_enum {
    mipc_sms_cfg_v2_cnf_tlv_NONE = 0,
    /* [3GPP] Current setting, the format is as same as the field 'SET_SCA' in MIPC_SMS_CFG_REQ of message version = 2 */
    /* type = byte_array */
    MIPC_SMS_CFG_V2_CNF_T_SCA                               = 0x8100,
    /* [3GPP] Current setting */
    /* type = uint8_t, refer to SMS_HANDLE_MODE */
    MIPC_SMS_CFG_V2_CNF_T_MT_HANDLE_MODE                    = 0x101,
    /* [3GPP]current setting */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_CFG_V2_CNF_T_SAVE_STORAGE                      = 0x102,
    /* [3GPP]current setting */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_CNF_T_MT_ACK_MODE                       = 0x103,
    /* [3GPP]current setting */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_CNF_T_SR_ACK_MODE                       = 0x104,
    /* [3GPP2]current setting */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_CNF_T_MT_ACK_MODE_3GPP2                 = 0x105,
    /* [3GPP2]current setting */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_CNF_T_SR_ACK_MODE_3GPP2                 = 0x106,
};

    /* MIPC_MSG.SMS_SEND_V2_REQ */
enum mipc_sms_send_v2_req_tlv_enum {
    mipc_sms_send_v2_req_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_SEND_V2_REQ_T_STANDARD                         = 0x100,
    /* [3GPP/3GPP2]3GPP: refer 3GPP TS27.005 4.3 AT+CMGS <pdu>; 3GPP2: refer 3GPP2 C.S0015-C 3.4.1 */
    /* type = byte_array */
    MIPC_SMS_SEND_V2_REQ_T_PDU                              = 0x8101,
    /* [3GPP2] dest_address in decimal number string(length contains the /0), refer to the 3rd parmameter of EC2KCMGS */
    /* type = string */
    MIPC_SMS_SEND_V2_REQ_T_NUM_3GPP2                        = 0x8102,
};

    /* MIPC_MSG.SMS_SEND_V2_CNF */
enum mipc_sms_send_v2_cnf_tlv_enum {
    mipc_sms_send_v2_cnf_tlv_NONE = 0,
    /* [3GPP/3GPP2]3GPP: refer 3GPP TS27.005 4.3 AT+CMGS <mr>; 3GPP2: refer 3GPP2 C.S0015-C 4.5.1 message identifier */
    /* type = uint16_t */
    MIPC_SMS_SEND_V2_CNF_T_SMS_ID                           = 0x100,
    /* [3GPP2]refer 3GPP2 C.S0015-C 3.4.3.6 */
    /* type = uint8_t */
    MIPC_SMS_SEND_V2_CNF_T_ERROR_CLASS_3GPP2                = 0x101,
    /* [3GPP2]refer 3GPP2 C.S0015-C 3.4.3.6 */
    /* type = uint8_t */
    MIPC_SMS_SEND_V2_CNF_T_CAUSE_CODE_3GPP2                 = 0x102,
};

    /* MIPC_MSG.SMS_READ_V2_REQ */
enum mipc_sms_read_v2_req_tlv_enum {
    mipc_sms_read_v2_req_tlv_NONE = 0,
    /* [3GPP/3GPP2] modem only support 3GPP; Default value is 3GPP */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_READ_V2_REQ_T_STANDARD                         = 0x100,
    /* [3GPP] The storage to read; if user not carry this field, use the field 'SAVE_STORAGE' in MIPC_SMS_CFG_REQ of message version = 2 as default */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_READ_V2_REQ_T_STORAGE                          = 0x101,
    /* [3GPP] The type of sort message to be read */
    /* type = uint8_t, refer to SMS_FLAG */
    MIPC_SMS_READ_V2_REQ_T_FLAG                             = 0x102,
    /* [3GPP] The index of short message to be read, only valid when the field 'FLAG' is INDEX */
    /* type = uint16_t */
    MIPC_SMS_READ_V2_REQ_T_MSG_IDX                          = 0x103,
    /* [3GPP]keep the status(refer to SMS_STATUS) unchange. default value is false(saved unread sms in MD would gose to read status in modem). Only supported in CPE&DATACARD project; Default value is FALSE */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_READ_V2_REQ_T_STATUS_UNCHANGE                  = 0x104,
};

    /* MIPC_MSG.SMS_READ_V2_CNF */
enum mipc_sms_read_v2_cnf_tlv_enum {
    mipc_sms_read_v2_cnf_tlv_NONE = 0,
    /* [3GPP]memory storage index */
    /* type = uint16_t */
    MIPC_SMS_READ_V2_CNF_T_MSG_IDX                          = 0x100,
    /* [3GPP]refer 3GPP TS27.005 4.3 AT+CMGS <pdu> */
    /* type = byte_array */
    MIPC_SMS_READ_V2_CNF_T_PDU                              = 0x8101,
    /* [3GPP] */
    /* type = uint8_t, refer to SMS_STATUS */
    MIPC_SMS_READ_V2_CNF_T_STATUS                           = 0x102,
};

    /* MIPC_MSG.SMS_DELETE_V2_REQ */
enum mipc_sms_delete_v2_req_tlv_enum {
    mipc_sms_delete_v2_req_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_DELETE_V2_REQ_T_STANDARD                       = 0x100,
    /* [3GPP/3GPP2]the storage to delete.3GPP: use SMS:CFG_V2_REQ:SAVE_STORAGE as default if not carry; 3GPP2: only support UIM, use UIM as default if not carry */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_DELETE_V2_REQ_T_STORAGE                        = 0x101,
    /* [3GPP/3GPP2]It indicates the type of short message to be deleted.3GPP2: only support ALL&INDEX */
    /* type = uint8_t, refer to SMS_FLAG */
    MIPC_SMS_DELETE_V2_REQ_T_FLAG                           = 0x102,
    /* It indicates the short message to be deleted; it is valid only when FLAG is INDEX; 1-based(index start from 1) */
    /* type = uint16_t */
    MIPC_SMS_DELETE_V2_REQ_T_MSG_IDX                        = 0x103,
};

    /* MIPC_MSG.SMS_DELETE_V2_CNF */
enum mipc_sms_delete_v2_cnf_tlv_enum {
    mipc_sms_delete_v2_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SMS_GET_STORE_STATUS_V2_REQ */
enum mipc_sms_get_store_status_v2_req_tlv_enum {
    mipc_sms_get_store_status_v2_req_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_GET_STORE_STATUS_V2_REQ_T_STANDARD             = 0x100,
    /* [3GPP/3GPP2]the storage to query.3GPP: use SAVE_STORAGE as default if not carry; 3GPP2: only support UIM, use UIM as default if not carry */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_GET_STORE_STATUS_V2_REQ_T_STORAGE              = 0x101,
};

    /* MIPC_MSG.SMS_GET_STORE_STATUS_V2_CNF */
enum mipc_sms_get_store_status_v2_cnf_tlv_enum {
    mipc_sms_get_store_status_v2_cnf_tlv_NONE = 0,
    /* It indicates the maximum number of messages that can be saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_GET_STORE_STATUS_V2_CNF_T_TOTAL                = 0x100,
    /* It indicate the number of short messages stored in this protocol */
    /* type = uint16_t */
    MIPC_SMS_GET_STORE_STATUS_V2_CNF_T_USED                 = 0x101,
};

    /* MIPC_MSG.SMS_WRITE_V2_REQ */
enum mipc_sms_write_v2_req_tlv_enum {
    mipc_sms_write_v2_req_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_WRITE_V2_REQ_T_STANDARD                        = 0x100,
    /* [3GPP/3GPP2]the storage to write.3GPP: use SMS:CFG_V2_REQ:SAVE_STORAGE as default if not carry; 3GPP2: only support UIM, use UIM as default if not carry */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_WRITE_V2_REQ_T_STORAGE                         = 0x101,
    /* [3GPP/3GPP2]3GPP: refer 3GPP TS27.005 4.3 AT+CMGS <pdu>; 3GPP2: refer 3GPP2 C.S0015-C 3.4.1 */
    /* type = byte_array */
    MIPC_SMS_WRITE_V2_REQ_T_PDU                             = 0x8102,
    /* [3GPP/3GPP2] SMS status */
    /* type = uint8_t, refer to SMS_STATUS */
    MIPC_SMS_WRITE_V2_REQ_T_STATUS                          = 0x103,
};

    /* MIPC_MSG.SMS_WRITE_V2_CNF */
enum mipc_sms_write_v2_cnf_tlv_enum {
    mipc_sms_write_v2_cnf_tlv_NONE = 0,
    /* [3GPP/3GPP2] writen index */
    /* type = uint16_t */
    MIPC_SMS_WRITE_V2_CNF_T_MSG_IDX                         = 0x100,
};

    /* MIPC_MSG.SMS_CBM_CFG_V2_REQ */
enum mipc_sms_cbm_cfg_v2_req_tlv_enum {
    mipc_sms_cbm_cfg_v2_req_tlv_NONE = 0,
    /* [3GPP] For AT+ETWS compatible */
    /* type = bitmap, refer to SMS_ETWS_PRIMARY_TYPE */
    MIPC_SMS_CBM_CFG_V2_REQ_T_ETWS_PRIMARY_CFG              = 0x100,
    /* [3GPP/3GPP2], the standard this MIPC_REQ to set */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_CBM_CFG_V2_REQ_T_STANDARD                      = 0x101,
    /* [3GPP/3GPP2] enable/disable CBM for corresponding STANDARD */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CBM_CFG_V2_REQ_T_ENABLE                        = 0x102,
    /* [3GPP/3GPP2]Set configuration type of channel.3GPP:TS23.041 9.4.1.2.2 Message Identifier; 3GPP2:9.3 Service Category in C.R1001 */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_V2_REQ_T_CH_CFG_TYPE                   = 0x103,
    /* [3GPP/3GPP2]Allow or ban channel range configuration (can be array format, 30 elements at most); the most significant 16 bits indicate the smallest number of channel in this range; the least significant 16 bits indicate the largest number of channel in this range; a range contains start channel and end channel */
    /* type = uint32_t */
    MIPC_SMS_CBM_CFG_V2_REQ_T_CH_RANGE                      = 0x104,
    /* [3GPP/3GPP2]Allow or ban channel configuration (can be array format, 60 elements at most) */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_V2_REQ_T_CH_SINGLE                     = 0x105,
    /* [3GPP/3GPP2]Set configuration type of language. 3GPP:DCS; 3GPP2:C.R1001 language indicators */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_V2_REQ_T_LAN_CFG_TYPE                  = 0x106,
    /* [3GPP/3GPP2]Allow or ban language range configuration (can be array format, 30 elements at most); the most significant 8 bits indicate the smallest number of language in this range; the least significant 8 bits indicate the largest number of language in this range; a range contains start and end language */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_V2_REQ_T_LAN_RANGE                     = 0x107,
    /* [3GPP/3GPP2]Allow or ban language range configuration (can be array format, 60 elements at most), similar to CH_SINGLE */
    /* type = uint8_t */
    MIPC_SMS_CBM_CFG_V2_REQ_T_LAN_SINGLE                    = 0x108,
    /* [3GPP]For 3GPP CBM, follow the format of +CBM URC to enable/disable the PDU_SEG in SMS:NEW_CBM_V2_IND */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CBM_CFG_V2_REQ_T_SEG_ENABLE                    = 0x109,
};

    /* MIPC_MSG.SMS_CBM_CFG_V2_CNF */
enum mipc_sms_cbm_cfg_v2_cnf_tlv_enum {
    mipc_sms_cbm_cfg_v2_cnf_tlv_NONE = 0,
    /* [3GPP/3GPP2], the standard configured */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_CBM_CFG_V2_CNF_T_STANDARD                      = 0x100,
    /* [3GPP/3GPP2] enable CBM for corresponding STANDARD */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_CBM_CFG_V2_CNF_T_ENABLE                        = 0x101,
    /* [3GPP/3GPP2]Set configuration type of channel.3GPP:TS23.041 9.4.1.2.2 Message Identifier; 3GPP2:9.3 Service Category in C.R1001 */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_V2_CNF_T_CH_CFG_TYPE                   = 0x102,
    /* [3GPP/3GPP2]Allow or ban channel range configuration (can be array format, 30 elements at most); the most significant 16 bits indicate the smallest number of channel in this range; the least significant 16 bits indicate the largest number of channel in this range; a range contains start channel and end channel */
    /* type = uint32_t */
    MIPC_SMS_CBM_CFG_V2_CNF_T_CH_RANGE                      = 0x103,
    /* [3GPP/3GPP2]Allow or ban channel configuration (can be array format, 60 elements at most) */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_V2_CNF_T_CH_SINGLE                     = 0x104,
    /* [3GPP/3GPP2]Set configuration type of language. 3GPP:DCS; 3GPP2:C.R1001 language indicators */
    /* type = uint8_t, refer to SMS_CBM_CFG_TYPE */
    MIPC_SMS_CBM_CFG_V2_CNF_T_LAN_CFG_TYPE                  = 0x105,
    /* [3GPP/3GPP2]Allow or ban language range configuration (can be array format, 30 elements at most); the most significant 8 bits indicate the smallest number of language in this range; the least significant 8 bits indicate the largest number of language in this range; a range contains start and end language */
    /* type = uint16_t */
    MIPC_SMS_CBM_CFG_V2_CNF_T_LAN_RANGE                     = 0x106,
    /* [3GPP/3GPP2]Allow or ban language range configuration (can be array format, 60 elements at most), similar to CH_SINGLE */
    /* type = uint8_t */
    MIPC_SMS_CBM_CFG_V2_CNF_T_LAN_SINGLE                    = 0x107,
    /* [3GPP/3GPP2]expand the LAN_RANGE/LAN_SINGLE into single format(not use range format) */
    /* type = uint8_t */
    MIPC_SMS_CBM_CFG_V2_CNF_T_LANGUAGE_SINGLE               = 0x108,
};

    /* MIPC_MSG.SMS_CFG_IND */
enum mipc_sms_cfg_ind_tlv_enum {
    mipc_sms_cfg_ind_tlv_NONE = 0,
    /* It indicates the format configured for this protocol */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_CFG_IND_T_FORMAT                               = 0x100,
    /* It indicates the SCA (SMS Center Address) configured for this protocol */
    /* type = string */
    MIPC_SMS_CFG_IND_T_SCA                                  = 0x8101,
    /* It indicates if the SMS module is initialized */
    /* type = uint8_t, refer to SMS_STATE */
    MIPC_SMS_CFG_IND_T_SMS_STATE                            = 0x102,
    /* It indicates the maximum number of messages that can be saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_CFG_IND_T_MAX_MESSAGE                          = 0x103,
    /* True: user replies ACK to network; False: modem replies ACK to network; if message is saved in SIM card: modem replies ACK to network */
    /* type = uint8_t, refer to SMS_ACK */
    MIPC_SMS_CFG_IND_T_PREFER_ACK                           = 0x104,
    /* It indicates the user's preferred storage used to save MT SMS (new MT SMS should use <save_in_modem or relay_to_ap> mode) */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_IND_T_PREFER_STORAGE                       = 0x105,
    /* It indicates the used messages number saved in this protocol */
    /* type = uint16_t */
    MIPC_SMS_CFG_IND_T_USED_MESSAGE                         = 0x106,
    /* [PHASE OUT] */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_IND_T_PREFER_STORAGE_C2K                   = 0x107,
    /* [PHASE OUT] */
    /* type = uint16_t */
    MIPC_SMS_CFG_IND_T_USED_MESSAGE_C2K                     = 0x108,
    /* [PHASE OUT] */
    /* type = uint16_t */
    MIPC_SMS_CFG_IND_T_MAX_MESSAGE_C2K                      = 0x109,
    /* For 3GPP format SMS only, it indicates the location for performing read/write/delete operation for SMS in 3GPP format SMS; this field is not necessary for 3GPP2 because 3GPP2 is always UIM and cannot be changed */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_CFG_IND_T_W_R_D_STORAGE                        = 0x10A,
};

    /* MIPC_MSG.SMS_NEW_SMS_IND */
enum mipc_sms_new_sms_ind_tlv_enum {
    mipc_sms_new_sms_ind_tlv_NONE = 0,
    /* The format of this SMS */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_NEW_SMS_IND_T_FORMAT                           = 0x100,
    /* Should be 1 in this case */
    /* type = uint16_t */
    MIPC_SMS_NEW_SMS_IND_T_PDU_COUNT                        = 0x103,
    /* When format is PDU_3GPP */
    /* type = struct_array, refer to sms_pdu */
    MIPC_SMS_NEW_SMS_IND_T_PDU_LIST                         = 0x8104,
    /* [PDU_3GPP2]valid when 3GPP2 format */
    /* type = byte_array */
    MIPC_SMS_NEW_SMS_IND_T_PDU_C2K                          = 0x8105,
};

    /* MIPC_MSG.SMS_STORE_STATUS_IND */
enum mipc_sms_store_status_ind_tlv_enum {
    mipc_sms_store_status_ind_tlv_NONE = 0,
    /* It indicates that the storage is full or device receives new short message */
    /* type = bitmap, refer to SMS_STORE_FLAG */
    MIPC_SMS_STORE_STATUS_IND_T_FLAG                        = 0x100,
    /* This field may exist when flag indicates the arrival of a new message */
    /* type = uint16_t */
    MIPC_SMS_STORE_STATUS_IND_T_MESSAGE_INDEX               = 0x101,
    /* [3GPP]the storage that new sms on */
    /* type = uint8_t, refer to SMS_STORAGE */
    MIPC_SMS_STORE_STATUS_IND_T_STORAGE                     = 0x102,
};

    /* MIPC_MSG.SMS_NEW_STATUS_REPORT_IND */
enum mipc_sms_new_status_report_ind_tlv_enum {
    mipc_sms_new_status_report_ind_tlv_NONE = 0,
    /* It indicates PDU detailed information */
    /* type = byte_array */
    MIPC_SMS_NEW_STATUS_REPORT_IND_T_PDU                    = 0x8100,
};

    /* MIPC_MSG.SMS_NEW_CBM_IND */
enum mipc_sms_new_cbm_ind_tlv_enum {
    mipc_sms_new_cbm_ind_tlv_NONE = 0,
    /* It indicates the type of CBM message */
    /* type = bitmap, refer to SMS_CBM_TYPE */
    MIPC_SMS_NEW_CBM_IND_T_CBM_TYPE                         = 0x100,
    /* Refer to TS23.041 9.3.24 for more information; it is only valid in CBM_TYPE = ETWS_PRIMARY */
    /* type = uint16_t */
    MIPC_SMS_NEW_CBM_IND_T_WARNING_TYPE                     = 0x101,
    /* Refer to TS23.041 9.4.1.2.2 for more information */
    /* type = uint16_t */
    MIPC_SMS_NEW_CBM_IND_T_MESSAGE_ID                       = 0x102,
    /* Refer to TS23.041 9.4.1.2.1 for more information */
    /* type = uint16_t */
    MIPC_SMS_NEW_CBM_IND_T_SERIAL_NUMBER                    = 0x103,
    /* Refer to TS23.041 9.4.2.2.4 for more information; it is invalid when CBM_TYPE = ETWS_PRIMARY */
    /* type = uint8_t */
    MIPC_SMS_NEW_CBM_IND_T_DCS                              = 0x104,
    /* Refer to TS23.041 9.4.2.2.5 for more information; it is invalid when CBM_TYPE = ETWS_PRIMARY */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_IND_T_DATA                             = 0x8105,
    /* Refer to TS23.041 9.4.1.3.6 for more information; it is only valid in CBM_TYPE = ETWS_PRIMARY */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_IND_T_SECUR_INFO                       = 0x106,
    /* Only pdu_3gpp or pdu_3gpp2 are supported */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_NEW_CBM_IND_T_FORMAT                           = 0x107,
    /* As defined in 3GPP2, refer to CS0015 table 3.4.2.2-1 with 0x01 prefix */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_IND_T_PDU_C2K                          = 0x108,
    /* As defined in 3GPP, refer to +CBM for more information; this field exists when PDU_3GPP_SEG_ENABLE is set previously */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_IND_T_PDU_3GPP_SEG                     = 0x109,
    /* It indicates information of Warning Area Coordinates, refer to ATIS-0700041 */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_IND_T_WAC_INFO                         = 0x10A,
};

    /* MIPC_MSG.SMS_SCBM_IND */
enum mipc_sms_scbm_ind_tlv_enum {
    mipc_sms_scbm_ind_tlv_NONE = 0,
    /* It indicate the SCBM status in device */
    /* type = uint8_t, refer to SMS_SCBM_STATUS */
    MIPC_SMS_SCBM_IND_T_STATUS_UPDATE                       = 0x100,
};

    /* MIPC_MSG.SMS_EXT_INFO_IND */
enum mipc_sms_ext_info_ind_tlv_enum {
    mipc_sms_ext_info_ind_tlv_NONE = 0,
    /* more info to comment, log shows that ims recv with this URC, show a sip:xxx */
    /* type = string */
    MIPC_SMS_EXT_INFO_IND_T_EPSI                            = 0x8100,
    /* Electronic Serial Num. 8 byte */
    /* type = byte_array */
    MIPC_SMS_EXT_INFO_IND_T_ESN_OLD                         = 0x101,
    /* Electronic Serial Num. 8 byte */
    /* type = byte_array */
    MIPC_SMS_EXT_INFO_IND_T_ESN_NEW                         = 0x102,
};

    /* MIPC_MSG.SMS_DUP_NEW_SMS_IND */
enum mipc_sms_dup_new_sms_ind_tlv_enum {
    mipc_sms_dup_new_sms_ind_tlv_NONE = 0,
    /* the format of this sms. PDU_3GPP or PDU_3GPP2 */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_DUP_NEW_SMS_IND_T_FORMAT                       = 0x100,
    /* valid when carry pdu format sms */
    /* type = byte_array */
    MIPC_SMS_DUP_NEW_SMS_IND_T_PDU                          = 0x8101,
};

    /* MIPC_MSG.SMS_CFG_V2_IND */
enum mipc_sms_cfg_v2_ind_tlv_enum {
    mipc_sms_cfg_v2_ind_tlv_NONE = 0,
    /* [3GPP]current setting. Refer SMS:CFG_V2_REQ for more. */
    /* type = uint8_t, refer to SMS_HANDLE_MODE */
    MIPC_SMS_CFG_V2_IND_T_MT_HANDLE_MODE                    = 0x100,
    /* [3GPP]current setting. Refer SMS:CFG_V2_REQ for more. */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_CFG_V2_IND_T_SAVE_STORAGE                      = 0x101,
    /* [3GPP]current setting. Refer SMS:CFG_V2_REQ for more. */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_IND_T_MT_ACK_MODE                       = 0x102,
    /* [3GPP]current setting. Refer SMS:CFG_V2_REQ for more. */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_IND_T_SR_ACK_MODE                       = 0x103,
    /* [3GPP2]current setting. Refer SMS:CFG_V2_REQ for more. */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_IND_T_MT_ACK_MODE_3GPP2                 = 0x104,
    /* [3GPP2]current setting. Refer SMS:CFG_V2_REQ for more. */
    /* type = uint8_t, refer to SMS_ACK_MODE */
    MIPC_SMS_CFG_V2_IND_T_SR_ACK_MODE_3GPP2                 = 0x105,
};

    /* MIPC_MSG.SMS_NEW_SMS_V2_IND */
enum mipc_sms_new_sms_v2_ind_tlv_enum {
    mipc_sms_new_sms_v2_ind_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_SMS_V2_IND_T_STANDARD                      = 0x100,
    /* [3GPP/3GPP2] */
    /* type = byte_array */
    MIPC_SMS_NEW_SMS_V2_IND_T_PDU                           = 0x8101,
    /* [3GPP]the storage that new sms saved */
    /* type = uint8_t, refer to SMS_MD_STORAGE */
    MIPC_SMS_NEW_SMS_V2_IND_T_STORAGE                       = 0x102,
    /* [3GPP]the storage index that new sms saved */
    /* type = uint16_t */
    MIPC_SMS_NEW_SMS_V2_IND_T_MSG_IDX                       = 0x103,
};

    /* MIPC_MSG.SMS_STORE_STATUS_V2_IND */
enum mipc_sms_store_status_v2_ind_tlv_enum {
    mipc_sms_store_status_v2_ind_tlv_NONE = 0,
    /* [3GPP] */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_STORE_STATUS_V2_IND_T_SIM_FULL                 = 0x100,
    /* [3GPP] */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_STORE_STATUS_V2_IND_T_ME_FULL                  = 0x101,
    /* [3GPP2] */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_STORE_STATUS_V2_IND_T_UIM_FULL                 = 0x102,
};

    /* MIPC_MSG.SMS_NEW_STATUS_REPORT_V2_IND */
enum mipc_sms_new_status_report_v2_ind_tlv_enum {
    mipc_sms_new_status_report_v2_ind_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_STATUS_REPORT_V2_IND_T_STANDARD            = 0x100,
    /* [3GPP/3GPP2] */
    /* type = byte_array */
    MIPC_SMS_NEW_STATUS_REPORT_V2_IND_T_PDU                 = 0x8101,
};

    /* MIPC_MSG.SMS_NEW_CBM_V2_IND */
enum mipc_sms_new_cbm_v2_ind_tlv_enum {
    mipc_sms_new_cbm_v2_ind_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_CBM_V2_IND_T_STANDARD                      = 0x100,
    /* [3GPP][ETWS_PRIMARY only]if etws_primary, this tlv must exist. If this tlv exist, means etws_primary. refer TS23.041 9.3.24 for more information */
    /* type = uint16_t */
    MIPC_SMS_NEW_CBM_V2_IND_T_WARNING_TYPE                  = 0x101,
    /* [3GPP/3GPP2]refer CH comment in config request */
    /* type = uint16_t */
    MIPC_SMS_NEW_CBM_V2_IND_T_CH                            = 0x102,
    /* [3GPP/3GPP2]refer LAN comment in config request */
    /* type = uint8_t */
    MIPC_SMS_NEW_CBM_V2_IND_T_LAN                           = 0x103,
    /* [3GPP]refer TS23.041 9.4.2.2.5 for more information, invalid when etws_primary */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_V2_IND_T_DATA                          = 0x8104,
    /* [3GPP][ETWS_PRIMARY only]refer TS23.041 9.4.1.3.6 for more information */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_V2_IND_T_SECUR_INFO                    = 0x105,
    /* [3GPP/3GPP2] 3GPP:refer TS23.041 9.4.1.2.1 for more information; 3GPP2:refer 3GPP2 C.S0015-C 3.4.2.2->3.4.3.7->4.5.1->MESSAGE_ID */
    /* type = uint16_t */
    MIPC_SMS_NEW_CBM_V2_IND_T_SN                            = 0x106,
    /* [3GPP2]refer CS0015 3.4.1 */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_V2_IND_T_PDU_3GPP2                     = 0x107,
    /* [3GPP]refer +CBM for more info, exist when SEG_ENABLE set before */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_V2_IND_T_PDU_SEG                       = 0x108,
    /* [3GPP]Warning Area Coordinates info, refer ATIS-0700041 */
    /* type = byte_array */
    MIPC_SMS_NEW_CBM_V2_IND_T_WAC_INFO                      = 0x109,
};

    /* MIPC_MSG.SMS_DUP_NEW_SMS_V2_IND */
enum mipc_sms_dup_new_sms_v2_ind_tlv_enum {
    mipc_sms_dup_new_sms_v2_ind_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_DUP_NEW_SMS_V2_IND_T_STANDARD                  = 0x100,
    /* [3GPP/3GPP2]3GPP: refer 3GPP TS27.005 4.3 AT+CMGS <pdu>; 3GPP2: refer 3GPP2 C.S0015-C 3.4.1 */
    /* type = byte_array */
    MIPC_SMS_DUP_NEW_SMS_V2_IND_T_PDU                       = 0x8101,
};

    /* MIPC_MSG.SMS_NEW_SMS_CMD */
enum mipc_sms_new_sms_cmd_tlv_enum {
    mipc_sms_new_sms_cmd_tlv_NONE = 0,
    /* It indicates the format of SMS */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_NEW_SMS_CMD_T_FORMAT                           = 0x100,
    /* This value should be 1 */
    /* type = uint16_t */
    MIPC_SMS_NEW_SMS_CMD_T_PDU_COUNT                        = 0x103,
    /* This field is used for PDU_3GPP */
    /* type = struct_array, refer to sms_pdu */
    MIPC_SMS_NEW_SMS_CMD_T_PDU_LIST                         = 0x8104,
    /* [PDU_3GPP2] valid when CDMA */
    /* type = byte_array */
    MIPC_SMS_NEW_SMS_CMD_T_PDU_C2K                          = 0x8105,
};

    /* MIPC_MSG.SMS_NEW_SMS_RSP */
enum mipc_sms_new_sms_rsp_tlv_enum {
    mipc_sms_new_sms_rsp_tlv_NONE = 0,
    /* It indicates ACK for MIPC_SMS_NEW_SMS_CMD; the value should be RP_ACK or RP_ERROR; Default value is RP_ERROR */
    /* type = uint8_t, refer to NEW_SMS_ACK */
    MIPC_SMS_NEW_SMS_RSP_T_NEW_SMS_ACK                      = 0x100,
    /* It indicates cause in TS23.040 9.2.3.22; Default value is 0xff */
    /* type = uint8_t */
    MIPC_SMS_NEW_SMS_RSP_T_CAUSE                            = 0x101,
    /* It indicates the format of SMS */
    /* type = uint8_t, refer to SMS_FORMAT */
    MIPC_SMS_NEW_SMS_RSP_T_FORMAT                           = 0x102,
    /* [PDU_3GPP2] error class of C2K operation, if not carry, would be 0x00; Default value is 0 */
    /* type = uint8_t, refer to SMS_C2K_ERR_CLASS */
    MIPC_SMS_NEW_SMS_RSP_T_ERR_CLASS_C2K                    = 0x103,
    /* [PDU_3GPP2] C2K error code; Default value is 0xffff */
    /* type = uint16_t, refer to SMS_C2K_ERR_CODE */
    MIPC_SMS_NEW_SMS_RSP_T_ERR_CODE_C2K                     = 0x104,
    /* For PDU_3GPP, refer to <ackpdu> in AT+CNMA; if this field exists, CAUSE can be ignored */
    /* type = byte_array */
    MIPC_SMS_NEW_SMS_RSP_T_ACK_PDU                          = 0x8105,
};

    /* MIPC_MSG.SMS_NEW_STATUS_REPORT_CMD */
enum mipc_sms_new_status_report_cmd_tlv_enum {
    mipc_sms_new_status_report_cmd_tlv_NONE = 0,
    /* Detailed information of PDU */
    /* type = byte_array */
    MIPC_SMS_NEW_STATUS_REPORT_CMD_T_PDU                    = 0x8100,
};

    /* MIPC_MSG.SMS_NEW_STATUS_REPORT_RSP */
enum mipc_sms_new_status_report_rsp_tlv_enum {
    mipc_sms_new_status_report_rsp_tlv_NONE = 0,
    /* It indicates ACK for MIPC_SMS_NEW_STATUS_REPORT_CMD, and the value should be RP_ACK or RP_ERROR; Default value is RP_ERROR */
    /* type = uint8_t, refer to NEW_SMS_ACK */
    MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_TYPE               = 0x100,
    /* It indicates cause in TS23.040 9.2.3.22 */
    /* type = uint8_t */
    MIPC_SMS_NEW_STATUS_REPORT_RSP_T_CAUSE                  = 0x101,
    /* For PDU_3GPP, refer to <ackpdu> in AT+CNMA; if this field exists, CAUSE can be ignored */
    /* type = byte_array */
    MIPC_SMS_NEW_STATUS_REPORT_RSP_T_ACK_PDU                = 0x8102,
};

    /* MIPC_MSG.SMS_NEW_SMS_V2_CMD */
enum mipc_sms_new_sms_v2_cmd_tlv_enum {
    mipc_sms_new_sms_v2_cmd_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_SMS_V2_CMD_T_STANDARD                      = 0x100,
    /* [3GPP/3GPP2] */
    /* type = byte_array */
    MIPC_SMS_NEW_SMS_V2_CMD_T_PDU                           = 0x8101,
};

    /* MIPC_MSG.SMS_NEW_SMS_V2_RSP */
enum mipc_sms_new_sms_v2_rsp_tlv_enum {
    mipc_sms_new_sms_v2_rsp_tlv_NONE = 0,
    /* [3GPP/3GPP2]; Default value is 3GPP */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_SMS_V2_RSP_T_STANDARD                      = 0x100,
    /* [3GPP]FALSE->RP_ERROR; TRUE->RP_ACK */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_NEW_SMS_V2_RSP_T_ACK                           = 0x101,
    /* [3GPP] cause in TS23.040 9.2.3.22 */
    /* type = uint8_t */
    MIPC_SMS_NEW_SMS_V2_RSP_T_CAUSE                         = 0x102,
    /* [3GPP] refer to <ackpdu> in AT+CNMA; if carry this, CAUSE would be ignored */
    /* type = byte_array */
    MIPC_SMS_NEW_SMS_V2_RSP_T_PDU                           = 0x8103,
    /* [3GPP2]refer 3GPP2 C.S0015-C 3.4.3.6; Default value is 0 */
    /* type = uint8_t */
    MIPC_SMS_NEW_SMS_V2_RSP_T_ERROR_CLASS_3GPP2             = 0x104,
    /* [3GPP2]refer 3GPP2 C.S0015-C 3.4.3.6; Default value is 0xff */
    /* type = uint8_t */
    MIPC_SMS_NEW_SMS_V2_RSP_T_CAUSE_CODE_3GPP2              = 0x105,
};

    /* MIPC_MSG.SMS_NEW_STATUS_REPORT_V2_CMD */
enum mipc_sms_new_status_report_v2_cmd_tlv_enum {
    mipc_sms_new_status_report_v2_cmd_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_STATUS_REPORT_V2_CMD_T_STANDARD            = 0x100,
    /* [3GPP/3GPP2] */
    /* type = byte_array */
    MIPC_SMS_NEW_STATUS_REPORT_V2_CMD_T_PDU                 = 0x8101,
};

    /* MIPC_MSG.SMS_NEW_STATUS_REPORT_V2_RSP */
enum mipc_sms_new_status_report_v2_rsp_tlv_enum {
    mipc_sms_new_status_report_v2_rsp_tlv_NONE = 0,
    /* [3GPP/3GPP2] */
    /* type = uint8_t, refer to SMS_STANDARD */
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_STANDARD            = 0x100,
    /* [3GPP]FALSE->RP_ERROR; TRUE->RP_ACK */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_ACK                 = 0x101,
    /* [3GPP] cause in TS23.040 9.2.3.22 */
    /* type = uint8_t */
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_CAUSE               = 0x102,
    /* [3GPP] refer to <ackpdu> in AT+CNMA; if carry this, CAUSE would be ignored */
    /* type = byte_array */
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_PDU                 = 0x8103,
    /* [3GPP2]refer 3GPP2 C.S0015-C 3.4.3.6 */
    /* type = uint8_t */
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_ERROR_CLASS_3GPP2   = 0x104,
    /* [3GPP2]refer 3GPP2 C.S0015-C 3.4.3.6 */
    /* type = uint8_t */
    MIPC_SMS_NEW_STATUS_REPORT_V2_RSP_T_CAUSE_CODE_3GPP2    = 0x105,
};




#endif /* __MIPC_MSG_SMS_CONST_H__ */
