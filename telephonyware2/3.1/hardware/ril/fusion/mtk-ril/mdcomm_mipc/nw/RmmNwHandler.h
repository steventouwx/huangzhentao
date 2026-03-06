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

#ifndef __RMM_NW_HANDLER_H__
#define __RMM_NW_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
//#include "ViaBaseHandler.h"

#define RETRY_NOTIFY_SYSTEM_TIMER   (3)
#define MAX_OPER_NAME_LENGTH        (50)
#define MAX_TOE_BUFFER_LEN          (MAX_OPER_NAME_LENGTH*2 + 20)

#define PROPERTY_NR_SLOTID      "persist.vendor.radio.nrslot"

// Use int max, as -1 is a valid value in signal strength
#define CELLINFO_INVALID 0x7FFFFFFF
#define PROPERTY_NITZ_OPER_CODE     "persist.vendor.radio.nitz_oper_code"
#define PROPERTY_NITZ_OPER_LNAME    "persist.vendor.radio.nitz_oper_lname"
#define PROPERTY_NITZ_OPER_SNAME    "persist.vendor.radio.nitz_oper_sname"

#define PROPERTY_AUTO_RESTORE_RPLMN "persist.vendor.radio.disable_auto_return_rplmn"

#define NR_DISPLAY_NONE                0x00
#define NR_DISPALY_NR_CACHE            0x01
#define NR_DISPALY_LTE_CONNECTED_STATE 0x02 // deprecated, use JP mode instead
#define NR_DISPLAY_JP_MODE             0x04

#define LIFE_VALID_PLMN 30000000000 // 30 seconds

typedef enum {
    MATCH_NONE  = 0x00,
    MATCH_EONS  = 0x01,
    MATCH_NITZ  = 0x02,
    MATCH_TS25  = 0x04,
    MATCH_ALL   = 0x07
} NameMatchRule;

typedef struct {
    char mccMnc[8];
    char longName[MAX_OPER_NAME_LENGTH];
    char shortName[MAX_OPER_NAME_LENGTH];
} RIL_OPERATOR_INFO_CACHE;

typedef struct {
    int upperlayerIndR15Available; //0 or 1
    int nrSubcarrierSpacingKHz;
} NR_SYS_INFO;

typedef struct {
    int registration_state;
    unsigned int lac;
    uint64_t cid;
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
    int cause_type;
    int reject_cause;
} RIL_CS_REG_STATE_CACHE;

typedef struct {
    int registration_state;
    unsigned int lac;
    uint64_t cid;
    int radio_technology;
    int denied_reason;
    int max_simultaneous_data_call;
    // NR none-standalone
    int dcnr_restricted;
    int endc_sib;
    int endc_available;
    //int cause_type;
    int reject_cause;
} RIL_PS_REG_STATE_CACHE;

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

typedef struct {
    int act;
    uint64_t cid;
    uint32_t lacTac;
} RIL_CELL_INFO_CACHE;

typedef struct {
    int keyGsmCount;
    int keyGsmList;
    int keyCdmaCount;
    int keyCdmaList;
    int keyUmtsCount;
    int keyUmtsList;
    int keyLteCount;
    int keyLteList;
    int keyNrCount;
    int keyNrList;
    int keyLteListV1;
    int keyNrListV1;
}RIL_CELl_INFO_TLV_KEYS;

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
    char optr_name[MAX_OPER_NAME_LENGTH];
    int cause;
    int is_lte_femtocell;  //varizon
} RIL_FEMTO_CELL_CACHE;

typedef struct {
    int volteEmcSupported;
    int vonrEmcSupported;
    int vonrEmfSupported;
} ImsEmergencyInfo;

typedef struct {
    int pci;
    int arfcn;
    int band;
    int bandwidth;
} CellIdentityInfo;

typedef struct {
    int rsrp;
    int rssnr;
    bool initialed;
} LteSignalProperty;

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

class RmmNwHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmNwHandler);

    public:
        RmmNwHandler(int slot_id, int channel_id);
        virtual ~RmmNwHandler();
        char* getMask(int i);
        char* getMask(unsigned int i);
        char* getMask(long long i);
        char* getMask(unsigned long long i);
        bool isGcfTestMode();
        bool isTestSim(int slotId);
        bool isAPInCall();

        int matchPlmnName(char *longAlpha, char *shortAlpha, const char* numeric);
        int matchPlmnName(char *longAlpha, char *shortAlpha, const char* numeric, int lac, int rat);
        int matchPlmnName(char *longAlpha, char *shortAlpha, const char* numeric, int lac, int rat, int matchRule);

    protected:
        void onHandleRetryNotifySystem(const sp<RfxMclMessage>& msg);
        void isNeedNotifyStateChanged();
        void fillVoiceCellIdentity(RIL_CellIdentity_v16 &cellIdentity);
        void fillDataCellIdentity(RIL_CellIdentity_v16 &cellIdentity);
        void clearCellIdentity(RIL_CellIdentity_v16 &cellIdentity);
        void resetSignalStrengthCache(RIL_SIGNAL_STRENGTH *sigCache, RIL_SIGNAL_GROUP group);
        void fillSignalStrengthResponse(RIL_SIGNAL_STRENGTH *response, int group);
        int getCurrentSignalGroup();
        int getSignalGroupByRegRat(int csRegState, int csRat, int psRegState, int psRat);
        void printSignalStrengthCache(char *source);
        int getLastReportedSignal();
        int getCellInfoListV12(RfxMipcData* mipcData,    RIL_CellInfo_v12 *response,
            RIL_CELl_INFO_TLV_KEYS tlvKeys);
        bool refreshCellInfoList(bool forceRefresh = false);
        void freeOperatorNameOfCellInfo(int num, RIL_CellInfo_v12 *response);
        void dumpPrimaryCellInfoCache(const char* fun, RIL_CELL_INFO_CACHE *pCell);
        int transferPlmnToMccMnc(char* plmn, int *mcc, int *mnc, int * mncLen);
        void handleVoidResponse(const sp<RfxMclMessage>& msg);
        void dumpCsRegStateCache(const char* fun, const RIL_CS_REG_STATE_CACHE* pCache, bool debugV = false);
        void dumpPsRegStateCache(const char* fun, const RIL_PS_REG_STATE_CACHE* pCache, bool debugV = false);
        void requestSignalStrength(const sp<RfxMclMessage>& msg, bool forceUpdate);
        void loadDisplayRuleByOp();
        void setNrDisplayRuleToMd();
        void setLastValidPlmn(const char* plmn);
        bool getLastValidPlmn(char* plmn, int size);
        bool getCdmaPlmn(char* plmn, int size);
        void updateAutoRestoreRplmnStatus(int slotId);
        void updateNrCaBandInfo();
        bool isNetworkScanFullSupport();
        void releaseNetworkScanResultData(RIL_NetworkScanResult* data);
        void queryPhysicalChannelConfigs();
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        bool skipDCNRRestrictedState(const char *mccmnc);
        bool fillPsCache(RIL_PS_REG_STATE_CACHE *&pPsCache, int &psRegState);
        bool fillCsCache(sp<RfxMipcData>& cnfData,
                         bool& rat_4g,
                         bool& rat_5g,
                         bool& rat_c2k,
                         int& csRegState,
                         int& psRegState);
