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
#include "RfxQualifiedNetworkInfo.h"

/*****************************************************************************
 * Class RfxQualifiedNetworkInfo
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxQualifiedNetworkInfo);

RfxQualifiedNetworkInfo::RfxQualifiedNetworkInfo(void *data, int length) :
        RfxBaseData(data, length) {
    if (data != NULL) {
        MipcQualifiedNetworkInfo *pdata = (MipcQualifiedNetworkInfo *)data;
        MipcQualifiedNetworkInfo *response =
                (MipcQualifiedNetworkInfo *)calloc(1, sizeof(MipcQualifiedNetworkInfo));
        RFX_ASSERT(response != NULL);
        strncpy(response->cmd, pdata->cmd, MIPC_MAX_EIWLAN_CMD_LEN - 1);
        strncpy(response->type, pdata->type, MIPC_MAX_EIWLAN_TYPE_LEN - 1);
        response->ratNum = pdata->ratNum;
        response->ratList = (char *)calloc(response->ratNum, sizeof(char));
        RFX_ASSERT(response->ratList != NULL);
        strncpy(response->ratList, pdata->ratList, response->ratNum);
        m_data = response;
        m_length = length;
    }
}

RfxQualifiedNetworkInfo::~RfxQualifiedNetworkInfo() {
    if (m_data != NULL) {
        FREEIF(((MipcQualifiedNetworkInfo *)m_data)->ratList);
    }
    FREEIF(m_data);
}
