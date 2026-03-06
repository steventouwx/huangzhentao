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

#include "RmcNetworkHandler.h"
#include <compiler/compiler_utils.h>
#include <math.h>                          /* log10 */
#include "RfxViaUtils.h"
#include "RfxCellInfoData.h"
#include "ViaBaseHandler.h"
#include <libmtkrilutils.h>
#include "utils/Timers.h"

char const *sOp12Plmn[] = {
    "311278", "311483", "310004", "311283", "311488",
    "310890", "311272", "311288", "311277", "311482",
    "311282", "311487", "310590", "311287", "311271",
    "311276", "311481", "311281", "311486", "310013",
    "311286", "311270", "311275", "311480", "311280",
    "311485", "310012", "311285", "311110", "311274",
    "311390", "311279", "311484", "310010", "311284",
    "311489", "310910", "311273", "311289"
};

static const char PROPERTY_RIL_TEST_SIM[4][35] = {
    "vendor.gsm.sim.ril.testsim",
    "vendor.gsm.sim.ril.testsim.2",
    "vendor.gsm.sim.ril.testsim.3",
    "vendor.gsm.sim.ril.testsim.4",
};

const static SpnTable s_mtk_ts25_table[] = {
    #include "mtk_ts25_table.h"
};

const static SpnTable s_mtk_plmn_table[] = {
    #include "mtk_plmn_table.h"
};

