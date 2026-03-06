/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#ifndef __RMC_NETWORK_HANDLER_H__
#define __RMC_NETWORK_HANDLER_H__

#undef NDEBUG
#ifdef LOG_NDEBUG
#undef LOG_NDEBUG
#endif

#include "RfxBaseHandler.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include <telephony/mtk_ril.h>
#include "ViaBaseHandler.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcNwHdlr"

#define MAX_OPER_NAME_LENGTH 50
#define MAX_TOE_BUFFER_LEN   (MAX_OPER_NAME_LENGTH*2 + 20)

typedef enum {
    CACHE_GROUP_GSM        = 0,
    CACHE_GROUP_C2K        = 1,
    CACHE_GROUP_1XRTT      = 2,
    CACHE_GROUP_EVDO       = 3,
    CACHE_GROUP_NR         = 4,
    CACHE_GROUP_COMMON_REQ = 5,
    CACHE_GROUP_ALL        = 6
} RIL_CACHE_GROUP;

typedef enum {
    SIGNAL_GROUP_NONE       = 0x00,
    SIGNAL_GROUP_GSM        = 0x01,
    SIGNAL_GROUP_1XRTT      = 0x02,
    SIGNAL_GROUP_UMTS       = 0x04,
    SIGNAL_GROUP_EVDO       = 0x08,
    SIGNAL_GROUP_TDSCDMA    = 0x10,
    SIGNAL_GROUP_LTE        = 0x20,
    SIGNAL_GROUP_NR         = 0x40,
    SIGNAL_GROUP_ALL        = 0x7F
} RIL_SIGNAL_GROUP;

typedef enum {
    ACT_NONE           = 0x0000,
    ACT_GPRS           = 0x0001,
    ACT_EDGE           = 0x0002,
    ACT_UMTS           = 0x0004,
    ACT_HSDPA          = 0x0008,
    ACT_HSUPA          = 0x0010,
    ACT_HSDPAP         = 0x0020,
    ACT_HSUPAP         = 0x0040,
    ACT_1XRTT          = 0x0100,
    ACT_HRPD           = 0x0200,
    ACT_EHRPD          = 0x0400,
    ACT_LTE            = 0x1000,
    ACT_LTE_CA         = 0x2000,
    ACT_ENDC           = 0x4000,
    ACT_NR             = 0x8000
} RIL_RAT_ACT;

typedef struct {
    int ssRsrp;
    int ssRsrq;
    int ssSinr;
    int csiRsrp;  // 25
    int csiRsrq;
    int csiSinr;
} RIL_NR_SIGNAL;

typedef struct {
    int sig1;
    int sig2;
    int rssi_in_qdbm;
    int rscp_in_qdbm;
    int ecn0_in_qdbm;
    int rsrq_in_qdbm;
    int rsrp_in_qdbm;
    int act;
    int sig3;
    int serv_band;
    int csiRsrq_in_qdb;
    int csiRsrp_in_qdbm;
    int csiSinr_in_qdb;
} MD_SIGNAL_STRENGTH;

typedef struct {
    int registration_state;
    unsigned int lac;
    unsigned long long cid;
    int radio_technology;
    int base_station_id;
    int base_station_latitude;
    int base_station_longitude;
    int css;
    int system_id;
    int network_id;
    int roaming_indicator;
    int is_in_prl;
    int default_roaming_indicator;
    int denied_reason;
    //int psc;
    int network_exist;
} RIL_VOICE_REG_STATE_CACHE;

typedef struct {
    int registration_state;
    unsigned int lac;
    unsigned long long cid;
    int radio_technology;
    int denied_reason;
    int max_simultaneous_data_call;
    // NR none-standalone
    int dcnr_restricted;
    int endc_sib;
    int endc_available;
} RIL_DATA_REG_STATE_CACHE;

typedef struct {
    int ca_info;
    int pcell_bw;
    int scell_bw1;
    int scell_bw2;
    int scell_bw3;
    int scell_bw4;
} RIL_CA_CACHE;

typedef struct {
    int is_endc;
    int band_1;
    int band_2;
} RIL_NR_CA_CACHE;

