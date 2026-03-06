/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2023. All rights reserved.
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

#include "RfxDchCloseSipData.h"
#include <telephony/mtk_ril.h>

#define LOG_TAG "RfxDchCloseSipData"

RFX_IMPLEMENT_DATA_CLASS(RfxDchCloseSipData);

RfxDchCloseSipData::RfxDchCloseSipData(void *data, int length) : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_DchMapDelStruct *pIn = (RIL_DchMapDelStruct*)data;
        RIL_DchMapDelStruct *pOut = (RIL_DchMapDelStruct *)calloc(1,
                sizeof(RIL_DchMapDelStruct));
        RFX_ASSERT(pOut != NULL);
        pOut->callId = pIn->callId;
        pOut->closeAllStream = pIn->closeAllStream;
        pOut->dcmap_count = pIn->dcmap_count ;
        if (pOut->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
            pOut->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }

        for (int i = 0; i < pOut->dcmap_count ; i++) {
            pOut->dc_config_index[i] = pIn->dc_config_index[i];
            pOut->dcmap_stream_id[i] = pIn->dcmap_stream_id[i];
        }

        m_data = (void*)pOut;
        m_length = length;
    }
}

RfxDchCloseSipData::~RfxDchCloseSipData() {
    // free memory
    if (m_data != NULL) {
        free(m_data);
        m_data = NULL;
    }
}

