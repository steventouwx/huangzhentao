#ifndef RMM_RCS_STACK_CHANNEL_H
#define RMM_RCS_STACK_CHANNEL_H

#include <inttypes.h>

namespace vendor::rmm::rcs {
    class RcsStackChannel {
        public:
            // not using sizeof to avoid padding in 64bits platform.
            // TODO: is there any better way to avoid hard-coded.
            static const int DATA_HEAD_LEN = 5 * sizeof(int32_t);
            static const int MAX_SIM_SUPPORTED = 2;

            static const int MAX_ADDRESS_LEN = 64;
            static const int MAX_IMPU_LEN = 256;
            static const int MAX_DOMAIN_LEN = 256;
            static const int MAX_ADDRESS_LIST_LEN = 256;
            static const int MAX_IMEI_LEN = 20;
            static const int MAX_URI_LEN = 128;
            static const int MAX_ASSOCIATED_URI = (MAX_URI_LEN << 2);
            static const int MAX_GRUU_LEN = 128;
            static const int MAX_SERVICE_ROUTE_LEN = 256;
            static const int MAX_TIMESTAMP_LEN = 256;
            static const int MAX_TIME_STAMP_LEN = 32;
            static const int MAX_SECURIT_VERIFY_LEN = 768;
            static const int MAX_UA_V1_LEN = 128;
            static const int MAX_UA_V2_LEN = 256;
            static const int MAX_PANI_LEN = 256;
            static const int MAX_PIDENTIFIER_LEN = 256;
            static const int MAX_PPA_LEN = 4096;

            typedef struct {
                int32_t     reqId;
                int32_t     dataLen;
                int32_t     flag;
                int32_t     account;
                int32_t     type;
                uint8_t     data[0];
            } ChannelData;

            typedef struct {
                int32_t uaType;               /* VoLTE_Stack_UA_Type_e */
                uint32_t connInfo;
            } ChannelInfo;

            typedef struct {
                int32_t connId;
                int32_t reqId;
                int32_t length;
                uint8_t data[0];
            } SipInfo;

            typedef struct {
                int32_t lat[2];             // [0..8388607], 23 bits field
                int32_t lng[2];             // [-8388608..8388607], 24 bits field
                int32_t acc;
                uint8_t timestamp[32];      // VOLTE_MAX_TIME_STAMP_LENGTH =32
            } LbsLocation;

            typedef struct {
                int32_t account;                //< the account id
                uint8_t cellId[64];             // VOLTE_MAX_CELL_ID_LENGTH = 64
                uint8_t callId[64];             // VOLTE_MAX_CALL_ID_LENGTH = 64
                LbsLocation locationInfo;
                uint8_t nextPcscf;              //< Try next pcscf for Register
            } RegItem;

            typedef struct {
                uint32_t account;               //< the account id
                uint8_t type;                   //< UA type
                uint8_t state;                  //< UA state; 0->disable, 1->enable
            } UaState;

            typedef struct {
                int32_t code;
                uint32_t userData[4];
            } RequestResult;

            typedef struct {
                uint32_t id;
            } DeregStartNotify;

            enum RegState {
                STATE_REGISTERED        = 1,
                STATE_UNREGISTERED      = 2,
                STATE_REGISTERING       = 3,
                STATE_DEREGISTERING     = 4,
                STATE_DISCONNECTED      = 5,
                STATE_AUTHENTICATING    = 6,
                STATE_OOS               = 7,
                STATE_CONNECTING        = 8
            };

