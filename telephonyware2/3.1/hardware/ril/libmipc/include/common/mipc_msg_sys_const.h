#ifndef __MIPC_MSG_SYS_CONST_H__
#define __MIPC_MSG_SYS_CONST_H__

enum MIPC_SYS_MSG_enum {
    MIPC_SYS_MSG_NONE = 0,
    /* This command is adopted to acquire modem information, e.g. radio band information, IMEI, MEID, ESN, serial number, and so on. */
    MIPC_SYS_GET_INFO_REQ                                   = 1,
    MIPC_SYS_GET_INFO_CNF                                   = 2,

    /* This command is used to send AT command. */
    MIPC_SYS_AT_REQ                                         = 3,
    MIPC_SYS_AT_CNF                                         = 4,

    /* This command is used to reboot the whole device. */
    MIPC_SYS_REBOOT_REQ                                     = 5,
    MIPC_SYS_REBOOT_CNF                                     = 6,

    /*  */
    MIPC_SYS_GET_MAPPING_REQ                                = 7,
    MIPC_SYS_GET_MAPPING_CNF                                = 8,

    /* set SIM mapping */
    MIPC_SYS_SET_MAPPING_REQ                                = 9,
    MIPC_SYS_SET_MAPPING_CNF                                = 10,

    /* This command is used to acquire thermal sensor number. */
    MIPC_SYS_GET_THERMAL_SENSOR_NUM_REQ                     = 15,
    MIPC_SYS_GET_THERMAL_SENSOR_NUM_CNF                     = 16,

    /* This command is used to acquire the thermal sensor information. */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_REQ                    = 17,
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF                    = 18,

    /* This command is used to set thermal alarm. One sensor has two alarms. */
    MIPC_SYS_SET_THERMAL_SENSOR_REQ                         = 19,
    MIPC_SYS_SET_THERMAL_SENSOR_CNF                         = 20,

    /* This command is used to acquire the current state of thermal sensor. */
    MIPC_SYS_GET_THERMAL_SENSOR_REQ                         = 21,
    MIPC_SYS_GET_THERMAL_SENSOR_CNF                         = 22,

    /* [PHASE OUT]inserted with GET_THERMAL_ACTUATOR_REQ */
    MIPC_SYS_GET_THERMAL_ACTUATOR_NUM_REQ                   = 23,
    MIPC_SYS_GET_THERMAL_ACTUATOR_NUM_CNF                   = 24,

    /* This command is used to set the thermal actuator. */
    MIPC_SYS_SET_THERMAL_ACTUATOR_REQ                       = 25,
    MIPC_SYS_SET_THERMAL_ACTUATOR_CNF                       = 26,

    /* [PHASE OUT]inserted with GET_THERMAL_ACTUATOR_REQ */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_REQ                  = 27,
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF                  = 28,

    /* set modem configuration request */
    MIPC_SYS_SET_CONFIG_REQ                                 = 29,
    MIPC_SYS_SET_CONFIG_CNF                                 = 30,

    /* get modem configuration request */
    MIPC_SYS_GET_CONFIG_REQ                                 = 31,
    MIPC_SYS_GET_CONFIG_CNF                                 = 32,

    /* register configuration indication */
    MIPC_SYS_REG_CONFIG_REQ                                 = 33,
    MIPC_SYS_REG_CONFIG_CNF                                 = 34,

    /* enable or disable ADPCLK report */
    MIPC_SYS_SET_ADPCLK_REQ                                 = 35,
    MIPC_SYS_SET_ADPCLK_CNF                                 = 36,

    /* get current ADPCLK report */
    MIPC_SYS_GET_ADPCLK_REQ                                 = 37,
    MIPC_SYS_GET_ADPCLK_CNF                                 = 38,

    /* set modem log mode */
    MIPC_SYS_SET_MD_LOG_MODE_REQ                            = 39,
    MIPC_SYS_SET_MD_LOG_MODE_CNF                            = 40,

    /* get modem log mode */
    MIPC_SYS_GET_MD_LOG_MODE_REQ                            = 41,
    MIPC_SYS_GET_MD_LOG_MODE_CNF                            = 42,

    /* set modem log level */
    MIPC_SYS_SET_MD_LOG_LEVEL_REQ                           = 43,
    MIPC_SYS_SET_MD_LOG_LEVEL_CNF                           = 44,

    /* get modem log level */
    MIPC_SYS_GET_MD_LOG_LEVEL_REQ                           = 45,
    MIPC_SYS_GET_MD_LOG_LEVEL_CNF                           = 46,

    /* set modem log location */
    MIPC_SYS_SET_MD_LOG_LOCATION_REQ                        = 47,
    MIPC_SYS_SET_MD_LOG_LOCATION_CNF                        = 48,

    /* get modem log location */
    MIPC_SYS_GET_MD_LOG_LOCATION_REQ                        = 49,
    MIPC_SYS_GET_MD_LOG_LOCATION_CNF                        = 50,

    /* This command is adopted to write NVRAM. */
    MIPC_SYS_WRITE_NVRAM_REQ                                = 51,
    MIPC_SYS_WRITE_NVRAM_CNF                                = 52,

    /* This command is adopted to read NVRAM. */
    MIPC_SYS_READ_NVRAM_REQ                                 = 53,
    MIPC_SYS_READ_NVRAM_CNF                                 = 54,

    /*  */
    MIPC_SYS_AUTH_REQ                                       = 55,
    MIPC_SYS_AUTH_CNF                                       = 56,

    /* set dat */
    MIPC_SYS_SET_DAT_REQ                                    = 57,
    MIPC_SYS_SET_DAT_CNF                                    = 58,

    /* get dat */
    MIPC_SYS_GET_DAT_REQ                                    = 59,
    MIPC_SYS_GET_DAT_CNF                                    = 60,

    /* modem configuration request */
    MIPC_SYS_MCF_REQ                                        = 61,
    MIPC_SYS_MCF_CNF                                        = 62,

    /* set FCC lock mode (need to auth first) */
    MIPC_SYS_SET_FCC_LOCK_REQ                               = 63,
    MIPC_SYS_SET_FCC_LOCK_CNF                               = 64,

    /* This command is used to set the system time, and the time given MUST be UTC time. */
    MIPC_SYS_SET_TIME_REQ                                   = 65,
    MIPC_SYS_SET_TIME_CNF                                   = 66,

    /* This command is used to acquire system time expressed in UTC. */
    MIPC_SYS_GET_TIME_REQ                                   = 67,
    MIPC_SYS_GET_TIME_CNF                                   = 68,

    /* This command is used to set SAR (Specific Absorption Rate) request. */
    MIPC_SYS_SET_SAR_REQ                                    = 69,
    MIPC_SYS_SET_SAR_CNF                                    = 70,

    /* This command is used to acquire SAR request. */
    MIPC_SYS_GET_SAR_REQ                                    = 71,
    MIPC_SYS_GET_SAR_CNF                                    = 72,

    /* set power saving mode */
    MIPC_SYS_SET_POWER_SAVING_REQ                           = 73,
    MIPC_SYS_SET_POWER_SAVING_CNF                           = 74,

    /* <trigger DEVICE to start or stop the statistics of some connection parameters> or  <read DEVICE statitic info>; cant carry START&STOP in one MIPC_REQ; execute priority: STOP > READ > PERIOD > START; */
    MIPC_SYS_CONNECTIVITY_STATISTICS_REQ                    = 75,
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF                    = 76,

    /* Query device SBP (Single Binary Platform) information. */
    MIPC_SYS_QUERY_SBP_REQ                                  = 77,
    MIPC_SYS_QUERY_SBP_CNF                                  = 78,

    /* to set tx ind interval */
    MIPC_SYS_SET_TX_IND_INTERVAL_REQ                        = 79,
    MIPC_SYS_SET_TX_IND_INTERVAL_CNF                        = 80,

    /* set Location Info */
    MIPC_SYS_SET_GEO_LOCATION_REQ                           = 81,
    MIPC_SYS_SET_GEO_LOCATION_CNF                           = 82,

    /* This command is used to configure and query DSBP (Dynamic Single Binary Platform) information. */
    MIPC_SYS_SET_DSBP_REQ                                   = 83,
    MIPC_SYS_SET_DSBP_CNF                                   = 84,

    /* This command is used to send SAR IND. */
    MIPC_SYS_SEND_SAR_IND_REQ                               = 85,
    MIPC_SYS_SEND_SAR_IND_CNF                               = 86,

    /* This command is used to infrom modem the reboot mode */
    MIPC_SYS_SILENT_REBOOT_REQ                              = 87,
    MIPC_SYS_SILENT_REBOOT_CNF                              = 88,

    /* This command is used to infrom modem current project setting is single sim or dual sim,and need set before power on to flight mode */
    MIPC_SYS_MULTI_SIM_CONFIG_REQ                           = 89,
    MIPC_SYS_MULTI_SIM_CONFIG_CNF                           = 90,

    /* This command is used to inform modem of the reboot mode. */
    MIPC_SYS_REBOOT_SET_REQ                                 = 91,
    MIPC_SYS_REBOOT_SET_CNF                                 = 92,

    /* This command is to acquire the information of thermal actuator. */
    MIPC_SYS_GET_THERMAL_ACTUATOR_REQ                       = 93,
    MIPC_SYS_GET_THERMAL_ACTUATOR_CNF                       = 94,

    /* [FOR INTERNAL DEBUG]This command is used to acquire the runtime of thermal sensor. */
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_REQ                 = 95,
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF                 = 96,

    /* [FOR INTERNAL DEBUG] reset_nv -> set_auto -> set_trip -> wb_nv */
    MIPC_SYS_SET_THERMAL_RUNTIME_REQ                        = 97,
    MIPC_SYS_SET_THERMAL_RUNTIME_CNF                        = 98,

    /* Set or acquire SBP information, including SBP ID, SBP feature configuration, and SBP feature data and parameters. */
    MIPC_SYS_SET_OR_GET_SBP_INFO_REQ                        = 99,
    MIPC_SYS_SET_OR_GET_SBP_INFO_CNF                        = 100,

    /*  */
    MIPC_SYS_GET_ALL_THERMAL_INFO_REQ                       = 101,
    MIPC_SYS_GET_ALL_THERMAL_INFO_CNF                       = 102,

    /* This command is used to request modem collected information from DMF. */
    MIPC_SYS_META_REQ                                       = 109,
    MIPC_SYS_META_CNF                                       = 110,

    /* This command is used to request modem collected information from DMF. */
    MIPC_SYS_DMF_REQ                                        = 111,
    MIPC_SYS_DMF_CNF                                        = 112,

    /* This command is used to check whether modem is alive */
    MIPC_SYS_ECHO_REQ                                       = 113,
    MIPC_SYS_ECHO_CNF                                       = 114,

    /* This command is used for META tool to control forwarder. */
    MIPC_SYS_META_FORWARDER_CTRL_REQ                        = 115,
    MIPC_SYS_META_FORWARDER_CTRL_CNF                        = 116,

    /* Set the flush interval of logging in stream mode */
    MIPC_SYS_SET_MD_LOG_FLUSH_INTERVAL_REQ                  = 119,
    MIPC_SYS_SET_MD_LOG_FLUSH_INTERVAL_CNF                  = 120,

    /* Get the flush interval of logging */
    MIPC_SYS_GET_MD_LOG_FLUSH_INTERVAL_REQ                  = 121,
    MIPC_SYS_GET_MD_LOG_FLUSH_INTERVAL_CNF                  = 122,

    /* establish heartbeat agent */
    MIPC_SYS_HBA_ESTABLISH_REQ                              = 123,
    MIPC_SYS_HBA_ESTABLISH_CNF                              = 124,

    /* this command is used to pause or stop heartbeat agent */
    MIPC_SYS_HBA_CTRL_REQ                                   = 125,
    MIPC_SYS_HBA_CTRL_CNF                                   = 126,

    /* send heartbeat packet now */
    MIPC_SYS_HBA_SEND_NOW_REQ                               = 127,
    MIPC_SYS_HBA_SEND_NOW_CNF                               = 128,

    /* resume heartbeat agent */
    MIPC_SYS_HBA_RESUME_REQ                                 = 129,
    MIPC_SYS_HBA_RESUME_CNF                                 = 130,

    /* This commond is used to mia_start_scenario_req_struct */
    MIPC_SYS_MIA_START_SCENARIO_REQ                         = 131,
    MIPC_SYS_MIA_START_SCENARIO_CNF                         = 132,

    /* This commond is used to mia_stop_scenario_req_struct */
    MIPC_SYS_MIA_STOP_SCENARIO_REQ                          = 133,
    MIPC_SYS_MIA_STOP_SCENARIO_CNF                          = 134,

    /* This commond is used to MIA_UPDATE_METRICS_REQ */
    MIPC_SYS_MIA_UPDATE_METRICS_REQ                         = 135,
    MIPC_SYS_MIA_UPDATE_METRICS_CNF                         = 136,

    /* This command is used to trigger log flush in DHL warp mode. */
    MIPC_SYS_TRIGGER_MD_LOG_FLUSH_REQ                       = 137,
    MIPC_SYS_TRIGGER_MD_LOG_FLUSH_CNF                       = 138,

    /* This command is used to acquire log flush status in DHL warp mode. */
    MIPC_SYS_GET_MD_LOG_FLUSH_STATUS_REQ                    = 139,
    MIPC_SYS_GET_MD_LOG_FLUSH_STATUS_CNF                    = 140,

    /* This command is used to acquire coclock nvdata for GNSS (Global Navigation Satellite System) */
    MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_REQ                    = 141,
    MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF                    = 142,

    /* Set SBP (Single Binary Platform) information, including SBP ID, SBP feature configuration, and SBP feature data and parameters. */
    MIPC_SYS_SET_SBP_INFO_REQ                               = 143,
    MIPC_SYS_SET_SBP_INFO_CNF                               = 144,

    /* Acquire SBP (Single Binary Platform) information, including SBP ID, SBP feature configuration, and SBP feature data and parameters. */
    MIPC_SYS_GET_SBP_INFO_REQ                               = 145,
    MIPC_SYS_GET_SBP_INFO_CNF                               = 146,

    /* This command is used to handle tx conditional IQ dump. */
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ              = 147,
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_CNF              = 148,

    /* Get the last AP be wakeup reason from MD. */
    MIPC_SYS_GET_WAKEUP_REASON_REQ                          = 149,
    MIPC_SYS_GET_WAKEUP_REASON_CNF                          = 150,

    /* Set AP sleep timer. Modem will send MIPC_SYS_TIMER_EXPIRE_IND to wakeup AP when timer end. */
    MIPC_SYS_SET_SLEEP_TIMER_REQ                            = 151,
    MIPC_SYS_SET_SLEEP_TIMER_CNF                            = 152,

    /* Get system sleep timer. */
    MIPC_SYS_GET_SLEEP_TIMER_REQ                            = 153,
    MIPC_SYS_GET_SLEEP_TIMER_CNF                            = 154,

    /* Used to notify modem that AP will go into sleep mode. */
    MIPC_SYS_SET_SLEEP_MODE_REQ                             = 155,
    MIPC_SYS_SET_SLEEP_MODE_CNF                             = 156,

    /* sys mcif set req. */
    MIPC_SYS_SEND_CONSYS_WIFI_REQ                           = 157,
    MIPC_SYS_SEND_CONSYS_WIFI_CNF                           = 158,

    /* sys mcif set req. */
    MIPC_SYS_SEND_CONSYS_BT_REQ                             = 159,
    MIPC_SYS_SEND_CONSYS_BT_CNF                             = 160,

    /* This command is used to set the filter of modem log. */
    MIPC_SYS_SET_MD_LOG_FILTER_REQ                          = 161,
    MIPC_SYS_SET_MD_LOG_FILTER_CNF                          = 162,

    /* This command is used to handle control system performance validation related HW module or SW framework. */
    MIPC_SYS_SET_SPV_CONTROL_REQ                            = 163,
    MIPC_SYS_SET_SPV_CONTROL_CNF                            = 164,

    /* This command is adopted to configure IDC parameters. */
    MIPC_SYS_SET_IDC_CFG_REQ                                = 179,
    MIPC_SYS_SET_IDC_CFG_CNF                                = 180,

    /* This command forward the AT command string from one HOST to another HOST.  Only use when scenario is limitation of hardware design. */
    MIPC_SYS_FORWARD_AT_REQ                                 = 181,
    MIPC_SYS_FORWARD_AT_CNF                                 = 182,

    /* This command is used to trigger coredump. */
    MIPC_SYS_TRIGGER_COREDUMP_REQ                           = 183,
    MIPC_SYS_TRIGGER_COREDUMP_CNF                           = 184,

    /* This command is adopted to get modem factory mode. */
    MIPC_SYS_GET_FACTORY_MODE_REQ                           = 185,
    MIPC_SYS_GET_FACTORY_MODE_CNF                           = 186,

    /* This command is adopted to set modem factory mode. */
    MIPC_SYS_SET_FACTORY_MODE_REQ                           = 187,
    MIPC_SYS_SET_FACTORY_MODE_CNF                           = 188,

    /* This command is used to Enable/Disable COEX UART test mode */
    MIPC_SYS_SET_COEX_UART_TEST_MODE_REQ                    = 189,
    MIPC_SYS_SET_COEX_UART_TEST_MODE_CNF                    = 190,

    /* This command is used to write data into COEX UART TX */
    MIPC_SYS_WRITE_COEX_UART_REQ                            = 191,
    MIPC_SYS_WRITE_COEX_UART_CNF                            = 192,

    /* This command is used to read data from COEX UART RX FIFO */
    MIPC_SYS_READ_COEX_UART_REQ                             = 193,
    MIPC_SYS_READ_COEX_UART_CNF                             = 194,

    /* This command is used to Enable/Disable COEX UART lookback mode */
    MIPC_SYS_SET_COEX_UART_LOOPBACK_MODE_REQ                = 195,
    MIPC_SYS_SET_COEX_UART_LOOPBACK_MODE_CNF                = 196,

    /* Set Low Voltage actuator by actuator ID and state. */
    MIPC_SYS_SET_LOWV_ACTUATOR_REQ                          = 199,
    MIPC_SYS_SET_LOWV_ACTUATOR_CNF                          = 200,

    /* Get Low Voltage actuator {number/actuator state by actuator ID}. */
    MIPC_SYS_GET_LOWV_ACTUATOR_REQ                          = 201,
    MIPC_SYS_GET_LOWV_ACTUATOR_CNF                          = 202,

    /* This command is set mspm cbrs validation session. */
    MIPC_SYS_SET_MSPM_SESSION_REQ                           = 203,
    MIPC_SYS_SET_MSPM_SESSION_CNF                           = 204,

    /* Config the value of ERRC(LTE RRC) OFFSET NVRAM parameters */
    MIPC_SYS_SET_ERRC_OFFSET_REQ                            = 209,
    MIPC_SYS_SET_ERRC_OFFSET_CNF                            = 210,

