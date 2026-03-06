#ifndef __MIPC_MSG_CALL_CONST_H__
#define __MIPC_MSG_CALL_CONST_H__

enum MIPC_CALL_MSG_enum {
    MIPC_CALL_MSG_NONE = 0,
    /* this command is used to dial a call */
    MIPC_CALL_DIAL_REQ                                      = 2305,
    MIPC_CALL_DIAL_CNF                                      = 2306,

    /* requests call-related supplementary services */
    MIPC_CALL_SS_REQ                                        = 2307,
    MIPC_CALL_SS_CNF                                        = 2308,

    /* this command is used to release call */
    MIPC_CALL_HANGUP_REQ                                    = 2309,
    MIPC_CALL_HANGUP_CNF                                    = 2310,

    /* answers and initiates a connection to an incoming call */
    MIPC_CALL_ANSWER_REQ                                    = 2311,
    MIPC_CALL_ANSWER_CNF                                    = 2312,

    /* get the call status */
    MIPC_CALL_GET_CALL_STATUS_REQ                           = 2313,
    MIPC_CALL_GET_CALL_STATUS_CNF                           = 2314,

    /* conference call operation */
    MIPC_CALL_CONFERENCE_REQ                                = 2315,
    MIPC_CALL_CONFERENCE_CNF                                = 2316,

    /* get the ims conference call information, get cs conference information please use GET_CALL_STATUS_REQ */
    MIPC_CALL_GET_CONFERENCE_INFO_REQ                       = 2317,
    MIPC_CALL_GET_CONFERENCE_INFO_CNF                       = 2318,

    /* get the reason for : (1) the failure in the last unsuccessful call setup or in-call modification  (2) the last call release */
    MIPC_CALL_GET_FINISH_REASON_REQ                         = 2319,
    MIPC_CALL_GET_FINISH_REASON_CNF                         = 2320,

    /* this command is intended to be send START_DTMF and STOP_DTMF message to NW separately */
    MIPC_CALL_DTMF_REQ                                      = 2321,
    MIPC_CALL_DTMF_CNF                                      = 2322,

    /* get the ECC list */
    MIPC_CALL_GET_ECC_LIST_REQ                              = 2323,
    MIPC_CALL_GET_ECC_LIST_CNF                              = 2324,

    /* set the ECC list to modem */
    MIPC_CALL_SET_ECC_LIST_REQ                              = 2325,
    MIPC_CALL_SET_ECC_LIST_CNF                              = 2326,

    /* set the flight mode ecc session */
    MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ               = 2327,
    MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_CNF               = 2328,

    /* request to exit ecbm mode */
    MIPC_CALL_SET_EXIT_ECBM_MODE_REQ                        = 2329,
    MIPC_CALL_SET_EXIT_ECBM_MODE_CNF                        = 2330,

    /* trigger an ecall. (for Telematics product) */
    MIPC_CALL_IVS_ONEKEY_ECALL_REQ                          = 2331,
    MIPC_CALL_IVS_ONEKEY_ECALL_CNF                          = 2332,

    /* add user-defined SIP headers */
    MIPC_CALL_SET_SIP_HEADER_REQ                            = 2333,
    MIPC_CALL_SET_SIP_HEADER_CNF                            = 2334,

    /* Query IMS SIP Header by Call Id and the result will be sent via MIPC_CALL_IMS_HEADER_IND */
    MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ              = 2335,
    MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_CNF              = 2336,

    /* Set IMS call additional information */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ                  = 2337,
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_CNF                  = 2338,

    /* accept/reject peer side RTT call modify request */
    MIPC_CALL_PEER_RTT_MODIFY_REQ                           = 2339,
    MIPC_CALL_PEER_RTT_MODIFY_CNF                           = 2340,

    /* IMS RTT call upgrade/downgrade */
    MIPC_CALL_LOCAL_RTT_MODIFY_REQ                          = 2341,
    MIPC_CALL_LOCAL_RTT_MODIFY_CNF                          = 2342,

    /* send Rtt Text */
    MIPC_CALL_RTT_TEXT_REQ                                  = 2343,
    MIPC_CALL_RTT_TEXT_CNF                                  = 2344,

    /* enable/disable IMS RTT capability */
    MIPC_CALL_RTT_MODE_REQ                                  = 2345,
    MIPC_CALL_RTT_MODE_CNF                                  = 2346,

    /* toggle Rtt Audio Indication */
    MIPC_CALL_RTT_AUDIO_REQ                                 = 2347,
    MIPC_CALL_RTT_AUDIO_CNF                                 = 2348,

    /* inform IMS Volte, about which RCS feature tags are need to be added in REGISTER request */
    MIPC_CALL_SET_RCS_STATE_AND_FEATURE_REQ                 = 2349,
    MIPC_CALL_SET_RCS_STATE_AND_FEATURE_CNF                 = 2350,

    /* used to update RCS status to modem module. */
    MIPC_CALL_UPDATE_RCS_SESSION_INFO_REQ                   = 2351,
    MIPC_CALL_UPDATE_RCS_SESSION_INFO_CNF                   = 2352,

    /* get the voice domain preference of the UE. */
    MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_REQ               = 2357,
    MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF               = 2358,

    /* pull call */
    MIPC_CALL_PULL_REQ                                      = 2359,
    MIPC_CALL_PULL_CNF                                      = 2360,

    /* get TTY mode */
    MIPC_CALL_GET_TTY_MODE_REQ                              = 2361,
    MIPC_CALL_GET_TTY_MODE_CNF                              = 2362,

    /* set IMS call mode to difficiate client API mode and original IMS mode */
    MIPC_CALL_SET_IMS_CALL_MODE_REQ                         = 2363,
    MIPC_CALL_SET_IMS_CALL_MODE_CNF                         = 2364,

    /* set TTY mode */
    MIPC_CALL_SET_TTY_MODE_REQ                              = 2365,
    MIPC_CALL_SET_TTY_MODE_CNF                              = 2366,

    /* set the MT to operate according to the specified voice domain preference for E-UTRAN */
    MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ               = 2367,
    MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_CNF               = 2368,

    /* this command is used to dial a conference call */
    MIPC_CALL_CONFERENCE_DIAL_REQ                           = 2369,
    MIPC_CALL_CONFERENCE_DIAL_CNF                           = 2370,

    /* this command is used to set GWSD mode */
    MIPC_CALL_SET_GWSD_MODE_REQ                             = 2371,
    MIPC_CALL_SET_GWSD_MODE_CNF                             = 2372,

    /* this command is used to set GWSD mode */
    MIPC_CALL_APPROVE_CSFB_REQ                              = 2373,
    MIPC_CALL_APPROVE_CSFB_CNF                              = 2374,

    /* this command is used to set GWSD call valid timer */
    MIPC_CALL_SET_GWSD_CALL_VALID_REQ                       = 2375,
    MIPC_CALL_SET_GWSD_CALL_VALID_CNF                       = 2376,

    /* this command is used to set GWSD ignore call interval */
    MIPC_CALL_SET_GWSD_IGNORE_CALL_INTERVAL_REQ             = 2377,
    MIPC_CALL_SET_GWSD_IGNORE_CALL_INTERVAL_CNF             = 2378,

    /* this command is used to set GWSD keep alive PDCP */
    MIPC_CALL_SET_GWSD_KA_PDCP_REQ                          = 2379,
    MIPC_CALL_SET_GWSD_KA_PDCP_CNF                          = 2380,

    /* this command is used to set GWSD keep alive IPDATA */
    MIPC_CALL_SET_GWSD_KA_IPDATA_REQ                        = 2381,
    MIPC_CALL_SET_GWSD_KA_IPDATA_CNF                        = 2382,

    /* This command is used to provide User Interactive Service information accompanying with a specific ongoing Call */
    MIPC_CALL_UIS_INFO_REQ                                  = 2387,
    MIPC_CALL_UIS_INFO_CNF                                  = 2388,

    /* This command is used to enable auto answer, default value is 1 */
    MIPC_CALL_AUTO_ANSWER_REQ                               = 2389,
    MIPC_CALL_AUTO_ANSWER_CNF                               = 2390,

    /* This command is used to set call/data prefer setting */
    MIPC_CALL_DATA_PREFER_SET_REQ                           = 2391,
    MIPC_CALL_DATA_PREFER_SET_CNF                           = 2392,

    /* request to approve this call or not */
    MIPC_CALL_ECC_REDIAL_APPROVE_REQ                        = 2395,
    MIPC_CALL_ECC_REDIAL_APPROVE_CNF                        = 2396,

    /* request for SIP/SDP negotiation for a new dcmap stream(usually for application DC) after call has been established */
    MIPC_CALL_DC_CREATE_SIP_REQ                             = 2397,
    MIPC_CALL_DC_CREATE_SIP_CNF                             = 2398,

    /* request for SIP/SDP negotiation for an existing dcmap stream */
    MIPC_CALL_DC_MODIFY_SIP_REQ                             = 2399,
    MIPC_CALL_DC_MODIFY_SIP_CNF                             = 2400,

    /* request for removing one or all DC stream(s) in following SIP/SDP negotiation due to DC manager has just closed one or more DC stream(s) */
    MIPC_CALL_DC_CLOSE_SIP_REQ                              = 2401,
    MIPC_CALL_DC_CLOSE_SIP_CNF                              = 2402,

    /* AP request to create new DataChannel(s) */
    MIPC_CALL_CREATE_DATA_CHANNEL_REQ                       = 2415,
    MIPC_CALL_CREATE_DATA_CHANNEL_CNF                       = 2416,

    /* AP request to close existing DataChannel(s) */
    MIPC_CALL_CLOSE_DATA_CHANNEL_REQ                        = 2417,
    MIPC_CALL_CLOSE_DATA_CHANNEL_CNF                        = 2418,

    /* call status changed indication */
    MIPC_CALL_STATUS_IND                                    = 18689,

    /* call event indication */
    MIPC_CALL_EVENT_IND                                     = 18690,

    /* this message is used to inform host that the call mode change */
    MIPC_CALL_MODE_IND                                      = 18691,

    /* this message is used to inofrm host response code from NW which UA received */
    MIPC_CALL_SIP_IND                                       = 18692,

    /* this message is used to indicate host to XML event package */
    MIPC_CALL_CONFERENCE_IND                                = 18693,

    /* this message is used to indicate host to XML event package */
    MIPC_CALL_IMS_EVENT_PACKAGE_IND                         = 18694,

    /* ecc list changed indication */
    MIPC_CALL_ECC_LIST_CHANGE_IND                           = 18695,

    /* supplementary service indication */
    MIPC_CALL_SS_IND                                        = 18696,

    /* this message is used to indicate ecc call back mode change */
    MIPC_CALL_ECBM_CHANGE_IND                               = 18697,

    /* this message is used to indicate CRSS  */
    MIPC_CALL_CRSS_IND                                      = 18698,

    /* report ect result */
    MIPC_CALL_ECT_IND                                       = 18699,

    /* report ciphering Indicator information */
    MIPC_CALL_CIPHER_IND                                    = 18700,

    /* Rtt Audio Indication */
    MIPC_CALL_RTT_AUDIO_IND                                 = 18701,

    /* inform AP IMS call text capability */
    MIPC_CALL_RTT_CAPABILITY_IND                            = 18702,

    /* phase out, move the parameter to LOCAL_RTT_MODIFY_CNF.infrom AP that IMS RTT call upgrade/downgrade success/fail */
    MIPC_CALL_LOCAL_RTT_MODIFY_RESULT_IND                   = 18703,

    /* inform AP that Peer side RTT call modify indication */
    MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND                    = 18704,

    /* inform AP that rtt text receicve */
    MIPC_CALL_RTT_TEXT_RECEIVE_IND                          = 18705,

    /* Indication for Digits Line Info */
    MIPC_CALL_RCS_DIGITS_LINE_IND                           = 18706,

    /* Indication for CDMA call info */
    MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND                  = 18707,

    /* Indication for CDMA extended call info */
    MIPC_CALL_EXTENDED_DISPLAY_INFO_IND                     = 18708,

    /* Indication for line control info */
    MIPC_CALL_LINE_CONTROL_INFO_IND                         = 18709,

    /* Indication for redirecting number info */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND                   = 18710,

    /* gwsd event indication */
    MIPC_CALL_GWSD_EVENT_IND                                = 18711,

    /* econf urc indication */
    MIPC_CALL_ECONF_IND                                     = 18712,

    /* this message is used to report IMS sip header */
    MIPC_CALL_IMS_SIP_HEADER_IND                            = 18713,

    /* this message is used to indicate ecc redial */
    MIPC_CALL_ECC_REDIAL_IND                                = 18715,

    /* This message is used to indicate emergency bearer services support */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND                  = 18716,

    /* This command is used to provide User Interactive Service information accompanying with a specific ongoing Call */
    MIPC_CALL_UIS_INFO_IND                                  = 18717,

    /* Set IMS call additional information */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND                      = 18718,

    /* this message is used to indicate the INVITE SIP message of MT CALL */
    MIPC_CALL_MT_SIP_INVITE_IND                             = 18719,

    /* this message is used to indicate the received DTMF */
    MIPC_CALL_RECV_DTMF_IND                                 = 18720,

    /* send the initial DC configuartions related to a call to DC manager, which should then start to creating the Boostrap DC */
    MIPC_CALL_DC_CONFIG_INIT_IND                            = 18721,

    /* update the DC configurations related to an existing call to DC manager */
    MIPC_CALL_DC_CONFIG_UPDATE_IND                          = 18722,

    /* notify DC manager to de-initialize all the DC streams related with a special call */
    MIPC_CALL_DC_CONFIG_DEINIT_IND                          = 18723,

    /* MD notify AP that Data Channel status changed */
    MIPC_CALL_CONNECT_DATA_CHANNEL_IND                      = 18726,

    /* MD notify AP that there has existing Data Channel(s) been closed */
    MIPC_CALL_CLOSE_DATA_CHANNEL_IND                        = 18727,

    /* MD request to create new Data Channel(s) */
    MIPC_CALL_CREATE_DATA_CHANNEL_IND                       = 18728,

