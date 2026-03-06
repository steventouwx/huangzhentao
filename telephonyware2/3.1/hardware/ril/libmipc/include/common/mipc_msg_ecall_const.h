#ifndef __MIPC_MSG_ECALL_CONST_H__
#define __MIPC_MSG_ECALL_CONST_H__

enum MIPC_ECALL_MSG_enum {
    MIPC_ECALL_MSG_NONE = 0,
    /* update MSD data to MD. (for Telematics product) */
    MIPC_ECALL_IVS_UPDATE_MSD_REQ                           = 4096,
    MIPC_ECALL_IVS_UPDATE_MSD_CNF                           = 4097,

    /* reset eCall state machine in IVS modem, and connect speech to audio channel.(for Telematics product) */
    MIPC_ECALL_IVS_RESET_REQ                                = 4098,
    MIPC_ECALL_IVS_RESET_CNF                                = 4099,

    /* set/clear test eCall address, include number and URI. (for Telematics product) */
    MIPC_ECALL_IVS_SET_TEST_ADDR_REQ                        = 4100,
    MIPC_ECALL_IVS_SET_TEST_ADDR_CNF                        = 4101,

    /* set/clear reconfiguration eCall address, include number and URI. (for Telematics product) */
    MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ                      = 4102,
    MIPC_ECALL_IVS_SET_RECONF_ADDR_CNF                      = 4103,

    /* set the priority of eCall address, including default values in USIM and set by customer. Both test eCall and reconfiguration eCall apply this priority. The default priority is 1 > 3 > 2 > 4 (refer to ECALL_ADDRESS_PRIORITY_CLASS). (for Telematics product) */
    MIPC_ECALL_IVS_SET_ADDR_PRI_REQ                         = 4104,
    MIPC_ECALL_IVS_SET_ADDR_PRI_CNF                         = 4105,

    /* get eCall related information in USIM. (for Telematics product) */
    MIPC_ECALL_IVS_GET_SIM_INFO_REQ                         = 4106,
    MIPC_ECALL_IVS_GET_SIM_INFO_CNF                         = 4107,

    /* This command configures the eCall related timers for telematic products from TLV TIMER1 (T1) to TIMER10 (T10). All timer settings are persistent across modem reboot.TLV TIMER3 (T3) is always enabled and not configurable in modem with default value of 2 seconds. and the result for IVS_CONFIG_TIMER_REQ. TLV TIMER10 (T10) is configurable. T10 is always enabled in modem and only applicable when ECALL_MODE=MIPC_ECALL_MODE_TYPE_ECALL_ONLY. The control point can query T10 status using MIPC_ECALL_IVS_GET_ECALL_MODE_REQ.TLV TIMER2/5/6/7 are fully supported. TLV TIMER1/9 are reserved. other timers not listed are in default settings. This CNF message returns the current settings of all the timers in modem, including T1,T2,T3,T5,T6,T7,T9,T10. 0 means the corresponding timer is disabled in modem. */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ                         = 4108,
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF                         = 4109,

    /* Switch eCall mode between eCall_only and eCall_and_normal. (for Telematics product) */
    MIPC_ECALL_IVS_SET_ECALL_MODE_REQ                       = 4110,
    MIPC_ECALL_IVS_SET_ECALL_MODE_CNF                       = 4111,

    /* This command is to query eCall mode information. (for Telematics product) */
    MIPC_ECALL_IVS_GET_ECALL_MODE_REQ                       = 4112,
    MIPC_ECALL_IVS_GET_ECALL_MODE_CNF                       = 4113,

    /* indicate the status of eCall. (for Telematics product) */
    MIPC_ECALL_STATUS_IND                                   = 20480,


};

    /* MIPC_MSG.ECALL_IVS_UPDATE_MSD_REQ */
enum mipc_ecall_ivs_update_msd_req_tlv_enum {
    mipc_ecall_ivs_update_msd_req_tlv_NONE = 0,
    /* the format of the provided eCall MSD */
    /* type = uint8_t, refer to ECALL_MSD_FORMAT */
    MIPC_ECALL_IVS_UPDATE_MSD_REQ_T_MSD_FORMAT              = 0x100,
    /* Minimum set of data. see CEN EN 15722:2015 */
    /* type = byte_array */
    MIPC_ECALL_IVS_UPDATE_MSD_REQ_T_MSD_DATA                = 0x101,
};

    /* MIPC_MSG.ECALL_IVS_UPDATE_MSD_CNF */
