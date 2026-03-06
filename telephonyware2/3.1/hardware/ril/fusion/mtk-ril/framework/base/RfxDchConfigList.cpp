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

#include "RfxDchConfigList.h"
#include "RfxBaseData.h"
#include "telephony/mtk_ril.h"

#define RFX_LOG_TAG "RfxDchConfigList"

RFX_IMPLEMENT_DATA_CLASS(RfxDchConfigList);

RfxDchConfigList::RfxDchConfigList(void *data, int length)
        : RfxBaseData(data, length) {
    if (NULL != data) {
        RIL_DchConfig* inConfigs =
                (RIL_DchConfig*)data;
        int count = length / sizeof(RIL_DchConfig);

        RIL_DchConfig *outConfigs =
            (RIL_DchConfig *) calloc(count, sizeof(RIL_DchConfig));
        RFX_ASSERT(outConfigs != NULL);

        RFX_LOG_D(RFX_LOG_TAG, "vodc RfxDchConfigList, %p, length:%d, count:%d, size:%d",
                outConfigs, length, count, sizeof(RIL_DchConfig));

        memcpy(outConfigs, inConfigs, length);

        for (int i = 0; i < count; i++) {
            RIL_DchConfig *pIn = &(inConfigs[i]);
            RIL_DchConfig *pOut = &(outConfigs[i]);

            if (pIn->remote_tls_id != NULL) {
                pOut->remote_tls_id = strdup(pIn->remote_tls_id);
            }
            if (pIn->local_tls_id != NULL) {
                pOut->local_tls_id = strdup(pIn->local_tls_id);
            }
            if (pIn->remote_fp_str != NULL) {
                pOut->remote_fp_str = strdup(pIn->remote_fp_str);
            }
            if (pIn->local_fp_str != NULL) {
                pOut->local_fp_str = strdup(pIn->local_fp_str);
            }
            if (pIn->qos_hint != NULL) {
                pOut->qos_hint = strdup(pIn->qos_hint);
            }
            if (pOut->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
                pOut->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
            }
            for (int j = 0; j < pOut->dcmap_count; j++) {
                if (pIn->dcmap_label[j] != NULL) {
                    pOut->dcmap_label[j] = strdup(pIn->dcmap_label[j]);
                }
                if (pIn->dcmap_subprotocol[j] != NULL) {
                    pOut->dcmap_subprotocol[j] = strdup(pIn->dcmap_subprotocol[j]);
                }
            }
        }
        m_data = (void*)outConfigs;
        m_length = length;
    }
}

RfxDchConfigList::~RfxDchConfigList() {
    int count = m_length / sizeof(RIL_DchConfig);
    RIL_DchConfig* dupData = (RIL_DchConfig*) m_data;

    if (m_data != NULL) {
        for (int i = 0; i < count; i++) {
            RIL_DchConfig *p = &(dupData[i]);
            FREEIF(p->remote_tls_id);
            FREEIF(p->local_tls_id);
            FREEIF(p->remote_fp_str);
            FREEIF(p->local_fp_str);
            FREEIF(p->qos_hint);
            RFX_LOG_D(RFX_LOG_TAG, "vodc ~RfxDchConfigList, %p, m_length:%d, count:%d-%d",
                    m_data, m_length, count, i);
            for (int j = 0; j < p->dcmap_count; j++) {
                FREEIF(p->dcmap_label[j]);
                FREEIF(p->dcmap_subprotocol[j]);
            }
        }
        // RFX_LOG_D(RFX_LOG_TAG, "vodc ~RfxDchConfigList-2");
        free(m_data);
        // RFX_LOG_D(RFX_LOG_TAG, "vodc ~RfxDchConfigList-3");
    }

    m_length = 0;
    m_data = NULL;
}
