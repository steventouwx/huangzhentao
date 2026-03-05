/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_nw_common.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_nw_common相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_NW_COMMON_H__
#define __SV_NW_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SV_NW_MODE_NONE    0x00    /**<  No network. */
#define SV_NW_MODE_GSM     0x01    /**<  Include GSM networks. */
#define SV_NW_MODE_WCDMA   0x02    /**<  Include WCDMA networks. */
#define SV_NW_MODE_CDMA    0x04    /**<  Include CDMA networks. */
#define SV_NW_MODE_EVDO    0x08    /**<  Include EVDO networks. */
#define SV_NW_MODE_LTE     0x10    /**<  Include LTE networks. */
#define SV_NW_MODE_TDSCDMA 0x20    /**<  Include TDSCDMA networks. */
#define SV_NW_MODE_NR5G    0x40    /**<  Include NR5G networks. */
#define SV_NW_MODE_PRL     0x10000 /**<  Give preference according to the preferred roaming list. */

#define SV_NW_NR5G_SO_NONE      0X00 /**<  No service option */
#define SV_NW_NR5G_SO_TDD       0X01 /**<  Include service option TDD, Time Division Duplex */
#define SV_NW_NR5G_SO_SUB6      0X02 /**<  Include service option SUB6, 5G frequency, SUB_6GHz */
#define SV_NW_NR5G_SO_MMWAVE    0X04 /**<  Include service option MMWARE, 5G frequency, 30~300GHz */
#define SV_NW_NR5G_SO_NSA       0X08 /**<  Include service option NSA, Non_Stand Alone, 5G base station and 4G core network */
#define SV_NW_NR5G_SO_SA        0X10 /**<  Include service option SA, Stan Alone, 5G base station and 5G core network */

#define SV_NW_LOWER_POWER_MASK_DISABLE  0x00
#define SV_NW_LOWER_POWER_MASK_NORMAL   0x01
#define SV_NW_LOWER_POWER_MASK_NETWORK  0x02
#define SV_NW_LOWER_POWER_MASK_SIM      0x04
#define SV_NW_LOWER_POWER_MASK_SMS      0x08
#define SV_NW_LOWER_POWER_MASK_VOICE    0x10

#define SV_NW_NITZ_BUF_LEN      30
#define SV_NW_SCAN_MAX_LIST_NUM 40
#define SV_NW_MAX_GSM_CELL_INFO_NUM     20
#define SV_NW_MAX_UMTS_CELL_INFO_NUM    20
#define SV_NW_MAX_LTE_CELL_INFO_NUM     20

#define SV_NW_MAX_WEA_ALERT_LENGTH_V01  512
#define SV_NW_MAX_ETWS_ALERT_LENGTH_V01 256
#define SV_NW_RAT_DEYN_REASON_MAX_V01 8

/**<  the signal strength level*/
typedef enum SV_NW_SIGNAL_STRENGTH_LEVEL_ENUM
{
    SV_NW_SIGNAL_STRENGTH_LEVEL_MIN             = -1,
    SV_NW_SIGNAL_STRENGTH_LEVEL_NONE            = 0,  /**<  No signal*/
    SV_NW_SIGNAL_STRENGTH_LEVEL_POOR            = 1,  /**<  Poor signal*/
    SV_NW_SIGNAL_STRENGTH_LEVEL_MODERATE        = 2,  /**<  Moderate signal*/
    SV_NW_SIGNAL_STRENGTH_LEVEL_GOOD            = 3,  /**<  Good signal*/
    SV_NW_SIGNAL_STRENGTH_LEVEL_GREAT           = 4,  /**<  Great signal*/
    SV_NW_SIGNAL_STRENGTH_LEVEL_MAX
}SV_NW_SIGNAL_STRENGTH_LEVEL_E;

/**<  roaming notification status*/
typedef enum SV_NW_ROAM_STATE_TYPE_ENUM
{
    SV_NW_ROAM_STATE_OFF  = 0,    /**<  None, or roaming indicator off. */
    SV_NW_ROAM_STATE_ON   = 1     /**<  Roaming indicator on. */
}SV_NW_ROAM_STATE_TYPE_E;

/**<  network status*/
typedef enum SV_NW_NETWORK_STATUS_TYPE_ENUM
{
    SV_NW_NETWORK_STATUS_NONE            = 0,    /**<  Unknown network status. */
    SV_NW_NETWORK_STATUS_CURRENT_SERVING = 1,    /**<  The serving network. */
    SV_NW_NETWORK_STATUS_PREFERRED       = 2,    /**<  Preferred network. */
    SV_NW_NETWORK_STATUS_NOT_PREFERRED   = 3,    /**<  Non-preferred network. */
    SV_NW_NETWORK_STATUS_AVAILABLE       = 4,    /**<  Available network. */
    SV_NW_NETWORK_STATUS_FORBIDDEN       = 5     /**<  Forbidden network. */
}SV_NW_NETWORK_STATUS_TYPE_E;

