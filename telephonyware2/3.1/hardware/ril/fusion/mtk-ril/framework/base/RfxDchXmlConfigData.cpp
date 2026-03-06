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

#include "RfxDchXmlConfigData.h"
#include "RfxBaseData.h"
#include "telephony/mtk_ril.h"

#define RFX_LOG_TAG "RfxDchXmlConfigData"

RFX_IMPLEMENT_DATA_CLASS(RfxDchXmlConfigData);

RfxDchXmlConfigData::RfxDchXmlConfigData(void *data, int length)
        : RfxBaseData(data, length) {
    if (NULL != data) {
        RIL_DchXmlConfig* inConfig =
                (RIL_DchXmlConfig*)data;

        RIL_DchXmlConfig *outConfig =
            (RIL_DchXmlConfig *) calloc(1, sizeof(RIL_DchXmlConfig));
        RFX_ASSERT(outConfig != NULL);

        RFX_LOG_D(RFX_LOG_TAG, "vodc RfxDchXmlConfigData, %p, length:%d, size:%d",
                outConfig, length, sizeof(RIL_DchXmlConfig));

        outConfig->call_id = inConfig->call_id;
        if (inConfig->xml_data != NULL) {
            outConfig->xml_data = strdup(inConfig->xml_data);
        }

        m_data = (void*)outConfig;
        m_length = length;
    }
}

RfxDchXmlConfigData::~RfxDchXmlConfigData() {
    RIL_DchXmlConfig* dupData = (RIL_DchXmlConfig*) m_data;

    if (m_data != NULL) {
        FREEIF(dupData->xml_data);
        RFX_LOG_D(RFX_LOG_TAG, "vodc ~RfxDchXmlConfigData, %p, m_length:%d",
                m_data, m_length);
        free(m_data);
    }

    m_length = 0;
    m_data = NULL;
}
