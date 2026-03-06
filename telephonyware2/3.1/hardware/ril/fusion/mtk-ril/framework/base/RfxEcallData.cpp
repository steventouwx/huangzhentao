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

#include "RfxEcallData.h"

RFX_IMPLEMENT_DATA_CLASS(RfxEcallData);

RfxEcallData::RfxEcallData(void *data, int length) : RfxBaseData(data, length) {
    RIL_ECallReqMsg *dupData;
    RIL_ECallReqMsg *srcData = (RIL_ECallReqMsg*)data;
    int strLength;

    dupData = (RIL_ECallReqMsg *)calloc(1, sizeof(RIL_ECallReqMsg));
    RFX_ASSERT(dupData != NULL);
    memset(dupData, 0x00, sizeof(RIL_ECallReqMsg));
    m_data = dupData;

    dupData->ecall_cat = srcData->ecall_cat;
    dupData->ecall_variant = srcData->ecall_variant;
    dupData->length = srcData->length;

    /* Copy address(dial string) */
    if (srcData->address == NULL) {
        return;
    }
    strLength = strlen(srcData->address) + 1;
    dupData->address = (char *)calloc(strLength, sizeof(char));
    RFX_ASSERT(dupData->address != NULL);
    memset(dupData->address, 0, strLength);
    memcpy(dupData->address, srcData->address, strLength);

    /* Copy msd data */
    if (srcData->msd_data == NULL) {
        return;
    }
    dupData->msd_data = (unsigned char *)calloc(srcData->length, sizeof(unsigned char));
    RFX_ASSERT(dupData->msd_data != NULL);
    memset(dupData->msd_data, 0, srcData->length);
    memcpy(dupData->msd_data, srcData->msd_data, srcData->length);
    dupData->domain = srcData->domain;
}

RfxEcallData::~RfxEcallData() {
    // free
    RIL_ECallReqMsg *data = (RIL_ECallReqMsg*)m_data;
    if (data != NULL) {
        if (data->address != NULL) {
            free(data->address);
        }
        if (data->msd_data != NULL) {
            free(data->msd_data);
        }
        free(m_data);
    }
}