    /* This command requests users to approve an incoming call in Call Present stage */
    MIPC_CALL_APPROVE_INCOMING_CMD                          = 35073,
    MIPC_CALL_APPROVE_INCOMING_RSP                          = 35074,

    /* notify DC manager that DC Capability is ready to be received */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD                      = 35075,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP                      = 35076,

    /* indicate the DC manager that remote side has requested for creating new dcmap stream */
    MIPC_CALL_DC_CREATE_CMD                                 = 35077,
    MIPC_CALL_DC_CREATE_RSP                                 = 35078,
};

    /* MIPC_MSG.CALL_DIAL_REQ */
enum mipc_call_dial_req_tlv_enum {
    mipc_call_dial_req_tlv_NONE = 0,
    /* this field is digits number when DIAL_ADDRESS_TYPE=MIPC_CALL_DIAL_ADDRESS_TYPE_NONE or MIPC_CALL_DIAL_ADDRESS_TYPE_NUMBER;
    this field is URI when DIAL_ADDRESS_TYPE=MIPC_CALL_DIAL_ADDRESS_TYPE_SIP_URI */
    /* type = string */
    MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS                       = 0x8100,
    /* the type of DIAL_ADDRESS */
    /* type = uint32_t, refer to CALL_DIAL_ADDRESS_TYPE */
    MIPC_CALL_DIAL_REQ_T_DIAL_ADDRESS_TYPE                  = 0x101,
    /* the call type. MIPC_CALL_DIAL_TYPE_RTT don't support currently; Default value is 1 */
    /* type = uint32_t, refer to CALL_DIAL_TYPE */
    MIPC_CALL_DIAL_REQ_T_TYPE                               = 0x102,
    /* the call domain; Default value is 0 */
    /* type = uint32_t, refer to CALL_DIAL_DOMAIN */
    MIPC_CALL_DIAL_REQ_T_DOMAIN                             = 0x103,
    /* only support AUTO, CS_ONLY, 3GPP2_ONLY; Default value is 0 */
    /* type = uint32_t, refer to CALL_DIAL_DOMAIN */
    MIPC_CALL_DIAL_REQ_T_ECC_RETRY_DOMAIN                   = 0x104,
    /* A bitmap indicating the Emergency Service Category Value according to 3GPP TS 24.008; Default value is 0xffff */
    /* type = uint16_t */
    MIPC_CALL_DIAL_REQ_T_ECC_CATEGORY                       = 0x105,
    /* Requests calling line identification restriction (once); Default value is False */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_DIAL_REQ_T_CLIR                               = 0x106,
    /* when set ecc test mode, it's necessary; Default value is False */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_DIAL_REQ_T_IS_ECC_TESTING                     = 0x107,
    /* Requests calling line identification restriction (once); Default value is 0 */
    /* type = uint8_t, refer to CALL_CLIR_MODE */
    MIPC_CALL_DIAL_REQ_T_CLIR_EXT                           = 0x108,
};

    /* MIPC_MSG.CALL_DIAL_CNF */
enum mipc_call_dial_cnf_tlv_enum {
    mipc_call_dial_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SS_REQ */
enum mipc_call_ss_req_tlv_enum {
    mipc_call_ss_req_tlv_NONE = 0,
    /* the action of call-related supplementary services */
    /* type = uint32_t, refer to CALL_SS_ACTION */
    MIPC_CALL_SS_REQ_T_ACTION                               = 0x100,
    /* this field is only necessary when action=MIPC_CALL_SS_ACTION_HOLD_CALL or MIPC_CALL_SS_ACTION_RESUME_CALL; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_SS_REQ_T_CALLID                               = 0x101,
    /* this field is only necessary when action=MIPC_CONST.CALL_SS_ACTION_EXPLICIT_CALL_AND_TRANSFER; Default value is 0 */
    /* type = uint8_t, refer to CALL_ECT_TYPE */
    MIPC_CALL_SS_REQ_T_ECT_TYPE                             = 0x102,
    /* this field is only necessary when action=MIPC_CONST.CALL_SS_ACTION_EXPLICIT_CALL_AND_TRANSFER; Default value is null character */
    /* type = string */
    MIPC_CALL_SS_REQ_T_ECT_NUMBER                           = 0x8103,
};

    /* MIPC_MSG.CALL_SS_CNF */
enum mipc_call_ss_cnf_tlv_enum {
    mipc_call_ss_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_HANGUP_REQ */
enum mipc_call_hangup_req_tlv_enum {
    mipc_call_hangup_req_tlv_NONE = 0,
    /* the mode of hangup call */
    /* type = uint32_t, refer to CALL_HANGUP_MODE */
    MIPC_CALL_HANGUP_REQ_T_MODE                             = 0x100,
    /* this field is only necessary when mode=MIPC_CALL_HANGUP_MODE_HANGUP or MIPC_CALL_HANGUP_MODE_FORCE_HANGUP; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_HANGUP_REQ_T_CALLID                           = 0x101,
    /* this field is only necessary when mode=MIPC_CALL_HANGUP_MODE_HANGUP; Default value is 0 */
    /* type = uint32_t, refer to CALL_HANGUP_CAUSE */
    MIPC_CALL_HANGUP_REQ_T_CAUSE                            = 0x102,
};

    /* MIPC_MSG.CALL_HANGUP_CNF */
enum mipc_call_hangup_cnf_tlv_enum {
    mipc_call_hangup_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_ANSWER_REQ */
enum mipc_call_answer_req_tlv_enum {
    mipc_call_answer_req_tlv_NONE = 0,
    /* the mode of answer call; Default value is 0 */
    /* type = uint32_t, refer to CALL_ANSWER_MODE */
    MIPC_CALL_ANSWER_REQ_T_MODE                             = 0x100,
    /* this field is only necessary when mode=MIPC_CALL_ANSWER_MODE_AUDIO_CALL, MIPC_CALL_ANSWER_MODE_Rx_VIDEO_CALL or MIPC_CALL_ANSWER_MODE_Tx_VIDEO_CALL; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_ANSWER_REQ_T_CALLID                           = 0x101,
};

    /* MIPC_MSG.CALL_ANSWER_CNF */
enum mipc_call_answer_cnf_tlv_enum {
    mipc_call_answer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_CALL_STATUS_REQ */
enum mipc_call_get_call_status_req_tlv_enum {
    mipc_call_get_call_status_req_tlv_NONE = 0,
    /* Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_GET_CALL_STATUS_REQ_T_CALLID                  = 0x100,
};

    /* MIPC_MSG.CALL_GET_CALL_STATUS_CNF */
enum mipc_call_get_call_status_cnf_tlv_enum {
    mipc_call_get_call_status_cnf_tlv_NONE = 0,
    /* the total number of current protocol call */
    /* type = uint32_t */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_COUNT                   = 0x100,
    /* the call id of call */
    /* type = uint32_t */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_CALLID                  = 0x10B,
    /* the direction of call */
    /* type = uint32_t, refer to CALL_DIRECTION */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_DIRECTION               = 0x10C,
    /* the mode of call. Whether the call is in the CS domain or IMS domain is determined by the specific value of call mode */
    /* type = uint32_t, refer to CALL_MODE */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_MODE                    = 0x10D,
    /* the call clcc state of call */
    /* type = uint32_t, refer to CALL_CLCC_STATE */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_CALL_CLCC_STATE         = 0x10E,
    /* the number type of call */
    /* type = uint32_t, refer to CALL_DIAL_ADDRESS_TYPE */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_NUMBER_TYPE             = 0x10F,
    /* type of address (refer TS 24.008 [8] subclause 10.5.4.7) */
    /* type = uint32_t */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_TON                     = 0x110,
    /* the number of call */
    /* type = string */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_NUMBER                  = 0x8111,
    /* the rat of call. Currently, RAT only indicates the call is in CS domain or IMS domain */
    /* type = uint32_t, refer to CALL_RAT */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_RAT                     = 0x112,
    /* the type of call */
    /* type = bitmap, refer to CALL_TYPE */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_TYPE                    = 0x113,
    /* the detail info of call */
    /* type = struct, refer to call_detail_info */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_DETAIL_INFO             = 0x8114,
    /* the video capability of call */
    /* type = struct, refer to call_video_cap */
    MIPC_CALL_GET_CALL_STATUS_CNF_T_VIDEO_CAP               = 0x115,
};

    /* MIPC_MSG.CALL_CONFERENCE_REQ */
enum mipc_call_conference_req_tlv_enum {
    mipc_call_conference_req_tlv_NONE = 0,
    /* the conference call id. this file is only used when ACTION=MIPC_CALL_CONF_ACTION_ADD_PARTICIPANT or MIPC_CALL_CONF_ACTION_REMOVE_PARTICIPANT; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_CONFERENCE_REQ_T_CONF_CALLID                  = 0x100,
    /* the conference action; Default value is 0 */
    /* type = uint32_t, refer to CALL_CONF_ACTION */
    MIPC_CALL_CONFERENCE_REQ_T_ACTION                       = 0x101,
    /* the participant number.this file is only used when ACTION=MIPC_CALL_CONF_ACTION_ADD_PARTICIPANT, MIPC_CALL_CONF_ACTION_REMOVE_PARTICIPANT or MIPC_CALL_CONF_ACTION_CDMA_FLASH; Default value is null character */
    /* type = string */
    MIPC_CALL_CONFERENCE_REQ_T_NUMBER                       = 0x8102,
    /* the target call id for action.this file is only used when ACTION=MIPC_CALL_CONF_ACTION_ADD_PARTICIPANT or MIPC_CALL_CONF_ACTION_SPLIT; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_CONFERENCE_REQ_T_TARGET_CALLID                = 0x103,
};

    /* MIPC_MSG.CALL_CONFERENCE_CNF */
enum mipc_call_conference_cnf_tlv_enum {
    mipc_call_conference_cnf_tlv_NONE = 0,
    /* error cause */
    /* type = uint16_t, refer to CALL_REJECT_CAUSE */
    MIPC_CALL_CONFERENCE_CNF_T_CAUSE                        = 0x100,
};

    /* MIPC_MSG.CALL_GET_CONFERENCE_INFO_REQ */
enum mipc_call_get_conference_info_req_tlv_enum {
    mipc_call_get_conference_info_req_tlv_NONE = 0,
    /* the conference call id */
    /* type = uint32_t */
    MIPC_CALL_GET_CONFERENCE_INFO_REQ_T_CONF_CALLID         = 0x100,
};

    /* MIPC_MSG.CALL_GET_CONFERENCE_INFO_CNF */
enum mipc_call_get_conference_info_cnf_tlv_enum {
    mipc_call_get_conference_info_cnf_tlv_NONE = 0,
    /* the number of conference participant */
    /* type = uint32_t */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_COUNT               = 0x100,
    /* the direction of conference participant */
    /* type = uint32_t, refer to CALL_DIRECTION */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_DIRECTION           = 0x10B,
    /* call number */
    /* type = string */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_NUMBER  = 0x810C,
    /* the name of conference participant */
    /* type = string */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_NAME    = 0x810D,
    /* the status of conference participant */
    /* type = uint32_t, refer to CONF_PARTICIPANT_STATUS */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_STATUS  = 0x10E,
    /* call user entity string */
    /* type = string */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_USER_ENTITY = 0x810F,
    /* call endpoint entity string */
    /* type = string */
    MIPC_CALL_GET_CONFERENCE_INFO_CNF_T_PARTICIPANT_ENDPOINT_ENTITY = 0x8110,
};

    /* MIPC_MSG.CALL_GET_FINISH_REASON_REQ */
enum mipc_call_get_finish_reason_req_tlv_enum {
    mipc_call_get_finish_reason_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_FINISH_REASON_CNF */
enum mipc_call_get_finish_reason_cnf_tlv_enum {
    mipc_call_get_finish_reason_cnf_tlv_NONE = 0,
    /* the value listed in GSM 04.08 annex H */
    /* type = uint32_t */
    MIPC_CALL_GET_FINISH_REASON_CNF_T_REASON                = 0x100,
    /* the describes reason value */
    /* type = string */
    MIPC_CALL_GET_FINISH_REASON_CNF_T_REASON_STR            = 0x101,
};

    /* MIPC_MSG.CALL_DTMF_REQ */
enum mipc_call_dtmf_req_tlv_enum {
    mipc_call_dtmf_req_tlv_NONE = 0,
    /* DTMF START or STOP */
    /* type = uint32_t, refer to DTMF_MODE */
    MIPC_CALL_DTMF_REQ_T_MODE                               = 0x100,
    /* this field is only necessary when mode=START or SINGLE_TONE or BURST_TONE; Default value is null character */
    /* type = string */
    MIPC_CALL_DTMF_REQ_T_DIGIT                              = 0x8101,
};

    /* MIPC_MSG.CALL_DTMF_CNF */
enum mipc_call_dtmf_cnf_tlv_enum {
    mipc_call_dtmf_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_ECC_LIST_REQ */
enum mipc_call_get_ecc_list_req_tlv_enum {
    mipc_call_get_ecc_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_ECC_LIST_CNF */
enum mipc_call_get_ecc_list_cnf_tlv_enum {
    mipc_call_get_ecc_list_cnf_tlv_NONE = 0,
    /* the total number of ECC number */
    /* type = uint32_t */
    MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_COUNT                 = 0x100,
    /* the info of ECC number */
    /* type = struct_array, refer to ecc_info */
    MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_LIST                  = 0x101,
    /* the info of ECC number */
    /* type = struct, refer to ecc_info */
    MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_TLV_ARRAY             = 0x102,
};

    /* MIPC_MSG.CALL_SET_ECC_LIST_REQ */
enum mipc_call_set_ecc_list_req_tlv_enum {
    mipc_call_set_ecc_list_req_tlv_NONE = 0,
    /* the total number of ECC number. when equal to 0, L5 will clear AP ECC list */
    /* type = uint32_t */
    MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_COUNT                 = 0x100,
    /* when set ecc list, MAX_CALL_ECC_LIST_RECORD_NUM is 16 */
    /* type = struct_array, refer to ecc_info */
    MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_LIST                  = 0x101,
    /* when set ecc list, MAX_CALL_ECC_LIST_RECORD_NUM is 16 */
    /* type = struct, refer to ecc_info */
    MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_TLV_ARRAY             = 0x102,
};

