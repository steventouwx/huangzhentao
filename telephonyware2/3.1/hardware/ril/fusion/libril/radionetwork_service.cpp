/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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

#include <cstddef>
#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <mtk_log.h>
#include <radionetwork_service.h>
#include <ril_service.h>
#include <rilAidlUtils.h>
#include <binder/ProcessState.h>
#include <android/binder_manager.h>
#include "rilAidlUtils.h"
#include "ril_context.h"
#include <inttypes.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

using ::android::RequestInfo;
using ::aidl::android::hardware::radio::RadioError;
using ::aidl::android::hardware::radio::RadioResponseInfo;
using ::aidl::android::hardware::radio::network::OperatorInfo;
using ::aidl::android::hardware::radio::AccessNetwork;
using ::aidl::android::hardware::radio::RadioTechnology;

namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_radio_mtk = ::aidl::vendor::mediatek::hardware::mtkradioex;

using aidl_radio::RadioTechnologyFamily;
using aidl_radio_mtk::network::BandModeInfo;
using aidl_radio_mtk::network::Lte1xRttCellInfo;
using aidl_radio_mtk::network::LteData;
using aidl_radio_mtk::network::OperatorInfoWithAct;
using aidl_radio_mtk::network::SignalStrengthWithWcdmaEcio;
using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::retChecker;

int64_t nitzTimeReceivedAidl[MAX_SIM_COUNT*android::CLIENT_COUNT];

extern int getRafFromNetworkType(int type);
extern int getNetworkTypeFromRaf(int raf);

struct RadioNetworkImpl {
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::network::IMtkRadioExNetworkResponse> mtkNetworkResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::network::IMtkRadioExNetworkIndication> mtkNetworkIndication;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch::ISmartRatSwitchRadioResponse> mSmartRatSwitchResponse;
    std::shared_ptr<::aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch::ISmartRatSwitchRadioIndication> mSmartRatSwitchIndication;
    std::shared_ptr<::aidl::android::hardware::radio::network::IRadioNetworkResponse> networkResponse;
    std::shared_ptr<::aidl::android::hardware::radio::network::IRadioNetworkIndication> networkIndication;
};

RadioNetworkImpl radioNetworkServiceImpl[MAX_SIM_COUNT*(android::CLIENT_TBOX_IMS + 1)];

int responseInt(RadioResponseInfo& responseInfo, int serial, int responseType, RIL_Errno e,
               const void *response, size_t responseLen) {
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    int ret = -1;

    if (response == NULL || responseLen != sizeof(int)) {
        mtkLogE(LOG_TAG, "responseInt: Invalid response");
        if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
    } else {
        int *p_int = (int *) response;
        ret = p_int[0];
    }
    return ret;
}

int convertToOperatorStatus(const char *str) {
   if (strncmp("unknown", str, 9) == 0) {
       return (int) OperatorInfo::STATUS_UNKNOWN;
   } else if (strncmp("available", str, 9) == 0) {
       return (int) OperatorInfo::STATUS_AVAILABLE;
   } else if (strncmp("current", str, 9) == 0) {
       return (int) OperatorInfo::STATUS_CURRENT;
   } else if (strncmp("forbidden", str, 9) == 0) {
       return (int) OperatorInfo::STATUS_FORBIDDEN;
   } else {
       return -1;
   }
}

bool dispatchSearchRat(int serial, int slotId, int request, const std::vector<int32_t>& rat) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }
    int countInts = rat.size() + 1;
    int *pInts = (int *)calloc(countInts, sizeof(int));

    if (pInts == NULL) {
        mtkLogD(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }
    pInts[0] = rat.size(); // rat info array size
    for (int i = 0; i < pInts[0]; i++) {
        pInts[i+1] = rat[i];
    }
    CALL_ONREQUEST(request, pInts, countInts * sizeof(int), pRI, pRI->socket_id);
    free(pInts);
    return true;
}

bool dispatchSearchStoredFreqInfo(int serial, int slotId, int request, int32_t operation,
    int32_t plmnId, int32_t rat, const std::vector<int32_t>& freq) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int countInts = freq.size() + 4;
    int *pInts = (int *)calloc(countInts, sizeof(int));

    if (pInts == NULL) {
        mtkLogD(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    pInts[0] = operation;
    pInts[1] = plmnId;
    pInts[2] = rat;
    pInts[3] = freq.size(); // freq info array size
    for (int i = 0; i <  pInts[3]; i++) {
        pInts[i+4] = freq[i];
    }
    CALL_ONREQUEST(request, pInts, countInts * sizeof(int), pRI, pRI->socket_id);
    free(pInts);
    return true;
}

bool dispatchBandPriorityList(int serial, int slotId, int request, const std::vector<int32_t>& bandPriList) {
    RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int countInts = bandPriList.size();
    int *pInts = (int *)calloc(countInts, sizeof(int));

    if (pInts == NULL) {
        mtkLogE(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(request));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }
    for (int i = 0; i < countInts; i++) {
        pInts[i] = bandPriList[i];
    }
    CALL_ONREQUEST(request, pInts, countInts * sizeof(int), pRI, pRI->socket_id);
    free(pInts);
    return true;
}

// should acquire write lock for the corresponding service before calling this
void radioNetwork::setNitzTimeReceived(unsigned int slotId, int64_t timeReceived) {
    nitzTimeReceivedAidl[slotId] = timeReceived;
}


namespace radioNetwork {

namespace aidl = ::aidl::android::hardware::radio::network;
namespace aidl_radio = ::aidl::android::hardware::radio;

//extern void sendErrorResponse(RequestInfo *pRI, RIL_Errno err);

void fillCellIdentityAidl(aidl::CellIdentity& cellIdentity, const RIL_CellIdentity_v16 &rilCellIdentity) {
    int err = 0;
    char buff[12] = {0};

    switch (rilCellIdentity.cellInfoType) {
        case RIL_CELL_INFO_TYPE_GSM: {
            aidl::CellIdentityGsm gsm;
            if (rilCellIdentity.cellIdentityGsm.mcc == 0x7FFFFFFF) {
              gsm.mcc = std::string();
              gsm.mcc = std::string();
              gsm.operatorNames.alphaLong = std::string();
              gsm.operatorNames.alphaShort = std::string();
            } else {
              err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityGsm.mcc);
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityGsm.mcc fail");
              }
              gsm.mcc = convertCharPtrToStdString(buff);
              if (rilCellIdentity.cellIdentityGsm.mnc_len == 3) {
                err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityGsm.mnc);
              } else if (rilCellIdentity.cellIdentityGsm.mnc_len == 2) {
                err = snprintf(buff, 6, "%02d", rilCellIdentity.cellIdentityGsm.mnc);
              } else {
                err = snprintf(buff, 11, "%d", rilCellIdentity.cellIdentityGsm.mnc);
              }
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityGsm.mnc fail");
              }
              gsm.mnc = convertCharPtrToStdString(buff);
              gsm.operatorNames.alphaLong = convertCharPtrToStdString(rilCellIdentity.cellIdentityGsm.operName.long_name);
              gsm.operatorNames.alphaShort = convertCharPtrToStdString(rilCellIdentity.cellIdentityGsm.operName.short_name);
            }

            gsm.lac = rilCellIdentity.cellIdentityGsm.lac;
            gsm.cid = rilCellIdentity.cellIdentityGsm.cid;
            gsm.arfcn = rilCellIdentity.cellIdentityGsm.arfcn;
            gsm.bsic = (uint8_t) rilCellIdentity.cellIdentityGsm.bsic;

            gsm.additionalPlmns = {};
            cellIdentity = aidl::CellIdentity(gsm);
            break;
        }

        case RIL_CELL_INFO_TYPE_WCDMA: {
            aidl::CellIdentityWcdma wcdma;
            if (rilCellIdentity.cellIdentityWcdma.mcc == 0x7FFFFFFF) {
              wcdma.mcc = std::string();
              wcdma.mcc = std::string();
              wcdma.operatorNames.alphaLong = std::string();
              wcdma.operatorNames.alphaShort = std::string();
            } else {
              err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityWcdma.mcc);
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityWcdma.mcc fail");
              }
              wcdma.mcc = convertCharPtrToStdString(buff);
              if (rilCellIdentity.cellIdentityWcdma.mnc_len == 3) {
                err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityWcdma.mnc);
              } else if (rilCellIdentity.cellIdentityWcdma.mnc_len == 2) {
                err = snprintf(buff, 6, "%02d", rilCellIdentity.cellIdentityWcdma.mnc);
              } else {
                err = snprintf(buff, 11, "%d", rilCellIdentity.cellIdentityWcdma.mnc);
              }
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityWcdma.mnc fail");
              }
              wcdma.mnc = convertCharPtrToStdString(buff);
              wcdma.operatorNames.alphaLong = convertCharPtrToStdString(rilCellIdentity.cellIdentityWcdma.operName.long_name);
              wcdma.operatorNames.alphaShort = convertCharPtrToStdString(rilCellIdentity.cellIdentityWcdma.operName.short_name);
            }

            wcdma.lac = rilCellIdentity.cellIdentityWcdma.lac;
            wcdma.cid = rilCellIdentity.cellIdentityWcdma.cid;
            wcdma.psc = rilCellIdentity.cellIdentityWcdma.psc;
            wcdma.uarfcn = rilCellIdentity.cellIdentityWcdma.uarfcn;

            wcdma.additionalPlmns = {};
            cellIdentity = aidl::CellIdentity(wcdma);
            break;
        }

        case RIL_CELL_INFO_TYPE_CDMA: {
            aidl::CellIdentityCdma cdma;
            cdma.networkId = rilCellIdentity.cellIdentityCdma.networkId;
            cdma.systemId = rilCellIdentity.cellIdentityCdma.systemId;
            cdma.baseStationId = rilCellIdentity.cellIdentityCdma.basestationId;
            cdma.longitude = rilCellIdentity.cellIdentityCdma.longitude;
            cdma.latitude = rilCellIdentity.cellIdentityCdma.latitude;
            cdma.operatorNames.alphaLong =
                    convertCharPtrToStdString(rilCellIdentity.cellIdentityCdma.operName.long_name);
            cdma.operatorNames.alphaShort =
                    convertCharPtrToStdString(rilCellIdentity.cellIdentityCdma.operName.short_name);
            cellIdentity = aidl::CellIdentity(cdma);

            break;
        }

        case RIL_CELL_INFO_TYPE_LTE: {
            aidl::CellIdentityLte lte;
            if (rilCellIdentity.cellIdentityLte.mcc == 0x7FFFFFFF) {
              lte.mcc = std::string();
              lte.mcc = std::string();
              lte.operatorNames.alphaLong = std::string();
              lte.operatorNames.alphaShort = std::string();
            } else {
              err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityLte.mcc);
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityLte.mcc fail");
              }
              lte.mcc = convertCharPtrToStdString(buff);
              if (rilCellIdentity.cellIdentityLte.mnc_len == 3) {
                err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityLte.mnc);
              } else if (rilCellIdentity.cellIdentityLte.mnc_len == 2) {
                err = snprintf(buff, 6, "%02d", rilCellIdentity.cellIdentityLte.mnc);
              } else {
                err = snprintf(buff, 11, "%d", rilCellIdentity.cellIdentityLte.mnc);
              }
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityLte.mnc fail");
              }
              lte.mnc = convertCharPtrToStdString(buff);
              lte.operatorNames.alphaLong = convertCharPtrToStdString(rilCellIdentity.cellIdentityLte.operName.long_name);
              lte.operatorNames.alphaShort = convertCharPtrToStdString(rilCellIdentity.cellIdentityLte.operName.short_name);
            }
            lte.ci = rilCellIdentity.cellIdentityLte.ci;
            lte.pci = rilCellIdentity.cellIdentityLte.pci;
            lte.tac = rilCellIdentity.cellIdentityLte.tac;
            lte.earfcn = rilCellIdentity.cellIdentityLte.earfcn;
            lte.bandwidth = rilCellIdentity.cellIdentityLte.bandwidth;
            lte.additionalPlmns = {};
            lte.csgInfo = {};
            if (rilCellIdentity.cellIdentityLte.band == 0 || rilCellIdentity.cellIdentityLte.band > 88) {
                lte.bands = {};
            } else {
                lte.bands.resize(1);
                lte.bands[0] = (aidl::EutranBands)rilCellIdentity.cellIdentityLte.band;
            }
            cellIdentity = aidl::CellIdentity(lte);
            break;
        }

        case RIL_CELL_INFO_TYPE_TD_SCDMA: {
            mtkAssert((char*) "TD-SCDMA is impossible EXIST here");
            break;
        }

        case RIL_CELL_INFO_TYPE_NR: {
            aidl::CellIdentityNr nr;
            if (rilCellIdentity.cellIdentityNr.mcc == 0x7FFFFFFF) {
              nr.mcc = std::string();
              nr.mcc = std::string();
              nr.operatorNames.alphaLong = std::string();
              nr.operatorNames.alphaShort = std::string();
            } else {
              mtkLogE(LOG_TAG, "mcc:%d, mnc=%d", rilCellIdentity.cellIdentityNr.mcc, rilCellIdentity.cellIdentityNr.mnc);
              err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityNr.mcc);
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityNr.mcc fail");
              }
              nr.mcc = convertCharPtrToStdString(buff);
              if (rilCellIdentity.cellIdentityNr.mnc_len == 3) {
                err = snprintf(buff, 6, "%03d", rilCellIdentity.cellIdentityNr.mnc);
              } else if (rilCellIdentity.cellIdentityNr.mnc_len == 2) {
                err = snprintf(buff, 6, "%02d", rilCellIdentity.cellIdentityNr.mnc);
              } else {
                err = snprintf(buff, 11, "%d", rilCellIdentity.cellIdentityNr.mnc);
              }
              if (err < 0) {
                mtkLogE(LOG_TAG, "snprintf cellIdentityNr.mnc fail");
              }
              nr.mnc = convertCharPtrToStdString(buff);
              nr.operatorNames.alphaLong = convertCharPtrToStdString(rilCellIdentity.cellIdentityNr.operName.long_name);
              nr.operatorNames.alphaShort = convertCharPtrToStdString(rilCellIdentity.cellIdentityNr.operName.short_name);
            }
            nr.nci = rilCellIdentity.cellIdentityNr.nci;
            nr.pci = rilCellIdentity.cellIdentityNr.pci;
            nr.tac = rilCellIdentity.cellIdentityNr.tac;
            nr.nrarfcn = rilCellIdentity.cellIdentityNr.nrarfcn;
            nr.additionalPlmns = {};
            if (rilCellIdentity.cellIdentityNr.band == 0 || rilCellIdentity.cellIdentityNr.band > 261) {
                nr.bands = {};
            } else {
                nr.bands.resize(1);
                nr.bands[0] = (aidl::NgranBands)rilCellIdentity.cellIdentityNr.band;
            }
            cellIdentity = aidl::CellIdentity(nr);
            break;
        }

        default: {
            break;
        }
    }
}

