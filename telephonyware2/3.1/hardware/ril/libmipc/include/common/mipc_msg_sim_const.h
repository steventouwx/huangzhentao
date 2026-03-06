#ifndef __MIPC_MSG_SIM_CONST_H__
#define __MIPC_MSG_SIM_CONST_H__

enum MIPC_SIM_MSG_enum {
    MIPC_SIM_MSG_NONE = 0,
    /* This command is used to enable/disable PIN. */
    MIPC_SIM_PIN_PROTECT_REQ                                = 1281,
    MIPC_SIM_PIN_PROTECT_CNF                                = 1282,

    /* This command is used to change PIN. */
    MIPC_SIM_CHANGE_PIN_REQ                                 = 1283,
    MIPC_SIM_CHANGE_PIN_CNF                                 = 1284,

    /* This command is used to verify PIN. */
    MIPC_SIM_VERIFY_PIN_REQ                                 = 1285,
    MIPC_SIM_VERIFY_PIN_CNF                                 = 1286,

    /* This command is used to unblock PIN. */
    MIPC_SIM_UNBLOCK_PIN_REQ                                = 1287,
    MIPC_SIM_UNBLOCK_PIN_CNF                                = 1288,

    /* This command is used to acquire the PIN info status of SIM card. */
    MIPC_SIM_GET_PIN_INFO_REQ                               = 1289,
    MIPC_SIM_GET_PIN_INFO_CNF                               = 1290,

    /* This command is used to acquire all PIN supported by modem. */
    MIPC_SIM_GET_PIN_LIST_REQ                               = 1291,
    MIPC_SIM_GET_PIN_LIST_CNF                               = 1292,

    /* This command is used to acquire SIM state. */
    MIPC_SIM_STATE_REQ                                      = 1293,
    MIPC_SIM_STATE_CNF                                      = 1294,

    /* This command is used to acquire SIM status. */
    MIPC_SIM_STATUS_REQ                                     = 1295,
    MIPC_SIM_STATUS_CNF                                     = 1296,

    /* This command is used to acquire the ICCID of SIM card. */
    MIPC_SIM_ICCID_REQ                                      = 1297,
    MIPC_SIM_ICCID_CNF                                      = 1298,

    /* This command is used to acquire the IMSI of SIM card. */
    MIPC_SIM_IMSI_REQ                                       = 1299,
    MIPC_SIM_IMSI_CNF                                       = 1300,

    /* This command is used to acquire the MSISDN in SIM card. */
    MIPC_SIM_MSISDN_REQ                                     = 1301,
    MIPC_SIM_MSISDN_CNF                                     = 1302,

    /* This command is used to acquire the ATR of SIM card. */
    MIPC_SIM_GET_ATR_INFO_REQ                               = 1303,
    MIPC_SIM_GET_ATR_INFO_CNF                               = 1304,

    /* This command is used to open a logical channel. */
    MIPC_SIM_OPEN_CHANNEL_REQ                               = 1305,
    MIPC_SIM_OPEN_CHANNEL_CNF                               = 1306,

    /* This command is used to close the logical channel. */
    MIPC_SIM_CLOSE_CHANNEL_REQ                              = 1307,
    MIPC_SIM_CLOSE_CHANNEL_CNF                              = 1308,

    /* This command is used to execute restricted access. */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ                  = 1309,
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF                  = 1310,

    /* This command is used to execute generic access. */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ                     = 1311,
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF                     = 1312,

    /* This command is used to execute long APDU access to SIM card. */
    MIPC_SIM_LONG_APDU_ACCESS_REQ                           = 1313,
    MIPC_SIM_LONG_APDU_ACCESS_CNF                           = 1314,

    /* This command is used to acquire the list of application */
    MIPC_SIM_APP_LIST_REQ                                   = 1315,
    MIPC_SIM_APP_LIST_CNF                                   = 1316,

    /* This command is used to acquire file status. */
    MIPC_SIM_FILE_STATUS_REQ                                = 1317,
    MIPC_SIM_FILE_STATUS_CNF                                = 1318,

    /* This command is used to acquire status of SIM pass-through mode or SIM application/power status. */
    MIPC_SIM_GET_RESET_REQ                                  = 1319,
    MIPC_SIM_GET_RESET_CNF                                  = 1320,

    /* This command is used to reset SIM. */
    MIPC_SIM_SET_RESET_REQ                                  = 1321,
    MIPC_SIM_SET_RESET_CNF                                  = 1322,

    /* This command is used to acquire saved terminal capability. */
    MIPC_SIM_GET_TERMINAL_CAPABILITY_REQ                    = 1323,
    MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF                    = 1324,

    /* This command is used to set terminal capability. */
    MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ                    = 1325,
    MIPC_SIM_SET_TERMINAL_CAPABILITY_CNF                    = 1326,

    /* This command is used to verify PIN by application. */
    MIPC_SIM_SET_PIN_EX_REQ                                 = 1327,
    MIPC_SIM_SET_PIN_EX_CNF                                 = 1328,

    /* This command is used to acquire PIN information by application. */
    MIPC_SIM_GET_PIN_EX_REQ                                 = 1329,
    MIPC_SIM_GET_PIN_EX_CNF                                 = 1330,

    /* This command is used to acquire response from GSM authentication performed. */
    MIPC_SIM_GET_GSM_AUTH_REQ                               = 1331,
    MIPC_SIM_GET_GSM_AUTH_CNF                               = 1332,

    /* This command is used to acquire response of extension authentication performed. */
    MIPC_SIM_GET_EXT_AUTH_REQ                               = 1333,
    MIPC_SIM_GET_EXT_AUTH_CNF                               = 1334,

    /* This command is used to query SIM facility. */
    MIPC_SIM_GET_FACILITY_REQ                               = 1335,
    MIPC_SIM_GET_FACILITY_CNF                               = 1336,

    /* This command is used to set SIM facility. */
    MIPC_SIM_SET_FACILITY_REQ                               = 1337,
    MIPC_SIM_SET_FACILITY_CNF                               = 1338,

    /* This command is used to acquire status of eUICC slots. */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_REQ                     = 1339,
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF                     = 1340,

    /* This command is used to request for SAP connection. */
    MIPC_SIM_ACCESS_PROFILE_CONNECT_REQ                     = 1341,
    MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF                     = 1342,

    /* This command is used to request SAP disconnection. */
    MIPC_SIM_ACCESS_PROFILE_DISCONNECT_REQ                  = 1343,
    MIPC_SIM_ACCESS_PROFILE_DISCONNECT_CNF                  = 1344,

    /* This command is used to request SAP power-on. */
    MIPC_SIM_ACCESS_PROFILE_POWER_ON_REQ                    = 1345,
    MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF                    = 1346,

    /* This command is used to request for SAP power-off. */
    MIPC_SIM_ACCESS_PROFILE_POWER_OFF_REQ                   = 1347,
    MIPC_SIM_ACCESS_PROFILE_POWER_OFF_CNF                   = 1348,

    /* This command is used to request for resetting SIM by SAP */
    MIPC_SIM_ACCESS_PROFILE_RESET_REQ                       = 1349,
    MIPC_SIM_ACCESS_PROFILE_RESET_CNF                       = 1350,

    /* This command is used to request for SAP APDU. */
    MIPC_SIM_ACCESS_PROFILE_APDU_REQ                        = 1351,
    MIPC_SIM_ACCESS_PROFILE_APDU_CNF                        = 1352,

    /* This command is used to set SIM power. */
    MIPC_SIM_SET_SIM_POWER_REQ                              = 1353,
    MIPC_SIM_SET_SIM_POWER_CNF                              = 1354,

    /* request to set sim slots mapping */
    MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ                 = 1355,
    MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_CNF                 = 1356,

    /* This command is used to perform extended generic access. */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ            = 1357,
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF            = 1358,

    /* This command is used to access record from SIM UICC file. */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ                    = 1359,
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF                    = 1360,

    /* This command is used to access binary from SIM UICC file. */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ                    = 1361,
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF                    = 1362,

    /* request to get sim slots mapping */
    MIPC_SIM_GET_PHYSICAL_SLOTS_MAPPING_REQ                 = 1363,
    MIPC_SIM_GET_PHYSICAL_SLOTS_MAPPING_CNF                 = 1364,

    /* This command is used to acquire response of extension authentication performed. */
    MIPC_SIM_GET_SIM_AUTH_REQ                               = 1365,
    MIPC_SIM_GET_SIM_AUTH_CNF                               = 1366,

    /* This command is used to acquire carrier lock information. */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_REQ                   = 1367,
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF                   = 1368,

    /* This command is used to set carriers lock information. */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ                   = 1369,
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF                   = 1370,

    /* This command is used to enter SIM depersonalization of specific SML. */
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ            = 1371,
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF            = 1372,

    /* This command is used to query SML information. */
    MIPC_SIM_SML_GET_LOCK_REQ                               = 1373,
    MIPC_SIM_SML_GET_LOCK_CNF                               = 1374,

    /* This command is used to set SIM ME lock. */
    MIPC_SIM_SML_SET_LOCK_REQ                               = 1375,
    MIPC_SIM_SML_SET_LOCK_CNF                               = 1376,

    /* This command is used to update device lock state. */
    MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ         = 1377,
    MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF         = 1378,

    /* This command is used to query device lock state */
    MIPC_SIM_SML_GET_DEV_LOCK_REQ                           = 1379,
    MIPC_SIM_SML_GET_DEV_LOCK_CNF                           = 1380,

    /* This command is used to remotely unlock SIM for different operator. */
    MIPC_SIM_SML_RSU_REQ                                    = 1381,
    MIPC_SIM_SML_RSU_CNF                                    = 1382,

    /* request to set aka sim */
    MIPC_SIM_VSIM_SET_AKA_REQ                               = 1383,
    MIPC_SIM_VSIM_SET_AKA_CNF                               = 1384,

    /* request to enable Vsim */
    MIPC_SIM_VSIM_ENABLE_REQ                                = 1385,
    MIPC_SIM_VSIM_ENABLE_CNF                                = 1386,

    /* request to disable Vsim */
    MIPC_SIM_VSIM_DISABLE_REQ                               = 1387,
    MIPC_SIM_VSIM_DISABLE_CNF                               = 1388,

    /* request to plug in/out vsim */
    MIPC_SIM_VSIM_PLUG_REQ                                  = 1389,
    MIPC_SIM_VSIM_PLUG_CNF                                  = 1390,

    /* request to set vsim timer */
    MIPC_SIM_VSIM_SET_TIMER_REQ                             = 1391,
    MIPC_SIM_VSIM_SET_TIMER_CNF                             = 1392,

    /* request to reset vsim  */
    MIPC_SIM_VSIM_RESET_REQ                                 = 1393,
    MIPC_SIM_VSIM_RESET_CNF                                 = 1394,

    /* request to send APDU  */
    MIPC_SIM_VSIM_APDU_REQ                                  = 1395,
    MIPC_SIM_VSIM_APDU_CNF                                  = 1396,

    /* request to auth */
    MIPC_SIM_VSIM_AUTH_REQ                                  = 1397,
    MIPC_SIM_VSIM_AUTH_CNF                                  = 1398,

    /* request CDMA subscription */
    MIPC_SIM_CDMA_SUBSCRIPTION_REQ                          = 1399,
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF                          = 1400,

    /* request CDMA subscription source */
    MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_REQ               = 1401,
    MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF               = 1402,

    /* This command is used to acquire PIN count. */
    MIPC_SIM_PIN_COUNT_QUERY_REQ                            = 1403,
    MIPC_SIM_PIN_COUNT_QUERY_CNF                            = 1404,

    /* get network lock info */
    MIPC_SIM_SML_GET_NETWORK_LOCK_REQ                       = 1405,
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF                       = 1406,

    /* request switch to esim/psim */
    MIPC_SIM_ESIM_SWITCH_SET_REQ                            = 1407,
    MIPC_SIM_ESIM_SWITCH_SET_CNF                            = 1408,

    /* query  esim/psim */
    MIPC_SIM_ESIM_SWITCH_GET_REQ                            = 1409,
    MIPC_SIM_ESIM_SWITCH_GET_CNF                            = 1410,

    /* query  esim eid */
    MIPC_SIM_ESIM_EID_QUERY_REQ                             = 1411,
    MIPC_SIM_ESIM_EID_QUERY_CNF                             = 1412,

    /* Request MEP(Multi-Enabled Profile) slots related information. Only supported when MEP feature is supported by modem */
    MIPC_SIM_MEP_SLOTS_INFO_GET_REQ                         = 1413,
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF                         = 1414,

    /* Set MEP(Multi-Enabled Profile) slots mapping to modem */
    MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ                      = 1415,
    MIPC_SIM_MEP_SLOTS_MAPPING_SET_CNF                      = 1416,

    /* query current switch hotswap off state */
    MIPC_SIM_HOTSWAP_DISABLE_GET_REQ                        = 1417,
    MIPC_SIM_HOTSWAP_DISABLE_GET_CNF                        = 1418,

    /* switch off or on sim hotswap detect.If hotswap detect or auto recovery is switch on, sim will be detected as inserted when sim plug in.If both hotswap detect and auto recovery is switch off, sim card will be detected as not inserted when sim plug in */
    MIPC_SIM_HOTSWAP_DISABLE_SET_REQ                        = 1419,
    MIPC_SIM_HOTSWAP_DISABLE_SET_CNF                        = 1420,

    /* query current switch recovery off state */
    MIPC_SIM_RECOVERY_DISABLE_GET_REQ                       = 1421,
    MIPC_SIM_RECOVERY_DISABLE_GET_CNF                       = 1422,

    /* switch off or on sim auto recovery when sim error occurred.If hotswap detect or auto recovery is switch on, sim will be detected as inserted when sim plug in.If both hotswap detect and auto recovery is switch off, sim card will be detected as not inserted when sim plug in */
    MIPC_SIM_RECOVERY_DISABLE_SET_REQ                       = 1423,
    MIPC_SIM_RECOVERY_DISABLE_SET_CNF                       = 1424,

    /* This command is used to query Public Service Identity of the SM-SC (PSISMSC) */
    MIPC_SIM_GET_PSISMSC_REQ                                = 1425,
    MIPC_SIM_GET_PSISMSC_CNF                                = 1426,

    /* This command is to query Short Message Service Parameters (SMSP) */
    MIPC_SIM_GET_SMSP_REQ                                   = 1427,
    MIPC_SIM_GET_SMSP_CNF                                   = 1428,

    /* This command is used to query the SIM service table. In SIM service table, each byte indicates which services are activated. If a service is not activated (or alloacted), users shall not select this service */
    MIPC_SIM_GET_SERVICE_TABLE_REQ                          = 1429,
    MIPC_SIM_GET_SERVICE_TABLE_CNF                          = 1430,

    /* This message is used to query Group Identifier (GID) */
    MIPC_SIM_QUERY_GID_REQ                                  = 1431,
    MIPC_SIM_QUERY_GID_CNF                                  = 1432,

