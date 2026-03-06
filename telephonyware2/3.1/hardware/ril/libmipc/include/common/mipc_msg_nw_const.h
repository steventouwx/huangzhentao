#ifndef __MIPC_MSG_NW_CONST_H__
#define __MIPC_MSG_NW_CONST_H__

enum MIPC_NW_MSG_enum {
    MIPC_NW_MSG_NONE = 0,
    /* This command is adopted to acquire the current radio state. */
    MIPC_NW_GET_RADIO_STATE_REQ                             = 1025,
    MIPC_NW_GET_RADIO_STATE_CNF                             = 1026,

    /* This command is adopted to turn on/off the SW radio. */
    MIPC_NW_SET_RADIO_STATE_REQ                             = 1027,
    MIPC_NW_SET_RADIO_STATE_CNF                             = 1028,

    /* This command is used to select mobile network automatically or manually. */
    MIPC_NW_SET_REGISTER_STATE_REQ                          = 1029,
    MIPC_NW_SET_REGISTER_STATE_CNF                          = 1030,

    /* This command is used to acquire current network registration status */
    MIPC_NW_GET_REGISTER_STATE_REQ                          = 1031,
    MIPC_NW_GET_REGISTER_STATE_CNF                          = 1032,

    /* This command is used to acquire all available network operators. */
    MIPC_NW_GET_PLMN_LIST_REQ                               = 1033,
    MIPC_NW_GET_PLMN_LIST_CNF                               = 1034,

    /* This command is used to perform PS attach or detach */
    MIPC_NW_SET_PS_REQ                                      = 1035,
    MIPC_NW_SET_PS_CNF                                      = 1036,

    /* This command is used to acquire the current PS attach status. */
    MIPC_NW_GET_PS_REQ                                      = 1037,
    MIPC_NW_GET_PS_CNF                                      = 1038,

    /* This command is used to set signal report interval/threshold. Modem shall follow this setting to send MIPC_NW_SIGNAL_IND to user. This setting only keep in RAM, it will be default value if modem reboot. */
    MIPC_NW_SET_SIGNAL_REQ                                  = 1039,
    MIPC_NW_SET_SIGNAL_CNF                                  = 1040,

    /* If host do NOT set signal setting by MIPC_NW_SET_SIGNAL_REQ after modem power on, modem shall return default value. */
    MIPC_NW_GET_SIGNAL_REQ                                  = 1041,
    MIPC_NW_GET_SIGNAL_CNF                                  = 1042,

    /* This command is used to acquire preferred PLMN list. */
    MIPC_NW_GET_PREFERRED_PROVIDER_REQ                      = 1043,
    MIPC_NW_GET_PREFERRED_PROVIDER_CNF                      = 1044,

    /* This command is used to set preferred PLMN list. */
    MIPC_NW_SET_PREFERRED_PROVIDER_REQ                      = 1045,
    MIPC_NW_SET_PREFERRED_PROVIDER_CNF                      = 1046,

    /* This command is used to set home PLMN. */
    MIPC_NW_SET_HOME_PROVIDER_REQ                           = 1047,
    MIPC_NW_SET_HOME_PROVIDER_CNF                           = 1048,

    /* This command is used to acquire current home PLMN. */
    MIPC_NW_GET_HOME_PROVIDER_REQ                           = 1049,
    MIPC_NW_GET_HOME_PROVIDER_CNF                           = 1050,

    /* This command is used to acquire current LTE IA status. */
    MIPC_NW_GET_IA_STATUS_REQ                               = 1051,
    MIPC_NW_GET_IA_STATUS_CNF                               = 1052,

    /* This command is used to acquire NITZ information (UTC time and time zone). */
    MIPC_NW_GET_NITZ_REQ                                    = 1053,
    MIPC_NW_GET_NITZ_CNF                                    = 1054,

    /* enable/disable fast dormancy */
    MIPC_NW_SET_IDLE_HINT_REQ                               = 1055,
    MIPC_NW_SET_IDLE_HINT_CNF                               = 1056,

    /* get current fast dormancy status */
    MIPC_NW_GET_IDLE_HINT_REQ                               = 1057,
    MIPC_NW_GET_IDLE_HINT_CNF                               = 1058,

    /* This command is used to acquire current base station information. */
    MIPC_NW_GET_BASE_STATIONS_REQ                           = 1059,
    MIPC_NW_GET_BASE_STATIONS_CNF                           = 1060,

    /* This command is used to acquire location information. */
    MIPC_NW_GET_LOCATION_INFO_REQ                           = 1061,
    MIPC_NW_GET_LOCATION_INFO_CNF                           = 1062,

    /* This command is used to set RAT mode. */
    MIPC_NW_SET_RAT_REQ                                     = 1063,
    MIPC_NW_SET_RAT_CNF                                     = 1064,

    /* This command is used to translate PLMN ID to network name. */
    MIPC_NW_GET_PROVIDER_NAME_REQ                           = 1065,
    MIPC_NW_GET_PROVIDER_NAME_CNF                           = 1066,

    /* This command is used to acquire modem current RAT information. */
    MIPC_NW_GET_RAT_REQ                                     = 1067,
    MIPC_NW_GET_RAT_CNF                                     = 1068,

    /* Enable or disable 5G. */
    MIPC_NW_SET_NR_REQ                                      = 1069,
    MIPC_NW_SET_NR_CNF                                      = 1070,

    /* get current CS attach status */
    MIPC_NW_GET_CS_REQ                                      = 1071,
    MIPC_NW_GET_CS_CNF                                      = 1072,

    /* This command is used to acquire available band mode. */
    MIPC_NW_GET_BAND_MODE_REQ                               = 1073,
    MIPC_NW_GET_BAND_MODE_CNF                               = 1074,

    /* This command is used to set band mode information. */
    MIPC_NW_SET_BAND_MODE_REQ                               = 1075,
    MIPC_NW_SET_BAND_MODE_CNF                               = 1076,

    /* This command is used to set channel lock parameters. */
    MIPC_NW_SET_CHANNEL_LOCK_REQ                            = 1077,
    MIPC_NW_SET_CHANNEL_LOCK_CNF                            = 1078,

    /* This command is used to read channel lock parameters. */
    MIPC_NW_GET_CHANNEL_LOCK_REQ                            = 1079,
    MIPC_NW_GET_CHANNEL_LOCK_CNF                            = 1080,

    /* This command is used to abort PLMN List search. */
    MIPC_NW_SET_ABORT_PLMN_LIST_SEARCH_REQ                  = 1081,
    MIPC_NW_SET_ABORT_PLMN_LIST_SEARCH_CNF                  = 1082,

    /* This command is used to acquire the capability for preferred list of networks. */
    MIPC_NW_GET_POL_CAPABILITY_REQ                          = 1083,
    MIPC_NW_GET_POL_CAPABILITY_CNF                          = 1084,

    /* This command is used to set the order of preferred RAT modes for network selection. */
    MIPC_NW_SET_PREFER_RAT_REQ                              = 1085,
    MIPC_NW_SET_PREFER_RAT_CNF                              = 1086,

    /* Get the operator name by PLMN id, if not provide the PLMN id will use currently register network plmn id. */
    MIPC_NW_GET_OPERATOR_NAME_REQ                           = 1087,
    MIPC_NW_GET_OPERATOR_NAME_CNF                           = 1088,

    /* Enable or disable the ENDC. */
    MIPC_NW_SET_ENDC_CONFIG_REQ                             = 1089,
    MIPC_NW_SET_ENDC_CONFIG_CNF                             = 1090,

    /* enable/disable lte carrier aggregation switch. */
    MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ          = 1091,
    MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_CNF          = 1092,

    /* Set the PS/CS registration state and roaming type for modem. */
    MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ   = 1093,
    MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_CNF   = 1094,

    /* Get lte carrier aggregation switch. */
    MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_REQ          = 1095,
    MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF          = 1096,

    /* Get current band info(band and bandwidth). */
    MIPC_NW_GET_CURRENT_BAND_INFO_REQ                       = 1097,
    MIPC_NW_GET_CURRENT_BAND_INFO_CNF                       = 1098,

    /*
      Description:
      This command is used to trigger or abort the cell measurement process.The control point can use TLV ACTION to start or abort an ongoing cell measurement.
      since cell measurement process might take some time. The default ACTION is abort.
      If the control point issues a START MIPC_NW_CELL_MEASUREMENT_REQ then ABORT MIPC_NW_CELL_MEASUREMENT_REQ,once modem finishes processing ABORT,it'll first send MIPC_RESULT_FAILURE in MIPC_NW_CELL_MEASUREMENT_CNF 
      to respond to START MIPC_NW_CELL_MEASUREMENT_REQ.Then modem sends MIPC_RESULT_SUCCESS in MIPC_NW_CELL_MEASUREMENT_CNF,to respond to ABORT MIPC_NW_CELL_MEASUREMENT_REQ.
      If the control point does not provide any action in TLV ACTION,or sends in an empty MIPC_NW_CELL_MEASUREMENT_REQ.modem aborts the cell measurement.
      The control point can specify which RAT(s) to perform cell measurement by specifying TLV RAT.If TLV RAT is not provided, modem measures on all supported RAT(s).
      Modem can also specify which band to use for measurement using TLV LTE_BAND and TLV NR_BAND. If band info are not provided, modem measures all bands as default behavior.
      If TLV RAT and band info TLV are in conflicting configuration, TLV RAT takes precedence.
      For example, if the control point provides TLV RAT=LTE and TLV NR_BAND, modem will start cell measurement on all LTE bands.
      result:
      -MIPC_RESULT_SUCCESS
      -MIPC_RESULT_FAIL
    */
    MIPC_NW_CELL_MEASUREMENT_REQ                            = 1099,
    MIPC_NW_CELL_MEASUREMENT_CNF                            = 1100,

    /* This command is used to update the cell and band white list. */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ                   = 1101,
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_CNF                   = 1102,

    /* This command is used to acquire LTE/NR CA information, including cell index, cell state, band, bandwidth cw0_cqi, and cw1_cqi. */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_REQ                     = 1103,
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF                     = 1104,

    /* Get 5G option configuration. */
    MIPC_NW_GET_NR_REQ                                      = 1105,
    MIPC_NW_GET_NR_CNF                                      = 1106,

    /* Get srxlev information. */
    MIPC_NW_GET_SRXLEV_REQ                                  = 1107,
    MIPC_NW_GET_SRXLEV_CNF                                  = 1108,

    /* This command is used to set roaming mode configuration. */
    MIPC_NW_SET_ROAMING_MODE_REQ                            = 1109,
    MIPC_NW_SET_ROAMING_MODE_CNF                            = 1110,

    /* This command is used to acquire roaming mode configuration. */
    MIPC_NW_GET_ROAMING_MODE_REQ                            = 1111,
    MIPC_NW_GET_ROAMING_MODE_CNF                            = 1112,

    /* This command is used to enable/disable URC of the defined IND type. */
    MIPC_NW_SET_URC_ENABLE_REQ                              = 1113,
    MIPC_NW_SET_URC_ENABLE_CNF                              = 1114,

    /* Get plmn list with PLMNs arranged based on Signal Strength in the particular rat. */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ                     = 1115,
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF                     = 1116,

    /* This command is used to set PLMN of specified RAT for searching. */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ             = 1117,
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_CNF             = 1118,

    /* This command is used to select CSG manually. */
    MIPC_NW_SET_SELECT_FEMTOCELL_REQ                        = 1119,
    MIPC_NW_SET_SELECT_FEMTOCELL_CNF                        = 1120,

    /* config a2 offset. */
    MIPC_NW_SET_CONFIG_A2_OFFSET_REQ                        = 1121,
    MIPC_NW_SET_CONFIG_A2_OFFSET_CNF                        = 1122,

    /* config b1 offset. */
    MIPC_NW_SET_CONFIG_B1_OFFSET_REQ                        = 1123,
    MIPC_NW_SET_CONFIG_B1_OFFSET_CNF                        = 1124,

    /* report anbr. */
    MIPC_NW_SET_REPORT_ANBR_REQ                             = 1125,
    MIPC_NW_SET_REPORT_ANBR_CNF                             = 1126,

    /* This command is used to set whether the network registration status needs to be reported or not. */
    MIPC_NW_SET_NETWORK_EVENT_REQ                           = 1127,
    MIPC_NW_SET_NETWORK_EVENT_CNF                           = 1128,

    /* enable ca plus filter. */
    MIPC_NW_SET_ENABLE_CA_PLUS_FILTER_REQ                   = 1129,
    MIPC_NW_SET_ENABLE_CA_PLUS_FILTER_CNF                   = 1130,

    /* This command is used to abort femtocell list. */
    MIPC_NW_ABORT_FEMTOCELL_LIST_REQ                        = 1131,
    MIPC_NW_ABORT_FEMTOCELL_LIST_CNF                        = 1132,

    /* This command is used to acquire femtocell list. */
    MIPC_NW_GET_FEMTOCELL_LIST_REQ                          = 1133,
    MIPC_NW_GET_FEMTOCELL_LIST_CNF                          = 1134,

    /* SET_PSEUDO_CELL_MODE_REQ. */
    MIPC_NW_SET_PSEUDO_CELL_MODE_REQ                        = 1135,
    MIPC_NW_SET_PSEUDO_CELL_MODE_CNF                        = 1136,

    /* GET_PSEUDO_CELL_INFO REQ. */
    MIPC_NW_GET_PSEUDO_CELL_INFO_REQ                        = 1137,
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF                        = 1138,

    /* SET_ROAMING_ENABLE_REQ. */
    MIPC_NW_SET_ROAMING_ENABLE_REQ                          = 1139,
    MIPC_NW_SET_ROAMING_ENABLE_CNF                          = 1140,

    /* GET_ROAMING_ENABLE_REQ. */
    MIPC_NW_GET_ROAMING_ENABLE_REQ                          = 1141,
    MIPC_NW_GET_ROAMING_ENABLE_CNF                          = 1142,

    /* This command is used to set the mode of femtocell system. */
    MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ         = 1143,
    MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF         = 1144,

    /* This command is used to acquire the mode of femtocell system. */
    MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ       = 1145,
    MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF       = 1146,

    /* This command is used to set network report status level. */
    MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ                     = 1147,
    MIPC_NW_SET_NW_IND_REPORT_LEVEL_CNF                     = 1148,

    /* Set disable 2g capability. This request is operator specificed, for the common use, please use the project config. */
    MIPC_NW_SET_DISABLE_2G_REQ                              = 1149,
    MIPC_NW_SET_DISABLE_2G_CNF                              = 1150,

    /* Get disable 2G config. */
    MIPC_NW_GET_DISABLE_2G_REQ                              = 1151,
    MIPC_NW_GET_DISABLE_2G_CNF                              = 1152,

    /* Set RAT switch for 4G/5G */
    MIPC_NW_SET_SMART_RAT_SWITCH_REQ                        = 1153,
    MIPC_NW_SET_SMART_RAT_SWITCH_CNF                        = 1154,

    /* Get RAT switch for 4G/5G */
    MIPC_NW_GET_SMART_RAT_SWITCH_REQ                        = 1155,
    MIPC_NW_GET_SMART_RAT_SWITCH_CNF                        = 1156,

    /* This interface allows users to perform RF test of control TX power. */
    MIPC_NW_VSS_ANTENNA_CONF_REQ                            = 1157,
    MIPC_NW_VSS_ANTENNA_CONF_CNF                            = 1158,

    /* This interface allows users to perform RF test of control TX power. */
    MIPC_NW_VSS_ANTENNA_INFO_REQ                            = 1159,
    MIPC_NW_VSS_ANTENNA_INFO_CNF                            = 1160,

    /* This interface is used to notify modem the radio capability of host. */
    MIPC_NW_SET_RADIO_CAPABILITY_REQ                        = 1161,
    MIPC_NW_SET_RADIO_CAPABILITY_CNF                        = 1162,

    /* set cdma roaming preference. */
    MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_REQ                 = 1163,
    MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_CNF                 = 1164,

    /* query cdma roaming preference. */
    MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_REQ                 = 1165,
    MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_CNF                 = 1166,

    /* query barring info. */
    MIPC_NW_GET_BARRING_INFO_REQ                            = 1167,
    MIPC_NW_GET_BARRING_INFO_CNF                            = 1168,

    /* query ehrpd info. */
    MIPC_NW_GET_EHRPD_INFO_REQ                              = 1169,
    MIPC_NW_GET_EHRPD_INFO_CNF                              = 1170,

    /* query egmss info. */
    MIPC_NW_GET_EGMSS_REQ                                   = 1171,
    MIPC_NW_GET_EGMSS_CNF                                   = 1172,

    /* set cache endc connect mode. */
    MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ                 = 1173,
    MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_CNF                 = 1174,

    /* In some test, there maybe two cases that need exclusive behavior of the same handset, so need a mode to make sure work well. */
    MIPC_NW_SET_PS_TEST_MODE_REQ                            = 1175,
    MIPC_NW_SET_PS_TEST_MODE_CNF                            = 1176,

    /* Get ps test mode. */
    MIPC_NW_GET_PS_TEST_MODE_REQ                            = 1177,
    MIPC_NW_GET_PS_TEST_MODE_CNF                            = 1178,

    /* This command is used to set signal strength reporting criteria. */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ                  = 1179,
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_CNF                  = 1180,

    /* This command is used to acquire the information of the CA. */
    MIPC_NW_GET_ECAINFO_REQ                                 = 1181,
    MIPC_NW_GET_ECAINFO_CNF                                 = 1182,

    /* query Activity info. */
    MIPC_NW_GET_ACTIVITY_INFO_REQ                           = 1183,
    MIPC_NW_GET_ACTIVITY_INFO_CNF                           = 1184,

    /* This command is used to turn on/off CA. */
    MIPC_NW_SET_CA_REQ                                      = 1187,
    MIPC_NW_SET_CA_CNF                                      = 1188,

    /* This command is used to query LTE RRC state. */
    MIPC_NW_GET_LTE_RRC_STATE_REQ                           = 1189,
    MIPC_NW_GET_LTE_RRC_STATE_CNF                           = 1190,

    /* This command is used to retrieve LTE available/unavailable 1xRTT cell list. */
    MIPC_NW_GET_LTE_1XRTT_CELL_LIST_REQ                     = 1191,
    MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF                     = 1192,

    /* clear lte available file. */
    MIPC_NW_CLEAR_LTE_AVAILABLE_FILE_REQ                    = 1193,
    MIPC_NW_CLEAR_LTE_AVAILABLE_FILE_CNF                    = 1194,

    /* This command is used to get CA capability. */
    MIPC_NW_GET_CA_LINK_CAPABILITY_REQ                      = 1195,
    MIPC_NW_GET_CA_LINK_CAPABILITY_CNF                      = 1196,

    /* This command is used to get CA capability. */
    MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ                   = 1197,
    MIPC_NW_GET_CA_LINK_ENABLE_STATUS_CNF                   = 1198,

    /* This command is used to get TM9 enable status. */
    MIPC_NW_GET_TM9_ENABLE_STATUS_REQ                       = 1199,
    MIPC_NW_GET_TM9_ENABLE_STATUS_CNF                       = 1200,

    /* TM9 enable status */
    MIPC_NW_SET_TM9_ENABLE_STATUS_REQ                       = 1201,
    MIPC_NW_SET_TM9_ENABLE_STATUS_CNF                       = 1202,

    /* This command is used to query the configuration of MOA-DM/OTA-DM. */
    MIPC_NW_GET_OMADM_CONF_REQ                              = 1203,
    MIPC_NW_GET_OMADM_CONF_CNF                              = 1204,

    /* This command is used to configure the modem settings for MOA-DM/OTA-DM. */
    MIPC_NW_SET_OMADM_CONF_REQ                              = 1205,
    MIPC_NW_SET_OMADM_CONF_CNF                              = 1206,

    /* This command is used to get ca band mode. */
    MIPC_NW_GET_CA_BAND_MODE_REQ                            = 1207,
    MIPC_NW_GET_CA_BAND_MODE_CNF                            = 1208,

    /* This command is used to set the CA combination band list. */
    MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ                   = 1209,
    MIPC_NW_SET_CA_LINK_ENABLE_STATUS_CNF                   = 1210,

    /* This command is used to get the LTE related data. */
    MIPC_NW_GET_LTE_DATA_REQ                                = 1211,
    MIPC_NW_GET_LTE_DATA_CNF                                = 1212,

    /* This command is used to set TUW timer length */
    MIPC_NW_SET_TUW_TIMER_LENGTH_REQ                        = 1213,
    MIPC_NW_SET_TUW_TIMER_LENGTH_CNF                        = 1214,

    /* This command is used to get TUW timer length. */
    MIPC_NW_GET_TUW_TIMER_LENGTH_REQ                        = 1215,
    MIPC_NW_GET_TUW_TIMER_LENGTH_CNF                        = 1216,

    /* This command is used to get 5GUW info. */
    MIPC_NW_GET_5GUW_INFO_REQ                               = 1217,
    MIPC_NW_GET_5GUW_INFO_CNF                               = 1218,

    /* This command is used to get nr ca band. */
    MIPC_NW_GET_NR_CA_BAND_REQ                              = 1219,
    MIPC_NW_GET_NR_CA_BAND_CNF                              = 1220,

    /* This command is used to get nr5g subcarrier spacing in khz. */
    MIPC_NW_GET_NR_SCS_REQ                                  = 1221,
    MIPC_NW_GET_NR_SCS_CNF                                  = 1222,

    /* This command is used to get physical channel configs info. */
    MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_REQ                = 1223,
    MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF                = 1224,

    /* The request of set or update the OS_ID. */
    MIPC_NW_OS_ID_UPDATE_REQ                                = 1225,
    MIPC_NW_OS_ID_UPDATE_CNF                                = 1226,

    /* Start network scan */
    MIPC_NW_START_NETWORK_SCAN_REQ                          = 1227,
    MIPC_NW_START_NETWORK_SCAN_CNF                          = 1228,

    /* This command is used to abort network scan */
    MIPC_NW_STOP_NETWORK_SCAN_REQ                           = 1229,
    MIPC_NW_STOP_NETWORK_SCAN_CNF                           = 1230,

    /* This command is used to get whether it is supported to do network scan through MIPC_NW_START_NETWORK_SCAN_REQ. */
    MIPC_NW_GET_NETWORK_SCAN_SUPPORT_STATUS_REQ             = 1231,
    MIPC_NW_GET_NETWORK_SCAN_SUPPORT_STATUS_CNF             = 1232,

    /* This command is used to set the preferred NSSAI. */
    MIPC_NW_SET_PREF_NSSAI_REQ                              = 1233,
    MIPC_NW_SET_PREF_NSSAI_CNF                              = 1234,

    /* This command is used to set the default NSSAI. */
    MIPC_NW_SET_DEFAULT_NSSAI_REQ                           = 1235,
    MIPC_NW_SET_DEFAULT_NSSAI_CNF                           = 1236,

    /* This command is used to acquire UE NSSAI, including default, allowed and preferred NSSAI. */
    MIPC_NW_GET_NSSAI_REQ                                   = 1237,
    MIPC_NW_GET_NSSAI_CNF                                   = 1238,

    /* Set 5GUC timer length and band bitmap */
    MIPC_NW_SET_5GUC_REQ                                    = 1239,
    MIPC_NW_SET_5GUC_CNF                                    = 1240,

    /* Get 5GUC current setting */
    MIPC_NW_GET_5GUC_SETTING_REQ                            = 1241,
    MIPC_NW_GET_5GUC_SETTING_CNF                            = 1242,

    /* Get 5GUC info */
    MIPC_NW_GET_5GUC_INFO_REQ                               = 1243,
    MIPC_NW_GET_5GUC_INFO_CNF                               = 1244,

    /* This command is used to get first plmn. */
    MIPC_NW_GET_FIRST_PLMN_REQ                              = 1245,
    MIPC_NW_GET_FIRST_PLMN_CNF                              = 1246,

    /* set usage setting of the UE. */
    MIPC_NW_SET_UE_USAGE_SETTING_REQ                        = 1247,
    MIPC_NW_SET_UE_USAGE_SETTING_CNF                        = 1248,

    /* get usage setting of the UE. */
    MIPC_NW_GET_UE_USAGE_SETTING_REQ                        = 1249,
    MIPC_NW_GET_UE_USAGE_SETTING_CNF                        = 1250,

    /* This command is used to set cag status. */
    MIPC_NW_SET_CAG_STATUS_REQ                              = 1251,
    MIPC_NW_SET_CAG_STATUS_CNF                              = 1252,

    /* This command is used to set cag select mode. */
    MIPC_NW_SET_CAG_SELECT_MODE_REQ                         = 1253,
    MIPC_NW_SET_CAG_SELECT_MODE_CNF                         = 1254,

    /* This command is used to get cag list. */
    MIPC_NW_GET_CAG_LIST_REQ                                = 1255,
    MIPC_NW_GET_CAG_LIST_CNF                                = 1256,

    /* Set allowed mcc list req. The MAX num of allowed mcc is 20. If there is more than one MCC in this country, please set all MCC to the list. */
    MIPC_NW_SET_ALLOWED_MCC_LIST_REQ                        = 1257,
    MIPC_NW_SET_ALLOWED_MCC_LIST_CNF                        = 1258,

    /* enable/disable n3 rf */
    MIPC_NW_SET_N3_POWER_STATE_REQ                          = 1259,
    MIPC_NW_SET_N3_POWER_STATE_CNF                          = 1260,

    /* Set n3 gateway type */
    MIPC_NW_SET_N3_GW_TYPE_REQ                              = 1261,
    MIPC_NW_SET_N3_GW_TYPE_CNF                              = 1262,

    /* Same as the set mode of AT+CEDRXS, but can set the value of PAGING_TIME_WINDOW additionally. */
    MIPC_NW_SET_EDRX_SETTING_REQ                            = 1263,
    MIPC_NW_SET_EDRX_SETTING_CNF                            = 1264,

    /* Same as the read mode of AT+CEDRXS. Returns the current settings for each defined value of ACT. */
    MIPC_NW_GET_EDRX_SETTING_REQ                            = 1265,
    MIPC_NW_GET_EDRX_SETTING_CNF                            = 1266,

    /* AT+CEDRXRDP action command */
    MIPC_NW_SET_EDRX_REQ                                    = 1267,
    MIPC_NW_SET_EDRX_CNF                                    = 1268,

    /* Same as the set mode of AT+CPSMS. It controls the setting of the UE's power saving mode(PSM) parameters. */
    MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ               = 1269,
    MIPC_NW_SET_POWER_SAVING_MODE_SETTING_CNF               = 1270,

    /* Same as the read mode of AT+CPSMS. It returns the setting of the UE's power saving mode(PSM) parameters. */
    MIPC_NW_GET_POWER_SAVING_MODE_SETTING_REQ               = 1271,
    MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF               = 1272,

    /* Same as AT+CMICO=2,... It enables UE to request the use of MICO mode from the network or re-negotiate the use of MICO mode from the network and optionally request an active timer value, during the registration procedure. */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ   = 1273,
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF   = 1274,

    /* Same as the read mode of +CMICO, returns the current settings. */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ   = 1275,
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF   = 1276,

    /* This command is used to get latest ca measurement info. */
    MIPC_NW_GET_LATEST_CA_MEASUREMENT_REQ                   = 1277,
    MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF                   = 1278,

    /* This command is used to qurey the current cell infomation. */
    MIPC_NW_GET_CURRENT_CELL_INFO_REQ                       = 4353,
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF                       = 4354,

    /* Triggers the MT to select and register on a CAG cell. */
    MIPC_NW_SET_CAG_SELECTION_REQ                           = 4355,
    MIPC_NW_SET_CAG_SELECTION_CNF                           = 4356,

    /*
      Get the current CAG selection mode, the currently selected CAG cell information and the current Access Technology.
      If the ME is not camped on a CAG cell when read command is issued, an error will be issued.
    */
    MIPC_NW_GET_CAG_SELECTION_REQ                           = 4357,
    MIPC_NW_GET_CAG_SELECTION_CNF                           = 4358,

    /*
      Enables and disables automatic time zone update via NITZ.
    */
    MIPC_NW_SET_AUTO_UPDATE_NITZ_REQ                        = 4359,
    MIPC_NW_SET_AUTO_UPDATE_NITZ_CNF                        = 4360,

    /*
      Returns the current setting of automatic time zone update.
    */
    MIPC_NW_GET_AUTO_UPDATE_NITZ_REQ                        = 4361,
    MIPC_NW_GET_AUTO_UPDATE_NITZ_CNF                        = 4362,

    /*
      This command triggers the MUSIM capable UE to request the network to release the NAS signalling connection for 3GPP access in EPS, or 5GS due to activity on another USIM.
    */
    MIPC_NW_SET_NAS_CONNECTION_RELEASE_REQ                  = 4363,
    MIPC_NW_SET_NAS_CONNECTION_RELEASE_CNF                  = 4364,

    /* This command is used to enable or disable TDD+FDD, TDD+TDD, FDD+TDD, and FDD+FDD carrier aggregation combinations with Carrier Configuration */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ                         = 4365,
    MIPC_NW_SET_CAP_NRCA_OPTION_CNF                         = 4366,

    /*
      This command is used to enable or disable NR carrier aggregation combinations with Carrier Configuration
    */
    MIPC_NW_SET_CAP_BC_LIST_REQ                             = 4367,
    MIPC_NW_SET_CAP_BC_LIST_CNF                             = 4368,

    /*
      This command is used to set network congestion configuration into SASE.
    */
    MIPC_NW_CONGESTION_CFG_REQ                              = 4369,
    MIPC_NW_CONGESTION_CFG_CNF                              = 4370,

    /*
      This command is used to acquire network configuration information.
    */
    MIPC_NW_GET_ENWCFGINFO_REQ                              = 4371,
    MIPC_NW_GET_ENWCFGINFO_CNF                              = 4372,

    /*
      The request used to set barring rlf config.
    */
    MIPC_NW_SET_BARRING_RLF_CONFIG_REQ                      = 4373,
    MIPC_NW_SET_BARRING_RLF_CONFIG_CNF                      = 4374,

    /*
      The request is used to set RX MIMO CAP
    */
    MIPC_NW_SET_RX_MIMO_CAP_REQ                             = 4375,
    MIPC_NW_SET_RX_MIMO_CAP_CNF                             = 4376,

    /*
      This command is used to read or write MIPI (Mobile Industry Processor Interface) device.
    */
    MIPC_NW_MIPI_REQ                                        = 4377,
    MIPC_NW_MIPI_CNF                                        = 4378,

    /*
      This command is used to read or write BPI device.
    */
    MIPC_NW_BPI_REQ                                         = 4379,
    MIPC_NW_BPI_CNF                                         = 4380,

    /* Set "deactivating EN-DC" feature */
    MIPC_NW_SET_ENDC_DEACT_REQ                              = 4381,
    MIPC_NW_SET_ENDC_DEACT_CNF                              = 4382,

    /* Get "deactivating EN-DC" feature */
    MIPC_NW_GET_ENDC_DEACT_REQ                              = 4383,
    MIPC_NW_GET_ENDC_DEACT_CNF                              = 4384,

    /* Set "Enable SA Silence" feature */
    MIPC_NW_SET_SA_SILENCE_REQ                              = 4385,
    MIPC_NW_SET_SA_SILENCE_CNF                              = 4386,

    /* Get "Enable SA Silence" feature */
    MIPC_NW_GET_SA_SILENCE_REQ                              = 4387,
    MIPC_NW_GET_SA_SILENCE_CNF                              = 4388,

    /* Set "tx_pwr_reduction" feature */
    MIPC_NW_SET_TX_POWER_REDUCTION_REQ                      = 4389,
    MIPC_NW_SET_TX_POWER_REDUCTION_CNF                      = 4390,

    /* Get "tx_pwr_reduction" feature */
    MIPC_NW_GET_TX_POWER_REDUCTION_REQ                      = 4391,
    MIPC_NW_GET_TX_POWER_REDUCTION_CNF                      = 4392,

    /* Set "LTE overheating" feature */
    MIPC_NW_SET_LTE_OVERHEATING_REQ                         = 4393,
    MIPC_NW_SET_LTE_OVERHEATING_CNF                         = 4394,

    /* Get "LTE overheating" feature */
    MIPC_NW_GET_LTE_OVERHEATING_REQ                         = 4395,
    MIPC_NW_GET_LTE_OVERHEATING_CNF                         = 4396,

    /* Set "NR overheating" feature */
    MIPC_NW_SET_NR_OVERHEATING_REQ                          = 4397,
    MIPC_NW_SET_NR_OVERHEATING_CNF                          = 4398,

    /* Get "NR overheating" feature */
    MIPC_NW_GET_NR_OVERHEATING_REQ                          = 4399,
    MIPC_NW_GET_NR_OVERHEATING_CNF                          = 4400,

    /* Set "POWER_SAVING" feature */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ                        = 4401,
    MIPC_NW_SET_UAI_POWER_SAVING_CNF                        = 4402,

    /* Get "POWER_SAVING" feature */
    MIPC_NW_GET_UAI_POWER_SAVING_REQ                        = 4403,
    MIPC_NW_GET_UAI_POWER_SAVING_CNF                        = 4404,

    /* Set "RRC_RELEASE_PREFERENCE" feature */
    MIPC_NW_SET_RRC_RELEASE_PREFERENCE_REQ                  = 4405,
    MIPC_NW_SET_RRC_RELEASE_PREFERENCE_CNF                  = 4406,

    /* Get "RRC_RELEASE_PREFERENCE" feature */
    MIPC_NW_GET_RRC_RELEASE_PREFERENCE_REQ                  = 4407,
    MIPC_NW_GET_RRC_RELEASE_PREFERENCE_CNF                  = 4408,

    /* Set "Fake RI" feature. If another request with same RAT is sent before the completion of the current one, the modem will return MIPC_RESULT_BUSY. */
    MIPC_NW_SET_FAKE_RI_REQ                                 = 4409,
    MIPC_NW_SET_FAKE_RI_CNF                                 = 4410,

    /* Get "Fake RI" feature. If another request with same RAT is sent before the completion of the current one, the modem will return MIPC_RESULT_BUSY. */
    MIPC_NW_GET_FAKE_RI_REQ                                 = 4411,
    MIPC_NW_GET_FAKE_RI_CNF                                 = 4412,

    /* This command is used to customize upper bound and lower bound of different LTE events. */
    MIPC_NW_SET_MR_THRESH_BOUND_REQ                         = 4413,
    MIPC_NW_SET_MR_THRESH_BOUND_CNF                         = 4414,

    /* This interface is used to delete the FTAI information. */
    MIPC_NW_DELETE_FTAI_ENTRY_REQ                           = 4415,
    MIPC_NW_DELETE_FTAI_ENTRY_CNF                           = 4416,

    /* The request of set no service timer. */
    MIPC_NW_SET_NO_SERVICE_TIMER_REQ                        = 4421,
    MIPC_NW_SET_NO_SERVICE_TIMER_CNF                        = 4422,

    /* The request of get no service timer. */
    MIPC_NW_GET_NO_SERVICE_TIMER_REQ                        = 4423,
    MIPC_NW_GET_NO_SERVICE_TIMER_CNF                        = 4424,

    /* This command is used to set ant bias. */
    MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ                        = 4429,
    MIPC_NW_SET_DYNAMIC_ANT_BIAS_CNF                        = 4430,

    /* This command is used to set tx power by band. */
    MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_REQ                   = 4431,
    MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_CNF                   = 4432,

    /* This command is used to get the CSCON state. */
    MIPC_NW_GET_CSCON_STATE_REQ                             = 4433,
    MIPC_NW_GET_CSCON_STATE_CNF                             = 4434,

    /* This command is used to DA prefer threshold. */
    MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ                 = 4437,
    MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_CNF                 = 4438,

    /* This command is used to set MAX EIRP. */
    MIPC_NW_SET_MAX_EIRP_REQ                                = 4439,
    MIPC_NW_SET_MAX_EIRP_CNF                                = 4440,

    /*
      This interface is used to clear the NSSAI configuartion. then cnf msg will response the clear result.
      result: If clear success, will return MIPC_RESULT_SUCCESS, otherwise return MIPC_RESULT_FAILURE.
    */
    MIPC_NW_CLEAR_NSSAI_REQ                                 = 4441,
    MIPC_NW_CLEAR_NSSAI_CNF                                 = 4442,

    /* This interface is used to NR local release and NR to LTE redirection. */
    MIPC_NW_LOCAL_REL_WITH_REDIR_TO_LTE_REQ                 = 4443,
    MIPC_NW_LOCAL_REL_WITH_REDIR_TO_LTE_CNF                 = 4444,

    /*
      This command is used to trigger service scan with service type and specific RATs.
      Take EPS fallback for example, UE registers on NR and wants to make a VoLTE call. UE needs to trigger EPS fallback to 4G first. In this scenario, UE can use this command to start normal service scan with AVAILABLE_RAT is 4G/5G and PREFER_RAT is 4G.
      User should always send this command with ACTION "STOP" when the service scan finish. If user receives MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_CNF, it means that the command is received by modem, but it does not mean that it is finished.
      AP or internal MTK IMS can use this command to trigger EPS fallback
    */
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ                = 4445,
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_CNF                = 4446,

    /*
      The control point uses this command to set the security configuration.
      If TLV ALLOW_NULL_ALGO=FALSE, then modem does not allow any network communications with null ciphering (both signaling and user data) or null integrity (signaling) modes for 3G and above even if the network only uses null algorithms.
      This setting must be respected even if 'cipheringDisabled' (as defined in TS 38.331) is in use by the network.
      For 2G, which does not use integrity protection, the modem must only disallow any network communications with null ciphering.
      Integrity protection for user data is optional and ciphering is required when TLV ALLOW_NULL_ALGO=FALSE.
      In case of emergency, the modem must bypass this setting.
    */
    MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ            = 4449,
    MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_CNF            = 4450,