    /*
      * control_type = MDDBG_CONTROL_TYPE_MMV1 [MMV1]
          * Must setting TLV control, pool_index, start_count, scan_buffer_no
          * Control = 0 => Control Buffer
          * Control = 1 => CDM
          * Control = 2 => ADM
          * Control = 3 => AFM
          * Control = 4 => CDM Leakage Scan start
          * Control = 5 => CDM Leakage Scan stop
      * control_type = MDDBG_CONTROL_TYPE_DIAG [Diagnostic Task]
          * Must setting TLV enable_set, timer_value
      * control_type = MDDBG_CONTROL_TYPE_WP [WatchPoint]
          * Control = 0 => MDDBG_SET_BP
              * Must setting TLV vpeIndex, selectBP, bp_addr, addr_mask
          * Control = 1 => MDDBG_REMOVE_BP
              * Must setting TLV vpeIndex, selectBP
          * Control = 2 => MDDBG_QUERY
              * Must setting TLV vpeIndex, selectBP
      * control_type = MDDBG_CONTROL_TYPE_BP [BreakPoint]
          * Control = 0 => MDDBG_SET_WP
              * Must setting TLV vpeIndex, selectWP, wp_addr, addr_mask, type
          * Control = 1 => MDDBG_REMOVE_WP
              * Must setting TLV vpeIndex, selectWP
          * Control = 2 => MDDBG_QUERY
              * Must setting TLV vpeIndex, selectWP
    */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ                          = 215,
    MIPC_SYS_SET_MDDBG_CONTROL_CNF                          = 216,

    /*
      This MIPC is only for Internal Test,TCXO can be considered as the clock of the system.
      When the system enters AOC3.0 sleep mode, the TCXO clock will be turned off to achieve power saving.
      We need to observe the duration of TCXO being turned off to ensure the normal sleep behavior of the system.
      The main purpose is to observe how long the system's TCXO clock is turned off to ensure the normal sleep behavior of the system.
    */
    MIPC_SYS_GET_TCXO_INFO_REQ                              = 219,
    MIPC_SYS_GET_TCXO_INFO_CNF                              = 220,

    /* This command triggers the corresponding chip-diagnosis according to the field 'OP' to check related HW status. This command would be used under RMA flow or before leaving factory to improve chip hardware quality. (This command would be used in both factory mode and normal mode) */
    MIPC_SYS_TRIGGER_CHIP_DIAGNOSIS_REQ                     = 221,
    MIPC_SYS_TRIGGER_CHIP_DIAGNOSIS_CNF                     = 222,

    /* This commond is used to host send app information to modem for start and end of power saving mode */
    MIPC_SYS_SEND_APP_EVENT_REQ                             = 223,
    MIPC_SYS_SEND_APP_EVENT_CNF                             = 224,

    /* This command is used to Get primary imei and primary slot from modem. */
    MIPC_SYS_GET_PRIMARY_IMEI_INFO_REQ                      = 5392,
    MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF                      = 5393,

    /* This command is the unsolicited indication of AT command. */
    MIPC_SYS_AT_IND                                         = 16385,

    /* This command is the unsolicited indication of thermal alarm. */
    MIPC_SYS_THERMAL_SENSOR_IND                             = 16386,

    /* the configuration change indication */
    MIPC_SYS_CONFIG_IND                                     = 16387,

    /*  */
    MIPC_SYS_ADPCLK_IND                                     = 16388,

    /* modem configuration indication */
    MIPC_SYS_MCF_IND                                        = 16389,

    /* This command is the unsolicited indication of SBP ID. */
    MIPC_SYS_SBP_IND                                        = 16390,

    /*  */
    MIPC_SYS_EL2_IP_UL_IND                                  = 16391,

    /*  */
    MIPC_SYS_EL2_IP_DL_IND                                  = 16392,

    /*  */
    MIPC_SYS_EL2_MAC_UL_IND                                 = 16393,

    /*  */
    MIPC_SYS_EL2_MAC_DL_IND                                 = 16394,

    /*  */
    MIPC_SYS_EL2_PDCP_UL_IND                                = 16395,

    /*  */
    MIPC_SYS_EL2_PDCP_DL_IND                                = 16396,

    /*  */
    MIPC_SYS_NL2_MAC_UL_IND                                 = 16397,

    /*  */
    MIPC_SYS_NL2_MAC_DL_IND                                 = 16398,

    /*  */
    MIPC_SYS_NL2_PDCP_UL_IND                                = 16399,

    /*  */
    MIPC_SYS_NL2_PDCP_DL_IND                                = 16400,

    /* location info indicate */
    MIPC_SYS_GEO_LOCATION_IND                               = 16401,

    /* This command is the unsolicited indication of modem initialization. */
    MIPC_SYS_MD_INIT_IND                                    = 16402,

    /* modem EE waring indication */
    MIPC_SYS_WARNING_IND                                    = 16403,

    /* nvram verified failed indication */
    MIPC_SYS_NV_SIG_ERR_IND                                 = 16404,

    /* report the statistics info of VODATA */
    MIPC_SYS_VODATA_STATISTICS_IND                          = 16405,

    /* This command is the unsolicited indication for reporting the state of modem for thermal control. */
    MIPC_SYS_THERMAL_ACTUATOR_IND                           = 16406,

    /* This command is the unsolicited indication for reporting DMF data. */
    MIPC_SYS_DMF_URC_IND                                    = 16408,

    /* This command is used for META tool. */
    MIPC_SYS_META_CONTROL_IND                               = 16409,

    /* modem reports heartbeat agent indication to ap */
    MIPC_SYS_HBA_TIMEOUT_IND                                = 16410,

    /* modem reports hardware filter source status indication */
    MIPC_SYS_HBA_HW_FILTER_SRC_STATE_IND                    = 16411,

    /* The AP sleep timer expired, modem send this indication to AP. */
    MIPC_SYS_TIMER_EXPIRE_IND                               = 16412,

    /* Indication of a frame configuration for an external collocated Mobile Wireless Standards (MWS) system. */
    MIPC_SYS_IDC_FRAME_CFG_IND                              = 16413,

    /* MWS scan frequency indication. */
    MIPC_SYS_IDC_SCAN_FREQ_IND                              = 16414,

    /* Controller of the MWS channel parameters indication. */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND                          = 16415,

    /* modem reports dmf em icd info indication */
    MIPC_SYS_DMF_EM_ICD_INFO_IND                            = 16416,

    /* This command is the unsolicited indication of forward AT command to the registered host. */
    MIPC_SYS_FORWARD_AT_IND                                 = 16418,

    /* This command is used modem indicate  the primary imei and slot information to the AP. */
    MIPC_SYS_PRIMARY_IMEI_SWITCH_IND                        = 16421,

    /* send REBOOT command to AP */
    MIPC_SYS_REBOOT_CMD                                     = 32769,
    MIPC_SYS_REBOOT_RSP                                     = 32770,

    /* set host configuration request */
    MIPC_SYS_SET_CONFIG_DIPC_CMD                            = 32771,
    MIPC_SYS_SET_CONFIG_DIPC_RSP                            = 32772,

    /* request to update the configuration */
    MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_CMD                    = 32773,
    MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_RSP                    = 32774,

    /* get host configuration request */
    MIPC_SYS_GET_CONFIG_DIPC_CMD                            = 32775,
    MIPC_SYS_GET_CONFIG_DIPC_RSP                            = 32776,

    /* set sAP time */
    MIPC_SYS_SET_TIME_CMD                                   = 32777,
    MIPC_SYS_SET_TIME_RSP                                   = 32778,

    /* set mcif cmd */
    MIPC_SYS_SEND_CONSYS_WIFI_CMD                           = 32779,
    MIPC_SYS_SEND_CONSYS_WIFI_RSP                           = 32780,

    /* set mcif cmd */
    MIPC_SYS_SEND_CONSYS_BT_CMD                             = 32781,
    MIPC_SYS_SEND_CONSYS_BT_RSP                             = 32782,

    /* This command is used to send AT command to the registered host. */
    MIPC_SYS_AT_CMD                                         = 32783,
    MIPC_SYS_AT_RSP                                         = 32784,

    /* This command is used to send forward AT command to the registered host. */
    MIPC_SYS_FORWARD_AT_CMD                                 = 32785,
    MIPC_SYS_FORWARD_AT_RSP                                 = 32786,


};

    /* MIPC_MSG.SYS_GET_INFO_REQ */
enum mipc_sys_get_info_req_tlv_enum {
    mipc_sys_get_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_INFO_CNF */
enum mipc_sys_get_info_cnf_tlv_enum {
    mipc_sys_get_info_cnf_tlv_NONE = 0,
    /* phase out, please use SYS_CELLULAR_CLASS_V1. A bitmask that represents what cellular technology are supported */
    /* type = bitmap, refer to SYS_CAP_CELLULAR_CLASS */
    MIPC_SYS_GET_INFO_CNF_T_SYS_CELLULAR_CLASS              = 0x100,
    /* The presence of circuit voice service, and how such service interacts with data service */
    /* type = uint8_t, refer to SYS_VOICE_CLASS */
    MIPC_SYS_GET_INFO_CNF_T_VOICE_CLASS                     = 0x101,
    /* A bitmap indicating the supported SIM types */
    /* type = bitmap, refer to SYS_SIM_CLASS */
    MIPC_SYS_GET_INFO_CNF_T_SIM_CLASS                       = 0x102,
    /* A bitmap that represents which radio technologies are supported by the function */
    /* type = bitmap, refer to SYS_CAP_DATA */
    MIPC_SYS_GET_INFO_CNF_T_DATA_CLASS                      = 0x103,
    /* A bitmap that represents which GSM bands are supported */
    /* type = bitmap, refer to SYS_CAP_GSM_BAND */
    MIPC_SYS_GET_INFO_CNF_T_GSM_BAND_CLASS                  = 0x104,
    /* A bitmap that represents which UMTS bands are supported */
    /* type = bitmap, refer to SYS_CAP_UMTS_BAND */
    MIPC_SYS_GET_INFO_CNF_T_UMTS_BAND_CLASS                 = 0x105,
    /* A bitmap that represents which TDS bands are supported */
    /* type = bitmap, refer to SYS_CAP_TDS_BAND */
    MIPC_SYS_GET_INFO_CNF_T_TDS_BAND_CLASS                  = 0x106,
    /* A bitmap that represents which CDMA bands are supported */
    /* type = bitmap, refer to SYS_CAP_CDMA_BAND */
    MIPC_SYS_GET_INFO_CNF_T_C2K_BAND_CLASS                  = 0x107,
    /* A bitmap indicating the supported LTE bands */
    /* type = struct, refer to sys_lte_band */
    MIPC_SYS_GET_INFO_CNF_T_LTE_BAND_CLASS                  = 0x108,
    /* A bitmap indicating the supported NR bands */
    /* type = struct, refer to sys_nr_band */
    MIPC_SYS_GET_INFO_CNF_T_NR_BAND_CLASS                   = 0x109,
    /* A bitmap indicating the supported types of SMS messages and directional flow */
    /* type = bitmap, refer to SYS_CAP_SMS */
    MIPC_SYS_GET_INFO_CNF_T_SMS_CAPS                        = 0x10A,
    /* A bitmap that represents the control capabilities that the device supports */
    /* type = bitmap, refer to SYS_CAP_CTRL */
    MIPC_SYS_GET_INFO_CNF_T_CTRL_CAPS                       = 0x10B,
    /* A bitmap indicating the supported authentication algorithm */
    /* type = bitmap, refer to SYS_AUTH_ALGO */
    MIPC_SYS_GET_INFO_CNF_T_AUTH_ALGO_CAPS                  = 0x10C,
    /* a bitmap that represents the service capabilities that the device supports */
    /* type = bitmap, refer to SYS_CAP_SERVICE */
    MIPC_SYS_GET_INFO_CNF_T_SERVICE_CAPS                    = 0x10D,
    /* A structure the contains how many SIM and protocol are supported */
    /* type = struct, refer to sys_modem */
    MIPC_SYS_GET_INFO_CNF_T_MULTI_MD                        = 0x10E,
    /* Device ID (IMEI) */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_DEVICE_ID                       = 0x810F,
    /* Manufacturer */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_MANUFCTR                        = 0x110,
    /* Firmware version */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_FIRMWARE                        = 0x111,
    /* Hardware version */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_HARDWARE                        = 0x112,
    /* maximum concurrent data sessions */
    /* type = uint16_t */
    MIPC_SYS_GET_INFO_CNF_T_MAX_ACTIVE_CTXT                 = 0x113,
    /* current protocol id */
    /* type = uint16_t */
    MIPC_SYS_GET_INFO_CNF_T_EXECUTOR_IDX                    = 0x114,
    /* custom data class name */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_CUSTOM_CLASS_NAME               = 0x115,
    /* product type */
    /* type = uint8_t, refer to SYS_PRODUCT_TYPE */
    MIPC_SYS_GET_INFO_CNF_T_PRODUCT_TYPE                    = 0x116,
    /* phase out, please use ESN_V1. Electronic Serial Num. Hex string type,fixed len */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_ESN                             = 0x8117,
    /* International Mobile Equipment Identity software version; the string type is fixed length */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_IMEISV                          = 0x8118,
    /* phase out, please use MEID_V1. Mobile Equipment Identifier. Hex string type,fixed len */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_MEID                            = 0x8119,
    /* Custom firmware version */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_FIRMWARE_CUSTOM                 = 0x11A,
    /* Hardware chip ID */
    /* type = byte_array */
    MIPC_SYS_GET_INFO_CNF_T_HARDWARE_ID                     = 0x811B,
    /* Serial number */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_SERIAL_NUMBER                   = 0x11C,
    /* Project name */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_PROJECT_NAME                    = 0x11D,
    /* Flavor name */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_FLAVOR_NAME                     = 0x11E,
    /* Electronic Serial Num. */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_ESN_V1                          = 0x811F,
    /* Mobile Equipment Identifier. */
    /* type = string */
    MIPC_SYS_GET_INFO_CNF_T_MEID_V1                         = 0x8120,
    /* A bitmask that represents what cellular technology are supported */
    /* type = bitmap, refer to SYS_CAP_CELLULAR_CLASS */
    MIPC_SYS_GET_INFO_CNF_T_SYS_CELLULAR_CLASS_V1           = 0x121,
};

    /* MIPC_MSG.SYS_AT_REQ */
enum mipc_sys_at_req_tlv_enum {
    mipc_sys_at_req_tlv_NONE = 0,
    /* AT command string to be executed */
    /* type = string */
    MIPC_SYS_AT_REQ_T_ATCMD                                 = 0x8100,
};

    /* MIPC_MSG.SYS_AT_CNF */
enum mipc_sys_at_cnf_tlv_enum {
    mipc_sys_at_cnf_tlv_NONE = 0,
    /* AT command execution result */
    /* type = string */
    MIPC_SYS_AT_CNF_T_ATCMD                                 = 0x8100,
};

    /* MIPC_MSG.SYS_REBOOT_REQ */
enum mipc_sys_reboot_req_tlv_enum {
    mipc_sys_reboot_req_tlv_NONE = 0,
    /* It specifies the reboot mode */
    /* type = uint8_t, refer to SYS_REBOOT_MODE */
    MIPC_SYS_REBOOT_REQ_T_MODE                              = 0x100,
    /* Download Timeout (unit: second, only valid when mode is DOWNLOAD or SETTIMEOUT); Default value is 0 */
    /* type = uint32_t */
    MIPC_SYS_REBOOT_REQ_T_TIMEOUT                           = 0x101,
};

    /* MIPC_MSG.SYS_REBOOT_CNF */
enum mipc_sys_reboot_cnf_tlv_enum {
    mipc_sys_reboot_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MAPPING_REQ */
enum mipc_sys_get_mapping_req_tlv_enum {
    mipc_sys_get_mapping_req_tlv_NONE = 0,
    /* type = uint8_t, refer to SYS_SIM_PS */
    MIPC_SYS_GET_MAPPING_REQ_T_MSG                          = 0x100,
};

    /* MIPC_MSG.SYS_GET_MAPPING_CNF */
enum mipc_sys_get_mapping_cnf_tlv_enum {
    mipc_sys_get_mapping_cnf_tlv_NONE = 0,
    /* the number of list element */
    /* type = uint8_t */
    MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_COUNT                = 0x100,
    /* mapping list elements. each element represents a sim to ps mapping */
    /* type = struct_array, refer to sys_mapping */
    MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_LIST                 = 0x101,
    /* mapping list elements. each element represents a sim to ps mapping */
    /* type = uint32_t */
    MIPC_SYS_GET_MAPPING_CNF_T_MAPPING_TLV_ARRAY            = 0x102,
};

    /* MIPC_MSG.SYS_SET_MAPPING_REQ */
enum mipc_sys_set_mapping_req_tlv_enum {
    mipc_sys_set_mapping_req_tlv_NONE = 0,
    /* the number of list element; Default value is 0 */
    /* type = uint8_t */
    MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_COUNT                = 0x100,
    /* mapping list elements. each element represents a sim to ps mapping */
    /* type = struct_array, refer to sys_mapping */
    MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_LIST                 = 0x101,
    /* mapping list elements. each element represents a sim to ps mapping */
    /* type = uint32_t */
    MIPC_SYS_SET_MAPPING_REQ_T_MAPPING_TLV_ARRAY            = 0x102,
};

