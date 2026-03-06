/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */
/*
 * File name:  RfxStatusDef.h
 * Author: Jun Liu (MTK80064)
 * Description:
 *  Define the keys of shared status.
 */

#ifndef __RFX_STATUS_DEFS_H__
#define __RFX_STATUS_DEFS_H__

    #define RFX_WAIT_FOR_ECPIN 1
    #define RFX_ECPIN_DONE 0


    #define RFX_UICC_APPLIST_UNKNOWN  -1
    #define RFX_UICC_APPLIST_NONE     0x00
    #define RFX_UICC_APPLIST_USIM     0x02
    #define RFX_UICC_APPLIST_CSIM     0x04

    /* CDMA card type */
    #define UIM_CARD  1
    #define SIM_CARD  2
    #define UIM_SIM_CARD  3
    #define UNKOWN_CARD  4
    #define CT_3G_UIM_CARD  5
    #define CT_UIM_SIM_CARD  6
    #define NEED_TO_INPUT_PIN  7
    #define CT_4G_UICC_CARD  8
    #define NOT_CT_UICC_CARD  9
    #define CT_EXCEL_GG_CARD  10
    #define LOCKED_CARD  18
    #define IVSR_LOST  19
    #define CARD_NOT_INSERTED  255

    #define RFX_CDMA_CARD_READY_DEFAULT  0x00
    #define RFX_CDMA_CARD_EUSIM_READY    0x01
    #define RFX_CDMA_CARD_ECT3G_READY    0x02
    #define RFX_CDMA_CARD_MCCMNC_READY   0x04
    #define RFX_CDMA_CARD_LOCKCARD_READY 0x08
/*****************************************************************************
 * Enum
 *****************************************************************************/

typedef unsigned int RfxStatusKeyEnum;

enum {
    RFX_STATUS_KEY_START,
    /*Please add your keys below this line*/

    /**
     * The card type of SIM card.
     * value type : int
     * RFX_CARD_TYPE_SIM  0x01
     * RFX_CARD_TYPE_USIM 0x02
     * RFX_CARD_TYPE_CSIM 0x04
     * RFX_CARD_TYPE_RUIM 0x08
     * RFX_CARD_TYPE_ISIM 0x10
     */
    RFX_STATUS_KEY_CARD_TYPE,

    /**
     * The card type of CDMA card.
     * value type : int
     * #define UIM_CARD  1  // CDMA only card but not CT card
     * #define SIM_CARD  2  // GSM card
     * #define UIM_SIM_CARD  3  // CDMA dual mode card but not CT card
     * #define UNKOWN_CARD  4  //unkonw card
     * #define CT_3G_UIM_CARD  5  // CT sigle mode card
     * #define CT_UIM_SIM_CARD  6  // CT dual mode card
     * #define NEED_TO_INPUT_PIN  7  // pin locked card
     * #define CT_4G_UICC_CARD  8  // CT 4G dual mode card
     * #define NOT_CT_UICC_CARD  9  // 4G dual mode card but not CT card
     * #define CT_EXCEL_GG_CARD  10 // CT excel GG card
     * #define LOCKED_CARD  18  // card is locked by modem
     * #define IVSR_LOST  19  // invalid sim recovery
     * #define CARD_NOT_INSERTED  255  // no card inserted
     */
    RFX_STATUS_KEY_CDMA_CARD_TYPE,

    /**
     * Use below flag to generate CDMA card type.
     * value type : int
     * RFX_CDMA_CARD_READY_DEFAULT  0x00
     * RFX_CDMA_CARD_EUSIM_READY  0x01
     * RFX_CDMA_CARD_ECT3G_READY 0x02
     * RFX_CDMA_CARD_MCCMNC_READY 0x04
     * RFX_CDMA_CARD_LOCKCARD_READY 0x08
     */
    RFX_STATUS_KEY_CDMA_CARD_READY,

    /**
     * Ready to read CDMA card file after C2K modem sends ciev 107.
     * value type : bool
     * false : CDMA card file is not ready to read. It is default value.
     * true : CDMA card file is ready to read.
     */
    RFX_STATUS_KEY_CDMA_FILE_READY,

    /**
     * CDMA 3g dualmode card flag.
     * value type : bool
     * false : it is not a CDMA 3g dualmode card. It is default value.
     * true : it is a CDMA 3g dualmode card.
     */
    RFX_STATUS_KEY_CDMA3G_DUALMODE_CARD,

    /**
     * Report uicc application list in OP09 A project for slot2.
     * value type : int
     * uicc_app_list = is_csim_exist | is_usim_exist | is_isim_exist (currently isim always 0)
     * is_usim_exist:2 is_csim_exist:4 (is_csim_exist | is_usim_exist): 6
     * For icc card uicc_app_list:0
     */
    RFX_STATUS_KEY_ESIMIND_APPLIST,

    /**
     * It shows if the card is locked in OP09 A project.
     * value type : bool
     * false : Card is not locked.
     * true : Card is locked.
     */
    RFX_STATUS_KEY_CDMA_LOCKED_CARD,

    /**
     * The uicc subscription changed status
     * value type : int
     * Init: -1
     * Deactivate: 0
     * Activate: 1
     */
    RFX_STATUS_KEY_UICC_SUB_CHANGED_STATUS,

    /**
     * The IMSI of CDMA application.
     * value type : String8
     * "" or IMSI
     */
    RFX_STATUS_KEY_C2K_IMSI,

    /**
      * Notify GSM MCC+MNC
      */
    RFX_STATUS_KEY_UICC_GSM_NUMERIC,

    /**
      * Notify CDMA MCC+MNC
      */
    RFX_STATUS_KEY_UICC_CDMA_NUMERIC,

    /**
     * Euicc card flag.
     * value type : bool
     * false : it is not a euicc card.
     * true : it is a euicc card.
     */
    RFX_STATUS_KEY_IS_EUICC_CARD,

    /**
     * Support command "AT+ECGLA" or not
     * value type : bool
     * false : not support
     * true : support
     */
    RFX_STATUS_KEY_ECGLA_CAPABILITY,

    /**
     * The EID of the SIM.
     * value type : String8
     * "": Default value; "N/A":No EUICC; Valid EID
     */
    RFX_STATUS_KEY_SIM_EID,

    /**
     * The current BTSAP status
     * Refer to BtSapStatus
     */
    RFX_STATUS_KEY_BTSAP_STATUS,

    /**
     * The current protocol
     */
    RFX_STATUS_KEY_BTSAP_CURRENT_PROTOCOL,

    /**
     * The support protocol
     */
    RFX_STATUS_KEY_BTSAP_SUPPORT_PROTOCOL,

    /**
     * Save ATR for BTSAP
     */
    RFX_STATUS_KEY_BTSAP_ATR,

    /**
     * The SIM ESIMS state. It will be updated upon receiving +ESIMS.
     * value type : int
     * 0: SIM Missing
     * 9: Virtual SIM on
     * 10: Virtual SIM off
     * 11: SIM plug out
     * 12: SIM plug in
     * 13: Recovery start
     * 14: Recovery end
     * 15: IMEI Lock
     */
    RFX_STATUS_KEY_SIM_ESIMS_CAUSE,

    /**
     * The flag is used for the indication of ECPIN
     */
    RFX_STATUS_KEY_ECPIN_STATE,

    /**
     * This is used to check the modem SIM task is ready or not.
     * NOTE: It is not SIM_STATE_CHANGED READY!!
     * value type : bool
     * false: modem SIM task is not ready. It is also default value
     * true: modem SIM task is ready.
     */
    RFX_STATUS_KEY_MODEM_SIM_TASK_READY,

    /**
     * SIM on/off state.
     * value type : int
     * -1: unknown
     * 10: SIM_POWER_STATE_SIM_OFF
     * 11: SIM_POWER_STATE_SIM_ON
     */
    RFX_STATUS_KEY_SIM_ONOFF_STATE,