    /* This message is used to get IMS private user identity (IMPI) */
    MIPC_SIM_GET_IMPI_REQ                                   = 1433,
    MIPC_SIM_GET_IMPI_CNF                                   = 1434,

    /* This message is used to get IMS public user identity (IMPU) */
    MIPC_SIM_GET_IMPU_REQ                                   = 1435,
    MIPC_SIM_GET_IMPU_CNF                                   = 1436,

    /* This message is used to get Elementary File Home Network Domain Name (EFDOMAIN) */
    MIPC_SIM_GET_DOMAIN_REQ                                 = 1437,
    MIPC_SIM_GET_DOMAIN_CNF                                 = 1438,

    /* This message is used to get Proxy - Call Session Control Function (PCSCF) */
    MIPC_SIM_GET_PCSCF_REQ                                  = 1439,
    MIPC_SIM_GET_PCSCF_CNF                                  = 1440,

    /* This command will be reported when the SIM state is changed. */
    MIPC_SIM_STATE_IND                                      = 17665,

    /* This command will be reported when the SIM status is changed. */
    MIPC_SIM_STATUS_IND                                     = 17666,

    /*  */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND                         = 17667,

    /* This command is used to reported ICCID of SIM card. */
    MIPC_SIM_ICCID_IND                                      = 17668,

    /* This command will be reported with cause when the SIM status is changed. */
    MIPC_SIM_EVENT_IND                                      = 17669,
    MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND                   = 17669,

    /* SIM physical slots mapping set req command done indication (phase out) */
    MIPC_SIM_PHYSICAL_SLOTS_MAPPING_DONE_IND                = 17670,

    /* csim imsi changed indication */
    MIPC_SIM_CSIM_IMSI_CHANGE_IND                           = 17671,

    /* This command is used to report SIM ME lock rules and device lock status. */
    MIPC_SIM_SML_STATUS_IND                                 = 17672,

    /* This command is used to report RSU event ID and event string. */
    MIPC_SIM_SML_RSU_IND                                    = 17673,

    /* vsim reset indication  */
    MIPC_SIM_VSIM_RESET_IND                                 = 17676,

    /* vsim APDU indication  */
    MIPC_SIM_VSIM_APDU_IND                                  = 17677,

    /* vsim APDU indication  */
    MIPC_SIM_VSIM_EVENT_IND                                 = 17678,

    /* This command is used to report the UICC application ID, channel information, and MCC/MNC when the application completes its initialization procedure. */
    MIPC_SIM_SIMAPP_IND                                     = 17679,

    /* This command is used to report whether the current inserted SIM card is a test SIM card or not. */
    MIPC_SIM_TEST_SIM_IND                                   = 17680,

    /* CT3G indication  */
    MIPC_SIM_CT3G_IND                                       = 17681,

    /* This command is used to report the type of SIM card. */
    MIPC_SIM_CARD_TYPE_IND                                  = 17682,

    /* This command is used to report the information of SIM. */
    MIPC_SIM_SIMIND_IND                                     = 17683,

    /* This command is used to report SIM status changed and indicate the host can query new SIM status. */
    MIPC_SIM_STATUS_CHANGED_IND                             = 17684,

    /* esim state indicator */
    MIPC_SIM_ESLOTESIM_STATE_IND                            = 17685,

    /* This command will report MEP (Multi-Enabled Profile) slots information when the slot information changes */
    MIPC_SIM_MEP_SLOTS_INFO_IND                             = 17686,


};

    /* MIPC_MSG.SIM_PIN_PROTECT_REQ */
enum mipc_sim_pin_protect_req_tlv_enum {
    mipc_sim_pin_protect_req_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_PIN_PROTECT_REQ_T_PIN_TYPE                     = 0x100,
    /* PIN operation */
    /* type = uint8_t, refer to SIM_PIN_PROTECTION */
    MIPC_SIM_PIN_PROTECT_REQ_T_PIN_OP                       = 0x101,
    /* PIN code */
    /* type = string */
    MIPC_SIM_PIN_PROTECT_REQ_T_PIN_CODE                     = 0x8102,
};

    /* MIPC_MSG.SIM_PIN_PROTECT_CNF */
enum mipc_sim_pin_protect_cnf_tlv_enum {
    mipc_sim_pin_protect_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_PIN_PROTECT_CNF_T_PIN_TYPE                     = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_PIN_PROTECT_CNF_T_PIN_STATE                    = 0x101,
    /* Remaining attempts */
    /* type = uint32_t */
    MIPC_SIM_PIN_PROTECT_CNF_T_REMAINING_ATTEMPTS           = 0x102,
};

    /* MIPC_MSG.SIM_CHANGE_PIN_REQ */
enum mipc_sim_change_pin_req_tlv_enum {
    mipc_sim_change_pin_req_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_CHANGE_PIN_REQ_T_PIN_TYPE                      = 0x100,
    /* Old PIN code */
    /* type = string */
    MIPC_SIM_CHANGE_PIN_REQ_T_OLD_PIN                       = 0x8101,
    /* New PIN code */
    /* type = string */
    MIPC_SIM_CHANGE_PIN_REQ_T_NEW_PIN                       = 0x8102,
};

    /* MIPC_MSG.SIM_CHANGE_PIN_CNF */
enum mipc_sim_change_pin_cnf_tlv_enum {
    mipc_sim_change_pin_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_CHANGE_PIN_CNF_T_PIN_TYPE                      = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_CHANGE_PIN_CNF_T_PIN_STATE                     = 0x101,
    /* Remaining attempts */
    /* type = uint32_t */
    MIPC_SIM_CHANGE_PIN_CNF_T_REMAINING_ATTEMPTS            = 0x102,
};

    /* MIPC_MSG.SIM_VERIFY_PIN_REQ */
enum mipc_sim_verify_pin_req_tlv_enum {
    mipc_sim_verify_pin_req_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_VERIFY_PIN_REQ_T_PIN_TYPE                      = 0x100,
    /* PIN code */
    /* type = string */
    MIPC_SIM_VERIFY_PIN_REQ_T_PIN_CODE                      = 0x8101,
};

    /* MIPC_MSG.SIM_VERIFY_PIN_CNF */
enum mipc_sim_verify_pin_cnf_tlv_enum {
    mipc_sim_verify_pin_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_VERIFY_PIN_CNF_T_PIN_TYPE                      = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_VERIFY_PIN_CNF_T_PIN_STATE                     = 0x101,
    /* Remaining attempts */
    /* type = uint32_t */
    MIPC_SIM_VERIFY_PIN_CNF_T_REMAINING_ATTEMPTS            = 0x102,
};

    /* MIPC_MSG.SIM_UNBLOCK_PIN_REQ */
enum mipc_sim_unblock_pin_req_tlv_enum {
    mipc_sim_unblock_pin_req_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_TYPE                     = 0x100,
    /* PUK code */
    /* type = string */
    MIPC_SIM_UNBLOCK_PIN_REQ_T_PUK_CODE                     = 0x8101,
    /* PIN code */
    /* type = string */
    MIPC_SIM_UNBLOCK_PIN_REQ_T_PIN_CODE                     = 0x8102,
};

    /* MIPC_MSG.SIM_UNBLOCK_PIN_CNF */
enum mipc_sim_unblock_pin_cnf_tlv_enum {
    mipc_sim_unblock_pin_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_UNBLOCK_PIN_CNF_T_PIN_TYPE                     = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_UNBLOCK_PIN_CNF_T_PIN_STATE                    = 0x101,
    /* Remaining attempts */
    /* type = uint32_t */
    MIPC_SIM_UNBLOCK_PIN_CNF_T_REMAINING_ATTEMPTS           = 0x102,
};

    /* MIPC_MSG.SIM_GET_PIN_INFO_REQ */
enum mipc_sim_get_pin_info_req_tlv_enum {
    mipc_sim_get_pin_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_PIN_INFO_CNF */
enum mipc_sim_get_pin_info_cnf_tlv_enum {
    mipc_sim_get_pin_info_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_GET_PIN_INFO_CNF_T_PIN_TYPE                    = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_GET_PIN_INFO_CNF_T_PIN_STATE                   = 0x101,
    /* Remaining attempts */
    /* type = uint32_t */
    MIPC_SIM_GET_PIN_INFO_CNF_T_REMAINING_ATTEMPTS          = 0x102,
};

    /* MIPC_MSG.SIM_GET_PIN_LIST_REQ */
enum mipc_sim_get_pin_list_req_tlv_enum {
    mipc_sim_get_pin_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_PIN_LIST_CNF */
enum mipc_sim_get_pin_list_cnf_tlv_enum {
    mipc_sim_get_pin_list_cnf_tlv_NONE = 0,
    /* PIN1 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_PIN1                        = 0x100,
    /* PIN2 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_PIN2                        = 0x101,
    /* NW PIN defined in 3GPP 22.022 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_NW_PIN                      = 0x102,
    /* SUB_NW PIN defined in 3GPP 22.022 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_SUB_NW_PIN                  = 0x103,
    /* SP PIN defined in 3GPP 22.022 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_SP_PIN                      = 0x104,
    /* CORP PIN defined in 3GPP 22.022 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_CORP_PIN                    = 0x105,
    /* SIM PIN defined in 3GPP 22.022 */
    /* type = struct, refer to sim_pin_desc */
    MIPC_SIM_GET_PIN_LIST_CNF_T_SIM_PIN                     = 0x106,
};

    /* MIPC_MSG.SIM_STATE_REQ */
enum mipc_sim_state_req_tlv_enum {
    mipc_sim_state_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_STATE_CNF */
enum mipc_sim_state_cnf_tlv_enum {
    mipc_sim_state_cnf_tlv_NONE = 0,
    /* SIM slot state */
    /* type = uint8_t, refer to SIM_STATE */
    MIPC_SIM_STATE_CNF_T_STATE                              = 0x100,
    /* SIM slot ID */
    /* type = uint32_t */
    MIPC_SIM_STATE_CNF_T_SIM_ID                             = 0x101,
    /* SIM protocol ID */
    /* type = uint32_t */
    MIPC_SIM_STATE_CNF_T_PS_ID                              = 0x102,
};

    /* MIPC_MSG.SIM_STATUS_REQ */
enum mipc_sim_status_req_tlv_enum {
    mipc_sim_status_req_tlv_NONE = 0,
    /* 0: Simple mode, only return STATUS, SIM_ID, PS_ID; 1: Full mode, return all TLVs defined in MIPC_SIM_STATUS_CNF; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_STATUS_REQ_T_MODE                              = 0x100,
};

    /* MIPC_MSG.SIM_STATUS_CNF */
enum mipc_sim_status_cnf_tlv_enum {
    mipc_sim_status_cnf_tlv_NONE = 0,
    /* SIM status */
    /* type = uint8_t, refer to SIM_STATUS */
    MIPC_SIM_STATUS_CNF_T_STATUS                            = 0x100,
    /* SIM slot ID; 0-based, which means the value should start from 0 */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_SIM_ID                            = 0x101,
    /* SIM protocol ID; 0-based, which means the value should start from 0 */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PS_ID                             = 0x102,
    /* Card present state for android */
    /* type = uint8_t, refer to SIM_CARD_PRESENT_STATE */
    MIPC_SIM_STATUS_CNF_T_CARD_PRESENT_STATE                = 0x103,
    /* It is currently not supported and is always 0 */
    /* type = uint8_t, refer to PIN_STATE */
    MIPC_SIM_STATUS_CNF_T_UPIN_STATUS                       = 0x104,
    /* Test SIM or not */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_STATUS_CNF_T_TEST_SIM                          = 0x105,
    /* 0xff means invalid */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_GSM_APP_IDX                       = 0x106,
    /* 0xff means invalid */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_CDMA_APP_IDX                      = 0x107,
    /* 0xff means invalid */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_ISIM_APP_IDX                      = 0x108,
    /* Number of application */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_APP_COUNT                         = 0x109,
    /* Application description */
    /* type = struct_array, refer to app_status_desc */
    MIPC_SIM_STATUS_CNF_T_APP_LIST                          = 0x810A,
    /* EID */
    /* type = string */
    MIPC_SIM_STATUS_CNF_T_EID                               = 0x810B,
    /* ICCID string */
    /* type = string */
    MIPC_SIM_STATUS_CNF_T_ICCID                             = 0x810C,
    /* ATR string */
    /* type = string */
    MIPC_SIM_STATUS_CNF_T_ATR                               = 0x810D,
    /* MSISDN ready */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_STATUS_CNF_T_MSISDN_READY                      = 0x10E,
    /* PIN1 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PIN1                              = 0x10F,
    /* PIN2 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PIN2                              = 0x110,
    /* PUK1 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PUK1                              = 0x111,
    /* PUK2 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PUK2                              = 0x112,
    /* Application description */
    /* type = struct, refer to app_status_desc */
    MIPC_SIM_STATUS_CNF_T_APP_TLV_ARRAY                     = 0x8113,
    /* physical slot ID */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PHY_SLOT_ID                       = 0x114,
    /* Port ID, logic SE interface ID */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CNF_T_PORT_ID                           = 0x115,
    /* The MEP mode as defined in SGP.22 */
    /* type = uint8_t, refer to SIM_MEP_MODE */
    MIPC_SIM_STATUS_CNF_T_MEP_MODE                          = 0x116,
    /* EID in byte/octet16 format per spec GSMA SGP.22. This is the same EID as TLV EID but in a different format. */
    /* type = byte_array */
    MIPC_SIM_STATUS_CNF_T_EID_BYTE                          = 0x8117,
};

    /* MIPC_MSG.SIM_ICCID_REQ */
enum mipc_sim_iccid_req_tlv_enum {
    mipc_sim_iccid_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ICCID_CNF */
enum mipc_sim_iccid_cnf_tlv_enum {
    mipc_sim_iccid_cnf_tlv_NONE = 0,
    /* ICCID string */
    /* type = string */
    MIPC_SIM_ICCID_CNF_T_ICCID                              = 0x8100,
};

    /* MIPC_MSG.SIM_IMSI_REQ */
enum mipc_sim_imsi_req_tlv_enum {
    mipc_sim_imsi_req_tlv_NONE = 0,
    /* 0: GSM; 1: C2K; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_IMSI_REQ_T_MODE                                = 0x100,
    /* Only work in C2K mode; use AT+ECIMI=<APP_ID> to query IMSI if APP_ID exists; Default value is 255. */
    /* type = uint8_t, refer to SIM_APP_TYPE_EX */
    MIPC_SIM_IMSI_REQ_T_APP_ID                              = 0x101,
};

    /* MIPC_MSG.SIM_IMSI_CNF */
enum mipc_sim_imsi_cnf_tlv_enum {
    mipc_sim_imsi_cnf_tlv_NONE = 0,
    /* IMSI string */
    /* type = string */
    MIPC_SIM_IMSI_CNF_T_IMSI                                = 0x8100,
    /* MNC length */
    /* type = uint8_t */
    MIPC_SIM_IMSI_CNF_T_MNC_LEN                             = 0x101,
};

