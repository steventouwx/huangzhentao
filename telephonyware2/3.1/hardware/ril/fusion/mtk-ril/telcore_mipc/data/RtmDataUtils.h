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

#ifndef __RTM_DATA_UTILS_H__
#define __RTM_DATA_UTILS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "rfx_properties.h"
#include <cutils/jstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREFER_DATA_SLOT_PROPERTY "vendor.ril.data.prefer_data_slot"
#define PLMN_MIN_LENGTH 5
#define PLMN_MAX_LENGTH 6

#ifndef UNUSED
    #define UNUSED(x) ((void)(x))   // eliminate "warning: unused parameter"
#endif

/*****************************************************************************
 * Class RtmDataUtils
 *****************************************************************************/

class RtmDataUtils {
public:
    RtmDataUtils() {}
    virtual ~RtmDataUtils() {}
    static void setPreferDataSlotToProperty(int slot);
    static bool isValidPlmn(const char *pchPlmn);
};

/*
 *  Save prefer data slot info for resend it to modem after rild reset.
 *  It will be triggerred when handleSetPreferredDataModem.
 */
inline void RtmDataUtils::setPreferDataSlotToProperty(int slot) {
    rfx_property_set(PREFER_DATA_SLOT_PROPERTY, String8::format("%d", slot).string());
}

inline bool RtmDataUtils::isValidPlmn(const char *pchPlmn) {
    int len = strlen(pchPlmn);
    if (len < PLMN_MIN_LENGTH || len > PLMN_MAX_LENGTH) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (pchPlmn[i] < '0' || pchPlmn[i] > '9') {
            return false;
        }
    }
    return true;
}

#endif /* __RTM_DATA_UTILS_H__ */
