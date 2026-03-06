/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2022. All rights reserved.
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

#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <ril_service.h>
#include <rilAidlUtils.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <radiodata_service.h>
#include <libmtkrilutils.h>
#include <limits.h>

#include <binder/ProcessState.h>
#include <android/binder_manager.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <android/binder_libbinder.h>
#include <binder/RpcServer.h>
#include <ril_adapter/RadioDataAdapater.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio = ::aidl::android::hardware::radio;
using rilAidlUtils::retChecker;

struct RadioDataImpl {
    std::shared_ptr<aidl_mtk_radio_data::IMtkRadioExDataResponse> mtkDataResponse;
    std::shared_ptr<aidl_mtk_radio_data::IMtkRadioExDataIndication> mtkDataIndication;
    std::shared_ptr<aidl_radio_data::IRadioDataResponse> dataResponse;
    std::shared_ptr<aidl_radio_data::IRadioDataIndication> dataIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistRadioResponse> mtkAssistDataResponse;
};

RadioDataImpl radioDataServiceImpl[MAX_SIM_COUNT * (android::CLIENT_TBOX + 1)];

void convertRilKeepaliveStatusToAidl(RIL_KeepaliveStatus *rilStatus,
        aidl_radio_data::KeepaliveStatus& aidlKeepAliveStatus);
void convertCharPtrToVecLinkAddress(const char *ptr, std::vector<aidl_radio_data::LinkAddress> &records);
void convertCharPtrToVecAidlString(const char *ptr, std::vector<std::string> &records);
aidl_radio_data::Qos convertRilDataCallQosToAidl(const RIL_Qos *src, int accessNetwork);
void convertRilDataCallQosSessionsToHal(std::vector<aidl_radio_data::QosSession> &dst,
        MTK_RIL_Data_Call_Response_v11 *dcResponse);
void convertRilDataCallListToHal(const void *response, size_t responseLen,
        std::vector<aidl_radio_data::SetupDataCallResult>& dcResultList, unsigned int slotId);
void convertRilDataCallToHal(MTK_RIL_Data_Call_Response_v11 *dcResponse,
        aidl_radio_data::SetupDataCallResult& dcResult, unsigned int slotId);
void convertRilSlicingConfigToHal(const void *response, size_t responseLen,
        aidl_radio_data::SlicingConfig& records);


static char* convertPdpProtocolTypeToString(aidl_radio_data::PdpProtocolType pdpType) {
    switch (pdpType) {
        case aidl_radio_data::PdpProtocolType::IP:
            return (char*)"IP";
        case aidl_radio_data::PdpProtocolType::IPV6:
            return (char*)"IPV6";
        case aidl_radio_data::PdpProtocolType::IPV4V6:
            return (char*)"IPV4V6";
        case aidl_radio_data::PdpProtocolType::PPP:
            return (char*)"PPP";
        default:
            return (char*)"IP";
    }
}

static aidl_radio_data::PdpProtocolType convertStringToPdpProtocolType(const char *ptr) {
    if (ptr != NULL) {
        if (0 == strcasecmp(ptr, "IP")) {
            return aidl_radio_data::PdpProtocolType::IP;
        } else if (0 == strcasecmp(ptr, "IPV4V6")) {
            return aidl_radio_data::PdpProtocolType::IPV4V6;
        } else if (0 == strcasecmp(ptr, "IPV6")) {
            return aidl_radio_data::PdpProtocolType::IPV6;
        } else if (0 == strcasecmp(ptr, "PPP")) {
            return aidl_radio_data::PdpProtocolType::PPP;
        }
    }
    return aidl_radio_data::PdpProtocolType::IP;
}

static unsigned int convertAuthTypeToInt(aidl_radio_data::ApnAuthType authType) {
    switch (authType) {
        case aidl_radio_data::ApnAuthType::NO_PAP_NO_CHAP:
            return 0;
        case aidl_radio_data::ApnAuthType::PAP_NO_CHAP:
            return 1;
        case aidl_radio_data::ApnAuthType::NO_PAP_CHAP:
            return 2;
        case aidl_radio_data::ApnAuthType::PAP_CHAP:
            return 3;
        default:
            return 0;
    }
}

static long stringToLong(char *str, int base, int *err) {
    long out;
    unsigned long ul;
    *err = 0;

    if (str == NULL) {
        *err = -1;
        return 0;
    }

    ul = strtoul(str, NULL, base);
    if (ul == ULONG_MAX) {
        mtkLogE(LOG_TAG, "stringToLong: error on strtoul");
        *err = -2;
        return 0;
    }
    out = (long)ul;

    return out;
}

void convertRilKeepaliveStatusToAidl(RIL_KeepaliveStatus *rilStatus,
        aidl_radio_data::KeepaliveStatus& aidlKeepAliveStatus) {
    aidlKeepAliveStatus.sessionHandle = (int) (rilStatus->sessionHandle);
    aidlKeepAliveStatus.code = (int)(rilStatus->code);
}

