// SPDX-License-Identifier: MediaTekProprietary

#ifndef VENDOR_MEDIATEK_HARDWARE_TBOX_TELESERVICE_H
#define VENDOR_MEDIATEK_HARDWARE_TBOX_TELESERVICE_H

#include <map>
#include "RpAidlAdapter.h"
#include <aidl/vendor/mediatek/hardware/tbox/BnTeleService.h>
#include <aidl/vendor/mediatek/hardware/tbox/ITeleServiceResponse.h>
#include <aidl/vendor/mediatek/hardware/tbox/ITeleServiceIndication.h>
#include <android/binder_parcel.h>
#include <vector>

static std::map<std::string, int> s_datafailReason = { {"iot_default", 0},{"iot_net_0", 0},{"default", 0},{"mms", 0},{"wap", 0},{"supl", 0}};

namespace VENDOR_SPACE = ::aidl::vendor::mediatek::hardware::tbox;

struct TeleService : public VENDOR_SPACE::BnTeleService{

    // Methods from VENDOR_SPACE::ITeleService follow.
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>> mTeleResponses;
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceIndication>> mTeleIndications;
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>> mapTokenResponses;
    std::map<int, std::shared_ptr<VENDOR_SPACE::ITeleServiceResponse>> mapTokenResponsesV1_1;

    int32_t mSlotId;
    int32_t mCallStatus;
    int32_t test_flag;
    RpAidlAdapter  * mAidlAdapter;
    RIL_Ecall_Type ecall_type;
    VENDOR_SPACE::ECallStateMsg currentCallMsg;
    void setCallStatus(int status);
    void setLastDataFailReason(std::string type, int reason);
    int32_t setUnsolicitedResponseFilter(int filter);