    /* MIPC_MSG.SIM_MSISDN_REQ */
enum mipc_sim_msisdn_req_tlv_enum {
    mipc_sim_msisdn_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_MSISDN_CNF */
enum mipc_sim_msisdn_cnf_tlv_enum {
    mipc_sim_msisdn_cnf_tlv_NONE = 0,
    /* msisdn count,support maxium 7 MSISDN  */
    /* type = uint8_t */
    MIPC_SIM_MSISDN_CNF_T_MSISDN_COUNT                      = 0x100,
    /* MSISDN list; the maximum length of an MSISDN is 24; MSISDN may not exist in SIM card */
    /* type = struct_array, refer to sim_msisdn */
    MIPC_SIM_MSISDN_CNF_T_MSISDN_LIST                       = 0x8101,
    /* MSISDN list; the maximum length of an MSISDN is 24; MSISDN may not exist in SIM card */
    /* type = struct, refer to sim_msisdn */
    MIPC_SIM_MSISDN_CNF_T_MSISDN_TLV_ARRAY                  = 0x8104,
};

    /* MIPC_MSG.SIM_GET_ATR_INFO_REQ */
enum mipc_sim_get_atr_info_req_tlv_enum {
    mipc_sim_get_atr_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_ATR_INFO_CNF */
enum mipc_sim_get_atr_info_cnf_tlv_enum {
    mipc_sim_get_atr_info_cnf_tlv_NONE = 0,
    /* Length of ATR */
    /* type = uint32_t */
    MIPC_SIM_GET_ATR_INFO_CNF_T_ATR_LEN                     = 0x100,
    /* ATR string */
    /* type = string */
    MIPC_SIM_GET_ATR_INFO_CNF_T_ATR                         = 0x8101,
};

    /* MIPC_MSG.SIM_OPEN_CHANNEL_REQ */
enum mipc_sim_open_channel_req_tlv_enum {
    mipc_sim_open_channel_req_tlv_NONE = 0,
    /* Length of application ID; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID_LEN                  = 0x100,
    /* Application ID; this field is used to select AID after open channel operation is executed; if APP_ID_LEN = 0, only open channel operation is executed, the select AID operation is not executed; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_OPEN_CHANNEL_REQ_T_APP_ID                      = 0x101,
    /* P2 defined in SIM SPEC(102.221); this field is used in selecting AID command after open channel operation is executed; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_OPEN_CHANNEL_REQ_T_P2                          = 0x102,
    /* Channel group of the channel ID, this group is optional; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_OPEN_CHANNEL_REQ_T_CHANNEL_GROUP               = 0x103,
};

    /* MIPC_MSG.SIM_OPEN_CHANNEL_CNF */
enum mipc_sim_open_channel_cnf_tlv_enum {
    mipc_sim_open_channel_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_SIM_OPEN_CHANNEL_CNF_T_SW                          = 0x100,
    /* Channel ID */
    /* type = uint8_t */
    MIPC_SIM_OPEN_CHANNEL_CNF_T_CHANNEL                     = 0x101,
    /* Response of open channel */
    /* type = byte_array */
    MIPC_SIM_OPEN_CHANNEL_CNF_T_RESP                        = 0x102,
};

    /* MIPC_MSG.SIM_CLOSE_CHANNEL_REQ */
enum mipc_sim_close_channel_req_tlv_enum {
    mipc_sim_close_channel_req_tlv_NONE = 0,
    /* Channel ID */
    /* type = uint8_t */
    MIPC_SIM_CLOSE_CHANNEL_REQ_T_CHANNEL_ID                 = 0x100,
    /* Close a group of channel; if CHANNEL_ID is not 0, close channel ID only; if CHANNEL_ID is 0 and CHANNEL_GROUP is not 0, all the channels of the group will be closed; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_CLOSE_CHANNEL_REQ_T_CHANNEL_GROUP              = 0x101,
    /* This field indicates if this command is es10 command */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_CLOSE_CHANNEL_REQ_T_IS_ES10                    = 0x102,
};

    /* MIPC_MSG.SIM_CLOSE_CHANNEL_CNF */
enum mipc_sim_close_channel_cnf_tlv_enum {
    mipc_sim_close_channel_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_SIM_CLOSE_CHANNEL_CNF_T_SW                         = 0x100,
};

    /* MIPC_MSG.SIM_CHANNEL_RESTRICTED_ACCESS_REQ */
enum mipc_sim_channel_restricted_access_req_tlv_enum {
    mipc_sim_channel_restricted_access_req_tlv_NONE = 0,
    /* Application ID */
    /* type = uint8_t, refer to SIM_APP_TYPE */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_APP_ID         = 0x100,
    /* Channel ID of the application; Default value is 0xFF. */
    /* type = uint8_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_SESSION_ID     = 0x101,
    /* Command */
    /* type = uint8_t, refer to SIM_ACCESS_COMMAND */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_CMD            = 0x102,
    /* File ID */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FILE_ID        = 0x103,
    /* P1 of APDU */
    /* type = uint8_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P1             = 0x104,
    /* P2 of APDU */
    /* type = uint8_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P2             = 0x105,
    /* P3 of APDU; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3             = 0x106,
    /* Data length in DATA */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN       = 0x107,
    /* Command data; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA           = 0x8108,
    /* File path; Default value is null character. */
    /* type = string */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH           = 0x109,
    /* P3 of APDU, support more than 256 bytes; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX          = 0x10A,
    /* It indicates whether to convert USIM FCP to SIM format or not, when the command type is "get response"; default value is 0 */
    /* type = uint8_t, refer to SIM_FCP_CONVERT */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FCP_CONVERT    = 0x10B,
};

    /* MIPC_MSG.SIM_CHANNEL_RESTRICTED_ACCESS_CNF */
enum mipc_sim_channel_restricted_access_cnf_tlv_enum {
    mipc_sim_channel_restricted_access_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_SW             = 0x100,
    /* Response length of APDU */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_LEN       = 0x101,
    /* Response APDU */
    /* type = byte_array */
    MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_APDU      = 0x8102,
};

    /* MIPC_MSG.SIM_CHANNEL_GENERIC_ACCESS_REQ */
enum mipc_sim_channel_generic_access_req_tlv_enum {
    mipc_sim_channel_generic_access_req_tlv_NONE = 0,
    /* Application ID */
    /* type = uint8_t, refer to SIM_APP_TYPE */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_APP_ID            = 0x100,
    /* Channel ID */
    /* type = uint8_t */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_CHANNEL_ID        = 0x101,
    /* APDU length */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_APDU_LEN          = 0x102,
    /* APDU to be sent to SIM card */
    /* type = byte_array */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ_T_APDU              = 0x8103,
};

    /* MIPC_MSG.SIM_CHANNEL_GENERIC_ACCESS_CNF */
enum mipc_sim_channel_generic_access_cnf_tlv_enum {
    mipc_sim_channel_generic_access_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF_T_SW                = 0x100,
    /* Response APDU length */
    /* type = uint16_t */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_LEN          = 0x101,
    /* Response APDU returned by SIM card */
    /* type = byte_array */
    MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_APDU         = 0x8102,
};

    /* MIPC_MSG.SIM_LONG_APDU_ACCESS_REQ */
enum mipc_sim_long_apdu_access_req_tlv_enum {
    mipc_sim_long_apdu_access_req_tlv_NONE = 0,
    /* Always 1 */
    /* type = uint8_t */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_VERSION                 = 0x100,
    /* Application ID length; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_APP_ID_LEN              = 0x101,
    /* Application ID */
    /* type = byte_array */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_APP_ID                  = 0x102,
    /* File path */
    /* type = string */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_PATH_ID                 = 0x103,
    /* File id */
    /* type = uint16_t */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_FILE_ID                 = 0x104,
    /* File offset */
    /* type = uint16_t */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_FILE_OFFSET             = 0x105,
    /* Number of bytes to be returned by SIM card; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_NUMBER_OF_BYTES         = 0x106,
    /* PIN2; Default value is null character. */
    /* type = string */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_LOCAL_PIN               = 0x107,
    /* Length of binary data to be sent to SIM card; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_BINARY_DATA_LEN         = 0x108,
    /* Binary data to be sent to SIM card; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_LONG_APDU_ACCESS_REQ_T_BINARY_DATA             = 0x8109,
};

    /* MIPC_MSG.SIM_LONG_APDU_ACCESS_CNF */
enum mipc_sim_long_apdu_access_cnf_tlv_enum {
    mipc_sim_long_apdu_access_cnf_tlv_NONE = 0,
    /* Always 1 */
    /* type = uint8_t */
    MIPC_SIM_LONG_APDU_ACCESS_CNF_T_VERSION                 = 0x100,
    /* Status word */
    /* type = uint16_t */
    MIPC_SIM_LONG_APDU_ACCESS_CNF_T_SW                      = 0x101,
    /* Reponse data length */
    /* type = uint16_t */
    MIPC_SIM_LONG_APDU_ACCESS_CNF_T_DATA_LEN                = 0x102,
    /* Response data from SIM card */
    /* type = byte_array */
    MIPC_SIM_LONG_APDU_ACCESS_CNF_T_DATA                    = 0x8103,
};

    /* MIPC_MSG.SIM_APP_LIST_REQ */
enum mipc_sim_app_list_req_tlv_enum {
    mipc_sim_app_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_APP_LIST_CNF */
enum mipc_sim_app_list_cnf_tlv_enum {
    mipc_sim_app_list_cnf_tlv_NONE = 0,
    /* Always 1 */
    /* type = uint8_t */
    MIPC_SIM_APP_LIST_CNF_T_VERSION                         = 0x100,
    /* Application count */
    /* type = uint8_t */
    MIPC_SIM_APP_LIST_CNF_T_APP_COUNT                       = 0x101,
    /* Active application index in application list */
    /* type = uint8_t */
    MIPC_SIM_APP_LIST_CNF_T_ACTIVE_APP_idx                  = 0x102,
    /* Application list */
    /* type = struct_array, refer to sim_app_info */
    MIPC_SIM_APP_LIST_CNF_T_APP_LIST                        = 0x103,
    /* Application list */
    /* type = struct, refer to sim_app_info */
    MIPC_SIM_APP_LIST_CNF_T_APP_TLV_ARRAY                   = 0x104,
};

    /* MIPC_MSG.SIM_FILE_STATUS_REQ */
enum mipc_sim_file_status_req_tlv_enum {
    mipc_sim_file_status_req_tlv_NONE = 0,
    /* Always 1 */
    /* type = uint8_t */
    MIPC_SIM_FILE_STATUS_REQ_T_VERSION                      = 0x100,
    /* AID length; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_FILE_STATUS_REQ_T_AID_LEN                      = 0x101,
    /* AID */
    /* type = byte_array */
    MIPC_SIM_FILE_STATUS_REQ_T_AID                          = 0x102,
    /* Length of file path; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_FILE_STATUS_REQ_T_FILE_PATH_LEN                = 0x103,
    /* File path */
    /* type = string */
    MIPC_SIM_FILE_STATUS_REQ_T_FILE_PATH                    = 0x104,
};

    /* MIPC_MSG.SIM_FILE_STATUS_CNF */
enum mipc_sim_file_status_cnf_tlv_enum {
    mipc_sim_file_status_cnf_tlv_NONE = 0,
    /* Always 1 */
    /* type = uint8_t */
    MIPC_SIM_FILE_STATUS_CNF_T_VERSION                      = 0x101,
    /* Status word */
    /* type = uint16_t */
    MIPC_SIM_FILE_STATUS_CNF_T_SW                           = 0x102,
    /* Types of file accessibility */
    /* type = uint8_t, refer to SIM_FILE_ACCESSIBILITY */
    MIPC_SIM_FILE_STATUS_CNF_T_FILE_ACCESSIBILITY           = 0x103,
    /* File type */
    /* type = uint8_t, refer to SIM_FILE_TYPE */
    MIPC_SIM_FILE_STATUS_CNF_T_FILE_TYPE                    = 0x104,
    /* File structure */
    /* type = uint8_t, refer to SIM_FILE_STRUCTURE */
    MIPC_SIM_FILE_STATUS_CNF_T_FILE                         = 0x105,
    /* The number of items in the UICC file; it is set to 1 for transparent and TLV files */
    /* type = uint8_t */
    MIPC_SIM_FILE_STATUS_CNF_T_ITEM_COUNT                   = 0x106,
    /* Will be phased out. SIZE_EXT is suggested. The size of each item in bytes; for transparent or TLV files, it is the size of the entire EF; for record-based files, it represents the total number of records */
    /* type = uint8_t */
    MIPC_SIM_FILE_STATUS_CNF_T_SIZE                         = 0x107,
    /*  An array of type mipc_sim_pin_type_enum that describes the access condition for each operation (READ, UPDATE, ACTIVATE, and DEACTIVATE in that order) applied on specific file */
    /* type = byte_array, refer to SIM_PIN_TYPE */
    MIPC_SIM_FILE_STATUS_CNF_T_LOCK_STATUS                  = 0x108,
    /* The size of each item in bytes; for transparent or TLV files, it is the size of the entire EF; for record-based files, it represents the total number of records */
    /* type = uint16_t */
    MIPC_SIM_FILE_STATUS_CNF_T_SIZE_EXT                     = 0x109,
};

    /* MIPC_MSG.SIM_GET_RESET_REQ */
enum mipc_sim_get_reset_req_tlv_enum {
    mipc_sim_get_reset_req_tlv_NONE = 0,
    /* 0: Query pass-through mode status; 1: Query SIM application/power status; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_GET_RESET_REQ_T_TYPE                           = 0x100,
};

    /* MIPC_MSG.SIM_GET_RESET_CNF */
enum mipc_sim_get_reset_cnf_tlv_enum {
    mipc_sim_get_reset_cnf_tlv_NONE = 0,
    /* Return the status of SIM pass-through mode or SIM power mode */
    /* type = uint8_t, refer to SIM_PASS_THROUGH_MODE */
    MIPC_SIM_GET_RESET_CNF_T_MODE                           = 0x100,
};

    /* MIPC_MSG.SIM_SET_RESET_REQ */
enum mipc_sim_set_reset_req_tlv_enum {
    mipc_sim_set_reset_req_tlv_NONE = 0,
    /* Set the status of SIM pass-through mode or SIM power mode */
    /* type = uint8_t, refer to SIM_PASS_THROUGH_MODE */
    MIPC_SIM_SET_RESET_REQ_T_MODE                           = 0x100,
};

    /* MIPC_MSG.SIM_SET_RESET_CNF */
enum mipc_sim_set_reset_cnf_tlv_enum {
    mipc_sim_set_reset_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_TERMINAL_CAPABILITY_REQ */
enum mipc_sim_get_terminal_capability_req_tlv_enum {
    mipc_sim_get_terminal_capability_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_TERMINAL_CAPABILITY_CNF */
enum mipc_sim_get_terminal_capability_cnf_tlv_enum {
    mipc_sim_get_terminal_capability_cnf_tlv_NONE = 0,
    /* Terminal capability object count */
    /* type = uint8_t */
    MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF_T_COUNT            = 0x100,
    /* Data length of terminal capability */
    /* type = uint16_t */
    MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF_T_TC_LEN           = 0x101,
    /* Terminal capability */
    /* type = byte_array */
    MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF_T_TC               = 0x102,
};

