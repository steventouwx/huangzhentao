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

#include "RfxDchImsConfigData.h"
#include "RfxBaseData.h"
#include "telephony/mtk_ril.h"

RFX_IMPLEMENT_DATA_CLASS(RfxDchImsConfigData);

RfxDchImsConfigData::RfxDchImsConfigData(void *data, int length)
        : RfxBaseData(data, length) {
    if (NULL != data) {
        RIL_DchImsDataConfig* pIn =
                (RIL_DchImsDataConfig*)data;
        RIL_DchImsDataConfig *pOut =
                (RIL_DchImsDataConfig*)calloc(1, sizeof(RIL_DchImsDataConfig));
        RFX_ASSERT(pOut != NULL);
        memset(pOut, 0, sizeof(RIL_DchImsDataConfig));
        memcpy(pOut, pIn, sizeof(RIL_DchImsDataConfig));

        if (pIn->if_name != NULL) {
            pOut->if_name = strdup(pIn->if_name);
        }

        for (int i = 0; i < pIn->dc_config_num; i++) {
            RIL_DchConfig* configIn = &(pIn->dc_config_list[i]);
            RIL_DchConfig* configOut = &(pOut->dc_config_list[i]);

            /* if (configIn->remote_addr != NULL) {
                configOut->remote_addr = strdup(configIn->remote_addr);
            }
            if (configIn->local_addr != NULL) {
                configOut->local_addr = strdup(configIn->local_addr);
            } */
            if (configIn->remote_tls_id != NULL) {
                configOut->remote_tls_id = strdup(configIn->remote_tls_id);
            }
            if (configIn->local_tls_id != NULL) {
                configOut->local_tls_id = strdup(configIn->local_tls_id);
            }
            if (configIn->remote_fp_str != NULL) {
                configOut->remote_fp_str = strdup(configIn->remote_fp_str);
            }
            if (configIn->local_fp_str != NULL) {
                configOut->local_fp_str = strdup(configIn->local_fp_str);
            }
            if (configIn->qos_hint != NULL) {
                configOut->qos_hint = strdup(configIn->qos_hint);
            }
            if (configOut->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
                configOut->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
            }
            for (int j = 0; j < configOut->dcmap_count; j++) {
                if (configIn->dcmap_label[j] != NULL) {
                    configOut->dcmap_label[j] = strdup(configIn->dcmap_label[j]);
                }
                if (configIn->dcmap_subprotocol[j] != NULL) {
                    configOut->dcmap_subprotocol[j] = strdup(configIn->dcmap_subprotocol[j]);
                }
            }
        }
        m_data = (void*)pOut;
        m_length = length;
    }
}

RfxDchImsConfigData::~RfxDchImsConfigData() {
    if (m_data) {
        RIL_DchImsDataConfig *p = (RIL_DchImsDataConfig*)m_data;
        FREEIF(p->if_name);
        for (int i = 0; i < p->dc_config_num; i++) {
            RIL_DchConfig* config = &(p->dc_config_list[i]);
            // FREEIF(config->remote_addr);
            // FREEIF(config->local_addr);
            FREEIF(config->remote_tls_id);
            FREEIF(config->local_tls_id);
            FREEIF(config->remote_fp_str);
            FREEIF(config->local_fp_str);
            FREEIF(config->qos_hint);
            for (int j = 0; j < config->dcmap_count; j++) {
                FREEIF(config->dcmap_label[j]);
                FREEIF(config->dcmap_subprotocol[j]);
            }
        }
        free(m_data);
        m_data = NULL;
    }
}
