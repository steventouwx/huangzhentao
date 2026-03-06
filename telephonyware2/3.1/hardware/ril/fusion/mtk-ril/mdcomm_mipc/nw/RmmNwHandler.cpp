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

#include "RmmNwHandler.h"
#include <compiler/compiler_utils.h>
#include <telephony/mtk_ril.h>
#include "libmtkrilutils.h"
#include "RfxMipcData.h"
#include "RfxViaUtils.h"
#include "RfxCellInfoData.h"
#include "RfxSignalStrengthData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include <cutils/memory.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmmNwHdlr"

#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
static const char *SKIP_DCNR_RESTRICTED_STATE_MCCMNC[] = {
    "42507"
};
#endif //MTK_HYP_DUAL_TELE_SUPPORT

static const char PROPERTY_RIL_TEST_SIM[4][35] = {
    "vendor.gsm.sim.ril.testsim",
    "vendor.gsm.sim.ril.testsim.2",
    "vendor.gsm.sim.ril.testsim.3",
    "vendor.gsm.sim.ril.testsim.4",
};

bool RmmNwHandler::sHandlerInit[MAX_SIM_COUNT] = { false };
RIL_OPERATOR_INFO_CACHE* RmmNwHandler::sOpInfoCache[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sOpInfoMutex[MAX_SIM_COUNT];
RIL_CS_REG_STATE_CACHE* RmmNwHandler::sCsRegStateCache[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sCsRegStateMutex[MAX_SIM_COUNT];
RIL_PS_REG_STATE_CACHE* RmmNwHandler::sPsRegStateCache[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sPsRegStateMutex[MAX_SIM_COUNT];
bool RmmNwHandler::sNwSelMode[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sNwSelModeMutex[MAX_SIM_COUNT];
ImsEmergencyInfo *RmmNwHandler::sImsEmergencyInfo[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sImsEmergencyMutex[MAX_SIM_COUNT];
NR_SYS_INFO *RmmNwHandler::sNrSysInfo[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sNrSysInfoMutex[MAX_SIM_COUNT];
bool RmmNwHandler::sUrcNrsysinfoEnable;
bool RmmNwHandler::sUrcChInfoEnable;
bool RmmNwHandler::sUrcNrCaBandEnable;
bool RmmNwHandler::sUrcNwCfgInfoEnable;
bool RmmNwHandler::sIsNeedNotifyState[MAX_SIM_COUNT];
int RmmNwHandler::sIsNeedNotifyStateId[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sNeedNotifyStateMutex[MAX_SIM_COUNT];
bool RmmNwHandler::sImsEccState[MAX_SIM_COUNT];
bool RmmNwHandler::sRegFailNotificationSwitch[MAX_SIM_COUNT];
RIL_CELL_INFO_CACHE* RmmNwHandler::sPrimaryCellInfoCache[MAX_SIM_COUNT];

bool RmmNwHandler::sPhysicalConfigSwitch[MAX_SIM_COUNT];
RIL_PhysicalChannelConfig RmmNwHandler::sPhysicalConfigCache[MAX_SIM_COUNT][MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM];
uint16_t RmmNwHandler::sPhysicalConfigCount[MAX_SIM_COUNT];
RIL_PhysicalChannelConfig RmmNwHandler::sPhysicalConfigNotified[MAX_SIM_COUNT][MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM];
uint16_t RmmNwHandler::sPhysicalConfigCountNotified[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sPhysicalConfigMutex[MAX_SIM_COUNT];
bool RmmNwHandler::s_enwscn_support = false;
pthread_mutex_t RmmNwHandler::s_signalStrengthMutex[MAX_SIM_COUNT];
RIL_SIGNAL_STRENGTH* RmmNwHandler::signal_strength_cache[MAX_SIM_COUNT];
int RmmNwHandler::lastReportedSignal[MAX_SIM_COUNT];
LteSignalProperty RmmNwHandler::mCurrentLteSignal[MAX_SIM_COUNT];
int RmmNwHandler::sPlmnListOngoingType;
bool RmmNwHandler::sPlmnListAbort;
RIL_FEMTO_CELL_CACHE* RmmNwHandler::sFemtocellCache[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::sFemtocellMutex[MAX_SIM_COUNT];
int RmmNwHandler::nr_display_rule[MAX_SIM_COUNT];
int RmmNwHandler::jp_mode_timer1_delay_timer[MAX_SIM_COUNT];
int RmmNwHandler::jp_mode_timer2_delay_timer[MAX_SIM_COUNT];
int RmmNwHandler::scg_connection_state[MAX_SIM_COUNT];
char RmmNwHandler::last_valid_plmn[MAX_SIM_COUNT][8];
nsecs_t RmmNwHandler::last_valid_plmn_time[MAX_SIM_COUNT];
bool RmmNwHandler::s_iwlan_in_legacy_mode;
int RmmNwHandler::s_iwlan_reg_status[MAX_SIM_COUNT] = { 0 };
pthread_mutex_t RmmNwHandler::s_iwlanRegStatusMutex[MAX_SIM_COUNT];
int RmmNwHandler::nr_connection_state[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::nr_connection_state_Mutex[MAX_SIM_COUNT];
RIL_NwCfgInfo* RmmNwHandler::nw_cfg_cache[MAX_SIM_COUNT];
pthread_mutex_t RmmNwHandler::s_nwCfgMutex[MAX_SIM_COUNT];

RmmNwHandler::RmmNwHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {

    int ret = 0;
    if (!sHandlerInit[m_slot_id]) {
        sHandlerInit[m_slot_id] = true;
        sOpInfoCache[m_slot_id] = (RIL_OPERATOR_INFO_CACHE*)calloc(1, sizeof(RIL_OPERATOR_INFO_CACHE));
        ret = pthread_mutex_init(&sOpInfoMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sCsRegStateCache[m_slot_id] = (RIL_CS_REG_STATE_CACHE*)calloc(1, sizeof(RIL_CS_REG_STATE_CACHE));
        ret = pthread_mutex_init(&sCsRegStateMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sPsRegStateCache[m_slot_id] = (RIL_PS_REG_STATE_CACHE*)calloc(1, sizeof(RIL_PS_REG_STATE_CACHE));
        ret = pthread_mutex_init(&sPsRegStateMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sNwSelMode[m_slot_id] = false;
        ret = pthread_mutex_init(&sNwSelModeMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sIsNeedNotifyState[m_slot_id] = true;
        sIsNeedNotifyStateId[m_slot_id] = 0;
        ret = pthread_mutex_init(&sNeedNotifyStateMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sImsEccState[m_slot_id] = false;
        sImsEmergencyInfo[m_slot_id] = (ImsEmergencyInfo *)calloc(1, sizeof(ImsEmergencyInfo));
        ret = pthread_mutex_init(&sImsEmergencyMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sNrSysInfo[m_slot_id] = (NR_SYS_INFO*) calloc(1, sizeof(NR_SYS_INFO));
        ret = pthread_mutex_init(&sNrSysInfoMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        sRegFailNotificationSwitch[m_slot_id] = true;

        sPhysicalConfigSwitch[m_slot_id] = true;
        for (int i = 0; i < MIPC_MAX_PHYSICAL_CHANNEL_CONFIGS_NUM; i++) {
            memset(&sPhysicalConfigCache[m_slot_id][i], 0, sizeof(RIL_PhysicalChannelConfig));
            memset(&sPhysicalConfigNotified[m_slot_id][i], 0, sizeof(RIL_PhysicalChannelConfig));
        }
        sPhysicalConfigCount[m_slot_id] = 0;
        sPhysicalConfigCountNotified[m_slot_id] = 0;
        ret = pthread_mutex_init(&sPhysicalConfigMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);

        signal_strength_cache[m_slot_id] =
            (RIL_SIGNAL_STRENGTH*)malloc(sizeof(RIL_SIGNAL_STRENGTH));
        ret = pthread_mutex_init(&s_signalStrengthMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        resetSignalStrengthCache(signal_strength_cache[m_slot_id], SIGNAL_GROUP_ALL);
        lastReportedSignal[m_slot_id] = 0;
        mCurrentLteSignal[m_slot_id].rsrp = 0x7FFFFFFF;
        mCurrentLteSignal[m_slot_id].rssnr = 0x7FFFFFFF;
        mCurrentLteSignal[m_slot_id].initialed = false;

        sPrimaryCellInfoCache[m_slot_id] =
            (RIL_CELL_INFO_CACHE*) calloc(1, sizeof(RIL_CELL_INFO_CACHE));
        sFemtocellCache[m_slot_id] = (RIL_FEMTO_CELL_CACHE*)calloc(1, sizeof(RIL_FEMTO_CELL_CACHE));
        ret = pthread_mutex_init(&sFemtocellMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);

        // default is 1 as (NR_DISPALY_NR_CACHE)
        // idle: cache cell's nr capability
        nr_display_rule[m_slot_id] = getDefault5GNsaDisplayRule();
        jp_mode_timer1_delay_timer[m_slot_id] = 20;
        jp_mode_timer2_delay_timer[m_slot_id] = 20;
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
            jp_mode_timer1_delay_timer[m_slot_id] = prop_nr_display_jp_mode_timer1_delay_timer;
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
            jp_mode_timer2_delay_timer[m_slot_id] = prop_nr_display_jp_mode_timer2_delay_timer;
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
        setNrDisplayRuleToMd();

        scg_connection_state[m_slot_id] = 0;

        memset(last_valid_plmn[m_slot_id], 0, 8);
        last_valid_plmn_time[m_slot_id] = -1;

        if (m_slot_id == 0) {
            rfx_property_get("ro.telephony.iwlan_operation_mode", prop,  "default");
            s_iwlan_in_legacy_mode = !strcmp("legacy", prop);
            logV(LOG_TAG, "iwlan_operation_mode:%s, s_iwlan_in_legacy_mode:%d",
                 prop, s_iwlan_in_legacy_mode);

            sUrcNrsysinfoEnable = isUrcNrSysInfoConfigured(); //default off
            sUrcChInfoEnable = isUrcChInfoConfigured();
            sUrcNrCaBandEnable = isUrcNrCaBandConfigured();
            sUrcNwCfgInfoEnable = isUrcNwCfgInfoConfigured();
        }

        ret = pthread_mutex_init(&s_iwlanRegStatusMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);

        nr_connection_state[m_slot_id] = 0;
        ret = pthread_mutex_init(&nr_connection_state_Mutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
        nw_cfg_cache[m_slot_id] = (RIL_NwCfgInfo*)calloc(1, sizeof(RIL_NwCfgInfo));
        ret = pthread_mutex_init(&s_nwCfgMutex[m_slot_id], NULL);
        RFX_ASSERT(ret == 0);
    }
}

RmmNwHandler::~RmmNwHandler() {
    if (sHandlerInit[m_slot_id]) {
        sHandlerInit[m_slot_id] = false;
        freeSetNull((char*)sCsRegStateCache[m_slot_id]);
    }
}

bool RmmNwHandler::isInService(int regState) {
    if (regState == 1 || regState == 5) {
        return true;
    }
    return false;
}

unsigned int RmmNwHandler::convertRegState(unsigned int uiRegState, bool isVoiceState)
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

char* RmmNwHandler::getMask(int i) {
    return getMask((unsigned long long) i);
}

char* RmmNwHandler::getMask(unsigned int i) {
    return getMask((unsigned long long) i);
}

char* RmmNwHandler::getMask(long long i) {
    return getMask((unsigned long long) i);
}

char* RmmNwHandler::getMask(unsigned long long i) {
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

int RmmNwHandler::transferPlmnToMccMnc(char* plmn, int *mcc, int *mnc, int * mncLen) {
    char mcc_str[4] = {0};
    char mnc_str[4] = {0};
    if (strlen(plmn) == 5 || strlen(plmn) == 6) {
        if (strlen(plmn) == 5) {
            *mncLen = 2;
        } else {
            *mncLen = 3;
        }
        strncpy(mcc_str, plmn, 3);
        strncpy(mnc_str, (plmn+3), (strlen(plmn)-3));
        *mcc = atoi(mcc_str);
        *mnc = atoi(mnc_str);
        return 0;
    }
    return -1;
}

unsigned int RmmNwHandler::convertCSNetworkType(unsigned int uiResponse) {
    RIL_RadioTechnology uiRet = RADIO_TECH_UNKNOWN;
    /* mapping */
    switch(uiResponse)
    {
        case 0x0001:            // GPRS
        case 0x0002:            // EDGE
            uiRet = RADIO_TECH_GSM; // GSM
            break;
        case 0x0004:            // UMTS
        case 0x0008:            // HSDPA
        case 0x0010:            // HSUPA
        case 0x0018:            // HSDPA_UPA
        case 0x0020:            // HSDPAP
        case 0x0030:            // HSDPAP_UPA
        case 0x0040:            // HSUPAP
        case 0x0048:            // HSUPAP_DPA
        case 0x0060:            // HSPAP
        case 0x0088:            // DC_DPA
        case 0x0098:            // DC_DPA_UPA
        case 0x00a0:            // DC_HSDPAP
        case 0x00b0:            // DC_HSDPAP_UPA
        case 0x00c8:            // DC_HSUPAP_DPA
        case 0x00e0:            // DC_HSPAP
            uiRet = RADIO_TECH_UMTS; // UMTS
            break;
        // for C2K
        case 0x0100:            // 1xRTT
            uiRet = RADIO_TECH_1xRTT; // 1xRTT
            break;
        case 0x0200:            // HRPD
            uiRet = RADIO_TECH_EVDO_A; // EVDO_A
            break;
        case 0x0400:            // EHRPD
            uiRet = RADIO_TECH_EHRPD; // EHRPD
            break;
        //for LTE
        case 0x1000:            // LTE
        case 0x2000:            // LTE_CA
        case 0x4000:
            uiRet = RADIO_TECH_LTE; // LTE
            break;
        case 0x8000:
            uiRet = RADIO_TECH_NR;
            break;
        default:
            uiRet = RADIO_TECH_UNKNOWN; // UNKNOWN
            break;
    }

    return (unsigned int)uiRet;
}

unsigned int RmmNwHandler::convertPSNetworkType(unsigned int uiResponse) {
    unsigned int uiRet = 0;

    /* mapping */
    switch(uiResponse) {
        case 0x0001:            // GPRS
            uiRet = 1;          // GPRS
            break;
        case 0x0002:            // EDGE
            uiRet = 2;          // EDGE
            break;
        case 0x0004:            // UMTS
            uiRet = 3;          // UMTS
            break;
        case 0x0008:            // HSDPA
            uiRet = 9;          // HSDPA
            break;
        case 0x0010:            // HSUPA
            uiRet = 10;         // HSUPA
            break;
        case 0x0018:            // HSDPA_UPA
            uiRet = 11;         // HSPA
            break;
        case 0x0020:            // HSDPAP
        case 0x0030:            // HSDPAP_UPA
        case 0x0040:            // HSUPAP
        case 0x0048:            // HSUPAP_DPA
        case 0x0060:            // HSPAP
        case 0x0088:            // DC_DPA
        case 0x0098:            // DC_DPA_UPA
        case 0x00a0:            // DC_HSDPAP
        case 0x00b0:            // DC_HSDPAP_UPA
        case 0x00c8:            // DC_HSUPAP_DPA
        case 0x00e0:            // DC_HSPAP
            uiRet = 15;         // HSPAP
            break;
        // for C2K
        case 0x0100:            // 1xRTT
            uiRet = 6;          // 1xRTT
            break;
        case 0x0200:            // HRPD
            uiRet = 8;          // EVDO_A
            break;
        case 0x0400:            // EHRPD
            uiRet = 13;         // EHRPD
            break;
        //for LTE
        case 0x1000:            // LTE
        case 0x4000:            // LTE + NR
            uiRet = 14;
            break;
        case 0x2000:            // LTE_CA
            uiRet = 19;
            break;
        case 0x8000:
            uiRet = 20;         // NR
            break;
        default:
            uiRet = 0;          // UNKNOWN
            break;
    }

    return uiRet;
}

void RmmNwHandler::isNeedNotifyStateChanged() {
    int radio_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, 0);
    bool mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    pthread_mutex_lock(&sNeedNotifyStateMutex[m_slot_id]);
    logV(LOG_TAG, "[%s] %d", __FUNCTION__, sIsNeedNotifyState[m_slot_id]);
    if (sIsNeedNotifyState[m_slot_id] && !mdOff && radio_state != RADIO_STATE_UNAVAILABLE) {
        sp<RfxMclMessage> urc;
        sIsNeedNotifyState[m_slot_id] = false;
        sIsNeedNotifyStateId[m_slot_id] = (sIsNeedNotifyStateId[m_slot_id] + 1) % 1000;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        // response to TeleCore
        responseToTelCore(urc);

        logV(LOG_TAG, "send RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM with %d",
             sIsNeedNotifyStateId[m_slot_id]);
        // start a watch dog to check whether system got our notification or not.
        sendEvent(RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM, RfxIntsData(&sIsNeedNotifyStateId[m_slot_id], 1), RIL_CMD_PROXY_3, m_slot_id,
                  -1, -1, s2ns(RETRY_NOTIFY_SYSTEM_TIMER), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
    }
    pthread_mutex_unlock(&sNeedNotifyStateMutex[m_slot_id]);
}

void RmmNwHandler::onHandleRetryNotifySystem(const sp<RfxMclMessage>& msg) {
    int radio_state = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE, 0);
    bool mdOff = getNonSlotMclStatusManager()->getBoolValue(RFX_STATUS_KEY_MODEM_POWER_OFF, false);
    if (radio_state == RADIO_STATE_UNAVAILABLE || mdOff) {
        logV(LOG_TAG, "%s No need notify for radioState(%d) or mdOff(%d)", __FUNCTION__, radio_state, mdOff);
        return;
    }

    sp<RfxMclMessage> urc;
    int *pInt = (int *)msg->getData()->getData();
    int retry_id = pInt[0];
    pthread_mutex_lock(&sNeedNotifyStateMutex[m_slot_id]);
    logV(LOG_TAG, "%s %d %d", __FUNCTION__, pInt[0], sIsNeedNotifyStateId[m_slot_id]);
    // after a timer, ap still doesn't query so we notify again.
    if (retry_id == sIsNeedNotifyStateId[m_slot_id]) {
        sp<RfxMclMessage> urc;
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
                m_slot_id, RfxVoidData());
        // response to TeleCore
        responseToTelCore(urc);

        // start a new watch dog again
        logV(LOG_TAG, "send RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM with %d",
             sIsNeedNotifyStateId[m_slot_id]);
        // start a watch dog to check whether system got our notification or not.
        sendEvent(RFX_MSG_EVENT_RETRY_NOTIFY_SYSTEM, RfxIntsData(&sIsNeedNotifyStateId[m_slot_id], 1),
                  RIL_CMD_PROXY_3, m_slot_id,
                  -1, -1, s2ns(RETRY_NOTIFY_SYSTEM_TIMER), MTK_RIL_REQUEST_PRIORITY_MEDIUM);
    }
    pthread_mutex_unlock(&sNeedNotifyStateMutex[m_slot_id]);
}

int RmmNwHandler::matchPlmnName(char *longAlpha, char *shortAlpha, const char* numeric) {
    return matchPlmnName(longAlpha, shortAlpha, numeric, -1, 0xFF, MATCH_NITZ|MATCH_TS25);
}

int RmmNwHandler::matchPlmnName(char *longAlpha, char *shortAlpha,
        const char* numeric, int lac, int rat) {
    return matchPlmnName(longAlpha, shortAlpha, numeric, lac, rat, MATCH_ALL);
}

int RmmNwHandler::matchPlmnName(char *longAlpha, char *shortAlpha,
        const char* numeric, int lac, int rat, int matchRule) {
    int result = MATCH_NONE;
    int matched = MATCH_NONE;
    if (numeric == NULL || (longAlpha == NULL && shortAlpha == NULL))
        return result;

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(
            MIPC_NW_GET_PROVIDER_NAME_REQ, m_slot_id);
    reqData->addMipcTlvStr(
            MIPC_NW_GET_PROVIDER_NAME_REQ_T_PLMN_ID, numeric);
    if (matchRule & MATCH_EONS && (lac >= 0 && lac < 0xFFFFFF)) {
        reqData->addMipcTlvUint32(
                MIPC_NW_GET_PROVIDER_NAME_REQ_T_LAC, (unsigned int)lac);
        reqData->addMipcTlvUint8(
                MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPL_INDEX, (rat == 11 || rat == 12 || rat == 13) ? 2 : 1);
    }
    if ((matchRule & MATCH_ALL) != MATCH_ALL) {
        reqData->addMipcTlvUint8(
                MIPC_NW_GET_PROVIDER_NAME_REQ_T_OPTION, 1);
    }
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getData() == NULL
        || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "getProviderName fail, result[%d]",
                cnfData->getResult());
        return result;
    }

    char *longName = cnfData->getMipcStr(
            MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME_LONG);
    char *shortName = cnfData->getMipcStr(
            MIPC_NW_GET_PROVIDER_NAME_CNF_T_NW_NAME);
    mipc_nw_name_pair_struct4 *eonsName = (mipc_nw_name_pair_struct4 *)cnfData->getMipcStr(
            MIPC_NW_GET_PROVIDER_NAME_CNF_T_EONS_NAME);
    mipc_nw_name_pair_struct4 *nitzName = (mipc_nw_name_pair_struct4 *)cnfData->getMipcStr(
            MIPC_NW_GET_PROVIDER_NAME_CNF_T_NITZ_NAME);
    mipc_nw_name_pair_struct4 *ts25Name = (mipc_nw_name_pair_struct4 *)cnfData->getMipcStr(
            MIPC_NW_GET_PROVIDER_NAME_CNF_T_TS25_NAME);

    if (eonsName != NULL && strlen(eonsName->long_name) > 0) {
        result |= MATCH_EONS;
    }
    if (nitzName != NULL && strlen(nitzName->long_name) > 0) {
        result |= MATCH_NITZ;
    }
    if (ts25Name != NULL && strlen(ts25Name->long_name) > 0) {
        result |= MATCH_TS25;
    }
    logD(LOG_TAG, "matchPlmnName: expected[%x],result[%x]", matchRule, result);

    result &= matchRule;
    if (longAlpha) {
        if (result & MATCH_EONS) {
            strncpy(longAlpha, eonsName->long_name, MAX_OPER_NAME_LENGTH-1);
            longAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
        } else if (result & MATCH_NITZ) {
            strncpy(longAlpha, nitzName->long_name, MAX_OPER_NAME_LENGTH-1);
            longAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
        } else if (result & MATCH_TS25) {
            strncpy(longAlpha, ts25Name->long_name, MAX_OPER_NAME_LENGTH-1);
            longAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
        } else {
            if (longName && strlen(longName) > 0) {
                strncpy(longAlpha, longName, MAX_OPER_NAME_LENGTH-1);
                longAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
                matched = matchRule;
            }
        }
    }
    if (shortAlpha) {
        if (result & MATCH_EONS) {
            strncpy(shortAlpha, eonsName->short_name, MAX_OPER_NAME_LENGTH-1);
            shortAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
        } else if (result & MATCH_NITZ) {
            strncpy(shortAlpha, nitzName->short_name, MAX_OPER_NAME_LENGTH-1);
            shortAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
        } else if (result & MATCH_TS25) {
            strncpy(shortAlpha, ts25Name->short_name, MAX_OPER_NAME_LENGTH-1);
            shortAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
        } else {
            if (shortName && strlen(shortName) > 0) {
                strncpy(shortAlpha, shortName, MAX_OPER_NAME_LENGTH-1);
                shortAlpha[MAX_OPER_NAME_LENGTH-1] = '\0';
                matched = matchRule;
            }
        }
    }

    return result ? result : matched;
}

void RmmNwHandler::updateAutoRestoreRplmnStatus(int slotId) {
    sp<RfxMipcData> mipcReq = RfxMipcData::obtainMipcData(
            MIPC_SYS_SET_OR_GET_SBP_INFO_REQ, slotId);

    mipcReq->addMipcTlvUint8(
            MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_MODE,
            MIPC_SYS_SBP_MODE_GET_CONFIG_WITH_FEATURE_STR);
    mipcReq->addMipcTlvStr(
            MIPC_SYS_SET_OR_GET_SBP_INFO_REQ_T_FEATURE_STR,
            String8::format("SBP_DISABLE_AUTO_RETURN_PRE_RPLMN"));
    sp<RfxMipcData> mipcCnf = callToMipcMsgSync(mipcReq);

    if (mipcCnf == NULL) {
        logE(LOG_TAG, "get AutoRestoreRplmn fail");
        return;
    }

    uint8_t result = mipcCnf->getMipcUint8Val(
            MIPC_SYS_SET_OR_GET_SBP_INFO_CNF_T_DATA, 0xff);
    if (result == 0xff) {
        logE(LOG_TAG, "get AutoRestoreRplmn result fail");
        return;
    }

    rfx_property_set(PROPERTY_AUTO_RESTORE_RPLMN, (result == 1) ? "1" : "0");
}

void RmmNwHandler::fillVoiceCellIdentity(RIL_CellIdentity_v16 &cellIdentity) {
    int mcc = 0;
    int mnc = 0;
    int mnc_len = 0;
    int mccmnc_len = 0;
    char *longname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char *shortname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char mccMnc[8] = {0};

    if (longname == NULL || shortname == NULL) {
        if (longname) freeSetNull(longname);
        if (shortname) freeSetNull(shortname);
        logE(LOG_TAG, "[%s] longname or shortname is null", __FUNCTION__);
        return;
    }
    pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
    strlcpy(mccMnc, sOpInfoCache[m_slot_id]->mccMnc, 8);
    strlcpy(longname, sOpInfoCache[m_slot_id]->longName, MAX_OPER_NAME_LENGTH);
    strlcpy(shortname, sOpInfoCache[m_slot_id]->shortName, MAX_OPER_NAME_LENGTH);
    pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
    mccmnc_len = strlen(mccMnc);
    if (mccmnc_len == 5 || mccmnc_len == 6) {
        mnc_len = (mccmnc_len == 6) ? 3 : 2;
        char mcc_str[4] = {0};
        char mnc_str[4] = {0};
        strncpy(mcc_str, mccMnc, 3);
        strncpy(mnc_str, (mccMnc + 3), (mccmnc_len - 3));
        mcc = atoi(mcc_str);
        mnc = atoi(mnc_str);
    } else {
        mcc = INT_MAX;
        mnc = INT_MAX;
        // reset longname/shortname because mccmnc may be empty and these are empty too.
        // It happends when the MD is in service right after poll_operator and before poll_voice/data
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);
    }

    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    cellIdentity.cellInfoType =
        getCellInfoTypeRadioTechnology(convertCSNetworkType(sCsRegStateCache[m_slot_id]->radio_technology));
    CellIdentityInfo info{INT_MAX, INT_MAX, 0, 0};
    if (isInService(sCsRegStateCache[m_slot_id]->registration_state) &&
            cellIdentity.cellInfoType != RIL_CELL_INFO_TYPE_CDMA) {
        queryPrimaryCellIdentityInfo(&info);
    }
    switch(cellIdentity.cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM: {
            cellIdentity.cellIdentityGsm.mcc = mcc;
            cellIdentity.cellIdentityGsm.mnc = mnc;
            cellIdentity.cellIdentityGsm.mnc_len= mnc_len;
            cellIdentity.cellIdentityGsm.lac = sCsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityGsm.lac > 0xffff)
                    cellIdentity.cellIdentityGsm.lac = INT_MAX;
            cellIdentity.cellIdentityGsm.cid = (int) sCsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityGsm.cid > 0xffff)
                    cellIdentity.cellIdentityGsm.cid = INT_MAX;
            cellIdentity.cellIdentityGsm.arfcn = info.arfcn;
            cellIdentity.cellIdentityGsm.bsic = info.pci;
            cellIdentity.cellIdentityGsm.operName.long_name = longname;
            cellIdentity.cellIdentityGsm.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_WCDMA: {
            cellIdentity.cellIdentityWcdma.mcc = mcc;
            cellIdentity.cellIdentityWcdma.mnc = mnc;
            cellIdentity.cellIdentityWcdma.mnc_len= mnc_len;
            cellIdentity.cellIdentityWcdma.lac = sCsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityWcdma.lac > 0xffff)
                cellIdentity.cellIdentityWcdma.lac = INT_MAX;
            cellIdentity.cellIdentityWcdma.cid = (int) sCsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityWcdma.cid > 268435455)
                cellIdentity.cellIdentityWcdma.cid = INT_MAX;
            cellIdentity.cellIdentityWcdma.psc = info.pci;
            cellIdentity.cellIdentityWcdma.uarfcn = info.arfcn;
            cellIdentity.cellIdentityWcdma.operName.long_name = longname;
            cellIdentity.cellIdentityWcdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_CDMA: {
            cellIdentity.cellIdentityCdma.networkId = sCsRegStateCache[m_slot_id]->network_id;
            cellIdentity.cellIdentityCdma.systemId = sCsRegStateCache[m_slot_id]->system_id;
            cellIdentity.cellIdentityCdma.basestationId = sCsRegStateCache[m_slot_id]->base_station_id;
            cellIdentity.cellIdentityCdma.longitude = sCsRegStateCache[m_slot_id]->base_station_longitude;
            cellIdentity.cellIdentityCdma.latitude = sCsRegStateCache[m_slot_id]->base_station_latitude;
            cellIdentity.cellIdentityCdma.operName.long_name = longname;
            cellIdentity.cellIdentityCdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_LTE: {
            cellIdentity.cellIdentityLte.mcc = mcc;
            cellIdentity.cellIdentityLte.mnc = mnc;
            cellIdentity.cellIdentityLte.mnc_len= mnc_len;
            cellIdentity.cellIdentityLte.ci = (int) sCsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityLte.ci > 268435455)
                cellIdentity.cellIdentityLte.ci = INT_MAX;
            cellIdentity.cellIdentityLte.tac = sCsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityLte.tac > 0xffff)
                cellIdentity.cellIdentityLte.tac = INT_MAX;
            cellIdentity.cellIdentityLte.pci = info.pci;
            cellIdentity.cellIdentityLte.earfcn = info.arfcn;
            cellIdentity.cellIdentityLte.band = info.band;
            cellIdentity.cellIdentityLte.operName.long_name = longname;
            cellIdentity.cellIdentityLte.operName.short_name = shortname;
            cellIdentity.cellIdentityLte.bandwidth = info.bandwidth;
            break;
        }

        case RIL_CELL_INFO_TYPE_NR:
            cellIdentity.cellIdentityNr.mcc = mcc;
            cellIdentity.cellIdentityNr.mnc = mnc;
            cellIdentity.cellIdentityNr.mnc_len = mnc_len;
            cellIdentity.cellIdentityNr.tac = sCsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityNr.tac > 0xffffff)
                cellIdentity.cellIdentityNr.tac = INT_MAX;
            cellIdentity.cellIdentityNr.nci = sCsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityNr.nci > 68719476735)
                cellIdentity.cellIdentityNr.nci = LONG_MAX;
            cellIdentity.cellIdentityNr.pci = info.pci;
            cellIdentity.cellIdentityNr.nrarfcn = info.arfcn;
            cellIdentity.cellIdentityNr.band = info.band;
            cellIdentity.cellIdentityNr.operName.long_name = longname;
            cellIdentity.cellIdentityNr.operName.short_name = shortname;
            break;

        case RIL_CELL_INFO_TYPE_TD_SCDMA:
            //TD_SCDMA is impossible EXIST here
            RFX_ASSERT(false);
            TELEPHONYWARE_FALLTHROUGH;
        default:
            if (longname) freeSetNull(longname);
            if (shortname) freeSetNull(shortname);
            break;

    }
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
}

void RmmNwHandler::fillDataCellIdentity(RIL_CellIdentity_v16 &cellIdentity) {
    int mcc = 0;
    int mnc = 0;
    int mnc_len = 0;
    int mccmnc_len = 0;
    char *longname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char *shortname = (char*) calloc(MAX_OPER_NAME_LENGTH, sizeof(char));
    char mccMnc[8] = {0};

    if (longname == NULL || shortname == NULL) {
        if (longname) freeSetNull(longname);
        if (shortname) freeSetNull(shortname);
        logE(LOG_TAG, "[%s] longname or shortname is null", __FUNCTION__);
        return;
    }
    pthread_mutex_lock(&sOpInfoMutex[m_slot_id]);
    strlcpy(mccMnc, sOpInfoCache[m_slot_id]->mccMnc, 8);
    strlcpy(longname, sOpInfoCache[m_slot_id]->longName, MAX_OPER_NAME_LENGTH);
    strlcpy(shortname, sOpInfoCache[m_slot_id]->shortName, MAX_OPER_NAME_LENGTH);
    pthread_mutex_unlock(&sOpInfoMutex[m_slot_id]);
    mccmnc_len = strlen(mccMnc);
    if (mccmnc_len == 5 || mccmnc_len == 6) {
        mnc_len = (mccmnc_len == 6) ? 3 : 2;
        char mcc_str[4] = {0};
        char mnc_str[4] = {0};
        strncpy(mcc_str, mccMnc, 3);
        strncpy(mnc_str, (mccMnc + 3), (mccmnc_len - 3));
        mcc = atoi(mcc_str);
        mnc = atoi(mnc_str);
    } else {
        mcc = INT_MAX;
        mnc = INT_MAX;
        // reset longname/shortname because mccmnc may be empty and these are empty too.
        // It happends when the MD is in service right after poll_operator and before poll_voice/data
        memset(longname, 0, MAX_OPER_NAME_LENGTH);
        memset(shortname, 0, MAX_OPER_NAME_LENGTH);
    }

    pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
    cellIdentity.cellInfoType =
            getCellInfoTypeRadioTechnology(convertPSNetworkType(sPsRegStateCache[m_slot_id]->radio_technology));
    CellIdentityInfo info{INT_MAX, INT_MAX, 0, 0};
    if (isInService(sPsRegStateCache[m_slot_id]->registration_state) &&
            cellIdentity.cellInfoType != RIL_CELL_INFO_TYPE_CDMA) {
        queryPrimaryCellIdentityInfo(&info);
    }
    switch(cellIdentity.cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM: {
            cellIdentity.cellIdentityGsm.mcc = mcc;
            cellIdentity.cellIdentityGsm.mnc = mnc;
            cellIdentity.cellIdentityGsm.mnc_len = mnc_len;
            cellIdentity.cellIdentityGsm.lac = sPsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityGsm.lac > 0xffff)
                cellIdentity.cellIdentityGsm.lac = INT_MAX;
            cellIdentity.cellIdentityGsm.cid = (int)sPsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityGsm.cid > 0xffff)
                cellIdentity.cellIdentityGsm.cid = INT_MAX;
            cellIdentity.cellIdentityGsm.arfcn = info.arfcn;
            cellIdentity.cellIdentityGsm.bsic = info.pci;
            cellIdentity.cellIdentityGsm.operName.long_name = longname;
            cellIdentity.cellIdentityGsm.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_WCDMA: {
            cellIdentity.cellIdentityWcdma.mcc = mcc;
            cellIdentity.cellIdentityWcdma.mnc = mnc;
            cellIdentity.cellIdentityWcdma.mnc_len= mnc_len;
            cellIdentity.cellIdentityWcdma.lac = sPsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityWcdma.lac > 0xffff)
                cellIdentity.cellIdentityWcdma.lac = INT_MAX;
            cellIdentity.cellIdentityWcdma.cid = (int) sPsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityWcdma.cid > 268435455)
                cellIdentity.cellIdentityWcdma.cid = INT_MAX;
            cellIdentity.cellIdentityWcdma.psc = info.pci;
            cellIdentity.cellIdentityWcdma.uarfcn = info.arfcn;
            cellIdentity.cellIdentityWcdma.operName.long_name = longname;
            cellIdentity.cellIdentityWcdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_CDMA: {
            cellIdentity.cellIdentityCdma.networkId = sCsRegStateCache[m_slot_id]->network_id;
            cellIdentity.cellIdentityCdma.systemId = sCsRegStateCache[m_slot_id]->system_id;
            cellIdentity.cellIdentityCdma.basestationId = sCsRegStateCache[m_slot_id]->base_station_id;
            cellIdentity.cellIdentityCdma.longitude = sCsRegStateCache[m_slot_id]->base_station_longitude;
            cellIdentity.cellIdentityCdma.latitude = sCsRegStateCache[m_slot_id]->base_station_latitude;
            cellIdentity.cellIdentityCdma.operName.long_name = longname;
            cellIdentity.cellIdentityCdma.operName.short_name = shortname;
            break;
        }

        case RIL_CELL_INFO_TYPE_LTE: {
            cellIdentity.cellIdentityLte.mcc = mcc;
            cellIdentity.cellIdentityLte.mnc = mnc;
            cellIdentity.cellIdentityLte.mnc_len= mnc_len;
            cellIdentity.cellIdentityLte.ci = (int) sPsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityLte.ci > 268435455)
                cellIdentity.cellIdentityLte.ci = INT_MAX;
            cellIdentity.cellIdentityLte.tac = sPsRegStateCache[m_slot_id]->lac; //RILD doesn't use Tac.
            if (cellIdentity.cellIdentityLte.tac > 0xffff)
                cellIdentity.cellIdentityLte.tac = INT_MAX;
            cellIdentity.cellIdentityLte.pci = info.pci;
            cellIdentity.cellIdentityLte.earfcn = info.arfcn;
            cellIdentity.cellIdentityLte.band = info.band;
            cellIdentity.cellIdentityLte.operName.long_name = longname;
            cellIdentity.cellIdentityLte.operName.short_name = shortname;
            cellIdentity.cellIdentityLte.bandwidth = info.bandwidth;
            break;
        }

        case RIL_CELL_INFO_TYPE_NR:
            cellIdentity.cellIdentityNr.mcc = mcc;
            cellIdentity.cellIdentityNr.mnc = mnc;
            cellIdentity.cellIdentityNr.mnc_len = mnc_len;
            cellIdentity.cellIdentityNr.tac = sPsRegStateCache[m_slot_id]->lac;
            if (cellIdentity.cellIdentityNr.tac > 0xffffff)
                cellIdentity.cellIdentityNr.tac = INT_MAX;
            cellIdentity.cellIdentityNr.nci = sPsRegStateCache[m_slot_id]->cid;
            if (cellIdentity.cellIdentityNr.nci > 68719476735)
                cellIdentity.cellIdentityNr.nci = LONG_MAX;
            cellIdentity.cellIdentityNr.pci = info.pci;
            cellIdentity.cellIdentityNr.nrarfcn = info.arfcn;
            cellIdentity.cellIdentityNr.band = info.band;
            cellIdentity.cellIdentityNr.operName.long_name = longname;
            cellIdentity.cellIdentityNr.operName.short_name = shortname;
            break;

        case RIL_CELL_INFO_TYPE_TD_SCDMA:
            //TD_SCDMA is impossible EXIST here
            RFX_ASSERT(false);
            TELEPHONYWARE_FALLTHROUGH;
        default:
            if (longname) freeSetNull(longname);
            if (shortname) freeSetNull(shortname);
            break;
    }
    pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);
}

void RmmNwHandler::clearCellIdentity(RIL_CellIdentity_v16 &cellIdentity) {
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


RIL_CellInfoType RmmNwHandler::getCellInfoTypeRadioTechnology(int rat) {
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
        case RADIO_TECH_TD_SCDMA: {
            return RIL_CELL_INFO_TYPE_TD_SCDMA;
        }
        case RADIO_TECH_NR:
            return RIL_CELL_INFO_TYPE_NR;
        default: {
            break;
        }
    }
    return RIL_CELL_INFO_TYPE_NONE;
}


bool RmmNwHandler::refreshCellInfoList(bool forceRefresh) {
    int err, num = 0;
    RIL_CellInfo_v12 *response = NULL;
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> urc;
    bool success = true;
    uint64_t cid, csCid, psCid;
    unsigned int lac, csLac, psLac;
    bool csInService, psInService;

    pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
    psInService = isInService(sPsRegStateCache[m_slot_id]->registration_state);
    psCid = sPsRegStateCache[m_slot_id]->cid;
    psLac = sPsRegStateCache[m_slot_id]->lac;
    pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);

    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    csInService = isInService(sCsRegStateCache[m_slot_id]->registration_state);
    csCid = sCsRegStateCache[m_slot_id]->cid;
    csLac = sCsRegStateCache[m_slot_id]->lac;
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);

    if (!forceRefresh) {
        if (psInService) {
            if (psLac != sPrimaryCellInfoCache[m_slot_id]->lacTac ||
                 psCid != sPrimaryCellInfoCache[m_slot_id]->cid) {
                cid = psCid;
                lac = psLac;
            } else {
                logV(LOG_TAG, "[%s]: no need refresh, cache cell is same with PS", __FUNCTION__);
                return success;
            }
        } else if (csInService) {
            if (csLac != sPrimaryCellInfoCache[m_slot_id]->lacTac ||
                 csCid != sPrimaryCellInfoCache[m_slot_id]->cid) {
                cid = csCid;
                lac = csLac;
            } else {
                logV(LOG_TAG, "[%s]: no need refresh, cache cell is same with CS", __FUNCTION__);
                return success;
            }
        } else {
            logV(LOG_TAG, "[%s]: no need refresh, both CS&PS are OOS", __FUNCTION__);
            return success;
        }
    }

    sp<RfxMipcData> requestData = RfxMipcData::obtainMipcData(MIPC_NW_GET_BASE_STATIONS_REQ,
            m_slot_id);
    sp<RfxMipcData> confirmData = callToMipcMsgSync(requestData);
    if (confirmData->getResult() != MIPC_RESULT_SUCCESS) {
        success = false;
        goto error;
    }

    // total num
    num = confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_COUNT, 0);
    num += confirmData->getMipcUint32Val(MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_COUNT, 0);

    logV(LOG_TAG, "[%s] +++++, num=%d, forceRefresh=%d", __FUNCTION__, num, forceRefresh);
    if (num > 0) {
        RIL_CELl_INFO_TLV_KEYS keys = {
            MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_GSM_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_CDMA_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_UMTS_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_COUNT,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_LTE_CELL_LIST_V1,
            MIPC_NW_GET_BASE_STATIONS_CNF_T_NR_CELL_LIST_V1
        };
        response = (RIL_CellInfo_v12 *) alloca(num * sizeof(RIL_CellInfo_v12));
        memset(response, 0, num * sizeof(RIL_CellInfo_v12));
        err = getCellInfoListV12(confirmData.get(), response, keys);
        if (err != 0) {
            success = false;
            goto error;
        }
        if ((csInService || psInService) && sPrimaryCellInfoCache[m_slot_id]->act == 256) {
            //+ECELL has no cid & lac.
            sPrimaryCellInfoCache[m_slot_id]->cid = cid;
            sPrimaryCellInfoCache[m_slot_id]->lacTac = lac;
        }

        if (!forceRefresh && (lac != sPrimaryCellInfoCache[m_slot_id]->lacTac ||
             cid != sPrimaryCellInfoCache[m_slot_id]->cid)) {
            logV(LOG_TAG, "[%s], response is not our expection", __FUNCTION__);
            success = false;
            goto error;
        }
        logV(LOG_TAG, "[%s], update done, report URC to RILJ", __FUNCTION__);
        urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_CELL_INFO_LIST,
         m_slot_id, RfxCellInfoData(response, num * sizeof(RIL_CellInfo_v12)));
        responseToTelCore(urc);
    }
error:
    logV(LOG_TAG, "[%s]: success = %s", __FUNCTION__, success ? "true": "false");
    RIL_CELL_INFO_CACHE *pCellCache = sPrimaryCellInfoCache[m_slot_id];
    dumpPrimaryCellInfoCache(__func__, pCellCache);
    freeOperatorNameOfCellInfo(num, response);
    return success;
}

void RmmNwHandler::dumpPrimaryCellInfoCache(const char* fun, RIL_CELL_INFO_CACHE *pCell) {
    char* ecell_cid_s = getMask((long long) pCell->cid);
    char* ecell_tac_s = getMask(pCell->lacTac);
    if (!ecell_cid_s || !ecell_tac_s) {
        logE(LOG_TAG, "[%s] can not getMask due to low memory", fun);
        goto error;
    }
    logD(LOG_TAG, "mMdEcell act=%d, cid=%s, lacTac=%s", pCell->act, ecell_cid_s, ecell_tac_s);
error:
    if (ecell_cid_s) free(ecell_cid_s);
    if (ecell_tac_s) free(ecell_tac_s);
}


int RmmNwHandler::getCellInfoListV12(RfxMipcData* mipcData, RIL_CellInfo_v12 *response,
        RIL_CELl_INFO_TLV_KEYS tlvKeys) {

    int INVALID = INT_MAX; // 0x7FFFFFFF;
    int INVALID_IE = 0XFF;
    int err, j=0,mcc=0,mnc=0,mnc_len=0, count = 0, size = 0, cellIndex = 0, matched = 0;
    uint16_t len = 0;
    void *data = NULL;
    char longName[MAX_OPER_NAME_LENGTH] = {0};
    char shortName[MAX_OPER_NAME_LENGTH] = {0};

    if (mipcData == NULL) {
        goto error;
    }

    // GSM
    count = mipcData->getMipcUint32Val(tlvKeys.keyGsmCount, 0);
    logV(LOG_TAG, "getCellInfoListV12, gsm  count=%d", count);
    if (count > 0) {
        data = mipcData->getMipcVal(tlvKeys.keyGsmList, &len);
        size = sizeof(mipc_nw_gsm_cell_struct4);
        if (data == NULL || len == 0 || len / size != count) {
            logE(LOG_TAG, "wrong GSM data, data= %p, len = %d, size = %d", data, len, size);
            goto error;
        }
        mipc_nw_gsm_cell_struct4* list = (mipc_nw_gsm_cell_struct4 *) data;
        for (j = 0; j < count; j++) {
            response[cellIndex].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
            if (strlen(list->provider_id) > 0) {
                err = transferPlmnToMccMnc(list->provider_id, &mcc, &mnc, &mnc_len);
                if (err != 0) {
                    logE(LOG_TAG, "getCellInfoListV12,GSM: plmn(%s) is wrong", list->provider_id);
                    goto error;
                }
            } else { // if no plmn, set INT_MAX as invalid
                mcc = INT_MAX;
                mnc = INT_MAX;
                mnc_len = 0;
            }
            char* cid_s = getMask(list->cid);
            char* lac_s = getMask(list->lac);
            logV(LOG_TAG, "GSM: registred=%u, state = %u, plmn = %s,"
                "mcc = %d, mnc = %d, mnc_len = %d, "
                "base_station_id = %u, arfch = %u, cid = %s, lac = %s, rx_level = %d, "
                "bitErrorRate = %d, ta = %d, long_name = %s, short_name = %s",
                list->registered, list->state, list->provider_id, mcc, mnc, mnc_len,
                list->base_station_id, list->arfcn, cid_s, lac_s, list->rx_level,
                list->bitErrorRate, list->ta, list->long_name, list->short_name);
            if (cid_s) free(cid_s);
            if (lac_s) free(lac_s);
            if (list->state == 1) {  // serving
                sPrimaryCellInfoCache[m_slot_id]->act = 0;  // GSM
                sPrimaryCellInfoCache[m_slot_id]->cid = list->cid;
                sPrimaryCellInfoCache[m_slot_id]->lacTac = list->lac;
                response[cellIndex].connectionStatus = PRIMARY_SERVING;
            } else {
                response[cellIndex].connectionStatus = NONE_SERVING;
            }
            response[cellIndex].registered = list->registered > 0 ? 1 : 0;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.mnc_len= mnc_len;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.mcc= mcc;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.mnc= mnc;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.cid = (int) list->cid;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.lac = (int) list->lac;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.arfcn = (int) list->arfcn;
            response[cellIndex].CellInfo.gsm.cellIdentityGsm.bsic = (int) list->base_station_id;
            int aosp_level = (list->rx_level + 2) / 2;  // rx_level == sig1
            if (aosp_level > 31) aosp_level = 31;
            else if (aosp_level < 0) aosp_level = 0;
            response[cellIndex].CellInfo.gsm.signalStrengthGsm.signalStrength = aosp_level;
            int timingAdvance = (int) list->ta;
            if (timingAdvance < 0 && timingAdvance != INVALID) timingAdvance = 0;
            else if (timingAdvance > 219 && timingAdvance != INVALID) timingAdvance = 219;
            response[cellIndex].CellInfo.gsm.signalStrengthGsm.timingAdvance = timingAdvance;
            if (strlen(list->provider_id) > 0) {
                matched = matchPlmnName(longName, shortName, list->provider_id);
                if (MATCH_NONE == matched) {
                    longName[0] = 0;
                    shortName[0] = 0;
                }
            }
            logV(LOG_TAG, "[%s] GSM: PLMN = %s, LName = %s, SName=%s, matched = %d",
                    __FUNCTION__, list->provider_id, longName, shortName, matched);
            asprintf(&(response[cellIndex].CellInfo.gsm.cellIdentityGsm.operName.long_name),
                    "%s", longName);
            asprintf(&(response[cellIndex].CellInfo.gsm.cellIdentityGsm.operName.short_name),
                    "%s", shortName);
            response[cellIndex].CellInfo.gsm.signalStrengthGsm.bitErrorRate = list->bitErrorRate;
            cellIndex ++;
            list ++;
        }
    }

    // CDMA
    count = mipcData->getMipcUint32Val(tlvKeys.keyCdmaCount, 0);
    logV(LOG_TAG, "getCellInfoListV12, CDMA count=%d", count);
    if (count > 0) {
        data = mipcData->getMipcVal(tlvKeys.keyCdmaList, &len);
        size = sizeof(mipc_nw_cdma_cell_struct4);
        if (data == NULL || len == 0 || len / size != count) {
           logE(LOG_TAG, "wrong LTE data, data= %p, len = %d, size = %d", data, len, size);
           goto error;
        }
        mipc_nw_cdma_cell_struct4* list = (mipc_nw_cdma_cell_struct4 *) data;
        char* nid_s = getMask(list->networkId);
        char* sid_s = getMask(list->systemId);
        char* bid_s = getMask(list->basestationId);
        char* longitude_s = getMask(list->longitude);
        char* latitude_s = getMask(list->latitude);
        logD(LOG_TAG, "CDMA: reg=%u, state=%u, nid=%s, sid=%s, bid=%s, longitude=%s,latitue=%s, "
                "dbm_cdma = %u, ecio_cdma = %u, dbm_evdo = %u, ecio_evdo= %u, snr_evdo = %u",
                list->registered, list->state, nid_s, sid_s, bid_s, longitude_s,  latitude_s,
                list->dbm_cdma, list->ecio_cdma, list->dbm_evdo, list->ecio_evdo, list->snr_evdo);
        if (nid_s) free(nid_s);
        if (sid_s) free(sid_s);
        if (bid_s) free(bid_s);
        if (longitude_s) free(longitude_s);
        if (latitude_s) free(latitude_s);
        for (j = 0; j < count; j++) {
            if (list->state == 1) {
                sPrimaryCellInfoCache[m_slot_id]->act = 256; // CDMA
            }
            response[cellIndex].cellInfoType = RIL_CELL_INFO_TYPE_CDMA;
            response[cellIndex].connectionStatus =
                    list->state == 1 ? PRIMARY_SERVING : NONE_SERVING;
            response[cellIndex].registered = list->registered > 0 ? 1 : 0;
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.networkId = list->networkId;
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.systemId = list->systemId;
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.basestationId = list->basestationId;
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.longitude = list->longitude;
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.latitude = list->latitude;
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.operName.long_name = NULL;  // unknown
            response[cellIndex].CellInfo.cdma.cellIdentityCdma.operName.short_name = NULL;  // unknown

            ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
            if (mViaHandler != NULL && list->dbm_cdma != INVALID) {
                response[cellIndex].CellInfo.cdma.signalStrengthCdma.dbm =
                        mViaHandler->convertCdmaEvdoSig(list->dbm_cdma, SIGNAL_CDMA_DBM);
            } else {
                response[cellIndex].CellInfo.cdma.signalStrengthCdma.dbm = INVALID;
            }
            if (mViaHandler != NULL && list->ecio_cdma != INVALID) {
                response[cellIndex].CellInfo.cdma.signalStrengthCdma.ecio =
                     mViaHandler->convertCdmaEvdoSig(list->ecio_cdma, SIGNAL_CDMA_ECIO);
            } else {
                response[cellIndex].CellInfo.cdma.signalStrengthCdma.ecio = INVALID;
            }
            if (mViaHandler != NULL && list->dbm_evdo != INVALID) {
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.dbm =
                        mViaHandler->convertCdmaEvdoSig(list->dbm_evdo, SIGNAL_EVDO_DBM);
            } else {
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.dbm = INVALID;

            }
            if (mViaHandler != NULL && list->ecio_evdo != INVALID) {
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.ecio =
                        mViaHandler->convertCdmaEvdoSig(list->ecio_evdo, SIGNAL_EVDO_ECIO);
            } else {
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.ecio = INVALID;
            }

            response[cellIndex].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio = list->snr_evdo;
            logV(LOG_TAG, "RIL_CELL_INFO_TYPE_C2K cdma_dbm=%d, "
                "cdma_ecio=%d, evdo_dbm=%d, evdo_ecio=%d, evdo_snr=%d ",
                response[cellIndex].CellInfo.cdma.signalStrengthCdma.dbm,
                response[cellIndex].CellInfo.cdma.signalStrengthCdma.ecio,
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.dbm,
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.ecio,
                response[cellIndex].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio);
            cellIndex ++;
            list ++;
        }
    }

    // UMTS
    count = mipcData->getMipcUint32Val(tlvKeys.keyUmtsCount, 0);
    logV(LOG_TAG, "getCellInfoListV12, UMTS count=%d", count);
    if (count > 0) {
        data = mipcData->getMipcVal(tlvKeys.keyUmtsList, &len);
        size = sizeof(mipc_nw_umts_cell_struct4);
        if (data == NULL || len == 0 || len / size != count) {
            logE(LOG_TAG, "wrong UTMS data, data= %p, len = %d, size = %d", data, len, size);
            goto error;
        }
        mipc_nw_umts_cell_struct4* list = (mipc_nw_umts_cell_struct4 *) data;
        for (j = 0; j < count; j++) {
            response[cellIndex].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
            if (strlen(list->provider_id) > 0) {
                err = transferPlmnToMccMnc(list->provider_id, &mcc, &mnc, &mnc_len);
                if (err != 0) {
                    logE(LOG_TAG, "getCellInfoListV12,UMTS: plmn(%s) is wrong", list->provider_id);
                    goto error;
                }
            }  else { // if no plmn, set INT_MAX as invalid
                mcc = INT_MAX;
                mnc = INT_MAX;
                mnc_len = 0;
            }
            char* cid_s = getMask(list->cid);
            char* lac_s = getMask(list->lac);
            char* psc_s = getMask(list->psc);
            logV(LOG_TAG, "UMTS: registred=%d, stat = %d, plmn = %s,"
                "mcc = %d, mnc = %d, mnc_len = %d,"
                "cid = %s, lac = %s, psc = %s, uarfcn = %u, rscp = %d,  echo = %d,"
                " long_name = %s, short_name = %s",
                list->registered, list->state, list->provider_id,  mcc, mnc, mnc_len,
                cid_s, lac_s, psc_s, list->uarfcn, list->rscp, list->ecno,
                list->long_name, list->short_name);
            if (cid_s) free(cid_s);
            if (lac_s) free(lac_s);
            if (psc_s) free(psc_s);
            if (list->state == 1) {  // serving
                sPrimaryCellInfoCache[m_slot_id]->act = 2;  // UMTS
                sPrimaryCellInfoCache[m_slot_id]->cid = list->cid;
                sPrimaryCellInfoCache[m_slot_id]->lacTac = list->lac;
                response[cellIndex].connectionStatus = PRIMARY_SERVING;
            } else {
                response[cellIndex].connectionStatus = NONE_SERVING;
            }
            response[cellIndex].registered = list->registered > 0 ? 1 : 0;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.mnc_len= mnc_len;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.cid = (int) list->cid;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.lac = (int) list->lac;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.psc = (int) list->psc;
            response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.uarfcn = (int) list->uarfcn;
            response[cellIndex].CellInfo.wcdma.signalStrengthWcdma.signalStrength =
                    convert3GRssiValue(list->rscp-120);  // rscp == sig1
            response[cellIndex].CellInfo.wcdma.signalStrengthWcdma.rscp = list->rscp;
            response[cellIndex].CellInfo.wcdma.signalStrengthWcdma.ecno = list->ecno;  // ecno == sig2
            if (strlen(list->provider_id) > 0) {
                matched = matchPlmnName(longName, shortName, list->provider_id);
                if (MATCH_NONE == matched) {
                    longName[0] = 0;
                    shortName[0] = 0;
                }
            }
            asprintf(&(response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.operName.long_name),
                    "%s", longName);
            asprintf(&(response[cellIndex].CellInfo.wcdma.cellIdentityWcdma.operName.short_name),
                    "%s", shortName);
            response[cellIndex].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate = 99;
            cellIndex ++;
            list ++;
        }
    }

    // LTE
    count = mipcData->getMipcUint32Val(tlvKeys.keyLteCount, 0);
    logV(LOG_TAG, "getCellInfoListV12, LTE count=%d", count);
    if (count > 0) {
        data = mipcData->getMipcVal(tlvKeys.keyLteList, &len);
        size = sizeof(mipc_nw_lte_cell_struct4);
        if (data == NULL || len == 0 || len / size != count) {
            logE(LOG_TAG, "wrong LTE data, data= %p, len = %d, size = %d", data, len, size);
            goto error;
        }
        mipc_nw_lte_cell_struct4* list = (mipc_nw_lte_cell_struct4 *) data;
        //extends CQI Table Index
        mipc_nw_lte_cell_v1_struct4* listV1 = NULL;
        data = mipcData->getMipcVal(tlvKeys.keyLteListV1, &len);
        size = sizeof(mipc_nw_lte_cell_v1_struct4);
        if (data != NULL && len / size == count) {
            listV1 = (mipc_nw_lte_cell_v1_struct4 *) data;
        }
        for (j = 0; j < count; j++) {
            response[cellIndex].cellInfoType = RIL_CELL_INFO_TYPE_LTE;
            if (strlen(list->provider_id) > 0) {
                err = transferPlmnToMccMnc(list->provider_id, &mcc, &mnc, &mnc_len);
                if (err != 0) {
                    logE(LOG_TAG, "getCellInfoListV12,LTE: plmn(%s) is wrong", list->provider_id);
                    goto error;
                }
            }  else { // if no plmn, set INT_MAX as invalid
                mcc = INT_MAX;
                mnc = INT_MAX;
                mnc_len = 0;
            }
            char* cid_s = getMask(list->cid);
            char* tac_s = getMask(list->tac);
            char* pci_s = getMask(list->physical_cell_id);
            logV(LOG_TAG, "LTE: state = %d, registred=%d, plmn = %s,"
                    "mcc = %d, mnc = %d, mnc_len = %d, arfch = %d,"
                    "cid = %s, tac = %s, pci = %s, rsrp = %d, rsrp_in_qdbm = %d,"
                    " rsrq = %d, rsrq_in_db = %d, cqi = %u, rssnr = %u, band = %u,"
                    "long_name = %s, short_name = %s",
            list->state, list->registered, list->provider_id ,mcc, mnc, mnc_len, list->earfcn,
            cid_s,  tac_s, pci_s, list->rsrp, list->rsrp_in_qdbm,
            list->rsrq, list->rsrq_in_qdbm, list->cqi, list->rssnr, list->dl_freq_band,
            list->long_name, list->short_name);
            if (cid_s) free(cid_s);
            if (tac_s) free(tac_s);
            if (pci_s) free(pci_s);
            if (list->state == 1) {  // serving
                sPrimaryCellInfoCache[m_slot_id]->act = 7;  // LTE
                sPrimaryCellInfoCache[m_slot_id]->cid = list->cid;
                sPrimaryCellInfoCache[m_slot_id]->lacTac = list->tac;
                response[cellIndex].connectionStatus = PRIMARY_SERVING;
            } else {
                response[cellIndex].connectionStatus = NONE_SERVING;
            }
            response[cellIndex].registered = list->registered > 0 ? 1 : 0;
            response[cellIndex].CellInfo.lte.cellIdentityLte.mcc = mcc;
            response[cellIndex].CellInfo.lte.cellIdentityLte.mnc = mnc;
            response[cellIndex].CellInfo.lte.cellIdentityLte.mnc_len= mnc_len;
            response[cellIndex].CellInfo.lte.cellIdentityLte.ci = list->cid;
            response[cellIndex].CellInfo.lte.cellIdentityLte.tac = (int) list->tac;
            response[cellIndex].CellInfo.lte.cellIdentityLte.pci = (int) list->physical_cell_id;
            response[cellIndex].CellInfo.lte.cellIdentityLte.earfcn =(int) list->earfcn;
            if (list->dl_freq_band >= 1 && list->dl_freq_band <=88) {
                response[cellIndex].CellInfo.lte.cellIdentityLte.band =(int) list->dl_freq_band;
            } else {
                response[cellIndex].CellInfo.lte.cellIdentityLte.band = 1;
            }
            if (strlen(list->provider_id) > 0) {
                matched = matchPlmnName(longName, shortName, list->provider_id);
                if (MATCH_NONE == matched) {
                    longName[0] = 0;
                    shortName[0] = 0;
                }
            }
            logV(LOG_TAG, "[%s] LTE: PLMN = %s, LName = %s, SName=%s, matched = %d",
                    __FUNCTION__, list->provider_id, longName, shortName, matched);
            asprintf(&(response[cellIndex].CellInfo.lte.cellIdentityLte.operName.long_name),
                    "%s", longName);
            asprintf(&(response[cellIndex].CellInfo.lte.cellIdentityLte.operName.short_name),
                    "%s", shortName);

            /* For LTE, the ext1((rssnr)) in +ECELL is rssi, and format follows 27.007 +CSQ. alps05878992  */
            response[cellIndex].CellInfo.lte.signalStrengthLte.signalStrength = list->rssnr;

            int aosp_rsrp = -list->rsrp_in_qdbm / 4;
            if (aosp_rsrp > 140) aosp_rsrp = 140;
            else if (aosp_rsrp < 44) aosp_rsrp = 44;
            response[cellIndex].CellInfo.lte.signalStrengthLte.rsrp = aosp_rsrp;

            int aosp_rsrq = list->rsrq_in_qdbm != INVALID ? -list->rsrq_in_qdbm / 4 : INVALID;
            if (aosp_rsrq > 35 && aosp_rsrq != INVALID) aosp_rsrq = 35;
            else if (aosp_rsrq < 3 && aosp_rsrq != INVALID) aosp_rsrq = 3;
            response[cellIndex].CellInfo.lte.signalStrengthLte.rsrq = aosp_rsrq;

            int aosp_ta = list->ta != INVALID ? (int) (list->ta) : INVALID;
            if (aosp_ta < 0 && aosp_ta != INVALID) aosp_ta = 0;
            else if (aosp_ta > 1282 && aosp_ta != INVALID) aosp_ta = 1282;
            response[cellIndex].CellInfo.lte.signalStrengthLte.timingAdvance = aosp_ta;

            /*For LTE, rssnr is not reported. alps05878992.
              Just fill the registered item by the rssnr in signal_strength_cache*/
            if (response[cellIndex].registered == 1) {
                response[cellIndex].CellInfo.lte.signalStrengthLte.rssnr =
                        signal_strength_cache[m_slot_id]->lte_rssnr;
            } else {
                response[cellIndex].CellInfo.lte.signalStrengthLte.rssnr = 0;
            }

            int cqi = list->cqi;
            if (cqi < 0 && cqi != INVALID) cqi = 0;
            else if (cqi > 30 && cqi != INVALID) cqi = 30;
            response[cellIndex].CellInfo.lte.signalStrengthLte.cqi = cqi;
            if (listV1 == NULL) {
                response[cellIndex].CellInfo.lte.signalStrengthLte.cqiTableIndex = INT_MAX;
            } else {
                response[cellIndex].CellInfo.lte.signalStrengthLte.cqiTableIndex = listV1->cqi_table_index;
                listV1 ++;
            }
            cellIndex ++;
            list ++;
        }
    }

    // NR
    count = mipcData->getMipcUint32Val(tlvKeys.keyNrCount, 0);
    logV(LOG_TAG, "getCellInfoListV12, NR count=%d", count);
    if (count > 0) {
        data = mipcData->getMipcVal(tlvKeys.keyNrList, &len);
        size = sizeof(mipc_nw_nr_cell_struct4);
        if (data == NULL || len == 0 || len / size != count) {
            logE(LOG_TAG, "wrong LTE data, data= %p, len = %d, size = %d", data, len, size);
            goto error;
        }
        mipc_nw_nr_cell_struct4* list = (mipc_nw_nr_cell_struct4 *) data;

        //extends CQI Table Index
        mipc_nw_nr_cell_v1_struct4* listV1 = NULL;
        data = mipcData->getMipcVal(tlvKeys.keyNrListV1, &len);
        size = sizeof(mipc_nw_nr_cell_v1_struct4);
        if (data != NULL && len / size == count) {
            listV1 = (mipc_nw_nr_cell_v1_struct4 *) data;
        }
        for (j = 0; j < count; j++) {
            response[cellIndex].cellInfoType = RIL_CELL_INFO_TYPE_NR;
            if (strlen(list->provider_id) > 0) {
                err = transferPlmnToMccMnc(list->provider_id, &mcc, &mnc, &mnc_len);
                if (err != 0) {
                    logE(LOG_TAG, "getCellInfoListV12,NR: plmn(%s) is wrong", list->provider_id);
                    list ++;
                    continue;
                }
            }  else { // if no plmn, set INT_MAX as invalid
                mcc = INT_MAX;
                mnc = INT_MAX;
                mnc_len = 0;
            }
            if (j == 0) { // only print primary item
                char* cid_s = getMask((unsigned long long) (list->cid));
                char* tac_s = getMask(list->tac);
                char* pci_s = getMask(list->physical_cell_id);
                logD(LOG_TAG, "NR:state = %d, registred=%d, plmn = %s,"
                        "pscPci = %s, nr_arfcn = %d, cid = %s, tac = %s, rsrp = %d, rsrq = %d,"
                        "sinr = %d, ta = %d, csirsrp = %u, csirsrq = %u, band = %u,"
                        "long_name = %s, short_name = %s, err = %d",
                    list->state, list->registered, list->provider_id,
                    pci_s, list->nr_arfcn, cid_s, tac_s, list->rsrp, list->rsrq,
                    list->sinr, list->ta, list->csirsrp, list->csirsrq, list->dl_freq_band,
                    list->long_name, list->short_name, err);
                if (cid_s) free(cid_s);
                if (tac_s) free(tac_s);
                if (pci_s) free(pci_s);
            }
            if (list->state == 1) {  // serving
                sPrimaryCellInfoCache[m_slot_id]->act = 11;   // NR
                sPrimaryCellInfoCache[m_slot_id]->cid = list->cid;
                sPrimaryCellInfoCache[m_slot_id]->lacTac = list->tac;
                response[cellIndex].connectionStatus = PRIMARY_SERVING;
            } else {
                response[cellIndex].connectionStatus = NONE_SERVING;
            }
            response[cellIndex].registered = list->registered > 0 ? 1 : 0;
            response[cellIndex].CellInfo.nr.cellidentity.mcc = mcc;
            response[cellIndex].CellInfo.nr.cellidentity.mnc = mnc;
            response[cellIndex].CellInfo.nr.cellidentity.mnc_len= mnc_len;
            response[cellIndex].CellInfo.nr.cellidentity.nci = list->cid;
            response[cellIndex].CellInfo.nr.cellidentity.nrarfcn = (int) list->nr_arfcn;
            response[cellIndex].CellInfo.nr.cellidentity.pci = list->physical_cell_id;
            response[cellIndex].CellInfo.nr.cellidentity.tac = (int) list->tac;
            if ((list->dl_freq_band >= 1 && list->dl_freq_band <=95)
                    || (list->dl_freq_band >= 257 && list->dl_freq_band <=261)) {
                response[cellIndex].CellInfo.nr.cellidentity.band =(int) list->dl_freq_band;
            } else {
                response[cellIndex].CellInfo.nr.cellidentity.band = 1;
            }
            if (strlen(list->provider_id) > 0) {
                matched = matchPlmnName(longName, shortName, list->provider_id);
                if (MATCH_NONE == matched) {
                    longName[0] = 0;
                    shortName[0] = 0;
                }
            }
            logD(LOG_TAG, "[%s] NR: PLMN = %s, LName = %s, SName=%s, matched = %d",
                    __FUNCTION__, list->provider_id, longName, shortName, matched);
            asprintf(&(response[cellIndex].CellInfo.nr.cellidentity.operName.long_name),
                    "%s", longName);
            asprintf(&(response[cellIndex].CellInfo.nr.cellidentity.operName.short_name),
                    "%s", shortName);

            /*
                mtk_nr_rsrp_qdbm = mtk_nr_rsrp_ie << 2 - 628;
                aosp_rsrp = -1 * mtk_nr_rsrp_qdbm >> 2;
                aosp_rsrp = -1 *(mtk_nr_rsrp_ie << 2 - 628) >>2 = 157 -mtk_nr_rsrp_ie;
            */
            int aosp_nr_ssrsrp =
                list->rsrp != INVALID_IE ? (157-list->rsrp) : INVALID;
            if (aosp_nr_ssrsrp > 140 && aosp_nr_ssrsrp != INVALID) aosp_nr_ssrsrp = 140;
            else if (aosp_nr_ssrsrp < 44 && aosp_nr_ssrsrp != INVALID) aosp_nr_ssrsrp = 44;
            response[cellIndex].CellInfo.nr.signalStrength.ssRsrp = aosp_nr_ssrsrp;

            /*
                mtk_nr_rsrq_qdbm = mtk_nr_rsrq_ie << 1 - 174;
                aosp_rsrq = -1 * mtk_nr_rsrq_qdbm >> 2;
                aosp_rsrp = -1 *(mtk_nr_rsrq_qdbm << 1 - 174) >>2
                            = (174 -mtk_nr_rsrq_ie << 1) >> 2 ;
                            = (87 -mtk_nr_rsrq_ie) >> 1;
            */
            int aosp_nr_ssrsrq =
            list->rsrq != INVALID_IE ? ((87 - list->rsrq) >> 1) : INVALID;
            if (aosp_nr_ssrsrq > 20 && aosp_nr_ssrsrq != INVALID) aosp_nr_ssrsrq = 20;
            else if (aosp_nr_ssrsrq < 3 && aosp_nr_ssrsrq != INVALID) aosp_nr_ssrsrq = 3;
            response[cellIndex].CellInfo.nr.signalStrength.ssRsrq = aosp_nr_ssrsrq;

            /*
                mtk_nr_sinr_qdbm = mtk_nr_sinr_ie << 1 - 94;
                aosp_sinr = mtk_nr_sinr_qdbm >> 2;
                aosp_sinr = (mtk_nr_sinr_ie << 1 - 94) >> 2 ;
                          = (mtk_nr_sinr_ie - 47) >> 1;
            */
            int aosp_nr_sssinr = list->sinr != INVALID_IE ? ((list->sinr -47) >> 1) : INVALID;
            if (aosp_nr_sssinr > 40 && aosp_nr_sssinr != INVALID) aosp_nr_sssinr = 40;
            else if (aosp_nr_sssinr < -23 && aosp_nr_sssinr != INVALID) aosp_nr_sssinr = -23;
            response[cellIndex].CellInfo.nr.signalStrength.ssSinr = aosp_nr_sssinr;

            int aosp_nr_csirsrp =
                list->csirsrp != INVALID_IE ? (157 -list->csirsrp) : INVALID;
            if (aosp_nr_csirsrp > 140 && aosp_nr_csirsrp != INVALID) aosp_nr_csirsrp = 140;
            else if (aosp_nr_csirsrp < 44 && aosp_nr_csirsrp != INVALID) aosp_nr_csirsrp = 44;
            response[cellIndex].CellInfo.nr.signalStrength.csiRsrp = aosp_nr_csirsrp;

            int aosp_nr_csirsrq =
                list->csirsrq != INVALID_IE ? ((87 -list->csirsrq) >> 1) : INVALID;
            if (aosp_nr_csirsrq > 20 && aosp_nr_csirsrq != INVALID) aosp_nr_csirsrq = 20;
            else if (aosp_nr_csirsrq < 3 && aosp_nr_csirsrq != INVALID) aosp_nr_csirsrq = 3;
            response[cellIndex].CellInfo.nr.signalStrength.csiRsrq = aosp_nr_csirsrq;

            int aosp_nr_csisinr =
                list->csisinr != INVALID_IE ? ((list->csisinr - 47) >>1) : INVALID;
            if (aosp_nr_csisinr > 40 && aosp_nr_csisinr != INVALID) aosp_nr_csisinr = 40;
            else if (aosp_nr_csisinr < -23 && aosp_nr_csisinr != INVALID) aosp_nr_csisinr = -23;
            response[cellIndex].CellInfo.nr.signalStrength.csiSinr = aosp_nr_csisinr;
            if (listV1 == NULL) {
                response[cellIndex].CellInfo.nr.signalStrength.csiCqiTableIndex = INT_MAX;
                response[cellIndex].CellInfo.nr.signalStrength.csiCqiReportCount = 0;
            } else { //extends CQI Table Index
                response[cellIndex].CellInfo.nr.signalStrength.csiCqiTableIndex = listV1->csi_cqi_table_index;
                int count = MIN(listV1->csi_cqi_report_num, MAX_COUNT_NR_CSI_CQI_REPORT);
                response[cellIndex].CellInfo.nr.signalStrength.csiCqiReportCount = count;
                for (int index = 0; index < count; index ++) {
                    response[cellIndex].CellInfo.nr.signalStrength.csiCqiReport[index] =
                            listV1->csi_cqi_report_list[index];
                }
                listV1 ++;
            }
            response[cellIndex].CellInfo.nr.signalStrength.timingAdvance = list->ta;
            list ++;
            cellIndex ++;
        }
    }
    return 0;

error:
    return -1;
}

void RmmNwHandler::freeOperatorNameOfCellInfo(int num, RIL_CellInfo_v12 *response) {
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

/* Android framework expect spec 27.007  AT+CSQ <rssi> 0~31 format when handling 3G signal strength.
   So we convert 3G signal to <rssi> in RILD */
int RmmNwHandler::convert3GRssiValue(int rscp_in_dbm) {
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

void RmmNwHandler::handleVoidResponse(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    RIL_Errno rilErrno = result == MIPC_RESULT_SUCCESS ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrno,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmmNwHandler::resetSignalStrengthCache(
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
        for (int i = 0; i < MAX_COUNT_NR_CSI_CQI_REPORT; i++) {
            signalCache->nr_csicqi_reports[i] = 0xFF;
        }
        signalCache->nr_timing_advance = CELLINFO_INVALID;
    }

    if (group == SIGNAL_GROUP_ALL)
        lastReportedSignal[m_slot_id] = SIGNAL_GROUP_NONE;
}

void RmmNwHandler::fillSignalStrengthResponse(RIL_SIGNAL_STRENGTH *response, int group) {
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
        response->lte_cqi_table_index = signalCache->lte_cqi_table_index;
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
        response->nr_csicqi_table_index = signalCache->nr_csicqi_table_index;
        response->nr_csicqi_report_count = signalCache->nr_csicqi_report_count;
        if (response->nr_csicqi_report_count > 0) {
            for (int index =0; index < response->nr_csicqi_report_count; index ++) {
                response->nr_csicqi_reports[index] = signalCache->nr_csicqi_reports[index];
            }
        }
        response->nr_timing_advance = signalCache->nr_timing_advance;
    } else {
        resetSignalStrengthCache(response, SIGNAL_GROUP_NR);
    }

    if (group != SIGNAL_GROUP_NONE)
        lastReportedSignal[m_slot_id] = group;
}

int RmmNwHandler::getCurrentSignalGroup() {
    return getSignalGroupByRegRat(
            convertRegState(sCsRegStateCache[m_slot_id]->registration_state, true),
            convertCSNetworkType(sCsRegStateCache[m_slot_id]->radio_technology),
            convertRegState(sPsRegStateCache[m_slot_id]->registration_state, false),
            convertPSNetworkType(sPsRegStateCache[m_slot_id]->radio_technology));
}

int RmmNwHandler::getSignalGroupByRegRat(
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
                if (scg_connection_state[m_slot_id] == 1)
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

void RmmNwHandler::printSignalStrengthCache(char* source) {
    logV(LOG_TAG,
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
        " lte_cqi_table_index: %d,"
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
        " csiSinr: %d,"
        " nr_csicqi_table_index: %d,"
        " nr_csicqi_report_count: %d,"
        " nr_timing_advance: %d",
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
        signal_strength_cache[m_slot_id]-> lte_cqi_table_index,
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
        signal_strength_cache[m_slot_id]-> csiSinr,
        signal_strength_cache[m_slot_id]-> nr_csicqi_table_index,
        signal_strength_cache[m_slot_id]-> nr_csicqi_report_count,
        signal_strength_cache[m_slot_id]->nr_timing_advance);
}

int RmmNwHandler::getLastReportedSignal() {
    return lastReportedSignal[m_slot_id];
}

bool RmmNwHandler::isRilTestMode() {
    char property_value[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.ril.test_mode", property_value, "0");
    return strcmp(property_value, "1") == 0;
}

// used for report wcdma ecio in signalStength, return true if you want to get WcdmaEcio.
bool RmmNwHandler::enableReportSignalStrengthWithWcdmaEcio() {
    bool enableReport = false;
    if (isRilTestMode()) {
        enableReport = true;
    }
    return enableReport;
}

void RmmNwHandler::dumpCsRegStateCache(const char* fun, const RIL_CS_REG_STATE_CACHE* pCache, bool debugV) {
    char* lac_s = getMask(pCache->lac);
    char* cid_s = getMask((unsigned long long)pCache->cid);
    if (!lac_s || !cid_s) {
        logE(LOG_TAG, "[%s] can not getMask due to low memory", fun);
        goto error;
    }

    if (debugV) {
        logV(LOG_TAG, "[%s][handleCsNetworkStateChanged] stat=%d, lac=%s, cid=%s, eAct=%d, css=%d"
             ", nwk_existence=%d, roam_indicator=%d, cause_type=%d, reject_cause=%d",
             fun, pCache->registration_state, lac_s, cid_s, pCache->radio_technology, pCache->css,
             pCache->network_exist, pCache->roaming_indicator,
             pCache->cause_type, pCache->reject_cause);
    } else {
        logD(LOG_TAG, "[%s][handleCsNetworkStateChanged] stat=%d, lac=%s, cid=%s, eAct=%d, css=%d"
             ", nwk_existence=%d, roam_indicator=%d, cause_type=%d, reject_cause=%d",
             fun, pCache->registration_state, lac_s, cid_s, pCache->radio_technology, pCache->css,
             pCache->network_exist, pCache->roaming_indicator,
             pCache->cause_type, pCache->reject_cause);
    }

error:
    freeSetNull(lac_s);
    freeSetNull(cid_s);
}

void RmmNwHandler::dumpPsRegStateCache(const char* fun, const RIL_PS_REG_STATE_CACHE *pCache, bool debugV) {
    char* lac_s = getMask(pCache->lac);
    char* cid_s = getMask((unsigned long long)pCache->cid);
    if (!lac_s || !cid_s) {
        logE(LOG_TAG, "[%s] can not getMask due to low memory", fun);
        goto error;
    }

    if (debugV) {
        logV(LOG_TAG, "[%s][handlePsNetworkStateChanged] stat=%d, lac=%s, cid=%s, eAct=%d, "
             "reject_cause=%d, dcnr_restricted=%d, endc_sib=%d, endc_available=%d",
             fun, pCache->registration_state, lac_s, cid_s, pCache->radio_technology,
             pCache->reject_cause,
             pCache->dcnr_restricted, pCache->endc_sib, pCache->endc_available);
    } else {
        logD(LOG_TAG, "[%s][handlePsNetworkStateChanged] stat=%d, lac=%s, cid=%s, eAct=%d, "
             "reject_cause=%d, dcnr_restricted=%d, endc_sib=%d, endc_available=%d",
             fun, pCache->registration_state, lac_s, cid_s, pCache->radio_technology,
             pCache->reject_cause,
             pCache->dcnr_restricted, pCache->endc_sib, pCache->endc_available);
    }

error:
    freeSetNull(lac_s);
    freeSetNull(cid_s);
}

void RmmNwHandler::requestSignalStrength(const sp<RfxMclMessage>& msg, bool forceUpdate) {
    int sigUpdated = SIGNAL_GROUP_NONE;
    int sigRegisted = SIGNAL_GROUP_NONE;
    sp<RfxMclMessage> response;
    RIL_SIGNAL_STRENGTH resp;
    RIL_SIGNAL_STRENGTH *rspCache = &resp;
    pthread_mutex_lock(&s_signalStrengthMutex[m_slot_id]);
    RIL_RadioState state =
        (RIL_RadioState)getMclStatusManager()->getIntValue(RFX_STATUS_KEY_RADIO_STATE);
    logV(LOG_TAG, "[%s] forceUpdate:%d, state:%d", __FUNCTION__, forceUpdate, state);
    if (RADIO_STATE_OFF == state) {
        fillSignalStrengthResponse(rspCache, SIGNAL_GROUP_NONE);
    } else {
        sigRegisted = getCurrentSignalGroup();
        // always force update in case OOS
        if (sigRegisted == SIGNAL_GROUP_NONE)
            forceUpdate = true;

        logV(LOG_TAG, "[%s] sigRegisted:%d", __FUNCTION__, sigRegisted);
        if (forceUpdate) {
            sp<RfxMipcData> requestData =
                RfxMipcData::obtainMipcData(MIPC_NW_GET_SIGNAL_REQ, m_slot_id);
            sp<RfxMipcData> confirmData = callToMipcMsgSync(requestData);
            if (confirmData->getResult() == MIPC_RESULT_SUCCESS) {
                RIL_SIGNAL_STRENGTH *signalCache = signal_strength_cache[m_slot_id];
                RIL_SIGNAL_STRENGTH origCache;
                memcpy(&origCache, signalCache, sizeof(RIL_SIGNAL_STRENGTH));
                uint16_t rspLen = 0;
                int signalType =
                    confirmData->getMipcUint8Val(MIPC_NW_GET_SIGNAL_CNF_T_SIGNAL_TYPE, 0xFF);
                switch (signalType) {
                case MIPC_NW_SIGNAL_TYPE_GSM: {
                    mipc_nw_gsm_signal_strength_struct4* pGsmSignal =
                        (mipc_nw_gsm_signal_strength_struct4*)confirmData->getMipcVal(MIPC_NW_GET_SIGNAL_CNF_T_GSM_SIGNAL, &rspLen);
                    if (pGsmSignal && rspLen == sizeof(mipc_nw_gsm_signal_strength_struct4)) {
                        signalCache->gsm_signal_strength = pGsmSignal->signal_strength;
                        signalCache->gsm_bit_error_rate = pGsmSignal->bit_error_rate;
                        signalCache->gsm_timing_advance = pGsmSignal->timing_advance;
                        sigUpdated |= SIGNAL_GROUP_GSM;
                    } else {
                        logE(LOG_TAG, "[%s] failed, pGsmSignal:%p, rspLen:%d, struct len:%d",
                             __FUNCTION__, pGsmSignal, (int)rspLen,
                             (int)sizeof(mipc_nw_gsm_signal_strength_struct4));
                    }
                    break;
                }

                case MIPC_NW_SIGNAL_TYPE_UMTS: {
                    mipc_nw_umts_signal_strength_struct4* pUmtsSignal =
                        (mipc_nw_umts_signal_strength_struct4*)confirmData->getMipcVal(MIPC_NW_GET_SIGNAL_CNF_T_UMTS_SIGNAL, &rspLen);
                    if (pUmtsSignal && rspLen == sizeof(mipc_nw_umts_signal_strength_struct4)) {
                        signalCache->wcdma_signal_strength = pUmtsSignal->signal_strength;
                        signalCache->wcdma_bit_error_rate = pUmtsSignal->bit_error_rate;
                        signalCache->wcdma_scdma_rscp = pUmtsSignal->rscp;
                        signalCache->wcdma_ecno = pUmtsSignal->ecno;
                        sigUpdated |= SIGNAL_GROUP_UMTS;
                    } else {
                        logE(LOG_TAG, "[%s] failed, pUmtsSignal:%p, rspLen:%d, struct len:%d",
                             __FUNCTION__, pUmtsSignal, (int)rspLen,
                             (int)sizeof(mipc_nw_umts_signal_strength_struct4));
                    }
                    break;
                }

                case MIPC_NW_SIGNAL_TYPE_LTE:
                case MIPC_NW_SIGNAL_TYPE_NR_NSA: {
                    mipc_nw_lte_signal_strength_struct4* pLteSignal =
                        (mipc_nw_lte_signal_strength_struct4*)confirmData->getMipcVal(MIPC_NW_GET_SIGNAL_CNF_T_LTE_SIGNAL, &rspLen);
                    if (pLteSignal && rspLen == sizeof(mipc_nw_lte_signal_strength_struct4)) {
                        signalCache->lte_signal_strength = pLteSignal->signal_strength;
                        signalCache->lte_rsrp = pLteSignal->rsrp;
                        signalCache->lte_rsrq = pLteSignal->rsrq;
                        signalCache->lte_rssnr = pLteSignal->rssnr;
                        signalCache->lte_cqi = pLteSignal->cqi;
                        signalCache->lte_timing_advance = pLteSignal->timing_advance;
                        signalCache->lte_cqi_table_index = INT_MAX;
                        sigUpdated |= SIGNAL_GROUP_LTE;
                    } else {
                        logE(LOG_TAG, "[%s] failed, pLteSignal:%p, rspLen:%d, struct len:%d",
                             __FUNCTION__, pLteSignal, (int)rspLen,
                             (int)sizeof(mipc_nw_lte_signal_strength_struct4));
                    }
                    //cqi table index
                    mipc_nw_lte_signal_strength_v1_struct4* pLteSignalV1 =
                        mipc_nw_get_signal_cnf_get_lte_signal_v1(confirmData->getData(), &rspLen);
                    if (pLteSignalV1 != NULL) {
                        signalCache->lte_cqi_table_index = pLteSignalV1->cqi_table_index;
                    }

                    if (signalType == MIPC_NW_SIGNAL_TYPE_LTE) {
                        break;
                    }
                    // else fallthrough, NR_NSA has LTE & NR signal
                    TELEPHONYWARE_FALLTHROUGH;
                }

                case MIPC_NW_SIGNAL_TYPE_NR: {
                    mipc_nw_nr_signal_strength_struct4* pNrSignal =
                        (mipc_nw_nr_signal_strength_struct4*)confirmData->getMipcVal(MIPC_NW_GET_SIGNAL_CNF_T_NR_SIGNAL, &rspLen);
                    if (pNrSignal && rspLen == sizeof(mipc_nw_nr_signal_strength_struct4)) {
                        signalCache->ssRsrp = pNrSignal->ss_rsrp;
                        signalCache->ssRsrq = pNrSignal->ss_rsrq;
                        signalCache->ssSinr = pNrSignal->ss_sinr;
                        signalCache->csiRsrp = pNrSignal->csi_rsrp;
                        signalCache->csiRsrq = pNrSignal->csi_rsrq;
                        signalCache->csiSinr = pNrSignal->csi_sinr;
                        signalCache->nr_csicqi_table_index = INT_MAX;
                        signalCache->nr_csicqi_report_count = 0;
                        signalCache->nr_timing_advance = 0;
                        sigUpdated |= SIGNAL_GROUP_NR;
                    } else {
                        logE(LOG_TAG, "[%s] failed, pNrSignal:%p, rspLen:%d, struct len:%d",
                             __FUNCTION__, pNrSignal, (int)rspLen,
                             (int)sizeof(mipc_nw_nr_signal_strength_struct4));
                    }
                    //extends CQI Table Index
                    mipc_nw_nr_signal_strength_v1_struct4 *pNrSignalV1 =
                            mipc_nw_get_signal_cnf_get_nr_signal_v1(confirmData->getData(), &rspLen);
                    if (pNrSignalV1 != NULL) {
                        signalCache->nr_csicqi_table_index = pNrSignalV1->csi_cqi_table_index;
                        if (pNrSignalV1 ->csi_cqi_report_num > 0) {
                            signalCache->nr_csicqi_report_count =
                                    MIN(pNrSignalV1 ->csi_cqi_report_num, MAX_COUNT_NR_CSI_CQI_REPORT);
                            for (int index = 0; index < signalCache->nr_csicqi_report_count; index ++) {
                                signalCache->nr_csicqi_reports[index] =
                                        pNrSignalV1->csi_cqi_report_list[index];
                            }
                        }
                    }

                    //extends nr timing_advance by v2
                    mipc_nw_nr_signal_strength_v2_struct4 *pNrSignalV2 =
                            mipc_nw_get_signal_cnf_get_nr_signal_v2(confirmData->getData(),
                                                                    &rspLen);
                    if (pNrSignalV2 != NULL) {
                        signalCache->nr_timing_advance = pNrSignalV2->timing_advance;
                    }
                    break;
                }

                case MIPC_NW_SIGNAL_TYPE_CDMA: {
                    mipc_nw_cdma_signal_strength_struct4* pCdmaSignal =
                        (mipc_nw_cdma_signal_strength_struct4*)confirmData->getMipcVal(MIPC_NW_GET_SIGNAL_CNF_T_CDMA_SIGNAL, &rspLen);
                    if (pCdmaSignal && rspLen == sizeof(mipc_nw_cdma_signal_strength_struct4)) {
                        if (pCdmaSignal->snr == CELLINFO_INVALID) { // 1x
                            signalCache->cdma_dbm = pCdmaSignal->dbm;
                            signalCache->cdma_ecio = pCdmaSignal->ecio;
                            sigUpdated |= SIGNAL_GROUP_1XRTT;
                        } else { // evdo
                            signalCache->evdo_dbm = pCdmaSignal->dbm;
                            signalCache->evdo_ecio = pCdmaSignal->ecio;
                            signalCache->evdo_snr = pCdmaSignal->snr;
                            sigUpdated |= SIGNAL_GROUP_EVDO;
                        }
                    } else {
                        logE(LOG_TAG, "[%s] failed, pCdmaSignal:%p, rspLen:%d, struct len:%d",
                             __FUNCTION__, pCdmaSignal, (int)rspLen,
                             (int)sizeof(mipc_nw_cdma_signal_strength_struct4));
                    }
                    break;
                }

                default:
                    break;
                }

                // todo: confirm with MD, lack tds
                if (memcmp(&origCache, signalCache, sizeof(*signalCache)) == 0)
                    sigUpdated = SIGNAL_GROUP_NONE;
            } else {
                logE(LOG_TAG, "[%s] confirmData result:%d", __FUNCTION__, confirmData->getResult());
            }
        }

        logV(LOG_TAG, "[%s]:sigRegisted[%#04X], sigUpdated[%#04X]",
             __FUNCTION__, sigRegisted, sigUpdated);
        printSignalStrengthCache((char*) __FUNCTION__);

        if (msg == NULL && forceUpdate && sigUpdated == SIGNAL_GROUP_NONE)
            goto error;

        if (sigRegisted == SIGNAL_GROUP_NONE) {
            fillSignalStrengthResponse(rspCache, getLastReportedSignal());
        } else {
            fillSignalStrengthResponse(rspCache, sigRegisted);
        }
    }
    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);

    if (msg != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)), msg, false);
        logV(LOG_TAG, "[%s] send rsp ", __FUNCTION__);
        responseToTelCore(response);
    } else {
        if ((getMclStatusManager()->getIntValue(RFX_STATUS_KEY_UNSOLICITED_RESPONSE_FILTER_CHANGED, 1) & RIL_UR_SIGNAL_STRENGTH)
                == RIL_UR_SIGNAL_STRENGTH) {
            response = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIGNAL_STRENGTH, m_slot_id,
                    RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)));
            logV(LOG_TAG, "[%s] send urc ", __FUNCTION__);
            responseToTelCore(response);
        }
    }

    if (enableReportSignalStrengthWithWcdmaEcio() && msg == NULL) {
        response = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIGNAL_STRENGTH_WITH_WCDMA_ECIO, m_slot_id,
                RfxSignalStrengthData(&resp, sizeof(RIL_SIGNAL_STRENGTH)));
        responseToTelCore(response);
    }
    return;