    /**
     * SIM on/off switching state.
     * value type : int
     * -1: unknown
     * 12: SIM_POWER_STATE_SIM_SWITCHING_OFF
     * 13: SIM_POWER_STATE_SIM_SWITCHING_ON
     */
    RFX_STATUS_KEY_SIM_ONOFF_SWITCHING_STATE,

    /**
     * SIM on/off feature enabled or disabled state.
     * value type : bool
     * false: Disabled
     * true: Enabled
     */
    RFX_STATUS_KEY_SIM_ONOFF_ENABLED,

    /**
     * Record if modem supports SIM on/off feature.
     * value type : bool
     * false: Not support
     * true: Support
     */
    RFX_STATUS_KEY_MODEM_SIMONOFF_SUPPORT,

    /**
     * UICC applications enable response state of requests.
     * value type : enum UiccApplicationsEnableResponseState
     * UICC_APPLICATIONS_ENABLE_RESPONSE_STATE_IDLE 0
     * UICC_APPLICATIONS_ENABLE_RESPONSE_STATE_WAITING_ENABLE 1
     * UICC_APPLICATIONS_ENABLE_RESPONSE_STATE_WAITING_DISABLE 2
     */
    RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_RESPONSE_STATE,

    /**
     * UICC applications enable state. By slot.
     * value type : enum UiccApplicationsEnableState
     * UICC_APPLICATIONS_ENABLE_STATE_UNKNOWN -1
     * UICC_APPLICATIONS_ENABLE_STATE_ENABLED 0
     * UICC_APPLICATIONS_ENABLE_STATE_ENABLING 1
     * UICC_APPLICATIONS_ENABLE_STATE_DISABLED 2
     * UICC_APPLICATIONS_ENABLE_STATE_DISABLING 3
     */
    RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE,

    /**
     * sim power state. By slot.
     * value type : enum simPowerState
     * SIM_POWER_STATE_UNKNOWN     -1
     * SIM_POWER_STATE_V11_SIM_OFF  0
     * SIM_POWER_STATE_V11_SIM_ON   1
     * SIM_POWER_STATE_PASS_THROUGH 2
     * SIM_POWER_STATE_V16_SIM_OFF  3
     * SIM_POWER_STATE_V16_SIM_ON   4
     */
    RFX_STATUS_KEY_SIM_POWER_STATE,

    /**
     * SIM switch mode, non-slot key.
     * value type : enum SimSwitchMode
     * SIM_SWITCH_MODE_UNKNOWN -1
     * SIM_SWITCH_MODE_AOSP_POWER_ON_OFF 0 - Euqate to 3 now
     * SIM_SWITCH_MODE_MTK_RADIO_ON_OFF 1
     * SIM_SWITCH_MODE_MTK_SIM_ON_OFF 2
     * SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE 3
     * SIM_SWITCH_MODE_NONE 4
     */
    RFX_STATUS_KEY_AP_SIM_SWITCH_MODE,

    /**
     * SIM switch mode, non-slot key.
     * value type : enum SimSwitchMode
     * SIM_SWITCH_MODE_UNKNOWN -1
     * SIM_SWITCH_MODE_AOSP_POWER_ON_OFF 0 - Euqate to 3 now
     * SIM_SWITCH_MODE_MTK_RADIO_ON_OFF 1
     * SIM_SWITCH_MODE_MTK_SIM_ON_OFF 2
     * SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE 3
     * SIM_SWITCH_MODE_NONE 4
     */
    RFX_STATUS_KEY_MD_SIM_SWITCH_MODE,

    RFX_STATUS_KEY_SERVICE_STATE,
   /**
    * Include string as following
    * "GSM,<rssi_dbm>,<ber>"
    * "UMTS,<rscp_dbm>,<ecn0>"
    * "LTE,<rsrp_dbm>,<rsrq_db>"
    * "CDMA,<cdma_dbm>,<cdma_ecio>"
    * "EVDO,<evdo_dbm>,<evdo_ecio>"
    */
    RFX_STATUS_KEY_SIGNAL_STRENGTH,
    RFX_STATUS_KEY_VOICE_TYPE,
    RFX_STATUS_KEY_DATA_TYPE,
    RFX_STATUS_KEY_RADIO_STATE,
    RFX_STATUS_KEY_REQUEST_RADIO_POWER,
    RFX_STATUS_KEY_REQUEST_MODEM_POWER,
    RFX_STATUS_KEY_MODEM_POWER_OFF,
  /**
   * Modem off state.
   * NOTE: Belong to non slot controller, use getNonSlotScopeStatusManager().
   * value type : int
   * MODEM_OFF_IN_IDLE
   * MODEM_OFF_BY_MODE_SWITCH
   * MODEM_OFF_BY_POWER_OFF
   * MODEM_OFF_BY_SIM_SWITCH
   */
    RFX_STATUS_KEY_MODEM_OFF_STATE,

    /**
     * The SIM state. It will be updated upon receiving the response of GET_SIM_STATUS.
     * And be set as NOT_READY when RADIO_UNAVAILABLE.
     * value type : int
     * RFX_SIM_STATE_UNKNOWN 0
     * RFX_SIM_STATE_ABSENT 1
     * RFX_SIM_STATE_PIN_REQUIRED 2
     * RFX_SIM_STATE_PUK_REQUIRED 3
     * RFX_SIM_STATE_NETWORK_LOCKED 4
     * RFX_SIM_STATE_READY 5
     * RFX_SIM_STATE_NOT_READY 6
     * RFX_SIM_STATE_PERM_DISABLED 7
     * RFX_SIM_STATE_CARD_IO_ERROR 8
     * RFX_SIM_STATE_CARD_RESTRICTED 9
     */
    RFX_STATUS_KEY_SIM_STATE,
    /**
     * The SIM inserted state. It will be updated upon receiving the response of GET_SIM_STATUS.
     * And be set as NOT_READY when RADIO_UNAVAILABLE.
     * value type : int
     */
    RFX_STATUS_KEY_SIM_INSERT_STATE,

    /**
     * The IMSI of GSM application.
     * value type : String8
     * "" or IMSI
     */
    RFX_STATUS_KEY_GSM_IMSI,

    /**
     * Indicate the voice call count
     */
    RFX_STATUS_KEY_VOICE_CALL_COUNT,

    /**
     * Indicate the current voice call index for ecall
     */
    RFX_STATUS_KEY_VOICE_CALL_INDEX,

    /**
      * Indicate the AP voice call count
      */
    RFX_STATUS_KEY_AP_VOICE_CALL_COUNT,

    /**
      * Indicate if ECall exists or not
      */
    RFX_STATUS_KEY_ECALL_EXIST,

    /**
      * Last call fail cause
      */
    RFX_STATUS_KEY_LAST_CALL_FAIL_CAUSE,

    /**
     * The raido acess family for each slot
     * value type : int
     * Return the networktype like RAF_LTE+RAF_UMTS+RAF_GSM
     */
    RFX_STATUS_KEY_SLOT_CAPABILITY,

    /**
     * Slot data connection status is changed.
     * status will be notified only if status is changed.
     * value type : int
     * DISCONNECTED: no active data connection exist.
     * CONNECTED: at least one data connection exist.
     */
    RFX_STATUS_KEY_DATA_CONNECTION,
    /**
     * The world mode switching state
     * value type : int
     * Switching:   0
     * Switch done: 1
     * Switch done but modem failure: -1
     */
    RFX_STATUS_KEY_WORLD_MODE_STATE,

    /**
     * The GSM world mode switching state
     * value type : int
     * Switching:   0
     * Switch done: 1
     */
    RFX_STATUS_KEY_GSM_WORLD_MODE_STATE,