    /* MIPC_MSG.SIM_SET_TERMINAL_CAPABILITY_REQ */
enum mipc_sim_set_terminal_capability_req_tlv_enum {
    mipc_sim_set_terminal_capability_req_tlv_NONE = 0,
    /* Terminal capability object count */
    /* type = uint8_t */
    MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ_T_COUNT            = 0x100,
    /* Length of terminal capability data; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ_T_TC_LEN           = 0x101,
    /* Terminal capability */
    /* type = byte_array */
    MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ_T_TC               = 0x102,
};

    /* MIPC_MSG.SIM_SET_TERMINAL_CAPABILITY_CNF */
enum mipc_sim_set_terminal_capability_cnf_tlv_enum {
    mipc_sim_set_terminal_capability_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_SET_PIN_EX_REQ */
enum mipc_sim_set_pin_ex_req_tlv_enum {
    mipc_sim_set_pin_ex_req_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_SET_PIN_EX_REQ_T_PIN_TYPE                      = 0x100,
    /* PIN operation */
    /* type = uint8_t, refer to SIM_PIN_OPERATION */
    MIPC_SIM_SET_PIN_EX_REQ_T_OP                            = 0x101,
    /*
      PIN code is required in 3 situations:
      1. Enable/Disable PIN
      2. Verify the old PIN when the PIN is change
      3. Verify PUK code when the operation is to unblock PIN
    */
    /* type = string */
    MIPC_SIM_SET_PIN_EX_REQ_T_PIN_CODE                      = 0x8102,
    /* New PIN code is required when the operation is "change PIN" or "unblock PIN" */
    /* type = string */
    MIPC_SIM_SET_PIN_EX_REQ_T_NEW_PIN_CODE                  = 0x8103,
    /* AID length; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_SET_PIN_EX_REQ_T_AID_LEN                       = 0x104,
    /* Application ID */
    /* type = byte_array */
    MIPC_SIM_SET_PIN_EX_REQ_T_AID                           = 0x105,
};

    /* MIPC_MSG.SIM_SET_PIN_EX_CNF */
enum mipc_sim_set_pin_ex_cnf_tlv_enum {
    mipc_sim_set_pin_ex_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_SET_PIN_EX_CNF_T_PIN_TYPE                      = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_SET_PIN_EX_CNF_T_PIN_STATE                     = 0x101,
    /* The number of remaining valid tries for PIN_TYPE */
    /* type = uint32_t */
    MIPC_SIM_SET_PIN_EX_CNF_T_REMAINING_ATTEMPTS            = 0x102,
};

    /* MIPC_MSG.SIM_GET_PIN_EX_REQ */
enum mipc_sim_get_pin_ex_req_tlv_enum {
    mipc_sim_get_pin_ex_req_tlv_NONE = 0,
    /* Reserved for future use, not used now */
    /* type = uint8_t */
    MIPC_SIM_GET_PIN_EX_REQ_T_VERSION                       = 0x100,
    /* AID length; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_GET_PIN_EX_REQ_T_AID_LEN                       = 0x101,
    /* Application ID */
    /* type = byte_array */
    MIPC_SIM_GET_PIN_EX_REQ_T_AID                           = 0x102,
};

    /* MIPC_MSG.SIM_GET_PIN_EX_CNF */
enum mipc_sim_get_pin_ex_cnf_tlv_enum {
    mipc_sim_get_pin_ex_cnf_tlv_NONE = 0,
    /* PIN type */
    /* type = uint8_t, refer to SIM_PIN_TYPE */
    MIPC_SIM_GET_PIN_EX_CNF_T_PIN_TYPE                      = 0x100,
    /* PIN state */
    /* type = uint8_t, refer to SIM_PIN_STATE */
    MIPC_SIM_GET_PIN_EX_CNF_T_PIN_STATE                     = 0x101,
    /* The number of remaining valid tries for PIN_TYPE */
    /* type = uint32_t */
    MIPC_SIM_GET_PIN_EX_CNF_T_REMAINING_ATTEMPTS            = 0x102,
};

    /* MIPC_MSG.SIM_GET_GSM_AUTH_REQ */
enum mipc_sim_get_gsm_auth_req_tlv_enum {
    mipc_sim_get_gsm_auth_req_tlv_NONE = 0,
    /* Fixed length: 16 bytes */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND1                       = 0x8100,
    /* Fixed length: 16 bytes; if RAND2 exists, SIM card will run GSM algorithm twice; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND2                       = 0x8101,
    /* Fixed length: 16 bytes; if RAND2 and RAND3 exist, SIM card will run GSM algorithm 3 times; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_REQ_T_RAND3                       = 0x8102,
};

    /* MIPC_MSG.SIM_GET_GSM_AUTH_CNF */
enum mipc_sim_get_gsm_auth_cnf_tlv_enum {
    mipc_sim_get_gsm_auth_cnf_tlv_NONE = 0,
    /* Status word */
    /* type = uint16_t */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_SW                          = 0x100,
    /* Fixed length: 4 bytes */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES1                       = 0x101,
    /* Fixed length: 8 bytes */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_KC1                         = 0x102,
    /* Fixed length: 4 bytes; this field exists only when RAND2 is sent */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES2                       = 0x103,
    /* Fixed length: 8 bytes; this field exists only when RAND2 is sent */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_KC2                         = 0x104,
    /* Fixed length: 4 bytes; this field exists only when RAND3 is sent */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_SRES3                       = 0x105,
    /* Fixed length: 8 bytes; this field exists only when RAND3 is sent */
    /* type = byte_array */
    MIPC_SIM_GET_GSM_AUTH_CNF_T_KC3                         = 0x106,
};

    /* MIPC_MSG.SIM_GET_EXT_AUTH_REQ */
enum mipc_sim_get_ext_auth_req_tlv_enum {
    mipc_sim_get_ext_auth_req_tlv_NONE = 0,
    /* Channel ID; it is suggested that either CH or APP_ID should be carried; if neither CH nor APP_ID is carried, default channel will be used; if both parameters are carried, CH will be used; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_GET_EXT_AUTH_REQ_T_CH                          = 0x100,
    /* Mode defined in 31.102 section 7.1.2 */
    /* type = uint8_t */
    MIPC_SIM_GET_EXT_AUTH_REQ_T_MODE                        = 0x101,
    /* Data length of CMD_DATA; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_LEN                     = 0x102,
    /* Authentication data defined in 31.102 section 7.1.2 */
    /* type = byte_array */
    MIPC_SIM_GET_EXT_AUTH_REQ_T_CMD_DATA                    = 0x8103,
    /* Application ID; it is suggested that either CH or APP_ID should be carried; if neither CH nor APP_ID is carried, default channel will be used; if both parameters are carried, CH will be used; Default value is 1. */
    /* type = uint8_t, refer to SIM_APP_TYPE */
    MIPC_SIM_GET_EXT_AUTH_REQ_T_APP_ID                      = 0x104,
};

    /* MIPC_MSG.SIM_GET_EXT_AUTH_CNF */
enum mipc_sim_get_ext_auth_cnf_tlv_enum {
    mipc_sim_get_ext_auth_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_SIM_GET_EXT_AUTH_CNF_T_SW                          = 0x100,
    /* Response length */
    /* type = uint16_t */
    MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_LEN                     = 0x101,
    /* Response returned from SIM card */
    /* type = byte_array */
    MIPC_SIM_GET_EXT_AUTH_CNF_T_RSP_DATA                    = 0x8102,
};

    /* MIPC_MSG.SIM_GET_FACILITY_REQ */
enum mipc_sim_get_facility_req_tlv_enum {
    mipc_sim_get_facility_req_tlv_NONE = 0,
    /* Application ID */
    /* type = uint8_t, refer to SIM_APP_TYPE */
    MIPC_SIM_GET_FACILITY_REQ_T_APP_ID                      = 0x100,
    /* Support FD, SC, AO, OI, OX, AI, and IR */
    /* type = string */
    MIPC_SIM_GET_FACILITY_REQ_T_FACILITY                    = 0x101,
};

    /* MIPC_MSG.SIM_GET_FACILITY_CNF */
enum mipc_sim_get_facility_cnf_tlv_enum {
    mipc_sim_get_facility_cnf_tlv_NONE = 0,
    /* 0: Not active; 1: Active */
    /* type = uint8_t */
    MIPC_SIM_GET_FACILITY_CNF_T_STATUS                      = 0x100,
};

    /* MIPC_MSG.SIM_SET_FACILITY_REQ */
enum mipc_sim_set_facility_req_tlv_enum {
    mipc_sim_set_facility_req_tlv_NONE = 0,
    /*  */
    /* type = uint8_t, refer to SIM_APP_TYPE */
    MIPC_SIM_SET_FACILITY_REQ_T_APP_ID                      = 0x100,
    /* Support FD SC, AO, OI, OX, AI, and IR */
    /* type = string */
    MIPC_SIM_SET_FACILITY_REQ_T_FACILITY                    = 0x101,
    /* PIN1 or PIN2 code; Default value is null character. */
    /* type = string */
    MIPC_SIM_SET_FACILITY_REQ_T_PASS_WORD                   = 0x8102,
    /* 0: Disable; 1: Enable; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_SET_FACILITY_REQ_T_MODE                        = 0x103,
};

    /* MIPC_MSG.SIM_SET_FACILITY_CNF */
enum mipc_sim_set_facility_cnf_tlv_enum {
    mipc_sim_set_facility_cnf_tlv_NONE = 0,
    /* Remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_SET_FACILITY_CNF_T_RETRY_COUNT                 = 0x100,
};

    /* MIPC_MSG.SIM_GET_EUICC_SLOTS_STATUS_REQ */
enum mipc_sim_get_euicc_slots_status_req_tlv_enum {
    mipc_sim_get_euicc_slots_status_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_EUICC_SLOTS_STATUS_CNF */
enum mipc_sim_get_euicc_slots_status_cnf_tlv_enum {
    mipc_sim_get_euicc_slots_status_cnf_tlv_NONE = 0,
    /*  Number of physical slots */
    /* type = uint8_t */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_COUNT  = 0x100,
    /* Slot info for all slots */
    /* type = struct_array, refer to sim_slots_info */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_LIST   = 0x8101,
    /* SIM card state */
    /* type = uint8_t, refer to SIM_CARD_PRESENT_STATE */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_CARD_STATE        = 0x102,
    /* Slots state of physical slots; 0: inactive; 1: active */
    /* type = uint8_t */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_STATE       = 0x103,
    /* Logical slots index of physical slot */
    /* type = uint8_t */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_LOGICAL_IDX       = 0x104,
    /* The ATR of the SIM card */
    /* type = string */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ATR               = 0x8105,
    /* The EID of the SIM card */
    /* type = string */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_EID               = 0x8106,
    /* The ICCID of the SIM card */
    /* type = string */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_ICCID             = 0x8107,
    /* Slot info for all slots */
    /* type = struct, refer to sim_slots_info */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_SLOTS_INFO_TLV_ARRAY = 0x8108,
    /* The EID of SIM cards. The index of this array is aligned with the slot-info index of SLOTS_INFO_TLV_ARRAY */
    /* type = struct, refer to eid_byte_struct */
    MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF_T_EID_BYTE_TLV_ARRAY = 0x8109,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_CONNECT_REQ */
enum mipc_sim_access_profile_connect_req_tlv_enum {
    mipc_sim_access_profile_connect_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_CONNECT_CNF */
enum mipc_sim_access_profile_connect_cnf_tlv_enum {
    mipc_sim_access_profile_connect_cnf_tlv_NONE = 0,
    /* It indicates the transport protocol used; 0x00: T=0; 0x01: T=1 */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_CUR_TYPE          = 0x100,
    /* It indicates the capability of transport protocol in SIM card; 0x00: T=0; 0x01: T=1; 0x02: both T=0 and T=1 are supported */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_SUPPORT_TYPE      = 0x101,
    /* The ATR response from SIM card */
    /* type = string */
    MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF_T_ATR               = 0x102,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_DISCONNECT_REQ */
enum mipc_sim_access_profile_disconnect_req_tlv_enum {
    mipc_sim_access_profile_disconnect_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_DISCONNECT_CNF */
enum mipc_sim_access_profile_disconnect_cnf_tlv_enum {
    mipc_sim_access_profile_disconnect_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_POWER_ON_REQ */
enum mipc_sim_access_profile_power_on_req_tlv_enum {
    mipc_sim_access_profile_power_on_req_tlv_NONE = 0,
    /* It indicates the transport protocol used, 0x00: T=0; 0x01: T=1 */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_POWER_ON_REQ_T_TYPE             = 0x100,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_POWER_ON_CNF */
enum mipc_sim_access_profile_power_on_cnf_tlv_enum {
    mipc_sim_access_profile_power_on_cnf_tlv_NONE = 0,
    /* It indicates the capability of transport protocol in the SIM card; 0x00: T=0; 0x01: T=1; 0x02: both T=0 and T=1 are supported */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF_T_CUR_TYPE         = 0x101,
    /* The ATR response from SIM card */
    /* type = string */
    MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF_T_ATR              = 0x102,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_POWER_OFF_REQ */
enum mipc_sim_access_profile_power_off_req_tlv_enum {
    mipc_sim_access_profile_power_off_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_POWER_OFF_CNF */
enum mipc_sim_access_profile_power_off_cnf_tlv_enum {
    mipc_sim_access_profile_power_off_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_RESET_REQ */
enum mipc_sim_access_profile_reset_req_tlv_enum {
    mipc_sim_access_profile_reset_req_tlv_NONE = 0,
    /* It indicates the transport protocol used; 0x00: T=0; 0x01: T=1 */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_RESET_REQ_T_TYPE                = 0x100,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_RESET_CNF */
enum mipc_sim_access_profile_reset_cnf_tlv_enum {
    mipc_sim_access_profile_reset_cnf_tlv_NONE = 0,
    /* It indicates the capability of transport protocol in SIM card; 0x00: T=0; 0x01: T=1; 0x02: both T=0 and T=1 are supported */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_RESET_CNF_T_CUR_TYPE            = 0x101,
    /* The ATR response from SIM card */
    /* type = string */
    MIPC_SIM_ACCESS_PROFILE_RESET_CNF_T_ATR                 = 0x102,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_APDU_REQ */
enum mipc_sim_access_profile_apdu_req_tlv_enum {
    mipc_sim_access_profile_apdu_req_tlv_NONE = 0,
    /* It indicates the capability of transport protocol in SIM card; 0x00: T=0; 0x01: T=1; 0x02: both T=0 and T=1 are supported */
    /* type = uint8_t */
    MIPC_SIM_ACCESS_PROFILE_APDU_REQ_T_TYPE                 = 0x100,
    /* The APDU to be sent to SIM card */
    /* type = string */
    MIPC_SIM_ACCESS_PROFILE_APDU_REQ_T_APDU                 = 0x101,
};

