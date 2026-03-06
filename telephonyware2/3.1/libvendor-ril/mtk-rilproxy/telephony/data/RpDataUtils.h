// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2015. All rights reserved.
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

#ifndef __RP_DATA_UTILS_H__
#define __RP_DATA_UTILS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <aidl/android/hardware/radio/data/PdpProtocolType.h>
using namespace std;

#include "power/RadioConstants.h"
#include "RfxStatusDefs.h"
#include "RfxController.h"
#include "DataCallResponse.h"
#include "../include/telephony/mtk_ril_ivt.h"

#include <binder/Parcel.h>
using ::android::Parcel;

#define MIN(a, b) (a < b) ? a : b

/*****************************************************************************
 * Class RpDataUtils
 *****************************************************************************/

class RpDataUtils {
private:
    static int mNetId;
public:
#ifdef UPDATE_APN
    static const int IOT_APN_1;
    static const int IOT_APN_2;
    static const int IOT_APN_3;
    static const int IOT_APN_4;
    static const int IOT_APN_5;
    static const int IOT_APN_6;
    static const int IOT_APN_7;
    static const int IOT_APN_8;
    static const int MOBILE_APN;
    static const int MMS_APN;
    static const int WAP_APN;
    static const int SUPL_APN;
    static const int RCS_APN;
    static const int BIP_APN;
#endif //UPDATE_APN
    RpDataUtils();
    virtual ~RpDataUtils();

    static const char* requestOrUrcToString(int reqId);

    static inline void split(string &s, char delim, vector<string> &ret);
    static inline void split(string &s, char delim, vector<string> &ret, int number);
    static inline void trim(string &s);
    static inline int random(int max);
    static int generateNetId();

    static inline string i2s(int number);
    static inline string i2s(long number);
    static inline string b2s(bool enable);
    static inline string i2s(vector<string>::size_type number);

    static void writeStringToParcel(Parcel *p, const char *s);
    static void fillDataCallResponse(Parcel*p, MTK_Data_Call_Response_v1 dataCallResponse);
    static void parseDataCallResponse(MTK_Data_Call_Response_v1 *dataCallResponse, Parcel*p);
    static char * strdupReadString(Parcel *p);
    static void freeMemory(char *&s);

    static vector<DataCallResponse> parseDataConnectionInfo(Parcel* parcel);

    static void configNetwork(int netId, string interfaceName, string gateWay, string dns);
    static void tearDownNetwork(int netId, string interfaceName);

    static const char *apnState2string(RIL_Data_Call_PdnState apnState);
    static const char *pdnState2string(int pdnState);
    static std::string profileId2Type(int32_t profileId);
    static int32_t type2profileId(std::string type);
    static ::aidl::android::hardware::radio::data::PdpProtocolType convertStringToPdpProtocolType(const char *ptr);
    static void saveCidByType(int cid, std::string type);
    static int32_t getMtuByMccmnc(char *mccmnc);
};

inline void RpDataUtils::split(string &s, char delim, vector<string> &ret) {
    string::size_type last = 0;
    string::size_type index = s.find_first_of(delim, last);
    while (index != string::npos) {
        ret.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (last < s.size()) {
        ret.push_back(s.substr(last, s.size() - last));
    }
}

inline void RpDataUtils::split(string &s, char delim, vector<string> &ret,
        int number) {
    if (number <= 1) {
        ret.push_back(s);
        return;
    }

    string::size_type last = 0;
    string::size_type index = s.find_first_of(delim, last);
    int count = 0;

    while (count < number) {
        if (index != string::npos) {
            ret.push_back(s.substr(last, index - last));
            count++;
            last = index + 1;
            index = s.find_first_of(delim, last);
        } else {
            if (last < s.size()) {
                ret.push_back(s.substr(last, s.size() - last));
            }
            break;
        }
    }
}

inline void RpDataUtils::trim(string &s) {
    string::iterator itor = s.begin();
    while (itor != s.end()) {
        if (*itor == ' ' || *itor == '\t' || *itor == '\n') {
            s.erase(itor);
        }
        itor++;
    }
}

inline int RpDataUtils::random(int max) {
    unsigned seed = time(0);
    if (seed == -1) {
        return 0;
    }
    srand(seed);
    int result = rand() % max;
    return result;
}

inline string RpDataUtils::i2s(int number) {
    char str[20];
    int ret = sprintf(str, "%d", number);
    if (ret < 0)
        return string("");
    return string(str);
}

inline string RpDataUtils::i2s(long number) {
    char str[20];
    int ret = sprintf(str, "%ld", number);
    if (ret < 0)
        return string("");
    return string(str);
}

inline string RpDataUtils::i2s(vector<string>::size_type number) {
    char str[20];
    int ret = sprintf(str, "%u", number);
    if (ret < 0)
        return string("");
    return string(str);
}

inline string RpDataUtils::b2s(bool enable) {
    if (enable) {
        return "true";
    } else {
        return "false";
    }
}
#endif /* __RP_DATA_UTILS_H__ */