int RmcNetworkHandler::ECELLext3ext4Support = 1;
pthread_mutex_t RmcNetworkHandler::ril_handler_init_mutex[MAX_SIM_COUNT];
bool RmcNetworkHandler::nwHandlerInit[MAX_SIM_COUNT] = {false};
pthread_mutex_t RmcNetworkHandler::ril_nw_femtoCell_mutex;
RIL_FEMTO_CELL_CACHE* RmcNetworkHandler::femto_cell_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_signalStrengthMutex[MAX_SIM_COUNT];
RIL_SIGNAL_STRENGTH* RmcNetworkHandler::signal_strength_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_voiceRegStateMutex[MAX_SIM_COUNT];
RIL_VOICE_REG_STATE_CACHE* RmcNetworkHandler::voice_reg_state_cache[MAX_SIM_COUNT];
RIL_DATA_REG_STATE_CACHE* RmcNetworkHandler::data_reg_state_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_urc_voiceRegStateMutex[MAX_SIM_COUNT];
RIL_VOICE_REG_STATE_CACHE* RmcNetworkHandler::urc_voice_reg_state_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_urc_dataRegStateMutex[MAX_SIM_COUNT];
RIL_DATA_REG_STATE_CACHE* RmcNetworkHandler::urc_data_reg_state_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_caMutex[MAX_SIM_COUNT];
RIL_CA_CACHE* RmcNetworkHandler::ca_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_nrCaMutex[MAX_SIM_COUNT];
RIL_NR_CA_CACHE* RmcNetworkHandler::nr_ca_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_nwCfgMutex[MAX_SIM_COUNT];
RIL_NwCfgInfo* RmcNetworkHandler::nw_cfg_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::s_wfcRegStatusMutex[MAX_SIM_COUNT];
RIL_OPERATOR_INFO_CACHE* RmcNetworkHandler::op_info_cache[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::op_info_Mutex[MAX_SIM_COUNT];
int RmcNetworkHandler::ril_wfc_reg_status[MAX_SIM_COUNT];
bool RmcNetworkHandler::mIsNeedNotifyState[MAX_SIM_COUNT];
int RmcNetworkHandler::mIsNeedNotifyState_Id[MAX_SIM_COUNT];
RIL_EonsNetworkFeatureInfo RmcNetworkHandler::eons_info[MAX_SIM_COUNT];
int RmcNetworkHandler::sPlmnListOngoingType = NW_PLMN_LIST_TYPE_NONE;
int RmcNetworkHandler::mPlmnListAbort = 0;
LteSignalProperty RmcNetworkHandler::mCurrentLteSignal[MAX_SIM_COUNT];
bool RmcNetworkHandler::physicalConfigSwitch[MAX_SIM_COUNT];
RIL_PhysicalChannelConfig RmcNetworkHandler::sPhyChlCfgNotifiedCache[MAX_SIM_COUNT][MAX_PHY_CHL_CFG_COUNT];
uint8_t RmcNetworkHandler::sPhyChlCfgNotifiedCount[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::sPhysicalConfigMutex[MAX_SIM_COUNT];
RIL_PhysicalChannelConfig RmcNetworkHandler::sPhysicalConfigUrcCache[MAX_SIM_COUNT][MAX_PHY_CHL_CFG_COUNT];
uint8_t RmcNetworkHandler::sPhysicalConfigUrcCount[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::sUrcPhysicalConfigMutex[MAX_SIM_COUNT];

pthread_mutex_t RmcNetworkHandler::ril_nw_nitzName_mutex[MAX_SIM_COUNT];
char RmcNetworkHandler::m_ril_nw_nitz_oper_code[MAX_SIM_COUNT][MAX_OPER_NAME_LENGTH];
char RmcNetworkHandler::m_ril_nw_nitz_oper_lname[MAX_SIM_COUNT][MAX_OPER_NAME_LENGTH];
char RmcNetworkHandler::m_ril_nw_nitz_oper_sname[MAX_SIM_COUNT][MAX_OPER_NAME_LENGTH];

char RmcNetworkHandler::last_valid_plmn[MAX_SIM_COUNT][8];
nsecs_t RmcNetworkHandler::last_valid_plmn_time[MAX_SIM_COUNT];

SPNTABLE RmcNetworkHandler::cache_spn_table[MAX_SIM_COUNT];

bool RmcNetworkHandler::m_dc_support[MAX_SIM_COUNT];
bool RmcNetworkHandler::ims_ecc_state[MAX_SIM_COUNT];
OEM_CA_INFO* RmcNetworkHandler::oem_ca_info_cache[MAX_SIM_COUNT];
int RmcNetworkHandler::support_dmftc02[MAX_SIM_COUNT];

int RmcNetworkHandler::nr_support;
int RmcNetworkHandler::data_context_ids[MAX_SIM_COUNT][16];

MD_EREG* RmcNetworkHandler::mdEreg[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::mdEregMutex[MAX_SIM_COUNT];
MD_EGREG* RmcNetworkHandler::mdEgreg[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::mdEgregMutex[MAX_SIM_COUNT];
MD_EOPS* RmcNetworkHandler::mdEops[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::mdEopsMutex[MAX_SIM_COUNT];

NR_SYS_INFO *RmcNetworkHandler::nrSysInfo[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::nrSysInfoMutex[MAX_SIM_COUNT];
bool RmcNetworkHandler::urc_nrsysinfo_enable;

ImsEmergencyInfo *RmcNetworkHandler::imsEmergencyInfo[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::imsEmergencyMutex[MAX_SIM_COUNT];

MD_ECELL* RmcNetworkHandler::mdEcell[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::mdEcellMutex[MAX_SIM_COUNT];

int RmcNetworkHandler::mSwitchRat;

int RmcNetworkHandler::nwInitDone = 0;
int RmcNetworkHandler::nr_display_rule[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::nrCacheMutex = PTHREAD_MUTEX_INITIALIZER;
LTE_ENDC_MAX_CAPABILITY RmcNetworkHandler::lteMaxNr[MAX_LTE_ENDC_MAX_CAPABILITY];
int RmcNetworkHandler::current_5gopt_config[MAX_SIM_COUNT];

int RmcNetworkHandler::last_physical_config_has_nr[MAX_SIM_COUNT];
int RmcNetworkHandler::lastReportedSignal[MAX_SIM_COUNT];

int RmcNetworkHandler::lte_tdd_connection_state[MAX_SIM_COUNT];
int RmcNetworkHandler::lte_fdd_connection_state[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::lte_connection_state_Mutex[MAX_SIM_COUNT];

int RmcNetworkHandler::nr_connection_state[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::nr_connection_state_Mutex[MAX_SIM_COUNT];

int RmcNetworkHandler::scg_connection_state[MAX_SIM_COUNT];
int RmcNetworkHandler::pre_scg_connection_state[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::scg_connection_state_Mutex[MAX_SIM_COUNT];

bool RmcNetworkHandler::registrationFailNotificationSwitch[MAX_SIM_COUNT];

int RmcNetworkHandler::jp_mode_timer1_state[MAX_SIM_COUNT];
int RmcNetworkHandler::jp_mode_timer1_delay_timer[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::jp_mode_timer1_mutex[MAX_SIM_COUNT];
int RmcNetworkHandler::jp_mode_timer1_delay_id[MAX_SIM_COUNT];
int RmcNetworkHandler::jp_mode_timer2_state[MAX_SIM_COUNT];
int RmcNetworkHandler::jp_mode_timer2_delay_timer[MAX_SIM_COUNT];
pthread_mutex_t RmcNetworkHandler::jp_mode_timer2_mutex[MAX_SIM_COUNT];
int RmcNetworkHandler::jp_mode_timer2_delay_id[MAX_SIM_COUNT];

int RmcNetworkHandler::endc_capability[MAX_SIM_COUNT];
int RmcNetworkHandler::lteBand[MAX_SIM_COUNT];
int RmcNetworkHandler::nrBand[MAX_SIM_COUNT];

bool RmcNetworkHandler::at_eplwss_support;
bool RmcNetworkHandler::urc_enrcaband_enable;
bool RmcNetworkHandler::urc_echinfo_enable;
bool RmcNetworkHandler::s_iwlan_in_legacy_mode;
bool RmcNetworkHandler::s_en3epcreg_support = false;
int RmcNetworkHandler::s_iwlan_reg_status[MAX_SIM_COUNT] = { 0 };
pthread_mutex_t RmcNetworkHandler::s_iwlanRegStatusMutex[MAX_SIM_COUNT];

bool RmcNetworkHandler::keep_rrc_urc;
bool RmcNetworkHandler::s_enwscn_support = false;

bool RmcNetworkHandler::m_csgListOngoing[MAX_SIM_COUNT];
bool RmcNetworkHandler::m_csgListAbort[MAX_SIM_COUNT];


RmcNetworkHandler::RmcNetworkHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {

    pthread_mutex_lock(&ril_handler_init_mutex[m_slot_id]);
    logV(LOG_TAG, "RmcNetworkHandler[%d] start", m_slot_id);
    if (nwHandlerInit[m_slot_id] == false) {
        nwHandlerInit[m_slot_id] = true;
        // request cache
        voice_reg_state_cache[m_slot_id] = (RIL_VOICE_REG_STATE_CACHE*)calloc(1, sizeof(RIL_VOICE_REG_STATE_CACHE));
        data_reg_state_cache[m_slot_id] = (RIL_DATA_REG_STATE_CACHE*)calloc(1, sizeof(RIL_DATA_REG_STATE_CACHE));
        signal_strength_cache[m_slot_id] = (RIL_SIGNAL_STRENGTH*)calloc(1, sizeof(RIL_SIGNAL_STRENGTH));
        // urc cache
        urc_voice_reg_state_cache[m_slot_id] = (RIL_VOICE_REG_STATE_CACHE*)calloc(1, sizeof(RIL_VOICE_REG_STATE_CACHE));
        urc_data_reg_state_cache[m_slot_id] = (RIL_DATA_REG_STATE_CACHE*)calloc(1, sizeof(RIL_DATA_REG_STATE_CACHE));
        // nw info cache
        op_info_cache[m_slot_id] = (RIL_OPERATOR_INFO_CACHE*)calloc(1, sizeof(RIL_OPERATOR_INFO_CACHE));
        pthread_mutex_init(&op_info_Mutex[m_slot_id], NULL);

        ca_cache[m_slot_id] = (RIL_CA_CACHE*)calloc(1, sizeof(RIL_CA_CACHE));
        nr_ca_cache[m_slot_id] = (RIL_NR_CA_CACHE*)calloc(1, sizeof(RIL_NR_CA_CACHE));
        nw_cfg_cache[m_slot_id] = (RIL_NwCfgInfo*)calloc(1, sizeof(RIL_NwCfgInfo));
        // femto cell cache
        femto_cell_cache[m_slot_id] = (RIL_FEMTO_CELL_CACHE*)calloc(1, sizeof(RIL_FEMTO_CELL_CACHE));

        pthread_mutex_init(&s_signalStrengthMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_voiceRegStateMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_urc_voiceRegStateMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_urc_dataRegStateMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_wfcRegStatusMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_caMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_nrCaMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_nwCfgMutex[m_slot_id], NULL);
        pthread_mutex_init(&s_iwlanRegStatusMutex[m_slot_id], NULL);
        eons_info[m_slot_id] = {EONS_INFO_NOT_RECEIVED, 0, 0};
        mCurrentLteSignal[m_slot_id].rsrp = 0x7FFFFFFF;
        mCurrentLteSignal[m_slot_id].rssnr = 0x7FFFFFFF;
        mCurrentLteSignal[m_slot_id].initialed = false;
        mIsNeedNotifyState[m_slot_id] = true;
        mIsNeedNotifyState_Id[m_slot_id] = 0;

        memset(last_valid_plmn[m_slot_id], 0, 8);
        last_valid_plmn_time[m_slot_id] = -1;

        m_dc_support[m_slot_id] = false;

        for (int i = 0; i < 16; i++) data_context_ids[m_slot_id][i] = -1;

        ims_ecc_state[m_slot_id] = false;

        // AT cache
        mdEreg[m_slot_id] = (MD_EREG*) calloc(1, sizeof(MD_EREG));
        memset(mdEreg[m_slot_id], 0, sizeof(MD_EREG));
        pthread_mutex_init(&mdEregMutex[m_slot_id], NULL);
        mdEgreg[m_slot_id] = (MD_EGREG*) calloc(1, sizeof(MD_EGREG));
        memset(mdEgreg[m_slot_id], 0, sizeof(MD_EGREG));
        pthread_mutex_init(&mdEgregMutex[m_slot_id], NULL);
        mdEops[m_slot_id] = (MD_EOPS*) calloc(1, sizeof(MD_EOPS));
        memset(mdEops[m_slot_id], 0, sizeof(MD_EOPS));
        pthread_mutex_init(&mdEopsMutex[m_slot_id], NULL);

        nrSysInfo[m_slot_id] = (NR_SYS_INFO*) calloc(1, sizeof(NR_SYS_INFO));
        pthread_mutex_init(&nrSysInfoMutex[m_slot_id], NULL);

        imsEmergencyInfo[m_slot_id] = (ImsEmergencyInfo *)calloc(1, sizeof(ImsEmergencyInfo));
        pthread_mutex_init(&imsEmergencyMutex[m_slot_id], NULL);

        mdEcell[m_slot_id] = (MD_ECELL*) calloc(1, sizeof(MD_ECELL));
        memset(mdEcell[m_slot_id], 0, sizeof(MD_ECELL));
        pthread_mutex_init(&mdEcellMutex[m_slot_id], NULL);

        current_5gopt_config[m_slot_id] = 0;

        last_physical_config_has_nr[m_slot_id] = 0;

        lastReportedSignal[m_slot_id] = 0;

        lte_tdd_connection_state[m_slot_id] = 0;
        lte_fdd_connection_state[m_slot_id] = 0;
        pthread_mutex_init(&lte_connection_state_Mutex[m_slot_id], NULL);

        nr_connection_state[m_slot_id] = 0;
        pthread_mutex_init(&nr_connection_state_Mutex[m_slot_id], NULL);

        scg_connection_state[m_slot_id] = 0;
        pre_scg_connection_state[m_slot_id] = 0;
        pthread_mutex_init(&scg_connection_state_Mutex[m_slot_id], NULL);

        registrationFailNotificationSwitch[m_slot_id] = true;

        // JP mode
        jp_mode_timer1_state[m_slot_id] = JP_MODE_TIMER_STATE_STOP;
        jp_mode_timer1_delay_timer[m_slot_id] = 20;
        pthread_mutex_init(&jp_mode_timer1_mutex[m_slot_id], NULL);
        jp_mode_timer1_delay_id[m_slot_id] = 0;
        jp_mode_timer2_state[m_slot_id] = JP_MODE_TIMER_STATE_STOP;
        jp_mode_timer2_delay_timer[m_slot_id] = 20;
        pthread_mutex_init(&jp_mode_timer2_mutex[m_slot_id], NULL);
        jp_mode_timer2_delay_id[m_slot_id] = 0;

        // default is 1 as (NR_DISPALY_NR_CACHE)
        // idle: cache cell's nr capability
        nr_display_rule[m_slot_id] = getDefault5GNsaDisplayRule();
        loadDisplayRuleByOp();
        logD(LOG_TAG, "Default nr_display_rule=%d, "
            "jp_mode_timer1_delay_timer=%d, "
            "jp_mode_timer2_delay_timer=%d",
            nr_display_rule[m_slot_id],
            jp_mode_timer1_delay_timer[m_slot_id],
            jp_mode_timer2_delay_timer[m_slot_id]);

        char prop[RFX_PROPERTY_VALUE_MAX] = {0};
        char prop_default[RFX_PROPERTY_VALUE_MAX] = {0};
        int prop_nr_display_rule = 0;
        int prop_nr_display_jp_mode_timer1_delay_timer = 0;
        int prop_nr_display_jp_mode_timer2_delay_timer = 0;

        String8 nrDisplayRule("persist.vendor.radio.nr_display_rule");
        rfx_property_get(nrDisplayRule.string(), prop, "-1");
        if (m_slot_id != 0) {
            memset(prop_default, 0, sizeof(prop_default));
            strncpy(prop_default, prop, RFX_PROPERTY_VALUE_MAX-1);
            nrDisplayRule.append(String8::format(".%d", (m_slot_id + 1)));
            rfx_property_get(nrDisplayRule.string(), prop, prop_default);
        }
        prop_nr_display_rule = atoi(prop);
        if (prop_nr_display_rule != -1) {
            nr_display_rule[m_slot_id] = prop_nr_display_rule;
        }

        String8 jpTime1("persist.vendor.radio.jp_mode_timer1_delay_timer");
        rfx_property_get(jpTime1.string(), prop, "-1");
        if (m_slot_id != 0) {
            memset(prop_default, 0, sizeof(prop_default));
            strncpy(prop_default, prop, RFX_PROPERTY_VALUE_MAX-1);
            jpTime1.append(String8::format(".%d", (m_slot_id + 1)));
            rfx_property_get(jpTime1.string(), prop, prop_default);
        }
        prop_nr_display_jp_mode_timer1_delay_timer = atoi(prop);
        if (prop_nr_display_jp_mode_timer1_delay_timer != -1) {
            jp_mode_timer1_delay_timer[m_slot_id] =
                prop_nr_display_jp_mode_timer1_delay_timer;
        }

        String8 jpTime2("persist.vendor.radio.jp_mode_timer2_delay_timer");
        rfx_property_get(jpTime2.string(), prop, "-1");
        if (m_slot_id != 0) {
            memset(prop_default, 0, sizeof(prop_default));
            strncpy(prop_default, prop, RFX_PROPERTY_VALUE_MAX-1);
            jpTime2.append(String8::format(".%d", (m_slot_id + 1)));
            rfx_property_get(jpTime2.string(), prop, prop_default);
        }
        prop_nr_display_jp_mode_timer2_delay_timer = atoi(prop);
        if (prop_nr_display_jp_mode_timer2_delay_timer != -1) {
            jp_mode_timer2_delay_timer[m_slot_id] =
                prop_nr_display_jp_mode_timer2_delay_timer;
        }

        logD(LOG_TAG, "prop nr_display_rule=%d, "
            "jp_mode_timer1_delay_timer=%d, "
            "jp_mode_timer2_delay_timer=%d",
            prop_nr_display_rule,
            prop_nr_display_jp_mode_timer1_delay_timer,
            prop_nr_display_jp_mode_timer2_delay_timer);

        if (((nr_display_rule[m_slot_id] & NR_DISPLAY_JP_MODE) == NR_DISPLAY_JP_MODE) &&
            ((nr_display_rule[m_slot_id] & (NR_DISPALY_NR_CACHE | NR_DISPALY_LTE_CONNECTED_STATE)) != 0)) {
            logE(LOG_TAG, "JP MODE is enabled, not allow other display settings");
            nr_display_rule[m_slot_id] = NR_DISPLAY_JP_MODE;
        }

        endc_capability[m_slot_id] = 1; // enable by default
        keep_rrc_urc = false; // true: enable cscon urc by default when screen is off
        oem_ca_info_cache[m_slot_id] = NULL;

        support_dmftc02[m_slot_id] = 0;
        lteBand[m_slot_id] = 0;
        nrBand[m_slot_id] = 0;

        if (m_slot_id == 0) {
            rfx_property_get("ro.telephony.iwlan_operation_mode", prop,  "default");
            s_iwlan_in_legacy_mode = !strcmp("legacy", prop);
            logV(LOG_TAG, "iwlan_operation_mode:%s, s_iwlan_in_legacy_mode:%d",
                 prop, s_iwlan_in_legacy_mode);
        }

        m_csgListOngoing[m_slot_id] = false;
        m_csgListAbort[m_slot_id] = false;

        rfx_property_get(String8::format("%s_%d", PROPERTY_NITZ_OPER_CODE, m_slot_id).string(),
            m_ril_nw_nitz_oper_code[m_slot_id], (char*) "");
        rfx_property_get(String8::format("%s_%d", PROPERTY_NITZ_OPER_LNAME, m_slot_id).string(),
            m_ril_nw_nitz_oper_lname[m_slot_id], (char*) "");
        rfx_property_get(String8::format("%s_%d", PROPERTY_NITZ_OPER_SNAME, m_slot_id).string(),
            m_ril_nw_nitz_oper_sname[m_slot_id], (char*) "");
        logD(LOG_TAG, "NITZ oper_code:%s, nitz_lName:%s, nitz_sName:%s",
                 m_ril_nw_nitz_oper_code[m_slot_id], m_ril_nw_nitz_oper_lname[m_slot_id], m_ril_nw_nitz_oper_sname[m_slot_id]);
    }

    if (!nwInitDone) {
        memset(lteMaxNr, 0, sizeof(lteMaxNr));
        nwInitDone = 1;
    }

    nr_support = -1;
    at_eplwss_support = false;
    urc_enrcaband_enable = isUrcNrCaBandConfigured();
    urc_echinfo_enable = isUrcChInfoConfigured();
    urc_nrsysinfo_enable = isUrcNrSysInfoConfigured(); //default is off

    physicalConfigSwitch[m_slot_id] = true;
    for (int i = 0; i < MAX_PHY_CHL_CFG_COUNT; i++) {
        memset(&sPhysicalConfigUrcCache[m_slot_id][i], 0, sizeof(RIL_PhysicalChannelConfig));
        memset(&sPhyChlCfgNotifiedCache[m_slot_id][i], 0, sizeof(RIL_PhysicalChannelConfig));
    }
    sPhysicalConfigUrcCount[m_slot_id] = 0;
    sPhyChlCfgNotifiedCount[m_slot_id] = 0;
    pthread_mutex_init(&sPhysicalConfigMutex[m_slot_id], NULL);
    pthread_mutex_init(&sUrcPhysicalConfigMutex[m_slot_id], NULL);

    pthread_mutex_unlock(&ril_handler_init_mutex[m_slot_id]);
}

/* Android framework expect spec 27.007  AT+CSQ <rssi> 0~31 format when handling 3G signal strength.
   So we convert 3G signal to <rssi> in RILD */
int RmcNetworkHandler::convert3GRssiValue(int rscp_in_dbm)
{
    int rssi = 0;
    int INVALID = 0x7FFFFFFF;

    if (rscp_in_dbm == INVALID) {
        return rssi;
    }

    // logD(LOG_TAG, "convert3GRssiValue rscp_in_dbm = %d", rscp_in_dbm);

    rssi = (rscp_in_dbm + 113) / 2;

    if (rssi > 31) {
        rssi = 31;
    } else if (rssi < 0) {
        rssi = 0;
    }

    return rssi;
}


int RmcNetworkHandler::getMdSignalStrengthByAT(
        MD_SIGNAL_STRENGTH *ecsq, RfxAtLine *line) {

    // Gen97 modem
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<rsrq_in_qdbm>,<rsrp_in_qdbm>,<Act>,<sig3>,<serv_band>
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<rsrq_in_qdbm>,<rsrp_in_qdbm>,<256>,<sig3>,<serv_band>
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<rsrq_in_qdbm>,<rsrp_in_qdbm>,<Act_EvDo>,<sig3>,<serv_band>
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<ssRsrq_in_qdb>,<ssRsrp_in_qdbm>
    //                  ,<NR>,<ssSINR_in_qdb>,<serv_band>[,<csiRsrq_in_qdb>,<csiRsrp_in_qdbm>,<csiSinr_in_qdb>]

    int err;

    // go to start position
    line->atTokStart(&err);
    if (err < 0) return -1;

    ecsq->sig1 = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->sig2 = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->rssi_in_qdbm = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->rscp_in_qdbm = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->ecn0_in_qdbm = line->atTokNextint(&err);
    if (err < 0) return -1;

    // for LTE
    ecsq->rsrq_in_qdbm = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->rsrp_in_qdbm = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->act = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->sig3 = line->atTokNextint(&err);
    if (err < 0) return -1;

    ecsq->serv_band = line->atTokNextint(&err);
    if (err < 0) return -1;

    if (ecsq->act == ACT_NR) {
        ecsq->csiRsrq_in_qdb = line->atTokNextint(&err);
        if (err < 0) return -1;
        ecsq->csiRsrp_in_qdbm = line->atTokNextint(&err);
        if (err < 0) return -1;
        ecsq->csiSinr_in_qdb = line->atTokNextint(&err);
        if (err < 0) return -1;
        if (ecsq->serv_band > 0) {
            nrBand[m_slot_id] = ecsq->serv_band;
        }
    }

    if ((ecsq->act == ACT_LTE || ecsq->act == ACT_LTE_CA) &&
            ecsq->serv_band > 0) {
        lteBand[m_slot_id] = ecsq->serv_band;
    }

    return 0;
}

int RmcNetworkHandler::updateSignalStrength(RfxAtLine *line)
{
    int ret = SIGNAL_GROUP_NONE;
    MD_SIGNAL_STRENGTH ecsq;
    RIL_SIGNAL_STRENGTH *signalCache = signal_strength_cache[m_slot_id];
    RIL_SIGNAL_STRENGTH origCache;

    // Gen97 modem
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<rsrq_in_qdbm>,<rsrp_in_qdbm>,<Act>,<sig3>,<serv_band>
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<rsrq_in_qdbm>,<rsrp_in_qdbm>,<256>,<sig3>,<serv_band>
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<rsrq_in_qdbm>,<rsrp_in_qdbm>,<Act_EvDo>,<sig3>,<serv_band>
    // +ECSQ: <sig1>,<sig2>,<rssi_in_qdbm>,<rscp_in_qdbm>,<ecn0_in_qdbm>,<ssRsrq_in_qdb>,<ssRsrp_in_qdbm>
    //                  ,<NR>,<ssSINR_in_qdb>,<serv_band>[,<csiRsrq_in_qdb>,<csiRsrp_in_qdbm>,<csiSinr_in_qdb>]

    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();

    if (getMdSignalStrengthByAT(&ecsq, line) < 0)
        return ret;

    memcpy(&origCache, signalCache, sizeof(*signalCache));

    switch (ecsq.act) {

    case ACT_GPRS:
    case ACT_EDGE:  {

        if (ecsq.sig1 < 0  || ecsq.sig1 > 63) {
            logE(LOG_TAG, "Recevice an invalid <rssi> value from modem");
            return ret;
        }
        // It's normal that <ber> is 99 with GSM sometimes.
        if ((ecsq.sig2 < 0 || ecsq.sig2 > 7) && ecsq.sig2 != 99) {
            logE(LOG_TAG, "Recevice an invalid <ber> value from modem");
            return ret;
        }
        signalCache->gsm_signal_strength = ((ecsq.rssi_in_qdbm >> 2)+113) >> 1;
        signalCache->gsm_bit_error_rate = ecsq.sig2;
        // range check
        if (signalCache->gsm_signal_strength < 0)
            signalCache->gsm_signal_strength = 0;
        else if (signalCache->gsm_signal_strength > 31)
            signalCache->gsm_signal_strength = 31;
        ret = SIGNAL_GROUP_GSM;
        updateSignalStrengthKey(String8("GSM").string(), ecsq.rssi_in_qdbm>>2, ecsq.sig2);
        break;
    }
    case ACT_1XRTT:  {

        if (mViaHandler == NULL)
            logE(LOG_TAG, "ecsq act is ACT_1XRTT but mViaHandler is NULL");
            return ret;

        if (ecsq.sig1 < 0  || ecsq.sig1 > 31) {
            logE(LOG_TAG, "Recevice an invalid <rssi> value from modem");
            return ret;
        }
        if (ecsq.sig2 < -128 || ecsq.sig2 > 0) {
            logE(LOG_TAG, "Recevice an invalid <ec/io> value from modem");
            return ret;
        }
        int cdmaDbm, cdmaEcio;

        cdmaDbm = mViaHandler->convertCdmaEvdoSig(ecsq.sig1, SIGNAL_CDMA_DBM);
        cdmaEcio = mViaHandler->convertCdmaEvdoSig(ecsq.sig2, SIGNAL_CDMA_ECIO);

        signalCache->cdma_dbm = cdmaDbm;
        signalCache->cdma_ecio = cdmaEcio;

        ret = SIGNAL_GROUP_1XRTT;
        updateSignalStrengthKey(String8("CDMA").string(),
                cdmaDbm > 0 ? -cdmaDbm: 255,
                (cdmaEcio >= 0) ? -cdmaEcio: 255);
        break;
    }
    case ACT_HRPD:
    case ACT_EHRPD:  {

        if (mViaHandler == NULL)
            logE(LOG_TAG, "ecsq act is ACT_HRPD or ACT_EHRPD but mViaHandler is NULL");
            return ret;
        if (ecsq.sig1 < 0  || ecsq.sig1 > 31) {
            logE(LOG_TAG, "Recevice an invalid <rssi> value from modem");
            return ret;
        }
        if (ecsq.sig2 < -512 || ecsq.sig2 > 0) {
            // logE(LOG_TAG, "Recevice an invalid <ec/io> value from modem");
            return ret;
        }
        if (ecsq.sig1 == 0 && ecsq.sig2 == -512) {
            // logE(LOG_TAG, "Recevice an invalid <ec/io> value from modem");
            return ret;
        }
        int evdoDbm, evdoEcio;

        evdoDbm = mViaHandler->convertCdmaEvdoSig(ecsq.sig1, SIGNAL_EVDO_DBM);
        evdoEcio = mViaHandler->convertCdmaEvdoSig(ecsq.sig2, SIGNAL_EVDO_ECIO);

        signalCache->evdo_dbm = evdoDbm;
        signalCache->evdo_ecio = evdoEcio;
        double temp = (double)ecsq.sig3 / 512;
        int sinrDb = 100 * log10(temp);
        // logD(LOG_TAG, "sinr:%d, sinr_dB:%f", ecsq.sig3, sinrDb);
        if (sinrDb >= 100) {
            signalCache->evdo_snr = 8;
        } else if (sinrDb >= 70) {
            signalCache->evdo_snr = 7;
        } else if (sinrDb >= 50) {
            signalCache->evdo_snr = 6;
        } else if (sinrDb >= 30) {
            signalCache->evdo_snr = 5;
        } else if (sinrDb >= -20) {
            signalCache->evdo_snr = 4;
        } else if (sinrDb >= -45) {
            signalCache->evdo_snr = 3;
        } else if (sinrDb >= -90) {
            signalCache->evdo_snr = 2;
        } else if (sinrDb > -120) {
            signalCache->evdo_snr = 1;
        } else {
            signalCache->evdo_snr = 0;
        }
        ret = SIGNAL_GROUP_EVDO;
        updateSignalStrengthKey(String8("EVDO").string(),
                (evdoDbm > 0) ? -evdoDbm: 255,
                (evdoEcio >= 0) ? -evdoEcio: 255);
        break;
    }
    case ACT_LTE:
    case ACT_LTE_CA:  {

        if (ecsq.sig1 < 0  || ecsq.sig1 > 34) {
            return ret;
        }
        if (ecsq.sig2 < 0 || ecsq.sig2 > 97) {
            logE(LOG_TAG, "Recevice an invalid <rsrp> value from modem");
            return ret;
        }
        int asu = ecsq.rssi_in_qdbm != 1 ?
            ((ecsq.rssi_in_qdbm >> 2) + 113) >> 1 : 99;
        if (asu < 0) asu = 0;
        else if (asu != 99 && asu > 31) asu = 31;
        signalCache->lte_signal_strength = asu;

        int rsrp = 0 - (ecsq.rsrp_in_qdbm >> 2);
        if (rsrp > 140) rsrp = 140;
        else if (rsrp < 44) rsrp = 44;
        signalCache->lte_rsrp = rsrp;

        int rsrq = 0 - (ecsq.rsrq_in_qdbm >> 2);
        if (rsrq > 20) rsrq = 20;
        else if (rsrq < 3) rsrq = 3;
        signalCache->lte_rsrq = rsrq;

        // from (snr in qdbm) to (snr in 0.1dbm)
        int rssnr = ecsq.sig3;
        if (rssnr != 0x7FFF) {
            rssnr = (rssnr * 10) >> 2;
            if (rssnr > 300) {
                rssnr = 300;
            } else if (rssnr < -200) {
                rssnr = -200;
            }
        }
        signalCache->lte_rssnr = rssnr; // unit: 0.1 dB
        signalCache->lte_cqi = 0;
        signalCache->lte_timing_advance = 0;

        ret = SIGNAL_GROUP_LTE;
        updateSignalStrengthKey(String8("LTE").string(),
                ecsq.rsrp_in_qdbm>>2, ecsq.rsrq_in_qdbm>>2);
        // NSA case
        if (last_physical_config_has_nr[m_slot_id] == 1)
            ret |= SIGNAL_GROUP_NR;
        break;
    }
    case ACT_NR: {

        if (ecsq.rsrp_in_qdbm == 32767)
            signalCache->ssRsrp = CELLINFO_INVALID;
        else {
            signalCache->ssRsrp = 0 - (ecsq.rsrp_in_qdbm >> 2);
            if (signalCache->ssRsrp < 44)
                signalCache->ssRsrp = 44;
            else if (signalCache->ssRsrp > 140)
                signalCache->ssRsrp = 140;
        }
        if (ecsq.rsrq_in_qdbm == 32767)
            signalCache->ssRsrq = CELLINFO_INVALID;
        else {
            signalCache->ssRsrq = 0 - (ecsq.rsrq_in_qdbm >> 2);
            if (signalCache->ssRsrq < 3)
                signalCache->ssRsrq = 3;
            else if (signalCache->ssRsrq > 20)
                signalCache->ssRsrq = 20;
        }
        if (ecsq.sig3 == 32767)
            signalCache->ssSinr = CELLINFO_INVALID;
        else {
            signalCache->ssSinr = (ecsq.sig3 >> 2);  // sig3 = ssSINR_in_qdb
            if (signalCache->ssSinr < -23)
                signalCache->ssSinr = -23;
            else if (signalCache->ssSinr > 40)
                signalCache->ssSinr = 40;
        }
        if (ecsq.csiRsrp_in_qdbm == 32767)
            signalCache->csiRsrp = CELLINFO_INVALID;
        else {
            signalCache->csiRsrp = 0 - (ecsq.csiRsrp_in_qdbm >> 2);
            if (signalCache->csiRsrp < 44)
                signalCache->csiRsrp = 44;
            else if (signalCache->csiRsrp > 140)
                signalCache->csiRsrp = 140;
        }
        if (ecsq.csiRsrq_in_qdb == 32767)
            signalCache->csiRsrq = CELLINFO_INVALID;
        else {
            signalCache->csiRsrq = 0 - (ecsq.csiRsrq_in_qdb >> 2);
            if (signalCache->csiRsrq < 3)
                signalCache->csiRsrq = 3;
            else if (signalCache->csiRsrq > 20)
                signalCache->csiRsrq = 20;
        }
        if (ecsq.csiSinr_in_qdb == 32767)
            signalCache->csiSinr = CELLINFO_INVALID;
        else {
            signalCache->csiSinr = (ecsq.csiSinr_in_qdb >> 2);
            if (signalCache->csiSinr < -23)
                signalCache->csiSinr = -23;
            // TODO: fwk accept 23 as maxium
            else if (signalCache->csiSinr > 23)
                signalCache->csiSinr = 23;
        }

        ret = SIGNAL_GROUP_NR;
        break;
    }
    default:

        if (ecsq.act >= ACT_UMTS  && ecsq.act < ACT_1XRTT) {
            if (ecsq.sig1 < 0  || ecsq.sig1 > 96) {
                logE(LOG_TAG, "Recevice an invalid <rscp> value from modem");
                return ret;
            }
            if (ecsq.sig2 < 0 || ecsq.sig2 > 49) {
                logE(LOG_TAG, "Recevice an invalid <ecn0> value from modem");
                return ret;
            }
            int rscp = 0 - (ecsq.rscp_in_qdbm >> 2);
            if (rscp > 120)
                rscp = 120;
            else if (rscp < 25)
                rscp = 25;

            int rssi = convert3GRssiValue(0 - rscp);
            if (!isTdd3G()) {
                signalCache->wcdma_signal_strength = rssi;
                signalCache->wcdma_scdma_rscp= ecsq.sig1;
                signalCache->wcdma_ecno = ecsq.sig2;
                ret = SIGNAL_GROUP_UMTS;
            } else {
                signalCache->tdscdma_signal_strength = rssi;
                signalCache->tdscdma_rscp= ecsq.sig1;
                ret = SIGNAL_GROUP_TDSCDMA;
            }
            updateSignalStrengthKey(String8("UMTS").string(), ecsq.rscp_in_qdbm>>2, (ecsq.sig2>>1)-24);
            break;
        }

        return ret;
    }

    if (memcmp(&origCache, signalCache, sizeof(*signalCache)) == 0)
        ret = SIGNAL_GROUP_NONE;

    return ret;
}

int RmcNetworkHandler::getCurrentSignalGroup() {
    return getSignalGroupByRegRat(
            convertRegState(mdEreg[m_slot_id]->stat, true),
            convertCSNetworkType(mdEreg[m_slot_id]->eAct),
            convertRegState(mdEgreg[m_slot_id]->stat, false),
            convertPSNetworkType(mdEgreg[m_slot_id]->eAct));
}


int RmcNetworkHandler::getSignalGroupByRegRat(
        int csRegState, int csRat, int psRegState, int psRat) {

    int group = SIGNAL_GROUP_NONE;

    if (isInService(csRegState)) {
        switch (csRat) {

            case RADIO_TECH_GSM:
                group |= SIGNAL_GROUP_GSM;
                break;

            case RADIO_TECH_1xRTT:
                group |= SIGNAL_GROUP_1XRTT;
                break;

            case RADIO_TECH_UMTS:
                group |= SIGNAL_GROUP_UMTS;
                break;

            case RADIO_TECH_EVDO_0:
            case RADIO_TECH_EVDO_A:
            case RADIO_TECH_EVDO_B:
            case RADIO_TECH_EHRPD:
                group |= SIGNAL_GROUP_EVDO;
                break;

            case RADIO_TECH_LTE:
            case RADIO_TECH_LTE_CA:
                group |= SIGNAL_GROUP_LTE;
                break;

            case RADIO_TECH_NR:
                group |= SIGNAL_GROUP_NR;
                break;

            default:
                break;
        }
    }

    if (isInService(psRegState)) {
        switch (psRat) {

            case RADIO_TECH_GPRS:
            case RADIO_TECH_EDGE:
                group |= SIGNAL_GROUP_GSM;
                break;

            case RADIO_TECH_1xRTT:
                group |= SIGNAL_GROUP_1XRTT;
                break;

            case RADIO_TECH_UMTS:
            case RADIO_TECH_HSDPA:
            case RADIO_TECH_HSUPA:
            case RADIO_TECH_HSPA:
            case RADIO_TECH_HSPAP:
                group |= SIGNAL_GROUP_UMTS;
                break;

            case RADIO_TECH_EVDO_0:
            case RADIO_TECH_EVDO_A:
            case RADIO_TECH_EVDO_B:
            case RADIO_TECH_EHRPD:
                group |= SIGNAL_GROUP_EVDO;
                break;

            case RADIO_TECH_LTE:
            case RADIO_TECH_LTE_CA:
                group |= SIGNAL_GROUP_LTE;
                if (last_physical_config_has_nr[m_slot_id] == 1)
                    group |= SIGNAL_GROUP_NR;
                break;

            case RADIO_TECH_NR:
                group |= SIGNAL_GROUP_NR;
                break;

            default:
                break;
        }
    }

    return group;
}

int RmcNetworkHandler::getLastReportedSignal() {
    return lastReportedSignal[m_slot_id];
}

void RmcNetworkHandler::fillSignalStrengthResponse(
        RIL_SIGNAL_STRENGTH *response, int group) {

    RIL_SIGNAL_STRENGTH *signalCache = signal_strength_cache[m_slot_id];

    if (group & SIGNAL_GROUP_GSM) {
        response->gsm_signal_strength = signalCache->gsm_signal_strength;
        response->gsm_bit_error_rate = signalCache->gsm_bit_error_rate;
        response->gsm_timing_advance = signalCache->gsm_timing_advance;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_GSM);
    }

    if (group & SIGNAL_GROUP_1XRTT) {
        response->cdma_dbm = signalCache->cdma_dbm;
        response->cdma_ecio = signalCache->cdma_ecio;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_1XRTT);
    }

    if (group & SIGNAL_GROUP_UMTS) {
        response->wcdma_signal_strength = signalCache->wcdma_signal_strength;
        response->wcdma_bit_error_rate = signalCache->wcdma_bit_error_rate;
        response->wcdma_scdma_rscp = signalCache->wcdma_scdma_rscp;
        response->wcdma_ecno = signalCache->wcdma_ecno;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_UMTS);
    }

    if (group & SIGNAL_GROUP_EVDO) {
        response->evdo_dbm = signalCache->evdo_dbm;
        response->evdo_ecio = signalCache->evdo_ecio;
        response->evdo_snr = signalCache->evdo_snr;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_EVDO);
    }

    if (group & SIGNAL_GROUP_TDSCDMA) {
        response->tdscdma_signal_strength = signalCache->tdscdma_signal_strength;
        response->tdscdma_bit_error_rate = signalCache->tdscdma_bit_error_rate;
        response->tdscdma_rscp = signalCache->tdscdma_rscp;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_TDSCDMA);
    }

    if (group & SIGNAL_GROUP_LTE) {
        response->lte_signal_strength = signalCache->lte_signal_strength;
        response->lte_rsrp = signalCache->lte_rsrp;
        response->lte_rsrq = signalCache->lte_rsrq;
        response->lte_rssnr = signalCache->lte_rssnr;
        response->lte_cqi = signalCache->lte_cqi;
        response->lte_timing_advance = signalCache->lte_timing_advance;
        response->lte_cqi_table_index = INT_MAX;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_LTE);
    }

    if (group & SIGNAL_GROUP_NR) {
        response->ssRsrp = signalCache->ssRsrp;
        response->ssRsrq = signalCache->ssRsrq;
        response->ssSinr = signalCache->ssSinr;
        response->csiRsrp = signalCache->csiRsrp;
        response->csiRsrq = signalCache->csiRsrq;
        response->csiSinr = signalCache->csiSinr;
        response->nr_csicqi_table_index = INT_MAX;
        response->nr_csicqi_report_count = 0;
        response->nr_timing_advance = 0;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_NR);
    }

    if (group != SIGNAL_GROUP_NONE)
        lastReportedSignal[m_slot_id] = group;
}

unsigned int RmcNetworkHandler::convertRegState(unsigned int uiRegState, bool isVoiceState)
{
    unsigned int uiRet = 0;

    switch (uiRegState)
    {
        case 6:         // Registered for "SMS only", home network
            uiRet = 1;  // Registered
            break;
        case 7:         // Registered for "SMS only", roaming
            uiRet = 5;  // roaming
            break;
        case 8:         // attached for emergency bearer service only
            uiRet = 0;  // not registered
            break;
        case 9:         // registered for "CSFB not prefereed", home network
            uiRet = 1;  // Registered
            break;
        case 10:        // registered for "CSFB not prefereed", roaming
            uiRet = 5;  // roaming
            break;
        case 101:       // no NW, but need to find NW
            if (isVoiceState) {
                uiRet = 1;  // 1xRTT normal service, Mapping '+VSER:0'
            } else {
                uiRet = 0;  // not registered
            }
            break;
        case 102:       // not registered, but MT find 1X NW existence
            if (isVoiceState) {
                uiRet = 2; // 1x is searching
            } else {
                uiRet = 0; // not registered
            }
            break;
        case 103:       // not registered, but MT find Do NW existence
            uiRet = 0;  // not registered
            break;
        case 104:       // not registered, but MT find Do&1X NW existence
            uiRet = 0;  // not registered
            break;
        default:
            uiRet = uiRegState;
            break;
    }

    return uiRet;
}

void RmcNetworkHandler::resetSignalStrengthCache(
            RIL_SIGNAL_STRENGTH *signalCache, RIL_SIGNAL_GROUP group) {
    if (group & SIGNAL_GROUP_GSM) {
        signalCache->gsm_signal_strength = 99;
        signalCache->gsm_bit_error_rate = 99;
        signalCache->gsm_timing_advance = CELLINFO_INVALID;
    }
    if (group & SIGNAL_GROUP_1XRTT) {
        signalCache->cdma_dbm = CELLINFO_INVALID;
        signalCache->cdma_ecio = CELLINFO_INVALID;
    }
    if (group & SIGNAL_GROUP_UMTS) {
        signalCache->wcdma_signal_strength = 99;
        signalCache->wcdma_bit_error_rate = 99;
        signalCache->wcdma_scdma_rscp = 255;
        signalCache->wcdma_ecno = 255;
    }
    if (group & SIGNAL_GROUP_EVDO) {
        signalCache->evdo_dbm = CELLINFO_INVALID;
        signalCache->evdo_ecio = CELLINFO_INVALID;
        signalCache->evdo_snr = CELLINFO_INVALID;
    }
    if (group & SIGNAL_GROUP_TDSCDMA) {
        signalCache->tdscdma_signal_strength = 99;
        signalCache->tdscdma_bit_error_rate = 99;
        signalCache->tdscdma_rscp = 255;
    }
    if (group & SIGNAL_GROUP_LTE) {
        signalCache->lte_signal_strength = 99;
        signalCache->lte_rsrp = CELLINFO_INVALID;
        signalCache->lte_rsrq = CELLINFO_INVALID;
        signalCache->lte_rssnr = CELLINFO_INVALID;
        signalCache->lte_cqi = CELLINFO_INVALID;
        signalCache->lte_timing_advance = CELLINFO_INVALID;
        signalCache->lte_cqi_table_index = INT_MAX;
    }
    if (group & SIGNAL_GROUP_NR) {
        signalCache->ssRsrp = CELLINFO_INVALID;
        signalCache->ssRsrq = CELLINFO_INVALID;
        signalCache->ssSinr = CELLINFO_INVALID;
        signalCache->csiRsrp = CELLINFO_INVALID;
        signalCache->csiRsrq = CELLINFO_INVALID;
        signalCache->csiSinr = CELLINFO_INVALID;
        signalCache->nr_csicqi_table_index = INT_MAX;
        signalCache->nr_csicqi_report_count = 0;
        signalCache->nr_timing_advance = CELLINFO_INVALID;
        for (int i = 0; i < MAX_COUNT_NR_CSI_CQI_REPORT; i++) {
            signalCache->nr_csicqi_reports[i] = 0xFF;
        }
    }

    if (group == SIGNAL_GROUP_ALL)
        lastReportedSignal[m_slot_id] = SIGNAL_GROUP_NONE;
}

// used for report wcdma ecio in signalStength, return true if you want to get WcdmaEcio.
bool RmcNetworkHandler::enableReportSignalStrengthWithWcdmaEcio() {
    bool enableReport = false;
    if (isRilTestMode()) {
        enableReport = true;
    }
    return enableReport;
}

bool RmcNetworkHandler::isTdd3G() {
    bool isTdd3G = false;
    int mainSlot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0);
    if (mainSlot == m_slot_id) {
        char worldMode_prop[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get("vendor.ril.nw.worldmode.activemode", worldMode_prop, "1");
        if (atoi(worldMode_prop) == 2) {
            isTdd3G = true;
        }
    }

    return isTdd3G;
}

int RmcNetworkHandler::isFemtocellSupport() {
    int isFemtocellSupport = 0;
    char optr[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("ro.vendor.mtk_femto_cell_support", optr, "0");
    isFemtocellSupport = atoi(optr);
    if (isMatchOpid("OP12") == true ||
            isMatchOpid("OP07") == true) {
        isFemtocellSupport = 1;
    }
    return isFemtocellSupport;
}

int RmcNetworkHandler::isOp12Plmn(const char* plmn) {
    unsigned long i = 0;
    if (plmn != NULL) {
        for (i = 0 ; i < (sizeof(sOp12Plmn)/sizeof(sOp12Plmn[0])) ; i++) {
            if (strcmp(plmn, sOp12Plmn[i]) == 0) {
                //logD(LOG_TAG, "[isOp12Plmn] plmn:%s", plmn);
                return true;
            }
        }
    }
    return false;
}

bool RmcNetworkHandler::isMatchOpid(const char* opid) {
    char optr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.operator.optr", optr, "0");

    if (strcmp(optr, opid) == 0) {
        return true;
    }
    return false;
}

char const *RmcNetworkHandler::sourceToString(int srcId) {
    switch (srcId) {
        case CACHE_GROUP_GSM:
            return "GSM";
        case CACHE_GROUP_C2K:
            return "C2K";
        case CACHE_GROUP_1XRTT:
            return "1XRTT";
        case CACHE_GROUP_EVDO:
            return "EVDO";
        case CACHE_GROUP_ALL:
            return "ALL";
        default:
            return "INVALID SRC";
    }
}

int RmcNetworkHandler::getCellInfoListV12(RfxAtLine* line, int num,
        RIL_CellInfo_v12 * response) {
    int INVALID = INT_MAX; // 0x7FFFFFFF;
    int INVALID_IE = 255;
    int err=0, i=0,act=0 ,cid=0,mcc=0,mnc=0,lacTac=0,pscPci=0, mnc_len=0;
    int sig1=0,sig2=0,rsrp=0,rsrq=0,rssnr=0,cqi=0,timingAdvance=0;
    int bsic=0;
    int arfcn=INVALID;
    /* C2K related cell info. */
    int nid = 0;
    int sid = 0;
    int base_station_id = 0;
    int base_station_longitude = 0;
    int base_station_latitude = 0;
    int cdma_dbm = INVALID;
    int cdma_ecio = INVALID;
    int evdo_dbm = INVALID;
    int evdo_ecio = INVALID;
    int snr = 0;
    int evdo_snr = INVALID;
    // for NR
    char *tmp = NULL;
    long long nr_cid = 0;
    int nr_tac = 0, nr_mcc = 0, nr_mnc = 0, nr_pci = 0, nr_bsic = 0, nr_arfcn = 0;
    int nr_ta = 0;
    int nr_ssrsrp_ie = 0, nr_ssrsrq_ie = 0, nr_sssinr_ie = 0;
    int nr_csirsrp_ie = 0, nr_csirsrq_ie = 0, nr_csisinr_ie = 0;

    // for Operator name
    char longname[MAX_OPER_NAME_LENGTH], shortname[MAX_OPER_NAME_LENGTH];
    char* mccmnc = NULL;
    char* mMnc = NULL;

     /* +ECELL: <num_of_cell>[,<act>,<cid>,<lac_or_tac>,<mcc>,<mnc>,
             <psc_or_pci>, <sig1>,<sig2>,<sig1_in_dbm>,<sig2_in_dbm>,<ta>,
             <ext1>,<ext2>,]
             [<ext3>,<ext4>]
             [,...]
             [,<Act>,<nid>,<sid>,<bs_id>,<bs_long>,<bs_lat>,<1xRTT_rssi>,
             <1xRTT_ec/io>,<EVDO_rssi>,<EVDO_ec/io>,<EVDO_snr>]
    */
     // ext3 is for gsm bsic
     // ext4 is for arfcn, uarfcn, earfch
    int ext3_ext4_enabled = ECELLext3ext4Support; //set to 1 if modem support

    for (i = 0 ; i < num ; i++) {
        /* Registered field is used to tell serving cell or neighboring cell.
           The first cell info returned is the serving cell,
           others are neighboring cell */
        if(i == 0 &&
                (isInService(voice_reg_state_cache[m_slot_id]->registration_state) ||
                isInService(data_reg_state_cache[m_slot_id]->registration_state))) {
            response[i].registered = 1;
            response[i].connectionStatus = PRIMARY_SERVING;
        } else {
            response[i].registered = 0;
            response[i].connectionStatus = NONE_SERVING;
        }
        act = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (act == 0 || act == 2 || act == 7) {
            cid = line->atTokNexthexint(&err);
            if (err < 0)
                goto error;

            lacTac = line->atTokNexthexint(&err);
            if (err < 0) {
                lacTac = INVALID;
            }
            mcc = line->atTokNextint(&err);
            if (err < 0) {
                mcc = INVALID;
            }
            mMnc= line->atTokNextstr(&err);
            if (err < 0) {
                mnc = INVALID;
            } else {
                mnc = atoi(mMnc);
                mnc_len = strlen(mMnc) > 2 ? 3 : 2;
            }
            pscPci = line->atTokNextint(&err);
            if (err < 0) {
                pscPci = INVALID;
            }
            sig1 = line->atTokNextint(&err);
            if (err < 0) {
                sig1 = INVALID;
            }
            sig2 = line->atTokNextint(&err);
            if (err < 0) {
                sig2 = INVALID;
            }
            rsrp = line->atTokNextint(&err);
            if (err < 0) {
                rsrp = INVALID;
            }
            rsrq = line->atTokNextint(&err);
            if (err < 0) {
                rsrq = INVALID;
            }
            timingAdvance = line->atTokNextint(&err);
            if (err < 0) {
                timingAdvance = INVALID;
            }
            rssnr = line->atTokNextint(&err);
            if (err < 0) {
                rssnr = INVALID;
            }
            cqi = line->atTokNextint(&err);
            if (err < 0) {
                cqi = INVALID;
            }
            if (ext3_ext4_enabled) {
                bsic = line->atTokNextint(&err);
                if (err < 0) {
                    bsic = 0;
                }
                arfcn = line->atTokNextint(&err);
                if (err < 0) {
                    arfcn = INVALID;
                }
            }
            char* cid_s = getMask(cid);
            char* lac_s = getMask(lacTac);
            char* pci_s = getMask(pscPci);
            if (!cid_s || !lac_s || !pci_s) {
                logE(LOG_TAG, "[%s] can not get memory to print log", __func__);
            } else {
                logV(LOG_TAG, "act=%d,cid=%s,mcc=%d,mnc=%d,mnc_len=%d,lacTac=%s,"
                        "pscPci=%s,sig1=%d,sig2=%d,rsrp=%d,rsrq=%d,ta=%d,"
                        "rssnr=%d,cqi=%d,bsic=%d,arfcn=%d",
                        act, cid_s, mcc, mnc, mnc_len, lac_s, pci_s, sig1, sig2, rsrp,
                        rsrq, timingAdvance, rssnr, cqi, bsic, arfcn);
            }
            if (cid_s) free(cid_s);
            if (lac_s) free(lac_s);
            if (pci_s) free(pci_s);
            // Skip cell information if signal is invalid in serving cell
            if (i == 0 && sig1 == 255 && sig2 == 255) {
                goto error;
            }
            // keep first cell info
            if (i == 0) {
                mdEcell[m_slot_id]->act = act;
                mdEcell[m_slot_id]->cid = cid;
                mdEcell[m_slot_id]->mcc = mcc;
                mdEcell[m_slot_id]->mnc = mnc;
                mdEcell[m_slot_id]->lacTac = lacTac;
                mdEcell[m_slot_id]->pscPci = pscPci;
                mdEcell[m_slot_id]->sig1 = sig1;
                mdEcell[m_slot_id]->sig2 = sig2;
                mdEcell[m_slot_id]->rsrp = rsrp;
                mdEcell[m_slot_id]->rsrq = rsrq;
                mdEcell[m_slot_id]->timingAdvance = timingAdvance;
                mdEcell[m_slot_id]->rssnr = rssnr;
                mdEcell[m_slot_id]->cqi = cqi;
                mdEcell[m_slot_id]->bsic = bsic;
                mdEcell[m_slot_id]->arfcn = arfcn;
            }
        } else if (act == 256) {
            response[i].registered = 1;
            response[i].connectionStatus = PRIMARY_SERVING;
            nid = line->atTokNextint(&err);
            if (err < 0) {
                nid = INVALID;
            }
            sid = line->atTokNextint(&err);
            if (err < 0) {
                sid = INVALID;
            }
            base_station_id = line->atTokNextint(&err);
            if (err < 0) {
                base_station_id = INVALID;
            }
            base_station_longitude = line->atTokNextint(&err);
            if (err < 0) {
                base_station_longitude = INVALID;
            }
            base_station_latitude = line->atTokNextint(&err);
            if (err < 0) {
                base_station_latitude = INVALID;
            }
            cdma_dbm = line->atTokNextint(&err);
            if (err < 0 || cdma_dbm < 0  || cdma_dbm > 31) {
                cdma_dbm = INVALID;
            }
            cdma_ecio = line->atTokNextint(&err);
            if (err < 0 || cdma_ecio < -128 || cdma_ecio > 0) {
                cdma_ecio = INVALID;
            }
            evdo_dbm = line->atTokNextint(&err);
            if (err < 0 || evdo_dbm < 0  || evdo_dbm > 31) {
                evdo_dbm = INVALID;
            }
            evdo_ecio = line->atTokNextint(&err);
            if (err < 0 || evdo_ecio < -512 || evdo_ecio > 0) {
                evdo_ecio = INVALID;
            }
            snr = line->atTokNextint(&err);
            if (err < 0) {
                snr = INVALID;
            }
            if (snr != INVALID) {
                float temp = (double) snr / 512;
                float snr_db = 100 * log10(temp);
                if (snr_db >= 100) {
                    evdo_snr = 8;
                } else if (snr_db >= 70) {
                    evdo_snr = 7;
                } else if (snr_db >= 50) {
                    evdo_snr = 6;
                } else if (snr_db >= 30) {
                    evdo_snr = 5;
                } else if (snr_db >= -20) {
                    evdo_snr = 4;
                } else if (snr_db >= -45) {
                    evdo_snr = 3;
                } else if (snr_db >= -90) {
                    evdo_snr = 2;
                } else if (snr_db > -120) {
                    evdo_snr = 1;
                } else {
                    evdo_snr = 0;
                }
            }

            // Print cell info log
            char* nid_s = getMask(nid);
            char* sid_s = getMask(sid);
            char* base_station_id_s = getMask(base_station_id);
            char* base_station_longitude_s = getMask(base_station_longitude);
            char* base_station_latitude_s = getMask(base_station_latitude);
            if (!nid_s || !sid_s || !base_station_id_s || !base_station_longitude_s
                    || !base_station_latitude_s) {
                logE(LOG_TAG, "[%s] can not get memory to print log", __func__);
            } else {
                logD(LOG_TAG, "nid=%s,sid=%s,base_station_id=%s,"
                        "base_station_longitude=%s,base_station_latitude=%s,"
                        "cdma_dbm=%d,cdma_ecio=%d,evdo_dbm=%d,evdo_ecio=%d,"
                        "snr=%d,evdo_snr=%d",
                        nid_s, sid_s, base_station_id_s, base_station_longitude_s,
                        base_station_latitude_s, cdma_dbm, cdma_ecio, evdo_dbm,
                        evdo_ecio, snr, evdo_snr);
            }
            if (nid_s) free(nid_s);
            if (sid_s) free(sid_s);
            if (base_station_id_s) free(base_station_id_s);
            if (base_station_longitude_s) free(base_station_longitude_s);
            if (base_station_latitude_s) free(base_station_latitude_s);
        } else if (act == 11) { // NR
            tmp = line->atTokNextstr(&err);
            if (err < 0) nr_cid = LONG_MAX;
            else nr_cid = strtoull(tmp, NULL, 16);
            nr_tac = line->atTokNexthexint(&err);
            if (err < 0) nr_tac = INVALID;
            nr_mcc = line->atTokNextint(&err);
            if (err < 0) nr_mcc = INVALID;
            mMnc= line->atTokNextstr(&err);
            if (err < 0) {
                nr_mnc = INVALID;
            } else {
                nr_mnc = atoi(mMnc);
                mnc_len = strlen(mMnc) > 2 ? 3 : 2;
            }
            nr_pci = line->atTokNextint(&err);
            if (err < 0) nr_pci = INVALID;

            nr_ssrsrp_ie= line->atTokNextint(&err);
            if (err < 0) nr_ssrsrp_ie = INVALID_IE;
            nr_ssrsrq_ie = line->atTokNextint(&err);
            if (err < 0) nr_ssrsrq_ie = INVALID_IE;
            nr_csirsrp_ie = line->atTokNextint(&err);
            if (err < 0) nr_ssrsrq_ie = INVALID_IE;
            nr_csirsrq_ie = line->atTokNextint(&err);
            if (err < 0) nr_ssrsrq_ie = INVALID_IE;
            nr_ta = line->atTokNextint(&err);
            if (err < 0) nr_ta = INVALID;

            nr_sssinr_ie = line->atTokNextint(&err);
            if (err < 0) nr_sssinr_ie = INVALID_IE;
            nr_csisinr_ie = line->atTokNextint(&err);
            if (err < 0) nr_csisinr_ie = INVALID_IE;

            nr_bsic= line->atTokNextint(&err);
            if (err < 0) nr_bsic = INVALID;
            nr_arfcn = line->atTokNextint(&err);
            if (err < 0) nr_arfcn = INVALID;
            char* s_nr_cid = getMask(nr_cid);
            char* s_nr_tac = getMask(nr_tac);
            char* s_nr_pci = getMask(nr_pci);
            if (!s_nr_cid || !s_nr_tac || !s_nr_pci) {
                logD(LOG_TAG, "act=%d, nr_cid=X, nr_tac=X, nr_mcc=%d, nr_mnc=%d, mnc_len=%d, "
                        "nr_pci=X, nr_ssrsrp_ie=%d, nr_ssrsrq_ie=%d, nr_csirsrp_ie=%d, "
                        "nr_csirsrq_ie=%d, nr_ta=%d, nr_sssinr=%d, nr_csisinr=%d, nr_bsic=%d, "
                        "nr_arfcn=%d",
                        act, nr_mcc, nr_mnc, mnc_len, nr_ssrsrp_ie, nr_ssrsrq_ie,
                        nr_csirsrp_ie, nr_csirsrq_ie, nr_ta, nr_sssinr_ie, nr_csisinr_ie, nr_bsic,
                        nr_arfcn);
            } else {
                logD(LOG_TAG, "act=%d, nr_cid=%s, nr_tac=%s, nr_mcc=%d, nr_mnc=%d, mnc_len=%d, "
                        "nr_pci=%s, nr_ssrsrp_ie=%d, nr_ssrsrq_ie=%d, nr_csirsrp_ie=%d, "
                        "nr_csirsrq_ie=%d, nr_ta=%d, nr_sssinr=%d, nr_csisinr=%d, nr_bsic=%d, "
                        "nr_arfcn=%d",
                        act, s_nr_cid, s_nr_tac, nr_mcc, nr_mnc, mnc_len, s_nr_pci, nr_ssrsrp_ie,
                        nr_ssrsrq_ie, nr_csirsrp_ie, nr_csirsrq_ie, nr_ta, nr_sssinr_ie,
                        nr_csisinr_ie, nr_bsic, nr_arfcn);
            }
            if (s_nr_cid) free(s_nr_cid);
            if (s_nr_tac) free(s_nr_tac);
            if (s_nr_pci) free(s_nr_pci);
            // keep first cell info
            if (i == 0) {
                mdEcell[m_slot_id]->act = act;
                mdEcell[m_slot_id]->cid = nr_cid;
                mdEcell[m_slot_id]->mcc = nr_mcc;
                mdEcell[m_slot_id]->mnc = nr_mnc;
                mdEcell[m_slot_id]->lacTac = nr_tac;
                mdEcell[m_slot_id]->pscPci = nr_pci;
                mdEcell[m_slot_id]->sig1 = nr_ssrsrp_ie;
                mdEcell[m_slot_id]->sig2 = nr_ssrsrq_ie;
                mdEcell[m_slot_id]->rsrp = nr_csirsrp_ie;
                mdEcell[m_slot_id]->rsrq = nr_csirsrq_ie;
                mdEcell[m_slot_id]->timingAdvance = nr_ta;
                mdEcell[m_slot_id]->rssnr = nr_csisinr_ie;
                mdEcell[m_slot_id]->cqi = INVALID;
                mdEcell[m_slot_id]->bsic = nr_bsic;
                mdEcell[m_slot_id]->arfcn = nr_arfcn;
            }
        } else {
            logD(LOG_TAG, "RIL_CELL_INFO_TYPE invalid act=%d", act);
        }

        /* <Act>  0: GSM , 2: UMTS , 7: LTE, 256: 1x */
        if(act == 7) {
            if (mcc == INVALID || mnc == INVALID) asprintf(&mccmnc, "00000");
            else if (mnc_len == 3) asprintf(&mccmnc, "%03d%03d", mcc, mnc);
            else asprintf(&mccmnc, "%03d%02d", mcc, mnc);
            getPLMNNameFromNumeric(mccmnc, longname, shortname, MAX_OPER_NAME_LENGTH);
            free(mccmnc);
            response[i].cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            response[i].CellInfo.lte.cellIdentityLte.ci = cid;
            response[i].CellInfo.lte.cellIdentityLte.mcc = mcc;
            response[i].CellInfo.lte.cellIdentityLte.mnc = mnc;
            response[i].CellInfo.lte.cellIdentityLte.mnc_len= mnc_len;
            response[i].CellInfo.lte.cellIdentityLte.tac = lacTac;
            response[i].CellInfo.lte.cellIdentityLte.pci = pscPci;
            response[i].CellInfo.lte.cellIdentityLte.earfcn = arfcn;
            if (lteBand[m_slot_id] >= 1 && lteBand[m_slot_id] <= 88) {
                response[i].CellInfo.lte.cellIdentityLte.band = lteBand[m_slot_id];
            } else {
                //set to 1 if band is not right
                logD(LOG_TAG, "set 1 as LTE cell info's band");
                response[i].CellInfo.lte.cellIdentityLte.band = 1;
            }
            asprintf(&(response[i].CellInfo.lte.cellIdentityLte.operName.long_name),
                    "%s", longname);
            asprintf(&(response[i].CellInfo.lte.cellIdentityLte.operName.short_name),
                    "%s", shortname);

            /*For LTE, the ext1 field is rssi, format follows 27.007 +CSQ*/
            response[i].CellInfo.lte.signalStrengthLte.signalStrength = rssnr;

            int aosp_rsrp = -rsrp / 4;
            if (aosp_rsrp > 140) aosp_rsrp = 140;
            else if (aosp_rsrp < 44) aosp_rsrp = 44;
            response[i].CellInfo.lte.signalStrengthLte.rsrp = aosp_rsrp;

            int aosp_rsrq = rsrq != INVALID ? -rsrq / 4 : INVALID;
            if (aosp_rsrq > 35 && aosp_rsrq != INVALID) aosp_rsrq = 35;
            else if (aosp_rsrq < 3 && aosp_rsrq != INVALID) aosp_rsrq = 3;
            response[i].CellInfo.lte.signalStrengthLte.rsrq = aosp_rsrq;

            int aosp_ta = timingAdvance != INVALID ? timingAdvance : INVALID;
            if (aosp_ta < 0 && aosp_ta != INVALID) aosp_ta = 0;
            else if (aosp_ta > 1282 && aosp_ta != INVALID) aosp_ta = 1282;
            response[i].CellInfo.lte.signalStrengthLte.timingAdvance = aosp_ta;

            if (response[i].registered == 1) {
                /* +ECELL doesn't supply LTE rssnr.
                 Just fill the registered item by the rssnr in signal_strength_cache*/
                response[i].CellInfo.lte.signalStrengthLte.rssnr =
                        signal_strength_cache[m_slot_id]->lte_rssnr;
            } else {
                response[i].CellInfo.lte.signalStrengthLte.rssnr = 0;
            }

            if (cqi < 0 && cqi != INVALID) cqi = 0;
            else if (cqi > 15 && cqi != INVALID) cqi = 15;
            response[i].CellInfo.lte.signalStrengthLte.cqi = cqi;
            response[i].CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;
        } else if(act == 2) {
            if (mcc == INVALID || mnc == INVALID) asprintf(&mccmnc, "00000");
            else if (mnc_len == 3) asprintf(&mccmnc, "%03d%03d", mcc, mnc);
            else asprintf(&mccmnc, "%03d%02d", mcc, mnc);
            getPLMNNameFromNumeric(mccmnc, longname, shortname, MAX_OPER_NAME_LENGTH);
            free(mccmnc);
            response[i].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            response[i].CellInfo.wcdma.cellIdentityWcdma.cid = cid;
            response[i].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            response[i].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            response[i].CellInfo.wcdma.cellIdentityWcdma.mnc_len= mnc_len;
            response[i].CellInfo.wcdma.cellIdentityWcdma.lac = lacTac;
            response[i].CellInfo.wcdma.cellIdentityWcdma.psc = pscPci;
            response[i].CellInfo.wcdma.cellIdentityWcdma.uarfcn = arfcn;
            asprintf(&(response[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                    "%s", longname);
            asprintf(&(response[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                    "%s", shortname);
            response[i].CellInfo.wcdma.signalStrengthWcdma.signalStrength =
                    convert3GRssiValue(sig1-120);
            response[i].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate = 99;
            response[i].CellInfo.wcdma.signalStrengthWcdma.rscp = sig1;
            response[i].CellInfo.wcdma.signalStrengthWcdma.ecno = sig2;
            logE(LOG_TAG, "rscp=%d, ecno=%d", sig1, sig2);
        } else if (act == 0) {
            if (mcc == INVALID || mnc == INVALID) asprintf(&mccmnc, "00000");
            else if (mnc_len == 3) asprintf(&mccmnc, "%03d%03d", mcc, mnc);
            else asprintf(&mccmnc, "%03d%02d", mcc, mnc);
            getPLMNNameFromNumeric(mccmnc, longname, shortname, MAX_OPER_NAME_LENGTH);
            free(mccmnc);
            response[i].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            response[i].CellInfo.gsm.cellIdentityGsm.cid = cid;
            response[i].CellInfo.gsm.cellIdentityGsm.mcc = mcc;
            response[i].CellInfo.gsm.cellIdentityGsm.mnc = mnc;
            response[i].CellInfo.gsm.cellIdentityGsm.mnc_len= mnc_len;
            response[i].CellInfo.gsm.cellIdentityGsm.lac = lacTac;
            response[i].CellInfo.gsm.cellIdentityGsm.arfcn = arfcn;
            response[i].CellInfo.gsm.cellIdentityGsm.bsic = bsic;
            asprintf(&(response[i].CellInfo.gsm.cellIdentityGsm.operName.long_name),
                    "%s", longname);
            asprintf(&(response[i].CellInfo.gsm.cellIdentityGsm.operName.short_name),
                    "%s", shortname);
            int aosp_level = (sig1 + 2) / 2;
            if (aosp_level > 31) aosp_level = 31;
            else if (aosp_level < 0) aosp_level = 0;
            response[i].CellInfo.gsm.signalStrengthGsm.signalStrength = aosp_level;
            response[i].CellInfo.gsm.signalStrengthGsm.bitErrorRate = sig2;

            if (timingAdvance < 0 && timingAdvance != INVALID) timingAdvance = 0;
            else if (timingAdvance > 219 && timingAdvance != INVALID) timingAdvance = 219;
            response[i].CellInfo.gsm.signalStrengthGsm.timingAdvance = timingAdvance;
        } else if (act == 256) {
            response[i].cellInfoType = RIL_CELL_INFO_TYPE_CDMA;
            response[i].CellInfo.cdma.cellIdentityCdma.networkId = nid;
            response[i].CellInfo.cdma.cellIdentityCdma.systemId = sid;
            response[i].CellInfo.cdma.cellIdentityCdma.basestationId = base_station_id;
            response[i].CellInfo.cdma.cellIdentityCdma.longitude = base_station_longitude;
            response[i].CellInfo.cdma.cellIdentityCdma.latitude = base_station_latitude;
            response[i].CellInfo.cdma.cellIdentityCdma.operName.long_name = NULL;  // unknown
            response[i].CellInfo.cdma.cellIdentityCdma.operName.short_name = NULL;  // unknown
            ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
            if (mViaHandler != NULL && cdma_dbm != INVALID) {
                response[i].CellInfo.cdma.signalStrengthCdma.dbm =
                        mViaHandler->convertCdmaEvdoSig(cdma_dbm,
                                SIGNAL_CDMA_DBM);
            } else {
                response[i].CellInfo.cdma.signalStrengthCdma.dbm = INVALID;
            }
            if (mViaHandler != NULL && cdma_ecio != INVALID) {
                response[i].CellInfo.cdma.signalStrengthCdma.ecio =
                        mViaHandler->convertCdmaEvdoSig(cdma_ecio,
                                SIGNAL_CDMA_ECIO);
            } else {
                response[i].CellInfo.cdma.signalStrengthCdma.ecio = INVALID;
            }
            if (mViaHandler != NULL && evdo_dbm != INVALID) {
                response[i].CellInfo.cdma.signalStrengthEvdo.dbm =
                        mViaHandler->convertCdmaEvdoSig(evdo_dbm,
                                SIGNAL_EVDO_DBM);
            } else {
                response[i].CellInfo.cdma.signalStrengthEvdo.dbm = INVALID;

            }
            if (mViaHandler != NULL && evdo_ecio != INVALID) {
                response[i].CellInfo.cdma.signalStrengthEvdo.ecio =
                        mViaHandler->convertCdmaEvdoSig(evdo_ecio,
                                SIGNAL_EVDO_ECIO);
            } else {
                response[i].CellInfo.cdma.signalStrengthEvdo.ecio = INVALID;

            }
            response[i].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio = evdo_snr;
            logD(LOG_TAG, "RIL_CELL_INFO_TYPE_C2K act=%d, cdma_dbm=%d, "
                    "cdma_ecio=%d, evdo_dbm=%d, evdo_ecio=%d, evdo_snr=%d ",
                    act, response[i].CellInfo.cdma.signalStrengthCdma.dbm,
                    response[i].CellInfo.cdma.signalStrengthCdma.ecio,
                    response[i].CellInfo.cdma.signalStrengthEvdo.dbm,
                    response[i].CellInfo.cdma.signalStrengthEvdo.ecio,
                    response[i].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio);
        } else if (act == 11) {
            if (nr_mcc == INVALID || nr_mnc == INVALID) asprintf(&mccmnc, "00000");
            else if (nr_mnc > 99) asprintf(&mccmnc, "%03d%03d", nr_mcc, nr_mnc);
            else asprintf(&mccmnc, "%03d%02d", nr_mcc, nr_mnc);
            getPLMNNameFromNumeric(mccmnc, longname, shortname, MAX_OPER_NAME_LENGTH);
            free(mccmnc);
            response[i].cellInfoType = RIL_CELL_INFO_TYPE_NR;
            response[i].CellInfo.nr.cellidentity.mcc = nr_mcc;
            response[i].CellInfo.nr.cellidentity.mnc = nr_mnc;
            response[i].CellInfo.nr.cellidentity.mnc_len = mnc_len;
            response[i].CellInfo.nr.cellidentity.nci = nr_cid;
            response[i].CellInfo.nr.cellidentity.nrarfcn = nr_arfcn;
            response[i].CellInfo.nr.cellidentity.pci = nr_pci;
            response[i].CellInfo.nr.cellidentity.tac = nr_tac;
            if ((nrBand[m_slot_id] >= 1 && nrBand[m_slot_id] <= 95) ||
                    (nrBand[m_slot_id] >= 257 && nrBand[m_slot_id] <= 261)) {
                response[i].CellInfo.nr.cellidentity.band = nrBand[m_slot_id];
            } else {
                // set to 1 when band is not right.
                logD(LOG_TAG, "set 1 as NR cell info's band");
                response[i].CellInfo.nr.cellidentity.band = 1;
            }
            asprintf(&(response[i].CellInfo.nr.cellidentity.operName.long_name), "%s", longname);
            asprintf(&(response[i].CellInfo.nr.cellidentity.operName.short_name), "%s", shortname);
            /*
                mtk_nr_rsrp_qdbm = mtk_nr_rsrp_ie << 2 - 628;
                aosp_rsrp = -1 * mtk_nr_rsrp_qdbm >> 2;
                aosp_rsrp = -1 *(mtk_nr_rsrp_ie << 2 - 628) >>2 = 157 -mtk_nr_rsrp_ie;
            */
            int aosp_nr_ssrsrp =
                nr_ssrsrp_ie != INVALID_IE ? (157 - nr_ssrsrp_ie) : INVALID;
            if (aosp_nr_ssrsrp > 140 && aosp_nr_ssrsrp != INVALID) aosp_nr_ssrsrp = 140;
            else if (aosp_nr_ssrsrp < 44 && aosp_nr_ssrsrp != INVALID) aosp_nr_ssrsrp = 44;
            response[i].CellInfo.nr.signalStrength.ssRsrp = aosp_nr_ssrsrp;

            /*
                mtk_nr_rsrq_qdbm = mtk_nr_rsrq_ie << 1 - 174;
                aosp_rsrq = -1 * mtk_nr_rsrq_qdbm >> 2;
                aosp_rsrp = -1 *(mtk_nr_rsrq_qdbm << 1 - 174) >>2
                            = (174 -mtk_nr_rsrq_ie << 1) >> 2 ;
                            = (87 -mtk_nr_rsrq_ie) >> 1;
            */
            int aosp_nr_ssrsrq =
                nr_ssrsrq_ie != INVALID_IE ? ((87 -nr_ssrsrq_ie) >> 1) : INVALID;
            if (aosp_nr_ssrsrq > 20 && aosp_nr_ssrsrq != INVALID) aosp_nr_ssrsrq = 20;
            else if (aosp_nr_ssrsrq < 3 && aosp_nr_ssrsrq != INVALID) aosp_nr_ssrsrq = 3;
            response[i].CellInfo.nr.signalStrength.ssRsrq = aosp_nr_ssrsrq;

            /*
                mtk_nr_sinr_qdbm = mtk_nr_sinr_ie << 1 - 94;
                aosp_sinr = mtk_nr_sinr_qdbm >> 2;
                aosp_sinr = (mtk_nr_sinr_ie << 1 - 94) >> 2 ;
                          = (mtk_nr_sinr_ie - 47) >> 1;
            */
            int aosp_nr_sssinr =
                nr_sssinr_ie != INVALID_IE ? ((nr_sssinr_ie - 47) >> 1) : INVALID;
            if (aosp_nr_sssinr > 40 && aosp_nr_sssinr != INVALID) aosp_nr_sssinr = 40;
            else if (aosp_nr_sssinr < -23 && aosp_nr_sssinr != INVALID) aosp_nr_sssinr = -23;
            response[i].CellInfo.nr.signalStrength.ssSinr = aosp_nr_sssinr;

            int aosp_nr_csirsrp =
                nr_csirsrp_ie != INVALID_IE ? (157-nr_csirsrp_ie) : INVALID;
            if (aosp_nr_csirsrp > 140 && aosp_nr_csirsrp != INVALID) aosp_nr_csirsrp = 140;
            else if (aosp_nr_csirsrp < 44 && aosp_nr_csirsrp != INVALID) aosp_nr_csirsrp = 44;
            response[i].CellInfo.nr.signalStrength.csiRsrp = aosp_nr_csirsrp;
            int aosp_nr_csirsrq =
                nr_csirsrq_ie != INVALID_IE ? ((87 - nr_csirsrq_ie) >> 1) : INVALID;
            if (aosp_nr_csirsrq > 20 && aosp_nr_csirsrq != INVALID) aosp_nr_csirsrq = 20;
            else if (aosp_nr_csirsrq < 3 && aosp_nr_csirsrq != INVALID) aosp_nr_csirsrq = 3;
            response[i].CellInfo.nr.signalStrength.csiRsrq = aosp_nr_csirsrq;
            int aosp_nr_csisinr =
                nr_csisinr_ie != INVALID_IE ? ((nr_csisinr_ie -47) >> 1) : INVALID;
            if (aosp_nr_csisinr > 40 && aosp_nr_csisinr != INVALID) aosp_nr_csisinr = 40;
            else if (aosp_nr_csisinr < -23 && aosp_nr_csisinr != INVALID) aosp_nr_csisinr = -23;
            response[i].CellInfo.nr.signalStrength.csiSinr = aosp_nr_csisinr;
            response[i].CellInfo.nr.signalStrength.csiCqiTableIndex = INT_MAX;
            response[i].CellInfo.nr.signalStrength.csiCqiReportCount = 0;
            response[i].CellInfo.nr.signalStrength.timingAdvance = nr_ta;
        } else {
            logD(LOG_TAG, "RIL_CELL_INFO_TYPE invalid act=%d", act);
        }
    }
    return 0;
error:
    return -1;
}

int RmcNetworkHandler::hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    RFX_LOG_E(LOG_TAG, "[%s] invalid char:%c", __FUNCTION__, c);
    return -1;
}

int RmcNetworkHandler::hexCharsToUcs2Encode(char* src) {
    int result = 0;
    for (unsigned int i = 0; i < 4; i++) {
        int tmp = hexCharToInt(src[i]);
        if (tmp < 0) return -1;

        result |= tmp << (3 - i) * 4;
    }

    return result;
}

void RmcNetworkHandler::convertToUtf8String(char *src, size_t n) {
    int len = strlen(src);
    char buf[MAX_OPER_NAME_LENGTH] = {0};

    if (len % 4 != 0) {
        RFX_LOG_E(LOG_TAG, "[%s] invalid hex char length:%d", __FUNCTION__, len);
    }

    for (unsigned int i = 0, idx = 0; i + 3 < len; i += 4) {
        int srcCode = hexCharsToUcs2Encode(src + i);
        if (srcCode < 0) break;
        if (srcCode > 0x7FF) {
            if (idx >= MAX_OPER_NAME_LENGTH - 4) {
                break;
            }
            buf[idx++] = (srcCode >> 12) | 0xE0;
            buf[idx++] = ((srcCode >> 6) & 0x3F) | 0x80;
            buf[idx++] = (srcCode & 0x3F) | 0x80;
        } else if (srcCode > 0x7F) {
            if (idx >= MAX_OPER_NAME_LENGTH - 3) {
                break;
            }
            buf[idx++] = (srcCode >> 6) | 0xC0;
            buf[idx++] = (srcCode & 0x3F) | 0x80;
        } else {
            if (idx >= MAX_OPER_NAME_LENGTH - 2) {
                break;
            }
            buf[idx++] = srcCode;
        }
    }

    strlcpy(src, buf, n);
}

void RmcNetworkHandler::convertToUtf8StringFromAscii(char *src, size_t n) {
    int len = strlen(src);
    char buf[MAX_OPER_NAME_LENGTH] = {0};

    for (unsigned int i = 0, idx = 0; i < len; i ++) {
        int srcCode = (int) src[i];
        if (srcCode > 0x7F) {
            if (idx >= MAX_OPER_NAME_LENGTH - 3) {
                break;
            }
            buf[idx++] = (srcCode >> 6) | 0xC0;
            buf[idx++] = (srcCode & 0x3F) | 0x80;
        } else {
            if (idx >= MAX_OPER_NAME_LENGTH - 2) {
                break;
            }
            buf[idx++] = srcCode;
        }
    }
    strlcpy(src, buf, n);
}

unsigned int RmcNetworkHandler::convertCSNetworkType(unsigned int uiResponse)
{
    RIL_RadioTechnology uiRet = RADIO_TECH_UNKNOWN;

    /* mapping */
    switch(uiResponse)
    {
        case 0x0001:     // GPRS
        case 0x0002:     // EDGE
            uiRet = RADIO_TECH_GSM;        // GSM
            break;
        case 0x0004:     // UMTS
        case 0x0008:     // HSDPA
        case 0x0010:     // HSUPA
        case 0x0018:     // HSDPA_UPA
        case 0x0020:     // HSDPAP
        case 0x0030:     // HSDPAP_UPA
        case 0x0040:     // HSUPAP
        case 0x0048:     // HSUPAP_DPA
        case 0x0060:     // HSPAP
        case 0x0088:     // DC_DPA
        case 0x0098:     // DC_DPA_UPA
        case 0x00a0:     // DC_HSDPAP
        case 0x00b0:     // DC_HSDPAP_UPA
        case 0x00c8:     // DC_HSUPAP_DPA
        case 0x00e0:     // DC_HSPAP
            uiRet = RADIO_TECH_UMTS;        // UMTS
            break;
        // for C2K
        case 0x0100:     // 1xRTT
            uiRet = RADIO_TECH_1xRTT;        // 1xRTT
            break;
        case 0x0200:     // HRPD
            uiRet = RADIO_TECH_EVDO_A;        // EVDO_A
            break;
        case 0x0400:     // EHRPD
            uiRet = RADIO_TECH_EHRPD;         // EHRPD
            break;
        //for LTE
        case 0x1000:     // LTE
        case 0x2000:     // LTE_CA
        case 0x4000:
            uiRet = RADIO_TECH_LTE;       // LTE
            break;
        case 0x8000:
            uiRet = RADIO_TECH_NR;
            break;
        default:
            uiRet = RADIO_TECH_UNKNOWN;        // UNKNOWN
            break;
    }

    return (unsigned int)uiRet;
}

unsigned int RmcNetworkHandler::convertPSNetworkType(unsigned int uiResponse)
{
    unsigned int uiRet = 0;

    /* mapping */
    switch(uiResponse)
    {
        case 0x0001:     // GPRS
            uiRet = 1;        // GPRS
            break;
        case 0x0002:     // EDGE
            uiRet = 2;        // EDGE
            break;
        case 0x0004:     // UMTS
            uiRet = 3;        // UMTS
            break;
        case 0x0008:     // HSDPA
            uiRet = 9;        // HSDPA
            break;
        case 0x0010:     // HSUPA
            uiRet = 10;        // HSUPA
            break;
        case 0x0018:     // HSDPA_UPA
            uiRet = 11;       // HSPA
            break;
        case 0x0020:     // HSDPAP
        case 0x0030:     // HSDPAP_UPA
        case 0x0040:     // HSUPAP
        case 0x0048:     // HSUPAP_DPA
        case 0x0060:     // HSPAP
        case 0x0088:     // DC_DPA
        case 0x0098:     // DC_DPA_UPA
        case 0x00a0:     // DC_HSDPAP
        case 0x00b0:     // DC_HSDPAP_UPA
        case 0x00c8:     // DC_HSUPAP_DPA
        case 0x00e0:     // DC_HSPAP
            uiRet = 15;       // HSPAP
            break;
        // for C2K
        case 0x0100:     // 1xRTT
            uiRet = 6;        // 1xRTT
            break;
        case 0x0200:     // HRPD
            uiRet = 8;        // EVDO_A
            break;
        case 0x0400:     // EHRPD
            uiRet = 13;         // EHRPD
            break;
        //for LTE
        case 0x1000:     // LTE
        case 0x4000:     // LTE + NR
            uiRet = 14;
            break;
        case 0x2000:     // LTE_CA
            uiRet = 19;
            break;
        case 0x8000:
            uiRet = 20;   // NR
            break;
        default:
            uiRet = 0;        // UNKNOWN
            break;
    }

    return uiRet;
}

void RmcNetworkHandler::updateWfcState(int status) {
    logV(LOG_TAG, "%s, WFC state of slot%d:%d->%d", __FUNCTION__,
            m_slot_id, ril_wfc_reg_status[m_slot_id], status);
    pthread_mutex_lock(&s_wfcRegStatusMutex[m_slot_id]);
    ril_wfc_reg_status[m_slot_id] = status;  // cache wfc status
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_WFC_STATE,
            ril_wfc_reg_status[m_slot_id]);
    if (s_iwlan_in_legacy_mode) {
        setMSimProperty(m_slot_id, (char*)PROPERTY_WFC_STATE,
                ((status == 1) ? (char*)"1" : (char*)"0"));
    }
    pthread_mutex_unlock(&s_wfcRegStatusMutex[m_slot_id]);
}

int RmcNetworkHandler::getOperatorNamesFromNumericCode(
    char *code,
    char *longname,
    char *shortname,
    int max_length) {

    char *oper_code, *oper_lname, *oper_sname;

    if (max_length > MAX_OPER_NAME_LENGTH) {
        logE(LOG_TAG, "The buffer size %d is not valid. We only accept the length less than or equal to %d",
                 max_length, MAX_OPER_NAME_LENGTH);
        return -1;
    }
    oper_code = m_ril_nw_nitz_oper_code[m_slot_id];
    oper_lname = m_ril_nw_nitz_oper_lname[m_slot_id];
    oper_sname = m_ril_nw_nitz_oper_sname[m_slot_id];

    longname[0] = '\0';
    shortname[0] = '\0';

    pthread_mutex_lock(&ril_nw_nitzName_mutex[m_slot_id]);
    // logD(LOG_TAG, "Get ril_nw_nitzName_mutex in the getOperatorNamesFromNumericCode");
    // logD(LOG_TAG, "getOperatorNamesFromNumericCode code:%s oper_code:%s", code, oper_code);

    /* Check if there is a NITZ name*/
    /* compare if the operator code is the same with <oper>*/
    if(strcmp(code, oper_code) == 0) {
        /* there is a NITZ Operator Name */
        /* get operator code and name */
        /* set short name with long name when short name is null and long name isn't, and vice versa */
        int nlnamelen = strlen(oper_lname);
        int nsnamelen = strlen(oper_sname);
        if(nlnamelen != 0 && nsnamelen != 0) {
            strncpy(longname,oper_lname, max_length);
            strncpy(shortname, oper_sname, max_length);
        } else if(strlen(oper_sname) != 0) {
            strncpy(longname, oper_sname, max_length);
            strncpy(shortname, oper_sname, max_length);
        } else if(strlen(oper_lname) != 0) {
            strncpy(longname, oper_lname, max_length);
            strncpy(shortname, oper_lname, max_length);
        }
        logD(LOG_TAG, "Return NITZ Operator Name: %s %s %s, lname length: %d, sname length: %d",
                oper_code, oper_lname, oper_sname, nlnamelen, nsnamelen);
    } else {
        //strcpy(longname, code);
        //strcpy(shortname, code);
        getPLMNNameFromNumeric(code, longname, shortname, max_length);
    }
     pthread_mutex_unlock(&ril_nw_nitzName_mutex[m_slot_id]);
    return 0;
}

int RmcNetworkHandler::getEonsNamesFromNumericCode(
        char *code,
        unsigned int lac,
        char *longname,
        char *shortname,
        int max_length,
        int rat)
{
    int err;
    int skip;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    char *oper_lname = NULL, *oper_sname = NULL;
    int dcs_long = 0;
    int dcs_short = 0;
    char tmpLongName[MAX_OPER_NAME_LENGTH] = {0};
    char tmpShortName[MAX_OPER_NAME_LENGTH] = {0};

    if (eons_info[m_slot_id].eons_status == EONS_INFO_RECEIVED_ENABLED &&
        (lac < 0xFFFFFF)) {
        if (isNrSupported()) {
            p_response = atSendCommandSingleline(String8::format("AT+EONS=2,\"%s\",%d,%d", code, lac, (rat == 11 ? 2 : 1)), "+EONS");
        } else {
            p_response = atSendCommandSingleline(String8::format("AT+EONS=2,\"%s\",%d", code, lac), "+EONS");
        }
        err = p_response->getError();
        if (err != 0 || p_response->getSuccess() == 0 ||
                p_response->getIntermediates() == NULL) {
            logE(LOG_TAG, "EONS got error response");
            goto error;
        } else {
            line = p_response->getIntermediates();
            line->atTokStart(&err);
            if (err >= 0) {
                skip = line->atTokNextint(&err);
                /* <Result> */
                if (err >= 0 && skip == 1) {
                    /* <dcs_long> */
                    dcs_long = line->atTokNextint(&err);
                    if (err < 0) goto error;

                    /* <long_oper> */
                    oper_lname = line->atTokNextstr(&err);
                    logD(LOG_TAG, "EONS Get operator long %s", oper_lname);
                    if (err < 0) goto error;
                    strncpy(tmpLongName, oper_lname, MAX_OPER_NAME_LENGTH);
                    tmpLongName[MAX_OPER_NAME_LENGTH-1] = '\0';
                    if (dcs_long == 1) {
                        convertToUtf8String(tmpLongName, MAX_OPER_NAME_LENGTH);
                    }

                    /* <dcs_short> */
                    dcs_short = line->atTokNextint(&err);
                    if (err < 0) goto error;

                    /* <short_oper> */
                    oper_sname = line->atTokNextstr(&err);
                    logD(LOG_TAG, "EONS Get operator short %s", oper_sname);
                    if (err < 0) goto error;
                    strncpy(tmpShortName, oper_sname, MAX_OPER_NAME_LENGTH);
                    tmpShortName[MAX_OPER_NAME_LENGTH-1] = '\0';
                    if (dcs_short == 1) {
                        convertToUtf8String(tmpShortName, MAX_OPER_NAME_LENGTH);
                    }

                    // check long_oper and short_oper are valid.
                    if((strcmp(tmpLongName, "") == 0)
                            && (strcmp(tmpShortName, "") == 0)){
                        logD(LOG_TAG, "EONS Get invalid operator name");
                        goto error;
                    }
                } else {
                    goto error;
                }
                /* Copy operator name */
                strncpy(longname, tmpLongName, max_length-1);
                strncpy(shortname, tmpShortName, max_length-1);
            }
        }

        logD(LOG_TAG, "Return EONS Operator Name: %s %s %s",
                code,
                tmpLongName,
                tmpShortName);
        return 0;
    }
error:
    return -1;
}

int RmcNetworkHandler::getPLMNNameFromNumeric(char *numeric, char *longname, char *shortname, int max_length) {
    int i = 0;
    int length_ts25 = sizeof(s_mtk_ts25_table)/sizeof(s_mtk_ts25_table[0]);
    int length_plmn = sizeof(s_mtk_plmn_table)/sizeof(s_mtk_plmn_table[0]);
    const SpnTable* spn = NULL;
    longname[0] = '\0';
    shortname[0] = '\0';

    // use the previous one
    if (strcmp(cache_spn_table[m_slot_id].mccMnc, numeric) == 0) {
        strncpy(longname, cache_spn_table[m_slot_id].spn, max_length);
        strncpy(shortname, cache_spn_table[m_slot_id].short_name, max_length);
        return 0;
    }

    // TS.25 table
    for (i=0; i < length_ts25; i++) {
        if (0 == strcmp(numeric, s_mtk_ts25_table[i].mccMnc))
        {
            spn = &s_mtk_ts25_table[i];
        }
    }

    // MTK customization PLMN name
    for (i=0; i < length_plmn; i++) {
        if (0 == strcmp(numeric, s_mtk_plmn_table[i].mccMnc))
        {
            spn = &s_mtk_plmn_table[i];
        }
    }

    if (spn != NULL) {
        strncpy(longname, spn->spn, max_length);
        strncpy(shortname, spn->short_name, max_length);

        // backup the spn for next use
        strncpy(cache_spn_table[m_slot_id].mccMnc, spn->mccMnc, 8);
        strncpy(cache_spn_table[m_slot_id].spn, spn->spn, MAX_OPER_NAME_LENGTH+2);
        strncpy(cache_spn_table[m_slot_id].short_name, spn->short_name, MAX_OPER_NAME_LENGTH+2);
        return 0;
    }
    strncpy(longname, numeric, max_length);
    strncpy(shortname, numeric, max_length);
    logV(LOG_TAG, "getPLMNNameFromNumeric: not found %s", numeric);
    return -1;
}

int RmcNetworkHandler::getOperatorNamesFromNumericCode(
    char *code,
    unsigned int lac,
    char *longname,
    char *shortname,
    int max_length,
    int rat)
{
    getOperatorNamesFromNumericCodeByDisplay(code, lac,
            longname, shortname, max_length, DISPLAY_ALL, rat);
    return 0;
}

int RmcNetworkHandler::getOperatorNamesFromNumericCodeByDisplay(
    char *code,
    unsigned int lac,
    char *longname,
    char *shortname,
    int max_length,
    int display,
    int rat)
{
    int err;
    char *oper_code, *oper_lname, *oper_sname;

    if (max_length > MAX_OPER_NAME_LENGTH) {
        logE(LOG_TAG, "The buffer size %d is not valid. We only accept the length less than or equal to %d",
             max_length, MAX_OPER_NAME_LENGTH);
        return -1;
    }

    longname[0] = '\0';
    shortname[0] = '\0';

    /* To support return EONS if available in RIL_REQUEST_OPERATOR START */
    if ((display & DISPLAY_EONS) == DISPLAY_EONS) {
        if (eons_info[m_slot_id].eons_status == EONS_INFO_RECEIVED_ENABLED) {
            err = getEonsNamesFromNumericCode(
                    code, lac, longname, shortname, max_length, rat);
            if (err == 0) {
                logD(LOG_TAG, "Get EONS %s, %s", longname, shortname);
                return 0;
            }
        }
        String8 ons = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_ONS);
        String8 onsShort = getMclStatusManager()->getString8Value(RFX_STATUS_KEY_SHORT_ONS);
        if (isHome(mdEreg[m_slot_id]->stat, mdEgreg[m_slot_id]->stat) && strcmp(code, mdEops[m_slot_id]->oper) == 0
                && (!ons.isEmpty() || !onsShort.isEmpty())) {
            strncpy(longname, ons.string(), max_length);
            longname[max_length -1] = '\0';
            strncpy(shortname, onsShort.string(), max_length);
            shortname[max_length -1] = '\0';
            logI(LOG_TAG, "Get ONS %s, %s", longname, shortname);
            return 0;
        }
        logV(LOG_TAG, "no ONS: ons empty[%d] onshort empty[%d]",
                ons.isEmpty(), onsShort.isEmpty());
    }
    /* To support return EONS if available in RIL_REQUEST_OPERATOR END */

    if ((display & DISPLAY_NITZ) == DISPLAY_NITZ) {
        int hasNitz = 0;
        oper_code = m_ril_nw_nitz_oper_code[m_slot_id];
        oper_lname = m_ril_nw_nitz_oper_lname[m_slot_id];
        oper_sname = m_ril_nw_nitz_oper_sname[m_slot_id];

        pthread_mutex_lock(&ril_nw_nitzName_mutex[m_slot_id]);

        /* Check if there is a NITZ name*/
        /* compare if the operator code is the same with <oper>*/
        if(strcmp(code, oper_code) == 0) {
            hasNitz = 1;
            /* there is a NITZ Operator Name*/
            /*get operator code and name*/
            /*set short name with long name when short name is null and long name isn't, and vice versa*/
            int nlnamelen = strlen(oper_lname);
            int nsnamelen = strlen(oper_sname);
            if (nlnamelen != 0 && nsnamelen != 0) {
                strncpy(longname,oper_lname, max_length);
                strncpy(shortname, oper_sname, max_length);
            } else if (strlen(oper_sname) != 0) {
                strncpy(longname, oper_sname, max_length);
                strncpy(shortname, oper_sname, max_length);
            } else if (strlen(oper_lname) != 0) {
                strncpy(longname, oper_lname, max_length);
                strncpy(shortname, oper_lname, max_length);
            }
            logD(LOG_TAG, "Return NITZ Operator Name: %s %s %s, lname length: %d, sname length: %d", oper_code,
                                                                                            oper_lname,
                                                                                            oper_sname,
                                                                                            nlnamelen,
                                                                                            nsnamelen);
        }
        pthread_mutex_unlock(&ril_nw_nitzName_mutex[m_slot_id]);
        if (hasNitz == 1) return 0;
    }

    if ((display & DISPLAY_TS25) == DISPLAY_TS25) {
        getPLMNNameFromNumeric(code, longname, shortname, max_length);
    }
    return 0;
}

void RmcNetworkHandler::isFemtoCell(int regState, int cid, int act) {
    int isFemtocell = 0;
    int eNBid = 0;
    String8 oper(getMclStatusManager()->getString8Value(RFX_STATUS_KEY_OPERATOR));
    eNBid = cid >> 8;
    if (isOp12Plmn(oper.string()) == true
            && act == 14  // LTE
            && regState == 1  // in home service
            && eNBid >= 1024000 && eNBid <= 1048575) { // OP12: 0xFA000 - 0xFFFFF
        isFemtocell = 2;
    }
    pthread_mutex_lock(&ril_nw_femtoCell_mutex);
    if (isFemtocell != femto_cell_cache[m_slot_id]->is_femtocell) {
        femto_cell_cache[m_slot_id]->is_femtocell = isFemtocell;
        pthread_mutex_unlock(&ril_nw_femtoCell_mutex);
        updateFemtoCellInfo();
        pthread_mutex_lock(&ril_nw_femtoCell_mutex);
    }
    pthread_mutex_unlock(&ril_nw_femtoCell_mutex);
}

void RmcNetworkHandler::updateFemtoCellInfo() {
    int err;
    unsigned long i;
    char *responseStr[10] = { 0 };
    char *shortOperName = (char *) calloc(1, sizeof(char)*MAX_OPER_NAME_LENGTH);
    char *longOperName = (char *) calloc(1, sizeof(char)*MAX_OPER_NAME_LENGTH);

    if (shortOperName == NULL || longOperName == NULL) {
        logE(LOG_TAG, "updateFemtoCellInfo calloc fail");
        if (longOperName != NULL) free(longOperName);
        if (shortOperName != NULL) free(shortOperName);
        return;
    }
    pthread_mutex_lock(&ril_nw_femtoCell_mutex);
    asprintf(&responseStr[0], "%d", femto_cell_cache[m_slot_id]->domain);
    asprintf(&responseStr[1], "%d", femto_cell_cache[m_slot_id]->state);
    asprintf(&responseStr[3], "%d", femto_cell_cache[m_slot_id]->plmn_id);

    err = getOperatorNamesFromNumericCode(responseStr[3], longOperName, shortOperName, MAX_OPER_NAME_LENGTH);
    if(err < 0) {
        asprintf(&responseStr[2], "");
    } else {
        asprintf(&responseStr[2], "%s", longOperName);
    }
    free(shortOperName);
    free(longOperName);

    asprintf(&responseStr[4], "%d", femto_cell_cache[m_slot_id]->act);

    if (femto_cell_cache[m_slot_id]->is_femtocell == 2) { // for LTE femtocell
        asprintf(&responseStr[5], "%d", femto_cell_cache[m_slot_id]->is_femtocell);
    } else if (femto_cell_cache[m_slot_id]->is_csg_cell == 1) { // for GSM femtocell
        asprintf(&responseStr[5], "%d", femto_cell_cache[m_slot_id]->is_csg_cell);
    } else {
        asprintf(&responseStr[5], "0");
    }
    asprintf(&responseStr[6], "%d", femto_cell_cache[m_slot_id]->csg_id);
    asprintf(&responseStr[7], "%d", femto_cell_cache[m_slot_id]->csg_icon_type);
    asprintf(&responseStr[8], "%s", femto_cell_cache[m_slot_id]->hnbName);
    asprintf(&responseStr[9], "%d", femto_cell_cache[m_slot_id]->cause);
    pthread_mutex_unlock(&ril_nw_femtoCell_mutex);

    // <domain>,<state>,<lognname>,<plmn_id>,<act>,<is_csg_cell/is_femto_cell>,<csg_id>,<csg_icon_type>,<hnb_name>,<cause> */
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_FEMTOCELL_INFO,
            m_slot_id, RfxStringsData(responseStr, 10));
    responseToTelCore(urc);
    // free memory
    for (i=0 ; i<(sizeof(responseStr)/sizeof(char*)) ; i++) {
        if (responseStr[i]) {
            // logD(LOG_TAG, "free responseStr[%d]=%s", i, responseStr[i]);
            free(responseStr[i]);
            responseStr[i] = NULL;
        }
    }
    return;
}

void RmcNetworkHandler::printSignalStrengthCache(char* source) {
    logD(LOG_TAG,
        "%s,"
        " gsm_signal_strength: %d,"
        " gsm_bit_error_rate: %d,"
        " gsm_timing_advance: %d,"
        " cdma_dbm: %d,"
        " cdma_ecio: %d,"
        " evdo_dbm: %d,"
        " evdo_ecio: %d,"
        " evdo_snr: %d,"
        " lte_signal_strength: %d,"
        " lte_rsrp: %d,"
        " lte_rsrq: %d,"
        " lte_rssnr: %d,"
        " lte_cqi: %d,"
        " lte_timing_advance: %d,"
        " tdscdma_signal_strength: %d,"
        " tdscdma_bit_error_rate: %d,"
        " tdscdma_rscp: %d,"
        " wcdma_signal_strength: %d,"
        " wcdma_bit_error_rate: %d,"
        " wcdma_scdma_rscp: %d,"
        " wcdma_ecno: %d,"
        " ssRsrp: %d,"
        " ssRsrq: %d,"
        " ssSinr: %d,"
        " csiRsrp: %d,"
        " csiRsrq: %d,"
        " csiSinr: %d",
        source,
        signal_strength_cache[m_slot_id]-> gsm_signal_strength,
        signal_strength_cache[m_slot_id]-> gsm_bit_error_rate,
        signal_strength_cache[m_slot_id]-> gsm_timing_advance,
        signal_strength_cache[m_slot_id]-> cdma_dbm,
        signal_strength_cache[m_slot_id]-> cdma_ecio,
        signal_strength_cache[m_slot_id]-> evdo_dbm,
        signal_strength_cache[m_slot_id]-> evdo_ecio,
        signal_strength_cache[m_slot_id]-> evdo_snr,
        signal_strength_cache[m_slot_id]-> lte_signal_strength,
        signal_strength_cache[m_slot_id]-> lte_rsrp,
        signal_strength_cache[m_slot_id]-> lte_rsrq,
        signal_strength_cache[m_slot_id]-> lte_rssnr,
        signal_strength_cache[m_slot_id]-> lte_cqi,
        signal_strength_cache[m_slot_id]-> lte_timing_advance,
        signal_strength_cache[m_slot_id]-> tdscdma_signal_strength,
        signal_strength_cache[m_slot_id]-> tdscdma_bit_error_rate,
        signal_strength_cache[m_slot_id]-> tdscdma_rscp,
        signal_strength_cache[m_slot_id]-> wcdma_signal_strength,
        signal_strength_cache[m_slot_id]-> wcdma_bit_error_rate,
        signal_strength_cache[m_slot_id]-> wcdma_scdma_rscp,
        signal_strength_cache[m_slot_id]-> wcdma_ecno,
        signal_strength_cache[m_slot_id]-> ssRsrp,
        signal_strength_cache[m_slot_id]-> ssRsrq,
        signal_strength_cache[m_slot_id]-> ssSinr,
        signal_strength_cache[m_slot_id]-> csiRsrp,
        signal_strength_cache[m_slot_id]-> csiRsrq,
        signal_strength_cache[m_slot_id]-> csiSinr);
}

RIL_RAT_GROUP RmcNetworkHandler::getVoiceRatGroup(int act) {
    RIL_RAT_GROUP result = RAT_GROUP_UNKNOWN;
    /* mapping */
    switch(act)
    {
        case 0x0001:     // GPRS
        case 0x0002:     // EDGE
            result = RAT_GROUP_2G;
            break;
        case 0x0004:     // UMTS
        case 0x0008:     // HSDPA
        case 0x0010:     // HSUPA
        case 0x0018:     // HSDPA_UPA
        case 0x0020:     // HSDPAP
        case 0x0030:     // HSDPAP_UPA
        case 0x0040:     // HSUPAP
        case 0x0048:     // HSUPAP_DPA
        case 0x0060:     // HSPAP
        case 0x0088:     // DC_DPA
        case 0x0098:     // DC_DPA_UPA
        case 0x00a0:     // DC_HSDPAP
        case 0x00b0:     // DC_HSDPAP_UPA
        case 0x00c8:     // DC_HSUPAP_DPA
        case 0x00e0:     // DC_HSPAP
            result = RAT_GROUP_3G;
            break;
        // for C2K
        case 0x0100:     // 1xRTT
        case 0x0200:     // HRPD
        case 0x0400:     // EHRPD
            result = RAT_GROUP_UNKNOWN;
            break;
        //for LTE
        case 0x1000:     // LTE
        case 0x2000:     // LTE_CA
            result = RAT_GROUP_4G;
            break;
        default:
            result = RAT_GROUP_UNKNOWN;
            break;
    }
    return result;
}

RIL_RAT_GROUP RmcNetworkHandler::getDataRatGroup(int act) {
    RIL_RAT_GROUP result = RAT_GROUP_UNKNOWN;
    /* mapping */
    switch(act)
    {
        case 0x0001:     // GPRS
        case 0x0002:     // EDGE
            result = RAT_GROUP_2G;
            break;
        case 0x0004:     // UMTS
        case 0x0008:     // HSDPA
        case 0x0010:     // HSUPA
        case 0x0018:     // HSDPA_UPA
        case 0x0020:     // HSDPAP
        case 0x0030:     // HSDPAP_UPA
        case 0x0040:     // HSUPAP
        case 0x0048:     // HSUPAP_DPA
        case 0x0060:     // HSPAP
        case 0x0088:     // DC_DPA
        case 0x0098:     // DC_DPA_UPA
        case 0x00a0:     // DC_HSDPAP
        case 0x00b0:     // DC_HSDPAP_UPA
        case 0x00c8:     // DC_HSUPAP_DPA
        case 0x00e0:     // DC_HSPAP
            result = RAT_GROUP_3G;
            break;
        // for C2K
        case 0x0100:     // 1xRTT
        case 0x0200:     // HRPD
        case 0x0400:     // EHRPD
            result = RAT_GROUP_UNKNOWN;
            break;
        //for LTE
        case 0x1000:     // LTE
        case 0x2000:     // LTE_CA
            result = RAT_GROUP_4G;
            break;
        default:
            result = RAT_GROUP_UNKNOWN;
            break;
    }
    return result;
}

void RmcNetworkHandler::isNeedNotifyStateChanged() {
    int radio_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, 0);
    bool mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    logV(LOG_TAG, "isNeedNotifyStateChanged: %d", mIsNeedNotifyState[m_slot_id]);
    if (mIsNeedNotifyState[m_slot_id] && !mdOff && radio_state != RADIO_STATE_UNAVAILABLE) {
        sp<RfxMclMessage> urc;
        mIsNeedNotifyState[m_slot_id] = false;
        mIsNeedNotifyState_Id[m_slot_id] = (mIsNeedNotifyState_Id[m_slot_id] + 1) % 1000;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        // response to TeleCore
        responseToTelCore(urc);

        logV(LOG_TAG, "send RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM with %d", mIsNeedNotifyState_Id[m_slot_id]);
        // start a watch dog to check whether system got our notification or not.
        int t[2] = {0};
        t[0] = mIsNeedNotifyState_Id[m_slot_id];
        sendEvent(RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM, RfxIntsData(t, 2), RIL_CMD_PROXY_3, m_slot_id,
                -1, -1, s2ns(RETRY_NOTIFY_SYSTEM_TIMER), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
    }
}

void RmcNetworkHandler::setLastValidPlmn(const char* plmn) {
    if (plmn != NULL &&
            (strncmp(plmn, "000000", 6) != 0) &&
            (plmn[0] >= '0' && plmn[0] <= '9')) {
        last_valid_plmn_time[m_slot_id] = systemTime(SYSTEM_TIME_MONOTONIC);
        strncpy(last_valid_plmn[m_slot_id], plmn, 7);
        last_valid_plmn[m_slot_id][7] = '\0';
        // logD(LOG_TAG, "setLastValidPlmn: %ld, %s",
        //         (last_valid_plmn_time[m_slot_id]/1000000000), last_valid_plmn[m_slot_id]);
    }
}

bool RmcNetworkHandler::getLastValidPlmn(char* plmn, int size) {
    nsecs_t current_time = systemTime(SYSTEM_TIME_MONOTONIC);
    // logD(LOG_TAG, "getLastValidPlmn: %ld, %s",
    //         (current_time/10000000000), last_valid_plmn[m_slot_id]);
    if (plmn != NULL && size >= 8
            && last_valid_plmn[m_slot_id][0] != '\0'
            && strlen(last_valid_plmn[m_slot_id]) >= 5) {
        plmn[0] = '\0';
        if (current_time - last_valid_plmn_time[m_slot_id] < LIFE_VALID_PLMN) {
            strncpy(plmn, last_valid_plmn[m_slot_id], 7);
            plmn[7] = '\0';
            return true;
        }
    }
    return false;
}

bool RmcNetworkHandler::getCdmaPlmn(char* plmn, int size) {
    int err = 0;
    char* cdma_plmn = NULL;
    CDMA_CELL_LOCATION_INFO cdma_location;
    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
    if (mViaHandler != NULL) {
        memset(&cdma_location, 0, sizeof(CDMA_CELL_LOCATION_INFO));
        err = mViaHandler->getCdmaLocationInfo(this, &cdma_location);
        if (err < 0) goto error;
        // Mcc is three digit int, 111 is invalid value.
        if (cdma_location.mcc > 99 && cdma_location.mcc < 1000 && cdma_location.mcc != 111) {
            if (cdma_location.mnc < 10) {
                // Mnc less than two digits, prefixed with the character "0"
                err = asprintf(&cdma_plmn, "%d%02d", cdma_location.mcc, cdma_location.mnc);
            } else {
                err = asprintf(&cdma_plmn, "%d%d", cdma_location.mcc, cdma_location.mnc);
            }
            if (err < 0) goto error;
            // The length of plmn is usually 5~6 digits, byte aligned, the minimum length is 8.
            if (plmn != NULL && size >= 8 && cdma_plmn != NULL && strlen(cdma_plmn) >= 5) {
                strncpy(plmn, cdma_plmn, 7);
                plmn[7] = '\0';
                free(cdma_plmn);
                cdma_plmn = NULL;
                logV(LOG_TAG, "getCdmaPlmn: plmn=%s", plmn);
                return true;
            } else {
                goto error;
            }
        } else {
            goto error;
        }
    } else {
        goto error;
    }

error:
    logE(LOG_TAG, "%s, return because error.", __FUNCTION__);
    if (cdma_plmn != NULL) {
        free(cdma_plmn);
        cdma_plmn = NULL;
    }
    return false;
}

bool RmcNetworkHandler::isRilTestMode() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    return strcmp(property_value, "1") == 0;
}

bool RmcNetworkHandler::supportMccMncUrc() {
    bool support = false;
    if (isRilTestMode()) {
        support = true;
    }
    return support;
}

bool RmcNetworkHandler::isNrSupported() {
    /* check if nr is supported */
    char NR_VER[] = "5G_SUPPORTED";
    if (nr_support < 0) {
        nr_support = getFeatureVersion(NR_VER, 0);
    }
    return (nr_support == 1) ? true: false;
}


int RmcNetworkHandler::isGcfTestMode() {
    int isTestMode = 0;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    memset(prop,0,sizeof(prop));
    rfx_property_get("vendor.gsm.gcf.testmode", prop, "0");
    isTestMode = atoi(prop);
    return isTestMode;
}

int RmcNetworkHandler::isTestSim(int slotId) {
    int ret = 0;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get(PROPERTY_RIL_TEST_SIM[slotId], prop, "0");
    ret = atoi(prop);
    return ret;
}

bool RmcNetworkHandler::isNetworkScanFullSupport() {
    return s_enwscn_support;
    //return false;
}

bool RmcNetworkHandler::isAPInCall() {
    bool ret = false;

    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (getMclStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0) > 0) {
            ret = true;
        }
    }
    return ret;
}

void RmcNetworkHandler::refreshEcellCache() {
    int err = 0;
    int num = 0;
    RIL_CellInfo_v12 *response = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> resp;
    RfxAtLine* line;
    sp<RfxMclMessage> urc;

    p_response = atSendCommandSingleline("AT+ECELL", "+ECELL:");

    // +ECELL: <num_of_cell>...
    if (p_response->getError() < 0 || p_response->getSuccess() == 0)
        goto error;

    line = p_response->getIntermediates();

    line->atTokStart(&err);
    if (err < 0) goto error;

    num = line->atTokNextint(&err);
    if (err < 0) goto error;
    if (num < 1) {
        logD(LOG_TAG, "No cell info listed, num=%d", num);
        goto error;
    }

    response = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
    memset(response, 0, num * sizeof(RIL_CellInfo_v12));

    err = getCellInfoListV12(line, num, response);
    if (err == 0) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CELL_INFO_LIST,
                m_slot_id, RfxCellInfoData(response, num * sizeof(RIL_CellInfo_v12)));
        responseToTelCore(urc);
    }
    freeOperatorNameOfCellInfo(num, response);
error:
    MD_ECELL *mMdEcell = mdEcell[m_slot_id];
    dumpEcellCache(__func__, mMdEcell);
    return;
}

void RmcNetworkHandler::freeOperatorNameOfCellInfo(int num, RIL_CellInfo_v12 *response) {
    if (response == NULL) return;
    for (int i = 0; i < num; i++) {
        switch (response[i].cellInfoType) {
            case RIL_CELL_INFO_TYPE_GSM: {
                if (response[i].CellInfo.gsm.cellIdentityGsm.operName.long_name)
                    free(response[i].CellInfo.gsm.cellIdentityGsm.operName.long_name);
                if (response[i].CellInfo.gsm.cellIdentityGsm.operName.short_name)
                    free(response[i].CellInfo.gsm.cellIdentityGsm.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_WCDMA: {
                if (response[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name)
                    free(response[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                if (response[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name)
                    free(response[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                break;
            }

            case RIL_CELL_INFO_TYPE_CDMA: {
                // It's NULL now.
                break;
            }

            case RIL_CELL_INFO_TYPE_LTE: {
                if (response[i].CellInfo.lte.cellIdentityLte.operName.long_name)
                    free(response[i].CellInfo.lte.cellIdentityLte.operName.long_name);
                if (response[i].CellInfo.lte.cellIdentityLte.operName.short_name)
                    free(response[i].CellInfo.lte.cellIdentityLte.operName.short_name);
                break;
            }


            case RIL_CELL_INFO_TYPE_NR: {
                if (response[i].CellInfo.nr.cellidentity.operName.long_name)
                    free(response[i].CellInfo.nr.cellidentity.operName.long_name);
                if (response[i].CellInfo.nr.cellidentity.operName.short_name)
                    free(response[i].CellInfo.nr.cellidentity.operName.short_name);
                break;
            }
            case RIL_CELL_INFO_TYPE_TD_SCDMA:
                //TD_SCDMA is impossible EXIST here
                RFX_ASSERT(false);
                TELEPHONYWARE_FALLTHROUGH;
            case RIL_CELL_INFO_TYPE_NONE:
                break;
        }
    }
    return;
}

void RmcNetworkHandler::dumpEcellCache(const char* fun, MD_ECELL *mMdEcell) {
    char* ecell_cid_s = getMask(mMdEcell->cid);
    char* ecell_tac_s = getMask(mMdEcell->lacTac);
    char* ecell_pci_s = getMask(mMdEcell->pscPci);
    if (!ecell_cid_s || !ecell_tac_s || !ecell_pci_s) {
        logE(LOG_TAG, "[%s] can not getMask due to low memory", fun);
        goto error;
    }
    logD(LOG_TAG, "mMdEcell act=%d, cid=%s, mcc=%d, mnc=%d, lacTac=%s, pscPci=%s,"
        " sig1=%d, sig2=%d, rsrp=%d, rsrq=%d, timingAdvance=%d,"
        " rssnr=%d, cqi=%d, bsic=%d, arfcn=%d",
        mMdEcell->act, ecell_cid_s, mMdEcell->mcc, mMdEcell->mnc, ecell_tac_s,
        ecell_pci_s, mMdEcell->sig1, mMdEcell->sig2, mMdEcell->rsrp, mMdEcell->rsrq,
        mMdEcell->timingAdvance, mMdEcell->rssnr, mMdEcell->cqi, mMdEcell->bsic,
        mMdEcell->arfcn);
error:
    if (ecell_cid_s) free(ecell_cid_s);
    if (ecell_tac_s) free(ecell_tac_s);
    if (ecell_pci_s) free(ecell_pci_s);
}

void RmcNetworkHandler::dumpEregCache(const char* fun, MD_EREG *mMdEreg, MD_ECELL *mMdEcell) {
    dumpEregCache(fun, mMdEreg, mMdEcell, false);
}

void RmcNetworkHandler::dumpEregCache(const char* fun, MD_EREG *mMdEreg, MD_ECELL *mMdEcell, bool debugV) {
    char* lac_s = getMask(mMdEreg->lac);
    char* cid_s = getMask(mMdEreg->ci);
    char* ecell_cid_s = getMask(mMdEcell->cid);
    char* ecell_tac_s = getMask(mMdEcell->lacTac);
    char* ecell_pci_s = getMask(mMdEcell->pscPci);
    if (!lac_s || !cid_s || !ecell_cid_s || !ecell_tac_s || !ecell_pci_s) {
        logE(LOG_TAG, "[%s] can not getMask due to low memory", fun);
        goto error;
    }
    if (debugV) {
        logV(LOG_TAG, "[%s] mMdEreg n=%d, stat=%d, lac=%s, ci=%s, eAct=%d"
                ", nwk_existence=%d, roam_indicator=%d, cause_type=%d, reject_cause=%d"
                " mMdEcell act=%d, cid=%s, mcc=%d, mnc=%d, lacTac=%s, pscPci=%s,"
                " sig1=%d, sig2=%d, rsrp=%d, rsrq=%d, timingAdvance=%d,"
                " rssnr=%d, cqi=%d, bsic=%d, arfcn=%d",
                fun,
                mMdEreg->n, mMdEreg->stat, lac_s, cid_s, mMdEreg->eAct,
                mMdEreg->nwk_existence, mMdEreg->roam_indicator, mMdEreg->cause_type,
                mMdEreg->reject_cause,
                mMdEcell->act, ecell_cid_s, mMdEcell->mcc, mMdEcell->mnc, ecell_tac_s,
                ecell_pci_s, mMdEcell->sig1, mMdEcell->sig2, mMdEcell->rsrp, mMdEcell->rsrq,
                mMdEcell->timingAdvance, mMdEcell->rssnr, mMdEcell->cqi, mMdEcell->bsic,
                mMdEcell->arfcn);
    } else {
        logD(LOG_TAG, "[%s] mMdEreg n=%d, stat=%d, lac=%s, ci=%s, eAct=%d"
                ", nwk_existence=%d, roam_indicator=%d, cause_type=%d, reject_cause=%d"
                " mMdEcell act=%d, cid=%s, mcc=%d, mnc=%d, lacTac=%s, pscPci=%s,"
                " sig1=%d, sig2=%d, rsrp=%d, rsrq=%d, timingAdvance=%d,"
                " rssnr=%d, cqi=%d, bsic=%d, arfcn=%d",
                fun,
                mMdEreg->n, mMdEreg->stat, lac_s, cid_s, mMdEreg->eAct,
                mMdEreg->nwk_existence, mMdEreg->roam_indicator, mMdEreg->cause_type,
                mMdEreg->reject_cause,
                mMdEcell->act, ecell_cid_s, mMdEcell->mcc, mMdEcell->mnc, ecell_tac_s,
                ecell_pci_s, mMdEcell->sig1, mMdEcell->sig2, mMdEcell->rsrp, mMdEcell->rsrq,
                mMdEcell->timingAdvance, mMdEcell->rssnr, mMdEcell->cqi, mMdEcell->bsic,
                mMdEcell->arfcn);
    }
error:
    if (lac_s) free(lac_s);
    if (cid_s) free(cid_s);
    if (ecell_cid_s) free(ecell_cid_s);
    if (ecell_tac_s) free(ecell_tac_s);
    if (ecell_pci_s) free(ecell_pci_s);
}

void RmcNetworkHandler::dumpEgregCache(const char* fun, MD_EGREG *mMdEgreg, MD_ECELL *mMdEcell) {
     dumpEgregCache(fun, mMdEgreg, mMdEcell, false);
}

void RmcNetworkHandler::dumpEgregCache(const char* fun, MD_EGREG *mMdEgreg, MD_ECELL *mMdEcell, bool debugV) {
    char* lac_s = getMask(mMdEgreg->lac);
    char* ci_s = getMask(mMdEgreg->ci);
    char* ecell_cid_s = getMask(mMdEcell->cid);
    char* ecell_lac_s = getMask(mMdEcell->lacTac);
    char* ecell_pci_s = getMask(mMdEcell->pscPci);
    if (!lac_s || !ci_s || !ecell_cid_s || !ecell_lac_s || !ecell_pci_s) {
        logE(LOG_TAG, "[%s] can not getMask due to low memory", fun);
        goto error;
    }
    if (debugV) {
        logV(LOG_TAG, "[%s] mMdEgreg n=%d, stat=%d, lac=%s, ci=%s, eAct=%d, rac=%d"
                ", nwk_existence=%d, roam_indicator=%d, cause_type=%d,"
                " reject_cause=%d, dcnr_restricted=%d, endc_sib=%d, endc_available=%d.  "
                " mMdEcell act=%d, cid=%s, mcc=%d, mnc=%d, lacTac=%s, pscPci=%s,"
                " sig1=%d, sig2=%d, rsrp=%d, rsrq=%d, timingAdvance=%d,"
                " rssnr=%d, cqi=%d, bsic=%d, arfcn=%d",
                fun,
                mMdEgreg->n, mMdEgreg->stat, lac_s, ci_s, mMdEgreg->eAct,
                mMdEgreg->rac,mMdEgreg->nwk_existence, mMdEgreg->roam_indicator,
                mMdEgreg->cause_type, mMdEgreg->reject_cause, mMdEgreg->dcnr_restricted,
                mMdEgreg->endc_sib, mMdEgreg->endc_available,
                mMdEcell->act, ecell_cid_s, mMdEcell->mcc, mMdEcell->mnc, ecell_lac_s,
                ecell_pci_s, mMdEcell->sig1, mMdEcell->sig2, mMdEcell->rsrp, mMdEcell->rsrq,
                mMdEcell->timingAdvance, mMdEcell->rssnr, mMdEcell->cqi, mMdEcell->bsic,
                mMdEcell->arfcn);
    } else {
        logD(LOG_TAG, "[%s] mMdEgreg n=%d, stat=%d, lac=%s, ci=%s, eAct=%d, rac=%d"
                ", nwk_existence=%d, roam_indicator=%d, cause_type=%d,"
                " reject_cause=%d, dcnr_restricted=%d, endc_sib=%d, endc_available=%d.  "
                " mMdEcell act=%d, cid=%s, mcc=%d, mnc=%d, lacTac=%s, pscPci=%s,"
                " sig1=%d, sig2=%d, rsrp=%d, rsrq=%d, timingAdvance=%d,"
                " rssnr=%d, cqi=%d, bsic=%d, arfcn=%d",
                fun,
                mMdEgreg->n, mMdEgreg->stat, lac_s, ci_s, mMdEgreg->eAct,
                mMdEgreg->rac,mMdEgreg->nwk_existence, mMdEgreg->roam_indicator,
                mMdEgreg->cause_type, mMdEgreg->reject_cause, mMdEgreg->dcnr_restricted,
                mMdEgreg->endc_sib, mMdEgreg->endc_available,
                mMdEcell->act, ecell_cid_s, mMdEcell->mcc, mMdEcell->mnc, ecell_lac_s,
                ecell_pci_s, mMdEcell->sig1, mMdEcell->sig2, mMdEcell->rsrp, mMdEcell->rsrq,
                mMdEcell->timingAdvance, mMdEcell->rssnr, mMdEcell->cqi, mMdEcell->bsic,
                mMdEcell->arfcn);
    }
error:
    if (lac_s) free(lac_s);
    if (ci_s) free(ci_s);
    if (ecell_cid_s) free(ecell_cid_s);
    if (ecell_lac_s) free(ecell_lac_s);
    if (ecell_pci_s) free(ecell_pci_s);
}

// Have to call free() after getting the resp

char* RmcNetworkHandler::getMask(int i) {
    return getMask((unsigned long long) i);
}

char* RmcNetworkHandler::getMask(unsigned int i) {
    return getMask((unsigned long long) i);
}

char* RmcNetworkHandler::getMask(long long i) {
    return getMask((unsigned long long) i);
}

char* RmcNetworkHandler::getMask(unsigned long long i) {
    char* d = NULL;
    asprintf(&d, "%llX", i);
    if (d == NULL) {
        logE(LOG_TAG, "getMask error");
        return NULL;
    }
    size_t size_ = strlen(d);
    if (size_ == 1 || size_ == 2) {
        d[0] = '*';
        return d;
    }
    for (size_t i = 0; i < (size_/2); i++) {
        d[i] = '*';
    }
    return d;
}

void RmcNetworkHandler::dumpLteEndcMaxCapability(unsigned int slot_id) {
    char* lac_s = NULL;
    char* ci_s = NULL;
    for (unsigned int i = 0; i < MAX_LTE_ENDC_MAX_CAPABILITY; i++) {
        lac_s = getMask(lteMaxNr[i].cid);
        ci_s = getMask(lteMaxNr[i].lacTac);
        if (lac_s != NULL && ci_s != NULL) {
            logV(LOG_TAG, "dumpLteEndcMaxCapability[%d][%d] "
                "cid=%s,"
                "lacTac=%s,"
                "dcnr_restricted=%d,"
                "endc_sib=%d,"
                "endc_available=%d",
                slot_id,
                i,
                ci_s,
                lac_s,
                lteMaxNr[i].dcnr_restricted,
                lteMaxNr[i].endc_sib,
                lteMaxNr[i].endc_available);
        } else {
            logV(LOG_TAG, "dumpLteEndcMaxCapability[%d][%d] "
                "dcnr_restricted=%d,"
                "endc_sib=%d,"
                "endc_available=%d",
                slot_id,
                i,
                lteMaxNr[i].dcnr_restricted,
                lteMaxNr[i].endc_sib,
                lteMaxNr[i].endc_available);
        }
        if (lac_s) free(lac_s);
        if (ci_s) free(ci_s);
    }
}

int RmcNetworkHandler::checkNrCapability(unsigned int slotId) {
    // check type
    int nw_type = getMclStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_PREFERRED_NW_TYPE);
    // check at+e5gopt
    // check at+epsconfig
    int nr_sw_slot = getNonSlotMclStatusManager()->getIntValue(RFX_STATUS_KEY_NR_SWITCH_SLOT);
    int err = -1, num = 1, hasNrCap = -1;
    if ((nw_type >= 23 && nw_type <= 33) // network type has nr
            && ((nr_sw_slot == (slotId+1)) || nr_sw_slot == 0) // check if nr protocal or both 5g
            && endc_capability[slotId] == 1) { // check is_endc_supported from +ECAPABILITY
        sp<RfxAtResponse> p_response;
        RfxAtLine* line = NULL;
        p_response = atSendCommandSingleline("AT+E5GOPT?", "+E5GOPT:");
        if (p_response->getError() < 0 || p_response->getSuccess() == 0)
            goto finish;
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        if (err < 0) goto finish;
        num = line->atTokNextint(&err);
        if (err < 0) goto finish;
        current_5gopt_config[slotId] = num;
        if (num == 5 || num == 7) { // support nsa
            hasNrCap = 1;
        }
    }
    logV(LOG_TAG, "checkNrCapability(%d) type=%d, e5gopt=%d, nr_sw_slot=%d, endc_capability=%d",
            hasNrCap, nw_type, num, nr_sw_slot, endc_capability[slotId]);
finish:
    return hasNrCap;
}

static RIL_CellInfoType getCellInfoTypeRadioTechnology(int rat) {
    switch(rat) {
        case RADIO_TECH_GPRS:
        case RADIO_TECH_EDGE:
        case RADIO_TECH_GSM: {
            return RIL_CELL_INFO_TYPE_GSM;
        }
        case RADIO_TECH_UMTS:
        case RADIO_TECH_HSDPA:
        case RADIO_TECH_HSUPA:
        case RADIO_TECH_HSPA:
        case RADIO_TECH_HSPAP: {
            return RIL_CELL_INFO_TYPE_WCDMA;
        }
        case RADIO_TECH_IS95A:
        case RADIO_TECH_IS95B:
        case RADIO_TECH_1xRTT:
        case RADIO_TECH_EVDO_0:
        case RADIO_TECH_EVDO_A:
        case RADIO_TECH_EVDO_B:
        case RADIO_TECH_EHRPD: {
            return RIL_CELL_INFO_TYPE_CDMA;
        }
        case RADIO_TECH_LTE:
        case RADIO_TECH_LTE_CA: {
            return RIL_CELL_INFO_TYPE_LTE;
        }
        case RADIO_TECH_NR:
            return RIL_CELL_INFO_TYPE_NR;

        case RADIO_TECH_TD_SCDMA:
            //TD_SCDMA is impossible EXIST here
            RFX_ASSERT(false);
            TELEPHONYWARE_FALLTHROUGH;
        default: {
            break;
        }
    }
    return RIL_CELL_INFO_TYPE_NONE;
}

void RmcNetworkHandler::fillVoiceCellIdentity(RIL_CellIdentity_v16 &cellIdentity) {
    int err = 0;
    int mcc = 0;
    int mnc = 0;
    int mnc_len = 0;
    char *longname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char *shortname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char mccMnc[8] = {0};
    CellIdentityInfo identityInfo{INT_MAX, INT_MAX, 0, 0};
    bool useEcell = true;

    if (longname == NULL || shortname == NULL) {
        if (longname) freeSetNull(longname);
        if (shortname) freeSetNull(shortname);
        logE(LOG_TAG, "fillVoiceCellIdentity longname or shortname is null");
        return;
    }
    pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
    memcpy(mccMnc, op_info_cache[m_slot_id]->mccMnc, 8);
    pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
    unsigned int lac = voice_reg_state_cache[m_slot_id]->lac;
    unsigned long long cid = voice_reg_state_cache[m_slot_id]->cid;
    if (strlen(mccMnc) == 5 || strlen(mccMnc) == 6) {
        if (strlen(mccMnc) == 6) mnc_len = 3;
        else mnc_len = 2;
        char mcc_str[4] = {0};
        char mnc_str[4] = {0};
        strncpy(mcc_str, mccMnc, 3);
        strncpy(mnc_str, (mccMnc+3), (strlen(mccMnc)-3));
        mcc = atoi(mcc_str);
        mnc = atoi(mnc_str);
        int rat = voice_reg_state_cache[m_slot_id]->radio_technology == RADIO_TECH_NR ? 11 : 0;
        if ((voice_reg_state_cache[m_slot_id]->radio_technology == RADIO_TECH_NR && lac < 0xffffff)
                || (voice_reg_state_cache[m_slot_id]->radio_technology != RADIO_TECH_NR && lac < 0xffff)) {
            lac = voice_reg_state_cache[m_slot_id]->lac;
        } else if ((mdEgreg[m_slot_id]->eAct == 0x8000 && mdEgreg[m_slot_id]->lac < 0xffffff)
                || (mdEgreg[m_slot_id]->eAct < 0x8000 && mdEgreg[m_slot_id]->lac < 0xffff)) {
            lac = mdEgreg[m_slot_id]->lac;
            rat = mdEgreg[m_slot_id]->eAct == 0x8000 ? 11 : 0;
        }
        err = getOperatorNamesFromNumericCode(
                mccMnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH, rat);
        if (err < 0) {
            memset(longname, 0, MAX_OPER_NAME_LENGTH);
            memset(shortname, 0, MAX_OPER_NAME_LENGTH);
        }
    } else {
        mcc = INT_MAX;
        mnc = INT_MAX;
        // reset longname/shortname because mccmnc may be empty and these are empty too.
        // It happends when the MD is in service right after poll_operator and before poll_voice/data
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);
    }

    if (isInService(voice_reg_state_cache[m_slot_id]->registration_state) &&
            (lac != mdEcell[m_slot_id]->lacTac ||
            cid != mdEcell[m_slot_id]->cid)) {
        // unlock mutex before using any AT cmd
        refreshEcellCache();
        // if AT+ECELL provides old data
        if (lac != mdEcell[m_slot_id]->lacTac ||
                cid != mdEcell[m_slot_id]->cid) {
            // reset
            memset(mdEcell[m_slot_id], 0, sizeof(MD_ECELL));
            mdEcell[m_slot_id]->bsic = 0xFF;
            mdEcell[m_slot_id]->arfcn = INT_MAX;
            mdEcell[m_slot_id]->pscPci = INT_MAX;
            logD(LOG_TAG, "[%s] Ecell'data wrong, query them from physicalchannel", __FUNCTION__);
            useEcell = false;
            if (cellIdentity.cellInfoType != RIL_CELL_INFO_TYPE_CDMA) {
                queryPrimaryCellIdentityInfo(&identityInfo);
            }
        }
    }

    cellIdentity.cellInfoType =
            getCellInfoTypeRadioTechnology(voice_reg_state_cache[m_slot_id]->radio_technology);
    switch(cellIdentity.cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM: {
            cellIdentity.cellIdentityGsm.mcc = mcc;
            cellIdentity.cellIdentityGsm.mnc = mnc;
            cellIdentity.cellIdentityGsm.mnc_len= mnc_len;
            cellIdentity.cellIdentityGsm.lac =voice_reg_state_cache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityGsm.lac > 0xffff)
                    cellIdentity.cellIdentityGsm.lac = INT_MAX;
            cellIdentity.cellIdentityGsm.cid = (int) voice_reg_state_cache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityGsm.cid > 0xffff)
                    cellIdentity.cellIdentityGsm.cid = INT_MAX;
            cellIdentity.cellIdentityGsm.arfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityGsm.bsic =
                    useEcell ? mdEcell[m_slot_id]->bsic : identityInfo.pci;
            cellIdentity.cellIdentityGsm.operName.long_name = longname;
            cellIdentity.cellIdentityGsm.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_WCDMA: {
            cellIdentity.cellIdentityWcdma.mcc = mcc;
            cellIdentity.cellIdentityWcdma.mnc = mnc;
            cellIdentity.cellIdentityWcdma.mnc_len= mnc_len;
            cellIdentity.cellIdentityWcdma.lac = voice_reg_state_cache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityWcdma.lac > 0xffff)
                cellIdentity.cellIdentityWcdma.lac = INT_MAX;
            cellIdentity.cellIdentityWcdma.cid = (int) voice_reg_state_cache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityWcdma.cid > 268435455)
                cellIdentity.cellIdentityWcdma.cid = INT_MAX;
            cellIdentity.cellIdentityWcdma.psc =
                    useEcell ? mdEcell[m_slot_id]->pscPci : identityInfo.pci;
            cellIdentity.cellIdentityWcdma.uarfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityWcdma.operName.long_name = longname;
            cellIdentity.cellIdentityWcdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_CDMA: {
            cellIdentity.cellIdentityCdma.networkId = voice_reg_state_cache[m_slot_id]->network_id;
            cellIdentity.cellIdentityCdma.systemId = voice_reg_state_cache[m_slot_id]->system_id;
            cellIdentity.cellIdentityCdma.basestationId = voice_reg_state_cache[m_slot_id]->base_station_id;
            cellIdentity.cellIdentityCdma.longitude = voice_reg_state_cache[m_slot_id]->base_station_longitude;
            cellIdentity.cellIdentityCdma.latitude = voice_reg_state_cache[m_slot_id]->base_station_latitude;
            cellIdentity.cellIdentityCdma.operName.long_name = longname;
            cellIdentity.cellIdentityCdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_LTE: {
            cellIdentity.cellIdentityLte.mcc = mcc;
            cellIdentity.cellIdentityLte.mnc = mnc;
            cellIdentity.cellIdentityLte.mnc_len= mnc_len;
            cellIdentity.cellIdentityLte.ci = (int) voice_reg_state_cache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityLte.ci > 268435455)
                cellIdentity.cellIdentityLte.ci = INT_MAX;
            cellIdentity.cellIdentityLte.pci =
                    useEcell ? mdEcell[m_slot_id]->pscPci : identityInfo.pci;
            cellIdentity.cellIdentityLte.tac = voice_reg_state_cache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityLte.tac > 0xffff)
                cellIdentity.cellIdentityLte.tac = INT_MAX;
            cellIdentity.cellIdentityLte.earfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityLte.operName.long_name = longname;
            cellIdentity.cellIdentityLte.operName.short_name = shortname;
            cellIdentity.cellIdentityLte.bandwidth = 100 * ca_cache[m_slot_id]->pcell_bw;
            cellIdentity.cellIdentityLte.band = lteBand[m_slot_id] > 0 ? lteBand[m_slot_id] : 1;
            break;
        }

        case RIL_CELL_INFO_TYPE_NR:
            cellIdentity.cellIdentityNr.mcc = mcc;
            cellIdentity.cellIdentityNr.mnc = mnc;
            cellIdentity.cellIdentityNr.mnc_len = mnc_len;
            cellIdentity.cellIdentityNr.tac = voice_reg_state_cache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityNr.tac > 0xffffff)
                cellIdentity.cellIdentityNr.tac = INT_MAX;
            cellIdentity.cellIdentityNr.nci = voice_reg_state_cache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityNr.nci > 68719476735)
                cellIdentity.cellIdentityNr.nci = LONG_MAX;
            cellIdentity.cellIdentityNr.pci =
                    useEcell ? mdEcell[m_slot_id]->pscPci : identityInfo.pci;
            cellIdentity.cellIdentityNr.nrarfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityNr.operName.long_name = longname;
            cellIdentity.cellIdentityNr.operName.short_name = shortname;
            cellIdentity.cellIdentityNr.band = nrBand[m_slot_id] > 0 ? nrBand[m_slot_id] : 1;
            break;

        case RIL_CELL_INFO_TYPE_TD_SCDMA:
            //TD_SCDMA is impossible EXIST here
            RFX_ASSERT(false);
            TELEPHONYWARE_FALLTHROUGH;
        default: {
            if (longname) freeSetNull(longname);
            if (shortname) freeSetNull(shortname);
            break;
        }
    }
}

void RmcNetworkHandler::clearCellIdentity(RIL_CellIdentity_v16 &cellIdentity) {
    switch(cellIdentity.cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM: {
            if (cellIdentity.cellIdentityGsm.operName.long_name)
                freeSetNull(cellIdentity.cellIdentityGsm.operName.long_name);
            if (cellIdentity.cellIdentityGsm.operName.short_name)
                freeSetNull(cellIdentity.cellIdentityGsm.operName.short_name);
            break;
        }

        case RIL_CELL_INFO_TYPE_WCDMA: {
            if (cellIdentity.cellIdentityWcdma.operName.long_name)
                freeSetNull(cellIdentity.cellIdentityWcdma.operName.long_name);
            if (cellIdentity.cellIdentityWcdma.operName.short_name)
                freeSetNull(cellIdentity.cellIdentityWcdma.operName.short_name);
            break;
        }

        case RIL_CELL_INFO_TYPE_CDMA: {
            if (cellIdentity.cellIdentityCdma.operName.long_name)
                freeSetNull(cellIdentity.cellIdentityCdma.operName.long_name);
            if (cellIdentity.cellIdentityCdma.operName.short_name)
                freeSetNull(cellIdentity.cellIdentityCdma.operName.short_name);
            break;
        }

        case RIL_CELL_INFO_TYPE_LTE: {
            if (cellIdentity.cellIdentityLte.operName.long_name)
                freeSetNull(cellIdentity.cellIdentityLte.operName.long_name);
            if (cellIdentity.cellIdentityLte.operName.short_name)
                freeSetNull(cellIdentity.cellIdentityLte.operName.short_name);
            break;
        }

        case RIL_CELL_INFO_TYPE_NR:
            if (cellIdentity.cellIdentityNr.operName.long_name)
                freeSetNull(cellIdentity.cellIdentityNr.operName.long_name);
            if (cellIdentity.cellIdentityNr.operName.short_name)
                freeSetNull(cellIdentity.cellIdentityNr.operName.short_name);
            break;

        case RIL_CELL_INFO_TYPE_TD_SCDMA:
            //TD_SCDMA is impossible EXIST here
            RFX_ASSERT(false);
            TELEPHONYWARE_FALLTHROUGH;
        default: {
            break;
        }
    }
}

void RmcNetworkHandler::fillDataCellIdentity(RIL_CellIdentity_v16 &cellIdentity) {
    int err = 0;
    int mcc = 0;
    int mnc = 0;
    int mnc_len = 0;
    char *longname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char *shortname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char mccMnc[8] = {0};
    CellIdentityInfo identityInfo{INT_MAX, INT_MAX, 0, 0};
    bool useEcell = true;

    if (longname == NULL || shortname == NULL) {
        if (longname) freeSetNull(longname);
        if (shortname) freeSetNull(shortname);
        logE(LOG_TAG, "fillDataCellIdentity longname or shortname is null");
        return;
    }

    pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
    memcpy(mccMnc, op_info_cache[m_slot_id]->mccMnc, 8);
    pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
    if (strlen(mccMnc) == 5 || strlen(mccMnc) == 6) {
        if (strlen(mccMnc) == 6) mnc_len = 3;
        else mnc_len = 2;
        char mcc_str[4] = {0};
        char mnc_str[4] = {0};
        strncpy(mcc_str, mccMnc, 3);
        strncpy(mnc_str, (mccMnc+3), (strlen(mccMnc)-3));
        mcc = atoi(mcc_str);
        mnc = atoi(mnc_str);
        pthread_mutex_lock(&mdEgregMutex[m_slot_id]);
        unsigned int lac = mdEgreg[m_slot_id]->lac;
        int rat = mdEgreg[m_slot_id]->eAct == 0x8000 ? 11 : 0;
        if ((mdEgreg[m_slot_id]->eAct == 0x8000 && mdEgreg[m_slot_id]->lac < 0xffffff)
                || (mdEgreg[m_slot_id]->eAct < 0x8000 && mdEgreg[m_slot_id]->lac < 0xffff)) {
            lac = mdEgreg[m_slot_id]->lac;
        } else if ((mdEreg[m_slot_id]->eAct == 0x8000 && mdEreg[m_slot_id]->lac < 0xffffff)
                || (mdEreg[m_slot_id]->eAct < 0x8000 && mdEreg[m_slot_id]->lac < 0xffff)) {
            lac = mdEreg[m_slot_id]->lac;
            rat = mdEreg[m_slot_id]->eAct == 0x8000 ? 11 : 0;
        }
        pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);

        err = getOperatorNamesFromNumericCode(
                mccMnc, lac, longname, shortname, MAX_OPER_NAME_LENGTH, rat);
        if (err < 0) {
            memset(longname, 0, MAX_OPER_NAME_LENGTH);
            memset(shortname, 0, MAX_OPER_NAME_LENGTH);
        }
    } else {
        mcc = INT_MAX;
        mnc = INT_MAX;
        // reset longname/shortname because mccmnc may be empty and these are empty too.
        // It happends when the MD is in service right after poll_operator and before poll_voice/data
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);
    }


    if (isInService(mdEgreg[m_slot_id]->stat) &&
            (mdEgreg[m_slot_id]->lac != mdEcell[m_slot_id]->lacTac ||
            mdEgreg[m_slot_id]->ci != mdEcell[m_slot_id]->cid)) {
        refreshEcellCache();
        // if AT+ECELL provides old data
        if (mdEgreg[m_slot_id]->lac != mdEcell[m_slot_id]->lacTac ||
                mdEgreg[m_slot_id]->ci != mdEcell[m_slot_id]->cid) {
            // reset
            memset(mdEcell[m_slot_id], 0, sizeof(MD_ECELL));
            mdEcell[m_slot_id]->bsic = 0xFF;
            mdEcell[m_slot_id]->arfcn = INT_MAX;
            mdEcell[m_slot_id]->pscPci = INT_MAX;
            logD(LOG_TAG, "[%s] Ecell'data wrong, query them from physicalchannel", __FUNCTION__);
            useEcell = false;
            if (cellIdentity.cellInfoType != RIL_CELL_INFO_TYPE_CDMA) {
                queryPrimaryCellIdentityInfo(&identityInfo);
            }
        }
    }
    pthread_mutex_lock(&mdEgregMutex[m_slot_id]);
    cellIdentity.cellInfoType =
            getCellInfoTypeRadioTechnology(convertPSNetworkType(mdEgreg[m_slot_id]->eAct));
    switch(cellIdentity.cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM: {
            cellIdentity.cellIdentityGsm.mcc = mcc;
            cellIdentity.cellIdentityGsm.mnc = mnc;
            cellIdentity.cellIdentityGsm.mnc_len = mnc_len;
            cellIdentity.cellIdentityGsm.lac = mdEgreg[m_slot_id]->lac;
            if (cellIdentity.cellIdentityGsm.lac > 0xffff)
                cellIdentity.cellIdentityGsm.lac = INT_MAX;
            cellIdentity.cellIdentityGsm.cid = (int) mdEgreg[m_slot_id]->ci;
            if (cellIdentity.cellIdentityGsm.cid > 0xffff)
                cellIdentity.cellIdentityGsm.cid = INT_MAX;
            cellIdentity.cellIdentityGsm.arfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityGsm.bsic =
                    useEcell ? mdEcell[m_slot_id]->bsic : identityInfo.pci;
            cellIdentity.cellIdentityGsm.operName.long_name = longname;
            cellIdentity.cellIdentityGsm.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_WCDMA: {
            cellIdentity.cellIdentityWcdma.mcc = mcc;
            cellIdentity.cellIdentityWcdma.mnc = mnc;
            cellIdentity.cellIdentityWcdma.mnc_len= mnc_len;
            cellIdentity.cellIdentityWcdma.lac = mdEgreg[m_slot_id]->lac;
            if (cellIdentity.cellIdentityWcdma.lac > 0xffff)
                cellIdentity.cellIdentityWcdma.lac = INT_MAX;
            cellIdentity.cellIdentityWcdma.cid = (int) mdEgreg[m_slot_id]->ci;
            if (cellIdentity.cellIdentityWcdma.cid > 268435455)
                cellIdentity.cellIdentityWcdma.cid = INT_MAX;
            cellIdentity.cellIdentityWcdma.psc =
                    useEcell ? mdEcell[m_slot_id]->pscPci : identityInfo.pci;
            cellIdentity.cellIdentityWcdma.uarfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityWcdma.operName.long_name = longname;
            cellIdentity.cellIdentityWcdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_CDMA: {
            cellIdentity.cellIdentityCdma.networkId = voice_reg_state_cache[m_slot_id]->network_id;
            cellIdentity.cellIdentityCdma.systemId = voice_reg_state_cache[m_slot_id]->system_id;
            cellIdentity.cellIdentityCdma.basestationId = voice_reg_state_cache[m_slot_id]->base_station_id;
            cellIdentity.cellIdentityCdma.longitude = voice_reg_state_cache[m_slot_id]->base_station_longitude;
            cellIdentity.cellIdentityCdma.latitude = voice_reg_state_cache[m_slot_id]->base_station_latitude;
            cellIdentity.cellIdentityCdma.operName.long_name = longname;
            cellIdentity.cellIdentityCdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_LTE: {
            cellIdentity.cellIdentityLte.mcc = mcc;
            cellIdentity.cellIdentityLte.mnc = mnc;
            cellIdentity.cellIdentityLte.mnc_len= mnc_len;
            cellIdentity.cellIdentityLte.ci = (int) mdEgreg[m_slot_id]->ci;
            if (cellIdentity.cellIdentityLte.ci > 268435455)
                cellIdentity.cellIdentityLte.ci = INT_MAX;
            cellIdentity.cellIdentityLte.pci =
                    useEcell ? mdEcell[m_slot_id]->pscPci : identityInfo.pci;
            cellIdentity.cellIdentityLte.tac = mdEgreg[m_slot_id]->lac; //RILD doesn't use Tac.
            if (cellIdentity.cellIdentityLte.tac > 0xffff)
                cellIdentity.cellIdentityLte.tac = INT_MAX;
            cellIdentity.cellIdentityLte.earfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityLte.operName.long_name = longname;
            cellIdentity.cellIdentityLte.operName.short_name = shortname;
            cellIdentity.cellIdentityLte.bandwidth = 100 * ca_cache[m_slot_id]->pcell_bw;
            cellIdentity.cellIdentityLte.band = lteBand[m_slot_id] > 0 ? lteBand[m_slot_id] : 1;
            break;
        }

        case RIL_CELL_INFO_TYPE_NR:
            cellIdentity.cellIdentityNr.mcc = mcc;
            cellIdentity.cellIdentityNr.mnc = mnc;
            cellIdentity.cellIdentityNr.mnc_len = mnc_len;
            cellIdentity.cellIdentityNr.tac = mdEgreg[m_slot_id]->lac;
            if (cellIdentity.cellIdentityNr.tac > 0xffffff)
                cellIdentity.cellIdentityNr.tac = INT_MAX;
            cellIdentity.cellIdentityNr.nci = mdEgreg[m_slot_id]->ci;
            if (cellIdentity.cellIdentityNr.nci > 68719476735)
                cellIdentity.cellIdentityNr.nci = LONG_MAX;
            cellIdentity.cellIdentityNr.pci =
                    useEcell ? mdEcell[m_slot_id]->pscPci : identityInfo.pci;
            cellIdentity.cellIdentityNr.nrarfcn =
                    useEcell ? mdEcell[m_slot_id]->arfcn : identityInfo.arfcn;
            cellIdentity.cellIdentityNr.operName.long_name = longname;
            cellIdentity.cellIdentityNr.operName.short_name = shortname;
            cellIdentity.cellIdentityNr.band = nrBand[m_slot_id] > 0 ? nrBand[m_slot_id] : 1;
            break;

        case RIL_CELL_INFO_TYPE_TD_SCDMA:
            //TD_SCDMA is impossible EXIST here
            RFX_ASSERT(false);
            TELEPHONYWARE_FALLTHROUGH;
        default: {
            if (longname) freeSetNull(longname);
            if (shortname) freeSetNull(shortname);
            break;
        }
    }
    pthread_mutex_unlock(&mdEgregMutex[m_slot_id]);
}

int RmcNetworkHandler::fillBarringInfosResponse(size_t count,
        RIL_BarringInfo* barringInfos, RIL_BarringInforResponse* barringInfoResp) {
    if (isInService(data_reg_state_cache[m_slot_id]->registration_state)) {
        fillDataCellIdentity(barringInfoResp->cellIdentity);
    } else if (isInService(voice_reg_state_cache[m_slot_id]->registration_state)) {
        fillVoiceCellIdentity(barringInfoResp->cellIdentity);
    } else {
        // mtkLogE(LOG_TAG, "fillBarringInfosResponse it is out of service");
        return -1;
    }
    if (count > MAX_BARRING_INFO_COUNT) count = MAX_BARRING_INFO_COUNT;
    barringInfoResp->count_barringInfo = count;
    for (size_t i = 0; i < count; i++) {
        barringInfoResp->barringInfos[i].service_type = barringInfos[i].service_type;
        barringInfoResp->barringInfos[i].barring_type = barringInfos[i].barring_type;
        barringInfoResp->barringInfos[i].factor = barringInfos[i].factor;
        barringInfoResp->barringInfos[i].timeSeconds = barringInfos[i].timeSeconds;
        barringInfoResp->barringInfos[i].isBarred = barringInfos[i].isBarred;
    }
    return 0;
}

void RmcNetworkHandler::sendRegistrationFailedEvent(RIL_Domain domain) {
    int d[2] = {0};
    d[0] = (int) domain;
    if (registrationFailNotificationSwitch[m_slot_id]) {
        sendEvent(RFX_MSG_EVENT_REGISTRATION_FAILED,
                RfxIntsData(d, 2), RIL_CMD_PROXY_3, m_slot_id);
    }
}

int RmcNetworkHandler::updatePlmnInfo() {
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;
    int err = 0, mode = 0, format = 0;
    char* plmn = NULL;

    p_response = atSendCommandSingleline("AT+EOPS?", "+EOPS:");

    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        logE(LOG_TAG, "AT+EOPS got error response");
        goto error;
    }

    line = p_response->getIntermediates();
    if (line == NULL) goto error;
    line->atTokStart(&err);
    if (err < 0) goto error;
    mode = line->atTokNextint(&err);
    if (err < 0) goto error;

    // a "+EOPS: 0" response is possible
    if (line->atTokHasmore()) {
        /* <format> */
        format = line->atTokNextint(&err);
        if (err < 0) goto error;
        /* <oper> */
        plmn = line->atTokNextstr(&err);
        if (err < 0) goto error;
    } else {
        goto error;
    }
    pthread_mutex_lock(&op_info_Mutex[m_slot_id]);
    strncpy(op_info_cache[m_slot_id]->mccMnc, plmn, strlen(plmn));
    pthread_mutex_unlock(&op_info_Mutex[m_slot_id]);
    return 0;
error:
    return -1;
}

void RmcNetworkHandler::connectionStateReport(int mode, int state, int access) {
    int old_state = 0, new_state = 0, tdd_fdd = 0, old_nr_state = 0;;
    int samrtState = 0;
    sp<RfxMclMessage> urc;
    bool isNeedNotify = true;

    if (access == 5) {
        pthread_mutex_lock(&nr_connection_state_Mutex[m_slot_id]);
        old_nr_state = nr_connection_state[m_slot_id];
        nr_connection_state[m_slot_id] = (mode == 1 && state == 8) ? 1 : 0;
        if (old_nr_state != nr_connection_state[m_slot_id]) {
            isNeedNotifyStateChanged();
        }
        logV(LOG_TAG, "connectionStateReport:old_nr_state[%d], nr_connection_state[%d]",
            old_nr_state, nr_connection_state[m_slot_id]);
        pthread_mutex_unlock(&nr_connection_state_Mutex[m_slot_id]);
        return;
    }

    pthread_mutex_lock(&lte_connection_state_Mutex[m_slot_id]);

    // 3    Indicates usage of radio access of type E-UTRA TDD, see 3GPP TS 36.300 [145].
    // 4    Indicates usage of radio access of type E-UTRA FDD, see 3GPP TS 36.300 [145].
    if (access == 3 || access == 4) {
        if (access == 3) tdd_fdd = 0;
        else tdd_fdd = 1;

        if (mode == 1 && state == 7) new_state = 1;
        else new_state = 0;
    } else {
        // we don't care of non-LTE case.
        new_state = -1;
    }

    if (new_state == -1) {
        pthread_mutex_unlock(&lte_connection_state_Mutex[m_slot_id]);
        return;  // we ignore non-LTE case
    }

    if (tdd_fdd == 0) {
        old_state = lte_tdd_connection_state[m_slot_id];
    } else {
        old_state = lte_fdd_connection_state[m_slot_id];
    }

    logD(LOG_TAG, "handleConnectionStateReport: %d, %d, %d,"
            "old_state=%d,"
            "new_state=%d,"
            "jp_mode_timer1_delay_id=%d",
            mode, state, access, old_state, new_state,
            jp_mode_timer1_delay_id[m_slot_id]);
    if (old_state != new_state) {
        if (tdd_fdd == 0) {
            lte_tdd_connection_state[m_slot_id] = new_state;
        } else {
            lte_fdd_connection_state[m_slot_id] = new_state;
        }
    }
    pthread_mutex_unlock(&lte_connection_state_Mutex[m_slot_id]);

    // for JP mode
    if (isJpMode()) {
        if (old_state != new_state) {
            int delaySecond = -1;
            if (new_state == 1 && jp_mode_timer1_delay_timer[m_slot_id] > 0) {
                /* Set timer1 as STARTING before GOING to ensure NR state right
                   if pulling state comes betweens STARTING and ONGOING */
                setJpModeTimer1Starting();
                delaySecond = 2;
            } else if (new_state == 0 && jp_mode_timer2_delay_timer[m_slot_id] > 0) {
                delaySecond = 0;
            }
            if (delaySecond >= 0) {
                int t[2] = {0};
                t[0] = new_state;
                jp_mode_timer1_delay_id[m_slot_id] =  // create new id to prevent changes quickly
                        (jp_mode_timer1_delay_id[m_slot_id] + 1) % 1000;
                t[1] = jp_mode_timer1_delay_id[m_slot_id];  // mark current delay id to know any change during 1s
                // we delay 1s because we might attach other cell within 1s.

                sendEvent(RFX_MSG_EVENT_JP_MODE_CONNECTION_EVENT, RfxIntsData(t, 2), RIL_CMD_PROXY_3, m_slot_id,
                    -1, -1, s2ns(delaySecond), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
                isNeedNotify = false;
            }
        }
    }
    if (isNeedNotify) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    }

    // JP mode end

    if (tdd_fdd == 0) {
        samrtState = (new_state | 0x08) | 0x20; // for TDD RRC state
    } else {
        samrtState = (new_state | 0x04) | 0x20; // for FDD RRC state
    }
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_SMART_RAT_SWITCH_INFO, m_slot_id,
            RfxIntsData(&samrtState, 1));
    responseToTelCore(urc);

    if (old_state != new_state) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_LTE_RRC_STATE, new_state);
    }
    return;
}

void RmcNetworkHandler::loadDisplayRuleByOp() {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};
    rfx_property_get("persist.vendor.operator.optr", prop, "");
    if (strncmp(prop, "OP16", 4) == 0) {
        logD(LOG_TAG, "loadDisplayRuleByOp OP16");
        nr_display_rule[m_slot_id] = NR_DISPLAY_NONE;
    } else if (strncmp(prop, "OP50", 4) == 0) {
        logD(LOG_TAG, "loadDisplayRuleByOp OP50");
        nr_display_rule[m_slot_id] = NR_DISPLAY_JP_MODE;
        jp_mode_timer1_delay_timer[m_slot_id] = 30;
        jp_mode_timer2_delay_timer[m_slot_id] = 30;
    } else if (strncmp(prop, "OP08", 4) == 0) {
        logD(LOG_TAG, "loadDisplayRuleByOp OP08");
        nr_display_rule[m_slot_id] = NR_DISPLAY_NONE;
    } else if (strncmp(prop, "OP07", 4) == 0) {
        logD(LOG_TAG, "loadDisplayRuleByOp OP07");
        nr_display_rule[m_slot_id] = NR_DISPLAY_NONE;
    }
}