            enum RegCause {
                CAUSE_NORMAL            = 0,
                CAUSE_INTERNAL_ERROR    = 1,
                CAUSE_STACK_ERROR       = 2,
                CAUSE_TIMER_ERROR       = 3,
                CAUSE_TRY_NEXT_PCSCF    = 4,
                CAUSE_PCSCF_ALL_FAILED  = 5,
                CAUSE_AUTH_ERROR        = 6,
                CAUSE_RESET             = 7,
                CAUSE_ASSURI_CHANGED    = 8,
                CAUSE_NETWORK_INITIATED = 10,
                CAUSE_CHANNEL_BIND      = 11,
                CAUSE_OOS               = 12,
                CAUSE_SIP_ERROR         = 13,
                CAUSE_PARAMETER_ERROR   = 14,
                CAUSE_NOT_BIND          = 15,
                CAUSE_NOT_AUTO_RE_REG   = 16,
                CAUSE_RETRY_AFTER       = 17,
                CAUSE_IMS_PDN_FATAL_FAIL= 18,
                CAUSE_MD_LOWER_LAYER_ERR= 19,
                CAUSE_C2K_FAIL          = 20,
                CAUSE_RETRY_BY_RFC5626  = 21,
                CAUSE_UNKNOWN           = 99
            };

            typedef struct {
                int32_t     id;                                         ///< the account id
                int32_t     state;                                      ///< refer to ::VoLTE_Event_Reg_State_e
                int32_t     cause;                                      /// < refer to ::VoLTE_Event_Reg_Cause_e
                int32_t     sip_cause;                                  ///< refer to ::VoLTE_Event_SIP_Cause_e

                uint32_t    conn_info;                                  ///< the connection information for the others UA to create the connection

                /* account information */
                int8_t      local_address[MAX_ADDRESS_LEN];             ///< local ip address
                int32_t     local_port;                                 ///< local port number
                int32_t     protocol_type;                              ///< refer to ::VoLTE_Stack_Protocol_Type_e
                int32_t     protocol_version;                           ///< refer to ::VoLTE_Stack_Protocol_Version_e
                int8_t      public_uid[MAX_IMPU_LEN];                   ///< the public user identity
                int8_t      private_uid[MAX_IMPU_LEN];                  ///< the private user identity
                int8_t      home_uri[MAX_DOMAIN_LEN];                   ///< the domain name of the home network
                int8_t      pcscf_address[MAX_ADDRESS_LIST_LEN];        ///< the current used PCSCF ip address
                int32_t     pcscf_port;                                 ///< the current used PCSCF port number
                int8_t      imei[MAX_IMEI_LEN];                         ///< the IMEI
                int8_t      associated_uri[MAX_ASSOCIATED_URI];         ///< the list of the associated URI
                int8_t      pub_gruu[MAX_GRUU_LEN];                     ///< the public gruu
                int8_t      temp_gruu[MAX_GRUU_LEN];                    ///< the temp gruu
                int8_t      service_route[MAX_SERVICE_ROUTE_LEN];       ///< the service route
                int8_t      path[MAX_URI_LEN];                          ///< the path
                int32_t     target_port_s;                              ///< target_port_s
                int32_t     net_type;                                   ///< refer to ::VoLTE_Event_Network_Type_e
                int32_t     net_type_3gpp;                              ///< refer to ::VoLTE_Event_Network_Type_e
                int32_t     emergency_type;                             ///< refer to ::VoLTE_Event_Emergency_Type_e
                int32_t     ems_mode;                                   ///< refer to ::VoLTE_Event_Ems_Mode_e
                int32_t     retry_after;                                ///< notify time delay for IMC retrying initial registration
                int8_t      reg_timestamp[MAX_TIMESTAMP_LEN];           ///< TMO P-Last-Access-Network-Info "REGISTRATION TIMESTAMP"
                int8_t      security_verify[MAX_SECURIT_VERIFY_LEN];    ///< security verify header in All the subsequent SIP requests
                int8_t      user_agent[MAX_UA_V1_LEN];                  ///< user-agent header for RCS usage
                int32_t     reg_uri_type;                                       /// todo: define enum for imsi and msisdn uri
                int8_t      msisdn_uri[MAX_IMPU_LEN];                   ///
                uint8_t     em_reg_timestamp[MAX_TIME_STAMP_LEN];       ///< last registered timestamp in milliseconds
                int8_t      instance_id[MAX_URI_LEN];                   ///< the instance id
                /*
                    pcscf_usage
                    0    default
                    1    insert pcscf_addr in method:PCO head (use same pcscf)
                    2    insert pcscf_addr in method:PCO tail
                */
                int8_t      pcscf_usage;
                int8_t      release_pdn;
                int8_t      voice_cap;
                int8_t      video_cap;
                int8_t      rcs_state;
                int32_t     rcs_feature;
                int8_t      tmp_voice_cap;
                int8_t      tmp_video_cap;
                int32_t     reg_service;                                ///< refer to ::ims_reg_service_enum
                int32_t     via_rport;                                  ///< Via rport
                int8_t      via_received[MAX_ADDRESS_LEN];              ///< Via received parameter
                int8_t      emergency_reg_got_error;
                int8_t      sms_cap;
                int8_t      subscribe_dialog_to_server;                 ///< subscription to the conference event package
                int8_t      digit_number_of_vline;
                int8_t      pani[MAX_PANI_LEN];                         ///< P-Acess-Network-Info header for RCS usage
                int8_t      digit_pidentifier[MAX_PIDENTIFIER_LEN];     // 3.6.8.1 Also include following: (Mandatory) ID: GID-MTRREQ-302326
                int8_t      digit_ppa_header_string[MAX_PPA_LEN];       // PPA for all the lines
                int32_t     reg_event;                                  ///< refer to ::ims_reg_event_enum
            } RegInfo;

