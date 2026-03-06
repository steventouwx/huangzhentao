#ifndef __MIPC_MSG_APN_CONST_H__
#define __MIPC_MSG_APN_CONST_H__

enum MIPC_APN_MSG_enum {
    MIPC_APN_MSG_NONE = 0,
    /*
      The control point sets the initial attach (IA) APN profile for LTE. This setting is persistent across reboot. The control point should send it at modem's first bootup or when SIM card changed. Modem will attempt to attach with it when attach condition is met. A success in MIPC_APN_SET_IA_CNF means that modem has stored this IA configuration persistently but does not mean the network has successfully used this IA APN to attach. The control points learn about the actual attach status and the IA APN used through MIPC_NW_IA_IND.
      If the control point does not want to provide an IA APN profile and wishes to let modem decide the IA APN profile, it should send MIPC_APN_SET_IA_MD_PREFER_REQ. MIPC_APN_SET_IA and MIPC_APN_SET_IA_MD_PREFER are mutually exclusive. If the control point sends both messages, modem might initiate re-attach based on the latest sent request for IA APN profile. If control point does not send either message and modem does not have any default or cached IA APN that can be used, modem does not initiate attach.
    */
    MIPC_APN_SET_IA_REQ                                     = 257,
    MIPC_APN_SET_IA_CNF                                     = 258,

    /* This command is used to acquire initial attach APN profile of LTE. */
    MIPC_APN_GET_IA_REQ                                     = 259,
    MIPC_APN_GET_IA_CNF                                     = 260,

    /* This command is used to add an APN profile for PDN/PDP/PDU activation. */
    MIPC_APN_ADD_PROFILE_REQ                                = 261,
    MIPC_APN_ADD_PROFILE_CNF                                = 262,

    /* This command is used to list current APN profiles set by host. */
    MIPC_APN_LIST_PROFILE_REQ                               = 263,
    MIPC_APN_LIST_PROFILE_CNF                               = 264,

    /* This command is used to delete an APN profile. */
    MIPC_APN_DEL_PROFILE_REQ                                = 265,
    MIPC_APN_DEL_PROFILE_CNF                                = 266,

    /* This command indicates whether host will set APN profile or not. */
    MIPC_APN_SET_PROFILE_STATUS_REQ                         = 267,
    MIPC_APN_SET_PROFILE_STATUS_CNF                         = 268,

    /* This command is used to list modem APN profiles and state. */
    MIPC_APN_LIST_MD_PROFILE_REQ                            = 269,
    MIPC_APN_LIST_MD_PROFILE_CNF                            = 270,

    /* This command is adopted to set OP12 specific APN for specific customer requirement. */
    MIPC_APN_SET_OP12_APN_REQ                               = 271,
    MIPC_APN_SET_VZW_APN_REQ                                = 271,
    MIPC_APN_SET_OP12_APN_CNF                               = 272,
    MIPC_APN_SET_VZW_APN_CNF                                = 272,

    /* This command is adopted to set OP12 specific APN timer for specific customer requirement. */
    MIPC_APN_SET_OP12_APN_TIMER_REQ                         = 273,
    MIPC_APN_SET_VZW_APN_TIMER_REQ                          = 273,
    MIPC_APN_SET_OP12_APN_TIMER_CNF                         = 274,
    MIPC_APN_SET_VZW_APN_TIMER_CNF                          = 274,

    /* Add a profile list for modem to completely replace old profile list with new profile list. */
    MIPC_APN_ADD_PROFILE_LIST_REQ                           = 275,
    MIPC_APN_ADD_PROFILE_LIST_CNF                           = 276,

    /* Modem preferies to use preset initial attach APN for LTE. This command indicates whether modem will select preset initial attach APN for LTE or not. */
    MIPC_APN_SET_IA_MD_PREFER_REQ                           = 277,
    MIPC_APN_SET_IA_MD_PREFER_CNF                           = 278,

    /* This command indicates the change of initial attach APN profile of LTE. */
    MIPC_APN_IA_CFG_IND                                     = 16641,

    /* This command indicates the change of modem APN profile. */
    MIPC_APN_PROFILE_CFG_IND                                = 16642,