    /**
     * The C2k world mode switching state
     * value type : int
     * Switching:   0
     * Switch done: 1
     */
    RFX_STATUS_KEY_CDMA_WORLD_MODE_STATE,

    /**
     * The world mode block state
     * value type : int
     * blocked:   1
     * not blocked: 0
     */
    RFX_STATUS_KEY_WORLD_MODE_BLOCKED_STATE,

    /**
     * The world mode block switching state
     * value type : int
     * block Switching:   1
     * not block switching: 1
     */
    RFX_STATUS_KEY_WORLD_MODE_BLOCKED_CHANGING_STATE,

    /**
     * The main capability slot id
     * value type : int
     * Return the main capability slot Id
     */
    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT,

    /**
     * The CDMA OTA provsison state
     * value type : int
     * programming started                       1
     * service programming lock unlocked         2
     * NAM parameters downloaded successfully    3
     * MDN downloaded successfully               4
     * IMSI downloaded successfully              5
     * PRL downloaded successfully               6
     * commit successfully                       7
     * programming successfully                  8
     * programming unsuccessfully                9
     * verify SPC failed                         10
     * a key exchanged                           11
     * SSD updated                               12
     * OTAPA started                             13
     * OTAPA stopped                             14
     */
    RFX_STATUS_KEY_OTA_STATUS,

    /**
      * Notify the current call state
      */
    RFX_STATUS_KEY_CALL_STATE,


    /**
     * Indicate whether the UE is in emergency mdoe
     * value type: boolean
     *  true:  In emergency mode
     *  false: Not in emergency mode
     */
    RFX_STATUS_KEY_EMERGENCY_MODE,

    /**
     * Emergency callback mode
     * value type: integer
     *  0: Not in emergency callback mode
     *  1: in emergency callback mode
     */
    RFX_STATUS_KEY_EMERGENCY_CALLBACK_MODE,

    /**
     * For Native Carrier Config. Indicate EAIC should be auto approved by RIL or not.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_AUTO_APPROVE_INCOMING,

    /**
     * Indicate whether the UE is in emergency mdoe
     * value type: boolean
     *  true:  In emergency mode
     *  false: Not in emergency mode
     */
    RFX_STATUS_KEY_EMERGENCY_MODE_IN_FLIGHT_MODE,

    RFX_STATUS_KEY_ATCI_IS_NUMERIC,
    /**
     * Indicate the cellular network PS state
     */
    RFX_STATUS_KEY_CELLULAR_PS_STATE,
    /**
     * Indicate the WFC state
     */
    RFX_STATUS_KEY_WFC_STATE,
    RFX_STATUS_KEY_WFC_LOCK_STATE,

    /**
     * IMS call status
     * value type: boolean
     *  true: IMS call ongoing
     *  false: No IMS call
     */
    RFX_STATUS_KEY_IMS_CALL_EXIST,

    /*
     * CDMA_SMS_INBOUND_NONE(0)
     * CDMA_SMS_INBOUND_IMS(1)
     * CDMA_SMS_INBOUND_CS(2)
     * CDMA_SMS_INBOUND_COMM(3)
     * CDMA_SMS_INBOUND_VMI(4)
     */
    RFX_STATUS_KEY_CDMA_INBOUND_SMS_TYPE,
    RFX_STATUS_KEY_CDMA_INBOUND_SMS_TXID,
    RFX_STATUS_KEY_CDMA_INBOUND_SMS_PRE_TXID,
    RFX_STATUS_KEY_CDMA_SMS_REPLY_SEQ_NO,
    RFX_STATUS_KEY_CDMA_SMS_ADDR,
    RFX_STATUS_KEY_CDMA_PENDING_VMI,

    /*
     * CDMA_MO_SMS_SENDING(0)
     * CDMA_MO_SMS_SENDED(1)
     */
    RFX_STATUS_KEY_CDMA_MO_SMS_STATE,

    /**
    * Store default data SIM.
    * -1: Unset
    * 0:  Slot 0
    * 1:  Slot 1
    */
    RFX_STATUS_KEY_DEFAULT_DATA_SIM,

    /**
     * ECC preferred RAT
     * value type: integer
     *  0: unknown
     *  1: gsm
     *  2: cdma
     */
    RFX_STATUS_KEY_ECC_PREFERRED_RAT,

    /**
     * is GWSD action(answer/hangup) ongoing
     * value type: bool
     */
    RFX_STATUS_KEY_IS_GWSD_ACTION_ONGOING,

    /**
     * CC fake mode for GWSD
     * value type: int
     */
    RFX_STATUS_KEY_CC_FAKE_MODE,

    /**
     * IMS CC fake mode for GWSD
     * value type: bool
     */
    RFX_STATUS_KEY_IMS_CC_FAKE_MODE,

    /**
     * Capability switch internal use
     * Indicate the fixed capability for a specific slot
     * value type: int
     */
    RFX_STATUS_KEY_SLOT_FIXED_CAPABILITY,

    /**
     * Store radio power of each protocol
     */
    RFX_STATUS_KEY_RADIO_POWER_MSIM_MODE,

    /**
     * Mutex lock for radio power and world mode.
     * 0: idle
     * 1: lock by radio power
     * 2: lock by world mode
     */
    RFX_STATUS_KEY_RADIO_LOCK,

    RFX_STATUS_KEY_GCF_TEST_MODE,

    /*
      * SMS_INBOUND_NONE (0)
      * SMS_INBOUND_IMS_3GPP (1)
      * SMS_INBOUND_CS_3GPP (2)
      */
    RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,

    /**
      * SMS_PHONE_STORAGE_AVAILABLE (0)
      * SMS_PHONE_STORAGE_FULL (1)
      */
    RFX_STATUS_KEY_SMS_PHONE_STORAGE,

    RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID,

    /**
     * SMS framework ready or not.
     * value type: bool
     * false: not ready
     * true: ready
     */
    RFX_STATUS_KEY_SMS_FW_READY,

    /**
     * rat controller preferred network type.
     * value type : int
     */
    RFX_STATUS_KEY_PREFERRED_NW_TYPE,

    /**
     * nr switch controller's chosen nr slot.
     * value type: int
     * 0: no chose
     * 1: slot 0
     * 2: slot 1
     */
    RFX_STATUS_KEY_NR_SWITCH_SLOT,

    /**
     * nr switch is ongoing or not.
     * value type: int
     * 0: switching
     * 1: not switching
     * Owner: NR switch
     */
    RFX_STATUS_KEY_IS_NR_SWITCHING,

    RFX_STATUS_KEY_IS_RAT_MODE_SWITCHING,

    /**
     * rat controller nws mode.
     * NWS_MODE_UNKNOWN    = -1, // for CSFB (GSM) network selecting mode
     * NWS_MODE_CDMALTE    = 0,  // for CDMALTE (CDMA) network selecting mode
     * NWS_MODE_CSFB       = 1,  // for CSFB (GSM) network selecting mode
     */
    RFX_STATUS_KEY_NWS_MODE,

    /**
     * 5 or 6 digit operator numeric code (MCC + MNC)
     * only for normal service
     * value type : String8
     * "" or MCC+MNC
     */
    RFX_STATUS_KEY_OPERATOR,

    /**
     * 5 or 6 digit operator numeric code (MCC + MNC)
     * for normal servic and in service
     * value type : String8
     * "" or MCC+MNC
     */
    RFX_STATUS_KEY_OPERATOR_INCLUDE_LIMITED,

    RFX_STATUS_KEY_MODESWITCH_FINISHED,

    /**
     * SIM can set the key to switch cdma 3G card. It shows who will trigger to switch: AP or GMSS
     * and switching to which card type: SIM or RUIM.
     * value type: int
     *  -1: default
     *  1: AP_TRIGGER_SWITCH_SIM
     *  2: GMSS_TRIGGER_SWITCH_SIM
     *  3: AP_TRIGGER_SWITCH_RUIM
     *  4: GMSS_TRIGGER_SWITCH_RUIM
     */
    RFX_STATUS_KEY_CDMA3G_SWITCH_CARD,