    static constexpr int RAF_GSM_GROUP = (RAF_GSM | RAF_GPRS | RAF_EDGE);
    static constexpr int RAF_HS_GROUP = (RAF_HSUPA | RAF_HSDPA | RAF_HSPA | RAF_HSPAP);
    static constexpr int RAF_CDMA_GROUP =(RAF_IS95A | RAF_IS95B | RAF_1xRTT);
    static constexpr int RAF_EVDO_GROUP =(RAF_EVDO_0 | RAF_EVDO_A | RAF_EVDO_B | RAF_EHRPD);
    static constexpr int RAF_WCDMA_GROUP = (RAF_HS_GROUP | RAF_UMTS);
    static constexpr int RAF_LTE_GROUP = (RAF_LTE | RAF_LTE_CA);
        // Methods from VENDOR_SPACE::ITeleService follow.
    ::ndk::ScopedAStatus GetOperatorCode(::aidl::vendor::mediatek::hardware::tbox::parcelable_GetOperatorCode* _aidl_return);
    ::ndk::ScopedAStatus IVSPushMSD(int32_t* _aidl_return);
    ::ndk::ScopedAStatus PSAPPushMSD(int32_t* _aidl_return);
    ::ndk::ScopedAStatus answerVcall(int32_t* _aidl_return);
    ::ndk::ScopedAStatus deactiveDataCall(int32_t in_profile_id);
    ::ndk::ScopedAStatus deleteReconfNumber(int32_t* _aidl_return);
    ::ndk::ScopedAStatus deleteTestNumber(int32_t* _aidl_return);
    ::ndk::ScopedAStatus enableIms(int32_t in_onoff, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setDefaultDataSlot(int32_t in_slotId, int32_t* _aidl_return);
    ::ndk::ScopedAStatus enableRadio(int32_t in_onfff, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return);
    ::ndk::ScopedAStatus endVcall(int32_t* _aidl_return);
    ::ndk::ScopedAStatus getDataCallInfo(int32_t in_profile_id, ::aidl::vendor::mediatek::hardware::tbox::parcelable_dataCallResultGet* _aidl_return);
    ::ndk::ScopedAStatus getApn(int32_t in_profile_id, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getApn* _aidl_return);
    ::ndk::ScopedAStatus getCSQ(::aidl::vendor::mediatek::hardware::tbox::parcelable_getCSQ* _aidl_return);
    ::ndk::ScopedAStatus getCardStatus(::aidl::vendor::mediatek::hardware::tbox::parcelable_getCardStatus* _aidl_return);
    ::ndk::ScopedAStatus getCardFullStatus(::aidl::vendor::mediatek::hardware::tbox::parcelable_getCardFullStatus* _aidl_return);
    ::ndk::ScopedAStatus getCurrentCall(::aidl::vendor::mediatek::hardware::tbox::parcelable_getCurrentCall* _aidl_return);
    ::ndk::ScopedAStatus getDataCallReason(int32_t in_profile, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getDataCallReason* _aidl_return);
    ::ndk::ScopedAStatus getEcallType(::aidl::vendor::mediatek::hardware::tbox::parcelable_getEcallType* _aidl_return);
    ::ndk::ScopedAStatus getFullSignalStrength(::aidl::vendor::mediatek::hardware::tbox::parcelable_getFullSignalStrength* _aidl_return);
    ::ndk::ScopedAStatus getIccid(::aidl::vendor::mediatek::hardware::tbox::parcelable_getIccid* _aidl_return);
    ::ndk::ScopedAStatus getImei(::aidl::vendor::mediatek::hardware::tbox::parcelable_getImei* _aidl_return);
    ::ndk::ScopedAStatus getImsRegState(::aidl::vendor::mediatek::hardware::tbox::parcelable_getImsRegState* _aidl_return);
    ::ndk::ScopedAStatus getImsi(::aidl::vendor::mediatek::hardware::tbox::parcelable_getImsi* _aidl_return);
    ::ndk::ScopedAStatus getLastDataCallfailReason(::aidl::vendor::mediatek::hardware::tbox::parcelable_getLastDataCallfailReason* _aidl_return);
    ::ndk::ScopedAStatus getMccMnc(::aidl::vendor::mediatek::hardware::tbox::parcelable_getMccMnc* _aidl_return);
    ::ndk::ScopedAStatus getModemStat(::aidl::vendor::mediatek::hardware::tbox::parcelable_getModemStat* _aidl_return);
    ::ndk::ScopedAStatus getNetState(::aidl::vendor::mediatek::hardware::tbox::parcelable_getNetState* _aidl_return);
    int getRafFromNetworkType(int type);
    int getAdjustedRaf(int raf);
    int getNetworkTypeFromRaf(int raf);
    ::ndk::ScopedAStatus getPreferredNetworkType(
        ::aidl::vendor::mediatek::hardware::tbox::
            parcelable_getPreferredNetworkType *_aidl_return);
    ::ndk::ScopedAStatus getRadioAccessType(::aidl::vendor::mediatek::hardware::tbox::parcelable_getRadioAccessType* _aidl_return);
    ::ndk::ScopedAStatus getSignalStrength(::aidl::vendor::mediatek::hardware::tbox::parcelable_getSignalStrength* _aidl_return);
    ::ndk::ScopedAStatus getStatusVcall(int32_t* _aidl_return);
    ::ndk::ScopedAStatus hangupEcall(int32_t* _aidl_return);
    ::ndk::ScopedAStatus holdVcall(int32_t* _aidl_return);
    ::ndk::ScopedAStatus startDtmf(const std::string& phonenum, int32_t* _aidl_return);
    ::ndk::ScopedAStatus stopDtmf(int32_t* _aidl_return);
    ::ndk::ScopedAStatus supplyIccPinForApp(const std::string& in_pin, const std::string& in_aid, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus supplyIccPin2ForApp(const std::string& in_pin, const std::string& in_aid, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus supplyIccPukForApp(const std::string& in_puk, const std::string& in_pin, const std::string& in_aid, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus supplyIccPuk2ForApp(const std::string& in_puk2, const std::string& in_pin, const std::string& in_aid, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus changeIccPinForApp(const std::string& old_pin, const std::string& new_pin, const std::string& in_aid, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus changeIccPin2ForApp(const std::string& old_pin2, const std::string& new_pin2, const std::string& in_aid, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus setFacilityLockForApp(const std::string& facility, int32_t lockState, const std::string& password, int32_t serviceClass, const std::string& appId, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getPinInfo* _aidl_return);
    ::ndk::ScopedAStatus iccCloseLogicalChannel(int32_t in_channel, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setSimCardPower(int32_t CardPowerState, int32_t* _aidl_return);
    ::ndk::ScopedAStatus iccOpenLogicalChannel(const std::string& in_aid, int32_t in_p2, ::aidl::vendor::mediatek::hardware::tbox::parcelable_iccOpenLogicalChannel* _aidl_return);
    ::ndk::ScopedAStatus iccTransmitApduLogicalChannel(int32_t in_channel, int32_t in_cla, int32_t in_instruction, int32_t in_p1, int32_t in_p2, int32_t in_p3, const std::string& in_data, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return);
    ::ndk::ScopedAStatus makeEcall(int8_t in_type, int32_t* _aidl_return);
    ::ndk::ScopedAStatus iccIoForApp(int32_t command, int32_t fileid, const std::string& pathid, int32_t in_p1, int32_t in_p2, int32_t in_p3,const std::string& data, const std::string& pin2, const std::string& aid_ptr, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getIccIo* _aidl_return);
    ::ndk::ScopedAStatus makeFastEcall(const ::aidl::vendor::mediatek::hardware::tbox::ECallReqMsg& in_msg, int32_t* _aidl_return);
    ::ndk::ScopedAStatus notifyIpoStatus(int32_t in_onoff, int32_t* _aidl_return);
    ::ndk::ScopedAStatus resetIvs(int32_t* _aidl_return);
    ::ndk::ScopedAStatus restartNetwork(int32_t* _aidl_return);
    ::ndk::ScopedAStatus sendAT(const std::string& in_atCmd, int64_t in_timeout_ms, ::aidl::vendor::mediatek::hardware::tbox::parcelable_sendAT* _aidl_return);
    ::ndk::ScopedAStatus sendSms(const ::aidl::vendor::mediatek::hardware::tbox::SmsInfo& in_msg, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setApn(const ::aidl::vendor::mediatek::hardware::tbox::ApnInfo& in_info, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setCTRLSequence(const ::aidl::vendor::mediatek::hardware::tbox::ECallCTRLSequence& in_quence, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setCellInfoInd(int32_t in_onoff, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setEcallType(int8_t in_type, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setEmsdpri(const ::aidl::vendor::mediatek::hardware::tbox::ECallPRI& in_pri, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setIVS(int8_t in_state, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setMSD(const ::aidl::vendor::mediatek::hardware::tbox::ECallSetMSD& in_msd, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setNadDeregTime(const ::aidl::vendor::mediatek::hardware::tbox::ECallTime& in_time, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setNadRegState(int8_t in_state, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setPASP(int8_t in_state, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setOprtMode(int32_t in_mode, int32_t* _aidl_return);
    ::ndk::ScopedAStatus getOprtMode(::aidl::vendor::mediatek::hardware::tbox::parcelable_getOprtMode* _aidl_return);
    ::ndk::ScopedAStatus setPreferredNetworkType(int32_t in_mode, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setReconfNumber(const ::aidl::vendor::mediatek::hardware::tbox::ECallSetNum& in_recNum, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setResponseFunctions(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceIndication>& in_teleServiceIndicationParam);
    ::ndk::ScopedAStatus setTestNumber(const ::aidl::vendor::mediatek::hardware::tbox::ECallSetNum& in_testNum, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setUnsolResponseFilter(::aidl::vendor::mediatek::hardware::tbox::Mtk_IndicationFilter in_filter, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setupDataCall(int32_t in_profile_id);
    ::ndk::ScopedAStatus startVcall(const std::string& in_phone_number, int32_t cli, int32_t* _aidl_return);
    ::ndk::ScopedAStatus endVcallId(int32_t call_id, int32_t* _aidl_return);
    ::ndk::ScopedAStatus controlCall(int32_t controlType, int32_t callId, int32_t* _aidl_return);
    ::ndk::ScopedAStatus getAvailableNetworksWithAct(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam, int32_t* _aidl_return);
    ::ndk::ScopedAStatus sendSmsNoEncoder(const ::aidl::vendor::mediatek::hardware::tbox::GsmSmsMessage& in_message, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam);
    ::ndk::ScopedAStatus getUsageSetting(::aidl::vendor::mediatek::hardware::tbox::parcelable_getUsageSetting* _aidl_return);
    ::ndk::ScopedAStatus setUsageSetting(::aidl::vendor::mediatek::hardware::tbox::UsageSetting in_usageSetting, int32_t* _aidl_return);
    ::ndk::ScopedAStatus setNetworkSelectionModeAutomatic(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam);
    ::ndk::ScopedAStatus setNetworkSelectionModeManual(const std::string& in_operatorNumeric, ::aidl::vendor::mediatek::hardware::tbox::AccessNetwork in_ran, const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam);
    ::ndk::ScopedAStatus getNetworkSelectionMode(::aidl::vendor::mediatek::hardware::tbox::parcelable_getNetworkSelectionMode* _aidl_return);
    ::ndk::ScopedAStatus isVoNrEnabled(::aidl::vendor::mediatek::hardware::tbox::parcelable_isVoNrEnabled* _aidl_return);
    ::ndk::ScopedAStatus setVoNrEnabled(bool enable, int32_t* _aidl_return);
    ::ndk::ScopedAStatus getNitzTime(::aidl::vendor::mediatek::hardware::tbox::parcelable_getNitzTime* _aidl_return);
    ::ndk::ScopedAStatus getVoiceRegistrationState(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam);
    ::ndk::ScopedAStatus getDataRegistrationState(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam);
    ::ndk::ScopedAStatus getCellInfoList(const std::shared_ptr<::aidl::vendor::mediatek::hardware::tbox::ITeleServiceResponse>& in_teleServiceResponseParam);
    ::ndk::ScopedAStatus getOperator(::aidl::vendor::mediatek::hardware::tbox::parcelable_getOperator* _aidl_return);
    ::ndk::ScopedAStatus setCallWaiting(bool enable, int32_t serviceClass, int32_t* _aidl_return);
    ::ndk::ScopedAStatus getCallWaiting(int32_t serviceClass, ::aidl::vendor::mediatek::hardware::tbox::parcelable_getCallWaiting* _aidl_return);
    ::ndk::ScopedAStatus getImsCfg(::aidl::vendor::mediatek::hardware::tbox::parcelable_getImsCfg* _aidl_return);
};
#endif  // VENDOR_MEDIATEK_HARDWARE_TELE_TELESERVICE_H