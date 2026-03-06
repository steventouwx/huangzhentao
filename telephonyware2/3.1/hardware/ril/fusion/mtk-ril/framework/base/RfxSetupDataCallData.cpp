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

#include "RfxSetupDataCallData.h"
#include "RfxLog.h"

RFX_IMPLEMENT_DATA_CLASS(RfxSetupDataCallData);

#define RFX_LOG_TAG "RfxSetupDataCallData"

RfxSetupDataCallData::RfxSetupDataCallData(void *data, int length) : RfxBaseData(data, length) {
    m_length = length;
    RFX_ASSERT(m_length == sizeof(RIL_SetupDataCallRequest));

    RIL_SetupDataCallRequest *pTmp =
            (RIL_SetupDataCallRequest *) calloc(1, sizeof(RIL_SetupDataCallRequest));
    if (pTmp == NULL) {
        m_data = NULL;
        return;
    }
    RIL_SetupDataCallRequest *pCur = (RIL_SetupDataCallRequest *) data;

    pTmp->accessNetwork = pCur->accessNetwork;

    pTmp->dataProfileInfo.profileId = pCur->dataProfileInfo.profileId;
    copyString(&(pTmp->dataProfileInfo.apn), pCur->dataProfileInfo.apn);
    copyString(&(pTmp->dataProfileInfo.protocol), pCur->dataProfileInfo.protocol);
    copyString(&(pTmp->dataProfileInfo.roamingProtocol), pCur->dataProfileInfo.roamingProtocol);
    pTmp->dataProfileInfo.authType = pCur->dataProfileInfo.authType;
    copyString(&(pTmp->dataProfileInfo.user), pCur->dataProfileInfo.user);
    copyString(&(pTmp->dataProfileInfo.password), pCur->dataProfileInfo.password);
    pTmp->dataProfileInfo.type = pCur->dataProfileInfo.type;
    pTmp->dataProfileInfo.maxConnsTime = pCur->dataProfileInfo.maxConnsTime;
    pTmp->dataProfileInfo.maxConns = pCur->dataProfileInfo.maxConns;
    pTmp->dataProfileInfo.waitTime = pCur->dataProfileInfo.waitTime;
    pTmp->dataProfileInfo.enabled = pCur->dataProfileInfo.enabled;
    pTmp->dataProfileInfo.supportedTypesBitmask = pCur->dataProfileInfo.supportedTypesBitmask;
    pTmp->dataProfileInfo.bearerBitmask = pCur->dataProfileInfo.bearerBitmask;
    pTmp->dataProfileInfo.mtu = pCur->dataProfileInfo.mtu;
    pTmp->dataProfileInfo.mtuV6= pCur->dataProfileInfo.mtuV6;
    copyString(&(pTmp->dataProfileInfo.mvnoType), pCur->dataProfileInfo.mvnoType);
    copyString(&(pTmp->dataProfileInfo.mvnoMatchData), pCur->dataProfileInfo.mvnoMatchData);
    pTmp->dataProfileInfo.inactiveTimer = pCur->dataProfileInfo.inactiveTimer;

    pTmp->roamingAllowed = pCur->roamingAllowed;
    pTmp->reason = pCur->reason;
    pTmp->pduSessionId = pCur->pduSessionId;

    pTmp->sliceInfoValid = pCur->sliceInfoValid;
    if (pCur->sliceInfoValid) {
        pTmp->sliceInfo.sst = pCur->sliceInfo.sst;
        pTmp->sliceInfo.sliceDifferentiator = pCur->sliceInfo.sliceDifferentiator;
        pTmp->sliceInfo.mappedHplmnSst = pCur->sliceInfo.mappedHplmnSst;
        pTmp->sliceInfo.mappedHplmnSD = pCur->sliceInfo.mappedHplmnSD;
    }

    pTmp->tdValid = pCur->tdValid;
    if (pCur->tdValid) {
        copyString(&(pTmp->TD.dnn), pCur->TD.dnn);
        pTmp->TD.osAppIdLen = pCur->TD.osAppIdLen;
        pTmp->TD.osAppId = (uint8_t *)calloc(1, pCur->TD.osAppIdLen * sizeof(uint8_t) + 1);
        RFX_ASSERT(pTmp->TD.osAppId != NULL);
        memcpy(pTmp->TD.osAppId, pCur->TD.osAppId, pCur->TD.osAppIdLen * sizeof(uint8_t));
        // IP descriptors
        copyString(&(pTmp->TD.ipDescriptors.ipv4), pCur->TD.ipDescriptors.ipv4);
        copyString(&(pTmp->TD.ipDescriptors.maskV4), pCur->TD.ipDescriptors.maskV4);
        copyString(&(pTmp->TD.ipDescriptors.ipv6), pCur->TD.ipDescriptors.ipv6);
        pTmp->TD.ipDescriptors.prefixLength = pCur->TD.ipDescriptors.prefixLength;
        pTmp->TD.ipDescriptors.port = pCur->TD.ipDescriptors.port;
        pTmp->TD.ipDescriptors.portStartRange = pCur->TD.ipDescriptors.portStartRange;
        pTmp->TD.ipDescriptors.portEndRange = pCur->TD.ipDescriptors.portEndRange;
        pTmp->TD.ipDescriptors.protocolId = pCur->TD.ipDescriptors.protocolId;
        // domain descriptors
        copyString(&(pTmp->TD.domainDescriptors), pCur->TD.domainDescriptors);
        // connectionCapabilities
        copyString(&(pTmp->TD.connectionCapabilities), pCur->TD.connectionCapabilities);
    }
    pTmp->matchAllRuleAllowed= pCur->matchAllRuleAllowed;

    m_data = pTmp;
}

RfxSetupDataCallData::~RfxSetupDataCallData() {
    RIL_SetupDataCallRequest* pTmp = (RIL_SetupDataCallRequest *)m_data;
    // free memory
    if (pTmp != NULL) {
        FREEIF(pTmp->dataProfileInfo.apn);
        FREEIF(pTmp->dataProfileInfo.protocol);
        FREEIF(pTmp->dataProfileInfo.roamingProtocol);
        FREEIF(pTmp->dataProfileInfo.user);
        FREEIF(pTmp->dataProfileInfo.password);
        FREEIF(pTmp->dataProfileInfo.mvnoType);
        FREEIF(pTmp->dataProfileInfo.mvnoMatchData);
        if (pTmp->tdValid) {
            FREEIF(pTmp->TD.dnn);
            FREEIF(pTmp->TD.osAppId);
            FREEIF(pTmp->TD.ipDescriptors.ipv4);
            FREEIF(pTmp->TD.ipDescriptors.maskV4);
            FREEIF(pTmp->TD.ipDescriptors.ipv6);
            FREEIF(pTmp->TD.domainDescriptors);
            FREEIF(pTmp->TD.connectionCapabilities);
        }
        free(pTmp);
    }
}

void RfxSetupDataCallData::copyString(char **dst, char *src) {
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