    /**
     * Indicate the capability switch states
     * value type: CapabilitySwitchState enum
     */
    RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE,

    /**
     * Modules can set this status key to do some handling before SIM switch,
     * please don't do long time consuming operation for it will cause performance and timeout
     * issues for SIM switch.
     * value type: int
     */
    RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE,

    /**
     * SIM switch will set current time stamp to this status key when set radio unavailable,
     * modules can use it to know when the latest radio unavailable was set by SIM switch.
     * value type: int64_t
     */
    RFX_STATUS_KEY_SIM_SWITCH_RADIO_UNAVAIL_TIME,

    /**
     * Indicate the STK service state
     */
    RFX_STATUS_KEY_IS_CAT_RUNNING,

    /**
    * Indicate UICC reset application
    */
    RFX_STATUS_KEY_UICC_RESET_APPLICATION,

    /**
     * Cache current stk command type
     */
    RFX_STATUS_KEY_STK_CACHE_CMD_TYPE,

    /**
     * Current STK command id
     */
    RFX_STATUS_KEY_STK_CMD_ID,

    /**
     * Capability switch internal use for sync status
     * value type: bool
     */
    RFX_STATUS_KEY_CAPABILITY_SWITCH_URC_CHANNEL,

    /**
     * Store PCO status for radio manager.
     * value type: String
     */
    RFX_STATUS_KEY_PCO_STATUS,

    /*
    * indicate AP has power off modem and power on yet
    */
    RFX_STATUS_KEY_HAD_POWER_OFF_MD,

    /**
     * TRN call ID
     * value type: integer
     */
    RFX_STATUS_KEY_TRN_CALLID,

    /**
     * Store TRN for Digits Service
     * value type: String
     */
    RFX_STATUS_KEY_TRN,

    /*
    * Indicate DSBP enhancement status
    * Value type: int
    */
    RFX_STATUS_KEY_DSBP_ENHANCEMENT_STATE,

    /**
     * Used for capability switch to notify silent boot if major sim is unsync when booting up
     * value type: bool
     */
    RFX_STATUS_KEY_CAPABILITY_SWITCH_KEEP_EBOOT,

    /**
     * Store CDMA Service Provider Name
     * value type: String
     */
    RFX_STATUS_KEY_CDMA_SPN,

    /**
     * Store Service Provider Name
     * value type: String
     */
    RFX_STATUS_KEY_GSM_SPN,

    /**
     * Store Group Identifier Level 1
     * value type: String
     */
    RFX_STATUS_KEY_GSM_GID1,

    /**
     * Store the first record of PLMN Network Name
     * value type: String
     */
    RFX_STATUS_KEY_GSM_PNN,

    /**
     * Store the first record of IP multimedia private identity
     * value type: String
     */
    RFX_STATUS_KEY_GSM_IMPI,

    /**
     * Store FCP context for cached files.
     * value type: String
     */
    RFX_STATUS_KEY_GSM_CACHE_FCP,

    /**
     * Store binary context for cached files.
     * value type: String
     */
    RFX_STATUS_KEY_GSM_CACHE_BINARY,

    /**
     * This is used to identify if modem supports +ICCID.
     * NOTE: For backward compatible.
     * value type : bool
     * false: modem does not support +ICCID
     * true: modem supports +ICCID.
     */
    RFX_STATUS_KEY_MD_ICCID_SUPPORT,

    /**
     * The ICCID for the SIM.
     * value type : String8
     * "" or "N/A" or valid iccid
     */
    RFX_STATUS_KEY_SIM_ICCID,

    /**
    * indicate socket state betwwen RILD and RILJ
    */
    RFX_STATUS_CONNECTION_STATE,

    /**
     * For Native Carrier Config. It is used to notify modules that
     * carrier config data is loaded.
     * value type : String8
     */
    RFX_STATUS_KEY_CARRIER_CONFIG_CHANGED,

    /**
     * Store ImsCallInfos
     * value type: vector<RfxImsCallInfo>
     */
    RFX_STATUS_KEY_IMS_CALL_LIST,

    /**
     * indicate restore conference participant or not
     * value type: String8
     */
    RFX_STATUS_KEY_RESTORE_IMS_CONFERENCE_PARTICIPANT,

    /**
     * Operate IMS conference participants by user untity from CEP or not.
     * value type: String8
     */
    RFX_STATUS_KEY_OPERATE_IMS_CONFERENCE_PARTICIPANTS_BY_USER_ENTITY,

    /**
     * Operator specific ims conferenc mode.
     * To indicate the what kind of handling should be taken during conferenc process.
     * Mode 0 : normal mode (follow the IR.94)
     * Mode 1 : KDDI mode (participant will still keep after merge on SIP layer and AP will view participant as terminated.
     * value type: String8
     */
    RFX_STATUS_KEY_IMS_SPECIFIC_CONFERENCE_MODE,

    /**
     * Present if current RF is switch to this Slot by Ims call
     * value type: String8
     */
    RFX_STATUS_KEY_IMS_CALL_OCUPPY_RF,

    /**
     * XUI info for IMS conference used. Available when IMS registered.
     * value type: vector<String>
     * indicate restore conference participant or not
     * value type: vector<string>
     */
    RFX_STATUS_KEY_XUI_INFO,

    /**
     * Terminal-based CLIR info for CLIR_N (parameter sets the adjustment for outgoing calls)
     * value type: integer
     *     0 : Netowrk Ddefault (presentation indicator is used according to the
     *                           subscription of the CLIR service)
     *     1 : Hide Number (CLIR invocation)
     *     2 : Show Number (CLIR suppression)
     */
    RFX_STATUS_KEY_TBCLIR_N,

    /**
     * For Native Carrier Config. indicate this sim operator support *82
     * is higher priority than CLIR invocation or not.
     * value type : String
     */
    RFX_STATUS_KEY_HIGH_PRIORITY_CLIR_PREFIX_SUPPORTED,

    /**
    * For Native Carrier Config. Indicate VOLTE is available or not for current operator.
    * value type: String
    */
    RFX_STATUS_KEY_CONFIG_DEVICE_VOLTE_AVAILABLE,

    /**
     * For Native Carrier Config. Indicate VT is available or not for current operator.
     * value type: String
     */
    RFX_STATUS_KEY_CONFIG_DEVICE_VT_AVAILABLE,

    /**
     * For Native Carrier Config. Indicate WFC is available or not for current operator.
     * value type: String
     */
    RFX_STATUS_KEY_CONFIG_DEVICE_WFC_AVAILABLE,

    /**
    * For Native Carrier Config. Indicate VONR is available or not for current operator.
    * value type: String
    */
    RFX_STATUS_KEY_CONFIG_DEVICE_VONR_AVAILABLE,

    /**
    * For Native Carrier Config. Indicate VINR is available or not for current operator.
    * value type: String
    */
    RFX_STATUS_KEY_CONFIG_DEVICE_VINR_AVAILABLE,

    /**
    * For Native Carrier Config. Indicate 5G VoWifi is available or not for current operator.
    * value type: String
    */
    RFX_STATUS_KEY_CONFIG_DEVICE_5G_VOWIFI_AVAILABLE,

    /**
     * For Native Carrier Config. Indicate IMS is allow turn off or not for current operator.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_ALLOW_TURN_OFF_IMS,

    /**
     * VoLTE enabled state
     * value type: integer
     *  1:  VoLTE enabled
     *  0: VoLTE disabled
     *  -1: invalid state
     */
    RFX_STATUS_KEY_VOLTE_STATE,