void convertCharPtrToVecLinkAddress(const char *ptr, std::vector<aidl_radio_data::LinkAddress> &records) {
    if (ptr == NULL || strlen(ptr) <= 0) {
        return;
    }

    int nSize = 0;
    char *token = NULL;
    const char *delim = " ";
    int i = 0;
    char *tmp = NULL;
    char *saveptr = NULL;

    if (asprintf(&tmp, "%s", ptr) < 0) {
        mtkLogE(LOG_TAG, "%s: asprintf error!", __FUNCTION__);
        return;
    }

    token = strstr(tmp, delim);
    while (token != NULL) {
        nSize++;
        token = strstr(token+1, delim);
    }
    nSize++;

    records.resize(nSize);

    token = strtok_r(tmp, delim, &saveptr);
    while (token != NULL && i >= 0 && i < nSize) {
        aidl_radio_data::LinkAddress linkAddress;
        char *found;
        int cnt = 0;
        int err = 0;
        long exts[3] = {0,LONG_MAX,LONG_MAX};
        while ((found = strsep(&token,"&")) != NULL) {
            mtkLogD(LOG_TAG, "%s: cnt=%d, found=%s, token=%s", __FUNCTION__,
                    cnt, isUserLoad() ? "xxx" : found, token);
            if (cnt == 0) {
                linkAddress.address = convertCharPtrToStdString(found);
            } else if (cnt > 0 && cnt <= 3) {
                exts[cnt-1] = stringToLong(found, 10, &err);
                if (err < 0) {
                    exts[cnt-1] = -1;
                    mtkLogE(LOG_TAG, "%s: err=%d, cnt=%d", __FUNCTION__,
                            err, cnt);
                }
            }
            cnt++;
        }
        mtkLogD(LOG_TAG, "%s: extra[0]=%ld, extra[1]=%ld, extra[2]=%ld", __FUNCTION__,
                exts[0], exts[1], exts[2]);
        linkAddress.addressProperties = exts[0];
        linkAddress.deprecationTime = exts[1];
        linkAddress.expirationTime = exts[2];
        records[i++] = linkAddress;
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(tmp);
}

void convertCharPtrToVecAidlString(const char *ptr, std::vector<std::string> &records) {
    if (ptr == NULL || strlen(ptr) <= 0) {
        return;
    }

    int nSize = 0;
    char *token = NULL;
    const char *delim = " ";
    int i = 0;
    char *tmp = NULL;
    char *saveptr = NULL;

    if (asprintf(&tmp, "%s", ptr) < 0) {
        mtkLogE(LOG_TAG, "%s: asprintf error!", __FUNCTION__);
        return;
    }

    token = strstr(tmp, delim);
    while (token != NULL) {
        nSize++;
        token = strstr(token+1, delim);
    }
    nSize++;

    records.resize(nSize);
    token = strtok_r(tmp, delim, &saveptr);
    while (token != NULL && i >= 0 && i < nSize) {
        records[i++] = convertCharPtrToStdString(token);
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(tmp);
}

aidl_radio_data::Qos convertRilDataCallQosToAidl(const RIL_Qos *src, int accessNetwork) {
    /* aidl_radio::AccessNetworks VS 3gpp 27.007 Sec 7.3
     * AccessNetworks 0: UNKNOWN
     * AccessNetworks 1: GERAN (0 GSM)
     * AccessNetworks 2: UTRAN (2 UTRAN)
     * AccessNetworks 3: EUTRAN(7 E-UTRAN)
     * AccessNetworks 6: NGRAN (12 NG-RAN), Currently RILD takes AOSP_V1_5::RadioAccessNetworks 4: as NGRAN,
                                            it is equals aidl_radio::AccessNetworks::CDMA2000
     */
    if (accessNetwork == (int) aidl_radio::AccessNetwork::EUTRAN) {
        aidl_radio_data::EpsQos tmpEpsQos = {};
        tmpEpsQos.qci = src->eps.qci;
        tmpEpsQos.downlink.maxBitrateKbps = src->eps.downlink.maxBitrateKbps;
        tmpEpsQos.downlink.guaranteedBitrateKbps = src->eps.downlink.guaranteedBitrateKbps;
        tmpEpsQos.uplink.maxBitrateKbps = src->eps.uplink.maxBitrateKbps;
        tmpEpsQos.uplink.guaranteedBitrateKbps = src->eps.uplink.guaranteedBitrateKbps;
        return aidl_radio_data::Qos(tmpEpsQos);
    } else if (accessNetwork == (int) aidl_radio::AccessNetwork::CDMA2000){
        aidl_radio_data::NrQos tmpNrQos = {};
        tmpNrQos.fiveQi = src->nr.fiveQi;
        tmpNrQos.downlink.maxBitrateKbps = src->nr.downlink.maxBitrateKbps;
        tmpNrQos.downlink.guaranteedBitrateKbps = src->nr.downlink.guaranteedBitrateKbps;
        tmpNrQos.uplink.maxBitrateKbps = src->nr.uplink.maxBitrateKbps;
        tmpNrQos.uplink.guaranteedBitrateKbps = src->nr.uplink.guaranteedBitrateKbps;
        tmpNrQos.qfi = (uint8_t) src->nr.qfi;
        tmpNrQos.averagingWindowMillis = src->nr.averagingWindowMs;
        return aidl_radio_data::Qos(tmpNrQos);
    } else {
        aidl_radio_data::Qos noinit = {};
        return noinit;
    }
}

void convertRilDataCallQosSessionsToHal(std::vector<aidl_radio_data::QosSession> &dst,
        MTK_RIL_Data_Call_Response_v11 *dcResponse) {
    int qosFilterCount = 0;
    RIL_QosFilter* pQf = NULL;
    int qosSessionCount = dcResponse->qosSessionCount;
    dst.resize(qosSessionCount);
    for (int i = 0; i < qosSessionCount; i++) {
        aidl_radio_data::QosSession* qs = &dst[i];
        qs->qosSessionId = dcResponse->qosSessions[i].qosSessionId;
        qs->qos = convertRilDataCallQosToAidl(&(dcResponse->qosSessions[i].qos),
                dcResponse->accessNetwork);
        qosFilterCount = dcResponse->qosSessions[i].qosFilterCount;
        qs->qosFilters.resize(qosFilterCount);
        for (int j = 0; j < qosFilterCount; j++) {
            aidl_radio_data::QosFilter* qf = &qs->qosFilters[j];
            pQf = &(dcResponse->qosSessions[i].qosFilters[j]);
            convertCharPtrToVecAidlString(pQf->localAddresses, qf->localAddresses);
            convertCharPtrToVecAidlString(pQf->remoteAddresses, qf->remoteAddresses);

            // QosPortRange: 20~65535
            if (pQf->localPort.start != 0 || pQf->localPort.end != 0) {
                qf->localPort = {};
                qf->localPort->start = pQf->localPort.start;
                qf->localPort->end = pQf->localPort.end;
            }
            if (pQf->remotePort.start != 0 || pQf->remotePort.end != 0) {
                qf->remotePort = {};
                qf->remotePort->start = pQf->remotePort.start;
                qf->remotePort->end = pQf->remotePort.end;
            }

            qf->protocol = (uint8_t) pQf->protocol;

            if (pQf->typeOfService >= 0) {
                qf->tos = aidl_radio_data::QosFilterTypeOfService(
                        (int8_t) pQf->typeOfService);
            }

            if (pQf->ipv6FlowLabel >= 0) {
                qf->flowLabel = aidl_radio_data::QosFilterIpv6FlowLabel(
                        (int32_t) pQf->ipv6FlowLabel);
            }

            if (pQf->ipSecSpi >= 0) {
                qf->spi = aidl_radio_data::QosFilterIpsecSpi((int32_t) pQf->ipSecSpi);
            }

            qf->direction = (uint8_t) pQf->direction;
            qf->precedence = pQf->precedence;
        }
    }
}

void convertRilDataCallListToHal(const void *response, size_t responseLen,
        std::vector<aidl_radio_data::SetupDataCallResult>& dcResultList, unsigned int slotId) {
    int num = responseLen / sizeof(MTK_RIL_Data_Call_Response_v11);

    MTK_RIL_Data_Call_Response_v11 *dcResponse = (MTK_RIL_Data_Call_Response_v11 *) response;
    dcResultList.resize(num);
    android::ClientId client = getClientBySlot(slotId);
    for (int i = 0; i < num; i++) {
        convertRilDataCallToHal(&dcResponse[i], dcResultList[i], slotId);
    }
}

void convertRilDataCallToHal(MTK_RIL_Data_Call_Response_v11 *dcResponse,
        aidl_radio_data::SetupDataCallResult& dcResult, unsigned int slotId) {
    int i = 0;
    dcResult.cause = (aidl_radio_data::DataCallFailCause) dcResponse->status;
    dcResult.suggestedRetryTime = dcResponse->suggestedRetryTime;
    dcResult.cid = dcResponse->cid;
    dcResult.active = dcResponse->active;
    dcResult.type = convertStringToPdpProtocolType(dcResponse->type);
    dcResult.ifname = convertCharPtrToStdString(dcResponse->ifname);
    convertCharPtrToVecLinkAddress(dcResponse->addresses, dcResult.addresses);
    convertCharPtrToVecAidlString(dcResponse->dnses, dcResult.dnses);
    convertCharPtrToVecAidlString(dcResponse->gateways, dcResult.gateways);
    convertCharPtrToVecAidlString(dcResponse->pcscf, dcResult.pcscf);
    dcResult.mtuV4 = dcResponse->mtu;
    dcResult.mtuV6 = dcResponse->mtuV6;

    mtkLogD(LOG_TAG, "%s, input response: cause=%d,suggestedRetryTime=%d,cid=%d,active=%d",
        __FUNCTION__, dcResponse->status, dcResponse->suggestedRetryTime, dcResponse->cid, dcResponse->active);
    mtkLogD(LOG_TAG, "%s, input response: type=%s,ifname=%s,addresses=%s,dnses=%s,gateways=%s,pcscf=%s,mtu=%d,mtuV6=%d",
        __FUNCTION__, dcResponse->type, dcResponse->ifname, dcResponse->addresses, dcResponse->dnses, dcResponse->gateways, dcResponse->pcscf, dcResponse->mtu, dcResponse->mtuV6);

    // defaultQos
    if (dcResponse->defaultQos != NULL) {
        dcResult.defaultQos = convertRilDataCallQosToAidl(dcResponse->defaultQos,
            dcResponse->accessNetwork);
    }

    // qosSessions
    convertRilDataCallQosSessionsToHal(dcResult.qosSessions, dcResponse);
    dcResult.handoverFailureMode = (uint8_t) dcResponse->handoverFailureMode;
    dcResult.pduSessionId = dcResponse->pduSessionId;

    // SliceInfo
    if (dcResponse->optionalSliceInfo == NULL) {
        dcResult.sliceInfo = {};
    } else {
        aidl_radio_data::SliceInfo sliceInfo;
        sliceInfo.sliceServiceType = (uint8_t) dcResponse->optionalSliceInfo->sst;
        sliceInfo.sliceDifferentiator = dcResponse->optionalSliceInfo->sliceDifferentiator;
        sliceInfo.mappedHplmnSst =
                (uint8_t) dcResponse->optionalSliceInfo->mappedHplmnSst;
        sliceInfo.mappedHplmnSd = dcResponse->optionalSliceInfo->mappedHplmnSD;
        sliceInfo.status = (uint8_t) dcResponse->optionalSliceInfo->status;
        dcResult.sliceInfo = sliceInfo;
    }

    // Traffic Descriptor
    if (dcResponse->tdCount > 0) {
        dcResult.trafficDescriptors.resize(dcResponse->tdCount);
        int validSize = 0;
        for (i = 0; i < dcResponse->tdCount; i++) {
            aidl_radio_data::TrafficDescriptor* td = &(dcResult.trafficDescriptors[validSize]);
            // Adjust td count.
            // Since current android version only care about DNN and OS APP ID of a TD,
            // if a TD has no DNN and OS APP ID, ignore it.
            if ((dcResponse->TDs[i].dnn == NULL || strlen(dcResponse->TDs[i].dnn) == 0) &&
                    dcResponse->TDs[i].osAppIdLen <= 0) {
                continue;
            }
            validSize++;
            if (dcResponse->TDs[i].dnn == NULL){
                td->dnn = {};
            } else {
                td->dnn = convertCharPtrToStdString(dcResponse->TDs[i].dnn);
            }
            if (dcResponse->TDs[i].osAppIdLen <= 0 || dcResponse->TDs[i].osAppId == NULL){
                td->osAppId = {};
            } else {
                aidl_radio_data::OsAppId aOsAppId;
                aOsAppId.osAppId.resize(dcResponse->TDs[i].osAppIdLen);
                for (int j = 0; j < dcResponse->TDs[i].osAppIdLen; j++) {
                    aOsAppId.osAppId[j] = (uint8_t) dcResponse->TDs[i].osAppId[j];
                }
                td->osAppId = aOsAppId;
            }
        }
        if (validSize != dcResponse->tdCount) {
            dcResult.trafficDescriptors.resize(validSize);
        }
    }
}

// mainly copy from convertRilDataCallToHal, currently the only difference is dcResult,
// its member TafficDescriptor is customized version with more parameters.
// see the defination of SetupDataCallResultSlice for more details.
void convertRilDataCallSliceToHal(MTK_RIL_Data_Call_Response_v11 *dcResponse,
        aidl_mtk_radio_data::SetupDataCallResultSlice& dcResult, unsigned int slotId) {
    int i = 0;
    dcResult.cause = (aidl_radio_data::DataCallFailCause) dcResponse->status;
    dcResult.suggestedRetryTime = dcResponse->suggestedRetryTime;
    dcResult.cid = dcResponse->cid;
    dcResult.active = dcResponse->active;
    dcResult.type = convertStringToPdpProtocolType(dcResponse->type);
    dcResult.ifname = convertCharPtrToStdString(dcResponse->ifname);
    convertCharPtrToVecLinkAddress(dcResponse->addresses, dcResult.addresses);
    convertCharPtrToVecAidlString(dcResponse->dnses, dcResult.dnses);
    convertCharPtrToVecAidlString(dcResponse->gateways, dcResult.gateways);
    convertCharPtrToVecAidlString(dcResponse->pcscf, dcResult.pcscf);
    dcResult.mtuV4 = dcResponse->mtu;
    dcResult.mtuV6 = dcResponse->mtuV6;

    // defaultQos
    if (dcResponse->defaultQos != NULL) {
        dcResult.defaultQos = convertRilDataCallQosToAidl(dcResponse->defaultQos,
            dcResponse->accessNetwork);
    }

    // qosSessions
    convertRilDataCallQosSessionsToHal(dcResult.qosSessions, dcResponse);
    dcResult.handoverFailureMode = (uint8_t) dcResponse->handoverFailureMode;
    dcResult.pduSessionId = dcResponse->pduSessionId;

    // SliceInfo
    if (dcResponse->optionalSliceInfo == NULL) {
        dcResult.sliceInfo = {};
    } else {
        aidl_radio_data::SliceInfo sliceInfo;
        sliceInfo.sliceServiceType = (uint8_t) dcResponse->optionalSliceInfo->sst;
        sliceInfo.sliceDifferentiator = dcResponse->optionalSliceInfo->sliceDifferentiator;
        sliceInfo.mappedHplmnSst =
                (uint8_t) dcResponse->optionalSliceInfo->mappedHplmnSst;
        sliceInfo.mappedHplmnSd = dcResponse->optionalSliceInfo->mappedHplmnSD;
        sliceInfo.status = (uint8_t) dcResponse->optionalSliceInfo->status;
        dcResult.sliceInfo = sliceInfo;
    }

    // Traffic Descriptor
    if (dcResponse->tdCount > 0) {
        dcResult.trafficDescriptors.resize(dcResponse->tdCount);
        for (i = 0; i < dcResponse->tdCount; i++) {
            aidl_mtk_radio_data::TrafficDescriptor* td = &(dcResult.trafficDescriptors[i]);
            if (dcResponse->TDs[i].dnn == NULL) {
                td->dnn = {};
            } else {
                td->dnn = convertCharPtrToStdString(dcResponse->TDs[i].dnn);
            }
            if (dcResponse->TDs[i].osAppIdLen <= 0 || dcResponse->TDs[i].osAppId == NULL) {
                td->osAppId = {};
            } else {
                aidl_radio_data::OsAppId aOsAppId;
                aOsAppId.osAppId.resize(dcResponse->TDs[i].osAppIdLen);
                for (int j = 0; j < dcResponse->TDs[i].osAppIdLen; j++) {
                    aOsAppId.osAppId[j] = (uint8_t) dcResponse->TDs[i].osAppId[j];
                }
                td->osAppId = aOsAppId;
            }
            // ipDescriptors
            aidl_mtk_radio_data::IpDescriptors ipDescriptors;
            if (dcResponse->TDs[i].ipDescriptors.ipv4 == NULL) {
                ipDescriptors.ipv4 = {};
            } else {
                ipDescriptors.ipv4 = convertCharPtrToStdString(dcResponse->TDs[i].ipDescriptors.ipv4);
            }
            if (dcResponse->TDs[i].ipDescriptors.maskV4 == NULL) {
                ipDescriptors.maskV4 = {};
            } else {
                ipDescriptors.maskV4 = convertCharPtrToStdString(dcResponse->TDs[i].ipDescriptors.maskV4);
            }
            if (dcResponse->TDs[i].ipDescriptors.ipv6 == NULL) {
                ipDescriptors.ipv6 = {};
            } else {
                ipDescriptors.ipv6 = convertCharPtrToStdString(dcResponse->TDs[i].ipDescriptors.ipv6);
            }
            ipDescriptors.prefixLength = dcResponse->TDs[i].ipDescriptors.prefixLength;
            ipDescriptors.port = dcResponse->TDs[i].ipDescriptors.port;
            ipDescriptors.portStartRange = dcResponse->TDs[i].ipDescriptors.portStartRange;
            ipDescriptors.portEndRange = dcResponse->TDs[i].ipDescriptors.portEndRange;
            ipDescriptors.protocolId = dcResponse->TDs[i].ipDescriptors.protocolId;
            td->ipDescriptors = ipDescriptors;
            // domainDescriptors
            if (dcResponse->TDs[i].domainDescriptors == NULL) {
                td->domainDescriptors = {};
            } else {
                td->domainDescriptors = convertCharPtrToStdString(dcResponse->TDs[i].domainDescriptors);
            }
            // connectionCapabilities
            if (dcResponse->TDs[i].connectionCapabilities == NULL) {
                td->connectionCapabilities = {};
            } else {
                td->connectionCapabilities = convertCharPtrToStdString(dcResponse->TDs[i].connectionCapabilities);
            }
        }
    }
}

void convertRilSlicingConfigToHal(const void *response, size_t responseLen,
        aidl_radio_data::SlicingConfig& records) {
    if (response == NULL || responseLen == 0) {
        return;
    }

    RIL_SlicingConfig *pData = (RIL_SlicingConfig*)response;
    mtkLogD(LOG_TAG, "[%s] ruleCount:%d, sliceCount: %d", __FUNCTION__,
            pData->ruleCount, pData->sliceCount);
    if (pData->ruleCount > 0) {
        records.urspRules.resize(pData->ruleCount);
        for (int i = 0; i < pData->ruleCount; i++) {
            records.urspRules[i].precedence = pData->urspRules[i].precedence;
            // TD
            if (pData->urspRules[i].tdCount > 0) {
                records.urspRules[i].trafficDescriptors.resize(pData->urspRules[i].tdCount);
                for (int j = 0; j < pData->urspRules[i].tdCount; j++) {
                    // dnn
                    if (pData->urspRules[i].trafficDescriptors[j].dnn != NULL) {
                        records.urspRules[i].trafficDescriptors[j].dnn =
                                convertCharPtrToStdString(
                                        pData->urspRules[i].trafficDescriptors[j].dnn);
                    }
                    // os app id
                    if (pData->urspRules[i].trafficDescriptors[j].osAppId != NULL ||
                            pData->urspRules[i].trafficDescriptors[j].osAppIdLen > 0) {
                        int len = pData->urspRules[i].trafficDescriptors[j].osAppIdLen;
                        aidl_radio_data::OsAppId aOsAppId;
                        aOsAppId.osAppId.resize(len);
                        for (int k = 0; k < len; k++) {
                            aOsAppId.osAppId[k]
                                    = pData->urspRules[i].trafficDescriptors[j].osAppId[k];
                        }
                        records.urspRules[i].trafficDescriptors[j].osAppId = aOsAppId;
                    }
                    mtkLogD(LOG_TAG, "[%s] rule[%d], precedence: %d, td[%d]: {%s,}", __FUNCTION__,
                            i, pData->urspRules[i].precedence, j,
                            pData->urspRules[i].trafficDescriptors[j].dnn == NULL ? "" :
                            pData->urspRules[i].trafficDescriptors[j].dnn);
                }
            }
            // RSD
            if (pData->urspRules[i].rsdCount > 0) {
                records.urspRules[i].routeSelectionDescriptor.resize(pData->urspRules[i].rsdCount);
                for (int j = 0; j < pData->urspRules[i].rsdCount; j++) {
                    // precedence
                    records.urspRules[i].routeSelectionDescriptor[j].precedence =
                            (uint8_t) pData->urspRules[i].routeSelectionDescriptor[j].precedence;
                    // sessionType
                    if (pData->urspRules[i].routeSelectionDescriptor[j].sessionType == NULL) {
                        records.urspRules[i].routeSelectionDescriptor[j].sessionType =
                            aidl_radio_data::PdpProtocolType::IP;
                    } else {
                        records.urspRules[i].routeSelectionDescriptor[j].sessionType =
                                convertStringToPdpProtocolType(
                                pData->urspRules[i].routeSelectionDescriptor[j].sessionType);
                    }
                    // sscMode
                    records.urspRules[i].routeSelectionDescriptor[j].sscMode =
                            (uint8_t) pData->urspRules[i].routeSelectionDescriptor[j].sscMode;
                    // sliceInfo
                    if (pData->urspRules[i].routeSelectionDescriptor[j].sliceCount > 0) {
                        records.urspRules[i].routeSelectionDescriptor[j].sliceInfo.resize(
                                pData->urspRules[i].routeSelectionDescriptor[j].sliceCount);
                        for (int k = 0;
                                k < pData->urspRules[i].routeSelectionDescriptor[j].sliceCount;
                                k++) {
                            records.urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].sliceServiceType =
                                    (uint8_t) pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].sst;
                            records.urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].sliceDifferentiator =
                                    pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].sliceDifferentiator;
                            records.urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].mappedHplmnSst =
                                    (uint8_t) pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].mappedHplmnSst;
                            records.urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].mappedHplmnSd =
                                    pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].mappedHplmnSD;
                            records.urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].status =
                                    (uint8_t) pData->urspRules[i].routeSelectionDescriptor[j].sliceInfo[k].status;
                        }
                    }
                    // dnn
                    if (pData->urspRules[i].routeSelectionDescriptor[j].dnnCount > 0) {
                        records.urspRules[i].routeSelectionDescriptor[j].dnn.resize(
                                pData->urspRules[i].routeSelectionDescriptor[j].dnnCount);
                        for (int k = 0;
                                k < pData->urspRules[i].routeSelectionDescriptor[j].dnnCount;
                                k++) {
                            records.urspRules[i].routeSelectionDescriptor[j].dnn[k] =
                                    convertCharPtrToStdString(
                                            pData->urspRules[i].routeSelectionDescriptor[j].dnn[k]);
                        }
                    }
                    mtkLogD(LOG_TAG, "[%s] rule[%d], precedence: %d, rsd[%d]: {%d,%d,%d}",
                            __FUNCTION__, i, pData->urspRules[i].precedence, j,
                            pData->urspRules[i].routeSelectionDescriptor[j].precedence,
                            convertStringToPdpProtocolType(
                                pData->urspRules[i].routeSelectionDescriptor[j].sessionType),
                            pData->urspRules[i].routeSelectionDescriptor[j].sscMode);
                }
            }
        }
    }
    if (pData->sliceCount > 0) {
        records.sliceInfo.resize(pData->sliceCount);
        for (int i = 0; i < pData->sliceCount; i++) {
            records.sliceInfo[i].sliceServiceType = (uint8_t) pData->sliceInfo[i].sst;
            records.sliceInfo[i].sliceDifferentiator = pData->sliceInfo[i].sliceDifferentiator;
            records.sliceInfo[i].mappedHplmnSst = (uint8_t) pData->sliceInfo[i].mappedHplmnSst;
            records.sliceInfo[i].mappedHplmnSd = pData->sliceInfo[i].mappedHplmnSD;
            records.sliceInfo[i].status = (uint8_t)pData->sliceInfo[i].status;
            mtkLogD(LOG_TAG, "[%s] sliceInfo[%d]: {%d,%d,%d,%d,%d}", __FUNCTION__, i,
                    pData->sliceInfo[i].sst, pData->sliceInfo[i].sliceDifferentiator,
                    pData->sliceInfo[i].mappedHplmnSst, pData->sliceInfo[i].mappedHplmnSD,
                    pData->sliceInfo[i].status);
        }
    }
}