typedef struct {
    char mccMnc[8];
    char longName[MAX_OPER_NAME_LENGTH];
    char shortName[MAX_OPER_NAME_LENGTH];
} RIL_OPERATOR_INFO_CACHE;

typedef struct {
    int domain;
    int state;
    int plmn_id;
    int act;
    int is_femtocell;
    int is_csg_cell;
    int csg_id;
    int csg_icon_type;
    char hnbName[MAX_OPER_NAME_LENGTH];
    int cause;
} RIL_FEMTO_CELL_CACHE;

/* EONS status reported from modem */
typedef enum
{
    EONS_INFO_NOT_RECEIVED,
    EONS_INFO_RECEIVED_DISABLED,
    EONS_INFO_RECEIVED_ENABLED
} RIL_EonsStatusInfo;

// Defines EONS network feature support info.
typedef struct {
    RIL_EonsStatusInfo eons_status;
    unsigned int lac_cs;
    unsigned int lac_ps;
} RIL_EonsNetworkFeatureInfo;

typedef struct SPNTABLE {
    char mccMnc[8];
    char spn[MAX_OPER_NAME_LENGTH+2];
    char short_name[MAX_OPER_NAME_LENGTH+2];
} SpnTable;

typedef enum {
    RAT_GROUP_UNKNOWN,
    RAT_GROUP_2G,
    RAT_GROUP_3G,
    RAT_GROUP_4G
} RIL_RAT_GROUP;

/* +EREG: <n>,<stat>,<lac>,<ci>,<eAct>,<nwk_existence >,<roam_indicator>,<cause_type>,<reject_cause> */
typedef struct {
    int n;
    int stat;
    unsigned int lac;
    unsigned long long ci;
    int eAct;
    int nwk_existence;
    int roam_indicator;
    int cause_type;
    int reject_cause;
} MD_EREG;

/* +EGREG: <n>, <stat>, <lac>, <cid>, <eAct>, <rac>,
        <nwk_existence>, <roam_indicator>, <cause_type>, <reject_cause> */
typedef struct {
    int n;
    int stat;
    unsigned int lac;
    unsigned long long ci;
    int eAct;
    int rac;
    int nwk_existence;
    int roam_indicator;
    int cause_type;
    int reject_cause;
    // NR
    int dcnr_restricted;
    int endc_sib;
    int endc_available;
} MD_EGREG;

typedef struct {
    int mode;
    int format;
    char oper[10];
} MD_EOPS;

typedef struct {
    int upperlayerIndR15Available; //0 or 1
    int nrSubcarrierSpacingKHz;
} NR_SYS_INFO;


typedef struct {
    int act;
    long long cid;
    int mcc;
    int mnc;
    int lacTac;
    int pscPci;
    int sig1;
    int sig2;
    int rsrp;
    int rsrq;
    int timingAdvance;
    int rssnr;
    int cqi;
    int bsic;
    int arfcn;
    int band;
} MD_ECELL;

#define MAX_LTE_ENDC_MAX_CAPABILITY 10
typedef struct {
    long long cid;
    int lacTac;
    int dcnr_restricted;
    int endc_sib;
    int endc_available;
} LTE_ENDC_MAX_CAPABILITY;

//OEM response/urc
typedef struct {
    char oem_type;
    char messageId[3];
    char payload_size[4];
    char tagOfCa;
    char lengthOfCa;
    char statusOfCa;
    char tagForTotalNRB;
    char lengthOfTotalNRBfield;
    char totalNRB[4];
} OEM_CA_INFO;

typedef struct {
    int volteEmcSupported;
    int vonrEmcSupported;
    int vonrEmfSupported;
} ImsEmergencyInfo;

typedef struct {
    int rsrp;
    int rssnr;
    bool initialed;
} LteSignalProperty;

typedef struct {
    int pci;
    int arfcn;
    int band;
    int bandwidth;
} CellIdentityInfo;

typedef enum {
    /* Not doing search */
    NW_PLMN_LIST_TYPE_NONE   = 0,
    /* query available network type or not support full network scan */
    NW_PLMN_LIST_TYPE_NORMAL = 1,
    /* full network scan */
    NW_PLMN_LIST_TYPE_NETWORK_SCAN = 2,
    /*others*/
    NW_PLMN_LIST_TYPE_OTHERS = 3
} NETWORK_PLMN_LIST_SEARCH_TYPE;