    /* MIPC_MSG.CALL_SET_ECC_LIST_CNF */
enum mipc_call_set_ecc_list_cnf_tlv_enum {
    mipc_call_set_ecc_list_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ */
enum mipc_call_set_flight_mode_ecc_session_req_tlv_enum {
    mipc_call_set_flight_mode_ecc_session_req_tlv_NONE = 0,
    /* 0 means normal mode, 1 means flight mode */
    /* type = uint8_t */
    MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ_T_IS_FLIGHT_MODE = 0x100,
};

    /* MIPC_MSG.CALL_SET_FLIGHT_MODE_ECC_SESSION_CNF */
enum mipc_call_set_flight_mode_ecc_session_cnf_tlv_enum {
    mipc_call_set_flight_mode_ecc_session_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_EXIT_ECBM_MODE_REQ */
enum mipc_call_set_exit_ecbm_mode_req_tlv_enum {
    mipc_call_set_exit_ecbm_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_EXIT_ECBM_MODE_CNF */
enum mipc_call_set_exit_ecbm_mode_cnf_tlv_enum {
    mipc_call_set_exit_ecbm_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_IVS_ONEKEY_ECALL_REQ */
enum mipc_call_ivs_onekey_ecall_req_tlv_enum {
    mipc_call_ivs_onekey_ecall_req_tlv_NONE = 0,
    /* the type of eCALL */
    /* type = uint8_t, refer to ECALL_TYPE */
    MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_TYPE                   = 0x100,
    /* only support binary now; Default value is 1 */
    /* type = uint8_t, refer to ECALL_MSD_FORMAT */
    MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_MSD_FORMAT             = 0x101,
    /* the MSD of eCALL.The type of string is in the format specified by MSD_FORMAT.Minimum set of data, see CEN EN 15722:2015 [157].; Default value is NULL */
    /* type = byte_array */
    MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_MSD                    = 0x102,
    /* the domain of eCALL.Default value is AUTO */
    /* type = uint32_t, refer to CALL_DIAL_DOMAIN */
    MIPC_CALL_IVS_ONEKEY_ECALL_REQ_T_DOMAIN                 = 0x103,
};

    /* MIPC_MSG.CALL_IVS_ONEKEY_ECALL_CNF */
enum mipc_call_ivs_onekey_ecall_cnf_tlv_enum {
    mipc_call_ivs_onekey_ecall_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_SIP_HEADER_REQ */
enum mipc_call_set_sip_header_req_tlv_enum {
    mipc_call_set_sip_header_req_tlv_NONE = 0,
    /* total number of additinal info */
    /* type = uint32_t */
    MIPC_CALL_SET_SIP_HEADER_REQ_T_TOTAL                    = 0x100,
    /* index of current additional info */
    /* type = uint32_t */
    MIPC_CALL_SET_SIP_HEADER_REQ_T_INDEX                    = 0x101,
    /* number of key or key/value pair  */
    /* type = uint32_t */
    MIPC_CALL_SET_SIP_HEADER_REQ_T_COUNT                    = 0x102,
    /* when the strlen of SIP headers more than MAX_CALL_SIP_HEADER_VALUE_PAIR_LEN, please use index slice */
    /* type = string */
    MIPC_CALL_SET_SIP_HEADER_REQ_T_VALUE_PAIR               = 0x8103,
};

    /* MIPC_MSG.CALL_SET_SIP_HEADER_CNF */
enum mipc_call_set_sip_header_cnf_tlv_enum {
    mipc_call_set_sip_header_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ */
enum mipc_call_enable_ims_sip_header_report_req_tlv_enum {
    mipc_call_enable_ims_sip_header_report_req_tlv_NONE = 0,
    /* the call id of query ims sip header */
    /* type = uint32_t */
    MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ_T_CALL_ID    = 0x100,
    /* wanted type of header,ex.13: CALL-ID header */
    /* type = uint32_t */
    MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ_T_HEADER_TYPE = 0x101,
};

    /* MIPC_MSG.CALL_ENABLE_IMS_SIP_HEADER_REPORT_CNF */
enum mipc_call_enable_ims_sip_header_report_cnf_tlv_enum {
    mipc_call_enable_ims_sip_header_report_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_CALL_ADDITIONAL_INFO_REQ */
enum mipc_call_set_call_additional_info_req_tlv_enum {
    mipc_call_set_call_additional_info_req_tlv_NONE = 0,
    /* The call mode will be applied till next command to change call_mode. */
    /* type = uint8_t, refer to CALL_ADDITIONAL_INFO_MODE */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_MODE           = 0x100,
    /* the type of IMS call additional information */
    /* type = uint8_t, refer to CALL_ADDITIONAL_INFO_TYPE */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_TYPE           = 0x101,
    /* total number of additinal info */
    /* type = uint32_t */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_TOTAL          = 0x102,
    /* index of current additional info */
    /* type = uint32_t */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_INDEX          = 0x103,
    /* number of key or key/value pair in additional info */
    /* type = uint32_t */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_COUNT          = 0x104,
    /* IMS call additional information */
    /* type = string */
    MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ_T_ADDITIONAL_INFO = 0x8105,
};

    /* MIPC_MSG.CALL_SET_CALL_ADDITIONAL_INFO_CNF */
enum mipc_call_set_call_additional_info_cnf_tlv_enum {
    mipc_call_set_call_additional_info_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_PEER_RTT_MODIFY_REQ */
enum mipc_call_peer_rtt_modify_req_tlv_enum {
    mipc_call_peer_rtt_modify_req_tlv_NONE = 0,
    /* the peer side RTT call id */
    /* type = uint32_t */
    MIPC_CALL_PEER_RTT_MODIFY_REQ_T_CALL_ID                 = 0x100,
    /* the result of peer side RTT call modify request */
    /* type = uint8_t, refer to CALL_PEER_RTT_MODIFY_RESULT */
    MIPC_CALL_PEER_RTT_MODIFY_REQ_T_RESULT                  = 0x101,
};

    /* MIPC_MSG.CALL_PEER_RTT_MODIFY_CNF */
enum mipc_call_peer_rtt_modify_cnf_tlv_enum {
    mipc_call_peer_rtt_modify_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_LOCAL_RTT_MODIFY_REQ */
enum mipc_call_local_rtt_modify_req_tlv_enum {
    mipc_call_local_rtt_modify_req_tlv_NONE = 0,
    /* the RTT call id */
    /* type = uint32_t */
    MIPC_CALL_LOCAL_RTT_MODIFY_REQ_T_CALL_ID                = 0x100,
    /* the operation of RTT call */
    /* type = uint8_t, refer to CALL_LOCAL_RTT_MODIFY_OP */
    MIPC_CALL_LOCAL_RTT_MODIFY_REQ_T_OP                     = 0x101,
};

    /* MIPC_MSG.CALL_LOCAL_RTT_MODIFY_CNF */
enum mipc_call_local_rtt_modify_cnf_tlv_enum {
    mipc_call_local_rtt_modify_cnf_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_LOCAL_RTT_MODIFY_CNF_T_CALL_ID                = 0x100,
    /* 0:success; 1:fail */
    /* type = uint8_t */
    MIPC_CALL_LOCAL_RTT_MODIFY_CNF_T_RESULT                 = 0x101,
};

    /* MIPC_MSG.CALL_RTT_TEXT_REQ */
enum mipc_call_rtt_text_req_tlv_enum {
    mipc_call_rtt_text_req_tlv_NONE = 0,
    /* the call id of RTT call */
    /* type = uint32_t */
    MIPC_CALL_RTT_TEXT_REQ_T_CALL_ID                        = 0x100,
    /* the len of rtt text */
    /* type = uint32_t */
    MIPC_CALL_RTT_TEXT_REQ_T_LEN                            = 0x101,
    /* the rtt text */
    /* type = string */
    MIPC_CALL_RTT_TEXT_REQ_T_TEXT                           = 0x102,
    /* M bit in RTP */
    /* type = uint8_t */
    MIPC_CALL_RTT_TEXT_REQ_T_M_BIT                          = 0x103,
};

    /* MIPC_MSG.CALL_RTT_TEXT_CNF */
enum mipc_call_rtt_text_cnf_tlv_enum {
    mipc_call_rtt_text_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_RTT_MODE_REQ */
enum mipc_call_rtt_mode_req_tlv_enum {
    mipc_call_rtt_mode_req_tlv_NONE = 0,
    /* the operation of IMS RTT capability */
    /* type = uint8_t, refer to CALL_RTT_MODE_OP */
    MIPC_CALL_RTT_MODE_REQ_T_OP                             = 0x100,
};

    /* MIPC_MSG.CALL_RTT_MODE_CNF */
enum mipc_call_rtt_mode_cnf_tlv_enum {
    mipc_call_rtt_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_RTT_AUDIO_REQ */
enum mipc_call_rtt_audio_req_tlv_enum {
    mipc_call_rtt_audio_req_tlv_NONE = 0,
    /* the call id of RTT Audio */
    /* type = uint32_t */
    MIPC_CALL_RTT_AUDIO_REQ_T_CALL_ID                       = 0x100,
    /* 0: disable audio indication; 1:enable audio indication */
    /* type = uint8_t */
    MIPC_CALL_RTT_AUDIO_REQ_T_ENABLE                        = 0x101,
};

    /* MIPC_MSG.CALL_RTT_AUDIO_CNF */
enum mipc_call_rtt_audio_cnf_tlv_enum {
    mipc_call_rtt_audio_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_RCS_STATE_AND_FEATURE_REQ */
enum mipc_call_set_rcs_state_and_feature_req_tlv_enum {
    mipc_call_set_rcs_state_and_feature_req_tlv_NONE = 0,
    /* the state of RCS */
    /* type = uint32_t, refer to CALL_RCS_STATE */
    MIPC_CALL_SET_RCS_STATE_AND_FEATURE_REQ_T_STATE         = 0x100,
    /* A bit mask indicate the corresponding RCS feature supported or not.; Default value is 0xffffffff */
    /* type = bitmap, refer to CALL_RCS_FEATURE */
    MIPC_CALL_SET_RCS_STATE_AND_FEATURE_REQ_T_FEATURE       = 0x101,
};

    /* MIPC_MSG.CALL_SET_RCS_STATE_AND_FEATURE_CNF */
enum mipc_call_set_rcs_state_and_feature_cnf_tlv_enum {
    mipc_call_set_rcs_state_and_feature_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_UPDATE_RCS_SESSION_INFO_REQ */
enum mipc_call_update_rcs_session_info_req_tlv_enum {
    mipc_call_update_rcs_session_info_req_tlv_NONE = 0,
    /* 0:disable;1:enable. */
    /* type = uint8_t */
    MIPC_CALL_UPDATE_RCS_SESSION_INFO_REQ_T_STATUS          = 0x100,
};

    /* MIPC_MSG.CALL_UPDATE_RCS_SESSION_INFO_CNF */
enum mipc_call_update_rcs_session_info_cnf_tlv_enum {
    mipc_call_update_rcs_session_info_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_VOICE_DOMAIN_PREFERENCE_REQ */
enum mipc_call_get_voice_domain_preference_req_tlv_enum {
    mipc_call_get_voice_domain_preference_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF */
enum mipc_call_get_voice_domain_preference_cnf_tlv_enum {
    mipc_call_get_voice_domain_preference_cnf_tlv_NONE = 0,
    /* indicates the voice domain preference of the UE. */
    /* type = uint32_t, refer to CALL_VOICE_DOMAIN */
    MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF_T_SETTING     = 0x100,
};

    /* MIPC_MSG.CALL_PULL_REQ */
enum mipc_call_pull_req_tlv_enum {
    mipc_call_pull_req_tlv_NONE = 0,
    /* target URI */
    /* type = string */
    MIPC_CALL_PULL_REQ_T_URI                                = 0x8100,
    /* the call mode of pull */
    /* type = uint8_t, refer to CALL_PULL_TYPE */
    MIPC_CALL_PULL_REQ_T_TYPE                               = 0x101,
};

    /* MIPC_MSG.CALL_PULL_CNF */
enum mipc_call_pull_cnf_tlv_enum {
    mipc_call_pull_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_TTY_MODE_REQ */
enum mipc_call_get_tty_mode_req_tlv_enum {
    mipc_call_get_tty_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_GET_TTY_MODE_CNF */
enum mipc_call_get_tty_mode_cnf_tlv_enum {
    mipc_call_get_tty_mode_cnf_tlv_NONE = 0,
    /* the mode of TTY call */
    /* type = uint8_t, refer to CALL_TTY_MODE */
    MIPC_CALL_GET_TTY_MODE_CNF_T_MODE                       = 0x100,
};

    /* MIPC_MSG.CALL_SET_IMS_CALL_MODE_REQ */
enum mipc_call_set_ims_call_mode_req_tlv_enum {
    mipc_call_set_ims_call_mode_req_tlv_NONE = 0,
    /* Call related Client API mode or original IMS mode */
    /* type = uint8_t, refer to CALL_IMS_CALL_MODE */
    MIPC_CALL_SET_IMS_CALL_MODE_REQ_T_OP                    = 0x100,
};

    /* MIPC_MSG.CALL_SET_IMS_CALL_MODE_CNF */
enum mipc_call_set_ims_call_mode_cnf_tlv_enum {
    mipc_call_set_ims_call_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_TTY_MODE_REQ */
enum mipc_call_set_tty_mode_req_tlv_enum {
    mipc_call_set_tty_mode_req_tlv_NONE = 0,
    /* the mode of TTY call */
    /* type = uint8_t, refer to CALL_TTY_MODE */
    MIPC_CALL_SET_TTY_MODE_REQ_T_MODE                       = 0x100,
};

    /* MIPC_MSG.CALL_SET_TTY_MODE_CNF */
enum mipc_call_set_tty_mode_cnf_tlv_enum {
    mipc_call_set_tty_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ */
enum mipc_call_set_voice_domain_preference_req_tlv_enum {
    mipc_call_set_voice_domain_preference_req_tlv_NONE = 0,
    /* the voice domain preference of the UE. The default value is manufacturer specific. */
    /* type = uint32_t, refer to CALL_VOICE_DOMAIN */
    MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ_T_SETTING     = 0x100,
};