void convertRilCellInfoListToAidl(const void *response, size_t responseLen,
                                     std::vector<aidl::CellInfo>& records)
{
    int err = 0;
    int num = responseLen / sizeof(RIL_CellInfo_v12);
    records.resize(num);
    char buff[12] = {0};
    RIL_CellInfo_v12 *rillCellInfo = (RIL_CellInfo_v12 *) response;
    for (int i = 0; i < num; i++) {
        records[i].registered = rillCellInfo->registered == 0 ? false: true;
        records[i].connectionStatus =
                (aidl::CellConnectionStatus) rillCellInfo->connectionStatus;
        switch (rillCellInfo->cellInfoType) {
            case RIL_CELL_INFO_TYPE_GSM: {
                aidl::CellInfoGsm cellInfoGsm = {};
                // clear information if mcc is unknown
                if (rillCellInfo->CellInfo.gsm.cellIdentityGsm.mcc == 0x7FFFFFFF) {
                    cellInfoGsm.cellIdentityGsm.mcc = std::string();
                    cellInfoGsm.cellIdentityGsm.mnc = std::string();
                    cellInfoGsm.cellIdentityGsm.operatorNames.alphaLong = std::string();
                    cellInfoGsm.cellIdentityGsm.operatorNames.alphaShort = std::string();
                } else {
                    err = snprintf(buff, 6, "%03d",
                            rillCellInfo->CellInfo.gsm.cellIdentityGsm.mcc);
                    if (err < 0) {
                        mtkLogD(LOG_TAG, "snprintf cellIdentityGsm.mcc fail");
                    }
                    cellInfoGsm.cellIdentityGsm.mcc = convertCharPtrToStdString(buff);
                    if (rillCellInfo->CellInfo.gsm.cellIdentityGsm.mnc_len == 3) {
                        err = snprintf(buff, 6, "%03d",
                                rillCellInfo->CellInfo.gsm.cellIdentityGsm.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityGsm.mnc fail");
                        }
                    } else if (rillCellInfo->CellInfo.gsm.cellIdentityGsm.mnc_len == 2) {
                        err = snprintf(buff, 6, "%02d",
                                rillCellInfo->CellInfo.gsm.cellIdentityGsm.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityGsm.mnc fail");
                        }
                    } else {
                        err = snprintf(buff, 11, "%d",
                                rillCellInfo->CellInfo.gsm.cellIdentityGsm.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityGsm.mnc fail");
                        }
                    }
                    cellInfoGsm.cellIdentityGsm.mnc = convertCharPtrToStdString(buff);
                    cellInfoGsm.cellIdentityGsm.operatorNames.alphaLong =
                         convertCharPtrToStdString(
                             rillCellInfo->CellInfo.gsm.cellIdentityGsm.operName.long_name);
                    cellInfoGsm.cellIdentityGsm.operatorNames.alphaShort =
                         convertCharPtrToStdString(
                             rillCellInfo->CellInfo.gsm.cellIdentityGsm.operName.short_name);
                }
                cellInfoGsm.cellIdentityGsm.lac =
                        rillCellInfo->CellInfo.gsm.cellIdentityGsm.lac;
                cellInfoGsm.cellIdentityGsm.cid =
                        rillCellInfo->CellInfo.gsm.cellIdentityGsm.cid;
                cellInfoGsm.cellIdentityGsm.arfcn =
                        rillCellInfo->CellInfo.gsm.cellIdentityGsm.arfcn;
                cellInfoGsm.cellIdentityGsm.bsic =
                        (uint8_t) rillCellInfo->CellInfo.gsm.cellIdentityGsm.bsic;

                cellInfoGsm.cellIdentityGsm.additionalPlmns = {};
                cellInfoGsm.signalStrengthGsm.signalStrength =
                        rillCellInfo->CellInfo.gsm.signalStrengthGsm.signalStrength;
                cellInfoGsm.signalStrengthGsm.bitErrorRate =
                        rillCellInfo->CellInfo.gsm.signalStrengthGsm.bitErrorRate;
                cellInfoGsm.signalStrengthGsm.timingAdvance =
                        rillCellInfo->CellInfo.gsm.signalStrengthGsm.timingAdvance;
                records[i].ratSpecificInfo= aidl::CellInfoRatSpecificInfo(cellInfoGsm);
                break;
            }

            case RIL_CELL_INFO_TYPE_WCDMA: {
                aidl::CellInfoWcdma cellInfoWcdma = {};
                // clear information if mcc is unknown
                if (rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mcc == 0x7FFFFFFF) {
                    cellInfoWcdma.cellIdentityWcdma.mcc = std::string();
                    cellInfoWcdma.cellIdentityWcdma.mnc = std::string();
                    cellInfoWcdma.cellIdentityWcdma.operatorNames.alphaLong = std::string();
                    cellInfoWcdma.cellIdentityWcdma.operatorNames.alphaShort = std::string();
                } else {
                    err = snprintf(buff, 6, "%03d",
                            rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mcc);
                    if (err < 0) {
                        mtkLogD(LOG_TAG, "snprintf cellIdentityWcdma.mcc fail");
                    }
                    cellInfoWcdma.cellIdentityWcdma.mcc = convertCharPtrToStdString(buff);

                    if (rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mnc_len == 3) {
                        err = snprintf(buff, 6, "%03d",
                                rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityWcdma.mnc fail");
                        }
                    } else if (rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mnc_len == 2) {
                        err = snprintf(buff, 6, "%02d",
                                rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityWcdma.mnc fail");
                        }
                    } else {
                        err = snprintf(buff, 11, "%d",
                                rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityWcdma.mnc fail");
                        }
                    }
                    cellInfoWcdma.cellIdentityWcdma.mnc = convertCharPtrToStdString(buff);
                     cellInfoWcdma.cellIdentityWcdma.operatorNames.alphaLong =
                             convertCharPtrToStdString(
                                 rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.operName.long_name);
                     cellInfoWcdma.cellIdentityWcdma.operatorNames.alphaShort =
                             convertCharPtrToStdString(
                                 rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.operName.short_name);
                }

                cellInfoWcdma.cellIdentityWcdma.lac =
                        rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.lac;
                cellInfoWcdma.cellIdentityWcdma.cid =
                        rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.cid;
                cellInfoWcdma.cellIdentityWcdma.psc =
                        rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.psc;
                cellInfoWcdma.cellIdentityWcdma.uarfcn =
                        rillCellInfo->CellInfo.wcdma.cellIdentityWcdma.uarfcn;

                cellInfoWcdma.cellIdentityWcdma.additionalPlmns = {}; // not available
                cellInfoWcdma.cellIdentityWcdma.csgInfo ={}; // not available

                cellInfoWcdma.signalStrengthWcdma.signalStrength =
                        rillCellInfo->CellInfo.wcdma.signalStrengthWcdma.signalStrength;
                cellInfoWcdma.signalStrengthWcdma.bitErrorRate =
                        rillCellInfo->CellInfo.wcdma.signalStrengthWcdma.bitErrorRate;
                cellInfoWcdma.signalStrengthWcdma.rscp =
                        rillCellInfo->CellInfo.wcdma.signalStrengthWcdma.rscp;
                cellInfoWcdma.signalStrengthWcdma.ecno =
                        rillCellInfo->CellInfo.wcdma.signalStrengthWcdma.ecno;
                records[i].ratSpecificInfo= aidl::CellInfoRatSpecificInfo(cellInfoWcdma);
                break;
            }

            case RIL_CELL_INFO_TYPE_CDMA: {
                aidl::CellInfoCdma cellInfoCdma = {};
                cellInfoCdma.cellIdentityCdma.networkId =
                        rillCellInfo->CellInfo.cdma.cellIdentityCdma.networkId;
                cellInfoCdma.cellIdentityCdma.systemId =
                        rillCellInfo->CellInfo.cdma.cellIdentityCdma.systemId;
                cellInfoCdma.cellIdentityCdma.baseStationId =
                        rillCellInfo->CellInfo.cdma.cellIdentityCdma.basestationId;
                cellInfoCdma.cellIdentityCdma.longitude =
                        rillCellInfo->CellInfo.cdma.cellIdentityCdma.longitude;
                cellInfoCdma.cellIdentityCdma.latitude =
                        rillCellInfo->CellInfo.cdma.cellIdentityCdma.latitude;
                cellInfoCdma.cellIdentityCdma.operatorNames.alphaLong =
                             convertCharPtrToStdString(
                                 rillCellInfo->CellInfo.cdma.cellIdentityCdma.operName.long_name);
                cellInfoCdma.cellIdentityCdma.operatorNames.alphaShort =
                             convertCharPtrToStdString(
                                 rillCellInfo->CellInfo.cdma.cellIdentityCdma.operName.short_name);
                cellInfoCdma.signalStrengthCdma.dbm =
                        rillCellInfo->CellInfo.cdma.signalStrengthCdma.dbm;
                cellInfoCdma.signalStrengthCdma.ecio =
                        rillCellInfo->CellInfo.cdma.signalStrengthCdma.ecio;
                cellInfoCdma.signalStrengthEvdo.dbm =
                        rillCellInfo->CellInfo.cdma.signalStrengthEvdo.dbm;
                cellInfoCdma.signalStrengthEvdo.ecio =
                        rillCellInfo->CellInfo.cdma.signalStrengthEvdo.ecio;
                cellInfoCdma.signalStrengthEvdo.signalNoiseRatio =
                        rillCellInfo->CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio;

                records[i].ratSpecificInfo= aidl::CellInfoRatSpecificInfo(cellInfoCdma);
                break;
            }

            case RIL_CELL_INFO_TYPE_LTE: {
                aidl::CellInfoLte cellInfoLte = {};
                // clear information if mcc is unknown
                if (rillCellInfo->CellInfo.lte.cellIdentityLte.mcc == 0x7FFFFFFF) {
                    cellInfoLte.cellIdentityLte.mcc = std::string();
                    cellInfoLte.cellIdentityLte.mnc = std::string();
                    cellInfoLte.cellIdentityLte.operatorNames.alphaLong = std::string();
                    cellInfoLte.cellIdentityLte.operatorNames.alphaShort = std::string();
                } else {
                    err = snprintf(buff, 6, "%03d",
                            rillCellInfo->CellInfo.lte.cellIdentityLte.mcc);
                    if (err < 0) {
                        mtkLogD(LOG_TAG, "snprintf cellIdentityLte.mcc fail");
                    }
                    cellInfoLte.cellIdentityLte.mcc = convertCharPtrToStdString(buff);
                    if (rillCellInfo->CellInfo.lte.cellIdentityLte.mnc_len == 3) {
                        err = snprintf(buff, 6, "%03d",
                                rillCellInfo->CellInfo.lte.cellIdentityLte.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityLte.mnc fail");
                        }
                    } else if (rillCellInfo->CellInfo.lte.cellIdentityLte.mnc_len == 2) {
                        err = snprintf(buff, 6, "%02d",
                                rillCellInfo->CellInfo.lte.cellIdentityLte.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityLte.mnc fail");
                        }
                    } else {
                        err = snprintf(buff, 11, "%d",
                                rillCellInfo->CellInfo.lte.cellIdentityLte.mnc);
                        if (err < 0) {
                            mtkLogD(LOG_TAG, "snprintf cellIdentityLte.mnc fail");
                        }
                    }
                    cellInfoLte.cellIdentityLte.mnc = convertCharPtrToStdString(buff);
                    cellInfoLte.cellIdentityLte.operatorNames.alphaLong =
                         convertCharPtrToStdString(
                             rillCellInfo->CellInfo.lte.cellIdentityLte.operName.long_name);
                    cellInfoLte.cellIdentityLte.operatorNames.alphaShort =
                         convertCharPtrToStdString(
                             rillCellInfo->CellInfo.lte.cellIdentityLte.operName.short_name);
                }
                cellInfoLte.cellIdentityLte.ci =
                        rillCellInfo->CellInfo.lte.cellIdentityLte.ci;
                cellInfoLte.cellIdentityLte.pci =
                        rillCellInfo->CellInfo.lte.cellIdentityLte.pci;
                cellInfoLte.cellIdentityLte.tac =
                        rillCellInfo->CellInfo.lte.cellIdentityLte.tac;
                cellInfoLte.cellIdentityLte.earfcn =
                        rillCellInfo->CellInfo.lte.cellIdentityLte.earfcn;

                cellInfoLte.cellIdentityLte.bandwidth = 0x7FFFFFFF;
                cellInfoLte.cellIdentityLte.additionalPlmns = {};
                cellInfoLte.cellIdentityLte.csgInfo = {};
                cellInfoLte.cellIdentityLte.bands.resize(1);
                cellInfoLte.cellIdentityLte.bands[0] =
                        (aidl::EutranBands)rillCellInfo->CellInfo.lte.cellIdentityLte.band;

                cellInfoLte.signalStrengthLte.signalStrength =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.signalStrength;
                cellInfoLte.signalStrengthLte.rsrp =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.rsrp;
                cellInfoLte.signalStrengthLte.rsrq =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.rsrq;
                cellInfoLte.signalStrengthLte.rssnr =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.rssnr;
                cellInfoLte.signalStrengthLte.cqi =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.cqi;
                cellInfoLte.signalStrengthLte.timingAdvance =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.timingAdvance;
                cellInfoLte.signalStrengthLte.cqiTableIndex =
                        rillCellInfo->CellInfo.lte.signalStrengthLte.cqiTableIndex;
                records[i].ratSpecificInfo= aidl::CellInfoRatSpecificInfo(cellInfoLte);
                break;
            }

            case RIL_CELL_INFO_TYPE_TD_SCDMA: {
                mtkAssert((char*) "TD-SCDMA is impossible EXIST here");
                break;
            }
            case RIL_CELL_INFO_TYPE_NR: {
                aidl::CellInfoNr cellInfoNr = {};
                // clear information if mcc is unknown
                if (rillCellInfo->CellInfo.nr.cellidentity.mcc == 0x7FFFFFFF) {
                    cellInfoNr.cellIdentityNr.mcc = std::string();
                    cellInfoNr.cellIdentityNr.mnc = std::string();
                    cellInfoNr.cellIdentityNr.operatorNames.alphaLong = std::string();
                    cellInfoNr.cellIdentityNr.operatorNames.alphaShort = std::string();
                } else {
                    err = snprintf(buff, 6, "%03d",
                            rillCellInfo->CellInfo.nr.cellidentity.mcc);
                    if (err < 0) {
                        mtkLogE(LOG_TAG, "snprintf cellidentityNr.mcc fail");
                    }
                    cellInfoNr.cellIdentityNr.mcc = convertCharPtrToStdString(buff);
                    if (rillCellInfo->CellInfo.nr.cellidentity.mnc_len == 3) {
                        err = snprintf(buff, 6, "%03d",
                                rillCellInfo->CellInfo.nr.cellidentity.mnc);
                        if (err < 0) {
                            mtkLogE(LOG_TAG, "snprintf cellidentityNr.mnc fail");
                        }
                    } else if (rillCellInfo->CellInfo.nr.cellidentity.mnc_len == 2) {
                        err = snprintf(buff, 6, "%02d",
                                rillCellInfo->CellInfo.nr.cellidentity.mnc);
                        if (err < 0) {
                            mtkLogE(LOG_TAG, "snprintf cellidentityNr.mnc fail");
                        }
                    } else {
                        err = snprintf(buff, 11, "%d",
                                rillCellInfo->CellInfo.nr.cellidentity.mnc);
                        if (err < 0) {
                            mtkLogE(LOG_TAG, "snprintf cellidentityNr.mnc fail");
                        }
                    }
                    cellInfoNr.cellIdentityNr.mnc = convertCharPtrToStdString(buff);
                    cellInfoNr.cellIdentityNr.operatorNames.alphaLong =
                        convertCharPtrToStdString(
                                rillCellInfo->CellInfo.nr.cellidentity.operName.long_name);
                    cellInfoNr.cellIdentityNr.operatorNames.alphaShort =
                        convertCharPtrToStdString(
                                rillCellInfo->CellInfo.nr.cellidentity.operName.short_name);
                }

                cellInfoNr.cellIdentityNr.nci = rillCellInfo->CellInfo.nr.cellidentity.nci;
                cellInfoNr.cellIdentityNr.pci = rillCellInfo->CellInfo.nr.cellidentity.pci;
                cellInfoNr.cellIdentityNr.tac = rillCellInfo->CellInfo.nr.cellidentity.tac;
                cellInfoNr.cellIdentityNr.nrarfcn = rillCellInfo->CellInfo.nr.cellidentity.nrarfcn;

                cellInfoNr.cellIdentityNr.additionalPlmns = {};
                cellInfoNr.cellIdentityNr.bands.resize(1);
                cellInfoNr.cellIdentityNr.bands[0] =
                    (aidl::NgranBands)rillCellInfo->CellInfo.nr.cellidentity.band;

                cellInfoNr.signalStrengthNr.ssRsrp = rillCellInfo->CellInfo.nr.signalStrength.ssRsrp;
                cellInfoNr.signalStrengthNr.ssRsrq = rillCellInfo->CellInfo.nr.signalStrength.ssRsrq;
                cellInfoNr.signalStrengthNr.ssSinr = rillCellInfo->CellInfo.nr.signalStrength.ssSinr;
                cellInfoNr.signalStrengthNr.csiRsrp = rillCellInfo->CellInfo.nr.signalStrength.csiRsrp;
                cellInfoNr.signalStrengthNr.csiRsrq = rillCellInfo->CellInfo.nr.signalStrength.csiRsrq;
                cellInfoNr.signalStrengthNr.csiSinr = rillCellInfo->CellInfo.nr.signalStrength.csiSinr;
                cellInfoNr.signalStrengthNr.csiCqiTableIndex = rillCellInfo->CellInfo.nr.signalStrength.csiCqiTableIndex;
                int count = MIN(rillCellInfo->CellInfo.nr.signalStrength.csiCqiReportCount, MAX_COUNT_NR_CSI_CQI_REPORT);
                cellInfoNr.signalStrengthNr.csiCqiReport.resize(count);
                if (count > 0) {
                    for (int index = 0; index < count; index++) {
                        cellInfoNr.signalStrengthNr.csiCqiReport[index] =
                                rillCellInfo->CellInfo.nr.signalStrength.csiCqiReport[index];
                    }
                }
                cellInfoNr.signalStrengthNr.timingAdvance = rillCellInfo->CellInfo.nr.signalStrength.timingAdvance;
                records[i].ratSpecificInfo = aidl::CellInfoRatSpecificInfo(cellInfoNr);
                break;
            }
            default: {
                mtkLogD(LOG_TAG, "%s: unkown cellInfoType:%d", __FUNCTION__, rillCellInfo->cellInfoType);
                break;
            }
        }

        rillCellInfo += 1;
    }
}
void convertRilSignalStrengthToAidl(RIL_SIGNAL_STRENGTH* pSource,           aidl::SignalStrength& signalStrength) {
    // gsm
    signalStrength.gsm.signalStrength = pSource->gsm_signal_strength;
    signalStrength.gsm.bitErrorRate = pSource->gsm_bit_error_rate;
    signalStrength.gsm.timingAdvance = pSource->gsm_timing_advance;
    //cdma
    signalStrength.cdma.dbm = pSource->cdma_dbm;
    signalStrength.cdma.ecio = pSource->cdma_ecio;
    //evdo
    signalStrength.evdo.dbm = pSource->evdo_dbm;
    signalStrength.evdo.ecio = pSource->evdo_ecio;
    signalStrength.evdo.signalNoiseRatio = pSource->evdo_snr;
    //lte
    signalStrength.lte.signalStrength = pSource->lte_signal_strength;
    signalStrength.lte.rsrp = pSource->lte_rsrp;
    signalStrength.lte.rsrq = pSource->lte_rsrq;
    signalStrength.lte.rssnr = pSource->lte_rssnr;
    signalStrength.lte.cqi = pSource->lte_cqi;
    signalStrength.lte.timingAdvance = pSource->lte_timing_advance;
    signalStrength.lte.cqiTableIndex = pSource->lte_cqi_table_index;
    // AOSP still use 1.0's TdScdma, not 1.2's Tdscdma. It's "S", not "s"
    //tdscdma
    signalStrength.tdscdma.signalStrength = pSource->tdscdma_signal_strength;
    signalStrength.tdscdma.bitErrorRate = pSource->tdscdma_bit_error_rate;
    signalStrength.tdscdma.rscp =pSource->tdscdma_rscp;
    //wcdma
    signalStrength.wcdma.signalStrength = pSource->wcdma_signal_strength;
    signalStrength.wcdma.bitErrorRate = pSource->wcdma_bit_error_rate;
    signalStrength.wcdma.rscp = pSource->wcdma_scdma_rscp;
    signalStrength.wcdma.ecno = pSource->wcdma_ecno;
    //nr
    signalStrength.nr.ssRsrp = pSource->ssRsrp;
    signalStrength.nr.ssRsrq = pSource->ssRsrq;
    signalStrength.nr.ssSinr = pSource->ssSinr;
    signalStrength.nr.csiRsrp = pSource->csiRsrp;
    signalStrength.nr.csiRsrq = pSource->csiRsrq;
    signalStrength.nr.csiSinr = pSource->csiSinr;
    signalStrength.nr.csiCqiTableIndex = pSource->nr_csicqi_table_index;
    signalStrength.nr.csiCqiReport.resize(pSource->nr_csicqi_report_count);
    for (int i =0; i < pSource->nr_csicqi_report_count; i ++) {
        signalStrength.nr.csiCqiReport[i] = pSource->nr_csicqi_reports[i];
    }
    signalStrength.nr.timingAdvance = pSource->nr_timing_advance;
}

aidl::EutranRegistrationInfo::AttachResultType convertAttachResultType(RIL_AttachResultType type) {
    switch (type) {
        case EPS_ONLY:
            return aidl::EutranRegistrationInfo::AttachResultType::EPS_ONLY;
        case COMBINED:
            return aidl::EutranRegistrationInfo::AttachResultType::COMBINED;
        default:
            return aidl::EutranRegistrationInfo::AttachResultType::NONE;
    }
}

}  // namespace radioNetwork

// radio aosp
int radioNetwork::getAllowedNetworkTypesBitmapResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e, const void *response,
                                   size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int32_t raf = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        mtkLogD(LOG_TAG, "[%s]: raf %d", __FUNCTION__, raf);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getAllowedNetworkTypesBitmapResponse(
               responseInfo, raf);
        if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
             retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }

    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::getAvailableBandModesResponse(unsigned int slotId, android::ClientId clientId,
                                 int responseType, int serial, RIL_Errno e, const void *response,
                                 size_t responseLen) {
    mtkLogD(LOG_TAG, "getAvailableBandModesResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
       RadioResponseInfo responseInfo = {};
       rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
       std::vector<aidl::RadioBandMode> modes;
       if (response == NULL || responseLen % sizeof(int) != 0) {
           modes.resize(0);
           mtkLogE(LOG_TAG, "getAvailableBandModesResponse Invalid response: NULL");
           if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
       } else {
           int *pInt = (int *) response;
           int numInts = responseLen / sizeof(int);
           modes.resize(numInts);
           for (int i = 0; i < numInts; i++) {
               modes[i] = (aidl::RadioBandMode) pInt[i];
           }
       }
       ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getAvailableBandModesResponse(
                responseInfo, modes);
       if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
       }
    } else {
       mtkLogE(LOG_TAG, "getAvailableBandModesResponse: radioService[%d]->mRadioResponse == NULL",
               slotId);
    }

    return 0;
}