    /**
     * Indicate screen state is on or off.
     * value type: int
     */
    RFX_STATUS_KEY_SCREEN_STATE,

    /**
     * For Telephony-assistant.
     * 1) User should use interface(sendRequestStrings) to send user data.
     * 2) The first string data must be "STATUS_SYNC".
     * 3) The sencond string data should be user data.
     * 4) RILD will update RFX_STATUS key: RFX_STATUS_KEY_TELEPHONY_ASSISTANT_STATUS
     * 5) Module should register to listen status(RFX_STATUS_KEY_TELEPHONY_ASSISTANT_STATUS)
     *    changed, if want to handle it.
     * value type: String
     */
    RFX_STATUS_KEY_TELEPHONY_ASSISTANT_STATUS,

    /** Temp data switch status.
     * Only for OEM customization, unused in internal solution.
     */
    RFX_STATUS_KEY_TEMP_DATA_SWTICH,

    /**
     * Indicate in call
     */
    RFX_STATUS_KEY_IN_CALL,

    /**
     * To flip between CHLD=0 and 1
     */
    RFX_STATUS_KEY_HANGUP_FLIP,

    /**
     * For Native Carrier Config. Indicate Customized User Agent Format
     * value type: String
     */
    RFX_STATUS_KEY_CUSTOMIZED_USER_AGENT_FORMAT,

    /**
     * Indicate the sim slot lock policy value
     * value type : int
     * SML_SLOT_LOCK_POLICY_UNKNOWN        -1
     * SML_SLOT_LOCK_POLICY_NONE        0
     * SML_SLOT_LOCK_POLICY_ONLY_SLOT1        1
     * SML_SLOT_LOCK_POLICY_ONLY_SLOT2        2
     * SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL        3
     * SML_SLOT_LOCK_POLICY_LK_SLOT1        4
     * SML_SLOT_LOCK_POLICY_LK_SLOT2        5
     * SML_SLOT_LOCK_POLICY_LK_SLOTA        6
     * SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_CS        7
     * SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_REVERSE        8
     * SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_ECC_FOR_VALID_NO_SERVICE        9
     * SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_VOICE        10
     * SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL_AND_RSU_VZW    11
     * SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_KDDI    13
     * SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_TMO    15
     * SML_SLOT_LOCK_POLICY_LEGACY        255
     */
    RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY,

    /**
     * Indicate the sim slot lock state value
     * value type : int
     * SML_SLOT_LOCK_POLICY_LOCK_STATE_UNKNOWN        -1
     * SML_SLOT_LOCK_POLICY_LOCK_STATE_YES        0
     * SML_SLOT_LOCK_POLICY_LOCK_STATE_NO         1
     */
    RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_LOCK_STATE,

    /**
     * Indicate the sim slot lock service capability
     * value type : int
     * SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_UNKNOWN        -1
     * SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_FULL         0
     * SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_CS_ONLY        1
     * SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_PS_ONLY        2
     * SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_ECC_ONLY        3
     * SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_NO_SERVICE        4
     */
    RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY,

    /**
     * Indicate the sim slot lock valid card value
     * value type : int
     * SML_SLOT_LOCK_POLICY_VALID_CARD_UNKNOWN         -1
     * SML_SLOT_LOCK_POLICY_VALID_CARD_YES          0
     * SML_SLOT_LOCK_POLICY_VALID_CARD_NO         1
     * SML_SLOT_LOCK_POLICY_VALID_CARD_ABSENT         2
     */
    RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_VALID_CARD,

    /*
     * Record the ongoing IMS PDN activation aid
     * value type: integer
     *  -1: EIMSPDN activation session is done (ie. EIMSPDN confirm is sent)
     *  non -1: current aid value in EIMSPDN deactivation notify URC
     */
    RFX_STATUS_KEY_IMS_PDN_ACT_AID,

    /*
     * Record the ongoing IMS PDN deactivation aid
     * value type: integer
     *  -1: EIMSPDN deactivation session is done (ie. EIMSPDN confirm is sent)
     *  non -1: current aid value in EIMSPDN deactivation notify URC
     */
    RFX_STATUS_KEY_IMS_PDN_DEACT_AID,

    /*
     * Record the ongoing EIMS PDN activation aid
     * value type: integer
     *  -1: EIMSPDN activation session is done (ie. EIMSPDN confirm is sent)
     *  non -1: current aid value in EIMSPDN deactivation notify URC
     */
    RFX_STATUS_KEY_EIMS_PDN_ACT_AID,

    /*
     * Record the ongoing EIMS PDN deactivation aid
     * value type: integer
     *  -1: EIMSPDN deactivation session is done (ie. EIMSPDN confirm is sent)
     *  non -1: current aid value in EIMSPDN deactivation notify URC
     */
    RFX_STATUS_KEY_EIMS_PDN_DEACT_AID,

    /*
    * Indicate modem reset or not
    */
    RFX_STATUS_KEY_MODEM_RESET,

    /*
     * Indicate SMS format change is ongoing.
     * Such as 3GPP to 3GPP2 or 3GPP2 to 3GPP
     */
    RFX_STATUS_KEY_SMS_FORMAT_CHANGE_ONGOING,

    /*
     * Indicate mode of GWSD
     */
    RFX_STATUS_KEY_GWSD_MODE,

    /*
     * IMS registration support feature, value may be 0x5, 0xd etc.
     * value type: integer
     *  1: voice over LTE
     *  2: rcs over LTE
     *  4: sms over LTE
     *  8: video over LTE
     * 16: voice over wifi
     */
    RFX_STATUS_KEY_IMS_REGISTRATION_STATE,

    /*
     * IMS registration rat. Add for VoData feature.
     * value type: integer
     * 0: LTE
     * 1: WIFI
     * 2: EHRPD
     * 3: GSM
     * 4: UMTS
     * 5: NR
     * 6: Data_1
     * 7: Data_2
     * 8: Data_3
     * 9: Data_4
     */
    RFX_STATUS_KEY_IMS_REGISTRATION_RAT,

    /*
     * slot EDALLOW Status
     * value type: integer
     *  0: MD's status is allow 0
     *  1: MD's status is allow 1
     *  -1: AP sent allow 0 to MD but not yet responsed.
     *  -2: AP sent allow 1 to MD but not yet responsed.
     */
    RFX_STATUS_KEY_SLOT_ALLOW,

    /*
     * Indicate the IMS emergency bearer support
     * value type: integer
     *  0 : Not support or in limited service.
     *  1 : Support
     */
    RFX_STATUS_KEY_IMS_EMERGENCY_BEARER_SUPPORT,

    /**
    * VOPS indication. The contents of the URC +CIREPI
    */
    RFX_STATUS_KEY_VOPS,

    /*
    * indicate whether has checked the ESMLSTATUS
    */
    RFX_STATUS_KEY_SML_SLOT_LOCK_CONFIG_CHECKED,

    /*
    * indicate whether MD supports slot lock feature
    */
    RFX_STATUS_KEY_SML_SLOT_LOCK_MD_SUPPORTED,

    /**
     * Store Preferred data SIM. Which is same as FWK's value
     * -1: Unset
     * 0:  Slot 0
     * 1:  Slot 1
     */
    RFX_STATUS_KEY_PREFERRED_DATA_SIM,

    /**
     * Indicate the +EIMSESS state for checking if emergency service is supported in limited service.
     */
    RFX_STATUS_KEY_IMS_EMERGENCY_SUPPORT_STATE,

    /**
    * Indicate network scan(plmn list) ongoing or not
    */
    RFX_STATUS_KEY_NETWORK_SCAN_ONGOING,
    /*
     * DSDA mode
     * 0:  disable
     * 1:  enable
     */
    RFX_STATUS_DSDA_MODE,

    /*
     * IMS registration info - Type
     */
    RFX_STATUS_KEY_IMS_REG_IND_TYPE,