#define PROPERTY_NITZ_OPER_CODE     "persist.vendor.radio.nitz_oper_code"
#define PROPERTY_NITZ_OPER_LNAME    "persist.vendor.radio.nitz_oper_lname"
#define PROPERTY_NITZ_OPER_SNAME    "persist.vendor.radio.nitz_oper_sname"

#define PROPERTY_WFC_STATE          "persist.vendor.radio.wfc_state"

#define PROPERTY_DISABLE_AUTO_RETURN_RPLMN "persist.vendor.radio.disable_auto_return_rplmn"

#define LIFE_VALID_PLMN 30000000000 // 30 seconds

// Use int max, as -1 is a valid value in signal strength
#define CELLINFO_INVALID 0x7FFFFFFF

#define DISPLAY_ALL 0x07
#define DISPLAY_TS25 0x01
#define DISPLAY_NITZ 0x02
#define DISPLAY_EONS 0x04

#define NR_DISPLAY_NONE 0x00
#define NR_DISPALY_NR_CACHE 0x01
#define NR_DISPALY_LTE_CONNECTED_STATE 0x02  // deprecated, use JP mode instead
#define NR_DISPLAY_JP_MODE 0x04  // CMCC as well

#define JP_MODE_TIMER_STATE_STOP 0x01
#define JP_MODE_TIMER_STATE_GOING 0x02
#define JP_MODE_TIMER_STATE_TIMEOUT 0x04
#define JP_MODE_TIMER_STATE_STARTING 0x08

#define RETRY_NOTIFY_SYSTEM_TIMER 3

#define MAX_PHY_CHL_CFG_COUNT 15

class RmcNetworkHandler : public RfxBaseHandler {
    public:
        RmcNetworkHandler(int slot_id, int channel_id);
        virtual ~RmcNetworkHandler() {}

        /**
         * Convert registration state
         * @param uiRegState registration state from modem
         * @param isVoiceState whether is voice state
         * @return registration state to framework
         */
        unsigned int convertRegState(unsigned int uiRegState, bool isVoiceState);
        int updateSignalStrength(RfxAtLine *line);
        int getCurrentSignalGroup();
        int getSignalGroupByRegRat(int csRegState, int csRat, int psRegState, int psRat);
        int getMdSignalStrengthByAT(MD_SIGNAL_STRENGTH *ecsq, RfxAtLine *line);
        void resetSignalStrengthCache(RIL_SIGNAL_STRENGTH *sigCache, RIL_SIGNAL_GROUP group);
        void fillSignalStrengthResponse(RIL_SIGNAL_STRENGTH *response, int group);
        int getLastReportedSignal();
        bool enableReportSignalStrengthWithWcdmaEcio();
        bool isTdd3G();
        int isFemtocellSupport();
        int isOp12Plmn(const char* plmn);
        bool isMatchOpid(const char* opid);
        int getCellInfoListV12(RfxAtLine* line, int num, RIL_CellInfo_v12 * response);
        int convert3GRssiValue(int rscp_in_dbm);
        static int hexCharToInt(char c);
        static int hexCharsToUcs2Encode(char* src);
        static void convertToUtf8String(char *src, size_t n);
        static void convertToUtf8StringFromAscii(char *src, size_t n);
        unsigned int convertCSNetworkType(unsigned int uiResponse);
        unsigned int convertPSNetworkType(unsigned int uiResponse);
        void updateWfcState(int status);
        int getPLMNNameFromNumeric(char *numeric,
            char *longname, char *shortname, int max_length);
        int getEonsNamesFromNumericCode(char *code,
            unsigned int lac, char *longname, char *shortname, int max_length, int rat);
        int getOperatorNamesFromNumericCode(char *code,
            char *longname, char *shortname, int max_length);
        int getOperatorNamesFromNumericCode(char *code,
            unsigned int lac, char *longname, char *shortname, int max_length, int rat = 0);
        int getOperatorNamesFromNumericCodeByDisplay(char *code,
            unsigned int lac, char *longname, char *shortname, int max_length, int display, int rat);
        void updateFemtoCellInfo();
        void isFemtoCell(int regState, int cid, int act);
        void printSignalStrengthCache(char *source);
        void isNeedNotifyStateChanged();