/**
 * @param numProfiles number of data profile
 * @param dataProfiles the pointer to the actual data profiles. The acceptable type is
          RIL_DataProfileInfo or RIL_DataProfileInfo_v15.
 * @param dataProfilePtrs the pointer to the pointers that point to each data profile structure
 * @param numfields number of string-type member in the data profile structure
 * @param ... the variadic parameters are pointers to each string-type member
 **/
template <typename T>
void freeSetDataProfileData(int numProfiles, T *dataProfiles, T **dataProfilePtrs,
                            int numfields, ...) {
    va_list args;
    va_start(args, numfields);

    // Iterate through each string-type field that need to be free.
    for (int i = 0; i < numfields; i++) {
        // Iterate through each data profile and free that specific string-type field.
        // The type 'char *T::*' is a type of pointer to a 'char *' member inside T structure.
        char *T::*ptr = va_arg(args, char *T::*);
        for (int j = 0; j < numProfiles; j++) {
            memsetAndFreeStrings(1, dataProfiles[j].*ptr);
        }
    }

    va_end(args);

#ifdef MEMSET_FREED
    memset(dataProfiles, 0, numProfiles * sizeof(T));
    memset(dataProfilePtrs, 0, numProfiles * sizeof(T *));
#endif
    free(dataProfiles);
    free(dataProfilePtrs);
}

///aosp radio
///response
int radioData::allocatePduSessionIdResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void* response, size_t responseLen) {
    mtkLogD(LOG_TAG, "allocatePduSessionIdResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->allocatePduSessionIdResponse(
                responseInfo, 0);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "allocatePduSessionIdResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "allocatePduSessionIdResponse:"
                "radioDataServiceImpl[%d]->dataResponse == NULL", slotId);
    }
    return 0;
}

int radioData::cancelHandoverResponse(unsigned int slotId, android::ClientId clientId, int responseType,
        int serial, RIL_Errno e, const void* response, size_t responseLen) {
    mtkLogD(LOG_TAG, "cancelHandoverResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->cancelHandoverResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cancelHandoverResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "cancelHandoverResponse: radioDataServiceImpl[%d]->dataResponse == NULL",
                slotId);
    }
    return 0;
}

int radioData::deactivateDataCallResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->deactivateDataCallResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int radioData::getDataCallListResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e,
                            const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);
    if (radioDataServiceImpl[slotId].dataResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    /// M: For VTS purpose @{
//    if (s_cardState[slotId] == aidl_radio::sim::CardStatus::STATE_ABSENT) {
//        responseInfo.error = aidl_radio::RadioError::NONE;
//    }
    /// @}

    std::vector<aidl_radio_data::SetupDataCallResult> result;
    if (response == NULL || responseLen % sizeof(MTK_RIL_Data_Call_Response_v11) != 0) {
        mtkLogE(LOG_TAG, "%s: invalid response", __FUNCTION__);
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        convertRilDataCallListToHal(response, responseLen, result, slotId);
    }

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    RadioDataAdapater::getDataCallListResponseEx(static_cast<void*>(result.data()), result.size(), slotId);
#endif

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->getDataCallListResponse(
            responseInfo, result);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int radioData::getSlicingConfigResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);
    if (radioDataServiceImpl[slotId].dataResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    aidl_radio_data::SlicingConfig slicingConfig = {};
    if (response == NULL || responselen != sizeof(RIL_SlicingConfig)) {
        mtkLogE(LOG_TAG, "%s Invalid response %p, len: %d", __FUNCTION__, response, responselen);
    } else {
        convertRilSlicingConfigToHal(response, responselen, slicingConfig);
    }

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->getSlicingConfigResponse(responseInfo,
            slicingConfig);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int radioData::releasePduSessionIdResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void* response, size_t responseLen) {
    mtkLogD(LOG_TAG, "releasePduSessionIdResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->releasePduSessionIdResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "releasePduSessionIdResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "releasePduSessionIdResponse: radioDataServiceImpl[%d]->dataResponse == NULL",
                slotId);
    }
    return 0;
}

int radioData::setDataAllowedResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setDataAllowedResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->setDataAllowedResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setDataAllowedResponse: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setDataAllowedResponse: "
                "radioDataServiceImpl[%d]->dataResponse == NULL", slotId);
    }
    return 0;
}

int radioData::setDataProfileResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setDataProfileResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->setDataProfileResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setDataProfileResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setDataProfileResponse: radioDataServiceImpl[%d]->dataResponse == NULL",
                slotId);
    }
    return 0;
}

int radioData::setDataThrottlingResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void* response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setDataThrottlingResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->setDataThrottlingResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setDataThrottlingResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setDataThrottlingResponse: "
                "radioDataServiceImpl[%d]->dataResponse == NULL", slotId);
    }
    return 0;
}

int radioData::setInitialAttachApnResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setInitialAttachApnResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->setInitialAttachApnResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setInitialAttachApnResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setInitialAttachApnResponse:"
                "radioDataServiceImpl[%d]->dataResponse == NULL", slotId);
    }
    return 0;
}

int radioData::setupDataCallResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e, const void *response,
                          size_t responseLen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    /// M: For VTS purpose @{
//    if (s_cardState[slotId] == aidl_radio::sim::CardStatus::STATE_ABSENT) {
//        responseInfo.error = aidl_radio::RadioError::NONE;
//    }
    /// @}
    aidl_radio_data::SetupDataCallResult result = {};

    if (response == NULL || responseLen != sizeof(MTK_RIL_Data_Call_Response_v11)) {
        mtkLogE(LOG_TAG, "%s: Invalid response", __FUNCTION__);
        if (e == RIL_E_SUCCESS) {
            responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }
        result.cause = aidl_radio_data::DataCallFailCause::ERROR_UNSPECIFIED;
        result.suggestedRetryTime = -1;
        result.cid = -1;
        result.active = aidl_radio_data::SetupDataCallResult::DATA_CONNECTION_STATUS_INACTIVE;
        result.type = aidl_radio_data::PdpProtocolType::UNKNOWN;
        //result.ifname = hidl_string();
        //result.addresses = hidl_vec<AOSP_V1_5::LinkAddress>();
        //result.dnses = hidl_vec<hidl_string>();
        //result.gateways = hidl_vec<hidl_string>();
        //result.pcscf = hidl_vec<hidl_string>();
        result.mtuV4 = 0;
        result.mtuV6 = 0;
    } else {
        convertRilDataCallToHal((MTK_RIL_Data_Call_Response_v11 *) response,
                result, slotId);
    }

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    RadioDataAdapater::setupDataCallResponseEx(serial, &result, responseType, SETUP_DATA_CALL_RESPONSE_AOSP, slotId);
#endif

    if (radioDataServiceImpl[slotId].dataResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->setupDataCallResponse(
            responseInfo, result);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }

    return 0;
}

int radioData::startHandoverResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void* response, size_t responseLen) {
    mtkLogD(LOG_TAG, "startHandoverResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->startHandoverResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "startHandoverResponse: slotId:%d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "startHandoverResponse: radioDataServiceImpl[%d]->dataResponse == NULL",
                slotId);
    }
    return 0;
}


int radioData::startKeepaliveResponse(unsigned int slotId, android::ClientId clientId,
                           int responseType, int serial, RIL_Errno e,
                           const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    aidl_radio_data::KeepaliveStatus ks = {};
    if (response == NULL || responselen != sizeof(RIL_KeepaliveStatus)) {
        mtkLogE(LOG_TAG, "%s: invalid response - %d", __FUNCTION__, static_cast<int>(e));
        if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
    } else {
        convertRilKeepaliveStatusToAidl((RIL_KeepaliveStatus*)(response), ks);
    }

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->startKeepaliveResponse(
            responseInfo, ks);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

int radioData::stopKeepaliveResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e,
                          const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    if (radioDataServiceImpl[slotId].dataResponse == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->stopKeepaliveResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
        retChecker->aospAidlReturnStatusChecker(slotId, ret);
    }
    return 0;
}

//indication
int radioData::dataCallListChangedInd(unsigned int slotId, int indicationType,
                           int token, RIL_Errno e, const void *response, size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s, slot:%d, tboxSlotId:%d, token:%d", __FUNCTION__, slotId, tboxSlotId, token);
    if (radioDataServiceImpl[slotId].dataIndication == NULL &&
            radioDataServiceImpl[tboxSlotId].dataIndication == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataIndication == NULL",
                __FUNCTION__, slotId);
        return 0;
    }

    if ((response == NULL && responselen != 0) ||
            responselen % sizeof(MTK_RIL_Data_Call_Response_v11) != 0) {
        mtkLogE(LOG_TAG, "%s: invalid response", __FUNCTION__);
        return 0;
    }

    std::vector<aidl_radio_data::SetupDataCallResult> dcList;

    if (radioDataServiceImpl[slotId].dataIndication != NULL) {
        convertRilDataCallListToHal(response, responselen, dcList, slotId);
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        RadioDataAdapater::dataCallListChangedIndEx(static_cast<void*>(dcList.data()), dcList.size(), slotId);
#endif

        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataIndication->dataCallListChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dcList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    }

    if (radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
        convertRilDataCallListToHal(response, responselen, dcList, tboxSlotId);

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        RadioDataAdapater::dataCallListChangedIndEx(static_cast<void*>(dcList.data()), dcList.size(), tboxSlotId);
#endif

        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].dataIndication->dataCallListChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dcList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    }

    return 0;
}