    /* The get of the null ciphering NULL algo allowed config state. */
    MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_REQ            = 4451,
    MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF            = 4452,

    /*
      The control point sets the user configuration and preferences using this command. At least one TLVs must be present in the REQ.
      Modem returns TLV RESULT = MIPC_RESULT_PARTIAL_FAILURE if one or more configurations are not successful.
      The control point must process the corresponding configuration's RESULT TLV to learn if the setting is configured successfully or not.
      For example, if TLV CELLULAR_ID_DISCLOSURE is set in REQ, the control point must process TLV CELLULAR_ID_DISCLOSURE_RESULT to learn about the result of this configuration.
    */
    MIPC_NW_SET_CONFIG_REQ                                  = 4453,
    MIPC_NW_SET_CONFIG_CNF                                  = 4454,

    /*
      The control point retrieves the user configuration and preferences using this command.
      At least one TLVs must be present in the REQ else modem rejects with MIPC_RESULT_MISSING_ARGUMENT in CNF.
      Modem returns TLV RESULT = MIPC_RESULT_PARTIAL_FAILURE if one or more configurations are not retrieved successfully.
      The control point must process the corresponding configuration's RESULT TLV to learn if the setting is successfully retrieved or not.
      For example, if TLV RETRIEVE_CELLULAR_ID_DISCLOSURE is set in REQ, the control point must process TLV CELLULAR_ID_DISCLOSURE_RESULT to learn about the result of this configuration.
    */
    MIPC_NW_GET_CONFIG_REQ                                  = 4455,
    MIPC_NW_GET_CONFIG_CNF                                  = 4456,

    /* This command will be reported when the registration state is changed. */
    MIPC_NW_REGISTER_IND                                    = 17409,

    /* This command is used to report the signal information when signal has changed. */
    MIPC_NW_SIGNAL_IND                                      = 17410,

    /*
      This command is used to report the network information when the PS attach state is changed.
      User can use MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ to configure different report level. Refer to MIPC_NW_SET_NW_IND_REPORT_LEVEL for the detail of configuration.
    */
    MIPC_NW_PS_IND                                          = 17411,

    /* This command is used to report the radio information when the radio status is changed. */
    MIPC_NW_RADIO_IND                                       = 17412,

    /* This command is used to report the initial attach information when the initial attach status changed. */
    MIPC_NW_IA_IND                                          = 17413,

    /* This command will be reported when NITZ is changed. */
    MIPC_NW_NITZ_IND                                        = 17414,

    /* This command is used to report the location information when the location is changed. */
    MIPC_NW_LOCATION_INFO_IND                               = 17415,

    /* CS state change indication. */
    MIPC_NW_CS_IND                                          = 17416,

    /* This command is used to report the signalling when the signalling connection status has changed. */
    MIPC_NW_CSCON_IND                                       = 17417,

    /* This command will be reported when preferred provider list is changed by network flow. */
    MIPC_NW_PREFERRED_PROVIDER_IND                          = 17418,

    /* dl and ul ca info of lte and nr. */
    MIPC_NW_CAINFO_IND                                      = 17419,

    /* This command will be reported when the EONS (Enhanced Operator Name Display) table is ready, and it alse indicates whether PLMN network name and operator name list service are enabled. */
    MIPC_NW_EONS_IND                                        = 17420,

    /* ciev state change indication. */
    MIPC_NW_CIEV_IND                                        = 17421,

    /* Inform the information of system selection, according to RAT, location and attach status. */
    MIPC_NW_EGMSS_IND                                       = 17422,

    /* This command is used to report the capability of packet switching data service. */
    MIPC_NW_PSBEARER_IND                                    = 17423,

    /* This command is used to report the cell information when the preferred provider list is changed by network flow. */
    MIPC_NW_ECELL_IND                                       = 17424,

    /* anbr changed indication */
    MIPC_NW_ANBR_IND                                        = 17425,

    /* irat changed indication */
    MIPC_NW_IRAT_IND                                        = 17426,

    /* This command is used to report network registration status. */
    MIPC_NW_EREGINFO_IND                                    = 17427,

    /* This command is used to report the modulation scheme of the active RAT. */
    MIPC_NW_EMODCFG_IND                                     = 17428,

    /* on lte network info. */
    MIPC_NW_EPCELLINFO_IND                                  = 17429,

    /* Pseudo_cell info urc. */
    MIPC_NW_PSEUDO_CELL_IND                                 = 17430,

    /* Pseudo_cell info urc. */
    MIPC_NW_NETWORK_INFO_IND                                = 17431,

    /* This command is used to report the PLMN ID when the PLMN ID has changed. */
    MIPC_NW_MCCMNC_IND                                      = 17432,

    /* Physical channel config changed indication. */
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND                    = 17433,

    /* the status of the OTASP. */
    MIPC_NW_OTACMSG_IND                                     = 17434,

    /* the status of the OTASP. */
    MIPC_NW_BARRING_INFO_IND                                = 17435,

    /* This interface is used to report radio capability to host. */
    MIPC_NW_RADIO_CAPABILITY_IND                            = 17436,

    /* This command is used to report the current RAT setting stored in the modem. */
    MIPC_NW_CURRENT_RAT_IND                                 = 17437,

    /* This command is used to report the camping status of the cell. */
    MIPC_NW_CAMP_STATE_IND                                  = 17438,

    /* This interface is used to report the sim which has nr capability to host. */
    MIPC_NW_NR_SWITCH_IND                                   = 17439,

    /* This interface is used to report the femtocell information. */
    MIPC_NW_FEMTOCELL_INFO_IND                              = 17440,

    /* onNotifyTXPower URC. */
    MIPC_NW_ETXPWR_IND                                      = 17441,

    /* handleTXPowerStatus URC. */
    MIPC_NW_ETXPWRSTUS_IND                                  = 17442,

    /* Indicates the status about IWLAN. */
    MIPC_NW_IWLAN_IND                                       = 17443,

    /* handle ECHINFO URC */
    MIPC_NW_CH_INFO_IND                                     = 17444,

    /* handle E5GUW URC */
    MIPC_NW_NRUW_INFO_IND                                   = 17445,

    /* NR ca bands */
    MIPC_NW_NR_CA_BAND_IND                                  = 17446,

    /* nr5g subcarrier spacing in khz */
    MIPC_NW_NR_SCS_IND                                      = 17447,

    /* Network scan result */
    MIPC_NW_NETWORK_SCAN_IND                                = 17448,

    /* CA info */
    MIPC_NW_CA_INFO_IND                                     = 17449,

    /* 5GUC info */
    MIPC_NW_NRUC_INFO_IND                                   = 17450,

    /* The first plmn */
    MIPC_NW_FIRST_PLMN_IND                                  = 17451,

    /* N3 register state info */
    MIPC_NW_N3_REG_STATE_IND                                = 17452,

    /* URC +CEDRXP, report the eDRX info */
    MIPC_NW_EDRX_IND                                        = 17453,

    /* URC +CMICO, report the MICO information */
    MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND       = 17454,

    /* Handle the NETWORK reject URC. */
    MIPC_NW_ENWREJAC_IND                                    = 17455,

    /* The basement detection */
    MIPC_NW_BASEMENT_DETECTION_IND                          = 17456,

    /* This command is used to report network configuration information. */
    MIPC_NW_ENWCFGINFO_IND                                  = 17457,

    /* The lcm hopping ind */
    MIPC_NW_LCM_HOPPING_IND                                 = 17459,

    /* This command is used to report NRRC RRC release UAI. */
    MIPC_NW_NRRC_RRC_RELEASE_UAI_IND                        = 17460,

    /* This command is used to report NRRC power saving UAI. */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND                        = 17461,

    /* This command is used to report Overheating UAI. */
    MIPC_NW_NRRC_OVERHEATINGASSISTANCE_UAI_IND              = 17462,

    /* This command is used to report NRRC timer status. */
    MIPC_NW_NRRC_TIMER_STATUS_IND                           = 17463,

    /* This command is used to report EPS fallback status. */
    MIPC_NW_EPSFB_IND                                       = 17466,

    /* This command is used to report TAU fail. */
    MIPC_NW_TAU_FAIL_IND                                    = 17467,

    /* This indication reports the cellular ID disclosure information. This is only sent when the control point has configured TLV CELLULAR_ID_DISCLOSURE via MIPC_NW_SET_CONFIG_REQ. */
    MIPC_NW_CELLULAR_ID_DISCLOSURE_IND                      = 17471,

    /* This indication reports the security Algorithms Update infomation. This is only sent when the control point has configured TLV ALLOW_NULL_ALGO via MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ. */
    MIPC_NW_SECURITY_ALGOS_IND                              = 17475,
};

    /* MIPC_MSG.NW_GET_RADIO_STATE_REQ */
enum mipc_nw_get_radio_state_req_tlv_enum {
    mipc_nw_get_radio_state_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_RADIO_STATE_CNF */
enum mipc_nw_get_radio_state_cnf_tlv_enum {
    mipc_nw_get_radio_state_cnf_tlv_NONE = 0,
    /* SW radio state */
    /* type = uint8_t, refer to NW_RADIO_STATE */
    MIPC_NW_GET_RADIO_STATE_CNF_T_SW_STATE                  = 0x100,
    /* HW radio state */
    /* type = uint8_t, refer to NW_RADIO_STATE */
    MIPC_NW_GET_RADIO_STATE_CNF_T_HW_STATE                  = 0x101,
};

    /* MIPC_MSG.NW_SET_RADIO_STATE_REQ */
enum mipc_nw_set_radio_state_req_tlv_enum {
    mipc_nw_set_radio_state_req_tlv_NONE = 0,
    /* SW radio state */
    /* type = uint8_t, refer to NW_RADIO_STATE */
    MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE                  = 0x100,
    /* Reason for turning on/off radio; Default value is 0. */
    /* type = uint8_t, refer to NW_RADIO_STATE_CAUSE */
    MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE                     = 0x101,
    /* HW radio state (only can be set by intenal AP) */
    /* type = uint8_t, refer to NW_RADIO_STATE */
    MIPC_NW_SET_RADIO_STATE_REQ_T_HW_STATE                  = 0x102,
};

    /* MIPC_MSG.NW_SET_RADIO_STATE_CNF */
enum mipc_nw_set_radio_state_cnf_tlv_enum {
    mipc_nw_set_radio_state_cnf_tlv_NONE = 0,
    /* SW radio state */
    /* type = uint8_t, refer to NW_RADIO_STATE */
    MIPC_NW_SET_RADIO_STATE_CNF_T_SW_STATE                  = 0x100,
    /* HW radio state */
    /* type = uint8_t, refer to NW_RADIO_STATE */
    MIPC_NW_SET_RADIO_STATE_CNF_T_HW_STATE                  = 0x101,
};

    /* MIPC_MSG.NW_SET_REGISTER_STATE_REQ */
enum mipc_nw_set_register_state_req_tlv_enum {
    mipc_nw_set_register_state_req_tlv_NONE = 0,
    /* Auto mode or manual mode */
    /* type = uint8_t, refer to NW_REGISTER_MODE */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_MODE                   = 0x100,
    /* It indicates the format of operator name, which is not used in current stage; if network is registered in manual mode, the format of operator name should be provided; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_FORMAT                 = 0x101,
    /* It indicates PLMN ID; if network is registered in manual mode, the PLMN ID (e.g. 46001) should be provided; Default value is null character. */
    /* type = string */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_OPER                   = 0x8102,
    /* Radio access technology; Default value is 0xFFFF. */
    /* type = uint8_t, refer to NW_ACT_TYPE */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_ACT                    = 0x103,
    /* It is used to select PLMN for specific ARFCN; the integer type range is 0~0xFFFFFFFF; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_ARFCN                  = 0x104,
    /* RAT mode; refer to AT+ERAT; Default value is 255. */
    /* type = uint8_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_RAT_MODE               = 0x105,
    /* REQ handling mode: 0 - non block, the response will return once send the cmd; 1 - block, the response will return until the register action finished; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_BLOCK                  = 0x106,
    /* 0: default is COPS; 1: EOPS; 2: COPS=2; 3: EECREGS=0. NOTE: if this not appear, MD will according to the mode and so on to decide which at_mode used. */
    /* type = uint8_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_CTRL_MODE              = 0x107,
    /* REQ search type: 0 - fullband; 1 - stored only. Just for mode == 7; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_SEARCH_TYPE            = 0x108,
    /* REQ rsrp;Just for mode == 7; Default value is 0. */
    /* type = int32_t */
    MIPC_NW_SET_REGISTER_STATE_REQ_T_RSRP                   = 0x109,
};

    /* MIPC_MSG.NW_SET_REGISTER_STATE_CNF */
enum mipc_nw_set_register_state_cnf_tlv_enum {
    mipc_nw_set_register_state_cnf_tlv_NONE = 0,
    /* [Phase out] Please use STATE_V1. */
    /* type = struct, refer to nw_reg_state */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_STATE                  = 0x100,
    /* Network error enum; refer to 3GPP 24.008 Annex G. */
    /* type = uint16_t */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_NW_ERR                 = 0x101,
    /* Current network register mode: auto or manual */
    /* type = uint8_t, refer to NW_REGISTER_MODE */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_MODE                   = 0x102,
    /* This represents the supported data speed for current radio technologies */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_DATA_SPEED             = 0x103,
    /* Provider short name */
    /* type = string */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_NW_NAME                = 0x104,
    /* A string to indicate the roaming */
    /* type = string */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_ROAMING_TEXT           = 0x105,
    /* Operation error; refer to NW_ERR */
    /* type = uint16_t */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_FAIL_CAUSE             = 0x106,
    /* Provider long name */
    /* type = string */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_NW_LONG_NAME           = 0x107,
    /* The information of registration */
    /* type = struct, refer to nw_reg_info */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_PS_INFO                = 0x108,
    /* Packet service register state */
    /* type = struct, refer to nw_reg_state_v1 */
    MIPC_NW_SET_REGISTER_STATE_CNF_T_STATE_V1               = 0x109,
};

    /* MIPC_MSG.NW_GET_REGISTER_STATE_REQ */
enum mipc_nw_get_register_state_req_tlv_enum {
    mipc_nw_get_register_state_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_REGISTER_STATE_CNF */
enum mipc_nw_get_register_state_cnf_tlv_enum {
    mipc_nw_get_register_state_cnf_tlv_NONE = 0,
    /* [Phase out] Please use STATE_V1. */
    /* type = struct, refer to nw_reg_state */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_STATE                  = 0x100,
    /* Network error enum; refer to 3GPP 24.008 Annex G. */
    /* type = uint16_t */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_NW_ERR                 = 0x101,
    /* Auto mode or manual mode */
    /* type = uint8_t, refer to NW_REGISTER_MODE */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_MODE                   = 0x102,
    /* This represents the supported data speed for current radio technologies */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_DATA_SPEED             = 0x103,
    /* Provider short name */
    /* type = string */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_NW_NAME                = 0x104,
    /* A string to indicate the roaming */
    /* type = string */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_ROAMING_TEXT           = 0x105,
    /* The operation is fail, refer to the network error for details */
    /* type = uint16_t */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_FAIL_CAUSE             = 0x106,
    /* PLMN ID (e.g. 46001) */
    /* type = string */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_ID                = 0x107,
    /* Provider long name */
    /* type = string */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_NW_LONG_NAME           = 0x108,
    /* The information of registration */
    /* type = struct, refer to nw_reg_info */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_PS_INFO                = 0x109,
    /* Packet service register state */
    /* type = struct, refer to nw_reg_state_v1 */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_STATE_V1               = 0x10A,
    /* Indicates the encoding scheme for TLV PLMN_SHORT_NAME */
    /* type = uint8_t, refer to ENCODING_SCHEME */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_SHORT_NAME_ENCODING_SCHEME = 0x10B,
    /* PLMN short name */
    /* type = byte_array */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_SHORT_NAME        = 0x10C,
    /* Indicates the encoding scheme for TLV PLMN_LONG_NAME */
    /* type = uint8_t, refer to ENCODING_SCHEME */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_LONG_NAME_ENCODING_SCHEME = 0x10D,
    /* PLMN long name */
    /* type = byte_array */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_LONG_NAME         = 0x10E,
    /* Indicates PLMN name source */
    /* type = uint8_t, refer to PLMN_NAME_SOURCE */
    MIPC_NW_GET_REGISTER_STATE_CNF_T_PLMN_NAME_SOURCE       = 0x10F,
};

    /* MIPC_MSG.NW_GET_PLMN_LIST_REQ */
enum mipc_nw_get_plmn_list_req_tlv_enum {
    mipc_nw_get_plmn_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PLMN_LIST_CNF */
enum mipc_nw_get_plmn_list_cnf_tlv_enum {
    mipc_nw_get_plmn_list_cnf_tlv_NONE = 0,
    /* The number of PLMN list */
    /* type = uint8_t */
    MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_COUNT                  = 0x100,
    /* PLMN list element */
    /* type = struct_array, refer to nw_provider */
    MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_LIST                   = 0x101,
    /* The cause of failure for this operation; refer to 3GPP TS 31.102 */
    /* type = uint16_t */
    MIPC_NW_GET_PLMN_LIST_CNF_T_FAIL_CAUSE                  = 0x102,
    /* Extended PLMN list element */
    /* type = struct_array, refer to nw_extend_provider */
    MIPC_NW_GET_PLMN_LIST_CNF_T_EXTEND_INFO_LIST            = 0x103,
    /* PLMN list element */
    /* type = struct, refer to nw_provider */
    MIPC_NW_GET_PLMN_LIST_CNF_T_INFO_TLV_ARRAY              = 0x104,
    /* Extended PLMN list element */
    /* type = struct, refer to nw_extend_provider */
    MIPC_NW_GET_PLMN_LIST_CNF_T_EXTEND_INFO_TLV_ARRAY       = 0x105,
};

    /* MIPC_MSG.NW_SET_PS_REQ */
enum mipc_nw_set_ps_req_tlv_enum {
    mipc_nw_set_ps_req_tlv_NONE = 0,
    /* Attach or Detach; Default value is 1. */
    /* type = uint8_t, refer to NW_PS */
    MIPC_NW_SET_PS_REQ_T_ACTION                             = 0x100,
    /* Control which used to do the ps attach/detach. The default behavior is CGATT; Default value is 0. */
    /* type = uint8_t, refer to NW_PS_CTRL_MODE */
    MIPC_NW_SET_PS_REQ_T_CTRL_MODE                          = 0x101,
};

    /* MIPC_MSG.NW_SET_PS_CNF */
enum mipc_nw_set_ps_cnf_tlv_enum {
    mipc_nw_set_ps_cnf_tlv_NONE = 0,
    /* It indicates PS state is detach or attach */
    /* type = uint8_t, refer to NW_PS */
    MIPC_NW_SET_PS_CNF_T_TACH                               = 0x100,
    MIPC_NW_SET_PS_CNF_T_STATE                              = 0x100,
    /* This represents the supported data speed for current radio technologies */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_SET_PS_CNF_T_DATA_SPEED                         = 0x101,
    /* For 5G only: Sub-6 or mmWave */
    /* type = uint8_t */
    MIPC_NW_SET_PS_CNF_T_NW_FREQUENCY                       = 0x102,
    /* The cause of failure for this operation */
    /* type = uint16_t */
    MIPC_NW_SET_PS_CNF_T_FAIL_CAUSE                         = 0x103,
};

    /* MIPC_MSG.NW_GET_PS_REQ */
enum mipc_nw_get_ps_req_tlv_enum {
    mipc_nw_get_ps_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PS_CNF */
enum mipc_nw_get_ps_cnf_tlv_enum {
    mipc_nw_get_ps_cnf_tlv_NONE = 0,
    /* It indicates PS state is detach or attach */
    /* type = uint8_t, refer to NW_PS */
    MIPC_NW_GET_PS_CNF_T_TACH                               = 0x100,
    MIPC_NW_GET_PS_CNF_T_STATE                              = 0x100,
    /* This represents the supported data speed for current radio technologies */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_GET_PS_CNF_T_DATA_SPEED                         = 0x101,
    /* For 5G only: Sub-6 or mmWave */
    /* type = uint8_t */
    MIPC_NW_GET_PS_CNF_T_NW_FREQUENCY                       = 0x102,
    /* The cause of failure for this operation */
    /* type = uint16_t */
    MIPC_NW_GET_PS_CNF_T_FAIL_CAUSE                         = 0x103,
    /* [Phase out] Please use REG_INFO_V1. */
    /* type = struct, refer to nw_ps_reg_info */
    MIPC_NW_GET_PS_CNF_T_REG_INFO                           = 0x104,
    /* The cell type */
    /* type = uint8_t, refer to NW_CELL_TYPE */
    MIPC_NW_GET_PS_CNF_T_CELL_TYPE                          = 0x105,
    /* The cell info: mipc_nw_gsm_cell_struct4 and so on; if the type of cell is CELL_TYPE_NSA_EXT, this field indicates the LTE cell */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_GET_PS_CNF_T_CELL_INFO                          = 0x8106,
    /* [Phase out] Please use NSA_EXT_CELL_INFO_V2. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_GET_PS_CNF_T_NSA_EXT_CELL_INFO                  = 0x8107,
    /* The gsm cell info: mipc_nw_gsm_cell_struct4. */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_GET_PS_CNF_T_GSM_CELL_INFO                      = 0x8108,
    /* The umts cell info: mipc_nw_umts_cell_struct4. */
    /* type = struct, refer to nw_umts_cell */
    MIPC_NW_GET_PS_CNF_T_UMTS_CELL_INFO                     = 0x8109,
    /* The LTE cell information: mipc_nw_lte_cell_struct4.NOTE: WHEN CELL TYPE IS CELL_TYPE_NSA_EXT, CELL INFO IS THE LTE CELL */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_GET_PS_CNF_T_LTE_CELL_INFO                      = 0x810A,
    /* [Phase out] Please use NR_CELL_INFO_V2 */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_GET_PS_CNF_T_NR_CELL_INFO                       = 0x810B,
    /* [Phase out] Please use CDMA_CELL_INFO_V1 */
    /* type = struct, refer to nw_cdma_cell */
    MIPC_NW_GET_PS_CNF_T_CDMA_CELL_INFO                     = 0x810C,
    /* The PS data registration state */
    /* type = struct, refer to nw_ps_reg_info_v1 */
    MIPC_NW_GET_PS_CNF_T_REG_INFO_V1                        = 0x10E,
    /* NR cell information. NOTE: JUST VALID WHEN CELL_TYPE IS CELL_TYPE_NSA_EXT, AND CELL_INFO IS LTE CELL. */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_GET_PS_CNF_T_NSA_EXT_CELL_INFO_V2               = 0x8110,
    /* NR cell information */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_GET_PS_CNF_T_NR_CELL_INFO_V2                    = 0x8111,
    /* C2K cell information */
    /* type = struct, refer to nw_cdma_cell_v1 */
    MIPC_NW_GET_PS_CNF_T_CDMA_CELL_INFO_V1                  = 0x8112,
};

    /* MIPC_MSG.NW_SET_SIGNAL_REQ */
enum mipc_nw_set_signal_req_tlv_enum {
    mipc_nw_set_signal_req_tlv_NONE = 0,
    /* The signal report interval, the default value is 5 seconds */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REQ_T_SIGNAL_STRENGTH_INTERVAL       = 0x100,
    /* The threshold for RSSI, the default value is 5 */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REQ_T_RSSI_THRESHOLD                 = 0x101,
    /* The threshold for error rate, the default value is 0 */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REQ_T_ERR_RATE_THRESHOLD             = 0x102,
    /* The threshold for RSRP, the default value is 2 db */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REQ_T_RSRP_THRESHOLD                 = 0x103,
    /* The threshold for SNR, the default value is 0 db */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REQ_T_SNR_THRESHOLD                  = 0x104,
    /*
      1: unit of RSRP_THRESHOLD: qdbm; unit of SNR_THRESHOLD: qdb;
      0: default value; unit of RSRP_THRESHOLD: dbm; unit of SNR_THRESHOLD: db
    */
    /* type = uint8_t, refer to NW_SIGNAL_THRESHOLD_MODE */
    MIPC_NW_SET_SIGNAL_REQ_T_THRESHOLD_MODE                 = 0x105,
};

    /* MIPC_MSG.NW_SET_SIGNAL_CNF */
enum mipc_nw_set_signal_cnf_tlv_enum {
    mipc_nw_set_signal_cnf_tlv_NONE = 0,
    /* The RSSI for current signal status */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_RSSI                           = 0x100,
    /* The error rate for current signal status */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_ERR_RATE                       = 0x101,
    /* The RSRP for current signal status */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_RSRP                           = 0x102,
    /* The SNR for current signal status */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_SNR                            = 0x103,
    /* Current setting for signal report interval */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_SIGNAL_STRENGTH_INTERVAL       = 0x104,
    /* Current setting for RSSI threshold */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_RSSI_THRESHOLD                 = 0x105,
    /* Current setting for error rate threshold */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_ERR_RATE_THRESHOLD             = 0x106,
    /* Current setting for RSRP threshold */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_RSRP_THRESHOLD                 = 0x107,
    /* Current setting for SNR threshold */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_CNF_T_SNR_THRESHOLD                  = 0x108,
    /* The cause of failure for this operation */
    /* type = uint16_t */
    MIPC_NW_SET_SIGNAL_CNF_T_FAIL_CAUSE                     = 0x109,
    /* Signal type; if the type is NSA, it should contain LTE and(or) NR signal info */
    /* type = uint8_t, refer to NW_SIGNAL_TYPE */
    MIPC_NW_SET_SIGNAL_CNF_T_SIGNAL_TYPE                    = 0x10A,
    /* GSM signal strength */
    /* type = struct, refer to nw_gsm_signal_strength */
    MIPC_NW_SET_SIGNAL_CNF_T_GSM_SIGNAL                     = 0x10B,
    /* UMTS signal strength */
    /* type = struct, refer to nw_umts_signal_strength */
    MIPC_NW_SET_SIGNAL_CNF_T_UMTS_SIGNAL                    = 0x10C,
    /* LTE signal strength */
    /* type = struct, refer to nw_lte_signal_strength */
    MIPC_NW_SET_SIGNAL_CNF_T_LTE_SIGNAL                     = 0x10D,
    /* NR signal strength */
    /* type = struct, refer to nw_nr_signal_strength */
    MIPC_NW_SET_SIGNAL_CNF_T_NR_SIGNAL                      = 0x10E,
    /* [Phase out] Please use RAW_SIGNAL_INFO_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_raw_signal_info */
    MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_LIST           = 0x10F,
    /* The raw signal info count; if the network registered in NSA, the count is 2; otherwise, the count is 1 */
    /* type = uint8_t */
    MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_COUNT          = 0x110,
    /* CDMA signal strength */
    /* type = struct, refer to nw_cdma_signal_strength */
    MIPC_NW_SET_SIGNAL_CNF_T_CDMA_SIGNAL                    = 0x111,
    /* [Phase out] Please use RAW_SIGNAL_INFO_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_raw_signal_info */
    MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_TLV_ARRAY      = 0x112,
    /* The raw signal data information; All RATs share this struct */
    /* type = struct, refer to nw_raw_signal_info_v1 */
    MIPC_NW_SET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_V1_TLV_ARRAY   = 0x113,
};

    /* MIPC_MSG.NW_GET_SIGNAL_REQ */
enum mipc_nw_get_signal_req_tlv_enum {
    mipc_nw_get_signal_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_SIGNAL_CNF */
enum mipc_nw_get_signal_cnf_tlv_enum {
    mipc_nw_get_signal_cnf_tlv_NONE = 0,
    /* The RSSI for current signal status */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_RSSI                           = 0x100,
    /* The error rate for current signal status */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_ERR_RATE                       = 0x101,
    /* The RSRP for current signal status */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_RSRP                           = 0x102,
    /* The SNR for current signal status */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_SNR                            = 0x103,
    /* Current setting for signal reporting interval, the default value is 5 seconds */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_SIGNAL_STRENGTH_INTERVAL       = 0x104,
    /* Current RSSI threshold setting for signal reporting, the default value is 5 */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_RSSI_THRESHOLD                 = 0x105,
    /* Current error rate threshold setting for signal reporting, the default value is 0 */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_ERR_RATE_THRESHOLD             = 0x106,
    /* Current RSRP threshold setting for signal reporting, the default value is 2 db */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_RSRP_THRESHOLD                 = 0x107,
    /* Current SNR threshold setting for signal reporting, the default value is 0 db */
    /* type = uint32_t */
    MIPC_NW_GET_SIGNAL_CNF_T_SNR_THRESHOLD                  = 0x108,
    /* The cause of failure for operation */
    /* type = uint16_t */
    MIPC_NW_GET_SIGNAL_CNF_T_FAIL_CAUSE                     = 0x109,
    /* Signal type; if the type is NSA, it should contain LTE and(or) NR signal info */
    /* type = uint8_t, refer to NW_SIGNAL_TYPE */
    MIPC_NW_GET_SIGNAL_CNF_T_SIGNAL_TYPE                    = 0x10A,
    /* GSM signal strength */
    /* type = struct, refer to nw_gsm_signal_strength */
    MIPC_NW_GET_SIGNAL_CNF_T_GSM_SIGNAL                     = 0x10B,
    /* UMTS signal strength */
    /* type = struct, refer to nw_umts_signal_strength */
    MIPC_NW_GET_SIGNAL_CNF_T_UMTS_SIGNAL                    = 0x10C,
    /* [Phase out] Please use LTE_SIGNAL_V2. */
    /* type = struct, refer to nw_lte_signal_strength */
    MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL                     = 0x10D,
    /* NR signal strength */
    /* type = struct, refer to nw_nr_signal_strength */
    MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL                      = 0x10E,
    /* [Phase out] Please use RAW_SIGNAL_INFO_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_raw_signal_info */
    MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_LIST           = 0x10F,
    /* The raw signal info count; if the network registered in NSA, the count is 2; otherwise, the count is 1 */
    /* type = uint8_t */
    MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_COUNT          = 0x110,
    /* CDMA signal strength */
    /* type = struct, refer to nw_cdma_signal_strength */
    MIPC_NW_GET_SIGNAL_CNF_T_CDMA_SIGNAL                    = 0x111,
    /* [Phase out] Please use LTE_SIGNAL_V2. */
    /* type = struct, refer to nw_lte_signal_strength_v1 */
    MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL_V1                  = 0x112,
    /* [Phase out] Please use NR_SIGNAL_V3. */
    /* type = struct, refer to nw_nr_signal_strength_v1 */
    MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL_V1                   = 0x113,
    /* NR signal strength include cqi/ta info. */
    /* type = struct, refer to nw_nr_signal_strength_v2 */
    MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL_V2                   = 0x114,
    /* [Phase out] Please use RAW_SIGNAL_INFO_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_raw_signal_info */
    MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_TLV_ARRAY      = 0x115,
    /* NR signal strength include CQI and TA info. */
    /* type = struct, refer to nw_nr_signal_strength_v3 */
    MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL_V3                   = 0x116,
    /* LTE signal strength */
    /* type = struct, refer to nw_lte_signal_strength_v2 */
    MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL_V2                  = 0x117,
    /* The raw signal data information; All RATs share this struct */
    /* type = struct, refer to nw_raw_signal_info_v1 */
    MIPC_NW_GET_SIGNAL_CNF_T_RAW_SIGNAL_INFO_V1_TLV_ARRAY   = 0x118,
};

    /* MIPC_MSG.NW_GET_PREFERRED_PROVIDER_REQ */
enum mipc_nw_get_preferred_provider_req_tlv_enum {
    mipc_nw_get_preferred_provider_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PREFERRED_PROVIDER_CNF */
enum mipc_nw_get_preferred_provider_cnf_tlv_enum {
    mipc_nw_get_preferred_provider_cnf_tlv_NONE = 0,
    /* The provider list length */
    /* type = uint8_t */
    MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_COUNT              = 0x100,
    /*  */
    /* type = struct_array, refer to nw_provider */
    MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_LIST               = 0x101,
    /* The cause of failure for current operation; refer to 3GPP TS 31.102 */
    /* type = uint16_t */
    MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_FAIL_CAUSE         = 0x102,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_GET_PREFERRED_PROVIDER_CNF_T_NW_PROVIDER_TLV_ARRAY = 0x103,
};

    /* MIPC_MSG.NW_SET_PREFERRED_PROVIDER_REQ */
enum mipc_nw_set_preferred_provider_req_tlv_enum {
    mipc_nw_set_preferred_provider_req_tlv_NONE = 0,
    /* The provider list length; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_COUNT              = 0x100,
    /*  */
    /* type = struct_array, refer to nw_provider */
    MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_LIST               = 0x101,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_SET_PREFERRED_PROVIDER_REQ_T_NW_PROVIDER_TLV_ARRAY = 0x102,
};

    /* MIPC_MSG.NW_SET_PREFERRED_PROVIDER_CNF */
enum mipc_nw_set_preferred_provider_cnf_tlv_enum {
    mipc_nw_set_preferred_provider_cnf_tlv_NONE = 0,
    /* The provider list length */
    /* type = uint8_t */
    MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_COUNT              = 0x100,
    /*  */
    /* type = struct_array, refer to nw_provider */
    MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_LIST               = 0x101,
    /* The cause of failure for current operation; refer to 3GPP TS 31.102 */
    /* type = uint16_t */
    MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_FAIL_CAUSE         = 0x102,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_SET_PREFERRED_PROVIDER_CNF_T_NW_PROVIDER_TLV_ARRAY = 0x103,
};

    /* MIPC_MSG.NW_SET_HOME_PROVIDER_REQ */
enum mipc_nw_set_home_provider_req_tlv_enum {
    mipc_nw_set_home_provider_req_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_SET_HOME_PROVIDER_REQ_T_PROVIDER                = 0x100,
};

    /* MIPC_MSG.NW_SET_HOME_PROVIDER_CNF */
enum mipc_nw_set_home_provider_cnf_tlv_enum {
    mipc_nw_set_home_provider_cnf_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_SET_HOME_PROVIDER_CNF_T_PROVIDER                = 0x8100,
    /* The cause of failure for current operation */
    /* type = uint16_t */
    MIPC_NW_SET_HOME_PROVIDER_CNF_T_FAIL_CAUSE              = 0x101,
};

    /* MIPC_MSG.NW_GET_HOME_PROVIDER_REQ */
enum mipc_nw_get_home_provider_req_tlv_enum {
    mipc_nw_get_home_provider_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_HOME_PROVIDER_CNF */
enum mipc_nw_get_home_provider_cnf_tlv_enum {
    mipc_nw_get_home_provider_cnf_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_GET_HOME_PROVIDER_CNF_T_PROVIDER                = 0x8100,
    /* The cause of failure for current operation */
    /* type = uint16_t */
    MIPC_NW_GET_HOME_PROVIDER_CNF_T_FAIL_CAUSE              = 0x101,
};

    /* MIPC_MSG.NW_GET_IA_STATUS_REQ */
enum mipc_nw_get_ia_status_req_tlv_enum {
    mipc_nw_get_ia_status_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_IA_STATUS_CNF */
enum mipc_nw_get_ia_status_cnf_tlv_enum {
    mipc_nw_get_ia_status_cnf_tlv_NONE = 0,
    /* APN name */
    /* type = string */
    MIPC_NW_GET_IA_STATUS_CNF_T_APN                         = 0x100,
    /* The RAT info */
    /* type = uint8_t */
    MIPC_NW_GET_IA_STATUS_CNF_T_RAT                         = 0x101,
    /*  */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_NW_GET_IA_STATUS_CNF_T_PDP_TYPE                    = 0x102,
    /*  */
    /* type = uint8_t, refer to APN_AUTH_TYPE */
    MIPC_NW_GET_IA_STATUS_CNF_T_AUTH_TYPE                   = 0x103,
    /* User ID */
    /* type = string */
    MIPC_NW_GET_IA_STATUS_CNF_T_USERID                      = 0x8104,
    /* Password for the user ID */
    /* type = string */
    MIPC_NW_GET_IA_STATUS_CNF_T_PASSWORD                    = 0x8105,
    /* The cause of attach reject */
    /* type = uint32_t */
    MIPC_NW_GET_IA_STATUS_CNF_T_CAUSE                       = 0x106,
};

    /* MIPC_MSG.NW_GET_NITZ_REQ */
enum mipc_nw_get_nitz_req_tlv_enum {
    mipc_nw_get_nitz_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NITZ_CNF */
enum mipc_nw_get_nitz_cnf_tlv_enum {
    mipc_nw_get_nitz_cnf_tlv_NONE = 0,
    /*  */
    /* type = struct, refer to nw_nitz_info */
    MIPC_NW_GET_NITZ_CNF_T_INFO                             = 0x100,
    /* Whether time_zone_offset_minutes in INFO field is valid; 0: invalid; 1: valid */
    /* type = uint8_t */
    MIPC_NW_GET_NITZ_CNF_T_TZ_VALID                         = 0x101,
};