enum mipc_ecall_ivs_update_msd_cnf_tlv_enum {
    mipc_ecall_ivs_update_msd_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_RESET_REQ */
enum mipc_ecall_ivs_reset_req_tlv_enum {
    mipc_ecall_ivs_reset_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_RESET_CNF */
enum mipc_ecall_ivs_reset_cnf_tlv_enum {
    mipc_ecall_ivs_reset_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_SET_TEST_ADDR_REQ */
enum mipc_ecall_ivs_set_test_addr_req_tlv_enum {
    mipc_ecall_ivs_set_test_addr_req_tlv_NONE = 0,
    /* indicate the type of ADDRESS. if the ADDR_TYPE is NONE, both the number and URI of test eCall will be cleared */
    /* type = uint32_t, refer to CALL_DIAL_ADDRESS_TYPE */
    MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDR_TYPE            = 0x100,
    /* eCall number or URI. if there is no ADDRESS, the number or URI of test eCall will be cleared */
    /* type = string */
    MIPC_ECALL_IVS_SET_TEST_ADDR_REQ_T_ADDRESS              = 0x101,
};

    /* MIPC_MSG.ECALL_IVS_SET_TEST_ADDR_CNF */
enum mipc_ecall_ivs_set_test_addr_cnf_tlv_enum {
    mipc_ecall_ivs_set_test_addr_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_SET_RECONF_ADDR_REQ */
enum mipc_ecall_ivs_set_reconf_addr_req_tlv_enum {
    mipc_ecall_ivs_set_reconf_addr_req_tlv_NONE = 0,
    /* indicate the type of ADDRESS. if the ADDR_TYPE is NONE, both the number and URI of reconfiguration eCall will be cleared */
    /* type = uint32_t, refer to CALL_DIAL_ADDRESS_TYPE */
    MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDR_TYPE          = 0x100,
    /* eCall number or URI. if there is no ADDRESS, the number or URI of reconfiguration eCall will be cleared */
    /* type = string */
    MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ_T_ADDRESS            = 0x101,
};

    /* MIPC_MSG.ECALL_IVS_SET_RECONF_ADDR_CNF */
enum mipc_ecall_ivs_set_reconf_addr_cnf_tlv_enum {
    mipc_ecall_ivs_set_reconf_addr_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_SET_ADDR_PRI_REQ */
enum mipc_ecall_ivs_set_addr_pri_req_tlv_enum {
    mipc_ecall_ivs_set_addr_pri_req_tlv_NONE = 0,
    /* the first priority address used by test eCall and reconfiguration eCall. */
    /* type = uint8_t, refer to ECALL_ADDRESS_PRIORITY_CLASS */
    MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_FIRST_PRI             = 0x100,
    /* the second priority address used by test eCall and reconfiguration eCall. Be different with FIRST_PRI */
    /* type = uint8_t, refer to ECALL_ADDRESS_PRIORITY_CLASS */
    MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_SECOND_PRI            = 0x101,
    /* the third priority address used by test eCall and reconfiguration eCall. Be different with FIRST_PRI and SECOND_PRI */
    /* type = uint8_t, refer to ECALL_ADDRESS_PRIORITY_CLASS */
    MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_THIRD_PRI             = 0x102,
    /* the fourth priority address used by test eCall and reconfiguration eCall. Be different with FIRST_PRI, SECOND_PRI and THIRD_PRI */
    /* type = uint8_t, refer to ECALL_ADDRESS_PRIORITY_CLASS */
    MIPC_ECALL_IVS_SET_ADDR_PRI_REQ_T_FOURTH_PRI            = 0x103,
};

    /* MIPC_MSG.ECALL_IVS_SET_ADDR_PRI_CNF */
enum mipc_ecall_ivs_set_addr_pri_cnf_tlv_enum {
    mipc_ecall_ivs_set_addr_pri_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_GET_SIM_INFO_REQ */
enum mipc_ecall_ivs_get_sim_info_req_tlv_enum {
    mipc_ecall_ivs_get_sim_info_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_GET_SIM_INFO_CNF */
enum mipc_ecall_ivs_get_sim_info_cnf_tlv_enum {
    mipc_ecall_ivs_get_sim_info_cnf_tlv_NONE = 0,
    /* indicate the type of sim */
    /* type = uint8_t, refer to ECALL_SIM_TYPE */
    MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_SIM_TYPE              = 0x100,
    /* test eCall URI in USIM */
    /* type = string */
    MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_TEST_ECALL_URI        = 0x101,
    /* test eCall number in USIM */
    /* type = string */
    MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_TEST_ECALL_NUM        = 0x102,
    /* reconfiguration eCall URI in USIM */
    /* type = string */
    MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_RECONF_ECALL_URI      = 0x103,
    /* reconfiguration eCall number in USIM */
    /* type = string */
    MIPC_ECALL_IVS_GET_SIM_INFO_CNF_T_RECONF_ECALL_NUM      = 0x104,
};