int radioData::keepaliveStatusInd(unsigned int slotId,
                       int indicationType, int token, RIL_Errno e, const void *response,
                       size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s, slot:%d, tboxSlotId:%d, token:%d", __FUNCTION__, slotId, tboxSlotId, token);

    if (radioDataServiceImpl[slotId].dataIndication == NULL &&
            radioDataServiceImpl[tboxSlotId].dataIndication == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataIndication == NULL",
                __FUNCTION__, slotId);
        mtkLogE(LOG_TAG, "%s: tbox-radioDataServiceImpl[%d].dataIndication == NULL",
                __FUNCTION__, tboxSlotId);
        return 0;
    }

    if (response == NULL || responselen != sizeof(aidl_radio_data::KeepaliveStatus)) {
        mtkLogE(LOG_TAG, "%s: invalid response", __FUNCTION__);
        return 0;
    }

    aidl_radio_data::KeepaliveStatus ks;
    convertRilKeepaliveStatusToAidl((RIL_KeepaliveStatus*)(response), ks);

    if (radioDataServiceImpl[slotId].dataIndication != NULL) {
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataIndication->keepaliveStatus(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), ks);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!",__FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    }

    if (radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].dataIndication->keepaliveStatus(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), ks);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!",__FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    }

    return 0;
}

int radioData::pcoDataInd(unsigned int slotId,
               int indicationType, int token, RIL_Errno e, const void *response,
               size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s: slotId:%d, tboxSlotId:%d", __FUNCTION__, slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].dataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
        if (response == NULL || responselen != sizeof(RIL_PCO_Data)) {
            mtkLogE(LOG_TAG, "%s: slotId:%d response invalid", __FUNCTION__, slotId);
            mtkLogE(LOG_TAG, "%s: tboxSlotId:%d response invalid", __FUNCTION__, tboxSlotId);
            return 0;
        }

        aidl_radio_data::PcoDataInfo pco = {};
        RIL_PCO_Data *rilPcoData = (RIL_PCO_Data *)response;
        pco.cid = rilPcoData->cid;
        pco.bearerProto = convertCharPtrToStdString(rilPcoData->bearer_proto);
        pco.pcoId = rilPcoData->pco_id;

        std::vector<uint8_t> data;
        data.resize(rilPcoData->contents_length);
        uint8_t *pTemp = (uint8_t*)(rilPcoData->contents);
        if (rilPcoData->contents_length == 1) {
            data[0] = (uint8_t)(pTemp[0] - '0');
        } else {
            for (int i = 0; i < rilPcoData->contents_length; i++) {
                data[i] = (uint8_t) pTemp[i];
            }
        }
        pco.contents = data;

        if (radioDataServiceImpl[slotId].dataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataIndication->pcoData(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pco);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId:%d, binder fail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataIndication->pcoData(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pco);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId:%d, binder fail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "pcoDataInd: radioService[%d]->mRadioIndication == NULL", slotId);
    }
    return 0;
}

// new add
int radioData::unthrottleApnInd(unsigned int slotId,
               int indicationType, int token, RIL_Errno e, const void *response,
               size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;

    mtkLogD(LOG_TAG, "unthrottleApnInd: slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].dataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
        if (response == NULL || responselen != sizeof(RIL_MtkDataProfileInfo)) {
            mtkLogE(LOG_TAG, "unthrottleApnInd: slotId:%d response invalid", slotId);
            mtkLogE(LOG_TAG, "unthrottleApnInd: tboxSlotId:%d response invalid", tboxSlotId);
            return 0;
        }

        aidl_radio_data::DataProfileInfo dpi = {};
        RIL_MtkDataProfileInfo *dpiRil = (RIL_MtkDataProfileInfo*)response;
        dpi.profileId = dpiRil->profileId;
        dpi.apn = convertCharPtrToStdString(dpiRil->apn);
        // Fillin real protocol and roamingProtocol
        dpi.protocol = aidl_radio_data::PdpProtocolType::IPV4V6;
        dpi.roamingProtocol = aidl_radio_data::PdpProtocolType::IPV4V6;
        // Fillin real authType
        dpi.authType = aidl_radio_data::ApnAuthType::PAP_CHAP;
        dpi.user = convertCharPtrToStdString(dpiRil->user);
        dpi.password = convertCharPtrToStdString(dpiRil->password);
        dpi.type = dpiRil->type;
        dpi.maxConnsTime = dpiRil->maxConnsTime;
        dpi.maxConns = dpiRil->maxConns;
        dpi.waitTime = dpiRil->waitTime;
        dpi.enabled = (dpiRil->enabled == 1 ? true : false);
        dpi.supportedApnTypesBitmap = dpiRil->supportedTypesBitmask;
        dpi.bearerBitmap = dpiRil->bearerBitmask;
        dpi.mtuV4 = dpiRil->mtu;
        dpi.mtuV6 = dpiRil->mtuV6;
        // Fillin real preferred, persistent, alwaysOn
        dpi.preferred = true;
        dpi.persistent = true;
        dpi.alwaysOn = true;
        // Fillin real trafficDescriptor
        dpi.trafficDescriptor = {};

        if (radioDataServiceImpl[slotId].dataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataIndication->unthrottleApn(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dpi);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "unthrottleApnInd: slotId:%d, binder fail!", slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].dataIndication->unthrottleApn(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), dpi);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "unthrottleApnInd: tboxSlotId:%d, binder fail!", tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    }
    return 0;
}

int radioData::slicingConfigChangedInd(unsigned int slotId,
               int indicationType, int token, RIL_Errno e, const void *response,
               size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s, slotId:%d, tboxSlotId:%d", __FUNCTION__, slotId, tboxSlotId);

    if (radioDataServiceImpl[slotId].dataIndication == NULL &&
            radioDataServiceImpl[tboxSlotId].dataIndication == NULL) {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataIndication == NULL",
                __FUNCTION__, slotId);
        mtkLogE(LOG_TAG, "%s: tbox-radioDataServiceImpl[%d].dataIndication == NULL",
                __FUNCTION__, tboxSlotId);
        return 0;
    }

    if (response == NULL || responselen != sizeof(RIL_SlicingConfig)) {
        mtkLogE(LOG_TAG, "%s: invalid response", __FUNCTION__);
        return 0;
    }

    aidl_radio_data::SlicingConfig slicingConfig = {};
    convertRilSlicingConfigToHal(response, responselen, slicingConfig);

    if (radioDataServiceImpl[slotId].dataIndication != NULL) {
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataIndication->slicingConfigChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                slicingConfig);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!",__FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    }

    if (radioDataServiceImpl[tboxSlotId].dataIndication != NULL) {
        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].dataIndication->slicingConfigChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                slicingConfig);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binder fail!",__FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    }
    return 0;
}

int radioData::clearDataResponseAndIndications(unsigned int slotId) {
    radioDataServiceImpl[slotId].dataResponse = NULL;
    radioDataServiceImpl[slotId].dataIndication = NULL;
    return 0;
}


//mtk radio


namespace android::hardware::radio::data {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::android::hardware::radio::data;
namespace aidlCommon = ::aidl::android::hardware::radio;
constexpr auto ok = &ScopedAStatus::ok;

RadioData::RadioData(int slotid){
    mSlotId = slotid;
}

ScopedAStatus RadioData::allocatePduSessionId(int32_t serial) {
    mtkLogD(LOG_TAG, "allocatePduSessionId: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_ALLOCATE_PDU_SESSION_ID);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_SUCCESS);
    }
    return ok();
}

ScopedAStatus RadioData::cancelHandover(int32_t serial, int32_t callId) {
    mtkLogD(LOG_TAG, "cancelHandover: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_CANCEL_HANDOVER);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_SUCCESS);
    }
    return ok();
}

ScopedAStatus RadioData::deactivateDataCall(int32_t serial, int32_t cid,
                                            aidl::DataRequestReason reason) {
    mtkLogD(LOG_TAG, "deactivateDataCall: serial %d, reason %d", serial, (int32_t)reason);

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    RadioDataAdapater::deactivateDataCallEx(cid);
#endif

    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_DEACTIVATE_DATA_CALL, false,
            2, (const char *) (std::to_string(cid)).c_str(),
            (const char *) (std::to_string((int32_t)reason)).c_str());
    return ok();
}