    /* MIPC_MSG.SIM_ACCESS_PROFILE_APDU_CNF */
enum mipc_sim_access_profile_apdu_cnf_tlv_enum {
    mipc_sim_access_profile_apdu_cnf_tlv_NONE = 0,
    /* The APDU returned from SIM card */
    /* type = string */
    MIPC_SIM_ACCESS_PROFILE_APDU_CNF_T_APDU                 = 0x100,
};

    /* MIPC_MSG.SIM_SET_SIM_POWER_REQ */
enum mipc_sim_set_sim_power_req_tlv_enum {
    mipc_sim_set_sim_power_req_tlv_NONE = 0,
    /* MODE 0: for RF operation only; MODE 1: for both SIM and RF operation; MODE 2: Power-off/power-on without eSIMs URC until modem is unplugged from SIM card or modem is plugged into SIM card; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_SET_SIM_POWER_REQ_T_MODE                       = 0x100,
    /* SIM power state */
    /* type = uint8_t, refer to SIM_POWER_STATE */
    MIPC_SIM_SET_SIM_POWER_REQ_T_SIM_POWER                  = 0x101,
};

    /* MIPC_MSG.SIM_SET_SIM_POWER_CNF */
enum mipc_sim_set_sim_power_cnf_tlv_enum {
    mipc_sim_set_sim_power_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ */
enum mipc_sim_set_physical_slots_mapping_req_tlv_enum {
    mipc_sim_set_physical_slots_mapping_req_tlv_NONE = 0,
    /* slots number */
    /* type = uint8_t */
    MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ_T_SLOTS_NUM     = 0x100,
    /* 0-based,slots_mapping_list = {1,0} means sim slot0->physical slot1, sim slot1->pysical slot0 */
    /* type = byte_array */
    MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ_T_SLOTS_MAPPING_LIST = 0x101,
};

    /* MIPC_MSG.SIM_SET_PHYSICAL_SLOTS_MAPPING_CNF */
enum mipc_sim_set_physical_slots_mapping_cnf_tlv_enum {
    mipc_sim_set_physical_slots_mapping_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ */
enum mipc_sim_extended_channel_generic_access_req_tlv_enum {
    mipc_sim_extended_channel_generic_access_req_tlv_NONE = 0,
    /* Channel ID  */
    /* type = uint8_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_SESSION_ID = 0x100,
    /* CLA byte of APDU which will be sent to SIM card */
    /* type = uint8_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_CLA      = 0x101,
    /* INS byte of APDU which will be sent to SIM card */
    /* type = uint8_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_INS      = 0x102,
    /* P1 byte of APDU which will be sent to SIM card */
    /* type = uint8_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P1       = 0x103,
    /* P2 byte of APDU which will be sent to SIM card */
    /* type = uint8_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P2       = 0x104,
    /* P3 byte of APDU which will be sent to SIM card; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_P3       = 0x105,
    /* Data length in DATA; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA_LEN = 0x106,
    /* Command data in APDU; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_DATA     = 0x8107,
    /* indicates if this command is es10 command */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ_T_IS_ES10  = 0x108,
};

    /* MIPC_MSG.SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF */
enum mipc_sim_extended_channel_generic_access_cnf_tlv_enum {
    mipc_sim_extended_channel_generic_access_cnf_tlv_NONE = 0,
    /* Status word responded from UICC */
    /* type = uint16_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_SW       = 0x100,
    /* Response length of APDU */
    /* type = uint16_t */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_LEN = 0x101,
    /* Response APDU */
    /* type = byte_array */
    MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF_T_RESP_APDU = 0x8102,
};

    /* MIPC_MSG.SIM_UICC_FILE_ACCESS_RECORD_REQ */
enum mipc_sim_uicc_file_access_record_req_tlv_enum {
    mipc_sim_uicc_file_access_record_req_tlv_NONE = 0,
    /* Application ID */
    /* type = byte_array */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_APP_ID           = 0x100,
    /* Length of APP_ID; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_APP_ID_LEN       = 0x101,
    /* File ID */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_FILE_ID          = 0x102,
    /* Record number */
    /* type = uint8_t */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_RECORD_NUM       = 0x103,
    /* Length of data; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_DATA_LEN         = 0x104,
    /* Command data; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_DATA             = 0x105,
    /* Path; Default value is null character. */
    /* type = string */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_PATH             = 0x106,
    /* Local PIN code; Default value is null character. */
    /* type = string */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ_T_PIN2             = 0x107,
};

    /* MIPC_MSG.SIM_UICC_FILE_ACCESS_RECORD_CNF */
enum mipc_sim_uicc_file_access_record_cnf_tlv_enum {
    mipc_sim_uicc_file_access_record_cnf_tlv_NONE = 0,
    /* Status word */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF_T_SW               = 0x100,
    /* Response length of APDU */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF_T_RESP_LEN         = 0x101,
    /* Response APDU */
    /* type = byte_array */
    MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF_T_RESP_APDU        = 0x102,
};

    /* MIPC_MSG.SIM_UICC_FILE_ACCESS_BINARY_REQ */
enum mipc_sim_uicc_file_access_binary_req_tlv_enum {
    mipc_sim_uicc_file_access_binary_req_tlv_NONE = 0,
    /* Application ID */
    /* type = byte_array */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_APP_ID           = 0x100,
    /* Length of APP_ID; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_APP_ID_LEN       = 0x101,
    /* File ID */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_FILE_ID          = 0x102,
    /* Offset */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_OFFSET           = 0x103,
    /* Length of data; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_DATA_LEN         = 0x104,
    /* Command data; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_DATA             = 0x105,
    /* Path; Default value is null character. */
    /* type = string */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_PATH             = 0x106,
    /* Local PIN code; Default value is null character. */
    /* type = string */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ_T_PIN2             = 0x107,
};

    /* MIPC_MSG.SIM_UICC_FILE_ACCESS_BINARY_CNF */
enum mipc_sim_uicc_file_access_binary_cnf_tlv_enum {
    mipc_sim_uicc_file_access_binary_cnf_tlv_NONE = 0,
    /* Status word */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF_T_SW               = 0x100,
    /* Response length of APDU */
    /* type = uint16_t */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF_T_RESP_LEN         = 0x101,
    /* Response APDU */
    /* type = byte_array */
    MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF_T_RESP_APDU        = 0x102,
};

    /* MIPC_MSG.SIM_GET_PHYSICAL_SLOTS_MAPPING_REQ */
enum mipc_sim_get_physical_slots_mapping_req_tlv_enum {
    mipc_sim_get_physical_slots_mapping_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_PHYSICAL_SLOTS_MAPPING_CNF */
enum mipc_sim_get_physical_slots_mapping_cnf_tlv_enum {
    mipc_sim_get_physical_slots_mapping_cnf_tlv_NONE = 0,
    /* 0-based, physical slot id, */
    /* type = uint8_t */
    MIPC_SIM_GET_PHYSICAL_SLOTS_MAPPING_CNF_T_ACTIVE_PHYSICAL_SLOT_ID = 0x101,
};

    /* MIPC_MSG.SIM_GET_SIM_AUTH_REQ */
enum mipc_sim_get_sim_auth_req_tlv_enum {
    mipc_sim_get_sim_auth_req_tlv_NONE = 0,
    /* P2 value of authentication command; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_GET_SIM_AUTH_REQ_T_P2                          = 0x100,
    /* AID Valuse; refer to ESTI 102.221 8.1 and 102.220; Default value is null character. */
    /* type = string */
    MIPC_SIM_GET_SIM_AUTH_REQ_T_AID                         = 0x101,
    /* Data length of CMD_DATA; Default value is 0. */
    /* type = uint16_t */
    MIPC_SIM_GET_SIM_AUTH_REQ_T_CMD_LEN                     = 0x102,
    /* Authentication data defined in 31.102 section 7.1.2; Default value is NULL. */
    /* type = byte_array */
    MIPC_SIM_GET_SIM_AUTH_REQ_T_CMD_DATA                    = 0x8103,
};

    /* MIPC_MSG.SIM_GET_SIM_AUTH_CNF */
enum mipc_sim_get_sim_auth_cnf_tlv_enum {
    mipc_sim_get_sim_auth_cnf_tlv_NONE = 0,
    /* Status word */
    /* type = uint16_t */
    MIPC_SIM_GET_SIM_AUTH_CNF_T_SW                          = 0x100,
    /* Response length */
    /* type = uint16_t */
    MIPC_SIM_GET_SIM_AUTH_CNF_T_RSP_LEN                     = 0x101,
    /* Response returned from SIM card */
    /* type = byte_array */
    MIPC_SIM_GET_SIM_AUTH_CNF_T_RSP_DATA                    = 0x8102,
};

    /* MIPC_MSG.SIM_SML_GET_ALLOWED_CARRIERS_REQ */
enum mipc_sim_sml_get_allowed_carriers_req_tlv_enum {
    mipc_sim_sml_get_allowed_carriers_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_SML_GET_ALLOWED_CARRIERS_CNF */
enum mipc_sim_sml_get_allowed_carriers_cnf_tlv_enum {
    mipc_sim_sml_get_allowed_carriers_cnf_tlv_NONE = 0,
    /* Carrier lock status setted by AT+ECRRST */
    /* type = uint8_t, refer to SIM_CRRST_STATE */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_STATE           = 0x100,
    /* Multiple SIM policies */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_MULTI_SIM_POLICY = 0x101,
    /*
      True: Only carriers included in the allow list and not included in the exclusion list are permitted to acquire normal service;
      False: All carriers are permitted except those included in the exclusion list and not included in the allow list
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_PRIORITIZED = 0x102,
    /* Number of permitted carriers */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_NUM = 0x103,
    /* List of permitted carriers */
    /* type = struct_array, refer to sim_carrier_struct */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS = 0x104,
    /* Number of excluded carriers */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS_NUM = 0x105,
    /* List of excluded carriers */
    /* type = struct_array, refer to sim_carrier_struct */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS = 0x106,
    /* List of permitted carriers */
    /* type = struct, refer to sim_carrier_struct */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_ALLOWED_CARRIERS_TLV_ARRAY = 0x107,
    /* List of excluded carriers */
    /* type = struct, refer to sim_carrier_struct */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_CARRIERS_TLV_ARRAY = 0x108,
    /* Carrier restriction status of device.RESTRICTED means device will be limited to use special operators's sim cards.Not Restricted means device has no limits for sim card.It contains multi Sim-Me-Lock type, such as 3GPP Personalisation of ME/Operator related RSU/Google Carrier Resstriction. */
    /* type = uint8_t, refer to SIM_CARRIER_RESTRICTION_STATUS */
    MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF_T_CARRIER_RESTRICTION_STATUS = 0x109,
};

    /* MIPC_MSG.SIM_SML_SET_ALLOWED_CARRIERS_REQ */
enum mipc_sim_sml_set_allowed_carriers_req_tlv_enum {
    mipc_sim_sml_set_allowed_carriers_req_tlv_NONE = 0,
    /* Multiple SIM policies */
    /* type = uint8_t */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_MULTI_SIM_POLICY = 0x100,
    /*
      True: Only carriers included in the allow list and not included in the exclusion list are permitted to acquire normal service;
      False: All carriers are permitted except those included in the exclusion list and not included in the allow list
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_PRIORITIZED = 0x101,
    /* Number of permitted carriers; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_NUM = 0x102,
    /* List of permitted carriers */
    /* type = struct_array, refer to sim_carrier_struct */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS = 0x103,
    /* Number of excluded carriers; Default value is 0. */
    /* type = uint8_t */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS_NUM = 0x104,
    /* List of excluded carriers */
    /* type = struct_array, refer to sim_carrier_struct */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS = 0x105,
    /* List of permitted carriers */
    /* type = struct, refer to sim_carrier_struct */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_ALLOWED_CARRIERS_TLV_ARRAY = 0x106,
    /* List of excluded carriers */
    /* type = struct, refer to sim_carrier_struct */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ_T_EXCLUDED_CARRIERS_TLV_ARRAY = 0x107,
};

    /* MIPC_MSG.SIM_SML_SET_ALLOWED_CARRIERS_CNF */
enum mipc_sim_sml_set_allowed_carriers_cnf_tlv_enum {
    mipc_sim_sml_set_allowed_carriers_cnf_tlv_NONE = 0,
    /* Number of permitted carriers which are set successfully */
    /* type = uint8_t */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF_T_ALLOWED_COUNT   = 0x100,
    /* Number of excluded carriers which are set successfully */
    /* type = uint8_t */
    MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF_T_EXCLUDED_COUNT  = 0x101,
};

    /* MIPC_MSG.SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ */
enum mipc_sim_sml_enter_sim_depersonalization_req_tlv_enum {
    mipc_sim_sml_enter_sim_depersonalization_req_tlv_NONE = 0,
    /* SIM ME lock category */
    /* type = uint8_t, refer to SIM_SML_CATEGORY */
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_CATEGORY = 0x100,
    /* Password for depersonalization */
    /* type = string */
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ_T_PIN_CODE = 0x8101,
};

    /* MIPC_MSG.SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF */
enum mipc_sim_sml_enter_sim_depersonalization_cnf_tlv_enum {
    mipc_sim_sml_enter_sim_depersonalization_cnf_tlv_NONE = 0,
    /* SIM ME lock category */
    /* type = uint8_t, refer to SIM_SML_CATEGORY */
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_CATEGORY = 0x100,
    /* Times left for user to try on SIM depersonalization verification */
    /* type = uint8_t */
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT = 0x101,
    /* Remaining retry count for all categories */
    /* type = byte_array */
    MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF_T_REMAIN_COUNT_LIST = 0x102,
};

    /* MIPC_MSG.SIM_SML_GET_LOCK_REQ */
enum mipc_sim_sml_get_lock_req_tlv_enum {
    mipc_sim_sml_get_lock_req_tlv_NONE = 0,
    /* SIM ME lock category */
    /* type = uint8_t, refer to SIM_SML_CATEGORY */
    MIPC_SIM_SML_GET_LOCK_REQ_T_CATEGORY                    = 0x100,
};

    /* MIPC_MSG.SIM_SML_GET_LOCK_CNF */
enum mipc_sim_sml_get_lock_cnf_tlv_enum {
    mipc_sim_sml_get_lock_cnf_tlv_NONE = 0,
    /* SIM ME lock category */
    /* type = uint8_t, refer to SIM_SML_CATEGORY */
    MIPC_SIM_SML_GET_LOCK_CNF_T_CATEGORY                    = 0x100,
    /* Category lock state; 1: Lock; 2: Unlock; 3: Auto lock; 4: Disable */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_STATE                       = 0x101,
    /* Times left for user to try on SML verification */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_RETRY_CNT                   = 0x102,
    /* Times left for modem to write SIM card code in category list */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_AUTOLOCK_CNT                = 0x103,
    /* Number of sets included in the category list */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_NUM_SET                     = 0x104,
    /* Total number of sets which can be set by the category */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_TOTAL_SET                   = 0x105,
    /* Current state of key; 0: Key is not set; 1: Key is well set */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_KEY_STATE                   = 0x106,
    /* Maximum times for user to try on SML verification */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_LOCK_CNF_T_MAX_CNT                     = 0x107,
    /* Unlock authentication by local or remote; 1: Unlock by remote server; 0: Unlock by local server */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_SML_GET_LOCK_CNF_T_RSU_ENABLE                  = 0x108,
};