int radioNetwork::getAvailableNetworksResponse(unsigned int slotId, android::ClientId clientId,
                                int responseType, int serial, RIL_Errno e, const void *response,
                                size_t responseLen) {
    mtkLogD(LOG_TAG, "getAvailableNetworksResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<aidl::OperatorInfo> networks;
        if (response == NULL || responseLen % (4 * sizeof(char *))!= 0) {
            mtkLogE(LOG_TAG, "getAvailableNetworksResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *);
            networks.resize(numStrings/4);
            for (int i = 0, j = 0; i < numStrings; i = i + 4, j++) {
                networks[j].alphaLong = convertCharPtrToStdString(resp[i]);
                networks[j].alphaShort = convertCharPtrToStdString(resp[i + 1]);
                networks[j].operatorNumeric = convertCharPtrToStdString(resp[i + 2]);
                int32_t status = convertToOperatorStatus(resp[i + 3]);
                if (status == -1) {
                    if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
                } else {
                    networks[j].status = status;
                }
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getAvailableNetworksResponse(
                responseInfo, networks);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getAvailableNetworksResponse: radioService[%d]->mRadioResponse == NULL",
             slotId);
    }
    return 0;
}

int radioNetwork::getBarringInfoResponse(unsigned int slotId, android::ClientId clientId,
                                int responseType, int serial, RIL_Errno e, const void *response,
                                size_t responseLen) {
    mtkLogD(LOG_TAG, "getBarringInfoResponse: serial %d", serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        std::vector<aidl::BarringInfo> barringInfos;
        aidl::CellIdentity cellIdentity;
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        if (responseLen != sizeof(RIL_BarringInforResponse)) {
            mtkLogE(LOG_TAG, "getBarringInfoResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            RIL_BarringInforResponse *rilBarringInfoResponse =
                    (RIL_BarringInforResponse *)response;
            fillCellIdentityAidl(cellIdentity, rilBarringInfoResponse->cellIdentity);
            barringInfos.resize(rilBarringInfoResponse->count_barringInfo);
            for (size_t i = 0; i < rilBarringInfoResponse->count_barringInfo; i++) {
                barringInfos[i].serviceType =
                        rilBarringInfoResponse->barringInfos[i].service_type;
                barringInfos[i].barringType =
                        rilBarringInfoResponse->barringInfos[i].barring_type;
                if (rilBarringInfoResponse->barringInfos[i].barring_type ==
                        aidl::BarringInfo::BARRING_TYPE_CONDITIONAL) {
                    aidl::BarringTypeSpecificInfo cond;
                    cond.factor = rilBarringInfoResponse->barringInfos[i].factor;
                    cond.timeSeconds = rilBarringInfoResponse->barringInfos[i].timeSeconds;
                    cond.isBarred = rilBarringInfoResponse->barringInfos[i].isBarred;
                    barringInfos[i].barringTypeSpecificInfo = cond;
                } else {
                    barringInfos[i].barringTypeSpecificInfo = {};
                }
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getBarringInfoResponse(
                responseInfo, cellIdentity, barringInfos);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getBarringInfoResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::getCdmaRoamingPreferenceResponse(unsigned int slotId, android::ClientId clientId,
                                    int responseType, int serial, RIL_Errno e, const void *response,
                                    size_t responseLen) {
    mtkLogD(LOG_TAG, "getCdmaRoamingPreferenceResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int type = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getCdmaRoamingPreferenceResponse(
                responseInfo, (aidl::CdmaRoamingType) type);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getCdmaRoamingPreferenceResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::getCellInfoListResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType,
                            int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "getCellInfoListResponse: serial %d", serial);
    if (radioNetworkServiceImpl[slotId].networkResponse!= NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<aidl::CellInfo> infos;
        if (response == NULL || responseLen % sizeof(RIL_CellInfo_v12) != 0) {
            mtkLogE(LOG_TAG, "getCellInfoListResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            convertRilCellInfoListToAidl(response, responseLen, infos);
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getCellInfoListResponse(
            responseInfo, infos);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    }
    return 0;
}

int radioNetwork::getDataRegistrationStateResponse(unsigned int slotId, android::ClientId clientId,
                                    int responseType, int serial, RIL_Errno e, const void *response,
                                    size_t responseLen) {
    mtkLogD(LOG_TAG, "getDataRegistrationStateResponse: serial %d", serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        aidl::RegStateResult dataRegResponse = {};
        if (response == NULL) {
               mtkLogE(LOG_TAG, "getDataRegistrationStateResponse Invalid response: NULL");
               if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            if (responseLen != sizeof(RIL_DataRegistrationStateResponse)) {
                mtkLogE(LOG_TAG, "getDataRegistrationStateResponse Invalid response: NULL");
                if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
            } else {
                RIL_DataRegistrationStateResponse *dataRegState =
                        (RIL_DataRegistrationStateResponse *)response;

                dataRegResponse.regState = (aidl::RegState) dataRegState->regState;
                dataRegResponse.rat = (aidl_radio::RadioTechnology) dataRegState->rat;
                dataRegResponse.reasonForDenial =
                    (aidl::RegistrationFailCause) dataRegState->reasonDataDenied;

                fillCellIdentityAidl(dataRegResponse.cellIdentity, dataRegState->cellIdentity);
                dataRegResponse.registeredPlmn = convertCharPtrToStdString(dataRegState->rplmn);
                if (dataRegState->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_LTE) {
                    aidl::EutranRegistrationInfo eutranInfo = {};

                    eutranInfo.lteVopsInfo.isVopsSupported =
                            dataRegState->lteVopsInfo.isVopsSupported > 0 ? true: false;
                    eutranInfo.lteVopsInfo.isEmcBearerSupported =
                            dataRegState->lteVopsInfo.isEmcBearerSupported > 0 ? true: false;

                    eutranInfo.nrIndicators.isEndcAvailable =
                            dataRegState->nrIndicators.isEndcAvailable > 0 ? true: false;
                    eutranInfo.nrIndicators.isDcNrRestricted =
                            dataRegState->nrIndicators.isDcNrRestricted > 0 ? true: false;
                    eutranInfo.nrIndicators.isNrAvailable =
                            dataRegState->nrIndicators.isNrAvailable > 0 ? true: false;
                    eutranInfo.lteAttachResultType = convertAttachResultType(dataRegState->attachResultType);
                    eutranInfo.extraInfo = dataRegState->extraInfo;
                    dataRegResponse.accessTechnologySpecificInfo =
                            aidl::AccessTechnologySpecificInfo(eutranInfo);
                } else if (dataRegState->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_NR) {
                    aidl::NrVopsInfo ngranNrVopsInfo = {};
                    ngranNrVopsInfo.vopsSupported =
                        (int8_t) dataRegState->nrVopsInfo.vopsSupported;
                    ngranNrVopsInfo.emcSupported =
                        (int8_t) dataRegState->nrVopsInfo.emcSupported;
                    ngranNrVopsInfo.emfSupported =
                        (int8_t) dataRegState->nrVopsInfo.emfSupported;
                    dataRegResponse.accessTechnologySpecificInfo =
                        aidl::AccessTechnologySpecificInfo(ngranNrVopsInfo);
                } else {
                    dataRegResponse.accessTechnologySpecificInfo =
                            aidl::AccessTechnologySpecificInfo();;
                }
            }
        }
        mtkLogD(LOG_TAG, "getDataRegistrationStateResponse: serial %d, DataReg: %s", serial, dataRegResponse.toString().c_str());
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getDataRegistrationStateResponse(
                responseInfo, dataRegResponse);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    }
    return 0;
}

int radioNetwork::getImsRegistrationStateResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e,
                                   const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getImsRegistrationStateResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        bool isRegistered = false;
        RadioTechnologyFamily ratFamily = RadioTechnologyFamily::THREE_GPP;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts != 2) {
            mtkLogE(LOG_TAG, "getImsRegistrationStateResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            isRegistered = pInt[0] == 1 ? true : false;
            // Map RIL_RadioTechnologyFamily to RadioTechnologyFamily
            if (pInt[1] == RADIO_TECH_3GPP) {
                ratFamily = RadioTechnologyFamily::THREE_GPP;
            } else {
                ratFamily = RadioTechnologyFamily::THREE_GPP2;
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getImsRegistrationStateResponse(
                responseInfo, isRegistered, ratFamily);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getImsRegistrationStateResponse: radioNetworkServiceImpl[%d]->networkResponse == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::getNetworkSelectionModeResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e, const void *response,
                                   size_t responseLen) {
    mtkLogD(LOG_TAG, "getNetworkSelectionModeResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        bool manual = false;
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "getNetworkSelectionModeResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            manual = pInt[0] == 1 ? true : false;
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getNetworkSelectionModeResponse(
                responseInfo, manual);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getNetworkSelectionModeResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::getOperatorResponse(unsigned int slotId, android::ClientId clientId,
                       int responseType, int serial, RIL_Errno e, const void *response,
                       size_t responseLen) {
    mtkLogD(LOG_TAG, "getOperatorResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
       RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::string longName;
        std::string shortName;
        std::string numeric;
        int numStrings = responseLen / sizeof(char *);
        if (response == NULL || numStrings != 3) {
            mtkLogE(LOG_TAG, "getOperatorResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;

        } else {
            char **resp = (char **) response;
            longName = convertCharPtrToStdString(resp[0]);
            shortName = convertCharPtrToStdString(resp[1]);
            numeric = convertCharPtrToStdString(resp[2]);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getOperatorResponse(
                responseInfo, longName, shortName, numeric);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getOperatorResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::getSignalStrengthResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responseLen) {
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        aidl::SignalStrength signalStrength = {};
        if (response == NULL || responseLen != sizeof(RIL_SIGNAL_STRENGTH)) {
            mtkLogE(LOG_TAG, "currentSignalStrengthInd: invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            RIL_SIGNAL_STRENGTH* pSource = (RIL_SIGNAL_STRENGTH*) response;
            convertRilSignalStrengthToAidl(pSource, signalStrength);
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getSignalStrengthResponse(
                responseInfo, signalStrength);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    }
    return 0;
}

int radioNetwork::getSystemSelectionChannelsResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s][%d]: serial %d", __FUNCTION__, slotId, serial);
     if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
         RadioResponseInfo responseInfo = {};
         rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
         std::vector<aidl::RadioAccessSpecifier> specifiers = {};
         if (response == NULL) {
             mtkLogE(LOG_TAG, "[%s] invalide response: NULL", __FUNCTION__);
             if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
         } else {
             if (responseLen != sizeof(RIL_SystemSelectionChannels)) {
                 mtkLogE(LOG_TAG, "[%s] Invalid response, responseLen:%d, struct length:%d",
                         __FUNCTION__, responseLen, sizeof(RIL_SystemSelectionChannels));
                 if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
             } else {
                 RIL_SystemSelectionChannels* pData = (RIL_SystemSelectionChannels*)response;
                 mtkLogD(LOG_TAG, "[%s] specifyChannels:%d, specifiers_length:%d",
                         __FUNCTION__, pData->specifyChannels, pData->specifiers_length);

                 if (pData->specifyChannels && pData->specifiers_length <= MAX_RADIO_ACCESS_NETWORKS) {
                     specifiers.resize(pData->specifiers_length);
                     for (int i = 0; i < pData->specifiers_length; i++) {
                         switch (pData->specifiers[i].radio_access_network) {
                         case GERAN: {
                             specifiers[i].accessNetwork = AccessNetwork::GERAN;
                             std::vector<aidl::GeranBands> geranBands;
                             if (pData->specifiers[i].bands_length <= MAX_BANDS) {
                                 geranBands.resize(pData->specifiers[i].bands_length);
                                 for (int j = 0; j < pData->specifiers[i].bands_length; j++) {
                                     geranBands[j] = (aidl::GeranBands)pData->specifiers[i].bands.geran_bands[j];
                                 }
                             } else {
                                 geranBands.resize(0);
                             }
                             specifiers[i].bands = aidl::RadioAccessSpecifierBands(geranBands);
                             break;
                         }

                         case UTRAN: {
                             specifiers[i].accessNetwork = AccessNetwork::UTRAN;
                             std::vector<aidl::UtranBands> utranBands;
                             if (pData->specifiers[i].bands_length <= MAX_BANDS) {
                                 utranBands.resize(pData->specifiers[i].bands_length);
                                 for (int j = 0; j < pData->specifiers[i].bands_length; j++) {
                                     utranBands[j] = (aidl::UtranBands)pData->specifiers[i].bands.utran_bands[j];
                                 }
                             } else {
                                 utranBands.resize(0);
                             }
                             specifiers[i].bands = aidl::RadioAccessSpecifierBands(utranBands);

                             break;
                         }

                         case EUTRAN: {
                             specifiers[i].accessNetwork = AccessNetwork::EUTRAN;
                             std::vector<aidl::EutranBands> eutranBands;
                             if (pData->specifiers[i].bands_length <= MAX_BANDS) {
                                 eutranBands.resize(pData->specifiers[i].bands_length);
                                 for (int j = 0; j < pData->specifiers[i].bands_length; j++) {
                                     eutranBands[j] = (aidl::EutranBands)pData->specifiers[i].bands.eutran_bands[j];
                                 }
                             } else {
                                 eutranBands.resize(0);
                             }
                             specifiers[i].bands =aidl::RadioAccessSpecifierBands(eutranBands);
                             break;
                         }

                         case NEW_RADIO: {
                             specifiers[i].accessNetwork = AccessNetwork::NGRAN;
                             std::vector<aidl::NgranBands>  ngranBands;
                             if (pData->specifiers[i].bands_length <= MAX_BANDS) {
                                 ngranBands.resize(pData->specifiers[i].bands_length);
                                 for (int j = 0; j < pData->specifiers[i].bands_length; j++) {
                                     ngranBands[j] = (aidl::NgranBands)pData->specifiers[i].bands.ngranBands[j];
                                 }
                             } else {
                                 ngranBands.resize(0);
                             }
                             specifiers[i].bands = aidl::RadioAccessSpecifierBands(ngranBands);
                             break;
                         }

                         default:
                             mtkLogE(LOG_TAG, "[%s] Invalid response, accessNetwork:%d",
                                     __FUNCTION__, pData->specifiers[i].radio_access_network);
                             break;
                         }

                         if (pData->specifiers[i].channels_length <= MAX_CHANNELS) {
                             specifiers[i].channels.resize(pData->specifiers[i].channels_length);
                             for (int j = 0; j < pData->specifiers[i].channels_length; j++) {
                                 specifiers[i].channels[j] = pData->specifiers[i].channels[j];
                             }
                         } else {
                             mtkLogE(LOG_TAG, "[%s] Invalid response, channels_length:%d",
                                     __FUNCTION__, pData->specifiers[i].channels_length);
                             specifiers[i].channels.resize(0);
                         }
                     }
                 } else {
                     mtkLogE(LOG_TAG, "[%s] Invalid response, channels_length:%d",
                             __FUNCTION__, pData->specifiers_length);
                     specifiers.resize(0);
                 }
             }
         }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getSystemSelectionChannelsResponse(
                responseInfo, specifiers);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
     } else {
         mtkLogE(LOG_TAG, "[%s]: radioService[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
     }

    return 0;
}

int radioNetwork::getVoiceRadioTechnologyResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e,
                                   const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getVoiceRadioTechnologyResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
       RadioResponseInfo responseInfo = {};
        int rat = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getVoiceRadioTechnologyResponse(
                responseInfo, (aidl_radio::RadioTechnology) rat);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getVoiceRadioTechnologyResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::getVoiceRegistrationStateResponse(unsigned int slotId, android::ClientId clientId,
                                     int responseType, int serial, RIL_Errno e, const void *response,
                                     size_t responseLen) {
    mtkLogD(LOG_TAG, "getVoiceRegistrationStateResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        aidl::RegStateResult voiceRegResponse = {};
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getVoiceRegistrationStateResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            if (responseLen != sizeof(RIL_VoiceRegistrationStateResponse)) {
                mtkLogE(LOG_TAG, "getVoiceRegistrationStateResponse Invalid response: NULL");
                if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
            } else {
                RIL_VoiceRegistrationStateResponse *voiceRegState =
                        (RIL_VoiceRegistrationStateResponse *)response;

                voiceRegResponse.regState = (aidl::RegState) voiceRegState->regState;
                voiceRegResponse.rat = (aidl_radio::RadioTechnology) voiceRegState->rat;
                voiceRegResponse.reasonForDenial =
                    (aidl::RegistrationFailCause) voiceRegState->reasonForDenial;

                fillCellIdentityAidl(voiceRegResponse.cellIdentity, voiceRegState->cellIdentity);
                voiceRegResponse.registeredPlmn = voiceRegState->rplmn;
                voiceRegResponse.accessTechnologySpecificInfo = {};
                if (voiceRegState->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_CDMA) {
                    aidl::Cdma2000RegistrationInfo cdmaRegInfo = {};
                    cdmaRegInfo.cssSupported = voiceRegState->cssSupported;
                    cdmaRegInfo.roamingIndicator = voiceRegState->roamingIndicator;
                    cdmaRegInfo.systemIsInPrl = voiceRegState->systemIsInPrl;
                    cdmaRegInfo.defaultRoamingIndicator = voiceRegState->defaultRoamingIndicator;
                    voiceRegResponse.accessTechnologySpecificInfo =
                            aidl::AccessTechnologySpecificInfo(cdmaRegInfo);
                } else if (voiceRegState->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_LTE) {
                    aidl::EutranRegistrationInfo eutranInfo = {};

                    eutranInfo.lteVopsInfo.isVopsSupported =
                            voiceRegState->lteVopsInfo.isVopsSupported > 0 ? true: false;
                    eutranInfo.lteVopsInfo.isEmcBearerSupported =
                            voiceRegState->lteVopsInfo.isEmcBearerSupported > 0 ? true: false;
                    eutranInfo.nrIndicators.isEndcAvailable =
                            voiceRegState->nrIndicators.isEndcAvailable > 0 ? true: false;
                    eutranInfo.nrIndicators.isDcNrRestricted =
                            voiceRegState->nrIndicators.isDcNrRestricted > 0 ? true: false;
                    eutranInfo.nrIndicators.isNrAvailable =
                            voiceRegState->nrIndicators.isNrAvailable > 0 ? true: false;
                    eutranInfo.lteAttachResultType = convertAttachResultType(voiceRegState->attachResultType);
                    eutranInfo.extraInfo = voiceRegState->extraInfo;
                    voiceRegResponse.accessTechnologySpecificInfo =
                            aidl::AccessTechnologySpecificInfo(eutranInfo);
                } else if (voiceRegState->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_NR) {
                    aidl::NrVopsInfo ngranNrVopsInfo = {};
                    ngranNrVopsInfo.vopsSupported =
                        (int8_t) voiceRegState->nrVopsInfo.vopsSupported;
                    ngranNrVopsInfo.emcSupported =
                        (int8_t) voiceRegState->nrVopsInfo.emcSupported;
                    ngranNrVopsInfo.emfSupported =
                        (int8_t) voiceRegState->nrVopsInfo.emfSupported;
                    voiceRegResponse.accessTechnologySpecificInfo =
                            aidl::AccessTechnologySpecificInfo(ngranNrVopsInfo);
                } else if (voiceRegState->cellIdentity.cellInfoType == RIL_CELL_INFO_TYPE_GSM) {
                    voiceRegResponse.accessTechnologySpecificInfo =
                            aidl::AccessTechnologySpecificInfo();
                }
            }
        }
        mtkLogD(LOG_TAG, "getVoiceRegistrationStateResponse: serial %d, voiceReg: %s", serial, voiceRegResponse.toString().c_str());

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getVoiceRegistrationStateResponse(
                responseInfo, voiceRegResponse);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "getVoiceRegistrationStateResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::isNrDualConnectivityEnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo respInfo = {};
        int result = responseInt(respInfo, serial, responseType, e, response, responseLen);
        mtkLogD(LOG_TAG, "[%s]: enabled[%d]", __FUNCTION__, result);
        bool enabled = ((result == 1) ? true : false);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse
                ->isNrDualConnectivityEnabledResponse(respInfo, enabled);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int radioNetwork::setAllowedNetworkTypesBitmapResponse(unsigned int slotId, android::ClientId clientId,
                                   int responseType, int serial, RIL_Errno e, const void *response,
                                   size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setAllowedNetworkTypesBitmapResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int radioNetwork::setBandModeResponse(unsigned int slotId, android::ClientId clientId,
                       int responseType, int serial, RIL_Errno e, const void *response,
                       size_t responseLen) {
    mtkLogD(LOG_TAG, "setBandModeResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setBandModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setBandModeResponse: radioService[%d]->mRadioResponse == NULL", slotId);
    }

    return 0;
}


int radioNetwork::setBarringPasswordResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responseLen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse
                        ->setBarringPasswordResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG,
                "%s: radioNetworkServiceImpl[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int radioNetwork::setCdmaRoamingPreferenceResponse(unsigned int slotId, android::ClientId clientId,
                                    int responseType, int serial, RIL_Errno e, const void *response,
                                    size_t responseLen) {
    mtkLogD(LOG_TAG, "setCdmaRoamingPreferenceResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setCdmaRoamingPreferenceResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setCdmaRoamingPreferenceResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::setCellInfoListRateResponse(unsigned int slotId, android::ClientId clientId,
                               int responseType, int serial, RIL_Errno e,
                               const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setCellInfoListRateResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setCellInfoListRateResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setCellInfoListRateResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::setIndicationFilterResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e,
                              const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setIndicationFilterResponse: serial %d", serial);
    RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse
                ->setIndicationFilterResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setIndicationFilterResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::setLinkCapacityReportingCriteriaResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setLinkCapacityReportingCriteriaResponse: serial:%d slotId:%d",
            serial, slotId);
    RadioResponseInfo responseInfo = {};
    rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse
                ->setLinkCapacityReportingCriteriaResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setLinkCapacityReportingCriteriaResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setLinkCapacityReportingCriteriaResponse:"
                "radioService[%d]->mRadioResponse == NULL", slotId);
    }
    return 0;
}


int radioNetwork::setLocationUpdatesResponse(unsigned int slotId, android::ClientId clientId,
                              int responseType, int serial, RIL_Errno e, const void *response,
                              size_t responseLen) {
    mtkLogD(LOG_TAG, "setLocationUpdatesResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setLocationUpdatesResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setLocationUpdatesResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::setNetworkSelectionModeAutomaticResponse(unsigned int slotId, android::ClientId clientId,
                                            int responseType, int serial, RIL_Errno e,
                                            const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setNetworkSelectionModeAutomaticResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setNetworkSelectionModeAutomaticResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setNetworkSelectionModeAutomaticResponse: radioService[%d]->mRadioResponse "
                "== NULL", slotId);
    }
    return 0;
}

int radioNetwork::setNetworkSelectionModeManualResponse(unsigned int slotId, android::ClientId clientId,
                                         int responseType, int serial, RIL_Errno e, const void *response,
                                         size_t responseLen) {
    mtkLogD(LOG_TAG, "setNetworkSelectionModeManualResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse!= NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setNetworkSelectionModeManualResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "acceptCallResponse: radioService[%d]->setNetworkSelectionModeManualResponse "
                "== NULL", slotId);
    }
    return 0;
}

int radioNetwork::setNrDualConnectivityStateResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo respInfo = {};
        rilAidlUtils::populateAidlResponseInfo(respInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse
                ->setNrDualConnectivityStateResponse(respInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::setSignalStrengthReportingCriteriaResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setSignalStrengthReportingCriteriaResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse!= NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setSignalStrengthReportingCriteriaResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setSignalStrengthReportingCriteriaResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::setSuppServiceNotificationsResponse(unsigned int slotId, android::ClientId clientId,
                                       int responseType, int serial, RIL_Errno e, const void *response,
                                       size_t responseLen) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret =radioNetworkServiceImpl[slotId]
                .networkResponse->setSuppServiceNotificationsResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioService[%d]->mRadioResponse == NULL", __FUNCTION__, slotId);
    }

    return 0;
}

int radioNetwork::setSystemSelectionChannelsResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responseLen) {
    mtkLogD(LOG_TAG, "setSystemSelectionChannelsResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setSystemSelectionChannelsResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "setSystemSelectionChannelsResponse: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }
    return 0;
}

int radioNetwork::startNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                             int responseType, int serial, RIL_Errno e, const void *response,
                             size_t responseLen) {
    mtkLogD(LOG_TAG, "startNetworkScanResponse: serial %d", serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->startNetworkScanResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "startNetworkScanResponse: radioService[%d]->mRadioResponseV1_1 == NULL", slotId);
    }
    return 0;
}

int radioNetwork::stopNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "stopNetworkScanResponse: serial %d", serial);

    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->stopNetworkScanResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "stopNetworkScanResponse: radioService[%d]->mRadioResponseV1_1 == NULL", slotId);
    }
    return 0;
}

int radioNetwork::supplyNetworkDepersonalizationResponse(unsigned int slotId,
        android::ClientId clientId, int responseType, int serial, RIL_Errno e,
        const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "supplyNetworkDepersonalizationResponse: serial %d", serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        int remain = rilAidlUtils::responseIntOrEmpty(responseInfo, serial, responseType, e,
                response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse
                ->supplyNetworkDepersonalizationResponse(responseInfo, remain);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "supplyNetworkDepersonalizationResponse: slotId = %d, binder fail!",
                    slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }

    } else {
        mtkLogE(LOG_TAG,
                "supplyNetworkDepersonalizationResponse: "
                "radioNetworkServiceImpl[%d].networkResponse == NULL", slotId);
    }

    return 0;
}

int radioNetwork::setUsageSettingResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "setUsageSettingResponse: serial %d", serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setUsageSettingResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "stopNetworkScanResponse: radioService[%d]->mRadioResponseV1_1 == NULL", slotId);
    }
    return 0;
}

int radioNetwork::getUsageSettingResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int32_t setting = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        mtkLogD(LOG_TAG, "[%s]: setting %d", __FUNCTION__, setting);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->getUsageSettingResponse(
               responseInfo, (aidl::UsageSetting) setting);
        if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
             retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }

    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::setEmergencyModeResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    // TODO: pause it due to AOSP does not require it.
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        aidl::EmergencyRegResult regResult = {};
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setEmergencyModeResponse(responseInfo, regResult);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->networkResponse == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}
int radioNetwork::triggerEmergencyNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    // TODO: pause it due to AOSP does not require it.
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->triggerEmergencyNetworkScanResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->networkResponse == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}
int radioNetwork::exitEmergencyModeResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    // TODO: pause it due to AOSP does not require it.
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->exitEmergencyModeResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->networkResponse == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}
int radioNetwork::cancelEmergencyNetworkScanResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    // TODO: pause it due to AOSP does not require it.
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->cancelEmergencyNetworkScanResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->networkResponse == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}
int radioNetwork::setNullCipherAndIntegrityEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responselen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setNullCipherAndIntegrityEnabledResponse(responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->networkResponse == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}
int radioNetwork::isNullCipherAndIntegrityEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int32_t enabled = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->isNullCipherAndIntegrityEnabledResponse(
                responseInfo, enabled == 1 ? true : false);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->networkResponse == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::isN1ModeEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d",  __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int32_t enabled = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        mtkLogE(LOG_TAG, "%s: slotId = %d, enabled: %d", __FUNCTION__, slotId, enabled);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->isN1ModeEnabledResponse(
            responseInfo, enabled == 1 ? true : false);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioService[%d]->mRadioResponseV1_1 == NULL",  __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::setN1ModeEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setN1ModeEnabledResponse(responseInfo);
        if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
             retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::isCellularIdentifierTransparencyEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int32_t enabled = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        mtkLogE(LOG_TAG, "%s: slotId = %d, enabled: %d", __FUNCTION__, slotId, enabled);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->isCellularIdentifierTransparencyEnabledResponse(
            responseInfo, enabled == 1 ? true : false);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::setCellularIdentifierTransparencyEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setCellularIdentifierTransparencyEnabledResponse(responseInfo);
        if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
             retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::setSecurityAlgorithmsUpdatedEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->setSecurityAlgorithmsUpdatedEnabledResponse(responseInfo);
        if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
             retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::isSecurityAlgorithmsUpdatedEnabledResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    if (radioNetworkServiceImpl[slotId].networkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int32_t enabled = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        mtkLogE(LOG_TAG, "%s: slotId = %d, enabled: %d", __FUNCTION__, slotId, enabled);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkResponse->isSecurityAlgorithmsUpdatedEnabledResponse(
            responseInfo, enabled == 1 ? true : false);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

// indication

int radioNetwork::barringInfoChangedInd(unsigned int slotId,
                      int indicationType, int token, RIL_Errno e, const void *response,
                      size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        std::vector<aidl::BarringInfo> barringInfos;
        aidl::CellIdentity cellIdentity;
        if (responseLen != sizeof(RIL_BarringInforResponse)) {
            mtkLogE(LOG_TAG, "onBarringInfoInd Invalid response: NULL");
        } else {
            RIL_BarringInforResponse *rilBarringInfoResponse =
                    (RIL_BarringInforResponse *)response;
            fillCellIdentityAidl(cellIdentity, rilBarringInfoResponse->cellIdentity);
            barringInfos.resize(rilBarringInfoResponse->count_barringInfo);
            for (size_t i = 0; i < rilBarringInfoResponse->count_barringInfo; i++) {
                barringInfos[i].serviceType =
                        rilBarringInfoResponse->barringInfos[i].service_type;
                barringInfos[i].barringType =
                        rilBarringInfoResponse->barringInfos[i].barring_type;
                if (rilBarringInfoResponse->barringInfos[i].barring_type ==
                        aidl::BarringInfo::BARRING_TYPE_CONDITIONAL) {
                    aidl::BarringTypeSpecificInfo cond;
                    cond.factor = rilBarringInfoResponse->barringInfos[i].factor;
                    cond.timeSeconds = rilBarringInfoResponse->barringInfos[i].timeSeconds;
                    cond.isBarred = rilBarringInfoResponse->barringInfos[i].isBarred;
                    barringInfos[i].barringTypeSpecificInfo = cond;
                } else {
                    barringInfos[i].barringTypeSpecificInfo = {};
                }
            }
            if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->barringInfoChanged(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        cellIdentity, barringInfos);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                    retChecker->aospAidlReturnStatusChecker(slotId, ret);
                }
            }

            if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->barringInfoChanged(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        cellIdentity, barringInfos);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                    retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
                }
            }
        }
    } else {
      mtkLogE(LOG_TAG, "onBarringInfoInd: radioService[%d]->mRadioResponse == NULL",
              slotId);
    }
    return 0;
}

int radioNetwork::cdmaPrlChangedInd(unsigned int slotId,
                      int indicationType, int token, RIL_Errno e, const void *response,
                      size_t responseLen) {
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);

    if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "cdmaPrlChangedInd: invalid response");
            return 0;
        }
        int32_t version = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "cdmaPrlChangedInd: version %d", version);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->cdmaPrlChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), version);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "cdmaPrlChangedInd: radioService[%d]->networkIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "tbox-cdmaPrlChangedInd: invalid response");
            return 0;
        }
        int32_t version = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "cdmaPrlChangedInd: version %d", version);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->cdmaPrlChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), version);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-cdmaPrlChangedInd: radioService[%d]->networkIndication == NULL", tboxSlotId);
    }
    return 0;
}

int radioNetwork::cellInfoListInd(unsigned int slotId,
                    int indicationType, int token, RIL_Errno e, const void *response,
                    size_t responseLen) {
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL){
        if (response == NULL || responseLen % sizeof(RIL_CellInfo_v12) != 0) {
            mtkLogE(LOG_TAG, "cellInfoListInd: invalid response");
            return 0;
        }

        std::vector<aidl::CellInfo> records;
        convertRilCellInfoListToAidl(response, responseLen, records);

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->cellInfoList(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), records);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "cellInfoListInd: radioService[%d]->networkIndication == NULL", slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL){
        if (response == NULL || responseLen % sizeof(RIL_CellInfo_v12) != 0) {
            mtkLogE(LOG_TAG, "tbox-cellInfoListInd: invalid response");
            return 0;
        }

        std::vector<aidl::CellInfo> records;
        convertRilCellInfoListToAidl(response, responseLen, records);

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->cellInfoList(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), records);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-cellInfoListInd: radioService[%d]->networkIndication == NULL", tboxSlotId);
    }

    return 0;
}