error:
    pthread_mutex_unlock(&s_signalStrengthMutex[m_slot_id]);
    logE(LOG_TAG, "[%s] must never return an error when radio is on", __FUNCTION__);
    if (msg != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxIntsData(), msg, false);
        responseToTelCore(response);
    }
}

bool RmmNwHandler::isGcfTestMode() {
    bool isTestMode;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    memset(prop,0,sizeof(prop));
    rfx_property_get("vendor.gsm.gcf.testmode", prop, "0");
    isTestMode = (atoi(prop) == 0) ? false : true;
    return isTestMode;
}

bool RmmNwHandler::isTestSim(int slotId) {
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get(PROPERTY_RIL_TEST_SIM[slotId], prop, "0");
    return (atoi(prop) == 0) ? false : true;
}

bool RmmNwHandler::isAPInCall() {
    bool ret = false;

    for (int slotId = RFX_SLOT_ID_0; slotId < RFX_SLOT_COUNT; slotId++) {
        if (getMclStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_AP_VOICE_CALL_COUNT, 0) > 0) {
            ret = true;
        }
    }
    return ret;
}

bool RmmNwHandler::isNetworkScanFullSupport() {
    return s_enwscn_support;
    //return false;
}

void RmmNwHandler::releaseNetworkScanResultData(RIL_NetworkScanResult* data) {
    if (data) {
        if (data->network_infos_length > 0 && data->network_infos) {
            int cellCount = data->network_infos_length;
            for (int i = 0; i < cellCount; i++) {
                RIL_CellInfo_v12 *info = &data->network_infos[i];

                switch(info->cellInfoType) {
                    case RIL_CELL_INFO_TYPE_GSM:
                        if (info->CellInfo.gsm.cellIdentityGsm.operName.long_name)
                            free(info->CellInfo.gsm.cellIdentityGsm.operName.long_name);
                        if (info->CellInfo.gsm.cellIdentityGsm.operName.short_name)
                            free(info->CellInfo.gsm.cellIdentityGsm.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_WCDMA:
                        if (info->CellInfo.wcdma.cellIdentityWcdma.operName.long_name)
                            free(info->CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                        if(info->CellInfo.wcdma.cellIdentityWcdma.operName.short_name)
                            free(info->CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_LTE:
                        if (info->CellInfo.lte.cellIdentityLte.operName.long_name)
                            free(info->CellInfo.lte.cellIdentityLte.operName.long_name);
                        if (info->CellInfo.lte.cellIdentityLte.operName.short_name)
                            free(info->CellInfo.lte.cellIdentityLte.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_NR:
                        if (info->CellInfo.nr.cellidentity.operName.long_name)
                            free(info->CellInfo.nr.cellidentity.operName.long_name);
                        if (info->CellInfo.nr.cellidentity.operName.short_name)
                            free(info->CellInfo.nr.cellidentity.operName.short_name);
                        break;
                    case RIL_CELL_INFO_TYPE_TD_SCDMA:
                    case RIL_CELL_INFO_TYPE_CDMA:
                    case RIL_CELL_INFO_TYPE_NONE:
                        break;
                }
            }
            free(data->network_infos);
            data->network_infos = NULL;
            data->network_infos_length = 0;
        }
    }
}

int RmmNwHandler::isFemtocellSupport() {
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

int RmmNwHandler::isEnableModulationReport()
{
    char optr[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.operator.optr", optr, "");

    return (strcmp("OP08", optr) == 0) ? 1 : 0;
}

bool RmmNwHandler::isMatchOpid(const char* opid) {
    char optr[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.operator.optr", optr, "0");

    if (strcmp(optr, opid) == 0) {
        return true;
    }
    return false;
}

int RmmNwHandler::fillBarringInfosResponse(size_t count,
        RIL_BarringInfo* barringInfos, RIL_BarringInforResponse* barringInfoResp) {
    if (isInService(sPsRegStateCache[m_slot_id]->registration_state)) {
        fillDataCellIdentity(barringInfoResp->cellIdentity);
    } else if (isInService(sCsRegStateCache[m_slot_id]->registration_state)) {
        fillVoiceCellIdentity(barringInfoResp->cellIdentity);
    } else {
        // mtkLogE(LOG_TAG, "fillBarringInfosResponse it is out of service");
        return -1;
    }
    if (count > MAX_BARRING_INFO_COUNT) count = MAX_BARRING_INFO_COUNT; // response structure barring info list count is MAX_BARRING_INFO_COUNT
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

void RmmNwHandler::loadDisplayRuleByOp() {
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

void RmmNwHandler::setNrDisplayRuleToMd() {
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ,
                                                          m_slot_id);
    uint8_t mode = ((nr_display_rule[m_slot_id] & NR_DISPLAY_JP_MODE) != 0) ?
        2 : nr_display_rule[m_slot_id];
    reqData->addMipcTlvUint8(MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_MODE, mode);
    if (mode == 2) {
        reqData->addMipcTlvUint8(MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_TIMER1,
                                 (uint8_t)jp_mode_timer1_delay_timer[m_slot_id]);
        reqData->addMipcTlvUint8(MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ_T_TIMER2,
                                 (uint8_t)jp_mode_timer2_delay_timer[m_slot_id]);
    }

    logD(LOG_TAG, "[%s] mode:%d, timer1:%d, timer2:%d", __FUNCTION__, mode,
         jp_mode_timer1_delay_timer[m_slot_id], jp_mode_timer2_delay_timer[m_slot_id]);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "[%s] fail, result:%d", __FUNCTION__, cnfData->getResult());
    }
}

void RmmNwHandler::setLastValidPlmn(const char* plmn) {
    if (plmn != NULL &&
            (strncmp(plmn, "000000", 6) != 0) &&
            (plmn[0] >= '0' && plmn[0] <= '9')) {
        last_valid_plmn_time[m_slot_id] = systemTime(SYSTEM_TIME_MONOTONIC);
        strncpy(last_valid_plmn[m_slot_id], plmn, 7);
        last_valid_plmn[m_slot_id][7] = '\0';
//         logD(LOG_TAG, "setLastValidPlmn: %ld, %s",
//                 (last_valid_plmn_time[m_slot_id]/1000000000), last_valid_plmn[m_slot_id]);
    }
}

bool RmmNwHandler::getLastValidPlmn(char* plmn, int size) {
    nsecs_t current_time = systemTime(SYSTEM_TIME_MONOTONIC);
//     logD(LOG_TAG, "getLastValidPlmn: %ld, %s",
//             (current_time/10000000000), last_valid_plmn[m_slot_id]);
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

bool RmmNwHandler::getCdmaPlmn(char* plmn, int size) {
    int err = 0;
    char* cdma_plmn = NULL;
    CDMA_CELL_LOCATION_INFO cdma_location;
    ViaBaseHandler *mViaHandler = RfxViaUtils::getViaHandler();
    logD(LOG_TAG, "%s +++++", __FUNCTION__);
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
                logD(LOG_TAG, "getCdmaPlmn: plmn=%s", plmn);
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

void RmmNwHandler::updateNrCaBandInfo() {
    uint8_t bandNum = 0;
    uint16_t len = 0;
    int data[3] = {0};
    sp<RfxMclMessage> urc;
    mipc_nr_ca_band_struct4* pBands = NULL;

    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_NR_CA_BAND_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "updateNrCaBandInfo: mipc request returns error");
        return;
    }

    bandNum = cnfData->getMipcUint8Val(MIPC_NW_GET_NR_CA_BAND_CNF_T_BAND_NUM, 0);
    if (bandNum <= 0 || bandNum > 2) {
        logE(LOG_TAG, "updateNrCaBandInfo: ignore invalid band num: %d", bandNum);
        return;
    }
    pBands = (mipc_nr_ca_band_struct4*)(cnfData->getMipcVal(MIPC_NW_GET_NR_CA_BAND_CNF_T_BAND, &len));
    if (pBands == NULL) return;
    data[0] = cnfData->getMipcInt32Val(MIPC_NW_GET_NR_CA_BAND_CNF_T_IS_ENDC, 0);
    data[1] = pBands->band[0];
    data[2] = bandNum == 2 ? pBands->band[1] : 0;
    logD(LOG_TAG, "updateNrCaBandInfo: is_endc[%d], band_1[%d], band_2[%d]",
        data[0], data[1], data[2]);
    urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_NR_CA_BAND_IND, m_slot_id, RfxIntsData(data, 3));
    responseToTelCore(urc);
}

void RmmNwHandler::queryPrimaryCellIdentityInfo(CellIdentityInfo* pOut) {

    if (pOut == NULL) {
        logE(LOG_TAG, "[%s]: pOut is NULL, return", __FUNCTION__);
        return;
    }

    sp<RfxMipcData> physicalChlCfgData = RfxMipcData::obtainMipcData(
        MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(physicalChlCfgData);
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "[%s]: mipc request returns error", __FUNCTION__);
        return;
    }
    uint16_t count = mipc_msg_get_tlv_num(cnfData->getData(),
        MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF_T_PHYSICAL_CHANNEL_CONFIGS_LIST_V1);
    logV(LOG_TAG, "[%s] count:%d ", __FUNCTION__, count);
    if (count > 0) {
        uint16_t len = 0;
        mipc_physical_channel_info_v1_struct4* pInfo = NULL;
        for (int i = 0; i < count; i++) {
            pInfo = mipc_nw_get_physical_channel_configs_cnf_get_physical_channel_configs_list_v1(
                    cnfData->getData(), &len, i);
            if (pInfo == NULL) {
                logE(LOG_TAG, "[%s] index :%d get null", __FUNCTION__, i);
                continue;
            }

            if (len != sizeof(mipc_physical_channel_info_v1_struct4)) {
                logE(LOG_TAG, "[%s] len :%d  Vs %d", __FUNCTION__, (int) len,
                        (int) sizeof(mipc_physical_channel_info_v1_struct4));
                continue;
            }
            if (((RIL_CellConnectionStatus) (pInfo->status)) == PRIMARY_SERVING) {
                pOut->bandwidth = pInfo->cell_bandwidth_downlink;
                pOut->arfcn = pInfo->downlink_channel_number;
                pOut->pci = (int) pInfo->physical_cell_id;
                pOut->band = (int) pInfo->band;
                char* pci_s = getMask(pOut->pci);
                logV(LOG_TAG, "[%s]: pci = %s, arfcn = %d, band %d, bandwidth = %d",
                        __FUNCTION__, pci_s, pOut->arfcn, pOut->band, pOut->bandwidth);
                if (pci_s) free(pci_s);
                break;
            }
        }
    }
}

void RmmNwHandler::queryPhysicalChannelConfigs() {
    if (!sPhysicalConfigSwitch[m_slot_id]) return;

    sp<RfxMipcData> physicalChlCfgData = RfxMipcData::obtainMipcData(
        MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(physicalChlCfgData);
    if (cnfData == NULL || cnfData->getResult() != MIPC_RESULT_SUCCESS) {
        logE(LOG_TAG, "queryPhysicalChannelConfigs: mipc request returns error");
        return;
    }

    bool hasChanged = false;
    RIL_PhysicalChannelConfig* pCache = sPhysicalConfigCache[m_slot_id];
    uint16_t count = mipc_msg_get_tlv_num(cnfData->getData(),
        MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF_T_PHYSICAL_CHANNEL_CONFIGS_LIST_V1);
    logV(LOG_TAG, "[%s] count:%u ", __FUNCTION__, count);
    if (count > 4) count = 4;

    pthread_mutex_lock(&sPhysicalConfigMutex[m_slot_id]);
    for (int i = count; i < sPhysicalConfigCount[m_slot_id]; i++) {
        memset(&pCache[i], 0, sizeof(RIL_PhysicalChannelConfig));
    }
    if (sPhysicalConfigCount[m_slot_id] != count) {
        hasChanged = true;
        sPhysicalConfigCount[m_slot_id] = count;
    }

    if (count > 0) {
        uint16_t len = 0;
        mipc_physical_channel_info_v1_struct4* pInfo = NULL;
        for (int i = 0; i < count; i++) {
            pInfo = (mipc_physical_channel_info_v1_struct4*)mipc_nw_get_physical_channel_configs_cnf_get_physical_channel_configs_list_v1(
                        cnfData->getData(), &len, i);
            if (pInfo == NULL) {
                logE(LOG_TAG, "[%s] index :%d get null", __FUNCTION__, i);
                continue;
            }

            if (len != sizeof(mipc_physical_channel_info_v1_struct4)) {
                logE(LOG_TAG, "[%s] len :%u is not match sizeof(mipc_physical_channel_info_v1_struct4): %lu", __FUNCTION__, len, sizeof(mipc_physical_channel_info_v1_struct4));
                continue;
            }

            if (!hasChanged) {
                int tempRat = (RIL_RadioTechnology)convertPSNetworkType(pInfo->rat);
                if (pCache[i].rat != tempRat) {
                    if (!((tempRat == RADIO_TECH_LTE_CA) && (pCache[i].rat == RADIO_TECH_LTE))) {
                        hasChanged = true;
                    }
                }
            }

            if (!hasChanged) {
                if (pCache[i].status != (RIL_CellConnectionStatus)pInfo->status
                    || pCache[i].cellBandwidthDownlink != pInfo->cell_bandwidth_downlink
                    || pCache[i].cellBandwidthUplink != pInfo->cell_bandwidth_uplink
                    || pCache[i].downlinkChannelNumber != pInfo->downlink_channel_number
                    || pCache[i].uplinkChannelNumber != pInfo->uplink_channel_number
                    || pCache[i].band != pInfo->band
                    || pCache[i].physicalCellId != pInfo->physical_cell_id
                    || pCache[i].fr != (FrequencyRange)pInfo->frequency_range) {
                    hasChanged = true;
                }
            }

            pCache[i].status = (RIL_CellConnectionStatus)pInfo->status;
            pCache[i].rat = (RIL_RadioTechnology)convertPSNetworkType(pInfo->rat);
            pCache[i].cellBandwidthDownlink = pInfo->cell_bandwidth_downlink;
            pCache[i].cellBandwidthUplink = pInfo->cell_bandwidth_uplink;
            pCache[i].downlinkChannelNumber = pInfo->downlink_channel_number;
            pCache[i].uplinkChannelNumber = pInfo->uplink_channel_number;
            pCache[i].band = pInfo->band;
            pCache[i].physicalCellId = pInfo->physical_cell_id;
            pCache[i].fr = (FrequencyRange)pInfo->frequency_range;

            if (pCache[i].rat == RADIO_TECH_LTE_CA)
                pCache[i].rat = RADIO_TECH_LTE;

            char* phyCid = getMask(pInfo->physical_cell_id);
            logV(LOG_TAG, "[%s] pCache[%d].rat:%d, pInfo[%d].rat:%u, pInfo->status:%u, pInfo->cell_bandwidth_downlink:%d,"
                        "pInfo->cell_bandwidth_uplink:%d, pInfo->downlink_channel_number:%d, "
                        "pInfo->uplink_channel_number:%d, pInfo->physical_cell_id:%s, pInfo->band:%u,pInfo->frequency_range:%u",
                        __FUNCTION__, i, pCache[i].rat, i, pInfo->rat, pInfo->status, pInfo->cell_bandwidth_downlink,
                        pInfo->cell_bandwidth_uplink, pInfo->downlink_channel_number, pInfo->uplink_channel_number,
                        phyCid, pInfo->band, pInfo->frequency_range);
            if (phyCid) free(phyCid);
        }
    }
    pthread_mutex_unlock(&sPhysicalConfigMutex[m_slot_id]);

    logD(LOG_TAG, "[%s] count:%u, hasChanged: %d ", __FUNCTION__, count, hasChanged);
    if (hasChanged) {
        sendEvent(RFX_MSG_EVENT_RERESH_PHYSICAL_CONFIG, RfxVoidData(),
                RIL_CMD_PROXY_3, m_slot_id);
    }
}

#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
bool RmmNwHandler::skipDCNRRestrictedState(const char* mccmnc) {
    bool ret = false;
    // Compare with mccmnc to skip dcnr restricted state
    for (size_t i = 0; i < NUM_ELEMS(SKIP_DCNR_RESTRICTED_STATE_MCCMNC); i++) {
        if (RfxMisc::strStartsWith(mccmnc, SKIP_DCNR_RESTRICTED_STATE_MCCMNC[i])) {
            ret = true;
            break;
        }
    }
    return ret;
}

bool RmmNwHandler::fillPsCache(RIL_PS_REG_STATE_CACHE*& pPsCache, int& psRegState) {
    sp<RfxMipcData> reqData = RfxMipcData::obtainMipcData(MIPC_NW_GET_PS_REQ, m_slot_id);
    sp<RfxMipcData> cnfData = callToMipcMsgSync(reqData);
    if (cnfData->getResult() == MIPC_RESULT_SUCCESS) {
        uint16_t rspDataLen = 0;
        mipc_nw_ps_reg_info_struct4* pInfo = (mipc_nw_ps_reg_info_struct4*)(cnfData->getMipcVal(
            MIPC_NW_GET_PS_CNF_T_REG_INFO, &rspDataLen));
        if (!pInfo || rspDataLen != sizeof(mipc_nw_ps_reg_info_struct4)) {
            logE(LOG_TAG, "[%s] pInfo:%p, rspDataLen:%d, struct len:%d", __FUNCTION__, pInfo,
                 (int)rspDataLen, (int)sizeof(mipc_nw_ps_reg_info_struct4));
            // goto error;
            return -1;
        }
        pthread_mutex_lock(&sPsRegStateMutex[m_slot_id]);
        pPsCache->registration_state = pInfo->stat;
        psRegState = convertRegState(pPsCache->registration_state, false);
        pPsCache->lac = (pInfo->lac_tac <= 0x7FFFFFFF) ? pInfo->lac_tac : 0x7FFFFFFF;
        pPsCache->cid = pInfo->cell_id;
        pPsCache->radio_technology =
            isInService(convertRegState(pInfo->stat, false)) ? pInfo->rat : 0;
        pPsCache->denied_reason = pInfo->reason_for_denial;
        pPsCache->max_simultaneous_data_call = pInfo->max_data_calls;
        if (skipDCNRRestrictedState((const char*)sOpInfoCache[m_slot_id]->mccMnc)) {
            // logD(LOG_TAG, "skip DCNR Restricted State for %s",
            // sOpInfoCache[m_slot_id]->mccMnc);
            pPsCache->dcnr_restricted = 0;
        } else {
            pPsCache->dcnr_restricted = pInfo->is_dcnr_restricted;
        }
        pPsCache->endc_sib = pInfo->cache_endc_connect;  // pInfo->is_nr_available is original value
        pPsCache->endc_available = pInfo->is_endc_available;
        // pPsCache->reject_cause = pInfo->;
        pthread_mutex_unlock(&sPsRegStateMutex[m_slot_id]);
        dumpPsRegStateCache(__FUNCTION__, pPsCache, true);
        if (pInfo->cache_endc_connect != pInfo->is_nr_available) {
            logV(LOG_TAG, "[%s] cache_endc:%d, endc_sib:%d", __FUNCTION__,
                 pInfo->cache_endc_connect, pInfo->is_nr_available);
        }
    } else {
        logE(LOG_TAG, "[%s] cnfData result:%d", __FUNCTION__, cnfData->getResult());
    }
    return 0;
}

bool RmmNwHandler::fillCsCache(sp<RfxMipcData>& cnfData,
                               bool& rat_4g,
                               bool& rat_5g,
                               bool& rat_c2k,
                               int& csRegState,
                               int& psRegState) {
    uint16_t rspDataLen = 0;
    mipc_nw_cs_reg_info_struct4* pInfo = (mipc_nw_cs_reg_info_struct4*)(cnfData->getMipcVal(
        MIPC_NW_GET_CS_CNF_T_REG_INFO, &rspDataLen));
    if (!pInfo || rspDataLen != sizeof(mipc_nw_cs_reg_info_struct4)) {
        logE(LOG_TAG, "[%s] pInfo:%p, rspDataLen:%d, struct len:%d", __FUNCTION__, pInfo,
             (int)rspDataLen, (int)sizeof(mipc_nw_cs_reg_info_struct4));
        // goto error;
        return -1;
    }

    RIL_CS_REG_STATE_CACHE* pCsCache = sCsRegStateCache[m_slot_id];
    RIL_PS_REG_STATE_CACHE* pPsCache = sPsRegStateCache[m_slot_id];
    pthread_mutex_lock(&sCsRegStateMutex[m_slot_id]);
    pCsCache->registration_state = pInfo->stat;
    pCsCache->radio_technology = pInfo->rat;
    pCsCache->roaming_indicator = pInfo->roaming_ind;
    pCsCache->css = pInfo->css;
    pCsCache->is_in_prl = pInfo->is_in_prl;
    pCsCache->default_roaming_indicator = pInfo->def_roaming_ind;
    pCsCache->denied_reason = pInfo->reason_for_denial;
    pthread_mutex_unlock(&sCsRegStateMutex[m_slot_id]);
    dumpCsRegStateCache(__FUNCTION__, pCsCache, true);
    csRegState = convertRegState(pCsCache->registration_state, true);
    psRegState = convertRegState(pPsCache->registration_state, false);
    if (!isInService(csRegState)) {
        int cs_type = convertCSNetworkType(pCsCache->radio_technology);
        if (cs_type == RADIO_TECH_LTE) {
            rat_4g = true;
        } else if (cs_type == RADIO_TECH_NR) {
            rat_5g = true;
        } else if (cs_type == RADIO_TECH_1xRTT || cs_type == RADIO_TECH_EVDO_A ||
                   cs_type == RADIO_TECH_EHRPD) {
            rat_c2k = true;
        }
        pCsCache->radio_technology = 0;
    }

    if (isSpecificVoiceStateConfigured() && !isInService(csRegState)) {
        int volteReg = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_IMS_REGISTRATION_STATE, 0);
        if ((volteReg & 0x01) == 0x01) {  // IMS registered and VOPS
            if (isInService(psRegState)) {
                pCsCache->denied_reason = 0;
            }
            pCsCache->registration_state = pPsCache->registration_state;
            pCsCache->radio_technology = pPsCache->radio_technology;
            pCsCache->lac = pPsCache->lac;
            pCsCache->cid = pPsCache->cid;
        }
    }
    return 0;
}
#endif //MTK_HYP_DUAL_TELE_SUPPORT