/**<  radio access technologies*/
typedef enum SV_NW_RADIO_TECH_TYPE_ENUM
{
    SV_NW_RADIO_TECH_TD_SCDMA = 1,
    SV_NW_RADIO_TECH_GSM      = 2,    /**<  GSM; only supports voice. */
    SV_NW_RADIO_TECH_HSPAP    = 3,    /**<  HSPA. */
    SV_NW_RADIO_TECH_LTE      = 4,    /**<  LTE. */
    SV_NW_RADIO_TECH_EHRPD    = 5,    /**<  EHRPD. */
    SV_NW_RADIO_TECH_EVDO_B   = 6,    /**<  EVDO B. */
    SV_NW_RADIO_TECH_HSPA     = 7,    /**<  HSPA. */
    SV_NW_RADIO_TECH_HSUPA    = 8,    /**<  HSUPA. */
    SV_NW_RADIO_TECH_HSDPA    = 9,    /**<  HSDPA. */
    SV_NW_RADIO_TECH_EVDO_A   = 10,   /**<  EVDO A. */
    SV_NW_RADIO_TECH_EVDO_0   = 11,   /**<  EVDO 0. */
    SV_NW_RADIO_TECH_1xRTT    = 12,   /**<  1xRTT. */
    SV_NW_RADIO_TECH_IS95B    = 13,   /**<  IS95B. */
    SV_NW_RADIO_TECH_IS95A    = 14,   /**<  IS95A. */
    SV_NW_RADIO_TECH_UMTS     = 15,   /**<  UMTS. */
    SV_NW_RADIO_TECH_EDGE     = 16,   /**<  EDGE. */
    SV_NW_RADIO_TECH_GPRS     = 17,   /**<  GPRS. */
    SV_NW_RADIO_TECH_NR5G     = 18,   /**<  NR5G. */
    SV_NW_RADIO_TECH_NONE     = 19    /**<  No technology selected. */
}SV_NW_RADIO_TECH_TYPE_E;

/**<  technical specification type*/
typedef enum SV_NW_TECH_DOMAIN_TYPE_ENUM
{
    SV_NW_TECH_DOMAIN_NONE    = 0,    /**<  Unknown technical specification. */
    SV_NW_TECH_DOMAIN_3GPP    = 1,    /**<  3GPP. */
    SV_NW_TECH_DOMAIN_3GPP2   = 2,    /**<  3GPP2. */
}SV_NW_TECH_DOMAIN_TYPE_E;