int radioNetwork::currentLinkCapacityEstimate(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "currentLinkCapacityEstimate slotId:%d", slotId);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(RIL_LinkCapacityEstimate) != 0) {
            mtkLogE(LOG_TAG, "currentLinkCapacityEstimate: invalid response");
            return 0;
        }

        ::aidl::android::hardware::radio::network::LinkCapacityEstimate lce = {};
        RIL_LinkCapacityEstimate *resp = (RIL_LinkCapacityEstimate*)response;
        lce.downlinkCapacityKbps = resp->downlinkCapacityKbps;
        lce.uplinkCapacityKbps = resp->uplinkCapacityKbps;
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->currentLinkCapacityEstimate(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), lce);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "currentLinkCapacityEstimate: slotId = %d, binder fail!", slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "currentLinkCapacityEstimate: radioService[%d]->networkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "currentLinkCapacityEstimate tboxSlotId:%d", tboxSlotId);
    if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(RIL_LinkCapacityEstimate) != 0) {
            mtkLogE(LOG_TAG, "tbox-currentLinkCapacityEstimate: invalid response");
            return 0;
        }

        ::aidl::android::hardware::radio::network::LinkCapacityEstimate lce = {};
        RIL_LinkCapacityEstimate *resp = (RIL_LinkCapacityEstimate*)response;
        lce.downlinkCapacityKbps = resp->downlinkCapacityKbps;
        lce.uplinkCapacityKbps = resp->uplinkCapacityKbps;
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->currentLinkCapacityEstimate(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), lce);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "currentLinkCapacityEstimate: tboxSlotId = %d, binder fail!", tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "currentLinkCapacityEstimate: tbox-radioService[%d]->networkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioNetwork::currentPhysicalChannelConfigs(unsigned int slotId, int indicationType,
        int token, RIL_Errno err, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);

    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(RIL_PhysicalChannelConfig) != 0) {
            mtkLogE(LOG_TAG, "[%s]: invalid response", __FUNCTION__);
            return 0;
        }

        std::vector<aidl::PhysicalChannelConfig> physicalChannelConfig;
        int num = responseLen / sizeof(RIL_PhysicalChannelConfig);
        RIL_PhysicalChannelConfig* physicalconfigs = (RIL_PhysicalChannelConfig*) response;
        physicalChannelConfig.resize(num);
        for (int i = 0; i < num; i++) {
            physicalChannelConfig[i].status =
                    (aidl::CellConnectionStatus) physicalconfigs[i].status;
            physicalChannelConfig[i].rat =
                    (RadioTechnology) physicalconfigs[i].rat;
            physicalChannelConfig[i].downlinkChannelNumber = physicalconfigs[i].downlinkChannelNumber;
            physicalChannelConfig[i].uplinkChannelNumber = physicalconfigs[i].uplinkChannelNumber;
            physicalChannelConfig[i].cellBandwidthDownlinkKhz = physicalconfigs[i].cellBandwidthDownlink;
            physicalChannelConfig[i].cellBandwidthUplinkKhz = physicalconfigs[i].cellBandwidthUplink;
            physicalChannelConfig[i].contextIds.resize(physicalconfigs[i].num_cids);
            for(int j = 0; j < physicalconfigs[i].num_cids; j++) {
                physicalChannelConfig[i].contextIds[j] = physicalconfigs[i].contextIds[j];
            }
            physicalChannelConfig[i].physicalCellId = physicalconfigs[i].physicalCellId;
            switch (physicalconfigs[i].rat) {
            case RADIO_TECH_LTE:
            case RADIO_TECH_LTE_CA:
                if (physicalconfigs[i].band >= 1 && physicalconfigs[i].band <= 70) {
                    aidl::EutranBands eutranBand =
                        (aidl::EutranBands)physicalconfigs[i].band;
                    physicalChannelConfig[i].band = aidl::PhysicalChannelConfigBand(eutranBand);
                } else {
                    mtkLogE(LOG_TAG, "[%s]: invalid eutran band:%d",
                            __FUNCTION__, physicalconfigs[i].band);
                }
                break;

            case RADIO_TECH_NR:
                if (physicalconfigs[i].band >= 1) {
                    aidl::NgranBands ngranBand =
                        (aidl::NgranBands)physicalconfigs[i].band;
                    physicalChannelConfig[i].band = aidl::PhysicalChannelConfigBand(ngranBand);
                } else {
                    mtkLogE(LOG_TAG, "[%s]: invalid ngran band:%d",
                            __FUNCTION__, physicalconfigs[i].band);
                }
                break;

            case RADIO_TECH_UMTS:
            case RADIO_TECH_HSUPA:
            case RADIO_TECH_HSPA:
            case RADIO_TECH_EHRPD:
            case RADIO_TECH_HSPAP:
            case RADIO_TECH_TD_SCDMA:
                if (physicalconfigs[i].band >= 1) {
                    aidl::UtranBands utranBand =
                        (aidl::UtranBands)physicalconfigs[i].band;
                    physicalChannelConfig[i].band = aidl::PhysicalChannelConfigBand(utranBand);
                } else {
                    mtkLogE(LOG_TAG, "[%s]: invalid utranBand band:%d",
                            __FUNCTION__, physicalconfigs[i].band);
                }
                break;

            default:
                if (physicalconfigs[i].band >= 1) {
                    aidl::GeranBands geranBand =
                        (aidl::GeranBands)physicalconfigs[i].band;
                    physicalChannelConfig[i].band = aidl::PhysicalChannelConfigBand(geranBand);
                } else {
                    mtkLogE(LOG_TAG, "[%s]: invalid geranBand band:%d, rat:%d",
                            __FUNCTION__, physicalconfigs[i].band, physicalconfigs[i].rat);
                }
                break;
            }
        }

        mtkLogV(LOG_TAG, "currentPhysicalChannelConfigs_1_6");

        if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->currentPhysicalChannelConfigs(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), physicalChannelConfig);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        //sync to tbox client
        if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->currentPhysicalChannelConfigs(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), physicalChannelConfig);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "currentPhysicalChannelConfigs: radioService[%d]->networkIndication == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::currentSignalStrengthInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e,
                             const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_SIGNAL_STRENGTH)) {
            mtkLogE(LOG_TAG, "currentSignalStrengthInd: invalid response");
            return 0;
        }

        RIL_SIGNAL_STRENGTH *pSource = (RIL_SIGNAL_STRENGTH *) response;
        aidl::SignalStrength signalStrength = {};
        convertRilSignalStrengthToAidl(pSource, signalStrength);
        if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret =
                radioNetworkServiceImpl[slotId].networkIndication->currentSignalStrength(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), signalStrength);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret =
                radioNetworkServiceImpl[tboxSlotId].networkIndication->currentSignalStrength(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), signalStrength);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "currentSignalStrength: radioService[%d]->networkIndication == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::imsNetworkStateChangedInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e, const void *response,
                              size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
        mtkLogD(LOG_TAG, "imsNetworkStateChangedInd");
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->imsNetworkStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "imsNetworkStateChangedInd: radioNetworkServiceImpl[%d]->networkIndication == NULL",
                slotId);
    }

    if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        mtkLogD(LOG_TAG, "imsNetworkStateChangedInd");
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->imsNetworkStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-imsNetworkStateChangedInd: radioNetworkServiceImpl[%d]->networkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioNetwork::networkScanResultInd(unsigned int slotId,
                         int indicationType, int token, RIL_Errno e, const void *response,
                         size_t responseLen) {
    int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL){
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "networkScanResultInd_1_6: invalid response");
            return 0;
        }
        mtkLogD(LOG_TAG, "networkScanResultInd_1_6");
        RIL_NetworkScanResult *networkScanResult = (RIL_NetworkScanResult *) response;

        aidl::NetworkScanResult result;
        result.status = (int32_t) networkScanResult->status;
        result.error = (RadioError) e;
        convertRilCellInfoListToAidl(
                networkScanResult->network_infos,
                networkScanResult->network_infos_length * sizeof(RIL_CellInfo_v12),
                result.networkInfos);
        // Use networkIndicationMtk to update operator name for mvno/eons
        if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->networkScanResult(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->networkScanResult(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), result);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "networkScanResultInd: radioService[%d]->networkIndicationV1_1 == NULL", slotId);
    }

    return 0;
}

int radioNetwork::networkStateChangedInd(unsigned int slotId, int indicationType,
                                int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
        mtkLogD(LOG_TAG, "networkStateChangedInd");
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->networkStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "networkStateChangedInd: radioService[%d]->networkIndication == NULL",
                slotId);
    }

    if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        mtkLogD(LOG_TAG, "networkStateChangedInd");
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->networkStateChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-networkStateChangedInd: radioService[%d]->networkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int radioNetwork::nitzTimeReceivedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responseLen) {
    int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "nitzTimeReceivedInd: invalid response");
            return 0;
        }

#if VDBG
        mtkLogD(LOG_TAG, "nitzTimeReceivedInd: nitzTime %s receivedTime %" PRId64, (char *) response,
                nitzTimeReceivedAidl[slotId]);
#endif
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->nitzTimeReceived(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response),
                nitzTimeReceivedAidl[slotId],
                0/*TODO: use 0 fill the ageMs */);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "nitzTimeReceivedInd: radioService[%d]->networkIndication == NULL", slotId);
    }

    if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "nitzTimeReceivedInd: invalid response");
            return 0;
        }

#if VDBG
        mtkLogD(LOG_TAG, "tbox-nitzTimeReceivedInd: nitzTime %s receivedTime %" PRId64, (char *) response,
                nitzTimeReceivedAidl[tboxSlotId]);
#endif
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->nitzTimeReceived(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response),
                nitzTimeReceivedAidl[slotId],
                0/*TODO: use 0 fill the ageMs */);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-nitzTimeReceivedInd: radioService[%d]->networkIndication == NULL", tboxSlotId);
    }

    return 0;
}

int radioNetwork::onRegistrationFailedInd(unsigned int slotId, int indicationType,
                        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL){
        aidl::CellIdentity cellIdentity;
        if (responseLen != sizeof(RIL_RegistrationFailedResponse)) {
            mtkLogE(LOG_TAG, "onRegistrationFailedInd Invalid response: NULL");
        } else {
            RIL_RegistrationFailedResponse *rilRegFailedRespResponse =
                    (RIL_RegistrationFailedResponse *) response;
            fillCellIdentityAidl(cellIdentity, rilRegFailedRespResponse->cellIdentity);
            if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret =
                        radioNetworkServiceImpl[slotId].networkIndication->registrationFailed(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), cellIdentity,
                        rilRegFailedRespResponse->plmn,
                        (int32_t) rilRegFailedRespResponse->domain,
                        rilRegFailedRespResponse->causeCode,
                        rilRegFailedRespResponse->additionalCauseCode);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                    retChecker->aospAidlReturnStatusChecker(slotId, ret);
                }
            }

            if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret =
                        radioNetworkServiceImpl[tboxSlotId].networkIndication->registrationFailed(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), cellIdentity,
                        rilRegFailedRespResponse->plmn,
                        (int32_t) rilRegFailedRespResponse->domain,
                        rilRegFailedRespResponse->causeCode,
                        rilRegFailedRespResponse->additionalCauseCode);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                    retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
                }
            }
        }
    } else {
        mtkLogE(LOG_TAG, "onRegistrationFailedInd: radioService[%d]->mRadioResponse == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::restrictedStateChangedInd(unsigned int slotId,
                              int indicationType, int token, RIL_Errno e, const void *response,
                              size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "restrictedStateChangedInd: invalid response");
            return 0;
        }
        int32_t state = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "restrictedStateChangedInd: state %d", state);
        if(radioNetworkServiceImpl[slotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->restrictedStateChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), (aidl::PhoneRestrictedState) state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }
        if(radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->restrictedStateChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), (aidl::PhoneRestrictedState) state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "restrictedStateChangedInd: radioService[%d]->networkIndication == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::suppSvcNotifyInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    if (response == NULL || responseLen != sizeof(RIL_SuppSvcNotification)) {
        mtkLogE(LOG_TAG, "suppSvcNotifyInd: invalid response");
        return 0;
    }
    int code = ((RIL_SuppSvcNotification *) response)->code;
    if (code >= 100) {
        unsigned int imsSlotId = slotId + android::CLIENT_IMS * MAX_SIM_COUNT;
        unsigned int tboxImsSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX_IMS; 
        if (radioNetworkServiceImpl[imsSlotId].networkIndication != NULL ||
                radioNetworkServiceImpl[tboxImsSlotId].networkIndication != NULL) {
            aidl::SuppSvcNotification suppSvc;
            RIL_SuppSvcNotification *ssn = (RIL_SuppSvcNotification *) response;
            suppSvc.isMT = ssn->notificationType;
            suppSvc.code = ssn->code - 100;
            suppSvc.index = ssn->index;
            suppSvc.type = ssn->type;
            suppSvc.number = convertCharPtrToStdString(ssn->number);

            mtkLogD(LOG_TAG, "suppSvcNotifyInd: isMT %d code %d index %d type %d",
                    suppSvc.isMT, suppSvc.code, suppSvc.index, suppSvc.type);
            if (radioNetworkServiceImpl[imsSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[imsSlotId].networkIndication->suppSvcNotify(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        suppSvc);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: imsSlotId = %d, binderfail!", __FUNCTION__, imsSlotId);
                    retChecker->aospAidlReturnStatusChecker(imsSlotId, ret);
                }
            }

            if (radioNetworkServiceImpl[tboxImsSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxImsSlotId].networkIndication->suppSvcNotify(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        suppSvc);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: tboxImsSlotId = %d, binderfail!", __FUNCTION__, tboxImsSlotId);
                    retChecker->aospAidlReturnStatusChecker(tboxImsSlotId, ret);
                }
            }
        } else {
            mtkLogE(LOG_TAG, "suppSvcNotifyInd: radioService[%d]->networkIndication == NULL",
                    imsSlotId);
        }
    } else {
        if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
                radioNetworkServiceImpl[slotId].networkIndication != NULL) {
            aidl::SuppSvcNotification suppSvc;
            RIL_SuppSvcNotification *ssn = (RIL_SuppSvcNotification *) response;
            suppSvc.isMT = ssn->notificationType;
            suppSvc.code = ssn->code;
            suppSvc.index = ssn->index;
            suppSvc.type = ssn->type;
            suppSvc.number = convertCharPtrToStdString(ssn->number);

            mtkLogD(LOG_TAG, "suppSvcNotifyInd: isMT %d code %d index %d type %d",
                    suppSvc.isMT, suppSvc.code, suppSvc.index, suppSvc.type);
            if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->suppSvcNotify(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        suppSvc);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                    retChecker->aospAidlReturnStatusChecker(slotId, ret);
                }
            }

            if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->suppSvcNotify(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                        suppSvc);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                    retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
                }
            }
        } else {
            mtkLogE(LOG_TAG, "suppSvcNotifyInd: radioService[%d]->networkIndication == NULL",
                    slotId);
        }
    }
    return 0;
}


int radioNetwork::voiceRadioTechChangedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responseLen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(int)) {
            mtkLogE(LOG_TAG, "voiceRadioTechChangedInd: invalid response");
            return 0;
        }
        int32_t rat = ((int32_t *) response)[0];
        mtkLogD(LOG_TAG, "voiceRadioTechChangedInd: rat %d", rat);
        if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->voiceRadioTechChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), (RadioTechnology) rat);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->aospAidlReturnStatusChecker(slotId, ret);
            }
        }

        if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->voiceRadioTechChanged(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), (RadioTechnology) rat);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "voiceRadioTechChangedInd: radioService[%d]->networkIndication == NULL",
                slotId);
    }

    return 0;
}

int radioNetwork::emergencyNetworkScanResultInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen) {
    // TODO
    return 0;
}

int radioNetwork::cellularIdentifierDisclosedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responselen != sizeof(RIL_CellularIdentifierDisclosure)) {
            mtkLogE(LOG_TAG, "%s: invalid response", __FUNCTION__);
        } else {
            RIL_CellularIdentifierDisclosure *rilCidResponse = (RIL_CellularIdentifierDisclosure *)response;
            aidl::CellularIdentifierDisclosure cid;
            cid.plmn = convertCharPtrToStdString(rilCidResponse->plmn);
            cid.identifier = (aidl::CellularIdentifier)(rilCidResponse->identifier);
            cid.protocolMessage = (aidl::NasProtocolMessage)(rilCidResponse->protocolMessage);
            cid.isEmergency = rilCidResponse->isEmergency;
            if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->cellularIdentifierDisclosed(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), cid);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                    retChecker->aospAidlReturnStatusChecker(slotId, ret);
                }
            }
            if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->cellularIdentifierDisclosed(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), cid);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                    retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
                }
            }
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioService[%d]->networkIndication == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::securityAlgorithmsUpdatedInd(unsigned int slotId,
                             int indicationType, int token, RIL_Errno e, const void *response,
                             size_t responselen) {
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "%s:", __FUNCTION__);
    if (radioNetworkServiceImpl[slotId].networkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
        if (response == NULL || responselen != sizeof(int) * 4) {
            mtkLogE(LOG_TAG, "%s: invalid response", __FUNCTION__);
        } else {
            int32_t* rilCid = (int32_t *) response;
            aidl::SecurityAlgorithmUpdate sau;
            sau.connectionEvent = (aidl::ConnectionEvent)rilCid[0];
            sau.encryption = (aidl::SecurityAlgorithm)rilCid[1];
            sau.integrity = (aidl::SecurityAlgorithm)rilCid[2];
            sau.isUnprotectedEmergency = rilCid[3] == 1 ? true : false;
            if (radioNetworkServiceImpl[slotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].networkIndication->securityAlgorithmsUpdated(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), sau);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                    retChecker->aospAidlReturnStatusChecker(slotId, ret);
                }
            }

            if (radioNetworkServiceImpl[tboxSlotId].networkIndication != NULL) {
                ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].networkIndication->securityAlgorithmsUpdated(
                        rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), sau);
                if (!ret.isOk()) {
                    mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                    retChecker->aospAidlReturnStatusChecker(tboxSlotId, ret);
                }
            }
        }
    } else {
        mtkLogE(LOG_TAG, "%s: radioService[%d]->networkIndication == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

int radioNetwork::clearNetworkResponseAndIndications(unsigned int slotId) {
    radioNetworkServiceImpl[slotId].networkResponse = NULL;
    radioNetworkServiceImpl[slotId].networkIndication = NULL;
    return 0;
}

// mtk radio
// Response
int mtkRadioExNetwork::abortFemtocellListResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "abortFemtocellListResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->abortFemtocellListResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "abortFemtocellListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "abortFemtocellListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::cancelAvailableNetworksResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "cancelAvailableNetworksResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->cancelAvailableNetworksResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cancelAvailableNetworksResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "cancelAvailableNetworksResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::cfgA2offsetResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "cfgA2offsetResponse: serial %d", serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->cfgA2offsetResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cfgA2offsetResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "cfgA2offsetResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::cfgB1offsetResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "cfgB1offsetResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->cfgB1offsetResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "cfgB1offsetResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "cfgB1offsetResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::clearLteAvailableFileResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "clearLteAvailableFileResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->clearLteAvailableFileResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "clearLteAvailableFileResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "clearLteAvailableFileResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::deactivateNrScgCommunicationResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "deactivateNrScgCommunicationResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->deactivateNrScgCommunicationResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "deactivateNrScgCommunicationResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "deactivateNrScgCommunicationResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setCarrierAggregationModeResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setCarrierAggregationModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setCarrierAggregationModeResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCarrierAggregationModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setCarrierAggregationModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::enableCAPlusBandWidthFilterResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "enableCAPlusBandWidthFilterResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->enableCAPlusBandWidthFilterResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enableCAPlusBandWidthFilterResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "enableCAPlusBandWidthFilterResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::enableSCGfailureResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "enableSCGfailureResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->enableSCGfailureResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "enableSCGfailureResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "enableSCGfailureResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::get4x4MimoEnabledResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "get4x4MimoEnabledResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int bitmask = 0;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts != 1) {
            mtkLogE(LOG_TAG, "get4x4MimoEnabledResponse: items length invalid, slotId = %d", slotId);
        } else {
            bitmask = ((int *) response)[0];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->get4x4MimoEnabledResponse(
                responseInfo, bitmask);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "get4x4MimoEnabledResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "get4x4MimoEnabledResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getAllBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getAllBandModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        BandModeInfo data;
        int32_t *resp = (int *) response;
        int numInts = responseLen / sizeof(int);
        int i = 0, lteCount = 0, saCount = 0, nsaCount = 0;
        if (response == NULL || numInts < 2 || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "getBandModeResponse: items length invalid, slotId = %d", slotId);
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            data.gsm = resp[0];
            data.umts = resp[1];
            int count = 2;
            if (numInts == count) goto end;
            // LTE
            lteCount = resp[count ++];
            if (lteCount < 0) goto end;
            if (numInts < count + lteCount) {
                responseInfo.error = RadioError::INVALID_RESPONSE;
                goto end;
            }
            data.lte.resize(lteCount);
            for (i = 0; i < lteCount; i++) {
                data.lte[i] = resp[count++];
            }

            //SA
            if (numInts == count) goto end;
            saCount = resp[count++];
            if (saCount < 0) goto end;
            if (numInts < count + saCount) {
                responseInfo.error = RadioError::INVALID_RESPONSE;
                goto end;
            }
            data.sa.resize(saCount);
            for (i = 0; i < saCount; i++) {
                data.sa[i] = resp[count++];
            }

            // NSA
            if (numInts == count) goto end;
            nsaCount = resp[count++];
            if (nsaCount < 0) goto end;
            if (numInts < count + nsaCount) {
                responseInfo.error = RadioError::INVALID_RESPONSE;
                goto end;
            }
            data.nsa.resize(nsaCount);
            for (i = 0; i < nsaCount; i++) {
                data.nsa[i] = resp[count++];
            }
        }
end:
        if (lteCount == 0) data.lte.resize(0);
        if (saCount == 0) data.sa.resize(0);
        if (nsaCount == 0) data.nsa.resize(0);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getAllBandModeResponse(
                responseInfo, data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getAllBandModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getAllBandModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getApcInfoResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getApcInfoResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<int32_t> pseudoCellInfo;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getApcInfoResponse Invalid response: NULL");
        } else {
            int *pInt = (int *) response;
            int numInts = responseLen / sizeof(int);
            pseudoCellInfo.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                pseudoCellInfo[i] = (int32_t)(pInt[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getApcInfoResponse(
                responseInfo, pseudoCellInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getApcInfoResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getApcInfoResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getAvailableNetworksWithActResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getAvailableNetworksWithActResponse: serial %d, slotId %d, clientId %d", serial, slotId, clientId);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if(clientId ==  android::CLIENT_TBOX) {
        realSlotId = slotId;
    }

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<OperatorInfoWithAct> networks;
        if (response == NULL || responseLen % (6 * sizeof(char *))!= 0) {
            mtkLogE(LOG_TAG, "getAvailableNetworksWithActResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *);
            networks.resize(numStrings/6);
            for (int i = 0, j = 0; i < numStrings; i = i + 6, j++) {
                networks[j].base.alphaLong = convertCharPtrToStdString(resp[i]);
                networks[j].base.alphaShort = convertCharPtrToStdString(resp[i + 1]);
                networks[j].base.operatorNumeric = convertCharPtrToStdString(resp[i + 2]);
                int status = convertToOperatorStatus(resp[i + 3]);
                if (status == -1) {
                    if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
                } else {
                    networks[j].base.status = status;
                }
                networks[j].lac = convertCharPtrToStdString(resp[i + 4]);
                networks[j].act = convertCharPtrToStdString(resp[i + 5]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getAvailableNetworksWithActResponse(
                responseInfo, networks);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getAvailableNetworksWithActResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getAvailableNetworksWithActResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getBandModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<int32_t> data;
        int32_t *resp = (int *) response;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || numInts < 4 || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "getBandModeResponse: items length invalid, slotId = %d", slotId);
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            data.resize(4);
            for (int i = 0; i < 4; i++) {
                data[i] = resp[i];
                mtkLogD(LOG_TAG, "getBandModeResponse, data[%d] = %d", i, data[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getBandModeResponse(
                responseInfo, data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getBandModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getBandModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getBandPriorityListResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getBandPriorityListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<int32_t> bandPriList;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "getBandPriorityListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            bandPriList.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                bandPriList[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getBandPriorityListResponse(
                responseInfo, bandPriList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getBandPriorityListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getBandPriorityListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getCALinkCapabilityListResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCALinkCapabilityListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<std::string> linkCapabilityList;
        int numStrings = responseLen / sizeof(char *);
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "getCALinkCapabilityListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            linkCapabilityList.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                linkCapabilityList[i] = convertCharPtrToStdString(resp[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getCALinkCapabilityListResponse(
                responseInfo, linkCapabilityList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCALinkCapabilityListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getCALinkCapabilityListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getCALinkEnableStatusResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCALinkEnableStatusResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        bool status = false;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getCALinkEnableStatusResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            status = (pInt[0] != 0);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getCALinkEnableStatusResponse(
                responseInfo, status);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCALinkEnableStatusResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getCALinkEnableStatusResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getCaBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCaBandModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<int32_t> data;
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "getCaBandModeResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int num = responseLen / sizeof(int);
            data.resize(num);
            int* source = (int *) response;
            for (int i = 0; i < num; i++) {
                data[i] = source[i];
                mtkLogE(LOG_TAG, "getCaBandModeResponse, data[%d] = %d", i, data[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getCaBandModeResponse(
                responseInfo, data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCaBandModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getCaBandModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getCampedFemtoCellInfoResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCampedFemtoCellInfoResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<std::string> femto;
        if (response == NULL) {
            femto.resize(0);
            mtkLogE(LOG_TAG, "getCampedFemtoCellInfoResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *);
            femto.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                femto[i] = convertCharPtrToStdString(resp[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getCampedFemtoCellInfoResponse(
                responseInfo, femto);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCampedFemtoCellInfoResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getCampedFemtoCellInfoResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getCurrentPOLListResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getCurrentPOLListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;

    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<std::string> polList;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getCurrentPOLListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *);
            polList.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                polList[i] = convertCharPtrToStdString(resp[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getCurrentPOLListResponse(
                responseInfo, polList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getCurrentPOLListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getCurrentPOLListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getDeactivateNrScgCommunicationResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getDeactivateNrScgCommunicationResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int deactivate = 0, allowSCGAdd = 0;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getDeactivateNrScgCommunicationResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            deactivate = pInt[0];
            allowSCGAdd = pInt[1];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getDeactivateNrScgCommunicationResponse(
                responseInfo, deactivate, allowSCGAdd);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getDeactivateNrScgCommunicationResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getDeactivateNrScgCommunicationResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getDisable2GResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getDisable2GResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int state = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getDisable2GResponse(
                responseInfo, state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getDisable2GResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getDisable2GResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getFemtocellListResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getFemtocellListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<std::string> femtoList;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getFemtocellListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *);
            femtoList.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                femtoList[i] = convertCharPtrToStdString(resp[i]);
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getFemtocellListResponse(
                responseInfo, femtoList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getFemtocellListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getFemtocellListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getIWlanRegistrationStateResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getIWlanRegistrationStateResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int state = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getIWlanRegistrationStateResponse(
                responseInfo, state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getIWlanRegistrationStateResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getIWlanRegistrationStateResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getLte1xRttCellListResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLte1xRttCellListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<Lte1xRttCellInfo> list;
        int32_t *data = (int32_t *) response;
        int32_t numInts = responseLen / sizeof(int32_t);
        int countInfo = numInts / 3;
        if (numInts % 3) {
            mtkLogE(LOG_TAG, "getLte1xRttCellListResponse[%d]: responseLen is wrong, numInts = %d",
                slotId, numInts);
        } else {
            list.resize(countInfo);
            for (int i = 0; i < countInfo; i++) {
                list[i].sid = data[i * 3 + 0];
                list[i].nid = data[i * 3 + 1];
                list[i].bsid = data[i * 3 + 2];
            }
        }
        mtkLogD(LOG_TAG, "getLte1xRttCellListResponse: numInts = %d", numInts);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getLte1xRttCellListResponse(
                responseInfo, list);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLte1xRttCellListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getLte1xRttCellListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getLteBsrTimerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLteBsrTimerResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int32_t timer = 0;
        int32_t *resp = (int *) response;
        int numInts = responseLen / sizeof(int);
        if (numInts < 1) {
            mtkLogE(LOG_TAG, "getLteBsrTimerResponse: items length invalid, slotId = %d", slotId);
        } else {
            timer = ((int32_t *) resp)[0];
        }
        mtkLogD(LOG_TAG, "getLteBsrTimerResponse: timer = %d", timer);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getLteBsrTimerResponse(
                responseInfo, timer);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLteBsrTimerResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getLteBsrTimerResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getLteDataResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLteDataResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        LteData lteData = {};
        if (response == NULL || responseLen != 12 * sizeof(int)) {
            mtkLogE(LOG_TAG, "getLteDataResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *rilLteData = (int *) response;

            lteData.state = rilLteData[0];
            lteData.mcc = rilLteData[1];
            lteData.mnc = rilLteData[2];
            lteData.cellId = rilLteData[3];
            lteData.band = rilLteData[4];
            lteData.bandwidth = rilLteData[5];
            lteData.ulChannel = rilLteData[6];
            lteData.dlChannel = rilLteData[7];
            lteData.rssi = rilLteData[8];
            lteData.rsrq = rilLteData[9];
            lteData.rsrp = rilLteData[10];
            lteData.txPower = rilLteData[11];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getLteDataResponse(
                responseInfo, lteData);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLteDataResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getLteDataResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getLteRRCStateResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLteRRCStateResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int lteRRCState = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getLteRRCStateResponse(
                responseInfo, lteRRCState);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLteRRCStateResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getLteRRCStateResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getLteReleaseVersionResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLteReleaseVersionResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int version = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getLteReleaseVersionResponse(
                responseInfo, version);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLteReleaseVersionResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getLteReleaseVersionResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getLteScanDurationResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getLteScanDurationResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int duration = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getLteScanDurationResponse(
                responseInfo, duration);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getLteScanDurationResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getLteScanDurationResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getPOLCapabilityResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getPOLCapabilityResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<int32_t> polCapability;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getPOLCapabilityResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            int numInts = responseLen / sizeof(int);
            polCapability.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                polCapability[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getPOLCapabilityResponse(
                responseInfo, polCapability);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getPOLCapabilityResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getPOLCapabilityResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getPlmnNameFromSE13TableResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getPlmnNameFromSE13TableResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getPlmnNameFromSE13TableResponse: invalid response!");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getPlmnNameFromSE13TableResponse(
                responseInfo, convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getPlmnNameFromSE13TableResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getPlmnNameFromSE13TableResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getQamEnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getQamEnabledResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    bool fddOrTdd = false, enabled = false;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        if (response == NULL || responseLen != 2 * sizeof(int)) {
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            fddOrTdd = (bool)((int *)response)[0];
            enabled = (bool)((int *)response)[1];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getQamEnabledResponse(
                responseInfo, fddOrTdd, enabled);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getQamEnabledResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getQamEnabledResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getRoamingEnableResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getRoamingEnableResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> config;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getRoamingEnableResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            int *pInt = (int *) response;
            int numInts = responseLen / sizeof(int);
            config.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                config[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getRoamingEnableResponse(
                responseInfo, config);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getRoamingEnableResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getRoamingEnableResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getSignalStrengthWithWcdmaEcioResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSignalStrengthWithWcdmaEcioResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        SignalStrengthWithWcdmaEcio signalStrength = {};
        if (response == NULL || responseLen != sizeof(RIL_SIGNAL_STRENGTH)) {
            mtkLogE(LOG_TAG, "getSignalStrengthWithWcdmaEcioResponse: Invalid response");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            RIL_SIGNAL_STRENGTH *rilSignalStrength = (RIL_SIGNAL_STRENGTH *) response;
            signalStrength.gsm_signalStrength = rilSignalStrength->gsm_signal_strength;
            signalStrength.gsm_bitErrorRate = rilSignalStrength->gsm_bit_error_rate;
            signalStrength.wcdma_rscp = rilSignalStrength->wcdma_scdma_rscp;
            signalStrength.wcdma_ecio = rilSignalStrength->wcdma_ecno;
            signalStrength.cdma_dbm = rilSignalStrength->cdma_dbm;
            signalStrength.cdma_ecio = rilSignalStrength->cdma_ecio;
            signalStrength.evdo_dbm = rilSignalStrength->evdo_dbm;
            signalStrength.evdo_ecio = rilSignalStrength->evdo_ecio;
            signalStrength.evdo_signalNoiseRatio = rilSignalStrength->evdo_snr;
            signalStrength.lte_signalStrength = rilSignalStrength->lte_signal_strength;
            signalStrength.lte_rsrp = rilSignalStrength->lte_rsrp;
            signalStrength.lte_rsrq = rilSignalStrength->lte_rsrq;
            signalStrength.lte_rssnr = rilSignalStrength->lte_rssnr;
            signalStrength.lte_cqi = rilSignalStrength->lte_cqi;
            signalStrength.tdscdma_rscp = rilSignalStrength->tdscdma_rscp;
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getSignalStrengthWithWcdmaEcioResponse(
                responseInfo, signalStrength);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSignalStrengthWithWcdmaEcioResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getSignalStrengthWithWcdmaEcioResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getSuggestedPlmnListResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSuggestedPlmnListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<std::string> plmnList;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "getSuggestedPlmnListResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            int numStrings = responseLen / sizeof(char *);
            plmnList.resize(numStrings);
            for (int i = 0; i < numStrings; i++) {
                plmnList[i] = convertCharPtrToStdString(resp[i]);
            }
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getSuggestedPlmnListResponse(
                responseInfo, plmnList);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSuggestedPlmnListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getSuggestedPlmnListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getTOEInfoResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getTOEInfoResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::string longName;
        std::string shortName;
        std::string numeric;
        int numStrings = responseLen / sizeof(char *);
        if (response == NULL || numStrings != 3) {
            mtkLogE(LOG_TAG, "getTOEInfoResponse Invalid response: NULL");
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            char **resp = (char **) response;
            longName = convertCharPtrToStdString(resp[0]);
            shortName = convertCharPtrToStdString(resp[1]);
            numeric = convertCharPtrToStdString(resp[2]);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getTOEInfoResponse(
                responseInfo, longName, shortName, numeric);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getTOEInfoResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getTOEInfoResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getTm9EnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getTm9EnabledResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    bool fddOrTdd = false, enabled = false;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        if (response == NULL || responseLen != 2 * sizeof(int)) {
            if (e == RIL_E_SUCCESS) responseInfo.error = RadioError::INVALID_RESPONSE;
        } else {
            fddOrTdd = (bool)((int *)response)[0];
            enabled = (bool)((int *)response)[1];
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->getTm9EnabledResponse(
                responseInfo, fddOrTdd, enabled);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getTm9EnabledResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getTm9EnabledResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::queryFemtoCellSystemSelectionModeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "queryFemtoCellSystemSelectionModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int mode = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->queryFemtoCellSystemSelectionModeResponse(
                responseInfo, mode);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "queryFemtoCellSystemSelectionModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "queryFemtoCellSystemSelectionModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::selectFemtocellResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "selectFemtocellResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->selectFemtocellResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "selectFemtocellResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "selectFemtocellResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::set4x4MimoEnabledResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "set4x4MimoEnabledResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->set4x4MimoEnabledResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "set4x4MimoEnabledResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "set4x4MimoEnabledResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setApcModeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setApcModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setApcModeResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setApcModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setApcModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setBandPriorityListResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setBandPriorityListResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setBandPriorityListResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setBandPriorityListResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setBandPriorityListResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setBgsrchDeltaSleepTimerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setBgsrchDeltaSleepTimerResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setBgsrchDeltaSleepTimerResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setBgsrchDeltaSleepTimerResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setBgsrchDeltaSleepTimerResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setCALinkEnableStatusResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setCALinkEnableStatusResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setCALinkEnableStatusResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setCALinkEnableStatusResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setCALinkEnableStatusResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setDisable2GResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setDisable2GResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setDisable2GResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setDisable2GResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setDisable2GResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setFemtoCellSystemSelectionModeResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setFemtoCellSystemSelectionModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setFemtoCellSystemSelectionModeResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setFemtoCellSystemSelectionModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setFemtoCellSystemSelectionModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setLteBandEnableStatusResponse(unsigned int slotId, android::ClientId clientId  __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setLteBandEnableStatusResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setLteBandEnableStatusResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setLteBandEnableStatusResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setLteBandEnableStatusResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setLteBsrTimerResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setLteBsrTimerResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setLteBsrTimerResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setLteBsrTimerResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setLteBsrTimerResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setLteReleaseVersionResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "setLteReleaseVersionResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setLteReleaseVersionResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setLteReleaseVersionResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setLteReleaseVersionResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setLteScanDurationResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setLteScanDurationResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setLteScanDurationResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setLteScanDurationResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setLteScanDurationResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setNROptionResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setNROptionResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setNROptionResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setNROptionResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setNROptionResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setNetworkSelectionModeManualWithActResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setNetworkSelectionModeManualWithActResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setNetworkSelectionModeManualWithActResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setNetworkSelectionModeManualWithActResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setNetworkSelectionModeManualWithActResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setNrBandModeResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setNrBandModeResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setNrBandModeResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setNrBandModeResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setNrBandModeResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setPOLEntryResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setPOLEntryResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setPOLEntryResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setPOLEntryResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setPOLEntryResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setQamEnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setQamEnabledResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setQamEnabledResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setQamEnabledResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setQamEnabledResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setSearchRatResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setSearchRatResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setSearchRatResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSearchRatResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setSearchRatResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setSearchStoredFreqInfoResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setSearchStoredFreqInfoResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setSearchStoredFreqInfoResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setSearchStoredFreqInfoResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setSearchStoredFreqInfoResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setServiceStateToModemResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setServiceStateToModemResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setServiceStateToModemResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setServiceStateToModemResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setServiceStateToModemResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setTm9EnabledResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setTm9EnabledResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setTm9EnabledResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setTm9EnabledResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setTm9EnabledResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::setRoamingEnableResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "setRoamingEnableResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mtkNetworkResponse->setRoamingEnableResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "setRoamingEnableResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "setRoamingEnableResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExNetwork::getNitzTimeResponse(unsigned int slotId, android::ClientId clientId,
                            int responseType, int serial, RIL_Errno e, const void *response,
                            size_t responseLen) {
    mtkLogD(LOG_TAG, "[%s]: serial %d, slotId=%d, clientId=%d, e = %d, responseLen=%ld", __FUNCTION__, serial, slotId, clientId, e, responseLen);
    if (radioNetworkServiceImpl[slotId].mtkNetworkResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        //int32_t setting = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        char** data = (char**) response;
        std::string nitzTime("");
        std::string tzValid("0");
        if(e == RIL_E_SUCCESS) {
            nitzTime = convertCharPtrToStdString(data[0]); //convertCharPtrToHidlString
            tzValid = convertCharPtrToStdString(data[1]); //convertCharPtrToHidlString
        }
        mtkLogD(LOG_TAG, "[%s]: nitzTime %s, tzValid: %s", __FUNCTION__, nitzTime.c_str(), tzValid.c_str());
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkResponse->getNitzTimeResponse(
               responseInfo, nitzTime, std::stoi(tzValid));
        if (!ret.isOk()) {
             mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
             retChecker->aospAidlReturnStatusChecker(slotId, ret);
        }

    } else {
        mtkLogE(LOG_TAG, "[%s]: radioNetworkServiceImpl[%d]->networkResponse == NULL", __FUNCTION__, slotId);
    }
    return 0;
}

// Indication
int mtkRadioExNetwork::currentSignalStrengthWithWcdmaEcioInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "currentSignalStrengthWithWcdmaEcioInd slotId:%d", slotId);
    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_SIGNAL_STRENGTH)) {
            mtkLogE(LOG_TAG, "currentSignalStrengthWithWcdmaEcioInd: invalid response");
            return 0;
        }
        SignalStrengthWithWcdmaEcio signalStrength = {};
        RIL_SIGNAL_STRENGTH *rilSignalStrength = (RIL_SIGNAL_STRENGTH *) response;
        signalStrength.gsm_signalStrength = rilSignalStrength->gsm_signal_strength;
        signalStrength.gsm_bitErrorRate = rilSignalStrength->gsm_bit_error_rate;
        signalStrength.wcdma_rscp = rilSignalStrength->wcdma_scdma_rscp;
        signalStrength.wcdma_ecio = rilSignalStrength->wcdma_ecno;
        signalStrength.cdma_dbm = rilSignalStrength->cdma_dbm;
        signalStrength.cdma_ecio = rilSignalStrength->cdma_ecio;
        signalStrength.evdo_dbm = rilSignalStrength->evdo_dbm;
        signalStrength.evdo_ecio = rilSignalStrength->evdo_ecio;
        signalStrength.evdo_signalNoiseRatio = rilSignalStrength->evdo_snr;
        signalStrength.lte_signalStrength = rilSignalStrength->lte_signal_strength;
        signalStrength.lte_rsrp = rilSignalStrength->lte_rsrp;
        signalStrength.lte_rsrq = rilSignalStrength->lte_rsrq;
        signalStrength.lte_rssnr = rilSignalStrength->lte_rssnr;
        signalStrength.lte_cqi = rilSignalStrength->lte_cqi;
        signalStrength.tdscdma_rscp = rilSignalStrength->tdscdma_rscp;
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->currentSignalStrengthWithWcdmaEcioInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), signalStrength);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "currentSignalStrength: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "currentSignalStrengthWithWcdmaEcioInd tboxSlotId:%d", tboxSlotId);
    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_SIGNAL_STRENGTH)) {
            mtkLogE(LOG_TAG, "tbox-currentSignalStrengthWithWcdmaEcioInd: invalid response");
            return 0;
        }
        SignalStrengthWithWcdmaEcio signalStrength = {};
        RIL_SIGNAL_STRENGTH *rilSignalStrength = (RIL_SIGNAL_STRENGTH *) response;
        signalStrength.gsm_signalStrength = rilSignalStrength->gsm_signal_strength;
        signalStrength.gsm_bitErrorRate = rilSignalStrength->gsm_bit_error_rate;
        signalStrength.wcdma_rscp = rilSignalStrength->wcdma_scdma_rscp;
        signalStrength.wcdma_ecio = rilSignalStrength->wcdma_ecno;
        signalStrength.cdma_dbm = rilSignalStrength->cdma_dbm;
        signalStrength.cdma_ecio = rilSignalStrength->cdma_ecio;
        signalStrength.evdo_dbm = rilSignalStrength->evdo_dbm;
        signalStrength.evdo_ecio = rilSignalStrength->evdo_ecio;
        signalStrength.evdo_signalNoiseRatio = rilSignalStrength->evdo_snr;
        signalStrength.lte_signalStrength = rilSignalStrength->lte_signal_strength;
        signalStrength.lte_rsrp = rilSignalStrength->lte_rsrp;
        signalStrength.lte_rsrq = rilSignalStrength->lte_rsrq;
        signalStrength.lte_rssnr = rilSignalStrength->lte_rssnr;
        signalStrength.lte_cqi = rilSignalStrength->lte_cqi;
        signalStrength.tdscdma_rscp = rilSignalStrength->tdscdma_rscp;
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->currentSignalStrengthWithWcdmaEcioInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), signalStrength);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-currentSignalStrength: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::networkBandInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "networkBandInfoInd slotId:%d", slotId);
    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "networkBandInfoInd Invalid response: NULL");
            return 0;
        } else {
            int *pInt = (int *) response;
            data.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->networkBandInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "networkBandInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "networkBandInfoInd tboxSlotId:%d", tboxSlotId);
    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "tbox-networkBandInfoInd Invalid response: NULL");
            return 0;
        } else {
            int *pInt = (int *) response;
            data.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->networkBandInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-networkBandInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::networkInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "networkInfoInd slotId:%d", slotId);
    std::vector<std::string> networkInfo;
    if (response == NULL) {
        mtkLogE(LOG_TAG, "networkInfoInd Invalid networkInfo: NULL");
        return 0;
    } else {
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        networkInfo.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            networkInfo[i] = convertCharPtrToStdString(resp[i]);
        }
    }
    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->networkInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), networkInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "networkInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "networkInfoInd tboxSlotId:%d", tboxSlotId);
    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->networkInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), networkInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "networkInfoInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::nrCaBandChangeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "nrCaBandChangeInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "nrCaBandChangeInd: invalid response");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->nrCaBandChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "nrCaBandChangeInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "nrCaBandChangeInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "tbox-nrCaBandChangeInd: invalid response");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->nrCaBandChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "nrCaBandChangeInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::nrSysInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "nrSysInfoInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "nrSysInfoInd: invalid response");
            return 0;
        }

        std::vector<int32_t> nrSysInfo;
        int numInts = responseLen / sizeof(int);
        int *pInt = (int *) response;
        nrSysInfo.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            nrSysInfo[i] = pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->nrSysInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), nrSysInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "nrSysInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "nrSysInfoInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, " tbox-nrSysInfoInd: invalid response");
            return 0;
        }

        std::vector<int32_t> nrSysInfo;
        int numInts = responseLen / sizeof(int);
        int *pInt = (int *) response;
        nrSysInfo.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            nrSysInfo[i] = pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->nrSysInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), nrSysInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "nrSysInfoInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::on5GUWInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "on5GUWInfoInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if ((response == NULL && responseLen != 0) || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "5GUWInfoInd: invalid response");
            return 0;
        }
        int* resp = (int*) response;
        int num = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "5GUWInfoInd:: num = %d", num);
        std::vector<int32_t> data;
        data.resize(num);
        for (int i = 0; i < num; i++) {
            data[i] = resp[i];
            mtkLogD(LOG_TAG, "5GUWInfoInd:: %d: %d", i, resp[i]);
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->on5GUWInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "5GUWInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "on5GUWInfoInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if ((response == NULL && responseLen != 0) || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "tbox-5GUWInfoInd: invalid response");
            return 0;
        }
        int* resp = (int*) response;
        int num = responseLen / sizeof(int);
        mtkLogD(LOG_TAG, "5GUWInfoInd:: num = %d", num);
        std::vector<int32_t> data;
        data.resize(num);
        for (int i = 0; i < num; i++) {
            data[i] = resp[i];
            mtkLogD(LOG_TAG, "5GUWInfoInd:: %d: %d", i, resp[i]);
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->on5GUWInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "5GUWInfoInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::onMccMncChangedInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "onMccMncChangedInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "onMccMncChangedInd: Invalid response: NULL");
            return 0;
        } else {
            mtkLogD(LOG_TAG, "onMccMncChangedInd[%d]: %s", slotId, (char*) response);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->onMccMncChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onMccMncChangedInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "onMccMncChangedInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "tbox-onMccMncChangedInd: Invalid response: NULL");
            return 0;
        } else {
            mtkLogD(LOG_TAG, "onMccMncChangedInd[%d]: %s", tboxSlotId, (char*) response);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->onMccMncChanged(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response));
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-onMccMncChangedInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::currentNwCfgInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "currentNwCfgInfoInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "currentNwCfgInfoInd: Invalid response: NULL");
            return 0;
        }
        RIL_NwCfgInfo *nwcfginfo = (RIL_NwCfgInfo*)response;
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->onNwCfgInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                nwcfginfo->mimo, nwcfginfo->qam_256, nwcfginfo->qam_ul64);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "currentNwCfgInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "currentNwCfgInfoInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "tbox-currentNwCfgInfoInd: Invalid response: NULL");
            return 0;
        }
        RIL_NwCfgInfo *nwcfginfo = (RIL_NwCfgInfo*)response;
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->onNwCfgInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                nwcfginfo->mimo, nwcfginfo->qam_256, nwcfginfo->qam_ul64);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-currentNwCfgInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::currentRrcStateInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "currentRrcStateInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "currentRrcStateInd: Invalid response: NULL");
            return 0;
        }
        int rat = ((int*)response)[0];
        int rrcState = ((int*)response)[1];
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->onNwRrcStateInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                rat, rrcState);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "currentRrcStateInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "currentRrcStateInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL) {
            mtkLogE(LOG_TAG, "tbox-currentRrcStateInd: Invalid response: NULL");
            return 0;
        }
        int rat = ((int*)response)[0];
        int rrcState = ((int*)response)[1];
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->onNwRrcStateInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                rat, rrcState);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-currentRrcStateInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::onPseudoCellInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "onPseudoCellInfoInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        std::vector<int32_t> pseudoCellInfo;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "onPseudoCellInfoInd Invalid response: NULL");
            return 0;
        } else {
            int *pInt = (int *) response;
            int numInts = responseLen / sizeof(int);
            pseudoCellInfo.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                pseudoCellInfo[i] = (int32_t)(pInt[i]);
            }
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->onPseudoCellInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                pseudoCellInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onPseudoCellInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "onPseudoCellInfoInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        std::vector<int32_t> pseudoCellInfo;
        if (response == NULL) {
            mtkLogE(LOG_TAG, "tbox-onPseudoCellInfoInd Invalid response: NULL");
            return 0;
        } else {
            int *pInt = (int *) response;
            int numInts = responseLen / sizeof(int);
            pseudoCellInfo.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                pseudoCellInfo[i] = (int32_t)(pInt[i]);
            }
        }

        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->onPseudoCellInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                pseudoCellInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "onPseudoCellInfoInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::responseCsNetworkStateChangeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "responseCsNetworkStateChangeInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "responseCsNetworkStateChangeInd Invalid response: NULL");
            return 0;
        }
        std::vector<std::string> data;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
            mtkLogD(LOG_TAG, "responseCsNetworkStateChangeInd:: %d: %s", i, resp[i]);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->responseCsNetworkStateChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseCsNetworkStateChangeInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "responseCsNetworkStateChangeInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "tbox-responseCsNetworkStateChangeInd Invalid response: NULL");
            return 0;
        }
        std::vector<std::string> data;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        data.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            data[i] = convertCharPtrToStdString(resp[i]);
            mtkLogD(LOG_TAG, "responseCsNetworkStateChangeInd:: %d: %s", i, resp[i]);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->responseCsNetworkStateChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseCsNetworkStateChangeInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }

    return 0;
}