bool RmcNetworkHandler::isJpMode() {
    return ((nr_display_rule[m_slot_id] & NR_DISPLAY_JP_MODE) == NR_DISPLAY_JP_MODE);
}

void RmcNetworkHandler::startJpModeTimer1() {
    if (jp_mode_timer1_delay_timer[m_slot_id] <= 0) {
        logW(LOG_TAG, "start JP timer1(%d) <=0, ignore ", jp_mode_timer1_delay_timer[m_slot_id]);
        return;
    }
    pthread_mutex_lock(&jp_mode_timer1_mutex[m_slot_id]);
    jp_mode_timer1_state[m_slot_id] = JP_MODE_TIMER_STATE_GOING;
    jp_mode_timer1_delay_id[m_slot_id] =
            (jp_mode_timer1_delay_id[m_slot_id] + 1) % 1000;
    logV(LOG_TAG, "%s, timer1 start", __FUNCTION__);
    pthread_mutex_unlock(&jp_mode_timer1_mutex[m_slot_id]);

    int t[2] = {0};
    t[0] = 1;
    t[1] = jp_mode_timer1_delay_id[m_slot_id];
    logD(LOG_TAG, "%s: id=%d", __FUNCTION__, t[1]);
    sendEvent(RFX_MSG_EVENT_JP_MODE_TIMER_EVENT, RfxIntsData(t, 2), RIL_CMD_PROXY_3, m_slot_id,
            -1, -1, s2ns(jp_mode_timer1_delay_timer[m_slot_id]), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
}
void RmcNetworkHandler::stopJpModeTimer1() {
    pthread_mutex_lock(&jp_mode_timer1_mutex[m_slot_id]);
    if (jp_mode_timer1_state[m_slot_id] != JP_MODE_TIMER_STATE_STOP) {
        jp_mode_timer1_state[m_slot_id] = JP_MODE_TIMER_STATE_STOP;
        logD(LOG_TAG, "%s, timer1 stop", __FUNCTION__);
    }
    // new event! incress id to skip all pending events.
    jp_mode_timer1_delay_id[m_slot_id] =
            (jp_mode_timer1_delay_id[m_slot_id] + 1) % 1000;
    pthread_mutex_unlock(&jp_mode_timer1_mutex[m_slot_id]);
}
void RmcNetworkHandler::setJpModeTimer1Timeout() {
    logV(LOG_TAG, "%s", __FUNCTION__);
    pthread_mutex_lock(&jp_mode_timer1_mutex[m_slot_id]);
    jp_mode_timer1_state[m_slot_id] = JP_MODE_TIMER_STATE_TIMEOUT;
    logD(LOG_TAG, "%s, timer1 timeout", __FUNCTION__);
    pthread_mutex_unlock(&jp_mode_timer1_mutex[m_slot_id]);
}

void RmcNetworkHandler::setJpModeTimer1Starting() {
    logV(LOG_TAG, "%s", __FUNCTION__);
    pthread_mutex_lock(&jp_mode_timer1_mutex[m_slot_id]);
    jp_mode_timer1_state[m_slot_id] = JP_MODE_TIMER_STATE_STARTING;
    logD(LOG_TAG, "%s, timer1 Starting", __FUNCTION__);
    pthread_mutex_unlock(&jp_mode_timer1_mutex[m_slot_id]);
}

void RmcNetworkHandler::startJpModeTimer2() {
    if (jp_mode_timer2_delay_timer[m_slot_id] <= 0) {
        logW(LOG_TAG, "start JP timer2(%d) <=0, ignore ", jp_mode_timer2_delay_timer[m_slot_id]);
        return;
    }
    pthread_mutex_lock(&jp_mode_timer2_mutex[m_slot_id]);
    jp_mode_timer2_state[m_slot_id] = JP_MODE_TIMER_STATE_GOING;
    jp_mode_timer2_delay_id[m_slot_id] =
            (jp_mode_timer2_delay_id[m_slot_id] + 1) % 1000;
    logV(LOG_TAG, "%s, timer2 start", __FUNCTION__);
    pthread_mutex_unlock(&jp_mode_timer2_mutex[m_slot_id]);

    int t[2] = {0};
    t[0] = 2;
    t[1] = jp_mode_timer2_delay_id[m_slot_id];
    logD(LOG_TAG, "%s: id=%d", __FUNCTION__, t[1]);
    sendEvent(RFX_MSG_EVENT_JP_MODE_TIMER_EVENT, RfxIntsData(t, 2), RIL_CMD_PROXY_3, m_slot_id,
            -1, -1, s2ns(jp_mode_timer2_delay_timer[m_slot_id]), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
}
void RmcNetworkHandler::stopJpModeTimer2() {
    pthread_mutex_lock(&jp_mode_timer2_mutex[m_slot_id]);
    if (jp_mode_timer2_state[m_slot_id] != JP_MODE_TIMER_STATE_STOP) {
        jp_mode_timer2_state[m_slot_id] = JP_MODE_TIMER_STATE_STOP;
        logD(LOG_TAG, "%s, timer2 stop", __FUNCTION__);
    }
    // new event! incress id to skip all pending events.
    jp_mode_timer2_delay_id[m_slot_id] =
            (jp_mode_timer2_delay_id[m_slot_id] + 1) % 1000;
    pthread_mutex_unlock(&jp_mode_timer2_mutex[m_slot_id]);
}
void RmcNetworkHandler::setJpModeTimer2Timeout() {
    logV(LOG_TAG, "%s", __FUNCTION__);
    pthread_mutex_lock(&jp_mode_timer2_mutex[m_slot_id]);
    jp_mode_timer2_state[m_slot_id] = JP_MODE_TIMER_STATE_TIMEOUT;
    logD(LOG_TAG, "%s, timer2 timeout", __FUNCTION__);
    pthread_mutex_unlock(&jp_mode_timer2_mutex[m_slot_id]);
}
void RmcNetworkHandler::onHandleJpModeTimerEvent(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "%s", __FUNCTION__);
    sp<RfxMclMessage> urc;
    int *pInt = (int *)msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(int);
    if (size < 2) {
        logE(LOG_TAG, "%s, data is wrong", __FUNCTION__);
        return;
    }
    int timer_type = pInt[0];
    int timer_id = pInt[1];
    bool changed = false;
    if (timer_type == 1) {
        if (timer_id == jp_mode_timer1_delay_id[m_slot_id]) {
            setJpModeTimer1Timeout();
            changed = true;
        } else {
            logV(LOG_TAG, "%s, skip timer1 event with id=%d", __FUNCTION__, timer_id);
        }
    } else {
        if (timer_id == jp_mode_timer2_delay_id[m_slot_id]) {
            setJpModeTimer2Timeout();
            changed = true;
        } else {
            logV(LOG_TAG, "%s, skip timer2 event with id=%d", __FUNCTION__, timer_id);
        }
    }
    if (changed) {
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                    m_slot_id, RfxVoidData());
        responseToTelCore(urc);
    }
    return;
}