/**<  The reason for the rejection of network registration*/
typedef enum SV_NW_DENY_REASON_TYPE_ENUM
{
    SV_NW_IMSI_UNKNOWN_IN_HSS_DENY_REASON                                   = 2, /**<  IMSI unknown in HSS */
    SV_NW_ILLEGAL_UE_DENY_REASON                                            = 3, /**<  Illegal UE */
    SV_NW_IMEI_NOT_ACCEPTED_DENY_REASON                                     = 5, /**<  IMEI not accepted */
    SV_NW_PEI_NOT_ACCEPTED_DENY_REASON                                      = 5, /**<  PEI not accepted */
    SV_NW_ILLEGAL_ME_DENY_REASON                                            = 6, /**<  Illegal ME */
    SV_NW_EPS_SERVICES_NOT_ALLOWED_DENY_REASON                              = 7, /**<  EPS services not allowed */
    SV_NW_5GS_SERVICES_NOT_ALLOWED_DENY_REASON                              = 7, /**<  5GS services not allowed */
    SV_NW_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED_DENY_REASON         = 8, /**<  EPS services and non-EPS services not allowed */
    SV_NW_UE_ID_CANNOT_BE_DERIVED_BY_THE_NETWORK_DENY_REASON                = 9, /**<  UE identifity cannot be derived by the network */
    SV_NW_IMPLICITY_DETACHED_DENY_REASON                                    = 10, /**<  Implicitly detached */
    SV_NW_PLMN_NOT_ALLOWED_DENY_REASON                                      = 11, /**<  PLMN not allowed */
    SV_NW_TRACKING_AREA_NOT_ALLOWED_DENY_REASON                             = 12, /**<  Tracking area not allowed */
    SV_NW_ROAMING_NOT_ALLOWED_IN_THIS_TRACNING_AREA_DENY_REASON             = 13, /**<  Roaming not allowed in this tracking area */
    SV_NW_EPS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN_DENY_REASON                 = 14, /**<  EPS services not allowed in this PLMN */
    SV_NW_NO_SUITALBE_CELLS_IN_TRACKING_AREA_DENY_REASON                    = 15, /**<  No suitable cells in tracking area */
    SV_NW_MSC_TEMPORARILY_NOT_REACHABLE_DENY_REASON                         = 16, /**<  MSC temporarily not reachable */
    SV_NW_NETWORK_FAILURE_DENY_REASON                                       = 17, /**<  Network failure */
    SV_NW_CS_DOMAIN_NOT_AVAILABLE_DENY_REASON                               = 18, /**<  CS domain not available */
    SV_NW_EMS_FAILURE_DENY_REASON                                           = 19, /**<  ESM failure */
    SV_NW_MAC_FAILURE_DENY_REASON                                           = 20, /**<  MAC failure */
    SV_NW_SYNC_FAILURE_DENY_REASON                                          = 21, /**<  Synch failure */
    SV_NW_CONGESTION_DENY_REASON                                            = 22, /**<  Congestion */
    SV_NW_UE_SECURITY_CAPABILITIES_MISMATCH_DENY_REASON                     = 23, /**<  UE security capabilities mismatch */
    SV_NW_SECURITY_MODE_REJECTED_DENY_REASON                                = 24, /**<  Security mode rejected, unspecified */
    SV_NW_NOT_AUTHORIZED_FOR_THIS_CSG_DENY_REASON                           = 25, /**<  Not authorized for this CSG */
    SV_NW_NON_EPS_AUTHENTICATION_UNCACCEPTABLE_DENY_REASON                  = 26, /**<  Non-EPS authentication unacceptable */
    SV_NW_NON_5GS_AUTHENTICATION_UNCACCEPTABLE_DENY_REASON                  = 26, /**<  Non-5GS authentication unacceptable */
    SV_NW_N1_MODE_NOT_ALLOWED_DENY_REASON                                   = 27, /**<  N1 mode not allowed */
    SV_NW_RESTRICTED_SERVICE_AREA_DENY_REASON                               = 28, /**<  restricted service area */
    SV_NW_REDIRECTION_TO_EPC_REQUIRED_DENY_REASON                           = 31, /**<  redirection to epc required */
    SV_NW_REQUESTED_SERVICE_OPTION_NOT_AUTHORIZED_IN_THIS_PLMN_DENY_REASON  = 35, /**<  Requested service option not authorized in this PLMN */
    SV_NW_CS_SERVICE_TEMPORARILY_NOT_AVAILABLE_DENY_REASON                  = 39, /**<  CS service temporarily not available */
    SV_NW_NO_ESP_BEARER_CONTEXT_ACTIVATED_DENY_REASON                       = 40, /**<  No EPS bearer context activated */
    SV_NW_LADN_NOT_AVAILABLE_DENY_REASON                                    = 43, /**<  LADN not available */
    SV_NW_NO_NETWORK_SLICES_AVAILABLE_DENY_REASON                           = 62, /**<  No network slices available */
    SV_NW_MAX_NUMBER_OF_PDU_SESSIONS_REACHED_DENY_REASON                    = 65, /**<  Max number of PDU sessions reached */
    SV_NW_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN_DENY_REASON     = 67, /**<  Insufficient resources for specific slice and dnn */
    SV_NW_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_DENY_REASON             = 69, /**<  Insufficient resources for specific slice */
    SV_NW_NGKSI_ALREADY_IN_USE_DENY_REASON                                  = 71, /**<  NGKSI already in use */
    SV_NW_NON_3GPP_ACCESS_TO_5GCN_NOT_ALLOWED_DENY_REASON                   = 72, /**<  Non 3gpp access to 5GCN not allowed */
    SV_NW_SERVICE_NETWORK_NOT_AUTHORIZED_DENY_REASON                        = 73, /**<  Service network not authorized */
    SV_NW_TEMPORARILY_NOT_AUTHORIZED_FOR_THIS_SNPN_DENY_REASON              = 74, /**<  Temporarily not authorized for this snpn */
    SV_NW_PERMANENTLY_NOT_AUTHORIZED_FOR_THIS_SNPN_DENY_REASON              = 75, /**<  Permanently not authorized for this snpn */
    SV_NW_NOT_AUTHORIZED_FOR_THIS_CAG_OR_AUTHORIZED_FOR_CAG_CELLS_ONLY_DENY_REASON = 76, /**<  Not authorized for this CAG or authorized for CAG cells only */
    SV_NW_WIRELINE_ACCESS_AREA_NOT_ALLOWED_DENY_REASON                      = 77, /**<  WIRELINE access area not allowed */
    SV_NW_PAYLOAD_WAS_NOT_FORWARDED_DENY_REASON                             = 90, /**<  Payload was not forwarded */
    SV_NW_DNN_NOT_SUPPORTED_WITH_NO_SLICE_OR_IN_SPECIFIC_SLICE_DENY_REASON  = 91, /**<  DNN not supported with no slice or in specific slice */
    SV_NW_INSUFFUCIENT_USERPLANE_RESOURCES_FOR_PDU_SESS_DENY_REASON         = 92, /**<  Insuffucient userplane resources for pdu sess */
    SV_NW_SEMAMTICALLY_INCORRECT_MESSAGE_DENY_REASON                        = 95, /**<  Semantically incorrect message. */
    SV_NW_INVALID_MANDATORY_INFORMATION_DENY_REASON                         = 96, /**<  Invalid mandatory information. */
    SV_NW_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED_DENY_REASON          = 97, /**<  Message type non-existent or not implemented. */
    SV_NW_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCAL_STATE_DENY_REASON       = 98, /**<  Message type not compatible with protocol state. */
    SV_NW_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED_DENY_REASON   = 99, /**<   Information element non-existent or not implemented. */
    SV_NW_CONDITIONAL_IE_ERROR_DENY_REASON                                  = 100, /**<  Conditional IE error. */
    SV_NW_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCAL_STATE_DENY_REASON            = 101, /**<  Message not compatible with protocol state. */
    SV_NW_PROTOCAL_ERROR_DENY_REASON                                        = 111, /**<  Protocol error, unspecified. */
}SV_NW_DENY_REASON_TYPE_E;

