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

#include "RfxDataCallResponseData.h"

#define RFX_LOG_TAG "RfxDataCallResponseData"

RFX_IMPLEMENT_DATA_CLASS(RfxDataCallResponseData);

RfxDataCallResponseData::RfxDataCallResponseData(void *data, int length)
    : RfxBaseData(data, length) {
    m_length = length;
    copyDataCallResponseData((MTK_RIL_Data_Call_Response_v11*)data);
}

RfxDataCallResponseData::RfxDataCallResponseData(MTK_RIL_Data_Call_Response_v11 *data, int num)
    : RfxBaseData(data, num * sizeof(MTK_RIL_Data_Call_Response_v11)) {
    m_length = num * sizeof(MTK_RIL_Data_Call_Response_v11);
    copyDataCallResponseData(data);
}

RfxDataCallResponseData::~RfxDataCallResponseData() {
    MTK_RIL_Data_Call_Response_v11 *tmpPtr = (MTK_RIL_Data_Call_Response_v11 *)m_data;
    int num = m_length / sizeof(MTK_RIL_Data_Call_Response_v11);

    if (tmpPtr != NULL) {
        for (int i = 0; i < num; i++) {
            FREEIF((&tmpPtr[i])->type);
            FREEIF((&tmpPtr[i])->ifname);
            FREEIF((&tmpPtr[i])->addresses);
            FREEIF((&tmpPtr[i])->gateways);
            FREEIF((&tmpPtr[i])->dnses);
            FREEIF((&tmpPtr[i])->pcscf);
            FREEIF((&tmpPtr[i])->defaultQos);
            int j = 0;
            for (j = 0; j < tmpPtr[i].qosSessionCount; j++) {
                for (int k = 0; k < tmpPtr[i].qosSessions[j].qosFilterCount; k++) {
                    FREEIF(tmpPtr[i].qosSessions[j].qosFilters[k].localAddresses);
                    FREEIF(tmpPtr[i].qosSessions[j].qosFilters[k].remoteAddresses);
                }
                FREEIF(tmpPtr[i].qosSessions[j].qosFilters);
            }
            FREEIF(tmpPtr[i].qosSessions);
            FREEIF(tmpPtr[i].optionalSliceInfo);
            for (j = 0; j < tmpPtr[i].tdCount; j++) {
                FREEIF((&tmpPtr[i])->TDs[j].dnn);
                FREEIF((&tmpPtr[i])->TDs[j].osAppId);
                FREEIF((&tmpPtr[i])->TDs[j].ipDescriptors.ipv4);
                FREEIF((&tmpPtr[i])->TDs[j].ipDescriptors.maskV4);
                FREEIF((&tmpPtr[i])->TDs[j].ipDescriptors.ipv6);
                FREEIF((&tmpPtr[i])->TDs[j].domainDescriptors);
                FREEIF((&tmpPtr[i])->TDs[j].connectionCapabilities);
            }
            FREEIF((&tmpPtr[i])->TDs);
        }
        free(tmpPtr);
        tmpPtr = NULL;
    }
}

