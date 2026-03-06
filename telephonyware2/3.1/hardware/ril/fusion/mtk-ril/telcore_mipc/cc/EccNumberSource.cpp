/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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
#include "EccNumberSource.h"
#include <libmtkrilutils.h>
#include "RfxLog.h"
#include "rfx_properties.h"

#define RFX_LOG_TAG "EccNumberSource"

#define MAX_PROP_CHARS       92
#define MCC_CHAR_LEN         3
#define PLMN_CHAR_LEN        6
#define PLMN_CHAR_MIN_LEN    5
#define MAX_ECC_NUM          16
#define MAX_ECC_BUF_SIZE     (MAX_ECC_NUM * 8 + 1)

typedef struct {
    const char* name;    // Region name
    const char* mccList; // Mcc list in this region separate by ',', Ex: "440,460"
} Region;

// Customize region here
static Region sRegionTable[] = {
    {"APAC", "505"}, // Australia
    {"LATAM", "724"},// Brazilian
    {"EMEA", "234"}  // UK
};

/*****************************************************************************
 * Class EccNumberSource
 *****************************************************************************/
EccNumberSource::EccNumberSource(int slotId) {
    mSlotId = slotId;
    mEccList.clear();
}

EccNumberSource::~EccNumberSource() {
}

bool EccNumberSource::addEmergencyNumber(EmergencyNumber ecc, String8 plmn, bool isSimInsert) {
    if (ECC_DEBUG == 1) {
        RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] current PLMN: %s, eccNumber:[%s,%s,%s,%d,%d]",
                mSlotId, __FUNCTION__, (char *)plmn.string(),
                (char *)ecc.number.string(),
                (char *)ecc.mcc.string(),
                (char *)ecc.mnc.string(),
                ecc.categories,
                ecc.condition);
    }

    // PLMN match for:
    // 1. Specific PLMN ECC
    // 2. MCC ECC for one country
    // 3. Common ECC (No PLMN info in xml)
    // And condition matches
    char currentMcc[MCC_CHAR_LEN + 1] = {0};
    strncpy(currentMcc, plmn.string(), MCC_CHAR_LEN);
    currentMcc[MCC_CHAR_LEN] = '\0';
    if (((plmn == (ecc.mcc + ecc.mnc)) ||
            ((ecc.mnc == "FF" || ecc.mnc == "FFF") &&
            ecc.mcc == currentMcc) ||
            (ecc.mcc == "" && ecc.mnc == "")) &&
            ((isSimInsert && ecc.condition > 0) ||
            (!isSimInsert && ecc.condition != CONDITION_SIM_ONLY))) {
        if (ECC_DEBUG == 1) {
            RFX_LOG_V(RFX_LOG_TAG, "[%d][%s] eccNumber match, add: %s", mSlotId, __FUNCTION__,
                ecc.number.string());
        }
        mEccList.push_back(ecc);
        return true;
    }
    return false;
}

void EccNumberSource::addToEccList(Vector<EmergencyNumber> &list) {
    for (int i = 0; i < (int)mEccList.size(); i++) {
        if (!findAndMerge(mEccList[i], list)) {
            list.push_back(mEccList[i]);
        }
    }
}

bool EccNumberSource::findAndMerge(EmergencyNumber number, Vector<EmergencyNumber> &list) {
    for (Vector<EmergencyNumber>::iterator iter = list.begin(); iter != list.end();) {
        if (iter == NULL) {
            return false;
        }

        if (isSimilar(number, *iter)) {
            if (getPriority(number) > getPriority(*iter)) {
                (*iter).categories = number.categories;
                (*iter).condition = number.condition;
                (*iter).mcc = number.mcc;
                (*iter).mnc = number.mnc;
            } else if (getPriority(number) == getPriority(*iter)) {
                // Operator specific ECC has higher priperity then country ECC
                if ((*iter).mcc == number.mcc &&
                        number.mnc != "" && number.mnc != "FFF" && number.mnc != "FF") {
                    (*iter).categories = number.categories;
                    (*iter).condition = number.condition;
                    (*iter).mnc = number.mnc;
                }
            } else {
                // Over CDMA SIM ECC service category by XML configure
                if (((number.sources & SOURCE_CONFIG)
                        && (number.categories != ECC_CATEGORY_NOT_DEFINED))
                        && (((*iter).sources & SOURCE_SIM)
                        && ((*iter).categories == ECC_CATEGORY_NOT_DEFINED))) {
                    (*iter).categories = number.categories;
                }
            }

            // merge sources
            (*iter).sources |= number.sources;
            return true;
        }
        ++iter;
    }
    return false;
}