/**<  RF BAND INFO*/
typedef enum SV_NW_BAND_INFO_ENUM
{
    SV_NW_ACTIVE_BAND_CDMA_0_V01 = 0, 
    SV_NW_ACTIVE_BAND_CDMA_1_V01 = 1, 
    SV_NW_ACTIVE_BAND_CDMA_3_V01 = 3, 
    SV_NW_ACTIVE_BAND_CDMA_4_V01 = 4, 
    SV_NW_ACTIVE_BAND_CDMA_5_V01 = 5, 
    SV_NW_ACTIVE_BAND_CDMA_6_V01 = 6, 
    SV_NW_ACTIVE_BAND_CDMA_7_V01 = 7, 
    SV_NW_ACTIVE_BAND_CDMA_8_V01 = 8, 
    SV_NW_ACTIVE_BAND_CDMA_9_V01 = 9, 
    SV_NW_ACTIVE_BAND_CDMA_10_V01 = 10, 
    SV_NW_ACTIVE_BAND_CDMA_11_V01 = 11, 
    SV_NW_ACTIVE_BAND_CDMA_12_V01 = 12, 
    SV_NW_ACTIVE_BAND_CDMA_13_V01 = 13, 
    SV_NW_ACTIVE_BAND_CDMA_14_V01 = 14, 
    SV_NW_ACTIVE_BAND_CDMA_15_V01 = 15, 
    SV_NW_ACTIVE_BAND_CDMA_16_V01 = 16, 
    SV_NW_ACTIVE_BAND_CDMA_17_V01 = 17, 
    SV_NW_ACTIVE_BAND_CDMA_18_V01 = 18, 
    SV_NW_ACTIVE_BAND_CDMA_19_V01 = 19, 
    SV_NW_ACTIVE_BAND_GSM_450_V01 = 40, 
    SV_NW_ACTIVE_BAND_GSM_480_V01 = 41, 
    SV_NW_ACTIVE_BAND_GSM_750_V01 = 42, 
    SV_NW_ACTIVE_BAND_GSM_850_V01 = 43, 
    SV_NW_ACTIVE_BAND_GSM_900_EXTENDED_V01 = 44, 
    SV_NW_ACTIVE_BAND_GSM_900_PRIMARY_V01 = 45, 
    SV_NW_ACTIVE_BAND_GSM_900_RAILWAYS_V01 = 46, 
    SV_NW_ACTIVE_BAND_GSM_1800_V01 = 47, 
    SV_NW_ACTIVE_BAND_GSM_1900_V01 = 48, 
    SV_NW_ACTIVE_BAND_WCDMA_2100_V01 = 80, 
    SV_NW_ACTIVE_BAND_WCDMA_PCS_1900_V01 = 81, 
    SV_NW_ACTIVE_BAND_WCDMA_DCS_1800_V01 = 82, 
    SV_NW_ACTIVE_BAND_WCDMA_1700_US_V01 = 83, 
    SV_NW_ACTIVE_BAND_WCDMA_850_V01 = 84, 
    SV_NW_ACTIVE_BAND_WCDMA_800_V01 = 85, 
    SV_NW_ACTIVE_BAND_WCDMA_2600_V01 = 86, 
    SV_NW_ACTIVE_BAND_WCDMA_900_V01 = 87, 
    SV_NW_ACTIVE_BAND_WCDMA_1700_JAPAN_V01 = 88, 
    SV_NW_ACTIVE_BAND_WCDMA_1500_JAPAN_V01 = 90, 
    SV_NW_ACTIVE_BAND_WCDMA_850_JAPAN_V01 = 91, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_1_V01 = 120, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_2_V01 = 121, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_3_V01 = 122, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_4_V01 = 123, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_5_V01 = 124, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_6_V01 = 125, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_7_V01 = 126, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_8_V01 = 127, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_9_V01 = 128, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_10_V01 = 129, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_11_V01 = 130, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_12_V01 = 131, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_13_V01 = 132, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_14_V01 = 133, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_17_V01 = 134, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_33_V01 = 135, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_34_V01 = 136, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_35_V01 = 137, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_36_V01 = 138, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_37_V01 = 139, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_38_V01 = 140, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_39_V01 = 141, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_40_V01 = 142, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_18_V01 = 143, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_19_V01 = 144, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_20_V01 = 145, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_21_V01 = 146, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_24_V01 = 147, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_25_V01 = 148, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_41_V01 = 149, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_42_V01 = 150, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_43_V01 = 151, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_23_V01 = 152, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_26_V01 = 153, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_32_V01 = 154, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_125_V01 = 155, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_126_V01 = 156, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_127_V01 = 157, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_28_V01 = 158, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_29_V01 = 159, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_30_V01 = 160, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_66_V01 = 161, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_250_V01 = 162, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_46_V01 = 163, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_27_V01 = 164, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_31_V01 = 165, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_71_V01 = 166, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_47_V01 = 167, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_48_V01 = 168, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_67_V01 = 169, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_68_V01 = 170, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_49_V01 = 171, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_85_V01 = 172, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_72_V01 = 173, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_73_V01 = 174, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_86_V01 = 175, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_53_V01 = 176, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_87_V01 = 177, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_88_V01 = 178, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_70_V01 = 179, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_44_V01 = 180, 
    SV_NW_ACTIVE_BAND_LTE_OPERATING_BAND_256_V01 = 181, 
    SV_NW_ACTIVE_BAND_TDSCDMA_BAND_A_V01 = 200, 
    SV_NW_ACTIVE_BAND_TDSCDMA_BAND_B_V01 = 201, 
    SV_NW_ACTIVE_BAND_TDSCDMA_BAND_C_V01 = 202, 
    SV_NW_ACTIVE_BAND_TDSCDMA_BAND_D_V01 = 203, 
    SV_NW_ACTIVE_BAND_TDSCDMA_BAND_E_V01 = 204, 
    SV_NW_ACTIVE_BAND_TDSCDMA_BAND_F_V01 = 205, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_1_V01 = 250, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_2_V01 = 251, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_3_V01 = 252, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_5_V01 = 253, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_7_V01 = 254, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_8_V01 = 255, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_20_V01 = 256, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_28_V01 = 257, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_38_V01 = 258, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_41_V01 = 259, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_50_V01 = 260, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_51_V01 = 261, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_66_V01 = 262, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_70_V01 = 263, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_71_V01 = 264, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_74_V01 = 265, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_75_V01 = 266, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_76_V01 = 267, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_77_V01 = 268, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_78_V01 = 269, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_79_V01 = 270, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_80_V01 = 271, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_81_V01 = 272, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_82_V01 = 273, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_83_V01 = 274, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_84_V01 = 275, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_85_V01 = 276, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_257_V01 = 277, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_258_V01 = 278, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_259_V01 = 279, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_260_V01 = 280, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_261_V01 = 281, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_12_V01 = 282, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_25_V01 = 283, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_34_V01 = 284, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_39_V01 = 285, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_40_V01 = 286, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_65_V01 = 287, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_86_V01 = 288, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_48_V01 = 289, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_14_V01 = 290, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_13_V01 = 291, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_18_V01 = 292, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_26_V01 = 293, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_30_V01 = 294, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_29_V01 = 295, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_53_V01 = 296, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_46_V01 = 297, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_91_V01 = 298, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_92_V01 = 299, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_93_V01 = 300, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_94_V01 = 301, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_96_V01 = 302, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_106_V01 = 303, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_67_V01 = 304, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_68_V01 = 305, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_89_V01 = 306, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_95_V01 = 307, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_97_V01 = 308, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_98_V01 = 309, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_99_V01 = 310, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_104_V01 = 311, 
    SV_NW_ACTIVE_BAND_NR5G_BAND_47_V01 = 312
}SV_NW_BAND_INFO_E;