    /* OP12 APN profile changed indication */
    MIPC_APN_OP12_CHG_IND                                   = 16643,
    MIPC_APN_VZW_CHG_IND                                    = 16643,


};

    /* MIPC_MSG.APN_SET_IA_REQ */
enum mipc_apn_set_ia_req_tlv_enum {
    mipc_apn_set_ia_req_tlv_NONE = 0,
    /* Access Point Name */
    /* type = string */
    MIPC_APN_SET_IA_REQ_T_APN                               = 0x101,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_APN_SET_IA_REQ_T_PDP_TYPE                          = 0x102,
    /* The IP address type for roaming */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_APN_SET_IA_REQ_T_ROAMING_TYPE                      = 0x103,
    /* Authentication type for the data call */
    /* type = uint8_t, refer to APN_AUTH_TYPE */
    MIPC_APN_SET_IA_REQ_T_AUTH_TYPE                         = 0x104,
    /* Authentication username */
    /* type = string */
    MIPC_APN_SET_IA_REQ_T_USERID                            = 0x8105,
    /* Authentication password */
    /* type = string */
    MIPC_APN_SET_IA_REQ_T_PASSWORD                          = 0x8106,
    /*
      RAT bitmask:
      Unknown, when no bit is set
      bit14 : LTE
      bit19 : LTE_CA
      bit20 : NR
      Default value is 0xFFFDFFFF, means to enable all excluding WIFI
    */
    /* type = uint32_t */
    MIPC_APN_SET_IA_REQ_T_BEARER_BITMASK                    = 0x107,
    /* Compression configuration */
    /* type = uint8_t, refer to APN_COMPRESSION */
    MIPC_APN_SET_IA_REQ_T_COMPRESSION                       = 0x108,
    /* The change reason of initial attach. */
    /* type = uint8_t, refer to APN_CHANGE_REASON */
    MIPC_APN_SET_IA_REQ_T_CHANGE_REASON                     = 0x109,
};

    /* MIPC_MSG.APN_SET_IA_CNF */
enum mipc_apn_set_ia_cnf_tlv_enum {
    mipc_apn_set_ia_cnf_tlv_NONE = 0,
    /* The number of IA APN */
    /* type = uint8_t */
    MIPC_APN_SET_IA_CNF_T_IA_COUNT                          = 0x100,
    /* The list of initial attach APN profile */
    /* type = struct_array, refer to apn_ia */
    MIPC_APN_SET_IA_CNF_T_IA_LIST                           = 0x8101,
    /* The list of initial attach APN profile */
    /* type = struct, refer to apn_ia */
    MIPC_APN_SET_IA_CNF_T_IA_TLV_ARRAY                      = 0x8102,
};

    /* MIPC_MSG.APN_GET_IA_REQ */
enum mipc_apn_get_ia_req_tlv_enum {
    mipc_apn_get_ia_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_GET_IA_CNF */
enum mipc_apn_get_ia_cnf_tlv_enum {
    mipc_apn_get_ia_cnf_tlv_NONE = 0,
    /* The number of IA APN */
    /* type = uint8_t */
    MIPC_APN_GET_IA_CNF_T_IA_COUNT                          = 0x100,
    /* The list of initial attach APN profile */
    /* type = struct_array, refer to apn_ia */
    MIPC_APN_GET_IA_CNF_T_IA_LIST                           = 0x8101,
    /* The list of initial attach APN profile */
    /* type = struct, refer to apn_ia */
    MIPC_APN_GET_IA_CNF_T_IA_TLV_ARRAY                      = 0x8102,
};

