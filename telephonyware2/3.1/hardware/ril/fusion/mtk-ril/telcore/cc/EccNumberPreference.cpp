/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2020. All rights reserved.
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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "EccNumberPreference.h"
#include <libmtkrilutils.h>
#include "RfxLog.h"
#include "rfx_properties.h"

#define RFX_LOG_TAG "EccNumberPreference"
#define MCC_CHAR_LEN         3
#define PLMN_CHAR_LEN        6

/*****************************************************************************
 * Class EccNumberSource
 *****************************************************************************/
EccNumberPreference::EccNumberPreference(int slotId) : mGsmOnlyNumberList(""),
        mGsmPreferredNumberList(""), mCdmaPreferredNumberList("") {
    mSlotId = slotId;
}

EccNumberPreference::~EccNumberPreference() {
}

void EccNumberPreference::update(String8 plmn) {
    TiXmlDocument eccXml;
    if (!eccXml.LoadFile(String8(ECC_PREFERENCE_FILE_PATH).string(), TIXML_ENCODING_UTF8)) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] Load ecc preference xml error", mSlotId, __FUNCTION__);
        return;
    }

    TiXmlElement* eccPref = eccXml.FirstChildElement(ECC_PREFERENCE_TABLE);
    if (eccPref == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] eccPref element is NULL", mSlotId, __FUNCTION__);
        return;
    }

    // reset
    mGsmOnlyNumberList = "";
    mGsmPreferredNumberList = "";
    mCdmaPreferredNumberList = "";

    parseTag(eccPref, String8(ECC_PREFERENCE_GSM_ONLY), mGsmOnlyNumberList, plmn);
    parseTag(eccPref, String8(ECC_PREFERENCE_GSM_PREF), mGsmPreferredNumberList, plmn);
    parseTag(eccPref, String8(ECC_PREFERENCE_CDMA_PREF), mCdmaPreferredNumberList, plmn);

    RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] for PLMN: %s GSM only: %s, GSM prefer: %s, CDMA prefer: %s",
            mSlotId, __FUNCTION__, plmn.string(), mGsmOnlyNumberList.string(),
            mGsmPreferredNumberList.string(), mCdmaPreferredNumberList.string());
}

bool EccNumberPreference::isGsmOnlyNumber(String8 number) {
    if (number.isEmpty() || mGsmOnlyNumberList.length() <= 0) {
        return false;
    }
    String8 matchList = String8(",") + mGsmOnlyNumberList + String8(",");
    if (matchList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }
    return false;
}

bool EccNumberPreference::isGsmPreferredNumber(String8 number) {
    if (number.isEmpty() || mGsmPreferredNumberList.length() <= 0) {
        return false;
    }
    String8 matchList = String8(",") + mGsmPreferredNumberList + String8(",");
    if (matchList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }
    return false;
}

bool EccNumberPreference::isCdmaPreferredNumber(String8 number) {
    if (number.isEmpty() || mCdmaPreferredNumberList.length() <= 0) {
        return false;
    }
    String8 matchList = String8(",") + mCdmaPreferredNumberList + String8(",");
    if (matchList.find(String8(",") + number + String8(",")) != -1) {
        return true;
    }
    return false;
}

void EccNumberPreference::parseTag(TiXmlElement* eccPref, String8 preferenceType,
        String8& preferenceList, String8 plmn) {
    char optr[MAX_PROP_CHARS] = {0};
    rfx_property_get(ECC_PREFERENCE_OPTR, optr, "OM");
    char currentMcc[MCC_CHAR_LEN + 1] = {0};
    strncpy(currentMcc, plmn.string(), MCC_CHAR_LEN);
    currentMcc[MCC_CHAR_LEN] = '\0';

    //RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] optr: %s, mcc: %s", mSlotId, __FUNCTION__, optr, currentMcc);

    TiXmlElement* elem = eccPref->FirstChildElement(preferenceType.string());
    while (elem) {
        TiXmlAttribute* attr = elem->FirstAttribute();
        if (attr == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[%d][%s] attr is NULL", mSlotId, __FUNCTION__);
            continue;
        }

        String8 attrName = String8("");
        String8 attrValue = String8("");
        while (attr) {
            attrName = attr->Name();
            attrValue = attr->Value();
            //RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] attrName: %s, attrValue: %s", mSlotId, __FUNCTION__,
            //        attrName.string(), attrValue.string());
            if (attrName == ECC_PREFERENCE_OPERATOR) {
                if (attrValue == optr) {
                    // Match OPTR preference
                } else if (attrValue == currentMcc && preferenceList.length() == 0) {
                    // Match MCC preference and preferenceList not set by OPTR preference
                    // If already got OPTR preference, skip for MCC preference
                    // Ex: optr(OP01) preference will have higher priority then MCC(460)
                    // <GsmOnly Operator="460" EccList="112,000,08,118" />
                    // <GsmOnly Operator="OP01" EccList="112" />
                } else {
                    // Skip in all other case
                    break;
                }
            } else if (attrName == ECC_PREFERENCE_ECC_LIST) {
                preferenceList = attrValue;
            }
            attr = attr->Next();
        }

        elem = elem->NextSiblingElement(preferenceType.string());
    }

    //RFX_LOG_D(RFX_LOG_TAG, "[%d][%s] type: %s, preferenceList: %s", mSlotId, __FUNCTION__,
    //        preferenceType.string(), preferenceList.string());
}