    /* MIPC_MSG.CALL_SET_VOICE_DOMAIN_PREFERENCE_CNF */
enum mipc_call_set_voice_domain_preference_cnf_tlv_enum {
    mipc_call_set_voice_domain_preference_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_CONFERENCE_DIAL_REQ */
enum mipc_call_conference_dial_req_tlv_enum {
    mipc_call_conference_dial_req_tlv_NONE = 0,
    /* the type of conference call; Default value is 1 */
    /* type = uint32_t, refer to CALL_CONFERENCE_DIAL_TYPE */
    MIPC_CALL_CONFERENCE_DIAL_REQ_T_TYPE                    = 0x100,
    /* Requests calling line identification restriction (once); Default value is 0 */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_CONFERENCE_DIAL_REQ_T_CLIR                    = 0x101,
    /* the count of dial_address; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_CONFERENCE_DIAL_REQ_T_COUNT                   = 0x102,
    /* telephone number or SIP URI; Default value is null character */
    /* type = string */
    MIPC_CALL_CONFERENCE_DIAL_REQ_T_DIAL_ADDRESS            = 0x8103,
    /* Requests calling line identification restriction (once); Default value is 0 */
    /* type = uint8_t, refer to CALL_CLIR_MODE */
    MIPC_CALL_CONFERENCE_DIAL_REQ_T_CLIR_EXT                = 0x104,
};

    /* MIPC_MSG.CALL_CONFERENCE_DIAL_CNF */
enum mipc_call_conference_dial_cnf_tlv_enum {
    mipc_call_conference_dial_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_GWSD_MODE_REQ */
enum mipc_call_set_gwsd_mode_req_tlv_enum {
    mipc_call_set_gwsd_mode_req_tlv_NONE = 0,
    /* the action of set GWSD mode */
    /* type = uint32_t, refer to SET_GWSD_MODE_ACTION */
    MIPC_CALL_SET_GWSD_MODE_REQ_T_ACTION                    = 0x101,
    /* the mode of GWSD */
    /* type = uint32_t */
    MIPC_CALL_SET_GWSD_MODE_REQ_T_MODE                      = 0x102,
    /* the keepalive mode; Default value is 0 */
    /* type = uint32_t */
    MIPC_CALL_SET_GWSD_MODE_REQ_T_KA_MODE                   = 0x103,
    /* the keepalive cycle; Default value is null character */
    /* type = string */
    MIPC_CALL_SET_GWSD_MODE_REQ_T_KA_CYCLE                  = 0x104,
};

    /* MIPC_MSG.CALL_SET_GWSD_MODE_CNF */
enum mipc_call_set_gwsd_mode_cnf_tlv_enum {
    mipc_call_set_gwsd_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_APPROVE_CSFB_REQ */
enum mipc_call_approve_csfb_req_tlv_enum {
    mipc_call_approve_csfb_req_tlv_NONE = 0,
    /* Default value is 0 */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_APPROVE_CSFB_REQ_T_IS_APPROVE                 = 0x100,
};

    /* MIPC_MSG.CALL_APPROVE_CSFB_CNF */
enum mipc_call_approve_csfb_cnf_tlv_enum {
    mipc_call_approve_csfb_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_GWSD_CALL_VALID_REQ */
enum mipc_call_set_gwsd_call_valid_req_tlv_enum {
    mipc_call_set_gwsd_call_valid_req_tlv_NONE = 0,
    /* the call valid timer */
    /* type = uint32_t */
    MIPC_CALL_SET_GWSD_CALL_VALID_REQ_T_TIMER               = 0x101,
};

    /* MIPC_MSG.CALL_SET_GWSD_CALL_VALID_CNF */
enum mipc_call_set_gwsd_call_valid_cnf_tlv_enum {
    mipc_call_set_gwsd_call_valid_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_GWSD_IGNORE_CALL_INTERVAL_REQ */
enum mipc_call_set_gwsd_ignore_call_interval_req_tlv_enum {
    mipc_call_set_gwsd_ignore_call_interval_req_tlv_NONE = 0,
    /* the ignore call interval */
    /* type = uint32_t */
    MIPC_CALL_SET_GWSD_IGNORE_CALL_INTERVAL_REQ_T_INTERVAL  = 0x101,
};

    /* MIPC_MSG.CALL_SET_GWSD_IGNORE_CALL_INTERVAL_CNF */
enum mipc_call_set_gwsd_ignore_call_interval_cnf_tlv_enum {
    mipc_call_set_gwsd_ignore_call_interval_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_GWSD_KA_PDCP_REQ */
enum mipc_call_set_gwsd_ka_pdcp_req_tlv_enum {
    mipc_call_set_gwsd_ka_pdcp_req_tlv_NONE = 0,
    /* the keep alive PDCP */
    /* type = string */
    MIPC_CALL_SET_GWSD_KA_PDCP_REQ_T_PDATA                  = 0x101,
};

    /* MIPC_MSG.CALL_SET_GWSD_KA_PDCP_CNF */
enum mipc_call_set_gwsd_ka_pdcp_cnf_tlv_enum {
    mipc_call_set_gwsd_ka_pdcp_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SET_GWSD_KA_IPDATA_REQ */
enum mipc_call_set_gwsd_ka_ipdata_req_tlv_enum {
    mipc_call_set_gwsd_ka_ipdata_req_tlv_NONE = 0,
    /* the keep alive IPDATA */
    /* type = string */
    MIPC_CALL_SET_GWSD_KA_IPDATA_REQ_T_PDATA                = 0x101,
};

    /* MIPC_MSG.CALL_SET_GWSD_KA_IPDATA_CNF */
enum mipc_call_set_gwsd_ka_ipdata_cnf_tlv_enum {
    mipc_call_set_gwsd_ka_ipdata_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_UIS_INFO_REQ */
enum mipc_call_uis_info_req_tlv_enum {
    mipc_call_uis_info_req_tlv_NONE = 0,
    /* call ID */
    /* type = uint32_t */
    MIPC_CALL_UIS_INFO_REQ_T_CALLID                         = 0x100,
    /* Indicates the type of the UIS Info */
    /* type = uint8_t, refer to CALL_UIS_INFO_TYPE */
    MIPC_CALL_UIS_INFO_REQ_T_TYPE                           = 0x101,
    /* Indicates the detailed UIS Info in string format; Default value is null character */
    /* type = string */
    MIPC_CALL_UIS_INFO_REQ_T_DATA                           = 0x102,
    /* Indicates the duration of the screen touching action */
    /* type = uint32_t */
    MIPC_CALL_UIS_INFO_REQ_T_DURATION                       = 0x103,
};

    /* MIPC_MSG.CALL_UIS_INFO_CNF */
enum mipc_call_uis_info_cnf_tlv_enum {
    mipc_call_uis_info_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_AUTO_ANSWER_REQ */
enum mipc_call_auto_answer_req_tlv_enum {
    mipc_call_auto_answer_req_tlv_NONE = 0,
    /* enable auto answer */
    /* type = uint8_t */
    MIPC_CALL_AUTO_ANSWER_REQ_T_ENABLE                      = 0x100,
};

    /* MIPC_MSG.CALL_AUTO_ANSWER_CNF */
enum mipc_call_auto_answer_cnf_tlv_enum {
    mipc_call_auto_answer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_DATA_PREFER_SET_REQ */
enum mipc_call_data_prefer_set_req_tlv_enum {
    mipc_call_data_prefer_set_req_tlv_NONE = 0,
    /* indicates the state of GPRS transfer preference, <state> of AT+EGTP */
    /* type = uint8_t, refer to CALL_PREFER_SET_STATE */
    MIPC_CALL_DATA_PREFER_SET_REQ_T_STATE                   = 0x100,
    /* indicates the setting of Monitor Peer PCH, <mode> of AT+EMPPCH */
    /* type = uint8_t, refer to CALL_PREFER_SET_MONITOR_MODE */
    MIPC_CALL_DATA_PREFER_SET_REQ_T_MODE                    = 0x101,
};

    /* MIPC_MSG.CALL_DATA_PREFER_SET_CNF */
enum mipc_call_data_prefer_set_cnf_tlv_enum {
    mipc_call_data_prefer_set_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_ECC_REDIAL_APPROVE_REQ */
enum mipc_call_ecc_redial_approve_req_tlv_enum {
    mipc_call_ecc_redial_approve_req_tlv_NONE = 0,
    /*  0 means disapprove this call, 1 means approve */
    /* type = uint8_t */
    MIPC_CALL_ECC_REDIAL_APPROVE_REQ_T_APPROVE              = 0x100,
    /*  call id */
    /* type = uint32_t */
    MIPC_CALL_ECC_REDIAL_APPROVE_REQ_T_CALL_ID              = 0x101,
};

    /* MIPC_MSG.CALL_ECC_REDIAL_APPROVE_CNF */
enum mipc_call_ecc_redial_approve_cnf_tlv_enum {
    mipc_call_ecc_redial_approve_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_DC_CREATE_SIP_REQ */
enum mipc_call_dc_create_sip_req_tlv_enum {
    mipc_call_dc_create_sip_req_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_CALL_ID                   = 0x100,
    /* The number of valid dc config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_NUM_DC_CONFIG             = 0x101,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_INDEX                     = 0x102,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_INDEX_TLV_ARRAY           = 0x102,
    /* 0: UE to Server, 1: UE to UE */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DC_TYPE                   = 0x103,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DC_TYPE_TLV_ARRAY         = 0x103,
    /* 0: IPv4; 1: IPv6 */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_IP_ADDR_TYPE              = 0x104,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_IP_ADDR_TYPE_TLV_ARRAY    = 0x104,
    /* remote udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_UDP_PORT           = 0x105,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_UDP_PORT_TLV_ARRAY = 0x105,
    /* remote sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_SCTP_PORT          = 0x106,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_SCTP_PORT_TLV_ARRAY = 0x106,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_UDP_PORT            = 0x107,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_UDP_PORT_TLV_ARRAY  = 0x107,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_SCTP_PORT           = 0x108,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_SCTP_PORT_TLV_ARRAY = 0x108,
    /* remote IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_ADDR               = 0x109,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_ADDR_TLV_ARRAY     = 0x109,
    /* local IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_ADDR                = 0x10A,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_ADDR_TLV_ARRAY      = 0x10A,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_DTLS_SETUP         = 0x10B,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_DTLS_SETUP_TLV_ARRAY = 0x10B,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_DTLS_SETUP          = 0x10C,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_DTLS_SETUP_TLV_ARRAY = 0x10C,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_TLS_ID             = 0x10D,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_TLS_ID_TLV_ARRAY   = 0x10D,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_TLS_ID              = 0x10E,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_TLS_ID_TLV_ARRAY    = 0x10E,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_FP_HASH_FUNC              = 0x10F,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_FP_HASH_FUNC_TLV_ARRAY    = 0x10F,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_FP_STR             = 0x110,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_FP_STR_TLV_ARRAY   = 0x110,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_FP_STR              = 0x111,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_FP_STR_TLV_ARRAY    = 0x111,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_MAX_MESSAGE_SIZE   = 0x112,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x112,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_MAX_MESSAGE_SIZE    = 0x113,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x113,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DATA_B_AS                 = 0x114,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DATA_B_AS_TLV_ARRAY       = 0x114,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_QOS_HINT_TLV_ARRAY        = 0x115,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_QOS_HINT                  = 0x115,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_NUM_DCMAP                 = 0x116,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_STREAM_ID           = 0x117,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_STREAM_ID_TLV_ARRAY = 0x117,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_PRIORITY            = 0x118,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_PRIORITY_TLV_ARRAY  = 0x118,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DC_CONFIG_INDEX           = 0x119,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DC_CONFIG_INDEX_TLV_ARRAY = 0x119,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_ORDERED             = 0x11A,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_ORDERED_TLV_ARRAY   = 0x11A,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_MAX_RETR            = 0x11B,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_MAX_RETR_TLV_ARRAY  = 0x11B,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_MAX_TIME            = 0x11C,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_MAX_TIME_TLV_ARRAY  = 0x11C,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_LABEL               = 0x11D,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_LABEL_TLV_ARRAY     = 0x11D,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_SUBPROTOCOL         = 0x11E,
    MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_SUBPROTOCOL_TLV_ARRAY = 0x11E,
};

    /* MIPC_MSG.CALL_DC_CREATE_SIP_CNF */
enum mipc_call_dc_create_sip_cnf_tlv_enum {
    mipc_call_dc_create_sip_cnf_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_SIP_CNF_T_CALL_ID                   = 0x100,
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_SIP_CNF_T_CAUSE                     = 0x101,
};

