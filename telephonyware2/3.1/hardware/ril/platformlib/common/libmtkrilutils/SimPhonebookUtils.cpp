/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2017. All rights reserved.
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
#include <regex>
#include <string>
#include <cstring>
#include <mtk_log.h>
#include <mtk_properties.h>
#include "SimPhonebookUtils.h"
#include "libmtkrilutils.h"

using std::string;
using std::regex;
using std::match_results;


#define RFX_LOG_TAG "SimPhonebookUtils"

const char SimPhonebookUtils::PAUSE = ',';
const char SimPhonebookUtils::WAIT = ';';
const char SimPhonebookUtils::WILD = 'N';


SimPhonebookUtils::~SimPhonebookUtils() {
}

#ifdef ESC_SUPPORT
#define SPEC_ENCODING_ARR_LEN 54
static unsigned short specialEncoding[SPEC_ENCODING_ARR_LEN][2] = {
#else
#define SPEC_ENCODING_ARR_LEN 44
static unsigned short specialEncoding[SPEC_ENCODING_ARR_LEN][2] = {
#endif
    {0x0040, 0x0000},  //@
    {0x00A3, 0x0001},
    {0x0024, 0x0002},
    {0x00A5, 0x0003},
    {0x00E8, 0x0004},
    {0x00E9, 0x0005},
    {0x00F9, 0x0006},
    {0x00EC, 0x0007},
    {0x00F2, 0x0008},
    {0x00C7, 0x0009},
    {0x0020, 0x0020},
    {0x00D8, 0x000B},
    {0x00F8, 0x000C},
    {0x0020, 0x0020},
    {0x00C5, 0x000E},
    {0x00E5, 0x000F},
    {0x0394, 0x0010},
    {0x005F, 0x0011},
    {0x03A6, 0x0012},
    {0x0393, 0x0013},
    {0x039B, 0x0014},
    {0x03A9, 0x0015},
    {0x03A0, 0x0016},
    {0x03A8, 0x0017},
    {0x03A3, 0x0018},
    {0x0398, 0x0019},
    {0x039E, 0x001A},
    {0x00C6, 0x001C},
    {0x00E6, 0x001D},
    {0x00DF, 0x001E},
    {0x00C9, 0x001F},
    {0x00A4, 0x0024},
    {0x00A1, 0x0040},
    {0x00C4, 0x005B},
    {0x00D6, 0x005C},
    {0x00D1, 0x005D},
    {0x00DC, 0x005E},
    {0x00A7, 0x005F},
    {0x00BF, 0x0060},
    {0x00E4, 0x007B},
    {0x00F6, 0x007C},
    {0x00F1, 0x007D},
    {0x00FC, 0x007E},
    {0x00E0, 0x007F},
#ifdef ESC_SUPPORT
    {0x0020, 0x0020},
    {0x005E, 0x1B14},
    {0x007B, 0x1B28},
    {0x007D, 0x1B29},
    {0x005C, 0x1B2F},
    {0x005B, 0x1B3C},
    {0x007E, 0x1B3D},
    {0x005D, 0x1B3E},
    {0x007C, 0x1B40},
    {0x20AC, 0x1B65}
#endif
};


int SimPhonebookUtils::hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return (c - '0');
    }
    if (c >= 'A' && c <= 'F') {
        return (c - 'A' + 10);
    }
    if (c >= 'a' && c <= 'f') {
        return (c - 'a' + 10);
    }
    return 0;
}

int SimPhonebookUtils::buildAdnAnrNumber(char * src, char *dest, int maxLen) {
    int i = 0;
    int j = 0;
    int ton = 0x81;
    for (i = 0; i < maxLen; i++) {
        if (src[i] == '+') {
            ton = 0x91;
            continue;
        } else if (src[i] == PAUSE) {
            dest[j] = 'p';
        } else if (src[i] == WAIT) {
            dest[j] = 'w';
        } else if (src[i] == WILD) {
            dest[j] = '?';
        } else {
            dest[j] = src[i];
        }
        j++;
    }
    return ton;
}