            typedef struct {
                int32_t     id;                                         ///< the account id
                int32_t     state;                                      ///< refer to ::VoLTE_Event_Reg_State_e
                int32_t     cause;                                      /// < refer to ::VoLTE_Event_Reg_Cause_e
                int32_t     sip_cause;                                  ///< refer to ::VoLTE_Event_SIP_Cause_e

                uint32_t    conn_info;                                  ///< the connection information for the others UA to create the connection

                /* account information */
                int8_t      local_address[MAX_ADDRESS_LEN];             ///< local ip address
                int32_t     local_port;                                 ///< local port number
                int32_t     protocol_type;                              ///< refer to ::VoLTE_Stack_Protocol_Type_e
                int32_t     protocol_version;                           ///< refer to ::VoLTE_Stack_Protocol_Version_e
                int8_t      public_uid[MAX_IMPU_LEN];                   ///< the public user identity
                int8_t      private_uid[MAX_IMPU_LEN];                  ///< the private user identity
                int8_t      home_uri[MAX_DOMAIN_LEN];                   ///< the domain name of the home network
                int8_t      pcscf_address[MAX_ADDRESS_LIST_LEN];        ///< the current used PCSCF ip address
                int32_t     pcscf_port;                                 ///< the current used PCSCF port number
                int8_t      imei[MAX_IMEI_LEN];                         ///< the IMEI
                int8_t      associated_uri[MAX_ASSOCIATED_URI];         ///< the list of the associated URI
                int8_t      pub_gruu[MAX_GRUU_LEN];                     ///< the public gruu
                int8_t      temp_gruu[MAX_GRUU_LEN];                    ///< the temp gruu
                int8_t      service_route[MAX_SERVICE_ROUTE_LEN];       ///< the service route
                int8_t      path[MAX_URI_LEN];                          ///< the path
                int32_t     target_port_s;                              ///< target_port_s
                int32_t     net_type;                                   ///< refer to ::VoLTE_Event_Network_Type_e
                int32_t     net_type_3gpp;                              ///< refer to ::VoLTE_Event_Network_Type_e
                int32_t     emergency_type;                             ///< refer to ::VoLTE_Event_Emergency_Type_e
                int32_t     ems_mode;                                   ///< refer to ::VoLTE_Event_Ems_Mode_e
                int32_t     retry_after;                                ///< notify time delay for IMC retrying initial registration
                int8_t      reg_timestamp[MAX_TIMESTAMP_LEN];           ///< TMO P-Last-Access-Network-Info "REGISTRATION TIMESTAMP"
                int8_t      security_verify[MAX_SECURIT_VERIFY_LEN];    ///< security verify header in All the subsequent SIP requests
                int8_t      user_agent[MAX_UA_V2_LEN];                  ///< user-agent header for RCS usage
                int32_t     reg_uri_type;                                       /// todo: define enum for imsi and msisdn uri
                int8_t      msisdn_uri[MAX_IMPU_LEN];                   ///
                uint8_t     em_reg_timestamp[MAX_TIME_STAMP_LEN];        ///< last registered timestamp in milliseconds
                int8_t      instance_id[MAX_URI_LEN];                   ///< the instance id
                /*
                    pcscf_usage
                    0    default
                    1    insert pcscf_addr in method:PCO head (use same pcscf)
                    2    insert pcscf_addr in method:PCO tail
                */
                int8_t      pcscf_usage;
                int8_t      release_pdn;
                int8_t      voice_cap;
                int8_t      video_cap;
                int8_t      rcs_state;
                int32_t     rcs_feature;
                int8_t      tmp_voice_cap;
                int8_t      tmp_video_cap;
                int32_t     reg_service;                                ///< refer to ::ims_reg_service_enum
                int32_t     via_rport;                                  ///< Via rport
                int8_t      via_received[MAX_ADDRESS_LEN];              ///< Via received parameter
                int8_t      emergency_reg_got_error;
                int8_t      sms_cap;
                int8_t      subscribe_dialog_to_server;                 ///< subscription to the conference event package
                int8_t      digit_number_of_vline;
                int8_t      pani[MAX_PANI_LEN];                         ///< P-Acess-Network-Info header for RCS usage
                int8_t      digit_pidentifier[MAX_PIDENTIFIER_LEN];     // 3.6.8.1 Also include following: (Mandatory) ID: GID-MTRREQ-302326
                int8_t      digit_ppa_header_string[MAX_PPA_LEN];       // PPA for all the lines
                int32_t     reg_event;                                  ///< refer to ::ims_reg_event_enum
            } RegInfov2;