    /* MIPC_MSG.SIM_SML_SET_LOCK_REQ */
enum mipc_sim_sml_set_lock_req_tlv_enum {
    mipc_sim_sml_set_lock_req_tlv_NONE = 0,
    /* SIM ME lock category */
    /* type = uint8_t, refer to SIM_SML_CATEGORY */
    MIPC_SIM_SML_SET_LOCK_REQ_T_CATEGORY                    = 0x100,
    /* Operation for SIM ME lock */
    /* type = uint8_t, refer to SIM_SML_OPERATION */
    MIPC_SIM_SML_SET_LOCK_REQ_T_OP                          = 0x101,
    /* String of category key; Default value is null character. */
    /* type = string */
    MIPC_SIM_SML_SET_LOCK_REQ_T_KEY                         = 0x8102,
    /* String of IMSI; Default value is null character. */
    /* type = string */
    MIPC_SIM_SML_SET_LOCK_REQ_T_DATA_IMSI                   = 0x103,
    /* String of GID1; Default value is null character. */
    /* type = string */
    MIPC_SIM_SML_SET_LOCK_REQ_T_GID1                        = 0x104,
    /* String of GID2; Default value is null character. */
    /* type = string */
    MIPC_SIM_SML_SET_LOCK_REQ_T_GID2                        = 0x105,
};

    /* MIPC_MSG.SIM_SML_SET_LOCK_CNF */
enum mipc_sim_sml_set_lock_cnf_tlv_enum {
    mipc_sim_sml_set_lock_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ */
enum mipc_sim_sml_enter_device_depersonalization_req_tlv_enum {
    mipc_sim_sml_enter_device_depersonalization_req_tlv_NONE = 0,
    /* String of device key for device lock verification */
    /* type = string */
    MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ_T_KEY   = 0x8100,
};

    /* MIPC_MSG.SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF */
enum mipc_sim_sml_enter_device_depersonalization_cnf_tlv_enum {
    mipc_sim_sml_enter_device_depersonalization_cnf_tlv_NONE = 0,
    /* Times left for user to retry on device lock verification */
    /* type = uint8_t */
    MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF_T_REMAIN_COUNT = 0x100,
};

    /* MIPC_MSG.SIM_SML_GET_DEV_LOCK_REQ */
enum mipc_sim_sml_get_dev_lock_req_tlv_enum {
    mipc_sim_sml_get_dev_lock_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_SML_GET_DEV_LOCK_CNF */
enum mipc_sim_sml_get_dev_lock_cnf_tlv_enum {
    mipc_sim_sml_get_dev_lock_cnf_tlv_NONE = 0,
    /* Device lock state; 0: Locked; 1: Unlocked */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_LOCK_STATE              = 0x100,
    /* Protection algorithm used by the device lock; 0: Plain text; 1: PBKDF2-HMAC-SHA256 */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_ALGO                    = 0x101,
    /* Maximum retry count for user to try on device lock verification */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_MAX_CNT                 = 0x102,
    /* Remaining retry count for user to try on device lock verification */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_DEV_LOCK_CNF_T_REMAIN_CNT              = 0x103,
};

    /* MIPC_MSG.SIM_SML_RSU_REQ */
enum mipc_sim_sml_rsu_req_tlv_enum {
    mipc_sim_sml_rsu_req_tlv_NONE = 0,
    /* Operator ID */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_REQ_T_OPERATOR_ID                      = 0x100,
    /* Request function ID */
    /* type = uint32_t, refer to SIM_SML_RSU_OPERATION */
    MIPC_SIM_SML_RSU_REQ_T_REQUEST_ID                       = 0x101,
    /* Start or stop temp unlock timer;This field is valid only when 'REQUEST_ID' = 4(MIPC_SIM_SML_RSU_OPERATION_UNLOCK_TIMER); Default value is 0. */
    /* type = uint32_t, refer to SML_RSU_REQUEST_TYPE */
    MIPC_SIM_SML_RSU_REQ_T_REQUEST_TYPE                     = 0x102,
    /* Data; Default value is null character. */
    /* type = string */
    MIPC_SIM_SML_RSU_REQ_T_DATA                             = 0x8103,
    /* Reserved Integer 1; Default value is 0. */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_REQ_T_RSV1                             = 0x104,
    /* Reserved Integer 2; Default value is 0. */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_REQ_T_RSV2                             = 0x105,
    /* RFU data string; Default value is null character. */
    /* type = string */
    MIPC_SIM_SML_RSU_REQ_T_RSV_STRING                       = 0x8106,
};

    /* MIPC_MSG.SIM_SML_RSU_CNF */
enum mipc_sim_sml_rsu_cnf_tlv_enum {
    mipc_sim_sml_rsu_cnf_tlv_NONE = 0,
    /* Operator ID */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_OPERATOR_ID                      = 0x100,
    /* Request function ID */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_REQUEST_ID                       = 0x101,
    /* Unlock timer operation ID */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_REQUEST_TYPE                     = 0x102,
    /* For OP12 start/stop unlock timer, this field means start/stop result and error cause; for others, means CME error code */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_ERROR_CODE                       = 0x103,
    /* Data */
    /* type = string */
    MIPC_SIM_SML_RSU_CNF_T_DATA                             = 0x8104,
    /* Remaining seconds of temporary unlock time */
    /* type = byte_array */
    MIPC_SIM_SML_RSU_CNF_T_TIME                             = 0x105,
    /* 2 x MSB for major version, and 2 x LSB for minor version */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_VERSION                          = 0x106,
    /* Lock status */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_STATUS                           = 0x107,
    /* Reserved integer 1 */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_RSV1                             = 0x108,
    /* Reserved integer 2 */
    /* type = uint32_t */
    MIPC_SIM_SML_RSU_CNF_T_RSV2                             = 0x109,
    /* RFU data string */
    /* type = string */
    MIPC_SIM_SML_RSU_CNF_T_RSV_STRING                       = 0x810A,
};

    /* MIPC_MSG.SIM_VSIM_SET_AKA_REQ */
enum mipc_sim_vsim_set_aka_req_tlv_enum {
    mipc_sim_vsim_set_aka_req_tlv_NONE = 0,
    /* 0-non-AKA, 1-AKA */
    /* type = uint8_t */
    MIPC_SIM_VSIM_SET_AKA_REQ_T_MODE                        = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_SET_AKA_CNF */
enum mipc_sim_vsim_set_aka_cnf_tlv_enum {
    mipc_sim_vsim_set_aka_cnf_tlv_NONE = 0,
    /* result of set aka sim */
    /* type = uint32_t */
    MIPC_SIM_VSIM_SET_AKA_CNF_T_RESULT                      = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_ENABLE_REQ */
enum mipc_sim_vsim_enable_req_tlv_enum {
    mipc_sim_vsim_enable_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_VSIM_ENABLE_CNF */
enum mipc_sim_vsim_enable_cnf_tlv_enum {
    mipc_sim_vsim_enable_cnf_tlv_NONE = 0,
    /* result of enable Vsim */
    /* type = uint32_t */
    MIPC_SIM_VSIM_ENABLE_CNF_T_RESULT                       = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_DISABLE_REQ */
enum mipc_sim_vsim_disable_req_tlv_enum {
    mipc_sim_vsim_disable_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_VSIM_DISABLE_CNF */
enum mipc_sim_vsim_disable_cnf_tlv_enum {
    mipc_sim_vsim_disable_cnf_tlv_NONE = 0,
    /* result of enable Vsim */
    /* type = uint32_t */
    MIPC_SIM_VSIM_DISABLE_CNF_T_RESULT                      = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_PLUG_REQ */
enum mipc_sim_vsim_plug_req_tlv_enum {
    mipc_sim_vsim_plug_req_tlv_NONE = 0,
    /* 0-in, 1-out */
    /* type = uint8_t */
    MIPC_SIM_VSIM_PLUG_REQ_T_MODE                           = 0x100,
    /* 0-LOCAL_SIM, 1-REMOTE_SIM; Default value is 0. */
    /* type = uint8_t, refer to SIM_VSIM_TYPE */
    MIPC_SIM_VSIM_PLUG_REQ_T_SIM_TYPE                       = 0x101,
};

    /* MIPC_MSG.SIM_VSIM_PLUG_CNF */
enum mipc_sim_vsim_plug_cnf_tlv_enum {
    mipc_sim_vsim_plug_cnf_tlv_NONE = 0,
    /* result of plug in/out vsim */
    /* type = uint32_t */
    MIPC_SIM_VSIM_PLUG_CNF_T_RESULT                         = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_SET_TIMER_REQ */
enum mipc_sim_vsim_set_timer_req_tlv_enum {
    mipc_sim_vsim_set_timer_req_tlv_NONE = 0,
    /* vsim timer, valid range : 1~255 */
    /* type = uint32_t */
    MIPC_SIM_VSIM_SET_TIMER_REQ_T_TIMER                     = 0x8100,
};

    /* MIPC_MSG.SIM_VSIM_SET_TIMER_CNF */
enum mipc_sim_vsim_set_timer_cnf_tlv_enum {
    mipc_sim_vsim_set_timer_cnf_tlv_NONE = 0,
    /* result of set vsim timer */
    /* type = uint32_t */
    MIPC_SIM_VSIM_SET_TIMER_CNF_T_RESULT                    = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_RESET_REQ */
enum mipc_sim_vsim_reset_req_tlv_enum {
    mipc_sim_vsim_reset_req_tlv_NONE = 0,
    /* reset result */
    /* type = uint32_t */
    MIPC_SIM_VSIM_RESET_REQ_T_RESULT                        = 0x100,
    /* data length; Default value is 0. */
    /* type = uint32_t */
    MIPC_SIM_VSIM_RESET_REQ_T_LENGTH                        = 0x101,
    /* reset data */
    /* type = string */
    MIPC_SIM_VSIM_RESET_REQ_T_DATA                          = 0x102,
};

    /* MIPC_MSG.SIM_VSIM_RESET_CNF */
enum mipc_sim_vsim_reset_cnf_tlv_enum {
    mipc_sim_vsim_reset_cnf_tlv_NONE = 0,
    /* result of reset vsim */
    /* type = uint32_t */
    MIPC_SIM_VSIM_RESET_CNF_T_RESULT                        = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_APDU_REQ */
enum mipc_sim_vsim_apdu_req_tlv_enum {
    mipc_sim_vsim_apdu_req_tlv_NONE = 0,
    /* data length; Default value is 0. */
    /* type = uint32_t */
    MIPC_SIM_VSIM_APDU_REQ_T_LENGTH                         = 0x100,
    /* APDU data */
    /* type = string */
    MIPC_SIM_VSIM_APDU_REQ_T_DATA                           = 0x8101,
};

    /* MIPC_MSG.SIM_VSIM_APDU_CNF */
enum mipc_sim_vsim_apdu_cnf_tlv_enum {
    mipc_sim_vsim_apdu_cnf_tlv_NONE = 0,
    /* result of send APDU */
    /* type = uint32_t */
    MIPC_SIM_VSIM_APDU_CNF_T_RESULT                         = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_AUTH_REQ */
enum mipc_sim_vsim_auth_req_tlv_enum {
    mipc_sim_vsim_auth_req_tlv_NONE = 0,
    /* 1-occuping RF, 0-releasing RF */
    /* type = uint8_t, refer to SIM_VSIM_AUTH_MODE */
    MIPC_SIM_VSIM_AUTH_REQ_T_MODE                           = 0x100,
};

    /* MIPC_MSG.SIM_VSIM_AUTH_CNF */
enum mipc_sim_vsim_auth_cnf_tlv_enum {
    mipc_sim_vsim_auth_cnf_tlv_NONE = 0,
    /* result of auth */
    /* type = uint32_t */
    MIPC_SIM_VSIM_AUTH_CNF_T_RESULT                         = 0x100,
};

    /* MIPC_MSG.SIM_CDMA_SUBSCRIPTION_REQ */
enum mipc_sim_cdma_subscription_req_tlv_enum {
    mipc_sim_cdma_subscription_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_CDMA_SUBSCRIPTION_CNF */
enum mipc_sim_cdma_subscription_cnf_tlv_enum {
    mipc_sim_cdma_subscription_cnf_tlv_NONE = 0,
    /* result  */
    /* type = uint32_t */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_RESULT                 = 0x100,
    /* MSISDN  */
    /* type = string */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_MSISDN                 = 0x101,
    /* CDMA home SID/NID count, max 20 */
    /* type = uint8_t */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_SID_NID_COUNT          = 0x102,
    /* home SID/NID pair list */
    /* type = struct_array, refer to sid_nid_list */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_SID_NID_LIST           = 0x103,
    /* mobile identification number */
    /* type = string */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_VMIN                   = 0x104,
    /* PRL ID */
    /* type = string */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_VPRLID                 = 0x105,
    /* home SID/NID pair list */
    /* type = struct, refer to sid_nid_list */
    MIPC_SIM_CDMA_SUBSCRIPTION_CNF_T_SID_NID_TLV_ARRAY      = 0x106,
};

    /* MIPC_MSG.SIM_CDMA_GET_SUBSCRIPTION_SOURCE_REQ */
enum mipc_sim_cdma_get_subscription_source_req_tlv_enum {
    mipc_sim_cdma_get_subscription_source_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF */
enum mipc_sim_cdma_get_subscription_source_cnf_tlv_enum {
    mipc_sim_cdma_get_subscription_source_cnf_tlv_NONE = 0,
    /* result of auth */
    /* type = uint32_t */
    MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF_T_RESULT      = 0x100,
    /* UIM state, 0- UIM disable, 1- UIM enable */
    /* type = uint8_t */
    MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF_T_UIM_STATE   = 0x101,
};

    /* MIPC_MSG.SIM_PIN_COUNT_QUERY_REQ */
enum mipc_sim_pin_count_query_req_tlv_enum {
    mipc_sim_pin_count_query_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_PIN_COUNT_QUERY_CNF */
enum mipc_sim_pin_count_query_cnf_tlv_enum {
    mipc_sim_pin_count_query_cnf_tlv_NONE = 0,
    /* Result of PIN count query */
    /* type = uint32_t */
    MIPC_SIM_PIN_COUNT_QUERY_CNF_T_RESULT                   = 0x100,
    /* PIN1 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PIN1                     = 0x101,
    /* PIN2 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PIN2                     = 0x102,
    /* PUK1 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PUK1                     = 0x103,
    /* PUK2 remaining attempts */
    /* type = uint8_t */
    MIPC_SIM_PIN_COUNT_QUERY_CNF_T_PUK2                     = 0x104,
};