    /* MIPC_MSG.SYS_SET_MAPPING_CNF */
enum mipc_sys_set_mapping_cnf_tlv_enum {
    mipc_sys_set_mapping_cnf_tlv_NONE = 0,
    /* the number of list element */
    /* type = uint8_t */
    MIPC_SYS_SET_MAPPING_CNF_T_MAPPING_COUNT                = 0x100,
    /* mapping list elements. each element represents a sim to ps mapping */
    /* type = struct_array, refer to sys_mapping */
    MIPC_SYS_SET_MAPPING_CNF_T_MAPPING_LIST                 = 0x101,
    /* mapping list elements. each element represents a sim to ps mapping */
    /* type = uint32_t */
    MIPC_SYS_SET_MAPPING_CNF_T_MAPPING_TLV_ARRAY            = 0x102,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_NUM_REQ */
enum mipc_sys_get_thermal_sensor_num_req_tlv_enum {
    mipc_sys_get_thermal_sensor_num_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_NUM_CNF */
enum mipc_sys_get_thermal_sensor_num_cnf_tlv_enum {
    mipc_sys_get_thermal_sensor_num_cnf_tlv_NONE = 0,
    /* The number of sensors */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_NUM_CNF_T_NUM               = 0x100,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_INFO_REQ */
enum mipc_sys_get_thermal_sensor_info_req_tlv_enum {
    mipc_sys_get_thermal_sensor_info_req_tlv_NONE = 0,
    /* Sensor ID (start from zero). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_REQ_T_ID               = 0x100,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_INFO_CNF */
enum mipc_sys_get_thermal_sensor_info_cnf_tlv_enum {
    mipc_sys_get_thermal_sensor_info_cnf_tlv_NONE = 0,
    /* Sensor ID (start from zero). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_ID               = 0x100,
    /* Sensor name. */
    /* type = string */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_NAME             = 0x101,
    /* Sensor type (0 for temperature). */
    /* type = uint8_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_TYPE             = 0x102,
    /* Type of measurement supported by the sensor (6 for periodic). */
    /* type = uint8_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MEAS_TYPE        = 0x103,
    /* Sensor min temperature (unit: /10degC). */
    /* type = int32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MIN_TEMPATURE    = 0x104,
    /* Max alarm temperature [if any valid rising alarm] (unit: /10 degC). */
    /* type = int32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MAX_TEMPATURE    = 0x105,
    /* Sensor accuracy (unit: /10 degC). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_ACCURACY         = 0x106,
    /* Worst case temperature resolution for the sensor (unit: /10 degC). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_RESOLUTION       = 0x107,
    /* Warning alarm temperature [if any valid rising alarm] (unit: /10 degC). */
    /* type = int32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_WARN_TEMPATURE   = 0x108,
    /* Thermal protection temperature [modem self protection] (unit: /10 degC). */
    /* type = int32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_HW_SHUTDOWN_TEMPERATURE = 0x109,
    /* Minimum sampling period (interval) (unit: ms). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF_T_MIN_SAMPLING_PERIOD = 0x10A,
};

    /* MIPC_MSG.SYS_SET_THERMAL_SENSOR_REQ */
enum mipc_sys_set_thermal_sensor_req_tlv_enum {
    mipc_sys_set_thermal_sensor_req_tlv_NONE = 0,
    /* [PAHSE OUT]please use CONFIG_TLV_ARRAY instead of CONFIG_COUNT&CONFIG_LIST */
    /* type = uint8_t */
    MIPC_SYS_SET_THERMAL_SENSOR_REQ_T_CONFIG_COUNT          = 0x100,
    /* [PAHSE OUT]please use CONFIG_TLV_ARRAY instead of CONFIG_COUNT&CONFIG_LIST */
    /* type = struct_array, refer to sys_thermal_sensor_config */
    MIPC_SYS_SET_THERMAL_SENSOR_REQ_T_CONFIG_LIST           = 0x101,
    /* Threshold configuration TLV array of the target sensor and target alarm */
    /* type = struct, refer to sys_thermal_sensor_config_e */
    MIPC_SYS_SET_THERMAL_SENSOR_REQ_T_CONFIG_E              = 0x102,
    MIPC_SYS_SET_THERMAL_SENSOR_REQ_T_CONFIG_TLV_ARRAY      = 0x102,
};

    /* MIPC_MSG.SYS_SET_THERMAL_SENSOR_CNF */
enum mipc_sys_set_thermal_sensor_cnf_tlv_enum {
    mipc_sys_set_thermal_sensor_cnf_tlv_NONE = 0,
    /* [PAHSE OUT]please use CONFIG_E instead of CONFIG_COUNT&CONFIG_LIST */
    /* type = uint8_t */
    MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_COUNT          = 0x100,
    /* [PAHSE OUT]please use CONFIG_E instead of CONFIG_COUNT&CONFIG_LIST */
    /* type = struct_array, refer to sys_thermal_sensor_config */
    MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_LIST           = 0x101,
    /* Configuration of the target sensor and target alarm. */
    /* type = struct, refer to sys_thermal_sensor_config_e */
    MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_E              = 0x102,
    MIPC_SYS_SET_THERMAL_SENSOR_CNF_T_CONFIG_TLV_ARRAY      = 0x102,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_REQ */
enum mipc_sys_get_thermal_sensor_req_tlv_enum {
    mipc_sys_get_thermal_sensor_req_tlv_NONE = 0,
    /* Sensor ID (start from zero). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_REQ_T_ID                    = 0x100,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_CNF */
enum mipc_sys_get_thermal_sensor_cnf_tlv_enum {
    mipc_sys_get_thermal_sensor_cnf_tlv_NONE = 0,
    /* Current temperature (unit: /10 degC). */
    /* type = int32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_CNF_T_TEMPERATURE           = 0x100,
};

    /* MIPC_MSG.SYS_GET_THERMAL_ACTUATOR_NUM_REQ */
enum mipc_sys_get_thermal_actuator_num_req_tlv_enum {
    mipc_sys_get_thermal_actuator_num_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_THERMAL_ACTUATOR_NUM_CNF */
enum mipc_sys_get_thermal_actuator_num_cnf_tlv_enum {
    mipc_sys_get_thermal_actuator_num_cnf_tlv_NONE = 0,
    /* The number of thermal actuators. */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_NUM_CNF_T_NUM             = 0x100,
};

    /* MIPC_MSG.SYS_SET_THERMAL_ACTUATOR_REQ */
enum mipc_sys_set_thermal_actuator_req_tlv_enum {
    mipc_sys_set_thermal_actuator_req_tlv_NONE = 0,
    /* Actuator ID (start from one). */
    /* type = uint32_t */
    MIPC_SYS_SET_THERMAL_ACTUATOR_REQ_T_ID                  = 0x100,
    /* Throttle level. */
    /* type = uint32_t */
    MIPC_SYS_SET_THERMAL_ACTUATOR_REQ_T_LEVEL               = 0x101,
    /* Throttle state. */
    /* type = uint32_t */
    MIPC_SYS_SET_THERMAL_ACTUATOR_REQ_T_STATE               = 0x102,
};

    /* MIPC_MSG.SYS_SET_THERMAL_ACTUATOR_CNF */
enum mipc_sys_set_thermal_actuator_cnf_tlv_enum {
    mipc_sys_set_thermal_actuator_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_THERMAL_ACTUATOR_INFO_REQ */
enum mipc_sys_get_thermal_actuator_info_req_tlv_enum {
    mipc_sys_get_thermal_actuator_info_req_tlv_NONE = 0,
    /* Actuator ID (start from one). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_REQ_T_ID             = 0x100,
};

    /* MIPC_MSG.SYS_GET_THERMAL_ACTUATOR_INFO_CNF */
enum mipc_sys_get_thermal_actuator_info_cnf_tlv_enum {
    mipc_sys_get_thermal_actuator_info_cnf_tlv_NONE = 0,
    /* Actuator ID (start from one). */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_ID             = 0x100,
    /* Thermal actuactor name. */
    /* type = string */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_NAME           = 0x101,
    /* Total throttling level of this actuator. */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_TOTAL_LEVEL    = 0x102,
    /* Current throttling level of this actuator */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_CURRENT_LEVEL  = 0x103,
    /* Response the same value as CURRENT_LEVEL. */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_USER_IMPACT    = 0x104,
    /* Response the same value as CURRENT_LEVEL. */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF_T_EFFICIENCY     = 0x105,
};

    /* MIPC_MSG.SYS_SET_CONFIG_REQ */
enum mipc_sys_set_config_req_tlv_enum {
    mipc_sys_set_config_req_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_SET_CONFIG_REQ_T_CLASS                         = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_SYS_SET_CONFIG_REQ_T_TYPE                          = 0x101,
    /* the configuration data */
    /* type = byte_array */
    MIPC_SYS_SET_CONFIG_REQ_T_DATA                          = 0x8102,
};

    /* MIPC_MSG.SYS_SET_CONFIG_CNF */
enum mipc_sys_set_config_cnf_tlv_enum {
    mipc_sys_set_config_cnf_tlv_NONE = 0,
    /* the response data */
    /* type = byte_array */
    MIPC_SYS_SET_CONFIG_CNF_T_DATA                          = 0x8100,
};

    /* MIPC_MSG.SYS_GET_CONFIG_REQ */
enum mipc_sys_get_config_req_tlv_enum {
    mipc_sys_get_config_req_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_GET_CONFIG_REQ_T_CLASS                         = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_SYS_GET_CONFIG_REQ_T_TYPE                          = 0x101,
};

    /* MIPC_MSG.SYS_GET_CONFIG_CNF */
enum mipc_sys_get_config_cnf_tlv_enum {
    mipc_sys_get_config_cnf_tlv_NONE = 0,
    /* the response data */
    /* type = byte_array */
    MIPC_SYS_GET_CONFIG_CNF_T_DATA                          = 0x8100,
};

    /* MIPC_MSG.SYS_REG_CONFIG_REQ */
enum mipc_sys_reg_config_req_tlv_enum {
    mipc_sys_reg_config_req_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_REG_CONFIG_REQ_T_CLASS                         = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_SYS_REG_CONFIG_REQ_T_TYPE                          = 0x101,
};

    /* MIPC_MSG.SYS_REG_CONFIG_CNF */
enum mipc_sys_reg_config_cnf_tlv_enum {
    mipc_sys_reg_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_ADPCLK_REQ */
enum mipc_sys_set_adpclk_req_tlv_enum {
    mipc_sys_set_adpclk_req_tlv_NONE = 0,
    /* enable or disable */
    /* type = uint8_t, refer to SYS_ADPCLK_STATE */
    MIPC_SYS_SET_ADPCLK_REQ_T_STATE                         = 0x100,
};

    /* MIPC_MSG.SYS_SET_ADPCLK_CNF */
enum mipc_sys_set_adpclk_cnf_tlv_enum {
    mipc_sys_set_adpclk_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_ADPCLK_REQ */
enum mipc_sys_get_adpclk_req_tlv_enum {
    mipc_sys_get_adpclk_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_ADPCLK_CNF */
enum mipc_sys_get_adpclk_cnf_tlv_enum {
    mipc_sys_get_adpclk_cnf_tlv_NONE = 0,
    /* type = uint8_t */
    MIPC_SYS_GET_ADPCLK_CNF_T_FREQ_INFO_COUNT               = 0x100,
    /* type = struct_array, refer to sys_adpclk_freq_info */
    MIPC_SYS_GET_ADPCLK_CNF_T_FREQ_INFO_LIST                = 0x101,
    /* type = struct, refer to sys_adpclk_freq_info */
    MIPC_SYS_GET_ADPCLK_CNF_T_FREQ_INFO_TLV_ARRAY           = 0x102,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_MODE_REQ */
enum mipc_sys_set_md_log_mode_req_tlv_enum {
    mipc_sys_set_md_log_mode_req_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_SET_MD_LOG_MODE_REQ_T_MODE                     = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_MODE_CNF */
enum mipc_sys_set_md_log_mode_cnf_tlv_enum {
    mipc_sys_set_md_log_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_MODE_REQ */
enum mipc_sys_get_md_log_mode_req_tlv_enum {
    mipc_sys_get_md_log_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_MODE_CNF */
enum mipc_sys_get_md_log_mode_cnf_tlv_enum {
    mipc_sys_get_md_log_mode_cnf_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_GET_MD_LOG_MODE_CNF_T_MODE                     = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_LEVEL_REQ */
enum mipc_sys_set_md_log_level_req_tlv_enum {
    mipc_sys_set_md_log_level_req_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_SET_MD_LOG_LEVEL_REQ_T_LEVEL                   = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_LEVEL_CNF */
enum mipc_sys_set_md_log_level_cnf_tlv_enum {
    mipc_sys_set_md_log_level_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_LEVEL_REQ */
enum mipc_sys_get_md_log_level_req_tlv_enum {
    mipc_sys_get_md_log_level_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_LEVEL_CNF */
enum mipc_sys_get_md_log_level_cnf_tlv_enum {
    mipc_sys_get_md_log_level_cnf_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_GET_MD_LOG_LEVEL_CNF_T_LEVEL                   = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_LOCATION_REQ */
enum mipc_sys_set_md_log_location_req_tlv_enum {
    mipc_sys_set_md_log_location_req_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_SET_MD_LOG_LOCATION_REQ_T_ENABLE               = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_LOCATION_CNF */
enum mipc_sys_set_md_log_location_cnf_tlv_enum {
    mipc_sys_set_md_log_location_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_LOCATION_REQ */
enum mipc_sys_get_md_log_location_req_tlv_enum {
    mipc_sys_get_md_log_location_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_LOCATION_CNF */
enum mipc_sys_get_md_log_location_cnf_tlv_enum {
    mipc_sys_get_md_log_location_cnf_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_GET_MD_LOG_LOCATION_CNF_T_ENABLE               = 0x100,
};

    /* MIPC_MSG.SYS_WRITE_NVRAM_REQ */
enum mipc_sys_write_nvram_req_tlv_enum {
    mipc_sys_write_nvram_req_tlv_NONE = 0,
    /* Logical data item */
    /* type = uint32_t */
    MIPC_SYS_WRITE_NVRAM_REQ_T_FILE_IDX                     = 0x100,
    MIPC_SYS_WRITE_NVRAM_REQ_T_LID                          = 0x100,
    /* Record index */
    /* type = uint32_t */
    MIPC_SYS_WRITE_NVRAM_REQ_T_RECORD_IDX                   = 0x101,
    /* The data used to write to NVRAM. */
    /* type = byte_array */
    MIPC_SYS_WRITE_NVRAM_REQ_T_DATA                         = 0x8102,
};

    /* MIPC_MSG.SYS_WRITE_NVRAM_CNF */
enum mipc_sys_write_nvram_cnf_tlv_enum {
    mipc_sys_write_nvram_cnf_tlv_NONE = 0,
    /* Data length written success in NVRAM. */
    /* type = uint32_t */
    MIPC_SYS_WRITE_NVRAM_CNF_T_DATA_LEN                     = 0x100,
    /* type = uint32_t, refer to SYS_NVRAM_CAUSE */
    MIPC_SYS_WRITE_NVRAM_CNF_T_CAUSE                        = 0x101,
};

    /* MIPC_MSG.SYS_READ_NVRAM_REQ */
enum mipc_sys_read_nvram_req_tlv_enum {
    mipc_sys_read_nvram_req_tlv_NONE = 0,
    /* Logical data item */
    /* type = uint32_t */
    MIPC_SYS_READ_NVRAM_REQ_T_FILE_IDX                      = 0x100,
    MIPC_SYS_READ_NVRAM_REQ_T_LID                           = 0x100,
    /* Record index */
    /* type = uint32_t */
    MIPC_SYS_READ_NVRAM_REQ_T_RECORD_IDX                    = 0x101,
};

    /* MIPC_MSG.SYS_READ_NVRAM_CNF */
enum mipc_sys_read_nvram_cnf_tlv_enum {
    mipc_sys_read_nvram_cnf_tlv_NONE = 0,
    /* Data which is read from NVRAM */
    /* type = byte_array */
    MIPC_SYS_READ_NVRAM_CNF_T_DATA                          = 0x8100,
    /* type = uint32_t, refer to SYS_NVRAM_CAUSE */
    MIPC_SYS_READ_NVRAM_CNF_T_CAUSE                         = 0x101,
};

    /* MIPC_MSG.SYS_AUTH_REQ */
enum mipc_sys_auth_req_tlv_enum {
    mipc_sys_auth_req_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_AUTH_REQ_T_OP                                  = 0x100,
    /* type = byte_array */
    MIPC_SYS_AUTH_REQ_T_ENCDATA                             = 0x8101,
};

    /* MIPC_MSG.SYS_AUTH_CNF */
enum mipc_sys_auth_cnf_tlv_enum {
    mipc_sys_auth_cnf_tlv_NONE = 0,
    /* type = byte_array */
    MIPC_SYS_AUTH_CNF_T_RAND                                = 0x8100,
};

    /* MIPC_MSG.SYS_SET_DAT_REQ */
enum mipc_sys_set_dat_req_tlv_enum {
    mipc_sys_set_dat_req_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_SET_DAT_REQ_T_INDEX                            = 0x100,
};

    /* MIPC_MSG.SYS_SET_DAT_CNF */
enum mipc_sys_set_dat_cnf_tlv_enum {
    mipc_sys_set_dat_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_DAT_REQ */
enum mipc_sys_get_dat_req_tlv_enum {
    mipc_sys_get_dat_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_DAT_CNF */
enum mipc_sys_get_dat_cnf_tlv_enum {
    mipc_sys_get_dat_cnf_tlv_NONE = 0,
    /* type = uint32_t */
    MIPC_SYS_GET_DAT_CNF_T_INDEX                            = 0x100,
};

    /* MIPC_MSG.SYS_MCF_REQ */
enum mipc_sys_mcf_req_tlv_enum {
    mipc_sys_mcf_req_tlv_NONE = 0,
    /* MCF function-option */
    /* type = uint8_t, refer to SYS_MCF_OP */
    MIPC_SYS_MCF_REQ_T_OP                                   = 0x10A,
    /* MCF file type */
    /* type = uint8_t, refer to SYS_MCF_CONFIG_TYPE */
    MIPC_SYS_MCF_REQ_T_CONFIG_TYPE                          = 0x10B,
    /* LID number list. If user don't provide this field, modem will dump all MCF LIDs. Maximum support number is 32. */
    /* type = string */
    MIPC_SYS_MCF_REQ_T_DUMP_LIDS                            = 0x10C,
    /* MCF file path type; Default type is OTA */
    /* type = uint8_t, refer to SYS_MCF_PATH_TYPE */
    MIPC_SYS_MCF_REQ_T_PATH_TYPE                            = 0x10D,
    /* Whether to trigger DSBP; 0: Don't trigger DSBP; 1: Trigger DSBP; Default value is 0. */
    /* type = uint8_t */
    MIPC_SYS_MCF_REQ_T_TRIGGER_DSBP                         = 0x10E,
    /*
      MCF action;
      When it is used to apply OTA file with ini(config) file, 0 means don't need to read ini and 1 means read ini.
      When it is used for reading/writing GID, refer to SYS_MCF_GID_ACTION;
      Default value is 0.
    */
    /* type = uint8_t */
    MIPC_SYS_MCF_REQ_T_ACTION                               = 0x10F,
    /* Method for searching LID parameters; 1: GID */
    /* type = uint8_t */
    MIPC_SYS_MCF_REQ_T_FORMAT                               = 0x110,
    /* GID number of LID parameters; Default value is 0. */
    /* type = uint32_t */
    MIPC_SYS_MCF_REQ_T_NUM                                  = 0x111,
    /* The length of a LID parameters; Default value is 0. */
    /* type = uint8_t */
    MIPC_SYS_MCF_REQ_T_LEN                                  = 0x112,
    /* Record ID of NVRAM LID; In OTA by OP case, the value of this field should be 1; Default value is 0. */
    /* type = uint16_t */
    MIPC_SYS_MCF_REQ_T_REC_ID                               = 0x113,
    /*
      The written value of a LID parameters;
      If user wants to set 0x12345678, the value of this field should be 0x78563412;
      Default value is 0.
    */
    /* type = byte_array */
    MIPC_SYS_MCF_REQ_T_VALUE                                = 0x114,
    /*
      File path and file name;
      If the value of this field is null character, modem will clear the apply list to default/reset status;
      If the value of this field is not null character, modem will append it to apply list;
      Default value is null character.
    */
    /* type = string */
    MIPC_SYS_MCF_REQ_T_CONFIG1                              = 0x115,
    /* Array index; Use ',' to seperate array index. E.g. "0,0,0"; Default value is null character. */
    /* type = string */
    MIPC_SYS_MCF_REQ_T_CONFIG                               = 0x116,
    /*
      Whether need to reset;
      0: Don't do reset;
      1: Do reset;
      Default value is 1.
    */
    /* type = uint8_t */
    MIPC_SYS_MCF_REQ_T_IS_RESET                             = 0x117,
};