    /* MIPC_MSG.APN_ADD_PROFILE_REQ */
enum mipc_apn_add_profile_req_tlv_enum {
    mipc_apn_add_profile_req_tlv_NONE = 0,
    /* The ID of APN profile */
    /* type = uint32_t */
    MIPC_APN_ADD_PROFILE_REQ_T_ID                           = 0x101,
    /* PLMN of operator network; Default value is 0x7f */
    /* type = string */
    MIPC_APN_ADD_PROFILE_REQ_T_PLMN_ID                      = 0x102,
    /* Access Point Name */
    /* type = string */
    MIPC_APN_ADD_PROFILE_REQ_T_APN                          = 0x103,
    /* The data connection type for APN */
    /* type = uint32_t, refer to APN_TYPE */
    MIPC_APN_ADD_PROFILE_REQ_T_APN_TYPE                     = 0x104,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_APN_ADD_PROFILE_REQ_T_PDP_TYPE                     = 0x105,
    /* The IP address type for roaming */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_APN_ADD_PROFILE_REQ_T_ROAMING_TYPE                 = 0x106,
    /* Authentication type for the data call */
    /* type = uint8_t, refer to APN_AUTH_TYPE */
    MIPC_APN_ADD_PROFILE_REQ_T_AUTH_TYPE                    = 0x107,
    /* Authentication username */
    /* type = string */
    MIPC_APN_ADD_PROFILE_REQ_T_USERID                       = 0x8108,
    /* Authentication password */
    /* type = string */
    MIPC_APN_ADD_PROFILE_REQ_T_PASSWORD                     = 0x8109,
    /*
      RAT bitmask:
      Unknown, when no bit is set
      bit14 : LTE
      bit19 : LTE_CA
      bit20 : NR
      Default value is 0xFFFDFFFF, means to enable all excluding WIFI
    */
    /* type = uint32_t */
    MIPC_APN_ADD_PROFILE_REQ_T_BEARER_BITMASK               = 0x10A,
    /* Compression */
    /* type = uint8_t, refer to APN_COMPRESSION */
    MIPC_APN_ADD_PROFILE_REQ_T_COMPRESSION                  = 0x10B,
    /* It indicates whether APN profile is enabled or not; MAX is not set */
    /* type = uint8_t, refer to APN_ENABLED_TYPE */
    MIPC_APN_ADD_PROFILE_REQ_T_ENABLED                      = 0x10C,
};

    /* MIPC_MSG.APN_ADD_PROFILE_CNF */
enum mipc_apn_add_profile_cnf_tlv_enum {
    mipc_apn_add_profile_cnf_tlv_NONE = 0,
    /* Number of APN profile in list */
    /* type = uint8_t */
    MIPC_APN_ADD_PROFILE_CNF_T_APN_COUNT                    = 0x100,
    /* The list of APN profiles */
    /* type = struct_array, refer to apn_profile */
    MIPC_APN_ADD_PROFILE_CNF_T_APN_LIST                     = 0x8101,
    /* The list of APN profiles */
    /* type = struct, refer to apn_profile */
    MIPC_APN_ADD_PROFILE_CNF_T_APN_TLV_ARRAY                = 0x8102,
};

    /* MIPC_MSG.APN_LIST_PROFILE_REQ */
enum mipc_apn_list_profile_req_tlv_enum {
    mipc_apn_list_profile_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_LIST_PROFILE_CNF */
enum mipc_apn_list_profile_cnf_tlv_enum {
    mipc_apn_list_profile_cnf_tlv_NONE = 0,
    /* Number of APN profile in list */
    /* type = uint8_t */
    MIPC_APN_LIST_PROFILE_CNF_T_APN_COUNT                   = 0x100,
    /* The list of APN profiles */
    /* type = struct_array, refer to apn_profile */
    MIPC_APN_LIST_PROFILE_CNF_T_APN_LIST                    = 0x8101,
    /* The list of APN profiles */
    /* type = struct, refer to apn_profile */
    MIPC_APN_LIST_PROFILE_CNF_T_APN_TLV_ARRAY               = 0x8102,
};

    /* MIPC_MSG.APN_DEL_PROFILE_REQ */
enum mipc_apn_del_profile_req_tlv_enum {
    mipc_apn_del_profile_req_tlv_NONE = 0,
    /* Profile ID */
    /* type = uint32_t */
    MIPC_APN_DEL_PROFILE_REQ_T_ID                           = 0x101,
};