    /**
     * IMS registration info - Account identifier
     */
    RFX_STATUS_KEY_IMS_REG_IND_ACCOUNT_ID,

    /**
     * IMS registration info - Expire time
     */
    RFX_STATUS_KEY_IMS_REG_IND_EXPIRE_TIME,

    /**
     * IMS registration info - Uri
     */
    RFX_STATUS_KEY_IMS_REG_IND_URI,

    /**
     * IMS registration info - Error code
     */
    RFX_STATUS_KEY_IMS_REG_IND_ERROR_CODE,

    /**
     * IMS registration info - Error message
     */
    RFX_STATUS_KEY_IMS_REG_IND_ERROR_MSG,

    /*
     * Record the IMS PDN state in MD
     * value type: integer
     *  0: init
     *  1: activated
     *  2: deactivated
     */
    RFX_STATUS_KEY_IMS_PDN_STATE,

    /*
     * Record the EIMS PDN state in MD
     * value type: integer
     *  0: init
     *  1: activated
     *  2: deactivated
     */
    RFX_STATUS_KEY_EIMS_PDN_STATE,

    /**
     * For Native Carrier Config. indicate reject IMS incoming call rules.
     * value type: String
     *  "0": Allow
     *  "1": Reject any video call when in IMS call, reject any incoming call when in video call.
     *  "2": Reject any IMS call when in conference(host) call.
     */
    RFX_STATUS_KEY_IMS_INCOMING_CALL_RULE,

    /**
     * For Native Carrier Config, indicate that need ignore CLIR when ECC.
     * (Always set as presentation not restricted)
     * value type: String
     *  "0": Should ignore CLIR when ECC (Always set as presentation not restricted)
     *  "1": Should not ignore CLIR when ECC
     */
    RFX_STATUS_KEY_CARRIER_IGNORE_CLIR_WHEN_ECC,
    /**
     * Is IMS call disconnected
     * value type: bool
     */
    RFX_STATUS_KEY_IMS_CALL_DISCONNECTED,
    /**
     * For USSD session control.
     * value type: Bool
     */
    RFX_STATUS_KEY_USSD_SESSION_ONGOING,

    /*
    * Indicate if consider the first user in CEP as host or not.
    */
    RFX_STATUS_KEY_IMS_CONFERENCE_FIRST_PARTICIPANT_AS_HOST,

    /*
    * Indicate if last disconnected call is CDMA call
     * value type: bool
    */
    RFX_STATUS_KEY_CDMA_CALL_DISCONNECTED,

    /*
    * Indicate MCCMNC changed (+EOPSU) for emergency call.
    */
    RFX_STATUS_KEY_MCCMNC_CHANGED,

    /*
    * Indicate if radio power turn on for emergency call.
    * value type: bool
    */
    RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC,

    /*
    * MTK BSP project indicate the selected slot for emergency call.
    * value type: int
    */
    RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT,

    /**
     * For Native Carrier Config. Indicate should update call number from ECPI or not.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_UPDATE_ADDRESS_FROM_ECPI,

    /**
     * For Native Carrier Config. Indicate should update call number by pau or not.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_UPDATE_ADDRESS_BY_PAU_FOR_MO,

    /**
     * For Native Carrier Config. Indicate should update call number by sip field.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_UPDATE_ADDRESS_BY_SIP_FIELD,

    /**
     * For Native Carrier Config. Indicate network will treat duplicated number
     * as one participant or not.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_TREAT_DUPLICATED_NUMBER_AS_ONE_PARTICIPANT,

    /**
     * Slot data connection except IMS data connection status is changed.
     * RFX_STATUS_KEY_DATA_CONNECTION will include all data connection.
     * status will be notified only if status is changed.
     * value type : int
     * DISCONNECTED: no active data connection exist.
     * CONNECTED: at least one data connection exist.
     */
    RFX_STATUS_KEY_DATA_CONNECTION_EXP_IMS,

    /**
     * For data and network MIPC. Indicate whether rild need to receive unsolicited response or not.
     * value type: int, refer to RIL_UnsolicitedResponseFilter
     */
    RFX_STATUS_KEY_UNSOLICITED_RESPONSE_FILTER_CHANGED,

    /**
     * Auto answer setting from engineer mode
     * value type: String
     */
    RFX_STATUS_KEY_AUTO_ANSWER,

    /**
     * Camped network supports VoLTE emergency bearers
     * value type: int
     */
    RFX_STATUS_KEY_VOLTE_EMC_SUPPORTED,

    /**
     * Camped network supports VoNR emergency service
     * value type: int
     */
    RFX_STATUS_KEY_VONR_EMC_SUPPORTED,

    /**
     * Camped network supports VoNR emergency service fallback
     * value type: int
     */
    RFX_STATUS_KEY_VONR_EMF_SUPPORTED,

    /**
     * For Native Carrier Config.
     * Indicate turn off eims when volte/vilte/vowifi/viwifi are off, ignore sms config.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_IGNORE_SMS_FOR_EIMS,

    /**
     * For lte rrc state changed.
     * -1:LTE RRC state unknown
     *  0:RRC idle
     *  1:RRC connected
     * value type: int
     */
    RFX_STATUS_KEY_LTE_RRC_STATE,

    /*
    * Indicate if sim initialize status.
    * value type: int
    * -1:no card or card removed
    *  0:init start
    *  1:init end
    */
    RFX_STATUS_KEY_SIM_INIT_STATUS,

     /**
     * For Native Carrier Config.
     * Indicate viwifi status not follow with wfc status.
     * value type: String
     */
    RFX_STATUS_KEY_CARRIER_VIWIFI_INDEPENDENT_VOWIFI,

    /**
     * indicate if current sim need ignore ims for 2/3g.
     * value type: string
     */
    RFX_STATUS_KEY_IGNORE_VOLTE_VILTE_FOR_23G,

    /**
     * ESIPCPI reason REG09 text state. By slot.
     * value type : enum ImsEsipcpiReason
     * IMS_ESIPCPI_REASON_UNKNOWN     -1
     * IMS_ESIPCPI_REASON_REG09_START  0
     * IMS_ESIPCPI_REASON_REG09_BLOCK  1
     * IMS_ESIPCPI_REASON_REG09_ACTIVE 2
     */
    RFX_STATUS_KEY_IMS_ESIPCPI_REASON,

    /**
     * For ons stored in simcards
     * value type: String
     */
    RFX_STATUS_KEY_ONS,

    /**
     * For short ons stored in simcards
     * value type: String
     */
    RFX_STATUS_KEY_SHORT_ONS,

    /*
     * Record the SBP ID which will be set to Modem by CCCI.
     * value type: String8
     */
    RFX_STATUS_KEY_SBP_ID,

    /**
     * Record the subordinate ID under one SBP which will be set to Modem by CCCI.
     * value type: String8
     */
    RFX_STATUS_KEY_SBP_SUB_ID,

    /**
     * Record the all handlers are created when rild started.
     * value type: bool
     */
    RFX_STATUS_KEY_RILD_INIT_HANDLER_DONE,

    /**
     * For MEP supported version
     * value type: int
     */
    RFX_STATUS_KEY_MEP_VERSION,

    /**
    * indicate socket state betwwen RILD and MTK RILJ
    */
    RFX_STATUS_MTK_RADIO_CONNECTION_STATE,

    /**
    * indicate AT+CHLD=2 sent to Modem, but OK not received
    */
    RFX_STATUS_KEY_IS_CHLD2_ONGOING,

    /**
     * For the scenario as below, currently only charter/comcast supported
     * 1. DDS setup internet pdn
     * 2. NDDS setup internet pdn
     * 3. Set prefer data to NDDS, so DDS is changed to NDDS
     * 4. Disconnect default pdn of NDDS, keep NDDS default pdn at modem side
     */
    RFX_STATUS_KEY_KEEP_DEFAULT_PDN_FOR_NDDS,