            // UA -> IMS Stack
            static const int REQ_CREATE             = 90001;  /* VoLTE_Stack_Channel_Info_t */
            static const int REQ_DESTROY            = 90002;  /* N/A */
            static const int REQ_CONNECT            = 90003;  /* VoLTE_Stack_Connection_Info_t */
            static const int REQ_DISCONNECT         = 90004;  /* N/A */
            static const int REQ_SEND_SIP           = 90005;  /* VoLTE_Stack_Sip_Info_t */
            static const int REQ_REG_ADD_BIND       = 91013;  /* VoLTE_Event_Reg_Item_t */
            static const int REQ_REG_DEL_BIND       = 91014;  /* VoLTE_Event_Reg_Item_t */
            static const int REQ_REG_CLEAR_BIND     = 91015;  /* N/A */
            static const int REQ_REG_GET_STATE      = 91018;  /* VoLTE_Event_Reg_Item_t */
            static const int REQ_REG_RESTORE        = 91023;  /* VoLTE_Event_Reg_Item_t */
            static const int CNF_REG_RESUME_DEREG   = 91033;  /* VoLTE_Event_Notify_t  */
            static const int IND_REG_ENABLE_DEREG   = 91045;  /* VoLTE_Event_External_UA_State_t  */

            // IMS Stack -> UA
            static const int RSP_CREATE             = 95001;  /* VoLTE_Stack_Result_t */
            static const int RSP_DESTROY            = 95002;  /* VoLTE_Stack_Result_t */
            static const int RSP_SEND_SIP           = 95005;  /* VoLTE_Stack_Result_t */
            static const int RSP_IND_SIPMSG         = 95006;  /* VoLTE_Stack_Sip_Info_t */
            static const int IND_DISCONNECT         = 95011;  /* VoLTE_Stack_Result_t */
            static const int IND_REG_STATE          = 96001;  /* VoLTE_Event_Reg_State_t */
            static const int IND_REG_DEREG_START    = 96011;

            static const int UA_RCS                 = 6;
            static const int UA_RCS_SIP             = 10;
            static const int UA_MAGIC_NUMBER        = 0x12140500;

            enum StackResult {
                Result_Disconnected     = -3,
                Result_Timeout          = -2,
                Result_Error            = -1,
                Result_Success          = 0
            };

            enum ChannelType {
                STATE_CHANNEL = 1,
                DATA_CHANNEL = 2
            };