bool EccNumberSource::isSimilar(EmergencyNumber number1, EmergencyNumber number2) {
    // Currently only check if number is equal, may also consider other parameters
    if (number1.number == number2.number) {
        return true;
    } else {
        return false;
    }
}

int EccNumberSource::getPriority(EmergencyNumber number) {
    int priority = 0;
    if (number.sources & SOURCE_NETWORK) {
        priority += 1 << 4;
    }
    if (number.sources & SOURCE_SIM) {
        priority += 1 << 3;
    }
    if (number.sources & SOURCE_CONFIG ||
            number.sources & SOURCE_OEM_PROPERTY ||
            number.sources & SOURCE_FRAMEWORK ||
            number.sources & SOURCE_TEST) {
        priority += 1 << 2;
    }
    // MCF priority should low than AP config in case
    // customer want to use AP xml to overide MCF
    if (number.sources & SOURCE_MCF) {
        priority += 1 << 1;
    }
    if (number.sources & SOURCE_DEFAULT) {
        priority += 1 << 0;
    }
    return priority;
}

/*****************************************************************************
 * Class XmlEccNumberSource
 *****************************************************************************/
XmlEccNumberSource::XmlEccNumberSource(int slotId) : EccNumberSource(slotId) {
    RFX_LOG_D(RFX_LOG_TAG, "[XML][%d][%s] START", mSlotId, __FUNCTION__);
    String8 filePath = getConfigFilePath();
    mEccXml = new TiXmlDocument();
    if (mEccXml != NULL && !mEccXml->LoadFile(filePath.string(), TIXML_ENCODING_UTF8)) {
        RFX_LOG_E(RFX_LOG_TAG, "[XML][%d][%s] Load ecc xml error", mSlotId, __FUNCTION__);
        delete(mEccXml);
        mEccXml = NULL;
    }
    RFX_LOG_D(RFX_LOG_TAG, "[XML][%d][%s] END", mSlotId, __FUNCTION__);
}

XmlEccNumberSource::~XmlEccNumberSource() {
    if (mEccXml != NULL) {
        delete(mEccXml);
        mEccXml = NULL;
    }
}