/**<  network service type*/
typedef enum SV_NW_SERVICE_TYPE_ENUM
{
    SV_NW_SERVICE_NONE    = 0,  /**<  No service. */
    SV_NW_SERVICE_LIMITED = 1,  /**<  Restricted service. */
    SV_NW_SERVICE_FULL    = 2,  /**<  Normal service. */
}SV_NW_SERVICE_TYPE_E;

typedef enum SV_NW_SELECTION_TYPE_ENUM
{
    SV_NW_SELECTION_AUTOMATIC = 0,     /**<  Automatic network selection. */
    SV_NW_SELECTION_MANUAL    = 1,     /**<  Manual network selection. */
}SV_NW_SELECTION_TYPE_E;

/**<  cell access state*/
typedef enum SV_NW_CELL_ACCESS_STATE_TYPE_ENUM
{
    SV_NW_CELL_ACCESS_NONE           = 0,             /**<  Unknown access. */
    SV_NW_CELL_ACCESS_NORMAL_ONLY    = 1,             /**<  Normal access only.*/
    SV_NW_CELL_ACCESS_EMERGENCY_ONLY = 2,             /**<  Emergency access only. */
    SV_NW_CELL_ACCESS_NO_CALLS       = 3,             /**<  No access */
    SV_NW_CELL_ACCESS_ALL_CALLS      = 4,             /**<  All access. */
}SV_NW_CELL_ACCESS_STATE_TYPE_E;

/** wea config items */
typedef enum
{
    SV_NW_WEA_CONFIG_PRESIDENTIAL_ALERT     = (1 << 0),   /**<  Presidential alert.*/
    SV_NW_WEA_CONFIG_EXTREME_ALERT          = (1 << 1),   /**<  Extreme alert.*/
    SV_NW_WEA_CONFIG_SEVERE_ALERT           = (1 << 2),   /**<  Severe alert.*/
    SV_NW_WEA_CONFIG_AMBER_ALERT            = (1 << 3),   /**<  Amber alert.*/
    SV_NW_WEA_CONFIG_PUBLIC_SAFETY_ALERT    = (1 << 4),   /**<  Public safety alert.*/
    SV_NW_WEA_CONFIG_STATE_LOCAL_TEST_ALERT = (1 << 5),   /**<  State local test alert.*/
    SV_NW_WEA_CONFIG_RMT_ALERT              = (1 << 6),   /**<  RMT alert.*/
    SV_NW_WEA_CONFIG_EXERCISE_ALERT         = (1 << 7),   /**<  Exercise alert.*/
    SV_NW_WEA_CONFIG_CMSP_DEFINED_ALERT     = (1 << 8),   /**<  CMSP defined alert.*/
    SV_NW_WEA_CONFIG_SPANISH_ALERT          = (1 << 9),   /**<  Spanish-language alert.*/
} SV_NW_WEA_CONFIG_E;

/** bind subscription */
typedef enum SV_NW_BIND_SUB_TYPE_ENUM
{
  SV_NW_BIND_SUB_PRIMARY = 0, 
  SV_NW_BIND_SUB_SECONDARY = 1, 
}SV_NW_BIND_SUB_TYPE_E;

typedef enum {
  SV_NW_RRC_STATE_UNKNOWN = 0,      /**<  RRC UNKNOWN. */
  SV_NW_RRC_STATE_IDLE = 1,         /**<  RRC IDLE. */
  SV_NW_RRC_STATE_CONNECTED = 2     /**<  RRC CONNECTED. */
}SV_NW_RRC_STATE_E;

typedef enum {
  SV_NW_DRX_DEFAULT = 0x00, /**<  DRX is not specified */
  SV_NW_DRX_CN6_T32 = 0x06, /**<   CN = 6, T = 32  */
  SV_NW_DRX_CN7_T64 = 0x07, /**<   CN = 7, T = 64  */
  SV_NW_DRX_CN8_T128 = 0x08, /**<  CN = 8, T = 128  */
  SV_NW_DRX_CN9_T256 = 0x09, /**<   CN = 9, T = 256   */
}SV_NW_DRX_E;

typedef struct 
{
    uint64_t   preferred_nw_mode;               /**<   Preferred network mode for connections; a bitmask of SV_NW_MODE_xxxx.*/
    SV_NW_ROAM_STATE_TYPE_E preferred_roaming;  /**<   depreferrted*/
}sv_nw_pref_nwmode_roaming_info_t;

/**<  network time information*/
typedef struct 
{
    char     nitz_time[SV_NW_NITZ_BUF_LEN + 1]; /**< UTC time in the format of YY/MM/DD,HH:MM:SS +/- TZ.
The “+” indicates eastern time zone and “-” indicates western time zone. The unit of TZ is 15 minutes.
For example, +32 indicates GMT+8 and -24 indicates GMT-6.*/
    uint64_t abs_time;                          /**< Absolute time, relative to 00:00:00 on January 1, 1970 (UTC). */
    int8_t   leap_sec;                          /**< Leap second (time error adjustment threshold). */
}sv_nw_nitz_time_info_t;