    /* MIPC_MSG.SYS_MCF_CNF */
enum mipc_sys_mcf_cnf_tlv_enum {
    mipc_sys_mcf_cnf_tlv_NONE = 0,
    /* MCF flow result; 0: Success */
    /* type = uint32_t */
    MIPC_SYS_MCF_CNF_T_MCF_RESULT                           = 0x100,
    /* DSBP processing result; 0: Success */
    /* type = uint32_t */
    MIPC_SYS_MCF_CNF_T_DSBP_RESULT                          = 0x101,
    /* MCF function-option; */
    /* type = uint8_t, refer to SYS_MCF_OP */
    MIPC_SYS_MCF_CNF_T_OP                                   = 0x10A,
    /* MCF file type */
    /* type = uint8_t, refer to SYS_MCF_CONFIG_TYPE */
    MIPC_SYS_MCF_CNF_T_CONFIG_TYPE                          = 0x10B,
    /* MCF file path type; Default type is OTA */
    /* type = uint8_t, refer to SYS_MCF_PATH_TYPE */
    MIPC_SYS_MCF_CNF_T_PATH_TYPE                            = 0x10D,
    /*
      MCF action;
      When it is used to apply OTA file with ini(config) file, 0 means don't need to read ini and 1 means read ini.
      When it is used for reading/writing GID, refer to SYS_MCF_GID_ACTION;
      Default value is 0.
    */
    /* type = uint8_t */
    MIPC_SYS_MCF_CNF_T_ACTION                               = 0x10F,
    /* Method for searching LID parameters; 1: GID */
    /* type = uint8_t */
    MIPC_SYS_MCF_CNF_T_FORMAT                               = 0x110,
    /* The length of a LID parameters */
    /* type = uint8_t */
    MIPC_SYS_MCF_CNF_T_LEN                                  = 0x112,
    /* The written value of a LID parameters. */
    /* type = byte_array */
    MIPC_SYS_MCF_CNF_T_VALUE                                = 0x114,
    /* File path and file name */
    /* type = string */
    MIPC_SYS_MCF_CNF_T_CONFIG1                              = 0x115,
};

    /* MIPC_MSG.SYS_SET_FCC_LOCK_REQ */
enum mipc_sys_set_fcc_lock_req_tlv_enum {
    mipc_sys_set_fcc_lock_req_tlv_NONE = 0,
    /* type = uint8_t */
    MIPC_SYS_SET_FCC_LOCK_REQ_T_MODE                        = 0x100,
};

    /* MIPC_MSG.SYS_SET_FCC_LOCK_CNF */
enum mipc_sys_set_fcc_lock_cnf_tlv_enum {
    mipc_sys_set_fcc_lock_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_TIME_REQ */
enum mipc_sys_set_time_req_tlv_enum {
    mipc_sys_set_time_req_tlv_NONE = 0,
    /* Year */
    /* type = uint32_t */
    MIPC_SYS_SET_TIME_REQ_T_YEAR                            = 0x100,
    /* Month */
    /* type = uint32_t */
    MIPC_SYS_SET_TIME_REQ_T_MONTH                           = 0x101,
    /* Day */
    /* type = uint32_t */
    MIPC_SYS_SET_TIME_REQ_T_DAY                             = 0x102,
    /* Hour */
    /* type = uint32_t */
    MIPC_SYS_SET_TIME_REQ_T_HOUR                            = 0x103,
    /* Minute */
    /* type = uint32_t */
    MIPC_SYS_SET_TIME_REQ_T_MINUTE                          = 0x104,
    /* Second */
    /* type = uint32_t */
    MIPC_SYS_SET_TIME_REQ_T_SECOND                          = 0x105,
    /* Time zone offset minutes */
    /* type = int32_t */
    MIPC_SYS_SET_TIME_REQ_T_TZ_OFF_MIN                      = 0x106,
};

    /* MIPC_MSG.SYS_SET_TIME_CNF */
enum mipc_sys_set_time_cnf_tlv_enum {
    mipc_sys_set_time_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_TIME_REQ */
enum mipc_sys_get_time_req_tlv_enum {
    mipc_sys_get_time_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_TIME_CNF */
enum mipc_sys_get_time_cnf_tlv_enum {
    mipc_sys_get_time_cnf_tlv_NONE = 0,
    /* Year */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_YEAR                            = 0x100,
    /* Month */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_MONTH                           = 0x101,
    /* Day */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_DAY                             = 0x102,
    /* Hour */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_HOUR                            = 0x103,
    /* Minute */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_MINUTE                          = 0x104,
    /* Second */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_SECOND                          = 0x105,
    /* Timestamp (FRC) */
    /* type = uint32_t */
    MIPC_SYS_GET_TIME_CNF_T_TIMESTAMP                       = 0x106,
    /* Time zone offset minutes */
    /* type = int32_t */
    MIPC_SYS_GET_TIME_CNF_T_TZ_OFF_MIN                      = 0x107,
};

    /* MIPC_MSG.SYS_SET_SAR_REQ */
enum mipc_sys_set_sar_req_tlv_enum {
    mipc_sys_set_sar_req_tlv_NONE = 0,
    /* SAR mode */
    /* type = uint8_t, refer to SYS_SAR_MODE */
    MIPC_SYS_SET_SAR_REQ_T_MODE                             = 0x100,
    /* SAR scenario index */
    /* type = uint32_t */
    MIPC_SYS_SET_SAR_REQ_T_INDEX                            = 0x101,
};

    /* MIPC_MSG.SYS_SET_SAR_CNF */
enum mipc_sys_set_sar_cnf_tlv_enum {
    mipc_sys_set_sar_cnf_tlv_NONE = 0,
    /* SAR mode */
    /* type = uint8_t, refer to SYS_SAR_MODE */
    MIPC_SYS_SET_SAR_CNF_T_MODE                             = 0x100,
    /* SAR scenario index */
    /* type = uint32_t */
    MIPC_SYS_SET_SAR_CNF_T_INDEX                            = 0x101,
};

    /* MIPC_MSG.SYS_GET_SAR_REQ */
enum mipc_sys_get_sar_req_tlv_enum {
    mipc_sys_get_sar_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_SAR_CNF */
enum mipc_sys_get_sar_cnf_tlv_enum {
    mipc_sys_get_sar_cnf_tlv_NONE = 0,
    /* SAR mode */
    /* type = uint8_t, refer to SYS_SAR_MODE */
    MIPC_SYS_GET_SAR_CNF_T_MODE                             = 0x100,
    /* SAR scenario index */
    /* type = uint32_t */
    MIPC_SYS_GET_SAR_CNF_T_INDEX                            = 0x101,
};

    /* MIPC_MSG.SYS_SET_POWER_SAVING_REQ */
enum mipc_sys_set_power_saving_req_tlv_enum {
    mipc_sys_set_power_saving_req_tlv_NONE = 0,
    /* 0: disable, 1: enable; Default value is 0 */
    /* type = uint8_t */
    MIPC_SYS_SET_POWER_SAVING_REQ_T_MODE                    = 0x100,
};

    /* MIPC_MSG.SYS_SET_POWER_SAVING_CNF */
enum mipc_sys_set_power_saving_cnf_tlv_enum {
    mipc_sys_set_power_saving_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_CONNECTIVITY_STATISTICS_REQ */
enum mipc_sys_connectivity_statistics_req_tlv_enum {
    mipc_sys_connectivity_statistics_req_tlv_NONE = 0,
    /* once HOST carry this TLV(no matter what value), means HOST need read statistic info & period(s), DEVICE would carry the info in CNF; Default value is 0 */
    /* type = uint8_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_READ_FLAG        = 0x100,
    /* once HOST carry this TLV(no matter what value), means HOST need trigger reset statistic counters, then start statistic, DEVICE would stop statistic until period expired(none 0) or receive STOP from HOST; Default value is 0 */
    /* type = uint8_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_START            = 0x101,
    /* once HOST carry this TLV(no matter what value), means DEVICE should stop statistic now, but dont reset the counters; Default value is 0 */
    /* type = uint8_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_STOP             = 0x102,
    /* once HOST carry this TLV, means DEVICE should set the PERIOD(uint:second; max value is 86400); Default value is 0 */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_REQ_T_PERIOD_VALUE     = 0x103,
};

    /* MIPC_MSG.SYS_CONNECTIVITY_STATISTICS_CNF */
enum mipc_sys_connectivity_statistics_cnf_tlv_enum {
    mipc_sys_connectivity_statistics_cnf_tlv_NONE = 0,
    /* indicate the total number of SMS successfully transmitted during the collection period */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_SMS_TX_COUNTER   = 0x100,
    /* indicate the total number of SMS successfully received during the collection period */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_SMS_RX_COUNTER   = 0x101,
    /* Phase out. Indicate the total amount of data (IP / non-IP) transmitted during the collection period expressed in bytes */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_TX_DATA          = 0x102,
    /* Phase out. Indicate the total amount of data (IP / non-IP) received during the collection period expressed in bytes */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_RX_DATA          = 0x103,
    /* The maximum IP message size that is used during the collection period(uint: B) */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_MAX_MESSAGE_SIZE = 0x104,
    /* The average IP message size that is used during the collection period(uint: B) */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_AVERAGE_MESSAGE_SIZE = 0x105,
    /* the period that set before, uint:second */
    /* type = uint32_t */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_PERIOD_VALUE     = 0x106,
    /* Indicate the total amount of data (IP/non-IP) transmitted during the collection period expressed in bytes. Value size is 8 byte */
    /* type = byte_array */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_TX_DATA_EXT      = 0x107,
    /* Indicate the total amount of data (IP/non-IP) received during the collection period expressed in bytes. Value size is 8 byte */
    /* type = byte_array */
    MIPC_SYS_CONNECTIVITY_STATISTICS_CNF_T_RX_DATA_EXT      = 0x108,
};

    /* MIPC_MSG.SYS_QUERY_SBP_REQ */
enum mipc_sys_query_sbp_req_tlv_enum {
    mipc_sys_query_sbp_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_QUERY_SBP_CNF */
enum mipc_sys_query_sbp_cnf_tlv_enum {
    mipc_sys_query_sbp_cnf_tlv_NONE = 0,
    /* System SBP ID */
    /* type = uint32_t */
    MIPC_SYS_QUERY_SBP_CNF_T_SBP_ID                         = 0x100,
    /* SIM system SBP ID */
    /* type = uint32_t */
    MIPC_SYS_QUERY_SBP_CNF_T_SIM_SBP_ID                     = 0x101,
    /* SBP feature */
    /* type = string */
    MIPC_SYS_QUERY_SBP_CNF_T_SBP_FEATURE_BYTE               = 0x102,
    /* SBP data */
    /* type = string */
    MIPC_SYS_QUERY_SBP_CNF_T_SBP_DATA_BYTE                  = 0x103,
};

    /* MIPC_MSG.SYS_SET_TX_IND_INTERVAL_REQ */
enum mipc_sys_set_tx_ind_interval_req_tlv_enum {
    mipc_sys_set_tx_ind_interval_req_tlv_NONE = 0,
    /* the value to set ind interval */
    /* type = uint8_t */
    MIPC_SYS_SET_TX_IND_INTERVAL_REQ_T_INTERVAL             = 0x100,
};

    /* MIPC_MSG.SYS_SET_TX_IND_INTERVAL_CNF */
enum mipc_sys_set_tx_ind_interval_cnf_tlv_enum {
    mipc_sys_set_tx_ind_interval_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_GEO_LOCATION_REQ */
enum mipc_sys_set_geo_location_req_tlv_enum {
    mipc_sys_set_geo_location_req_tlv_NONE = 0,
    /* request id, 0 ~ 7 */
    /* type = uint8_t, refer to SYS_ACCOUNT_ID */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCOUNT_ID              = 0x8100,
    /* 0: Do not broadcast this information to all IMS accounts. 1: Broadcast this information to all IMS accounts */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_BROADCAST_FLAG          = 0x101,
    /* latitude from GPS, 0 as failed */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_LATITUDE                = 0x8102,
    /* longitude from GPS, 0 as failed */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_LONGITUDE               = 0x8103,
    /* accurate from GPS, 0 as failed */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY                = 0x8104,
    /* Positioning method */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_METHOD                  = 0x8105,
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_CITY                    = 0x8106,
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_STATE                   = 0x8107,
    /* Zip code */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ZIP                     = 0x8108,
    /* Two-letter country code defined by ISO 3166-1 */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_COUNTRY_CODE            = 0x8109,
    /* UE Wi-Fi interface mac address */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_UE_WLAN_MAC             = 0x810A,
    /* confidence */
    /* type = uint32_t */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_CONFIDENCE              = 0x810B,
    /* altitude */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ALTITUDE                = 0x810C,
    /* accuracy semi Major Axis */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_SEMI_MAJOR_AXIS = 0x810D,
    /* accuracy semi Minor Axis */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_SEMI_MINOR_AXIS = 0x810E,
    /* accuracy vertical Axis */
    /* type = string */
    MIPC_SYS_SET_GEO_LOCATION_REQ_T_ACCURACY_VERTICAL_AXIS  = 0x810F,
};

    /* MIPC_MSG.SYS_SET_GEO_LOCATION_CNF */
enum mipc_sys_set_geo_location_cnf_tlv_enum {
    mipc_sys_set_geo_location_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_DSBP_REQ */
enum mipc_sys_set_dsbp_req_tlv_enum {
    mipc_sys_set_dsbp_req_tlv_NONE = 0,
    /* DSBP mode */
    /* type = uint8_t, refer to SYS_DSBP_MODE */
    MIPC_SYS_SET_DSBP_REQ_T_MODE                            = 0x100,
};

    /* MIPC_MSG.SYS_SET_DSBP_CNF */
enum mipc_sys_set_dsbp_cnf_tlv_enum {
    mipc_sys_set_dsbp_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SEND_SAR_IND_REQ */
enum mipc_sys_send_sar_ind_req_tlv_enum {
    mipc_sys_send_sar_ind_req_tlv_NONE = 0,
    /* SAR command type; the value of this field should be 0 or 1 */
    /* type = uint8_t */
    MIPC_SYS_SEND_SAR_IND_REQ_T_CMD_TYPE                    = 0x100,
    /* Command parameter */
    /* type = string */
    MIPC_SYS_SEND_SAR_IND_REQ_T_CMD_PARAM                   = 0x101,
};

    /* MIPC_MSG.SYS_SEND_SAR_IND_CNF */
enum mipc_sys_send_sar_ind_cnf_tlv_enum {
    mipc_sys_send_sar_ind_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SILENT_REBOOT_REQ */
enum mipc_sys_silent_reboot_req_tlv_enum {
    mipc_sys_silent_reboot_req_tlv_NONE = 0,
    /* 0 means normal mode(default mode), 1 means silent reboot. when silent reboot, MD will use the verified sim pin to auto verify sim pin; Default value is 0 */
    /* type = uint8_t */
    MIPC_SYS_SILENT_REBOOT_REQ_T_MODE                       = 0x100,
};

    /* MIPC_MSG.SYS_SILENT_REBOOT_CNF */
enum mipc_sys_silent_reboot_cnf_tlv_enum {
    mipc_sys_silent_reboot_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_MULTI_SIM_CONFIG_REQ */
enum mipc_sys_multi_sim_config_req_tlv_enum {
    mipc_sys_multi_sim_config_req_tlv_NONE = 0,
    /* 1 means single sim mode,2 dual sim mode */
    /* type = uint8_t */
    MIPC_SYS_MULTI_SIM_CONFIG_REQ_T_MODE                    = 0x101,
};

    /* MIPC_MSG.SYS_MULTI_SIM_CONFIG_CNF */
enum mipc_sys_multi_sim_config_cnf_tlv_enum {
    mipc_sys_multi_sim_config_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_REBOOT_SET_REQ */
enum mipc_sys_reboot_set_req_tlv_enum {
    mipc_sys_reboot_set_req_tlv_NONE = 0,
    /*
      0: Normal mode (default)
      1: Silent reboot
    */
    /* type = uint8_t */
    MIPC_SYS_REBOOT_SET_REQ_T_MODE                          = 0x100,
};

    /* MIPC_MSG.SYS_REBOOT_SET_CNF */
enum mipc_sys_reboot_set_cnf_tlv_enum {
    mipc_sys_reboot_set_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_THERMAL_ACTUATOR_REQ */
enum mipc_sys_get_thermal_actuator_req_tlv_enum {
    mipc_sys_get_thermal_actuator_req_tlv_NONE = 0,
    /* It indicates whether to get the number of actuators; Default value is false */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_GET_THERMAL_ACTUATOR_REQ_T_GET_ACTUATOR_NUM    = 0x100,
    /* The actuator ID which is used to acquire the actuator information; Default value is 0xffffffff */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_REQ_T_ACTUATOR_ID         = 0x101,
};

    /* MIPC_MSG.SYS_GET_THERMAL_ACTUATOR_CNF */
enum mipc_sys_get_thermal_actuator_cnf_tlv_enum {
    mipc_sys_get_thermal_actuator_cnf_tlv_NONE = 0,
    /* The number of actuators. */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_ACTUATOR_CNF_T_ACTUATOR_NUM        = 0x100,
    /* The actuator information of specific actuator. */
    /* type = struct, refer to sys_thermal_actuator_state_info */
    MIPC_SYS_GET_THERMAL_ACTUATOR_CNF_T_ACTUATOR_STATE_INFO = 0x101,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_RUNTIME_REQ */
enum mipc_sys_get_thermal_sensor_runtime_req_tlv_enum {
    mipc_sys_get_thermal_sensor_runtime_req_tlv_NONE = 0,
    /* It indicate if open get-autonomou mode. Default value is 0(false). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_REQ_T_GET_MD_AUTO_ENABLE = 0x100,
    /* Sensor ID (start from zero) which is used to acquire the trip point; Default value is 0xffffffff */
    /* type = uint32_t */
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_REQ_T_SENSOR_ID     = 0x101,
};

    /* MIPC_MSG.SYS_GET_THERMAL_SENSOR_RUNTIME_CNF */
enum mipc_sys_get_thermal_sensor_runtime_cnf_tlv_enum {
    mipc_sys_get_thermal_sensor_runtime_cnf_tlv_NONE = 0,
    /* Current autonomous mode = 0(false) or 1(true). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF_T_MD_AUTO_ENABLE = 0x100,
    /* Trip point information for specific sensor. */
    /* type = struct, refer to sys_thermal_trip_map */
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF_T_TRIP          = 0x101,
    MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF_T_TRIP_TLV_ARRAY = 0x101,
};

    /* MIPC_MSG.SYS_SET_THERMAL_RUNTIME_REQ */
enum mipc_sys_set_thermal_runtime_req_tlv_enum {
    mipc_sys_set_thermal_runtime_req_tlv_NONE = 0,
    /* It indicate if reset fonfig from NVRAM. Default value is 0(false). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_RESET_CFG_FROM_NV    = 0x100,
    /* It indicate if open md autonomou mode. Default value is 0xff. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_MD_AUTO_ENABLE       = 0x101,
    /* It give trip change information to modifiy sensor trip point in scheduling. */
    /* type = struct, refer to sys_thermal_trip_change */
    MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_TRIP_CHANGE          = 0x102,
    /* It indicate if save config to NVRAM. Default value is 0(false). */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_THERMAL_RUNTIME_REQ_T_SAVE_CFG_TO_NV       = 0x103,
};