int SimPhonebookUtils::transferSpecialAlpaToReal(char * src, char *dest, int maxLen) {
    int i = 0;
    for (i = 0; i < maxLen; i++) {
        if (src[i] == 'p') {
            dest[i] = PAUSE;
        } else if (src[i] == 'w') {
            dest[i] = WAIT;
        } else if (src[i] == '?') {
            dest[i] = WILD;
        } else {
            dest[i] = src[i];
        }
    }
    return 0;
}

int SimPhonebookUtils::hexCharToDecInt(char *hex, int length) {
    int i = 0;
    int value, digit;

    for (i = 0, value = 0; i < length && hex[i] != '\0'; i++) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            digit = hex[i] - '0';
        }
        else if (hex[i] >= 'A' && hex[i] <= 'F') {
            digit = hex[i] - 'A' + 10;
        }
        else if (hex[i] >= 'a' && hex[i] <= 'f') {
            digit = hex[i] - 'a' + 10;
        }
        else {
            return -1;
        }
        value = value*16 + digit;
    }

    return value;
}

string SimPhonebookUtils::prependPlusToNumber(string number) {
    string ret = "";
    regex pattern("^([*][#]|[*]{1,2}|[#]{1,2})([0-9]{2,3})([*])([0-9]+)(.*)(#)$");
    match_results<string::const_iterator> result;
    bool valid = regex_match(number, result, pattern);
    if (valid && (result.length() > 0)) {
        ret += result[1];
        ret += result[2];
        ret += result[3];
        ret += "+";
        ret += result[4];
        ret += result[5];
        ret += result[6];
    } else {
        regex pattern1("(^[#*])(.*)([#*])(.*)(#)$");
        match_results<string::const_iterator> result1;
        bool valid1 = regex_match(number, result1, pattern1);
        if (valid1 && (result1.length() > 0)) {
            string nullString = "";
            if (nullString.compare(result[2]) == 0) {
                ret += result[1];
                ret += result[3];
                ret += result[4];
                ret += result[5];
                ret += "+";
            } else {
                ret += result[1];
                ret += result[2];
                ret += result[3];
                ret += "+";
                ret += result[4];
                ret += result[5];
            }
        } else {
            regex pattern2("(^[#*])(.*)([#*])(.*)");
            match_results<string::const_iterator> result2;
            bool valid2 = regex_match(number, result2, pattern2);
            if (valid2 && (result2.length() > 0)) {
                ret += result2[1];
                ret += result2[2];
                ret += result2[3];
                ret += "+";
                ret += result2[4];
            } else {
                ret += "+";
                ret += number;
            }
        }
    }
    return ret;
}

char* SimPhonebookUtils::getPhbStorageString(int type) {
    char* str = NULL;
    switch (type) {
        case RIL_PHB_ADN:
            str = (char*)"SM";
            break;
        case RIL_PHB_FDN:
            str = (char*)"FD";
            break;
        case RIL_PHB_MSISDN:
            str = (char*)"ON";
            break;
        case RIL_PHB_ECC:
            str = (char*)"EN";
            break;
    }
    return str;
}