void RmcNetworkHandler::onHandleJpModeConnectionEvent(const sp<RfxMclMessage>& msg) {
    logV(LOG_TAG, "%s", __FUNCTION__);
    sp<RfxMclMessage> urc;
    int *pInt = (int *)msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(int);
    if (size < 2) {
        logE(LOG_TAG, "%s, data is wrong", __FUNCTION__);
        return;
    }
    int new_state = pInt[0];
    int timer1_delay_id = pInt[1];
    // check whether any events within 1s
    if (timer1_delay_id != jp_mode_timer1_delay_id[m_slot_id]) {
        logV(LOG_TAG, "%s timer1 event %d, skip %d", __FUNCTION__,
            jp_mode_timer1_delay_id[m_slot_id],
            timer1_delay_id);
        return;
    }
    if (new_state == 1 && // enter connected mode
            mdEgreg[m_slot_id]->endc_sib == 2 &&  // current cell has NR
            isInService(mdEgreg[m_slot_id]->stat) && // in service
            (mdEgreg[m_slot_id]->eAct == 0x1000 ||
                mdEgreg[m_slot_id]->eAct == 0x2000)) { // is lte or lte_ca
        startJpModeTimer1();
    } else {
        // enter idle mode
        stopJpModeTimer1();
        stopJpModeTimer2();
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(urc);
    return;
}

void RmcNetworkHandler::updateSbpInfos() {
    int err = 0;
    int disable_auto_return_rplmn = 0;
    RfxAtLine* line = NULL;
    sp<RfxAtResponse> p_response;

    // check SBP_DISABLE_AUTO_RETURN_PRE_RPLMN
    p_response = atSendCommandSingleline("AT+ESBP=7,\"SBP_DISABLE_AUTO_RETURN_PRE_RPLMN\"", "+ESBP:");
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) {
        goto error;
    }

    // handle intermediate
    line = p_response->getIntermediates();
    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    disable_auto_return_rplmn = line->atTokNextint(&err);
    if (err < 0) goto error;

    rfx_property_set(PROPERTY_DISABLE_AUTO_RETURN_RPLMN,
            (disable_auto_return_rplmn == 1) ? "1" : "0");
    return;

error:
    logE(LOG_TAG, "There is something wrong with AT+ESBP");
    return;
}