    /* MIPC_MSG.NW_SET_IDLE_HINT_REQ */
enum mipc_nw_set_idle_hint_req_tlv_enum {
    mipc_nw_set_idle_hint_req_tlv_NONE = 0,
    /* Control the set operation; Default value is 0. */
    /* type = uint8_t, refer to NW_FAST_DORMANCY */
    MIPC_NW_SET_IDLE_HINT_REQ_T_STATUS                      = 0x100,
    /* ID(0~255) of the timer to be set when STATUS is SET_TIMER; Indicate screen is on(1) or off(0) when STATUS is SCREEN_STATUS; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_SET_IDLE_HINT_REQ_T_PARAM_1                     = 0x101,
    /* Just used when STATUS is SET_TIMER, it represents the value(0~65535) of timer to be set in unit 0.1 second; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_SET_IDLE_HINT_REQ_T_PARAM_2                     = 0x102,
};

    /* MIPC_MSG.NW_SET_IDLE_HINT_CNF */
enum mipc_nw_set_idle_hint_cnf_tlv_enum {
    mipc_nw_set_idle_hint_cnf_tlv_NONE = 0,
    /*  */
    /* type = uint8_t, refer to NW_FAST_DORMANCY */
    MIPC_NW_SET_IDLE_HINT_CNF_T_STATUS                      = 0x100,
    /* The fail cause for current operation. */
    /* type = uint16_t */
    MIPC_NW_SET_IDLE_HINT_CNF_T_FAIL_CAUSE                  = 0x101,
};

    /* MIPC_MSG.NW_GET_IDLE_HINT_REQ */
enum mipc_nw_get_idle_hint_req_tlv_enum {
    mipc_nw_get_idle_hint_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_IDLE_HINT_CNF */
enum mipc_nw_get_idle_hint_cnf_tlv_enum {
    mipc_nw_get_idle_hint_cnf_tlv_NONE = 0,
    /*  */
    /* type = uint8_t, refer to NW_FAST_DORMANCY */
    MIPC_NW_GET_IDLE_HINT_CNF_T_STATUS                      = 0x100,
    /* The fail cause for current operation. */
    /* type = uint16_t */
    MIPC_NW_GET_IDLE_HINT_CNF_T_FAIL_CAUSE                  = 0x101,
    /* 0: current network doesn't support R8 Dormancy; 1: current network supports R8 Fast Dormancy. */
    /* type = uint8_t */
    MIPC_NW_GET_IDLE_HINT_CNF_T_R8_FD_STATUS                = 0x102,
};

    /* MIPC_MSG.NW_GET_BASE_STATIONS_REQ */
enum mipc_nw_get_base_stations_req_tlv_enum {
    mipc_nw_get_base_stations_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_BASE_STATIONS_CNF */
enum mipc_nw_get_base_stations_cnf_tlv_enum {
    mipc_nw_get_base_stations_cnf_tlv_NONE = 0,
    /* The GSM cell list length */
    /* type = uint32_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_COUNT          = 0x100,
    /*  */
    /* type = struct_array, refer to nw_gsm_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_LIST           = 0x8101,
    /* The UTMS cell list length */
    /* type = uint32_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_COUNT         = 0x102,
    /*  */
    /* type = struct_array, refer to nw_umts_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_LIST          = 0x8103,
    /* The TDSCDMA cell list length */
    /* type = uint32_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_TDSCDMA_CELL_COUNT      = 0x104,
    /*  */
    /* type = struct_array, refer to nw_tdscdma_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_TDSCDMA_CELL_LIST       = 0x8105,
    /* The LTE cell list length */
    /* type = uint32_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_COUNT          = 0x106,
    /*  */
    /* type = struct_array, refer to nw_lte_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST           = 0x8107,
    /* The CDMA cell list length */
    /* type = uint32_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_COUNT         = 0x108,
    /* [Phase out] Please use CDMA_CELL_V1_TLV_ARRAY */
    /* type = struct_array, refer to nw_cdma_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_LIST          = 0x8109,
    /* The NR cell list length */
    /* type = uint32_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_COUNT           = 0x10A,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY instead of NR_CELL_COUNT and NR_CELL_LIST. */
    /* type = struct_array, refer to nw_nr_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST            = 0x810B,
    /* The cause of failure for current operation */
    /* type = uint16_t */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_FAIL_CAUSE              = 0x10C,
    /* [Phase out] Please use LTE_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_lte_cell_v1 */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST_V1        = 0x810D,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_nr_cell_v1 */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST_V1         = 0x810E,
    /*  */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_TLV_ARRAY      = 0x810F,
    /*  */
    /* type = struct, refer to nw_umts_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_TLV_ARRAY     = 0x8110,
    /*  */
    /* type = struct, refer to nw_tdscdma_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_TDSCDMA_CELL_TLV_ARRAY  = 0x8111,
    /* [Phase out] Please use LTE_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_TLV_ARRAY      = 0x8112,
    /* [Phase out] Please use CDMA_CELL_V1_TLV_ARRAY */
    /* type = struct, refer to nw_cdma_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_TLV_ARRAY     = 0x8113,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_TLV_ARRAY       = 0x8114,
    /* NR cell information. */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_V2_TLV_ARRAY    = 0x8115,
    /* LTE cell information */
    /* type = struct, refer to nw_lte_cell_v2 */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_V2_TLV_ARRAY   = 0x8116,
    /* C2K cell information */
    /* type = struct, refer to nw_cdma_cell_v1 */
    MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_V1_TLV_ARRAY  = 0x8117,
};

    /* MIPC_MSG.NW_GET_LOCATION_INFO_REQ */
enum mipc_nw_get_location_info_req_tlv_enum {
    mipc_nw_get_location_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LOCATION_INFO_CNF */
enum mipc_nw_get_location_info_cnf_tlv_enum {
    mipc_nw_get_location_info_cnf_tlv_NONE = 0,
    /* The information of location area and tracking area */
    /* type = struct, refer to nw_location_info */
    MIPC_NW_GET_LOCATION_INFO_CNF_T_INFO                    = 0x8100,
    /* The failure cause for current operation, refer to 3GPP 27.007 clause 9.1.2. */
    /* type = uint16_t */
    MIPC_NW_GET_LOCATION_INFO_CNF_T_FAIL_CAUSE              = 0x101,
};

    /* MIPC_MSG.NW_SET_RAT_REQ */
enum mipc_nw_set_rat_req_tlv_enum {
    mipc_nw_set_rat_req_tlv_NONE = 0,
    /* The RAT info. This parameter is for compability with previous version. Please use the BIT_RAT first; Default value is 0xFF. */
    /* type = uint8_t */
    MIPC_NW_SET_RAT_REQ_T_RAT                               = 0x100,
    /* The perfer RAT info; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_RAT_REQ_T_PREFER_RAT                        = 0x101,
    /* The RAT information with bit format; Default value is 255. */
    /* type = uint32_t, refer to NW_BIT_RAT */
    MIPC_NW_SET_RAT_REQ_T_BIT_RAT                           = 0x102,
};

    /* MIPC_MSG.NW_SET_RAT_CNF */
enum mipc_nw_set_rat_cnf_tlv_enum {
    mipc_nw_set_rat_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PROVIDER_NAME_REQ */
enum mipc_nw_get_provider_name_req_tlv_enum {
    mipc_nw_get_provider_name_req_tlv_NONE = 0,
    /* PLMN of operator network */
    /* type = string */
    MIPC_NW_GET_PROVIDER_NAME_REQ_T_PLMN_ID                 = 0x100,
    /* Location Area Code; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_GET_PROVIDER_NAME_REQ_T_LAC                     = 0x101,
    /* The default value is 0 which means MIPC_NW_GET_PROVIDER_NAME_CNF just return the NW_NAME and NW_LONG_NAME; when the value set to 1, MIPC_NW_GET_PROVIDER_NAME_CNF will try to return the information of all names (EONS/NITZ/TS25) if this information exists */
    /* type = uint8_t */
    MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPTION                  = 0x102,
    /* Default value is 0, which means it will check registration status of PLMN from EONS API for returning operator name, otherwise depend on OPL_INDEX value. */
    /* type = uint8_t */
    MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPL_INDEX               = 0x103,
};

    /* MIPC_MSG.NW_GET_PROVIDER_NAME_CNF */
enum mipc_nw_get_provider_name_cnf_tlv_enum {
    mipc_nw_get_provider_name_cnf_tlv_NONE = 0,
    /* PLMN of operator network */
    /* type = string */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_ID                 = 0x8100,
    /* Location Area Code */
    /* type = uint32_t */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_LAC                     = 0x8101,
    /* Provider name. The short name priority is EONS, NITZ, and TS25 sequentially. */
    /* type = string */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME                 = 0x8102,
    /* Provider name. The long name priority is EONS, NITZ, and TS25 sequentially. */
    /* type = string */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME_LONG            = 0x8103,
    /* The EONS (Enhanced Operator Name String) name for the PLMN */
    /* type = struct, refer to nw_name_pair */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_EONS_NAME               = 0x8104,
    /* The NITZ (Network Identity and Time Zone) name for the PLMN */
    /* type = struct, refer to nw_name_pair */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_NITZ_NAME               = 0x8105,
    /* The TS25 short name for the PLMN */
    /* type = struct, refer to nw_name_pair */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_TS25_NAME               = 0x8106,
    /* Indicates the encoding scheme for TLV PLMN_SHORT_NAME */
    /* type = uint8_t, refer to ENCODING_SCHEME */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_SHORT_NAME_ENCODING_SCHEME = 0x107,
    /* PLMN short name */
    /* type = byte_array */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_SHORT_NAME         = 0x108,
    /* Indicates the encoding scheme for TLV PLMN_LONG_NAME */
    /* type = uint8_t, refer to ENCODING_SCHEME */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_LONG_NAME_ENCODING_SCHEME = 0x109,
    /* PLMN long name */
    /* type = byte_array */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_LONG_NAME          = 0x10A,
    /* Indicates PLMN name source */
    /* type = uint8_t, refer to PLMN_NAME_SOURCE */
    MIPC_NW_GET_PROVIDER_NAME_CNF_T_PLMN_NAME_SOURCE        = 0x10B,
};

    /* MIPC_MSG.NW_GET_RAT_REQ */
enum mipc_nw_get_rat_req_tlv_enum {
    mipc_nw_get_rat_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_RAT_CNF */
enum mipc_nw_get_rat_cnf_tlv_enum {
    mipc_nw_get_rat_cnf_tlv_NONE = 0,
    /* Access technology of current register PLMN. */
    /* type = uint8_t, refer to NW_ACCESS_TECH */
    MIPC_NW_GET_RAT_CNF_T_ACT                               = 0x100,
    /* It indicates GPRS status; 0: GPRS; 1: EDGE */
    /* type = uint8_t */
    MIPC_NW_GET_RAT_CNF_T_GPRS_STATUS                       = 0x101,
    /* RAT mode setting of MS */
    /* type = uint8_t */
    MIPC_NW_GET_RAT_CNF_T_RAT_MODE                          = 0x102,
    /* Discarded; use AT+EPRATL to set preferred RAT list */
    /* type = uint8_t */
    MIPC_NW_GET_RAT_CNF_T_PREFER_RAT                        = 0x103,
    /*
      0: Unlock; it indicates RAT can be set
      1: Lock; it indicates RAT cannot be set
    */
    /* type = uint8_t, refer to NW_RAT_LOCK */
    MIPC_NW_GET_RAT_CNF_T_LOCK                              = 0x104,
    /* The RAT information with bit format. */
    /* type = bitmap, refer to NW_BIT_RAT */
    MIPC_NW_GET_RAT_CNF_T_BIT_RAT                           = 0x105,
    /* The capability of current protocol */
    /* type = bitmap, refer to NW_BIT_RAT */
    MIPC_NW_GET_RAT_CNF_T_RADIO_CAPABILITY                  = 0x106,
};

    /* MIPC_MSG.NW_SET_NR_REQ */
enum mipc_nw_set_nr_req_tlv_enum {
    mipc_nw_set_nr_req_tlv_NONE = 0,
    /* 1: LTE ONLY(disable NSA+SA); 3: SA ONLY; 5: NSA ONLY; 7: NSA+SA. */
    /* type = uint8_t */
    MIPC_NW_SET_NR_REQ_T_NR_OPT                             = 0x100,
    /* Control if need to do the local release; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_VG_OPTION_OPERATION */
    MIPC_NW_SET_NR_REQ_T_ACT_OPERATION                      = 0x101,
};

    /* MIPC_MSG.NW_SET_NR_CNF */
enum mipc_nw_set_nr_cnf_tlv_enum {
    mipc_nw_set_nr_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CS_REQ */
enum mipc_nw_get_cs_req_tlv_enum {
    mipc_nw_get_cs_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CS_CNF */
enum mipc_nw_get_cs_cnf_tlv_enum {
    mipc_nw_get_cs_cnf_tlv_NONE = 0,
    /* [Phase out] Please use REG_INFO_V1. */
    /* type = struct, refer to nw_cs_reg_info */
    MIPC_NW_GET_CS_CNF_T_REG_INFO                           = 0x8100,
    /* The cell type. */
    /* type = uint8_t, refer to NW_CELL_TYPE */
    MIPC_NW_GET_CS_CNF_T_CELL_TYPE                          = 0x101,
    /* The cell info: mipc_nw_gsm_cell_struct4 and so on. */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_GET_CS_CNF_T_CELL_INFO                          = 0x8102,
    /* The gsm cell info: mipc_nw_gsm_cell_struct4. */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_GET_CS_CNF_T_GSM_CELL_INFO                      = 0x8103,
    /* The umts cell info: mipc_nw_umts_cell_struct4. */
    /* type = struct, refer to nw_umts_cell */
    MIPC_NW_GET_CS_CNF_T_UMTS_CELL_INFO                     = 0x8104,
    /* The lte cell info: mipc_nw_lte_cell_struct4. */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_GET_CS_CNF_T_LTE_CELL_INFO                      = 0x8105,
    /* [Phase out] Please use NR_CELL_INFO_V2. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_GET_CS_CNF_T_NR_CELL_INFO                       = 0x8106,
    /* [Phase out] Please use CDMA_CELL_INFO_V1 */
    /* type = struct, refer to nw_cdma_cell */
    MIPC_NW_GET_CS_CNF_T_CDMA_CELL_INFO                     = 0x8107,
    /* CS registration info. */
    /* type = struct, refer to nw_cs_reg_info_v1 */
    MIPC_NW_GET_CS_CNF_T_REG_INFO_V1                        = 0x8108,
    /* NR cell information */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_GET_CS_CNF_T_NR_CELL_INFO_V2                    = 0x8109,
    /* C2K cell information */
    /* type = struct, refer to nw_cdma_cell_v1 */
    MIPC_NW_GET_CS_CNF_T_CDMA_CELL_INFO_V1                  = 0x810A,
};

    /* MIPC_MSG.NW_GET_BAND_MODE_REQ */
enum mipc_nw_get_band_mode_req_tlv_enum {
    mipc_nw_get_band_mode_req_tlv_NONE = 0,
    /* NW_BAND_OPTION_CURRENT_SETTING is adopted to acquire current setting bands information; NW_BAND_OPTION_SYSTEM_SUPPORT is adopted to return all system supports bands; the default option is NW_BAND_OPTION_CURRENT_SETTING */
    /* type = uint8_t, refer to NW_BAND_OPTION */
    MIPC_NW_GET_BAND_MODE_REQ_T_OPTION                      = 0x100,
};

    /* MIPC_MSG.NW_GET_BAND_MODE_CNF */
enum mipc_nw_get_band_mode_cnf_tlv_enum {
    mipc_nw_get_band_mode_cnf_tlv_NONE = 0,
    /* A bitmap that represents the supported GSM bands */
    /* type = bitmap, refer to SYS_CAP_GSM_BAND */
    MIPC_NW_GET_BAND_MODE_CNF_T_GSM_BAND                    = 0x100,
    /* A bitmap that represents the supported UMTS bands */
    /* type = bitmap, refer to SYS_CAP_UMTS_BAND */
    MIPC_NW_GET_BAND_MODE_CNF_T_UMTS_BAND                   = 0x101,
    /* A bitmap that represents the supported LTE bands */
    /* type = struct, refer to sys_lte_band */
    MIPC_NW_GET_BAND_MODE_CNF_T_LTE_BAND                    = 0x102,
    /* A bitmap that represents the supported NR bands */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_GET_BAND_MODE_CNF_T_NR_BAND                     = 0x103,
    /* A bitmap that represents the supported NR NSA bands; if NR_SA_BAND equals to NR_NSA_BAND, it indicates NR_NSA_BAND doesn't exist */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_GET_BAND_MODE_CNF_T_NR_NSA_BAND                 = 0x104,
};

    /* MIPC_MSG.NW_SET_BAND_MODE_REQ */
enum mipc_nw_set_band_mode_req_tlv_enum {
    mipc_nw_set_band_mode_req_tlv_NONE = 0,
    /* A bitmap that represents the supported GSM bands */
    /* type = bitmap, refer to SYS_CAP_GSM_BAND */
    MIPC_NW_SET_BAND_MODE_REQ_T_GSM_BAND                    = 0x100,
    /* A bitmap that represents the supported UMTS bands */
    /* type = bitmap, refer to SYS_CAP_UMTS_BAND */
    MIPC_NW_SET_BAND_MODE_REQ_T_UMTS_BAND                   = 0x101,
    /* A bitmap that represents the supported LTE bands */
    /* type = struct, refer to sys_lte_band */
    MIPC_NW_SET_BAND_MODE_REQ_T_LTE_BAND                    = 0x102,
    /* A bitmap that represents the supported NR bands */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_SET_BAND_MODE_REQ_T_NR_BAND                     = 0x103,
    /* A bitmap that represents the supported NR NSA bands; if NR_NSA_BAND doesn't not exist, modem will set both NR_SA_BAND and NR_NSA_BAND to NR_BAND. */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_SET_BAND_MODE_REQ_T_NR_NSA_BAND                 = 0x104,
};

    /* MIPC_MSG.NW_SET_BAND_MODE_CNF */
enum mipc_nw_set_band_mode_cnf_tlv_enum {
    mipc_nw_set_band_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CHANNEL_LOCK_REQ */
enum mipc_nw_set_channel_lock_req_tlv_enum {
    mipc_nw_set_channel_lock_req_tlv_NONE = 0,
    /* The list length for info list; Default value is 0. */
    /* type = uint16_t */
    MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST_COUNT  = 0x100,
    /* Lock channel information */
    /* type = struct_array, refer to nw_channel_lock_info */
    MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST        = 0x101,
    /* Lock channel information */
    /* type = struct, refer to nw_channel_lock_info_v1 */
    MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_LIST_V1     = 0x102,
    MIPC_NW_SET_CHANNEL_LOCK_REQ_T_CH_LOCK_INFO_TLV_ARRAY_V1 = 0x102,
};

    /* MIPC_MSG.NW_SET_CHANNEL_LOCK_CNF */
enum mipc_nw_set_channel_lock_cnf_tlv_enum {
    mipc_nw_set_channel_lock_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CHANNEL_LOCK_REQ */
enum mipc_nw_get_channel_lock_req_tlv_enum {
    mipc_nw_get_channel_lock_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CHANNEL_LOCK_CNF */
enum mipc_nw_get_channel_lock_cnf_tlv_enum {
    mipc_nw_get_channel_lock_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_ABORT_PLMN_LIST_SEARCH_REQ */
enum mipc_nw_set_abort_plmn_list_search_req_tlv_enum {
    mipc_nw_set_abort_plmn_list_search_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_ABORT_PLMN_LIST_SEARCH_CNF */
enum mipc_nw_set_abort_plmn_list_search_cnf_tlv_enum {
    mipc_nw_set_abort_plmn_list_search_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_POL_CAPABILITY_REQ */
enum mipc_nw_get_pol_capability_req_tlv_enum {
    mipc_nw_get_pol_capability_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_POL_CAPABILITY_CNF */
enum mipc_nw_get_pol_capability_cnf_tlv_enum {
    mipc_nw_get_pol_capability_cnf_tlv_NONE = 0,
    /* type = struct, refer to nw_pol_info */
    MIPC_NW_GET_POL_CAPABILITY_CNF_T_POL_INFO               = 0x100,
    /* type = struct, refer to nw_pol_info_v1 */
    MIPC_NW_GET_POL_CAPABILITY_CNF_T_POL_INFO_V1            = 0x101,
};

    /* MIPC_MSG.NW_SET_PREFER_RAT_REQ */
enum mipc_nw_set_prefer_rat_req_tlv_enum {
    mipc_nw_set_prefer_rat_req_tlv_NONE = 0,
    /* The length for RAT_LIST, the maximum length is 4; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_PREFER_RAT_REQ_T_RAT_NUM                    = 0x100,
    /* The RAT selection list with priority: rat_list[0] > rat_list[1] > rat_list[2] > rat_list[3] */
    /* type = byte_array, refer to NW_PREFER_RAT_TYPE */
    MIPC_NW_SET_PREFER_RAT_REQ_T_RAT_LIST                   = 0x101,
};

    /* MIPC_MSG.NW_SET_PREFER_RAT_CNF */
enum mipc_nw_set_prefer_rat_cnf_tlv_enum {
    mipc_nw_set_prefer_rat_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_OPERATOR_NAME_REQ */
enum mipc_nw_get_operator_name_req_tlv_enum {
    mipc_nw_get_operator_name_req_tlv_NONE = 0,
    /* PLMN ID, for example 46000 */
    /* type = string */
    MIPC_NW_GET_OPERATOR_NAME_REQ_T_PLMN_ID                 = 0x100,
};

    /* MIPC_MSG.NW_GET_OPERATOR_NAME_CNF */
enum mipc_nw_get_operator_name_cnf_tlv_enum {
    mipc_nw_get_operator_name_cnf_tlv_NONE = 0,
    /* The info for the given plmn id */
    /* type = struct, refer to nw_provider */
    MIPC_NW_GET_OPERATOR_NAME_CNF_T_PROVIDER_INFO           = 0x8100,
};

    /* MIPC_MSG.NW_SET_ENDC_CONFIG_REQ */
enum mipc_nw_set_endc_config_req_tlv_enum {
    mipc_nw_set_endc_config_req_tlv_NONE = 0,
    /* 1: enable endc; 0: disable endc. */
    /* type = uint8_t */
    MIPC_NW_SET_ENDC_CONFIG_REQ_T_ENDC_STATE                = 0x100,
};

    /* MIPC_MSG.NW_SET_ENDC_CONFIG_CNF */
enum mipc_nw_set_endc_config_cnf_tlv_enum {
    mipc_nw_set_endc_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ */
enum mipc_nw_set_lte_carrier_aggregation_switch_req_tlv_enum {
    mipc_nw_set_lte_carrier_aggregation_switch_req_tlv_NONE = 0,
    /* enable/disable lte carrier aggregation switch; Default value is 0. */
    /* type = uint8_t, refer to NW_LTE_CARRIER_ARRREGATION_SWITCH */
    MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ_T_STATUS = 0x100,
};

    /* MIPC_MSG.NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_CNF */
enum mipc_nw_set_lte_carrier_aggregation_switch_cnf_tlv_enum {
    mipc_nw_set_lte_carrier_aggregation_switch_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ */
enum mipc_nw_set_ps_cs_registration_state_roaming_type_req_tlv_enum {
    mipc_nw_set_ps_cs_registration_state_roaming_type_req_tlv_NONE = 0,
    /* type = struct, refer to nw_ps_cs_reg_roaming_info */
    MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ_T_PS_CS_REG_ROAMING_INFO = 0x100,
};

    /* MIPC_MSG.NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_CNF */
enum mipc_nw_set_ps_cs_registration_state_roaming_type_cnf_tlv_enum {
    mipc_nw_set_ps_cs_registration_state_roaming_type_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_REQ */
enum mipc_nw_get_lte_carrier_aggregation_switch_req_tlv_enum {
    mipc_nw_get_lte_carrier_aggregation_switch_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF */
enum mipc_nw_get_lte_carrier_aggregation_switch_cnf_tlv_enum {
    mipc_nw_get_lte_carrier_aggregation_switch_cnf_tlv_NONE = 0,
    /*  */
    /* type = uint8_t, refer to NW_LTE_CARRIER_ARRREGATION_SWITCH */
    MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF_T_STATUS = 0x100,
};

    /* MIPC_MSG.NW_GET_CURRENT_BAND_INFO_REQ */
enum mipc_nw_get_current_band_info_req_tlv_enum {
    mipc_nw_get_current_band_info_req_tlv_NONE = 0,
    /* Indicates which rat related information will be retrive. LTE: 1; NR: 2, just support this two rat currently */
    /* type = uint8_t */
    MIPC_NW_GET_CURRENT_BAND_INFO_REQ_T_RAT                 = 0x100,
};

    /* MIPC_MSG.NW_GET_CURRENT_BAND_INFO_CNF */
enum mipc_nw_get_current_band_info_cnf_tlv_enum {
    mipc_nw_get_current_band_info_cnf_tlv_NONE = 0,
    /* The band list count. */
    /* type = uint8_t */
    MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_LIST_COUNT     = 0x100,
    /* The band list for band, for example, band41 */
    /* type = uint8_t */
    MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_LIST           = 0x101,
    /* The band width list count. */
    /* type = uint8_t */
    MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_WIDTH_COUNT    = 0x102,
    /* The band width list for band(uint32 represent one bandwidth) */
    /* type = uint32_t */
    MIPC_NW_GET_CURRENT_BAND_INFO_CNF_T_BAND_WIDTH_LIST     = 0x103,
};

    /* MIPC_MSG.NW_CELL_MEASUREMENT_REQ */
enum mipc_nw_cell_measurement_req_tlv_enum {
    mipc_nw_cell_measurement_req_tlv_NONE = 0,
    /* 1: Start cell measurement; 0: Abort cell measurement; If the control point does not provide any action in TLV ACTION,or sends in an empty MIPC_NW_CELL_MEASUREMENT_REQ.modem aborts the cell measurement. */
    /* type = uint8_t */
    MIPC_NW_CELL_MEASUREMENT_REQ_T_ACTION                   = 0x100,
    /* A bitmap representing that the supported LTE bands are used for measurement. If TLV LTE_BAND is missing and TLV RAT specificed to LTE, modem measures all the LTE band info. */
    /* type = struct, refer to sys_lte_band */
    MIPC_NW_CELL_MEASUREMENT_REQ_T_LTE_BAND                 = 0x101,
    /* A bitmap representing that the supported NR bands are used for measurement. If TLV NR_BAND is missing and TLV RAT specificed to NR, modem measures all the NR band info. */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_CELL_MEASUREMENT_REQ_T_NR_BAND                  = 0x102,
    /* Scan type for cell measurement; Default value is 0. */
    /* type = uint8_t, refer to NW_CELLMEASUREMENT_SCAN_TYPE */
    MIPC_NW_CELL_MEASUREMENT_REQ_T_SCAN_TYPE                = 0x103,
    /*
      The RAT information in bit format; LTE: 0x0004(MIPC_NW_CELLMEASUREMENT_BIT_RAT_LTE); NR: 0x0080(MIPC_NW_CELLMEASUREMENT_BIT_RAT_NR); LTE+NR: 0x0084(MIPC_NW_CELLMEASUREMENT_BIT_RAT_NR | MIPC_NW_CELLMEASUREMENT_BIT_RAT_LTE).
      The control point can specify which RAT(s) to perform cell measurement by specifying TLV RAT.If TLV RAT is not provided, modem measures on all supported RAT(s).
    */
    /* type = uint16_t, refer to NW_CELLMEASUREMENT_BIT_RAT */
    MIPC_NW_CELL_MEASUREMENT_REQ_T_RAT                      = 0x104,
};

    /* MIPC_MSG.NW_CELL_MEASUREMENT_CNF */
enum mipc_nw_cell_measurement_cnf_tlv_enum {
    mipc_nw_cell_measurement_cnf_tlv_NONE = 0,
    /* The cell count for the cell measurement list */
    /* type = uint8_t */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_CELL_LIST_COUNT          = 0x100,
    /* The list of the cell measurement */
    /* type = struct_array, refer to nw_cellmeasurement_info */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_CELL_LIST                = 0x101,
    /* The PLMN information for all cells */
    /* type = struct_array, refer to cell_plmn */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_PLMN_ARRAY               = 0x102,
    /* The cell count for the LTE cell measurement list */
    /* type = uint8_t */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_LIST_COUNT      = 0x103,
    /* The list of the LTE cell measurement */
    /* type = struct_array, refer to nw_cellmeasurement_info */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_LIST            = 0x8104,
    /* The PLMN information for LTE cells */
    /* type = struct_array, refer to cell_plmn */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_PLMN_ARRAY           = 0x8105,
    /* The cell count for the NR cell measurement list */
    /* type = uint8_t */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_LIST_COUNT       = 0x106,
    /* The list of the NR cell measurement */
    /* type = struct_array, refer to nw_cellmeasurement_info */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_LIST             = 0x8107,
    /* The PLMN information for NR cells */
    /* type = struct_array, refer to cell_plmn */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_PLMN_ARRAY            = 0x8108,
    /* The BAND information for LTE cells; the type of this field is a list, and the list count equals LTE_CELL_LIST_COUNT; the band info of each field in LTE_CELL_BAND has a one-to-one correspondence with the cell in LTE_CELL_LIST; note: Only cell_band is valid in nw_cell_band_bandwidth */
    /* type = struct_array, refer to nw_cell_band_bandwidth */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_BAND_LIST       = 0x8109,
    /* The BAND information for NR cells; the type of this field is a list, and the list count equals NR_CELL_LIST_COUNT; the band info of each field in NR_CELL_BAND has a one-to-one correspondence with the cell in NR_CELL_LIST; note: Only cell_band is valid in nw_cell_band_bandwidth */
    /* type = struct_array, refer to nw_cell_band_bandwidth */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_BAND_LIST        = 0x810A,
    /* [Phase out] Please use LTE_CELL_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_cellmeasurement_info */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_TLV_ARRAY       = 0x810B,
    /* The PLMN information for LTE cells */
    /* type = TLV, refer to cell_plmn_tlv */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_PLMN_TLV_ARRAY       = 0x810C,
    /* [Phase out] Please use NR_CELL_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_cellmeasurement_info */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_TLV_ARRAY        = 0x810D,
    /* The PLMN information for NR cells */
    /* type = TLV, refer to cell_plmn_tlv */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_PLMN_TLV_ARRAY        = 0x810E,
    /* [Phase out] Please use LTE_CELL_BAND_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_cell_band_bandwidth */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_BAND_TLV_ARRAY  = 0x810F,
    /* [Phase out] Please use NR_CELL_BAND_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_cell_band_bandwidth */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_BAND_TLV_ARRAY   = 0x8110,
    /* The list of the LTE cell measurement */
    /* type = struct, refer to nw_cellmeasurement_info_v1 */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_V1_TLV_ARRAY    = 0x8111,
    /* The list of the NR cell measurement */
    /* type = struct, refer to nw_cellmeasurement_info_v1 */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_V1_TLV_ARRAY     = 0x8112,
    /* The BAND information for LTE cells; the type of this field is a list, and the list count equals LTE_CELL_LIST_COUNT; the band info of each field in LTE_CELL_BAND has a one-to-one correspondence with the cell in LTE_CELL_LIST; note: Only cell_band is valid in nw_cell_band_bandwidth */
    /* type = struct, refer to nw_cell_band_bandwidth_v1 */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_LTE_CELL_BAND_V1_TLV_ARRAY = 0x8113,
    /* The BAND information for NR cells; the type of this field is a list, and the list count equals NR_CELL_LIST_COUNT; the band info of each field in NR_CELL_BAND has a one-to-one correspondence with the cell in NR_CELL_LIST; note: Only cell_band is valid in nw_cell_band_bandwidth */
    /* type = struct, refer to nw_cell_band_bandwidth_v1 */
    MIPC_NW_CELL_MEASUREMENT_CNF_T_NR_CELL_BAND_V1_TLV_ARRAY = 0x8114,
};

    /* MIPC_MSG.NW_CELL_BAND_WHITE_LIST_LOCK_REQ */
enum mipc_nw_cell_band_white_list_lock_req_tlv_enum {
    mipc_nw_cell_band_white_list_lock_req_tlv_NONE = 0,
    /* A bitmap representing the LTE bands used for measurement */
    /* type = struct, refer to sys_lte_band */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_BAND        = 0x100,
    /* A bitmap representing the NR bands used for measurement */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_BAND         = 0x101,
    /* The count of LTE cell in LTE cell white list */
    /* type = uint8_t */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_CELL_COUNT  = 0x102,
    /* LTE cell white list, just fill in the earfcn and physical_cell_id of nw_lte_cell */
    /* type = struct_array, refer to nw_lte_cell */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_CELL_LIST   = 0x103,
    /* The count of NR cell in NR cell white list */
    /* type = uint8_t */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_CELL_COUNT   = 0x104,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY instead of NR_CELL_COUNT and NR_CELL_LIST. */
    /* type = struct_array, refer to nw_nr_cell */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_CELL_LIST    = 0x105,
    /* LTE cell white list, just fill in the earfcn and physical_cell_id of nw_lte_cell */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_LTE_CELL_TLV_ARRAY = 0x107,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_CELL_TLV_ARRAY = 0x108,
    /* NR cell white list, just fill in the nr_arfcn and physical_cell_id of nw_nr_cell_v2 */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ_T_NR_CELL_V2_TLV_ARRAY = 0x109,
};

    /* MIPC_MSG.NW_CELL_BAND_WHITE_LIST_LOCK_CNF */
enum mipc_nw_cell_band_white_list_lock_cnf_tlv_enum {
    mipc_nw_cell_band_white_list_lock_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CELL_BAND_BANDWIDTH_REQ */
enum mipc_nw_get_cell_band_bandwidth_req_tlv_enum {
    mipc_nw_get_cell_band_bandwidth_req_tlv_NONE = 0,
    /*  This field currently supports both RAT LTE and NR */
    /* type = uint8_t */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_REQ_T_RAT               = 0x100,
};

    /* MIPC_MSG.NW_GET_CELL_BAND_BANDWIDTH_CNF */
enum mipc_nw_get_cell_band_bandwidth_cnf_tlv_enum {
    mipc_nw_get_cell_band_bandwidth_cnf_tlv_NONE = 0,
    /* This parameter will be removed, the value is invalid */
    /* type = uint8_t */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NUM_SERVING_CELL  = 0x100,
    /* This parameter will be removed, the value is invalid */
    /* type = struct_array, refer to nw_cell_band_bandwidth */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_SERVING_CELL_LIST = 0x8101,
    /* This parameter will be removed, the value is invalid */
    /* type = struct_array, refer to nw_cell_band_bandwidth */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_SERVING_CELL_LIST = 0x8102,
    /* Number of DL LTE serving cell, maximum is 32 */
    /* type = uint8_t */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_DL_SERVING_CELL_COUNT = 0x103,
    /* Number of UL LTE serving cell, maximum is 32 */
    /* type = uint8_t */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_UL_SERVING_CELL_COUNT = 0x104,
    /* Number of DL NR serving cell, maximum is 32 */
    /* type = uint8_t */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_DL_SERVING_CELL_COUNT = 0x105,
    /* Number of UL NR serving cell, maximum is 32 */
    /* type = uint8_t */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_UL_SERVING_CELL_COUNT = 0x106,
    /* DL LTE serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_DL_SERVING_CELL_LIST = 0x107,
    /* UL LTE serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_UL_SERVING_CELL_LIST = 0x108,
    /* DL NR serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_DL_SERVING_CELL_LIST = 0x109,
    /* UL NR serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_UL_SERVING_CELL_LIST = 0x10A,
    /* DL LTE serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_DL_SERVING_CELL_TLV_ARRAY = 0x10B,
    /* UL LTE serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_LTE_UL_SERVING_CELL_TLV_ARRAY = 0x10C,
    /* DL NR serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_DL_SERVING_CELL_TLV_ARRAY = 0x10D,
    /* UL NR serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_NR_UL_SERVING_CELL_TLV_ARRAY = 0x10E,
    /* UMTS cell frequency information */
    /* type = struct, refer to nw_umts_cell_frequency_info */
    MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF_T_UMTS_CELL_FREQUENCY_INFO = 0x10F,
};

    /* MIPC_MSG.NW_GET_NR_REQ */
enum mipc_nw_get_nr_req_tlv_enum {
    mipc_nw_get_nr_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NR_CNF */
enum mipc_nw_get_nr_cnf_tlv_enum {
    mipc_nw_get_nr_cnf_tlv_NONE = 0,
    /* 1: LTE ONLY(disable NSA+SA); 3: SA ONLY; 5: NSA ONLY; 7: NSA+SA. */
    /* type = uint8_t */
    MIPC_NW_GET_NR_CNF_T_NR_OPT                             = 0x100,
};

    /* MIPC_MSG.NW_GET_SRXLEV_REQ */
enum mipc_nw_get_srxlev_req_tlv_enum {
    mipc_nw_get_srxlev_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_SRXLEV_CNF */
enum mipc_nw_get_srxlev_cnf_tlv_enum {
    mipc_nw_get_srxlev_cnf_tlv_NONE = 0,
    /* LTE SRXLEV INFO. */
    /* type = struct, refer to nw_srxlev_info */
    MIPC_NW_GET_SRXLEV_CNF_T_LTE_SRXLEV_INFO                = 0x100,
    /* NR SRXLEV INFO. */
    /* type = struct, refer to nw_srxlev_info */
    MIPC_NW_GET_SRXLEV_CNF_T_NR_SRXLEV_INFO                 = 0x101,
};

    /* MIPC_MSG.NW_SET_ROAMING_MODE_REQ */
enum mipc_nw_set_roaming_mode_req_tlv_enum {
    mipc_nw_set_roaming_mode_req_tlv_NONE = 0,
    /* It indicates whether the roaming configuration are disable; each bit represents one configuration; Default value is 0. */
    /* type = bitmap, refer to NW_ROAMING_MODE */
    MIPC_NW_SET_ROAMING_MODE_REQ_T_ROAMING_MODE             = 0x100,
};