    /* MIPC_MSG.SYS_SET_THERMAL_RUNTIME_CNF */
enum mipc_sys_set_thermal_runtime_cnf_tlv_enum {
    mipc_sys_set_thermal_runtime_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_OR_GET_SBP_INFO_REQ */
enum mipc_sys_set_or_get_sbp_info_req_tlv_enum {
    mipc_sys_set_or_get_sbp_info_req_tlv_NONE = 0,
    /* Mode of SBP */
    /* type = uint8_t, refer to SYS_SBP_MODE */
    MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_MODE                 = 0x100,
    /* This feature is identified by integer value */
    /* type = uint32_t */
    MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_FEATURE_INT          = 0x101,
    /* This feature is identified by name string */
    /* type = string */
    MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_FEATURE_STR          = 0x102,
    /* SBP feature data or config (integer between 0 ~ 255) */
    /* type = uint8_t */
    MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_DATA                 = 0x103,
    /* PARAM can only acquired and set for current protocol stack. Without special PARAM, the acquire/set feature/data will be applied for all protocol stack. */
    /* type = uint8_t */
    MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_PARAM                = 0x104,
};

    /* MIPC_MSG.SYS_SET_OR_GET_SBP_INFO_CNF */
enum mipc_sys_set_or_get_sbp_info_cnf_tlv_enum {
    mipc_sys_set_or_get_sbp_info_cnf_tlv_NONE = 0,
    /* Integer between 0 ~ 255 */
    /* type = uint8_t */
    MIPC_SYS_SET_OR_GET_SBP_INFO_CNF_T_DATA                 = 0x100,
};

    /* MIPC_MSG.SYS_GET_ALL_THERMAL_INFO_REQ */
enum mipc_sys_get_all_thermal_info_req_tlv_enum {
    mipc_sys_get_all_thermal_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_ALL_THERMAL_INFO_CNF */
enum mipc_sys_get_all_thermal_info_cnf_tlv_enum {
    mipc_sys_get_all_thermal_info_cnf_tlv_NONE = 0,
    /* Current autonomous mode = 0(false) or 1(true) */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_AUTO_FLAG           = 0x101,
    /* Temperature information TLV array of thermal sensor */
    /* type = struct, refer to sys_thermal_sensor_info_e */
    MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_THERMAL_TEMP_INFO   = 0x102,
    MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_THERMAL_TEMP_INFO_TLV_ARRAY = 0x102,
    /* Actuator state information TLV array of thermal actuator */
    /* type = struct, refer to sys_thermal_actuator_state_info */
    MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_ACTUATOR_STATE_INFO = 0x103,
    MIPC_SYS_GET_ALL_THERMAL_INFO_CNF_T_ACTUATOR_STATE_INFO_TLV_ARRAY = 0x103,
};

    /* MIPC_MSG.SYS_META_REQ */
enum mipc_sys_meta_req_tlv_enum {
    mipc_sys_meta_req_tlv_NONE = 0,
    /* Local parameters used in modem */
    /* type = byte_array */
    MIPC_SYS_META_REQ_T_LOCAL                               = 0x100,
    /* Peer buffer used in modem */
    /* type = byte_array */
    MIPC_SYS_META_REQ_T_PEER                                = 0x101,
    /* Check SUM */
    /* type = byte_array */
    MIPC_SYS_META_REQ_T_CHECKSUM                            = 0x102,
};

    /* MIPC_MSG.SYS_META_CNF */
enum mipc_sys_meta_cnf_tlv_enum {
    mipc_sys_meta_cnf_tlv_NONE = 0,
    /* Local parameters used in modem */
    /* type = byte_array */
    MIPC_SYS_META_CNF_T_LOCAL                               = 0x100,
    /* Peer buffer used in modem */
    /* type = byte_array */
    MIPC_SYS_META_CNF_T_PEER                                = 0x101,
    /* Check SUM */
    /* type = byte_array */
    MIPC_SYS_META_CNF_T_CHECKSUM                            = 0x102,
};

    /* MIPC_MSG.SYS_DMF_REQ */
enum mipc_sys_dmf_req_tlv_enum {
    mipc_sys_dmf_req_tlv_NONE = 0,
    /* DMF category ID */
    /* type = uint32_t */
    MIPC_SYS_DMF_REQ_T_CATEGORY_ID                          = 0x100,
    /* The byte array of DMF struct. */
    /* type = byte_array */
    MIPC_SYS_DMF_REQ_T_RAWDATA                              = 0x8101,
};

    /* MIPC_MSG.SYS_DMF_CNF */
enum mipc_sys_dmf_cnf_tlv_enum {
    mipc_sys_dmf_cnf_tlv_NONE = 0,
    /* DMF category ID */
    /* type = uint32_t */
    MIPC_SYS_DMF_CNF_T_CATEGORY_ID                          = 0x100,
    /* The byte array of DMF struct. */
    /* type = byte_array */
    MIPC_SYS_DMF_CNF_T_RAWDATA_TLV_ARRAY                    = 0x8101,
    /* DMF AT command string. */
    /* type = string */
    MIPC_SYS_DMF_CNF_T_ATSTRING_TLV_ARRAY                   = 0x8102,
};

    /* MIPC_MSG.SYS_ECHO_REQ */
enum mipc_sys_echo_req_tlv_enum {
    mipc_sys_echo_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_ECHO_CNF */
enum mipc_sys_echo_cnf_tlv_enum {
    mipc_sys_echo_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_META_FORWARDER_CTRL_REQ */
enum mipc_sys_meta_forwarder_ctrl_req_tlv_enum {
    mipc_sys_meta_forwarder_ctrl_req_tlv_NONE = 0,
    /*  */
    /* type = byte_array */
    MIPC_SYS_META_FORWARDER_CTRL_REQ_T_SELECTOR             = 0x100,
    /*  */
    /* type = byte_array */
    MIPC_SYS_META_FORWARDER_CTRL_REQ_T_CHECKSUM             = 0x101,
};

    /* MIPC_MSG.SYS_META_FORWARDER_CTRL_CNF */
enum mipc_sys_meta_forwarder_ctrl_cnf_tlv_enum {
    mipc_sys_meta_forwarder_ctrl_cnf_tlv_NONE = 0,
    /*  */
    /* type = byte_array */
    MIPC_SYS_META_FORWARDER_CTRL_CNF_T_SELECTOR             = 0x100,
    /*  */
    /* type = byte_array */
    MIPC_SYS_META_FORWARDER_CTRL_CNF_T_CHECKSUM             = 0x101,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_FLUSH_INTERVAL_REQ */
enum mipc_sys_set_md_log_flush_interval_req_tlv_enum {
    mipc_sys_set_md_log_flush_interval_req_tlv_NONE = 0,
    /*
      Flush interval: N (uint:ms)
      N < default_value: set default value
      N >= default value: set flush interval value
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MD_LOG_FLUSH_INTERVAL_REQ_T_FLUSH_INTERVAL = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_FLUSH_INTERVAL_CNF */
enum mipc_sys_set_md_log_flush_interval_cnf_tlv_enum {
    mipc_sys_set_md_log_flush_interval_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_FLUSH_INTERVAL_REQ */
enum mipc_sys_get_md_log_flush_interval_req_tlv_enum {
    mipc_sys_get_md_log_flush_interval_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_FLUSH_INTERVAL_CNF */
enum mipc_sys_get_md_log_flush_interval_cnf_tlv_enum {
    mipc_sys_get_md_log_flush_interval_cnf_tlv_NONE = 0,
    /* Get current flush interval of logging (uint:ms) */
    /* type = uint32_t */
    MIPC_SYS_GET_MD_LOG_FLUSH_INTERVAL_CNF_T_FLUSH_INTERVAL = 0x100,
};

    /* MIPC_MSG.SYS_HBA_ESTABLISH_REQ */
enum mipc_sys_hba_establish_req_tlv_enum {
    mipc_sys_hba_establish_req_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_PROXY_KEY                  = 0x100,
    /* interface id, value range is 0~19 */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IF_ID                      = 0x101,
    /* hitchhike interval */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_HITCHHIKE_INTERVAL         = 0x102,
    /* heartbeat pattern */
    /* type = byte_array */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_HB_PATTERN                 = 0x8103,
    /* heartbeat ack pattern */
    /* type = byte_array */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_HB_ACK_PATTERN             = 0x8104,
    /* dynamic cycle option */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_ALLOW_DYNAMIC_CYCLE        = 0x105,
    /* cycle value */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_CYCLE_VALUE                = 0x106,
    /* max cycle, unit is second */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_MAX_CYCLE                  = 0x107,
    /* cycle step */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_CYCLE_STEP                 = 0x108,
    /* success number of cycle step */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_CYCLE_STEP_SUCCESS_NUM     = 0x109,
    /* ipv4 terms of service */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV4_TOS                   = 0x10A,
    /* ipv4 Time To Live */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV4_TTL                   = 0x10B,
    /* ipv4 id */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV4_ID                    = 0x10C,
    /* ipv6 flow label */
    /* type = uint32_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV6_FLOW_LABEL            = 0x10D,
    /* ipv6 hop limit */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV6_HOP_LIMIT             = 0x10E,
    /* ipv6 traffic class */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IPV6_TCLASS                = 0x10F,
    /* tcp receive wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_RCV_WSCALE             = 0x110,
    /* tcp send wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_SND_WSCALE             = 0x111,
    /* tcp use wscale or not */
    /* type = uint32_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_WSCALE_OK              = 0x112,
    /* tcp send windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_SND_WINDOW             = 0x113,
    /* tcp receive windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_RCV_WINDOW             = 0x114,
    /* tcp send next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_SND_NXT_SEQ            = 0x115,
    /* tcp receive next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_TCP_RCV_NXT_SEQ            = 0x116,
    /* source v4 address; Default value is NULL */
    /* type = struct, refer to data_v4_addr */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_V4_SRC_ADDR                = 0x8117,
    /* destination v4 address; Default value is NULL */
    /* type = struct, refer to data_v4_addr */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_V4_DEST_ADDR               = 0x8118,
    /* source v6 address; Default value is NULL */
    /* type = struct, refer to data_v6_addr */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_V6_SRC_ADDR                = 0x8119,
    /* destination v6 address; Default value is NULL */
    /* type = struct, refer to data_v6_addr */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_V6_DEST_ADDR               = 0x811A,
    /* ipv4 or ipv6 */
    /* type = uint16_t, refer to BOOLEAN */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_IS_IPV6                    = 0x11B,
    /* TCP default value is 6 */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_PROTOCOL                   = 0x11C,
    /* source port */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_SRC_PORT                   = 0x11D,
    /* destination port */
    /* type = uint16_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_DEST_PORT                  = 0x11E,
    /* maximum retry times */
    /* type = uint32_t */
    MIPC_SYS_HBA_ESTABLISH_REQ_T_MAX_RETRY_TIMES            = 0x11F,
};

    /* MIPC_MSG.SYS_HBA_ESTABLISH_CNF */
enum mipc_sys_hba_establish_cnf_tlv_enum {
    mipc_sys_hba_establish_cnf_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_ESTABLISH_CNF_T_PROXY_KEY                  = 0x100,
};

    /* MIPC_MSG.SYS_HBA_CTRL_REQ */
enum mipc_sys_hba_ctrl_req_tlv_enum {
    mipc_sys_hba_ctrl_req_tlv_NONE = 0,
    /* indicate to pause/resume/stop heartbeat agent; Default value is 0 */
    /* type = uint8_t, refer to SYS_HBA_CTRL_MODE */
    MIPC_SYS_HBA_CTRL_REQ_T_MODE                            = 0x100,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_CTRL_REQ_T_PROXY_KEY                       = 0x101,
};

    /* MIPC_MSG.SYS_HBA_CTRL_CNF */
enum mipc_sys_hba_ctrl_cnf_tlv_enum {
    mipc_sys_hba_ctrl_cnf_tlv_NONE = 0,
    /* indicate to pause/resume/stop heartbeat agent */
    /* type = uint8_t, refer to SYS_HBA_CTRL_MODE */
    MIPC_SYS_HBA_CTRL_CNF_T_MODE                            = 0x100,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_CTRL_CNF_T_PROXY_KEY                       = 0x101,
    /* ipv4 terms of service */
    /* type = uint8_t */
    MIPC_SYS_HBA_CTRL_CNF_T_IPV4_TOS                        = 0x102,
    /* ipv4 Time To Live */
    /* type = uint8_t */
    MIPC_SYS_HBA_CTRL_CNF_T_IPV4_TTL                        = 0x103,
    /* ipv4 id */
    /* type = uint16_t */
    MIPC_SYS_HBA_CTRL_CNF_T_IPV4_ID                         = 0x104,
    /* ipv6 flow label */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_IPV6_FLOW_LABEL                 = 0x105,
    /* ipv6 hop limit */
    /* type = uint8_t */
    MIPC_SYS_HBA_CTRL_CNF_T_IPV6_HOP_LIMIT                  = 0x106,
    /* ipv6 traffic class */
    /* type = uint8_t */
    MIPC_SYS_HBA_CTRL_CNF_T_IPV6_TCLASS                     = 0x107,
    /* tcp receive wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_RCV_WSCALE                  = 0x108,
    /* tcp send wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_SND_WSCALE                  = 0x109,
    /* tcp use wscale or not */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_WSCALE_OK                   = 0x10A,
    /* tcp send windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_SND_WINDOW                  = 0x10B,
    /* tcp receive windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_RCV_WINDOW                  = 0x10C,
    /* tcp send next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_SND_NXT_SEQ                 = 0x10D,
    /* tcp receive next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_TCP_RCV_NXT_SEQ                 = 0x10E,
    /* number of heartbeat packet sent */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_SEND_COUNT                      = 0x10F,
    /* number of heartbeat packet replies received */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_RECV_COUNT                      = 0x110,
    /* RRC quick release times */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_SHORT_RRC_COUNT                 = 0x111,
    /* number of hitchhike */
    /* type = uint32_t */
    MIPC_SYS_HBA_CTRL_CNF_T_HITCHHIKE_COUNT                 = 0x112,
    /* current TX cycle value */
    /* type = uint16_t */
    MIPC_SYS_HBA_CTRL_CNF_T_CURRENT_CYCLE                   = 0x113,
};

    /* MIPC_MSG.SYS_HBA_SEND_NOW_REQ */
enum mipc_sys_hba_send_now_req_tlv_enum {
    mipc_sys_hba_send_now_req_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_SEND_NOW_REQ_T_PROXY_KEY                   = 0x100,
};

    /* MIPC_MSG.SYS_HBA_SEND_NOW_CNF */
enum mipc_sys_hba_send_now_cnf_tlv_enum {
    mipc_sys_hba_send_now_cnf_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_SEND_NOW_CNF_T_PROXY_KEY                   = 0x100,
};

    /* MIPC_MSG.SYS_HBA_RESUME_REQ */
enum mipc_sys_hba_resume_req_tlv_enum {
    mipc_sys_hba_resume_req_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_RESUME_REQ_T_PROXY_KEY                     = 0x100,
    /* ipv4 terms of service */
    /* type = uint8_t */
    MIPC_SYS_HBA_RESUME_REQ_T_IPV4_TOS                      = 0x101,
    /* ipv4 Time To Live */
    /* type = uint8_t */
    MIPC_SYS_HBA_RESUME_REQ_T_IPV4_TTL                      = 0x102,
    /* ipv4 id */
    /* type = uint16_t */
    MIPC_SYS_HBA_RESUME_REQ_T_IPV4_ID                       = 0x103,
    /* ipv6 flow label */
    /* type = uint32_t */
    MIPC_SYS_HBA_RESUME_REQ_T_IPV6_FLOW_LABEL               = 0x104,
    /* ipv6 hop limit */
    /* type = uint8_t */
    MIPC_SYS_HBA_RESUME_REQ_T_IPV6_HOP_LIMIT                = 0x105,
    /* ipv6 traffic class */
    /* type = uint8_t */
    MIPC_SYS_HBA_RESUME_REQ_T_IPV6_TCLASS                   = 0x106,
    /* tcp receive wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_RCV_WSCALE                = 0x107,
    /* tcp send wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_SND_WSCALE                = 0x108,
    /* tcp use wscale or not */
    /* type = uint32_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_WSCALE_OK                 = 0x109,
    /* tcp send windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_SND_WINDOW                = 0x10A,
    /* tcp receive windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_RCV_WINDOW                = 0x10B,
    /* tcp send next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_SND_NXT_SEQ               = 0x10C,
    /* tcp receive next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_RESUME_REQ_T_TCP_RCV_NXT_SEQ               = 0x10D,
};

    /* MIPC_MSG.SYS_HBA_RESUME_CNF */
enum mipc_sys_hba_resume_cnf_tlv_enum {
    mipc_sys_hba_resume_cnf_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_RESUME_CNF_T_PROXY_KEY                     = 0x100,
};

    /* MIPC_MSG.SYS_MIA_START_SCENARIO_REQ */
enum mipc_sys_mia_start_scenario_req_tlv_enum {
    mipc_sys_mia_start_scenario_req_tlv_NONE = 0,
    /* information of thermal actuator */
    /* type = struct, refer to sys_mia_metrics */
    MIPC_SYS_MIA_START_SCENARIO_REQ_T_SCENARIO_METRICS      = 0x100,
};

    /* MIPC_MSG.SYS_MIA_START_SCENARIO_CNF */
enum mipc_sys_mia_start_scenario_cnf_tlv_enum {
    mipc_sys_mia_start_scenario_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_MIA_STOP_SCENARIO_REQ */
enum mipc_sys_mia_stop_scenario_req_tlv_enum {
    mipc_sys_mia_stop_scenario_req_tlv_NONE = 0,
    /* information of thermal actuator */
    /* type = struct, refer to sys_mia_metrics */
    MIPC_SYS_MIA_STOP_SCENARIO_REQ_T_SCENARIO_METRICS       = 0x100,
};

    /* MIPC_MSG.SYS_MIA_STOP_SCENARIO_CNF */
enum mipc_sys_mia_stop_scenario_cnf_tlv_enum {
    mipc_sys_mia_stop_scenario_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_MIA_UPDATE_METRICS_REQ */
enum mipc_sys_mia_update_metrics_req_tlv_enum {
    mipc_sys_mia_update_metrics_req_tlv_NONE = 0,
    /* information of thermal actuator */
    /* type = struct, refer to sys_mia_metrics */
    MIPC_SYS_MIA_UPDATE_METRICS_REQ_T_SCENARIO_METRICS      = 0x100,
};

    /* MIPC_MSG.SYS_MIA_UPDATE_METRICS_CNF */
enum mipc_sys_mia_update_metrics_cnf_tlv_enum {
    mipc_sys_mia_update_metrics_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_TRIGGER_MD_LOG_FLUSH_REQ */
enum mipc_sys_trigger_md_log_flush_req_tlv_enum {
    mipc_sys_trigger_md_log_flush_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_TRIGGER_MD_LOG_FLUSH_CNF */
enum mipc_sys_trigger_md_log_flush_cnf_tlv_enum {
    mipc_sys_trigger_md_log_flush_cnf_tlv_NONE = 0,
    /* The result of trigger log flush */
    /* type = uint32_t, refer to SYS_LOG_FLUSH_TRIGGER_RESULT */
    MIPC_SYS_TRIGGER_MD_LOG_FLUSH_CNF_T_TRIGGER_RESULT      = 0x100,
};