/**<  operator information*/
typedef struct 
{
    char long_eons[512 + 1];    /**<   Full name of the operator.*/
    char short_eons[512 + 1];   /**<   Short name of the operator.*/
    char mcc[3 + 1];            /**<   Mobile country code.*/
    char mnc[3 + 1];            /**<   Mobile network code.*/
}sv_nw_mobile_operator_name_info_t;

/**<  network scanning information*/
typedef struct 
{
    sv_nw_mobile_operator_name_info_t operator_name;   /**<   Operator.*/
    SV_NW_NETWORK_STATUS_TYPE_E       network_status;  /**<   Network status.*/
    SV_NW_RADIO_TECH_TYPE_E           rat;             /**<   Radio access technologies.*/
}sv_nw_scan_entry_info_t;

typedef struct 
{
    uint32_t                entry_len;                      /**< Must be set to the number of elements in entry. */
    sv_nw_scan_entry_info_t entry[SV_NW_SCAN_MAX_LIST_NUM]; /**<   Scan entry.*/
}sv_nw_scan_result_list_info_t;

/**< rat deny reason list*/
typedef struct {
  uint8_t valid;                            /**<   if the unit valid or not.*/
  SV_NW_RADIO_TECH_TYPE_E radio_tech;       /**<   Radio technology.*/
  SV_NW_DENY_REASON_TYPE_E deny_reason;     /**<   Deny Reason.*/
}sv_nw_radio_tech_deny_reason_t;

/**<  The information about the network registration status for voice and data-dialing*/
typedef struct
{
    SV_NW_TECH_DOMAIN_TYPE_E tech_domain;        /**<   Technology domain: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    SV_NW_RADIO_TECH_TYPE_E  radio_tech;         /**<   Radio technology; see #SV_NW_RADIO_TECH_TYPE_E.*/
    SV_NW_ROAM_STATE_TYPE_E  roaming;            /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/
    SV_NW_DENY_REASON_TYPE_E deny_reason;        /**<   Set when registration state is #SV_NW_DENY_REASON_TYPE_E, please refer to 3GPP 24301 SPEC ANNEX A chapter*/
    SV_NW_SERVICE_TYPE_E     reg_state;          /**<   Registration state.*/
    char                     mcc[3+1];           /**<   Mobile country code.*/
    char                     mnc[3+1];           /**<   Mobile network code.*/
    uint8_t                  forbidden;          /**<   Forbidden: 0 -- No, 1 -- Yes.*/
    uint32_t                 cid;                /**<   Cell ID for the registered 3GPP system.*/
    uint16_t                 lac;                /**<   Locatin area code for the registered 3GPP system.*/
    uint16_t                 psc;                /**<   Primary scrambling code (WCDMA only); 0 -- None.*/
    uint16_t                 tac;                /**<   Tracking area code information for LTE.*/
    uint8_t                  inPRL;              /**<   0 -- Not in PRL, 1 -- In PRL.*/
    uint8_t                  css;                /**<   Concurrent services supported: 0 -- No, 1 -- Yes.*/
    uint16_t                 sid;                /**<   CDMA system ID.*/
    uint16_t                 nid;                /**<   CDMA network ID.*/
    uint16_t                 bsid;               /**<   Base station ID. @newpagetable */

    uint16_t                 nr5g_svc_opt;       /**<   NR5G service option or type, defined by SV_NW_NR5G_SO_XXX */
    uint16_t                 nr5g_pci;           /**<   NR5G physical Cell ID */
    uint64_t                 nr5g_cid;           /**<   NR5G SA Cell ID */
    uint32_t                 nr5g_tac;           /**<   NR5G tac info */

    uint8_t                  endc_available;     /**<   EUTRA-NR Dual Connectivity */
    sv_nw_radio_tech_deny_reason_t deny_reason_list[SV_NW_RAT_DEYN_REASON_MAX_V01]; /**< Rat deny reason list */

}sv_nw_reg_status_info_t;

typedef struct 
{
    SV_NW_SELECTION_TYPE_E  nw_selection_type;    /**<   Network selection type. */
    char                    mcc[3 + 1];           /**<   Mobile country code for a manual network selection.*/
    char                    mnc[3 + 1];           /**<   Mobile network code for a manual network selection.*/
    SV_NW_RADIO_TECH_TYPE_E rat;                  /**<   Radio technology.*/
}sv_nw_pref_net_t;

/**<  GSM signal strength*/
typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates received signal strength. A signed value; -125 or lower indicates no signal.*/
}sv_nw_gsm_signal_info_t;

/**<  WCDMA signal strength*/
typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}sv_nw_wcdma_signal_info_t;

/**<  TDSCDMA signal strength*/
typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec.    a signed value; -125 or lower indicates no signal.*/
    int8_t      rscp;       /**<   RSCP in dBm.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
    int8_t      sinr;       /**<   Measured SINR in dB. @newpagetable */
}sv_nw_tdscdma_signal_info_t;

/**<  LTE signal strength*/
typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec. A signed value; -125 or lower indicates no signal.*/
    int8_t      rsrq;       /**<   RSRQ value in dB (signed integer value), as measured by L1. Range: -3 to -20 (-3 equals -3 dB, -20 equals -20 dB).*/
    int16_t     rsrp;       /**<   Current RSRP in dBm, as measured by L1. Range: -44 to -140 (-44 equals -44 dBm, -140 equals -140 dBm).*/
    int16_t     snr;        /**<   SNR level as a scaled integer in units of 0.1 dB; e.g., -16 dB has a value of -160 and 24.6 dB has a value of 246.*/
}sv_nw_lte_signal_info_t;

