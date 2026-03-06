/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2021. All rights reserved.
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
#include "RfxCdmaCbSmsCfgData.h"
#include "RfxLog.h"
#include <telephony/mtk_ril.h>

/*****************************************************************************
 * Class RfxCdmaCbSmsCfgData
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RfxCdmaCbSmsCfgData);
RfxCdmaCbSmsCfgData::RfxCdmaCbSmsCfgData(void *data, int length) :
        RfxBaseData(data, length) {
    if (data != NULL) {
        int num = length / sizeof(RIL_CDMA_BroadcastSmsConfigInfo *);
        RIL_CDMA_BroadcastSmsConfigInfo **pCdmaBrSmsCnf = (RIL_CDMA_BroadcastSmsConfigInfo**)data;
        RIL_CDMA_BroadcastSmsConfigInfo **pData = (RIL_CDMA_BroadcastSmsConfigInfo**)calloc(
                1, sizeof(RIL_CDMA_BroadcastSmsConfigInfo*)*num);
        RFX_ASSERT(pData != NULL);

        for (int i = 0; i < num; i++) {
            pData[i] = (RIL_CDMA_BroadcastSmsConfigInfo*)calloc(
                    1, sizeof(RIL_CDMA_BroadcastSmsConfigInfo));
            RFX_ASSERT(pData[i] != NULL);
            pData[i]->service_category = pCdmaBrSmsCnf[i]->service_category;
            pData[i]->language = pCdmaBrSmsCnf[i]->language;
            pData[i]->selected = pCdmaBrSmsCnf[i]->selected;
        }
        m_data = (void*)pData;
        m_length = length;
    }
}

RfxCdmaCbSmsCfgData::~RfxCdmaCbSmsCfgData() {
    // free memory
    if (m_data != NULL) {
        RfxCdmaCbSmsCfgData **pData = (RfxCdmaCbSmsCfgData**)m_data;
        int num = m_length / sizeof(RfxCdmaCbSmsCfgData *);
        for (int i = 0; i < num; i++) {
            if (pData[i] != NULL) {
                free(pData[i]);
            }
        }
        free(m_data);
        m_data = NULL;
    }
}