    /* MIPC_MSG.SYS_GET_MD_LOG_FLUSH_STATUS_REQ */
enum mipc_sys_get_md_log_flush_status_req_tlv_enum {
    mipc_sys_get_md_log_flush_status_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_MD_LOG_FLUSH_STATUS_CNF */
enum mipc_sys_get_md_log_flush_status_cnf_tlv_enum {
    mipc_sys_get_md_log_flush_status_cnf_tlv_NONE = 0,
    /* Log flush status; 1: is flushing; 0: is not flushing */
    /* type = uint32_t */
    MIPC_SYS_GET_MD_LOG_FLUSH_STATUS_CNF_T_STATUS           = 0x100,
};

    /* MIPC_MSG.SYS_GET_GNSS_COCLOCK_NVDATA_REQ */
enum mipc_sys_get_gnss_coclock_nvdata_req_tlv_enum {
    mipc_sys_get_gnss_coclock_nvdata_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_GNSS_COCLOCK_NVDATA_CNF */
enum mipc_sys_get_gnss_coclock_nvdata_cnf_tlv_enum {
    mipc_sys_get_gnss_coclock_nvdata_cnf_tlv_NONE = 0,
    /* Calibrated S-curve C0 (ppm) for initial clock drift calculation */
    /* type = uint32_t */
    MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF_T_C0               = 0x101,
    /* Calibrated S-curve C1 (ppm/Celsius) for initial clock drift calculation */
    /* type = uint32_t */
    MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF_T_C1               = 0x102,
    /* TSX temperature of capID calibration (Celsius) for initial clock drift estimation if no calibrated C0/C1 received */
    /* type = uint32_t */
    MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF_T_CAPID_TEMP       = 0x103,
};

    /* MIPC_MSG.SYS_SET_SBP_INFO_REQ */
enum mipc_sys_set_sbp_info_req_tlv_enum {
    mipc_sys_set_sbp_info_req_tlv_NONE = 0,
    /* Mode of SBP */
    /* type = uint8_t, refer to SYS_SBP_SET_MODE */
    MIPC_SYS_SET_SBP_INFO_REQ_T_MODE                        = 0x100,
    /* This feature is identified by integer value */
    /* type = uint32_t */
    MIPC_SYS_SET_SBP_INFO_REQ_T_FEATURE_INT                 = 0x101,
    /* This feature is identified by name string */
    /* type = string */
    MIPC_SYS_SET_SBP_INFO_REQ_T_FEATURE_STR                 = 0x102,
    /* SBP feature data or configuration (integer between 0 ~ 255) */
    /* type = uint8_t */
    MIPC_SYS_SET_SBP_INFO_REQ_T_DATA                        = 0x103,
    /* PARAM can only set for current protocol stack. Without special PARAM, the setting will be applied for all protocol stack. */
    /* type = uint8_t */
    MIPC_SYS_SET_SBP_INFO_REQ_T_PARAM                       = 0x104,
};

    /* MIPC_MSG.SYS_SET_SBP_INFO_CNF */
enum mipc_sys_set_sbp_info_cnf_tlv_enum {
    mipc_sys_set_sbp_info_cnf_tlv_NONE = 0,
    /* Integer between 0 ~ 255 */
    /* type = uint8_t */
    MIPC_SYS_SET_SBP_INFO_CNF_T_DATA                        = 0x100,
};

    /* MIPC_MSG.SYS_GET_SBP_INFO_REQ */
enum mipc_sys_get_sbp_info_req_tlv_enum {
    mipc_sys_get_sbp_info_req_tlv_NONE = 0,
    /* Mode of SBP */
    /* type = uint8_t, refer to SYS_SBP_GET_MODE */
    MIPC_SYS_GET_SBP_INFO_REQ_T_MODE                        = 0x100,
    /* This feature is identified by integer value */
    /* type = uint32_t */
    MIPC_SYS_GET_SBP_INFO_REQ_T_FEATURE_INT                 = 0x101,
    /* This feature is identified by name string */
    /* type = string */
    MIPC_SYS_GET_SBP_INFO_REQ_T_FEATURE_STR                 = 0x102,
    /* PARAM can only be acquired for current protocol stack. */
    /* type = uint8_t */
    MIPC_SYS_GET_SBP_INFO_REQ_T_PARAM                       = 0x104,
};

    /* MIPC_MSG.SYS_GET_SBP_INFO_CNF */
enum mipc_sys_get_sbp_info_cnf_tlv_enum {
    mipc_sys_get_sbp_info_cnf_tlv_NONE = 0,
    /* Integer between 0 ~ 255 */
    /* type = uint8_t */
    MIPC_SYS_GET_SBP_INFO_CNF_T_DATA                        = 0x100,
};

    /* MIPC_MSG.SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ */
enum mipc_sys_set_recording_tx_cond_iq_dump_req_tlv_enum {
    mipc_sys_set_recording_tx_cond_iq_dump_req_tlv_NONE = 0,
    /* Tx conditional IQ dump has five types of capture mode. */
    /* type = uint8_t, refer to SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_CAPTURE_MODE */
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_CAPTURE_MODE = 0x100,
    /* According to different capture mode and capture condition sends different message to module NR1. */
    /* type = uint8_t */
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_CAPTURE_CONDITION = 0x101,
    /* In some capture modes/conditions, there are more parameters needed. */
    /* type = uint8_t */
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_PARAM3     = 0x102,
    /* In some capture modes/conditions, there are more parameters needed. */
    /* type = uint8_t */
    MIPC_SYS_SET_RECORDING_TX_COND_IQ_DUMP_REQ_T_PARAM4     = 0x103,
};

    /* MIPC_MSG.SYS_SET_RECORDING_TX_COND_IQ_DUMP_CNF */
enum mipc_sys_set_recording_tx_cond_iq_dump_cnf_tlv_enum {
    mipc_sys_set_recording_tx_cond_iq_dump_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_WAKEUP_REASON_REQ */
enum mipc_sys_get_wakeup_reason_req_tlv_enum {
    mipc_sys_get_wakeup_reason_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_WAKEUP_REASON_CNF */
enum mipc_sys_get_wakeup_reason_cnf_tlv_enum {
    mipc_sys_get_wakeup_reason_cnf_tlv_NONE = 0,
    /* The reason enum from MD */
    /* type = uint8_t, refer to SYS_WAKEUP_REASON */
    MIPC_SYS_GET_WAKEUP_REASON_CNF_T_WAKEUP_REASON          = 0x100,
};

    /* MIPC_MSG.SYS_SET_SLEEP_TIMER_REQ */
enum mipc_sys_set_sleep_timer_req_tlv_enum {
    mipc_sys_set_sleep_timer_req_tlv_NONE = 0,
    /* The timer value, unit is second; Non 0 value means in SLEEP MODE and start timer; 0 value means stop timer. */
    /* type = uint32_t */
    MIPC_SYS_SET_SLEEP_TIMER_REQ_T_VALUE                    = 0x100,
};

    /* MIPC_MSG.SYS_SET_SLEEP_TIMER_CNF */
enum mipc_sys_set_sleep_timer_cnf_tlv_enum {
    mipc_sys_set_sleep_timer_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_SLEEP_TIMER_REQ */
enum mipc_sys_get_sleep_timer_req_tlv_enum {
    mipc_sys_get_sleep_timer_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_SLEEP_TIMER_CNF */
enum mipc_sys_get_sleep_timer_cnf_tlv_enum {
    mipc_sys_get_sleep_timer_cnf_tlv_NONE = 0,
    /* The timer value, unit is second; Remaining timer from set timer. */
    /* type = uint32_t */
    MIPC_SYS_GET_SLEEP_TIMER_CNF_T_VALUE                    = 0x100,
};

    /* MIPC_MSG.SYS_SET_SLEEP_MODE_REQ */
enum mipc_sys_set_sleep_mode_req_tlv_enum {
    mipc_sys_set_sleep_mode_req_tlv_NONE = 0,
    /* The sleep mode stats, true is go into sleep, false is out sleep. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_SLEEP_MODE_REQ_T_SLEEP_MODE                = 0x100,
};

    /* MIPC_MSG.SYS_SET_SLEEP_MODE_CNF */
enum mipc_sys_set_sleep_mode_cnf_tlv_enum {
    mipc_sys_set_sleep_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SEND_CONSYS_WIFI_REQ */
enum mipc_sys_send_consys_wifi_req_tlv_enum {
    mipc_sys_send_consys_wifi_req_tlv_NONE = 0,
    /* Length of mcif payload. */
    /* type = uint16_t */
    MIPC_SYS_SEND_CONSYS_WIFI_REQ_T_PAYLOAD_LEN             = 0x100,
    /* Byte array of mcif payload. */
    /* type = byte_array */
    MIPC_SYS_SEND_CONSYS_WIFI_REQ_T_PAYLOAD                 = 0x8101,
};

    /* MIPC_MSG.SYS_SEND_CONSYS_WIFI_CNF */
enum mipc_sys_send_consys_wifi_cnf_tlv_enum {
    mipc_sys_send_consys_wifi_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SEND_CONSYS_BT_REQ */
enum mipc_sys_send_consys_bt_req_tlv_enum {
    mipc_sys_send_consys_bt_req_tlv_NONE = 0,
    /* Length of mcif payload. */
    /* type = uint16_t */
    MIPC_SYS_SEND_CONSYS_BT_REQ_T_PAYLOAD_LEN               = 0x100,
    /* Byte array of mcif payload. */
    /* type = byte_array */
    MIPC_SYS_SEND_CONSYS_BT_REQ_T_PAYLOAD                   = 0x8101,
};

    /* MIPC_MSG.SYS_SEND_CONSYS_BT_CNF */
enum mipc_sys_send_consys_bt_cnf_tlv_enum {
    mipc_sys_send_consys_bt_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_MD_LOG_FILTER_REQ */
enum mipc_sys_set_md_log_filter_req_tlv_enum {
    mipc_sys_set_md_log_filter_req_tlv_NONE = 0,
    /* Binary data of modem log filter. */
    /* type = byte_array */
    MIPC_SYS_SET_MD_LOG_FILTER_REQ_T_RAW_DATA               = 0x100,
};

    /* MIPC_MSG.SYS_SET_MD_LOG_FILTER_CNF */
enum mipc_sys_set_md_log_filter_cnf_tlv_enum {
    mipc_sys_set_md_log_filter_cnf_tlv_NONE = 0,
    /* Result of the modem log filter setting. */
    /* type = uint32_t, refer to SYS_MD_LOG_RESULT */
    MIPC_SYS_SET_MD_LOG_FILTER_CNF_T_SETTING_RESULT         = 0x100,
};

    /* MIPC_MSG.SYS_SET_SPV_CONTROL_REQ */
enum mipc_sys_set_spv_control_req_tlv_enum {
    mipc_sys_set_spv_control_req_tlv_NONE = 0,
    /* SPV service control type. */
    /* type = uint8_t, refer to SYS_SPV_CONTROL_TYPE */
    MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONTROL_TYPE             = 0x100,
    /* SPV service control category. */
    /* type = uint32_t */
    MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONTROL                  = 0x101,
    /* 1st configuration value (optional, depends on control category). Default value is 0. */
    /* type = uint32_t */
    MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONFIG1                  = 0x102,
    /* 2nd configuration value (optional, depends on control category). Default value is 0. */
    /* type = uint32_t */
    MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONFIG2                  = 0x103,
    /* 3rd configuration value (optional, depends on control category). Default value is 0. */
    /* type = uint32_t */
    MIPC_SYS_SET_SPV_CONTROL_REQ_T_CONFIG3                  = 0x104,
};

    /* MIPC_MSG.SYS_SET_SPV_CONTROL_CNF */
enum mipc_sys_set_spv_control_cnf_tlv_enum {
    mipc_sys_set_spv_control_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_IDC_CFG_REQ */
enum mipc_sys_set_idc_cfg_req_tlv_enum {
    mipc_sys_set_idc_cfg_req_tlv_NONE = 0,
    /* Enable or disable frame synchronize. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_FRAME_SYNC_SUPPORT           = 0x100,
    /* Enable or disable TX pause. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_TX_PAUSE_SUPPORT             = 0x101,
    /* Enable or disable Mobile Wireless Standards (MWS) TX. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_MWS_TX_SUPPORT               = 0x102,
    /* Enable or disable RX protect. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_RX_PROTECT_SUPPORT           = 0x103,
    /* Enable or disable inactivity duration. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_INACT_DURATION_SUPPORT       = 0x104,
    /* Enable or disable power backoff. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_PWR_BACKOFF_SUPPORT          = 0x105,
    /* Enable or disable RRC state. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_RRC_STATE_SUPPORT            = 0x106,
    /* Enable or disable TX port. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_TX_PORT_SUPPORT              = 0x107,
    /* Enable or disable resend. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_SET_IDC_CFG_REQ_T_RESEND_SUPPORT               = 0x108,
    /* GPIO configuration. */
    /* type = struct, refer to sys_idc_gpio_cfg */
    MIPC_SYS_SET_IDC_CFG_REQ_T_GPIO_CFG                     = 0x109,
};

    /* MIPC_MSG.SYS_SET_IDC_CFG_CNF */
enum mipc_sys_set_idc_cfg_cnf_tlv_enum {
    mipc_sys_set_idc_cfg_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_FORWARD_AT_REQ */
enum mipc_sys_forward_at_req_tlv_enum {
    mipc_sys_forward_at_req_tlv_NONE = 0,
    /* AT command string to be executed. Max length 2048. */
    /* type = string */
    MIPC_SYS_FORWARD_AT_REQ_T_ATCMD                         = 0x8100,
    /* 0:URC, 1:RSP */
    /* type = uint8_t, refer to SYS_FORWARD_AT_REQ_FORWARD_TYPE */
    MIPC_SYS_FORWARD_AT_REQ_T_FORWARD_TYPE                  = 0x101,
};

    /* MIPC_MSG.SYS_FORWARD_AT_CNF */
enum mipc_sys_forward_at_cnf_tlv_enum {
    mipc_sys_forward_at_cnf_tlv_NONE = 0,
    /* AT command execution result. */
    /* type = string */
    MIPC_SYS_FORWARD_AT_CNF_T_ATCMD                         = 0x8100,
};

    /* MIPC_MSG.SYS_TRIGGER_COREDUMP_REQ */
enum mipc_sys_trigger_coredump_req_tlv_enum {
    mipc_sys_trigger_coredump_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_TRIGGER_COREDUMP_CNF */
enum mipc_sys_trigger_coredump_cnf_tlv_enum {
    mipc_sys_trigger_coredump_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_FACTORY_MODE_REQ */
enum mipc_sys_get_factory_mode_req_tlv_enum {
    mipc_sys_get_factory_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_FACTORY_MODE_CNF */
enum mipc_sys_get_factory_mode_cnf_tlv_enum {
    mipc_sys_get_factory_mode_cnf_tlv_NONE = 0,
    /* Current modem factory mode. */
    /* type = uint8_t, refer to SYS_FACTORY_MODE */
    MIPC_SYS_GET_FACTORY_MODE_CNF_T_MODE                    = 0x100,
};

    /* MIPC_MSG.SYS_SET_FACTORY_MODE_REQ */
enum mipc_sys_set_factory_mode_req_tlv_enum {
    mipc_sys_set_factory_mode_req_tlv_NONE = 0,
    /* Modem factory mode. */
    /* type = uint8_t, refer to SYS_FACTORY_MODE */
    MIPC_SYS_SET_FACTORY_MODE_REQ_T_MODE                    = 0x100,
};

    /* MIPC_MSG.SYS_SET_FACTORY_MODE_CNF */
enum mipc_sys_set_factory_mode_cnf_tlv_enum {
    mipc_sys_set_factory_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_COEX_UART_TEST_MODE_REQ */
enum mipc_sys_set_coex_uart_test_mode_req_tlv_enum {
    mipc_sys_set_coex_uart_test_mode_req_tlv_NONE = 0,
    /* COEX UART mode operation */
    /* type = uint8_t, refer to SYS_COEX_UART_MODE_OP */
    MIPC_SYS_SET_COEX_UART_TEST_MODE_REQ_T_OP               = 0x100,
};

    /* MIPC_MSG.SYS_SET_COEX_UART_TEST_MODE_CNF */
enum mipc_sys_set_coex_uart_test_mode_cnf_tlv_enum {
    mipc_sys_set_coex_uart_test_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_WRITE_COEX_UART_REQ */
enum mipc_sys_write_coex_uart_req_tlv_enum {
    mipc_sys_write_coex_uart_req_tlv_NONE = 0,
    /* Data need to transfer from COEX UART TX */
    /* type = byte_array */
    MIPC_SYS_WRITE_COEX_UART_REQ_T_DATA                     = 0x100,
};

    /* MIPC_MSG.SYS_WRITE_COEX_UART_CNF */
enum mipc_sys_write_coex_uart_cnf_tlv_enum {
    mipc_sys_write_coex_uart_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_READ_COEX_UART_REQ */
enum mipc_sys_read_coex_uart_req_tlv_enum {
    mipc_sys_read_coex_uart_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_READ_COEX_UART_CNF */
enum mipc_sys_read_coex_uart_cnf_tlv_enum {
    mipc_sys_read_coex_uart_cnf_tlv_NONE = 0,
    /* 128 is reserved for future project use(ex : Antica). In Caymus, we only use 64 byte array size. */
    /* type = byte_array */
    MIPC_SYS_READ_COEX_UART_CNF_T_DATA                      = 0x100,
};

    /* MIPC_MSG.SYS_SET_COEX_UART_LOOPBACK_MODE_REQ */
enum mipc_sys_set_coex_uart_loopback_mode_req_tlv_enum {
    mipc_sys_set_coex_uart_loopback_mode_req_tlv_NONE = 0,
    /* COEX UART mode operation */
    /* type = uint8_t, refer to SYS_COEX_UART_MODE_OP */
    MIPC_SYS_SET_COEX_UART_LOOPBACK_MODE_REQ_T_OP           = 0x100,
};

    /* MIPC_MSG.SYS_SET_COEX_UART_LOOPBACK_MODE_CNF */
enum mipc_sys_set_coex_uart_loopback_mode_cnf_tlv_enum {
    mipc_sys_set_coex_uart_loopback_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_LOWV_ACTUATOR_REQ */
enum mipc_sys_set_lowv_actuator_req_tlv_enum {
    mipc_sys_set_lowv_actuator_req_tlv_NONE = 0,
    /* Actuator ID (start from one). */
    /* type = uint32_t */
    MIPC_SYS_SET_LOWV_ACTUATOR_REQ_T_ID                     = 0x100,
    /* Throttle state. */
    /* type = uint32_t */
    MIPC_SYS_SET_LOWV_ACTUATOR_REQ_T_STATE                  = 0x101,
};