    /* MIPC_MSG.APN_DEL_PROFILE_CNF */
enum mipc_apn_del_profile_cnf_tlv_enum {
    mipc_apn_del_profile_cnf_tlv_NONE = 0,
    /* Profile number in list */
    /* type = uint8_t */
    MIPC_APN_DEL_PROFILE_CNF_T_APN_COUNT                    = 0x100,
    /* The list of APN profiles */
    /* type = struct_array, refer to apn_profile */
    MIPC_APN_DEL_PROFILE_CNF_T_APN_LIST                     = 0x8101,
    /* The list of APN profiles */
    /* type = struct, refer to apn_profile */
    MIPC_APN_DEL_PROFILE_CNF_T_APN_TLV_ARRAY                = 0x8102,
};

    /* MIPC_MSG.APN_SET_PROFILE_STATUS_REQ */
enum mipc_apn_set_profile_status_req_tlv_enum {
    mipc_apn_set_profile_status_req_tlv_NONE = 0,
    /* It indicates whether host will set APN profile or not */
    /* type = uint8_t, refer to APN_PROFILE_MODE */
    MIPC_APN_SET_PROFILE_STATUS_REQ_T_MODE                  = 0x101,
};

    /* MIPC_MSG.APN_SET_PROFILE_STATUS_CNF */
enum mipc_apn_set_profile_status_cnf_tlv_enum {
    mipc_apn_set_profile_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_LIST_MD_PROFILE_REQ */
enum mipc_apn_list_md_profile_req_tlv_enum {
    mipc_apn_list_md_profile_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_LIST_MD_PROFILE_CNF */
enum mipc_apn_list_md_profile_cnf_tlv_enum {
    mipc_apn_list_md_profile_cnf_tlv_NONE = 0,
    /* Number of APN profile in list */
    /* type = uint8_t */
    MIPC_APN_LIST_MD_PROFILE_CNF_T_APN_COUNT                = 0x100,
    /* The list of modem APN profiles */
    /* type = struct_array, refer to md_apn_profile */
    MIPC_APN_LIST_MD_PROFILE_CNF_T_APN_LIST                 = 0x8101,
    /* The list of APN profiles */
    /* type = struct, refer to md_apn_profile */
    MIPC_APN_LIST_MD_PROFILE_CNF_T_APN_TLV_ARRAY            = 0x8102,
};

    /* MIPC_MSG.APN_SET_OP12_APN_REQ */
enum mipc_apn_set_op12_apn_req_tlv_enum {
    mipc_apn_set_op12_apn_req_tlv_NONE = 0,
    /* It indicates the APN identity to be edited */
    /* type = uint8_t */
    MIPC_APN_SET_OP12_APN_REQ_T_APN_ID                      = 0x100,
    /* It indicates the APN class */
    /* type = uint8_t */
    MIPC_APN_SET_OP12_APN_REQ_T_CLASS                       = 0x101,
    /* The network identifier, such as IMS, VZWADMIN, etc. */
    /* type = string */
    MIPC_APN_SET_OP12_APN_REQ_T_NETWORK_IDENTIFIER          = 0x102,
    /* PDP type, such as IP, IPv6, IPv4v6 */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_APN_SET_OP12_APN_REQ_T_PDP_TYPE                    = 0x103,
    /* APN bearer, such as LTE */
    /* type = string */
    MIPC_APN_SET_OP12_APN_REQ_T_APN_BEARER                  = 0x104,
    /* It indicates whether APN profile is enabled or not; MAX is not set */
    /* type = uint8_t, refer to APN_ENABLED_TYPE */
    MIPC_APN_SET_OP12_APN_REQ_T_ENABLED                     = 0x105,
    /* APN inactivity timer */
    /* type = uint32_t */
    MIPC_APN_SET_OP12_APN_REQ_T_APN_TIMER                   = 0x106,
};

enum mipc_apn_set_vzw_apn_req_tlv_enum {
    mipc_apn_set_vzw_apn_req_tlv_NONE = 0,
    /* It indicates the APN identity to be edited */
    /* type = uint8_t */
    MIPC_APN_SET_VZW_APN_REQ_T_APN_ID                       = 0x100,
    /* It indicates the APN class */
    /* type = uint8_t */
    MIPC_APN_SET_VZW_APN_REQ_T_CLASS                        = 0x101,
    /* The network identifier, such as IMS, VZWADMIN, etc. */
    /* type = string */
    MIPC_APN_SET_VZW_APN_REQ_T_NETWORK_IDENTIFIER           = 0x102,
    /* PDP type, such as IP, IPv6, IPv4v6 */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_APN_SET_VZW_APN_REQ_T_PDP_TYPE                     = 0x103,
    /* APN bearer, such as LTE */
    /* type = string */
    MIPC_APN_SET_VZW_APN_REQ_T_APN_BEARER                   = 0x104,
    /* It indicates whether APN profile is enabled or not; MAX is not set */
    /* type = uint8_t, refer to APN_ENABLED_TYPE */
    MIPC_APN_SET_VZW_APN_REQ_T_ENABLED                      = 0x105,
    /* APN inactivity timer */
    /* type = uint32_t */
    MIPC_APN_SET_VZW_APN_REQ_T_APN_TIMER                    = 0x106,
};

