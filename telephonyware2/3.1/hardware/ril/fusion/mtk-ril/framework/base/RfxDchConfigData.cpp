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

#include "RfxDchConfigData.h"
#include "RfxBaseData.h"
#include "telephony/mtk_ril.h"

RFX_IMPLEMENT_DATA_CLASS(RfxDchConfigData);

RfxDchConfigData::RfxDchConfigData(void *data, int length)
        : RfxBaseData(data, length) {
    if (data != NULL) {
        RIL_DchConfig* pIn =
                (RIL_DchConfig*)data;
        RIL_DchConfig *pOut =
                (RIL_DchConfig*)calloc(1, sizeof(RIL_DchConfig));
        RFX_ASSERT(pOut != NULL);
        memset(pOut, 0, sizeof(RIL_DchConfig));
        memcpy(pOut, pIn, sizeof(RIL_DchConfig));

        /* if (pIn->remote_addr != NULL) {
            pOut->remote_addr = strdup(pIn->remote_addr);
        }
        if (pIn->local_addr != NULL) {
            pOut->local_addr = strdup(pIn->local_addr);
        } */
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

        m_data = (void*)pOut;
        m_length = length;
    }
}

RfxDchConfigData::~RfxDchConfigData() {
    if (m_data) {
        RIL_DchConfig *p = (RIL_DchConfig*)m_data;
        // FREEIF(p->remote_addr);
        // FREEIF(p->local_addr);
        FREEIF(p->remote_tls_id);
        FREEIF(p->local_tls_id);
        FREEIF(p->remote_fp_str);
        FREEIF(p->local_fp_str);
        FREEIF(p->qos_hint);
        for (int j = 0; j < p->dcmap_count; j++) {
            FREEIF(p->dcmap_label[j]);
            FREEIF(p->dcmap_subprotocol[j]);
        }

        free(m_data);
        m_data = NULL;
    }
}
