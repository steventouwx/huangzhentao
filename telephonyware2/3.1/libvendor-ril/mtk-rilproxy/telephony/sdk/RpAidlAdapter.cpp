// SPDX-License-Identifier: MediaTekProprietary

#include "android/binder_auto_utils.h"
#include "android/binder_status.h"
#include "utils/Errors.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <mtk_log.h>
#include <stdint.h>
#include <string>
#define LOG_TAG "RP_Aidl_ADAPTER"

#include <inttypes.h>
#include "RpAidlAdapter.h"
#include <pthread.h>
#include <string.h>
#include "TeleService.h"
#include "TeleData.h"
#include "VoiceService.h"
#include "../include/telephony/ril.h"
#include "RpDataUtils.h"
#include "Wrapper.h"
#include "SignalStrength.h"
#include "RpApnController.h"
#include "MbrainClient.h"
#include <prop/properties.h>

using aidl::vendor::mediatek::hardware::tbox::ITeleService;
using aidl::vendor::mediatek::hardware::tbox::ITeleData;

using namespace VENDOR_SPACE;

int RpAidlAdapter::tokenCounter = 0;
Mutex RpAidlAdapter::token_mutex;
Mutex RpAidlAdapter::m_reqMapMutex;
int RpAidlAdapter::INDEX_TELE = 1000;
int RpAidlAdapter::INDEX_DATA = 1000;

extern Mutex gRequestLock;
const char* RpAidlAdapter::teleserviceNames[MAX_SLOT_NUM] = {"tele_slot1", "tele_slot2"};
const char* RpAidlAdapter::dataserviceNames[MAX_SLOT_NUM] = {"data_slot1", "data_slot2"};
const char* RpAidlAdapter::voiceserviceNames[MAX_SLOT_NUM] = {"voice_slot1", "voice_slot2"};

RpAidlAdapter* RpAidlAdapter::sInstance = NULL;
std::shared_ptr<TeleService> teleService[MAX_SLOT_NUM];
std::shared_ptr<TeleData> dataService[MAX_SLOT_NUM];
std::shared_ptr<VoiceService> voiceService[MAX_SLOT_NUM];
bool RpAidlAdapter::isLockRelease = true;
bool RpAidlAdapter::isStartLockRelease = false;
const std::unordered_set<int> RpAidlAdapter::allAsyncRequests = {
    RIL_REQUEST_SETUP_DATA_CALL,
    RIL_REQUEST_DEACTIVATE_DATA_CALL,
    RIL_REQUEST_SEND_SMS,
    RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL,
    RIL_REQUEST_GET_CURRENT_CALLS,
    RIL_REQUEST_RADIO_POWER,
    RIL_REQUEST_MODEM_POWEROFF,
    RIL_REQUEST_MODEM_POWERON,
    RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP,
    RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE,
    RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT,
    RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
    RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL,
    RIL_REQUEST_DATA_REGISTRATION_STATE,
    RIL_REQUEST_VOICE_REGISTRATION_STATE,
    RIL_REQUEST_GET_CELL_INFO_LIST,
    RIL_REQUEST_LAST_CALL_FAIL_CAUSE,
};
int RpAidlAdapter::defaultAtSlot = 0;
std::mutex RpAidlAdapter::mtx;
std::condition_variable RpAidlAdapter::cv;
bool RpAidlAdapter::rilAidlThreadPoolCompleted = false;