    /**
    * For IMS Provision Config, refer to Mtk_ims_config.h @{
    */
    RFX_STATUS_KEY_IMS_CONFIG_VOCODER_AMRMODESET, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_VOCODER_AMRWBMODESET, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_SIP_SESSION_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_MIN_SE, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_CANCELLATION_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_TDELAY, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_SILENT_REDIAL_ENABLE, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_SIP_T1_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_SIP_T2_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_SIP_TF_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_VLT_SETTING_ENABLED, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_LVC_SETTING_ENABLED, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_DOMAIN_NAME, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_SMS_FORMAT, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_SMS_OVER_IP, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_PUBLISH_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_PUBLISH_TIMER_EXTENDED, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_CAPABILITY_DISCOVERY_ENABLED, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_CAPABILITIES_CACHE_EXPIRATION, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_AVAILABILITY_CACHE_EXPIRATION, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_CAPABILITIES_POLL_INTERVAL, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_SOURCE_THROTTLE_PUBLISH, // IMS_CONFIG_UNIT_MILLISECONDS
    RFX_STATUS_KEY_IMS_CONFIG_MAX_NUMENTRIES_IN_RCL, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_CAPAB_POLL_LIST_SUB_EXP, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_GZIP_FLAG, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_EAB_SETTING_ENABLED, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_VOICE_OVER_WIFI_ROAMING, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_VOICE_OVER_WIFI_MODE, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_VOICE_OVER_WIFI_SETTING_ENABLED, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_RTT_SETTING_ENABLED, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_EPDG_ADDRESS, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_PUBLISH_ERROR_RETRY_TIMER, // IMS_CONFIG_UNIT_SECONDS
    RFX_STATUS_KEY_IMS_CONFIG_VOICE_OVER_WIFI_MDN, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_VOICE_DOMAIN_PREFERENCE, // IMS_CONFIG_UNIT_NONE
    RFX_STATUS_KEY_IMS_CONFIG_VONR_SETTING_ENABLED, // IMS_CONFIG_UNIT_NONE
    // end of IMS Provision Config }@

    /**
     * For checking if PCO data feature supported, currently only VZW supported.
     */
    RFX_STATUS_KEY_PCO_DATA_FEATURE_SUPPORTED,

    /**
     * For SIM switch from pSIM to eSIM, and vice versa.
     * value type: int
     */
    RFX_STATUS_KEY_SIM_SWITCH_TYPE_STATE,

    /**
     * SIM phonebook version, non-slot key.
     * value type : enum PhbVersion
     * PHB_VERSION_UNKNOWN -1
     * PHB_VERSION_MTK     0
     * PHB_VERSION_SIMIO   1
     */
    RFX_STATUS_KEY_SIM_PHONEBOOK_VERSION,

    /*Please add your keys above this line*/
    RFX_STATUS_KEY_END_OF_ENUM
};

#define RFX_CARD_TYPE_SIM  0x01
#define RFX_CARD_TYPE_USIM 0x02
#define RFX_CARD_TYPE_CSIM 0x04
#define RFX_CARD_TYPE_RUIM 0x08
#define RFX_CARD_TYPE_ISIM 0x10

#define RFX_SIM_STATE_UNKNOWN 0
#define RFX_SIM_STATE_ABSENT 1
#define RFX_SIM_STATE_PIN_REQUIRED 2
#define RFX_SIM_STATE_PUK_REQUIRED 3
#define RFX_SIM_STATE_NETWORK_LOCKED 4
#define RFX_SIM_STATE_READY 5
#define RFX_SIM_STATE_NOT_READY 6
#define RFX_SIM_STATE_PERM_DISABLED 7
#define RFX_SIM_STATE_CARD_IO_ERROR 8
#define RFX_SIM_STATE_CARD_RESTRICTED 9


/* MODEM_OFF_STATE*/
#define MODEM_OFF_IN_IDLE             (0)
#define MODEM_OFF_BY_MODE_SWITCH      (1)
#define MODEM_OFF_BY_POWER_OFF        (2)
#define MODEM_OFF_BY_SIM_SWITCH       (3)
#define MODEM_OFF_BY_RESET_RADIO      (4)
#define MODEM_OFF_BY_WORLD_PHONE      (5)

/*SMS type*/
#define SMS_INBOUND_NONE (0)
#define SMS_INBOUND_3GPP_CMT (1)
#define SMS_INBOUND_3GPP_CDS (2)
#define SMS_INBOUND_3GPP_CMTI (3)

/*SMS phone storage status*/
#define SMS_PHONE_STORAGE_AVAILABLE (0)
#define SMS_PHONE_STORAGE_FULL (1)

#define CDMA_MO_SMS_SENDING (0)
#define CDMA_MO_SMS_SENT (1)

#define CDMA_SMS_INBOUND_NONE    (0)
#define CDMA_SMS_INBOUND_COMM    (1)
#define CDMA_SMS_INBOUND_VMI     (2)

#define AP_TRIGGER_SWITCH_SIM (1)
#define GMSS_TRIGGER_SWITCH_SIM (2)
#define AP_TRIGGER_SWITCH_RUIM (3)
#define GMSS_TRIGGER_SWITCH_RUIM (4)

#define RFX_NO_ONGOING_IMS_PDN_AID     -1

/* For RAT SWITCH*/
typedef enum {
    RAT_SWITCH_UNKNOWN = -1,
    /* Rat switch for mode controller */
    RAT_SWITCH_INIT = 0,
    /* RAT switch done for NWS */
    RAT_SWITCH_NWS = 1,
    /* RAT switch done for RIL Request and signal */
    RAT_SWITCH_NORMAL = 2,
    /* Rat switch for some restricted mode. ex: ECC redial */
    RAT_SWITCH_RESTRICT = 3
} RatSwitchCaller;

/* RFX_STATUS_KEY_RADIO_LOCK */
typedef enum {
    RADIO_LOCK_IDLE = 0,
    RADIO_LOCK_BY_RADIO,
    RADIO_LOCK_BY_WORLD_MODE,
    RADIO_LOCK_BY_SIM_SWITCH,
    RADIO_LOCK_BY_DATA,
    RADIO_LOCK_BY_THERMAL,
    RADIO_LOCK_BY_NW,
} RadioPowerLock;

/* DATA CONNECTION STATE*/
#define DATA_STATE_DISCONNECTED        (0)
#define DATA_STATE_CONNECTED           (1)

/*IMS/EIMS PDN CONNECTION STATE*/
#define IMS_OR_EIMS_PDN_INIT 0
#define IMS_OR_EIMS_PDN_ACTIVATED 1
#define IMS_OR_EIMS_PDN_DEACTIVATED 2

#define WORLD_MODE_SWITCHING (0)

/* For Bluetooth SIM Access Profile */
typedef enum
{
   BT_SAP_INIT,
   BT_SAP_CONNECTION_SETUP,
   BT_SAP_ONGOING_CONNECTION,
   BT_SAP_DISCONNECT,
   BT_SAP_POWER_ON,
   BT_SAP_POWER_OFF,
} BtSapStatus;

/* RFX_STATUS_KEY_CAPABILITY_SWITCH_STATE */
typedef enum {
    CAPABILITY_SWITCH_STATE_IDLE = 0,
    CAPABILITY_SWITCH_STATE_START = 1,
    CAPABILITY_SWITCH_STATE_ENDING = 2,
    CAPABILITY_SWITCH_STATE_NO_NEED = 3,
} CapabilitySwitchState;

/* RFX_STATUS_KEY_CAPABILITY_SWITCH_WAIT_MODULE */
#define CAPABILITY_SWITCH_WAIT_MODEM_CONFIG  (0x01)