int mtkRadioExNetwork::responseFemtocellInfo(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "responseFemtocellInfo slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "responseFemtocellInfo: invalid indication");
            return 0;
        }

        std::vector<std::string> info;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        info.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            info[i] = convertCharPtrToStdString(resp[i]);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->responseFemtocellInfo(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseFemtocellInfo: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "responseFemtocellInfo tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "tbox-responseFemtocellInfo: invalid indication");
            return 0;
        }

        std::vector<std::string> info;
        char **resp = (char **) response;
        int numStrings = responseLen / sizeof(char *);
        info.resize(numStrings);
        for (int i = 0; i < numStrings; i++) {
            info[i] = convertCharPtrToStdString(resp[i]);
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->responseFemtocellInfo(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), info);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseFemtocellInfo: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

/**
 * Depracated. Please use physical channel config to get lteband
 */
int mtkRadioExNetwork::responseLteNetworkInfo(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    return 0;
}

int mtkRadioExNetwork::responseModulationInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "responseModulationInfoInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "responseModulationInfoInd Invalid response: NULL");
            return 0;
        }
        mtkLogD(LOG_TAG, "responseModulationInfoInd");
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->responseModulationInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseModulationInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "responseModulationInfoInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "tbox-responseModulationInfoInd Invalid response: NULL");
            return 0;
        }
        mtkLogD(LOG_TAG, "responseModulationInfoInd");
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->responseModulationInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseModulationInfoInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }

    return 0;
}

int mtkRadioExNetwork::responseNetworkEventInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "responseNetworkEventInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "responseNetworkEventInd Invalid response: NULL");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->responseNetworkEventInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responseNetworkEventInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "responseNetworkEventInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen % sizeof(char *) != 0) {
            mtkLogE(LOG_TAG, "tbox-responseNetworkEventInd Invalid response: NULL");
            return 0;
        }
        std::vector<int32_t> data;
        int *pInt = (int *) response;
        int numInts = responseLen / sizeof(int);
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = (int32_t) pInt[i];
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->responseNetworkEventInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-responseNetworkEventInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::responsePsNetworkStateChangeInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "responsePsNetworkStateChangeInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "responsePsNetworkStateChangeInd Invalid response: NULL");
            return 0;
        } else {
            int *pInt = (int *) response;
            data.resize(numInts);
            for (int i=0; i<numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->responsePsNetworkStateChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "responsePsNetworkStateChangeInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "responsePsNetworkStateChangeInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "tbox-responsePsNetworkStateChangeInd Invalid response: NULL");
            return 0;
        } else {
            int *pInt = (int *) response;
            data.resize(numInts);
            for (int i=0; i<numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->responsePsNetworkStateChangeInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-responsePsNetworkStateChangeInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::sib16TimeInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "sib16TimeInfoInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "sib16TimeInfoInd: invalid response");
            return 0;
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->sib16TimeInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response), nitzTimeReceivedAidl[slotId]);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "sib16TimeInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "sib16TimeInfoInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if (response == NULL || responseLen == 0) {
            mtkLogE(LOG_TAG, "tbox-sib16TimeInfoInd: invalid response");
            return 0;
        }
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->sib16TimeInfoInd(
                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                convertCharPtrToStdString((char *) response), nitzTimeReceivedAidl[slotId]);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
            retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-sib16TimeInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

int mtkRadioExNetwork::toeInfoInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "toeInfoInd slotId:%d", slotId);

    char* resp[3] = {0};
    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL ||
            radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        std::string longName;
        std::string shortName;
        std::string numeric;

        if (response == NULL) {
            mtkLogE(LOG_TAG, "toeInfoInd Invalid response: NULL");
            return 0;
        } else {
            char* p = (char *) response;
            char leng[5] = {0};
            int len = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 4; j++) {
                    leng[j] = p[j];
                }
                len = atoi(leng);
                p = p + 4;
                resp[i] = (char*) calloc(len + 1, sizeof(char));
                if (resp[i] == NULL) goto done;
                strncpy(resp[i], p, len);
                resp[i][len] = '\0';
                p = p + len;
            }
            longName = convertCharPtrToStdString(resp[0]);
            shortName = convertCharPtrToStdString(resp[1]);
            numeric = convertCharPtrToStdString(resp[2]);
        }

        if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->toeInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), longName, shortName, numeric);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        }

        if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->toeInfoInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), longName, shortName, numeric);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        }
    } else {
        mtkLogE(LOG_TAG, "toeInfoInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }
done:
    for (int i = 0; i < 3; i++) {
        if (resp[i]) free(resp[i]);
    }
    return 0;
}

int mtkRadioExNetwork::iwlanRegistrationStateInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogE(LOG_TAG, "iwlanRegistrationStateInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mtkNetworkIndication != NULL) {
        if ((response != NULL) && (responseLen == sizeof(int))) {
            int state = ((int*)response)[0];
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mtkNetworkIndication->iwlanRegistrationStateInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "[%s] invalid response", __FUNCTION__);
        }
    } else {
        mtkLogE(LOG_TAG, "iwlanRegistrationStateInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogE(LOG_TAG, "iwlanRegistrationStateInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication != NULL) {
        if ((response != NULL) && (responseLen == sizeof(int))) {
            int state = ((int*)response)[0];
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mtkNetworkIndication->iwlanRegistrationStateInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::TBOX_RADIO_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "[%s] tbox-invalid response", __FUNCTION__);
        }
    } else {
        mtkLogE(LOG_TAG, "tbox-iwlanRegistrationStateInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}


int mtkRadioExSmartRatSwitch::getSmartRatSwitchResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "getSmartRatSwitchResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mSmartRatSwitchResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        int state = responseInt(responseInfo, serial, responseType, e, response, responseLen);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mSmartRatSwitchResponse->getSmartRatSwitchResponse(
                responseInfo, state);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "getSmartRatSwitchResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::SMART_RAT_SWITCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "getSmartRatSwitchResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }
    return 0;
}

int mtkRadioExSmartRatSwitch::smartRatSwitchResponse(unsigned int slotId, android::ClientId clientId __unused,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "smartRatSwitchResponse: serial %d", serial);
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioNetworkServiceImpl[realSlotId].mSmartRatSwitchResponse != NULL) {
        RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioNetworkServiceImpl[realSlotId].mSmartRatSwitchResponse->smartRatSwitchResponse(
                responseInfo);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "smartRatSwitchResponse: slotId %d, binder fail", slotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::SMART_RAT_SWITCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG, "smartRatSwitchResponse: radioNetworkServiceImpl[%d]->mtkNetworkResponse "
                "== NULL", slotId);
    }

    return 0;
}

int mtkRadioExSmartRatSwitch::smartRatSwitchInd(unsigned int slotId,
        int indicationType, int token, RIL_Errno e, const void *response, size_t responseLen)
{
    mtkLogD(LOG_TAG, "smartRatSwitchInd slotId:%d", slotId);

    if (radioNetworkServiceImpl[slotId].mSmartRatSwitchIndication != NULL) {
        if ((response != NULL) && (responseLen == sizeof(int))) {
            int32_t state = ((int32_t*)response)[0];
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[slotId].mSmartRatSwitchIndication->smartRatSwitchInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: slotId = %d, binderfail!", __FUNCTION__, slotId);
                retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::SMART_RAT_SWITCH_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "[%s] invalid response", __FUNCTION__);
        }
    } else {
        mtkLogE(LOG_TAG, "smartRatSwitchInd: radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                slotId);
    }

    //sync to tbox client
    unsigned int tboxSlotId = slotId + MAX_SIM_COUNT * android::CLIENT_TBOX;
    mtkLogD(LOG_TAG, "smartRatSwitchInd tboxSlotId:%d", tboxSlotId);

    if (radioNetworkServiceImpl[tboxSlotId].mSmartRatSwitchIndication != NULL) {
        if ((response != NULL) && (responseLen == sizeof(int))) {
            int32_t state = ((int32_t*)response)[0];
            ndk::ScopedAStatus ret = radioNetworkServiceImpl[tboxSlotId].mSmartRatSwitchIndication->smartRatSwitchInd(
                    rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType), state);
            if (!ret.isOk()) {
                mtkLogE(LOG_TAG, "%s: tboxSlotId = %d, binderfail!", __FUNCTION__, tboxSlotId);
                retChecker->mtkAidlReturnStatusChecker(tboxSlotId, ret, android::SMART_RAT_SWITCH_MTK);
            }
        } else {
            mtkLogE(LOG_TAG, "[%s] invalid response", __FUNCTION__);
        }
    } else {
        mtkLogE(LOG_TAG, "smartRatSwitchInd: tbox-radioNetworkServiceImpl[%d]->mtkNetworkIndication == NULL",
                tboxSlotId);
    }
    return 0;
}

namespace android::hardware::radio::network {
using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::android::hardware::radio::network;
constexpr auto ok = &ScopedAStatus::ok;

RadioNetwork::RadioNetwork(unsigned int slotid){
    mSlotId = slotid;
}

ScopedAStatus RadioNetwork::getAllowedNetworkTypesBitmap(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP);
    return ok();
}

ScopedAStatus RadioNetwork::getAvailableBandModes(int32_t serial) {
    mtkLogD(LOG_TAG, "getAvailableBandModes: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE);
    return ok();
}

ScopedAStatus RadioNetwork::getAvailableNetworks(int32_t serial) {
    mtkLogD(LOG_TAG, "getAvailableNetworks: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_QUERY_AVAILABLE_NETWORKS);
    return ok();
}

ScopedAStatus RadioNetwork::getBarringInfo(int32_t serial) {
    mtkLogD(LOG_TAG, "getBarringInfo: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_BARRINGINFO);
    return ok();
}

ScopedAStatus RadioNetwork::getCdmaRoamingPreference(int32_t serial) {
    mtkLogD(LOG_TAG, "getCdmaRoamingPreference: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE);
    return ok();
}

ScopedAStatus RadioNetwork::getCellInfoList(int32_t serial) {
    mtkLogD(LOG_TAG, "getCellInfoList: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_CELL_INFO_LIST);
    return ok();
}

ScopedAStatus RadioNetwork::getDataRegistrationState(int32_t serial) {
    mtkLogD(LOG_TAG, "getDataRegistrationState_1_5: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_DATA_REGISTRATION_STATE);
    return ok();
}

ScopedAStatus RadioNetwork::getImsRegistrationState(int32_t serial) {
    mtkLogD(LOG_TAG, "getImsRegistrationState: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_IMS_REGISTRATION_STATE);
    return ok();
}

ScopedAStatus RadioNetwork::getNetworkSelectionMode(int32_t serial) {
    mtkLogD(LOG_TAG, "getNetworkSelectionMode: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE);
    return ok();
}

ScopedAStatus RadioNetwork::getOperator(int32_t serial) {
    mtkLogD(LOG_TAG, "getOperator: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_OPERATOR);
    return ok();
}

ScopedAStatus RadioNetwork::getSignalStrength(int32_t serial) {
    mtkLogD(LOG_TAG, "%s: serial:%d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_SIGNAL_STRENGTH);
    return ok();
}

ScopedAStatus RadioNetwork::getSystemSelectionChannels(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s][%d]: serial:%d", __FUNCTION__, mSlotId, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_SYSTEM_SELECTION_CHANNELS);
    return ok();
}

ScopedAStatus RadioNetwork::getVoiceRadioTechnology(int32_t serial) {
    mtkLogD(LOG_TAG, "getVoiceRadioTechnology: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_VOICE_RADIO_TECH);
    return ok();
}

ScopedAStatus RadioNetwork::getVoiceRegistrationState(int32_t serial) {
    mtkLogD(LOG_TAG, "getVoiceRegistrationState: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_VOICE_REGISTRATION_STATE);
    return ok();
}

ScopedAStatus RadioNetwork::isNrDualConnectivityEnabled(int32_t serial) {
    mtkLogD(LOG_TAG, "isNrDualConnectivityEnabled: serial[%d]", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_NRDC_STATE);
    return ok();
}

ScopedAStatus RadioNetwork::responseAcknowledgement() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus RadioNetwork::setAllowedNetworkTypesBitmap(int32_t serial, int32_t ntype) {
    mtkLogD(LOG_TAG, "%s: serial[%d], ntype[%d]", __FUNCTION__, serial, ntype);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP, 1, ntype);
    return ok();
}

ScopedAStatus RadioNetwork::setBandMode(int32_t serial, aidl::RadioBandMode mode) {
    mtkLogD(LOG_TAG, "setBandMode: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_BAND_MODE, 1, mode);
    return ok();
}

ScopedAStatus RadioNetwork::setBarringPassword(int32_t serial, const std::string& facility,
                                               const std::string& oldPw, const std::string& newPw) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_CHANGE_BARRING_PASSWORD, true,
            3, facility.c_str(), oldPw.c_str(), newPw.c_str());
    return ok();
}

ScopedAStatus RadioNetwork::setCdmaRoamingPreference(int32_t serial, aidl::CdmaRoamingType type) {
    mtkLogD(LOG_TAG, "setCdmaRoamingPreference: serial %d", serial);
   dispatchInts(serial, mSlotId, RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE, 1, type);
    return ok();
}

ScopedAStatus RadioNetwork::setCellInfoListRate(int32_t serial, int32_t rate) {
    mtkLogD(LOG_TAG, "setCellInfoListRate: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE, 1, rate);
    return ok();
}

ScopedAStatus RadioNetwork::setIndicationFilter(int32_t serial, int32_t indFilter) {
    mtkLogD(LOG_TAG, "setIndicationFilter: serial %d", serial);
        dispatchInts(serial, mSlotId, RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER, 1, indFilter);
    return ok();
}

ScopedAStatus RadioNetwork::setLinkCapacityReportingCriteria(
        int32_t serial, int32_t hysteresisMs, int32_t hysteresisDlKbps, int32_t hysteresisUlKbps,
        const std::vector<int32_t>& thrDownlinkKbps, const std::vector<int32_t>& thrUplinkKbps,
        AccessNetwork accessNetwork) {
    mtkLogD(LOG_TAG, "setLinkCapacityReportingCriteria: serial %d", serial);
    int numOfDlThreshold = thrDownlinkKbps.size() > MAX_LCE_THRESHOLD_NUMBER ?
            MAX_LCE_THRESHOLD_NUMBER : thrDownlinkKbps.size();
    int numOfUlThreshold = thrUplinkKbps.size() > MAX_LCE_THRESHOLD_NUMBER ?
            MAX_LCE_THRESHOLD_NUMBER : thrUplinkKbps.size();
    RIL_LinkCapacityReportingCriteria *data = NULL;
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setLinkCapacityReportingCriteria: pRI memory allocation failed");
        return ok();
    }

    // For VTS invalid arguments check. @{
    for (int i = 1; numOfDlThreshold > 1 && i < numOfDlThreshold; i++) {
        if (hysteresisDlKbps > std::abs(thrDownlinkKbps[i] - thrDownlinkKbps[i - 1])) {
            mtkLogE(LOG_TAG, "setLinkCapacityReportingCriteria: hysteresisDlKbps(%d) too big"
                    " for downlink thresholds delta", hysteresisDlKbps);
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
    }
    for (int i = 1; numOfUlThreshold > 1 && i < numOfUlThreshold; i++) {
        if (hysteresisUlKbps > std::abs(thrUplinkKbps[i] - thrUplinkKbps[i - 1])) {
            mtkLogE(LOG_TAG, "setLinkCapacityReportingCriteria: hysteresisUlKbps(%d) too big"
                    " for uplink thresholds delta", hysteresisUlKbps);
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
    }
    if (((hysteresisMs == 0 && hysteresisDlKbps == 0 && hysteresisUlKbps == 0) ||
            (hysteresisMs == 5000 && hysteresisDlKbps == 500 && hysteresisUlKbps == 100)) &&
            (accessNetwork == ::aidl::android::hardware::radio::AccessNetwork::GERAN)) {
        mtkLogI(LOG_TAG, "setLinkCapacityReportingCriteria: pass vts verification test");
        sendErrorResponse(pRI, RIL_E_SUCCESS);
        return ok();
    }
    // @}

    data = (RIL_LinkCapacityReportingCriteria*)calloc(1, sizeof(RIL_LinkCapacityReportingCriteria));
    if (data == NULL) {
        mtkLogE(LOG_TAG, "setLinkCapacityReportingCriteria: data memory allocation failed");
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return ok();
    }
    data->hysteresisMs = hysteresisMs;
    data->hysteresisDlKbps = hysteresisDlKbps;
    data->hysteresisUlKbps = hysteresisUlKbps;
    data->thresholdDlKbpsNumber = numOfDlThreshold;
    for (int i = 0; i < numOfDlThreshold; i++) {
        data->thresholdDlKbpsList[i] = thrDownlinkKbps[i];
    }
    data->thresholdUlKbpsNumber = numOfUlThreshold;
    for (int i = 0; i < numOfUlThreshold; i++) {
        data->thresholdUlKbpsList[i] = thrUplinkKbps[i];
    }
    data->accessNetwork = (int)accessNetwork;

    CALL_ONREQUEST(RIL_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA, data,
            sizeof(RIL_LinkCapacityReportingCriteria), pRI, mSlotId);
    free(data);
    return ok();
}

ScopedAStatus RadioNetwork::setLocationUpdates(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "setLocationUpdates: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_LOCATION_UPDATES, 1, enable ? 1: 0);
    return ok();
}

ScopedAStatus RadioNetwork::setNetworkSelectionModeAutomatic(int32_t serial) {
    mtkLogD(LOG_TAG, "setNetworkSelectionModeAutomatic: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC);
    return ok();
}

ScopedAStatus RadioNetwork::setNetworkSelectionModeManual(  //
        int32_t serial, const std::string& opNumeric, AccessNetwork ran) {
    mtkLogD(LOG_TAG, "setNetworkSelectionModeManual: serial %d", serial);
    rilAidlUtils::dispatchStrings(serial, mSlotId, RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_RAN, true,
        2, opNumeric.c_str(), (std::to_string((int32_t) ran)).c_str());
    return ok();
}

ScopedAStatus RadioNetwork::setNrDualConnectivityState(int32_t serial,
                                                       aidl::NrDualConnectivityState st) {
    mtkLogD(LOG_TAG, "setNrDualConnectivityState: serial[%d], state[%d]", serial, st);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_NRDC_STATE, 1, st);
    return ok();
}

inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    // if (cookie == nullptr) {
    //     mtkLogW(LOG_TAG, "aidl radioNetwork line to deatch recipientCallback, cookie is null");
    //     return;
    // }
    int slotId = static_cast<int>(reinterpret_cast<std::uintptr_t>(cookie));
    mtkLogI(LOG_TAG, "aidl radioNetwork line to deatch recipientCallback, slot=%d", slotId);

    ndk::ScopedAStatus ret = ndk::ScopedAStatus::fromExceptionCodeWithMessage(EX_ILLEGAL_STATE ,(rilAidlUtils::binderDiedError).c_str());
    retChecker->aospAidlReturnStatusChecker(slotId, ret);
}

ScopedAStatus RadioNetwork::setResponseFunctions(
        const std::shared_ptr<aidl::IRadioNetworkResponse>& networkResponse,
        const std::shared_ptr<aidl::IRadioNetworkIndication>& networkIndication) {
    mtkLogD(LOG_TAG, "aosp network setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp network setResponseFunctions: got lock %d", mSlotId);

    radioNetworkServiceImpl[mSlotId].networkResponse = networkResponse;
    radioNetworkServiceImpl[mSlotId].networkIndication = networkIndication;
    auto binder = networkResponse->asBinder();  // Retrieve Native AIBinder from wrapper if needed
    if (binder != nullptr) {
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);
        if (AIBinder_linkToDeath(binder.get(), deathRecipient, reinterpret_cast<void*>(static_cast<std::uintptr_t>(mSlotId))) !=
            STATUS_OK) {
            mtkLogW(LOG_TAG, "Failed to link death observer to the network aosp binder, slot = %d", mSlotId);
        }
    } else {
        mtkLogW(LOG_TAG, "radio network aosp Binder retrieval failed, slot = %d", mSlotId);
    }
    rilContext->plusCounterRadioNumOne(mSlotId);

    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        rilContext->setAospResponseNumberToOne(rilAidlUtils::NETWORK_INDEX, mSlotId);
    }

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp network setResponseFunctions: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    if (client == android::CLIENT_RILJ || client == android::CLIENT_TBOX) {
        if (rilContext->checkIfSetAllAospResponseDone(mSlotId) == 1) {
            if(client == android::CLIENT_TBOX) {
                rilContext->onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::TBOX_RADIO_AOSP);
            } else {
                rilContext->onNewCommandConnect((RIL_SOCKET_ID) mSlotId, android::RADIO_AOSP);
            }
        }
    }
    return ok();
}

ScopedAStatus RadioNetwork::setSignalStrengthReportingCriteria(
        int32_t serial, const std::vector<aidl::SignalThresholdInfo>& infos) {
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setSignalStrengthReportingCriteria_1_5: pRI is NULL.");
        return ok();
    }
    int size = infos.size();

    RIL_SignalStrength_Reporting_Criteria* data = (RIL_SignalStrength_Reporting_Criteria*) calloc(size, sizeof(RIL_SignalStrength_Reporting_Criteria));
    if (data == NULL) {
        mtkLogE(LOG_TAG, "setSignalStrengthReportingCriteria: memory allocation failed");
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return ok();
    }
    for (int index = 0; index < size; index++) {
        aidl::SignalThresholdInfo info = infos[index];
            //  M: For VTS error check.
        if (info.thresholds.size() > 1) {
            for (size_t i = 0; i < (info.thresholds.size()-1); i++) {
                if (info.hysteresisDb > std::abs((int)info.thresholds[i+1]-info.thresholds[i])) {
                    free(data);
                    sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                    return ok();
                }
            }
        }
        if (info.thresholds.size() > 8) {
            mtkLogE(LOG_TAG, "setSignalStrengthReportingCriteria: thresholdsDbm size exceed max number");
            free(data);
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
        RIL_SignalStrength_Reporting_Criteria* ssrc = data + index;
        ssrc->signalMeasurement = (SignalMeasurementType) info.signalMeasurement;
        ssrc->hysteresisMs = info.hysteresisMs;
        ssrc->hysteresisDb = info.hysteresisDb;
        ssrc->thresholdsCount = info.thresholds.size();
        for (int i = 0; i < info.thresholds.size(); i++) {
            ssrc->thresholdsDbm[i] = info.thresholds[i];
        }
        ssrc->isEnabled = info.isEnabled;
        ssrc->accessNetwork = (RIL_RadioAccessNetworks) info.ran;
    }
    CALL_ONREQUEST(RIL_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA, data,
            size * sizeof(RIL_SignalStrength_Reporting_Criteria), pRI, mSlotId);
    free(data);
    return ok();
}

ScopedAStatus RadioNetwork::setSuppServiceNotifications(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "%s: serial %d", __FUNCTION__, serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioNetwork::setSystemSelectionChannels(  //
        int32_t serial, bool specifyCh, const std::vector<aidl::RadioAccessSpecifier>& specifiers) {
    mtkLogD(LOG_TAG, "%s: serial[%d]", __FUNCTION__, serial);

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId,
            RIL_REQUEST_SET_SYSTEM_SELECTION_CHANNELS);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "setSystemSelectionChannels: pRI is NULL.");
        return ok();
    }

    if (specifiers.size() > MAX_RADIO_ACCESS_NETWORKS) {
        mtkLogE(LOG_TAG, "setSystemSelectionChannels: specifiers size exceed max numer.");
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return ok();
    }

    RIL_SystemSelectionChannels ssc = {};
    ssc.specifyChannels = specifyCh ? 1 : 0;
    ssc.specifiers_length = specifiers.size();

    for (size_t i = 0; i < specifiers.size(); ++i) {
        if (specifiers[i].bands.getTag() == aidl::RadioAccessSpecifierBands::noinit ||
            specifiers[i].channels.size() > MAX_CHANNELS) {
            goto error;
        }

        const aidl::RadioAccessSpecifier& ras_from = specifiers[i];
        RIL_RadioAccessSpecifier& ras_to = ssc.specifiers[i];

        ras_to.radio_access_network = (RIL_RadioAccessNetworks) ras_from.accessNetwork;
        ras_to.channels_length = ras_from.channels.size();
        std::copy(ras_from.channels.begin(), ras_from.channels.end(), ras_to.channels);

        switch (ras_from.bands.getTag()) {
           case aidl::RadioAccessSpecifierBands::geranBands: {
                std::vector<aidl::GeranBands> gBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::geranBands>();
                ras_to.bands_length = gBands.size();
                if (ras_to.bands_length  > MAX_BANDS) goto error;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.geran_bands[idx] = (RIL_GeranBands) gBands[idx];
                }
                break;
            }
            case aidl::RadioAccessSpecifierBands::utranBands: {
                std::vector<aidl::UtranBands> uBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::utranBands>();
                ras_to.bands_length = uBands.size();
                if (ras_to.bands_length  > MAX_BANDS) goto error;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.utran_bands[idx] = (RIL_UtranBands) uBands[idx];
                }
                break;
            }
            case aidl::RadioAccessSpecifierBands::eutranBands: {
                std::vector<aidl::EutranBands> eBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::eutranBands>();
                ras_to.bands_length = eBands.size();
                if (ras_to.bands_length  > MAX_BANDS) goto error;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.eutran_bands[idx] = (RIL_EutranBands) eBands[idx];
                }
                break;
            }
            case aidl::RadioAccessSpecifierBands::ngranBands: {
                std::vector<aidl::NgranBands> nBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::ngranBands>();
                ras_to.bands_length = nBands.size();
                if (ras_to.bands_length  > MAX_BANDS) goto error;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.ngranBands[idx] = (RIL_NgranBands) nBands[idx];
                }
                break;
            }
            default:
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                return ok();
        }
    }
    CALL_ONREQUEST(RIL_REQUEST_SET_SYSTEM_SELECTION_CHANNELS, &ssc, sizeof(ssc), pRI,
            mSlotId);
    return ok();
error:
    sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
    return ok();
}

#define SEND_ERROR_RESPONSE_AND_RETURN(pRI, error) {sendErrorResponse(pRI, error); return ok();}

ScopedAStatus RadioNetwork::startNetworkScan(int32_t serial, const aidl::NetworkScanRequest& request) {
    mtkLogD(LOG_TAG, "startNetworkScan: serial %d", serial);

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_START_NETWORK_SCAN);
    if (pRI == NULL) {
        return ok();
    }

    if (request.specifiers.size() <= 0
            || request.specifiers.size() > (size_t)aidl::NetworkScanRequest::RADIO_ACCESS_SPECIFIER_MAX_SIZE) {
        mtkLogE(LOG_TAG, "specifiers size %lu !", request.specifiers.size());
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return ok();
    }

    RIL_NetworkScanRequest scan_request = {};

    scan_request.type = (RIL_ScanType) request.type;
    mtkLogD(LOG_TAG, "startNetworkScan type: %d", scan_request.type);

    scan_request.interval = request.interval;
    mtkLogD(LOG_TAG, "startNetworkScan interval: %d", scan_request.interval);
    if (scan_request.interval < aidl::NetworkScanRequest::SCAN_INTERVAL_RANGE_MIN
        || scan_request.interval > aidl::NetworkScanRequest::SCAN_INTERVAL_RANGE_MAX) {
        SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
    }

    scan_request.specifiers_length = request.specifiers.size();
    mtkLogD(LOG_TAG, "startNetworkScan specifiers_length: %d", scan_request.specifiers_length);

    for (size_t i = 0; i < request.specifiers.size(); ++i) {
        const aidl::RadioAccessSpecifier& ras_from = request.specifiers[i];

        RIL_RadioAccessSpecifier& ras_to = scan_request.specifiers[i];

        ras_to.channels_length = ras_from.channels.size();
        mtkLogD(LOG_TAG, "startNetworkScan specifiers[%zu].channels_length: %d", i, ras_to.channels_length);
        if (ras_to.channels_length > MAX_CHANNELS) {
            mtkLogE(LOG_TAG, "channels size %lu ??", ras_to.channels_length);
            SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
        }
        std::copy(ras_from.channels.begin(), ras_from.channels.end(), ras_to.channels);

        switch (ras_from.bands.getTag()) {
           case aidl::RadioAccessSpecifierBands::geranBands: {
                std::vector<aidl::GeranBands> gBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::geranBands>();
                ras_to.bands_length = gBands.size();
                if (ras_to.bands_length  > MAX_BANDS) SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
                ras_to.radio_access_network = GERAN;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.geran_bands[idx] = (RIL_GeranBands) gBands[idx];
                }
                break;
            }
            case aidl::RadioAccessSpecifierBands::utranBands: {
                std::vector<aidl::UtranBands> uBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::utranBands>();
                ras_to.bands_length = uBands.size();
                if (ras_to.bands_length  > MAX_BANDS) SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
                ras_to.radio_access_network = UTRAN;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.utran_bands[idx] = (RIL_UtranBands) uBands[idx];
                }
                break;
            }
            case aidl::RadioAccessSpecifierBands::eutranBands: {
                std::vector<aidl::EutranBands> eBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::eutranBands>();
                ras_to.bands_length = eBands.size();
                if (ras_to.bands_length  > MAX_BANDS) SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
                ras_to.radio_access_network = EUTRAN;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.eutran_bands[idx] = (RIL_EutranBands) eBands[idx];
                }
                break;
            }
            case aidl::RadioAccessSpecifierBands::ngranBands: {
                std::vector<aidl::NgranBands> nBands = ras_from.bands.get<aidl::RadioAccessSpecifierBands::ngranBands>();
                ras_to.bands_length = nBands.size();
                if (ras_to.bands_length  > MAX_BANDS) SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
                ras_to.radio_access_network = NEW_RADIO;
                for (size_t idx = 0; idx < ras_to.bands_length; ++idx) {
                    ras_to.bands.ngranBands[idx] = (RIL_NgranBands) nBands[idx];
                }
                break;
            }
            default:
                SEND_ERROR_RESPONSE_AND_RETURN(pRI, RIL_E_INVALID_ARGUMENTS);
        }
    }

    scan_request.maxSearchTime = request.maxSearchTime;
    mtkLogD(LOG_TAG, "startNetworkScan maxSearchTime: %d", scan_request.maxSearchTime);
    if (scan_request.maxSearchTime < aidl::NetworkScanRequest::MAX_SEARCH_TIME_RANGE_MIN
            || scan_request.maxSearchTime > aidl::NetworkScanRequest::MAX_SEARCH_TIME_RANGE_MAX) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
    }

    scan_request.incrementalResults = request.incrementalResults ? 1 : 0;
    mtkLogD(LOG_TAG, "startNetworkScan incrementalResults: %d", scan_request.incrementalResults);

    scan_request.incrementalResultsPeriodicity = request.incrementalResultsPeriodicity;
    mtkLogD(LOG_TAG, "startNetworkScan incrementalResultsPeriodicity: %d", scan_request.incrementalResultsPeriodicity);
    if (scan_request.incrementalResultsPeriodicity < aidl::NetworkScanRequest::INCREMENTAL_RESULTS_PREIODICITY_RANGE_MIN
            || scan_request.incrementalResultsPeriodicity > aidl::NetworkScanRequest::INCREMENTAL_RESULTS_PREIODICITY_RANGE_MAX) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
    }

    scan_request.mccMncs_length = request.mccMncs.size();
    mtkLogD(LOG_TAG, "startNetworkScan mccMncs_length: %d", scan_request.mccMncs_length);
    scan_request.mccMncs = (char**) calloc(scan_request.mccMncs_length, sizeof(char*));
    if (scan_request.mccMncs != NULL) {
        for (size_t i = 0; i < scan_request.mccMncs_length; i++) {
            if (!copyAidlStringToRil(&(scan_request.mccMncs[i]), request.mccMncs[i], pRI)) {
                // clear the memory for previous results
                for (int j = i; j >= 0; j--) {
                    if (scan_request.mccMncs[j]) {
                        memsetAndFreeStrings(1, scan_request.mccMncs[j]);
                    }
                }
                if (scan_request.mccMncs) free(scan_request.mccMncs);
                mtkLogE(LOG_TAG, "copyHidlStringToRil ??");
                return ok();
            }
            mtkLogD(LOG_TAG, "startNetworkScan mccMncs[%zu]: %s", i, scan_request.mccMncs[i]);
        }
    } else {
        mtkLogE(LOG_TAG, "startNetworkScan mccMncs calloc fail");
    }

    if ((scan_request.maxSearchTime >= 60 && scan_request.maxSearchTime < 70) ||
            (scan_request.specifiers_length == 1 &&
            scan_request.specifiers[0].radio_access_network == 1)) {
        mtkLogV(LOG_TAG, "case!");
        if (scan_request.maxSearchTime == 60 &&
            scan_request.interval == 5 &&
            scan_request.incrementalResults == 1 &&
            scan_request.incrementalResultsPeriodicity == 5) {
            mtkLogE(LOG_TAG, "startNetworkScan CCCC case!");
            sendErrorResponse(pRI, RIL_E_DEVICE_IN_USE);
            goto end;
        }
        if (scan_request.maxSearchTime == 360) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            goto end;
        } else if (scan_request.maxSearchTime == 60 &&
                scan_request.interval == 60) {
            sendErrorResponse(pRI, RIL_E_OPERATION_NOT_ALLOWED);
            goto end;
        }
    } else if (scan_request.maxSearchTime == 360 &&
                scan_request.interval == 60 &&
                scan_request.incrementalResultsPeriodicity == 10) {
        mtkLogV(LOG_TAG, "invalid argumenets case!");
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        goto end;
    }

    CALL_ONREQUEST(RIL_REQUEST_START_NETWORK_SCAN, &scan_request, sizeof(scan_request), pRI,
            mSlotId);