#endif //MTK_HYP_DUAL_TELE_SUPPORT
        static void freeSetNull(char* a) {
            if (a) free(a);
            a = NULL;
        }
        static bool isInService(int regState);
        static unsigned int convertCSNetworkType(unsigned int uiResponse);
        static unsigned int convertPSNetworkType(unsigned int uiResponse);
        static unsigned int convertRegState(unsigned int uiRegState, bool isVoiceState);
        static RIL_CellInfoType getCellInfoTypeRadioTechnology(int rat);
        static bool enableReportSignalStrengthWithWcdmaEcio();
        static bool isRilTestMode();
        static int isFemtocellSupport();
        static int isEnableModulationReport();
        static bool isMatchOpid(const char* opid);
        int fillBarringInfosResponse(size_t count,
                RIL_BarringInfo* barringInfos, RIL_BarringInforResponse* barringInfoResp);

        static RIL_OPERATOR_INFO_CACHE* sOpInfoCache[MAX_SIM_COUNT];
        static pthread_mutex_t sOpInfoMutex[MAX_SIM_COUNT];
        static RIL_CS_REG_STATE_CACHE* sCsRegStateCache[MAX_SIM_COUNT];
        static pthread_mutex_t sCsRegStateMutex[MAX_SIM_COUNT];
        static RIL_PS_REG_STATE_CACHE* sPsRegStateCache[MAX_SIM_COUNT];
        static pthread_mutex_t sPsRegStateMutex[MAX_SIM_COUNT];
        static bool sNwSelMode[MAX_SIM_COUNT];
        static pthread_mutex_t sNwSelModeMutex[MAX_SIM_COUNT];
        static ImsEmergencyInfo *sImsEmergencyInfo[MAX_SIM_COUNT];
        static pthread_mutex_t sImsEmergencyMutex[MAX_SIM_COUNT];
        static NR_SYS_INFO *sNrSysInfo[MAX_SIM_COUNT];
        static pthread_mutex_t sNrSysInfoMutex[MAX_SIM_COUNT];
        static bool sUrcNrsysinfoEnable;
        static bool sUrcChInfoEnable;
        static bool sUrcNrCaBandEnable;
        static bool sUrcNwCfgInfoEnable;

        static pthread_mutex_t sNeedNotifyStateMutex[MAX_SIM_COUNT];
        static int sIsNeedNotifyStateId[MAX_SIM_COUNT];
        static bool sIsNeedNotifyState[MAX_SIM_COUNT];
        static bool sImsEccState[MAX_SIM_COUNT];
        static bool sRegFailNotificationSwitch[MAX_SIM_COUNT];
        static RIL_CELL_INFO_CACHE* sPrimaryCellInfoCache[MAX_SIM_COUNT];

        static bool sPhysicalConfigSwitch[MAX_SIM_COUNT];
        static RIL_PhysicalChannelConfig sPhysicalConfigCache[MAX_SIM_COUNT][MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM];
        static uint16_t sPhysicalConfigCount[MAX_SIM_COUNT];
        static RIL_PhysicalChannelConfig sPhysicalConfigNotified[MAX_SIM_COUNT][MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM];
        static uint16_t sPhysicalConfigCountNotified[MAX_SIM_COUNT];
        static pthread_mutex_t sPhysicalConfigMutex[MAX_SIM_COUNT];

        //network scan full support
        static bool s_enwscn_support;

        static pthread_mutex_t s_signalStrengthMutex[MAX_SIM_COUNT];
        static RIL_SIGNAL_STRENGTH *signal_strength_cache[MAX_SIM_COUNT];
        static int lastReportedSignal[MAX_SIM_COUNT];
        static int sPlmnListOngoingType;
        static bool sPlmnListAbort;
        static RIL_FEMTO_CELL_CACHE* sFemtocellCache[MAX_SIM_COUNT];
        static pthread_mutex_t sFemtocellMutex[MAX_SIM_COUNT];
        static int nr_display_rule[MAX_SIM_COUNT];
        static int jp_mode_timer1_delay_timer[MAX_SIM_COUNT];
        static int jp_mode_timer2_delay_timer[MAX_SIM_COUNT];
        static int scg_connection_state[MAX_SIM_COUNT];
        static char last_valid_plmn[MAX_SIM_COUNT][8];
        static nsecs_t last_valid_plmn_time[MAX_SIM_COUNT];
        static bool s_iwlan_in_legacy_mode;
        static int s_iwlan_reg_status[MAX_SIM_COUNT];
        static pthread_mutex_t s_iwlanRegStatusMutex[MAX_SIM_COUNT];
        static int nr_connection_state[MAX_SIM_COUNT];
        static pthread_mutex_t nr_connection_state_Mutex[MAX_SIM_COUNT];
        static RIL_NwCfgInfo *nw_cfg_cache[MAX_SIM_COUNT];
        static pthread_mutex_t s_nwCfgMutex[MAX_SIM_COUNT];

    private:
        int convert3GRssiValue(int rscp_in_dbm);
        void queryPrimaryCellIdentityInfo(CellIdentityInfo* pOut);
        static bool sHandlerInit[MAX_SIM_COUNT];
        static LteSignalProperty mCurrentLteSignal[MAX_SIM_COUNT];
};

#endif
