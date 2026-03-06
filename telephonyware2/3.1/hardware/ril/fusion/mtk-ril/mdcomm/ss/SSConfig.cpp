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

#include "SSConfig.h"
#include "RfxLog.h"

#include <string.h>

#ifdef RFX_LOG_TAG
#undef RFX_LOG_TAG
#endif
#define RFX_LOG_TAG "SS-Config"

const char* SSConfig::NOT_SHOW_FORWARDED_TOAST[] = {
    /* Verizon */
    "310004", "310005", "310006", "310010", "310012", "310013",
    "310350", "310590", "310820", "310890", "310910", "311012",
    "311110", "311270", "311271", "311272", "311273", "311274",
    "311275", "311276", "311277", "311278", "311279", "311280",
    "311281", "311282", "311283", "311284", "311285", "311286",
    "311287", "311288", "311289", "311390", "311480", "311481",
    "311482", "311483", "311484", "311485", "311486", "311487",
    "311488", "311489", "311590", "312770"
};

const char* SSConfig::JAPAN_OP_REPLACE_CF_NUMBER[] = {
    /* KDDI */
    "44170", "44052", "44076", "44071", "44053", "44077", "44008",
    "44072", "44054", "44079", "44007", "44073", "44055", "44088",
    "44051", "44075", "44056", "44085", "44083", "44081", "44080",
    "44086", "44084", "44082",
    /* SoftBank */
    "44163", "44047", "44095", "44041", "44164", "44046", "44097",
    "44042", "44090", "44165", "44092", "44098", "44043", "44093",
    "44048", "44006", "44161", "44044", "44094", "44004", "44045",
    "44020", "44096", "44040"
};

const char* SSConfig::USSI_WITH_NO_LANG[] = {
    /* Smart Cambodia */
    "45606"
};

const char* SSConfig::CONVERT_409_TO_NOT_SUPPROT_CALLER_ID[] = {
    /* CMCC */
    "46000", "46002", "46004", "46007", "46008",
    /* CU */
    "46001", "46006", "46009", "45407"
};

const char* SSConfig::CONVERT_409_TO_NOT_SUPPROT_CB[] = {
    /* CMCC */
    "46000", "46002", "46004", "46007", "46008",
    /* CT */
    "45502", "46003", "46011", "46012", "45507"
};

const char* SSConfig::NOT_SUPPORT_USSI_OVER_CALL[] = {
    /* Mobitel */
    "41301",
    /* Telemig / Brazil */
    "72410"
};

// ALPS07640779
const char* SSConfig::NOT_SUPPORT_START_USSI_OVER_CALL[] = {
    /* Indonesia - XL */
    "51011",
    /* Indonesia - Telkomsel */
    "51010"
};

const char* SSConfig::NOT_SUPPORT_STK_USSI[] = {
    /* Two Degrees / New Zealand */
    "53024",
    /* Spice / India */
    "40444",
    /* GrameenPhone / Bangladesh */
    "47001"
};

const char* SSConfig::USSD_DOMAIN_CS_ONLY[] = {
    /* Megafon / Russian */
    /// "25002" //ALPS07750338
};

const char* SSConfig::NOT_RETRY_USSD_OVER_CS[] = {
    /* TMO-US */
    "310160", "310260", "310490", "310580", "310660",
    "310200", "310210", "310220", "310230", "310240",
    "310250", "310270", "310310", "310800"
};


bool SSConfig::isInList(const char* mccmnc, char const *list[], unsigned long size) {
    if (mccmnc == NULL || size == 0) {
        return false;
    }

    for (unsigned long i = 0; i < size; i++) {
        if (strcmp(mccmnc, list[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Do not poppup "It's a forwarded call" toast even when ECMCCSS 257 received
 */
bool SSConfig::notShowForwardedToast(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_SHOW_FORWARDED_TOAST,
            sizeof(NOT_SHOW_FORWARDED_TOAST) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "notShowForwardedToast, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Send USSI without language info
 */
bool SSConfig::ussiWithNoLang(const char* mccmnc) {
    bool r = isInList(mccmnc, USSI_WITH_NO_LANG, sizeof(USSI_WITH_NO_LANG) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "ussiWithNoLang, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * For some operator which not support Caller ID, need to convert the received "409 conflict error"
 * to "request not support"
 */
bool SSConfig::convert409ToNotSupportCallerID(const char* mccmnc) {
    bool r = isInList(mccmnc, CONVERT_409_TO_NOT_SUPPROT_CALLER_ID,
            sizeof(CONVERT_409_TO_NOT_SUPPROT_CALLER_ID) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "convert409ToNotSupportCallerID, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * For some operator which not support call barring, need to convert the received "409 conflict
 * error" to "request not support"
 */
bool SSConfig::convert409ToNotSupportCB(const char* mccmnc) {
    bool r = isInList(mccmnc, CONVERT_409_TO_NOT_SUPPROT_CB,
            sizeof(CONVERT_409_TO_NOT_SUPPROT_CB) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "convert409ToNotSupportCB, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Operator doesn't support send USSI during call
 */
bool SSConfig::isNotSupportUSSIOverCall(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_SUPPORT_USSI_OVER_CALL, sizeof(NOT_SUPPORT_USSI_OVER_CALL) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "isNotSupportUSSIOverCall, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Operator doesn't support start USSI during call, but can start USSI before call
 */
bool SSConfig::isNotSupportStartUSSIDuringCall(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_SUPPORT_START_USSI_OVER_CALL,
            sizeof(NOT_SUPPORT_START_USSI_OVER_CALL) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "isNotSupportStartUSSIDuringCall, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Not support STK USSI, need to do STK USSD over CSFB directly
 */
bool SSConfig::notSupportStkUssi(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_SUPPORT_STK_USSI, sizeof(NOT_SUPPORT_STK_USSI) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "notSupportStkUssi, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Japan operator requirement to replace number before sending to network.
 */
bool SSConfig::needReplaceCFNumberForJPN(const char* mccmnc) {
    bool r = isInList(mccmnc, JAPAN_OP_REPLACE_CF_NUMBER,
            sizeof(JAPAN_OP_REPLACE_CF_NUMBER) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "JAPAN_OP_REPLACE_CF_NUMBER, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Ussd is only for CS domain.
 */
bool SSConfig::isUssdDomainCsOnly(const char* mccmnc) {
    bool r = isInList(mccmnc, USSD_DOMAIN_CS_ONLY,
            sizeof(USSD_DOMAIN_CS_ONLY) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "USSD_DOMAIN_CS_ONLY, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}

/**
 * Ussi failed, but do NOT try CS domain.
 */
bool SSConfig::notRetryUssdOverCs(const char* mccmnc) {
    bool r = isInList(mccmnc, NOT_RETRY_USSD_OVER_CS, sizeof(NOT_RETRY_USSD_OVER_CS) / sizeof(char*));
    RFX_LOG_D(RFX_LOG_TAG, "notTryUssdOverCS, mccmnc = %s, r = %s",
            mccmnc, r ? "True" : "False");
    return r;
}