    /* MIPC_MSG.SYS_SET_LOWV_ACTUATOR_CNF */
enum mipc_sys_set_lowv_actuator_cnf_tlv_enum {
    mipc_sys_set_lowv_actuator_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_LOWV_ACTUATOR_REQ */
enum mipc_sys_get_lowv_actuator_req_tlv_enum {
    mipc_sys_get_lowv_actuator_req_tlv_NONE = 0,
    /* It indicates whether to get the number of actuators; Default value is false. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_GET_LOWV_ACTUATOR_REQ_T_GET_ACTUATOR_NUM       = 0x100,
    /* The actuator ID which is used to acquire the actuator information; Default value is 0xffffffff. */
    /* type = uint32_t */
    MIPC_SYS_GET_LOWV_ACTUATOR_REQ_T_ACTUATOR_ID            = 0x101,
};

    /* MIPC_MSG.SYS_GET_LOWV_ACTUATOR_CNF */
enum mipc_sys_get_lowv_actuator_cnf_tlv_enum {
    mipc_sys_get_lowv_actuator_cnf_tlv_NONE = 0,
    /* The number of actuators. */
    /* type = uint32_t */
    MIPC_SYS_GET_LOWV_ACTUATOR_CNF_T_ACTUATOR_NUM           = 0x100,
    /* The actuator information of specific actuator. */
    /* type = struct, refer to sys_thermal_actuator_state_info */
    MIPC_SYS_GET_LOWV_ACTUATOR_CNF_T_ACTUATOR_STATE_INFO    = 0x101,
};

    /* MIPC_MSG.SYS_SET_MSPM_SESSION_REQ */
enum mipc_sys_set_mspm_session_req_tlv_enum {
    mipc_sys_set_mspm_session_req_tlv_NONE = 0,
    /* Mspm session type. */
    /* type = uint8_t, refer to MSPM_SESSION_TYPE */
    MIPC_SYS_SET_MSPM_SESSION_REQ_T_TYPE                    = 0x100,
    /* mspm session procedure. */
    /* type = string */
    MIPC_SYS_SET_MSPM_SESSION_REQ_T_PROCEDURE               = 0x101,
    /* Modem factory mode. */
    /* type = uint8_t, refer to MSPM_BLOCK_MODE */
    MIPC_SYS_SET_MSPM_SESSION_REQ_T_BLOCK_MODE              = 0x102,
    /* wait time by seconds */
    /* type = uint32_t */
    MIPC_SYS_SET_MSPM_SESSION_REQ_T_WAIT_TIME               = 0x103,
};

    /* MIPC_MSG.SYS_SET_MSPM_SESSION_CNF */
enum mipc_sys_set_mspm_session_cnf_tlv_enum {
    mipc_sys_set_mspm_session_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_ERRC_OFFSET_REQ */
enum mipc_sys_set_errc_offset_req_tlv_enum {
    mipc_sys_set_errc_offset_req_tlv_NONE = 0,
    /* Function enable or disable, 0 : disable; 1 : enable */
    /* type = uint8_t */
    MIPC_SYS_SET_ERRC_OFFSET_REQ_T_VALID_FLAG               = 0x100,
    /* [MCF]Customized offset for low to high priority reselection(serving cell non ENDC neighbor cell non ENDC offset).(dB) */
    /* type = uint16_t */
    MIPC_SYS_SET_ERRC_OFFSET_REQ_T_NONNBR_NONOFFSET         = 0x101,
    /* [MCF]Customized offset for low to high priority reselection(serving cell ENDC neighbor cell ENDC offset).(dB) */
    /* type = uint16_t */
    MIPC_SYS_SET_ERRC_OFFSET_REQ_T_NBR_OFFSET               = 0x102,
    /* [MCF]Customized offset for low to high priority reselection(serving cell non ENDC neighbor cell ENDC offset).(dB) */
    /* type = uint16_t */
    MIPC_SYS_SET_ERRC_OFFSET_REQ_T_NONNBR_OFFSET            = 0x103,
};

    /* MIPC_MSG.SYS_SET_ERRC_OFFSET_CNF */
enum mipc_sys_set_errc_offset_cnf_tlv_enum {
    mipc_sys_set_errc_offset_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_MDDBG_CONTROL_REQ */
enum mipc_sys_set_mddbg_control_req_tlv_enum {
    mipc_sys_set_mddbg_control_req_tlv_NONE = 0,
    /*
      MDDBG_CONTROL_TYPE Enum.
    */
    /* type = uint8_t, refer to SYS_MDDBG_CONTROL_TYPE */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_CONTROL_TYPE           = 0x100,
    /*
      Control option.
      Only valid when control_type = 0, 2, 3
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_CONTROL                = 0x101,
    /*
      Control Buffer Pool index.
      Only valid when control_type = 0
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_POOL_INDEX             = 0x102,
    /*
      Indicate buffer index to start scan.
      Only valid when control_type = 0
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_START_COUNT            = 0x103,
    /*
      Number of Buffer to be scan.
      Only valid when control_type = 0
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_SCAN_BUFFER_NO         = 0x104,
    /*
      Enable and disable the DIAG Task.
      Only valid when control_type = 1
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_ENABLE_SET             = 0x105,
    /*
      Expiration time for invoking DIAG Task
      Only valid when control_type = 1
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_TIMER_VALUE            = 0x106,
    /*
      Target VPE
      Only valid when control_type = 2, 3
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_VPEINDEX               = 0x107,
    /*
      Select Watchpoint.
      Only valid when control_type = 2
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_SELECTWP               = 0x108,
    /*
      The address you want Watchpoint to set.
      Only valid when control_type = 2
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_WP_ADDR                = 0x109,
    /*
      Select Breakpoint.
      Only valid when control_type = 3
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_SELECTBP               = 0x10A,
    /*
      The address you want Breakpoint to set.
      Only valid when control_type = 3
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_BP_ADDR                = 0x10B,
    /*
      Set address bit to be ignored.
      Only valid when control_type = 2, 3
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_ADDR_MASK              = 0x10C,
    /*
      Access type of Watchpoint(Read/Write/RW).
      Only valid when control_type = 2
    */
    /* type = uint32_t */
    MIPC_SYS_SET_MDDBG_CONTROL_REQ_T_TYPE                   = 0x10D,
};

    /* MIPC_MSG.SYS_SET_MDDBG_CONTROL_CNF */
enum mipc_sys_set_mddbg_control_cnf_tlv_enum {
    mipc_sys_set_mddbg_control_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_TCXO_INFO_REQ */
enum mipc_sys_get_tcxo_info_req_tlv_enum {
    mipc_sys_get_tcxo_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_TCXO_INFO_CNF */
enum mipc_sys_get_tcxo_info_cnf_tlv_enum {
    mipc_sys_get_tcxo_info_cnf_tlv_NONE = 0,
    /* TCXO can be considered as the clock of the system, TCXO_OFF_COUNTER is the total number of times TCXO is turned off */
    /* type = uint32_t */
    MIPC_SYS_GET_TCXO_INFO_CNF_T_TCXO_OFF_COUNTER           = 0x100,
    /* TCXO_OFF_DURATION is the total duration of TCXO off time. */
    /* type = uint32_t */
    MIPC_SYS_GET_TCXO_INFO_CNF_T_TCXO_OFF_DURATION          = 0x101,
};

    /* MIPC_MSG.SYS_TRIGGER_CHIP_DIAGNOSIS_REQ */
enum mipc_sys_trigger_chip_diagnosis_req_tlv_enum {
    mipc_sys_trigger_chip_diagnosis_req_tlv_NONE = 0,
    /* If the operation is failed, modem will encounter assert. */
    /* type = uint32_t, refer to SYS_CHIP_DIAGNOSIS_OPERATION */
    MIPC_SYS_TRIGGER_CHIP_DIAGNOSIS_REQ_T_OP                = 0x100,
    /* mask/unmask events for DVFS */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_TRIGGER_CHIP_DIAGNOSIS_REQ_T_DVFS_SWITCH       = 0x101,
};

    /* MIPC_MSG.SYS_TRIGGER_CHIP_DIAGNOSIS_CNF */
enum mipc_sys_trigger_chip_diagnosis_cnf_tlv_enum {
    mipc_sys_trigger_chip_diagnosis_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SEND_APP_EVENT_REQ */
enum mipc_sys_send_app_event_req_tlv_enum {
    mipc_sys_send_app_event_req_tlv_NONE = 0,
    /* 1 start, 0 stop */
    /* type = uint8_t */
    MIPC_SYS_SEND_APP_EVENT_REQ_T_VOIP_STATUS               = 0x100,
};

    /* MIPC_MSG.SYS_SEND_APP_EVENT_CNF */
enum mipc_sys_send_app_event_cnf_tlv_enum {
    mipc_sys_send_app_event_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_PRIMARY_IMEI_INFO_REQ */
enum mipc_sys_get_primary_imei_info_req_tlv_enum {
    mipc_sys_get_primary_imei_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_PRIMARY_IMEI_INFO_CNF */
enum mipc_sys_get_primary_imei_info_cnf_tlv_enum {
    mipc_sys_get_primary_imei_info_cnf_tlv_NONE = 0,
    /* Primary IMEI corresponding slog id */
    /* type = uint8_t */
    MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_PRIMARY_SLOT_ID    = 0x100,
    /* Primary imei number of mobile deveice,IMEI is a 15-digit code,add one more 0. */
    /* type = string */
    MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_PRIMARY_IMEI       = 0x8101,
    /* SVN is Software Version Number,IMEI is a 15-digit code used to uniquely identify a mobile device, such as a cell phone, and contains some information used to identify the device, including the manufacturer code, device type code, and the last two digits of the software version number (SVN), add one more 0 end. */
    /* type = string */
    MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF_T_IMEI_SVN           = 0x8102,
};

    /* MIPC_MSG.SYS_AT_IND */
enum mipc_sys_at_ind_tlv_enum {
    mipc_sys_at_ind_tlv_NONE = 0,
    /* type = string */
    MIPC_SYS_AT_IND_T_ATCMD                                 = 0x8100,
};

    /* MIPC_MSG.SYS_THERMAL_SENSOR_IND */
enum mipc_sys_thermal_sensor_ind_tlv_enum {
    mipc_sys_thermal_sensor_ind_tlv_NONE = 0,
    /* Current temperature (unit: /10 degC). */
    /* type = int32_t */
    MIPC_SYS_THERMAL_SENSOR_IND_T_TEMPERATURE               = 0x100,
    /* [PHASE OUT]matched threshold structure */
    /* type = struct, refer to sys_thermal_sensor_config */
    MIPC_SYS_THERMAL_SENSOR_IND_T_THRESHOLD                 = 0x101,
    /* [PHASE OUT]please use INFO_TLV_ARRAY instead of INFO_COUNT&INFO */
    /* type = uint8_t */
    MIPC_SYS_THERMAL_SENSOR_IND_T_INFO_COUNT                = 0x102,
    /* [PHASE OUT]please use INFO_TLV_ARRAY instead of INFO_COUNT&INFO */
    /* type = struct, refer to sys_thermal_sensor_info */
    MIPC_SYS_THERMAL_SENSOR_IND_T_INFO                      = 0x103,
    /* Matched threshold structure. */
    /* type = struct, refer to sys_thermal_sensor_config_e */
    MIPC_SYS_THERMAL_SENSOR_IND_T_CONFIG_E                  = 0x104,
    /* Temperature information TLV array. */
    /* type = struct, refer to sys_thermal_sensor_info_e */
    MIPC_SYS_THERMAL_SENSOR_IND_T_INFO_E                    = 0x105,
    MIPC_SYS_THERMAL_SENSOR_IND_T_INFO_TLV_ARRAY            = 0x105,
};

    /* MIPC_MSG.SYS_CONFIG_IND */
enum mipc_sys_config_ind_tlv_enum {
    mipc_sys_config_ind_tlv_NONE = 0,
    /* the reason of configuration change */
    /* type = uint32_t, refer to SYS_CONFIG_CHANGE_REASON */
    MIPC_SYS_CONFIG_IND_T_REASON                            = 0x100,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_CONFIG_IND_T_CLASS                             = 0x101,
};

    /* MIPC_MSG.SYS_ADPCLK_IND */
enum mipc_sys_adpclk_ind_tlv_enum {
    mipc_sys_adpclk_ind_tlv_NONE = 0,
    /* type = uint8_t */
    MIPC_SYS_ADPCLK_IND_T_FREQ_INFO_COUNT                   = 0x100,
    /* type = struct_array, refer to sys_adpclk_freq_info */
    MIPC_SYS_ADPCLK_IND_T_FREQ_INFO_LIST                    = 0x101,
    /* type = struct, refer to sys_adpclk_freq_info */
    MIPC_SYS_ADPCLK_IND_T_FREQ_INFO_TLV_ARRAY               = 0x102,
};

    /* MIPC_MSG.SYS_MCF_IND */
enum mipc_sys_mcf_ind_tlv_enum {
    mipc_sys_mcf_ind_tlv_NONE = 0,
    /* type = uint8_t */
    MIPC_SYS_MCF_IND_T_TYPE                                 = 0x10A,
    /* type = uint8_t */
    MIPC_SYS_MCF_IND_T_RESULT                               = 0x10B,
};

    /* MIPC_MSG.SYS_SBP_IND */
enum mipc_sys_sbp_ind_tlv_enum {
    mipc_sys_sbp_ind_tlv_NONE = 0,
    /* System SBP ID */
    /* type = uint32_t */
    MIPC_SYS_SBP_IND_T_SBP_ID                               = 0x100,
    /* SIM system SBP ID */
    /* type = uint32_t */
    MIPC_SYS_SBP_IND_T_SIM_SBP_ID                           = 0x8101,
};

    /* MIPC_MSG.SYS_EL2_IP_UL_IND */
enum mipc_sys_el2_ip_ul_ind_tlv_enum {
    mipc_sys_el2_ip_ul_ind_tlv_NONE = 0,
    /* el2 ip ul tx bps */
    /* type = uint32_t */
    MIPC_SYS_EL2_IP_UL_IND_T_TX_BPS                         = 0x100,
};

    /* MIPC_MSG.SYS_EL2_IP_DL_IND */
enum mipc_sys_el2_ip_dl_ind_tlv_enum {
    mipc_sys_el2_ip_dl_ind_tlv_NONE = 0,
    /* el2 ip dl tx bps */
    /* type = uint32_t */
    MIPC_SYS_EL2_IP_DL_IND_T_TX_BPS                         = 0x100,
};

    /* MIPC_MSG.SYS_EL2_MAC_UL_IND */
enum mipc_sys_el2_mac_ul_ind_tlv_enum {
    mipc_sys_el2_mac_ul_ind_tlv_NONE = 0,
    /* el2 mac ul tx bps */
    /* type = uint32_t */
    MIPC_SYS_EL2_MAC_UL_IND_T_TX_BPS                        = 0x100,
};

    /* MIPC_MSG.SYS_EL2_MAC_DL_IND */
enum mipc_sys_el2_mac_dl_ind_tlv_enum {
    mipc_sys_el2_mac_dl_ind_tlv_NONE = 0,
    /* el2 mac dl tx bps */
    /* type = uint32_t */
    MIPC_SYS_EL2_MAC_DL_IND_T_TX_BPS                        = 0x100,
};

    /* MIPC_MSG.SYS_EL2_PDCP_UL_IND */
enum mipc_sys_el2_pdcp_ul_ind_tlv_enum {
    mipc_sys_el2_pdcp_ul_ind_tlv_NONE = 0,
    /* el2 pdcp ul tx bps */
    /* type = uint32_t */
    MIPC_SYS_EL2_PDCP_UL_IND_T_TX_BPS                       = 0x100,
};

    /* MIPC_MSG.SYS_EL2_PDCP_DL_IND */
enum mipc_sys_el2_pdcp_dl_ind_tlv_enum {
    mipc_sys_el2_pdcp_dl_ind_tlv_NONE = 0,
    /* el2 pdcp dl tx bps */
    /* type = uint32_t */
    MIPC_SYS_EL2_PDCP_DL_IND_T_TX_BPS                       = 0x100,
};

    /* MIPC_MSG.SYS_NL2_MAC_UL_IND */
enum mipc_sys_nl2_mac_ul_ind_tlv_enum {
    mipc_sys_nl2_mac_ul_ind_tlv_NONE = 0,
    /* nl2 mac ul tx bps */
    /* type = uint32_t */
    MIPC_SYS_NL2_MAC_UL_IND_T_TX_BPS                        = 0x100,
};

    /* MIPC_MSG.SYS_NL2_MAC_DL_IND */
enum mipc_sys_nl2_mac_dl_ind_tlv_enum {
    mipc_sys_nl2_mac_dl_ind_tlv_NONE = 0,
    /* nl2 mac dl tx bps */
    /* type = uint32_t */
    MIPC_SYS_NL2_MAC_DL_IND_T_TX_BPS                        = 0x100,
};

    /* MIPC_MSG.SYS_NL2_PDCP_UL_IND */
enum mipc_sys_nl2_pdcp_ul_ind_tlv_enum {
    mipc_sys_nl2_pdcp_ul_ind_tlv_NONE = 0,
    /* nl2 pdcp ul tx bps */
    /* type = uint32_t */
    MIPC_SYS_NL2_PDCP_UL_IND_T_TX_BPS                       = 0x100,
};

    /* MIPC_MSG.SYS_NL2_PDCP_DL_IND */
enum mipc_sys_nl2_pdcp_dl_ind_tlv_enum {
    mipc_sys_nl2_pdcp_dl_ind_tlv_NONE = 0,
    /* nl2 pdcp dl tx bps */
    /* type = uint32_t */
    MIPC_SYS_NL2_PDCP_DL_IND_T_TX_BPS                       = 0x100,
};

    /* MIPC_MSG.SYS_GEO_LOCATION_IND */
enum mipc_sys_geo_location_ind_tlv_enum {
    mipc_sys_geo_location_ind_tlv_NONE = 0,
    /* request id, 0 ~ 7 */
    /* type = uint8_t, refer to SYS_ACCOUNT_ID */
    MIPC_SYS_GEO_LOCATION_IND_T_ACCOUNT_ID                  = 0x8100,
    /* 0: Do not broadcast this information to all IMS accounts. 1: Broadcast this information to all IMS accounts */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_GEO_LOCATION_IND_T_BROADCAST_FLAG              = 0x101,
    /* latitude from GPS, 0 as failed */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_LATITUDE                    = 0x8102,
    /* longitude from GPS, 0 as failed */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_LONGITUDE                   = 0x8103,
    /* accurate from GPS, 0 as failed */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_ACCURACY                    = 0x8104,
    /* Positioning method */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_METHOD                      = 0x8105,
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_CITY                        = 0x8106,
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_STATE                       = 0x8107,
    /* Zip code */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_ZIP                         = 0x8108,
    /* Two-letter country code defined by ISO 3166-1 */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_COUNTRY_CODE                = 0x8109,
    /* UE Wi-Fi interface mac address */
    /* type = string */
    MIPC_SYS_GEO_LOCATION_IND_T_UE_WLAN_MAC                 = 0x810A,
    /* confidence */
    /* type = uint32_t */
    MIPC_SYS_GEO_LOCATION_IND_T_CONFIDENCE                  = 0x810B,
};