    /* MIPC_MSG.CALL_DC_MODIFY_SIP_REQ */
enum mipc_call_dc_modify_sip_req_tlv_enum {
    mipc_call_dc_modify_sip_req_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_CALL_ID                   = 0x100,
    /* The number of valid dc config */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_NUM_DC_CONFIG             = 0x101,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_INDEX                     = 0x102,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_INDEX_TLV_ARRAY           = 0x102,
    /* 0: UE to Server, 1: UE to UE */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DC_TYPE                   = 0x103,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DC_TYPE_TLV_ARRAY         = 0x103,
    /* 0: IPv4; 1: IPv6 */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_IP_ADDR_TYPE              = 0x104,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_IP_ADDR_TYPE_TLV_ARRAY    = 0x104,
    /* remote udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_UDP_PORT           = 0x105,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_UDP_PORT_TLV_ARRAY = 0x105,
    /* remote sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_SCTP_PORT          = 0x106,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_SCTP_PORT_TLV_ARRAY = 0x106,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_UDP_PORT            = 0x107,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_UDP_PORT_TLV_ARRAY  = 0x107,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_SCTP_PORT           = 0x108,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_SCTP_PORT_TLV_ARRAY = 0x108,
    /* remote IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_ADDR               = 0x109,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_ADDR_TLV_ARRAY     = 0x109,
    /* local IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_ADDR                = 0x10A,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_ADDR_TLV_ARRAY      = 0x10A,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_DTLS_SETUP         = 0x10B,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_DTLS_SETUP_TLV_ARRAY = 0x10B,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_DTLS_SETUP          = 0x10C,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_DTLS_SETUP_TLV_ARRAY = 0x10C,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_TLS_ID             = 0x10D,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_TLS_ID_TLV_ARRAY   = 0x10D,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_TLS_ID              = 0x10E,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_TLS_ID_TLV_ARRAY    = 0x10E,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_FP_HASH_FUNC              = 0x10F,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_FP_HASH_FUNC_TLV_ARRAY    = 0x10F,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_FP_STR             = 0x110,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_FP_STR_TLV_ARRAY   = 0x110,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_FP_STR              = 0x111,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_FP_STR_TLV_ARRAY    = 0x111,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_MAX_MESSAGE_SIZE   = 0x112,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x112,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_MAX_MESSAGE_SIZE    = 0x113,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x113,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DATA_B_AS                 = 0x114,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DATA_B_AS_TLV_ARRAY       = 0x114,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_QOS_HINT_TLV_ARRAY        = 0x115,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_QOS_HINT                  = 0x115,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_NUM_DCMAP                 = 0x116,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_STREAM_ID           = 0x117,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_STREAM_ID_TLV_ARRAY = 0x117,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_PRIORITY            = 0x118,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_PRIORITY_TLV_ARRAY  = 0x118,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DC_CONFIG_INDEX           = 0x119,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DC_CONFIG_INDEX_TLV_ARRAY = 0x119,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_ORDERED             = 0x11A,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_ORDERED_TLV_ARRAY   = 0x11A,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_MAX_RETR            = 0x11B,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_MAX_RETR_TLV_ARRAY  = 0x11B,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_MAX_TIME            = 0x11C,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_MAX_TIME_TLV_ARRAY  = 0x11C,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_LABEL               = 0x11D,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_LABEL_TLV_ARRAY     = 0x11D,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_SUBPROTOCOL         = 0x11E,
    MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_SUBPROTOCOL_TLV_ARRAY = 0x11E,
};

    /* MIPC_MSG.CALL_DC_MODIFY_SIP_CNF */
enum mipc_call_dc_modify_sip_cnf_tlv_enum {
    mipc_call_dc_modify_sip_cnf_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_MODIFY_SIP_CNF_T_CALL_ID                   = 0x100,
    /* type = uint16_t */
    MIPC_CALL_DC_MODIFY_SIP_CNF_T_CAUSE                     = 0x101,
};

    /* MIPC_MSG.CALL_DC_CLOSE_SIP_REQ */
enum mipc_call_dc_close_sip_req_tlv_enum {
    mipc_call_dc_close_sip_req_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CLOSE_SIP_REQ_T_CALL_ID                    = 0x100,
    /* close all stream or not */
    /* type = uint8_t */
    MIPC_CALL_DC_CLOSE_SIP_REQ_T_CLOSE_ALL_STREAM           = 0x101,
    /* The DCMAP streams that need to be deleted */
    /* type = uint8_t */
    MIPC_CALL_DC_CLOSE_SIP_REQ_T_NUM_DCMAP_DEL              = 0x102,
    /* Mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_CLOSE_SIP_REQ_T_DC_CONFIG_INDEX            = 0x103,
    /* The 'dcmap-stream-id' parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel.dcmap-stream-id = 1*5DIGIT */
    /* type = uint16_t */
    MIPC_CALL_DC_CLOSE_SIP_REQ_T_DCMAP_STREAM_ID            = 0x104,
    MIPC_CALL_DC_CLOSE_SIP_REQ_T_DCMAP_STREAM_ID_TLV_ARRAY  = 0x104,
};

    /* MIPC_MSG.CALL_DC_CLOSE_SIP_CNF */
enum mipc_call_dc_close_sip_cnf_tlv_enum {
    mipc_call_dc_close_sip_cnf_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CLOSE_SIP_CNF_T_CALL_ID                    = 0x100,
    /* type = uint16_t */
    MIPC_CALL_DC_CLOSE_SIP_CNF_T_CAUSE                      = 0x101,
};


    /* MIPC_MSG.CALL_CREATE_DATA_CHANNEL_REQ */
enum mipc_call_create_data_channel_req_tlv_enum {
    mipc_call_create_data_channel_req_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CREATE_DATA_CHANNEL_REQ_T_CALL_ID             = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CREATE_DATA_CHANNEL_REQ_T_DATA                = 0x101,
};

    /* MIPC_MSG.CALL_CREATE_DATA_CHANNEL_CNF */
enum mipc_call_create_data_channel_cnf_tlv_enum {
    mipc_call_create_data_channel_cnf_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CREATE_DATA_CHANNEL_CNF_T_CALL_ID             = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CREATE_DATA_CHANNEL_CNF_T_DATA                = 0x101,
};

    /* MIPC_MSG.CALL_CLOSE_DATA_CHANNEL_REQ */
enum mipc_call_close_data_channel_req_tlv_enum {
    mipc_call_close_data_channel_req_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CLOSE_DATA_CHANNEL_REQ_T_CALL_ID              = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CLOSE_DATA_CHANNEL_REQ_T_DATA                 = 0x101,
};

    /* MIPC_MSG.CALL_CLOSE_DATA_CHANNEL_CNF */
enum mipc_call_close_data_channel_cnf_tlv_enum {
    mipc_call_close_data_channel_cnf_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CLOSE_DATA_CHANNEL_CNF_T_CALL_ID              = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CLOSE_DATA_CHANNEL_CNF_T_DATA                 = 0x101,
};

    /* MIPC_MSG.CALL_STATUS_IND */
enum mipc_call_status_ind_tlv_enum {
    mipc_call_status_ind_tlv_NONE = 0,
    /* call ID */
    /* type = uint32_t */
    MIPC_CALL_STATUS_IND_T_CALLID                           = 0x100,
    /* type = uint32_t, refer to CALL_DIRECTION */
    MIPC_CALL_STATUS_IND_T_DIRECTION                        = 0x101,
    /* type = uint32_t, refer to CALL_MODE */
    MIPC_CALL_STATUS_IND_T_MODE                             = 0x102,
    /* type of address (refer TS 24.008 [8] subclause 10.5.4.7) */
    /* type = uint32_t */
    MIPC_CALL_STATUS_IND_T_TON                              = 0x103,
    /* type = string */
    MIPC_CALL_STATUS_IND_T_NUMBER                           = 0x8104,
    /* type = bitmap, refer to CALL_TYPE */
    MIPC_CALL_STATUS_IND_T_TYPE                             = 0x105,
    /* type = struct, refer to call_detail_info */
    MIPC_CALL_STATUS_IND_T_DETAIL_INFO                      = 0x8106,
    /* type = struct, refer to call_video_cap */
    MIPC_CALL_STATUS_IND_T_VIDEO_CAP                        = 0x107,
    /* the message type */
    /* type = uint32_t, refer to CALL_MSG_TYPE */
    MIPC_CALL_STATUS_IND_T_MSG_TYPE                         = 0x109,
    /* This information is only provided for MSG_TYPE=DISCONNECT, which is sent when modem receive RELEASE ir RELEAS COMPLETE CC nessage from the network. The value can refer to TS 24.008 Annex H and CALL_DISCONNECTED_CAUSE enums which are MTK defined. */
    /* type = uint32_t, refer to CALL_DISCONNECTED_CAUSE */
    MIPC_CALL_STATUS_IND_T_DISC_CAUSE                       = 0x10A,
    /* type = string */
    MIPC_CALL_STATUS_IND_T_PAU                              = 0x810B,
};

    /* MIPC_MSG.CALL_EVENT_IND */
enum mipc_call_event_ind_tlv_enum {
    mipc_call_event_ind_tlv_NONE = 0,
    /* call event */
    /* type = uint32_t, refer to CALL_EVENT */
    MIPC_CALL_EVENT_IND_T_EVENT                             = 0x100,
    /* the reason of call reject (when EVENT=MT_REJECT) */
    /* type = uint32_t, refer to CALL_REJECT_REASON */
    MIPC_CALL_EVENT_IND_T_REJECT_REASON                     = 0x10B,
    /* the handover information (when EVENT=SRVCC) */
    /* type = uint32_t */
    MIPC_CALL_EVENT_IND_T_SRVCCH                            = 0x115,
    /* the handover information (when EVENT=MT_REDIRECT) */
    /* type = string */
    MIPC_CALL_EVENT_IND_T_REDIRECT_NUMBER                   = 0x811F,
    /* the current voice codec (when EVENT=AUDIO_CODEC_CHANGE) */
    /* type = uint16_t, refer to CALL_AUDIO_CODEC */
    MIPC_CALL_EVENT_IND_T_AUDIO_CODEC                       = 0x129,
    /* the current voice codec (when EVENT=SPEECH_ATTACH) */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_EVENT_IND_T_SPEECH_ON                         = 0x133,
    /* the current voice codec (when EVENT=SPEECH_ATTACH) */
    /* type = uint32_t, refer to SPEECH_RAT */
    MIPC_CALL_EVENT_IND_T_SPEECH_RAT                        = 0x134,
    /* the current voice codec (when EVENT=SPEECH_ATTACH) */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_EVENT_IND_T_SPEECH_IRHO_ON                    = 0x135,
    /* the current call rat (when EVENT=RAT) */
    /* type = uint8_t, refer to CALL_RAT */
    MIPC_CALL_EVENT_IND_T_RAT                               = 0x13C,
    /* the call num (when EVENT=ECONFSRVCC) */
    /* type = uint32_t */
    MIPC_CALL_EVENT_IND_T_COUNT                             = 0x15B,
    /* the call id (when EVENT=ECONFSRVCC) */
    /* type = uint32_t */
    MIPC_CALL_EVENT_IND_T_CALL_ID                           = 0x15C,
};

    /* MIPC_MSG.CALL_MODE_IND */
enum mipc_call_mode_ind_tlv_enum {
    mipc_call_mode_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_MODE_IND_T_CALLID                             = 0x100,
    /* type = uint32_t, refer to CALL_MODE */
    MIPC_CALL_MODE_IND_T_MODE                               = 0x101,
    /* type = uint32_t, refer to SDP_DIRECTION */
    MIPC_CALL_MODE_IND_T_SDP_CAMERA_DIRECTION               = 0x102,
    /* type = uint32_t, refer to SDP_DIRECTION */
    MIPC_CALL_MODE_IND_T_SDP_AUDIO_DIRECTION                = 0x103,
    /* type = uint32_t, refer to SDP_AUDIO_CODEC */
    MIPC_CALL_MODE_IND_T_SDP_AUDIO_CODEC                    = 0x104,
};

    /* MIPC_MSG.CALL_SIP_IND */
enum mipc_call_sip_ind_tlv_enum {
    mipc_call_sip_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_SIP_IND_T_CALLID                              = 0x100,
    /* type = uint32_t, refer to SIP_DIRECTION */
    MIPC_CALL_SIP_IND_T_DIRECTION                           = 0x101,
    /* type = uint32_t, refer to SIP_MSG_TYPE */
    MIPC_CALL_SIP_IND_T_MSG_TYPE                            = 0x102,
    /* It can used when MSG_TYPE=REQUEST or RESPONSE */
    /* type = uint32_t, refer to SIP_METHOD */
    MIPC_CALL_SIP_IND_T_METHOD                              = 0x103,
    /* only used when MSG_TYPE=RESPONSE */
    /* type = uint32_t */
    MIPC_CALL_SIP_IND_T_RESPONSE_CODE                       = 0x104,
    /* reason header field in SIP message */
    /* type = string */
    MIPC_CALL_SIP_IND_T_REASON_TEXT                         = 0x105,
};

    /* MIPC_MSG.CALL_CONFERENCE_IND */
enum mipc_call_conference_ind_tlv_enum {
    mipc_call_conference_ind_tlv_NONE = 0,
    /* the conference cal id */
    /* type = uint32_t */
    MIPC_CALL_CONFERENCE_IND_T_CONF_CALLID                  = 0x100,
    /* type = uint32_t */
    MIPC_CALL_CONFERENCE_IND_T_COUNT                        = 0x101,
    /* type = uint32_t, refer to CALL_DIRECTION */
    MIPC_CALL_CONFERENCE_IND_T_DIRECTION                    = 0x10B,
    /* call number */
    /* type = string */
    MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_NUMBER           = 0x810C,
    /* type = string */
    MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_NAME             = 0x810D,
    /* type = uint32_t, refer to CONF_PARTICIPANT_STATUS */
    MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_STATUS           = 0x10E,
    /* call user entity string */
    /* type = string */
    MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_USER_ENTITY      = 0x810F,
    /* call endpoint entity string */
    /* type = string */
    MIPC_CALL_CONFERENCE_IND_T_PARTICIPANT_ENDPOINT_ENTITY  = 0x8110,
};

    /* MIPC_MSG.CALL_IMS_EVENT_PACKAGE_IND */
enum mipc_call_ims_event_package_ind_tlv_enum {
    mipc_call_ims_event_package_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_CALLID                = 0x100,
    /* type = uint32_t, refer to IMS_EVENT_PACKAGE_TYPE */
    MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_TYPE                  = 0x101,
    /* type = string */
    MIPC_CALL_IMS_EVENT_PACKAGE_IND_T_DATA                  = 0x8102,
};