    /* MIPC_MSG.SIM_SML_GET_NETWORK_LOCK_REQ */
enum mipc_sim_sml_get_network_lock_req_tlv_enum {
    mipc_sim_sml_get_network_lock_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_SML_GET_NETWORK_LOCK_CNF */
enum mipc_sim_sml_get_network_lock_cnf_tlv_enum {
    mipc_sim_sml_get_network_lock_cnf_tlv_NONE = 0,
    /* carrier id */
    /* type = uint32_t */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_CARRIER_ID          = 0x100,
    /* supported sml lock cats, every bit maps to a sml lock cats, if bit value is 1 means this cat is supported. 3GPP_NW(bit0)/3GPP_NS(bit1)/3GPP_SP(bit2)/3GPP_CP(bit3)/3GPP_SIM(bit4)/NS_NP(bit5)/SIM_C(bit6) */
    /* type = uint32_t */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_SUPPORTED_CATS      = 0x101,
    /* now fixed 0x01 */
    /* type = uint32_t */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_VERSION             = 0x102,
    /* network lock status, 0 means unlocked, 1 means locked */
    /* type = uint8_t */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_LOCK_STATUS         = 0x103,
    /* sim lock is enable or configured in modem side, 1 means md configured, 0 means not configured */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_LOCK_FUSE           = 0x104,
    /* unlock by local or remote, 1 means unlock by remote server, 0 means by local */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_RSU_ENABLE          = 0x105,
    /* reserve to save sim me lock rules */
    /* type = uint16_t */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_RULE                = 0x106,
    /* reserve to save sim me lock subrules */
    /* type = uint16_t */
    MIPC_SIM_SML_GET_NETWORK_LOCK_CNF_T_SUBRULE             = 0x107,
};

    /* MIPC_MSG.SIM_ESIM_SWITCH_SET_REQ */
enum mipc_sim_esim_switch_set_req_tlv_enum {
    mipc_sim_esim_switch_set_req_tlv_NONE = 0,
    /* 1:switch to esim, 0: switch to psim */
    /* type = uint8_t */
    MIPC_SIM_ESIM_SWITCH_SET_REQ_T_MODE                     = 0x100,
};

    /* MIPC_MSG.SIM_ESIM_SWITCH_SET_CNF */
enum mipc_sim_esim_switch_set_cnf_tlv_enum {
    mipc_sim_esim_switch_set_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ESIM_SWITCH_GET_REQ */
enum mipc_sim_esim_switch_get_req_tlv_enum {
    mipc_sim_esim_switch_get_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ESIM_SWITCH_GET_CNF */
enum mipc_sim_esim_switch_get_cnf_tlv_enum {
    mipc_sim_esim_switch_get_cnf_tlv_NONE = 0,
    /* esim support or not */
    /* type = uint8_t */
    MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_SUPPORT             = 0x100,
    /* 0:psim, 1:esim */
    /* type = uint8_t */
    MIPC_SIM_ESIM_SWITCH_GET_CNF_T_ESIM_PSIM_STATE          = 0x101,
};

    /* MIPC_MSG.SIM_ESIM_EID_QUERY_REQ */
enum mipc_sim_esim_eid_query_req_tlv_enum {
    mipc_sim_esim_eid_query_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ESIM_EID_QUERY_CNF */
enum mipc_sim_esim_eid_query_cnf_tlv_enum {
    mipc_sim_esim_eid_query_cnf_tlv_NONE = 0,
    /* eid */
    /* type = string */
    MIPC_SIM_ESIM_EID_QUERY_CNF_T_EID                       = 0x8100,
    /* EID in byte/octet16 format per spec GSMA SGP.22. This is the same EID as TLV EID but in a different format. */
    /* type = byte_array */
    MIPC_SIM_ESIM_EID_QUERY_CNF_T_EID_BYTE                  = 0x8101,
};

    /* MIPC_MSG.SIM_MEP_SLOTS_INFO_GET_REQ */
enum mipc_sim_mep_slots_info_get_req_tlv_enum {
    mipc_sim_mep_slots_info_get_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_MEP_SLOTS_INFO_GET_CNF */
enum mipc_sim_mep_slots_info_get_cnf_tlv_enum {
    mipc_sim_mep_slots_info_get_cnf_tlv_NONE = 0,
    /* The total number of physical slot */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PHY_SLOT_COUNT        = 0x100,
    /* The list of SIM card state. Array index is physical slot ID and array value is SIM card state. */
    /* type = uint8_t, refer to SIM_CARD_PRESENT_STATE */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_CARD_STATE            = 0x101,
    /* ATR string og the physical slot */
    /* type = string */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_ATR                   = 0x8102,
    /* EID string of the physical slot */
    /* type = string */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_EID                   = 0x8103,
    /* For MEP slot, port number is the logical SE interface of the physical slot; for a lagecy SIM or eSIM which does not support MEP, the port number is 1 */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PORT_NUM              = 0x8104,
    /* The detail information for all ports. For MEP slot, the port information is about Logical SE interface; For a lagecy SIM or eSIM which does not support MEP, the port information is about physical slot information */
    /* type = TLV, refer to mep_port_info */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_PORT_INFO             = 0x8105,
    /* The MEP mode as defined in SGP.22 */
    /* type = uint8_t, refer to SIM_MEP_MODE */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_MEP_MODE              = 0x106,
    /* The EID of SIM cards. The index of this array is aligned with the index of CARD_STATE (i.e., index of physical slots) */
    /* type = struct, refer to eid_byte_struct */
    MIPC_SIM_MEP_SLOTS_INFO_GET_CNF_T_EID_BYTE_TLV_ARRAY    = 0x8107,
};

    /* MIPC_MSG.SIM_MEP_SLOTS_MAPPING_SET_REQ */
enum mipc_sim_mep_slots_mapping_set_req_tlv_enum {
    mipc_sim_mep_slots_mapping_set_req_tlv_NONE = 0,
    /* The total number of SIM that user can get normal service */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_SIM_COUNT          = 0x100,
    /* The physical slot ID list. The index of list corresponds to SIM index. For example, SIM[0] is mapped to PHY_SLOT_ID[0], SIM[1] is mapped to PHY_SLOT_ID[1] ... and so on. */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_PHY_SLOT_ID        = 0x101,
    /* The port ID list of the physical slot. The index of list corresponds to SIM index. For example, SIM[0] is mapped to PORT_ID[0], SIM[1] is mapped to PORT_ID[1] ... and so on. */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ_T_PORT_ID            = 0x102,
};

    /* MIPC_MSG.SIM_MEP_SLOTS_MAPPING_SET_CNF */
enum mipc_sim_mep_slots_mapping_set_cnf_tlv_enum {
    mipc_sim_mep_slots_mapping_set_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_PSISMSC_REQ */
enum mipc_sim_get_psismsc_req_tlv_enum {
    mipc_sim_get_psismsc_req_tlv_NONE = 0,
    /* Application ID, 0: ISIM, 1:USIM */
    /* type = uint8_t */
    MIPC_SIM_GET_PSISMSC_REQ_T_APP_ID                       = 0x100,
};

    /* MIPC_MSG.SIM_GET_PSISMSC_CNF */
enum mipc_sim_get_psismsc_cnf_tlv_enum {
    mipc_sim_get_psismsc_cnf_tlv_NONE = 0,
    /* The type of PSISMSC is 1D TLV array which the element is string */
    /* type = string */
    MIPC_SIM_GET_PSISMSC_CNF_T_PSISMSC_TLV_ARRAY            = 0x8100,
};

    /* MIPC_MSG.SIM_GET_SMSP_REQ */
enum mipc_sim_get_smsp_req_tlv_enum {
    mipc_sim_get_smsp_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_SMSP_CNF */
enum mipc_sim_get_smsp_cnf_tlv_enum {
    mipc_sim_get_smsp_cnf_tlv_NONE = 0,
    /* The type of SMSP is 1D TLV array which the element is byte array */
    /* type = byte_array */
    MIPC_SIM_GET_SMSP_CNF_T_SMSP_TLV_ARRAY                  = 0x8100,
};

    /* MIPC_MSG.SIM_GET_SERVICE_TABLE_REQ */
enum mipc_sim_get_service_table_req_tlv_enum {
    mipc_sim_get_service_table_req_tlv_NONE = 0,
    /* Application ID, 0: ISIM, 1: USIM */
    /* type = uint8_t */
    MIPC_SIM_GET_SERVICE_TABLE_REQ_T_APP_ID                 = 0x100,
};

    /* MIPC_MSG.SIM_GET_SERVICE_TABLE_CNF */
enum mipc_sim_get_service_table_cnf_tlv_enum {
    mipc_sim_get_service_table_cnf_tlv_NONE = 0,
    /* USIM service table (UST) in byte array */
    /* type = byte_array */
    MIPC_SIM_GET_SERVICE_TABLE_CNF_T_UST                    = 0x8100,
    /* ISIM service table (IST) in byte array */
    /* type = byte_array */
    MIPC_SIM_GET_SERVICE_TABLE_CNF_T_IST                    = 0x8101,
};

    /* MIPC_MSG.SIM_QUERY_GID_REQ */
enum mipc_sim_query_gid_req_tlv_enum {
    mipc_sim_query_gid_req_tlv_NONE = 0,
    /* There are threee types: GID_TYPE=0 means to query both GID1 and GID2, GID_TYPE=1 means to query GID1 only, GID_TYPE=2 means to query GID2 only */
    /* type = uint8_t */
    MIPC_SIM_QUERY_GID_REQ_T_GID_TYPE                       = 0x100,
};

    /* MIPC_MSG.SIM_QUERY_GID_CNF */
enum mipc_sim_query_gid_cnf_tlv_enum {
    mipc_sim_query_gid_cnf_tlv_NONE = 0,
    /* GID1 in byte array */
    /* type = byte_array */
    MIPC_SIM_QUERY_GID_CNF_T_GID1                           = 0x8100,
    /* GID2 in byte array */
    /* type = byte_array */
    MIPC_SIM_QUERY_GID_CNF_T_GID2                           = 0x8101,
};

    /* MIPC_MSG.SIM_GET_IMPI_REQ */
enum mipc_sim_get_impi_req_tlv_enum {
    mipc_sim_get_impi_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_IMPI_CNF */
enum mipc_sim_get_impi_cnf_tlv_enum {
    mipc_sim_get_impi_cnf_tlv_NONE = 0,
    /* IMPI string */
    /* type = string */
    MIPC_SIM_GET_IMPI_CNF_T_IMPI                            = 0x8100,
};

    /* MIPC_MSG.SIM_GET_IMPU_REQ */
enum mipc_sim_get_impu_req_tlv_enum {
    mipc_sim_get_impu_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_IMPU_CNF */
enum mipc_sim_get_impu_cnf_tlv_enum {
    mipc_sim_get_impu_cnf_tlv_NONE = 0,
    /* IMPU in 1D TLV array and each element is string */
    /* type = string */
    MIPC_SIM_GET_IMPU_CNF_T_IMPU_TLV_ARRAY                  = 0x8100,
};

    /* MIPC_MSG.SIM_GET_DOMAIN_REQ */
enum mipc_sim_get_domain_req_tlv_enum {
    mipc_sim_get_domain_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_DOMAIN_CNF */
enum mipc_sim_get_domain_cnf_tlv_enum {
    mipc_sim_get_domain_cnf_tlv_NONE = 0,
    /* DOMAIN in byte array */
    /* type = byte_array */
    MIPC_SIM_GET_DOMAIN_CNF_T_DOMAIN                        = 0x8100,
};

    /* MIPC_MSG.SIM_GET_PCSCF_REQ */
enum mipc_sim_get_pcscf_req_tlv_enum {
    mipc_sim_get_pcscf_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_GET_PCSCF_CNF */
enum mipc_sim_get_pcscf_cnf_tlv_enum {
    mipc_sim_get_pcscf_cnf_tlv_NONE = 0,
    /* This TLV array is used to indicate the type of each PCSCF. Each element in this TLV array is corresponding to the element of PCSCF_TLV_ARRAY */
    /* type = uint8_t */
    MIPC_SIM_GET_PCSCF_CNF_T_TYPE_TLV_ARRAY                 = 0x8100,
    /* PCSCF in 1D TLV array and each element is byte array */
    /* type = byte_array */
    MIPC_SIM_GET_PCSCF_CNF_T_PCSCF_TLV_ARRAY                = 0x8101,
};

    /* MIPC_MSG.SIM_STATE_IND */
enum mipc_sim_state_ind_tlv_enum {
    mipc_sim_state_ind_tlv_NONE = 0,
    /* This field is mapping to MBIM SIM state */
    /* type = uint8_t, refer to SIM_STATE */
    MIPC_SIM_STATE_IND_T_STATE                              = 0x100,
    /* SIM slot ID */
    /* type = uint32_t */
    MIPC_SIM_STATE_IND_T_SIM_ID                             = 0x101,
    /* SIM protocol ID */
    /* type = uint32_t */
    MIPC_SIM_STATE_IND_T_PS_ID                              = 0x102,
    /* SIM card is present or not */
    /* type = uint32_t */
    MIPC_SIM_STATE_IND_T_IS_PRESENT                         = 0x103,
    /* Sub state of SIM card */
    /* type = uint8_t, refer to SIM_SUB_STATE */
    MIPC_SIM_STATE_IND_T_SUB_STATE                          = 0x104,
};

    /* MIPC_MSG.SIM_STATUS_IND */
enum mipc_sim_status_ind_tlv_enum {
    mipc_sim_status_ind_tlv_NONE = 0,
    /* SIM status */
    /* type = uint8_t, refer to SIM_STATUS */
    MIPC_SIM_STATUS_IND_T_STATUS                            = 0x100,
    /* SIM slot ID */
    /* type = uint32_t */
    MIPC_SIM_STATUS_IND_T_SIM_ID                            = 0x101,
    /* SIM protocol ID */
    /* type = uint32_t */
    MIPC_SIM_STATUS_IND_T_PS_ID                             = 0x102,
};

    /* MIPC_MSG.SIM_EUICC_SLOTS_STATUS_IND */
enum mipc_sim_euicc_slots_status_ind_tlv_enum {
    mipc_sim_euicc_slots_status_ind_tlv_NONE = 0,
    /* physical slots number */
    /* type = uint8_t */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_COUNT      = 0x100,
    /* slots info */
    /* type = struct_array, refer to sim_slots_info */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_LIST       = 0x8101,
    /* sim card state */
    /* type = uint8_t, refer to SIM_CARD_PRESENT_STATE */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_CARD_STATE            = 0x102,
    /* type = uint8_t */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_STATE           = 0x103,
    /* type = uint8_t */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_LOGICAL_IDX           = 0x104,
    /* type = string */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_ATR                   = 0x8105,
    /* type = string */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_EID                   = 0x8106,
    /* type = string */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_ICCID                 = 0x8107,
    /* slots info */
    /* type = struct, refer to sim_slots_info */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_SLOTS_INFO_TLV_ARRAY  = 0x8108,
    /* The EID of SIM cards. The index of this array is aligned with the slot-info index of SLOTS_INFO_TLV_ARRAY */
    /* type = struct, refer to eid_byte_struct */
    MIPC_SIM_EUICC_SLOTS_STATUS_IND_T_EID_BYTE_TLV_ARRAY    = 0x8109,
};