    /* MIPC_MSG.APN_SET_OP12_APN_CNF */
enum mipc_apn_set_op12_apn_cnf_tlv_enum {
    mipc_apn_set_op12_apn_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

enum mipc_apn_set_vzw_apn_cnf_tlv_enum {
    mipc_apn_set_vzw_apn_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_SET_OP12_APN_TIMER_REQ */
enum mipc_apn_set_op12_apn_timer_req_tlv_enum {
    mipc_apn_set_op12_apn_timer_req_tlv_NONE = 0,
    /* It indicates the APN identity to be edited */
    /* type = uint8_t */
    MIPC_APN_SET_OP12_APN_TIMER_REQ_T_APN_ID                = 0x100,
    /* [phase out] Max connection count, replace with EXT_MAX_CONN */
    /* type = uint8_t */
    MIPC_APN_SET_OP12_APN_TIMER_REQ_T_MAX_CONN              = 0x101,
    /* [phase out] Max connection, replace with EXT_MAX_CONN_T */
    /* type = uint8_t */
    MIPC_APN_SET_OP12_APN_TIMER_REQ_T_MAX_CONN_T            = 0x102,
    /* Wait time by seconds */
    /* type = uint32_t */
    MIPC_APN_SET_OP12_APN_TIMER_REQ_T_WAIT_TIME             = 0x103,
    /* Throttle timer by seconds */
    /* type = uint32_t */
    MIPC_APN_SET_OP12_APN_TIMER_REQ_T_THROTTLE_TIME         = 0x104,
};

enum mipc_apn_set_vzw_apn_timer_req_tlv_enum {
    mipc_apn_set_vzw_apn_timer_req_tlv_NONE = 0,
    /* It indicates the APN identity to be edited */
    /* type = uint8_t */
    MIPC_APN_SET_VZW_APN_TIMER_REQ_T_APN_ID                 = 0x100,
    /* [phase out] Max connection count, replace with EXT_MAX_CONN */
    /* type = uint8_t */
    MIPC_APN_SET_VZW_APN_TIMER_REQ_T_MAX_CONN               = 0x101,
    /* [phase out] Max connection, replace with EXT_MAX_CONN_T */
    /* type = uint8_t */
    MIPC_APN_SET_VZW_APN_TIMER_REQ_T_MAX_CONN_T             = 0x102,
    /* Wait time by seconds */
    /* type = uint32_t */
    MIPC_APN_SET_VZW_APN_TIMER_REQ_T_WAIT_TIME              = 0x103,
    /* Throttle timer by seconds */
    /* type = uint32_t */
    MIPC_APN_SET_VZW_APN_TIMER_REQ_T_THROTTLE_TIME          = 0x104,
};