            RcsStackChannel(int32_t slotId, ChannelType type);

            ~RcsStackChannel() {
                if (m_raw_data) {
                    free(m_raw_data);
                    m_raw_data = 0;
                }
            }

            uint8_t * getRawData() const {
                return m_raw_data;
            }

            void releaseRawData() {
                if (m_raw_data) {
                    free(m_raw_data);
                    m_raw_data = 0;
                }
            }

            int create(uint32_t connInfo, char *userInfo, const char *associated);

            int destroy();

            int bind();

            int unbind();

            int getRegState();

            int setDeregIndEnabled(bool enable);

            int resumeDeregistration();

            int restore();

            int sendSipMessage(int32_t reqId, int32_t connId, const uint8_t *messageData, int32_t messageLen);

            static char *reqToString(int reqId) {
                switch (reqId) {
                    case REQ_CREATE:            return (char *)"REQ_CREATE";
                    case REQ_DESTROY:           return (char *)"REQ_DESTROY";
                    case REQ_CONNECT:           return (char *)"REQ_CONNECT";
                    case REQ_DISCONNECT:        return (char *)"REQ_DISCONNECT";
                    case REQ_SEND_SIP:          return (char *)"REQ_SEND_SIP";
                    case REQ_REG_ADD_BIND:      return (char *)"REQ_REG_ADD_BIND";
                    case REQ_REG_DEL_BIND:      return (char *)"REQ_REG_DEL_BIND";
                    case REQ_REG_CLEAR_BIND:    return (char *)"REQ_REG_CLEAR_BIND";
                    case REQ_REG_GET_STATE:     return (char *)"REQ_REG_GET_STATE";
                    case REQ_REG_RESTORE:       return (char *)"REQ_REG_RESTORE";
                    case CNF_REG_RESUME_DEREG:  return (char *)"CNF_REG_RESUME_DEREG";
                    case IND_REG_ENABLE_DEREG:  return (char *)"IND_REG_ENABLE_DEREG";
                    case RSP_CREATE:            return (char *)"RSP_CREATE";
                    case RSP_DESTROY:           return (char *)"RSP_DESTROY";
                    case RSP_SEND_SIP:          return (char *)"RSP_SEND_SIP";
                    case RSP_IND_SIPMSG:        return (char *)"RSP_IND_SIPMSG";
                    case IND_DISCONNECT:        return (char *)"IND_DISCONNECT";
                    case IND_REG_STATE:         return (char *)"IND_REG_STATE";
                    case IND_REG_DEREG_START:   return (char *)"IND_REG_DEREG_START";
                    default:                    return (char *)"Unknown";
                }
            }

        private:

            enum SipMethod {
                METHOD_REGISTER,       ///<  0, SIP REGISTER
                METHOD_INVITE,         ///<  1, SIP INVITE
                METHOD_REFER,          ///<  2, SIP REFER
                METHOD_UPDATE,         ///<  3, SIP UPDATE
                METHOD_CANCEL,         ///<  4, SIP CANCEL
                METHOD_MESSAGE,        ///<  5, SIP MESSAGE
                METHOD_ACK,            ///<  6, SIP ACK
                METHOD_BYE,            ///<  7, SIP BYE
                METHOD_OPTIONS,        ///<  8, SIP OPTIONS
                METHOD_SUBSCRIBE,      ///<  9, SIP SUBSCRIBE
                METHOD_NOTIFY,         ///< 10, SIP NOTIFY
                METHOD_PUBLISH,        ///< 11, SIP PUBLISH
                METHOD_INFO,           ///< 12, SIP INFO
                METHOD_PRACK,          ///< 13, SIP PRACK
                METHOD_NUM,
                METHOD_MAX = 0xFFFFFFFF
            };