ScopedAStatus RadioData::getDataCallList(int32_t serial) {
    mtkLogD(LOG_TAG, "getDataCallList: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_DATA_CALL_LIST);
    return ok();
}

ScopedAStatus RadioData::getSlicingConfig(int32_t serial) {
    mtkLogD(LOG_TAG, "getSlicingConfig: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SLICING_CONFIG);
    return ok();
}

ScopedAStatus RadioData::releasePduSessionId(int32_t serial, int32_t id) {
    mtkLogD(LOG_TAG, "releasePduSessionId: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_RELEASE_PDU_SESSION_ID);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_SUCCESS);
    }
    return ok();
}

ScopedAStatus RadioData::responseAcknowledgement() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus RadioData::setDataAllowed(int32_t serial, bool allow) {
    mtkLogD(LOG_TAG, "setDataAllowed: serial:%d slotId:%d", serial, mSlotId);
    mtk_property_set("vendor.ril.data.legacy_allow_mode", "1");
    dispatchInts(serial, mSlotId, RIL_REQUEST_ALLOW_DATA, 1, (allow ? 1 : 0));
    return ok();
}

ScopedAStatus RadioData::setDataProfile(int32_t serial,
        const std::vector<aidl::DataProfileInfo>& profiles) {
    mtkLogD(LOG_TAG, "setDataProfile: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_DATA_PROFILE);
    if (pRI != NULL) {
        size_t num = profiles.size();
        RIL_MtkDataProfileInfo *dataProfiles =
            (RIL_MtkDataProfileInfo*)calloc(num, sizeof(RIL_MtkDataProfileInfo));
        if (dataProfiles == NULL) {
            mtkLogE(LOG_TAG, "Memory allocation failed for request %s",
                    requestToString(pRI->pCI->requestNumber));
            sendErrorResponse(pRI, RIL_E_NO_MEMORY);
            return ok();
        }
        RIL_MtkDataProfileInfo **dataProfilePtrs =
               (RIL_MtkDataProfileInfo**)calloc(num, sizeof(RIL_MtkDataProfileInfo*));

        if (dataProfilePtrs == NULL) {
            mtkLogE(LOG_TAG, "Memory allocation failed for request %s",
                    requestToString(pRI->pCI->requestNumber));
            free(dataProfiles);
            sendErrorResponse(pRI, RIL_E_NO_MEMORY);
            return ok();
        }
        bool success = false;
        for (size_t i = 0; i < num; i++) {
            dataProfilePtrs[i] = &dataProfiles[i];
            success = rilAidlUtils::copyAidlStringToRil(&dataProfiles[i].apn,
                    profiles[i].apn, pRI, true);
            if (success && !rilAidlUtils::copyAidlStringToRil(&dataProfiles[i].user,
                    profiles[i].user, pRI, true)) {
                success = false;
            }
            if (success && !rilAidlUtils::copyAidlStringToRil(&dataProfiles[i].password,
                    profiles[i].password, pRI, true)) {
                success = false;
            }
            if (!success) {
                freeSetDataProfileData(num, dataProfiles, dataProfilePtrs, 3,
                        &RIL_MtkDataProfileInfo::apn, &RIL_MtkDataProfileInfo::user,
                        &RIL_MtkDataProfileInfo::password);
                return ok();
            }
            dataProfiles[i].protocol = convertPdpProtocolTypeToString(profiles[i].protocol);
            dataProfiles[i].roamingProtocol = convertPdpProtocolTypeToString(
                    profiles[i].roamingProtocol);
            dataProfiles[i].profileId = (RIL_DataProfile)profiles[i].profileId;
            dataProfiles[i].authType =
                    convertAuthTypeToInt(profiles[i].authType) & APN_AUTH_TYPE_MAX_NUM;
            dataProfiles[i].type = (int) profiles[i].type;
            dataProfiles[i].maxConnsTime = profiles[i].maxConnsTime;
            dataProfiles[i].maxConns = profiles[i].maxConns;
            dataProfiles[i].waitTime = profiles[i].waitTime;
            dataProfiles[i].enabled = BOOL_TO_INT(profiles[i].enabled);
            dataProfiles[i].supportedTypesBitmask = profiles[i].supportedApnTypesBitmap;

            // Shift by 1 bit back due to the discrepancy between Android Q and Android P
            // dataProfiles[i].bearerBitmask = profiles[i].bearerBitmap
            dataProfiles[i].bearerBitmask = profiles[i].bearerBitmap >> 1;
            mtkLogD(LOG_TAG, "setDataProfile: supportedTypesBitmask=%d, bearerBitmask %d",
                    dataProfiles[i].supportedTypesBitmask,
                    dataProfiles[i].bearerBitmask);
            dataProfiles[i].mtu = profiles[i].mtuV4;
            dataProfiles[i].mtuV6 = profiles[i].mtuV6;
            dataProfiles[i].mvnoType = (char*)"";
            dataProfiles[i].mvnoMatchData = (char*)"";

            // M: use data profile to sync apn tables to modem
            // set default value for inactiveTimer
            unsigned int inactiveTimer = (unsigned int)profiles[i].authType;
            dataProfiles[i].inactiveTimer = (inactiveTimer >> APN_INACTIVE_TIMER_KEY);
        }
        CALL_ONREQUEST(RIL_REQUEST_SET_DATA_PROFILE, dataProfilePtrs,
                num * sizeof(RIL_MtkDataProfileInfo *), pRI, mSlotId);
        freeSetDataProfileData(num, dataProfiles, dataProfilePtrs, 3,
                &RIL_MtkDataProfileInfo::apn, &RIL_MtkDataProfileInfo::user,
                &RIL_MtkDataProfileInfo::password);
    }
    return ok();
}

ScopedAStatus RadioData::setDataThrottling(int32_t serial, aidl::DataThrottlingAction dta,
                                           int64_t completionDurationMs) {
    mtkLogD(LOG_TAG, "setDataThrottling: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SET_DATA_THROTTLING);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_SUCCESS);
    }
    return ok();
}

ScopedAStatus RadioData::setInitialAttachApn(int32_t serial,
        const std::optional<aidl::DataProfileInfo>& info) {
    mtkLogD(LOG_TAG, "setInitialAttachApn: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_INITIAL_ATTACH_APN);
    if (pRI != NULL) {
        if (info.has_value()) {
            RIL_InitialAttachApn_v15 iaa = {};
            if (!rilAidlUtils::copyAidlStringToRil(&iaa.apn, info->apn, pRI, true)) {
                mtkLogE(LOG_TAG, "setInitialAttachApn: copy apn fail!");
                return ok();
            }
            iaa.protocol = convertPdpProtocolTypeToString(info->protocol);
            iaa.roamingProtocol = convertPdpProtocolTypeToString(info->roamingProtocol);
            iaa.authtype = (int)info->authType;
            if (!rilAidlUtils::copyAidlStringToRil(&iaa.username, info->user, pRI, true)) {
                memsetAndFreeStrings(1, iaa.apn);
                mtkLogE(LOG_TAG, "setInitialAttachApn: copy username fail!");
                return ok();
            }
            if (!rilAidlUtils::copyAidlStringToRil(&iaa.password, info->password, pRI, true)) {
                memsetAndFreeStrings(2, iaa.apn, iaa.username);
                mtkLogE(LOG_TAG, "setInitialAttachApn: copy password fail!");
                return ok();
            }
            iaa.supportedTypesBitmask = (int)info->supportedApnTypesBitmap;
            iaa.bearerBitmask = (int)info->bearerBitmap;
            iaa.modemCognitive = BOOL_TO_INT(info->persistent);
            iaa.mtu = info->mtuV4;
            iaa.mvnoType = (char*)"";
            iaa.mvnoMatchData = (char*)"";
            iaa.canHandleIms = 0; // useless
            CALL_ONREQUEST(RIL_REQUEST_SET_INITIAL_ATTACH_APN, &iaa, sizeof(iaa), pRI, mSlotId);
            memsetAndFreeStrings(3, iaa.apn, iaa.username, iaa.password);
        } else {
            sendErrorResponse(pRI, RIL_E_SUCCESS);
        }
    }
    return ok();
}

ScopedAStatus RadioData::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioDataResponse>& dataResponse,
        const std::shared_ptr<aidl::IRadioDataIndication>& dataIndication) {
    mtkLogD(LOG_TAG, "aosp data setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp data setResponseFunctions: got lock %d", mSlotId);

    radioDataServiceImpl[mSlotId].dataResponse = dataResponse;
    radioDataServiceImpl[mSlotId].dataIndication = dataIndication;
    plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        rilAidlUtils::setAospResponseNumberToOne(rilAidlUtils::DATA_INDEX, mSlotId);
    }
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp data setResponseFunctions: release lock %d", mSlotId);

    // client is connected. Send initial indications.
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        if (rilAidlUtils::checkIfSetAllAospResponseDone(mSlotId) == 1) {
            if(client == android::CLIENT_TBOX) {
                android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::TBOX_RADIO_AOSP);
            } else {
                android::onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::RADIO_AOSP);
            }

        }
    }

    return ok();
}

ScopedAStatus RadioData::setupDataCall(
        int32_t serial, aidlCommon::AccessNetwork accessNetwork,
        const aidl::DataProfileInfo& dataProfileInfo, bool roamingAllowed,
        aidl::DataRequestReason reason, const std::vector<aidl::LinkAddress>& addresses,
        const std::vector<std::string>& dnses, int32_t pduSessId,
        const std::optional<aidl::SliceInfo>& sliceInfo, bool matchAllRuleAllowed) {
    mtkLogD(LOG_TAG, "setupDataCall: serial %d", serial);
    mtk_property_set("vendor.ril.data.preferred_data_mode", "1");
    int requestId = RIL_REQUEST_SETUP_DATA_CALL;
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, requestId);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setupDataCall: pRI is NULL!!");
        return ok();
    }

    RIL_SetupDataCallRequest sdcr = {};
    // access network
    sdcr.accessNetwork = (RIL_RadioAccessNetworks) accessNetwork;
    // data profile info
    sdcr.dataProfileInfo.profileId = dataProfileInfo.profileId;
    if (!rilAidlUtils::copyAidlStringToRil(&sdcr.dataProfileInfo.apn, dataProfileInfo.apn, pRI, true)) {
        mtkLogW(LOG_TAG, "setupDataCall: copy dataProfileInfo.apn fail!!");
        return ok();
    }
    sdcr.dataProfileInfo.protocol = convertPdpProtocolTypeToString(dataProfileInfo.protocol);
    sdcr.dataProfileInfo.roamingProtocol =
            convertPdpProtocolTypeToString(dataProfileInfo.roamingProtocol);
    sdcr.dataProfileInfo.authType = (int) dataProfileInfo.authType;
    if (!rilAidlUtils::copyAidlStringToRil(&sdcr.dataProfileInfo.user, dataProfileInfo.user, pRI, true)) {
        memsetAndFreeStrings(1, sdcr.dataProfileInfo.apn);
        mtkLogW(LOG_TAG, "setupDataCall: copy dataProfileInfo.user fail!!");
        return ok();
    }
    if (!rilAidlUtils::copyAidlStringToRil(&sdcr.dataProfileInfo.password,
            dataProfileInfo.password, pRI, true)) {
        memsetAndFreeStrings(2, sdcr.dataProfileInfo.apn, sdcr.dataProfileInfo.user);
        mtkLogW(LOG_TAG, "setupDataCall: copy dataProfileInfo.password fail!!");
        return ok();
    }
    sdcr.dataProfileInfo.type = dataProfileInfo.type;
    sdcr.dataProfileInfo.maxConnsTime = dataProfileInfo.maxConnsTime;
    sdcr.dataProfileInfo.maxConns = dataProfileInfo.maxConns;
    sdcr.dataProfileInfo.waitTime = dataProfileInfo.waitTime;
    sdcr.dataProfileInfo.enabled = BOOL_TO_INT(dataProfileInfo.enabled);
    sdcr.dataProfileInfo.supportedTypesBitmask = dataProfileInfo.supportedApnTypesBitmap;
    sdcr.dataProfileInfo.bearerBitmask = dataProfileInfo.bearerBitmap;
    sdcr.dataProfileInfo.mtu = dataProfileInfo.mtuV4;
    sdcr.dataProfileInfo.mtuV6 = dataProfileInfo.mtuV6;

    // TD
    if (dataProfileInfo.trafficDescriptor.dnn.has_value()
            || dataProfileInfo.trafficDescriptor.osAppId.has_value()) {
        sdcr.tdValid = SETUP_DATA_CALL_TD_AOSP;
        if (dataProfileInfo.trafficDescriptor.dnn.has_value()) {
            if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.dnn,
                    dataProfileInfo.trafficDescriptor.dnn.value(), pRI)) {
                mtkLogE(LOG_TAG, "setupDataCall: copy dataProfileInfo.dnn fail!!");
                goto finish;
            }
        } else {
            sdcr.TD.dnn = (char*)"";
        }
        if (dataProfileInfo.trafficDescriptor.osAppId.has_value()) {
            int len = dataProfileInfo.trafficDescriptor.osAppId->osAppId.size();
            if (len > 0) {
                mtkLogD(LOG_TAG, "setupDataCall: osAppId len = %d", len);
                sdcr.TD.osAppIdLen = len;
                sdcr.TD.osAppId = (uint8_t *)calloc(len, sizeof(uint8_t));
                if (sdcr.TD.osAppId == NULL) {
                    sendErrorResponse(pRI, RIL_E_NO_MEMORY);
                    mtkLogE(LOG_TAG, "setupDataCall: osAppId calloc failed!");
                    goto finish;
                }
                for (int i = 0; i < len; i++) {
                    sdcr.TD.osAppId[i] = (uint8_t) dataProfileInfo.trafficDescriptor.osAppId->osAppId[i];
                }
            } else {
                mtkLogW(LOG_TAG, "setupDataCall: osAppId len = %d", len);
            }
        }
    } else {
        sdcr.tdValid = SETUP_DATA_CALL_TD_NONE;
        sdcr.TD = {};
    }

    // roaming allowed
    sdcr.roamingAllowed = BOOL_TO_INT(roamingAllowed);
    // reason
    sdcr.reason = (int) reason;
    // pdu session id
    sdcr.pduSessionId = pduSessId;

    // slice info
    if (sliceInfo.has_value()) {
        sdcr.sliceInfoValid = 1;
        sdcr.sliceInfo.sst = (RIL_SliceServiceType) sliceInfo->sliceServiceType;
        sdcr.sliceInfo.sliceDifferentiator = (int) sliceInfo->sliceDifferentiator;
        sdcr.sliceInfo.mappedHplmnSst= (RIL_SliceServiceType) sliceInfo->mappedHplmnSst;
        sdcr.sliceInfo.mappedHplmnSD= (int) sliceInfo->mappedHplmnSd;
        sdcr.sliceInfo.status = (RIL_SliceStatus) sliceInfo->status;
    } else {
        sdcr.sliceInfoValid = 0;
        sdcr.sliceInfo = {};
    }

    // match all rule allowed
    sdcr.matchAllRuleAllowed = matchAllRuleAllowed;

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    if (!RadioDataAdapater::setupDataCallEx(serial, &sdcr, sizeof(sdcr), &radioDataServiceImpl[mSlotId], mSlotId)) {
        sendErrorResponse(pRI, RIL_E_CANCELLED);
        return ok();
    }
#endif

    CALL_ONREQUEST(requestId, &sdcr, sizeof(sdcr), pRI, mSlotId);
finish:
    memsetAndFreeStrings(3, sdcr.dataProfileInfo.apn, sdcr.dataProfileInfo.user,
            sdcr.dataProfileInfo.password);
    if (dataProfileInfo.trafficDescriptor.dnn.has_value() && sdcr.TD.dnn != NULL) {
        free(sdcr.TD.dnn);
    }
    if (dataProfileInfo.trafficDescriptor.osAppId.has_value() && sdcr.TD.osAppId != NULL) {
        free(sdcr.TD.osAppId);
    }
    return ok();
}

ScopedAStatus RadioData::startHandover(int32_t serial, int32_t callId) {
    mtkLogD(LOG_TAG, "startHandover: serial:%d slotId:%d", serial, mSlotId);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_START_HANDOVER);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_SUCCESS);
    }
    return ok();
}