        char const *sourceToString(int srcId);
        RIL_RAT_GROUP getVoiceRatGroup(int act);
        RIL_RAT_GROUP getDataRatGroup(int act);

        void setLastValidPlmn(const char* plmn);
        bool getLastValidPlmn(char* plmn, int size);
        bool getCdmaPlmn(char* plmn, int size);
        bool isRilTestMode();
        bool supportMccMncUrc();

        bool isInService(int regState) {
            if (regState == 1 || regState == 5) {
                return true;
            }
            return false;
        }

        bool isHome(int csRegState, int psRegState) {
            return (csRegState == 1 || psRegState == 1);
        }

        bool isNrSupported();
        int isGcfTestMode();
        int isTestSim(int slotId);
        bool isAPInCall();
        bool isNetworkScanFullSupport();
        void cleanNetworkScanResultOperator(RIL_NetworkScanResult* result);

        void refreshEcellCache();
        void freeOperatorNameOfCellInfo(int num, RIL_CellInfo_v12 *response);
        void dumpEregCache(const char* fun, MD_EREG *mMdEreg, MD_ECELL *mMdEcell);
        void dumpEregCache(const char* fun, MD_EREG *mMdEreg, MD_ECELL *mMdEcell, bool debugV);
        void dumpEgregCache(const char* fun, MD_EGREG *mMdEgreg, MD_ECELL *mMdEcell);
        void dumpEgregCache(const char* fun, MD_EGREG *mMdEgreg, MD_ECELL *mMdEcell, bool debugV);
        void dumpEcellCache(const char* fun, MD_ECELL *mMdEcell);
        char* getMask(int i);
        char* getMask(unsigned int i);
        char* getMask(long long i);
        char* getMask(unsigned long long i);
        void dumpLteEndcMaxCapability(unsigned int slot_id);
        int checkNrCapability(unsigned int slot_id);
        void connectionStateReport(int mode, int state, int access);
        int fillBarringInfosResponse(size_t count,
                RIL_BarringInfo* barringInfos, RIL_BarringInforResponse* barringInfoResp);
        void fillDataCellIdentity(RIL_CellIdentity_v16 &cellIdentity);
        void fillVoiceCellIdentity(RIL_CellIdentity_v16 &cellIdentity);
        void clearCellIdentity(RIL_CellIdentity_v16 & cellIdentity);
        void sendRegistrationFailedEvent(RIL_Domain domain);
        int updatePlmnInfo();

        void freeSetNull(char* a) {
            if (a) free(a);
            a = NULL;
        }

        void loadDisplayRuleByOp();

        // JP mode
        bool isJpMode();
        void startJpModeTimer1();
        void stopJpModeTimer1();
        void setJpModeTimer1Timeout();
        void setJpModeTimer1Starting();
        void startJpModeTimer2();
        void stopJpModeTimer2();
        void setJpModeTimer2Timeout();
        void onHandleJpModeTimerEvent(const sp<RfxMclMessage>& msg);
        void onHandleJpModeConnectionEvent(const sp<RfxMclMessage>& msg);
        void onHandleRetryNotifySystem(const sp<RfxMclMessage>& msg);
        void resetNrCaCache(RIL_NR_CA_CACHE * cache);
        void updateNrCaBandInfo();
        void updateSbpInfos();
        void updateSignalStrengthKey(const char* type, int sig1, int sig2);
        bool parsePhyChlCfg(RIL_RadioTechnology curRat, RfxAtLine *line,
                RIL_PhysicalChannelConfig* resp);
        bool queryPrimaryCellIdentityInfo(CellIdentityInfo* pOut);

        static RIL_EonsNetworkFeatureInfo eons_info[MAX_SIM_COUNT];
        /* modem ECELL ext3 ext4 support
         * value: 0 not surrpot
         *        1 support
         **/
        static int ECELLext3ext4Support;
        static RIL_FEMTO_CELL_CACHE *femto_cell_cache[MAX_SIM_COUNT];