/**<  NR5G signal strength*/
typedef struct 
{
    int16_t     rsrq;       /**<   Current RSRQ in dBm, as measured by L1.*/
    int16_t     rsrp;       /**<   Current RSRP in dBm, as measured by L1. Range: -44 to -140 (-44 equals -44 dBm, -140 equals -140 dBm).*/
    int16_t     snr;        /**<   SNR level as a scaled integer in units of 0.1 dB; e.g., -16 dB has a value of -160 and 24.6 dB has a value of 246.*/
}sv_nw_nr5g_signal_info_t;

/**<  CDMA signal strength*/
typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}sv_nw_cdma_signal_info_t;

/**<  HDR signal strength*/
typedef struct 
{
    int8_t      rssi;       /**<   rssi in dbm. indicates forward link pilot power (agc) + ec/io. a signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   ec/io value representing negative 0.5 db increments, e.g., 2 equals -1 dbm.*/
    int8_t      sinr;       /**<   sinr level.*/
    int32_t     io;         /**<   received io in dbm. */
}sv_nw_hdr_signal_info_t;

/**<  signal strength*/
typedef struct 
{
    uint8_t                     has_gsm;     /**<   Must be set to TRUE if gsm_sig_info is being passed. */
    sv_nw_gsm_signal_info_t     gsm;         /**<   GSM signal information. */
    uint8_t                     has_wcdma;   /**<   Must be set to TRUE if wcdma_sig_info is being passed. */
    sv_nw_wcdma_signal_info_t   wcdma;       /**<   WCDMA signal information. */
    uint8_t                     has_tdscdma; /**<   Must be set to TRUE if tdscdma_sig_info is being passed. */
    sv_nw_tdscdma_signal_info_t tdscdma;     /**<   TDSCDMA signal information. */
    uint8_t                     has_lte;     /**<   Must be set to TRUE if lte_sig_info is being passed. */
    sv_nw_lte_signal_info_t     lte;         /**<   LTE signal information. */
    uint8_t                     has_nr5g;    /**<   Must be set to TRUE if nr5g_sig_info is being passed. */
    sv_nw_nr5g_signal_info_t    nr5g;        /**<   NR5G signal information. */
    uint8_t                     has_cdma;    /**<   Must be set to TRUE if cdma_sig_info is being passed. */
    sv_nw_cdma_signal_info_t    cdma;        /**<   CDMA signal information. */
    uint8_t                     has_hdr;     /**<   Must be set to TRUE if hdr_sig_info is being passed. */
    sv_nw_hdr_signal_info_t     hdr;         /**<   HDR signal information. */
}sv_nw_signal_strength_info_t;

/**<  GSM cell information*/
typedef struct
{
    uint32_t    cid;        /**<   Cell ID, (0 indicates information is not represent).*/
    char        plmn[3];    /**<   MCC/MNC inforamtion code.(This field should be ignored when cid is not present). */
    uint16_t    lac;        /**<   Location area code.(This field should be ignord when cid is not present). */
    uint16_t    arfcn;      /**<   Absolute RF channel number. */
    uint8_t     bsic;       /**<   Base station identity code. (0 indicates information is not present). */
    int16_t     rssi;       /**<   Received signal strength indicator in dBm.*/
}sv_nw_gsm_info_t;

/**<  umts cell information*/
typedef struct
{
    uint32_t cid;           /**<   Cell ID (0 indicates information is not present). */
    uint32_t lcid;          /**<   UTRAN Cell ID (0 indicates information is not present). */
    char plmn[3];           /**<   MCC/MNC information code.(This field should be ignored when cid is not present). */
    uint16_t lac;           /**<   Location area code. (This field should be ignored when cid is not present). */
    uint16_t uarfcn;        /**<   UTRA absolute RF channel number. */
    uint16_t psc;           /**<   Primary scrambling code. */
    int16_t  rssi;          /**<   Received signal strength indicator in dBm.*/
}sv_nw_umts_info_t;

/**<  LTE cell information*/
typedef struct
{
    uint32_t cid;           /**<   Global cell ID in the system information block (0 indicates information is not present). */
    char plmn[3];           /**<   MCC/MNC information code.(This filed should be ignored when cid is not present). */
    uint16_t tac;           /**<   Tracing area code (This field should be ignored when cid is not present). */
    uint16_t pci;           /**<   Physical cell ID. Range: 0 to 503. */
    uint32_t earfcn;        /**<   E-UTRA absolute radio frequency channel number of the cell. RANGE: 0 TO 65535. */
    int16_t  rssi;          /**<   Received signal strength indicator in dBm.*/
}sv_nw_lte_info_t;

/**<  NR5G cell information*/
typedef struct {

  uint64_t cid;             /**<   Global cell ID in the system information block (0 indicates information is not present). */
  char plmn[3];             /**<   MCC/MNC information coded(This field should be ignored when cid is not present.) */
  uint32_t tac;             /**<   Tracking area code (This field should be ignored when cid is not present). */
  uint16_t pci;             /**<   Physical cell ID. Range: 0 to 503.*/
  uint32_t arfcn;           /**<   UTRA absolute radio frequency channel number of the cell. Range: 0 to 65535.*/
  int16_t rsrq;             /**<   Current RSRQ in units of 0.1 dB, as measured by L1.*/
  int16_t rsrp;             /**<   Current RSRP in units of 0.1 dB, as measured by L1.*/
  int16_t snr;              /**<   Current SNR  in dBm, as measured by L1.*/
}sv_nw_nr5g_info_t;

/**<  CDMA cell information*/
typedef struct 
{
   uint16_t sid;            /**<   System ID. */
   uint16_t nid;            /**<   Network ID. */
   uint16_t bid;            /**<   Base station ID. */
   uint16_t refpn;          /**<   Reference PN. */
   uint32_t base_lat;       /**<   Latitude of the current base station in units of 0.25 sec. */
   uint32_t base_long;      /**<   Longitude of the current base station in units of 0.25 sec. */
   int16_t  rssi;           /**<   Received signal strength indicator in dBm. */
}sv_nw_cdma_info_t;