void RfxDataCallResponseData::copyDataCallResponseData(MTK_RIL_Data_Call_Response_v11 *data) {
    int num = m_length / sizeof(MTK_RIL_Data_Call_Response_v11);
    if (num == 0 || data == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "[%s] The number of responses is %d (if num != 0, means data is null)",
                __FUNCTION__, num);
        return;
    }

    MTK_RIL_Data_Call_Response_v11 *tmpPtr = (MTK_RIL_Data_Call_Response_v11 *)calloc(1, m_length);
    RFX_ASSERT(tmpPtr != NULL);
    MTK_RIL_Data_Call_Response_v11 *curPtr = data;

    for (int i = 0; i < num; i++) {
        tmpPtr[i].status                 = curPtr[i].status;
        tmpPtr[i].suggestedRetryTime     = curPtr[i].suggestedRetryTime;
        tmpPtr[i].cid                    = curPtr[i].cid;
        tmpPtr[i].active                 = curPtr[i].active;
        tmpPtr[i].mtu                    = curPtr[i].mtu;
        tmpPtr[i].mtuV6                  = curPtr[i].mtuV6;
        tmpPtr[i].accessNetwork          = curPtr[i].accessNetwork;
        tmpPtr[i].qosSessionCount        = curPtr[i].qosSessionCount;
        tmpPtr[i].handoverFailureMode    = curPtr[i].handoverFailureMode;
        tmpPtr[i].pduSessionId           = curPtr[i].pduSessionId;
        tmpPtr[i].tdCount                = curPtr[i].tdCount;
        tmpPtr[i].tdType                 = curPtr[i].tdType;

        copyString(&(tmpPtr[i].type),      curPtr[i].type);
        copyString(&(tmpPtr[i].ifname),    curPtr[i].ifname);
        copyString(&(tmpPtr[i].addresses), curPtr[i].addresses);
        copyString(&(tmpPtr[i].dnses),     curPtr[i].dnses);
        copyString(&(tmpPtr[i].gateways),  curPtr[i].gateways);
        copyString(&(tmpPtr[i].pcscf),     curPtr[i].pcscf);

        /* AOSP_V1_5::RadioAccessNetworks VS 3gpp 27.007 Sec 7.3
         * RadioAccessNetworks 0: UNKNOWN
         * RadioAccessNetworks 1: GERAN (0 GSM)
         * RadioAccessNetworks 2: UTRAN (2 UTRAN)
         * RadioAccessNetworks 3: EUTRAN(7 E-UTRAN)
         * RadioAccessNetworks 4: NGRAN (12 NG-RAN)*/
        if (curPtr[i].accessNetwork == NG_RAN && curPtr[i].optionalSliceInfo != NULL){
            // Slice info
            tmpPtr[i].optionalSliceInfo = (RIL_SliceInfo *)calloc(1, sizeof(RIL_SliceInfo));
            RFX_ASSERT(tmpPtr[i].optionalSliceInfo != NULL);
            tmpPtr[i].optionalSliceInfo->sst = curPtr[i].optionalSliceInfo->sst;
            tmpPtr[i].optionalSliceInfo->sliceDifferentiator
                    = curPtr[i].optionalSliceInfo->sliceDifferentiator;
            tmpPtr[i].optionalSliceInfo->mappedHplmnSst = curPtr[i].optionalSliceInfo->mappedHplmnSst;
            tmpPtr[i].optionalSliceInfo->mappedHplmnSD = curPtr[i].optionalSliceInfo->mappedHplmnSD;
            tmpPtr[i].optionalSliceInfo->status = curPtr[i].optionalSliceInfo->status;
        }

        // Traffic descriptor
        if (curPtr[i].tdCount != 0 && curPtr[i].TDs != NULL) {
            tmpPtr[i].TDs = (RIL_TrafficDescriptor *)calloc(1,
                    curPtr[i].tdCount * sizeof(RIL_TrafficDescriptor));
            RFX_ASSERT(tmpPtr[i].TDs != NULL);
            for (int j = 0; j < curPtr[i].tdCount; j++) {
                // dnn
                copyString(&(tmpPtr[i].TDs[j].dnn), curPtr[i].TDs[j].dnn);
                // osappid
                tmpPtr[i].TDs[j].osAppIdLen = curPtr[i].TDs[j].osAppIdLen;
                if (tmpPtr[i].TDs[j].osAppIdLen > 0) {
                    tmpPtr[i].TDs[j].osAppId = (uint8_t *) calloc(1,
                            curPtr[i].TDs[j].osAppIdLen * sizeof(uint8_t) + 1);
                    memcpy(tmpPtr[i].TDs[j].osAppId, curPtr[i].TDs[j].osAppId,
                            curPtr[i].TDs[j].osAppIdLen * sizeof(uint8_t));
                }
                // ipDescriptors
                copyString(&(tmpPtr[i].TDs[j].ipDescriptors.ipv4), curPtr[i].TDs[j].ipDescriptors.ipv4);
                copyString(&(tmpPtr[i].TDs[j].ipDescriptors.maskV4), curPtr[i].TDs[j].ipDescriptors.maskV4);
                copyString(&(tmpPtr[i].TDs[j].ipDescriptors.ipv6), curPtr[i].TDs[j].ipDescriptors.ipv6);
                tmpPtr[i].TDs[j].ipDescriptors.prefixLength = curPtr[i].TDs[j].ipDescriptors.prefixLength;
                tmpPtr[i].TDs[j].ipDescriptors.port = curPtr[i].TDs[j].ipDescriptors.port;
                tmpPtr[i].TDs[j].ipDescriptors.portStartRange = curPtr[i].TDs[j].ipDescriptors.portStartRange;
                tmpPtr[i].TDs[j].ipDescriptors.portEndRange = curPtr[i].TDs[j].ipDescriptors.portEndRange;
                tmpPtr[i].TDs[j].ipDescriptors.protocolId = curPtr[i].TDs[j].ipDescriptors.protocolId;
                // domainDescriptors
                copyString(&(tmpPtr[i].TDs[j].domainDescriptors), curPtr[i].TDs[j].domainDescriptors);
                // connectionCapabilities
                copyString(&(tmpPtr[i].TDs[j].connectionCapabilities), curPtr[i].TDs[j].connectionCapabilities);
            }
        }

        // Qos
        if (curPtr[i].defaultQos != NULL) {
            tmpPtr[i].defaultQos = (RIL_Qos *)calloc(1, sizeof(RIL_Qos));
            RFX_ASSERT(tmpPtr[i].defaultQos != NULL);
            copyQos(*(tmpPtr[i].defaultQos), *(curPtr[i].defaultQos), curPtr[i].accessNetwork);
        }

        // QosSession
        if (tmpPtr[i].qosSessionCount != 0 && curPtr[i].qosSessions != NULL) {
            tmpPtr[i].qosSessions = (RIL_QosSession *)calloc(1,
                    tmpPtr[i].qosSessionCount * sizeof(RIL_QosSession));
            RFX_ASSERT(tmpPtr[i].qosSessions != NULL);
            for (int k = 0; k < tmpPtr[i].qosSessionCount; k++) {
                copyQosSession(tmpPtr[i].qosSessions[k], curPtr[i].qosSessions[k],
                        curPtr[i].accessNetwork);
            }
        }
    }
    m_data = tmpPtr;
}