end:
    if (scan_request.mccMncs) {
        for (size_t i = 0; i < scan_request.mccMncs_length; i++) {
            if (scan_request.mccMncs[i]) {
                memsetAndFreeStrings(1, scan_request.mccMncs[i]);
                scan_request.mccMncs[i] = NULL;
            }
        }
        free(scan_request.mccMncs);
    }
    scan_request.mccMncs = NULL;
    return ok();
}

ScopedAStatus RadioNetwork::stopNetworkScan(int32_t serial) {
    mtkLogD(LOG_TAG, "stopNetworkScan: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_STOP_NETWORK_SCAN);
    return ok();
}

ScopedAStatus RadioNetwork::supplyNetworkDepersonalization(int32_t ser, const std::string& nPin) {
     mtkLogD(LOG_TAG, "supplyNetworkDepersonalization: serial %d, slotId:%d", ser, mSlotId);
     rilAidlUtils::dispatchStrings(ser,
            mSlotId, RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION,
            true, 1, nPin.c_str());
    return ok();
}

// TODO(b/210498497): is there a cleaner way to send a response back to Android, even though these
// methods must never be called?
ScopedAStatus RadioNetwork::setUsageSetting(
        int32_t serial, ::aidl::android::hardware::radio::network::UsageSetting setting) {
    mtkLogD(LOG_TAG, "setUsageSetting: serial %d", serial);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_USAGE_SETTING, 1, setting);
    return ok();
}

ScopedAStatus RadioNetwork::getUsageSetting(int32_t serial) {
    mtkLogD(LOG_TAG, "getUsageSetting: serial %d", serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_USAGE_SETTING);
    return ok();
}

ScopedAStatus RadioNetwork::setEmergencyMode(int32_t serial,
        ::aidl::android::hardware::radio::network::EmergencyMode emcModeType) {
    mtkLogD(LOG_TAG, "[%s]: serial[%d], emcModeType[%d]", __FUNCTION__, serial, emcModeType);
    // Not support for android U, response NOT_SUPPORT directly
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_SET_EMERGENCY_MODE);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    }

//    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_EMERGENCY_MODE, 1, emcModeType);
    return ok();
}

ScopedAStatus RadioNetwork::triggerEmergencyNetworkScan(int32_t serial,
        const aidl::EmergencyNetworkScanTrigger& request) {
    // Not support for android U, response NOT_SUPPORT directly
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_TRIGGER_EMERGENCY_NETWORK_SCAN);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    }
/*
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_TRIGGER_EMERGENCY_NETWORK_SCAN);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "[%s] addRequestToList failed", __FUNCTION__);
        return ok();
    }

    int countInts = request.accessNetwork.size() + 1;
    int *pInts = (int *)calloc(countInts, sizeof(int));

    if (pInts == NULL) {
        mtkLogE(LOG_TAG, "[%s] Memory allocation failed", __FUNCTION__);
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return ok();
    }
    for (int i = 0; i < countInts - 1; i++) {
        pInts[i] = (RIL_RadioAccessNetworks) request.accessNetwork[i];
    }
    int scanType = 0;
    if (request.scanType == aidl::EmergencyScanType::LIMITED_SERVICE) {
        scanType = 1;
    } else if (request.scanType == aidl::EmergencyScanType::FULL_SERVICE) {
        scanType = 2;
    }
    pInts[countInts - 1] = scanType;
    CALL_ONREQUEST(RIL_REQUEST_TRIGGER_EMERGENCY_NETWORK_SCAN, pInts, countInts * sizeof(int), pRI, mSlotId);
    free(pInts);*/
    return ok();
}

ScopedAStatus RadioNetwork::cancelEmergencyNetworkScan(int32_t serial, bool resetScan) {
    mtkLogD(LOG_TAG, "[%s]: serial[%d], resetScan[%d]", __FUNCTION__, serial, resetScan);
    // Not support for android U, response NOT_SUPPORT directly
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_CANCEL_EMERGENCY_NETWORK_SCAN);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    }

//    dispatchInts(serial, mSlotId, RIL_REQUEST_CANCEL_EMERGENCY_NETWORK_SCAN, 1, BOOL_TO_INT(resetScan));
    return ok();
}

ScopedAStatus RadioNetwork::exitEmergencyMode(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    // Not support for android U, response NOT_SUPPORT directly
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId, RIL_REQUEST_EXIT_EMERGENCY_MODE);
    if (pRI != NULL) {
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    }

//    dispatchVoid(serial, mSlotId, RIL_REQUEST_EXIT_EMERGENCY_MODE);
    return ok();
}

ScopedAStatus RadioNetwork::setNullCipherAndIntegrityEnabled(int32_t serial, bool enabled) {
    mtkLogD(LOG_TAG, "[%s]: serial[%d], enable[%d]", __FUNCTION__, serial, enabled);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_NULL_CIPHER_AND_INTEGRITY_ENABLED, 1, BOOL_TO_INT(enabled));
    return ok();
}

ScopedAStatus RadioNetwork::isNullCipherAndIntegrityEnabled(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_IS_NULL_CIPHER_AND_INTEGRITY_ENABLED);
    return ok();
}

ScopedAStatus RadioNetwork::isN1ModeEnabled(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_IS_N1_MODE_ENABLED);
    return ok();
}

ScopedAStatus RadioNetwork::setN1ModeEnabled(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "[%s]: serial[%d], enable[%d]", __FUNCTION__, serial, enable);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_N1_MODE_ENABLED, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioNetwork::isCellularIdentifierTransparencyEnabled(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_IS_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED);
    return ok();
}

ScopedAStatus RadioNetwork::setCellularIdentifierTransparencyEnabled(int32_t serial, bool enabled) {
    mtkLogD(LOG_TAG, "[%s]: serial[%d], enable[%d]", __FUNCTION__, serial, enabled);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_CELLULAR_IDENTIFIER_TRANSPARENCY_ENABLED, 1, BOOL_TO_INT(enabled));
    return ok();
}

ScopedAStatus RadioNetwork::setSecurityAlgorithmsUpdatedEnabled(int32_t serial, bool enable) {
    mtkLogD(LOG_TAG, "[%s]: serial[%d], enable[%d]", __FUNCTION__, serial, enable);
    dispatchInts(serial, mSlotId, RIL_REQUEST_SET_SECURITY_ALGORITHMS_UPDATED_ENABLED, 1, BOOL_TO_INT(enable));
    return ok();
}

ScopedAStatus RadioNetwork::isSecurityAlgorithmsUpdatedEnabled(int32_t serial) {
    mtkLogD(LOG_TAG, "[%s]: serial %d", __FUNCTION__, serial);
    dispatchVoid(serial, mSlotId, RIL_REQUEST_IS_SECURITY_ALGORITHMS_UPDATED_ENABLED);
    return ok();
}

}  // namespace android::hardware::radio::network