ScopedAStatus RadioData::startKeepalive(int32_t serial, const aidl::KeepaliveRequest& keepalive) {
    mtkLogD(LOG_TAG, "%s(): %d, keepalive: %s", __FUNCTION__, serial, keepalive.toString().c_str());
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_START_KEEPALIVE);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "startKeepalive: : pRI is NULL!!");
        return ok();
    }

    RIL_KeepaliveRequest kaReq = {};

    kaReq.type = static_cast<RIL_KeepaliveType>(keepalive.type);
    switch(kaReq.type) {
        case NATT_IPV4:
        case IPV4_TCP:
        case IPV4_UDP:
            if (keepalive.sourceAddress.size() != 4 ||
                    keepalive.destinationAddress.size() != 4) {
                mtkLogE(LOG_TAG, "Invalid address for keepalive!");
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                return ok();
            }
            break;
        case NATT_IPV6:
        case IPV6_TCP:
        case IPV6_UDP:
            if (keepalive.sourceAddress.size() != 16 ||
                    keepalive.destinationAddress.size() != 16) {
                mtkLogE(LOG_TAG, "Invalid address for keepalive!");
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                return ok();
            }
            break;
        default:
            mtkLogE(LOG_TAG, "Unknown packet keepalive type!");
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
    }

    // Validate destination buffer size to avoid overflow
    if (sizeof(kaReq.sourceAddress) < keepalive.sourceAddress.size() ||
        sizeof(kaReq.destinationAddress) < keepalive.destinationAddress.size()) {
        mtkLogE(LOG_TAG, "Internal error: buffer size mismatch!");
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return ok();
    }

    // Copying source and destination addresses
    memcpy(kaReq.sourceAddress, keepalive.sourceAddress.data(), keepalive.sourceAddress.size());
    kaReq.sourcePort = keepalive.sourcePort;

    memcpy(kaReq.destinationAddress, keepalive.destinationAddress.data(),
        keepalive.destinationAddress.size());
    kaReq.destinationPort = keepalive.destinationPort;


    kaReq.maxKeepaliveIntervalMillis = keepalive.maxKeepaliveIntervalMillis;
    kaReq.cid = keepalive.cid; // This is the context ID of the data call

    mtkLogD(LOG_TAG, "%s(): type=%d, sourceAddress=%d.%d.%d.%d, sourcePort=%d,"
            " destinationAddress=%d.%d.%d.%d, destinationPort=%d, maxKeepaliveIntervalMillis=%d,"
            " cid=%d", __FUNCTION__, kaReq.type, kaReq.sourceAddress[0] & 0xff,
            kaReq.sourceAddress[1] & 0xff, kaReq.sourceAddress[2] & 0xff,
            kaReq.sourceAddress[3] & 0xff, kaReq.sourcePort, kaReq.destinationAddress[0] & 0xff,
            kaReq.destinationAddress[1] & 0xff, kaReq.destinationAddress[2] & 0xff,
            kaReq.destinationAddress[3] & 0xff, kaReq.destinationPort,
            kaReq.maxKeepaliveIntervalMillis, kaReq.cid);

    CALL_ONREQUEST(pRI->pCI->requestNumber, &kaReq, sizeof(RIL_KeepaliveRequest), pRI, mSlotId);
    return ok();
}

ScopedAStatus RadioData::stopKeepalive(int32_t serial, int32_t sessionHandle) {
    mtkLogD(LOG_TAG, "%s(): %d", __FUNCTION__, serial);
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_STOP_KEEPALIVE);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "stopKeepalive: pRI is NULL!!");
        return ok();
    }
    CALL_ONREQUEST(pRI->pCI->requestNumber, &sessionHandle, sizeof(uint32_t), pRI, mSlotId);
    return ok();
}

}  // namespace android::hardware::radio::data


namespace vendor::mediatek::hardware::mtkradioex::data {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::data;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioData::MtkRadioData(int slotid) {
    mSlotId = slotid;
}

ScopedAStatus MtkRadioData::dataConnectionAttach(int32_t serial, int32_t type, int32_t clientId) {
    mtkLogD(LOG_TAG, "dataConnectionAttach: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_DATA_CONNECTION_ATTACH, 1, type);
    return ok();
}

ScopedAStatus MtkRadioData::dataConnectionDetach(int32_t serial, int32_t type, int32_t clientId) {
    mtkLogD(LOG_TAG, "dataConnectionDetach: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_DATA_CONNECTION_DETACH, 1, type);
    return ok();
}

ScopedAStatus MtkRadioData::enableDsdaIndication(int32_t serial, bool enable, int32_t clientId) {
    mtkLogD(LOG_TAG, "enableDsdaIndication: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId, RIL_REQUEST_ENABLE_DSDA_INDICATION, 1, (enable ? 1 : 0));
    return ok();
}

ScopedAStatus MtkRadioData::getDsdaStatus(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getDsdaStatus: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_DSDA_STATUS);
    return ok();
}

ScopedAStatus MtkRadioData::resetAllConnections(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "resetAllConnections: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId, RIL_REQUEST_RESET_ALL_CONNECTIONS);
    return ok();
}

ScopedAStatus MtkRadioData::resetMdDataRetryCount(int32_t serial, const std::string& apn, int32_t clientId) {
    mtkLogD(LOG_TAG, "resetMdDataRetryCount: serial: %d clientId:%d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    // TODO: if need handle clientId ?
    rilAidlUtils::dispatchString(serial, mSlotId, RIL_REQUEST_RESET_MD_DATA_RETRY_COUNT, apn.c_str());
    return ok();
}

ScopedAStatus MtkRadioData::setFdMode(int32_t serial, int32_t mode, int32_t param1,
        int32_t param2, int32_t clientId) {
    mtkLogD(LOG_TAG, "setFdMode: serial:%d slotId:%d clientId:%d", serial, mSlotId, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_ASSIST)) {
        return ok();
    }

    /* AT+EFD=<mode>[,<param1>[,<param2>]] */
    /* For all modes: but mode 0 & 1 only has one argument */
    if (mode == 0 || mode == 1) {
        dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_FD_MODE,
                2, 1, mode);
    }
    if (mode == 2) {
        dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_FD_MODE,
                4, 3, mode, param1, param2);
    }
    if (mode == 3) {
        dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_FD_MODE,
                3, 2, mode, param1);
    }
    return ok();
}

ScopedAStatus MtkRadioData::syncDataSettingsToMd(int32_t serial,
        const std::vector<int32_t>& settings, int32_t clientId) {
    mtkLogD(LOG_TAG, "syncDataSettingsToMd: serial:%d slotId:%d clientId:%d, settings[0]:%d settings[1]:%d settings[2]:%d settings[3]:%d",
            serial, mSlotId, clientId, settings[0], settings[1], settings[2], settings[3]);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_ASSIST)) {
        return ok();
    }

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    if (!RadioDataAdapater::syncDataSettingsToMdEx(settings[0], clientId)) {
        return ok();
    }
#endif

    if (settings.size() == 5) {
        dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MD, 5,
                settings[0], settings[1], settings[2], settings[3], settings[4]);
    }
    return ok();
}

ScopedAStatus MtkRadioData::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioData::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExDataResponse>& radioResponse, const std::shared_ptr<aidl::IMtkRadioExDataIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk data setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk data setResponseFunctions: got lock %d", mSlotId);

    radioDataServiceImpl[mSlotId].mtkDataResponse = radioResponse;
    radioDataServiceImpl[mSlotId].mtkDataIndication = radioIndication;
    plusCounterMtkRadioNumOne(mSlotId);

    rilAidlUtils::setMtkResponseNumberToOne(rilAidlUtils::DATA_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk data setResponseFunctions: release lock %d", mSlotId);

    // client is connected. Send initial indications.
    android::ClientId client = getClientBySlot(mSlotId);
    if (rilAidlUtils::checkIfSetAllMtkResponseDone(mSlotId) == 1) {
        if (client == android::CLIENT_TBOX) {
            android::onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::TBOX_RADIO_MTK);
        } else {
            android::onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::RADIO_MTK);
        }
    }

    return ok();
}

ScopedAStatus MtkRadioData::setResponseFunctionsAssist(const std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::assist::IAssistRadioResponse>& radioResponse) {
    mtkLogD(LOG_TAG, "setResponseFunctionsAssist, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsAssist: got lock %d", mSlotId);
    radioDataServiceImpl[mSlotId].mtkAssistDataResponse = radioResponse;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "setResponseFunctionsAssist: release lock %d", mSlotId);
    return ok();
}

ScopedAStatus MtkRadioData::setupDataCallSlice(
        int32_t serial, ::aidl::android::hardware::radio::AccessNetwork accessNetwork,
        const ::aidl::android::hardware::radio::data::DataProfileInfo& dataProfileInfo,
        bool roamingAllowed, ::aidl::android::hardware::radio::data::DataRequestReason reason,
        const std::vector<::aidl::android::hardware::radio::data::LinkAddress>& addresses,
        const std::vector<std::string>& dnses, int32_t pduSessionId,
        const std::optional<::aidl::android::hardware::radio::data::SliceInfo>& sliceInfo,
        bool matchAllRuleAllowed,
        const std::optional<aidl_mtk_radio_data::TrafficDescriptor>& trafficDescriptor,
        const std::optional<std::string>& verifyDescriptor __unused,
        int32_t responseMode,
        int32_t clientId) {
    mtkLogD(LOG_TAG, "setupDataCallSlice: serial %d, clientId: %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    mtk_property_set("vendor.ril.data.preferred_data_mode", "1");
    int requestId =
            ((responseMode == SETUP_DATA_CALL_RESPONSE_AOSP) ?
                    RIL_REQUEST_SETUP_DATA_CALL : RIL_REQUEST_SETUP_DATA_CALL_SLICE);
    android::RequestInfo *pRI = android::addRequestToList(serial, mSlotId, requestId);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setupDataCallSlice: pRI is NULL!!");
        return ok();
    }
    // legacy chips before gen98 don't support, directly return.
    char mipcSupport[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("ro.vendor.mtk_mipc_support", mipcSupport, "0");
    if (mipcSupport[0] == '0') {
        mtkLogD(LOG_TAG, "setupDataCallSlice: not supported!");
        sendErrorResponse(pRI, RIL_E_SUCCESS);
        return ok();
    }

    RIL_SetupDataCallRequest sdcr = {};
    memset(&sdcr, 0, sizeof(sdcr));
    // access network
    sdcr.accessNetwork = (RIL_RadioAccessNetworks) accessNetwork;
    // data profile info
    sdcr.dataProfileInfo.profileId = dataProfileInfo.profileId;
    if (!rilAidlUtils::copyAidlStringToRil(&sdcr.dataProfileInfo.apn, dataProfileInfo.apn, pRI, true)) {
        mtkLogW(LOG_TAG, "setupDataCallSlice: copy dataProfileInfo.apn fail!!");
        return ok();
    }
    sdcr.dataProfileInfo.protocol = convertPdpProtocolTypeToString(dataProfileInfo.protocol);
    sdcr.dataProfileInfo.roamingProtocol =
            convertPdpProtocolTypeToString(dataProfileInfo.roamingProtocol);
    sdcr.dataProfileInfo.authType = (int) dataProfileInfo.authType;
    if (!rilAidlUtils::copyAidlStringToRil(&sdcr.dataProfileInfo.user, dataProfileInfo.user, pRI, true)) {
        memsetAndFreeStrings(1, sdcr.dataProfileInfo.apn);
        mtkLogW(LOG_TAG, "setupDataCallSlice: copy dataProfileInfo.user fail!!");
        return ok();
    }
    if (!rilAidlUtils::copyAidlStringToRil(&sdcr.dataProfileInfo.password,
            dataProfileInfo.password, pRI, true)) {
        memsetAndFreeStrings(2, sdcr.dataProfileInfo.apn, sdcr.dataProfileInfo.user);
        mtkLogW(LOG_TAG, "setupDataCallSlice: copy dataProfileInfo.password fail!!");
        return ok();
    }
    sdcr.dataProfileInfo.type = dataProfileInfo.type;
    sdcr.dataProfileInfo.maxConnsTime = dataProfileInfo.maxConnsTime;
    sdcr.dataProfileInfo.maxConns = dataProfileInfo.maxConns;
    sdcr.dataProfileInfo.waitTime = dataProfileInfo.waitTime;
    sdcr.dataProfileInfo.enabled = BOOL_TO_INT(dataProfileInfo.enabled);
    sdcr.dataProfileInfo.supportedTypesBitmask = dataProfileInfo.supportedApnTypesBitmap;
    sdcr.dataProfileInfo.bearerBitmask = dataProfileInfo.bearerBitmap;
    sdcr.dataProfileInfo.mtu = dataProfileInfo.mtuV4;
    sdcr.dataProfileInfo.mtuV6 = dataProfileInfo.mtuV6;

    // TD
    // this is used to control the response path,
    // 0 - setupDataCallResponse
    // 1 - setupDataCallResponseSlice
    sdcr.tdValid = (responseMode == SETUP_DATA_CALL_RESPONSE_AOSP) ? SETUP_DATA_CALL_TD_AOSP : SETUP_DATA_CALL_TD_EXT;
    if (trafficDescriptor.has_value()) {
        if (trafficDescriptor->dnn.has_value()) {
            if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.dnn,
                    trafficDescriptor->dnn.value(), pRI)) {
                mtkLogE(LOG_TAG, "setupDataCallSlice: copy dataProfileInfo.dnn fail!!");
                goto finish;
            }
        }
        if (trafficDescriptor->osAppId.has_value()) {
            int len = trafficDescriptor->osAppId->osAppId.size();
            if (len > 0) {
                mtkLogD(LOG_TAG, "setupDataCallSlice: osAppId len = %d", len);
                sdcr.TD.osAppIdLen = len;
                sdcr.TD.osAppId = (uint8_t *)calloc(len, sizeof(uint8_t));
                if (sdcr.TD.osAppId == NULL) {
                    sendErrorResponse(pRI, RIL_E_NO_MEMORY);
                    mtkLogE(LOG_TAG, "setupDataCallSlice: osAppId calloc failed!");
                    goto finish;
                }
                for (int i = 0; i < len; i++) {
                    sdcr.TD.osAppId[i] = (uint8_t) trafficDescriptor->osAppId->osAppId[i];
                }
            } else {
                mtkLogW(LOG_TAG, "setupDataCallSlice: osAppId len = %d", len);
            }
        }
        if (trafficDescriptor->ipDescriptors.has_value()) {
            if (trafficDescriptor->ipDescriptors->ipv4.has_value()) {
                if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.ipDescriptors.ipv4,
                        trafficDescriptor->ipDescriptors->ipv4.value(), pRI)) {
                    mtkLogE(LOG_TAG, "setupDataCallSlice: copy trafficDescriptor.ipDescriptors.ipv4 fail!!");
                    goto finish;
                }
            }
            if (trafficDescriptor->ipDescriptors->maskV4.has_value()) {
                if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.ipDescriptors.maskV4,
                        trafficDescriptor->ipDescriptors->maskV4.value(), pRI)) {
                    mtkLogE(LOG_TAG, "setupDataCallSlice: copy trafficDescriptor.ipDescriptors.maskV4 fail!!");
                    goto finish;
                }
            }
            if (trafficDescriptor->ipDescriptors->ipv6.has_value()) {
                if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.ipDescriptors.ipv6,
                        trafficDescriptor->ipDescriptors->ipv6.value(), pRI)) {
                    mtkLogE(LOG_TAG, "setupDataCallSlice: copy trafficDescriptor.ipDescriptors.ipv6 fail!!");
                    goto finish;
                }
            }
            sdcr.TD.ipDescriptors.prefixLength = trafficDescriptor->ipDescriptors->prefixLength;
            sdcr.TD.ipDescriptors.port = trafficDescriptor->ipDescriptors->port;
            sdcr.TD.ipDescriptors.portStartRange = trafficDescriptor->ipDescriptors->portStartRange;
            sdcr.TD.ipDescriptors.portEndRange = trafficDescriptor->ipDescriptors->portEndRange;
            sdcr.TD.ipDescriptors.protocolId = trafficDescriptor->ipDescriptors->protocolId;
        }
        if (trafficDescriptor->domainDescriptors.has_value()) {
            if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.domainDescriptors,
                    trafficDescriptor->domainDescriptors.value(), pRI)) {
                mtkLogE(LOG_TAG, "setupDataCallSlice: copy trafficDescriptor.domainDescriptors fail!!");
                goto finish;
            }
        }
        if (trafficDescriptor->connectionCapabilities.has_value()) {
            if (!rilAidlUtils::copyAidlStringToRil(&sdcr.TD.connectionCapabilities,
                    trafficDescriptor->connectionCapabilities.value(), pRI)) {
                mtkLogE(LOG_TAG, "setupDataCallSlice: copy trafficDescriptor.connectionCapabilities fail!!");
                goto finish;
            }
        }
    }

    // roaming allowed
    sdcr.roamingAllowed = BOOL_TO_INT(roamingAllowed);
    // reason
    sdcr.reason = (int) reason;
    // pdu session id
    sdcr.pduSessionId = pduSessionId;

    // slice info
    if (sliceInfo.has_value()) {
        sdcr.sliceInfoValid = 1;
        sdcr.sliceInfo.sst = (RIL_SliceServiceType) sliceInfo->sliceServiceType;
        sdcr.sliceInfo.sliceDifferentiator = (int) sliceInfo->sliceDifferentiator;
        sdcr.sliceInfo.mappedHplmnSst = (RIL_SliceServiceType) sliceInfo->mappedHplmnSst;
        sdcr.sliceInfo.mappedHplmnSD = (int) sliceInfo->mappedHplmnSd;
        sdcr.sliceInfo.status = (RIL_SliceStatus) sliceInfo->status;
    } else {
        sdcr.sliceInfoValid = 0;
    }

    // match all rule allowed
    sdcr.matchAllRuleAllowed = matchAllRuleAllowed;

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    if (!RadioDataAdapater::setupDataCallSliceEx(serial, &sdcr, sizeof(sdcr), &radioDataServiceImpl[mSlotId], mSlotId)) {
        return ok();
    }