    /* MIPC_MSG.CALL_ECC_LIST_CHANGE_IND */
enum mipc_call_ecc_list_change_ind_tlv_enum {
    mipc_call_ecc_list_change_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.CALL_SS_IND */
enum mipc_call_ss_ind_tlv_enum {
    mipc_call_ss_ind_tlv_NONE = 0,
    /* it is manufacturer specific, which of these codes are supported */
    /* type = uint32_t, refer to CALL_SS_CODE1 */
    MIPC_CALL_SS_IND_T_CODE1                                = 0x100,
    /* it is manufacturer specific, which of these codes are supported */
    /* type = uint32_t, refer to CALL_SS_CODE2 */
    MIPC_CALL_SS_IND_T_CODE2                                = 0x101,
    /* it is manufacturer specific, which of these codes are supported */
    /* type = uint32_t */
    MIPC_CALL_SS_IND_T_INDEX                                = 0x102,
    /* type = string */
    MIPC_CALL_SS_IND_T_NUMBER                               = 0x8103,
    /* type of address (refer TS 24.008 [8] subclause 10.5.4.7) */
    /* type = uint32_t */
    MIPC_CALL_SS_IND_T_TOA                                  = 0x104,
    /* string type subaddress of format specified by SATYPE */
    /* type = string */
    MIPC_CALL_SS_IND_T_SUBADDR                              = 0x105,
    /* type of subaddress octet in integer format (refer TS 24.008 [8] subclause 10.5.4.8) */
    /* type = uint32_t */
    MIPC_CALL_SS_IND_T_SATYPE                               = 0x106,
    /* Supplementary service notification, URC: +ECMCCSS */
    /* type = string */
    MIPC_CALL_SS_IND_T_RAW_STRING                           = 0x8107,
};

    /* MIPC_MSG.CALL_ECBM_CHANGE_IND */
enum mipc_call_ecbm_change_ind_tlv_enum {
    mipc_call_ecbm_change_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to CALL_ECBM_MODE */
    MIPC_CALL_ECBM_CHANGE_IND_T_MODE                        = 0x100,
};

    /* MIPC_MSG.CALL_CRSS_IND */
enum mipc_call_crss_ind_tlv_enum {
    mipc_call_crss_ind_tlv_NONE = 0,
    /* type of CRSS IND */
    /* type = uint8_t, refer to CRSS_TYPE */
    MIPC_CALL_CRSS_IND_T_CRSS_TYPE                          = 0x100,
    /* string type phone number of format specified by <type> */
    /* type = string */
    MIPC_CALL_CRSS_IND_T_NUMBER                             = 0x8101,
    /* type of address octet in integer format (refer TS 24.008 [8] subclause 10.5.4.7) */
    /* type = uint8_t, refer to NUMBER_TYPE */
    MIPC_CALL_CRSS_IND_T_CALL_NUMBER_TYPE                   = 0x102,
    /* indicates number presentation */
    /* type = uint8_t, refer to NUMBER_PRESENTATION */
    MIPC_CALL_CRSS_IND_T_NUMBER_PRESENTATION                = 0x103,
    /* string type subaddress of format specified by <satype> */
    /* type = string */
    MIPC_CALL_CRSS_IND_T_SUB_ADDRESS                        = 0x8104,
    /* type of subaddress octet in integer format (refer TS 24.008 [8] subclause 10.5.4.8) */
    /* type = uint8_t */
    MIPC_CALL_CRSS_IND_T_SA_TYPE                            = 0x105,
    /* type = string */
    MIPC_CALL_CRSS_IND_T_ALPHAID                            = 0x8106,
    /* string type caller name */
    /* type = string */
    MIPC_CALL_CRSS_IND_T_NAME                               = 0x8107,
};

    /* MIPC_MSG.CALL_ECT_IND */
enum mipc_call_ect_ind_tlv_enum {
    mipc_call_ect_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_ECT_IND_T_CALL_ID                             = 0x100,
    /* FALSE: fail; TRUE: success */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_ECT_IND_T_ECT_RESULT                          = 0x101,
    /* type = uint32_t */
    MIPC_CALL_ECT_IND_T_CAUSE                               = 0x102,
};

    /* MIPC_MSG.CALL_CIPHER_IND */
enum mipc_call_cipher_ind_tlv_enum {
    mipc_call_cipher_ind_tlv_NONE = 0,
    /* FALSE: do not show cipher indication; TRUE:show cipher indication */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_CIPHER_IND_T_SIM_CIPHER_IND                   = 0x100,
    /* FALSE: no connection; TRUE:has connection */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_CIPHER_IND_T_MM_CONNECTION                    = 0x101,
    /* type = uint8_t, refer to CALL_CIPHER_ON_STATUS */
    MIPC_CALL_CIPHER_IND_T_CS_CIPHER_ON                     = 0x102,
    /* type = uint8_t, refer to CALL_CIPHER_ON_STATUS */
    MIPC_CALL_CIPHER_IND_T_PS_CIPHER_ON                     = 0x103,
};

    /* MIPC_MSG.CALL_RTT_AUDIO_IND */
enum mipc_call_rtt_audio_ind_tlv_enum {
    mipc_call_rtt_audio_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_RTT_AUDIO_IND_T_CALL_ID                       = 0x100,
    /* type = uint8_t, refer to CALL_RTT_AUDIO_TYPE */
    MIPC_CALL_RTT_AUDIO_IND_T_TYPE                          = 0x101,
};

    /* MIPC_MSG.CALL_RTT_CAPABILITY_IND */
enum mipc_call_rtt_capability_ind_tlv_enum {
    mipc_call_rtt_capability_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_RTT_CAPABILITY_IND_T_CALL_ID                  = 0x100,
    /* UE RTT capability, 1 for RTT enable, 0 for RTT disable */
    /* type = uint8_t */
    MIPC_CALL_RTT_CAPABILITY_IND_T_LOCAL_TEXT_CAPABILITY    = 0x101,
    /* negotiated remote SDP text port status, 1 for non-zero negotiated remote port, 0 for zero negotiated remote port. */
    /* type = uint8_t */
    MIPC_CALL_RTT_CAPABILITY_IND_T_REMOTE_TEXT_CAPABILITY   = 0x102,
    /* negotiated local SDP text port status, 1 for non-zero negotiated local port, 0 for zero negotiated local port */
    /* type = uint8_t */
    MIPC_CALL_RTT_CAPABILITY_IND_T_LOCAL_TEXT_STATUS        = 0x103,
    /* if having 'text' in the Contact header of the remote SIP response, this is reported to 1. If not, this is reported to 0 */
    /* type = uint8_t */
    MIPC_CALL_RTT_CAPABILITY_IND_T_REAL_REMOTE_TEXT_CAPABILITY = 0x104,
};

    /* MIPC_MSG.CALL_LOCAL_RTT_MODIFY_RESULT_IND */
enum mipc_call_local_rtt_modify_result_ind_tlv_enum {
    mipc_call_local_rtt_modify_result_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_LOCAL_RTT_MODIFY_RESULT_IND_T_CALL_ID         = 0x100,
    /* 0:success; 1:fail */
    /* type = uint8_t */
    MIPC_CALL_LOCAL_RTT_MODIFY_RESULT_IND_T_RESULT          = 0x101,
};

    /* MIPC_MSG.CALL_PEER_RTT_MODIFY_RESULT_IND */
enum mipc_call_peer_rtt_modify_result_ind_tlv_enum {
    mipc_call_peer_rtt_modify_result_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND_T_CALL_ID          = 0x100,
    /* 0: peer request RTT downgrade,1: peer request RTT upgrade */
    /* type = uint8_t, refer to CALL_LOCAL_RTT_MODIFY_OP */
    MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND_T_OP               = 0x101,
};

    /* MIPC_MSG.CALL_RTT_TEXT_RECEIVE_IND */
enum mipc_call_rtt_text_receive_ind_tlv_enum {
    mipc_call_rtt_text_receive_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_CALL_ID                = 0x100,
    /* type = uint32_t */
    MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_LEN                    = 0x101,
    /* type = string */
    MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_TEXT                   = 0x102,
    /* M bit in RTP */
    /* type = uint8_t */
    MIPC_CALL_RTT_TEXT_RECEIVE_IND_T_M_BIT                  = 0x103,
};

    /* MIPC_MSG.CALL_RCS_DIGITS_LINE_IND */
enum mipc_call_rcs_digits_line_ind_tlv_enum {
    mipc_call_rcs_digits_line_ind_tlv_NONE = 0,
    /* type = string */
    MIPC_CALL_RCS_DIGITS_LINE_IND_T_DIGITS_LINE             = 0x100,
};

    /* MIPC_MSG.CALL_DISPLAY_AND_SIGNALS_INFO_IND */
enum mipc_call_display_and_signals_info_ind_tlv_enum {
    mipc_call_display_and_signals_info_ind_tlv_NONE = 0,
    /* type = string */
    MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_DISPLAY        = 0x8100,
    /* type = uint32_t */
    MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_SIGNAL_TYPE    = 0x101,
    /* type = uint32_t */
    MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_ALERT_PITCH    = 0x102,
    /* type = uint32_t */
    MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND_T_SIGNAL         = 0x103,
};

    /* MIPC_MSG.CALL_EXTENDED_DISPLAY_INFO_IND */
enum mipc_call_extended_display_info_ind_tlv_enum {
    mipc_call_extended_display_info_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_EXTENDED_DISPLAY_INFO_IND_T_DISPLAY_TAG       = 0x100,
    /* type = string */
    MIPC_CALL_EXTENDED_DISPLAY_INFO_IND_T_INFO              = 0x8101,
};

    /* MIPC_MSG.CALL_LINE_CONTROL_INFO_IND */
enum mipc_call_line_control_info_ind_tlv_enum {
    mipc_call_line_control_info_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_LINE_CONTROL_INFO_IND_T_POLARITY_INCLUDED     = 0x100,
    /* type = uint32_t */
    MIPC_CALL_LINE_CONTROL_INFO_IND_T_TOGGLE_MODE           = 0x101,
    /* type = uint32_t */
    MIPC_CALL_LINE_CONTROL_INFO_IND_T_REVERSE_POLARITY      = 0x102,
    /* type = uint32_t */
    MIPC_CALL_LINE_CONTROL_INFO_IND_T_POWER_DENIAL_TIME     = 0x103,
};

    /* MIPC_MSG.CALL_REDIRECTING_NUMBER_INFO_IND */
enum mipc_call_redirecting_number_info_ind_tlv_enum {
    mipc_call_redirecting_number_info_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_EXT_BIT_1       = 0x100,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER_TYPE     = 0x101,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER_PLAN     = 0x102,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_EXT_BIT_2       = 0x103,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_PI              = 0x104,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_SI              = 0x105,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_EXT_BIT_3       = 0x106,
    /* type = uint32_t */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_REDIRECTION_REASON = 0x107,
    /* type = string */
    MIPC_CALL_REDIRECTING_NUMBER_INFO_IND_T_NUMBER          = 0x8108,
};

    /* MIPC_MSG.CALL_GWSD_EVENT_IND */
enum mipc_call_gwsd_event_ind_tlv_enum {
    mipc_call_gwsd_event_ind_tlv_NONE = 0,
    /* gwsd event */
    /* type = uint32_t, refer to GWSD_EVENT */
    MIPC_CALL_GWSD_EVENT_IND_T_EVENT                        = 0x100,
    /* the call id (when EVENT=ECRFCPY) */
    /* type = uint32_t */
    MIPC_CALL_GWSD_EVENT_IND_T_CALL_ID                      = 0x10B,
    /* the update status (when EVENT=ECRFCPY) */
    /* type = uint32_t */
    MIPC_CALL_GWSD_EVENT_IND_T_UPDATE_STATUS                = 0x10C,
    /* the ton (when EVENT=EICPGU) */
    /* type = uint32_t */
    MIPC_CALL_GWSD_EVENT_IND_T_TON                          = 0x115,
    /* the number (when EVENT=EICPGU) */
    /* type = string */
    MIPC_CALL_GWSD_EVENT_IND_T_NUMBER                       = 0x8116,
    /* the result (when EVENT=EICPGRES) */
    /* type = uint32_t */
    MIPC_CALL_GWSD_EVENT_IND_T_RESULT                       = 0x11F,
};

    /* MIPC_MSG.CALL_ECONF_IND */
enum mipc_call_econf_ind_tlv_enum {
    mipc_call_econf_ind_tlv_NONE = 0,
    /* the conference call id (when EVENT=ECONF) */
    /* type = uint32_t */
    MIPC_CALL_ECONF_IND_T_CONF_CALL_ID                      = 0x100,
    /* the operation: 0=add, 1=remove (when EVENT=ECONF) */
    /* type = uint32_t */
    MIPC_CALL_ECONF_IND_T_OPERATION                         = 0x101,
    /* the number (when EVENT=ECONF) */
    /* type = string */
    MIPC_CALL_ECONF_IND_T_NUMBER                            = 0x8102,
    /* the econf result (when EVENT=ECONF) */
    /* type = uint32_t */
    MIPC_CALL_ECONF_IND_T_RESULT                            = 0x103,
    /* the econf failed cause (when EVENT=ECONF) */
    /* type = uint32_t */
    MIPC_CALL_ECONF_IND_T_CAUSE                             = 0x104,
    /* the joined call id (when EVENT=ECONF) */
    /* type = uint32_t */
    MIPC_CALL_ECONF_IND_T_JOINED_CALL_ID                    = 0x105,
};

    /* MIPC_MSG.CALL_IMS_SIP_HEADER_IND */
enum mipc_call_ims_sip_header_ind_tlv_enum {
    mipc_call_ims_sip_header_ind_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_CALL_IMS_SIP_HEADER_IND_T_CALL_ID                  = 0x100,
    /* type = uint32_t */
    MIPC_CALL_IMS_SIP_HEADER_IND_T_HEADER_TYPE              = 0x101,
    /* type = uint32_t */
    MIPC_CALL_IMS_SIP_HEADER_IND_T_TOTAL_COUNT              = 0x102,
    /* type = uint32_t */
    MIPC_CALL_IMS_SIP_HEADER_IND_T_INDEX                    = 0x103,
    /* type = string */
    MIPC_CALL_IMS_SIP_HEADER_IND_T_VALUE                    = 0x8104,
};

    /* MIPC_MSG.CALL_ECC_REDIAL_IND */
enum mipc_call_ecc_redial_ind_tlv_enum {
    mipc_call_ecc_redial_ind_tlv_NONE = 0,
    /* 0 means request AP to leave flight mode and 30 means NW request to dial CS normal call, others means call id */
    /* type = uint8_t */
    MIPC_CALL_ECC_REDIAL_IND_T_CALL_ID                      = 0x100,
};