bool XmlEccNumberSource::update(String8 plmn, bool isSimInsert) {
    RFX_LOG_D(RFX_LOG_TAG, "[XML][%d][%s] START for plmn: %s, isSimInsert: %d", mSlotId,
            __FUNCTION__, plmn.string(), isSimInsert);

    // Skip update for invalid PLMN case
    if (String8("FFFFFF") == plmn || String8("000000") == plmn || mEccXml == NULL) {
        return false;
    }

    mEccList.clear();

    TiXmlElement* eccs = mEccXml->FirstChildElement(ECC_TABLE);
    if (eccs == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[XML][%d][%s] eccs element is NULL", mSlotId, __FUNCTION__);
        return false;
    }

    TiXmlElement* elem = eccs->FirstChildElement(ECC_ENTRY);
    while (elem) {
        TiXmlAttribute* attr = elem->FirstAttribute();
        if (attr == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "[XML][%d][%s] attr is NULL", mSlotId, __FUNCTION__);
            continue;
        }

        String8 attrName = String8("");
        String8 attrValue = String8("");
        EmergencyNumber eccNumber = {String8(""), String8(""), String8(""), 0, 0, SOURCE_CONFIG};
        while (attr) {
            attrName = attr->Name();
            attrValue = attr->Value();
            if (attrName == ECC_NUMBER) {
                eccNumber.number = attrValue;
            } else if (attrName == ECC_CATEGORY) {
                eccNumber.categories = atoi(attrValue.string());
            } else if (attrName == ECC_CONDITION) {
                int condition = atoi(attrValue.string());
                if (!isSimInsert && condition == CONDITION_MMI) {
                    // Convert to ECC when no SIM
                    eccNumber.condition = CONDITION_NO_SIM;
                } else {
                    eccNumber.condition = condition;
                }
            } else if (attrName == ECC_PLMN) {
                // Support customize ECC by region
                String8 mccList = getMccList(attrValue);
                if (mccList.length() > 0) {
                    if (plmn.length() > MCC_CHAR_LEN) {
                        // Found mcc in region, replace with current PLMN
                        char currentMcc[MCC_CHAR_LEN + 1] = {0};
                        char currentMnc[MCC_CHAR_LEN + 1] = {0};
                        strncpy(currentMcc, plmn.string(), MCC_CHAR_LEN);
                        currentMcc[MCC_CHAR_LEN] = '\0';
                        strncpy(currentMnc, plmn.string() + MCC_CHAR_LEN, MCC_CHAR_LEN);
                        currentMnc[MCC_CHAR_LEN] = '\0';
                        if (mccList.find(currentMcc) != -1) {
                            eccNumber.mcc = currentMcc;
                            eccNumber.mnc = currentMnc;
                        } else {
                            // Not found, Keep as region name
                            eccNumber.mcc = attrValue;
                        }
                    }  else {
                        // Keep as region name
                        eccNumber.mcc = attrValue;
                    }
                } else {
                    if (attrValue.length() >= PLMN_CHAR_LEN) {
                        char attrMcc[MCC_CHAR_LEN + 1] = {0};
                        char attrMnc[MCC_CHAR_LEN + 1] = {0};
                        strncpy(attrMcc, attrValue.string(), MCC_CHAR_LEN);
                        attrMcc[MCC_CHAR_LEN] = '\0';
                        // +1 for skip blank in xml (Ex: 460 11)
                        strncpy(attrMnc, attrValue.string() + MCC_CHAR_LEN + 1, MCC_CHAR_LEN);
                        attrMnc[MCC_CHAR_LEN] = '\0';
                        eccNumber.mcc = attrMcc;
                        eccNumber.mnc = attrMnc;
                    }
                }
            }
            attr = attr->Next();
        }

        addEmergencyNumber(eccNumber, plmn, isSimInsert);

        elem = elem->NextSiblingElement(ECC_ENTRY);
    }

    if (ECC_DEBUG == 1) {
        RFX_LOG_V(RFX_LOG_TAG, "[XML][%d][%s] END size: %d", mSlotId, __FUNCTION__,
                (int)mEccList.size());
    }
    return true;
}

String8 XmlEccNumberSource::getConfigFilePath() {
    char optr[RFX_PROPERTY_VALUE_MAX] = {0};
    String8 filePath = String8(ECC_OM_FILE_PATH);
    rfx_property_get(ECC_OPERATOR_PROP, optr, "");
    if (strlen(optr) > 0) {
        filePath = String8(ECC_PATH) + String8("ecc_list_") + String8(optr) + String8(".xml");
    }

    // Check if exist
    FILE* file = fopen(filePath.string(), "r");
    if (file == NULL) {
        // Use default xml
        filePath = String8(ECC_OM_FILE_PATH);
    } else {
        if (fclose(file) != 0) {
            RFX_LOG_E(RFX_LOG_TAG, "[XML][%d][%s] close file error", mSlotId, __FUNCTION__);
        }
    }

    if (ECC_DEBUG == 1) {
        RFX_LOG_V(RFX_LOG_TAG, "[XML][%d][%s] optr: %s, ECC filePath: %s", mSlotId, __FUNCTION__,
                optr, (char *)filePath.string());
    }
    return filePath;
}

String8 XmlEccNumberSource::getMccList(String8 region) {
    int tableSize = sizeof(sRegionTable) / sizeof(Region);
    for (int i = 0; i < tableSize; i++) {
        if (region == String8(sRegionTable[i].name)) {
            if (ECC_DEBUG == 1) {
                RFX_LOG_D(RFX_LOG_TAG, "[XML][%d][%s] found mccList: %s for %s",
                        mSlotId, __FUNCTION__, sRegionTable[i].mccList,
                        sRegionTable[i].name);
            }
            return String8(sRegionTable[i].mccList);
        }
    }
    return String8("");
}

/*****************************************************************************
 * Class FrameworkEccNumberSource
 *****************************************************************************/
FrameworkEccNumberSource::FrameworkEccNumberSource(int slotId) : EccNumberSource(slotId) {
    mEccListWithCard = String8("");
    mEccListNoCard = String8("");
}

FrameworkEccNumberSource::~FrameworkEccNumberSource() {
}