void RmcNetworkHandler::cleanNetworkScanResultOperator(RIL_NetworkScanResult* result) {
    RIL_NetworkScanResult * pData = (RIL_NetworkScanResult *) result;
    if (pData != NULL && pData->network_infos_length > 0 && pData->network_infos != NULL) {
        int countCell = pData->network_infos_length;
        for (int i = 0; i < countCell; i++) {
            switch(pData->network_infos[i].cellInfoType) {
                case RIL_CELL_INFO_TYPE_GSM:
                    if (pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.long_name)
                        free(pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.long_name);
                    if (pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.short_name)
                        free(pData->network_infos[i].CellInfo.gsm.cellIdentityGsm.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_WCDMA:
                    if (pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name)
                        free(pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                    if(pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name)
                        free(pData->network_infos[i].CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_LTE:
                    if (pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.long_name)
                        free(pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.long_name);
                    if (pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.short_name)
                        free(pData->network_infos[i].CellInfo.lte.cellIdentityLte.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_NR:
                    if (pData->network_infos[i].CellInfo.nr.cellidentity.operName.long_name)
                        free(pData->network_infos[i].CellInfo.nr.cellidentity.operName.long_name);
                    if (pData->network_infos[i].CellInfo.nr.cellidentity.operName.short_name)
                        free(pData->network_infos[i].CellInfo.nr.cellidentity.operName.short_name);
                    break;
                case RIL_CELL_INFO_TYPE_TD_SCDMA:
                case RIL_CELL_INFO_TYPE_CDMA:
                case RIL_CELL_INFO_TYPE_NONE:
                    break;
            }
        }
    }
}

void RmcNetworkHandler::onHandleRetryNotifySystem(const sp<RfxMclMessage>& msg) {
    int radio_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, 0);
    bool mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    if (radio_state == RADIO_STATE_UNAVAILABLE || mdOff) {
        logV(LOG_TAG, "%s No need notify for radioState(%d) or mdOff(%d)", __FUNCTION__, radio_state, mdOff);
        return;
    }
    sp<RfxMclMessage> urc;
    int *pInt = (int *)msg->getData()->getData();
    int size = msg->getData()->getDataLength() / sizeof(int);
    if (size < 1) {
        logE(LOG_TAG, "%s, data is wrong", __FUNCTION__);
        return;
    }

    int retry_id = pInt[0];
    logV(LOG_TAG, "%s %d %d", __FUNCTION__, pInt[0], mIsNeedNotifyState_Id[m_slot_id]);
    // after a timer, ap still doesn't query so we notify again.
    if (retry_id == mIsNeedNotifyState_Id[m_slot_id]) {
        sp<RfxMclMessage> urc;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        // response to TeleCore
        responseToTelCore(urc);

        // start a new watch dog again
        logV(LOG_TAG, "send RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM with %d", mIsNeedNotifyState_Id[m_slot_id]);
        // start a watch dog to check whether system got our notification or not.
        int t[2] = {0};
        t[0] = mIsNeedNotifyState_Id[m_slot_id];
        sendEvent(RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM, RfxIntsData(t, 2), RIL_CMD_PROXY_3, m_slot_id,
                -1, -1, s2ns(RETRY_NOTIFY_SYSTEM_TIMER), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
    }
}

void RmcNetworkHandler::updateSignalStrengthKey(const char* type, int sig1, int sig2) {
    char* content = NULL;
    asprintf(&content, "%s,%d,%d", type, sig1, sig2);
    if (content != NULL) {
        logV(LOG_TAG, "updateSignalStrengthKey, %s", content);
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_SIGNAL_STRENGTH,
                String8(content));
        free(content);
    }
}

void RmcNetworkHandler::resetNrCaCache(RIL_NR_CA_CACHE* cache) {
    pthread_mutex_lock(&s_nrCaMutex[m_slot_id]);
    cache->is_endc = 0;
    cache->band_1 = 0;
    cache->band_2 = 0;
    pthread_mutex_unlock(&s_nrCaMutex[m_slot_id]);
}

void RmcNetworkHandler::updateNrCaBandInfo() {
    int err;
    sp<RfxAtResponse> p_response;
    RfxAtLine* line;

    resetNrCaCache(nr_ca_cache[m_slot_id]);

    int *is_endc = &(nr_ca_cache[m_slot_id]->is_endc);
    int *band_1 = &(nr_ca_cache[m_slot_id]->band_1);
    int *band_2 = &(nr_ca_cache[m_slot_id]->band_2);

    // is_endc - SA(0)/SCG(1)
    // +ENRCABAND:<is_endc>,<band_1>[,<band_2>].
    // This command should be send only when SCG connected or SA
    p_response = atSendCommandSingleline("AT+ENRCABAND?", "+ENRCABAND:");

    pthread_mutex_lock(&s_nrCaMutex[m_slot_id]);
    // check error
    err = p_response->getError();
    if (err != 0 ||
            p_response == NULL ||
            p_response->getSuccess() == 0 ||
            p_response->getIntermediates() == NULL) goto error;

    // handle intermediate
    line = p_response->getIntermediates();

    // go to start position
    line->atTokStart(&err);
    if (err < 0) goto error;

    *is_endc = line->atTokNextint(&err);
    if (err < 0) goto error;

    *band_1 = line->atTokNextint(&err);
    if (err < 0) goto error;

    if (line->atTokHasmore()) {
        *band_2 = line->atTokNextint(&err);
        if (err < 0) goto error;
    }

    pthread_mutex_unlock(&s_nrCaMutex[m_slot_id]);
    return;
error:
    pthread_mutex_unlock(&s_nrCaMutex[m_slot_id]);
    resetNrCaCache(nr_ca_cache[m_slot_id]);
    return;
}

bool RmcNetworkHandler::parsePhyChlCfg(RIL_RadioTechnology curRat, RfxAtLine *line,
        RIL_PhysicalChannelConfig* resp) {
    /*+EPHYCONFIG: <rat>, <dl_ch_num>, <ul_ch_num>, <dl_cell_bw>, <ul_cell_bw>,<band>, <pci>,<pcell or scell>*/

    int err;
    pthread_mutex_lock(&scg_connection_state_Mutex[m_slot_id]);
    int t_scg_connection_state = scg_connection_state[m_slot_id];
    pthread_mutex_unlock(&scg_connection_state_Mutex[m_slot_id]);

    int rat = 0, dl_ch_num = 0, ul_ch_num = 0, dl_cell_bw = 0, ul_cell_bw = 0, pci = 0, band = 0;
    bool isNrScell = false;
    int status = 0;
    int INVALID = INT_MAX; // 0x7FFFFFFF;

    line->atTokStart(&err);
    if (err < 0) {
        logE(LOG_TAG, "[%s] ERROR! return false", __FUNCTION__);
        return false;
    }
    rat = line->atTokNextint(&err);
    if (err < 0) {
        logE(LOG_TAG, "[%s] get rat ERROR! return false", __FUNCTION__);
        return false;
    }
    if (rat <= 0)  {
        logE(LOG_TAG, "[%s] rat:%d, ERROR! return false", __FUNCTION__, rat);
        return false;
    }

    //curRat is 0, means it dont know ps reg statu now, need all info in the line
    //0: NONE 1:GSM 2:UMTS 4:LTE 128:NR
    if (rat == 128 && (curRat == 20 || curRat == 0)) { //NR
        rat = RADIO_TECH_NR;
    } else if (rat == 128 && ((curRat == 14 || curRat == 19) && t_scg_connection_state)) { //scg connected
        rat = RADIO_TECH_NR;
        isNrScell = true;
    } else if (rat == 4 && (curRat == 14 || curRat == 19 || curRat == 0)) { //4:LTE
        rat = RADIO_TECH_LTE;
    } else if (rat == 2
           && (curRat == 3 || curRat == 9|| curRat == 10 || curRat == 11 || curRat == 15 || curRat == 0)) {//2:UMTS
        rat = RADIO_TECH_UMTS;
    } else if (rat == 1 && (curRat == 1 || curRat == 2 || curRat == 0)) { //1:GSM
        rat = RADIO_TECH_GSM;
    } else {
        logD(LOG_TAG, "[%s] rat %d (0: NONE 1:GSM 2:UMTS 4:LTE 128:NR), t_scg_connection_state %d,"
                " not match radioTec: %d skip this line",
                __FUNCTION__, rat, t_scg_connection_state, curRat);
        return false;
    }

    resp->rat = (RIL_RadioTechnology)rat; //rat

    dl_ch_num = line->atTokNextint(&err);
    if (err < 0) {
        dl_ch_num = INVALID;
    }
    resp->downlinkChannelNumber = dl_ch_num;

    ul_ch_num = line->atTokNextint(&err);
    if (err < 0) {
        ul_ch_num = INVALID;
    }
    resp->uplinkChannelNumber = ul_ch_num;

    dl_cell_bw = line->atTokNextint(&err);
    if (err < 0) {
        dl_cell_bw = INVALID;
    }
    resp->cellBandwidthDownlink = dl_cell_bw;

    ul_cell_bw = line->atTokNextint(&err);
    if (err < 0) {
        ul_cell_bw = INVALID;
    }
    resp->cellBandwidthUplink = ul_cell_bw;

    band = line->atTokNextint(&err);
    if (err < 0) {
        band = 1;
    }
    resp->band = band;

    pci = line->atTokNextint(&err);
    if (err < 0) {
        pci = 0;
    }
    resp->physicalCellId = pci;

    status = line->atTokNextint(&err);
    if (err < 0) {
        status = 0;
    }
    resp->status = (RIL_CellConnectionStatus)status;
    if (isNrScell) {
        resp->status = SECONDARY_SERVING;
    }

    char* phyCid = getMask((int)resp->physicalCellId);
    logV(LOG_TAG, "[%s], curRat:%d, rat %d, status %d, downlinkChannelNumber %d,"
            "uplinkChannelNumber %d, cellBandwidthDownlink %d, cellBandwidthUplink %d, band %d, "
            "physicalCellId %s", __FUNCTION__, curRat, resp->rat, resp->status,
            resp->downlinkChannelNumber, resp->uplinkChannelNumber,
            resp->cellBandwidthDownlink, resp->cellBandwidthUplink, resp->band, phyCid);
    if (phyCid) free(phyCid);
    return true;
}

bool RmcNetworkHandler::queryPrimaryCellIdentityInfo(CellIdentityInfo* pOut) {
    /*+EPHYCONFIG=2
        +EPHYCONFIG: <rat>, <dl_ch_num>, <ul_ch_num>, <dl_cell_bw>, <ul_cell_bw>,<band>, <pci>,<pcell or scell>
        ...list
    */
    sp<RfxAtResponse> p_response = atSendCommandMultiline("AT+EPHYCONFIG=2", "+EPHYCONFIG:");
    // check error
    int err = p_response->getError();
    if (err != 0 || p_response == NULL || p_response->getSuccess() == 0) {
        logE(LOG_TAG, "[%s]:AT+EPHYCONFIG=2 error return false", __FUNCTION__);
        return false;
    }

    RfxAtLine *line = p_response->getIntermediates();
    RIL_RadioTechnology curRat = (RIL_RadioTechnology)convertPSNetworkType(urc_data_reg_state_cache[m_slot_id]->radio_technology);
    RIL_PhysicalChannelConfig info;
    int ret = false;
    for (; line != NULL; line = line->getNext()) {
        ret = parsePhyChlCfg(curRat, line, &info);
        if (!ret) {
            continue;
        }
        if (info.status == PRIMARY_SERVING) {
            pOut->bandwidth = info.cellBandwidthDownlink;
            pOut->arfcn = info.downlinkChannelNumber;
            pOut->pci = (int) info.physicalCellId;
            pOut->band = (int) info.band;
            char* pci_s = getMask(pOut->pci);
            logV(LOG_TAG, "[%s]: pci = %s, arfcn = %d, band %d, bandwidth = %d",
                    __FUNCTION__, pci_s, pOut->arfcn, pOut->band, pOut->bandwidth);
            if (pci_s) free(pci_s);
            break;
        }
    }
    return ret;
}