void RfxDataCallResponseData::copyQos(RIL_Qos &dst, const RIL_Qos &src, int accessNetwork) {
    RFX_LOG_D(RFX_LOG_TAG, "copyQos, accessNetwork:%d", accessNetwork);
    /* AOSP_V1_5::RadioAccessNetworks VS 3gpp 27.007 Sec 7.3
     * RadioAccessNetworks 0: UNKNOWN
     * RadioAccessNetworks 1: GERAN (0 GSM)
     * RadioAccessNetworks 2: UTRAN (2 UTRAN)
     * RadioAccessNetworks 3: EUTRAN(7 E-UTRAN)
     * RadioAccessNetworks 4: NGRAN (12 NG-RAN)*/
    if (accessNetwork == E_UTRAN) {
        // Qos
        dst.eps.qci = src.eps.qci;
        dst.eps.downlink.maxBitrateKbps = src.eps.downlink.maxBitrateKbps;
        dst.eps.downlink.guaranteedBitrateKbps = src.eps.downlink.guaranteedBitrateKbps;
        dst.eps.uplink.maxBitrateKbps = src.eps.uplink.maxBitrateKbps;
        dst.eps.uplink.guaranteedBitrateKbps = src.eps.uplink.guaranteedBitrateKbps;
    } else if (accessNetwork == NG_RAN){
        // Qos
        dst.nr.fiveQi = src.nr.fiveQi;
        dst.nr.downlink.maxBitrateKbps = src.nr.downlink.maxBitrateKbps;
        dst.nr.downlink.guaranteedBitrateKbps = src.nr.downlink.guaranteedBitrateKbps;
        dst.nr.uplink.maxBitrateKbps = src.nr.uplink.maxBitrateKbps;
        dst.nr.uplink.guaranteedBitrateKbps = src.nr.uplink.guaranteedBitrateKbps;
        dst.nr.qfi = src.nr.qfi;
        dst.nr.averagingWindowMs = src.nr.averagingWindowMs;
    }
}

void RfxDataCallResponseData::copyQosSession(RIL_QosSession &dst,
        const RIL_QosSession &src, int accessNetwork) {
    dst.qosSessionId = src.qosSessionId;
    copyQos(dst.qos, src.qos, accessNetwork);
    dst.qosFilterCount = src.qosFilterCount;
    if (dst.qosFilterCount <= 0 || src.qosFilters == NULL) {
        return;
    }
    dst.qosFilters = (RIL_QosFilter *)calloc(1, src.qosFilterCount * sizeof(RIL_QosFilter));
    RFX_ASSERT(dst.qosFilters != NULL);
    for (int i = 0; i < src.qosFilterCount; i++) {
        copyQosFilter(dst.qosFilters[i], src.qosFilters[i]);
    }
}

void RfxDataCallResponseData:: copyQosFilter(RIL_QosFilter &dst, const RIL_QosFilter &src) {
    copyString(&(dst.localAddresses), src.localAddresses);
    copyString(&(dst.remoteAddresses), src.remoteAddresses);
    dst.localPort.start  = src.localPort.start;
    dst.localPort.end    = src.localPort.end;
    dst.remotePort.start = src.remotePort.start;
    dst.remotePort.end   = src.remotePort.end;
    dst.protocol         = src.protocol;
    dst.typeOfService    = src.typeOfService;
    dst.ipv6FlowLabel    = src.ipv6FlowLabel;
    dst.ipSecSpi         = src.ipSecSpi;
    dst.direction        = src.direction;
    dst.precedence       = src.precedence;
}

void RfxDataCallResponseData::copyString(char **dst, char *src) {
    if (src != NULL) {
        *dst = (char *) calloc(strlen(src) + 1, sizeof(char));
        if (*dst == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        strncpy(*dst, src, strlen(src));
    } else {
        *dst = (char *) calloc(1, sizeof(char));
        if (*dst == NULL) {
            RFX_LOG_E(RFX_LOG_TAG, "OOM");
            return;
        }
        (*dst)[0] = 0;
    }
}