/* Value of RFX_STATUS_KEY_DSBP_ENHANCEMENT_STATE */
typedef enum {
    DSBP_ENHANCEMENT_END = 0,
    DSBP_ENHANCEMENT_START = 1,
} DSBPEnhancementStatus;

// UICC Applications enable/disable feature
/*RFX_STATUS_KEY_MD_SIM_SWITCH_MODE & RFX_STATUS_KEY_AP_SIM_SWITCH_MODE value*/
typedef enum {
    SIM_SWITCH_MODE_UNKNOWN = -1,   // Default value, it is not expected for a project
    SIM_SWITCH_MODE_AOSP_POWER_ON_OFF = 0,
    SIM_SWITCH_MODE_MTK_RADIO_ON_OFF = 1,
    SIM_SWITCH_MODE_MTK_SIM_ON_OFF = 2,
    SIM_SWITCH_MODE_AOSP_UICC_APPLICATIONS_ENABLE_DISABLE = 3,
    SIM_SWITCH_MODE_NONE = 4,
} SimSwitchMode;

/*RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_RESPONSE_STATE value*/
typedef enum {
    UICC_APPLICATIONS_ENABLE_RESPONSE_STATE_IDLE = 0,  // Default value
    UICC_APPLICATIONS_ENABLE_RESPONSE_STATE_WAITING_ENABLE = 1,
    UICC_APPLICATIONS_ENABLE_RESPONSE_STATE_WAITING_DISABLE = 2,
} UiccApplicationsEnableResponseState;

/*RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE value*/
typedef enum {
    UICC_APPLICATIONS_ENABLE_STATE_UNKNOWN = -1,  // Default value
    UICC_APPLICATIONS_ENABLE_STATE_ENABLED = 0,
    UICC_APPLICATIONS_ENABLE_STATE_ENABLING = 1,
    UICC_APPLICATIONS_ENABLE_STATE_DISABLED = 2,
    UICC_APPLICATIONS_ENABLE_STATE_DISABLING = 3,
} UiccApplicationsEnableState;

/*RFX_STATUS_KEY_SIM_POWER_STATE value*/
typedef enum {
    SIM_POWER_STATE_UNKNOWN = -1,
    SIM_POWER_STATE_V11_SIM_OFF = 0,
    SIM_POWER_STATE_V11_SIM_ON = 1,
    SIM_POWER_STATE_PASS_THROUGH = 2,
    SIM_POWER_STATE_V16_SIM_OFF = 3,
    SIM_POWER_STATE_V16_SIM_ON = 4,
} simPowerState;

typedef enum {
    SIM_INIT_STATE_ABSENT  = -1,
    SIM_INIT_STATE_INSERT  = 0,
    SIM_INIT_STATE_DETECT  = 1,
    SIM_INIT_STATE_READY   = 2,
    SIM_INIT_STATE_FINISH  = 3,
} SimInitState;

typedef enum {
    SIM_TYPE_STATE_IDLE = 0,
    SIM_TYPE_STATE_PROCESSING = 1,
} UiccSwitchTypeState;

/*RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY value*/
typedef enum {
    SML_SLOT_LOCK_POLICY_UNKNOWN = -1,
    SML_SLOT_LOCK_POLICY_NONE = 0,
    SML_SLOT_LOCK_POLICY_ONLY_SLOT1 = 1,
    SML_SLOT_LOCK_POLICY_ONLY_SLOT2 = 2,
    SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL = 3,
    SML_SLOT_LOCK_POLICY_LK_SLOT1 = 4,
    SML_SLOT_LOCK_POLICY_LK_SLOT2 = 5,
    SML_SLOT_LOCK_POLICY_LK_SLOTA = 6,
    SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_CS = 7,
    SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_REVERSE = 8,
    SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_ECC_FOR_VALID_NO_SERVICE = 9,
    //10 ~ 50 as rsu
    SML_SLOT_LOCK_POLICY_LK_SLOTA_RESTRICT_INVALID_VOICE = 10,
    SML_SLOT_LOCK_POLICY_ALL_SLOTS_INDIVIDUAL_AND_RSU_VZW = 11,
    SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_KDDI = 13,
    SML_SLOT_LOCK_POLICY_LK_SLOTA_RSU_TMO = 15,
    //other non-rsu policy
    /*
     * Thailand TRUE requirements. See ALPS06412394.
     * rule = 3,
     * sub_rule = 512
     */
    SML_SLOT_LOCK_POLICY_TRUE = 51,
    /*
     * JIO PLMN Lock requirements.
     * rule = 9,
     * sub_rule = 516
     */
    SML_SLOT_LOCK_POLICY_JIO_PLMN = 52,
    SML_SLOT_LOCK_POLICY_LEGACY = 255
} SmlSlotLockPolicy;

/*RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_LOCK_STATE value*/
typedef enum {
    SML_SLOT_LOCK_POLICY_LOCK_STATE_UNKNOWN = -1,
    SML_SLOT_LOCK_POLICY_LOCK_STATE_YES = 0,
    SML_SLOT_LOCK_POLICY_LOCK_STATE_NO = 1
} SmlSlotLockState;

/*RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY value*/
typedef enum {
    SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_UNKNOWN = -1,
    SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_FULL = 0,
    SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_CS_ONLY = 1,
    SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_PS_ONLY = 2,
    SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_ECC_ONLY = 3,
    SML_SLOT_LOCK_POLICY_SERVICE_CAPABILITY_NO_SERVICE = 4
} SmlSlotLockServiceCapability;

/*RFX_STATUS_KEY_SML_SLOT_LOCK_POLICY_VALID_CARD value*/
typedef enum {
    SML_SLOT_LOCK_POLICY_VALID_CARD_UNKNOWN = -1,
    SML_SLOT_LOCK_POLICY_VALID_CARD_YES = 0,
    SML_SLOT_LOCK_POLICY_VALID_CARD_NO = 1,
    SML_SLOT_LOCK_POLICY_VALID_CARD_ABSENT = 2
} SmlSlotLockValidCard;

/* RFX_STATUS_KEY_IMS_ESIPCPI_REASON */
typedef enum {
    IMS_ESIPCPI_REASON_UNKNOWN     = -1,
    IMS_ESIPCPI_REASON_REG09_START = 0,
    IMS_ESIPCPI_REASON_REG09_BLOCK = 1,
    IMS_ESIPCPI_REASON_REG09_ACTIVE = 2
} ImsEsipcpiReason;

/* RFX_STATUS_KEY_SIM_PHONEBOOK_VERSION */
typedef enum {
    PHB_VERSION_UNKNOWN = -1,
    PHB_VERSION_MTK     = 0,
    PHB_VERSION_SIMIO   = 1,
} PhbVersion;

/*RFX_STATUS_KEY_ECC_PREFERRED_RAT*/
#define ECC_PREFER_RAT_AUTO         (0)
#define ECC_PREFER_RAT_GSM_ONLY     (1)
#define ECC_PREFER_RAT_CDMA_ONLY    (2)
#define ECC_PREFER_RAT_GSM_PREF     (3)
#define ECC_PREFER_RAT_CDMA_PREF    (4)


#define RFX_STATUS_DEFAULT_VALUE_ENTRY(key, value) {key, #key, value}

#define RFX_STATUS_DEFAULT_VALUE_TABLE_BEGIN(class_name)                       \
        const class_name::StatusDefaultValueEntry class_name::s_default_value_table[] = {

#define RFX_STATUS_DEFAULT_VALUE_TABLE_END                         \
        RFX_STATUS_DEFAULT_VALUE_ENTRY(RFX_STATUS_KEY_END_OF_ENUM, RfxVariant())}

#define RFX_STATUS_DECLARE_DEFAULT_VALUE_TABLE                     \
        static const StatusDefaultValueEntry s_default_value_table[]
#endif /* __RFX_STATUS_DEFS_H__ */