void FrameworkEccNumberSource::set(const char *eccWithSim, const char *eccNoSim) {
    if (eccWithSim != NULL) {
        mEccListWithCard = String8(eccWithSim);
    }
    if (eccNoSim != NULL) {
        mEccListNoCard = String8(eccNoSim);
    }
}

bool FrameworkEccNumberSource::update(bool isSimInsert) {
    mEccList.clear();

    String8 eccListString = isSimInsert ? mEccListWithCard : mEccListNoCard;
    char* eccList = (char *)strdup(eccListString.string());
    char* eccCategory = NULL;
    char* ecc = NULL;
    char* savePtr = NULL;

    if (eccList != NULL) {
        if (strlen(eccList) > 0) {
            ecc = strtok_r(eccList, ",", &savePtr);
            while (ecc != NULL) {
                EmergencyNumber en;
                en.number = ecc;
                en.mcc = "";
                en.mnc = "";
                en.categories = ECC_CATEGORY_NOT_DEFINED;
                en.sources = SOURCE_FRAMEWORK;
                en.condition = CONDITION_ALWAYS;

                // Support setting emergency number with category,
                // the format is <service category>+<number>, e.g. 1+110
                char *plusChar = strchr(ecc, '+');
                if (plusChar != NULL) {
                    eccCategory = ecc;
                    *plusChar = '\0';
                    ecc = plusChar + 1;
                    en.categories = atoi(eccCategory);
                    if (ecc != NULL) {
                        en.number = ecc;
                    }
                }
                mEccList.push_back(en);
                ecc = strtok_r(NULL, ",", &savePtr);
            }
        }
        free(eccList);
    }

    return true;
}

/*****************************************************************************
 * Class OemPropertyEccNumberSource
 *****************************************************************************/
OemPropertyEccNumberSource::OemPropertyEccNumberSource(int slotId) : EccNumberSource(slotId) {
}

OemPropertyEccNumberSource::~OemPropertyEccNumberSource() {
}

bool OemPropertyEccNumberSource::update(String8 currentPlmn, bool isSimInsert) {
    mEccList.clear();

    RFX_LOG_I(RFX_LOG_TAG, "[Property][%d][%s] START for plmn: %s, isSimInsert: %d", mSlotId,
            __FUNCTION__, (char *)currentPlmn.string(), isSimInsert);

    char eccCount[MAX_PROP_CHARS] = {0};
    char eccNumber[MAX_PROP_CHARS] = {0};
    char eccType[MAX_PROP_CHARS] = {0};
    char eccCondition[MAX_PROP_CHARS] = {0};
    char eccPlmn[MAX_PROP_CHARS] = {0};
    char propertyNumber[MAX_PROP_CHARS] = {0};
    char propertyType[MAX_PROP_CHARS] = {0};
    char propertyCondition[MAX_PROP_CHARS] = {0};
    char propertyPlmn[MAX_PROP_CHARS] = {0};
    int ret = 0;

    rfx_property_get(PROPERTY_ECC_COUNT, eccCount, "0");
    for (int i = 0; i < atoi(eccCount); i++) {
        // Get ECC number from property
        ret = snprintf(propertyNumber, MAX_PROP_CHARS, "%s%d", PROPERTY_ECC_NUMBER, i);
        if (ret < 0) {
            return false;
        }
        rfx_property_get(propertyNumber, eccNumber, "");
        if (strlen(eccNumber) > 0) {
            EmergencyNumber en;
            en.number = eccNumber;
            en.mcc = "";
            en.mnc = "";
            en.categories = ECC_CATEGORY_NOT_DEFINED;
            en.sources = SOURCE_OEM_PROPERTY;
            en.condition = CONDITION_ALWAYS;

            // Get ECC type from property
            ret = snprintf(propertyType, MAX_PROP_CHARS, "%s%d", PROPERTY_ECC_TYPE, i);
            if (ret < 0) {
                return false;
            }
            rfx_property_get(propertyType, eccType, "");
            if (strlen(eccType) > 0) {
                String8 type = String8(eccType);
                int category = 0;
                if (type.find(PROPERTY_ECC_TYPE_KEY_POLICE) != -1) {
                    category |= POLICE;
                }
                if (type.find(PROPERTY_ECC_TYPE_KEY_AMBULANCE) != -1) {
                    category |= AMBULANCE;
                }
                if (type.find(PROPERTY_ECC_TYPE_KEY_FIRE_BRIGADE) != -1) {
                    category |= FIRE_BRIGADE;
                }
                if (type.find(PROPERTY_ECC_TYPE_KEY_MARINE_GUARD) != -1) {
                    category |= MARINE_GUARD;
                }
                if (type.find(PROPERTY_ECC_TYPE_KEY_MOUTAIN_RESCUE) != -1) {
                    category |= MOUNTAIN_RESCUE;
                }
                en.categories = category;
            }

            // Get ECC condition from property
            ret = snprintf(propertyCondition, MAX_PROP_CHARS, "%s%d", PROPERTY_ECC_CONDITION, i);
            if (ret < 0) {
                return false;
            }
            rfx_property_get(propertyCondition, eccCondition, "false");
            if (strcmp(eccCondition, "true") == 0) {
                en.condition = CONDITION_NO_SIM;
            }

            // Get ECC PLMN from property
            ret = snprintf(propertyPlmn, MAX_PROP_CHARS, "%s%d", PROPERTY_ECC_PLMN, i);
            if (ret < 0) {
                return false;
            }
            rfx_property_get(propertyPlmn, eccPlmn, "");
            if (strlen(eccPlmn) > PLMN_CHAR_LEN) {
                char eccMcc[MCC_CHAR_LEN + 1] = {0};
                char eccMnc[MCC_CHAR_LEN + 1] = {0};
                strncpy(eccMcc, eccPlmn, MCC_CHAR_LEN);
                eccMcc[MCC_CHAR_LEN] = '\0';
                // +1 for skip blank in xml (Ex: 460 11)
                strncpy(eccMnc, eccPlmn + MCC_CHAR_LEN + 1, MCC_CHAR_LEN);
                eccMnc[MCC_CHAR_LEN] = '\0';
                en.mcc = eccMcc;
                en.mnc = eccMnc;
            }

            addEmergencyNumber(en, currentPlmn, isSimInsert);
        }
    }

    return true;
}