#endif

    CALL_ONREQUEST(requestId, &sdcr, sizeof(sdcr), pRI, mSlotId);
finish:
    memsetAndFreeStrings(3, sdcr.dataProfileInfo.apn, sdcr.dataProfileInfo.user, sdcr.dataProfileInfo.password);
    FREEIF(sdcr.TD.dnn);
    FREEIF(sdcr.TD.osAppId);
    FREEIF(sdcr.TD.ipDescriptors.ipv4);
    FREEIF(sdcr.TD.ipDescriptors.maskV4);
    FREEIF(sdcr.TD.ipDescriptors.ipv6);
    FREEIF(sdcr.TD.domainDescriptors);
    FREEIF(sdcr.TD.connectionCapabilities);
    return ok();
}

}

int mtkRadioExData::clearMtkDataResponseAndIndications(unsigned int slotId) {
    radioDataServiceImpl[slotId].mtkDataResponse = NULL;
    radioDataServiceImpl[slotId].mtkDataIndication = NULL;
    return 0;
}

int mtkRadioExData::clearAssistResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioDataServiceImpl[realSlotId].mtkAssistDataResponse = NULL;
    return 0;
}

int mtkRadioExData::syncDataSettingsToMdResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "syncDataSettingsToMdResponse, serial:%d, slotId:%d", serial, slotId);
    if (clientId == android::CLIENT_ASSIST) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioDataServiceImpl[realSlotId].mtkAssistDataResponse == NULL) {
            mtkLogE(LOG_TAG, "syncDataSettingsToMdResponse assit:"
                    "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
            return 0;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioDataServiceImpl[realSlotId].mtkAssistDataResponse
                ->syncDataSettingsToMdResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "syncDataSettingsToMdResponse assit: slotId:%d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::ASSIST_MTK);
        }
    }

    return 0;
}

int mtkRadioExData::resetMdDataRetryCountResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "resetMdDataRetryCountResponse, serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].mtkDataResponse == NULL) {
        mtkLogE(LOG_TAG, "resetMdDataRetryCountResponse: "
                "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->resetMdDataRetryCountResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "keepaliveStatusInd: slotId = %d, binder fail!", slotId);
        retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
    }
    return 0;
}

int mtkRadioExData::dataConnectionAttachResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "dataConnectionAttachResponse, serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].mtkDataResponse == NULL) {
        mtkLogE(LOG_TAG, "dataConnectionAttachResponse: "
                "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->dataConnectionAttachResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "dataConnectionAttachResponse: slotId:%d, binder fail!", slotId);
        retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
    }
    return 0;
}

int mtkRadioExData::dataConnectionDetachResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "dataConnectionDetachResponse, serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].mtkDataResponse == NULL) {
        mtkLogE(LOG_TAG, "dataConnectionDetachResponse: "
                "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->dataConnectionDetachResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "dataConnectionDetachResponse: slotId:%d, binder fail!", slotId);
        retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
    }
    return 0;
}

int mtkRadioExData::resetAllConnectionsResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "resetAllConnectionsResponse, serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].mtkDataResponse == NULL) {
        mtkLogE(LOG_TAG, "resetAllConnectionsResponse: "
                "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->resetAllConnectionsResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "resetAllConnectionsResponse: slotId:%d, binder fail!", slotId);
        retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
    }
    return 0;
}

int mtkRadioExData::enableDsdaIndicationResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "enableDsdaIndicationResponse, serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].mtkDataResponse == NULL) {
        mtkLogE(LOG_TAG, "enableDsdaIndicationResponse: "
                "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
        return 0;
    }

    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->enableDsdaIndicationResponse(
            responseInfo);
    if (!ret.isOk()) {
        mtkLogE(LOG_TAG, "enableDsdaIndicationResponse: slotId:%d, binder fail!", slotId);
        retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
    }
    return 0;
}

int mtkRadioExData::getDsdaStatusResponse(unsigned int slotId,
        android::ClientId clientId __unused, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getDsdaStatusResponse: serial:%d, slotId:%d", serial, slotId);
    if (radioDataServiceImpl[slotId].mtkDataResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int dsdaStatus = 0;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "getDsdaStatusResponse response is invalid slotId:%d", slotId);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        } else {
            int *p_int = (int*)response;
            dsdaStatus = p_int[0];
        }
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->getDsdaStatusResponse(
                responseInfo, dsdaStatus);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getDsdaStatusResponse: slotId:%d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getDsdaStatusResponse: radioDataServiceImpl[%d]->dataResponse == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExData::setFdModeResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial,
        RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setFdModeResponse, serial:%d, slotId:%d", serial, slotId);
    if (clientId == android::CLIENT_ASSIST) {
        unsigned int realSlotId = slotId % MAX_SIM_COUNT;
        if (radioDataServiceImpl[realSlotId].mtkAssistDataResponse == NULL) {
            mtkLogE(LOG_TAG, "setFdModeResponse assit:"
                    "radioDataServiceImpl[%d].mtkDataResponse == NULL", slotId);
            return 0;
        }

        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        ndk::ScopedAStatus ret = radioDataServiceImpl[realSlotId].mtkAssistDataResponse
                ->setFdModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setFdModeResponse assit: slotId:%d, binder fail!",
                    slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::ASSIST_MTK);
        }
    }

    return 0;
}

int mtkRadioExData::setupDataCallSliceResponse(unsigned int slotId, android::ClientId clientId,
                          int responseType, int serial, RIL_Errno e, const void *response,
                          size_t responseLen) {
    mtkLogD(LOG_TAG, "%s, serial:%d, slotId:%d", __FUNCTION__, serial, slotId);
    aidl_radio::RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

    /// M: For VTS purpose @{
//    if (s_cardState[slotId] == aidl_radio::sim::CardStatus::STATE_ABSENT) {
//        responseInfo.error = aidl_radio::RadioError::NONE;
//    }
    /// @}
    aidl_radio_data::SetupDataCallResult result = {};
    aidl_mtk_radio_data::SetupDataCallResultSlice resultSlice = {};
    int tdType = SETUP_DATA_CALL_RESPONSE_AOSP;

    if (response == NULL || responseLen != sizeof(MTK_RIL_Data_Call_Response_v11)) {
        mtkLogE(LOG_TAG, "%s: Invalid response", __FUNCTION__);
        if (e == RIL_E_SUCCESS) {
            responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }
        result.cause = aidl_radio_data::DataCallFailCause::ERROR_UNSPECIFIED;
        result.suggestedRetryTime = -1;
        result.cid = -1;
        result.active = aidl_radio_data::SetupDataCallResult::DATA_CONNECTION_STATUS_INACTIVE;
        result.type = aidl_radio_data::PdpProtocolType::UNKNOWN;
        //result.ifname = hidl_string();
        //result.addresses = hidl_vec<AOSP_V1_5::LinkAddress>();
        //result.dnses = hidl_vec<hidl_string>();
        //result.gateways = hidl_vec<hidl_string>();
        //result.pcscf = hidl_vec<hidl_string>();
        result.mtuV4 = 0;
        result.mtuV6 = 0;
    } else {
        tdType = ((MTK_RIL_Data_Call_Response_v11 *) response)->tdType;
        if (tdType == SETUP_DATA_CALL_RESPONSE_AOSP) {
            convertRilDataCallToHal((MTK_RIL_Data_Call_Response_v11 *) response,
                    result, slotId);
        } else {
            convertRilDataCallSliceToHal((MTK_RIL_Data_Call_Response_v11 *) response,
                    resultSlice, slotId);
        }
    }

    android::ClientId client = getClientBySlot(slotId);

    if (tdType == SETUP_DATA_CALL_RESPONSE_AOSP) {  // AOSP setupDataCallResponse
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        RadioDataAdapater::setupDataCallResponseEx(serial, &result, responseType, tdType, slotId);
#endif
        if (radioDataServiceImpl[slotId].dataResponse == NULL) {
            mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
            return 0;
        }
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].dataResponse->setupDataCallResponse(
                responseInfo, result);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {  // customized setupDataCallResponseSlice
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        RadioDataAdapater::setupDataCallSliceResponseEx(serial, &resultSlice, responseType, tdType, slotId);
#endif
        if (radioDataServiceImpl[slotId].dataResponse == NULL) {
            mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].dataResponse == NULL", __FUNCTION__, slotId);
        return 0;
        }
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataResponse->setupDataCallResponseSlice(
                responseInfo, resultSlice);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binder fail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    }

    return 0;
}