    /* MIPC_MSG.APN_SET_OP12_APN_TIMER_CNF */
enum mipc_apn_set_op12_apn_timer_cnf_tlv_enum {
    mipc_apn_set_op12_apn_timer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

enum mipc_apn_set_vzw_apn_timer_cnf_tlv_enum {
    mipc_apn_set_vzw_apn_timer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_ADD_PROFILE_LIST_REQ */
enum mipc_apn_add_profile_list_req_tlv_enum {
    mipc_apn_add_profile_list_req_tlv_NONE = 0,
    /* It indicates the number of profiles included in the list; profile count should be less than 10 */
    /* type = uint8_t */
    MIPC_APN_ADD_PROFILE_LIST_REQ_T_PROFILE_COUNT           = 0x100,
    /* The profile list to be added */
    /* type = struct, refer to apn_profile_v2 */
    MIPC_APN_ADD_PROFILE_LIST_REQ_T_PROFILE_LIST            = 0x8101,
    MIPC_APN_ADD_PROFILE_LIST_REQ_T_PROFILE_TLV_ARRAY       = 0x8101,
};

    /* MIPC_MSG.APN_ADD_PROFILE_LIST_CNF */
enum mipc_apn_add_profile_list_cnf_tlv_enum {
    mipc_apn_add_profile_list_cnf_tlv_NONE = 0,
    /* It indicates the number of profiles included in the list */
    /* type = uint8_t */
    MIPC_APN_ADD_PROFILE_LIST_CNF_T_PROFILE_COUNT           = 0x100,
    /* The profile list to be added */
    /* type = struct, refer to apn_profile_v2 */
    MIPC_APN_ADD_PROFILE_LIST_CNF_T_PROFILE_LIST            = 0x8101,
    MIPC_APN_ADD_PROFILE_LIST_CNF_T_PROFILE_TLV_ARRAY       = 0x8101,
};

    /* MIPC_MSG.APN_SET_IA_MD_PREFER_REQ */
enum mipc_apn_set_ia_md_prefer_req_tlv_enum {
    mipc_apn_set_ia_md_prefer_req_tlv_NONE = 0,
    /* It indicates whether modem will select preset initial attach APN for LTE or not */
    /* type = uint8_t, refer to IA_MD_PREFER_TYPE */
    MIPC_APN_SET_IA_MD_PREFER_REQ_T_IA_MD_PREFER            = 0x100,
};

    /* MIPC_MSG.APN_SET_IA_MD_PREFER_CNF */
enum mipc_apn_set_ia_md_prefer_cnf_tlv_enum {
    mipc_apn_set_ia_md_prefer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_IA_CFG_IND */
enum mipc_apn_ia_cfg_ind_tlv_enum {
    mipc_apn_ia_cfg_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_PROFILE_CFG_IND */
enum mipc_apn_profile_cfg_ind_tlv_enum {
    mipc_apn_profile_cfg_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.APN_OP12_CHG_IND */
enum mipc_apn_op12_chg_ind_tlv_enum {
    mipc_apn_op12_chg_ind_tlv_NONE = 0,
    /* apn profile count */
    /* type = uint8_t */
    MIPC_APN_OP12_CHG_IND_T_APN_COUNT                       = 0x100,
    /* [phase out]the list of APN profile, replace with EXT_APN_LIST */
    /* type = struct_array, refer to op12_apn_profile_legacy */
    MIPC_APN_OP12_CHG_IND_T_APN_LIST                        = 0x101,
    /* the list of APN profile */
    /* type = struct, refer to op12_apn_profile */
    MIPC_APN_OP12_CHG_IND_T_EXT_APN_LIST                    = 0x102,
    MIPC_APN_OP12_CHG_IND_T_APN_TLV_ARRAY                   = 0x102,
};

enum mipc_apn_vzw_chg_ind_tlv_enum {
    mipc_apn_vzw_chg_ind_tlv_NONE = 0,
    /* apn profile count */
    /* type = uint8_t */
    MIPC_APN_VZW_CHG_IND_T_APN_COUNT                        = 0x100,
    /* [phase out]the list of APN profile, replace with EXT_APN_LIST */
    /* type = struct_array, refer to op12_apn_profile_legacy */
    MIPC_APN_VZW_CHG_IND_T_APN_LIST                         = 0x101,
    /* the list of APN profile */
    /* type = struct, refer to op12_apn_profile */
    MIPC_APN_VZW_CHG_IND_T_EXT_APN_LIST                     = 0x102,
    MIPC_APN_VZW_CHG_IND_T_APN_TLV_ARRAY                    = 0x102,
};




#endif /* __MIPC_MSG_APN_CONST_H__ */