/*****************************************************************************
 * Class TestEccNumberSource
 *****************************************************************************/
TestEccNumberSource::TestEccNumberSource(int slotId) : EccNumberSource(slotId) {
}

TestEccNumberSource::~TestEccNumberSource() {
}

bool TestEccNumberSource::update(bool isSimInsert) {
    if (ECC_DEBUG == 1) {
        RFX_LOG_V(RFX_LOG_TAG, "[TEST][%d][%s] isSimInsert: %d",
                mSlotId, __FUNCTION__, isSimInsert);
    }
    if (!isSimInsert) {
        // Test ECC only works when SIM insert
        return false;
    }

    char eccList[MAX_PROP_CHARS] = {0};
    char* ecc = NULL;
    char* savePtr = NULL;

    mEccList.clear();

    // get from Test ECC
    rfx_property_get(PROPERTY_ECC_TEST, eccList, "");
    if (strlen(eccList) > 0) {
        ecc = strtok_r(eccList, ",;", &savePtr);
        while (ecc != NULL) {
            EmergencyNumber en;
            en.number = ecc;
            en.mcc = "";
            en.mnc = "";
            en.categories = ECC_CATEGORY_NOT_DEFINED;
            en.sources = SOURCE_TEST;
            en.condition = CONDITION_ALWAYS;
            mEccList.push_back(en);

            ecc = strtok_r(NULL, ",;", &savePtr);
        }
    }
    return true;
}

/*****************************************************************************
 * Class ModemEccNumberSource
 *****************************************************************************/
ModemEccNumberSource::ModemEccNumberSource(int slotId) : EccNumberSource(slotId) {
}

ModemEccNumberSource::~ModemEccNumberSource() {
}

bool ModemEccNumberSource::update(RIL_EmergencyNumber *list, int count) {
    mEccList.clear();
    if (list == NULL || count == 0) {
        return true;
    }

    for (int i = 0; i < count; i++) {
        EmergencyNumber en;
        en.number = list[i].number;
        en.mcc = list[i].mcc;
        en.mnc = list[i].mnc;
        en.categories = list[i].categories;
        en.sources = list[i].sources;
        en.condition = list[i].isFake ? CONDITION_MMI : CONDITION_ALWAYS;
        mEccList.push_back(en);
    }

    return true;
}