    /* MIPC_MSG.NW_SET_ROAMING_MODE_CNF */
enum mipc_nw_set_roaming_mode_cnf_tlv_enum {
    mipc_nw_set_roaming_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ROAMING_MODE_REQ */
enum mipc_nw_get_roaming_mode_req_tlv_enum {
    mipc_nw_get_roaming_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ROAMING_MODE_CNF */
enum mipc_nw_get_roaming_mode_cnf_tlv_enum {
    mipc_nw_get_roaming_mode_cnf_tlv_NONE = 0,
    /* It indicates whether the roaming configuration are disable; each bit represents one configuration */
    /* type = bitmap, refer to NW_ROAMING_MODE */
    MIPC_NW_GET_ROAMING_MODE_CNF_T_ROAMING_MODE             = 0x100,
};

    /* MIPC_MSG.NW_SET_URC_ENABLE_REQ */
enum mipc_nw_set_urc_enable_req_tlv_enum {
    mipc_nw_set_urc_enable_req_tlv_NONE = 0,
    /*  The IND type to be set; Default value is 0. */
    /* type = uint8_t, refer to NW_IND_TYPE */
    MIPC_NW_SET_URC_ENABLE_REQ_T_TYPE                       = 0x100,
    /*  1: Enable; 0: Disable the given IND; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_URC_ENABLE_REQ_T_ENABLE                     = 0x101,
};

    /* MIPC_MSG.NW_SET_URC_ENABLE_CNF */
enum mipc_nw_set_urc_enable_cnf_tlv_enum {
    mipc_nw_set_urc_enable_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_SUGGESTED_PLMN_LIST_REQ */
enum mipc_nw_get_suggested_plmn_list_req_tlv_enum {
    mipc_nw_get_suggested_plmn_list_req_tlv_NONE = 0,
    /* 0:GMSS, 1:UMTS, 3:LTE; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_RAT               = 0x100,
    /* number of PLMN that needs to be listed; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_NUM               = 0x101,
    /* guard timer for PLMN list procedure; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ_T_TIMER             = 0x102,
};

    /* MIPC_MSG.NW_GET_SUGGESTED_PLMN_LIST_CNF */
enum mipc_nw_get_suggested_plmn_list_cnf_tlv_enum {
    mipc_nw_get_suggested_plmn_list_cnf_tlv_NONE = 0,
    /* the number of suggested plmn list. */
    /* type = uint8_t */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_COUNT             = 0x100,
    /* suggested PLMN list */
    /* type = struct_array, refer to nw_suggested */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_LIST              = 0x101,
    /* The fail cause for current operation. */
    /* type = uint16_t */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_FAIL_CAUSE        = 0x102,
    /* [Phase out] Please use PLMN_TLV_V1_ARRAY. */
    /* type = struct, refer to nw_suggested */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_PLMN_TLV_ARRAY    = 0x103,
    /* TLV array of suggested PLMN */
    /* type = struct, refer to nw_suggested_v1 */
    MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF_T_PLMN_V1_TLV_ARRAY = 0x104,
};

    /* MIPC_MSG.NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ */
enum mipc_nw_set_search_store_frequency_info_req_tlv_enum {
    mipc_nw_set_search_store_frequency_info_req_tlv_NONE = 0,
    /* 0: Clear all settings of this command; 1: Add new ARFCN to PLMN of the specified RAT for searching; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_OPER      = 0x100,
    /* Only support LTE(4) and NR(128); Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_RAT       = 0x101,
    /* PLMN ID; Default value is null character. */
    /* type = string */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_PLMN      = 0x102,
    /* Number of ARFCN to be added, the current maximum number is 10 */
    /* type = uint8_t */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_COUNT     = 0x103,
    /* ARFCN; the maximum count of this field is 10 */
    /* type = struct_array, refer to nw_arfcn */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_ARFCN     = 0x104,
    /* ARFCN; the maximum count of this field is 10 */
    /* type = struct, refer to nw_arfcn */
    MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ_T_ARFCN_TLV_ARRAY = 0x105,
};

    /* MIPC_MSG.NW_SET_SEARCH_STORE_FREQUENCY_INFO_CNF */
enum mipc_nw_set_search_store_frequency_info_cnf_tlv_enum {
    mipc_nw_set_search_store_frequency_info_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_SELECT_FEMTOCELL_REQ */
enum mipc_nw_set_select_femtocell_req_tlv_enum {
    mipc_nw_set_select_femtocell_req_tlv_NONE = 0,
    /* PLMN ID; Default value is null character. */
    /* type = string */
    MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_PLMN                 = 0x100,
    /* 0: GSM, 2: UMTS, 7: LTE; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_ACT                  = 0x101,
    /* CSG ID; Default value is null character. */
    /* type = string */
    MIPC_NW_SET_SELECT_FEMTOCELL_REQ_T_CSG_ID               = 0x102,
};

    /* MIPC_MSG.NW_SET_SELECT_FEMTOCELL_CNF */
enum mipc_nw_set_select_femtocell_cnf_tlv_enum {
    mipc_nw_set_select_femtocell_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CONFIG_A2_OFFSET_REQ */
enum mipc_nw_set_config_a2_offset_req_tlv_enum {
    mipc_nw_set_config_a2_offset_req_tlv_NONE = 0,
    /* offset; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CONFIG_A2_OFFSET_REQ_T_OFFSET               = 0x100,
    /* thresh_bound; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CONFIG_A2_OFFSET_REQ_T_THRESH_BOUND         = 0x101,
};

    /* MIPC_MSG.NW_SET_CONFIG_A2_OFFSET_CNF */
enum mipc_nw_set_config_a2_offset_cnf_tlv_enum {
    mipc_nw_set_config_a2_offset_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CONFIG_B1_OFFSET_REQ */
enum mipc_nw_set_config_b1_offset_req_tlv_enum {
    mipc_nw_set_config_b1_offset_req_tlv_NONE = 0,
    /* offset; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CONFIG_B1_OFFSET_REQ_T_OFFSET               = 0x100,
    /* thresh_bound; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CONFIG_B1_OFFSET_REQ_T_THRESH_BOUND         = 0x101,
};

    /* MIPC_MSG.NW_SET_CONFIG_B1_OFFSET_CNF */
enum mipc_nw_set_config_b1_offset_cnf_tlv_enum {
    mipc_nw_set_config_b1_offset_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_REPORT_ANBR_REQ */
enum mipc_nw_set_report_anbr_req_tlv_enum {
    mipc_nw_set_report_anbr_req_tlv_NONE = 0,
    /* 0:disable ENABR URC, 1:enable EANBR URC, 2:send phone`s ANBR valuse to NW; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_MODE                      = 0x100,
    /* ebi value,5-15; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_EBI                       = 0x101,
    /* 0:downlink, 1:uplink; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_IS_UL                     = 0x102,
    /* valid rang 1-63; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_BEARE_ID                  = 0x104,
    /* Bitrate value 0-8000, 0xffff means invalid value; Default value is 0. */
    /* type = uint16_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_BITRATE                   = 0x103,
    /* valid rang 1-15; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_PDU_SESSION_ID            = 0x105,
    /* ext_param; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_REPORT_ANBR_REQ_T_EXT_PARAM                 = 0x106,
};

    /* MIPC_MSG.NW_SET_REPORT_ANBR_CNF */
enum mipc_nw_set_report_anbr_cnf_tlv_enum {
    mipc_nw_set_report_anbr_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_NETWORK_EVENT_REQ */
enum mipc_nw_set_network_event_req_tlv_enum {
    mipc_nw_set_network_event_req_tlv_NONE = 0,
    /* 0: Disable EREGINFO URC; 1: Enable EREGINFO URC */
    /* type = uint8_t */
    MIPC_NW_SET_NETWORK_EVENT_REQ_T_MODE                    = 0x100,
};

    /* MIPC_MSG.NW_SET_NETWORK_EVENT_CNF */
enum mipc_nw_set_network_event_cnf_tlv_enum {
    mipc_nw_set_network_event_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_ENABLE_CA_PLUS_FILTER_REQ */
enum mipc_nw_set_enable_ca_plus_filter_req_tlv_enum {
    mipc_nw_set_enable_ca_plus_filter_req_tlv_NONE = 0,
    /* 0:disable, 1:enable. */
    /* type = uint8_t */
    MIPC_NW_SET_ENABLE_CA_PLUS_FILTER_REQ_T_ENABLE          = 0x100,
};

    /* MIPC_MSG.NW_SET_ENABLE_CA_PLUS_FILTER_CNF */
enum mipc_nw_set_enable_ca_plus_filter_cnf_tlv_enum {
    mipc_nw_set_enable_ca_plus_filter_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_ABORT_FEMTOCELL_LIST_REQ */
enum mipc_nw_abort_femtocell_list_req_tlv_enum {
    mipc_nw_abort_femtocell_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_ABORT_FEMTOCELL_LIST_CNF */
enum mipc_nw_abort_femtocell_list_cnf_tlv_enum {
    mipc_nw_abort_femtocell_list_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_FEMTOCELL_LIST_REQ */
enum mipc_nw_get_femtocell_list_req_tlv_enum {
    mipc_nw_get_femtocell_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_FEMTOCELL_LIST_CNF */
enum mipc_nw_get_femtocell_list_cnf_tlv_enum {
    mipc_nw_get_femtocell_list_cnf_tlv_NONE = 0,
    /* Count of LTE femtocell list */
    /* type = uint8_t */
    MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_CELL_COUNT             = 0x100,
    /* The list of FEMTOCELL */
    /* type = struct_array, refer to nw_femtocell_info */
    MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_LIST         = 0x101,
    /* The cause of failure for current operation */
    /* type = uint16_t */
    MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FAIL_CAUSE             = 0x102,
    /* [Phase out] Please use FEMTOCELL_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_femtocell_info */
    MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_TLV_ARRAY    = 0x103,
    /* The list of FEMTOCELL */
    /* type = struct, refer to nw_femtocell_info_v1 */
    MIPC_NW_GET_FEMTOCELL_LIST_CNF_T_FEMTOCELL_V1_TLV_ARRAY = 0x104,
};

    /* MIPC_MSG.NW_SET_PSEUDO_CELL_MODE_REQ */
enum mipc_nw_set_pseudo_cell_mode_req_tlv_enum {
    mipc_nw_set_pseudo_cell_mode_req_tlv_NONE = 0,
    /* the mode of the apc; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_APC_MODE             = 0x100,
    /* urc_enable value; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_URC_ENABLE           = 0x101,
    /* timer value; Default value is 0. */
    /* type = uint16_t */
    MIPC_NW_SET_PSEUDO_CELL_MODE_REQ_T_TIMER                = 0x102,
};

    /* MIPC_MSG.NW_SET_PSEUDO_CELL_MODE_CNF */
enum mipc_nw_set_pseudo_cell_mode_cnf_tlv_enum {
    mipc_nw_set_pseudo_cell_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PSEUDO_CELL_INFO_REQ */
enum mipc_nw_get_pseudo_cell_info_req_tlv_enum {
    mipc_nw_get_pseudo_cell_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PSEUDO_CELL_INFO_CNF */
enum mipc_nw_get_pseudo_cell_info_cnf_tlv_enum {
    mipc_nw_get_pseudo_cell_info_cnf_tlv_NONE = 0,
    /* The mode of the pseudo cell apc */
    /* type = uint8_t */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_APC_MODE             = 0x100,
    /* the enable state of the URC  */
    /* type = uint8_t */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_URC_ENABLE           = 0x101,
    /* the interval of the periods */
    /* type = uint16_t */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_TIMER                = 0x102,
    /* the count of the pseudocell list */
    /* type = uint8_t */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_CELL_COUNT           = 0x103,
    /* The list of FEMTOCELL. */
    /* type = struct_array, refer to nw_pseudocell_info */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_LIST      = 0x104,
    /* The fail cause for current operation . */
    /* type = uint16_t */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_FAIL_CAUSE           = 0x105,
    /* [Phase out] Please use PSEUDOCELL_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_pseudocell_info */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_TLV_ARRAY = 0x106,
    /* The TLV array of FEMTOCELL. */
    /* type = struct, refer to nw_pseudocell_info_v1 */
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF_T_PSEUDOCELL_V1_TLV_ARRAY = 0x107,
};

    /* MIPC_MSG.NW_SET_ROAMING_ENABLE_REQ */
enum mipc_nw_set_roaming_enable_req_tlv_enum {
    mipc_nw_set_roaming_enable_req_tlv_NONE = 0,
    /* The protocol index of the roaming enable; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_ROAMING_ENABLE_REQ_T_PROTOCOL_INDEX         = 0x100,
    /* Bar dom voice roaming enabled; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_ROAMING_ENABLE_REQ_T_DOM_VOICE              = 0x101,
    /* Bar dom data roaming enabled; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_ROAMING_ENABLE_REQ_T_DOM_DATA               = 0x102,
    /* Bar int voice roaming enabled; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_ROAMING_ENABLE_REQ_T_INT_VOICE              = 0x103,
    /* Bar int data roaming enabled; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_ROAMING_ENABLE_REQ_T_INT_DATA               = 0x104,
    /* Bar LTE data roaming enabled; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_ROAMING_ENABLE_REQ_T_LTE_DATA               = 0x105,
};

    /* MIPC_MSG.NW_SET_ROAMING_ENABLE_CNF */
enum mipc_nw_set_roaming_enable_cnf_tlv_enum {
    mipc_nw_set_roaming_enable_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ROAMING_ENABLE_REQ */
enum mipc_nw_get_roaming_enable_req_tlv_enum {
    mipc_nw_get_roaming_enable_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ROAMING_ENABLE_CNF */
enum mipc_nw_get_roaming_enable_cnf_tlv_enum {
    mipc_nw_get_roaming_enable_cnf_tlv_NONE = 0,
    /* The protocol index of the roaming enable. */
    /* type = uint8_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_PROTOCOL_INDEX         = 0x100,
    /* Bar dom voice roaming enabled. */
    /* type = uint8_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_DOM_VOICE              = 0x101,
    /* Bar dom data roaming enabled. */
    /* type = uint8_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_DOM_DATA               = 0x102,
    /* Bar int voice roaming enabled. */
    /* type = uint8_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_INT_VOICE              = 0x103,
    /* Bar int data roaming enabled. */
    /* type = uint8_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_INT_DATA               = 0x104,
    /* Bar LTE data roaming enabled. */
    /* type = uint8_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_LTE_DATA               = 0x105,
    /* The fail cause for current operation . */
    /* type = uint16_t */
    MIPC_NW_GET_ROAMING_ENABLE_CNF_T_FAIL_CAUSE             = 0x106,
};

    /* MIPC_MSG.NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ */
enum mipc_nw_set_femtocell_system_selection_mode_req_tlv_enum {
    mipc_nw_set_femtocell_system_selection_mode_req_tlv_NONE = 0,
    /* The mode of the femtocell system selection; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ_T_MODE  = 0x100,
};

    /* MIPC_MSG.NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF */
enum mipc_nw_set_femtocell_system_selection_mode_cnf_tlv_enum {
    mipc_nw_set_femtocell_system_selection_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ */
enum mipc_nw_query_femtocell_system_selection_mode_req_tlv_enum {
    mipc_nw_query_femtocell_system_selection_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF */
enum mipc_nw_query_femtocell_system_selection_mode_cnf_tlv_enum {
    mipc_nw_query_femtocell_system_selection_mode_cnf_tlv_NONE = 0,
    /* The mode of the femtocell system selection */
    /* type = uint8_t */
    MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF_T_MODE = 0x100,
    /* The cause of failure for current operation */
    /* type = uint16_t */
    MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF_T_FAIL_CAUSE = 0x101,
};

    /* MIPC_MSG.NW_SET_NW_IND_REPORT_LEVEL_REQ */
enum mipc_nw_set_nw_ind_report_level_req_tlv_enum {
    mipc_nw_set_nw_ind_report_level_req_tlv_NONE = 0,
    /*
      It indicates the PS state reporting level;
      0: No change PS state reporting level
      3: Enable MIPC_NW_PS_IND when any information has changed;
      5: Enable MIPC_NW_PS_IND when the network registration status or radio technology has changed
      Default value is 0
    */
    /* type = uint8_t */
    MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_PS_STATE_LEVEL    = 0x100,
    /* It indicates the CS state reporting level; 3: Enable EREG URC with full information; 5: Enable EREG URC when <stat> or <eAct> changes; 0: Do not change CS state reporting level; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ_T_CS_STATE_LEVEL    = 0x101,
};

    /* MIPC_MSG.NW_SET_NW_IND_REPORT_LEVEL_CNF */
enum mipc_nw_set_nw_ind_report_level_cnf_tlv_enum {
    mipc_nw_set_nw_ind_report_level_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_DISABLE_2G_REQ */
enum mipc_nw_set_disable_2g_req_tlv_enum {
    mipc_nw_set_disable_2g_req_tlv_NONE = 0,
    /* Indicates which rat is disable: 0 => no RAT are disable; 1 => disable 2G; Default value is 0xFF. */
    /* type = uint8_t */
    MIPC_NW_SET_DISABLE_2G_REQ_T_MODE                       = 0x100,
};

    /* MIPC_MSG.NW_SET_DISABLE_2G_CNF */
enum mipc_nw_set_disable_2g_cnf_tlv_enum {
    mipc_nw_set_disable_2g_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_DISABLE_2G_REQ */
enum mipc_nw_get_disable_2g_req_tlv_enum {
    mipc_nw_get_disable_2g_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_DISABLE_2G_CNF */
enum mipc_nw_get_disable_2g_cnf_tlv_enum {
    mipc_nw_get_disable_2g_cnf_tlv_NONE = 0,
    /* Indicates which rat is disable: 0 => no RAT are disable; 1 => disable 2G. */
    /* type = uint8_t */
    MIPC_NW_GET_DISABLE_2G_CNF_T_MODE                       = 0x100,
};

    /* MIPC_MSG.NW_SET_SMART_RAT_SWITCH_REQ */
enum mipc_nw_set_smart_rat_switch_req_tlv_enum {
    mipc_nw_set_smart_rat_switch_req_tlv_NONE = 0,
    /* The expected mode for switch control; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_RAT_SWITCH_MODE */
    MIPC_NW_SET_SMART_RAT_SWITCH_REQ_T_MODE                 = 0x100,
    /* Indicates which RAT will be used, just support two RAT for this REQ: 0: 4G; 1: 5G; Default value is 0xFF. */
    /* type = uint8_t */
    MIPC_NW_SET_SMART_RAT_SWITCH_REQ_T_RAT                  = 0x101,
};

    /* MIPC_MSG.NW_SET_SMART_RAT_SWITCH_CNF */
enum mipc_nw_set_smart_rat_switch_cnf_tlv_enum {
    mipc_nw_set_smart_rat_switch_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_SMART_RAT_SWITCH_REQ */
enum mipc_nw_get_smart_rat_switch_req_tlv_enum {
    mipc_nw_get_smart_rat_switch_req_tlv_NONE = 0,
    /* The mode for switch control; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_RAT_SWITCH_MODE */
    MIPC_NW_GET_SMART_RAT_SWITCH_REQ_T_MODE                 = 0x100,
};

    /* MIPC_MSG.NW_GET_SMART_RAT_SWITCH_CNF */
enum mipc_nw_get_smart_rat_switch_cnf_tlv_enum {
    mipc_nw_get_smart_rat_switch_cnf_tlv_NONE = 0,
    /* RAT switch state. */
    /* type = uint8_t */
    MIPC_NW_GET_SMART_RAT_SWITCH_CNF_T_STATE                = 0x100,
};

    /* MIPC_MSG.NW_VSS_ANTENNA_CONF_REQ */
enum mipc_nw_vss_antenna_conf_req_tlv_enum {
    mipc_nw_vss_antenna_conf_req_tlv_NONE = 0,
    /* 0: signal information is not available on all Rx chains.  1: Rx diversity bitmask for chain 0.  2: Rx diversity bitmask for chain 1 is available.  3: Signal information on both Rx chains is available. */
    /* type = uint8_t */
    MIPC_NW_VSS_ANTENNA_CONF_REQ_T_ANTENNA_TYPE             = 0x100,
};

    /* MIPC_MSG.NW_VSS_ANTENNA_CONF_CNF */
enum mipc_nw_vss_antenna_conf_cnf_tlv_enum {
    mipc_nw_vss_antenna_conf_cnf_tlv_NONE = 0,
    /* 0: signal information is not available on all Rx chains.  1: Rx diversity bitmask for chain 0.  2: Rx diversity bitmask for chain 1 is available.  3: Signal information on both Rx chains is available. */
    /* type = uint8_t */
    MIPC_NW_VSS_ANTENNA_CONF_CNF_T_ANTENNA_TYPE             = 0x100,
};

    /* MIPC_MSG.NW_VSS_ANTENNA_INFO_REQ */
enum mipc_nw_vss_antenna_info_req_tlv_enum {
    mipc_nw_vss_antenna_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_VSS_ANTENNA_INFO_CNF */
enum mipc_nw_vss_antenna_info_cnf_tlv_enum {
    mipc_nw_vss_antenna_info_cnf_tlv_NONE = 0,
    /* primary antenna RSSI */
    /* type = int32_t */
    MIPC_NW_VSS_ANTENNA_INFO_CNF_T_PRIMARY_ANTENNA_RSSI     = 0x100,
    /* relative phase */
    /* type = int32_t */
    MIPC_NW_VSS_ANTENNA_INFO_CNF_T_RELATIVE_PHASE           = 0x101,
    /* secondary antenna RSSI */
    /* type = int32_t */
    MIPC_NW_VSS_ANTENNA_INFO_CNF_T_SECONDARY_ANTENNA_RSSI   = 0x102,
    /* phase1 */
    /* type = int32_t */
    MIPC_NW_VSS_ANTENNA_INFO_CNF_T_PHASE1                   = 0x103,
    /* rx0 state, 0: not valid, 1: valid */
    /* type = int32_t */
    MIPC_NW_VSS_ANTENNA_INFO_CNF_T_RX_STATE_0               = 0x104,
    /* rx1 state, 0: not valid, 1: valid */
    /* type = int32_t */
    MIPC_NW_VSS_ANTENNA_INFO_CNF_T_RX_STATE_1               = 0x105,
};

    /* MIPC_MSG.NW_SET_RADIO_CAPABILITY_REQ */
enum mipc_nw_set_radio_capability_req_tlv_enum {
    mipc_nw_set_radio_capability_req_tlv_NONE = 0,
    /* bit0: GSM, bit1: UMTS, bit2: LTE, bit3: NR, bit4: C2K; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_SET_RADIO_CAPABILITY_REQ_T_RADIO_CAPABILITY     = 0x100,
    /* 0: no need do mode switch according to data change when there are one more C cards, 1: need do mode switch according to data change when there are one more C cards; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_RADIO_CAPABILITY_REQ_T_SWITCH_MODE_BY_DATA  = 0x101,
};

    /* MIPC_MSG.NW_SET_RADIO_CAPABILITY_CNF */
enum mipc_nw_set_radio_capability_cnf_tlv_enum {
    mipc_nw_set_radio_capability_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CDMA_ROAMING_PREFERENCE_REQ */
enum mipc_nw_set_cdma_roaming_preference_req_tlv_enum {
    mipc_nw_set_cdma_roaming_preference_req_tlv_NONE = 0,
    /* 0: for Home Networks only, 2: for Roaming on Any Network; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_REQ_T_ROAMING_TYPE  = 0x100,
};

    /* MIPC_MSG.NW_SET_CDMA_ROAMING_PREFERENCE_CNF */
enum mipc_nw_set_cdma_roaming_preference_cnf_tlv_enum {
    mipc_nw_set_cdma_roaming_preference_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CDMA_ROAMING_PREFERENCE_REQ */
enum mipc_nw_get_cdma_roaming_preference_req_tlv_enum {
    mipc_nw_get_cdma_roaming_preference_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CDMA_ROAMING_PREFERENCE_CNF */
enum mipc_nw_get_cdma_roaming_preference_cnf_tlv_enum {
    mipc_nw_get_cdma_roaming_preference_cnf_tlv_NONE = 0,
    /* 0: for Home Networks only, 2: for Roaming on Any Network. */
    /* type = uint8_t */
    MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_CNF_T_ROAMING_TYPE  = 0x100,
};

    /* MIPC_MSG.NW_GET_BARRING_INFO_REQ */
enum mipc_nw_get_barring_info_req_tlv_enum {
    mipc_nw_get_barring_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_BARRING_INFO_CNF */
enum mipc_nw_get_barring_info_cnf_tlv_enum {
    mipc_nw_get_barring_info_cnf_tlv_NONE = 0,
    /* 0: disable unsolicited result code , 1: enable unsolicited result code. */
    /* type = uint8_t */
    MIPC_NW_GET_BARRING_INFO_CNF_T_MODE                     = 0x100,
    /* number of current camping on network barring. */
    /* type = uint8_t */
    MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT                    = 0x101,
    /* [Phase out] Please use BARRING_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST             = 0x102,
    /* current camping on network rat 2:UMTS 4:LTE 128:NR. */
    /* type = uint8_t */
    MIPC_NW_GET_BARRING_INFO_CNF_T_RAT                      = 0x103,
    /* number of UMTS barring. */
    /* type = uint8_t */
    MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT_UMTS               = 0x104,
    /* [Phase out] Please use UMTS_BARRING_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST_UMTS        = 0x105,
    /* number of LTE barring. */
    /* type = uint8_t */
    MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT_LTE                = 0x106,
    /* [Phase out] Please use LTE_BARRING_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST_LTE         = 0x107,
    /* number of NR barring. */
    /* type = uint8_t */
    MIPC_NW_GET_BARRING_INFO_CNF_T_COUNT_NR                 = 0x108,
    /* [Phase out] Please use NR_BARRING_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_LIST_NR          = 0x109,
    /* [Phase out] Please use BARRING_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_TLV_ARRAY        = 0x10A,
    /* [Phase out] Please use UMTS_BARRING_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_UMTS_BARRING_TLV_ARRAY   = 0x10B,
    /* [Phase out] Please use LTE_BARRING_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_LTE_BARRING_TLV_ARRAY    = 0x10C,
    /* [Phase out] Please use NR_BARRING_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_barring_info */
    MIPC_NW_GET_BARRING_INFO_CNF_T_NR_BARRING_TLV_ARRAY     = 0x10D,
    /* The current camping on network barring info LIST of the barring service. */
    /* type = struct, refer to nw_barring_info_v1 */
    MIPC_NW_GET_BARRING_INFO_CNF_T_BARRING_V1_TLV_ARRAY     = 0x10E,
    /* The UMTS barring info LIST of the barring service. */
    /* type = struct, refer to nw_barring_info_v1 */
    MIPC_NW_GET_BARRING_INFO_CNF_T_UMTS_BARRING_V1_TLV_ARRAY = 0x10F,
    /* The LTE barring info LIST of the barring service. */
    /* type = struct, refer to nw_barring_info_v1 */
    MIPC_NW_GET_BARRING_INFO_CNF_T_LTE_BARRING_V1_TLV_ARRAY = 0x110,
    /* The NR barring info LIST of the barring service. */
    /* type = struct, refer to nw_barring_info_v1 */
    MIPC_NW_GET_BARRING_INFO_CNF_T_NR_BARRING_V1_TLV_ARRAY  = 0x111,
};

    /* MIPC_MSG.NW_GET_EHRPD_INFO_REQ */
enum mipc_nw_get_ehrpd_info_req_tlv_enum {
    mipc_nw_get_ehrpd_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_EHRPD_INFO_CNF */
enum mipc_nw_get_ehrpd_info_cnf_tlv_enum {
    mipc_nw_get_ehrpd_info_cnf_tlv_NONE = 0,
    /* protocol revision in use, 0-10. */
    /* type = uint8_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_REV                        = 0x100,
    /* mobile country code. */
    /* type = uint16_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_MCC                        = 0x101,
    /* mobile network code. */
    /* type = uint16_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_MNC                        = 0x102,
    /* network identification. */
    /* type = uint16_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_NID                        = 0x103,
    /* system identification. */
    /* type = uint16_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_SID                        = 0x104,
    /* base station identification. */
    /* type = uint16_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_BS_ID                      = 0x105,
    /* latitude of the current base station, in units of 0.25 seconds. */
    /* type = uint32_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_BS_LAT                     = 0x106,
    /* longitude of the current base station, in units of 0.25 seconds. */
    /* type = uint32_t */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_BS_LONG                    = 0x107,
    /* EVDO sector id. */
    /* type = string */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_SECTOR_ID                  = 0x108,
    /* EVDO subnet mask. */
    /* type = string */
    MIPC_NW_GET_EHRPD_INFO_CNF_T_SUBNET_MASK                = 0x109,
};

    /* MIPC_MSG.NW_GET_EGMSS_REQ */
enum mipc_nw_get_egmss_req_tlv_enum {
    mipc_nw_get_egmss_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_EGMSS_CNF */
enum mipc_nw_get_egmss_cnf_tlv_enum {
    mipc_nw_get_egmss_cnf_tlv_NONE = 0,
    /* rat. */
    /* type = uint32_t */
    MIPC_NW_GET_EGMSS_CNF_T_RAT                             = 0x100,
    /* mcc */
    /* type = uint32_t */
    MIPC_NW_GET_EGMSS_CNF_T_MCC                             = 0x101,
    /* attach status: 1 attached to the network on the rat; 0 have selected the rat and is going to perform PLMN search. */
    /* type = uint32_t */
    MIPC_NW_GET_EGMSS_CNF_T_STATUS                          = 0x102,
    /* current reported rat */
    /* type = uint32_t */
    MIPC_NW_GET_EGMSS_CNF_T_CUR_REPORTED_RAT                = 0x103,
    /* is home country or not, 0: not home contury, 1: home country */
    /* type = uint32_t */
    MIPC_NW_GET_EGMSS_CNF_T_IS_HOME_COUNTRY                 = 0x104,
};

    /* MIPC_MSG.NW_SET_CACHE_ENDC_CONNECT_MODE_REQ */
enum mipc_nw_set_cache_endc_connect_mode_req_tlv_enum {
    mipc_nw_set_cache_endc_connect_mode_req_tlv_NONE = 0,
    /* 0: no cache , 1: d cache mode,it means we will cache endc when the cell is used to be endc 2: jp mode, we will cache endc for a while of timer1 when idle to connected to LTE cell and cache endc for a while of timer2 when scg connect is break out; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_MODE          = 0x100,
    /* the value of timer1, uint:seconds; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_TIMER1        = 0x101,
    /* the value of timer2,uint:seconds; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_TIMER2        = 0x102,
};

    /* MIPC_MSG.NW_SET_CACHE_ENDC_CONNECT_MODE_CNF */
enum mipc_nw_set_cache_endc_connect_mode_cnf_tlv_enum {
    mipc_nw_set_cache_endc_connect_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_PS_TEST_MODE_REQ */
enum mipc_nw_set_ps_test_mode_req_tlv_enum {
    mipc_nw_set_ps_test_mode_req_tlv_NONE = 0,
    /* The test mode. */
    /* type = uint8_t, refer to NW_PS_TEST_MODE */
    MIPC_NW_SET_PS_TEST_MODE_REQ_T_MODE                     = 0x100,
    /* Specific test profile under the test mode. */
    /* type = uint32_t */
    MIPC_NW_SET_PS_TEST_MODE_REQ_T_PROFILE                  = 0x101,
};

    /* MIPC_MSG.NW_SET_PS_TEST_MODE_CNF */
enum mipc_nw_set_ps_test_mode_cnf_tlv_enum {
    mipc_nw_set_ps_test_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PS_TEST_MODE_REQ */
enum mipc_nw_get_ps_test_mode_req_tlv_enum {
    mipc_nw_get_ps_test_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PS_TEST_MODE_CNF */
enum mipc_nw_get_ps_test_mode_cnf_tlv_enum {
    mipc_nw_get_ps_test_mode_cnf_tlv_NONE = 0,
    /* The test mode. */
    /* type = uint8_t, refer to NW_PS_TEST_MODE */
    MIPC_NW_GET_PS_TEST_MODE_CNF_T_MODE                     = 0x100,
    /* Specific test profile under the test mode. */
    /* type = uint32_t */
    MIPC_NW_GET_PS_TEST_MODE_CNF_T_PROFILE                  = 0x101,
};

    /* MIPC_MSG.NW_SET_SIGNAL_REPORT_CRITERIA_REQ */
enum mipc_nw_set_signal_report_criteria_req_tlv_enum {
    mipc_nw_set_signal_report_criteria_req_tlv_NONE = 0,
    /* 0: Disable; 1: Enable; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_MODE           = 0x100,
    /* The RAT of the signal to be set (e.g., 1-GSM, 2-UMTS, 3-LTE, 4-C2K, 5-NR); Default value is 0. */
    /* type = uint8_t, refer to NW_SIGNAL_RAT */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_RAT            = 0x101,
    /* The signal measurement type; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_SIGNAL_MEASUREMENT_TYPE */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_SIGNAL_MEASUREMENT = 0x102,
    /* A hysteresis time in milliseconds; 0: Disable hysteresis; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_HYSTERESIS_MS  = 0x103,
    /* An interval in dB defining the required magnitude change between reports; its value must be smaller than the smallest threshold delta; 0: Disable hysteresis; Default value is 0. */
    /* type = uint32_t */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_HYSTERESIS_DB  = 0x104,
    /* [Phase out] Please use THRESHOLD_V1. */
    /* type = struct, refer to nw_threshold_array */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_THRESHOLD      = 0x105,
    /*
      An array of triggered thresholds in dBm.
      If both of THRESHOLD_V1 and THRESHOLD are set, modem will preferentially adopt THRESHOLD_V1 instead of THRESHOLD.
    */
    /* type = struct, refer to nw_threshold_array_v1 */
    MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ_T_THRESHOLD_V1   = 0x106,
};

    /* MIPC_MSG.NW_SET_SIGNAL_REPORT_CRITERIA_CNF */
enum mipc_nw_set_signal_report_criteria_cnf_tlv_enum {
    mipc_nw_set_signal_report_criteria_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ECAINFO_REQ */
enum mipc_nw_get_ecainfo_req_tlv_enum {
    mipc_nw_get_ecainfo_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ECAINFO_CNF */
enum mipc_nw_get_ecainfo_cnf_tlv_enum {
    mipc_nw_get_ecainfo_cnf_tlv_NONE = 0,
    /* the cainfo of the response. */
    /* type = struct, refer to nw_ecainfo */
    MIPC_NW_GET_ECAINFO_CNF_T_ECAINFO                       = 0x100,
};

    /* MIPC_MSG.NW_GET_ACTIVITY_INFO_REQ */
enum mipc_nw_get_activity_info_req_tlv_enum {
    mipc_nw_get_activity_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ACTIVITY_INFO_CNF */
enum mipc_nw_get_activity_info_cnf_tlv_enum {
    mipc_nw_get_activity_info_cnf_tlv_NONE = 0,
    /* period(in ms) for which Tx power level n is active. */
    /* type = struct, refer to nw_tx */
    MIPC_NW_GET_ACTIVITY_INFO_CNF_T_TX                      = 0x100,
    /* period(in ms) for which Rx is active. */
    /* type = uint32_t */
    MIPC_NW_GET_ACTIVITY_INFO_CNF_T_RX                      = 0x101,
    /* period(in ms) when modem is power collapsed. */
    /* type = uint32_t */
    MIPC_NW_GET_ACTIVITY_INFO_CNF_T_SLEEP_TIME              = 0x102,
    /* period(in ms) when modem is awake and in idle mode. */
    /* type = uint32_t */
    MIPC_NW_GET_ACTIVITY_INFO_CNF_T_IDLE_TIME               = 0x103,
    /* number of tx power levels, power levels is a series of contiguous (but not necessarily equally-sized) ranges of transmit power level. */
    /* type = uint8_t */
    MIPC_NW_GET_ACTIVITY_INFO_CNF_T_NUM_TX_LEVELS           = 0x104,
};

    /* MIPC_MSG.NW_SET_CA_REQ */
enum mipc_nw_set_ca_req_tlv_enum {
    mipc_nw_set_ca_req_tlv_NONE = 0,
    /* Now just support set LTE CA; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_SET_CA_RAT */
    MIPC_NW_SET_CA_REQ_T_RAT                                = 0x100,
    /* 0: turn off LTE Carrier Aggregation, 1: turn on LTE Carrier Aggregation; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_CA_MODE */
    MIPC_NW_SET_CA_REQ_T_MODE                               = 0x101,
};

    /* MIPC_MSG.NW_SET_CA_CNF */
enum mipc_nw_set_ca_cnf_tlv_enum {
    mipc_nw_set_ca_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_RRC_STATE_REQ */
enum mipc_nw_get_lte_rrc_state_req_tlv_enum {
    mipc_nw_get_lte_rrc_state_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_RRC_STATE_CNF */
enum mipc_nw_get_lte_rrc_state_cnf_tlv_enum {
    mipc_nw_get_lte_rrc_state_cnf_tlv_NONE = 0,
    /* LTE RRC connected mode */
    /* type = uint8_t */
    MIPC_NW_GET_LTE_RRC_STATE_CNF_T_MODE                    = 0x100,
    /* LTE RRC state */
    /* type = uint8_t, refer to LTE_RRC_STATE */
    MIPC_NW_GET_LTE_RRC_STATE_CNF_T_STATE                   = 0x101,
};

    /* MIPC_MSG.NW_GET_LTE_1XRTT_CELL_LIST_REQ */
enum mipc_nw_get_lte_1xrtt_cell_list_req_tlv_enum {
    mipc_nw_get_lte_1xrtt_cell_list_req_tlv_NONE = 0,
    /* 0: LTE available 1xRTT cell lists, 1: LTE unavailable 1xRTT cell lists; Default value is 0xFF. */
    /* type = uint8_t, refer to CELL_LIST_TYPE */
    MIPC_NW_GET_LTE_1XRTT_CELL_LIST_REQ_T_TYPE              = 0x100,
};