int SimPhonebookUtils::encodeUCS2_0x81(char *src, char *des, int maxLen) {
    int i, j, k, len = 0;
    unsigned int base = 0xFF000000;
    unsigned short tmpAlphaId[RIL_MAX_PHB_NAME_LEN+3+1];

    len = (int)strlen(src);
    for (i=0, j=0; i < len; i+=4, j++) {
        tmpAlphaId[j] = (unsigned short) hexCharToDecInt(src+i, 4);
    }
    tmpAlphaId[j] = '\0';
    len = j;

    mtkLogD(RFX_LOG_TAG, "0x81: len: %d", len);

    if (len <= 3)   // at least 3 characters
        return 0;
    if ( ((len+3)*2+1) > maxLen)  // the destinaiton buffer is not enough(include '\0')
        return 0;

    for (i = 0; i < len; i++) {
        int needSpecialEncoding = 0;

        if (tmpAlphaId[i] & 0x8000)  // Because base pointer of 0x81 is 0hhh hhhh h000 0000
            return 0;

        for (k = 0; k < SPEC_ENCODING_ARR_LEN; k++) {
            if (tmpAlphaId[i] == specialEncoding[k][0]) {
                mtkLogD(RFX_LOG_TAG, "Haman 0x81 found: i: %d, tmpAlphaId: %d", i, tmpAlphaId[i]);
                tmpAlphaId[i] = specialEncoding[k][1];
                needSpecialEncoding = 1;
                break;
            }
        }

        if (needSpecialEncoding != 1) {
            if (tmpAlphaId[i] < 0x80) {
#ifndef ESC_SUPPORT
                if (tmpAlphaId[i] == 0x0020 ||
                    tmpAlphaId[i] == 0x005E ||
                    tmpAlphaId[i] == 0x007B ||
                    tmpAlphaId[i] == 0x007D ||
                    tmpAlphaId[i] == 0x005B ||
                    tmpAlphaId[i] == 0x007E ||
                    tmpAlphaId[i] == 0x005D ||
                    tmpAlphaId[i] == 0x005C ||
                    tmpAlphaId[i] == 0x007C )
                    return 0;
                else
#endif
                {
                    if (tmpAlphaId[i] == 0x0060) {
                        if (base == 0xFF000000) {
                            base = 0;
                            tmpAlphaId[i] = 0x00E0;
                        } else {
                            return 0;
                        }
                    }

                    continue;
                }
            }

            if (base == 0xFF000000) {
                base = tmpAlphaId[i] & RIL_PHB_UCS2_81_MASK;
            }
            tmpAlphaId[i] ^= base;
            mtkLogD(RFX_LOG_TAG, "0x81: alpha: %x", tmpAlphaId[i]);
            if ( tmpAlphaId[i] >= 0x80)
                break;
            tmpAlphaId[i] |= 0x80;
        }
    }

    if (i != len) {
        return 0;
    }
    // Make up UCS2 0x81 String
    if (sprintf(des, "81%02X%02X", len, base>>7) < 0) {
        return 0;
    }
    int realLen = 0;
    char* pLen = des;
    des += 6;

    for (i=0; i < len; i++, des+=2) {
        if ((tmpAlphaId[i] & 0xFF00) != 0x1B00) {
            mtkLogD(RFX_LOG_TAG, "Haman1B 0x81: i: %d, tmpAlphaId: %x", i, tmpAlphaId[i]);
            if (sprintf(des, "%02X", tmpAlphaId[i]) < 0) {
                return 0;
            }
            mtkLogD(RFX_LOG_TAG, "Haman pleni: %s", pLen);
        }
        else {
            if (sprintf(des, "%04X", tmpAlphaId[i]) < 0) {
                return 0;
            }
            mtkLogD(RFX_LOG_TAG, "Haman 0x81: i: %d, tmpAlphaId: %x", i, tmpAlphaId[i]);
            mtkLogD(RFX_LOG_TAG, "Haman plenx: %s", pLen);
            des+=2;
            realLen++;
        }
    }

    realLen += len;
    if (realLen > len) {
        *(pLen + 2) = (char)((realLen/16) > 9 ? ((realLen/16) - 10 + 'A') : (realLen/16) + '0');
        *(pLen + 3) = (char)((realLen%16) > 9 ? ((realLen%16) - 10 + 'A') : (realLen%16) + '0');
    }
    *des = '\0';

    return realLen;
}