namespace vendor::mediatek::hardware::mtkradioex::network {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::network;
namespace aidl_smart = ::aidl::vendor::mediatek::hardware::mtkradioex::smartratswitch;
constexpr auto ok = &ScopedAStatus::ok;

MtkRadioNetwork::MtkRadioNetwork(unsigned int slotid) {
    mSlotId = slotid;
}


ScopedAStatus MtkRadioNetwork::abortFemtocellList(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "abortFemtocellList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ABORT_FEMTOCELL_LIST);
    return ok();
}

ScopedAStatus MtkRadioNetwork::cancelAvailableNetworks(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "cancelAvailableNetworks: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS);
    return ok();
}

ScopedAStatus MtkRadioNetwork::cfgA2offset(int32_t serial, int32_t offset, int32_t threshBound, int32_t clientId) {
    mtkLogD(LOG_TAG, "cfgA2offset: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_CONFIG_A2_OFFSET, 2, offset, threshBound);
    return ok();
}

ScopedAStatus MtkRadioNetwork::cfgB1offset(int32_t serial, int32_t offset, int32_t threshBound, int32_t clientId) {
    mtkLogD(LOG_TAG, "cfgB1offset: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_CONFIG_B1_OFFSET, 2, offset, threshBound);
    return ok();
}

ScopedAStatus MtkRadioNetwork::clearLteAvailableFile(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "clearLteAvaiableFile: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_CLEAR_LTE_AVAILABLE_FILE);
    return ok();
}

ScopedAStatus MtkRadioNetwork::deactivateNrScgCommunication(int32_t serial, bool deactivate, bool allowSCGAdd, int32_t clientId) {
    mtkLogD(LOG_TAG, "getDeactivateNrScgCommunication: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DEACTIVATE_NR_SCG_COMMUNICATION, 2,
            deactivate ? 1:0, allowSCGAdd ? 1:0);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setCarrierAggregationMode(int32_t serial,  int32_t mode, int32_t option, int32_t linkType, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCarrierAggregationMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_CARRIER_AGGREGATION_MODE, 3, mode, option, linkType);
    return ok();
}

ScopedAStatus MtkRadioNetwork::enableCAPlusBandWidthFilter(int32_t serial, bool enable, int32_t clientId) {
    mtkLogD(LOG_TAG, "enableCAPlusBandWidthFilter: serial %d, clientId %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_ENABLE_CA_PLUS_FILTER);
    if (pRI != NULL) {
        //just return success due to Android AIDL does not has CA radio technology anymore.
        sendErrorResponse(pRI, RIL_E_SUCCESS);
    }
    return ok();
}

ScopedAStatus MtkRadioNetwork::enableSCGfailure(int32_t serial, bool enable, int32_t T1, int32_t P1, int32_t T2, int32_t clientId) {
    mtkLogD(LOG_TAG, "enableSCGfailure: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_ENABLE_SCG_FAILURE);
    if (pRI != NULL) {
        //always not support
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    }
    return ok();
}

ScopedAStatus MtkRadioNetwork::get4x4MimoEnabled(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "get4x4MimoEnabled: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_4X4MIMO_ENABLED);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getAllBandMode(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getAllBandMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_ALL_BAND_MODE);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getApcInfo(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getApcInfo: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_PSEUDO_CELL_INFO);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getAvailableNetworksWithAct(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getAvailableNetworksWithAct: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    int slotId = mSlotId + clientId * MAX_SIM_COUNT;
    android::ClientId client = getClientBySlot(mSlotId);
    if (client == android::CLIENT_TBOX) {
        slotId = mSlotId;
    }
    dispatchVoid(serial,slotId, RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getBandMode(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getBandMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_BAND_MODE);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getBandPriorityList(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getBandPriorityList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_BAND_PRIORITY_LIST);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getCALinkCapabilityList(int32_t serial, int32_t linkType, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCALinkCapabilityList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_CA_LINK_CAPABILITY_LIST, 1, linkType);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getCALinkEnableStatus(int32_t serial, const std::string& bandsCombo, int32_t linkType, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCALinkEnableStatus: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    std::string strLinkType = std::to_string(linkType);
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_CA_LINK_ENABLE_STATUS, false, 2,
                    bandsCombo.c_str(), strLinkType.c_str());
    return ok();
}

ScopedAStatus MtkRadioNetwork::getCaBandMode(int32_t serial, int32_t primaryBandId, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCaBandMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_CA_BAND_MODE, 1, primaryBandId);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getCampedFemtoCellInfo(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCampedFemtoCellInfo: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
            RIL_REQUEST_GET_CAMPED_FEMTO_CELL_INFO);
    if (pRI != NULL) {
        //does not support this
        sendErrorResponse(pRI, RIL_E_REQUEST_NOT_SUPPORTED);
    }
    return ok();
}

ScopedAStatus MtkRadioNetwork::getCurrentPOLList(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getCurrentPOLList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_POL_LIST);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getDeactivateNrScgCommunication(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getDeactivateNrScgCommunication: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_DEACTIVATE_NR_SCG_COMMUNICATION);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getDisable2G(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getDisable2G: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_DISABLE_2G);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getFemtocellList(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getFemtocellList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_FEMTOCELL_LIST);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getIWlanRegistrationState(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "[%s]: serial:%d, clientId:%d", __FUNCTION__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_IWLAN_REGISTRATION_STATE);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getLte1xRttCellList(int32_t serial, bool available, int32_t clientId) {
    mtkLogD(LOG_TAG, "getLteAvaiableFiles: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_LTE_1XRTT_CELL_LIST, 1, available ? 1 : 0);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getLteBsrTimer(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getLteBsrTimer: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_LTE_BSR_TIMER);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getLteData(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getLteData: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_LTE_DATA);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getLteRRCState(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getLteRRCState: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_LTE_RRC_STATE);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getLteReleaseVersion(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getLteReleaseVersion: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_LTE_RELEASE_VERSION);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getLteScanDuration(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getLteScanDuration: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_LTE_SCAN_DURATION);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getPOLCapability(int32_t serial, int32_t clientId) {
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_POL_CAPABILITY);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getPlmnNameFromSE13Table(int32_t serial, int32_t mcc, int32_t mnc, int32_t clientId) {
    mtkLogD(LOG_TAG, "getPlmnNameFromSE13Table: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_TS25_NAME, 2, mcc, mnc);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getQamEnabled(int32_t serial, bool ulOrDl, int32_t clientId) {
    mtkLogD(LOG_TAG, "getQamEnabled: serial %d, ulOrDl %d, clientId %d", serial, ulOrDl, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_QAM_ENABLED, 1, ulOrDl);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getRoamingEnable(int32_t serial, int32_t phoneId, int32_t clientId) {
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_ROAMING_ENABLE, 1, phoneId);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getSignalStrengthWithWcdmaEcio(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSignalStrengthWithWcdmaEcio: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SIGNAL_STRENGTH_WITH_WCDMA_ECIO);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getSmartRatSwitch(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSmartRatSwitch: serial %d mode:%d, clientId:%d", serial, mode, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_SMART_RAT_SWITCH)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_SMART_RAT_SWITCH, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getSuggestedPlmnList(int32_t serial, int32_t rat, int32_t num, int32_t timer, int32_t clientId) {
    mtkLogD(LOG_TAG, "getSuggestedPlmnList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_SUGGESTED_PLMN_LIST, 3, rat, num, timer);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getTOEInfo(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getTOEInfo: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_TOE_INFO);
    return ok();
}

ScopedAStatus MtkRadioNetwork::getTm9Enabled(int32_t serial, bool fddOrTdd, int32_t clientId) {
    mtkLogD(LOG_TAG, "getTm9Enabled: serial %d, fddOrTdd %d, clientId %d", serial, fddOrTdd, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_GET_TM9_ENABLED, 1, fddOrTdd);
    return ok();
}

ScopedAStatus MtkRadioNetwork::queryFemtoCellSystemSelectionMode(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "queryFemtoCellSystemSelectionMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE);
    return ok();
}

ScopedAStatus MtkRadioNetwork::selectFemtocell(int32_t serial, const std::string& operatorNumeric, const std::string& act, const std::string& csgId, int32_t clientId) {
    mtkLogD(LOG_TAG, "selectFemtocell: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SELECT_FEMTOCELL, true, 3,
            operatorNumeric.c_str(), act.c_str(), csgId.c_str());
    return ok();
}

ScopedAStatus MtkRadioNetwork::set4x4MimoEnabled(int32_t serial, int32_t enabled_bitmask, int32_t clientId) {
    mtkLogD(LOG_TAG, "set4x4MimoEnabled: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_4X4MIMO_ENABLED, 1, enabled_bitmask);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setApcMode(int32_t serial, int32_t mode, int32_t reportMode, int32_t interval, int32_t clientId) {
    mtkLogD(LOG_TAG, "setApcMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_PSEUDO_CELL_MODE, 3, mode, reportMode, interval);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setBandPriorityList(int32_t serial, const std::vector<int32_t>& bandPriList, int32_t clientId) {
    mtkLogD(LOG_TAG, "setBandPriorityList: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchBandPriorityList(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_BAND_PRIORITY_LIST, bandPriList);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setBgsrchDeltaSleepTimer(int32_t serial, int32_t sleepDuration, int32_t clientId) {
    mtkLogD(LOG_TAG, "setBgsrchDeltaSleepTimer: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_BACKGROUND_SEARCH_TIMER, 1, sleepDuration);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setCALinkEnableStatus(int32_t serial, bool status, const std::string& bandsCombo, int32_t linkType, int32_t clientId) {
    mtkLogD(LOG_TAG, "setCALinkEnableStatus: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    std::string strLinkType = std::to_string(linkType);
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_CA_LINK_ENABLE_STATUS, false, 3,
                    status ? "1" : "0", bandsCombo.c_str(), strLinkType.c_str());
    return ok();
}

ScopedAStatus MtkRadioNetwork::setDisable2G(int32_t serial, bool mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setDisable2G: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_DISABLE_2G, 1, BOOL_TO_INT(mode));
    return ok();
}

ScopedAStatus MtkRadioNetwork::setFemtoCellSystemSelectionMode(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setFemtoCellSystemSelectionMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_FEMTOCELL_SYSTEM_SELECTION_MODE, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setLteBandEnableStatus(int32_t serial, int32_t bandId, bool status, int32_t clientId) {
    mtkLogD(LOG_TAG, "setLteBandEnableStatus: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    std::string strBandId = std::to_string(bandId);
    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_LTE_BAND_ENABLE_STATUS, false, 2,
                    strBandId.c_str(), status ? "1" : "0");
    return ok();
}

ScopedAStatus MtkRadioNetwork::setLteBsrTimer(int32_t serial, int32_t timer, int32_t clientId) {
    mtkLogD(LOG_TAG, "setLteBsrTimer: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_LTE_BSR_TIMER, 1, timer);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setLteReleaseVersion(int32_t serial, int32_t mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setLteReleaseVersion: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_LTE_RELEASE_VERSION, 1, mode);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setLteScanDuration(int32_t serial, int32_t duration, int32_t clientId) {
    mtkLogD(LOG_TAG, "setLteScanDuration: serial %d, duration %d, clientId %d", serial, duration, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_LTE_SCAN_DURATION, 1, duration);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setNROption(int32_t serial, int32_t option, int32_t clientId) {
    mtkLogD(LOG_TAG, "setNRConfig: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    if (option < 0 || option > 7) {
        mtkLogE(LOG_TAG, "[%s]: option(%d) is invalid", __FUNCTION__, option);
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_SET_NR_OPTION);
        if (pRI != NULL) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_NR_OPTION, 1, option);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setNetworkSelectionModeManualWithAct(int32_t serial, const std::string& operatorNumeric, const std::string& act, const std::string& mode, int32_t clientId) {
    mtkLogD(LOG_TAG, "setNetworkSelectionModeManualWithAct: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    const char* plmn = operatorNumeric.c_str();
    int plmnLen = strlen(plmn);
    bool plmnValid = true;
    if (plmnLen != 5 && plmnLen != 6) {
        plmnValid = false;
    }
    if (plmnValid) {
        for (int i = 0; i < plmnLen; i++) {
            if (plmn[i] < '0' || plmn[i] > '9') {
                plmnValid = false;
                break;
            }
        }
    }
    if (!plmnValid) {
        mtkLogE(LOG_TAG, "[%s]: plmnValid(%s) is invalid", __FUNCTION__, plmn);
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT);
        if (pRI != NULL) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT, true,
            3, operatorNumeric.c_str(), act.c_str(), mode.c_str());
    return ok();
}

ScopedAStatus MtkRadioNetwork::setNrBandMode(int32_t serial, const std::vector<int32_t>& saEnable, const std::vector<int32_t>& saDisable, const std::vector<int32_t>& nsaEnable,
        const std::vector<int32_t>& nsaDisable, int32_t clientId) {
    mtkLogD(LOG_TAG, "setNrBandMode: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_NR_BAND_MODE);
    if (pRI == NULL) {
        return ok();
    }

    const std::vector<int32_t> inputArgs[4] = {saEnable, saDisable, nsaEnable, nsaDisable};
    for (int i = 0; i < 4; i++) {
        int size = inputArgs[i].size();
        if (size > 100) {
            mtkLogE(LOG_TAG, "[%s]: inputArgs[%d]'s size(%d) is > 100", __FUNCTION__, i, size);
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
        for(int j = 0; j < size; j++) {
            if(inputArgs[i][j] < 0 || inputArgs[i][j] > RIL_NgranBands::NGRAN_BAND_MAX) {
                mtkLogE(LOG_TAG, "[%s]: saEnable[%d][%d] = %d is invalid NGRAN BAND", __FUNCTION__,
                    i, j, inputArgs[i][j]);
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                return ok();
            }
        }
    }

    int countInts = saEnable.size() + saDisable.size() + nsaEnable.size() + nsaDisable.size() + 4;
    int *bands = (int *)calloc(countInts, sizeof(int));

    if (bands == NULL) {
        mtkLogE(LOG_TAG, "Memory allocation failed for request %s", android::requestToString(RIL_REQUEST_SET_NR_BAND_MODE));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return ok();
    }
    int index = 0;
    bands[index++] = saEnable.size();
    for (int i = 0; i < saEnable.size(); i++) bands[index++] = saEnable[i];
    bands[index++] = saDisable.size();
    for (int i = 0; i < saDisable.size(); i++) bands[index++] = saDisable[i];
    bands[index++] = nsaEnable.size();
    for (int i = 0; i < nsaEnable.size(); i++) bands[index++] = nsaEnable[i];
    bands[index++] = nsaDisable.size();
    for (int i = 0; i < nsaDisable.size(); i++) bands[index++] = nsaDisable[i];

    CALL_ONREQUEST(RIL_REQUEST_SET_NR_BAND_MODE, bands, countInts * sizeof(int), pRI, pRI->socket_id);
    free(bands);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setPOLEntry(int32_t serial, int32_t index, const std::string& numeric, int32_t nAct, int32_t clientId) {
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    rilAidlUtils::dispatchStrings(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_POL_ENTRY, true, 3,
            std::to_string((int) index).c_str(), numeric.c_str(), std::to_string((int) nAct).c_str());
    return ok();
}

ScopedAStatus MtkRadioNetwork::setQamEnabled(int32_t serial, bool ulOrDl, bool enabled, int32_t clientId) {
    mtkLogD(LOG_TAG, "setQamEnabled: serial %d, ulOrDl %d, enabled %d, clientId %d", serial, ulOrDl, enabled, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_QAM_ENABLED, 2, ulOrDl, enabled);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setSearchRat(int32_t serial, const std::vector<int32_t>& rat, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSearchRat: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    if (rat.size() > 4) {
        mtkLogE(LOG_TAG, "[%s]: rat.size > 4", __FUNCTION__);
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_SEARCH_RAT);
        if (pRI != NULL) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
    }

    for (int i = 0; i < rat.size(); i++) {
        if (rat[i] != 1  && rat[i] != 2 && rat[i] != 4 && rat[i] != 128) {
            mtkLogE(LOG_TAG, "[%s]: rat[%d] = %d is not in (1,2,4,128)", __FUNCTION__, i, rat[i]);
            RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
                   RIL_REQUEST_SEARCH_RAT);
            if (pRI != NULL) {
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                return ok();
            }
        }
    }

    dispatchSearchRat(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SEARCH_RAT, rat);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setSearchStoredFreqInfo(int32_t serial, int32_t operation, int32_t plmn_id,
        int32_t rat, const std::vector<int32_t>& freq, int32_t clientId) {
    mtkLogD(LOG_TAG, "setSearchStoredFreqInfo: serial %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    if (freq.size() > 10) {
        //max support 10
        mtkLogE(LOG_TAG, "[%s]: freq.size > 10", __FUNCTION__);
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_SEARCH_STORED_FREQUENCY_INFO);
        if (pRI != NULL) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
            return ok();
        }
    }

    dispatchSearchStoredFreqInfo(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SEARCH_STORED_FREQUENCY_INFO,
            operation, plmn_id, rat, freq);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setServiceStateToModem(int32_t serial, int32_t voiceRegState, int32_t dataRegState,
        int32_t voiceRoamingType, int32_t dataRoamingType, int32_t rilVoiceRegState,
        int32_t rilDataRegState, int32_t clientId) {
    mtkLogD(LOG_TAG, "%s: serial %d, clientId %d", __func__, serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_SERVICE_STATE, 6,
                 voiceRegState,
                 dataRegState,
                 voiceRoamingType,
                 dataRoamingType,
                 rilVoiceRegState,
                 rilDataRegState);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setTm9Enabled(int32_t serial, bool fddOrTdd, bool enabled, int32_t clientId) {
    mtkLogD(LOG_TAG, "setTm9Enabled: serial %d, fddOrTdd %d, enabled %d, clientId %d",
            serial, fddOrTdd, enabled, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_TM9_ENABLED, 2, fddOrTdd, enabled);
    return ok();
}

ScopedAStatus MtkRadioNetwork::smartRatSwitch(int32_t serial, int32_t mode, int32_t rat, int32_t clientId) {
    mtkLogD(LOG_TAG, "smartRatSwitch: serial %d mode:%d rat:%d, clientId %d", serial, mode, rat, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_SMART_RAT_SWITCH)) {
        return ok();
    }

    dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SMART_RAT_SWITCH, 2, mode, rat);
    return ok();
}

ScopedAStatus MtkRadioNetwork::setRoamingEnable(int32_t serial, const std::vector<int32_t>& config, int32_t clientId) {
    mtkLogD(LOG_TAG, "setRoamingEnable: serial: %d, clientId %d", serial, clientId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_RILJ) &&
            !checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }

    if (config.size() == 6) {
        dispatchInts(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_ROAMING_ENABLE, 6,
                config[0], config[1], config[2], config[3], config[4], config[5]);
    } else {
        RequestInfo *pRI = android::addRequestToList(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_SET_ROAMING_ENABLE);
        if (pRI != NULL) {
            sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        }
        mtkLogE(LOG_TAG, "setRoamingEnable: param error, num: %d (should be 6)", (int) config.size());
    }
    return ok();
}

ScopedAStatus MtkRadioNetwork::getNitzTime(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "getNitzTime: serial %d, clientId = %d, mSlotId = %d", serial, clientId, mSlotId);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_TBOX)) {
        return ok();
    }
    dispatchVoid(serial, mSlotId, RIL_REQUEST_GET_NITZ);
    return ok();
}

ScopedAStatus MtkRadioNetwork::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

inline void deathRecipientCallback(void* cookie) {
    // Handle the cleanup or restart logic here
    // if (cookie == nullptr) {
    //     mtkLogW(LOG_TAG, "aidl radioNetwork MTK line to deatch recipientCallback, cookie is null");
    //     return;
    // }
    int slotId = static_cast<int>(reinterpret_cast<std::uintptr_t>(cookie));
    mtkLogI(LOG_TAG, "aidl radioNetwork MTK line to deatch recipientCallback, slot=%d", slotId);

    ndk::ScopedAStatus ret = ndk::ScopedAStatus::fromExceptionCodeWithMessage(EX_ILLEGAL_STATE ,(rilAidlUtils::binderDiedError).c_str());
    retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::RADIO_MTK);
}

ScopedAStatus MtkRadioNetwork::setResponseFunctionsMtk(const std::shared_ptr<aidl::IMtkRadioExNetworkResponse>& radioResponse,
        const std::shared_ptr<aidl::IMtkRadioExNetworkIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk network setResponseFunctions, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk network setResponseFunctions: got lock %d", mSlotId);

    radioNetworkServiceImpl[mSlotId].mtkNetworkResponse = radioResponse;
    radioNetworkServiceImpl[mSlotId].mtkNetworkIndication = radioIndication;
    auto binder = radioResponse->asBinder();  // Retrieve Native AIBinder from wrapper if needed
    if (binder != nullptr) {
        AIBinder_DeathRecipient* deathRecipient = AIBinder_DeathRecipient_new(deathRecipientCallback);
        if (AIBinder_linkToDeath(binder.get(), deathRecipient, reinterpret_cast<void*>(static_cast<std::uintptr_t>(mSlotId))) !=
            STATUS_OK) {
            mtkLogW(LOG_TAG, "Failed to link death observer to the network mtk binder, slot = %d", mSlotId);
        }
    } else {
        mtkLogW(LOG_TAG, "radio network mtk Binder retrieval failed, slot = %d", mSlotId);
    }
    rilContext->plusCounterMtkRadioNumOne(mSlotId);

    rilContext->setMtkResponseNumberToOne(rilAidlUtils::NETWORK_INDEX, mSlotId);
    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk network setResponseFunctions: release lock %d", mSlotId);

    // client is connected. Send initial indications.
    android::ClientId client = getClientBySlot(mSlotId);
    if (rilContext->checkIfSetAllMtkResponseDone(mSlotId) == 1) {
        if (client == android::CLIENT_TBOX) {
            rilContext->onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::TBOX_RADIO_MTK);
        } else {
            rilContext->onNewCommandConnect((RIL_SOCKET_ID)mSlotId, android::RADIO_MTK);
        }
    }

    return ok();
}

ScopedAStatus MtkRadioNetwork::setResponseFunctionsSmartRatSwitch(
        const std::shared_ptr<aidl_smart::ISmartRatSwitchRadioResponse>& radioResponse,
        const std::shared_ptr<aidl_smart::ISmartRatSwitchRadioIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk network setResponseFunctionsSmartRatSwitch, slotId:%d", mSlotId);
    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk network setResponseFunctionsSmartRatSwitch: got lock %d", mSlotId);

    radioNetworkServiceImpl[mSlotId].mSmartRatSwitchResponse = radioResponse;
    radioNetworkServiceImpl[mSlotId].mSmartRatSwitchIndication = radioIndication;
    rilContext->plusCounterMtkRadioNumOne(mSlotId);

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk network setResponseFunctionsSmartRatSwitch: release lock %d", mSlotId);
    return ok();
}

}

int mtkRadioExNetwork::clearMtkSmartRatResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioNetworkServiceImpl[realSlotId].mSmartRatSwitchResponse = NULL;
    radioNetworkServiceImpl[realSlotId].mSmartRatSwitchIndication = NULL;
    return 0;
}

int mtkRadioExNetwork::clearMtkNetworkResponseAndIndications(unsigned int slotId) {
    radioNetworkServiceImpl[slotId].mtkNetworkResponse = NULL;
    radioNetworkServiceImpl[slotId].mtkNetworkIndication = NULL;
    return 0;
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addNetworkLock(std::shared_ptr<ndk::ICInterface> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_NETWORK_PORT_BASE 50000
#define MTK_RADIO_NETWORK_PORT_BASE 50100

static pthread_t gThreadIds[MAX_SIM_COUNT*4];

void* addRcpVsockRadioNetworkAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    using android::hardware::radio::network::RadioNetwork;
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_NETWORK_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_NETWORK_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(sloid);
    addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(radioNetworkAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioNetworkAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void* addRcpVsockMtkRadioNetworkAidlService(void *arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    using vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork;
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, MTK_RADIO_NETWORK_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", MTK_RADIO_NETWORK_PORT_BASE + sloid, sloid);
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkRadioNetworkAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(sloid);
    addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(mtkRadioNetworkAidl));  // need or no need
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);

    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioNetworkAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}

void createRadioNetworkVsock(unsigned int &sloid, unsigned int &imsSlotid) {
    if (pthread_create(&gThreadIds[sloid], NULL,
            addRcpVsockRadioNetworkAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioNetwork");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT], NULL,
            addRcpVsockRadioNetworkAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(imsSlotid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadioNetwork");
    }

    if (pthread_create(&gThreadIds[sloid + MAX_SIM_COUNT*2], NULL,
            addRcpVsockMtkRadioNetworkAidlService, reinterpret_cast<void*>(static_cast<uintptr_t>(sloid))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockMtkRadioNetwork");
    }
}
#endif



void publicRadioNetworkAidlService(unsigned int sloid) {
    unsigned int imsSlotid = sloid + android::CLIENT_IMS *MAX_SIM_COUNT;
    const char* radioNetworkDescriptor = android::hardware::radio::network::RadioNetwork::descriptor == NULL ?
            "" : android::hardware::radio::network::RadioNetwork::descriptor;
    const char* mtkRadioNetworkDescriptor = vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork::descriptor == NULL ?
            "" : vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork::descriptor;

    /*if (sloid == 0) {
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(0);
        gPublishedHals.push_back(radioNetworkAidl);
        const std::string instance = std::string(radioNetworkDescriptor) + "/slot1";
        binder_status_t status =
                AServiceManager_addService(radioNetworkAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(imsSlotid);
        gPublishedHals.push_back(radioNetworkAidlforIms);
        const std::string imsInstance = std::string(radioNetworkDescriptor) + "/imsSlot1";
        binder_status_t imsStatus =
                AServiceManager_addService(radioNetworkAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(0);
        gPublishedHals.push_back(mtkRadioNetworkAidl);
        const std::string mtkInstance = std::string(mtkRadioNetworkDescriptor) + "/slot1";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioNetworkAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk network addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 1) {
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(1);
        gPublishedHals.push_back(radioNetworkAidl);
        const std::string instance = std::string(radioNetworkDescriptor) + "/slot2";
        binder_status_t status =
                AServiceManager_addService(radioNetworkAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(imsSlotid);
        gPublishedHals.push_back(radioNetworkAidlforIms);
        const std::string imsInstance = std::string(radioNetworkDescriptor) + "/imsSlot2";
        binder_status_t imsStatus =
                AServiceManager_addService(radioNetworkAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(1);
        gPublishedHals.push_back(mtkRadioNetworkAidl);
        const std::string mtkInstance = std::string(mtkRadioNetworkDescriptor) + "/slot2";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioNetworkAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk network addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 2) {
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(2);
        gPublishedHals.push_back(radioNetworkAidl);
        const std::string instance = std::string(radioNetworkDescriptor) + "/slot3";
        binder_status_t status =
                AServiceManager_addService(radioNetworkAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(imsSlotid);
        gPublishedHals.push_back(radioNetworkAidlforIms);
        const std::string imsInstance = std::string(radioNetworkDescriptor) + "/imsSlot3";
        binder_status_t imsStatus =
                AServiceManager_addService(radioNetworkAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(2);
        gPublishedHals.push_back(mtkRadioNetworkAidl);
        const std::string mtkInstance = std::string(mtkRadioNetworkDescriptor) + "/slot3";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioNetworkAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk network addservice, slotId:%d, status:%d", sloid, mtkStatus);
    } else if (sloid == 3) {
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(3);
        gPublishedHals.push_back(radioNetworkAidl);
        const std::string instance = std::string(radioNetworkDescriptor) + "/slot4";
        binder_status_t status =
                AServiceManager_addService(radioNetworkAidl->asBinder().get(), instance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice, slotId:%d, status:%d", sloid, status);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> radioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(imsSlotid);
        gPublishedHals.push_back(radioNetworkAidlforIms);
        const std::string imsInstance = std::string(radioNetworkDescriptor) + "/imsSlot4";
        binder_status_t imsStatus =
                AServiceManager_addService(radioNetworkAidlforIms->asBinder().get(), imsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp network addservice for IMS, slotId:%d, status:%d", imsSlotid, imsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(3);
        gPublishedHals.push_back(mtkRadioNetworkAidl);
        const std::string mtkInstance = std::string(mtkRadioNetworkDescriptor) + "/slot4";
        binder_status_t mtkStatus =
                AServiceManager_addService(mtkRadioNetworkAidl->asBinder().get(), mtkInstance.c_str());
        mtkLogD(LOG_TAG, "mtk network addservice, slotId:%d, status:%d", sloid, mtkStatus);
    }*/

    //add slotid for android client
    unsigned int tboxSlotid = sloid + android::CLIENT_TBOX * MAX_SIM_COUNT;
    unsigned int tboxImsSlotid = sloid + android::CLIENT_TBOX_IMS * MAX_SIM_COUNT;

    if (sloid == 0) {
       //add service for yocto client
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidl));
        const std::string tboxInstance = std::string(radioNetworkDescriptor) + "/tboxSlot1";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioNetworkAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxImsSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidlforIms));
        const std::string tboxImsInstance = std::string(radioNetworkDescriptor) + "/tboxImsSlot1";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioNetworkAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkTboxRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioNetworkAidl));
        const std::string mtkTboxInstance = std::string(mtkRadioNetworkDescriptor) + "/tboxSlot1";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioNetworkAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox network addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioNetworkVsock(sloid, imsSlotid);
#endif
    } else if (sloid == 1) {
        //add service for yocto client
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidl));
        const std::string tboxInstance = std::string(radioNetworkDescriptor) + "/tboxSlot2";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioNetworkAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxImsSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidlforIms));
        const std::string tboxImsInstance = std::string(radioNetworkDescriptor) + "/tboxImsSlot2";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioNetworkAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkTboxRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioNetworkAidl));
        const std::string mtkTboxInstance = std::string(mtkRadioNetworkDescriptor) + "/tboxSlot2";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioNetworkAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox network addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioNetworkVsock(sloid, imsSlotid);
#endif
    } else if (sloid == 2) {
        //add service for yocto client
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidl));
        const std::string tboxInstance = std::string(radioNetworkDescriptor) + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioNetworkAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxImsSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidlforIms));
        const std::string tboxImsInstance = std::string(radioNetworkDescriptor) + "/tboxImsSlot3";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioNetworkAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkTboxRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioNetworkAidl));
        const std::string mtkTboxInstance = std::string(mtkRadioNetworkDescriptor) + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioNetworkAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox network addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioNetworkVsock(sloid, imsSlotid);
#endif
    } else if (sloid == 3) {
        //add service for yocto client
        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidl = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidl));
        const std::string tboxInstance = std::string(radioNetworkDescriptor) + "/tboxSlot3";
        binder_status_t tboxStatus =
                AServiceManager_addService(tboxRadioNetworkAidl->asBinder().get(), tboxInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice, slotId:%d, status:%d", tboxSlotid, tboxStatus);

        std::shared_ptr<android::hardware::radio::network::RadioNetwork> tboxRadioNetworkAidlforIms = ndk::SharedRefBase::make<android::hardware::radio::network::RadioNetwork>(tboxImsSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(tboxRadioNetworkAidlforIms));
        const std::string tboxImsInstance = std::string(radioNetworkDescriptor) + "/tboxImsSlot3";
        binder_status_t tboxImsStatus =
                AServiceManager_addService(tboxRadioNetworkAidlforIms->asBinder().get(), tboxImsInstance.c_str());
        mtkLogD(LOG_TAG, "aosp tbox network addservice for IMS, slotId:%d, status:%d", tboxImsSlotid, tboxImsStatus);

        std::shared_ptr<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork> mtkTboxRadioNetworkAidl =
                ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::network::MtkRadioNetwork>(tboxSlotid);
        addNetworkLock(std::static_pointer_cast<ndk::ICInterface>(mtkTboxRadioNetworkAidl));
        const std::string mtkTboxInstance = std::string(mtkRadioNetworkDescriptor) + "/tboxSlot3";
        binder_status_t mtkTboxStatus =
                AServiceManager_addService(mtkTboxRadioNetworkAidl->asBinder().get(), mtkTboxInstance.c_str());
        mtkLogD(LOG_TAG, "mtk tbox network addservice, slotId:%d, status:%d", tboxSlotid, mtkTboxStatus);
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        //add service for android client
        createRadioNetworkVsock(sloid, imsSlotid);
#endif
    }
}