    /* MIPC_MSG.NW_GET_LTE_1XRTT_CELL_LIST_CNF */
enum mipc_nw_get_lte_1xrtt_cell_list_cnf_tlv_enum {
    mipc_nw_get_lte_1xrtt_cell_list_cnf_tlv_NONE = 0,
    /* number of LTE available/unavailable 1xRTT cell lists. */
    /* type = uint8_t */
    MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_COUNT             = 0x100,
    /* The cell LIST info of LTE available/unavailable 1xRTT cell lists. */
    /* type = struct_array, refer to 1xrtt_cell_info */
    MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_CELL_LIST         = 0x101,
    /* The cell LIST info of LTE available/unavailable 1xRTT cell lists. */
    /* type = struct, refer to 1xrtt_cell_info */
    MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF_T_CELL_TLV_ARRAY    = 0x102,
};

    /* MIPC_MSG.NW_CLEAR_LTE_AVAILABLE_FILE_REQ */
enum mipc_nw_clear_lte_available_file_req_tlv_enum {
    mipc_nw_clear_lte_available_file_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_CLEAR_LTE_AVAILABLE_FILE_CNF */
enum mipc_nw_clear_lte_available_file_cnf_tlv_enum {
    mipc_nw_clear_lte_available_file_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CA_LINK_CAPABILITY_REQ */
enum mipc_nw_get_ca_link_capability_req_tlv_enum {
    mipc_nw_get_ca_link_capability_req_tlv_NONE = 0,
    /* 0: Get CA downlink capability, 1: Get CA downlink and uplink capability; Default value is 0xFF. */
    /* type = uint8_t, refer to CA_LINK_MODE */
    MIPC_NW_GET_CA_LINK_CAPABILITY_REQ_T_MODE               = 0x100,
};

    /* MIPC_MSG.NW_GET_CA_LINK_CAPABILITY_CNF */
enum mipc_nw_get_ca_link_capability_cnf_tlv_enum {
    mipc_nw_get_ca_link_capability_cnf_tlv_NONE = 0,
    /* number of band combo */
    /* type = uint8_t */
    MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_COUNT              = 0x100,
    /* CA link capability. */
    /* type = struct_array, refer to band_combo_info */
    MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_BAND_COMBO         = 0x101,
    /* CA link capability. */
    /* type = struct, refer to band_combo_info */
    MIPC_NW_GET_CA_LINK_CAPABILITY_CNF_T_BAND_COMBO_TLV_ARRAY = 0x102,
};

    /* MIPC_MSG.NW_GET_CA_LINK_ENABLE_STATUS_REQ */
enum mipc_nw_get_ca_link_enable_status_req_tlv_enum {
    mipc_nw_get_ca_link_enable_status_req_tlv_NONE = 0,
    /* Get UL/DL CA enable status for the specified band_combo; Default value is 0xFF. */
    /* type = uint8_t, refer to CA_LINK_ENABLE_MODE */
    MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ_T_MODE            = 0x100,
    /* specific band combo; Default value is null character. */
    /* type = string */
    MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ_T_BAND_COMBO      = 0x101,
};

    /* MIPC_MSG.NW_GET_CA_LINK_ENABLE_STATUS_CNF */
enum mipc_nw_get_ca_link_enable_status_cnf_tlv_enum {
    mipc_nw_get_ca_link_enable_status_cnf_tlv_NONE = 0,
    /* UL/DL CA enable status of specific band combo. */
    /* type = uint8_t, refer to BAND_COMBO_ENABLE_STATUS */
    MIPC_NW_GET_CA_LINK_ENABLE_STATUS_CNF_T_STATUS          = 0x100,
};

    /* MIPC_MSG.NW_GET_TM9_ENABLE_STATUS_REQ */
enum mipc_nw_get_tm9_enable_status_req_tlv_enum {
    mipc_nw_get_tm9_enable_status_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_TM9_ENABLE_STATUS_CNF */
enum mipc_nw_get_tm9_enable_status_cnf_tlv_enum {
    mipc_nw_get_tm9_enable_status_cnf_tlv_NONE = 0,
    /* FDD TM9 enable status */
    /* type = uint8_t, refer to TM9_ENABLE_STATUS */
    MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_FDD_SETTING     = 0x100,
    /* TDD TM9 enable status */
    /* type = uint8_t, refer to TM9_ENABLE_STATUS */
    MIPC_NW_GET_TM9_ENABLE_STATUS_CNF_T_TM9_TDD_SETTING     = 0x101,
};

    /* MIPC_MSG.NW_SET_TM9_ENABLE_STATUS_REQ */
enum mipc_nw_set_tm9_enable_status_req_tlv_enum {
    mipc_nw_set_tm9_enable_status_req_tlv_NONE = 0,
    /* 0: Set FDD TM9, 1: Set TDD TM9; Default value is 0xFF. */
    /* type = uint8_t, refer to TM9_SETTING_TYPE */
    MIPC_NW_SET_TM9_ENABLE_STATUS_REQ_T_TYPE                = 0x100,
    /* TDD TM9 enable status; Default value is 0xFF. */
    /* type = uint8_t, refer to TM9_ENABLE_STATUS */
    MIPC_NW_SET_TM9_ENABLE_STATUS_REQ_T_STATUS              = 0x101,
};

    /* MIPC_MSG.NW_SET_TM9_ENABLE_STATUS_CNF */
enum mipc_nw_set_tm9_enable_status_cnf_tlv_enum {
    mipc_nw_set_tm9_enable_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_OMADM_CONF_REQ */
enum mipc_nw_get_omadm_conf_req_tlv_enum {
    mipc_nw_get_omadm_conf_req_tlv_NONE = 0,
    /* OMA-DM node type; Default value is 0. */
    /* type = uint8_t, refer to OMADM_NODE_TYPE */
    MIPC_NW_GET_OMADM_CONF_REQ_T_TYPE                       = 0x100,
};

    /* MIPC_MSG.NW_GET_OMADM_CONF_CNF */
enum mipc_nw_get_omadm_conf_cnf_tlv_enum {
    mipc_nw_get_omadm_conf_cnf_tlv_NONE = 0,
    /* OMA-DM configured value */
    /* type = string */
    MIPC_NW_GET_OMADM_CONF_CNF_T_NODE_VALUE                 = 0x100,
};

    /* MIPC_MSG.NW_SET_OMADM_CONF_REQ */
enum mipc_nw_set_omadm_conf_req_tlv_enum {
    mipc_nw_set_omadm_conf_req_tlv_NONE = 0,
    /* OMA-DM node type; Default value is 0. */
    /* type = uint8_t, refer to OMADM_NODE_TYPE */
    MIPC_NW_SET_OMADM_CONF_REQ_T_TYPE                       = 0x100,
    /* OMA-DM configured value */
    /* type = string */
    MIPC_NW_SET_OMADM_CONF_REQ_T_NODE_VALUE                 = 0x101,
};

    /* MIPC_MSG.NW_SET_OMADM_CONF_CNF */
enum mipc_nw_set_omadm_conf_cnf_tlv_enum {
    mipc_nw_set_omadm_conf_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CA_BAND_MODE_REQ */
enum mipc_nw_get_ca_band_mode_req_tlv_enum {
    mipc_nw_get_ca_band_mode_req_tlv_NONE = 0,
    /* Primary band for which CA bitmap needs to be provided; Default value is 0. */
    /* type = int32_t */
    MIPC_NW_GET_CA_BAND_MODE_REQ_T_PRIMARY_ID               = 0x100,
};

    /* MIPC_MSG.NW_GET_CA_BAND_MODE_CNF */
enum mipc_nw_get_ca_band_mode_cnf_tlv_enum {
    mipc_nw_get_ca_band_mode_cnf_tlv_NONE = 0,
    /* bitmap band info */
    /* type = struct, refer to nw_ca_band */
    MIPC_NW_GET_CA_BAND_MODE_CNF_T_BAND                     = 0x100,
};

    /* MIPC_MSG.NW_SET_CA_LINK_ENABLE_STATUS_REQ */
enum mipc_nw_set_ca_link_enable_status_req_tlv_enum {
    mipc_nw_set_ca_link_enable_status_req_tlv_NONE = 0,
    /* The list type of CA combination band list; Default value is 0. */
    /* type = uint8_t, refer to CA_COMB_LIST_TYPE */
    MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_LIST_TYPE       = 0x100,
    /* CA combination band list; Default value is null character. */
    /* type = string */
    MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_CA_COMB_LIST    = 0x101,
    /* UL/DL, just for future expansion, now this parameter is invalid; Default value is 0xFF. */
    /* type = uint8_t */
    MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ_T_LINK_TYPE       = 0x102,
};

    /* MIPC_MSG.NW_SET_CA_LINK_ENABLE_STATUS_CNF */
enum mipc_nw_set_ca_link_enable_status_cnf_tlv_enum {
    mipc_nw_set_ca_link_enable_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_DATA_REQ */
enum mipc_nw_get_lte_data_req_tlv_enum {
    mipc_nw_get_lte_data_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_DATA_CNF */
enum mipc_nw_get_lte_data_cnf_tlv_enum {
    mipc_nw_get_lte_data_cnf_tlv_NONE = 0,
    /* The registration state of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_REG_STATE                    = 0x100,
    /* MCC */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_MCC                          = 0x101,
    /* MNC */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_MNC                          = 0x102,
    /* cell id */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_CELL_ID                      = 0x103,
    /* The band of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_BAND                         = 0x104,
    /* The bandwidth of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_BANDWIDTH                    = 0x105,
    /* ul channel */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_UL_CHANNEL                   = 0x106,
    /* dl channel */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_DL_CHANNEL                   = 0x107,
    /* The rssi of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_RSSI                         = 0x108,
    /* The rsrp of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_RSRP                         = 0x109,
    /* The rsrq of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_RSRQ                         = 0x10A,
    /* The tx power of LTE. */
    /* type = uint32_t */
    MIPC_NW_GET_LTE_DATA_CNF_T_TX_POWER                     = 0x10B,
};

    /* MIPC_MSG.NW_SET_TUW_TIMER_LENGTH_REQ */
enum mipc_nw_set_tuw_timer_length_req_tlv_enum {
    mipc_nw_set_tuw_timer_length_req_tlv_NONE = 0,
    /* Number of updated tuw, 1~3; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_TUW_TIMER_LENGTH_REQ_T_TUW_NUM              = 0x100,
    /* The tuw id and tuw length */
    /* type = struct_array, refer to nw_tuw_info */
    MIPC_NW_SET_TUW_TIMER_LENGTH_REQ_T_TUW                  = 0x101,
    /* [Phase out] Please use TUW_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_tuw_info */
    MIPC_NW_SET_TUW_TIMER_LENGTH_REQ_T_TUW_TLV_ARRAY        = 0x102,
    /* The TLV array of TUW information */
    /* type = struct, refer to nw_tuw_info_v1 */
    MIPC_NW_SET_TUW_TIMER_LENGTH_REQ_T_TUW_V1_TLV_ARRAY     = 0x103,
};

    /* MIPC_MSG.NW_SET_TUW_TIMER_LENGTH_CNF */
enum mipc_nw_set_tuw_timer_length_cnf_tlv_enum {
    mipc_nw_set_tuw_timer_length_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_TUW_TIMER_LENGTH_REQ */
enum mipc_nw_get_tuw_timer_length_req_tlv_enum {
    mipc_nw_get_tuw_timer_length_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_TUW_TIMER_LENGTH_CNF */
enum mipc_nw_get_tuw_timer_length_cnf_tlv_enum {
    mipc_nw_get_tuw_timer_length_cnf_tlv_NONE = 0,
    /* TUW1 length */
    /* type = uint32_t */
    MIPC_NW_GET_TUW_TIMER_LENGTH_CNF_T_TUW1                 = 0x100,
    /* TUW2 length */
    /* type = uint32_t */
    MIPC_NW_GET_TUW_TIMER_LENGTH_CNF_T_TUW2                 = 0x101,
    /* TUW3 length */
    /* type = uint32_t */
    MIPC_NW_GET_TUW_TIMER_LENGTH_CNF_T_TUW3                 = 0x102,
};

    /* MIPC_MSG.NW_GET_5GUW_INFO_REQ */
enum mipc_nw_get_5guw_info_req_tlv_enum {
    mipc_nw_get_5guw_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_5GUW_INFO_CNF */
enum mipc_nw_get_5guw_info_cnf_tlv_enum {
    mipc_nw_get_5guw_info_cnf_tlv_NONE = 0,
    /* 0: don't display 5G UW, 1: display 5G UW. */
    /* type = uint8_t */
    MIPC_NW_GET_5GUW_INFO_CNF_T_DISPLAY_5GUW                = 0x100,
    /* 0: not on n77 band, 1: on n77 band */
    /* type = uint8_t */
    MIPC_NW_GET_5GUW_INFO_CNF_T_ON_N77_BAND                 = 0x101,
    /* 0: not on fr2 band, 1: on fr2 band */
    /* type = uint8_t */
    MIPC_NW_GET_5GUW_INFO_CNF_T_ON_FR2_BAND                 = 0x102,
    /* 0: not allowed, 1: allowed */
    /* type = uint8_t */
    MIPC_NW_GET_5GUW_INFO_CNF_T_5GUW_ALLOWED                = 0x103,
};

    /* MIPC_MSG.NW_GET_NR_CA_BAND_REQ */
enum mipc_nw_get_nr_ca_band_req_tlv_enum {
    mipc_nw_get_nr_ca_band_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NR_CA_BAND_CNF */
enum mipc_nw_get_nr_ca_band_cnf_tlv_enum {
    mipc_nw_get_nr_ca_band_cnf_tlv_NONE = 0,
    /*  */
    /* type = int32_t */
    MIPC_NW_GET_NR_CA_BAND_CNF_T_IS_ENDC                    = 0x100,
    /* The num of nr ca band */
    /* type = uint8_t */
    MIPC_NW_GET_NR_CA_BAND_CNF_T_BAND_NUM                   = 0x101,
    /* The nr ca band */
    /* type = struct, refer to nr_ca_band */
    MIPC_NW_GET_NR_CA_BAND_CNF_T_BAND                       = 0x102,
};

    /* MIPC_MSG.NW_GET_NR_SCS_REQ */
enum mipc_nw_get_nr_scs_req_tlv_enum {
    mipc_nw_get_nr_scs_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NR_SCS_CNF */
enum mipc_nw_get_nr_scs_cnf_tlv_enum {
    mipc_nw_get_nr_scs_cnf_tlv_NONE = 0,
    /* nr5g subcarrier spacing in khz */
    /* type = int32_t */
    MIPC_NW_GET_NR_SCS_CNF_T_SCS                            = 0x100,
};

    /* MIPC_MSG.NW_GET_PHYSICAL_CHANNEL_CONFIGS_REQ */
enum mipc_nw_get_physical_channel_configs_req_tlv_enum {
    mipc_nw_get_physical_channel_configs_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF */
enum mipc_nw_get_physical_channel_configs_cnf_tlv_enum {
    mipc_nw_get_physical_channel_configs_cnf_tlv_NONE = 0,
    /* [Phase out] Please use PHYSICAL_CHANNEL_CONFIGS_V2_TLV_ARRAY */
    /* type = struct, refer to physical_channel_info_v1 */
    MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF_T_PHYSICAL_CHANNEL_CONFIGS_LIST_V1 = 0x100,
    /* physical channel configs information. */
    /* type = struct, refer to physical_channel_info_v2 */
    MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF_T_PHYSICAL_CHANNEL_CONFIGS_V2_TLV_ARRAY = 0x101,
};

    /* MIPC_MSG.NW_OS_ID_UPDATE_REQ */
enum mipc_nw_os_id_update_req_tlv_enum {
    mipc_nw_os_id_update_req_tlv_NONE = 0,
    /* The number of the os_id,the number value of:0~15 */
    /* type = uint8_t */
    MIPC_NW_OS_ID_UPDATE_REQ_T_OS_ID_NUM                    = 0x100,
    /* The set of the os_id. */
    /* type = struct, refer to os_id_info */
    MIPC_NW_OS_ID_UPDATE_REQ_T_OS_ID_LIST                   = 0x101,
};

    /* MIPC_MSG.NW_OS_ID_UPDATE_CNF */
enum mipc_nw_os_id_update_cnf_tlv_enum {
    mipc_nw_os_id_update_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NETWORK_SCAN_SUPPORT_STATUS_REQ */
enum mipc_nw_get_network_scan_support_status_req_tlv_enum {
    mipc_nw_get_network_scan_support_status_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NETWORK_SCAN_SUPPORT_STATUS_CNF */
enum mipc_nw_get_network_scan_support_status_cnf_tlv_enum {
    mipc_nw_get_network_scan_support_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_PREF_NSSAI_REQ */
enum mipc_nw_set_pref_nssai_req_tlv_enum {
    mipc_nw_set_pref_nssai_req_tlv_NONE = 0,
    /* Preferred NSSAI list */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_SET_PREF_NSSAI_REQ_T_PREFERRED_NSSAI_3GPP_LIST  = 0x100,
    /* Preferred NSSAI list for non-3GPP */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_SET_PREF_NSSAI_REQ_T_PREFERRED_NSSAI_NON3GPP_LIST = 0x101,
};

    /* MIPC_MSG.NW_SET_PREF_NSSAI_CNF */
enum mipc_nw_set_pref_nssai_cnf_tlv_enum {
    mipc_nw_set_pref_nssai_cnf_tlv_NONE = 0,
    /* Preferred NSSAI list */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_SET_PREF_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_LIST  = 0x100,
    /* Preferred NSSAI list for non-3GPP */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_SET_PREF_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_LIST = 0x101,
};

    /* MIPC_MSG.NW_SET_DEFAULT_NSSAI_REQ */
enum mipc_nw_set_default_nssai_req_tlv_enum {
    mipc_nw_set_default_nssai_req_tlv_NONE = 0,
    /* Default NSSAI list */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_SET_DEFAULT_NSSAI_REQ_T_DEFAULT_CONFIGURED_NSSAI_LIST = 0x100,
};

    /* MIPC_MSG.NW_SET_DEFAULT_NSSAI_CNF */
enum mipc_nw_set_default_nssai_cnf_tlv_enum {
    mipc_nw_set_default_nssai_cnf_tlv_NONE = 0,
    /* Default NSSAI list */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_SET_DEFAULT_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_LIST = 0x100,
};

    /* MIPC_MSG.NW_GET_NSSAI_REQ */
enum mipc_nw_get_nssai_req_tlv_enum {
    mipc_nw_get_nssai_req_tlv_NONE = 0,
    /* It indicates the NSSAI type to be acquired; Default value is 0. */
    /* type = uint8_t, refer to NSSAI_TYPE_ENUM */
    MIPC_NW_GET_NSSAI_REQ_T_NSSAI_TYPE                      = 0x100,
    /*
      PLMN_ID[0] = MCC1,
      PLMN_ID[1] = MCC2,
      PLMN_ID[2] = MCC3,
      PLMN_ID[3] = MNC1,
      PLMN_ID[4] = MNC2,
      PLMN_ID[5] = MNC3
      Default value is NULL.
    */
    /* type = byte_array */
    MIPC_NW_GET_NSSAI_REQ_T_PLMN_ID                         = 0x101,
};

    /* MIPC_MSG.NW_GET_NSSAI_CNF */
enum mipc_nw_get_nssai_cnf_tlv_enum {
    mipc_nw_get_nssai_cnf_tlv_NONE = 0,
    /* Default NSSAI list */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_DEFAULT_CONFIGURED_NSSAI_LIST   = 0x100,
    /* Rejected NSSAI list for 3GPP */
    /* type = struct, refer to rejected_s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_REJECTED_NSSAI_3GPP_LIST        = 0x101,
    /* Rejected NSSAI list for non-3GPP */
    /* type = struct, refer to rejected_s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_REJECTED_NSSAI_NON3GPP_LIST     = 0x102,
    /* Configured NSSAI list */
    /* type = struct, refer to plmn_specific_s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_CONFIGURED_NSSAI_LIST           = 0x103,
    /* Allowed NSSAI list for 3GPP */
    /* type = struct, refer to plmn_specific_s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_ALLOWED_NSSAI_3GPP_LIST         = 0x104,
    /* Allowed NSSAI list for non-3GPP */
    /* type = struct, refer to plmn_specific_s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_ALLOWED_NSSAI_NON3GPP_LIST      = 0x105,
    /* Preferred NSSAI list for 3GPP */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_PREFERRED_NSSAI_3GPP_LIST       = 0x106,
    /* Preferred NSSAI list for non-3GPP */
    /* type = struct, refer to s_nssai_struct */
    MIPC_NW_GET_NSSAI_CNF_T_PREFERRED_NSSAI_NON3GPP_LIST    = 0x107,
};

    /* MIPC_MSG.NW_SET_5GUC_REQ */
enum mipc_nw_set_5guc_req_tlv_enum {
    mipc_nw_set_5guc_req_tlv_NONE = 0,
    /* 0~240 seconds, default setting of 30 seconds, omit means no change; Default value is 0x7FFFFFFF. */
    /* type = uint32_t */
    MIPC_NW_SET_5GUC_REQ_T_REFRESH_TIMER_LENGTH             = 0x100,
    /* A bitmap of nsa band, omit means no change. n41,n258,n260,n261,n262 default enable */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_SET_5GUC_REQ_T_NSA_BAND                         = 0x101,
    /* A bitmap of sa band, omit means no change. n41,n258,n260,n261,n262 default enable */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_SET_5GUC_REQ_T_SA_BAND                          = 0x102,
    /* 0: disable bandwidth check, 1: enable bandwidth check, omit means no change; Default value is 0x7FFFFFFF. */
    /* type = uint32_t, refer to NW_BW_CHECK */
    MIPC_NW_SET_5GUC_REQ_T_BW_CHECK_ENABLE                  = 0x103,
    /* 0~240 MHz, default setting of 60 MHz, omit means no change; Default value is 0x7FFFFFFF. */
    /* type = uint32_t */
    MIPC_NW_SET_5GUC_REQ_T_BW_CHECK_THRESHOLD               = 0x104,
};

    /* MIPC_MSG.NW_SET_5GUC_CNF */
enum mipc_nw_set_5guc_cnf_tlv_enum {
    mipc_nw_set_5guc_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_5GUC_SETTING_REQ */
enum mipc_nw_get_5guc_setting_req_tlv_enum {
    mipc_nw_get_5guc_setting_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_5GUC_SETTING_CNF */
enum mipc_nw_get_5guc_setting_cnf_tlv_enum {
    mipc_nw_get_5guc_setting_cnf_tlv_NONE = 0,
    /* 0~240 seconds, default setting of 30 seconds */
    /* type = uint32_t */
    MIPC_NW_GET_5GUC_SETTING_CNF_T_REFRESH_TIMER_LENGTH     = 0x100,
    /* A bitmap of nsa band. n41,n258,n260,n261,n262 default enable */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_GET_5GUC_SETTING_CNF_T_NSA_BAND                 = 0x101,
    /* A bitmap of sa band. n41,n258,n260,n261,n262 default enable */
    /* type = struct, refer to sys_nr_band */
    MIPC_NW_GET_5GUC_SETTING_CNF_T_SA_BAND                  = 0x102,
    /* 0: disable bandwidth check, 1: enable bandwidth check */
    /* type = uint32_t, refer to NW_BW_CHECK */
    MIPC_NW_GET_5GUC_SETTING_CNF_T_BW_CHECK_ENABLE          = 0x103,
    /* 0~240 MHz */
    /* type = uint32_t */
    MIPC_NW_GET_5GUC_SETTING_CNF_T_BW_CHECK_THRESHOLD       = 0x104,
};

    /* MIPC_MSG.NW_GET_5GUC_INFO_REQ */
enum mipc_nw_get_5guc_info_req_tlv_enum {
    mipc_nw_get_5guc_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_5GUC_INFO_CNF */
enum mipc_nw_get_5guc_info_cnf_tlv_enum {
    mipc_nw_get_5guc_info_cnf_tlv_NONE = 0,
    /* 0: not display 5G UC, 1: display 5G UC */
    /* type = uint32_t, refer to NW_5GUC_STATE */
    MIPC_NW_GET_5GUC_INFO_CNF_T_DISPLAY_5GUC                = 0x100,
    /* 0: not on UC band(ex: n41), 1: on UC band(ex: n41) */
    /* type = uint32_t, refer to NW_UC_BAND */
    MIPC_NW_GET_5GUC_INFO_CNF_T_ON_UC_BAND                  = 0x101,
    /* 0~300 MHz */
    /* type = uint32_t */
    MIPC_NW_GET_5GUC_INFO_CNF_T_AGG_BW                      = 0x102,
};

    /* MIPC_MSG.NW_GET_FIRST_PLMN_REQ */
enum mipc_nw_get_first_plmn_req_tlv_enum {
    mipc_nw_get_first_plmn_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_FIRST_PLMN_CNF */
enum mipc_nw_get_first_plmn_cnf_tlv_enum {
    mipc_nw_get_first_plmn_cnf_tlv_NONE = 0,
    /* mcc */
    /* type = string */
    MIPC_NW_GET_FIRST_PLMN_CNF_T_MCC                        = 0x100,
    /* mnc */
    /* type = string */
    MIPC_NW_GET_FIRST_PLMN_CNF_T_MNC                        = 0x101,
};

    /* MIPC_MSG.NW_SET_UE_USAGE_SETTING_REQ */
enum mipc_nw_set_ue_usage_setting_req_tlv_enum {
    mipc_nw_set_ue_usage_setting_req_tlv_NONE = 0,
    /* 0: voice centric; 1: data centric; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_UE_USAGE_SETTING_REQ_T_USAGE_SETTING        = 0x100,
};

    /* MIPC_MSG.NW_SET_UE_USAGE_SETTING_CNF */
enum mipc_nw_set_ue_usage_setting_cnf_tlv_enum {
    mipc_nw_set_ue_usage_setting_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_UE_USAGE_SETTING_REQ */
enum mipc_nw_get_ue_usage_setting_req_tlv_enum {
    mipc_nw_get_ue_usage_setting_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_UE_USAGE_SETTING_CNF */
enum mipc_nw_get_ue_usage_setting_cnf_tlv_enum {
    mipc_nw_get_ue_usage_setting_cnf_tlv_NONE = 0,
    /* usage setting's value returned */
    /* type = uint8_t */
    MIPC_NW_GET_UE_USAGE_SETTING_CNF_T_USAGE_SETTING        = 0x100,
};

    /* MIPC_MSG.NW_SET_CAG_STATUS_REQ */
enum mipc_nw_set_cag_status_req_tlv_enum {
    mipc_nw_set_cag_status_req_tlv_NONE = 0,
    /* the status of the set cag:0, disable; 1, enable; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CAG_STATUS_REQ_T_STATUS                     = 0x100,
};

    /* MIPC_MSG.NW_SET_CAG_STATUS_CNF */
enum mipc_nw_set_cag_status_cnf_tlv_enum {
    mipc_nw_set_cag_status_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CAG_SELECT_MODE_REQ */
enum mipc_nw_set_cag_select_mode_req_tlv_enum {
    mipc_nw_set_cag_select_mode_req_tlv_NONE = 0,
    /* the select mode of the set cag:0, auto select; 1, manual select; Default value is 0. */
    /* type = uint8_t */
    MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_SELECT_MODE           = 0x100,
    /* the PLMN ID of the set manual cag; Default value is null character. */
    /* type = string */
    MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_PLMN                  = 0x101,
    /* the CAG ID of the set manual cag; Default value is 0xFFFFFFFF. */
    /* type = uint32_t */
    MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_CAG_ID                = 0x102,
    /* the ACT of the set manual cag; Default value is 0xFF. */
    /* type = uint8_t */
    MIPC_NW_SET_CAG_SELECT_MODE_REQ_T_ACT                   = 0x103,
};

    /* MIPC_MSG.NW_SET_CAG_SELECT_MODE_CNF */
enum mipc_nw_set_cag_select_mode_cnf_tlv_enum {
    mipc_nw_set_cag_select_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CAG_LIST_REQ */
enum mipc_nw_get_cag_list_req_tlv_enum {
    mipc_nw_get_cag_list_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CAG_LIST_CNF */
enum mipc_nw_get_cag_list_cnf_tlv_enum {
    mipc_nw_get_cag_list_cnf_tlv_NONE = 0,
    /* The list of the plmn and CAG list. */
    /* type = struct, refer to plmn_cag_info */
    MIPC_NW_GET_CAG_LIST_CNF_T_PLMN_CAG_LIST                = 0x100,
};

    /* MIPC_MSG.NW_SET_ALLOWED_MCC_LIST_REQ */
enum mipc_nw_set_allowed_mcc_list_req_tlv_enum {
    mipc_nw_set_allowed_mcc_list_req_tlv_NONE = 0,
    /* 0: disable this feature, 1: enable this feature; Default value is 0xFF. */
    /* type = uint8_t, refer to NW_ALLOWED_MCC_LIST_ACTION */
    MIPC_NW_SET_ALLOWED_MCC_LIST_REQ_T_ACTION               = 0x100,
    /* The list of allowed mcc. */
    /* type = struct, refer to nw_allowed_mcc_list */
    MIPC_NW_SET_ALLOWED_MCC_LIST_REQ_T_ALLOWED_MCC_LIST     = 0x101,
};

    /* MIPC_MSG.NW_SET_ALLOWED_MCC_LIST_CNF */
enum mipc_nw_set_allowed_mcc_list_cnf_tlv_enum {
    mipc_nw_set_allowed_mcc_list_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_N3_POWER_STATE_REQ */
enum mipc_nw_set_n3_power_state_req_tlv_enum {
    mipc_nw_set_n3_power_state_req_tlv_NONE = 0,
    /* 0: disable n3 power, 1: enable n3 power */
    /* type = uint32_t, refer to N3_POWER_STATE */
    MIPC_NW_SET_N3_POWER_STATE_REQ_T_MODE                   = 0x100,
};

    /* MIPC_MSG.NW_SET_N3_POWER_STATE_CNF */
enum mipc_nw_set_n3_power_state_cnf_tlv_enum {
    mipc_nw_set_n3_power_state_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_N3_GW_TYPE_REQ */
enum mipc_nw_set_n3_gw_type_req_tlv_enum {
    mipc_nw_set_n3_gw_type_req_tlv_NONE = 0,
    /* 5G Untrusted Non-3GPP, 0: disable, 1: enable. If this tlv is not added, the gateway type will be disabled. */
    /* type = uint8_t */
    MIPC_NW_SET_N3_GW_TYPE_REQ_T_N3IWF                      = 0x100,
    /* 5G Trusted Non-3GPP, 0: disable, 1: enable. If this tlv is not added, the gateway type will be disabled. */
    /* type = uint8_t */
    MIPC_NW_SET_N3_GW_TYPE_REQ_T_TNGF                       = 0x101,
    /* 5G Wireline, 0: disable, 1: enable. If this tlv is not added, the gateway type will be disabled. */
    /* type = uint8_t */
    MIPC_NW_SET_N3_GW_TYPE_REQ_T_WAGF                       = 0x102,
};

    /* MIPC_MSG.NW_SET_N3_GW_TYPE_CNF */
enum mipc_nw_set_n3_gw_type_cnf_tlv_enum {
    mipc_nw_set_n3_gw_type_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_EDRX_SETTING_REQ */
enum mipc_nw_set_edrx_setting_req_tlv_enum {
    mipc_nw_set_edrx_setting_req_tlv_NONE = 0,
    /* Indicates to disable or enable the use of eDRX in the UE */
    /* type = uint8_t */
    MIPC_NW_SET_EDRX_SETTING_REQ_T_MODE                     = 0x100,
    /* Indicates the type of access technology */
    /* type = uint8_t, refer to NW_EDRX_ACCESS_TECHNOLOGY_ENUM */
    MIPC_NW_SET_EDRX_SETTING_REQ_T_ACT                      = 0x101,
    /* The eDRX value requested */
    /* type = uint8_t */
    MIPC_NW_SET_EDRX_SETTING_REQ_T_REQUESTED_EDRX_VALUE     = 0x102,
    /* The paging time window value requested */
    /* type = uint8_t */
    MIPC_NW_SET_EDRX_SETTING_REQ_T_REQUESTED_PAGING_TIME_WINDOW = 0x103,
};

    /* MIPC_MSG.NW_SET_EDRX_SETTING_CNF */
enum mipc_nw_set_edrx_setting_cnf_tlv_enum {
    mipc_nw_set_edrx_setting_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_EDRX_SETTING_REQ */
enum mipc_nw_get_edrx_setting_req_tlv_enum {
    mipc_nw_get_edrx_setting_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_EDRX_SETTING_CNF */
enum mipc_nw_get_edrx_setting_cnf_tlv_enum {
    mipc_nw_get_edrx_setting_cnf_tlv_NONE = 0,
    /* Indicates the type of access technology */
    /* type = uint8_t, refer to NW_EDRX_ACCESS_TECHNOLOGY_ENUM */
    MIPC_NW_GET_EDRX_SETTING_CNF_T_ACT                      = 0x100,
    /* The eDRX value requested */
    /* type = uint8_t */
    MIPC_NW_GET_EDRX_SETTING_CNF_T_REQUESTED_EDRX_VALUE     = 0x101,
};

    /* MIPC_MSG.NW_SET_EDRX_REQ */
enum mipc_nw_set_edrx_req_tlv_enum {
    mipc_nw_set_edrx_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_EDRX_CNF */
enum mipc_nw_set_edrx_cnf_tlv_enum {
    mipc_nw_set_edrx_cnf_tlv_NONE = 0,
    /* Indicates the type of access technology */
    /* type = uint8_t, refer to NW_EDRX_ACCESS_TECHNOLOGY_ENUM */
    MIPC_NW_SET_EDRX_CNF_T_ACT                              = 0x100,
    /* The eDRX value requested */
    /* type = uint8_t */
    MIPC_NW_SET_EDRX_CNF_T_REQUESTED_EDRX_VALUE             = 0x101,
    /* The eDRX value provided by NW */
    /* type = uint8_t */
    MIPC_NW_SET_EDRX_CNF_T_NW_PROVIDED_EDRX_VALUE           = 0x102,
    /* The value of the paging time window */
    /* type = uint8_t */
    MIPC_NW_SET_EDRX_CNF_T_PAGING_TIME_WINDOW               = 0x103,
};

    /* MIPC_MSG.NW_SET_POWER_SAVING_MODE_SETTING_REQ */
enum mipc_nw_set_power_saving_mode_setting_req_tlv_enum {
    mipc_nw_set_power_saving_mode_setting_req_tlv_NONE = 0,
    /* Indication to disable or enable the use of PSM in the UE. */
    /* type = uint8_t, refer to NW_PSM_MODE_ENUM */
    MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ_T_PSM_MODE    = 0x100,
    /* Requested extended periodic TAU value(T3412) to be allocate to the UE in E-UTRAN. */
    /* type = uint8_t */
    MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ_T_REQUESTED_PERIODIC_TAU_VALUE = 0x101,
    /* Requested Active Time value(T3324) to be allocated to the UE. */
    /* type = uint8_t */
    MIPC_NW_SET_POWER_SAVING_MODE_SETTING_REQ_T_REQUESTED_ACTIVE_TIME_VALUE = 0x102,
};

    /* MIPC_MSG.NW_SET_POWER_SAVING_MODE_SETTING_CNF */
enum mipc_nw_set_power_saving_mode_setting_cnf_tlv_enum {
    mipc_nw_set_power_saving_mode_setting_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_POWER_SAVING_MODE_SETTING_REQ */
enum mipc_nw_get_power_saving_mode_setting_req_tlv_enum {
    mipc_nw_get_power_saving_mode_setting_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_POWER_SAVING_MODE_SETTING_CNF */
enum mipc_nw_get_power_saving_mode_setting_cnf_tlv_enum {
    mipc_nw_get_power_saving_mode_setting_cnf_tlv_NONE = 0,
    /* Indication to disable or enable the use of PSM in the UE. */
    /* type = uint8_t, refer to NW_PSM_MODE_ENUM */
    MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF_T_PSM_MODE    = 0x100,
    /* Requested extended periodic TAU value(T3412) to be allocate to the UE in E-UTRAN. */
    /* type = uint8_t */
    MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF_T_REQUESTED_PERIODIC_TAU_VALUE = 0x101,
    /* Requested Active Time value(T3324) to be allocated to the UE. */
    /* type = uint8_t */
    MIPC_NW_GET_POWER_SAVING_MODE_SETTING_CNF_T_REQUESTED_ACTIVE_TIME_VALUE = 0x102,
};

    /* MIPC_MSG.NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ */
enum mipc_nw_set_mobile_initiated_connection_only_mode_req_tlv_enum {
    mipc_nw_set_mobile_initiated_connection_only_mode_req_tlv_NONE = 0,
    /* The requested MICO mode. */
    /* type = uint8_t */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ_T_REQUESTED_MICO_MODE = 0x100,
    /* Requested Active Time value(T3324) to be allocated to the UE. */
    /* type = uint8_t */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ_T_REQUESTED_ACTIVE_TIME = 0x101,
};

    /* MIPC_MSG.NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF */
enum mipc_nw_set_mobile_initiated_connection_only_mode_cnf_tlv_enum {
    mipc_nw_set_mobile_initiated_connection_only_mode_cnf_tlv_NONE = 0,
    /* The current MICO mode. */
    /* type = uint8_t */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_CURRENT_MICO_MODE = 0x100,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the registration area allocation indication. */
    /* type = uint8_t */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_RAAI_VALUE = 0x101,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the use of the Strictly Periodic Registration Timer. */
    /* type = uint8_t */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_SPRT_VALUE = 0x102,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the Active Time value(T3324) assigned by the network. */
    /* type = uint8_t */
    MIPC_NW_SET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_ALLOCATED_ACTIVE_TIME = 0x103,
};