/**<  cell information*/
typedef struct 
{
    SV_NW_RADIO_TECH_TYPE_E serving_rat;
    SV_NW_RRC_STATE_E       rrc_state;
    uint8_t                 gsm_info_valid;                         /**<  Must be set to true if gsm_info is being passed */
    uint8_t                 gsm_info_len;                           /**<  Must be set to the number of elements in entry*/
    sv_nw_gsm_info_t        gsm_info[SV_NW_MAX_GSM_CELL_INFO_NUM];  /**<  GSM cell information (Serving and neighbor. */  
    uint8_t                 umts_info_valid;                        /**<  Must be set to true if umts_info is being passed */
    uint8_t                 umts_info_len;                          /**<  Must be set to the number of elements in entry*/
    sv_nw_umts_info_t       umts_info[SV_NW_MAX_UMTS_CELL_INFO_NUM];/**<  UMTS cell information (Serving and neighbor). */
    uint8_t                 lte_info_valid;                         /**<  Must be set to true if lte_info is being passed */
    uint8_t                 lte_info_len;                           /**<  Must be set to the number of elements in entry*/
    sv_nw_lte_info_t        lte_info[SV_NW_MAX_LTE_CELL_INFO_NUM];  /**<  LTE cell information (Serving and neighbor). */
    uint8_t                 nr5g_info_valid;                        /**<  Must be set to true if nr5g_info is being passed */
    sv_nw_nr5g_info_t       nr5g_info;                              /**<  NR5G cell information (Serving). */
    uint8_t                 cdma_info_valid;                        /**<  Must be set to true if cdma_info is being passed */
    sv_nw_cdma_info_t       cdma_info;                              /**<  CDMA cell information (Serving). */  
}sv_nw_cell_info_t;

/**<  WEA configuration items*/
typedef struct 
{
    uint8_t Presidential_alert;         /**<  Enable Presidential_alert: 0 -- Disable(can't Disable); 1 -- Enable */
    uint8_t Extreme_alert;              /**<  Enable or disable Extreme_alert: 0 -- Disable; 1 -- Enable */
    uint8_t Severe_alert;               /**<  Enable or disable Severe_alert: 0 -- Disable; 1 -- Enable */
    uint8_t Amber_alert;                /**<  Enable or disable Amber_alert: 0 -- Disable; 1 -- Enable */
    uint8_t PublicSafety_alert;         /**<  Enable or disable PublicSafety_alert: 0 -- Disable; 1 -- Enable */
    uint8_t StateLocalTest_alert;       /**<  Enable or disable StateLocalTest_alert: 0 -- Disable; 1 -- Enable */
    uint8_t RMT_alert;                  /**<  Enable or disable RMT_alert: 0 -- Disable; 1 -- Enable */
    uint8_t Exercise_alert;             /**<  Enable or disable Exercise_alert: 0 -- Disable; 1 -- Enable */
    uint8_t CMSPDefined_alert;          /**<  Enable or disable CMSPDefined_alert: 0 -- Disable; 1 -- Enable */
    uint8_t Spanish_alert;              /**<  Enable or disable Spanish_alert: 0 -- Disable; 1 -- Enable */
}sv_nw_wea_config_t;

/**<  WEA alert information*/
typedef struct 
{
    uint8_t wea_alert_info_valid; 
    char wea_alert_info[SV_NW_MAX_WEA_ALERT_LENGTH_V01];
}sv_nw_wea_alert_info_t;

/**<  ETWS alert information*/
typedef struct 
{
    uint8_t etws_primary_info_valid;
    char etws_primary_info[SV_NW_MAX_ETWS_ALERT_LENGTH_V01];
    uint8_t etws_secondary_info_valid;
    char etws_secondary_info[SV_NW_MAX_ETWS_ALERT_LENGTH_V01];
}sv_nw_etws_alert_info_t;

/** ims config items */
typedef enum
{
    SV_NW_IMS_VOLTE_CONFIG      = (1 << 0),   /**<  config volte enable.*/
    SV_NW_IMS_ENABLE_CONFIG     = (1 << 1),   /**<  config ims enable.*/
    SV_NW_IMS_SMS_CONFIG        = (1 << 2),   /**<  config sms enable.*/
} SV_NW_IMS_CONFIG_E;

typedef struct
{
    /* Optional */
    uint8_t volte_enabled_valid;
    uint8_t volte_enabled;
    
    /* Optional */
    uint8_t ims_enable_valid;
    uint8_t ims_enable;
    
    /* Optional */
    uint8_t sms_enabled_valid;
    uint8_t sms_enabled;
}sv_nw_ims_config_ex_t;

typedef struct
{
  /* Optional */
  uint8_t enable_endc_valid;
  uint8_t enable_endc;
}sv_nw_get_endc_config_t;

typedef struct
{
  uint8_t enable_endc;
}sv_nw_set_endc_config_t;

typedef struct 
{
  uint8_t drx_valid;
  SV_NW_DRX_E drx;
}sv_nw_get_drx_t;

typedef struct 
{
  uint8_t drx_level_valid;  /**< Must be set to true if drx_level is being passed */
  uint8_t drx_level;
}sv_nw_get_negotiated_drx_t;

typedef struct 
{
  SV_NW_DRX_E drx;
}sv_nw_set_drx_t;

typedef struct 
{
    uint16_t pid;
    uint16_t band;
    uint16_t dl_earfcn;
    uint16_t ul_earfcn;
}sv_nw_lte_rach_fail_info_t;

#ifdef __cplusplus
}
#endif

#endif