        static pthread_mutex_t ril_nw_femtoCell_mutex;

        static pthread_mutex_t s_signalStrengthMutex[MAX_SIM_COUNT];
        static RIL_SIGNAL_STRENGTH *signal_strength_cache[MAX_SIM_COUNT];
        static pthread_mutex_t s_voiceRegStateMutex[MAX_SIM_COUNT];
        static RIL_VOICE_REG_STATE_CACHE *voice_reg_state_cache[MAX_SIM_COUNT];
        static RIL_DATA_REG_STATE_CACHE *data_reg_state_cache[MAX_SIM_COUNT];
        static RIL_OPERATOR_INFO_CACHE *op_info_cache[MAX_SIM_COUNT];
        static pthread_mutex_t op_info_Mutex[MAX_SIM_COUNT];

        static pthread_mutex_t s_urc_voiceRegStateMutex[MAX_SIM_COUNT];
        static RIL_VOICE_REG_STATE_CACHE *urc_voice_reg_state_cache[MAX_SIM_COUNT];
        static pthread_mutex_t s_urc_dataRegStateMutex[MAX_SIM_COUNT];
        static RIL_DATA_REG_STATE_CACHE *urc_data_reg_state_cache[MAX_SIM_COUNT];
        static char last_valid_plmn[MAX_SIM_COUNT][8];
        static nsecs_t last_valid_plmn_time[MAX_SIM_COUNT];

        static pthread_mutex_t s_caMutex[MAX_SIM_COUNT];
        static pthread_mutex_t s_nrCaMutex[MAX_SIM_COUNT];
        static RIL_CA_CACHE *ca_cache[MAX_SIM_COUNT];
        static pthread_mutex_t s_nwCfgMutex[MAX_SIM_COUNT];
        static RIL_NwCfgInfo *nw_cfg_cache[MAX_SIM_COUNT];
        static RIL_NR_CA_CACHE *nr_ca_cache[MAX_SIM_COUNT];

        /* WFC registration status */
        static int ril_wfc_reg_status[MAX_SIM_COUNT];
        static pthread_mutex_t s_wfcRegStatusMutex[MAX_SIM_COUNT];

        static bool mIsNeedNotifyState[MAX_SIM_COUNT];
        static int mIsNeedNotifyState_Id[MAX_SIM_COUNT];
        static int sPlmnListOngoingType;
        static int mPlmnListAbort;

        // cache SPN_TABLE
        static SPNTABLE cache_spn_table[MAX_SIM_COUNT];

        static bool m_dc_support[MAX_SIM_COUNT];
        static bool ims_ecc_state[MAX_SIM_COUNT];
        static int data_context_ids[MAX_SIM_COUNT][16];

        static MD_EREG *mdEreg[MAX_SIM_COUNT];
        static pthread_mutex_t mdEregMutex[MAX_SIM_COUNT];
        static MD_EGREG *mdEgreg[MAX_SIM_COUNT];
        static pthread_mutex_t mdEgregMutex[MAX_SIM_COUNT];
        static MD_EOPS *mdEops[MAX_SIM_COUNT];
        static pthread_mutex_t mdEopsMutex[MAX_SIM_COUNT];

        static NR_SYS_INFO *nrSysInfo[MAX_SIM_COUNT];
        static pthread_mutex_t nrSysInfoMutex[MAX_SIM_COUNT];
        static bool urc_nrsysinfo_enable;

        static ImsEmergencyInfo *imsEmergencyInfo[MAX_SIM_COUNT];
        static pthread_mutex_t imsEmergencyMutex[MAX_SIM_COUNT];

        static MD_ECELL *mdEcell[MAX_SIM_COUNT];
        static pthread_mutex_t mdEcellMutex[MAX_SIM_COUNT];

        static int nr_display_rule[MAX_SIM_COUNT];
        static int nwInitDone;
        static LTE_ENDC_MAX_CAPABILITY lteMaxNr[MAX_LTE_ENDC_MAX_CAPABILITY];
        static pthread_mutex_t nrCacheMutex;
        static int current_5gopt_config[MAX_SIM_COUNT];

        // if last physical config urc has nr, it's 1, otherwise it's 0
        static int last_physical_config_has_nr[MAX_SIM_COUNT];