            enum SipHeader {
                HDR_ACCEPT,                    /* 0 */
                HDR_ACCEPT_CHARSET,            /* 01 */
                HDR_ACCEPT_CONTACT,            /* 02 */
                HDR_ACCEPT_ENCODING,           /* 03 */
                HDR_ACCEPT_LANGUAGE,           /* 04 */
                HDR_ACCEPT_RANGES,             /* 05 */
                HDR_AGE,                       /* 06 */
                HDR_ALERT_INFO,                /* 07 */
                HDR_ALLOW,                     /* 08 */
                HDR_AUTH_INFO,                 /* 09 */
                HDR_AUTHORIZATION,             /* 10 */
                HDR_BSID,                      /* 11 */
                HDR_CACHE_CONTROL,             /* 12 */
                HDR_CALL_ID,                   /* 13 */
                HDR_CONTACT,                   /* 14 */
                HDR_CONNECTION,                /* 15 */
                HDR_CONTENT_DISPOSITION,       /* 16 */
                HDR_CONTENT_ENCODING,          /* 17 */
                HDR_CONTENT_LANGUAGE,          /* 18 */
                HDR_CONTENT_LENGTH,            /* 19 */
                HDR_CONTENT_TYPE,              /* 20 */
                HDR_CONTENT_ID,                /* 21 */
                HDR_CONTENT_LOCATION,          /* 22 */
                HDR_CONTENT_MD5,               /* 23 */
                HDR_CONTENT_RANGE,             /* 24 */
                HDR_CONTENT_TRANSER_ENCODING,  /* 25 */
                HDR_COOKIE,                    /* 26 */
                HDR_CSEQ,                      /* 27 */
                HDR_DATE,                      /* 28 */
                HDR_ERROR_INFO,                /* 29 */
                HDR_EXPECT,                    /* 30 */
                HDR_EXPIRES,                   /* 31 */
                HDR_ETAG,                      /* 32 */
                HDR_FROM,                      /* 33 */
                HDR_HOST,                      /* 34 */
                HDR_IF_MATCH,                  /* 35 */
                HDR_IF_MODIFIED_SINCE,         /* 36 */
                HDR_IF_NONE_MATCH,             /* 37 */
                HDR_IF_RANGE,                  /* 38 */
                HDR_IF_UNMODIFIED_SINCE,       /* 39 */
                HDR_IN_REPLY_TO,               /* 40 */
                HDR_LAST_MODIFIED,             /* 41 */
                HDR_LOCATION,                  /* 42 */
                HDR_MAX_FORWARDS,              /* 43 */
                HDR_METHOD,                    /* 44 */
                HDR_MIN_EXPIRES,               /* 45 */
                HDR_MIME_VERSION,              /* 46 */
                HDR_ORGANIZATION,              /* 47 */
                HDR_PRAGMA,                    /* 48 */
                HDR_PRIORITY,                  /* 49 */
                HDR_PROXY_AUTHENTICATE,        /* 50 */
                HDR_PROXY_AUTHORIZATION,       /* 51 */
                HDR_PROXY_REQUIRE,             /* 52 */
                HDR_RANGE,                     /* 53 */
                HDR_REASON_PHRASE,             /* 54 */
                HDR_RECORD_ROUTE,              /* 55 */
                HDR_REPLY_TO,                  /* 56 */
                HDR_REFER,                     /* 57 */
                HDR_REFER_TO,                  /* 58 */
                HDR_REQUIRE,                   /* 59 */
                HDR_RESP_CODE,                 /* 60 */
                HDR_RETRY_AFTER,               /* 61 */
                HDR_ROUTE,                     /* 62 */
                HDR_SERVER,                    /* 63 */
                HDR_SET_COOKIE,                /* 64 */
                HDR_SET_COOKIE2,               /* 65 */
                HDR_SESSION_EXPIRES,           /* 66 */
                HDR_SUBJECT,                   /* 67 */
                HDR_SUPPORTED,                 /* 68 */
                HDR_TIMESTAMP,                 /* 69 */
                HDR_TRAILER,                   /* 70 */
                HDR_TRANSFER_ENCODING,         /* 71 */
                HDR_TE,                        /* 72 */
                HDR_TO,                        /* 73 */
                HDR_UNSUPPORTED,               /* 74 */
                HDR_UPGRADE,                   /* 75 */
                HDR_URI,                       /* 76 */
                HDR_USER_AGENT,                /* 77 */
                HDR_VIA,                       /* 78 */
                HDR_VARY,                      /* 79 */
                HDR_WARNING,                   /* 80 */
                HDR_WWW_AUTHENTICATE,          /* 81 */
                HDR_REQUEST_URI,               /* 82 */
                HDR_START_LINE_PROTO,          /* 83 */
                HDR_MIN_SE,                    /* 84 */
                HDR_EVENT,                     /* 85 */
                HDR_SUBSCRIPTION_STATE,        /* 86 */
                HDR_SIP_IF_MATCH,              /* 87 */
                HDR_SIP_ETAG,                  /* 88 */
                HDR_P_ALERTING_MODE,           /* 89 */
                HDR_P_PREFERRED_IDENTITY,      /* 90 */
                HDR_PRIVACY,                   /* 91 */
                HDR_PRIV_ANSWER_MODE,          /* 92 */
                HDR_ANSWER_MODE,               /* 93 */
                HDR_REFERRED_BY,               /* 94 */
                HDR_REPLACES,                  /* 95 */
                HDR_JOIN,                      /* 96 */
                HDR_ALLOW_EVENTS,              /* 97 */
                HDR_MSG_WAITING,               /* 98 */
                HDR_MSG_ACCOUNT,               /* 99 */
                HDR_MW_VOICE_MSG,              /* 100 */
                HDR_MW_FAX_MSG,                /* 101 */
                HDR_MW_PAGER_MSG,              /* 102 */
                HDR_MW_MM_MSG,                 /* 103 */
                HDR_MW_TEXT_MSG,               /* 104 */
                HDR_MW_NONE_MSG,               /* 105 */
                HDR_RACK,                      /* 106 */
                HDR_RSEQ,                      /* 107 */
                HDR_SESSION,                   /* 108 */
                HDR_TRANSPORT,                 /* 109 */
                HDR_RTPINFO,                   /* 110 */
                HDR_REASON,                    /* 111 */
                HDR_REJECT_CONTACT,            /* 112 */
                HDR_P_ASSERTED_IDENTITY,       /* 113 */
                HDR_SERVICE_ROUTE,             /* 114 */
                HDR_P_ACCESS_NETWORK_INFO,     /* 115 */
                HDR_P_ASSOCIATED_URI,          /* 116 */
                HDR_P_CALLED_PARTY_ID,         /* 117 */
                HDR_P_VISITED_NETWORK_ID,      /* 118 */
                HDR_P_ASSERTED_SERVICE,        /* 119 */
                HDR_P_PREFERRED_SERVICE,       /* 120 */
                HDR_SECURITY_CLIENT,           /* 121 */
                HDR_SECURITY_SERVER,           /* 122 */
                HDR_SECURITY_VERIFY,           /* 123 */
                HDR_HISTORY_INFO,              /* 124 */
                HDR_REQUEST_DISPOSITION,       /* 125 */
                HDR_P_EARLY_MEDIA,             /* 126 */
                HDR_PATH,                      /* 127 */
                HDR_P_LAST_ACCESS_NETWORK_INFO,/* 128 */
                HDR_P_EMERGENCY_CALL_MODE_PREFERENCE,  /* 129 */
                HDR_GEOLOCATION,               /* 130 */
                HDR_GEOLOCATION_ROUTING,       /* 131 */
                HDR_FEATURE_CAPS,              /* 132 */
                HDR_RECV_INFO,                 /* 133 */
                HDR_INFO_PACKAGE,              /* 134 */
                HDR_P_EMERGENCY_INFO,          /* 135 */
                HDR_IDENTITY,                  /* 136 */
                HDR_IDENTITY_INFO,             /* 137 */
                HDR_CELLULAR_NETWORK_INFO,     /* 138 */
                HDR_UNRECOGNIZED,
                HDR_NUM,
                HDR_MAX = 0xFFFFFFFF
            };

            int32_t m_slot;
            int32_t m_account;
            ChannelType m_type;
            uint8_t *m_raw_data;
            bool m_is_bind;
    };
} //namespace: rmm

#endif