int SimPhonebookUtils::encodeUCS2_0x82(char *src, char *des, int maxLen) {
    int i, j, k, len;
    int min, max, base;
    int needSpecialEncoding = 0;
    unsigned short tmpAlphaId[RIL_MAX_PHB_NAME_LEN+4+1];

    int realLen = 0;
    char* pLen = des;

    min = max = base = -1;

    len = (int)strlen(src);
    for (i=0, j=0; i < len; i+=4, j++) {
        tmpAlphaId[j] = (unsigned short) hexCharToDecInt(src+i, 4);
    }
    tmpAlphaId[j] = '\0';
    len = j;

    mtkLogD(RFX_LOG_TAG, "0x82: len: %d", len);

    if (len <= 4)   // at least 4 characters
        return 0;
    if (((len+4)*2+1) > maxLen)  // the destinaiton buffer is not enough(include '\0')
        return 0;

    for (i=0; i < len; i++) {
        for (k = 0; k < SPEC_ENCODING_ARR_LEN; k++) {
            if (tmpAlphaId[i] == specialEncoding[k][0]) {
                needSpecialEncoding = 1;
                break;
            }
        }

        if (needSpecialEncoding == 0) {
            if ((tmpAlphaId[i] < 0x0080) && (tmpAlphaId[i] != 0x0060)) {
#ifndef ESC_SUPPORT
                 if (tmpAlphaId[i] == 0x0020 ||
                    tmpAlphaId[i] == 0x005E ||
                    tmpAlphaId[i] == 0x007B ||
                    tmpAlphaId[i] == 0x007D ||
                    tmpAlphaId[i] == 0x005B ||
                    tmpAlphaId[i] == 0x007E ||
                    tmpAlphaId[i] == 0x005D ||
                    tmpAlphaId[i] == 0x005C ||
                    tmpAlphaId[i] == 0x007C )
                    return 0;
#endif
            } else {    // (tmpAlphaId[i] >= 0x80) || (tmpAlphaId[i] == 0x0060)
                if (min < 0) {
                    min = max = tmpAlphaId[i];
                } else if (tmpAlphaId[i] < min) {
                    min = tmpAlphaId[i];
                } else if (tmpAlphaId[i] > max) {
                    max = tmpAlphaId[i];
                }
            }
        } else {
            needSpecialEncoding = 0;
        }
    }

    if (min < 0)
        min = max = 0;

    mtkLogD(RFX_LOG_TAG, "0x82: min: 0x%x, max: 0x%x", min, max);

    if ((max - min) >= 128)
        return 0;

    base = min;

    // encode the String
    for (i=0; i < len; i++) {
        for (k = 0; k < SPEC_ENCODING_ARR_LEN; k++) {
            if (tmpAlphaId[i] == specialEncoding[k][0]) {
                tmpAlphaId[i] = specialEncoding[k][1];
                needSpecialEncoding = 1;
                break;
            }
        }

        if (needSpecialEncoding == 0) {
            if ((tmpAlphaId[i] < 0x0080) && (tmpAlphaId[i] != 0x0060))
                continue;

            tmpAlphaId[i] -= base;
            tmpAlphaId[i] |= 0x80;
        } else {
            needSpecialEncoding = 0;
        }
    }

    // Make up UCS2 0x82 String
    if (sprintf(des, "82%02X%04X", len, base) < 0) {
        return 0;
    }
    des += 8;

    for (i=0; i < len; i++, des+=2) {
        if ((tmpAlphaId[i] & 0xFF00) != 0x1B00) {
            if (sprintf(des, "%02X", tmpAlphaId[i]) < 0) {
                return 0;
            }
        } else {
            if (sprintf(des, "%04X", tmpAlphaId[i]) < 0) {
                return 0;
            }
            des+=2;
            realLen++;
        }
    }

    realLen += len;

    if (realLen > len) {
        *(pLen + 2) = (char)((realLen/16) > 9 ? ((realLen/16) - 10 + 'A') : (realLen/16) + '0');
        *(pLen + 3) = (char)((realLen%16) > 9 ? ((realLen%16) - 10 + 'A') : (realLen%16) + '0');
    }

    *des = '\0';

    return realLen;
}