    /* MIPC_MSG.ECALL_IVS_CONFIG_TIMER_REQ */
enum mipc_ecall_ivs_config_timer_req_tlv_enum {
    mipc_ecall_ivs_config_timer_req_tlv_NONE = 0,
    /* configuartion for T1, Manually initiated eCall (MIeC) false triggering cancellation period. (not supported) */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER1                = 0x100,
    /* configuartion for T2, IVS Call Cleardown Fallback Timer (CCFT). time value range(second): 0(default), timer disabled; 1-43200seconds, timer enabled with this value; 0xFFFE, not specified, previous setting will be used; 0xFFFF, timer enabled with default value 3600s(1h). */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER2                = 0x101,
    /* configuartion for T3, IVS INITIATION signal duration, always enabled in MD with default 2s and cannot be changed. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER3                = 0x102,
    /* configuartion for T5, IVS wait for SEND MSD period. time value range(second): 0(default), timer disabled; 1-43200seconds, timer enabled with this value; 0xFFFE, not specified, previous setting will be used; 0xFFFF, timer enabled with default value 5s. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER5                = 0x103,
    /* configuartion for T6, IVS wait for AL-ACK period. time value range(second): 0(default), timer disabled; 1-43200seconds, timer enabled with this value; 0xFFFE, not specified, previous setting will be used; 0xFFFF, timer enabled with default value 5s. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER6                = 0x104,
    /* configuartion for T7, IVS MSD maximum transmission time. time value range(second): 0(default), timer disabled; 1-43200seconds, timer enabled with this value; 0xFFFE, not specified, previous setting will be used; 0xFFFF, timer enabled with default value 20s. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER7                = 0x105,
    /* configuartion for T9, IVS NAD minimum network registration period, default 3600s.(not supported) */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER9                = 0x106,
    /* configuartion for T10, IVS NAD network 'Deregistration Fallback Timer' (DTF), only for 'eCall only'. time value range (minute): 1-720 minutes, timer enabled with this value; 0xFFFE, not specified, previous setting will be used; 0xFFFF, timer enabled with default value 720min(12h). */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_REQ_T_TIMER10               = 0x107,
};

    /* MIPC_MSG.ECALL_IVS_CONFIG_TIMER_CNF */
enum mipc_ecall_ivs_config_timer_cnf_tlv_enum {
    mipc_ecall_ivs_config_timer_cnf_tlv_NONE = 0,
    /* configuartion for ecall timer T1, currently always return 0. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER1                = 0x100,
    /* configuartion for ecall timer T2 */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER2                = 0x101,
    /* configuartion for ecall timer T3, currently always return 2. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER3                = 0x102,
    /* configuartion for ecall timer T5 */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER5                = 0x103,
    /* configuartion for ecall timer T6 */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER6                = 0x104,
    /* configuartion for ecall timer T7 */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER7                = 0x105,
    /* configuartion for ecall timer T9, currently always return 0. */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER9                = 0x106,
    /* configuartion for ecall timer T10 */
    /* type = uint16_t */
    MIPC_ECALL_IVS_CONFIG_TIMER_CNF_T_TIMER10               = 0x107,
};

    /* MIPC_MSG.ECALL_IVS_SET_ECALL_MODE_REQ */
enum mipc_ecall_ivs_set_ecall_mode_req_tlv_enum {
    mipc_ecall_ivs_set_ecall_mode_req_tlv_NONE = 0,
    /* Indicate the type of eCall mode */
    /* type = uint8_t, refer to ECALL_MODE_TYPE */
    MIPC_ECALL_IVS_SET_ECALL_MODE_REQ_T_ECALL_MODE          = 0x100,
};

    /* MIPC_MSG.ECALL_IVS_SET_ECALL_MODE_CNF */
enum mipc_ecall_ivs_set_ecall_mode_cnf_tlv_enum {
    mipc_ecall_ivs_set_ecall_mode_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_GET_ECALL_MODE_REQ */
enum mipc_ecall_ivs_get_ecall_mode_req_tlv_enum {
    mipc_ecall_ivs_get_ecall_mode_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.ECALL_IVS_GET_ECALL_MODE_CNF */
enum mipc_ecall_ivs_get_ecall_mode_cnf_tlv_enum {
    mipc_ecall_ivs_get_ecall_mode_cnf_tlv_NONE = 0,
    /* Current mode of eCall */
    /* type = uint8_t, refer to ECALL_MODE_TYPE */
    MIPC_ECALL_IVS_GET_ECALL_MODE_CNF_T_ECALL_MODE          = 0x100,
    /* Timer10 (T10) status. This TLV is only applicable when TLV ECALL_MODE = MIPC_ECALL_MODE_TYPE_ECALL_ONLY */
    /* type = uint8_t, refer to ECALL_T10_STATUS */
    MIPC_ECALL_IVS_GET_ECALL_MODE_CNF_T_TIMER10_STATUS      = 0x101,
};

    /* MIPC_MSG.ECALL_STATUS_IND */
enum mipc_ecall_status_ind_tlv_enum {
    mipc_ecall_status_ind_tlv_NONE = 0,
    /* type = uint8_t, refer to ECALL_STATUS */
    MIPC_ECALL_STATUS_IND_T_STATE                           = 0x100,
    /* call id for eCall */
    /* type = uint8_t */
    MIPC_ECALL_STATUS_IND_T_CALL_ID                         = 0x101,
};




#endif /* __MIPC_MSG_ECALL_CONST_H__ */