    /* MIPC_MSG.NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_REQ */
enum mipc_nw_get_mobile_initiated_connection_only_mode_req_tlv_enum {
    mipc_nw_get_mobile_initiated_connection_only_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF */
enum mipc_nw_get_mobile_initiated_connection_only_mode_cnf_tlv_enum {
    mipc_nw_get_mobile_initiated_connection_only_mode_cnf_tlv_NONE = 0,
    /* The requested MICO mode. */
    /* type = uint8_t */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_REQUESTED_MICO_MODE = 0x100,
    /* The current MICO mode. */
    /* type = uint8_t */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_CURRENT_MICO_MODE = 0x101,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the registration area allocation indication. */
    /* type = uint8_t */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_RAAI_VALUE = 0x102,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the use of the Strictly Periodic Registration Timer. */
    /* type = uint8_t */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_SPRT_VALUE = 0x103,
    /* Requested Active Time value(T3324) to be allocated to the UE. */
    /* type = uint8_t */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_REQUESTED_ACTIVE_TIME = 0x104,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the Active Time value(T3324) assigned by the network. */
    /* type = uint8_t */
    MIPC_NW_GET_MOBILE_INITIATED_CONNECTION_ONLY_MODE_CNF_T_ALLOCATED_ACTIVE_TIME = 0x105,
};

    /* MIPC_MSG.NW_GET_LATEST_CA_MEASUREMENT_REQ */
enum mipc_nw_get_latest_ca_measurement_req_tlv_enum {
    mipc_nw_get_latest_ca_measurement_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LATEST_CA_MEASUREMENT_CNF */
enum mipc_nw_get_latest_ca_measurement_cnf_tlv_enum {
    mipc_nw_get_latest_ca_measurement_cnf_tlv_NONE = 0,
    /* The lte cc_measurement list */
    /* type = struct, refer to lte_cc_meas_info */
    MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_LTE_CC_MEAS_LIST = 0x100,
    /* The nr cc_measurement list */
    /* type = struct, refer to nr_cc_meas_info */
    MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_NR_CC_MEAS_LIST = 0x101,
    /* The lte cc_measurement list */
    /* type = struct, refer to lte_cc_meas_info_v1 */
    MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_LTE_CC_MEAS_TLV_ARRAY_V1 = 0x102,
    /* The nr cc_measurement list */
    /* type = struct, refer to nr_cc_meas_info_v1 */
    MIPC_NW_GET_LATEST_CA_MEASUREMENT_CNF_T_NR_CC_MEAS_TLV_ARRAY_V1 = 0x103,
};

    /* MIPC_MSG.NW_GET_CURRENT_CELL_INFO_REQ */
enum mipc_nw_get_current_cell_info_req_tlv_enum {
    mipc_nw_get_current_cell_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CURRENT_CELL_INFO_CNF */
enum mipc_nw_get_current_cell_info_cnf_tlv_enum {
    mipc_nw_get_current_cell_info_cnf_tlv_NONE = 0,
    /* The cell connect act */
    /* type = uint8_t */
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_CONNECT_ACT         = 0x100,
    /* The wcdma current cell infomation */
    /* type = struct, refer to wcdma_current_cell_info */
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_WCDMA_CELL_INFO     = 0x101,
    /* The lte ul current cell list */
    /* type = struct, refer to current_cell_info */
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_LTE_CURRENT_CELL_LIST = 0x102,
    /* The nr dl current cell list */
    /* type = struct, refer to current_cell_info */
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_NR_CURRENT_CELL_LIST = 0x103,
    /* The lte ul current cell list */
    /* type = struct, refer to current_cell_info_v1 */
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_LTE_CURRENT_CELL_TLV_ARRAY_V1 = 0x104,
    /* The nr dl current cell list */
    /* type = struct, refer to current_cell_info_v1 */
    MIPC_NW_GET_CURRENT_CELL_INFO_CNF_T_NR_CURRENT_CELL_TLV_ARRAY_V1 = 0x105,
};

    /* MIPC_MSG.NW_SET_CAG_SELECTION_REQ */
enum mipc_nw_set_cag_selection_req_tlv_enum {
    mipc_nw_set_cag_selection_req_tlv_NONE = 0,
    /* Determine whether the selection is done automatically by the MT or is done in manual selection mode on the CAG cell identified by <CAG_INFO> (which shall be given in format <FORMAT>) to a certain access technology, indicated in <ACT>; Default value is 0. */
    /* type = uint8_t, refer to NW_CAG_MODE */
    MIPC_NW_SET_CAG_SELECTION_REQ_T_MODE                    = 0x100,
    /* Indicate the format of <CAG_INFO>; Default value is 0. */
    /* type = uint8_t, refer to NW_CAG_INFO_FORMAT */
    MIPC_NW_SET_CAG_SELECTION_REQ_T_FORMAT                  = 0x101,
    /*
      <FORMAT> indicates if the format is alphanumeric or numeric
      <CAG_INFO> consists of HRNN, CAG ID and Associated PLMN MCC MNC, each delimited by a comma and in this particular order only.
      If HRNN is unavailable, it shall be an empty field.
      When selecting the CAG with <MODE> as 1, the CAG ID and Associated PLMN MCC MNC are mandatory while the HRNN is optional.
      In the alphanumeric format HRNN, CAG ID and Associated PLMN MCC MNC would be displayed while in numeric format only CAG ID and Associated PLMN MCC MNC would be displayed.
      Default value is null character.
    */
    /* type = string */
    MIPC_NW_SET_CAG_SELECTION_REQ_T_CAG_INFO                = 0x102,
    /* Access technology selected; Default value is 0. */
    /* type = uint8_t, refer to NW_ACT_TYPE */
    MIPC_NW_SET_CAG_SELECTION_REQ_T_ACT                     = 0x103,
};

    /* MIPC_MSG.NW_SET_CAG_SELECTION_CNF */
enum mipc_nw_set_cag_selection_cnf_tlv_enum {
    mipc_nw_set_cag_selection_cnf_tlv_NONE = 0,
    /* Access technology selected */
    /* type = uint8_t, refer to NW_ACT_TYPE */
    MIPC_NW_SET_CAG_SELECTION_CNF_T_ACT                     = 0x100,
};

    /* MIPC_MSG.NW_GET_CAG_SELECTION_REQ */
enum mipc_nw_get_cag_selection_req_tlv_enum {
    mipc_nw_get_cag_selection_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CAG_SELECTION_CNF */
enum mipc_nw_get_cag_selection_cnf_tlv_enum {
    mipc_nw_get_cag_selection_cnf_tlv_NONE = 0,
    /* Determine whether the selection is done automatically by the MT or is done in manual selection mode on the CAG cell identified by <CAG_INFO> (which shall be given in format <FORMAT>) to a certain access technology, indicated in <ACT>. */
    /* type = uint8_t, refer to NW_CAG_MODE */
    MIPC_NW_GET_CAG_SELECTION_CNF_T_MODE                    = 0x100,
    /* Indicate the format of <CAG_INFO>. */
    /* type = uint8_t, refer to NW_CAG_INFO_FORMAT */
    MIPC_NW_GET_CAG_SELECTION_CNF_T_FORMAT                  = 0x101,
    /*
      <FORMAT> indicates if the format is alphanumeric or numeric
      <CAG_INFO> consists of HRNN, CAG ID and Associated PLMN MCC MNC, each delimited by a comma and in this particular order only.
      If HRNN is unavailable, it shall be an empty field.
      When selecting the CAG with <MODE> as 1, the CAG ID and Associated PLMN MCC MNC are mandatory while the HRNN is optional.
      In the alphanumeric format HRNN, CAG ID and Associated PLMN MCC MNC would be displayed while in numeric format only CAG ID and Associated PLMN MCC MNC would be displayed.
    */
    /* type = string */
    MIPC_NW_GET_CAG_SELECTION_CNF_T_CAG_INFO                = 0x102,
    /* Access technology selected */
    /* type = uint8_t, refer to NW_ACT_TYPE */
    MIPC_NW_GET_CAG_SELECTION_CNF_T_ACT                     = 0x103,
};

    /* MIPC_MSG.NW_SET_AUTO_UPDATE_NITZ_REQ */
enum mipc_nw_set_auto_update_nitz_req_tlv_enum {
    mipc_nw_set_auto_update_nitz_req_tlv_NONE = 0,
    /* Whether automatic update time zone via NITZ is enable. Default is disable. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_AUTO_UPDATE_NITZ_REQ_T_ENABLE               = 0x100,
};

    /* MIPC_MSG.NW_SET_AUTO_UPDATE_NITZ_CNF */
enum mipc_nw_set_auto_update_nitz_cnf_tlv_enum {
    mipc_nw_set_auto_update_nitz_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_AUTO_UPDATE_NITZ_REQ */
enum mipc_nw_get_auto_update_nitz_req_tlv_enum {
    mipc_nw_get_auto_update_nitz_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_AUTO_UPDATE_NITZ_CNF */
enum mipc_nw_get_auto_update_nitz_cnf_tlv_enum {
    mipc_nw_get_auto_update_nitz_cnf_tlv_NONE = 0,
    /* Whether automatic update time zone via NITZ is enable. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_AUTO_UPDATE_NITZ_CNF_T_ENABLE               = 0x100,
};

    /* MIPC_MSG.NW_SET_NAS_CONNECTION_RELEASE_REQ */
enum mipc_nw_set_nas_connection_release_req_tlv_enum {
    mipc_nw_set_nas_connection_release_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_NAS_CONNECTION_RELEASE_CNF */
enum mipc_nw_set_nas_connection_release_cnf_tlv_enum {
    mipc_nw_set_nas_connection_release_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CAP_NRCA_OPTION_REQ */
enum mipc_nw_set_cap_nrca_option_req_tlv_enum {
    mipc_nw_set_cap_nrca_option_req_tlv_NONE = 0,
    /* Enable/Disable TDD+TDD carrier aggregation combination in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TT_SA_ENABLE          = 0x100,
    /* Enable/Disable TDD+FDD carrier aggregation combination in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TF_SA_ENABLE          = 0x101,
    /* Enable/Disable FDD+TDD carrier aggregation combination in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FT_SA_ENABLE          = 0x102,
    /* Enable/Disable FDD+FDD carrier aggregation combination in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FF_SA_ENABLE          = 0x103,
    /* Enable/Disable TDD+TDD carrier aggregation combination in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TT_NSA_ENABLE         = 0x104,
    /* Enable/Disable TDD+FDD carrier aggregation combination in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_TF_NSA_ENABLE         = 0x105,
    /* Enable/Disable FDD+TDD carrier aggregation combination in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FT_NSA_ENABLE         = 0x106,
    /* Enable/Disable FDD+FDD carrier aggregation combination in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_NRCA_OPTION_REQ_T_FF_NSA_ENABLE         = 0x107,
};

    /* MIPC_MSG.NW_SET_CAP_NRCA_OPTION_CNF */
enum mipc_nw_set_cap_nrca_option_cnf_tlv_enum {
    mipc_nw_set_cap_nrca_option_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CAP_BC_LIST_REQ */
enum mipc_nw_set_cap_bc_list_req_tlv_enum {
    mipc_nw_set_cap_bc_list_req_tlv_NONE = 0,
    /* Band Combination string of TDD+TDD carrier aggregation combinations in NR SA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_SA_SRC_TLV_ARRAY       = 0x100,
    /* Band Combination string of TDD+FDD carrier aggregation combinations in NR SA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_SA_SRC_TLV_ARRAY       = 0x101,
    /* Band Combination string of FDD+TDD carrier aggregation combinations in NR SA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_SA_SRC_TLV_ARRAY       = 0x102,
    /* Band Combination string of FDD+FDD carrier aggregation combinations in NR SA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_SA_SRC_TLV_ARRAY       = 0x103,
    /* Band Combination string of TDD+TDD carrier aggregation combinations in NR NSA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_NSA_SRC_TLV_ARRAY      = 0x104,
    /* Band Combination string of TDD+FDD carrier aggregation combinations in NR NSA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_NSA_SRC_TLV_ARRAY      = 0x105,
    /* Band Combination string of FDD+TDD carrier aggregation combinations in NR NSA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_NSA_SRC_TLV_ARRAY      = 0x106,
    /* Band Combination string of FDD+FDD carrier aggregation combinations in NR NSA */
    /* type = string */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_NSA_SRC_TLV_ARRAY      = 0x107,
    /* Option of TDD+TDD carrier aggregation combinations in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_SA_OPTION_TLV_ARRAY    = 0x108,
    /* Option of TDD+FDD carrier aggregation combinations in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_SA_OPTION_TLV_ARRAY    = 0x109,
    /* Option of FDD+TDD carrier aggregation combinations in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_SA_OPTION_TLV_ARRAY    = 0x10A,
    /* Option of FDD+FDD carrier aggregation combinations in NR SA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_SA_OPTION_TLV_ARRAY    = 0x10B,
    /* Option of TDD+TDD carrier aggregation combinations in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TT_NSA_OPTION_TLV_ARRAY   = 0x10C,
    /* Option of TDD+FDD carrier aggregation combinations in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_TF_NSA_OPTION_TLV_ARRAY   = 0x10D,
    /* Option of FDD+TDD carrier aggregation combinations in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FT_NSA_OPTION_TLV_ARRAY   = 0x10E,
    /* Option of FDD+FDD carrier aggregation combinations in NR NSA */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_FF_NSA_OPTION_TLV_ARRAY   = 0x10F,
    /* Indicate whether the Carrier Configuration XML parsing done */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CAP_BC_LIST_REQ_T_IS_XML_PARSING_DONE       = 0x110,
};

    /* MIPC_MSG.NW_SET_CAP_BC_LIST_CNF */
enum mipc_nw_set_cap_bc_list_cnf_tlv_enum {
    mipc_nw_set_cap_bc_list_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_CONGESTION_CFG_REQ */
enum mipc_nw_congestion_cfg_req_tlv_enum {
    mipc_nw_congestion_cfg_req_tlv_NONE = 0,
    /* The network congestion configuration info. */
    /* type = struct, refer to nw_congestion_info */
    MIPC_NW_CONGESTION_CFG_REQ_T_NW_CONGESTION_CFG          = 0x100,
    /* The network congestion configuration info V1. */
    /* type = struct, refer to nw_congestion_info_v1 */
    MIPC_NW_CONGESTION_CFG_REQ_T_NW_CONGESTION_CFG_V1       = 0x101,
    /*
      The list of network congestion configuration V1 information. User can use this value to set multiple nwtwork congestion configuration V1. If user set NW_CONGESTION_CFG_V1_TLV_ARRAY, modem will prioritize it instead of NW_CONGESTION_CFG_V1 and NW_CONGESTION_CFG.
    */
    /* type = struct, refer to nw_congestion_info_v1 */
    MIPC_NW_CONGESTION_CFG_REQ_T_NW_CONGESTION_CFG_V1_TLV_ARRAY = 0x102,
};

    /* MIPC_MSG.NW_CONGESTION_CFG_CNF */
enum mipc_nw_congestion_cfg_cnf_tlv_enum {
    mipc_nw_congestion_cfg_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ENWCFGINFO_REQ */
enum mipc_nw_get_enwcfginfo_req_tlv_enum {
    mipc_nw_get_enwcfginfo_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ENWCFGINFO_CNF */
enum mipc_nw_get_enwcfginfo_cnf_tlv_enum {
    mipc_nw_get_enwcfginfo_cnf_tlv_NONE = 0,
    /* Indicate 4X4MINO state: 0, inactive; 1, active */
    /* type = uint8_t */
    MIPC_NW_GET_ENWCFGINFO_CNF_T_4X4MINO_STATE              = 0x100,
    /* Indicate 256QAM state: 0, inactive; 1, active */
    /* type = uint8_t */
    MIPC_NW_GET_ENWCFGINFO_CNF_T_256QAM_STATE               = 0x101,
    /* Indicate 64QAM state: 0, inactive; 1, active */
    /* type = uint8_t */
    MIPC_NW_GET_ENWCFGINFO_CNF_T_64QAM_STATE                = 0x102,
};

    /* MIPC_MSG.NW_SET_BARRING_RLF_CONFIG_REQ */
enum mipc_nw_set_barring_rlf_config_req_tlv_enum {
    mipc_nw_set_barring_rlf_config_req_tlv_NONE = 0,
    /* Barring for all rlf threshold. */
    /* type = uint8_t */
    MIPC_NW_SET_BARRING_RLF_CONFIG_REQ_T_THRESHOLD          = 0x100,
    /* Barring for all rlf watch period, valus in sec. */
    /* type = uint16_t */
    MIPC_NW_SET_BARRING_RLF_CONFIG_REQ_T_WATCH_PERIOD       = 0x101,
    /* Barring for all rlf barring time, values in sec. */
    /* type = uint32_t */
    MIPC_NW_SET_BARRING_RLF_CONFIG_REQ_T_BARRING_TIME       = 0x102,
};

    /* MIPC_MSG.NW_SET_BARRING_RLF_CONFIG_CNF */
enum mipc_nw_set_barring_rlf_config_cnf_tlv_enum {
    mipc_nw_set_barring_rlf_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_RX_MIMO_CAP_REQ */
enum mipc_nw_set_rx_mimo_cap_req_tlv_enum {
    mipc_nw_set_rx_mimo_cap_req_tlv_NONE = 0,
    /* CTRL Operation */
    /* type = uint8_t, refer to RX_MIMO_CTRL_OPERATION */
    MIPC_NW_SET_RX_MIMO_CAP_REQ_T_CTRL_OPERATION            = 0x100,
    /* Release Level */
    /* type = uint8_t, refer to RX_MIMO_RELEASE_LEVEL */
    MIPC_NW_SET_RX_MIMO_CAP_REQ_T_RELEASE_LEVEL             = 0x101,
};

    /* MIPC_MSG.NW_SET_RX_MIMO_CAP_CNF */
enum mipc_nw_set_rx_mimo_cap_cnf_tlv_enum {
    mipc_nw_set_rx_mimo_cap_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_MIPI_REQ */
enum mipc_nw_mipi_req_tlv_enum {
    mipc_nw_mipi_req_tlv_NONE = 0,
    /* MIPI operation */
    /* type = uint8_t, refer to NW_MIPI_OP */
    MIPC_NW_MIPI_REQ_T_OP                                   = 0x100,
    /* MIPI port */
    /* type = uint8_t */
    MIPC_NW_MIPI_REQ_T_PORT                                 = 0x101,
    /*
      If the field 'ADDR' less than or equal to 0x1F, both NW_MIPI_RW_TYPE_MIPI_RW and NW_MIPI_RW_TYPE_MIPI_EXTRW_1BYTE are valid;
      If the field 'ADDR' greater than 0x1F, only NW_MIPI_RW_TYPE_MIPI_RW is valid;
    */
    /* type = uint8_t, refer to NW_MIPI_RW_TYPE */
    MIPC_NW_MIPI_REQ_T_RW_TYPE                              = 0x102,
    /* The valid range of this field is 0 to 0xF */
    /* type = uint8_t */
    MIPC_NW_MIPI_REQ_T_USID                                 = 0x103,
    /* MIPI codeword address */
    /* type = uint16_t */
    MIPC_NW_MIPI_REQ_T_ADDR                                 = 0x104,
    /* MIPI data; This field is mandatory when the field 'OP' is NW_MIPI_OP_WRITE */
    /* type = uint32_t */
    MIPC_NW_MIPI_REQ_T_DATA                                 = 0x105,
};

    /* MIPC_MSG.NW_MIPI_CNF */
enum mipc_nw_mipi_cnf_tlv_enum {
    mipc_nw_mipi_cnf_tlv_NONE = 0,
    /* This field would be carried when MIPC_T_RESULT is not MIPC_RESULT_SUCCESS */
    /* type = uint8_t, refer to NW_MIPI_CAUSE */
    MIPC_NW_MIPI_CNF_T_CAUSE                                = 0x100,
    /* MIPI operation */
    /* type = uint8_t, refer to NW_MIPI_OP */
    MIPC_NW_MIPI_CNF_T_OP                                   = 0x101,
    /* MIPI data; This field is mandatory when the field 'OP' is NW_MIPI_OP_READ */
    /* type = uint32_t */
    MIPC_NW_MIPI_CNF_T_DATA                                 = 0x102,
};

    /* MIPC_MSG.NW_BPI_REQ */
enum mipc_nw_bpi_req_tlv_enum {
    mipc_nw_bpi_req_tlv_NONE = 0,
    /* BPI operation */
    /* type = uint8_t, refer to NW_BPI_OP */
    MIPC_NW_BPI_REQ_T_OP                                    = 0x100,
    /* BPI data; This field is mandatory when the field 'OP' is NW_BPI_OP_WRITE */
    /* type = uint32_t */
    MIPC_NW_BPI_REQ_T_DATA                                  = 0x101,
};

    /* MIPC_MSG.NW_BPI_CNF */
enum mipc_nw_bpi_cnf_tlv_enum {
    mipc_nw_bpi_cnf_tlv_NONE = 0,
    /* BPI operation */
    /* type = uint8_t, refer to NW_BPI_OP */
    MIPC_NW_BPI_CNF_T_OP                                    = 0x100,
    /* BPI data; This field is mandatory when the field 'OP' is NW_BPI_OP_READ */
    /* type = uint32_t */
    MIPC_NW_BPI_CNF_T_DATA                                  = 0x101,
};

    /* MIPC_MSG.NW_SET_ENDC_DEACT_REQ */
enum mipc_nw_set_endc_deact_req_tlv_enum {
    mipc_nw_set_endc_deact_req_tlv_NONE = 0,
    /*
      0: disable
      1: enable
      2: Deactivate without send SCG failure
    */
    /* type = uint8_t, refer to NW_ENDC_DEACTIVATE_MODE */
    MIPC_NW_SET_ENDC_DEACT_REQ_T_DEACTIVATE_MODE            = 0x100,
    /*  */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_ENDC_DEACT_REQ_T_ALLOW_SCG_ADD              = 0x101,
    /*
      0: Do nothing
      1: Trigger to send NR A2 event
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_ENDC_DEACT_REQ_T_SEND_FAKE_A2_EVENT         = 0x102,
};

    /* MIPC_MSG.NW_SET_ENDC_DEACT_CNF */
enum mipc_nw_set_endc_deact_cnf_tlv_enum {
    mipc_nw_set_endc_deact_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ENDC_DEACT_REQ */
enum mipc_nw_get_endc_deact_req_tlv_enum {
    mipc_nw_get_endc_deact_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ENDC_DEACT_CNF */
enum mipc_nw_get_endc_deact_cnf_tlv_enum {
    mipc_nw_get_endc_deact_cnf_tlv_NONE = 0,
    /*
      0: disable
      1: enable
      2: Deactivate without send SCG failure
    */
    /* type = uint8_t, refer to NW_ENDC_DEACTIVATE_MODE */
    MIPC_NW_GET_ENDC_DEACT_CNF_T_DEACTIVATE_MODE            = 0x100,
    /*  */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_ENDC_DEACT_CNF_T_ALLOW_SCG_ADD              = 0x101,
    /*
      0: Do nothing
      1: Trigger to send NR A2 event
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_ENDC_DEACT_CNF_T_SEND_FAKE_A2_EVENT         = 0x102,
};

    /* MIPC_MSG.NW_SET_SA_SILENCE_REQ */
enum mipc_nw_set_sa_silence_req_tlv_enum {
    mipc_nw_set_sa_silence_req_tlv_NONE = 0,
    /*
      0: SA silence off
      1: SA silence on
      2: SA silence enable handover only
      3: SA silence reset only
    */
    /* type = uint8_t, refer to NW_SA_SLIENCE_MODE */
    MIPC_NW_SET_SA_SILENCE_REQ_T_SILENCE_MODE               = 0x100,
};

    /* MIPC_MSG.NW_SET_SA_SILENCE_CNF */
enum mipc_nw_set_sa_silence_cnf_tlv_enum {
    mipc_nw_set_sa_silence_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_SA_SILENCE_REQ */
enum mipc_nw_get_sa_silence_req_tlv_enum {
    mipc_nw_get_sa_silence_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_SA_SILENCE_CNF */
enum mipc_nw_get_sa_silence_cnf_tlv_enum {
    mipc_nw_get_sa_silence_cnf_tlv_NONE = 0,
    /*
      0: SA silence off
      1: SA silence on
      2: SA silence enable handover only
    */
    /* type = uint8_t, refer to NW_SA_SLIENCE_MODE */
    MIPC_NW_GET_SA_SILENCE_CNF_T_SILENCE_MODE               = 0x100,
    /*
      0: no blocking
      1: blocking
    */
    /* type = uint8_t, refer to NW_SA_BLOCKING_MODE */
    MIPC_NW_GET_SA_SILENCE_CNF_T_BLOCKING_MODE              = 0x101,
};

    /* MIPC_MSG.NW_SET_TX_POWER_REDUCTION_REQ */
enum mipc_nw_set_tx_power_reduction_req_tlv_enum {
    mipc_nw_set_tx_power_reduction_req_tlv_NONE = 0,
    /*
      0: LTE
      1: NR
      2: NR_FR1
      3: NR_FR2
      4: LTE_NR_FR1
      5: LTE_NR_FR2
      6: LTE_NR
      7: GSM
      8: WCDMA
      9: C2K
    */
    /* type = uint8_t, refer to NW_TX_RAT_MODE */
    MIPC_NW_SET_TX_POWER_REDUCTION_REQ_T_RAT                = 0x100,
    /* Units: dbm */
    /* type = uint8_t */
    MIPC_NW_SET_TX_POWER_REDUCTION_REQ_T_TXPWR_BACKOFF      = 0x101,
    /*
      0: MCG
      1: SCG
      2: ALL CG
    */
    /* type = uint8_t, refer to NW_CG_TYPE */
    MIPC_NW_SET_TX_POWER_REDUCTION_REQ_T_CG_TYPE            = 0x102,
};

    /* MIPC_MSG.NW_SET_TX_POWER_REDUCTION_CNF */
enum mipc_nw_set_tx_power_reduction_cnf_tlv_enum {
    mipc_nw_set_tx_power_reduction_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_TX_POWER_REDUCTION_REQ */
enum mipc_nw_get_tx_power_reduction_req_tlv_enum {
    mipc_nw_get_tx_power_reduction_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_TX_POWER_REDUCTION_CNF */
enum mipc_nw_get_tx_power_reduction_cnf_tlv_enum {
    mipc_nw_get_tx_power_reduction_cnf_tlv_NONE = 0,
    /*
      0: LTE
      1: NR
      2: NR_FR1
      3: NR_FR2
      4: LTE_NR_FR1
      5: LTE_NR_FR2
      6: LTE_NR
      7: GSM
      8: WCDMA
      9: C2K
    */
    /* type = uint8_t, refer to NW_TX_RAT_MODE */
    MIPC_NW_GET_TX_POWER_REDUCTION_CNF_T_RAT                = 0x100,
    /* Units: dbm */
    /* type = uint8_t */
    MIPC_NW_GET_TX_POWER_REDUCTION_CNF_T_TXPWR_BACKOFF      = 0x101,
    /*
      0: MCG
      1: SCG
      2: ALL CG
    */
    /* type = uint8_t, refer to NW_CG_TYPE */
    MIPC_NW_GET_TX_POWER_REDUCTION_CNF_T_CG_TYPE            = 0x102,
};

    /* MIPC_MSG.NW_SET_LTE_OVERHEATING_REQ */
enum mipc_nw_set_lte_overheating_req_tlv_enum {
    mipc_nw_set_lte_overheating_req_tlv_NONE = 0,
    /*
      0: leaving uai
      1: enter uai
    */
    /* type = uint8_t, refer to NW_UAI_OP */
    MIPC_NW_SET_LTE_OVERHEATING_REQ_T_OH_OP                 = 0x100,
    /* 0-19 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CAT_DL                = 0x101,
    /* 0-21 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CAT_UL                = 0x102,
    /* 0-31 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CC_DL                 = 0x103,
    /* 0-31 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_LTE_OVERHEATING_REQ_T_CC_UL                 = 0x104,
};

    /* MIPC_MSG.NW_SET_LTE_OVERHEATING_CNF */
enum mipc_nw_set_lte_overheating_cnf_tlv_enum {
    mipc_nw_set_lte_overheating_cnf_tlv_NONE = 0,
    /*
      0: None
      1: Not Supported
      2: T345 Pending
      3: Under LV Session
      4: No Need to Send
      5: Connected Not Allowed
    */
    /* type = uint8_t, refer to NW_SASE_CAUSE */
    MIPC_NW_SET_LTE_OVERHEATING_CNF_T_CAUSE                 = 0x100,
};

    /* MIPC_MSG.NW_GET_LTE_OVERHEATING_REQ */
enum mipc_nw_get_lte_overheating_req_tlv_enum {
    mipc_nw_get_lte_overheating_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_LTE_OVERHEATING_CNF */
enum mipc_nw_get_lte_overheating_cnf_tlv_enum {
    mipc_nw_get_lte_overheating_cnf_tlv_NONE = 0,
    /*
      0: leaving uai
      1: enter uai
    */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_LTE_OVERHEATING_CNF_T_OH_OP                 = 0x100,
    /* 0-19 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CAT_DL                = 0x101,
    /* 0-21 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CAT_UL                = 0x102,
    /* 0-31 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CC_DL                 = 0x103,
    /* 0-31 (reference 36.331 R14 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_LTE_OVERHEATING_CNF_T_CC_UL                 = 0x104,
    /*
      0: Fail
      1: Success
    */
    /* type = uint8_t, refer to NW_OVERHEATING_STATUS */
    MIPC_NW_GET_LTE_OVERHEATING_CNF_T_OH_STATUS             = 0x105,
};

    /* MIPC_MSG.NW_SET_NR_OVERHEATING_REQ */
enum mipc_nw_set_nr_overheating_req_tlv_enum {
    mipc_nw_set_nr_overheating_req_tlv_NONE = 0,
    /*
      0: leaving uai
      1: enter uai
    */
    /* type = uint8_t, refer to NW_UAI_OP */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_OH_OP                  = 0x100,
    /* 0-31 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_CC_DL                  = 0x101,
    /* 0-31 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_CC_UL                  = 0x102,
    /* 0-10 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR1_DL              = 0x103,
    /* 0-10 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR1_UL              = 0x104,
    /* 0-11 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR2_DL              = 0x105,
    /* 0-11 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_BW_FR2_UL              = 0x106,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_DL */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR1_DL            = 0x107,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_UL */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR1_UL            = 0x108,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_DL */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR2_DL            = 0x109,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_UL */
    MIPC_NW_SET_NR_OVERHEATING_REQ_T_MIMO_FR2_UL            = 0x10A,
};

    /* MIPC_MSG.NW_SET_NR_OVERHEATING_CNF */
enum mipc_nw_set_nr_overheating_cnf_tlv_enum {
    mipc_nw_set_nr_overheating_cnf_tlv_NONE = 0,
    /*
      0: None
      1: Not Supported
      2: T345 Pending
      3: Under LV Session
      4: No Need to Send
      5: Connected Not Allowed
    */
    /* type = uint8_t, refer to NW_SASE_CAUSE */
    MIPC_NW_SET_NR_OVERHEATING_CNF_T_CAUSE                  = 0x100,
};

    /* MIPC_MSG.NW_GET_NR_OVERHEATING_REQ */
enum mipc_nw_get_nr_overheating_req_tlv_enum {
    mipc_nw_get_nr_overheating_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NR_OVERHEATING_CNF */
enum mipc_nw_get_nr_overheating_cnf_tlv_enum {
    mipc_nw_get_nr_overheating_cnf_tlv_NONE = 0,
    /*
      0: leaving uai
      1: enter uai
    */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_OH_OP                  = 0x100,
    /* 0-31 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_CC_DL                  = 0x101,
    /* 0-31 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_CC_UL                  = 0x102,
    /* 0-10 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR1_DL              = 0x103,
    /* 0-10 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR1_UL              = 0x104,
    /* 0-11 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR2_DL              = 0x105,
    /* 0-11 (Reference 38.331 Spec) */
    /* type = uint8_t */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_BW_FR2_UL              = 0x106,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_DL */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR1_DL            = 0x107,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_UL */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR1_UL            = 0x108,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_DL */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR2_DL            = 0x109,
    /*
      0: MIMO_LAYERS_DL2 (Reference 38.331 Spec)
      1: MIMO_LAYERS_DL4
      2: MIMO_LAYERS_DL8
    */
    /* type = uint8_t, refer to NW_MIMO_FR_UL */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_MIMO_FR2_UL            = 0x10A,
    /*
      0: Fail
      1: Success
    */
    /* type = uint8_t, refer to NW_OVERHEATING_STATUS */
    MIPC_NW_GET_NR_OVERHEATING_CNF_T_OH_STATUS              = 0x10B,
};

    /* MIPC_MSG.NW_SET_UAI_POWER_SAVING_REQ */
enum mipc_nw_set_uai_power_saving_req_tlv_enum {
    mipc_nw_set_uai_power_saving_req_tlv_NONE = 0,
    /*
      0: MCG
      1: SCG
    */
    /* type = uint8_t, refer to NW_CG_TYPE */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_CG_TYPE              = 0x100,
    /*
      0: max_cc
      1: max_mimo
      2: drx
      3: max_bw
      4: min_sched
    */
    /* type = uint8_t, refer to NW_UAI_POWER_SAVING_TYPE */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_UAI_TYPE             = 0x101,
    /*
      0: leave
      1: enter
    */
    /* type = uint8_t, refer to NW_UAI_OP */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_OP                   = 0x102,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM1               = 0x103,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM2               = 0x104,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM3               = 0x105,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM4               = 0x106,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM5               = 0x107,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM6               = 0x108,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM7               = 0x109,
    /* Depend on the uai_type */
    /* type = uint8_t */
    MIPC_NW_SET_UAI_POWER_SAVING_REQ_T_PARAM8               = 0x10A,
};

    /* MIPC_MSG.NW_SET_UAI_POWER_SAVING_CNF */
enum mipc_nw_set_uai_power_saving_cnf_tlv_enum {
    mipc_nw_set_uai_power_saving_cnf_tlv_NONE = 0,
    /*
      0: None
      1: Not Supported
      2: T345 Pending
      3: Under LV Session
      4: No Need to Send
      5: Connected Not Allowed
    */
    /* type = uint8_t, refer to NW_SASE_CAUSE */
    MIPC_NW_SET_UAI_POWER_SAVING_CNF_T_CAUSE                = 0x100,
};

    /* MIPC_MSG.NW_GET_UAI_POWER_SAVING_REQ */
enum mipc_nw_get_uai_power_saving_req_tlv_enum {
    mipc_nw_get_uai_power_saving_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_UAI_POWER_SAVING_CNF */
enum mipc_nw_get_uai_power_saving_cnf_tlv_enum {
    mipc_nw_get_uai_power_saving_cnf_tlv_NONE = 0,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_MAX_CC           = 0x101,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_MAX_CC           = 0x102,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_MIMO             = 0x103,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_MIMO             = 0x104,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_DRX              = 0x105,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_DRX              = 0x106,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_BW               = 0x107,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_BW               = 0x108,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_MCG_MIN_SCHED        = 0x109,
    /* 0-4 */
    /* type = uint8_t, refer to NW_UAI_REQ_OP */
    MIPC_NW_GET_UAI_POWER_SAVING_CNF_T_SCG_MIN_SCHED        = 0x10A,
};

    /* MIPC_MSG.NW_SET_RRC_RELEASE_PREFERENCE_REQ */
enum mipc_nw_set_rrc_release_preference_req_tlv_enum {
    mipc_nw_set_rrc_release_preference_req_tlv_NONE = 0,
    /*
      0: UAI_RRC_STATE_RRC_IDLE 
      1: UAI_RRC_STATE_RRC_INACTIVE
      2: UAI_RRC_STATE_RRC_OUT_OF_CONNECTED
      3: UAI_RRC_STATE_RRC_CONNECTED
    */
    /* type = uint8_t, refer to NW_RRC_RELEASE_OP */
    MIPC_NW_SET_RRC_RELEASE_PREFERENCE_REQ_T_RELEASE_OP     = 0x100,
};

    /* MIPC_MSG.NW_SET_RRC_RELEASE_PREFERENCE_CNF */
enum mipc_nw_set_rrc_release_preference_cnf_tlv_enum {
    mipc_nw_set_rrc_release_preference_cnf_tlv_NONE = 0,
    /*
      0: None
      1: Not Supported
      2: T345 Pending
      3: Under LV Session
      4: No Need to Send
      5: Connected Not Allowed
    */
    /* type = uint8_t, refer to NW_SASE_CAUSE */
    MIPC_NW_SET_RRC_RELEASE_PREFERENCE_CNF_T_CAUSE          = 0x100,
};

    /* MIPC_MSG.NW_GET_RRC_RELEASE_PREFERENCE_REQ */
enum mipc_nw_get_rrc_release_preference_req_tlv_enum {
    mipc_nw_get_rrc_release_preference_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_RRC_RELEASE_PREFERENCE_CNF */
enum mipc_nw_get_rrc_release_preference_cnf_tlv_enum {
    mipc_nw_get_rrc_release_preference_cnf_tlv_NONE = 0,
    /*
      0: UAI_RRC_STATE_RRC_IDLE 
      1: UAI_RRC_STATE_RRC_INACTIVE
      2: UAI_RRC_STATE_RRC_OUT_OF_CONNECTED
      3: UAI_RRC_STATE_RRC_CONNECTED
    */
    /* type = uint8_t, refer to NW_RRC_RELEASE_OP */
    MIPC_NW_GET_RRC_RELEASE_PREFERENCE_CNF_T_RELEASE_STATUS = 0x100,
};

