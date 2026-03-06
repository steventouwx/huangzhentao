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

#include "RfxSlicingConfigResponseData.h"

#define RFX_LOG_TAG "RfxSlicingConfigData"

RFX_IMPLEMENT_DATA_CLASS(RfxSlicingConfigResponseData);

RfxSlicingConfigResponseData::RfxSlicingConfigResponseData(void *data, int length) :
        RfxBaseData(data, length) {
    m_length = length;
    copySlicingConfigData((RIL_SlicingConfig*)data);
}

void RfxSlicingConfigResponseData::copySlicingConfigData(RIL_SlicingConfig *data) {
    if (data == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] The responses is NULL", __FUNCTION__);
        return;
    }
    RFX_LOG_D(RFX_LOG_TAG, "[%s] Slicing config rule count: %d, slice info count: %d",
            __FUNCTION__, data->ruleCount, data->sliceCount);
    RIL_SlicingConfig *pData = (RIL_SlicingConfig *)calloc(1, sizeof(RIL_SlicingConfig));
    RFX_ASSERT(pData != NULL);
    pData->ruleCount = data->ruleCount;
    pData->sliceCount = data->sliceCount;

    // Copy URSP rule
    if (data->ruleCount > 0 && data->urspRules != NULL) {
        pData->urspRules = (RIL_UrspRule *)calloc(pData->ruleCount, sizeof(RIL_UrspRule));
        RFX_ASSERT(pData->urspRules != NULL);
        memcpy(pData->urspRules, data->urspRules, pData->ruleCount * sizeof(RIL_UrspRule));
        for (int i = 0; i < pData->ruleCount; i++) {
            RFX_LOG_D(RFX_LOG_TAG, "[%s] URSP rule TD count: %d, RSD count: %d", __FUNCTION__,
                    data->urspRules[i].tdCount, data->urspRules[i].rsdCount);
            // Copy TD info
            if (data->urspRules[i].tdCount > 0 && data->urspRules[i].trafficDescriptors != NULL) {
                pData->urspRules[i].tdCount = data->urspRules[i].tdCount;
                pData->urspRules[i].trafficDescriptors = (RIL_TrafficDescriptor *)calloc(
                        data->urspRules[i].tdCount, sizeof(RIL_TrafficDescriptor));
                RFX_ASSERT(pData->urspRules[i].trafficDescriptors != NULL);
                // Copy dnn and os APP id
                for (int j = 0; j < data->urspRules[i].tdCount; j++) {
                    if (data->urspRules[i].trafficDescriptors[j].dnn != NULL) {
                        asprintf(&(pData->urspRules[i].trafficDescriptors[j].dnn), "%s",
                                data->urspRules[i].trafficDescriptors[j].dnn);
                    }
                    if (data->urspRules[i].trafficDescriptors[j].osAppIdLen > 0) {
                        pData->urspRules[i].trafficDescriptors[j].osAppId = (uint8_t*)calloc(
                                data->urspRules[i].trafficDescriptors[j].osAppIdLen,
                                sizeof(uint8_t));
                        RFX_ASSERT(pData->urspRules[i].trafficDescriptors[j].osAppId != NULL);
                        memcpy(pData->urspRules[i].trafficDescriptors[j].osAppId,
                                data->urspRules[i].trafficDescriptors[j].osAppId,
                                data->urspRules[i].trafficDescriptors[j].osAppIdLen);
                        // os APP id lenght
                        pData->urspRules[i].trafficDescriptors[j].osAppIdLen =
                                data->urspRules[i].trafficDescriptors[j].osAppIdLen;
                    }
                    RFX_LOG_D(RFX_LOG_TAG, "[%s] URSP rule[%d] TD[%d]: {DNN:%s, OsAppIdLen:%d}",
                            __FUNCTION__, i, j,
                            pData->urspRules[i].trafficDescriptors[j].dnn == NULL ? "" :
                            pData->urspRules[i].trafficDescriptors[j].dnn,
                            pData->urspRules[i].trafficDescriptors[j].osAppIdLen);
                }
            }

            // Copy RSD info
            if (data->urspRules[i].rsdCount > 0 &&
                    data->urspRules[i].routeSelectionDescriptor != NULL) {
                pData->urspRules[i].routeSelectionDescriptor =
                        (RIL_RouteSelectionDescriptor *)calloc(data->urspRules[i].rsdCount,
                        sizeof(RIL_RouteSelectionDescriptor));
                RFX_ASSERT(pData->urspRules[i].routeSelectionDescriptor != NULL);
                memcpy(pData->urspRules[i].routeSelectionDescriptor,
                        data->urspRules[i].routeSelectionDescriptor,
                        data->urspRules[i].rsdCount * sizeof(RIL_RouteSelectionDescriptor));
                // Copy sessionType and sliceInfo
                for (int j = 0; j < data->urspRules[i].rsdCount; j++) {
                    if (data->urspRules[i].routeSelectionDescriptor[j].sessionType != NULL) {
                        asprintf(&(pData->urspRules[i].routeSelectionDescriptor[j].sessionType),
                                "%s", data->urspRules[i].routeSelectionDescriptor[j].sessionType);
                    }
                    if (data->urspRules[i].routeSelectionDescriptor[j].sliceInfo != NULL) {
                        pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo =
                                (RIL_SliceInfo*)calloc(
                                data->urspRules[i].routeSelectionDescriptor[j].sliceCount,
                                sizeof(RIL_SliceInfo));
                        RFX_ASSERT(pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo
                                != NULL);
                        memcpy(pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo,
                                data->urspRules[i].routeSelectionDescriptor[j].sliceInfo,
                                data->urspRules[i].routeSelectionDescriptor[j].sliceCount *
                                sizeof(RIL_SliceInfo));
                    }
                    // {precedence,sessionType,sscMode,sliceCount,dnnCount}
                    RFX_LOG_D(RFX_LOG_TAG, "[%s] URSP rule[%d] RSD[%d]: {%d,%s,%d,%d,%d},data {%d,%s,%d,%d,%d}",
                            __FUNCTION__, i, j,
                            pData->urspRules[i].routeSelectionDescriptor[j].precedence,
                            pData->urspRules[i].routeSelectionDescriptor[j].sessionType,
                            pData->urspRules[i].routeSelectionDescriptor[j].sscMode,
                            pData->urspRules[i].routeSelectionDescriptor[j].sliceCount,
                            pData->urspRules[i].routeSelectionDescriptor[j].dnnCount,
                            data->urspRules[i].routeSelectionDescriptor[j].precedence,
                            data->urspRules[i].routeSelectionDescriptor[j].sessionType,
                            data->urspRules[i].routeSelectionDescriptor[j].sscMode,
                            data->urspRules[i].routeSelectionDescriptor[j].sliceCount,
                            data->urspRules[i].routeSelectionDescriptor[j].dnnCount);
                }
            }
        }
    }

    // Copy Slice info
    if (data->sliceCount > 0 && data->sliceInfo != NULL) {
        pData->sliceInfo = (RIL_SliceInfo *)calloc(pData->sliceCount, sizeof(RIL_SliceInfo));
        RFX_ASSERT(pData->sliceInfo != NULL);
        memcpy(pData->sliceInfo, data->sliceInfo, pData->sliceCount * sizeof(RIL_SliceInfo));
        for (int i = 0; i < pData->sliceCount; i++) {
            RFX_LOG_D(RFX_LOG_TAG, "[%s] Slice info count: %d, [%d]{%d,%d,%d,%d,%d}", __FUNCTION__,
                    pData->sliceCount, i,
                    pData->sliceInfo[i].sst,
                    pData->sliceInfo[i].sliceDifferentiator,
                    pData->sliceInfo[i].mappedHplmnSst,
                    pData->sliceInfo[i].mappedHplmnSD,
                    pData->sliceInfo[i].status);
        }
    }
    m_data = pData;
}

RfxSlicingConfigResponseData::~RfxSlicingConfigResponseData() {
    if (m_data != NULL) {
        RIL_SlicingConfig *pData = (RIL_SlicingConfig*)m_data;
        // Free URSP rule memory
        for (int i = 0; i < pData->ruleCount; i++) {
            // Free dnn and os APP id
            for (int j = 0; j < pData->urspRules[i].tdCount; j++) {
                FREEIF(pData->urspRules[i].trafficDescriptors[j].dnn);
                FREEIF(pData->urspRules[i].trafficDescriptors[j].osAppId);
            }
            // Free TD info
            FREEIF(pData->urspRules[i].trafficDescriptors);

            // Free sessionType and sliceInfo
            for (int j = 0; j < pData->urspRules[i].rsdCount; j++) {
                FREEIF(pData->urspRules[i].routeSelectionDescriptor[j].sessionType);
                FREEIF(pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo);
            }
            // Free RSD info
            FREEIF(pData->urspRules[i].routeSelectionDescriptor);
        }
        FREEIF(pData->urspRules);

        // Free slice info memory
        FREEIF(pData->sliceInfo);

        // Free slicing config memory
        FREEIF(m_data);
    }

    m_length = 0;
}