int mtkRadioExData::mobileDataUsageInd(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "mobileDataUsageInd: slotId:%d", slotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "mobileDataUsageInd: slotId:%d response invalid", slotId);
            return 0;
        }

        std::vector<int32_t> data;
        int num = responseLen / sizeof(int);
        data.resize(num);
        int *pTemp = (int*)response;
        for (int i = 0; i < num; i++) {
            data[i] = (int32_t) pTemp[i];
        }

        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->mobileDataUsageInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "mobileDataUsageInd: slotId:%d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "mobileDataUsageInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "mobileDataUsageInd: tboxSlotId:%d", tboxSlotId);
    if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "mobileDataUsageInd: tboxSlotId:%d response invalid", tboxSlotId);
            return 0;
        }

        std::vector<int32_t> data;
        int num = responseLen / sizeof(int);
        data.resize(num);
        int *pTemp = (int*)response;
        for (int i = 0; i < num; i++) {
            data[i] = (int32_t) pTemp[i];
        }

        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->mobileDataUsageInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "mobileDataUsageInd: tboxSlotId:%d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-mobileDataUsageInd: radioService[%d]->mRadioIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExData::onDsdaChangedInd(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "onDsdaChangedInd: slotId:%d", slotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "onDsdaChangedInd: slotId:%d response invalid", slotId);
            return 0;
        }

        int *mode = (int *)response;
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->onDsdaChangedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), mode[0]);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onDsdaChangedInd: slotId:%d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onDsdaChangedInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "onDsdaChangedInd: tboxSlotId:%d", tboxSlotId);
    if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "onDsdaChangedInd: tboxSlotId:%d response invalid", tboxSlotId);
            return 0;
        }

        int *mode = (int *)response;
        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->onDsdaChangedInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), mode[0]);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onDsdaChangedInd: tboxSlotId:%d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-onDsdaChangedInd: radioService[%d]->mRadioIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExData::onMdDataRetryCountReset(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "onMdDataRetryCountReset: slotId:%d", slotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->onMdDataRetryCountReset(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onMdDataRetryCountReset: slotId:%d, binder fail!", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onMdDataRetryCountReset: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "onMdDataRetryCountReset: tboxSlotId:%d", tboxSlotId);
    if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->onMdDataRetryCountReset(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "onMdDataRetryCountReset: tboxSlotId:%d, binder fail!", tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-onMdDataRetryCountReset: radioService[%d]->mRadioIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExData::onPlmnDataInd(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "onPlmnDataInd: slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_PlmnMvnoData)) {
            mtkLogE(LOG_TAG, "onPlmnDataInd: slotId:%d response invalid", slotId);
            return 0;
        }

        aidl_mtk_radio_data::PlmnMvnoInfo plmnMvnoInfo = {};
        RIL_PlmnMvnoData *rilPlmnMvnoData = (RIL_PlmnMvnoData *)response;
        plmnMvnoInfo.gsmPlmn = convertCharPtrToStdString(rilPlmnMvnoData->gsmPlmn);
        plmnMvnoInfo.cdmaPlmn = convertCharPtrToStdString(rilPlmnMvnoData->cdmaPlmn);
        plmnMvnoInfo.gsmSpn = convertCharPtrToStdString(rilPlmnMvnoData->gsmSpn);
        plmnMvnoInfo.cdmaSpn = convertCharPtrToStdString(rilPlmnMvnoData->cdmaSpn);
        plmnMvnoInfo.gsmImsi = convertCharPtrToStdString(rilPlmnMvnoData->gsmImsi);
        plmnMvnoInfo.cdmaImsi = convertCharPtrToStdString(rilPlmnMvnoData->cdmaImsi);
        plmnMvnoInfo.gid1 = convertCharPtrToStdString(rilPlmnMvnoData->gid1);
        plmnMvnoInfo.pnn = convertCharPtrToStdString(rilPlmnMvnoData->pnn);
        plmnMvnoInfo.impi = convertCharPtrToStdString(rilPlmnMvnoData->impi);

        if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->onPlmnDataInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), plmnMvnoInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onPlmnDataInd: slotId:%d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->onPlmnDataInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), plmnMvnoInfo);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onPlmnDataInd: tboxSlotId:%d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "onPlmnDataInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExData::pcoDataAfterAttachedInd(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "pcoDataAfterAttachedInd: slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_PCO_Data_attached)) {
            mtkLogE(LOG_TAG, "pcoDataAfterAttachedInd: slotId:%d response invalid", slotId);
            return 0;
        }

        aidl_mtk_radio_data::PcoDataAttachedInfo pco = {};
        RIL_PCO_Data_attached *rilPcoData = (RIL_PCO_Data_attached*)response;
        pco.cid = rilPcoData->cid;
        pco.apnName = convertCharPtrToStdString(rilPcoData->apn_name);
        pco.bearerProto = convertCharPtrToStdString(rilPcoData->bearer_proto);
        pco.pcoId = rilPcoData->pco_id;

        std::vector<uint8_t> data;
        data.resize(rilPcoData->contents_length);
        uint8_t *pTemp = (uint8_t*)(rilPcoData->contents);
        if (rilPcoData->contents_length == 1) {
            data[0] = (uint8_t)(pTemp[0] - '0');
        } else {
            for (int i = 0; i < rilPcoData->contents_length; i++) {
                data[i] = (uint8_t) pTemp[i];
            }
        }
        pco.contents = data;

        if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->pcoDataAfterAttached(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pco);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "pcoDataAfterAttachedInd: slotId:%d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->pcoDataAfterAttached(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), pco);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "pcoDataAfterAttachedInd: tboxSlotId:%d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "pcoDataAfterAttachedInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExData::networkRejectCauseInd(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "networkRejectCauseInd: slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "networkRejectCauseInd: slotId:%d response invalid", slotId);
            return 0;
        }

        int *pTemp = (int*)response;
        int num = responseLen / sizeof(int);
        if (num < 3) {
            mtkLogE(LOG_TAG, "networkRejectCauseInd: slotId:%d num invalid", slotId);
            return 0;
        }

        std::vector<int32_t> data;
        data.resize(num);
        for (int i = 0; i < num; i++) {
            data[i] = (int32_t) pTemp[i];
        }

        if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->networkRejectCauseInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "networkRejectCauseInd: slotId:%d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->networkRejectCauseInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "networkRejectCauseInd: tboxSlotId:%d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "networkRejectCauseInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExData::onNwLimitInd(unsigned int slotId,
        int indicationType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "onNwLimitInd: slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "onNwLimitInd: slotId:%d response invalid", slotId);
            return 0;
        }

        int *pTemp = (int*)response;
        int num = responseLen / sizeof(int);

        std::vector<int32_t> data;
        data.resize(num);
        for (int i = 0; i < num; i++) {
            data[i] = (int32_t) pTemp[i];
        }

        if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->onNwLimitInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onNwLimitInd: slotId:%d, binder fail!", slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->onNwLimitInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "onNwLimitInd: tboxSlotId:%d, binder fail!", tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "onNwLimitInd: radioService[%d]->mRadioIndication == NULL",
                slotId);
    }
    return 0;
}

int mtkRadioExData::qualifiedNetworkTypesChangedInd(unsigned int slotId, int indicationType,
        int serial, RIL_Errno e, const void *response, size_t responseLen) {
    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "qualifiedNetworkTypesChangedInd: slotId:%d, tboxSlotId:%d", slotId, tboxSlotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL ||
            radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "qualifiedNetworkTypesChangedInd: slotId:%d response invalid",
                    slotId);
            return 0;
        }

        int *pTemp = (int*)response;
        int num = responseLen / sizeof(int);
        if (num < 3) {
            mtkLogE(LOG_TAG, "qualifiedNetworkTypesChangedInd: slotId:%d num invalid",
                    slotId);
            return 0;
        }

        std::vector<int32_t> data;
        data.resize(num);
        for (int i = 0; i < num; i++) {
            data[i] = (int32_t) pTemp[i];
        }

        if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->qualifiedNetworkTypesChangedInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "qualifiedNetworkTypesChangedInd: slotId:%d, binder fail!",
                        slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
            ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->qualifiedNetworkTypesChangedInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "qualifiedNetworkTypesChangedInd: tboxSlotId:%d, binder fail!",
                        tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "qualifiedNetworkTypesChangedInd: "
                "radioService[%d]->mRadioIndication == NULL", slotId);
    }
    return 0;
}

int mtkRadioExData::postUrspRule(unsigned int slotId, int indicationType,
        int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "%s: slotId:%d", __FUNCTION__, slotId);
    if (radioDataServiceImpl[slotId].mtkDataIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: slotId:%d response invalid", __FUNCTION__, slotId);
            return 0;
        }

        std::vector<aidl_mtk_radio_data::UrspRule> rules;
        ndk::ScopedAStatus ret = radioDataServiceImpl[slotId].mtkDataIndication->postUrspRule(
                slotId,
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *)response),
                rules);// not used currently
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId:%d, binder fail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioDataServiceImpl[%d].mtkDataIndication == NULL", __FUNCTION__, slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s: tboxSlotId:%d", __FUNCTION__, tboxSlotId);
    if (radioDataServiceImpl[tboxSlotId].mtkDataIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId:%d response invalid", __FUNCTION__, tboxSlotId);
            return 0;
        }

        std::vector<aidl_mtk_radio_data::UrspRule> rules;
        ndk::ScopedAStatus ret = radioDataServiceImpl[tboxSlotId].mtkDataIndication->postUrspRule(
                slotId,
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *)response),
                rules);// not used currently
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId:%d, binder fail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: tbox-radioDataServiceImpl[%d].mtkDataIndication == NULL", __FUNCTION__, tboxSlotId);
    }
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addDataLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_DATA_PORT_BASE 51000
#define MTK_RADIO_DATA_PORT_BASE 51100

static pthread_t gThreadIds[MAX_SIM_COUNT*2];

void* addRcpVsockRadioDataAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    using android::hardware::radio::data::RadioData;
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "addRcpVsockRadioDataAidlService setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_DATA_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "addRcpVsockRadioDataAidlService setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "addRcpVsockRadioDataAidlService VSOCK listen port: %u sloid:%d", RADIO_DATA_PORT_BASE + sloid, sloid);

    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    std::shared_ptr<android::hardware::radio::data::RadioData> radioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(sloid);
    addDataLock(std::static_pointer_cast<ndk::ICInterface>(radioDataAidl));  // need or no need
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioDataAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "addRcpVsockRadioDataAidlService VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "addRcpVsockRadioDataAidlService VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioDataAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    using vendor::mediatek::hardware::mtkradioex::data::MtkRadioData;
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "addRcpVsockMtkRadioDataAidlService setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_DATA_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "addRcpVsockMtkRadioDataAidlService setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "addRcpVsockMtkRadioDataAidlService VSOCK listen port: %u sloid:%d", MTK_RADIO_DATA_PORT_BASE + sloid, sloid);

    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkRadioDataAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(sloid);
    addDataLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioDataAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioDataAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "addRcpVsockMtkRadioDataAidlService VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "addRcpVsockMtkRadioDataAidlService VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioDataVsock(unsigned int &sloid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioDataAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioData");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockMtkRadioDataAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioData");
    }
}
#endif

void publicRadioDataAidlService(unsigned int sloid) {
    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::data::RadioData> radioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(0);
        gPublishedHals.push_back(radioDataAidl);
        const std::string instance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioDataAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp data addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(0);
        gPublishedHals.push_back(mtkRadioDataAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioDataAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk data addservice, slotId:%d, status:%d", sloid, mtkStatus);

    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::data::RadioData> radioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(1);
        gPublishedHals.push_back(radioDataAidl);
        const std::string instance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioDataAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp data addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(1);
        gPublishedHals.push_back(mtkRadioDataAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioDataAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk data addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::data::RadioData> radioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(2);
        gPublishedHals.push_back(radioDataAidl);
        const std::string instance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioDataAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp data addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(2);
        gPublishedHals.push_back(mtkRadioDataAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioDataAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk data addservice, slotId:%d, status:%d", sloid, mtkStatus);

    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::data::RadioData> radioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(3);
        gPublishedHals.push_back(radioDataAidl);
        const std::string instance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioDataAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp data addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(3);
        gPublishedHals.push_back(mtkRadioDataAidl);
        const std::string mtkInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioDataAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk data addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    // add slotid for yocto client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;

    if (sloid == 0) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioDataVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::data::RadioData> tboxRadioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioDataAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioDataAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox data addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkTboxRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioDataAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioDataAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox data addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 1) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioDataVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::data::RadioData> tboxRadioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioDataAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioDataAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox data addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkTboxRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioDataAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioDataAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox data addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 2) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioDataVsock(sloid);
#endif

        // add service for yocto client
        std::shared_ptr<android::hardware::radio::data::RadioData> tboxRadioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioDataAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioDataAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox data addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkTboxRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioDataAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioDataAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox data addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    } else if (sloid == 3) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioDataVsock(sloid);
#endif
        // add service for yocto client
        std::shared_ptr<android::hardware::radio::data::RadioData> tboxRadioDataAidl = ndk::SharedRefBase::make<android::hardware::radio::data::RadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioDataAidl));
        const std::string tboxInstance = std::string() + android::hardware::radio::data::RadioData::descriptor + "/tboxSlot4";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioDataAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox data addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData> mtkTboxRadioDataAidl = ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::data::MtkRadioData>(tboxSlotid);
        addDataLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioDataAidl));
        const std::string mtkTboxInstance = std::string() + vendor::mediatek::hardware::mtkradioex::data::MtkRadioData::descriptor + "/tboxSlot4";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioDataAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox data addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
    }
}