static VENDOR_SPACE::VcallMsg callMsg={-1,"",VENDOR_SPACE::VcallStateType::E_CA_VCALL_STATE_END, -1, 0};
static VENDOR_SPACE::VcallMsg old_callMsg={-1,"",VENDOR_SPACE::VcallStateType::E_CA_VCALL_STATE_END, -1, 0};
static MTK_Data_Call_Response_v1 mDataCallRep[8] = {
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0},
    {0, 0, 0, 0, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", 0, 0}};

#ifdef NET_SOCKET_SUPPORT
int writeToNetsocket(void *data, size_t len);
#endif

namespace rilimpl {
    std::string convertCharPtrToAidlString(const char *ptr);
    void responseGetImei(int token, RIL_Errno e, void *response, size_t responselen);
    void responseIccOpenLogicalChannel(int token, RIL_Errno e, void *response, size_t responselen);
    void responseDataCall(int slotId, int token, RIL_Errno e, void *response, size_t responselen, int actType);
    void responseSignalStrength(int token, RIL_Errno e, void *response, size_t responselen);
    void pdnStateChangedInd(int slotId, int token, void *response, size_t responselen);
    void responseNoData(int token, RIL_Errno e, void *response, size_t responselen);
    void responseRetryTimes(int token, RIL_Errno e, void *response,size_t responselen);
    void responseGetApn(int token, RIL_Errno e, void *response, size_t responselen);
    void responseSendSms(int token, RIL_Errno e, void *response, size_t responselen);
    void newSmsInd(int slotId, int token, void *response, size_t responselen);
    void responseCurrentCalls(int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void responseLastCallFailCause(int token, RIL_Errno e, void *response, size_t responselen);
    void* callStatechanged(void* arg, int slot_id);
    void responseDataNoHandle(int token, RIL_Errno e, void *response, size_t responselen);
    void signalStrengthChangedInd(int slotId, const void *response, size_t responselen);
    void cellInfoListInd(int slotId, const void *response, size_t responselen);
    void ecallIndicationInd(int slotId, const void *response, size_t responselen);
    void responseImsRegState(int token, RIL_Errno e, void *response, size_t responselen);
    void responseGetOprtMode(int token, RIL_Errno e, void *response, size_t responselen);
    void translateCallState(RIL_CallState inCallState, int* stateEnum);
    void handleCallMsgInd(int slotId);
    void freeCalllist(RIL_Call **pCallLists, int callCount);
    void responseCallWaiting(int token, RIL_Errno e, void *response, size_t responselen);
    void keepaliveStatus(int slotId, const void *response, size_t responselen);
    void Autoanswer(int slotId);
    void* handlGetCardStatus(void* arg);
    void* handlGetMccMnc(void* arg);
    void* handlGetRfStatus(void* arg);
    std::string convertCharPtrToStdString(const char* str);
    void responseSetSimIo(int token, RIL_Errno e, void *response, size_t responselen);
    void responsePsNetworkStateChangeInd(int slotId, const void *response, size_t responselen);
    void responseCsNetworkStateChangeInd(int slotId, const void *response, size_t responselen);
    void responseCardStateChangedInd(int slotId, const void *response, size_t responselen);
    void nitzTimeReceived(int slotId, const void *response, size_t responselen);
    void getAvailableNetworksWithActResponse(int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void sendSmsResponse(int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void requestResposneNoDataForAsync(int request, int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void getRegistrationStateRespose(int request, int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void getCellInfoListResponse(int request, int slotId, int token, RIL_Errno e, void *response, size_t responselen);
    void urcIndWithoutData(int urc, int slotId, const void *response, size_t responselen);
    void radioStateChanged(int slotId, const void *response, size_t responselen);
};

void rilimpl::urcIndWithoutData(int urc, int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (teleService[slotId]->mTeleIndications.empty()) {
        RLOGE("%s,%d mTeleIndications is empty", __func__, slotId);
        return;
    }

    instance->mTeleServiceClientMapMutex.lock();
    for (auto& it : teleService[slotId]->mTeleIndications) {
        RLOGD("mTeleIndications(%d), urcInd(%d)", it.first, urc);
        if ((it.second)) {
            ::ndk::ScopedAStatus rc = ::ndk::ScopedAStatus::ok();
            switch (urc) {
                case RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED:
                {
                    rc = it.second->imsNetworkStateChanged();
                    break;
                }
                case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
                {
                    rc = it.second->networkStateChanged();
                    break;
                }
            }
            if (!rc.isOk()) {
                RLOGE("%s,%d call  urcInd(%d) failed.", __func__, slotId, urc);
            }
        }
    }
    instance->mTeleServiceClientMapMutex.unlock();
    return;
}

void rilimpl::getCellInfoListResponse(int request, int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    if (e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("getCellInfoListResponse request failed. e=%d", e);
        return;
    }
    std::vector<VENDOR_SPACE::CellInfo> records;
    char* mcc = NULL;
    char* mnc = NULL;
    char* alphAlong = NULL;
    char* alphaShort = NULL;
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int32_t size = p.readInt32();
    records.resize(size);
    RLOGD("getCellInfoListResponse size=%d", size);
    for (int i = 0; i < size; i++) {
        records[i].registered = p.readBool();
        records[i].connectionStatus = static_cast<VENDOR_SPACE::CellConnectionStatus>(p.readInt32());
        int32_t tag = p.readInt32();
        switch (tag) {
            case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::gsm: {
                VENDOR_SPACE::CellInfoGsm cell;
                mcc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityGsm.mcc = convertCharPtrToStdString(mcc);
                mnc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityGsm.mnc = convertCharPtrToStdString(mnc);
                cell.cellIdentityGsm.lac = p.readInt32();
                cell.cellIdentityGsm.cid = p.readInt32();
                cell.cellIdentityGsm.arfcn = p.readInt32();
                cell.cellIdentityGsm.bsic = (uint8_t)(p.readInt32());
                alphAlong = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityGsm.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                alphaShort = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityGsm.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                cell.cellIdentityGsm.additionalPlmns = {};
                cell.signalStrengthGsm.signalStrength = p.readInt32();
                cell.signalStrengthGsm.bitErrorRate = p.readInt32();
                cell.signalStrengthGsm.timingAdvance = p.readInt32();
                records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                break;
            }
            case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::wcdma: {
                VENDOR_SPACE::CellInfoWcdma cell;
                mcc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityWcdma.mcc = convertCharPtrToStdString(mcc);
                mnc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityWcdma.mnc = convertCharPtrToStdString(mnc);
                cell.cellIdentityWcdma.lac = p.readInt32();
                cell.cellIdentityWcdma.cid = p.readInt32();
                cell.cellIdentityWcdma.psc = p.readInt32();
                cell.cellIdentityWcdma.uarfcn = p.readInt32();
                alphAlong = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityWcdma.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                alphaShort = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityWcdma.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                cell.cellIdentityWcdma.additionalPlmns = {};
                cell.cellIdentityWcdma.csgInfo = {};
                cell.signalStrengthWcdma.signalStrength = p.readInt32();
                cell.signalStrengthWcdma.bitErrorRate = p.readInt32();
                cell.signalStrengthWcdma.rscp = p.readInt32();
                cell.signalStrengthWcdma.ecno = p.readInt32();
                records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                break;
            }
            case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::tdscdma: {
                RLOGD("don't support error");
                break;
            }
            case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::cdma: {
                VENDOR_SPACE::CellInfoCdma cell;
                cell.cellIdentityCdma.networkId = p.readInt32();
                cell.cellIdentityCdma.systemId = p.readInt32();
                cell.cellIdentityCdma.baseStationId = p.readInt32();
                cell.cellIdentityCdma.longitude = p.readInt32();
                cell.cellIdentityCdma.latitude = p.readInt32();
                alphAlong = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityCdma.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                alphaShort = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityCdma.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                cell.signalStrengthCdma.dbm = p.readInt32();
                cell.signalStrengthCdma.ecio = p.readInt32();
                cell.signalStrengthEvdo.dbm = p.readInt32();
                cell.signalStrengthEvdo.ecio = p.readInt32();
                cell.signalStrengthEvdo.signalNoiseRatio = p.readInt32();
                records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                break;
            }
            case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::lte: {
                VENDOR_SPACE::CellInfoLte cell;
                mcc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityLte.mcc = convertCharPtrToStdString(mcc);
                mnc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityLte.mnc = convertCharPtrToStdString(mnc);
                cell.cellIdentityLte.ci = p.readInt32();
                cell.cellIdentityLte.pci = p.readInt32();
                cell.cellIdentityLte.tac = p.readInt32();
                cell.cellIdentityLte.earfcn = p.readInt32();
                alphAlong = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityLte.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                alphaShort = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityLte.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                cell.cellIdentityLte.bandwidth = p.readInt32();
                int size = p.readInt32();
                if (size == 1) {
                    cell.cellIdentityLte.bands.push_back((VENDOR_SPACE::EutranBands)p.readInt32());
                } else {
                    cell.cellIdentityLte.bands = {};
                }
                cell.cellIdentityLte.additionalPlmns = {};
                cell.cellIdentityLte.csgInfo = {};
                cell.signalStrengthLte.signalStrength = p.readInt32();
                cell.signalStrengthLte.rsrp = p.readInt32();
                cell.signalStrengthLte.rsrq = p.readInt32();
                cell.signalStrengthLte.rssnr = p.readInt32();
                cell.signalStrengthLte.cqi = p.readInt32();
                cell.signalStrengthLte.timingAdvance = p.readInt32();
                cell.signalStrengthLte.cqiTableIndex = p.readInt32();
                records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                break;
            }
            case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::nr: {
                VENDOR_SPACE::CellInfoNr cell;
                mcc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityNr.mcc = convertCharPtrToStdString(mcc);
                mnc = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityNr.mnc = convertCharPtrToStdString(mnc);
                cell.cellIdentityNr.nci = p.readUint64();
                cell.cellIdentityNr.pci = p.readUint32();
                cell.cellIdentityNr.tac = p.readInt32();
                cell.cellIdentityNr.nrarfcn = p.readInt32();
                alphAlong = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityNr.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                alphaShort = RpDataUtils::strdupReadString(&p);
                cell.cellIdentityNr.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                int size = p.readInt32();
                if (size == 1) {
                    cell.cellIdentityNr.bands.push_back((VENDOR_SPACE::NgranBands)p.readInt32());
                } else {
                    cell.cellIdentityNr.bands = {};
                }
                cell.cellIdentityNr.additionalPlmns = {};
                cell.signalStrengthNr.ssRsrp = p.readInt32();
                cell.signalStrengthNr.ssRsrq = p.readInt32();
                cell.signalStrengthNr.ssSinr = p.readInt32();
                cell.signalStrengthNr.csiRsrp = p.readInt32();
                cell.signalStrengthNr.csiRsrq = p.readInt32();
                cell.signalStrengthNr.csiSinr = p.readInt32();
                cell.signalStrengthNr.csiCqiTableIndex = p.readInt32();
                int32_t count = p.readInt32();
                cell.signalStrengthNr.csiCqiReport.resize(count);
                for (int i = 0; i < count; i++) {
                    cell.signalStrengthNr.csiCqiReport[i] = p.readInt32();
                }
                cell.signalStrengthNr.timingAdvance = p.readInt32();
                records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);

                break;
            }
        }
        if (mcc) {
            RpDataUtils::freeMemory(mcc);
        }
        if (mnc) {
            RpDataUtils::freeMemory(mnc);
        }
        if (alphAlong) {
            RpDataUtils::freeMemory(alphAlong);
        }
        if (alphaShort) {
            RpDataUtils::freeMemory(alphaShort);
        }
        RLOGD("getCellInfoListResponse index=%d, info=%s", i, records[i].toString().c_str());
    }
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
    if (iter != teleService[slotId]->mapTokenResponses.end()) {
        std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
        ::ndk::ScopedAStatus rc = teleResponse->getCellInfoListResponse(records);
        if (!rc.isOk())
            RLOGD("%s,%d call getCellInfoListResponse failed.\n", __func__, __LINE__);
        teleService[slotId]->mapTokenResponses.erase(token);
    } else {
        RLOGD("getCellInfoListResponse:No TeleReponse client match the token %d!\n", token);
    }
    return;
}

void rilimpl::getRegistrationStateRespose(int request, int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("getRegistrationState request failed. e=%d", e);
        return;
    }

    Parcel p;
    char* mcc = NULL;
    char* mnc = NULL;
    char* alphAlong = NULL;
    char* alphaShort = NULL;
    p.write(response, responselen);
    p.setDataPosition(0);
    RegStateResult reg;
    reg.regState = static_cast<RegState>(p.readInt32());
    reg.rat = static_cast<RadioTechnology>(p.readInt32());
    reg.reasonForDenial = static_cast<RegistrationFailCause>(p.readInt32());
    char* registeredPlmn = RpDataUtils::strdupReadString(&p);
    reg.registeredPlmn = convertCharPtrToStdString(registeredPlmn);
    int32_t tag = p.readInt32();
    switch (tag) {
        case (int32_t)VENDOR_SPACE::CellIdentity::Tag::gsm: {
            VENDOR_SPACE::CellIdentityGsm cell;
            mcc = RpDataUtils::strdupReadString(&p);
            cell.mcc = convertCharPtrToStdString(mcc);
            mnc = RpDataUtils::strdupReadString(&p);
            cell.mnc = convertCharPtrToStdString(mnc);
            cell.lac = p.readInt32();
            cell.cid = p.readInt32();
            cell.arfcn = p.readInt32();
            cell.bsic = (uint8_t)(p.readInt32());
            alphAlong = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
            alphaShort = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
            cell.additionalPlmns = {};
            reg.cellIdentity = VENDOR_SPACE::CellIdentity(cell);
            break;
        }
        case (int32_t)VENDOR_SPACE::CellIdentity::Tag::wcdma: {
            VENDOR_SPACE::CellIdentityWcdma cell;
            mcc = RpDataUtils::strdupReadString(&p);
            cell.mcc = convertCharPtrToStdString(mcc);
            mnc = RpDataUtils::strdupReadString(&p);
            cell.mnc = convertCharPtrToStdString(mnc);
            cell.lac = p.readInt32();
            cell.cid = p.readInt32();
            cell.psc = p.readInt32();
            cell.uarfcn = p.readInt32();
            alphAlong = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
            alphaShort = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
            cell.additionalPlmns = {};
            reg.cellIdentity = VENDOR_SPACE::CellIdentity(cell);
            break;
        }
        case (int32_t)VENDOR_SPACE::CellIdentity::Tag::tdscdma: {
            RLOGD("don't support error");
            break;
        }
        case (int32_t)VENDOR_SPACE::CellIdentity::Tag::cdma: {
            VENDOR_SPACE::CellIdentityCdma cell;
            cell.networkId = p.readInt32();
            cell.systemId = p.readInt32();
            cell.baseStationId = p.readInt32();
            cell.longitude = p.readInt32();
            cell.latitude = p.readInt32();
            alphAlong = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
            alphaShort = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
            reg.cellIdentity = VENDOR_SPACE::CellIdentity(cell);
            break;
        }
        case (int32_t)VENDOR_SPACE::CellIdentity::Tag::lte: {
            VENDOR_SPACE::CellIdentityLte cell;
            mcc = RpDataUtils::strdupReadString(&p);
            cell.mcc = convertCharPtrToStdString(mcc);
            mnc = RpDataUtils::strdupReadString(&p);
            cell.mnc = convertCharPtrToStdString(mnc);
            cell.ci = p.readInt32();
            cell.pci = p.readInt32();
            cell.tac = p.readInt32();
            cell.earfcn = p.readInt32();
            alphAlong = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
            alphaShort = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
            cell.bandwidth = p.readInt32();
            int size = p.readInt32();
            if (size == 1) {
            cell.bands.push_back((VENDOR_SPACE::EutranBands)p.readInt32());
            } else {
            cell.bands = {};
            }
            cell.additionalPlmns = {};
            cell.csgInfo = {};
            reg.cellIdentity = VENDOR_SPACE::CellIdentity(cell);
            break;
        }
        case (int32_t)VENDOR_SPACE::CellIdentity::Tag::nr: {
            VENDOR_SPACE::CellIdentityNr cell;
            mcc = RpDataUtils::strdupReadString(&p);
            cell.mcc = convertCharPtrToStdString(mcc);
            mnc = RpDataUtils::strdupReadString(&p);
            cell.mnc = convertCharPtrToStdString(mnc);
            cell.nci = p.readUint64();
            cell.pci = p.readUint32();
            cell.tac = p.readInt32();
            cell.nrarfcn = p.readInt32();
            alphAlong = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
            alphaShort = RpDataUtils::strdupReadString(&p);
            cell.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
            int size = p.readInt32();
            if (size == 1) {
            cell.bands.push_back((VENDOR_SPACE::NgranBands)p.readInt32());
            } else {
            cell.bands = {};
            }
            cell.additionalPlmns = {};
            reg.cellIdentity = VENDOR_SPACE::CellIdentity(cell);
            break;
        }
    }
    int32_t aTag = p.readInt32();
    switch (aTag) {
        case (int32_t)VENDOR_SPACE::AccessTechnologySpecificInfo::Tag::cdmaInfo: {
            VENDOR_SPACE::Cdma2000RegistrationInfo cdmaRegInfo = {};
            cdmaRegInfo.cssSupported = p.readBool();
            cdmaRegInfo.roamingIndicator = p.readInt32();
            cdmaRegInfo.systemIsInPrl = p.readInt32();
            cdmaRegInfo.defaultRoamingIndicator = p.readInt32();
            reg.accessTechnologySpecificInfo = VENDOR_SPACE::AccessTechnologySpecificInfo(cdmaRegInfo);
            break;
        }
        case (int32_t)VENDOR_SPACE::AccessTechnologySpecificInfo::Tag::eutranInfo: {
            VENDOR_SPACE::EutranRegistrationInfo eutranInfo = {};
            eutranInfo.lteVopsInfo.isVopsSupported = p.readBool();
            eutranInfo.lteVopsInfo.isEmcBearerSupported = p.readBool();
            eutranInfo.nrIndicators.isEndcAvailable = p.readBool();
            eutranInfo.nrIndicators.isDcNrRestricted = p.readBool();
            eutranInfo.nrIndicators.isNrAvailable = p.readBool();
            eutranInfo.lteAttachResultType = (VENDOR_SPACE::EutranRegistrationInfo::AttachResultType)p.readByte();
            eutranInfo.extraInfo = p.readInt32();
            reg.accessTechnologySpecificInfo = VENDOR_SPACE::AccessTechnologySpecificInfo(eutranInfo);
            break;
        }
        case (int32_t)VENDOR_SPACE::AccessTechnologySpecificInfo::Tag::ngranNrVopsInfo: {
            VENDOR_SPACE::NrVopsInfo ngranNrVopsInfo = {};
            ngranNrVopsInfo.vopsSupported = p.readByte();
            ngranNrVopsInfo.emcSupported = p.readByte();
            ngranNrVopsInfo.emfSupported = p.readByte();
            reg.accessTechnologySpecificInfo = VENDOR_SPACE::AccessTechnologySpecificInfo(ngranNrVopsInfo);
            break;
        }
    }
    if (mcc) {
        RpDataUtils::freeMemory(mcc);
    }
    if (mnc) {
        RpDataUtils::freeMemory(mnc);
    }
    if (alphAlong) {
        RpDataUtils::freeMemory(alphAlong);
    }
    if (alphaShort) {
        RpDataUtils::freeMemory(alphaShort);
    }
    ndk::ScopedAStatus rc = ndk::ScopedAStatus::fromStatus(STATUS_OK);
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
    if (iter != teleService[slotId]->mapTokenResponses.end()) {
        std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
        ::ndk::ScopedAStatus rc = teleResponse->sendSmsResponse((int32_t)e);
        if (request == RIL_REQUEST_VOICE_REGISTRATION_STATE) {
            rc = teleResponse->getVoiceRegistrationStateResponse(reg);
            if (!rc.isOk())
                RLOGD("%s,%d call getVoiceRegistrationStateResponse failed.\n", __func__, __LINE__);
        } else if (request == RIL_REQUEST_DATA_REGISTRATION_STATE) {
            rc = teleResponse->getDataRegistrationStateResponse(reg);
            if (!rc.isOk())
                RLOGD("%s,%d call getDataRegistrationStateResponse failed.\n", __func__, __LINE__);
        } else {
            RLOGE("error request: %d", request);
            // return;
        }
        teleService[slotId]->mapTokenResponses.erase(token);
    } else {
        RLOGD("getRegistrationStateRespose:No TeleReponse client match the token %d!\n", token);
    }

    if (registeredPlmn != NULL) {
        RpDataUtils::freeMemory(registeredPlmn);
    }
    return;
}

void rilimpl::requestResposneNoDataForAsync(int request, int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    switch (request) {
        case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL: {
            std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
            if (iter != teleService[slotId]->mapTokenResponses.end()) {
            std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
            ::ndk::ScopedAStatus rc = teleResponse->setNetworkSelectionModeManualResponse((int32_t)e);
            if (!rc.isOk())
                RLOGE("%s,%d call RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL failed.\n", __func__, __LINE__);
            teleService[slotId]->mapTokenResponses.erase(token);
            } else {
                RLOGE("RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL:No TeleReponse client match the token %d!\n", token);
            }
            break;
        }
        case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC: {
            std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
            if (iter != teleService[slotId]->mapTokenResponses.end()) {
            std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
            ::ndk::ScopedAStatus rc = teleResponse->setNetworkSelectionModeAutomaticResponse((int32_t)e);
            if (!rc.isOk())
                RLOGD("%s,%d call RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC failed.\n", __func__, __LINE__);
            teleService[slotId]->mapTokenResponses.erase(token);
            } else {
            RLOGE("RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:No TeleReponse client match the token %d!\n", token);
            }
            break;
        }
    }
    return;
}

void rilimpl::sendSmsResponse(int slotId, int token, RIL_Errno e, void* response, size_t responselen) {
  int32_t support = property_get_int32("persist.vendor.sms_codec_support", 0);
  RLOGD("sendSmsResponse: SMS codec supported = %d", support);
  if (!support) {
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("sendSmsResponse request failed. e=%d", e);
        return;
    }

    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    SendSmsResult sms;
    sms.messageRef = p.readInt32();
    char* ackPdu = RpDataUtils::strdupReadString(&p);
    sms.ackPDU = convertCharPtrToStdString(ackPdu);
    sms.errorCode = p.readInt32();
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
    if (iter != teleService[slotId]->mapTokenResponses.end()) {
        std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
        ::ndk::ScopedAStatus rc = teleResponse->sendSmsNoEncoderResponse(sms);
        if (!rc.isOk()) {
        RLOGE("sendSmsNoEncoderResponse: slotId %d, binder fail", slotId);
        }
        teleService[slotId]->mapTokenResponses.erase(token);
    } else {
        RLOGD("RIL_REQUEST_SEND_SMS:No(Encoder) TeleReponse client match the token %d!\n", token);
    }
    if (ackPdu != NULL) {
        RpDataUtils::freeMemory(ackPdu);
    }
  } else {
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
    if (iter != teleService[slotId]->mapTokenResponses.end()) {
      std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
      ::ndk::ScopedAStatus rc = teleResponse->sendSmsResponse((int32_t)e);
      if (!rc.isOk())
        RLOGD("%s,%d call sendSmsResponse failed.\n", __func__, __LINE__);
      teleService[slotId]->mapTokenResponses.erase(token);
    } else {
      RLOGD("RIL_REQUEST_SEND_SMS:No TeleReponse client match the token %d!\n", token);
    }
  }
  return;
}

void rilimpl::getAvailableNetworksWithActResponse(int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("getAvailableNetworksWithActResponse request failed. e=%d", e);
        //TeleService[slotId]->mTeleResponses[slotId]->getAvailableNetworksWithActResponse
        return;
    }

    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    std::vector<OperatorInfoWithAct> networks;
    int size = p.readInt32();
    networks.resize(size);
    for (int i = 0; i < size; i++) {
        char* lac = RpDataUtils::strdupReadString(&p);
        networks[i].lac = convertCharPtrToStdString(lac);
        char* act = RpDataUtils::strdupReadString(&p);
        networks[i].act = convertCharPtrToStdString(act);
        char* alphaLong = RpDataUtils::strdupReadString(&p);
        networks[i].base.alphaLong = convertCharPtrToStdString(alphaLong);
        char* alphaShort = RpDataUtils::strdupReadString(&p);
        networks[i].base.alphaShort = convertCharPtrToStdString(alphaShort);
        char* operatorNumeric = RpDataUtils::strdupReadString(&p);
        networks[i].base.operatorNumeric = convertCharPtrToStdString(operatorNumeric);
        int32_t status = p.readInt32();
        networks[i].base.status = status;
        if (lac) {
      RpDataUtils::freeMemory(lac);
        }
        if (act) {
      RpDataUtils::freeMemory(act);
        }
        if (alphaLong) {
      RpDataUtils::freeMemory(alphaLong);
        }
        if (alphaShort) {
      RpDataUtils::freeMemory(alphaShort);
        }
        if (operatorNumeric) {
      RpDataUtils::freeMemory(operatorNumeric);
        }
    }
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
    if (iter != teleService[slotId]->mapTokenResponses.end()) {
        std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
        ::ndk::ScopedAStatus rc = teleResponse->getAvailableNetworksWithActResponse(networks);
        if (!rc.isOk())
      RLOGE("%s,%d call getAvailableNetworksWithActResponse failed.\n", __func__, __LINE__);
        teleService[slotId]->mapTokenResponses.erase(token);
    } else {
        RLOGE("getAvailableNetworksWithActResponse:No TeleReponse client match the token %d!\n", token);
    }
}

std::string rilimpl::convertCharPtrToStdString(const char* str) {
    return str == NULL ? std::string("") : std::string(str);
}

void rilimpl::nitzTimeReceived(int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if (teleService[slotId]->mTeleIndications.empty()) {
        RLOGE("%s,%d mTeleIndications is empty", __func__, __LINE__);
        return;
    }

    if (response == NULL || responselen == 0) {
        RLOGE("%s,%d response or responselen is null", __func__, __LINE__);
        return;
    }

    Parcel* p = new Parcel();
    p->write(response, responselen);
    p->setDataPosition(0);

    long receivedTimeMs;
    long ageMs;
    char* time = RpDataUtils::strdupReadString(p);
    std::string nitzTime = convertCharPtrToStdString(time);
    RpDataUtils::freeMemory(time);
    p->readInt64(&receivedTimeMs);
    p->readInt64(&ageMs);

    delete p;
    instance->mTeleServiceClientMapMutex.lock();
    for (auto &it : teleService[slotId]->mTeleIndications) {
        RLOGD("mTeleIndications(%d)->nitzTimeReceived", it.first);
        if (it.second) {
            ::ndk::ScopedAStatus rc = (it.second)->nitzTimeReceived(nitzTime, receivedTimeMs, ageMs);
            if (!rc.isOk()) {
                RLOGE("%s,%d call nitzTimeReceived failed.", __func__, __LINE__);
            }
        }
    }
    instance->mTeleServiceClientMapMutex.unlock();
    return;
}

void rilimpl::radioStateChanged(int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if (teleService[slotId]->mTeleIndications.empty()) {
        RLOGE("%s,%d mTeleIndications is empty", __func__, __LINE__);
        return;
    }

    if (response == NULL || responselen == 0) {
        RLOGE("%s,%d response or responselen is null", __func__, __LINE__);
        return;
    }

    Parcel* p = new Parcel();
    p->write(response, responselen);
    p->setDataPosition(0);

    int value = 0;
    p->readInt32(&value);

    delete p;
    instance->mTeleServiceClientMapMutex.lock();
    for (auto &it : teleService[slotId]->mTeleIndications) {
        RLOGD("mTeleIndications(%d)->radioStateChanged", it.first);
        if (it.second) {
            ::ndk::ScopedAStatus rc = (it.second)->radioStateChanged(static_cast<RadioState>(value));
            if (!rc.isOk()) {
                RLOGE("%s,%d call radioStateChanged failed.", __func__, __LINE__);
            }
        }
    }
    instance->mTeleServiceClientMapMutex.unlock();
    return;
}

void rilimpl::responsePsNetworkStateChangeInd(int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (teleService[slotId]->mTeleIndications.empty()) {
        RLOGE("%s,%d mTeleIndications is empty", __func__, __LINE__);
        return;
    }

    if (response == NULL || responselen == 0) {
        RLOGE("%s,%d response or responselen is null", __func__, __LINE__);
        return;
    }

    Parcel* p = new Parcel();
    p->write(response, responselen);
    p->setDataPosition(0);

    int32_t num;
    p->readInt32(&num);
    std::vector<int32_t> ps_data(num);
    for (int i = 0; i < num; ++i) {
        p->readInt32(&ps_data[i]);
    }

    delete p;
    instance->mTeleServiceClientMapMutex.lock();
    for (auto &it : teleService[slotId]->mTeleIndications) {
        RLOGD("mTeleIndications(%d)->responsePsNetworkStateChangeInd", it.first);
        if (it.second) {
            ::ndk::ScopedAStatus rc = (it.second)->responsePsNetworkStateChangeInd(ps_data);
            if (!rc.isOk()) {
                RLOGE("%s,%d call responsePsNetworkStateChangeInd failed.", __func__, __LINE__);
            }
        }
    }
    instance->mTeleServiceClientMapMutex.unlock();
    return;
}

void rilimpl::responseCsNetworkStateChangeInd(int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (teleService[slotId]->mTeleIndications.empty()) {
        RLOGE("%s,%d mTeleIndications is empty", __func__, __LINE__);
        return;
    }

    if (response == NULL || responselen == 0) {
        RLOGE("%s,%d response or responselen is null", __func__, __LINE__);
        return;
    }

    Parcel* p = new Parcel();
    p->write(response, responselen);
    p->setDataPosition(0);

    int num;
    p->readInt32(&num);
    std::vector<std::string> data(num);
    for (int i = 0; i < num; ++i) {
        char* val = RpDataUtils::strdupReadString(p);
        if (val == nullptr) {
            RLOGE("responseCsNetworkStateChangeInd: failed to read string");
            delete p;
            return;
        }
        data[i] = convertCharPtrToStdString(val);
        RpDataUtils::freeMemory(val);
    }
    delete p;

    instance->mTeleServiceClientMapMutex.lock();
    for (auto& it : teleService[slotId]->mTeleIndications) {
        RLOGD("mTeleIndications(%d)->responseCsNetworkStateChangeInd", it.first);
        if ((it.second)) {
            ::ndk::ScopedAStatus rc = it.second->responseCsNetworkStateChangeInd(data);
            if (!rc.isOk()) {
                RLOGE("%s,%d call responseCsNetworkStateChangeInd failed.", __func__, __LINE__);
            }
        }
    }
    instance->mTeleServiceClientMapMutex.unlock();
    return;
}

void rilimpl::responseCardStateChangedInd(int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (teleService[slotId]->mTeleIndications.empty()) {
        RLOGE("%s,%d mTeleIndications is empty", __func__, __LINE__);
        return;
    }

    RLOGD("responseCardStateChangedInd(%d) IN",slotId);
    pthread_t card_tid;
    pthread_t mccmnc_tid;

    int* slotIdPtr1 = new int(slotId);
    int* slotIdPtr2 = new int(slotId);
    pthread_create(&card_tid, NULL, handlGetCardStatus, slotIdPtr1);
    pthread_create(&mccmnc_tid, NULL, handlGetMccMnc, slotIdPtr2);

    instance->mTeleServiceClientMapMutex.lock();

    for (auto& it : teleService[slotId]->mTeleIndications) {
        RLOGD("mTeleIndications(%d)->responseCardStateChangedInd", it.first);
        if ((it.second)) {
            RLOGD("responseCardStateChangedInd");
            ::ndk::ScopedAStatus rc = it.second->responseCardStateChangedInd();
            if (!rc.isOk()) {
                RLOGE("%s,%d call responseCsNetworkStateChangeInd failed.", __func__, __LINE__);
            }
        }
    }
    instance->mTeleServiceClientMapMutex.unlock();
    return;
}

void rilimpl::signalStrengthChangedInd(int slotId, const void *response, size_t responselen) {
    VENDOR_SPACE::MTK_SignalStrength msg;
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if((teleService[slotId]->mTeleIndications).size() > 0) {
        if((response == NULL) || (responselen == 0)) {
            return;
        }

        if (responselen >= sizeof (RIL_SignalStrength_v5)) {
            memset(&msg, 0 ,sizeof(VENDOR_SPACE::MTK_SignalStrength));
            int32_t lte_cqiTableIndex = 0;
            Parcel* p = new Parcel();
            p->write(response, responselen);
            p->setDataPosition(0);
            p->readInt32(&msg.GW_SignalStrength.signalStrength);
            p->readInt32(&msg.GW_SignalStrength.bitErrorRate);
            p->readInt32(&msg.GW_SignalStrength.timingAdvance);
            p->readInt32(&msg.CDMA_SignalStrength.dbm);
            p->readInt32(&msg.CDMA_SignalStrength.ecio);
            p->readInt32(&msg.EVDO_SignalStrength.dbm);
            p->readInt32(&msg.EVDO_SignalStrength.ecio);
            p->readInt32(&msg.EVDO_SignalStrength.signalNoiseRatio);
            p->readInt32(&msg.LTE_SignalStrength.signalStrength);
            p->readInt32(&msg.LTE_SignalStrength.rsrp);
            p->readInt32(&msg.LTE_SignalStrength.rsrq);
            p->readInt32(&msg.LTE_SignalStrength.rssnr);
            p->readInt32(&msg.LTE_SignalStrength.cqi);
            p->readInt32(&msg.LTE_SignalStrength.timingAdvance);
            p->readInt32(&lte_cqiTableIndex);
            p->readInt32(&msg.TD_SCDMA_SignalStrength.signalStrength);
            p->readInt32(&msg.TD_SCDMA_SignalStrength.bitErrorRate);
            p->readInt32(&msg.TD_SCDMA_SignalStrength.rscp);
            p->readInt32(&msg.WCDMA_SignalStrength.signalStrength);
            p->readInt32(&msg.WCDMA_SignalStrength.bitErrorRate);
            p->readInt32(&msg.WCDMA_SignalStrength.rscp);
            p->readInt32(&msg.WCDMA_SignalStrength.ecno);
            p->readInt32(&msg.NR_SignalStrength.ssRsrp);
            p->readInt32(&msg.NR_SignalStrength.ssRsrq);
            p->readInt32(&msg.NR_SignalStrength.ssSinr);
            p->readInt32(&msg.NR_SignalStrength.csiRsrp);
            p->readInt32(&msg.NR_SignalStrength.csiRsrq);
            p->readInt32(&msg.NR_SignalStrength.csiSinr);

            delete p;

            instance->mTeleServiceClientMapMutex.lock();
            for(auto &it : teleService[slotId]->mTeleIndications) {
                RLOGD("mTeleIndications(%d)->signalStrengthChangedInd(calllist)", it.first);
                if(it.second) {
                    ::ndk::ScopedAStatus rc = (it.second)->notifySignalStrength(msg);
                    if (!rc.isOk())
                        RLOGE("%s,%d call notifySignalStrength failed.", __func__, __LINE__);
                }
            }
            instance->mTeleServiceClientMapMutex.unlock();
        }
    }
    return;
}

void rilimpl::cellInfoListInd(int slotId, const void *response, size_t responselen){
    if ((response == NULL) || (responselen == 0)) {
        RLOGD("cellInfoListInd urc failed");
        return;
    }
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if ((teleService[slotId]->mTeleIndications).size() > 0) {
        std::vector<VENDOR_SPACE::CellInfo> records;
        char* mcc = NULL;
        char* mnc = NULL;
        char* alphAlong = NULL;
        char* alphaShort = NULL;
        Parcel p;
        p.write(response, responselen);
        p.setDataPosition(0);
        int32_t size = p.readInt32();
        records.resize(size);
        RLOGD("cellInfoListInd size=%d", size);
        for (int i = 0; i < size; i++) {
            records[i].registered = p.readBool();
            records[i].connectionStatus = static_cast<VENDOR_SPACE::CellConnectionStatus>(p.readInt32());
            int32_t tag = p.readInt32();
            switch (tag) {
                case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::gsm: {
                    VENDOR_SPACE::CellInfoGsm cell;
                    mcc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityGsm.mcc = convertCharPtrToStdString(mcc);
                    mnc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityGsm.mnc = convertCharPtrToStdString(mnc);
                    cell.cellIdentityGsm.lac = p.readInt32();
                    cell.cellIdentityGsm.cid = p.readInt32();
                    cell.cellIdentityGsm.arfcn = p.readInt32();
                    cell.cellIdentityGsm.bsic = (uint8_t)(p.readInt32());
                    alphAlong = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityGsm.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                    alphaShort = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityGsm.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                    cell.cellIdentityGsm.additionalPlmns = {};
                    cell.signalStrengthGsm.signalStrength = p.readInt32();
                    cell.signalStrengthGsm.bitErrorRate = p.readInt32();
                    cell.signalStrengthGsm.timingAdvance = p.readInt32();
                    records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                    break;
                }
                case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::wcdma: {
                    VENDOR_SPACE::CellInfoWcdma cell;
                    mcc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityWcdma.mcc = convertCharPtrToStdString(mcc);
                    mnc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityWcdma.mnc = convertCharPtrToStdString(mnc);
                    cell.cellIdentityWcdma.lac = p.readInt32();
                    cell.cellIdentityWcdma.cid = p.readInt32();
                    cell.cellIdentityWcdma.psc = p.readInt32();
                    cell.cellIdentityWcdma.uarfcn = p.readInt32();
                    alphAlong = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityWcdma.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                    alphaShort = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityWcdma.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                    cell.cellIdentityWcdma.additionalPlmns = {};
                    cell.cellIdentityWcdma.csgInfo = {};
                    cell.signalStrengthWcdma.signalStrength = p.readInt32();
                    cell.signalStrengthWcdma.bitErrorRate = p.readInt32();
                    cell.signalStrengthWcdma.rscp = p.readInt32();
                    cell.signalStrengthWcdma.ecno = p.readInt32();
                    records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                    break;
                }
                case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::tdscdma: {
                    RLOGD("don't support error");
                    break;
                }
                case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::cdma: {
                    VENDOR_SPACE::CellInfoCdma cell;
                    cell.cellIdentityCdma.networkId = p.readInt32();
                    cell.cellIdentityCdma.systemId = p.readInt32();
                    cell.cellIdentityCdma.baseStationId = p.readInt32();
                    cell.cellIdentityCdma.longitude = p.readInt32();
                    cell.cellIdentityCdma.latitude = p.readInt32();
                    alphAlong = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityCdma.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                    alphaShort = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityCdma.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                    cell.signalStrengthCdma.dbm = p.readInt32();
                    cell.signalStrengthCdma.ecio = p.readInt32();
                    cell.signalStrengthEvdo.dbm = p.readInt32();
                    cell.signalStrengthEvdo.ecio = p.readInt32();
                    cell.signalStrengthEvdo.signalNoiseRatio = p.readInt32();
                    records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                    break;
                }
                case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::lte: {
                    VENDOR_SPACE::CellInfoLte cell;
                    mcc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityLte.mcc = convertCharPtrToStdString(mcc);
                    mnc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityLte.mnc = convertCharPtrToStdString(mnc);
                    cell.cellIdentityLte.ci = p.readInt32();
                    cell.cellIdentityLte.pci = p.readInt32();
                    cell.cellIdentityLte.tac = p.readInt32();
                    cell.cellIdentityLte.earfcn = p.readInt32();
                    alphAlong = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityLte.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                    alphaShort = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityLte.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                    cell.cellIdentityLte.bandwidth = p.readInt32();
                    int size = p.readInt32();
                    if (size == 1) {
                        cell.cellIdentityLte.bands.push_back((VENDOR_SPACE::EutranBands)p.readInt32());
                    } else {
                        cell.cellIdentityLte.bands = {};
                    }
                    cell.cellIdentityLte.additionalPlmns = {};
                    cell.cellIdentityLte.csgInfo = {};
                    cell.signalStrengthLte.signalStrength = p.readInt32();
                    cell.signalStrengthLte.rsrp = p.readInt32();
                    cell.signalStrengthLte.rsrq = p.readInt32();
                    cell.signalStrengthLte.rssnr = p.readInt32();
                    cell.signalStrengthLte.cqi = p.readInt32();
                    cell.signalStrengthLte.timingAdvance = p.readInt32();
                    cell.signalStrengthLte.cqiTableIndex = p.readInt32();
                    records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);
                    break;
                }
                case (int32_t)VENDOR_SPACE::CellInfoRatSpecificInfo::Tag::nr: {
                    VENDOR_SPACE::CellInfoNr cell;
                    mcc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityNr.mcc = convertCharPtrToStdString(mcc);
                    mnc = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityNr.mnc = convertCharPtrToStdString(mnc);
                    cell.cellIdentityNr.nci = p.readUint64();
                    cell.cellIdentityNr.pci = p.readUint32();
                    cell.cellIdentityNr.tac = p.readInt32();
                    cell.cellIdentityNr.nrarfcn = p.readInt32();
                    alphAlong = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityNr.operatorNames.alphaLong = convertCharPtrToStdString(alphAlong);
                    alphaShort = RpDataUtils::strdupReadString(&p);
                    cell.cellIdentityNr.operatorNames.alphaShort = convertCharPtrToStdString(alphaShort);
                    int size = p.readInt32();
                    if (size == 1) {
                        cell.cellIdentityNr.bands.push_back((VENDOR_SPACE::NgranBands)p.readInt32());
                    } else {
                        cell.cellIdentityNr.bands = {};
                    }
                    cell.cellIdentityNr.additionalPlmns = {};
                    cell.signalStrengthNr.ssRsrp = p.readInt32();
                    cell.signalStrengthNr.ssRsrq = p.readInt32();
                    cell.signalStrengthNr.ssSinr = p.readInt32();
                    cell.signalStrengthNr.csiRsrp = p.readInt32();
                    cell.signalStrengthNr.csiRsrq = p.readInt32();
                    cell.signalStrengthNr.csiSinr = p.readInt32();
                    cell.signalStrengthNr.csiCqiTableIndex = p.readInt32();
                    int32_t count = p.readInt32();
                    cell.signalStrengthNr.csiCqiReport.resize(count);
                    for (int i = 0; i < count; i++) {
                        cell.signalStrengthNr.csiCqiReport[i] = p.readInt32();
                    }
                    cell.signalStrengthNr.timingAdvance = p.readInt32();
                    records[i].ratSpecificInfo = VENDOR_SPACE::CellInfoRatSpecificInfo(cell);

                    break;
                }
            }
            if (mcc) {
                RpDataUtils::freeMemory(mcc);
            }
            if (mnc) {
                RpDataUtils::freeMemory(mnc);
            }
            if (alphAlong) {
                RpDataUtils::freeMemory(alphAlong);
            }
            if (alphaShort) {
                RpDataUtils::freeMemory(alphaShort);
            }
            RLOGD("cellInfoListInd index=%d, info=%s", i, records[i].toString().c_str());
        }

        instance->mTeleServiceClientMapMutex.lock();
        for (auto& it : teleService[slotId]->mTeleIndications) {
            RLOGD("mTeleIndications(%d)->cellInfoListInd(calllist)", it.first);
            if (it.second) {
                ::ndk::ScopedAStatus rc = (it.second)->cellInfoListInd(records);
                if (!rc.isOk())
                    ALOGD("%s,%d call cellInfoListInd failed.", __func__, __LINE__);
            }
        }
        instance->mTeleServiceClientMapMutex.unlock();
    }
    return;
}

void rilimpl::ecallIndicationInd(int slotId, const void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    VENDOR_SPACE::EcallMsg msg;
    int num;
    if((teleService[slotId]->mTeleIndications).size() > 0) {
        if((response == NULL) || (responselen == 0)) {
            return;
        }

        Parcel* p = new Parcel();
        p->write(response, responselen);
        p->setDataPosition(0);
        p->readInt32(&num);
        p->readInt32(&msg.indType);
        p->readInt32(&msg.call_id);
        delete p;

        instance->mTeleServiceClientMapMutex.lock();
        for(auto &it : teleService[slotId]->mTeleIndications) {
            RLOGD("mTeleIndications(%d)->ecallMsgInd(ecallMsg)", it.first);
            if(it.second) {
                ::ndk::ScopedAStatus rc = (it.second)->ecallMsgInd(msg);
                if (!rc.isOk())
                    ALOGD("%s,%d call ecallMsgInd failed.", __func__, __LINE__);
            }
        }
        instance->mTeleServiceClientMapMutex.unlock();
    }
    return;
}

void rilimpl::responseDataNoHandle(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("onRequestComplete request failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    (instance->parcelResponse).write(response, responselen);
    (instance->parcelResponse).setDataPosition(0);
    instance->wakeup();
}

std::string rilimpl::convertCharPtrToAidlString(const char *ptr) {
    return std::string(ptr);
}

void rilimpl::responseGetApn(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("onRequestComplete request failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    (instance->parcelResponse).write(response, responselen);
    (instance->parcelResponse).setDataPosition(0);
    instance->wakeup();
}

void* rilimpl::callStatechanged(void* arg, int slot_id) {
    RpAidlAdapter* instanceCall = RpAidlAdapter::getInstance();
    Parcel p2;
    int token2 = instanceCall->generateToken();
    RLOGD("when callstagechanged on urc, get last call fail cause, token(%d)", token2);
    instanceCall->addPendingRequest(token2, RIL_REQUEST_LAST_CALL_FAIL_CAUSE);
    status_t status = p2.writeInt32(RIL_REQUEST_LAST_CALL_FAIL_CAUSE);
    if (status != OK) {
        RLOGD("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    status = p2.writeInt32(token2);
    if (status != OK) {
        RLOGD("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    instanceCall->sendRequest(p2,slot_id);

    Parcel p1;
    int token1 = instanceCall->generateToken();
    RLOGD("when callstagechanged on urc, get current call, token(%d)", token1);
    instanceCall->addPendingRequest(token1, RIL_REQUEST_GET_CURRENT_CALLS);
    status = p1.writeInt32(RIL_REQUEST_GET_CURRENT_CALLS);
    if (status != OK) {
        RLOGD("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    status = p1.writeInt32(token1);
    if (status != OK) {
        RLOGD("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    instanceCall->sendRequest(p1,slot_id);

    return NULL;
}

void rilimpl::translateCallState(RIL_CallState inCallState, int* stateEnum) {
    switch (inCallState) {
        case RIL_CALL_ACTIVE: {
            *stateEnum = 3; //E_CA_VCALL_STATE_ACTIVE
            break;
        }
        case RIL_CALL_HOLDING: {
            *stateEnum = 4; //E_CA_VCALL_STATE_HOLDING
            break;
        }
        case RIL_CALL_DIALING: {
            *stateEnum = 1; //E_CA_VCALL_STATE_DIALING
            break;
        }
        case RIL_CALL_ALERTING: {
            *stateEnum = 2; //E_CA_VCALL_STATE_ALERTING
            break;
        }
        case RIL_CALL_INCOMING: {
            *stateEnum = 0; //E_CA_VCALL_STATE_INCOMING
            break;
        }
        case RIL_CALL_WAITING: {
            *stateEnum = 6; //E_CA_VCALL_STATE_WAITING
            break;
        }
        default: {
            *stateEnum = 5; //E_CA_VCALL_STATE_END
        }
    }
}

void rilimpl::handleCallMsgInd(int slotId) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if((callMsg.CallId != old_callMsg.CallId) ||
       (callMsg.phone_number != old_callMsg.phone_number) ||
       (callMsg.state != old_callMsg.state) ||
       (callMsg.call_end_reason != old_callMsg.call_end_reason) ||
       (callMsg.isMT!= old_callMsg.isMT)) {
        instance->mTeleServiceClientMapMutex.lock();
        for(auto &it : teleService[slotId]->mTeleIndications) {
            RLOGD("mTeleIndications(%d)->vcallMsgInd(calllist)", it.first);
            if(it.second) {
                ::ndk::ScopedAStatus rc = (it.second)->vcallMsgInd(callMsg);
                if (!rc.isOk())
                    ALOGD("%s,%d call vcallMsgInd failed.\n", __func__, __LINE__);
            }
        }
        instance->mTeleServiceClientMapMutex.unlock();
        old_callMsg.CallId = callMsg.CallId;
        old_callMsg.phone_number = callMsg.phone_number;
        old_callMsg.state = callMsg.state;
        old_callMsg.call_end_reason = callMsg.call_end_reason;
        old_callMsg.isMT = callMsg.isMT;
    }
}

void rilimpl::freeCalllist(RIL_Call **pCallLists, int callCount) {
    if (pCallLists != NULL) {
        for (int i = 0; i < callCount; i++) {
            if (pCallLists[i] != NULL) {
                free(pCallLists[i]->number);
                free(pCallLists[i]->name);
                free(pCallLists[i]);
            }
        }
        free(pCallLists);
    }
}
void rilimpl::Autoanswer(int slotId) {
    char tempstr[128];
    memset(tempstr, 0, sizeof(tempstr));
    property_get("ro.telephony.call.autocall", tempstr, "-1");
    ALOGI("callMsg.State = %d ro.telephony.call.autocall =  %s slot_id = %d",callMsg.state, tempstr, slotId);
    if(strstr(tempstr, "1") != NULL) {
        RpAidlAdapter* instanceCall = RpAidlAdapter::getInstance();
        Parcel p;
        int token = instanceCall->generateToken();
        instanceCall->addPendingRequest(token, RIL_REQUEST_ANSWER);
        status_t status = p.writeInt32(RIL_REQUEST_ANSWER);
        if (status != OK) {
            RLOGD("ML_VcallautoAnswer writeInt32 failed");
        }
        status = p.writeInt32(token);
        if (status != OK) {
            RLOGD("ML_VcallautoAnswer writeInt32 failed");
        }

        int32_t ret = -1;
        {
            Mutex::Autolock autoLock(gRequestLock);
            instanceCall->sendRequest(p,slotId);
            nsecs_t mRequestATTimeout = 1e9;
            instanceCall->setTimeOut(mRequestATTimeout);
            status_t res  = OK;
            res = instanceCall->wait();
            if (res != OK) {
                if (res == TIMED_OUT) {
                    RLOGW("answerVcall wait Response timeout\n");
                    instanceCall->removePendingRequest(token);
                }
            }else {
                ret = instanceCall->responseErr;
            }
        }

        RLOGD("answerVcall response %d\n", ret);
    } else {
        RLOGD("answerVcall cancel and ro.telephony.call.autocall is %s\n", tempstr);
    }
}

void* rilimpl::handlGetCardStatus(void* arg) {
    RLOGD("%s", __FUNCTION__);
    int slot_id = *((int*)arg);
    delete static_cast<int*>(arg);
    status_t res  = OK;
    int ret = -1;
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    Parcel p;
    Parcel *pResponse;
    int token = instance->generateToken();
    instance->addPendingRequest(token, RIL_REQUEST_GET_SIM_STATUS);
    status_t status = p.writeInt32(RIL_REQUEST_GET_SIM_STATUS);
    if (status != OK) {
        RLOGD("handlGetCardStatus writeInt32 failed");
        return nullptr;
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGD("handlGetCardStatus writeInt32 failed");
        return nullptr;
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        instance->sendRequest(p,slot_id);
        res = instance->wait();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                instance->removePendingRequest(token);
            }
            status = 2;
        }else {
            ret = instance->responseErr;
            if(ret == OK) {
                instance->parcelResponse.setDataPosition(0);
                pResponse = &instance->parcelResponse;
                pResponse->readInt32(&status);
            }
        }
    }
    std::string mbrain_string = "";
    if(status == 1)
    {
        mbrain_string = "simcard status is present";
    } else if(status == 0) {
        mbrain_string = "simcard status is absent";
    } else{
        mbrain_string = "simcard status is unknown";
    }
        notifyMbrainCardStatusChanged(slot_id, mbrain_string);
        RLOGI("%s: status %d response %d.",__FUNCTION__, status, ret);
    return NULL;
}

void* rilimpl::handlGetMccMnc(void* arg) {
    int slot_id = *((int*)arg);
    RLOGD("%s", __FUNCTION__);
    delete static_cast<int*>(arg);
    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("vendor.gsm.ril.uicc.mccmnc", value, "");
    RLOGD("%s %s\n",__FUNCTION__, value);
    std::string mbrain_string(value);
    notifyMbrainMccMncStatusChanged(slot_id, mbrain_string);
    return NULL;
}

void* rilimpl::handlGetRfStatus(void* arg) {
    RLOGD("%s", __FUNCTION__);
    Parcel p;
    Parcel *pResponse;
    char * rsp = NULL;
    int32_t ret = -1;
    status_t res  = OK;
    int size = 0;
    int slot_id = *((int*)arg);

    std::string cmdStringParam = "AT+EGMC=0,\"query_rf_cal_status\"";
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    int token = instance->generateToken();
    RLOGD("%s token %d",__FUNCTION__, token);

    instance->addPendingRequest(token, RIL_REQUEST_OEM_HOOK_RAW);
    status_t status = p.writeInt32(RIL_REQUEST_OEM_HOOK_RAW);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    status = p.writeInt32(token);
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    status = p.writeInt32((int32_t)cmdStringParam.size());
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    status = p.write((const void*)(cmdStringParam.c_str()),cmdStringParam.size());
    if (status != OK) {
        RLOGE("%s writeInt32 failed", __FUNCTION__);
        return nullptr;
    }
    {
        Mutex::Autolock autoLock(gRequestLock);
        instance->sendRequest(p,slot_id);
        nsecs_t mRequestATTimeout = 5e9;
        instance->setTimeOut(mRequestATTimeout);
        res = instance->wait();
        instance->resetTimeout();
        if (res != OK) {
            if (res == TIMED_OUT) {
                RLOGW("%s wait Response timeout\n", __FUNCTION__);
                instance->removePendingRequest(token);
            }
            rsp = "";
        }else {
            ret = instance->responseErr;
            if(ret == OK) {
                pResponse = &instance->parcelResponse;
                instance->parcelResponse.setDataPosition(0);
                pResponse->readInt32(&size);
                rsp = (char *) calloc(1, size + 1);
                if(rsp != NULL) {
                    pResponse->read(rsp, size);
                    rsp[size] = '\0';
                }else {
                    rsp = "";
                    RLOGW("%s calloc fail\n", __FUNCTION__);
                }
            } else {
                rsp = "";
            }
        }
    }

    RLOGI("%s: size: %d, %s",__FUNCTION__, size, rsp);

    std::string rfResult;
    if (NULL != strstr(rsp, "yes")) {
        rfResult = "Rf status is Yes";
    } else if (NULL != strstr(rsp, "no")) {
        rfResult = "Rf status is No";
    } else {
        rfResult = "Unknow";
    }
    notifyMbrainRfStatusStatusChanged(slot_id, rfResult);
    if((ret == OK) && (!(rfResult.empty()))) {
        RpDataUtils::freeMemory(rsp);
        rsp = nullptr;
    }
    RLOGI("%s response %s\n", __FUNCTION__, rfResult.c_str());
    RLOGI("%s response %d\n", __FUNCTION__, ret);
    return NULL;
}

void rilimpl::responseCurrentCalls(int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("responseCurrentCalls request failed. e=%d", e);
        return;
    }
    int stateEnum;
    int callCount = 0;
    int uusPresent = 0;
    RIL_Call **pCallLists = NULL;
    RIL_Call *pCallList = NULL;
    int32_t res = 0;
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);

    if (p.dataAvail() > 0) {
        p.readInt32(&callCount);
        pCallLists = (RIL_Call **) calloc(1, sizeof(RIL_Call *) * callCount);
        if (pCallLists == NULL) {
            RLOGE("responseCurrentCalls pCallLists calloc failed.");
            return;
        }

        RLOGD("responseCurrentCalls callnum: %d" ,callCount);
        for (int i = 0; i < callCount; i++) {
            pCallList = (RIL_Call *) calloc(1, sizeof(RIL_Call));
            if (pCallList == NULL) {
                RLOGE("responseCurrentCalls pCallList (%d) calloc failed.", i);
                if (pCallLists != NULL) {
                    for (int j = 0; j < i; j++) {
                        if (pCallLists[j] != NULL) {
                            free(pCallLists[j]->number);
                            free(pCallLists[j]->name);
                            free(pCallLists[j]);
                        }
                    }
                    free(pCallLists);
                }
                return;
            }
            pCallLists[i] = pCallList;
            p.readInt32(&res);
            pCallLists[i]->state = (RIL_CallState) res;
            p.readInt32(&pCallLists[i]->index);
            p.readInt32(&pCallLists[i]->toa);
            p.readInt32(&res);
            pCallLists[i]->isMpty = (uint8_t) res;
            p.readInt32(&res);
            pCallLists[i]->isMT = (uint8_t) res;
            p.readInt32(&res);
            pCallLists[i]->als = (uint8_t) res;
            p.readInt32(&res);
            pCallLists[i]->isVoice = (uint8_t) res;
            p.readInt32(&res);
            pCallLists[i]->isVoicePrivacy = (uint8_t) res;
            pCallLists[i]->number = RpDataUtils::strdupReadString(&p);
            p.readInt32(&pCallLists[i]->numberPresentation);
            pCallLists[i]->name = RpDataUtils::strdupReadString(&p);
            p.readInt32(&pCallLists[i]->namePresentation);
            p.readInt32(&uusPresent);
        }
    }

    if (callCount > 0 && pCallLists != NULL && pCallLists[0] != NULL) {
        callMsg.CallId = pCallLists[0]->index;
        callMsg.phone_number = pCallLists[0]->number;
        RLOGD("responseCurrentCalls state: %d" ,pCallLists[0]->state);

        translateCallState(pCallLists[0]->state, &stateEnum);

        teleService[slotId]->setCallStatus(stateEnum);
        callMsg.state = VENDOR_SPACE::VcallStateType(stateEnum);
        callMsg.call_end_reason = -1;
        teleService[slotId]->currentCallMsg.call_id = pCallLists[0]->index;
        teleService[slotId]->currentCallMsg.state = VENDOR_SPACE::VcallStateType(stateEnum);
        teleService[slotId]->currentCallMsg.isMT = pCallLists[0]->isMT;
        callMsg.isMT = pCallLists[0]->isMT;
        teleService[slotId]->currentCallMsg.number = pCallLists[0]->number;
    } else {
        callMsg.CallId = -1;
        callMsg.state = VENDOR_SPACE::VcallStateType::E_CA_VCALL_STATE_END;
        callMsg.phone_number = convertCharPtrToAidlString("");
        teleService[slotId]->setCallStatus(5);
        teleService[slotId]->currentCallMsg.call_id = -1;
        teleService[slotId]->currentCallMsg.state = VENDOR_SPACE::VcallStateType::E_CA_VCALL_STATE_END;
        teleService[slotId]->currentCallMsg.isMT = -1;
        teleService[slotId]->currentCallMsg.number = convertCharPtrToAidlString("");
    }

    RLOGD("vcallMsgInd callMsg.phone_number %s, old_callMsg.phone_number:%s", callMsg.phone_number.c_str(),old_callMsg.phone_number.c_str());
    RLOGD("vcallMsgInd callMsg.state %d, old_callMsg.state=%d", callMsg.state, old_callMsg.state);
    RLOGD("vcallMsgInd callMsg.call_end_reason %d, old_callMsg.call_end_reason=%d", callMsg.call_end_reason,old_callMsg.call_end_reason);
    handleCallMsgInd(slotId);
    freeCalllist(pCallLists, callCount);

}
void rilimpl::responseLastCallFailCause(int token, RIL_Errno e, void *response, size_t responselen) {
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("responseLastCallFailCause request failed. e=%d", e);
        return;
    }
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int failCause = p.readInt32();
    callMsg.call_end_reason = failCause;
    RLOGD("responseLastCallFailCause %d", failCause);


//    for(auto &it : teleService->mTeleIndications) {
//        RLOGD("mTeleIndications(%d)->vcallMsgInd(calllist)", it.first);
//        int rc = (it.second)->vcallMsgInd(callMsg);
//        if (!rc.isOk())
//            ALOGD("%s() call vcallMsgInd !isOk %s", __func__, rc.description().c_str());
//    }
}

void rilimpl::responseNoData(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (e != RIL_E_SUCCESS) {
        RLOGD("onRequestComplete request failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    instance->responseErr = 0;
    instance->wakeup();
}

void rilimpl::responseRetryTimes(int token, RIL_Errno e, void *response, size_t responselen) {
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int retryTimes = p.readInt32();

    RLOGD("retryTimes =%d  ", retryTimes);
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    instance->responseErr = 0;

    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseRetryTimes failed. e=%d retryTimes=%d ", e, retryTimes);
        instance->responseErr = (int32_t)e;
    }
    (instance->parcelResponse).freeData();
    instance->parcelResponse.setDataPosition(0);
    instance->parcelResponse.writeInt32(retryTimes);
    instance->wakeup();
}

void rilimpl::responseSetSimIo(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseSetSimIo failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int count = p.readInt32();
    int sw1 = p.readInt32();
    int sw2 = p.readInt32();
    char* responseStr = RpDataUtils::strdupReadString(&p);
    RLOGD("responseSetSimIo sw1=%d  sw2 =%d, responseStr = %s ", sw1,sw2,responseStr);

    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    instance->parcelResponse.writeInt32(sw1);
    instance->parcelResponse.writeInt32(sw2);
    RpDataUtils::writeStringToParcel(&instance->parcelResponse,responseStr);
    if(responseStr) {
        RpDataUtils::freeMemory(responseStr);
    }
    (instance->parcelResponse).setDataPosition(0);
    instance->wakeup();
}


void rilimpl::responseGetImei(int token, RIL_Errno e, void *response, size_t responselen) {

    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseGetImei failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int count = p.readInt32();
    char* imei = RpDataUtils::strdupReadString(&p);
    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    RpDataUtils::writeStringToParcel(&instance->parcelResponse,imei);
    if(imei) {
        RpDataUtils::freeMemory(imei);
    }
    (instance->parcelResponse).setDataPosition(0);
    instance->wakeup();
}

void rilimpl::responseIccOpenLogicalChannel(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseIccOpenLogicalChannel failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int count = p.readInt32();
    char* mChannelId = RpDataUtils::strdupReadString(&p);
    if (mChannelId == nullptr) {
    instance->responseErr = -1;
    return;
    }
    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    RpDataUtils::writeStringToParcel(&instance->parcelResponse,mChannelId);
    if(mChannelId) {
        RpDataUtils::freeMemory(mChannelId);
    }
    (instance->parcelResponse).setDataPosition(0);
    instance->wakeup();
}

static void freeMemory(char* str) {
    if (str != NULL) {
        free(str);
    }
}

void rilimpl::responseCallWaiting(int token, RIL_Errno e, void *response, size_t responselen) {
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    bool enable = p.readBool();
    RLOGD("enable =%d  ", enable);
    int serviceClass = p.readInt32();
    RLOGD("serviceClass =%d  ", serviceClass);
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    instance->responseErr = 0;

    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseCallWaiting failed. e=%d enable=%d  serviceClass=%d ", e, enable, serviceClass);
        instance->responseErr = (int32_t)e;
    }
    (instance->parcelResponse).freeData();
    instance->parcelResponse.setDataPosition(0);
    instance->parcelResponse.writeBool(enable);
    instance->parcelResponse.writeInt32(serviceClass);
    instance->wakeup();
}

void rilimpl::responseDataCall(int slotId, int token, RIL_Errno e, void *response, size_t responselen, int actType) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    int ril_err = RIL_E_SUCCESS;
    int profile_id = 0;
    std::map<int, int>::iterator iter;

    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("persist.android.dataOnOff", value, "-1");
    int dataOnoff = atoi(value);

    if(slotId < 0) {
        return;
    }

    if (instance->isProfileIdPending(token, &profile_id) == false) {
        RLOGD("responseDataCall no pending profileId found!\n");
        return;
    }
    instance->removePendingProfileId(token);

    if (e != RIL_E_SUCCESS || response == NULL || responselen == 0) {
        RLOGD("responseDataCall request failed. e=%d", e);
        ril_err = -1;
    }

    VENDOR_SPACE::SetupDataCallResult result = {};
    std::string apntype;

    if(dataService[slotId]->mDataIndication){
        if (e != RIL_E_SUCCESS || response == NULL || responselen == 0) {
            RLOGD("responseDataCall failed. e=%d", e);
            result.apnType = convertCharPtrToAidlString(RpDataUtils::profileId2Type(profile_id).c_str());
            result.profileId = profile_id;
            result.pdnState = PDN_FAILED;
            result.type = std::string();
            result.ifname = std::string();
            result.addresses = std::string();
            result.dnses = std::string();
            result.gateways = std::string();
            result.pcscf = std::string();
            if (e != RIL_E_ABORTED) {
                RLOGD("responsedatacall notifyMbrainDataResponse: "
                    "slotId=%d, dataOnoff=%d, profile_id=%d, ifname=%s, addresses=%s, pdnState=%d, e=%d",
                    slotId, dataOnoff, profile_id, result.ifname.c_str(), result.addresses.c_str(), result.pdnState, e);
                notifyMbrainDataResponse(slotId, dataOnoff, actType, profile_id,
                    result.ifname, result.addresses, result.pdnState, -1, -1, e);
            }
            goto out;
        }
        MTK_Data_Call_Response_v1 rsp;
        parseDataCallResponse(&rsp, response, responselen);
        apntype = std::string(rsp.apnType);
        result.profileId = RpDataUtils::type2profileId(apntype);
        result.pdnState = rsp.pdnState;
        result.failcause = rsp.status;
        result.apnType = convertCharPtrToAidlString(rsp.apnType);
        result.type = convertCharPtrToAidlString(rsp.type);
        result.ifname = convertCharPtrToAidlString(rsp.ifname);
        result.addresses = convertCharPtrToAidlString(rsp.addresses);
        result.dnses = convertCharPtrToAidlString(rsp.dnses);
        result.gateways = convertCharPtrToAidlString(rsp.gateways);
        result.pcscf = convertCharPtrToAidlString(rsp.pcscf);
        result.mtu = rsp.mtu;

        // When pdn state is ML_DATA_CALL_DISCONNECTED(4) or ML_DATA_CALL_FAILED(6), notify mbrain.
        if (rsp.pdnState == 4 || rsp.pdnState == 6) {
            RLOGD("responsedatacall notifyMbrainDataResponse: "
                "slotId=%d, dataOnoff=%d, profile_id=%d, ifname=%s, addresses=%s, pdnState=%d, cid=%d, failcause=%d, e=%d",
                slotId, dataOnoff, profile_id, result.ifname.c_str(), result.addresses.c_str(),
                result.pdnState, rsp.cId, result.failcause, e);
            notifyMbrainDataResponse(slotId, dataOnoff, actType, profile_id,
                result.ifname, result.addresses, result.pdnState, rsp.cId, result.failcause, e);
        }

        freeMemory(rsp.apnType);
        freeMemory(rsp.type);
        freeMemory(rsp.ifname);
        freeMemory(rsp.addresses);
        freeMemory(rsp.dnses);
        freeMemory(rsp.gateways);
        freeMemory(rsp.pcscf);
out:
        RLOGD("responseDataCall profile_id =%d", result.profileId);
        ::ndk::ScopedAStatus rc;

        if(dataService[slotId]->mDataResponse) {
            rc = dataService[slotId]->mDataResponse->setupDataCallResponse(ril_err,result);
            if (!rc.isOk())
                ALOGD("%s,%d call setupDataCallResponse failed.\n", __func__, __LINE__);
        }
    }
}

#ifdef NET_SOCKET_SUPPORT
static int profileIdToMtu(int profileId) {
    char value[PROPERTY_VALUE_MAX] = { 0 };
    if (profileId == RpDataUtils::MOBILE_APN) {
        property_get("vendor.ril.default.profile.mtu", value, "0");
        ALOGD("%s, prop value=%s, profileId=%d\n", __func__, value, profileId);
        return atoi(value);
    } else if (profileId == RpDataUtils::RCS_APN) {
        ALOGD("%s, prop value=%s, profileId=%d\n", __func__, value, profileId);
        property_get("vendor.ril.rcs.profile.mtu", value, "0");
        return atoi(value);
    } else if (profileId == RpDataUtils::BIP_APN) {
        ALOGD("%s, prop value=%s, profileId=%d\n", __func__, value, profileId);
        property_get("vendor.ril.bip.profile.mtu", value, "0");
        return atoi(value);
    } else {
        ALOGD("%s, prop value=%s, profileId=%d, no matched, return 0\n", __func__, value, profileId);
        return 0;
    }
}

static int getPreferMtu(int mtu_in, int profileId_in) {
    int mtuFromApnConfig = profileIdToMtu(profileId_in);

    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("vendor.gsm.ril.uicc.mccmnc", value, "");
    int mtuFromMccmnc = RpDataUtils::getMtuByMccmnc(value);
    if (mtu_in <= 0) {
        if (mtuFromApnConfig > 0) {
            return mtuFromApnConfig;
        } else if (mtuFromMccmnc > 0) {
            return mtuFromMccmnc;
        } else {
            return 1500;
        }
    } else {
        return mtu_in;
    }
}
#endif

void rilimpl::pdnStateChangedInd(int slotId, int token, void *response, size_t responselen) {
    VENDOR_SPACE::SetupDataCallResult result = {};
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if((slotId < 0) || (response == NULL) || (responselen == 0)) {
        return;
    }

    char value[PROPERTY_VALUE_MAX] = { 0 };
    property_get("persist.android.dataOnOff", value, "-1");
    int dataOnoff = atoi(value);

    int mIndex = 0;
    MTK_Data_Call_Response_v1 rsp;
    parseDataCallResponse(&rsp, response, responselen);
    std::string apntype = std::string(rsp.apnType);
    result.profileId = RpDataUtils::type2profileId(apntype);
    if (result.profileId < 1) {
        ALOGD("%s,%d profile id not existed, then skip.\n", __func__, __LINE__);
        goto out;
    }
    result.pdnState = rsp.pdnState;
    result.failcause = rsp.status;
    result.apnType = convertCharPtrToAidlString(rsp.apnType);
    result.type = convertCharPtrToAidlString(rsp.type);
    result.ifname = convertCharPtrToAidlString(rsp.ifname);
    result.addresses = convertCharPtrToAidlString(rsp.addresses);
    result.dnses = convertCharPtrToAidlString(rsp.dnses);
    result.gateways = convertCharPtrToAidlString(rsp.gateways);
    result.pcscf = convertCharPtrToAidlString(rsp.pcscf);
    result.mtu = rsp.mtu;
    #ifdef NET_SOCKET_SUPPORT
    struct netSocketData dcRes;
    memset(&dcRes,0,sizeof(struct netSocketData));
    dcRes.profileId = result.profileId;
    dcRes.pdnState = result.pdnState;
    dcRes.failcause =  result.failcause;
    dcRes.online = result.online;
    dcRes.mtu = getPreferMtu(rsp.mtu, result.profileId);
    dcRes.mtuV6 = getPreferMtu(rsp.mtuV6, result.profileId);

    RLOGD("pdnStateChangedInd, output mtu: %d, mtuV6: %d", dcRes.mtu, dcRes.mtuV6);

    memcpy(dcRes.apnType, rsp.apnType, strlen(rsp.apnType));
    memcpy(dcRes.type, rsp.type, strlen(rsp.type));
    memcpy(dcRes.ifname, rsp.ifname, strlen(rsp.ifname));
    memcpy(dcRes.addresses, rsp.addresses, strlen(rsp.addresses));
    memcpy(dcRes.dnses, rsp.dnses, strlen(rsp.dnses));
    memcpy(dcRes.gateways, rsp.gateways, strlen(rsp.gateways));
    // memcpy(dcRes.pcscf, rsp.pcscf, strlen(rsp.pcscf));
    writeToNetsocket(&dcRes,sizeof(struct netSocketData));
    #endif

    // When pdn state is ML_DATA_CALL_CONNECTED(2) ML_DATA_CALL_DISCONNECTED(4) or ML_DATA_CALL_FAILED(6), notify mbrain.
    if (result.pdnState == 2 || result.pdnState == 4 || result.pdnState == 6) {
        RLOGD("pdnStateChangedInd: notifyMbrainPdnStateChanged: "
            "slotId=%d, dataOnoff=%d, profile_id=%d, ifname=%s, addresses=%s, pdnState=%d, cid=%d",
            slotId, dataOnoff, result.profileId, result.ifname.c_str(), result.addresses.c_str(), result.pdnState, rsp.cId);
        notifyMbrainPdnStateChanged(slotId, dataOnoff, result.profileId,
            result.ifname, result.addresses, result.pdnState, rsp.cId);
    }

    mIndex = result.profileId - 1;
    parseDataCallResponse(&(mDataCallRep[mIndex]), response, responselen);
    RLOGD("save value in RpAidlAdapter->pdnStateChangedInd, mIndex=%d, mDataCallRep.pdnState=%d,"
        " dataCallRep.status=%d, dataCallRep.apnType=%s, dataCallRep.type=%s, dataCallRep.ifname=%s,"
        " dataCallRep.addresses=%s, dataCallRep.dnses=%s, dataCallRep.gateways=%s, dataCallRep.pcscf=%s,"
        " dataCallRep.mtu=%d, dataCallRep.mtuV6=%d\n", mIndex, mDataCallRep[mIndex].pdnState, mDataCallRep[mIndex].status,
        mDataCallRep[mIndex].apnType, mDataCallRep[mIndex].type, mDataCallRep[mIndex].ifname,
        mDataCallRep[mIndex].addresses, mDataCallRep[mIndex].dnses, mDataCallRep[mIndex].gateways,
        mDataCallRep[mIndex].pcscf, mDataCallRep[mIndex].mtu, mDataCallRep[mIndex].mtuV6);

    teleService[slotId]->setLastDataFailReason(apntype, rsp.status);
    instance->mTeleDataClientMapMutex.lock();
    if(dataService[slotId]->mapDataClientIndications.size() > 0) {
        for(auto &it : dataService[slotId]->mapDataClientIndications) {
            if(it.second) {
                ::ndk::ScopedAStatus rc = (it.second)->dataStatusChanged(result);
                if (!rc.isOk())
                    ALOGD("%s,%d call dataStatusChanged failed.", __func__, __LINE__);
            }
        }
    }
    instance->mTeleDataClientMapMutex.unlock();

out:
    if(rsp.apnType) {
        RpDataUtils::freeMemory(rsp.apnType);
    }
    if(rsp.type) {
        RpDataUtils::freeMemory(rsp.type);
    }
    if(rsp.ifname) {
        RpDataUtils::freeMemory(rsp.ifname);
    }
    if(rsp.addresses) {
        RpDataUtils::freeMemory(rsp.addresses);
    }
    if(rsp.dnses) {
        RpDataUtils::freeMemory(rsp.dnses);
    }
    if(rsp.gateways) {
        RpDataUtils::freeMemory(rsp.gateways);
    }
    if(rsp.pcscf) {
        RpDataUtils::freeMemory(rsp.pcscf);
    }
}

void rilimpl::responseSendSms(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    RLOGD("responseSendSms ");

    instance->responseErr = 0;
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseVoiceRadioTech failed. e=%d\n", e);
        instance->responseErr = -1;
    } else {
        RLOGD("[responseSendSms] responseVoiceRadioTech succeed\n");
    }
    return;
}

void rilimpl::newSmsInd(int slotId, int token, void *response, size_t responselen) {
    VENDOR_SPACE::SmsInfo msg;
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if((teleService[slotId]->mTeleIndications).size() > 0) {
        if((response == NULL) || (responselen == 0)) {
            return;
        }
        Parcel p;
        p.write(response, responselen);
        p.setDataPosition(0);
        int32_t support = property_get_int32("persist.vendor.sms_codec_support", 0);
        if (!support) {
            RLOGD("newSmsInd: SMS codec don't supported");
            std::vector<uint8_t> pdu;
            p.readByteVector(&pdu);
            instance->mTeleServiceClientMapMutex.lock();
            for(auto &it : teleService[slotId]->mTeleIndications) {
                RLOGD("mTeleIndications(%d)->newSmsInd(calllist)", it.first);
                if(it.second) {
                    ::ndk::ScopedAStatus rc = (it.second)->newSms(pdu);
                    if (!rc.isOk())
                        RLOGD("%s,%d call SmsInfoReceived failed.\n", __func__, __LINE__);
                }
            }
            instance->mTeleServiceClientMapMutex.unlock();
            return;
        }
        msg.format = (VENDOR_SPACE::SmsFormat)p.readInt32();
        char* read_phone_number = RpDataUtils::strdupReadString(&p);
        if (read_phone_number == nullptr) {
            return;
        }
        msg.phone_number = convertCharPtrToAidlString(read_phone_number);
        char* read_sms_data = RpDataUtils::strdupReadString(&p);
        if (read_sms_data == nullptr) {
            RpDataUtils::freeMemory(read_phone_number);
            return;
        }
        msg.sms_data = convertCharPtrToAidlString(read_sms_data);
        msg.sms_data_len = p.readInt32();
        RLOGD("newSmsInd msg.sms_data %s",msg.sms_data.c_str());
        RLOGD("newSmsInd msg.phone_number %s",msg.phone_number.c_str());
        RLOGD("newSmsInd msg.format %d",msg.format);

        instance->mTeleServiceClientMapMutex.lock();
        for(auto &it : teleService[slotId]->mTeleIndications) {
            RLOGD("mTeleIndications(%d)->newSmsInd(calllist)", it.first);
            if(it.second) {
                ::ndk::ScopedAStatus rc = (it.second)->SmsInfoReceived(msg);
                if (!rc.isOk())
                    RLOGD("%s,%d call SmsInfoReceived failed.\n", __func__, __LINE__);
            }
        }
        instance->mTeleServiceClientMapMutex.unlock();
        free(read_phone_number);
        free(read_sms_data);
    }
}

void rilimpl::responseImsRegState(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("responseImsRegState request failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }
    Parcel p;
    status_t status = p.write(response, responselen);
    if (status != OK) {
        RLOGE("writeInt32 failed");
        return;
    }
    p.setDataPosition(0);
    int n = p.readInt32();
    int ims_reg = p.readInt32();
    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    instance->parcelResponse.setDataPosition(0);
    status = instance->parcelResponse.writeInt32(n);
    if (status != OK) {
        RLOGE("writeInt32 failed");
        return;
    }
    status = instance->parcelResponse.writeInt32(ims_reg);
    if (status != OK) {
        RLOGE("writeInt32 failed");
        return;
    }
    RLOGD("responseImsRegState %d", ims_reg);
    instance->wakeup();
}

void rilimpl::responseGetOprtMode(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0)) {
        RLOGD("responseGetOprtMode request failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }
    Parcel p;
    p.write(response, responselen);
    p.setDataPosition(0);
    int n = p.readInt32();
    int oprt_mode = p.readInt32();
    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    instance->parcelResponse.setDataPosition(0);
    instance->parcelResponse.writeInt32(n);
    instance->parcelResponse.writeInt32(oprt_mode);
    RLOGD("responseGetOprtMode %d", oprt_mode);
    instance->wakeup();
}

void rilimpl::responseSignalStrength(int token, RIL_Errno e, void *response, size_t responselen) {
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    if(e != RIL_E_SUCCESS || (response == NULL) || (responselen == 0) || (responselen < sizeof (RIL_SignalStrength_v5))) {
        RLOGD("responseSignalStrength request failed. e=%d", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    instance->responseErr = 0;
    (instance->parcelResponse).freeData();
    instance->parcelResponse.setDataPosition(0);
    instance->parcelResponse.write(response, responselen);
    RLOGD("responseSignalStrength");
    instance->wakeup();

#if 0
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();
    int32_t sig_level;
    int32_t csq;
    if (e != RIL_E_SUCCESS || response == NULL) {
        RLOGD("responseSignalStrength failed. e=%d\n", e);
        instance->responseErr = -1;
        instance->wakeup();
        return;
    }

    if (responselen >= sizeof (RIL_SignalStrength_v5)) {
        RIL_SignalStrength_v14 *p_cur = (RIL_SignalStrength_v14*)calloc(1,
                sizeof(RIL_SignalStrength_v14));
        if(p_cur == NULL) {
            instance->responseErr = -1;
            instance->wakeup();
            return;
        }
        Parcel* p = new Parcel();
        p->write(response, responselen);
        p->setDataPosition(0);
        p->readInt32(&p_cur->GW_SignalStrength.signalStrength);
        p->readInt32(&p_cur->GW_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->GW_SignalStrength.timingAdvance);
        p->readInt32(&p_cur->CDMA_SignalStrength.dbm);
        p->readInt32(&p_cur->CDMA_SignalStrength.ecio);
        p->readInt32(&p_cur->EVDO_SignalStrength.dbm);
        p->readInt32(&p_cur->EVDO_SignalStrength.ecio);
        p->readInt32(&p_cur->EVDO_SignalStrength.signalNoiseRatio);
        p->readInt32(&p_cur->LTE_SignalStrength.signalStrength);
        p->readInt32(&p_cur->LTE_SignalStrength.rsrp);
        p->readInt32(&p_cur->LTE_SignalStrength.rsrq);
        p->readInt32(&p_cur->LTE_SignalStrength.rssnr);
        p->readInt32(&p_cur->LTE_SignalStrength.cqi);
        p->readInt32(&p_cur->LTE_SignalStrength.timingAdvance);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.signalStrength);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->TD_SCDMA_SignalStrength.rscp);
        p->readInt32(&p_cur->WCDMA_SignalStrength.signalStrength);
        p->readInt32(&p_cur->WCDMA_SignalStrength.bitErrorRate);
        p->readInt32(&p_cur->WCDMA_SignalStrength.rscp);
        p->readInt32(&p_cur->WCDMA_SignalStrength.ecno);
        p->readInt32(&p_cur->NR_SignalStrength.ssRsrp);
        p->readInt32(&p_cur->NR_SignalStrength.ssRsrq);
        p->readInt32(&p_cur->NR_SignalStrength.ssSinr);
        p->readInt32(&p_cur->NR_SignalStrength.csiRsrp);
        p->readInt32(&p_cur->NR_SignalStrength.csiRsrq);
        p->readInt32(&p_cur->NR_SignalStrength.csiSinr);

        std::shared_ptr<SignalStrength> mSignalStrength = new SignalStrength(p_cur->GW_SignalStrength.signalStrength,
            p_cur->GW_SignalStrength.bitErrorRate,
            p_cur->CDMA_SignalStrength.dbm,
            p_cur->CDMA_SignalStrength.ecio,
            p_cur->EVDO_SignalStrength.dbm,
            p_cur->EVDO_SignalStrength.ecio,
            p_cur->EVDO_SignalStrength.signalNoiseRatio,
            p_cur->LTE_SignalStrength.signalStrength,
            p_cur->LTE_SignalStrength.rsrp,
            p_cur->LTE_SignalStrength.rsrq,
            p_cur->LTE_SignalStrength.rssnr,
            p_cur->LTE_SignalStrength.cqi,
            p_cur->TD_SCDMA_SignalStrength.rscp,
            p_cur->WCDMA_SignalStrength.signalStrength,
            p_cur->WCDMA_SignalStrength.rscp,
            p_cur->NR_SignalStrength.csiRsrp,
            p_cur->NR_SignalStrength.csiRsrq,
            p_cur->NR_SignalStrength.csiSinr,
            p_cur->NR_SignalStrength.ssRsrp,
            p_cur->NR_SignalStrength.ssRsrq,
            p_cur->NR_SignalStrength.ssSinr);

        if (p_cur != NULL) {
            free(p_cur);
        }

        mSignalStrength->validateInput();
        //Todo. setLteRsrpThresholds and setLteRsrpBoost
        sig_level = mSignalStrength->getLevel();
        csq = mSignalStrength->getAsuLevel();
        instance->responseErr = 0;
        (instance->parcelResponse).freeData();
        instance->parcelResponse.setDataPosition(0);
        instance->parcelResponse.writeInt32(sig_level);
        instance->parcelResponse.writeInt32(csq);
        RLOGD("responseSignalStrength sig_level %d, %d\n", sig_level,csq);
        instance->wakeup();
        delete p;
    }
#endif
}

void rilimpl::keepaliveStatus(int slotId, const void *response, size_t responselen) {
    VENDOR_SPACE::KeepaliveStatus status;
    RpAidlAdapter* instance = RpAidlAdapter::getInstance();

    if(dataService[slotId]->mapDataClientIndications.size() > 0) {
        if((response == NULL) || (responselen == 0)) {
            RLOGW("keepaliveStatus, just response NULL, slotId %d", slotId);
            return;
        }
        Parcel p;
        p.write(response, responselen);
        p.setDataPosition(0);
        status.sessionHandle = p.readInt32();
        status.code = p.readInt32();
        RLOGD("keepaliveStatus(%d) sessionHandle %d, code %d", slotId, status.sessionHandle, status.code);

        instance->mTeleDataClientMapMutex.lock();
        for(auto &it : dataService[slotId]->mapDataClientIndications) {
            if(it.second) {
                ::ndk::ScopedAStatus rc = (it.second)->keepaliveStatus(status);
                if (!rc.isOk())
                    ALOGD("%s,%d call keepaliveStatus failed.", __func__, __LINE__);
            }
        }
        instance->mTeleDataClientMapMutex.unlock();
    }
}
RpAidlAdapter::RpAidlAdapter() : mSdkClient(NULL) {
    for (int i = 0; i < MAXCLIENT_NUM*2; i++) {
        mTeleClientAlive[i] = 0;
        mDataClientAlive[i] = 0;
    }
    responseErr = -1;
}

RpAidlAdapter::~RpAidlAdapter() {
}

int RpAidlAdapter::generateToken() {
    int next = 0;
    token_mutex.lock();
    next = tokenCounter++;
    if (next == __INT_MAX__) {
        tokenCounter = 0;
    }
    token_mutex.unlock();
    return next;
}

RpAidlAdapter *RpAidlAdapter::getInstance() {
    if (sInstance == NULL) {
        RLOGD("RpAidlAdapter getInstance %d %d\n",getpid(),gettid());
        sInstance = new RpAidlAdapter();
        sInstance->mSdkClient = NULL;
        sInstance->init();
    }
    //RLOGD("RpAidlAdapter getInstance\n");
    return sInstance;
}

void RpAidlAdapter::closeInstance() {
    if (sInstance != NULL) {
        sInstance->deInit();
        sInstance = NULL;
    }
}

int RpAidlAdapter::getFreeTeleClientIndex(int *clientIdx) {
    for (int i = 0 ; i < MAXCLIENT_NUM*2; i++) {
        if (mTeleClientAlive[i] == 0) {
            RLOGD("getFreeTeleClientIndex %d\n",INDEX_BASE + i);
            *clientIdx = INDEX_BASE + i;
            mTeleClientAlive[i] = 1;
            return 0;
        }
    }
    return -1;
}

int RpAidlAdapter::getFreeDataClientIndex(int *clientIdx) {
    for (int i = 0 ; i < MAXCLIENT_NUM*2; i++) {
        if (mDataClientAlive[i] == 0) {
            RLOGD("getFreeDataClientIndex %d\n",INDEX_BASE + i);
            *clientIdx = INDEX_BASE + i;
            mDataClientAlive[i] = 1;
            return 0;
        }
    }
    return -1;
}

void RpAidlAdapter::deathRecipientCallback(void* cookie) {
    if(cookie == nullptr) {
        RLOGW("deathRecipientCallback cookie == nullptr");
        return;
    }
    int64_t clientindex = (int64_t)(cookie);
    if((clientindex > (INDEX_BASE + MAXCLIENT_NUM*2 - 2)) || (clientindex < INDEX_BASE)) {
        RLOGE("invalid client idx %d!", clientindex);
        return;
    }
    RLOGE("The client %d has died!", clientindex);

    int sim_count = MAX_SLOT_NUM;
    int otherslotId = 0;

    mTeleServiceClientMapMutex.lock();
    for (int i = 0; i < sim_count; i++) {
        std::map<int, shared_ptr<VENDOR_SPACE::ITeleServiceIndication>>::iterator iter =
                teleService[i]->mTeleIndications.find(clientindex);
        if (iter != teleService[i]->mTeleIndications.end()) {
            teleService[i]->mTeleIndications.erase(clientindex);
            teleService[i]->mTeleResponses.erase(clientindex);
            otherslotId = ((i == 0) ? 1 : 0);
            mTeleClientAlive[clientindex - INDEX_BASE] = 0;
            break;
        }
    }
    if (sim_count == 2) {
        teleService[otherslotId]->mTeleIndications.erase(clientindex+1);
        teleService[otherslotId]->mTeleResponses.erase(clientindex+1);
        mTeleClientAlive[clientindex - INDEX_BASE + 1] = 0;
    }
    mTeleServiceClientMapMutex.unlock();

    mTeleDataClientMapMutex.lock();
    for (int i = 0; i < sim_count; i++) {
        std::map<int, shared_ptr<VENDOR_SPACE::ITeleDataIndication>>::iterator iter =
                dataService[i]->mapDataClientIndications.find(clientindex);
        if (iter != dataService[i]->mapDataClientIndications.end()) {
            dataService[i]->mapDataClientIndications.erase(clientindex);
            dataService[i]->mapDataClientResponses.erase(clientindex);
            otherslotId = ((i == 0) ? 1 : 0);
            mDataClientAlive[clientindex - INDEX_BASE] = 0;
            break;
        }
    }
    if (sim_count == 2) {
        dataService[otherslotId]->mapDataClientIndications.erase(clientindex+1);
        dataService[otherslotId]->mapDataClientResponses.erase(clientindex+1);
        mDataClientAlive[clientindex - INDEX_BASE + 1] = 0;
    }
    mTeleDataClientMapMutex.unlock();
}

void RpAidlAdapter::registerService() {
    //configureRpcThreadpool(1, true);
    //ABinderProcess_startThreadPool();
    property_set("ro.telephony.call.autocall", "0");
    int sim_count = MAX_SLOT_NUM;
    for (int i = 0; i < sim_count; i++)
    {
        teleService[i] = ndk::SharedRefBase::make<TeleService>();
        (teleService[i]->mTeleResponses).clear();
        (teleService[i]->mTeleIndications).clear();
        (teleService[i]->mapTokenResponses).clear();
        (teleService[i]->mapTokenResponsesV1_1).clear();
        teleService[i]->mSlotId = i;
        teleService[i]->mCallStatus = 5;
        teleService[i]->ecall_type = RIL_Ecall_Type::EN16454_ECALL;
        teleService[i]->currentCallMsg ={-1, VENDOR_SPACE::VcallStateType::E_CA_VCALL_STATE_END, -1, ""};
        teleService[i]->mAidlAdapter = this;
        binder_exception_t status = AServiceManager_addService((teleService[i])->asBinder().get(), teleserviceNames[i]);
        if (status != EX_NONE) {
            RLOGD("teleServive register Error!!!");
        }

        dataService[i] = ndk::SharedRefBase::make<TeleData>();
        dataService[i]->mSlotId = i;
        dataService[i]->mDataResponse = NULL;
        dataService[i]->mDataIndication = NULL;
        (dataService[i]->mapDataClientResponses).clear();
        (dataService[i]->mapDataClientIndications).clear();
        dataService[i]->mAidlAdapter = this;
        dataService[i]->mLastToken = -1;
        status = AServiceManager_addService((dataService[i])->asBinder().get(), dataserviceNames[i]);
        if (status != EX_NONE) {
            RLOGD("dataService register Error!!!");
        }
        RLOGD("dataService register done!!!");

        voiceService[i] = ndk::SharedRefBase::make<VoiceService>();
        voiceService[i]->mSlotId = i;
        voiceService[i]->mAidlAdapter = this;
        voiceService[i]->mTeleService = teleService[i];
        status = AServiceManager_addService((voiceService[i])->asBinder().get(), voiceserviceNames[i]);
        if (status != EX_NONE) {
            RLOGD("voiceService register Error!!!");
        }
        RLOGD("voiceService register done!!!");
    }
    pthread_t tid;
    pthread_create(&tid, NULL, rilimpl::handlGetRfStatus, &defaultAtSlot);
    startupWakeup();
}


void ril_aidl_thread_pool() {
    RLOGD("ril_aidl_thread_pool!!!");
    status_t res  = OK;
    RpAidlAdapter * instance = RpAidlAdapter::getInstance();
    res = instance->startupWait();
    if (res != OK) {
        if (res == TIMED_OUT) {
            RLOGW("ril_aidl_thread_pool wait registerService timeout!\n");
        }else {
            RLOGW("ril_aidl_thread_pool wait registerService fail!\n");
        }
    }
    {
        std::lock_guard<std::mutex> lock(RpAidlAdapter::mtx);
        RpAidlAdapter::rilAidlThreadPoolCompleted = true;
    }
    RpAidlAdapter::cv.notify_all();
    RLOGI("ril_aidl_thread_pool DONE!!!");
}

void RpAidlAdapter::init() {
    registerService();
}

void RpAidlAdapter::deInit() {

}

void RpAidlAdapter::wakeup() {
    m_RespMutex.lock();
    RpAidlAdapter::isLockRelease = true;
    m_RespCondition.broadcast();
    m_RespMutex.unlock();
}

status_t RpAidlAdapter::wait() {
    status_t res = OK;
    m_RespMutex.lock();
    if(!(RpAidlAdapter::isLockRelease)) {
        res = m_RespCondition.waitRelative(m_RespMutex,mRequestTimeout);
    } else {
        RLOGW("already wakeup(), no need wait");
    }
    m_RespMutex.unlock();
    return res;
}

void RpAidlAdapter::startupWakeup() {
    m_startupMutex.lock();
    RpAidlAdapter::isStartLockRelease = true;
    m_startupCondition.signal();
    m_startupMutex.unlock();
}

status_t RpAidlAdapter::startupWait() {
    status_t res = OK;
    m_startupMutex.lock();
    if(!(RpAidlAdapter::isStartLockRelease)) {
        res = m_startupCondition.waitRelative(m_startupMutex,mRequestTimeout);
    } else {
        RLOGW("%s already wakeup(), no need wait",__FUNCTION__);
    }
    m_startupMutex.unlock();
    return res;
}

void RpAidlAdapter::addPendingRequest(int token, int requestId) {
    m_reqMapMutex.lock();
    mPendingRequests[token] = requestId;
    m_reqMapMutex.unlock();
}

void RpAidlAdapter::removePendingRequest(int token) {
    m_reqMapMutex.lock();
    mPendingRequests.erase(token);
    m_reqMapMutex.unlock();
}

bool RpAidlAdapter::isRequestPending(int token, int *requestId) {
    bool ret = true;
    std::map<int, int>::iterator iter;

    m_reqMapMutex.lock();
    iter = mPendingRequests.find(token);
    if (iter == mPendingRequests.end()) {
        ret = false;
    } else {
        *requestId = iter->second;
    }
    m_reqMapMutex.unlock();
    return ret;
}

void RpAidlAdapter::addPendingProfileId(int token, int profileId) {
    m_reqMapMutex.lock();
    mPendingProfileId[token] = profileId;
    m_reqMapMutex.unlock();
}

void RpAidlAdapter::removePendingProfileId(int token) {
    m_reqMapMutex.lock();
    mPendingProfileId.erase(token);
    m_reqMapMutex.unlock();
}

bool RpAidlAdapter::isProfileIdPending(int token, int *profileId) {
    bool ret = true;
    std::map<int, int>::iterator iter;

    m_reqMapMutex.lock();
    iter = mPendingProfileId.find(token);
    if (iter == mPendingProfileId.end()) {
        ret = false;
    } else {
        *profileId = iter->second;
    }
    m_reqMapMutex.unlock();
    return ret;
}

int RpAidlAdapter::setTimeOut(nsecs_t time) {
    RLOGD("%s, %ld" PRId64, __FUNCTION__, time);
    mRequestTimeout = time;
    return 1;
}
void RpAidlAdapter::resetTimeout() {
    RLOGD("%s to default", __FUNCTION__);
    mRequestTimeout = 5e9;
}

int32_t RpAidlAdapter::getDataCallInfo(int profileId, MTK_Data_Call_Response_v1 *dataCallRep) {
    RLOGD("%s", __FUNCTION__);
    int index = profileId - 1;
    dataCallRep->pdnState = mDataCallRep[index].pdnState;
    dataCallRep->status = mDataCallRep[index].status;
    dataCallRep->mtu = mDataCallRep[index].mtu;

    dataCallRep->apnType = (char *)malloc(32);
    memcpy(dataCallRep->apnType, mDataCallRep[index].apnType, strlen(mDataCallRep[index].apnType));
    dataCallRep->apnType[strlen(mDataCallRep[index].apnType)] = '\0';

    dataCallRep->type = (char *)malloc(32);
    memcpy(dataCallRep->type, mDataCallRep[index].type, strlen(mDataCallRep[index].type));
    dataCallRep->type[strlen(mDataCallRep[index].type)] = '\0';

    dataCallRep->ifname = (char *)malloc(32);
    memcpy(dataCallRep->ifname, mDataCallRep[index].ifname, strlen(mDataCallRep[index].ifname));
    dataCallRep->ifname[strlen(mDataCallRep[index].ifname)] = '\0';

    dataCallRep->addresses = (char *)malloc(128);
    memcpy(dataCallRep->addresses, mDataCallRep[index].addresses, strlen(mDataCallRep[index].addresses));
    dataCallRep->addresses[strlen(mDataCallRep[index].addresses)] = '\0';

    dataCallRep->dnses = (char *)malloc(128);
    memcpy(dataCallRep->dnses, mDataCallRep[index].dnses, strlen(mDataCallRep[index].dnses));
    dataCallRep->dnses[strlen(mDataCallRep[index].dnses)] = '\0';

    dataCallRep->gateways = (char *)malloc(128);
    memcpy(dataCallRep->gateways, mDataCallRep[index].gateways, strlen(mDataCallRep[index].gateways));
    dataCallRep->gateways[strlen(mDataCallRep[index].gateways)] = '\0';

    dataCallRep->pcscf = (char *)malloc(32);
    memcpy(dataCallRep->pcscf, mDataCallRep[index].pcscf, strlen(mDataCallRep[index].pcscf));
    dataCallRep->pcscf[strlen(mDataCallRep[index].pcscf)] = '\0';

    // RLOGD("output value in RpAidlAdapter->getDataCallInfo, profileId=%d, dataCallRep->pdnState=%d,"
    //     " dataCallRep->status=%d, dataCallRep->apnType=%s, dataCallRep->type=%s, dataCallRep->ifname=%s,"
    //     " dataCallRep->addresses=%s, dataCallRep->nses=%s, dataCallRep->gateways=%s, dataCallRep->pcscf=%s,"
    //     " dataCallRep->mtu=%d\n", profileId, dataCallRep->pdnState, dataCallRep->status, dataCallRep->apnType,
    //     dataCallRep->type, dataCallRep->ifname, dataCallRep->addresses, dataCallRep->dnses, dataCallRep->gateways,
    //     dataCallRep->pcscf, dataCallRep->mtu);
    return 0;
}

int32_t RpAidlAdapter::sendRequest(Parcel& dataParcel,int slotId) {
    m_RespMutex.lock();
    dataParcel.setDataPosition(0);
    int32_t request = dataParcel.readInt32();
    if (!IsAsyncRequest(request)) {
        RpAidlAdapter::isLockRelease = false;
    }
    m_RespMutex.unlock();
    if(mSdkClient == NULL) {
        RLOGW("%s mSdkClient is NULL, return\n",__FUNCTION__);
        return -1;
    } else {
        mSdkClient->sendRequest(dataParcel, slotId);
        return 0;
    }
}

std::string RpAidlAdapter::convertCharPtrToAidlString(const char *ptr) {
    return std::string(ptr);
}

void RpAidlAdapter::onRequestComplete(int slotId, int token, RIL_Errno e, void *response, size_t responselen) {
    int request_id = 0;

    if (slotId < 0) {
        return;
    }

    if (isRequestPending(token, &request_id) == false) {
        RLOGD("onRequestComplete no pending request for this response!\n");
        return;
    }

    removePendingRequest(token);

    RLOGD("received token(%d)=> %s response", token, requestToString(request_id));
    switch (request_id) {
        case RIL_REQUEST_DEVICE_IDENTITY:
        {
            rilimpl::responseGetImei(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_SIM_OPEN_CHANNEL:
        {
            rilimpl::responseIccOpenLogicalChannel(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_SETUP_DATA_CALL:
        {
            rilimpl::responseDataCall(slotId, token, e, response, responselen, 1);
            break;
        }
        case RIL_REQUEST_DEACTIVATE_DATA_CALL:
        {
            rilimpl::responseDataCall(slotId, token, e, response, responselen, 0);
            break;
        }
        case RIL_REQUEST_SIGNAL_STRENGTH:
        {
            rilimpl::responseSignalStrength(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_LOCAL_UPDATE_APN:
        case RIL_REQUEST_DIAL:
        case RIL_REQUEST_ANSWER:
        case RIL_REQUEST_HANGUP_ALL:
        case RIL_REQUEST_DTMF_START:
        case RIL_REQUEST_DTMF_STOP:
        case RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
        case RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE:
        case RIL_REQUEST_SIM_CLOSE_CHANNEL:
        case RIL_REQUEST_ECALL_RESET_IVS:
        case RIL_REQUEST_ECALL_SET_MSD:
        case RIL_REQUEST_ECALL_SET_TEST_NUM:
        case RIL_REQUEST_ECALL_SET_RECONF_NUM:
        case RIL_REQUEST_ECALL_MAKE_ECALL:
        case RIL_REQUEST_ECALL_FAST_MAKE_ECALL:
        case RIL_REQUEST_ECALL_SET_IVS:
        case RIL_REQUEST_ECALL_SET_PSAP:
        case RIL_REQUEST_ECALL_IVS_PUSH_MSD:
        case RIL_REQUEST_ECALL_PSAP_PULL_MSD:
        case RIL_REQUEST_ECALL_CTRL_SEQUENCE:
        case RIL_REQUEST_ECALL_SET_REGISTRATION_STATE:
        case RIL_REQUEST_ECALL_SET_PRI:
        case RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME:
        case RIL_REQUEST_ECALL_SET_OPRT_MODE:
        case RIL_REQUEST_STOP_KEEPALIVE:
        case RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER:
        case RIL_REQUEST_SET_IMSCFG:
        case RIL_REQUEST_SET_USAGE_SETTING:
        case RIL_REQUEST_ENABLE_VONR:
        case RIL_REQUEST_SET_CALL_WAITING:
        case RIL_REQUEST_SET_PREFERRED_DATA_MODEM:
        case RIL_REQUEST_SET_SIM_CARD_POWER:
        case RIL_REQUEST_HANGUP:
        case RIL_REQUEST_HOLD_CALL:
        {
            rilimpl::responseNoData(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_ENTER_SIM_PIN:
        case RIL_REQUEST_ENTER_SIM_PIN2:
        case RIL_REQUEST_ENTER_SIM_PUK:
        case RIL_REQUEST_ENTER_SIM_PUK2:
        case RIL_REQUEST_CHANGE_SIM_PIN:
        case RIL_REQUEST_CHANGE_SIM_PIN2:
        case RIL_REQUEST_SET_FACILITY_LOCK:
        {
            rilimpl::responseRetryTimes(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_QUERY_CALL_WAITING:
        {
            rilimpl::responseCallWaiting(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_ECALL_GET_OPRT_MODE:
        {
            rilimpl::responseGetOprtMode(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_SIM_IO:
        {
            rilimpl::responseSetSimIo(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_LOCAL_GET_APN:
        {
            rilimpl::responseGetApn(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_SEND_SMS:
        {
            rilimpl::sendSmsResponse(slotId, token, e, response, responselen);
            break;
        }
        case RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL:
        {
            std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter =
                teleService[slotId]->mapTokenResponsesV1_1.find(token);
            if(iter != teleService[slotId]->mapTokenResponsesV1_1.end()) {
                std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
                ::ndk::ScopedAStatus rc = teleResponse->iccTransmitApduLogicalChannelResponse((int32_t)e);
                if (!rc.isOk())
                    RLOGD("%s,%d call iccTransmitApduLogicalChannelResponse failed.\n", __func__, __LINE__);
                teleService[slotId]->mapTokenResponsesV1_1.erase(token);
            } else {
                RLOGD("RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL:No TeleReponse client match the token %d!\n",token);
            }
            break;
        }
        case RIL_REQUEST_GET_CURRENT_CALLS:
        {
            rilimpl::responseCurrentCalls(slotId,token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_LAST_CALL_FAIL_CAUSE:
        {
            rilimpl::responseLastCallFailCause(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_SIM_GET_ICCID:
        case RIL_REQUEST_GET_SIM_STATUS:
        case RIL_REQUEST_OEM_HOOK_RAW:
        case RIL_REQUEST_LOCAL_RADIO_ACCESS_TYPE:
        case RIL_REQUEST_LOCAL_REG_STATE_TYPE:
        case RIL_REQUEST_GET_ALLOWED_NETWORK_TYPES_BITMAP:
        case RIL_REQUEST_LOCAL_GET_OPERATOR_CODE:
        case RIL_REQUEST_GET_IMSI:
        case RIL_REQUEST_LOCAL_SIGNAL_STRENGTH:
        case RIL_REQUEST_START_KEEPALIVE:
        case RIL_REQUEST_GET_USAGE_SETTING:
        case RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE:
        case RIL_REQUEST_IS_VONR_ENABLED:
        case RIL_REQUEST_GET_NITZ:
        case RIL_REQUEST_OPERATOR:
        case RIL_REQUEST_GET_IMSCFG:
        {
            rilimpl::responseDataNoHandle(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_IMS_REGISTRATION_STATE:
        {
            rilimpl::responseImsRegState(token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_RADIO_POWER:
        case RIL_REQUEST_MODEM_POWEROFF:
        case RIL_REQUEST_MODEM_POWERON:
        {
            std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter = teleService[slotId]->mapTokenResponses.find(token);
            if(iter != teleService[slotId]->mapTokenResponses.end()) {
                std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
                ::ndk::ScopedAStatus rc = teleResponse->enableRadioResponse((int32_t)e);
                if (!rc.isOk())
                    RLOGD("%s,%d call enableRadioResponse failed.\n", __func__, __LINE__);
                teleService[slotId]->mapTokenResponses.erase(token);
            } else {
                RLOGD("RIL_REQUEST_RADIO_POWER:No TeleReponse client match the token %d!\n",token);
            }
            break;
        }
        case RIL_REQUEST_SET_ALLOWED_NETWORK_TYPES_BITMAP:
        case RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE:
        {
            std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>>::iterator iter =
                teleService[slotId]->mapTokenResponses.find(token);
            if(iter != teleService[slotId]->mapTokenResponses.end()) {
                std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse> teleResponse = iter->second;
                ::ndk::ScopedAStatus rc = teleResponse->setPreferredNetworkTypeResponse((int32_t)e);
                if (!rc.isOk())
                    RLOGD("%s,%d call setPreferredNetworkTypeResponse failed.\n", __func__, __LINE__);
                teleService[slotId]->mapTokenResponses.erase(token);

            } else {
                RLOGD("RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE:No TeleReponse client match the token %d!\n",token);
            }
            break;
        }
        case RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT: {
            rilimpl::getAvailableNetworksWithActResponse(slotId, token, e, response,responselen);
            break;
        }
        case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
        case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL:
        {
            rilimpl::requestResposneNoDataForAsync(request_id, slotId, token, e, response, responselen);
            break;
        }
        case RIL_REQUEST_DATA_REGISTRATION_STATE:
        case RIL_REQUEST_VOICE_REGISTRATION_STATE:
        {
            rilimpl::getRegistrationStateRespose(request_id, slotId, token, e, response, responselen);
            break;
        }
        case RIL_REQUEST_GET_CELL_INFO_LIST:
        {
            rilimpl::getCellInfoListResponse(request_id, slotId, token, e, response, responselen);
            break;
        }
        default:
            RLOGD("uncared response!\n");
            break;
    }
    RLOGD("RpAidlAdapter::onRequestComplete done!");
}

void RpAidlAdapter::onUnsolicitedResponse(int slotId, int unsolResponse,const void *data, size_t datalen) {
    std::unique_lock<std::mutex> lock(mtx);
    while (!rilAidlThreadPoolCompleted) {
        RLOGI("Waiting for thread pool to complete...");
        cv.wait(lock);
    }
    switch (unsolResponse) {
        case RIL_UNSOL_SETUP_PDN_STATE_CHANGED:
        case RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED:
        case RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED: {
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::pdnStateChangedInd(slotId, 0, (void*) data, datalen);
            break;
        }
        case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED: {
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::callStatechanged(NULL,slotId);
            break;
        }
        case RIL_UNSOL_SIGNAL_STRENGTH:
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::signalStrengthChangedInd(slotId, data,datalen);
            break;
        case RIL_UNSOL_VOICE_RADIO_TECH_CHANGED:
            break;
        case RIL_UNSOL_RESPONSE_NEW_SMS: {
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::newSmsInd(slotId, 0,(void *)data,datalen);
            break;
        }
        case RIL_UNSOL_CELL_INFO_LIST: {
            rilimpl::cellInfoListInd(slotId, data,datalen);
            break;
        }
        case RIL_UNSOL_KEEPALIVE_STATUS: {
            rilimpl::keepaliveStatus(slotId, data,datalen);
            break;
        }
        case RIL_UNSOL_ECALL_INDICATIONS: {
            rilimpl::ecallIndicationInd(slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED: {
            rilimpl::responsePsNetworkStateChangeInd(slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_RESPONSE_CS_NETWORK_STATE_CHANGED: {
            rilimpl::responseCsNetworkStateChangeInd(slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED: {
            RLOGD("received urc RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED");
            rilimpl::responseCardStateChangedInd(slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_NITZ_TIME_RECEIVED: {
            RLOGD("received urc RIL_UNSOL_NITZ_TIME_RECEIVED");
            rilimpl::nitzTimeReceived(slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
        case RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED: {
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::urcIndWithoutData(unsolResponse, slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED: {
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::radioStateChanged(slotId, data, datalen);
            break;
        }
        case RIL_UNSOL_CALL_RING: {
            RLOGD("received %s urc", requestToString(unsolResponse));
            rilimpl::Autoanswer(slotId);
            break;
        }
        default:
            //RLOGD("don't care urc!\n");
            break;
    }

}

bool RpAidlAdapter::IsAsyncRequest(int request) {
    return allAsyncRequests.count(request) > 0;
}