    /* MIPC_MSG.NW_SET_FAKE_RI_REQ */
enum mipc_nw_set_fake_ri_req_tlv_enum {
    mipc_nw_set_fake_ri_req_tlv_NONE = 0,
    /*
      0: disable
      1: enable
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_FAKE_RI_REQ_T_ENABLE                        = 0x100,
    /*
      0: FR1
      1: FR2
      0xFF: invalid
      When the RAT is LTE, user should carry 0xFF in this field.
    */
    /* type = uint8_t, refer to NW_FAKE_RI_BAND */
    MIPC_NW_SET_FAKE_RI_REQ_T_BAND                          = 0x101,
    /*
      0: MCG
      1: SCG
      0xFF: invalid
      When the RAT is LTE, user should carry 0xFF in this field.
    */
    /* type = uint8_t, refer to NW_CG_TYPE */
    MIPC_NW_SET_FAKE_RI_REQ_T_CG_TYPE                       = 0x102,
    /*
      0: rank1
      1: rank2
      2: rank4
    */
    /* type = uint8_t, refer to NW_FAKE_RI_CTRL_RANK */
    MIPC_NW_SET_FAKE_RI_REQ_T_CTRL_RANK                     = 0x103,
    /*
      1: LTE
      2: NR
      Default is 2 (NR)
    */
    /* type = uint8_t, refer to NW_FAKE_RI_RAT */
    MIPC_NW_SET_FAKE_RI_REQ_T_RAT                           = 0x104,
};

    /* MIPC_MSG.NW_SET_FAKE_RI_CNF */
enum mipc_nw_set_fake_ri_cnf_tlv_enum {
    mipc_nw_set_fake_ri_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_FAKE_RI_REQ */
enum mipc_nw_get_fake_ri_req_tlv_enum {
    mipc_nw_get_fake_ri_req_tlv_NONE = 0,
    /*
      1: LTE
      2: NR
      Default is 2 (NR)
    */
    /* type = uint8_t, refer to NW_FAKE_RI_RAT */
    MIPC_NW_GET_FAKE_RI_REQ_T_RAT                           = 0x100,
};

    /* MIPC_MSG.NW_GET_FAKE_RI_CNF */
enum mipc_nw_get_fake_ri_cnf_tlv_enum {
    mipc_nw_get_fake_ri_cnf_tlv_NONE = 0,
    /*
      0: disable
      1: enable
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_FAKE_RI_CNF_T_ENABLE                        = 0x100,
    /*
      0: FR1
      1: FR2
      0xFF: invalid
      When the RAT is LTE, modem would carry 0xFF in this field.
    */
    /* type = uint8_t, refer to NW_FAKE_RI_BAND */
    MIPC_NW_GET_FAKE_RI_CNF_T_BAND                          = 0x101,
    /*
      0: MCG
      1: SCG
      0xFF: invalid
      When the RAT is LTE, modem would carry 0xFF in this field.
    */
    /* type = uint8_t, refer to NW_CG_TYPE */
    MIPC_NW_GET_FAKE_RI_CNF_T_CG_TYPE                       = 0x102,
    /*
      0: rank1
      1: rank2
      2: rank4
    */
    /* type = uint8_t, refer to NW_FAKE_RI_CTRL_RANK */
    MIPC_NW_GET_FAKE_RI_CNF_T_CTRL_RANK                     = 0x103,
};

    /* MIPC_MSG.NW_SET_MR_THRESH_BOUND_REQ */
enum mipc_nw_set_mr_thresh_bound_req_tlv_enum {
    mipc_nw_set_mr_thresh_bound_req_tlv_NONE = 0,
    /*
      Config info
    */
    /* type = struct, refer to lte_bound_info */
    MIPC_NW_SET_MR_THRESH_BOUND_REQ_T_CONF_INFO             = 0x100,
};

    /* MIPC_MSG.NW_SET_MR_THRESH_BOUND_CNF */
enum mipc_nw_set_mr_thresh_bound_cnf_tlv_enum {
    mipc_nw_set_mr_thresh_bound_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_DELETE_FTAI_ENTRY_REQ */
enum mipc_nw_delete_ftai_entry_req_tlv_enum {
    mipc_nw_delete_ftai_entry_req_tlv_NONE = 0,
    /* The integrity attributes of the deleted FTAI; Default value is FTAI_ATTR_NONE; */
    /* type = uint8_t, refer to NW_FTAI_ATTR_ALL_WITH_TYPE */
    MIPC_NW_DELETE_FTAI_ENTRY_REQ_T_FTAI_DEL_ATTR           = 0x100,
    /* The TAI list of the interface */
    /* type = struct, refer to ftai_info */
    MIPC_NW_DELETE_FTAI_ENTRY_REQ_T_TAI_LIST                = 0x101,
};

    /* MIPC_MSG.NW_DELETE_FTAI_ENTRY_CNF */
enum mipc_nw_delete_ftai_entry_cnf_tlv_enum {
    mipc_nw_delete_ftai_entry_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_NO_SERVICE_TIMER_REQ */
enum mipc_nw_set_no_service_timer_req_tlv_enum {
    mipc_nw_set_no_service_timer_req_tlv_NONE = 0,
    /* The no service timer of the interface set, uint of second. */
    /* type = uint16_t */
    MIPC_NW_SET_NO_SERVICE_TIMER_REQ_T_NO_SERVICE_TIMER     = 0x100,
};

    /* MIPC_MSG.NW_SET_NO_SERVICE_TIMER_CNF */
enum mipc_nw_set_no_service_timer_cnf_tlv_enum {
    mipc_nw_set_no_service_timer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NO_SERVICE_TIMER_REQ */
enum mipc_nw_get_no_service_timer_req_tlv_enum {
    mipc_nw_get_no_service_timer_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_NO_SERVICE_TIMER_CNF */
enum mipc_nw_get_no_service_timer_cnf_tlv_enum {
    mipc_nw_get_no_service_timer_cnf_tlv_NONE = 0,
    /* The no service timer of the interface set, uint of second. */
    /* type = uint16_t */
    MIPC_NW_GET_NO_SERVICE_TIMER_CNF_T_NO_SERVICE_TIMER     = 0x100,
};

    /* MIPC_MSG.NW_SET_DYNAMIC_ANT_BIAS_REQ */
enum mipc_nw_set_dynamic_ant_bias_req_tlv_enum {
    mipc_nw_set_dynamic_ant_bias_req_tlv_NONE = 0,
    /* Scenario index. */
    /* type = uint8_t */
    MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ_T_SCEN_IDX             = 0x100,
    /* Sim index. */
    /* type = uint8_t */
    MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ_T_SIM_IDX              = 0x101,
    /* rat bitmap. */
    /* type = uint32_t, refer to NW_BIT_RAT */
    MIPC_NW_SET_DYNAMIC_ANT_BIAS_REQ_T_BIT_RAT              = 0x102,
};

    /* MIPC_MSG.NW_SET_DYNAMIC_ANT_BIAS_CNF */
enum mipc_nw_set_dynamic_ant_bias_cnf_tlv_enum {
    mipc_nw_set_dynamic_ant_bias_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_ABSOLUTE_MAX_TX_POWER_REQ */
enum mipc_nw_set_absolute_max_tx_power_req_tlv_enum {
    mipc_nw_set_absolute_max_tx_power_req_tlv_NONE = 0,
    /*
      4: LTE , 7: NR
    */
    /* type = uint8_t */
    MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_REQ_T_RAT             = 0x100,
    /*
      0: clear NVRAM; 1: write NVRAM
    */
    /* type = uint8_t */
    MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_REQ_T_NV_WRITE        = 0x101,
    /*
      The band and power pair list(the power value unit is 1/8 dBm, range is 0~29*8)
    */
    /* type = struct, refer to nw_band_power_pair */
    MIPC_NW_SET_ABSOLUTE_MAX_TX_POWER_REQ_T_BAND_POWER_TLV_ARRAY = 0x102,
};

    /* MIPC_MSG.NW_SET_ABSOLUTE_MAX_TX_POWER_CNF */
enum mipc_nw_set_absolute_max_tx_power_cnf_tlv_enum {
    mipc_nw_set_absolute_max_tx_power_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CSCON_STATE_REQ */
enum mipc_nw_get_cscon_state_req_tlv_enum {
    mipc_nw_get_cscon_state_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_CSCON_STATE_CNF */
enum mipc_nw_get_cscon_state_cnf_tlv_enum {
    mipc_nw_get_cscon_state_cnf_tlv_NONE = 0,
    /*
      CSCON state
    */
    /* type = struct, refer to nw_cscon_status */
    MIPC_NW_GET_CSCON_STATE_CNF_T_CSCON_STATE               = 0x100,
};

    /* MIPC_MSG.NW_SET_DA_PREFER_CUSTOM_THRESH_REQ */
enum mipc_nw_set_da_prefer_custom_thresh_req_tlv_enum {
    mipc_nw_set_da_prefer_custom_thresh_req_tlv_NONE = 0,
    /*
      The thresh for RSRP
    */
    /* type = int16_t */
    MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ_T_RSRP_THRESH   = 0x100,
    /*
      The thresh for RSRQ
    */
    /* type = int16_t */
    MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ_T_RSRQ_THRESH   = 0x101,
    /*
      4: LTE , 7: NR
    */
    /* type = uint8_t */
    MIPC_NW_SET_DA_PREFER_CUSTOM_THRESH_REQ_T_RAT           = 0x102,
};

    /* MIPC_MSG.NW_SET_DA_PREFER_CUSTOM_THRESH_CNF */
enum mipc_nw_set_da_prefer_custom_thresh_cnf_tlv_enum {
    mipc_nw_set_da_prefer_custom_thresh_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_MAX_EIRP_REQ */
enum mipc_nw_set_max_eirp_req_tlv_enum {
    mipc_nw_set_max_eirp_req_tlv_NONE = 0,
    /*
      4: LTE , 7: NR
    */
    /* type = uint8_t */
    MIPC_NW_SET_MAX_EIRP_REQ_T_RAT                          = 0x100,
    /*
      0: clear NVRAM; 1: write NVRAM
    */
    /* type = uint8_t */
    MIPC_NW_SET_MAX_EIRP_REQ_T_NV_WRITE                     = 0x101,
    /*
      The band and power pair list(the power value unit is 1/8 dBm/MHz, range is 0~29*8)
    */
    /* type = struct, refer to nw_band_power_pair */
    MIPC_NW_SET_MAX_EIRP_REQ_T_BAND_POWER_TLV_ARRAY         = 0x102,
};

    /* MIPC_MSG.NW_SET_MAX_EIRP_CNF */
enum mipc_nw_set_max_eirp_cnf_tlv_enum {
    mipc_nw_set_max_eirp_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_CLEAR_NSSAI_REQ */
enum mipc_nw_clear_nssai_req_tlv_enum {
    mipc_nw_clear_nssai_req_tlv_NONE = 0,
    /*
      Clear NSSAI type;0-clear all, 1- clear preferred 3gpp , 2- clear preferred non3gpp, 3- clear preferred NSSAI all, 4- clear default NSSAI
    */
    /* type = uint8_t, refer to NW_CLEAR_NSSAI_TYPE */
    MIPC_NW_CLEAR_NSSAI_REQ_T_CLEAR_NSSAI_OPTION            = 0x100,
};

    /* MIPC_MSG.NW_CLEAR_NSSAI_CNF */
enum mipc_nw_clear_nssai_cnf_tlv_enum {
    mipc_nw_clear_nssai_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_LOCAL_REL_WITH_REDIR_TO_LTE_REQ */
enum mipc_nw_local_rel_with_redir_to_lte_req_tlv_enum {
    mipc_nw_local_rel_with_redir_to_lte_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_LOCAL_REL_WITH_REDIR_TO_LTE_CNF */
enum mipc_nw_local_rel_with_redir_to_lte_cnf_tlv_enum {
    mipc_nw_local_rel_with_redir_to_lte_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ */
enum mipc_nw_trigger_network_service_scan_req_tlv_enum {
    mipc_nw_trigger_network_service_scan_req_tlv_NONE = 0,
    /* The action of service scan. User should always send ACTION is STOP when the service scan finishes. */
    /* type = uint8_t, refer to NW_SCAN_ACTION */
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_ACTION       = 0x100,
    /* Network servcice type */
    /* type = uint8_t, refer to NW_SERVICE_SCAN_TYPE */
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_SERVICE_TYPE = 0x101,
    /* Available RAT for service scan, should not be RAT_NONE. See NW_SCAN_RAT for all available RAT bitmap. This field is mandatory when ACTION is START and SERVICE_TYPE is NORMAL. */
    /* type = bitmap, refer to NW_SCAN_RAT */
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_AVAILABLE_RAT = 0x102,
    /* Prefer RAT for service scan. If no prefer RAT, NO_PREFER must be set. See NW_PREFER_RAT for all prefer RAT bitmap. This field is mandatory when ACTION is START and SERVICE_TYPE is NORMAL. */
    /* type = bitmap, refer to NW_PREFER_RAT */
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_PREFER_RAT   = 0x103,
    /* If ACTION is START, the default value is true which means protocol will retry the scan if lower layer is busy temporary. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_TRIGGER_NETWORK_SERVICE_SCAN_REQ_T_IS_NEED_RETRY = 0x104,
};

    /* MIPC_MSG.NW_TRIGGER_NETWORK_SERVICE_SCAN_CNF */
enum mipc_nw_trigger_network_service_scan_cnf_tlv_enum {
    mipc_nw_trigger_network_service_scan_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_SET_CONFIG_REQ */
enum mipc_nw_set_config_req_tlv_enum {
    mipc_nw_set_config_req_tlv_NONE = 0,
    /*
      This TLV sets whether user wants to be notified about real-time transparency information has happened in over-the-air plaintext IMSI,IMEI and unciphered SUCI disclosures which appears before NAS security context is established.
      This setting is stored persistently across reboot and default value is disable (FALSE).
      The control point must process TLV CONFIGURED_CELLULAR_ID_DISCLOSURE to see if the configuration is successful or not.
      If the control point has configured this to TRUE (enable), the control point should also register and process MIPC_NW_CELLULAR_ID_DISCLOSURE_IND to learn about the cellular ID disclosure information.
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CONFIG_REQ_T_CELLULAR_ID_DISCLOSURE         = 0x100,
};

    /* MIPC_MSG.NW_SET_CONFIG_CNF */
enum mipc_nw_set_config_cnf_tlv_enum {
    mipc_nw_set_config_cnf_tlv_NONE = 0,
    /* The result of the feature CELLULAR_ID_DISCLOSURE setting in modem. */
    /* type = uint8_t, refer to NW_CONFIG_RESULT */
    MIPC_NW_SET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE_RESULT  = 0x100,
    /* The actual configured CELLULAR_ID_DISCLOSURE setting in modem resulting from TLV CELLULAR_ID_DISCLOSURE in REQ. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_CONFIG_CNF_T_CONFIGURED_CELLULAR_ID_DISCLOSURE = 0x101,
};

    /* MIPC_MSG.NW_GET_CONFIG_REQ */
enum mipc_nw_get_config_req_tlv_enum {
    mipc_nw_get_config_req_tlv_NONE = 0,
    /*
      Retrieve CELLULAR_ID_DISCLOSURE configuration or not
    */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_CONFIG_REQ_T_RETRIEVE_CELLULAR_ID_DISCLOSURE = 0x100,
};

    /* MIPC_MSG.NW_GET_CONFIG_CNF */
enum mipc_nw_get_config_cnf_tlv_enum {
    mipc_nw_get_config_cnf_tlv_NONE = 0,
    /* The result of retrieving CELLULAR_ID_DISCLOSURE setting in modem. */
    /* type = uint8_t, refer to NW_CONFIG_RESULT */
    MIPC_NW_GET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE_RESULT  = 0x100,
    /* The cellular ID disclosure configuration is enabled or not. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_CONFIG_CNF_T_CELLULAR_ID_DISCLOSURE         = 0x101,
};

    /* MIPC_MSG.NW_REGISTER_IND */
enum mipc_nw_register_ind_tlv_enum {
    mipc_nw_register_ind_tlv_NONE = 0,
    /* [Phase out] Please use STATE_V1. */
    /* type = struct, refer to nw_reg_state */
    MIPC_NW_REGISTER_IND_T_STATE                            = 0x100,
    /* Network error enum, refer to 3GPP 24.008 Annex G. */
    /* type = uint16_t */
    MIPC_NW_REGISTER_IND_T_NW_ERR                           = 0x101,
    /* The mode of registration */
    /* type = uint8_t, refer to NW_REGISTER_MODE */
    MIPC_NW_REGISTER_IND_T_MODE                             = 0x102,
    /* This represents the supported data speed for current radio technologies */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_REGISTER_IND_T_DATA_SPEED                       = 0x103,
    /* Provider short name */
    /* type = string */
    MIPC_NW_REGISTER_IND_T_NW_NAME                          = 0x104,
    /* Roaming text information */
    /* type = string */
    MIPC_NW_REGISTER_IND_T_ROAMING_TEXT                     = 0x105,
    /* The cause of failure */
    /* type = uint16_t */
    MIPC_NW_REGISTER_IND_T_FAIL_CAUSE                       = 0x106,
    /* Provider long name */
    /* type = string */
    MIPC_NW_REGISTER_IND_T_NW_LONG_NAME                     = 0x107,
    /* The information of registration */
    /* type = struct, refer to nw_reg_info */
    MIPC_NW_REGISTER_IND_T_PS_INFO                          = 0x108,
    /* type = struct, refer to nw_reg_state_v1 */
    MIPC_NW_REGISTER_IND_T_STATE_V1                         = 0x109,
    /* Indicates the encoding scheme for TLV PLMN_SHORT_NAME */
    /* type = uint8_t, refer to ENCODING_SCHEME */
    MIPC_NW_REGISTER_IND_T_SHORT_NAME_ENCODING_SCHEME       = 0x10A,
    /* PLMN short name */
    /* type = byte_array */
    MIPC_NW_REGISTER_IND_T_PLMN_SHORT_NAME                  = 0x10B,
    /* Indicates the encoding scheme for TLV PLMN_LONG_NAME */
    /* type = uint8_t, refer to ENCODING_SCHEME */
    MIPC_NW_REGISTER_IND_T_LONG_NAME_ENCODING_SCHEME        = 0x10C,
    /* PLMN long name */
    /* type = byte_array */
    MIPC_NW_REGISTER_IND_T_PLMN_LONG_NAME                   = 0x10D,
    /* Indicates PLMN name source */
    /* type = uint8_t, refer to PLMN_NAME_SOURCE */
    MIPC_NW_REGISTER_IND_T_PLMN_NAME_SOURCE                 = 0x10E,
};

    /* MIPC_MSG.NW_SIGNAL_IND */
enum mipc_nw_signal_ind_tlv_enum {
    mipc_nw_signal_ind_tlv_NONE = 0,
    /* RSSI for current signal status */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_RSSI                               = 0x100,
    /* Error rate for current signal status */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_ERR_RATE                           = 0x101,
    /* RSRP for current signal status */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_RSRP                               = 0x102,
    /* SNR for current signal status */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_SNR                                = 0x103,
    /*
      Current interval setting for signal reporting; the unit is seconds
      The default value is 5 seconds
    */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_SIGNAL_STRENGTH_INTERVAL           = 0x104,
    /* Current RSSI threshold setting for signal reporting, the default value is 5 */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_RSSI_THRESHOLD                     = 0x105,
    /* Current error rate threshold setting for signal reporting, the default value is 0 */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_ERR_RATE_THRESHOLD                 = 0x106,
    /* Current RSRP threshold setting for signal reporting, the default value is 2 db */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_RSRP_THRESHOLD                     = 0x107,
    /* Current SNR threshold setting for signal reporting, the default value is 0 db */
    /* type = uint32_t */
    MIPC_NW_SIGNAL_IND_T_SNR_THRESHOLD                      = 0x108,
    /* Signal type; if the type is NSA, it should contain LTE and(or) NR signal info */
    /* type = uint8_t, refer to NW_SIGNAL_TYPE */
    MIPC_NW_SIGNAL_IND_T_SIGNAL_TYPE                        = 0x109,
    /* GSM signal strength */
    /* type = struct, refer to nw_gsm_signal_strength */
    MIPC_NW_SIGNAL_IND_T_GSM_SIGNAL                         = 0x10A,
    /* UMTS signal strength */
    /* type = struct, refer to nw_umts_signal_strength */
    MIPC_NW_SIGNAL_IND_T_UMTS_SIGNAL                        = 0x10B,
    /* [Phase out] Please use LTE_SIGNAL_V2. */
    /* type = struct, refer to nw_lte_signal_strength */
    MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL                         = 0x10C,
    /* NR signal strength */
    /* type = struct, refer to nw_nr_signal_strength */
    MIPC_NW_SIGNAL_IND_T_NR_SIGNAL                          = 0x10D,
    /* [Phase out] Please use RAW_SIGNAL_INFO_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_raw_signal_info */
    MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_LIST               = 0x10E,
    /* The raw signal info count; if the network registered in NSA, the count is 2; otherwise, the count is 1 */
    /* type = uint8_t */
    MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_COUNT              = 0x10F,
    /* CDMA signal strength */
    /* type = struct, refer to nw_cdma_signal_strength */
    MIPC_NW_SIGNAL_IND_T_CDMA_SIGNAL                        = 0x110,
    /* [Phase out] Please use LTE_SIGNAL_V2. */
    /* type = struct, refer to nw_lte_signal_strength_v1 */
    MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL_V1                      = 0x111,
    /* [Phase out] Please use NR_SIGNAL_V3. */
    /* type = struct, refer to nw_nr_signal_strength_v1 */
    MIPC_NW_SIGNAL_IND_T_NR_SIGNAL_V1                       = 0x112,
    /* type = struct, refer to nw_nr_signal_strength_v2 */
    MIPC_NW_SIGNAL_IND_T_NR_SIGNAL_V2                       = 0x113,
    /* [Phase out] Please use RAW_SIGNAL_INFO_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_raw_signal_info */
    MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_TLV_ARRAY          = 0x114,
    /* NR signal strength include CQI and TA info. */
    /* type = struct, refer to nw_nr_signal_strength_v3 */
    MIPC_NW_SIGNAL_IND_T_NR_SIGNAL_V3                       = 0x115,
    /* LTE signal strength */
    /* type = struct, refer to nw_lte_signal_strength_v2 */
    MIPC_NW_SIGNAL_IND_T_LTE_SIGNAL_V2                      = 0x116,
    /* The raw signal data information; All RATs share this struct */
    /* type = struct, refer to nw_raw_signal_info_v1 */
    MIPC_NW_SIGNAL_IND_T_RAW_SIGNAL_INFO_V1_TLV_ARRAY       = 0x117,
};

    /* MIPC_MSG.NW_PS_IND */
enum mipc_nw_ps_ind_tlv_enum {
    mipc_nw_ps_ind_tlv_NONE = 0,
    /* It indicates PS state is detach or attach */
    /* type = uint8_t, refer to NW_PS */
    MIPC_NW_PS_IND_T_TACH                                   = 0x100,
    /* This represents the supported data speed for current radio technologies */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_PS_IND_T_DATA_SPEED                             = 0x101,
    /* Only valid for 5G */
    /* type = uint8_t */
    MIPC_NW_PS_IND_T_NW_FREQUENCY                           = 0x102,
    /* [Phase out] Please use REG_INFO_V1. */
    /* type = struct, refer to nw_ps_reg_info */
    MIPC_NW_PS_IND_T_REG_INFO                               = 0x103,
    /* The cell type */
    /* type = uint8_t, refer to NW_CELL_TYPE */
    MIPC_NW_PS_IND_T_CELL_TYPE                              = 0x104,
    /* The cell info: mipc_nw_gsm_cell_struct4 and so on. Note: Acquire cell info according to the cell type */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_PS_IND_T_CELL_INFO                              = 0x8105,
    /* The GSM cell info: mipc_nw_gsm_cell_struct4 */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_PS_IND_T_GSM_CELL_INFO                          = 0x8106,
    /* The UMTS cell info: mipc_nw_umts_cell_struct4 */
    /* type = struct, refer to nw_umts_cell */
    MIPC_NW_PS_IND_T_UMTS_CELL_INFO                         = 0x8107,
    /* The LTE cell info: mipc_nw_lte_cell_struct4 */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_PS_IND_T_LTE_CELL_INFO                          = 0x8108,
    /* [Phase out] Please use NR_CELL_INFO_V2. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_PS_IND_T_NR_CELL_INFO                           = 0x8109,
    /* [Phase out] Please use CDMA_CELL_INFO_V1 */
    /* type = struct, refer to nw_cdma_cell */
    MIPC_NW_PS_IND_T_CDMA_CELL_INFO                         = 0x810A,
    /* The PS data registration state */
    /* type = struct, refer to nw_ps_reg_info_v1 */
    MIPC_NW_PS_IND_T_REG_INFO_V1                            = 0x10C,
    /* NR cell information */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_PS_IND_T_NR_CELL_INFO_V2                        = 0x810E,
    /* C2K cell information */
    /* type = struct, refer to nw_cdma_cell_v1 */
    MIPC_NW_PS_IND_T_CDMA_CELL_INFO_V1                      = 0x810F,
};

    /* MIPC_MSG.NW_RADIO_IND */
enum mipc_nw_radio_ind_tlv_enum {
    mipc_nw_radio_ind_tlv_NONE = 0,
    /*
      SW radio status;
      0: Off;
      1: On
    */
    /* type = uint8_t */
    MIPC_NW_RADIO_IND_T_SW_STATE                            = 0x100,
    /* HW radio status */
    /* type = uint8_t */
    MIPC_NW_RADIO_IND_T_HW_STATE                            = 0x101,
};

    /* MIPC_MSG.NW_IA_IND */
enum mipc_nw_ia_ind_tlv_enum {
    mipc_nw_ia_ind_tlv_NONE = 0,
    /* Access Point Name */
    /* type = string */
    MIPC_NW_IA_IND_T_APN                                    = 0x100,
    /*
      RAT information:
      0: None;
      4: LTE
    */
    /* type = uint8_t */
    MIPC_NW_IA_IND_T_RAT                                    = 0x101,
    /* The IP address type */
    /* type = uint8_t, refer to APN_PDP_TYPE */
    MIPC_NW_IA_IND_T_PDP_TYPE                               = 0x102,
    /* Authentication type */
    /* type = uint8_t, refer to APN_AUTH_TYPE */
    MIPC_NW_IA_IND_T_AUTH_TYPE                              = 0x103,
    /* Authentication username */
    /* type = string */
    MIPC_NW_IA_IND_T_USERID                                 = 0x8104,
    /* Authentication password */
    /* type = string */
    MIPC_NW_IA_IND_T_PASSWORD                               = 0x8105,
    /* It indicates APNs with the same APN name but different parameters */
    /* type = uint32_t */
    MIPC_NW_IA_IND_T_APN_INDEX                              = 0x106,
    /* The cause of attach reject; refer ps_public_enum.h */
    /* type = uint32_t */
    MIPC_NW_IA_IND_T_CAUSE                                  = 0x107,
};

    /* MIPC_MSG.NW_NITZ_IND */
enum mipc_nw_nitz_ind_tlv_enum {
    mipc_nw_nitz_ind_tlv_NONE = 0,
    /* type = struct, refer to nw_nitz_info */
    MIPC_NW_NITZ_IND_T_INFO                                 = 0x100,
    /* The type of NITZ info */
    /* type = uint8_t, refer to NITZ_IND_TYPE */
    MIPC_NW_NITZ_IND_T_TYPE                                 = 0x101,
};

    /* MIPC_MSG.NW_LOCATION_INFO_IND */
enum mipc_nw_location_info_ind_tlv_enum {
    mipc_nw_location_info_ind_tlv_NONE = 0,
    /* type = struct, refer to nw_location_info */
    MIPC_NW_LOCATION_INFO_IND_T_INFO                        = 0x8100,
};

    /* MIPC_MSG.NW_CS_IND */
enum mipc_nw_cs_ind_tlv_enum {
    mipc_nw_cs_ind_tlv_NONE = 0,
    /* [Phase out] Please use INFO_V1. */
    /* type = struct, refer to nw_reg_change_info */
    MIPC_NW_CS_IND_T_INFO                                   = 0x8100,
    /* The cell type. */
    /* type = uint8_t, refer to NW_CELL_TYPE */
    MIPC_NW_CS_IND_T_CELL_TYPE                              = 0x101,
    /* The cell info: mipc_nw_gsm_cell_struct4 and so on. NOTE: according to the cell type to get the cell info. */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_CS_IND_T_CELL_INFO                              = 0x8102,
    /* The gsm cell info: mipc_nw_gsm_cell_struct4. */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_CS_IND_T_GSM_CELL_INFO                          = 0x8103,
    /* The umts cell info: mipc_nw_umts_cell_struct4. */
    /* type = struct, refer to nw_umts_cell */
    MIPC_NW_CS_IND_T_UMTS_CELL_INFO                         = 0x8104,
    /* The lte cell info: mipc_nw_lte_cell_struct4. */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_CS_IND_T_LTE_CELL_INFO                          = 0x8105,
    /* [Phase out] Please use NR_CELL_INFO_V2. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_CS_IND_T_NR_CELL_INFO                           = 0x8106,
    /* [Phase out] Please use CDMA_CELL_INFO_V1 */
    /* type = struct, refer to nw_cdma_cell */
    MIPC_NW_CS_IND_T_CDMA_CELL_INFO                         = 0x8107,
    /* type = struct, refer to nw_reg_change_info_v1 */
    MIPC_NW_CS_IND_T_INFO_V1                                = 0x8108,
    /* NR cell information */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_CS_IND_T_NR_CELL_INFO_V2                        = 0x8109,
    /* C2K cell information */
    /* type = struct, refer to nw_cdma_cell_v1 */
    MIPC_NW_CS_IND_T_CDMA_CELL_INFO_V1                      = 0x810A,
};

    /* MIPC_MSG.NW_CSCON_IND */
enum mipc_nw_cscon_ind_tlv_enum {
    mipc_nw_cscon_ind_tlv_NONE = 0,
    /* type = struct, refer to nw_cscon_status */
    MIPC_NW_CSCON_IND_T_STATUS                              = 0x100,
};

    /* MIPC_MSG.NW_PREFERRED_PROVIDER_IND */
enum mipc_nw_preferred_provider_ind_tlv_enum {
    mipc_nw_preferred_provider_ind_tlv_NONE = 0,
    /* The provider list length */
    /* type = uint8_t */
    MIPC_NW_PREFERRED_PROVIDER_IND_T_PROVIDER_COUNT         = 0x100,
    /*  */
    /* type = struct_array, refer to nw_provider */
    MIPC_NW_PREFERRED_PROVIDER_IND_T_PROVIDER_LIST          = 0x101,
    /*  */
    /* type = struct, refer to nw_provider */
    MIPC_NW_PREFERRED_PROVIDER_IND_T_NW_PROVIDER_TLV_ARRAY  = 0x102,
};

    /* MIPC_MSG.NW_CAINFO_IND */
enum mipc_nw_cainfo_ind_tlv_enum {
    mipc_nw_cainfo_ind_tlv_NONE = 0,
    /* num of dl lte serving cell, the max of it is 32 */
    /* type = uint8_t */
    MIPC_NW_CAINFO_IND_T_LTE_DL_SERVING_CELL_COUNT          = 0x100,
    /* num of ul lte serving cell, the max of it is 32 */
    /* type = uint8_t */
    MIPC_NW_CAINFO_IND_T_LTE_UL_SERVING_CELL_COUNT          = 0x101,
    /* num of dl nr serving cell, the max of it is 32 */
    /* type = uint8_t */
    MIPC_NW_CAINFO_IND_T_NR_DL_SERVING_CELL_COUNT           = 0x102,
    /* num of ul nr serving cell, the max of it is 32 */
    /* type = uint8_t */
    MIPC_NW_CAINFO_IND_T_NR_UL_SERVING_CELL_COUNT           = 0x103,
    /* DL LTE serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_LTE_DL_SERVING_CELL_LIST           = 0x104,
    /* UL LTE serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_LTE_UL_SERVING_CELL_LIST           = 0x105,
    /* DL NR serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_NR_DL_SERVING_CELL_LIST            = 0x106,
    /* UL NR serving cell list */
    /* type = struct_array, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_NR_UL_SERVING_CELL_LIST            = 0x107,
    /* DL LTE serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_LTE_DL_SERVING_CELL_TLV_ARRAY      = 0x108,
    /* UL LTE serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_LTE_UL_SERVING_CELL_TLV_ARRAY      = 0x109,
    /* DL NR serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_NR_DL_SERVING_CELL_TLV_ARRAY       = 0x10A,
    /* UL NR serving cell list */
    /* type = struct, refer to nw_lte_nr_ca_info */
    MIPC_NW_CAINFO_IND_T_NR_UL_SERVING_CELL_TLV_ARRAY       = 0x10B,
};

    /* MIPC_MSG.NW_EONS_IND */
enum mipc_nw_eons_ind_tlv_enum {
    mipc_nw_eons_ind_tlv_NONE = 0,
    /* It indicates whether PLMN network name service is enabled */
    /* type = uint32_t */
    MIPC_NW_EONS_IND_T_PNN                                  = 0x100,
    /* It indicates whether operator name list service is enabled */
    /* type = uint32_t */
    MIPC_NW_EONS_IND_T_OPL                                  = 0x101,
};

    /* MIPC_MSG.NW_CIEV_IND */
enum mipc_nw_ciev_ind_tlv_enum {
    mipc_nw_ciev_ind_tlv_NONE = 0,
    /* ciev urc type. */
    /* type = uint32_t */
    MIPC_NW_CIEV_IND_T_CIEV_TYPE                            = 0x100,
    /* the ecbm status value of ciev 102 */
    /* type = uint32_t */
    MIPC_NW_CIEV_IND_T_ECBM_STATUS                          = 0x101,
    /* plmn of operator NW of ciev 10 */
    /* type = string */
    MIPC_NW_CIEV_IND_T_PLMN_ID                              = 0x102,
    /* provider name long of ciev 10 */
    /* type = string */
    MIPC_NW_CIEV_IND_T_NW_NAME_LONG                         = 0x103,
    /* provider name short of ciev 10 */
    /* type = string */
    MIPC_NW_CIEV_IND_T_NW_NAME_SHORT                        = 0x104,
    /* PRL ID of ciev 131 */
    /* type = uint32_t */
    MIPC_NW_CIEV_IND_T_PRL_ID                               = 0x105,
};

    /* MIPC_MSG.NW_EGMSS_IND */
enum mipc_nw_egmss_ind_tlv_enum {
    mipc_nw_egmss_ind_tlv_NONE = 0,
    /* rat. */
    /* type = uint32_t */
    MIPC_NW_EGMSS_IND_T_RAT                                 = 0x100,
    /* mcc */
    /* type = uint32_t */
    MIPC_NW_EGMSS_IND_T_MCC                                 = 0x101,
    /* attach status: 1 attached to the network on the rat; 0 have selected the rat and is going to perform PLMN search. */
    /* type = uint32_t */
    MIPC_NW_EGMSS_IND_T_STATUS                              = 0x102,
    /* current reported rat */
    /* type = uint32_t */
    MIPC_NW_EGMSS_IND_T_CUR_REPORTED_RAT                    = 0x103,
    /* is home country or not, 0: not home contury, 1: home country */
    /* type = uint32_t */
    MIPC_NW_EGMSS_IND_T_IS_HOME_COUNTRY                     = 0x104,
};

    /* MIPC_MSG.NW_PSBEARER_IND */
enum mipc_nw_psbearer_ind_tlv_enum {
    mipc_nw_psbearer_ind_tlv_NONE = 0,
    /* Cell data speed supported */
    /* type = uint32_t, refer to NW_DATA_SPEED */
    MIPC_NW_PSBEARER_IND_T_CELL_DATA_SPEED_SUPPORT          = 0x100,
    /* Maximum data bearer capability */
    /* type = uint32_t, refer to NW_DATA_BEARER_CAPABILITY */
    MIPC_NW_PSBEARER_IND_T_MAX_DATA_BEARER_CAPABILITY       = 0x101,
    /* The number of secondary cells used for downlink */
    /* type = uint32_t */
    MIPC_NW_PSBEARER_IND_T_SEC_CELL_NUM_IN_DL               = 0x102,
    /* The number of secondary cells used for uplink */
    /* type = uint32_t */
    MIPC_NW_PSBEARER_IND_T_SEC_CELL_NUM_IN_UL               = 0x103,
};