    /* MIPC_MSG.SYS_MD_INIT_IND */
enum mipc_sys_md_init_ind_tlv_enum {
    mipc_sys_md_init_ind_tlv_NONE = 0,
    /* Initial ID of modem */
    /* type = uint8_t, refer to MD_INIT_ID */
    MIPC_SYS_MD_INIT_IND_T_INIT_ID                          = 0x100,
};

    /* MIPC_MSG.SYS_WARNING_IND */
enum mipc_sys_warning_ind_tlv_enum {
    mipc_sys_warning_ind_tlv_NONE = 0,
    /* detailed warning info */
    /* type = string */
    MIPC_SYS_WARNING_IND_T_INFO                             = 0x100,
};

    /* MIPC_MSG.SYS_NV_SIG_ERR_IND */
enum mipc_sys_nv_sig_err_ind_tlv_enum {
    mipc_sys_nv_sig_err_ind_tlv_NONE = 0,
    /* type = uint8_t */
    MIPC_SYS_NV_SIG_ERR_IND_T_ERROR_CODE                    = 0x100,
};

    /* MIPC_MSG.SYS_VODATA_STATISTICS_IND */
enum mipc_sys_vodata_statistics_ind_tlv_enum {
    mipc_sys_vodata_statistics_ind_tlv_NONE = 0,
    /* sim slot id, 0 based */
    /* type = uint8_t */
    MIPC_SYS_VODATA_STATISTICS_IND_T_SIM_ID                 = 0x100,
    /* Total tx bytes on internet interface for SIM (bytes) since interface up */
    /* type = uint32_t */
    MIPC_SYS_VODATA_STATISTICS_IND_T_TX_BYTES               = 0x101,
    /* Total rx bytes on internet interface for SIM (bytes) since interface up */
    /* type = uint32_t */
    MIPC_SYS_VODATA_STATISTICS_IND_T_RX_BYTES               = 0x102,
    /* Total tx pkts on internet interface for SIM since interface up */
    /* type = uint32_t */
    MIPC_SYS_VODATA_STATISTICS_IND_T_TX_PKT                 = 0x103,
    /* Total rx pkts on internet interface for SIM since interface up */
    /* type = uint32_t */
    MIPC_SYS_VODATA_STATISTICS_IND_T_RX_PKT                 = 0x104,
};

    /* MIPC_MSG.SYS_THERMAL_ACTUATOR_IND */
enum mipc_sys_thermal_actuator_ind_tlv_enum {
    mipc_sys_thermal_actuator_ind_tlv_NONE = 0,
    /* IMS only indicate = {0: disable, 1: enable}. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_THERMAL_ACTUATOR_IND_T_IMS_ONLY_IND            = 0x100,
    /* Flight mode indicate = {0: disable, 1: enable}. */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_THERMAL_ACTUATOR_IND_T_FLIGHT_MODE_IND         = 0x101,
    /* Charger indicate = {0: cancel, 1: notify} */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_THERMAL_ACTUATOR_IND_T_CHARGE_IND              = 0x102,
};

    /* MIPC_MSG.SYS_DMF_URC_IND */
enum mipc_sys_dmf_urc_ind_tlv_enum {
    mipc_sys_dmf_urc_ind_tlv_NONE = 0,
    /* DMF category ID */
    /* type = uint32_t */
    MIPC_SYS_DMF_URC_IND_T_CATEGORY_ID                      = 0x100,
    /* The byte array of DMF struct. */
    /* type = byte_array */
    MIPC_SYS_DMF_URC_IND_T_RAWDATA                          = 0x8101,
    /* DMF AT command string. */
    /* type = string */
    MIPC_SYS_DMF_URC_IND_T_ATSTRING                         = 0x8102,
};

    /* MIPC_MSG.SYS_META_CONTROL_IND */
enum mipc_sys_meta_control_ind_tlv_enum {
    mipc_sys_meta_control_ind_tlv_NONE = 0,
    /* This field is used to report system trace. */
    /* type = byte_array */
    MIPC_SYS_META_CONTROL_IND_T_SYSTRACE                    = 0x100,
};

    /* MIPC_MSG.SYS_HBA_TIMEOUT_IND */
enum mipc_sys_hba_timeout_ind_tlv_enum {
    mipc_sys_hba_timeout_ind_tlv_NONE = 0,
    /* current heartbeat proxy's unique ID */
    /* type = string */
    MIPC_SYS_HBA_TIMEOUT_IND_T_PROXY_KEY                    = 0x100,
    /* ipv4 terms of service */
    /* type = uint8_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_IPV4_TOS                     = 0x101,
    /* ipv4 Time To Live */
    /* type = uint8_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_IPV4_TTL                     = 0x102,
    /* ipv4 id */
    /* type = uint16_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_IPV4_ID                      = 0x103,
    /* ipv6 flow label */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_IPV6_FLOW_LABEL              = 0x104,
    /* ipv6 hop limit */
    /* type = uint8_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_IPV6_HOP_LIMIT               = 0x105,
    /* ipv6 traffic class */
    /* type = uint8_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_IPV6_TCLASS                  = 0x106,
    /* tcp receive wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_RCV_WSCALE               = 0x107,
    /* tcp send wscale */
    /* type = uint8_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_SND_WSCALE               = 0x108,
    /* tcp use wscale or not */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_WSCALE_OK                = 0x109,
    /* tcp send windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_SND_WINDOW               = 0x10A,
    /* tcp receive windown */
    /* type = uint16_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_RCV_WINDOW               = 0x10B,
    /* tcp send next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_SND_NXT_SEQ              = 0x10C,
    /* tcp receive next sequence */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_TCP_RCV_NXT_SEQ              = 0x10D,
    /* current TX cycle value */
    /* type = uint16_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_CURRENT_CYCLE                = 0x10E,
    /* number of heartbeat packet sent */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_SEND_COUNT                   = 0x10F,
    /* number of heartbeat packet replies received */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_RECV_COUNT                   = 0x110,
    /* RRC quick release times */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_SHORT_RRC_COUNT              = 0x111,
    /* number of hitchhike */
    /* type = uint32_t */
    MIPC_SYS_HBA_TIMEOUT_IND_T_HITCHHIKE_COUNT              = 0x112,
};

    /* MIPC_MSG.SYS_HBA_HW_FILTER_SRC_STATE_IND */
enum mipc_sys_hba_hw_filter_src_state_ind_tlv_enum {
    mipc_sys_hba_hw_filter_src_state_ind_tlv_NONE = 0,
    /* hardware filter source status. 0 - avaliabe (recovery from exhaust), 1 - exhaust */
    /* type = uint8_t */
    MIPC_SYS_HBA_HW_FILTER_SRC_STATE_IND_T_HW_FILTER_SRC_STATE = 0x100,
};

    /* MIPC_MSG.SYS_TIMER_EXPIRE_IND */
enum mipc_sys_timer_expire_ind_tlv_enum {
    mipc_sys_timer_expire_ind_tlv_NONE = 0,
    /* 0: false, 1: true */
    /* type = uint8_t, refer to BOOLEAN */
    MIPC_SYS_TIMER_EXPIRE_IND_T_TIMER_EXPIRE                = 0x100,
};

    /* MIPC_MSG.SYS_IDC_FRAME_CFG_IND */
enum mipc_sys_idc_frame_cfg_ind_tlv_enum {
    mipc_sys_idc_frame_cfg_ind_tlv_NONE = 0,
    /* Frame duration in microseconds. */
    /* type = uint16_t */
    MIPC_SYS_IDC_FRAME_CFG_IND_T_FRAME_DURATION             = 0x100,
    /* Frame offset in microseconds. */
    /* type = uint16_t */
    MIPC_SYS_IDC_FRAME_CFG_IND_T_FS_OFFSET                  = 0x101,
    /* Frame sync jitter in microseconds. */
    /* type = uint16_t */
    MIPC_SYS_IDC_FRAME_CFG_IND_T_FS_JITTER                  = 0x102,
    /* Duration of the period in microseconds. */
    /* type = uint16_t */
    MIPC_SYS_IDC_FRAME_CFG_IND_T_PERIOD_DURATION_TLV_ARRAY  = 0x103,
    /* Type of the period. */
    /* type = uint8_t, refer to SYS_IDC_PERIOD_TYPE */
    MIPC_SYS_IDC_FRAME_CFG_IND_T_PERIOD_TYPE_TLV_ARRAY      = 0x104,
};

    /* MIPC_MSG.SYS_IDC_SCAN_FREQ_IND */
enum mipc_sys_idc_scan_freq_ind_tlv_enum {
    mipc_sys_idc_scan_freq_ind_tlv_NONE = 0,
    /* Lower edge of the MWS scan frequency in MHz. */
    /* type = uint16_t */
    MIPC_SYS_IDC_SCAN_FREQ_IND_T_SCAN_FREQ_LOW_TLV_ARRAY    = 0x100,
    /* Upper edge of the MWS scan frequency in MHz. */
    /* type = uint16_t */
    MIPC_SYS_IDC_SCAN_FREQ_IND_T_SCAN_FREQ_HIGH_TLV_ARRAY   = 0x101,
};

    /* MIPC_MSG.SYS_IDC_CHANNEL_PARAM_IND */
enum mipc_sys_idc_channel_param_ind_tlv_enum {
    mipc_sys_idc_channel_param_ind_tlv_NONE = 0,
    /* MWS channel is enabled or disabled. */
    /* type = uint8_t */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_CHANNEL_ENABLE         = 0x100,
    /* RX center frequency(MHz) */
    /* type = uint16_t */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_RX_CENTER_FREQ         = 0x101,
    /* TX center frequency(MHz) */
    /* type = uint16_t */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_TX_CENTER_FREQ         = 0x102,
    /* RX channel bandwidth(kHz) */
    /* type = uint16_t */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_RX_CHANNEL_BW          = 0x103,
    /* TX channel bandwidth(kHz) */
    /* type = uint16_t */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_TX_CHANNEL_BW          = 0x104,
    /* Channel type */
    /* type = uint8_t, refer to SYS_IDC_CHANNEL_TYPE */
    MIPC_SYS_IDC_CHANNEL_PARAM_IND_T_CHANNEL_TYPE           = 0x105,
};

    /* MIPC_MSG.SYS_DMF_EM_ICD_INFO_IND */
enum mipc_sys_dmf_em_icd_info_ind_tlv_enum {
    mipc_sys_dmf_em_icd_info_ind_tlv_NONE = 0,
    /* dmf for em icd info raw data, lenght is controled by pdu len from dmf send */
    /* type = byte_array */
    MIPC_SYS_DMF_EM_ICD_INFO_IND_T_DMF_RAWDATA              = 0x100,
};

    /* MIPC_MSG.SYS_FORWARD_AT_IND */
enum mipc_sys_forward_at_ind_tlv_enum {
    mipc_sys_forward_at_ind_tlv_NONE = 0,
    /* type = string */
    MIPC_SYS_FORWARD_AT_IND_T_ATCMD                         = 0x8100,
};

    /* MIPC_MSG.SYS_PRIMARY_IMEI_SWITCH_IND */
enum mipc_sys_primary_imei_switch_ind_tlv_enum {
    mipc_sys_primary_imei_switch_ind_tlv_NONE = 0,
    /* Primary IMEI corresponding slog id */
    /* type = uint8_t */
    MIPC_SYS_PRIMARY_IMEI_SWITCH_IND_T_PRIMARY_SLOT_ID      = 0x100,
    /* Primary imei number of mobile deveice,IMEI is a 15-digit code,add one more 0 end. */
    /* type = string */
    MIPC_SYS_PRIMARY_IMEI_SWITCH_IND_T_PRIMARY_IMEI         = 0x8101,
    /* SVN is Software Version Number,IMEI is a 15-digit code used to uniquely identify a mobile device, such as a cell phone, and contains some information used to identify the device, including the manufacturer code, device type code, and the last two digits of the software version number (SVN), add one more 0 end. */
    /* type = string */
    MIPC_SYS_PRIMARY_IMEI_SWITCH_IND_T_IMEI_SVN             = 0x8102,
};

    /* MIPC_MSG.SYS_REBOOT_CMD */
enum mipc_sys_reboot_cmd_tlv_enum {
    mipc_sys_reboot_cmd_tlv_NONE = 0,
    /* specify the reboot mode */
    /* type = uint8_t, refer to SYS_REBOOT_MODE */
    MIPC_SYS_REBOOT_CMD_T_MODE                              = 0x100,
    /* Download Timeout (Unit: second, Only valid when MODE is DOWNLOAD or SETTIMEOUT) */
    /* type = uint32_t */
    MIPC_SYS_REBOOT_CMD_T_TIMEOUT                           = 0x101,
    /* new sbp id */
    /* type = uint32_t */
    MIPC_SYS_REBOOT_CMD_T_SBP_ID                            = 0x102,
};

    /* MIPC_MSG.SYS_REBOOT_RSP */
enum mipc_sys_reboot_rsp_tlv_enum {
    mipc_sys_reboot_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SET_CONFIG_DIPC_CMD */
enum mipc_sys_set_config_dipc_cmd_tlv_enum {
    mipc_sys_set_config_dipc_cmd_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_SET_CONFIG_DIPC_CMD_T_CLASS                    = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_SYS_SET_CONFIG_DIPC_CMD_T_TYPE                     = 0x101,
    /* the configuration data */
    /* type = byte_array */
    MIPC_SYS_SET_CONFIG_DIPC_CMD_T_DATA                     = 0x102,
};

    /* MIPC_MSG.SYS_SET_CONFIG_DIPC_RSP */
enum mipc_sys_set_config_dipc_rsp_tlv_enum {
    mipc_sys_set_config_dipc_rsp_tlv_NONE = 0,
    /* the response data */
    /* type = byte_array */
    MIPC_SYS_SET_CONFIG_DIPC_RSP_T_DATA                     = 0x100,
};

    /* MIPC_MSG.SYS_CONFIG_NEEDED_TO_UPDATE_CMD */
enum mipc_sys_config_needed_to_update_cmd_tlv_enum {
    mipc_sys_config_needed_to_update_cmd_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_CMD_T_CLASS            = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_CMD_T_TYPE             = 0x101,
};

    /* MIPC_MSG.SYS_CONFIG_NEEDED_TO_UPDATE_RSP */
enum mipc_sys_config_needed_to_update_rsp_tlv_enum {
    mipc_sys_config_needed_to_update_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_GET_CONFIG_DIPC_CMD */
enum mipc_sys_get_config_dipc_cmd_tlv_enum {
    mipc_sys_get_config_dipc_cmd_tlv_NONE = 0,
    /* the configuration class */
    /* type = uint32_t, refer to SYS_CONFIG_CLASS */
    MIPC_SYS_GET_CONFIG_DIPC_CMD_T_CLASS                    = 0x100,
    /* the string of configuration type */
    /* type = string */
    MIPC_SYS_GET_CONFIG_DIPC_CMD_T_TYPE                     = 0x101,
};

    /* MIPC_MSG.SYS_GET_CONFIG_DIPC_RSP */
enum mipc_sys_get_config_dipc_rsp_tlv_enum {
    mipc_sys_get_config_dipc_rsp_tlv_NONE = 0,
    /* the response data */
    /* type = byte_array */
    MIPC_SYS_GET_CONFIG_DIPC_RSP_T_DATA                     = 0x100,
};

    /* MIPC_MSG.SYS_SET_TIME_CMD */
enum mipc_sys_set_time_cmd_tlv_enum {
    mipc_sys_set_time_cmd_tlv_NONE = 0,
    /* type = struct, refer to nw_nitz_info */
    MIPC_SYS_SET_TIME_CMD_T_INFO                            = 0x100,
    /* if time_zone_offset_minutes in INFO field is valid, 0 means invalid and 1 means valid */
    /* type = uint8_t */
    MIPC_SYS_SET_TIME_CMD_T_TZ_VALID                        = 0x101,
};

    /* MIPC_MSG.SYS_SET_TIME_RSP */
enum mipc_sys_set_time_rsp_tlv_enum {
    mipc_sys_set_time_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SEND_CONSYS_WIFI_CMD */
enum mipc_sys_send_consys_wifi_cmd_tlv_enum {
    mipc_sys_send_consys_wifi_cmd_tlv_NONE = 0,
    /* Length of mcif payload. */
    /* type = uint16_t */
    MIPC_SYS_SEND_CONSYS_WIFI_CMD_T_PAYLOAD_LEN             = 0x100,
    /* Byte array of mcif payload. */
    /* type = byte_array */
    MIPC_SYS_SEND_CONSYS_WIFI_CMD_T_PAYLOAD                 = 0x8101,
};

    /* MIPC_MSG.SYS_SEND_CONSYS_WIFI_RSP */
enum mipc_sys_send_consys_wifi_rsp_tlv_enum {
    mipc_sys_send_consys_wifi_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_SEND_CONSYS_BT_CMD */
enum mipc_sys_send_consys_bt_cmd_tlv_enum {
    mipc_sys_send_consys_bt_cmd_tlv_NONE = 0,
    /* Length of mcif payload. */
    /* type = uint16_t */
    MIPC_SYS_SEND_CONSYS_BT_CMD_T_PAYLOAD_LEN               = 0x100,
    /* Byte array of mcif payload. */
    /* type = byte_array */
    MIPC_SYS_SEND_CONSYS_BT_CMD_T_PAYLOAD                   = 0x8101,
};

    /* MIPC_MSG.SYS_SEND_CONSYS_BT_RSP */
enum mipc_sys_send_consys_bt_rsp_tlv_enum {
    mipc_sys_send_consys_bt_rsp_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.SYS_AT_CMD */
enum mipc_sys_at_cmd_tlv_enum {
    mipc_sys_at_cmd_tlv_NONE = 0,
    /* AT command string send to the registed host. */
    /* type = string */
    MIPC_SYS_AT_CMD_T_ATCMD                                 = 0x8100,
};

    /* MIPC_MSG.SYS_AT_RSP */
enum mipc_sys_at_rsp_tlv_enum {
    mipc_sys_at_rsp_tlv_NONE = 0,
    /* AT command execution result. */
    /* type = string */
    MIPC_SYS_AT_RSP_T_ATCMD                                 = 0x8100,
};

    /* MIPC_MSG.SYS_FORWARD_AT_CMD */
enum mipc_sys_forward_at_cmd_tlv_enum {
    mipc_sys_forward_at_cmd_tlv_NONE = 0,
    /* Forwarded AT command string send to the registed host. */
    /* type = string */
    MIPC_SYS_FORWARD_AT_CMD_T_ATCMD                         = 0x8100,
};

    /* MIPC_MSG.SYS_FORWARD_AT_RSP */
enum mipc_sys_forward_at_rsp_tlv_enum {
    mipc_sys_forward_at_rsp_tlv_NONE = 0,
    /* AT command execution result. */
    /* type = string */
    MIPC_SYS_FORWARD_AT_RSP_T_ATCMD                         = 0x8100,
};

#endif /* __MIPC_MSG_SYS_CONST_H__ */