    /* MIPC_MSG.CALL_EMERGENCY_BEARER_SUPPORT_IND */
enum mipc_call_emergency_bearer_support_ind_tlv_enum {
    mipc_call_emergency_bearer_support_ind_tlv_NONE = 0,
    /* Emergency bearer services support indicator for Iu mode */
    /* type = uint8_t, refer to EMERGENCY_CALL_S1_SUPPORT */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_S1_SUPPORT     = 0x100,
    /* notify AP the emergency service support status in the current cell  */
    /* type = uint8_t, refer to EMERGENCY_CALL_RAT */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_RAT            = 0x101,
    /* notify AP the emergency service support status in the current cell */
    /* type = uint8_t, refer to EMERGENCY_CALL_SUPPORT_EMC */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_SUPPORT_EMC    = 0x102,
    /* notify AP the emergency bearer services support indicator for Iu mode  */
    /* type = uint8_t, refer to EMERGENCY_CALL_EMB_IU_SUPP */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMB_IU_SUPP    = 0x103,
    /* notify AP the emergency services support in 5GS for 3GPP access */
    /* type = uint8_t, refer to EMERGENCY_CALL_EMS_5G_SUPP */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMS_5G_SUPP    = 0x104,
    /* notify AP the emergency services support in 5GS for 3GPP access */
    /* type = uint8_t, refer to EMERGENCY_CALL_EMF_5G_SUPP */
    MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND_T_EMF_5G_SUPP    = 0x105,
};

    /* MIPC_MSG.CALL_UIS_INFO_IND */
enum mipc_call_uis_info_ind_tlv_enum {
    mipc_call_uis_info_ind_tlv_NONE = 0,
    /* call ID */
    /* type = uint32_t */
    MIPC_CALL_UIS_INFO_IND_T_CALLID                         = 0x100,
    /* Indicates the type of the UIS Info */
    /* type = uint8_t, refer to CALL_UIS_INFO_TYPE */
    MIPC_CALL_UIS_INFO_IND_T_TYPE                           = 0x101,
    /* Indicates the detailed UIS Info in string format */
    /* type = string */
    MIPC_CALL_UIS_INFO_IND_T_DATA                           = 0x102,
    /* Only used by URC, indicates the final result of the AT command */
    /* type = uint8_t, refer to CALL_UIS_INFO_RESULT */
    MIPC_CALL_UIS_INFO_IND_T_RESULT                         = 0x103,
    /* Only used by URC, indicates the detailed cause */
    /* type = uint32_t, refer to CALL_UIS_INFO_CAUSE */
    MIPC_CALL_UIS_INFO_IND_T_CAUSE                          = 0x104,
};

    /* MIPC_MSG.CALL_CALL_ADDITIONAL_INFO_IND */
enum mipc_call_call_additional_info_ind_tlv_enum {
    mipc_call_call_additional_info_ind_tlv_NONE = 0,
    /* call ID */
    /* type = uint32_t */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_CALLID             = 0x100,
    /* The call mode will be applied till next command to change call_mode. */
    /* type = uint8_t, refer to CALL_ADDITIONAL_INFO_MODE */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_MODE               = 0x101,
    /* type = uint8_t, refer to CALL_ADDITIONAL_INFO_TYPE */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_TYPE               = 0x102,
    /* total number of additinal info */
    /* type = uint32_t */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_TOTAL              = 0x103,
    /* index of current additional info */
    /* type = uint32_t */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_INDEX              = 0x104,
    /* number of key or key/value pair in additional info */
    /* type = uint32_t */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_COUNT              = 0x105,
    /* type = string */
    MIPC_CALL_CALL_ADDITIONAL_INFO_IND_T_ADDITIONAL_INFO    = 0x8106,
};

    /* MIPC_MSG.CALL_MT_SIP_INVITE_IND */
enum mipc_call_mt_sip_invite_ind_tlv_enum {
    mipc_call_mt_sip_invite_ind_tlv_NONE = 0,
    /* remote MT's phone number */
    /* type = string */
    MIPC_CALL_MT_SIP_INVITE_IND_T_FROM_NUMBER               = 0x8100,
    /* total count of this MT SIP INVITE will be reported */
    /* type = uint8_t */
    MIPC_CALL_MT_SIP_INVITE_IND_T_TOTAL_COUNT               = 0x101,
    /* nth part of the all MT SIP INVITE */
    /* type = uint8_t */
    MIPC_CALL_MT_SIP_INVITE_IND_T_INDEX                     = 0x102,
    /* MT SIP INVITE message */
    /* type = string */
    MIPC_CALL_MT_SIP_INVITE_IND_T_MT_SIP_INVITE             = 0x8103,
};

    /* MIPC_MSG.CALL_RECV_DTMF_IND */
enum mipc_call_recv_dtmf_ind_tlv_enum {
    mipc_call_recv_dtmf_ind_tlv_NONE = 0,
    /* DTMF START, STOP or STOP_ALL */
    /* type = uint32_t, refer to RECV_DTMF_MODE */
    MIPC_CALL_RECV_DTMF_IND_T_MODE                          = 0x100,
    /* this field is only necessary when mode=START or STOP */
    /* type = string */
    MIPC_CALL_RECV_DTMF_IND_T_DIGIT                         = 0x101,
};

    /* MIPC_MSG.CALL_DC_CONFIG_INIT_IND */
enum mipc_call_dc_config_init_ind_tlv_enum {
    mipc_call_dc_config_init_ind_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_CALL_ID                  = 0x100,
    /* used by AP DC manager to create IMS socket */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_IF_NAME                  = 0x101,
    /* The number of valid dc config */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_NUM_DC_CONFIG            = 0x102,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_INDEX                    = 0x103,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_INDEX_TLV_ARRAY          = 0x103,
    /* 0: UE to Server, 1: UE to UE */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_TYPE                  = 0x104,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_TYPE_TLV_ARRAY        = 0x104,
    /* 0: IPv4; 1: IPv6 */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_IP_ADDR_TYPE             = 0x105,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_IP_ADDR_TYPE_TLV_ARRAY   = 0x105,
    /* remote udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_UDP_PORT          = 0x106,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_UDP_PORT_TLV_ARRAY = 0x106,
    /* remote sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_SCTP_PORT         = 0x107,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_SCTP_PORT_TLV_ARRAY = 0x107,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_UDP_PORT           = 0x108,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_UDP_PORT_TLV_ARRAY = 0x108,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_SCTP_PORT          = 0x109,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_SCTP_PORT_TLV_ARRAY = 0x109,
    /* remote IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_ADDR              = 0x10A,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_ADDR_TLV_ARRAY    = 0x10A,
    /* local IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_ADDR               = 0x10B,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_ADDR_TLV_ARRAY     = 0x10B,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_DTLS_SETUP        = 0x10C,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_DTLS_SETUP_TLV_ARRAY = 0x10C,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_DTLS_SETUP         = 0x10D,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_DTLS_SETUP_TLV_ARRAY = 0x10D,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_TLS_ID            = 0x10E,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_TLS_ID_TLV_ARRAY  = 0x10E,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_TLS_ID             = 0x10F,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_TLS_ID_TLV_ARRAY   = 0x10F,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_FP_HASH_FUNC             = 0x110,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_FP_HASH_FUNC_TLV_ARRAY   = 0x110,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_FP_STR            = 0x111,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_FP_STR_TLV_ARRAY  = 0x111,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_FP_STR             = 0x112,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_FP_STR_TLV_ARRAY   = 0x112,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_MAX_MESSAGE_SIZE  = 0x113,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x113,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_MAX_MESSAGE_SIZE   = 0x114,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x114,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DATA_B_AS                = 0x115,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DATA_B_AS_TLV_ARRAY      = 0x115,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_QOS_HINT_TLV_ARRAY       = 0x116,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_QOS_HINT                 = 0x116,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_NUM_DCMAP                = 0x117,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_STREAM_ID          = 0x118,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_STREAM_ID_TLV_ARRAY = 0x118,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_PRIORITY           = 0x119,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_PRIORITY_TLV_ARRAY = 0x119,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_CONFIG_INDEX          = 0x11A,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_CONFIG_INDEX_TLV_ARRAY = 0x11A,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_ORDERED            = 0x11B,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_ORDERED_TLV_ARRAY  = 0x11B,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_RETR           = 0x11C,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_RETR_TLV_ARRAY = 0x11C,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_TIME           = 0x11D,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_TIME_TLV_ARRAY = 0x11D,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_LABEL              = 0x11E,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_LABEL_TLV_ARRAY    = 0x11E,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_SUBPROTOCOL        = 0x11F,
    MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_SUBPROTOCOL_TLV_ARRAY = 0x11F,
};

    /* MIPC_MSG.CALL_DC_CONFIG_UPDATE_IND */
enum mipc_call_dc_config_update_ind_tlv_enum {
    mipc_call_dc_config_update_ind_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_CALL_ID                = 0x100,
    /* used by AP DC manager to create IMS socket */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IF_NAME                = 0x101,
    /* The number of valid dc config */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_NUM_DC_CONFIG          = 0x102,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_INDEX                  = 0x103,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_INDEX_TLV_ARRAY        = 0x103,
    /* 0: UE to Server, 1: UE to UE */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_TYPE                = 0x104,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_TYPE_TLV_ARRAY      = 0x104,
    /* 0: IPv4; 1: IPv6 */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IP_ADDR_TYPE           = 0x105,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IP_ADDR_TYPE_TLV_ARRAY = 0x105,
    /* remote udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_UDP_PORT        = 0x106,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_UDP_PORT_TLV_ARRAY = 0x106,
    /* remote sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_SCTP_PORT       = 0x107,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_SCTP_PORT_TLV_ARRAY = 0x107,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_UDP_PORT         = 0x108,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_UDP_PORT_TLV_ARRAY = 0x108,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_SCTP_PORT        = 0x109,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_SCTP_PORT_TLV_ARRAY = 0x109,
    /* remote IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_ADDR            = 0x10A,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_ADDR_TLV_ARRAY  = 0x10A,
    /* local IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_ADDR             = 0x10B,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_ADDR_TLV_ARRAY   = 0x10B,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_DTLS_SETUP      = 0x10C,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_DTLS_SETUP_TLV_ARRAY = 0x10C,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_DTLS_SETUP       = 0x10D,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_DTLS_SETUP_TLV_ARRAY = 0x10D,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_TLS_ID          = 0x10E,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_TLS_ID_TLV_ARRAY = 0x10E,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_TLS_ID           = 0x10F,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_TLS_ID_TLV_ARRAY = 0x10F,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_FP_HASH_FUNC           = 0x110,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_FP_HASH_FUNC_TLV_ARRAY = 0x110,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_FP_STR          = 0x111,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_FP_STR_TLV_ARRAY = 0x111,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_FP_STR           = 0x112,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_FP_STR_TLV_ARRAY = 0x112,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_MAX_MESSAGE_SIZE = 0x113,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x113,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_MAX_MESSAGE_SIZE = 0x114,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x114,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DATA_B_AS              = 0x115,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DATA_B_AS_TLV_ARRAY    = 0x115,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_QOS_HINT_TLV_ARRAY     = 0x116,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_QOS_HINT               = 0x116,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_NUM_DCMAP              = 0x117,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_STREAM_ID        = 0x118,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_STREAM_ID_TLV_ARRAY = 0x118,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_PRIORITY         = 0x119,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_PRIORITY_TLV_ARRAY = 0x119,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_CONFIG_INDEX        = 0x11A,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_CONFIG_INDEX_TLV_ARRAY = 0x11A,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_ORDERED          = 0x11B,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_ORDERED_TLV_ARRAY = 0x11B,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_RETR         = 0x11C,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_RETR_TLV_ARRAY = 0x11C,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_TIME         = 0x11D,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_TIME_TLV_ARRAY = 0x11D,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_LABEL            = 0x11E,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_LABEL_TLV_ARRAY  = 0x11E,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_SUBPROTOCOL      = 0x11F,
    MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_SUBPROTOCOL_TLV_ARRAY = 0x11F,
};

    /* MIPC_MSG.CALL_DC_CONFIG_DEINIT_IND */
enum mipc_call_dc_config_deinit_ind_tlv_enum {
    mipc_call_dc_config_deinit_ind_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CONFIG_DEINIT_IND_T_CALL_ID                = 0x100,
};

    /* MIPC_MSG.CALL_CONNECT_DATA_CHANNEL_IND */
enum mipc_call_connect_data_channel_ind_tlv_enum {
    mipc_call_connect_data_channel_ind_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CONNECT_DATA_CHANNEL_IND_T_CALL_ID            = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CONNECT_DATA_CHANNEL_IND_T_DATA               = 0x101,
};

    /* MIPC_MSG.CALL_CLOSE_DATA_CHANNEL_IND */
enum mipc_call_close_data_channel_ind_tlv_enum {
    mipc_call_close_data_channel_ind_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CLOSE_DATA_CHANNEL_IND_T_CALL_ID              = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CLOSE_DATA_CHANNEL_IND_T_DATA                 = 0x101,
};

    /* MIPC_MSG.CALL_CREATE_DATA_CHANNEL_IND */
enum mipc_call_create_data_channel_ind_tlv_enum {
    mipc_call_create_data_channel_ind_tlv_NONE = 0,
    /* call id */
    /* type = uint32_t */
    MIPC_CALL_CREATE_DATA_CHANNEL_IND_T_CALL_ID             = 0x100,
    /* Data Channel parameters as xml format */
    /* type = string */
    MIPC_CALL_CREATE_DATA_CHANNEL_IND_T_DATA                = 0x101,
};

    /* MIPC_MSG.CALL_APPROVE_INCOMING_CMD */
enum mipc_call_approve_incoming_cmd_tlv_enum {
    mipc_call_approve_incoming_cmd_tlv_NONE = 0,
    /* call ID */
    /* type = uint32_t */
    MIPC_CALL_APPROVE_INCOMING_CMD_T_CALLID                 = 0x100,
    /* type = string */
    MIPC_CALL_APPROVE_INCOMING_CMD_T_NUMBER                 = 0x8101,
    /* type of address (refer TS 24.008 [8] subclause 10.5.4.7) */
    /* type = uint32_t */
    MIPC_CALL_APPROVE_INCOMING_CMD_T_TOA                    = 0x102,
    /* sequence number which is piggybacked from the incoming call */
    /* type = uint32_t */
    MIPC_CALL_APPROVE_INCOMING_CMD_T_SEQ_NO                 = 0x103,
    /* type = uint32_t, refer to CALL_MODE */
    MIPC_CALL_APPROVE_INCOMING_CMD_T_MODE                   = 0x104,
    /* type = uint8_t */
    MIPC_CALL_APPROVE_INCOMING_CMD_T_EVOLTESI_FLOW          = 0x105,
};