        static int lastReportedSignal[MAX_SIM_COUNT];

        static int lte_tdd_connection_state[MAX_SIM_COUNT];
        static int lte_fdd_connection_state[MAX_SIM_COUNT];
        static pthread_mutex_t lte_connection_state_Mutex[MAX_SIM_COUNT];

        static int nr_connection_state[MAX_SIM_COUNT];
        static pthread_mutex_t nr_connection_state_Mutex[MAX_SIM_COUNT];

        static int scg_connection_state[MAX_SIM_COUNT];
        static int pre_scg_connection_state[MAX_SIM_COUNT];
        static pthread_mutex_t scg_connection_state_Mutex[MAX_SIM_COUNT];

        static bool registrationFailNotificationSwitch[MAX_SIM_COUNT];

        static int jp_mode_timer1_state[MAX_SIM_COUNT];
        static int jp_mode_timer1_delay_timer[MAX_SIM_COUNT];
        static pthread_mutex_t jp_mode_timer1_mutex[MAX_SIM_COUNT];
        static int jp_mode_timer1_delay_id[MAX_SIM_COUNT];
        static int jp_mode_timer2_state[MAX_SIM_COUNT];
        static int jp_mode_timer2_delay_timer[MAX_SIM_COUNT];
        static pthread_mutex_t jp_mode_timer2_mutex[MAX_SIM_COUNT];
        static int jp_mode_timer2_delay_id[MAX_SIM_COUNT];

        static int endc_capability[MAX_SIM_COUNT];
        static int lteBand[MAX_SIM_COUNT];
        static int nrBand[MAX_SIM_COUNT];

        // if it is true, the rrc urc won't be off when screen off
        static bool keep_rrc_urc;

        //network scan full support
        static bool s_enwscn_support;

        static bool m_csgListOngoing[MAX_SIM_COUNT];
        static bool m_csgListAbort[MAX_SIM_COUNT];

        static OEM_CA_INFO *oem_ca_info_cache[MAX_SIM_COUNT];
        static int support_dmftc02[MAX_SIM_COUNT];

    protected:
        static pthread_mutex_t ril_nw_nitzName_mutex[MAX_SIM_COUNT];
        static char m_ril_nw_nitz_oper_code[MAX_SIM_COUNT][MAX_OPER_NAME_LENGTH];
        static char m_ril_nw_nitz_oper_lname[MAX_SIM_COUNT][MAX_OPER_NAME_LENGTH];
        static char m_ril_nw_nitz_oper_sname[MAX_SIM_COUNT][MAX_OPER_NAME_LENGTH];
        // Support 5G
        static int nr_support;

        static int mSwitchRat;

        static bool at_eplwss_support;
        static bool urc_enrcaband_enable;
        static bool urc_echinfo_enable;

        static bool s_iwlan_in_legacy_mode;

        static bool s_en3epcreg_support;
        static int s_iwlan_reg_status[MAX_SIM_COUNT];
        static pthread_mutex_t s_iwlanRegStatusMutex[MAX_SIM_COUNT];

        static bool physicalConfigSwitch[MAX_SIM_COUNT];
        static RIL_PhysicalChannelConfig sPhyChlCfgNotifiedCache[MAX_SIM_COUNT][MAX_PHY_CHL_CFG_COUNT];
        static uint8_t sPhyChlCfgNotifiedCount[MAX_SIM_COUNT];
        static pthread_mutex_t sPhysicalConfigMutex[MAX_SIM_COUNT];
        static RIL_PhysicalChannelConfig sPhysicalConfigUrcCache[MAX_SIM_COUNT][MAX_PHY_CHL_CFG_COUNT];
        static uint8_t sPhysicalConfigUrcCount[MAX_SIM_COUNT];
        static pthread_mutex_t sUrcPhysicalConfigMutex[MAX_SIM_COUNT];

    private:
        static LteSignalProperty mCurrentLteSignal[MAX_SIM_COUNT];
        static bool nwHandlerInit[MAX_SIM_COUNT];
        static pthread_mutex_t ril_handler_init_mutex[MAX_SIM_COUNT];
};

#endif