    /* MIPC_MSG.NW_ECELL_IND */
enum mipc_nw_ecell_ind_tlv_enum {
    mipc_nw_ecell_ind_tlv_NONE = 0,
    /* The GSM cell list length */
    /* type = uint32_t */
    MIPC_NW_ECELL_IND_T_GSM_CELL_COUNT                      = 0x100,
    /*  */
    /* type = struct_array, refer to nw_gsm_cell */
    MIPC_NW_ECELL_IND_T_GSM_CELL_LIST                       = 0x101,
    /* The UTMS cell list length */
    /* type = uint32_t */
    MIPC_NW_ECELL_IND_T_UMTS_CELL_COUNT                     = 0x102,
    /*  */
    /* type = struct_array, refer to nw_umts_cell */
    MIPC_NW_ECELL_IND_T_UMTS_CELL_LIST                      = 0x103,
    /* The LTE cell list length */
    /* type = uint32_t */
    MIPC_NW_ECELL_IND_T_LTE_CELL_COUNT                      = 0x106,
    /*  */
    /* type = struct_array, refer to nw_lte_cell */
    MIPC_NW_ECELL_IND_T_LTE_CELL_LIST                       = 0x107,
    /* The CDMA cell list length */
    /* type = uint32_t */
    MIPC_NW_ECELL_IND_T_CDMA_CELL_COUNT                     = 0x108,
    /* [Phase out] Please use CDMA_CELL_INFO_V1 */
    /* type = struct_array, refer to nw_cdma_cell */
    MIPC_NW_ECELL_IND_T_CDMA_CELL_LIST                      = 0x109,
    /* The NR cell list length */
    /* type = uint32_t */
    MIPC_NW_ECELL_IND_T_NR_CELL_COUNT                       = 0x10A,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY instead of NR_CELL_COUNT and NR_CELL_LIST. */
    /* type = struct_array, refer to nw_nr_cell */
    MIPC_NW_ECELL_IND_T_NR_CELL_LIST                        = 0x10B,
    /* The cause of failure for current operation */
    /* type = uint16_t */
    MIPC_NW_ECELL_IND_T_FAIL_CAUSE                          = 0x10C,
    /* [Phase out] Please use LTE_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_lte_cell_v1 */
    MIPC_NW_ECELL_IND_T_LTE_CELL_LIST_V1                    = 0x10D,
    MIPC_NW_ECELL_IND_T_LTE_CELL_TLV_ARRAY_V1               = 0x10D,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_nr_cell_v1 */
    MIPC_NW_ECELL_IND_T_NR_CELL_LIST_V1                     = 0x10E,
    MIPC_NW_ECELL_IND_T_NR_CELL_TLV_ARRAY_V1                = 0x10E,
    /* [Phase out] Please use LTE_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_lte_cell */
    MIPC_NW_ECELL_IND_T_LTE_CELL_TLV_ARRAY                  = 0x10F,
    /* [Phase out] Please use NR_CELL_V2_TLV_ARRAY. */
    /* type = struct, refer to nw_nr_cell */
    MIPC_NW_ECELL_IND_T_NR_CELL_TLV_ARRAY                   = 0x110,
    /*  */
    /* type = struct, refer to nw_gsm_cell */
    MIPC_NW_ECELL_IND_T_GSM_CELL_TLV_ARRAY                  = 0x111,
    /*  */
    /* type = struct, refer to nw_umts_cell */
    MIPC_NW_ECELL_IND_T_UMTS_CELL_TLV_ARRAY                 = 0x112,
    /* [Phase out] Please use CDMA_CELL_V1_TLV_ARRAY */
    /* type = struct, refer to nw_cdma_cell */
    MIPC_NW_ECELL_IND_T_CDMA_CELL_TLV_ARRAY                 = 0x113,
    /* NR cell information */
    /* type = struct, refer to nw_nr_cell_v2 */
    MIPC_NW_ECELL_IND_T_NR_CELL_V2_TLV_ARRAY                = 0x114,
    /* LTE cell information */
    /* type = struct, refer to nw_lte_cell_v2 */
    MIPC_NW_ECELL_IND_T_LTE_CELL_V2_TLV_ARRAY               = 0x115,
    /* C2K cell information */
    /* type = struct, refer to nw_cdma_cell_v1 */
    MIPC_NW_ECELL_IND_T_CDMA_CELL_V1_TLV_ARRAY              = 0x116,
};

    /* MIPC_MSG.NW_ANBR_IND */
enum mipc_nw_anbr_ind_tlv_enum {
    mipc_nw_anbr_ind_tlv_NONE = 0,
    /* type = struct, refer to nw_anbr_info */
    MIPC_NW_ANBR_IND_T_INFO                                 = 0x100,
};

    /* MIPC_MSG.NW_IRAT_IND */
enum mipc_nw_irat_ind_tlv_enum {
    mipc_nw_irat_ind_tlv_NONE = 0,
    /* type = struct, refer to nw_irat_info */
    MIPC_NW_IRAT_IND_T_INFO                                 = 0x100,
};

    /* MIPC_MSG.NW_EREGINFO_IND */
enum mipc_nw_ereginfo_ind_tlv_enum {
    mipc_nw_ereginfo_ind_tlv_NONE = 0,
    /*
      It indicates the access technology selected;
      7: LTE
    */
    /* type = uint8_t */
    MIPC_NW_EREGINFO_IND_T_ACT                              = 0x100,
    /* 0: Successful completion of Routing Area Update procedure; 1: Successful completion of Tracking Area Update procedure */
    /* type = uint8_t */
    MIPC_NW_EREGINFO_IND_T_EVENT_TYPE                       = 0x101,
};

    /* MIPC_MSG.NW_EMODCFG_IND */
enum mipc_nw_emodcfg_ind_tlv_enum {
    mipc_nw_emodcfg_ind_tlv_NONE = 0,
    /* It indicates the modulation scheme of the active RAT */
    /* type = uint8_t, refer to NW_SIGNAL_MODULATION */
    MIPC_NW_EMODCFG_IND_T_MODULATION                        = 0x100,
};

    /* MIPC_MSG.NW_EPCELLINFO_IND */
enum mipc_nw_epcellinfo_ind_tlv_enum {
    mipc_nw_epcellinfo_ind_tlv_NONE = 0,
    /* on lte network info, value 1-44. */
    /* type = uint8_t */
    MIPC_NW_EPCELLINFO_IND_T_LTE_BAND                       = 0x100,
};

    /* MIPC_MSG.NW_PSEUDO_CELL_IND */
enum mipc_nw_pseudo_cell_ind_tlv_enum {
    mipc_nw_pseudo_cell_ind_tlv_NONE = 0,
    /* num of pseudo cell num,the value is 1 or 2 */
    /* type = uint8_t */
    MIPC_NW_PSEUDO_CELL_IND_T_CELL_COUNT                    = 0x100,
    /* [Phase out] Please use PSEUDOCELL_V1_TLV_ARRAY instead of CELL_COUNT and PSEUDOCELL_LIST. */
    /* type = struct_array, refer to nw_pseudocell_info */
    MIPC_NW_PSEUDO_CELL_IND_T_PSEUDOCELL_LIST               = 0x101,
    /* [Phase out] Please use PSEUDOCELL_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_pseudocell_info */
    MIPC_NW_PSEUDO_CELL_IND_T_PSEUDOCELL_TLV_ARRAY          = 0x102,
    /* pseudo cell list of urc */
    /* type = struct, refer to nw_pseudocell_info_v1 */
    MIPC_NW_PSEUDO_CELL_IND_T_PSEUDOCELL_V1_TLV_ARRAY       = 0x103,
};

    /* MIPC_MSG.NW_NETWORK_INFO_IND */
enum mipc_nw_network_info_ind_tlv_enum {
    mipc_nw_network_info_ind_tlv_NONE = 0,
    /* Type of network info */
    /* type = uint16_t */
    MIPC_NW_NETWORK_INFO_IND_T_TYPE                         = 0x100,
    /* network info of the urc */
    /* type = string */
    MIPC_NW_NETWORK_INFO_IND_T_NW_INFO                      = 0x101,
};

    /* MIPC_MSG.NW_MCCMNC_IND */
enum mipc_nw_mccmnc_ind_tlv_enum {
    mipc_nw_mccmnc_ind_tlv_NONE = 0,
    /* The PLMN ID of the network provider */
    /* type = string */
    MIPC_NW_MCCMNC_IND_T_PLMN_ID                            = 0x100,
};

    /* MIPC_MSG.NW_PHYSICAL_CHANNEL_CONFIGS_IND */
enum mipc_nw_physical_channel_configs_ind_tlv_enum {
    mipc_nw_physical_channel_configs_ind_tlv_NONE = 0,
    /* The count for physical ch info list. */
    /* type = uint8_t */
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST_COUNT = 0x100,
    /* [Phase out] Please use PHYSICAL_CH_INFO_V2_TLV_ARRAY instead of PHYSICAL_CH_INFO_LIST_COUNT and PHYSICAL_CH_INFO_LIST. */
    /* type = struct_array, refer to physical_channel_info */
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST = 0x101,
    /* [Phase out] Please use PHYSICAL_CH_INFO_V2_TLV_ARRAY. */
    /* type = struct, refer to physical_channel_info_v1 */
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_LIST_V1 = 0x102,
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_TLV_ARRAY_V1 = 0x102,
    /* [Phase out] Please use PHYSICAL_CH_INFO_V2_TLV_ARRAY. */
    /* type = struct, refer to physical_channel_info */
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_TLV_ARRAY = 0x103,
    /* The physical channel information. */
    /* type = struct, refer to physical_channel_info_v2 */
    MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND_T_PHYSICAL_CH_INFO_V2_TLV_ARRAY = 0x104,
};

    /* MIPC_MSG.NW_OTACMSG_IND */
enum mipc_nw_otacmsg_ind_tlv_enum {
    mipc_nw_otacmsg_ind_tlv_NONE = 0,
    /* value 1-14. */
    /* type = uint8_t */
    MIPC_NW_OTACMSG_IND_T_OTA_STATE                         = 0x100,
};

    /* MIPC_MSG.NW_BARRING_INFO_IND */
enum mipc_nw_barring_info_ind_tlv_enum {
    mipc_nw_barring_info_ind_tlv_NONE = 0,
    /* number of barring. */
    /* type = uint8_t */
    MIPC_NW_BARRING_INFO_IND_T_COUNT                        = 0x100,
    /* [Phase out] Please use BARRING_V1_TLV_ARRAY. */
    /* type = struct_array, refer to nw_barring_info */
    MIPC_NW_BARRING_INFO_IND_T_BARRING_LIST                 = 0x101,
    /* rat, 1:GSM 2:UMTS 4:LTE 128:NR */
    /* type = uint8_t */
    MIPC_NW_BARRING_INFO_IND_T_RAT                          = 0x102,
    /* [Phase out] Please use BARRING_V1_TLV_ARRAY. */
    /* type = struct, refer to nw_barring_info */
    MIPC_NW_BARRING_INFO_IND_T_BARRING_TLV_ARRAY            = 0x103,
    /* The barring info LIST of the barring service. */
    /* type = struct, refer to nw_barring_info_v1 */
    MIPC_NW_BARRING_INFO_IND_T_BARRING_V1_TLV_ARRAY         = 0x104,
};

    /* MIPC_MSG.NW_RADIO_CAPABILITY_IND */
enum mipc_nw_radio_capability_ind_tlv_enum {
    mipc_nw_radio_capability_ind_tlv_NONE = 0,
    /* bit0: GSM, bit1: UMTS, bit2: LTE, bit3: NR, bit4: C2K */
    /* type = uint32_t */
    MIPC_NW_RADIO_CAPABILITY_IND_T_RADIO_CAPABILITY         = 0x100,
};

    /* MIPC_MSG.NW_CURRENT_RAT_IND */
enum mipc_nw_current_rat_ind_tlv_enum {
    mipc_nw_current_rat_ind_tlv_NONE = 0,
    /* The RAT information by bit */
    /* type = bitmap, refer to NW_BIT_RAT */
    MIPC_NW_CURRENT_RAT_IND_T_CURRENT_BIT_RAT               = 0x100,
    /* RAT information by the value */
    /* type = uint8_t, refer to NW_RAT_MODE */
    MIPC_NW_CURRENT_RAT_IND_T_CURRENT_RAT                   = 0x101,
    /* The preferred RAT */
    /* type = uint8_t */
    MIPC_NW_CURRENT_RAT_IND_T_PREFER_RAT                    = 0x102,
};

    /* MIPC_MSG.NW_CAMP_STATE_IND */
enum mipc_nw_camp_state_ind_tlv_enum {
    mipc_nw_camp_state_ind_tlv_NONE = 0,
    /* [Phase out] Please use REG_INFO_V1. */
    /* type = struct, refer to nw_ps_reg_info */
    MIPC_NW_CAMP_STATE_IND_T_REG_INFO                       = 0x100,
    /* PLMN ID (e.g., 46001) */
    /* type = string */
    MIPC_NW_CAMP_STATE_IND_T_PLMN_ID                        = 0x101,
    /* The provider short name (e.g. CMCC) */
    /* type = string */
    MIPC_NW_CAMP_STATE_IND_T_NW_NAME                        = 0x102,
    /* The PS data registration state */
    /* type = struct, refer to nw_ps_reg_info_v1 */
    MIPC_NW_CAMP_STATE_IND_T_REG_INFO_V1                    = 0x103,
};

    /* MIPC_MSG.NW_NR_SWITCH_IND */
enum mipc_nw_nr_switch_ind_tlv_enum {
    mipc_nw_nr_switch_ind_tlv_NONE = 0,
    /* The sim that has nr capability */
    /* type = uint8_t */
    MIPC_NW_NR_SWITCH_IND_T_NR_SIM                          = 0x100,
};

    /* MIPC_MSG.NW_FEMTOCELL_INFO_IND */
enum mipc_nw_femtocell_info_ind_tlv_enum {
    mipc_nw_femtocell_info_ind_tlv_NONE = 0,
    /* Femtocell type; 1: CSG type; 2: CDMA type; 3: LTE type */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_TYPE                       = 0x100,
    /* 1XRTT type */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_IS_1X_FEMTOCELL            = 0x101,
    /* EVDO type */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_IS_EVDO_FEMTOCELL          = 0x102,
    /* This field is valid when TYPE is 3; 2: LTE femtocell; 0: not LTE femtocell */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_IS_FEMTOCELL               = 0x103,
    /* 0: CS; 1: PS. */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_DOMAIN                     = 0x104,
    /* Network register status */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_STATE                      = 0x105,
    /* Radio access technology */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_ACT                        = 0x106,
    /* 0: Current cell is not CSG cell; 1: Current cell is csg cell */
    /* type = uint8_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_IS_CSG_CELL                = 0x107,
    /* CSG ID */
    /* type = uint32_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_CSG_ID                     = 0x108,
    /* 0: CSG type is not allowed; 1: CSG type is allowed; 2: Operator; 3: Unauthorized operator; 0xff: Invalid */
    /* type = uint16_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_CSG_ICON_TYPE              = 0x109,
    /* The reject cause of registered CSG */
    /* type = uint16_t */
    MIPC_NW_FEMTOCELL_INFO_IND_T_CAUSE                      = 0x10A,
    /* PLMN ID */
    /* type = string */
    MIPC_NW_FEMTOCELL_INFO_IND_T_PLMN_ID                    = 0x10B,
    /* Operator name */
    /* type = string */
    MIPC_NW_FEMTOCELL_INFO_IND_T_OPER_NAME                  = 0x10C,
    /* HNB name in Hex decimal format */
    /* type = string */
    MIPC_NW_FEMTOCELL_INFO_IND_T_HNBNAME                    = 0x10D,
};

    /* MIPC_MSG.NW_ETXPWR_IND */
enum mipc_nw_etxpwr_ind_tlv_enum {
    mipc_nw_etxpwr_ind_tlv_NONE = 0,
    /* 2: UTRAN, 7:E-UTRAN. */
    /* type = uint32_t */
    MIPC_NW_ETXPWR_IND_T_ACT                                = 0x100,
    /* indicate TX power value in 1/8 dBm. */
    /* type = int32_t */
    MIPC_NW_ETXPWR_IND_T_TX_POWER                           = 0x101,
};

    /* MIPC_MSG.NW_ETXPWRSTUS_IND */
enum mipc_nw_etxpwrstus_ind_tlv_enum {
    mipc_nw_etxpwrstus_ind_tlv_NONE = 0,
    /* indicates the TX power Event, 0: TX power low indication, 1:TX power high indication, 0xff:default value. */
    /* type = uint16_t */
    MIPC_NW_ETXPWRSTUS_IND_T_EVENT                          = 0x100,
    /* Indicates the SAR scenario index */
    /* type = int16_t */
    MIPC_NW_ETXPWRSTUS_IND_T_SAR_SCENARIO_INDEX             = 0x101,
};

    /* MIPC_MSG.NW_IWLAN_IND */
enum mipc_nw_iwlan_ind_tlv_enum {
    mipc_nw_iwlan_ind_tlv_NONE = 0,
    /* IWLAN Status. */
    /* type = uint32_t, refer to NW_IWLAN_STATUS */
    MIPC_NW_IWLAN_IND_T_STATUS                              = 0x100,
};

    /* MIPC_MSG.NW_CH_INFO_IND */
enum mipc_nw_ch_info_ind_tlv_enum {
    mipc_nw_ch_info_ind_tlv_NONE = 0,
    /* rat value */
    /* type = int32_t, refer to NW_CH_INFO_RAT */
    MIPC_NW_CH_INFO_IND_T_RAT                               = 0x100,
    /* current camped band */
    /* type = int32_t */
    MIPC_NW_CH_INFO_IND_T_BAND                              = 0x101,
    /* current camped channel */
    /* type = int32_t */
    MIPC_NW_CH_INFO_IND_T_CHANNEL                           = 0x102,
    /*  */
    /* type = int32_t */
    MIPC_NW_CH_INFO_IND_T_IS_ENDC                           = 0x103,
};

    /* MIPC_MSG.NW_NRUW_INFO_IND */
enum mipc_nw_nruw_info_ind_tlv_enum {
    mipc_nw_nruw_info_ind_tlv_NONE = 0,
    /* 0: don't display 5G UW, 1: display 5G UW. */
    /* type = uint8_t */
    MIPC_NW_NRUW_INFO_IND_T_DISPLAY_5GUW                    = 0x100,
    /* 0: not on n77 band, 1: on n77 band */
    /* type = uint8_t */
    MIPC_NW_NRUW_INFO_IND_T_ON_N77_BAND                     = 0x101,
    /* 0: not on fr2 band, 1: on fr2 band */
    /* type = uint8_t */
    MIPC_NW_NRUW_INFO_IND_T_ON_FR2_BAND                     = 0x102,
    /* 0: not allowed, 1: allowed */
    /* type = uint8_t */
    MIPC_NW_NRUW_INFO_IND_T_5GUW_ALLOWED                    = 0x103,
};

    /* MIPC_MSG.NW_NR_CA_BAND_IND */
enum mipc_nw_nr_ca_band_ind_tlv_enum {
    mipc_nw_nr_ca_band_ind_tlv_NONE = 0,
    /*  */
    /* type = int32_t */
    MIPC_NW_NR_CA_BAND_IND_T_IS_ENDC                        = 0x100,
    /* The num of nr ca band */
    /* type = uint8_t */
    MIPC_NW_NR_CA_BAND_IND_T_BAND_NUM                       = 0x101,
    /* The nr ca band */
    /* type = struct, refer to nr_ca_band */
    MIPC_NW_NR_CA_BAND_IND_T_BAND                           = 0x102,
};

    /* MIPC_MSG.NW_NR_SCS_IND */
enum mipc_nw_nr_scs_ind_tlv_enum {
    mipc_nw_nr_scs_ind_tlv_NONE = 0,
    /* nr5g subcarrier spacing in khz */
    /* type = int32_t */
    MIPC_NW_NR_SCS_IND_T_SCS                                = 0x100,
};

    /* MIPC_MSG.NW_NRUC_INFO_IND */
enum mipc_nw_nruc_info_ind_tlv_enum {
    mipc_nw_nruc_info_ind_tlv_NONE = 0,
    /* 0: not display 5G UC, 1: display 5G UC */
    /* type = uint32_t, refer to NW_5GUC_STATE */
    MIPC_NW_NRUC_INFO_IND_T_DISPLAY_5GUC                    = 0x100,
    /* 0: not on UC band(ex: n41), 1: on UC band(ex: n41) */
    /* type = uint32_t, refer to NW_UC_BAND */
    MIPC_NW_NRUC_INFO_IND_T_ON_UC_BAND                      = 0x101,
    /* 0~300 MHz */
    /* type = uint32_t */
    MIPC_NW_NRUC_INFO_IND_T_AGG_BW                          = 0x102,
};

    /* MIPC_MSG.NW_FIRST_PLMN_IND */
enum mipc_nw_first_plmn_ind_tlv_enum {
    mipc_nw_first_plmn_ind_tlv_NONE = 0,
    /* mcc */
    /* type = string */
    MIPC_NW_FIRST_PLMN_IND_T_MCC                            = 0x100,
    /* mnc */
    /* type = string */
    MIPC_NW_FIRST_PLMN_IND_T_MNC                            = 0x101,
};

    /* MIPC_MSG.NW_N3_REG_STATE_IND */
enum mipc_nw_n3_reg_state_ind_tlv_enum {
    mipc_nw_n3_reg_state_ind_tlv_NONE = 0,
    /* N3 register state */
    /* type = uint32_t, refer to N3_REG_STATE */
    MIPC_NW_N3_REG_STATE_IND_T_REG_STATE                    = 0x100,
    /* N3 gateway type by bit. bit0: N3IWF(5G Untrusted Non-3GPP), 0 disable, 1 enable. bit1: TNGF(5G Trusted Non-3GPP), 0 disable, 1 enable. bit2: WAGF(5G Wireline), 0 disable, 1: enable */
    /* type = uint32_t */
    MIPC_NW_N3_REG_STATE_IND_T_GW_TYPE                      = 0x101,
    /* N3 device type */
    /* type = uint32_t, refer to N3_DEVICE_TYPE */
    MIPC_NW_N3_REG_STATE_IND_T_DEVICE_TYPE                  = 0x102,
    /* N3 cause */
    /* type = uint32_t */
    MIPC_NW_N3_REG_STATE_IND_T_CAUSE                        = 0x103,
};

    /* MIPC_MSG.NW_EDRX_IND */
enum mipc_nw_edrx_ind_tlv_enum {
    mipc_nw_edrx_ind_tlv_NONE = 0,
    /* Indicates the type of access technology */
    /* type = uint8_t, refer to NW_EDRX_ACCESS_TECHNOLOGY_ENUM */
    MIPC_NW_EDRX_IND_T_ACT                                  = 0x100,
    /* The eDRX value requested */
    /* type = uint8_t */
    MIPC_NW_EDRX_IND_T_REQUESTED_EDRX_VALUE                 = 0x101,
    /* The eDRX value provided by NW */
    /* type = uint8_t */
    MIPC_NW_EDRX_IND_T_NW_PROVIDED_EDRX_VALUE               = 0x102,
    /* The value of the paging time window */
    /* type = uint8_t */
    MIPC_NW_EDRX_IND_T_PAGING_TIME_WINDOW                   = 0x103,
};

    /* MIPC_MSG.NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND */
enum mipc_nw_mobile_initiated_connection_only_mode_ind_tlv_enum {
    mipc_nw_mobile_initiated_connection_only_mode_ind_tlv_NONE = 0,
    /* The current MICO mode. */
    /* type = uint8_t */
    MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_CURRENT_MICO_MODE = 0x100,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the registration area allocation indication. */
    /* type = uint8_t */
    MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_RAAI_VALUE = 0x101,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the use of the Strictly Periodic Registration Timer. */
    /* type = uint8_t */
    MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_SPRT_VALUE = 0x102,
    /* Only present when CURRENT_MICO_MODE = 1. Indicates the Active Time value(T3324) assigned by the network. */
    /* type = uint8_t */
    MIPC_NW_MOBILE_INITIATED_CONNECTION_ONLY_MODE_IND_T_ALLOCATED_ACTIVE_TIME = 0x103,
};

    /* MIPC_MSG.NW_ENWREJAC_IND */
enum mipc_nw_enwrejac_ind_tlv_enum {
    mipc_nw_enwrejac_ind_tlv_NONE = 0,
    /* Indicates the auth_rej_cause, if 2G AUTH/3G AUTH/5G AUTH, the set flag 1. */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_AUTH_REJ_CAUSE                   = 0x100,
    /* Reject cause Refer to 3GPP TS 24.008, TS 24.301 */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_MM_LU_REJ_CAUSE                  = 0x101,
    /* Reject cause Refer to 3GPP TS 24.008, TS 24.301 */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_ATTACH_REJ_CAUSE                 = 0x102,
    /* Reject cause Refer to 3GPP TS 24.008, TS 24.301 */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_REG_UPDATE_REJ_CAUSE             = 0x103,
    /* The values range 0-4 */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_MM_LU_ATTEMPT_COUNTER            = 0x104,
    /* The value def of 2G ATTACH/3G ATTACH. 5G REGISTRARION */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_ATTACH_ATTEMPT_COUNTER           = 0x105,
    /* The value def of 3G RA update/4G TA update */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_REG_UPDATE_ATTEMPT_COUNTER       = 0x106,
    /* The value def of 0- GSM, 2- UTRAN and 7- EUTRAN */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_RAT                              = 0x107,
    /* Reject cause Refer to 3GPP TS 24.008, TS 24.301 */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_SERVICE_REJ_CAUSE                = 0x108,
    /* Reject cause Refer to 3GPP TS 24.008, TS 24.301 */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_ESM_REJ_CAUSE                    = 0x109,
    /* Attach//LU/RAU/TAU */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_PROC_RESULT                      = 0x10A,
    /* The value def of 2/3/4/5G nas procedure */
    /* type = uint8_t */
    MIPC_NW_ENWREJAC_IND_T_NAS_PROC                         = 0x10B,
    /* plmn id, for example, 46001 */
    /* type = string */
    MIPC_NW_ENWREJAC_IND_T_PLMN_ID                          = 0x10C,
};

    /* MIPC_MSG.NW_BASEMENT_DETECTION_IND */
enum mipc_nw_basement_detection_ind_tlv_enum {
    mipc_nw_basement_detection_ind_tlv_NONE = 0,
    /* 0: enter basement, 1: leave basement */
    /* type = uint32_t, refer to NW_BASEMENT_STATUS */
    MIPC_NW_BASEMENT_DETECTION_IND_T_STATUS                 = 0x100,
};

    /* MIPC_MSG.NW_ENWCFGINFO_IND */
enum mipc_nw_enwcfginfo_ind_tlv_enum {
    mipc_nw_enwcfginfo_ind_tlv_NONE = 0,
    /* Indicate 4X4MINO state: 0, inactive; 1, active */
    /* type = uint8_t */
    MIPC_NW_ENWCFGINFO_IND_T_4X4MINO_STATE                  = 0x100,
    /* Indicate 256QAM state: 0, inactive; 1, active */
    /* type = uint8_t */
    MIPC_NW_ENWCFGINFO_IND_T_256QAM_STATE                   = 0x101,
    /* Indicate 64QAM state: 0, inactive; 1, active */
    /* type = uint8_t */
    MIPC_NW_ENWCFGINFO_IND_T_64QAM_STATE                    = 0x102,
};

    /* MIPC_MSG.NW_NRRC_RRC_RELEASE_UAI_IND */
enum mipc_nw_nrrc_rrc_release_uai_ind_tlv_enum {
    mipc_nw_nrrc_rrc_release_uai_ind_tlv_NONE = 0,
    /* This field indicates release-Preference UAI report status. */
    /* type = uint8_t, refer to RELEASE_PREFERENCE_UAI_REPORT_STATUS */
    MIPC_NW_NRRC_RRC_RELEASE_UAI_IND_T_REPORT_STATUS        = 0x100,
    /*
      0: not allowed;
      1: allowed;
      This field indicates whether UE is allowed or not to report value CONNECTED as desired RRC state; Valid only when REPORT_STATUS is 1 (NW_CONFIGURED)
    */
    /* type = uint8_t */
    MIPC_NW_NRRC_RRC_RELEASE_UAI_IND_T_CONNECTED_REPORTING  = 0x101,
    /* This field indicates UE's preferred RRC state. Valid only when REPORT_STATUS is 2 (REPORT_SENT) */
    /* type = uint8_t, refer to UE_PREFERRED_RRC_STATE */
    MIPC_NW_NRRC_RRC_RELEASE_UAI_IND_T_PREFERRED_RRC_STATE  = 0x102,
};

    /* MIPC_MSG.NW_NRRC_POWERSAVING_UAI_IND */
enum mipc_nw_nrrc_powersaving_uai_ind_tlv_enum {
    mipc_nw_nrrc_powersaving_uai_ind_tlv_NONE = 0,
    /* Cell group ID; 0: MCG; 1: SCG */
    /* type = uint8_t */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_CELL_GROUP_ID        = 0x100,
    /* This field indicates maxCC-Preference report status. */
    /* type = uint8_t, refer to NRRC_PREFERENCE_REPORT_STATUS */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MAXCC_PREFERENCE_STATUS = 0x101,
    /* This field indicates maxMIMO-LayerPreference report status. */
    /* type = uint8_t, refer to NRRC_PREFERENCE_REPORT_STATUS */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MAXMIMO_LAYERPREFERENCE_STATUS = 0x102,
    /* This field indicates drx-Preference report status. */
    /* type = uint8_t, refer to NRRC_PREFERENCE_REPORT_STATUS */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_DRX_PREFERENCE_STATUS = 0x103,
    /* This field indicates maxBW-Preference report status. */
    /* type = uint8_t, refer to NRRC_PREFERENCE_REPORT_STATUS */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MAXBW_PREFERENCE_STATUS = 0x104,
    /* This field indicates maxBW-Preference report status. */
    /* type = uint8_t, refer to NRRC_PREFERENCE_REPORT_STATUS */
    MIPC_NW_NRRC_POWERSAVING_UAI_IND_T_MINSCHEDULINGOFFSET_PREFERENCE_STATUS = 0x105,
};

    /* MIPC_MSG.NW_NRRC_OVERHEATINGASSISTANCE_UAI_IND */
enum mipc_nw_nrrc_overheatingassistance_uai_ind_tlv_enum {
    mipc_nw_nrrc_overheatingassistance_uai_ind_tlv_NONE = 0,
    /* This field indicates Overheating UAI report status. */
    /* type = uint8_t, refer to OVERHEATING_UAI_REPORT_STATUS */
    MIPC_NW_NRRC_OVERHEATINGASSISTANCE_UAI_IND_T_REPORT_STATUS = 0x100,
};

    /* MIPC_MSG.NW_NRRC_TIMER_STATUS_IND */
enum mipc_nw_nrrc_timer_status_ind_tlv_enum {
    mipc_nw_nrrc_timer_status_ind_tlv_NONE = 0,
    /* Timer name */
    /* type = uint8_t, refer to NRRC_TIMER_NAME */
    MIPC_NW_NRRC_TIMER_STATUS_IND_T_TIMER_NAME              = 0x100,
    /* Timer status */
    /* type = uint8_t, refer to NRRC_TIMER_STATUS */
    MIPC_NW_NRRC_TIMER_STATUS_IND_T_TIMER_STATUS            = 0x101,
    /* This field indicates timer length in milliseconds when timer is configured/started and the remaining timer in milliseconds when timer is stopped; The value 0xFFFFFFFF means Infinity. */
    /* type = uint32_t */
    MIPC_NW_NRRC_TIMER_STATUS_IND_T_TIMER_VALUE             = 0x102,
};

    /* MIPC_MSG.NW_EPSFB_IND */
enum mipc_nw_epsfb_ind_tlv_enum {
    mipc_nw_epsfb_ind_tlv_NONE = 0,
    /* EPS fallback state */
    /* type = uint8_t, refer to NW_EPSFB_STATE_ENUM */
    MIPC_NW_EPSFB_IND_T_STATE                               = 0x100,
    /* EPS fallback type */
    /* type = uint8_t, refer to NW_EPSFB_TYPE_ENUM */
    MIPC_NW_EPSFB_IND_T_TYPE                                = 0x101,
};

    /* MIPC_MSG.NW_TAU_FAIL_IND */
enum mipc_nw_tau_fail_ind_tlv_enum {
    mipc_nw_tau_fail_ind_tlv_NONE = 0,
    /* Cause of TAU fail */
    /* type = uint8_t, refer to NW_TAU_FAIL_CAUSE_ENUM */
    MIPC_NW_TAU_FAIL_IND_T_CAUSE                            = 0x100,
};

    /* MIPC_MSG.NW_START_NETWORK_SCAN_REQ */
enum mipc_nw_start_network_scan_req_tlv_enum {
    mipc_nw_start_network_scan_req_tlv_NONE = 0,
    /* 0: one_shot, 1: periodic */
    /* type = uint32_t, refer to NW_SCAN_TYPE */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_SCAN_TYPE              = 0x100,
    /* periodic interval, 5~300 sec */
    /* type = uint32_t */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_INTERVAL               = 0x101,
    /* Its value must smaller than 3600 sec */
    /* type = uint32_t */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_MAX_SEARCH_TIME        = 0x102,
    /* 0: disable(one final result), 1: enable(multiple partial result) */
    /* type = uint32_t, refer to NW_INCREMENTAL_RESULTS */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_INCREMENTAL_RESULTS    = 0x103,
    /* 1~10 sec, 0: default modem value, send whenever updated */
    /* type = uint32_t */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_INCREMENTAL_RESULT_PERIODICITY = 0x104,
    /* The list of rat info. 1 RAT 1 record, the max num of record is 8. */
    /* type = struct, refer to nw_record_info */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_RECORD_LIST            = 0x105,
    /* PLMN id, the max num of plmn is 20 */
    /* type = struct, refer to nw_plmn_info */
    MIPC_NW_START_NETWORK_SCAN_REQ_T_PLMN_LIST              = 0x106,
};

    /* MIPC_MSG.NW_START_NETWORK_SCAN_CNF */
enum mipc_nw_start_network_scan_cnf_tlv_enum {
    mipc_nw_start_network_scan_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_STOP_NETWORK_SCAN_REQ */
enum mipc_nw_stop_network_scan_req_tlv_enum {
    mipc_nw_stop_network_scan_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_STOP_NETWORK_SCAN_CNF */
enum mipc_nw_stop_network_scan_cnf_tlv_enum {
    mipc_nw_stop_network_scan_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_NETWORK_SCAN_IND */
enum mipc_nw_network_scan_ind_tlv_enum {
    mipc_nw_network_scan_ind_tlv_NONE = 0,
    /* 0: partial, 1: complete */
    /* type = uint32_t, refer to NW_SCAN_STATUS */
    MIPC_NW_NETWORK_SCAN_IND_T_SCAN_STATUS                  = 0x100,
    /* network info */
    /* type = struct, refer to nw_scan_info */
    MIPC_NW_NETWORK_SCAN_IND_T_RECORD_LIST                  = 0x101,
};

    /* MIPC_MSG.NW_CA_INFO_IND */
enum mipc_nw_ca_info_ind_tlv_enum {
    mipc_nw_ca_info_ind_tlv_NONE = 0,
    /* CA info */
    /* type = struct, refer to nw_ecainfo */
    MIPC_NW_CA_INFO_IND_T_CAINFO                            = 0x100,
};

    /* MIPC_MSG.NW_LCM_HOPPING_IND */
enum mipc_nw_lcm_hopping_ind_tlv_enum {
    mipc_nw_lcm_hopping_ind_tlv_NONE = 0,
    /* The mipi bitmapp value for the indication */
    /* type = uint32_t */
    MIPC_NW_LCM_HOPPING_IND_T_MIPI_BITMAP                   = 0x100,
};



    /* MIPC_MSG.NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ */
enum mipc_nw_set_allowed_security_algo_config_req_tlv_enum {
    mipc_nw_set_allowed_security_algo_config_req_tlv_NONE = 0,
    /* Allow null encryption in network communication or not. The default value is allowed(True). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ_T_ALLOW_NULL_ALGO = 0x100,
};

    /* MIPC_MSG.NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_CNF */
enum mipc_nw_set_allowed_security_algo_config_cnf_tlv_enum {
    mipc_nw_set_allowed_security_algo_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_REQ */
enum mipc_nw_get_allowed_security_algo_config_req_tlv_enum {
    mipc_nw_get_allowed_security_algo_config_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF */
enum mipc_nw_get_allowed_security_algo_config_cnf_tlv_enum {
    mipc_nw_get_allowed_security_algo_config_cnf_tlv_NONE = 0,
    /* The result of Allow null encryption state in network. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF_T_ALLOW_NULL_ALGO = 0x100,
};

    /* MIPC_MSG.NW_CELLULAR_ID_DISCLOSURE_IND */
enum mipc_nw_cellular_id_disclosure_ind_tlv_enum {
    mipc_nw_cellular_id_disclosure_ind_tlv_NONE = 0,
    /* PLMN ID, for example 46000 */
    /* type = string */
    MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_PLMN_ID            = 0x100,
    /* Cellular ID type. */
    /* type = uint8_t, refer to NW_CELLULAR_ID_TYPE */
    MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_CELLULAR_ID_TYPE   = 0x101,
    /* NAS message type. */
    /* type = uint8_t, refer to NW_NAS_MSG_TYPE */
    MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_NAS_MSG_TYPE       = 0x102,
    /* Emergency state. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_CELLULAR_ID_DISCLOSURE_IND_T_IS_EMERGENCY       = 0x103,
};

    /* MIPC_MSG.NW_SECURITY_ALGOS_IND */
enum mipc_nw_security_algos_ind_tlv_enum {
    mipc_nw_security_algos_ind_tlv_NONE = 0,
    /* Connection event. */
    /* type = uint8_t, refer to NW_CONNECTION_EVENT */
    MIPC_NW_SECURITY_ALGOS_IND_T_CONNECTION_EVENT           = 0x100,
    /* Integrity algo. */
    /* type = uint8_t, refer to NW_INTEGRITY_ALGO */
    MIPC_NW_SECURITY_ALGOS_IND_T_INTEGRITY_ALGO             = 0x101,
    /* Encryption algo. */
    /* type = uint8_t, refer to NW_ENCRYPTION_ALGO */
    MIPC_NW_SECURITY_ALGOS_IND_T_ENCRYPTION_ALGO            = 0x102,
    /* Unprotected_emergency state. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_NW_SECURITY_ALGOS_IND_T_IS_UNPROTECTED_EMERGENCY   = 0x103,
};

#endif /* __MIPC_MSG_NW_CONST_H__ */