    /* MIPC_MSG.CALL_APPROVE_INCOMING_RSP */
enum mipc_call_approve_incoming_rsp_tlv_enum {
    mipc_call_approve_incoming_rsp_tlv_NONE = 0,
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_CALL_APPROVE_INCOMING_RSP_T_IS_APPROVE             = 0x100,
    /* type = uint32_t */
    MIPC_CALL_APPROVE_INCOMING_RSP_T_CAUSE                  = 0x101,
    /* call ID */
    /* type = uint32_t */
    MIPC_CALL_APPROVE_INCOMING_RSP_T_CALLID                 = 0x102,
    /* sequence number which is piggybacked from the incoming call */
    /* type = uint32_t */
    MIPC_CALL_APPROVE_INCOMING_RSP_T_SEQ_NO                 = 0x103,
};

    /* MIPC_MSG.CALL_DC_BOOTSTRAP_CFG_QRY_CMD */
enum mipc_call_dc_bootstrap_cfg_qry_cmd_tlv_enum {
    mipc_call_dc_bootstrap_cfg_qry_cmd_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD_T_CALL_ID            = 0x100,
    /* Operator's ID (SBP ID) */
    /* type = uint16_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD_T_OP_ID              = 0x101,
};

    /* MIPC_MSG.CALL_DC_BOOTSTRAP_CFG_QRY_RSP */
enum mipc_call_dc_bootstrap_cfg_qry_rsp_tlv_enum {
    mipc_call_dc_bootstrap_cfg_qry_rsp_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_CALL_ID            = 0x100,
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_RESULT             = 0x101,
    /* The number of valid dc config local */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DC_CONFIG_LOCAL = 0x102,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_UDP_PORT     = 0x103,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_UDP_PORT_TLV_ARRAY = 0x103,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_SCTP_PORT    = 0x104,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_SCTP_PORT_TLV_ARRAY = 0x104,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_INDEX              = 0x105,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_INDEX_TLV_ARRAY    = 0x105,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it's passive */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_DTLS_SETUP   = 0x106,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_DTLS_SETUP_TLV_ARRAY = 0x106,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_TLS_ID       = 0x107,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_TLS_ID_TLV_ARRAY = 0x107,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_FP_HASH_FUNC       = 0x108,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_FP_HASH_FUNC_TLV_ARRAY = 0x108,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_FP_STR       = 0x109,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_FP_STR_TLV_ARRAY = 0x109,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_MAX_MESSAGE_SIZE = 0x10A,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x10A,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DATA_B_AS          = 0x10B,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DATA_B_AS_TLV_ARRAY = 0x10B,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_QOS_HINT           = 0x10C,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_QOS_HINT_TLV_ARRAY = 0x10C,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DCMAP          = 0x10D,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_STREAM_ID    = 0x10E,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_STREAM_ID_TLV_ARRAY = 0x10E,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_PRIORITY     = 0x10F,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_PRIORITY_TLV_ARRAY = 0x10F,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DC_CONFIG_INDEX    = 0x110,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DC_CONFIG_INDEX_TLV_ARRAY = 0x110,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_ORDERED      = 0x111,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_ORDERED_TLV_ARRAY = 0x111,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_MAX_RETR     = 0x112,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_MAX_RETR_TLV_ARRAY = 0x112,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_MAX_TIME     = 0x113,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_MAX_TIME_TLV_ARRAY = 0x113,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_LABEL        = 0x114,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_LABEL_TLV_ARRAY = 0x114,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_SUBPROTOCOL  = 0x115,
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_SUBPROTOCOL_TLV_ARRAY = 0x115,
};

    /* MIPC_MSG.CALL_DC_CREATE_CMD */
enum mipc_call_dc_create_cmd_tlv_enum {
    mipc_call_dc_create_cmd_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_CALL_ID                       = 0x100,
    /* The number of valid dc config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_NUM_DC_CONFIG                 = 0x101,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_INDEX                         = 0x102,
    MIPC_CALL_DC_CREATE_CMD_T_INDEX_TLV_ARRAY               = 0x102,
    /* 0: UE to Server, 1: UE to UE */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_DC_TYPE                       = 0x103,
    MIPC_CALL_DC_CREATE_CMD_T_DC_TYPE_TLV_ARRAY             = 0x103,
    /* 0: IPv4; 1: IPv6 */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_IP_ADDR_TYPE                  = 0x104,
    MIPC_CALL_DC_CREATE_CMD_T_IP_ADDR_TYPE_TLV_ARRAY        = 0x104,
    /* remote udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_UDP_PORT               = 0x105,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_UDP_PORT_TLV_ARRAY     = 0x105,
    /* remote sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_SCTP_PORT              = 0x106,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_SCTP_PORT_TLV_ARRAY    = 0x106,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_UDP_PORT                = 0x107,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_UDP_PORT_TLV_ARRAY      = 0x107,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_SCTP_PORT               = 0x108,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_SCTP_PORT_TLV_ARRAY     = 0x108,
    /* remote IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_ADDR                   = 0x109,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_ADDR_TLV_ARRAY         = 0x109,
    /* local IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_ADDR                    = 0x10A,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_ADDR_TLV_ARRAY          = 0x10A,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_DTLS_SETUP             = 0x10B,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_DTLS_SETUP_TLV_ARRAY   = 0x10B,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_DTLS_SETUP              = 0x10C,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_DTLS_SETUP_TLV_ARRAY    = 0x10C,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_TLS_ID                 = 0x10D,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_TLS_ID_TLV_ARRAY       = 0x10D,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_TLS_ID                  = 0x10E,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_TLS_ID_TLV_ARRAY        = 0x10E,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_FP_HASH_FUNC                  = 0x10F,
    MIPC_CALL_DC_CREATE_CMD_T_FP_HASH_FUNC_TLV_ARRAY        = 0x10F,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_FP_STR                 = 0x110,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_FP_STR_TLV_ARRAY       = 0x110,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_FP_STR                  = 0x111,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_FP_STR_TLV_ARRAY        = 0x111,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_MAX_MESSAGE_SIZE       = 0x112,
    MIPC_CALL_DC_CREATE_CMD_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x112,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_MAX_MESSAGE_SIZE        = 0x113,
    MIPC_CALL_DC_CREATE_CMD_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x113,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_CMD_T_DATA_B_AS                     = 0x114,
    MIPC_CALL_DC_CREATE_CMD_T_DATA_B_AS_TLV_ARRAY           = 0x114,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_QOS_HINT_TLV_ARRAY            = 0x115,
    MIPC_CALL_DC_CREATE_CMD_T_QOS_HINT                      = 0x115,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_NUM_DCMAP                     = 0x116,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_STREAM_ID               = 0x117,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_STREAM_ID_TLV_ARRAY     = 0x117,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_PRIORITY                = 0x118,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_PRIORITY_TLV_ARRAY      = 0x118,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_DC_CONFIG_INDEX               = 0x119,
    MIPC_CALL_DC_CREATE_CMD_T_DC_CONFIG_INDEX_TLV_ARRAY     = 0x119,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_ORDERED                 = 0x11A,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_ORDERED_TLV_ARRAY       = 0x11A,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_RETR                = 0x11B,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_RETR_TLV_ARRAY      = 0x11B,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_TIME                = 0x11C,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_TIME_TLV_ARRAY      = 0x11C,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_LABEL                   = 0x11D,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_LABEL_TLV_ARRAY         = 0x11D,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_SUBPROTOCOL             = 0x11E,
    MIPC_CALL_DC_CREATE_CMD_T_DCMAP_SUBPROTOCOL_TLV_ARRAY   = 0x11E,
};

    /* MIPC_MSG.CALL_DC_CREATE_RSP */
enum mipc_call_dc_create_rsp_tlv_enum {
    mipc_call_dc_create_rsp_tlv_NONE = 0,
    /* identify the related call session */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_CALL_ID                       = 0x100,
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_RESULT                        = 0x101,
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_CAUSE                         = 0x102,
    /* The number of valid dc config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_NUM_DC_CONFIG                 = 0x103,
    /* index of the config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_INDEX                         = 0x104,
    MIPC_CALL_DC_CREATE_RSP_T_INDEX_TLV_ARRAY               = 0x104,
    /* 0: UE to Server, 1: UE to UE */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_DC_TYPE                       = 0x105,
    MIPC_CALL_DC_CREATE_RSP_T_DC_TYPE_TLV_ARRAY             = 0x105,
    /* 0: IPv4; 1: IPv6 */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_IP_ADDR_TYPE                  = 0x106,
    MIPC_CALL_DC_CREATE_RSP_T_IP_ADDR_TYPE_TLV_ARRAY        = 0x106,
    /* remote udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_UDP_PORT               = 0x107,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_UDP_PORT_TLV_ARRAY     = 0x107,
    /* remote sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_SCTP_PORT              = 0x108,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_SCTP_PORT_TLV_ARRAY    = 0x108,
    /* local udp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_UDP_PORT                = 0x109,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_UDP_PORT_TLV_ARRAY      = 0x109,
    /* local sctp port */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_SCTP_PORT               = 0x10A,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_SCTP_PORT_TLV_ARRAY     = 0x10A,
    /* remote IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_ADDR                   = 0x10B,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_ADDR_TLV_ARRAY         = 0x10B,
    /* local IP address in binary */
    /* type = byte_array */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_ADDR                    = 0x10C,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_ADDR_TLV_ARRAY          = 0x10C,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_DTLS_SETUP             = 0x10D,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_DTLS_SETUP_TLV_ARRAY   = 0x10D,
    /* Indicates the role when creating DTLS association, usually in SDP offer it's activepass or active, in SDP answer it'ps passive */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_DTLS_SETUP              = 0x10E,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_DTLS_SETUP_TLV_ARRAY    = 0x10E,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_TLS_ID                 = 0x10F,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_TLS_ID_TLV_ARRAY       = 0x10F,
    /* DTLS association ID, tls-id-value = 20*(tls-id-char) */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_TLS_ID                  = 0x110,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_TLS_ID_TLV_ARRAY        = 0x110,
    /* Could be sha-256, sha-384, sha-512, normally we use sha-256 */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_FP_HASH_FUNC                  = 0x111,
    MIPC_CALL_DC_CREATE_RSP_T_FP_HASH_FUNC_TLV_ARRAY        = 0x111,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_FP_STR                 = 0x112,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_FP_STR_TLV_ARRAY       = 0x112,
    /* Secure Hash Standard (SHS) */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_FP_STR                  = 0x113,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_FP_STR_TLV_ARRAY        = 0x113,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_MAX_MESSAGE_SIZE       = 0x114,
    MIPC_CALL_DC_CREATE_RSP_T_REMOTE_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x114,
    /* The maximum SCTP size peer could receive according to SDP negotiation */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_MAX_MESSAGE_SIZE        = 0x115,
    MIPC_CALL_DC_CREATE_RSP_T_LOCAL_MAX_MESSAGE_SIZE_TLV_ARRAY = 0x115,
    /* Bandwidth for this DC stream */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_RSP_T_DATA_B_AS                     = 0x116,
    MIPC_CALL_DC_CREATE_RSP_T_DATA_B_AS_TLV_ARRAY           = 0x116,
    /* e.g.: loss=0.000002;latency=1000 */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_QOS_HINT_TLV_ARRAY            = 0x117,
    MIPC_CALL_DC_CREATE_RSP_T_QOS_HINT                      = 0x117,
    /* The number of valid dcmap elements */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_NUM_DCMAP                     = 0x118,
    /* The dcmap-stream-id parameter indicates the SCTP stream identifier within the SCTP association used to form the data channel */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_STREAM_ID               = 0x119,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_STREAM_ID_TLV_ARRAY     = 0x119,
    /* The priority parameter indicates the data channel's priority relative to the priorities of other data channels, which may additionally exist over the same SCTP association.  */
    /* type = uint16_t */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_PRIORITY                = 0x11A,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_PRIORITY_TLV_ARRAY      = 0x11A,
    /* mapping to related DC config */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_DC_CONFIG_INDEX               = 0x11B,
    MIPC_CALL_DC_CREATE_RSP_T_DC_CONFIG_INDEX_TLV_ARRAY     = 0x11B,
    /* The ordered parameter with value true indicates that the receiver will dispatch DATA chunks in the data channel to the upper layer while preserving the order. The ordered parameter is optional and takes two values: true for ordered delivery and false for unordered delivery,with true as the default value. */
    /* type = uint8_t */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_ORDERED                 = 0x11C,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_ORDERED_TLV_ARRAY       = 0x11C,
    /* This parameter indicates that the data channel is partially reliable. The max-retr parameter indicates the maximal number of times a user message will be retransmitted. */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_MAX_RETR                = 0x11D,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_MAX_RETR_TLV_ARRAY      = 0x11D,
    /* This parameter indicates that the data channel is partially reliable. A user message will no longer be transmitted or retransmitted after a specified lifetime, given in milliseconds, in the max-time parameter. */
    /* type = uint32_t */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_MAX_TIME                = 0x11E,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_MAX_TIME_TLV_ARRAY      = 0x11E,
    /* The label parameter indicates the name of the channel. It represents a label that can be used to distinguish, in the context of the WebRTC API. */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_LABEL                   = 0x11F,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_LABEL_TLV_ARRAY         = 0x11F,
    /* The subprotocol parameter indicates which protocol the client expects to exchange via the channel */
    /* type = string */
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_SUBPROTOCOL             = 0x120,
    MIPC_CALL_DC_CREATE_RSP_T_DCMAP_SUBPROTOCOL_TLV_ARRAY   = 0x120,
};


#endif /* __MIPC_MSG_CALL_CONST_H__ */