    /* MIPC_MSG.SIM_ICCID_IND */
enum mipc_sim_iccid_ind_tlv_enum {
    mipc_sim_iccid_ind_tlv_NONE = 0,
    /* ICCID string */
    /* type = string */
    MIPC_SIM_ICCID_IND_T_ICCID                              = 0x8100,
};

    /* MIPC_MSG.SIM_EVENT_IND */
enum mipc_sim_event_ind_tlv_enum {
    mipc_sim_event_ind_tlv_NONE = 0,
    /* SIM is inserted to modem or not */
    /* type = uint8_t */
    MIPC_SIM_EVENT_IND_T_IS_SIM_INSERTED                    = 0x100,
    /* SIM change cause */
    /* type = uint8_t, refer to SIM_CAUSE */
    MIPC_SIM_EVENT_IND_T_CAUSE                              = 0x101,
    /* Additional cause of SIM change; currently only the operation of SIM refresh will report additional cause */
    /* type = uint8_t, refer to SIM_ADDITIONAL_CAUSE */
    MIPC_SIM_EVENT_IND_T_ADDITIONAL_CAUSE                   = 0x102,
};

enum mipc_sim_status_change_with_cause_ind_tlv_enum {
    mipc_sim_status_change_with_cause_ind_tlv_NONE = 0,
    /* SIM is inserted to modem or not */
    /* type = uint8_t */
    MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_IS_SIM_INSERTED = 0x100,
    /* SIM change cause */
    /* type = uint8_t, refer to SIM_CAUSE */
    MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_CAUSE           = 0x101,
    /* Additional cause of SIM change; currently only the operation of SIM refresh will report additional cause */
    /* type = uint8_t, refer to SIM_ADDITIONAL_CAUSE */
    MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND_T_ADDITIONAL_CAUSE = 0x102,
};

    /* MIPC_MSG.SIM_PHYSICAL_SLOTS_MAPPING_DONE_IND */
enum mipc_sim_physical_slots_mapping_done_ind_tlv_enum {
    mipc_sim_physical_slots_mapping_done_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_CSIM_IMSI_CHANGE_IND */
enum mipc_sim_csim_imsi_change_ind_tlv_enum {
    mipc_sim_csim_imsi_change_ind_tlv_NONE = 0,
    /* 0 means csim ismi_m not programmed, 1 means csim imsi_m programmed */
    /* type = uint8_t */
    MIPC_SIM_CSIM_IMSI_CHANGE_IND_T_STATUS                  = 0x100,
};

    /* MIPC_MSG.SIM_SML_STATUS_IND */
enum mipc_sim_sml_status_ind_tlv_enum {
    mipc_sim_sml_status_ind_tlv_NONE = 0,
    /* Lock rule */
    /* type = uint8_t */
    MIPC_SIM_SML_STATUS_IND_T_LOCK_RULE                     = 0x100,
    /* Lock sub rule */
    /* type = uint16_t */
    MIPC_SIM_SML_STATUS_IND_T_LOCK_SUB_RULE                 = 0x101,
    /* Device lock state */
    /* type = uint8_t */
    MIPC_SIM_SML_STATUS_IND_T_DEVICE_LOCK_STATE             = 0x102,
    /* Maximum possible capability of current protocol */
    /* type = uint8_t */
    MIPC_SIM_SML_STATUS_IND_T_RULE_POLICY                   = 0x103,
    /* SIM card valid state under current rule setting */
    /* type = uint8_t */
    MIPC_SIM_SML_STATUS_IND_T_SIM_VALIDITY                  = 0x104,
    /* Remaining times left for user to retry device lock verification */
    /* type = uint8_t */
    MIPC_SIM_SML_STATUS_IND_T_DEVICE_LOCK_REMAIN_CNT        = 0x105,
};

    /* MIPC_MSG.SIM_SML_RSU_IND */
enum mipc_sim_sml_rsu_ind_tlv_enum {
    mipc_sim_sml_rsu_ind_tlv_NONE = 0,
    /* Operator ID */
    /* type = uint8_t */
    MIPC_SIM_SML_RSU_IND_T_OPERATOR_ID                      = 0x100,
    /* Operator event ID */
    /* type = uint16_t */
    MIPC_SIM_SML_RSU_IND_T_EVENT_ID                         = 0x101,
    /* Event string */
    /* type = string */
    MIPC_SIM_SML_RSU_IND_T_EVENT_STRING                     = 0x102,
};

    /* MIPC_MSG.SIM_VSIM_RESET_IND */
enum mipc_sim_vsim_reset_ind_tlv_enum {
    mipc_sim_vsim_reset_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_VSIM_APDU_IND */
enum mipc_sim_vsim_apdu_ind_tlv_enum {
    mipc_sim_vsim_apdu_ind_tlv_NONE = 0,
    /* APDU data */
    /* type = string */
    MIPC_SIM_VSIM_APDU_IND_T_DATA                           = 0x8100,
};

    /* MIPC_MSG.SIM_VSIM_EVENT_IND */
enum mipc_sim_vsim_event_ind_tlv_enum {
    mipc_sim_vsim_event_ind_tlv_NONE = 0,
    /* event */
    /* type = uint8_t */
    MIPC_SIM_VSIM_EVENT_IND_T_EVENT                         = 0x100,
    /* APDU data */
    /* type = string */
    MIPC_SIM_VSIM_EVENT_IND_T_DATA                          = 0x8101,
};

    /* MIPC_MSG.SIM_SIMAPP_IND */
enum mipc_sim_simapp_ind_tlv_enum {
    mipc_sim_simapp_ind_tlv_NONE = 0,
    /* Application ID */
    /* type = uint8_t, refer to SIM_APP_TYPE_EX */
    MIPC_SIM_SIMAPP_IND_T_APP_ID                            = 0x100,
    /* The identifier of the channel on which the application is running */
    /* type = uint8_t */
    MIPC_SIM_SIMAPP_IND_T_CH_ID                             = 0x101,
    /* Mobile Country Code */
    /* type = string */
    MIPC_SIM_SIMAPP_IND_T_MCC                               = 0x102,
    /* Mobile Network Code */
    /* type = string */
    MIPC_SIM_SIMAPP_IND_T_MNC                               = 0x103,
};

    /* MIPC_MSG.SIM_TEST_SIM_IND */
enum mipc_sim_test_sim_ind_tlv_enum {
    mipc_sim_test_sim_ind_tlv_NONE = 0,
    /* Inserted SIM card is a test SIM or not; */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SIM_TEST_SIM_IND_T_TEST_SIM                        = 0x100,
};

    /* MIPC_MSG.SIM_CT3G_IND */
enum mipc_sim_ct3g_ind_tlv_enum {
    mipc_sim_ct3g_ind_tlv_NONE = 0,
    /* is CT3G or not */
    /* type = uint8_t */
    MIPC_SIM_CT3G_IND_T_TYPE                                = 0x100,
};

    /* MIPC_MSG.SIM_CARD_TYPE_IND */
enum mipc_sim_card_type_ind_tlv_enum {
    mipc_sim_card_type_ind_tlv_NONE = 0,
    /*
      SIM/USIM state;
      0: SIM
      1: USIM
      2: No SIM and No USIM
    */
    /* type = uint8_t */
    MIPC_SIM_CARD_TYPE_IND_T_USIM_STATE                     = 0x100,
    /*
      UIM/CSIM state;
      0: UIM
      1: CSIM
      2: No UIM + no CSIM
      3: UIM + CSIM
    */
    /* type = uint8_t */
    MIPC_SIM_CARD_TYPE_IND_T_CSIM_STATE                     = 0x101,
    /*
      ISIM state;
      1: ISIM is supported
      2: No ISIM
    */
    /* type = uint8_t */
    MIPC_SIM_CARD_TYPE_IND_T_ISIM_STATE                     = 0x102,
};

    /* MIPC_MSG.SIM_SIMIND_IND */
enum mipc_sim_simind_ind_tlv_enum {
    mipc_sim_simind_ind_tlv_NONE = 0,
    /* Event type */
    /* type = uint8_t, refer to SIM_EVENT */
    MIPC_SIM_SIMIND_IND_T_EVENT                             = 0x100,
    /* Application ID */
    /* type = uint8_t */
    MIPC_SIM_SIMIND_IND_T_APP_ID                            = 0x101,
    /* SPN string */
    /* type = string */
    MIPC_SIM_SIMIND_IND_T_SPN                               = 0x102,
    /* IMSI string */
    /* type = string */
    MIPC_SIM_SIMIND_IND_T_IMSI                              = 0x8103,
    /* GID1 string */
    /* type = string */
    MIPC_SIM_SIMIND_IND_T_GID1                              = 0x104,
    /* PNN full name string */
    /* type = string */
    MIPC_SIM_SIMIND_IND_T_PNN_FULL_NAME                     = 0x105,
    /* IMPI string */
    /* type = string */
    MIPC_SIM_SIMIND_IND_T_IMPI                              = 0x106,
    /* File number, maximum is 10 */
    /* type = uint8_t */
    MIPC_SIM_SIMIND_IND_T_FILE_NUM                          = 0x107,
    /* File list; the maximum count of file is 10 */
    /* type = struct_array, refer to file_list */
    MIPC_SIM_SIMIND_IND_T_FILE_LIST                         = 0x8108,
    /* File list; the maximum count of file is 10 */
    /* type = struct, refer to file_list */
    MIPC_SIM_SIMIND_IND_T_FILE_TLV_ARRAY                    = 0x8109,
};

    /* MIPC_MSG.SIM_STATUS_CHANGED_IND */
enum mipc_sim_status_changed_ind_tlv_enum {
    mipc_sim_status_changed_ind_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_ESLOTESIM_STATE_IND */
enum mipc_sim_eslotesim_state_ind_tlv_enum {
    mipc_sim_eslotesim_state_ind_tlv_NONE = 0,
    /* esim support or not */
    /* type = uint8_t */
    MIPC_SIM_ESLOTESIM_STATE_IND_T_ESIM_SUPPORT             = 0x100,
    /* 0:psim, 1:esim */
    /* type = uint8_t */
    MIPC_SIM_ESLOTESIM_STATE_IND_T_ESIM_PSIM_STATE          = 0x101,
};

    /* MIPC_MSG.SIM_MEP_SLOTS_INFO_IND */
enum mipc_sim_mep_slots_info_ind_tlv_enum {
    mipc_sim_mep_slots_info_ind_tlv_NONE = 0,
    /* The total number of physical slot */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_PHY_SLOT_COUNT            = 0x100,
    /* The list of SIM card state. Array index is physical slot ID and array value is SIM card state. */
    /* type = uint8_t, refer to SIM_CARD_PRESENT_STATE */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_CARD_STATE                = 0x101,
    /* ATR string og the physical slot */
    /* type = string */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_ATR                       = 0x8102,
    /* EID string of the physical slot */
    /* type = string */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_EID                       = 0x8103,
    /* For MEP slot, port number is the logical SE interface of the physical slot; for a lagecy SIM or eSIM which does not support MEP, the port number is 1 */
    /* type = uint8_t */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_PORT_NUM                  = 0x8104,
    /* The detail information for all ports. For MEP slot, the port information is about Logical SE interface; For a lagecy SIM or eSIM which does not support MEP, the port information is about physical slot information */
    /* type = TLV, refer to mep_port_info */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_PORT_INFO                 = 0x8105,
    /* The MEP mode as defined in SGP.22 */
    /* type = uint8_t, refer to SIM_MEP_MODE */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_MEP_MODE                  = 0x106,
    /* The EID of SIM cards. The index of this array is aligned with the index of CARD_STATE (i.e., index of physical slots) */
    /* type = struct, refer to eid_byte_struct */
    MIPC_SIM_MEP_SLOTS_INFO_IND_T_EID_BYTE_TLV_ARRAY        = 0x8107,
};

    /* MIPC_MSG.SIM_HOTSWAP_DISABLE_GET_REQ */
enum mipc_sim_hotswap_disable_get_req_tlv_enum {
    mipc_sim_hotswap_disable_get_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_HOTSWAP_DISABLE_GET_CNF */
enum mipc_sim_hotswap_disable_get_cnf_tlv_enum {
    mipc_sim_hotswap_disable_get_cnf_tlv_NONE = 0,
    /* 1 means switch hotswap off, 0 means switch on.Default is switch on. */
    /* type = uint8_t */
    MIPC_SIM_HOTSWAP_DISABLE_GET_CNF_T_STATE                = 0x100,
};

    /* MIPC_MSG.SIM_HOTSWAP_DISABLE_SET_REQ */
enum mipc_sim_hotswap_disable_set_req_tlv_enum {
    mipc_sim_hotswap_disable_set_req_tlv_NONE = 0,
    /* 1 means switch hotswap off, 0 means switch on.Default is switch on. */
    /* type = uint8_t */
    MIPC_SIM_HOTSWAP_DISABLE_SET_REQ_T_STATE                = 0x100,
};

    /* MIPC_MSG.SIM_HOTSWAP_DISABLE_SET_CNF */
enum mipc_sim_hotswap_disable_set_cnf_tlv_enum {
    mipc_sim_hotswap_disable_set_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_RECOVERY_DISABLE_GET_REQ */
enum mipc_sim_recovery_disable_get_req_tlv_enum {
    mipc_sim_recovery_disable_get_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SIM_RECOVERY_DISABLE_GET_CNF */
enum mipc_sim_recovery_disable_get_cnf_tlv_enum {
    mipc_sim_recovery_disable_get_cnf_tlv_NONE = 0,
    /* 1 means switch recovery off, 0 means switch on.Default is switch on. */
    /* type = uint8_t */
    MIPC_SIM_RECOVERY_DISABLE_GET_CNF_T_STATE               = 0x100,
};

    /* MIPC_MSG.SIM_RECOVERY_DISABLE_SET_REQ */
enum mipc_sim_recovery_disable_set_req_tlv_enum {
    mipc_sim_recovery_disable_set_req_tlv_NONE = 0,
    /* 1 means switch recovery off, 0 means switch on.Default is switch on. */
    /* type = uint8_t */
    MIPC_SIM_RECOVERY_DISABLE_SET_REQ_T_STATE               = 0x100,
};

    /* MIPC_MSG.SIM_RECOVERY_DISABLE_SET_CNF */
enum mipc_sim_recovery_disable_set_cnf_tlv_enum {
    mipc_sim_recovery_disable_set_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

#endif /* __MIPC_MSG_SIM_CONST_H__ */
