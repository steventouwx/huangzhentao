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

#ifndef __ECC_NUMBER_PREFERENCE_H__
#define __ECC_NUMBER_PREFERENCE_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <telephony/mtk_ril.h>
#include <utils/String8.h>
#include <utils/Vector.h>
#include "tinyxml.h"

using namespace android;

#define ECC_PREFERENCE_FILE_PATH   "/system/vendor/etc/ecc_list_preference.xml"
#define ECC_PREFERENCE_TABLE       "EccPrefTable"
#define ECC_PREFERENCE_GSM_ONLY    "GsmOnly"
#define ECC_PREFERENCE_GSM_PREF    "GsmPref"
#define ECC_PREFERENCE_CDMA_PREF   "CdmaPref"
#define ECC_PREFERENCE_OPERATOR    "Operator"
#define ECC_PREFERENCE_ECC_LIST    "EccList"
#define ECC_PREFERENCE_OPTR        "persist.vendor.operator.optr"

class EccNumberPreference {
public:
    EccNumberPreference(int slotId);
    virtual ~EccNumberPreference();

// Override
public:

    // Check if the given number is GSM only number
    //
    // RETURNS:
    bool isGsmOnlyNumber(String8 number);

    // Check if the given number is GSM preferred number
    //
    // RETURNS:
    bool isGsmPreferredNumber(String8 number);

    // Check if the given number is CDMA preferred number
    //
    // RETURNS:
    bool isCdmaPreferredNumber(String8 number);

    // Update emergency number preference according to PLMN
    //
    // RETURNS:
    void update(String8 plmn);

private:
    void parseTag(TiXmlElement* eccPref, String8 preferenceType,
            String8& preferenceList, String8 plmn);

private:
    int mSlotId;
    String8 mGsmOnlyNumberList;
    String8 mGsmPreferredNumberList;
    String8 mCdmaPreferredNumberList;
};

#endif /* __ECC_NUMBER_PREFERENCE_H__ */
