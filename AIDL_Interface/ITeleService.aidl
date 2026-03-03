/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package vendor.mediatek.hardware.tbox;

import vendor.mediatek.hardware.tbox.ApnInfo;
import vendor.mediatek.hardware.tbox.DataRegStateResult;
import vendor.mediatek.hardware.tbox.ITeleServiceIndication;
import vendor.mediatek.hardware.tbox.ITeleServiceResponse;
import vendor.mediatek.hardware.tbox.MTK_DataCallFailCause;
import vendor.mediatek.hardware.tbox.MTK_SignalStrength;
import vendor.mediatek.hardware.tbox.MTK_CardStatus;
import vendor.mediatek.hardware.tbox.Mtk_IndicationFilter;
import vendor.mediatek.hardware.tbox.RadioTechnology;
import vendor.mediatek.hardware.tbox.SmsInfo;
import vendor.mediatek.hardware.tbox.VoiceRegStateResult;
import vendor.mediatek.hardware.tbox.ITeleServiceResponse;
import vendor.mediatek.hardware.tbox.ECallCTRLSequence;
import vendor.mediatek.hardware.tbox.ECallPRI;
import vendor.mediatek.hardware.tbox.ECallReqMsg;
import vendor.mediatek.hardware.tbox.ECallSetMSD;
import vendor.mediatek.hardware.tbox.ECallSetNum;
import vendor.mediatek.hardware.tbox.ECallStateMsg;
import vendor.mediatek.hardware.tbox.ECallTime;
import vendor.mediatek.hardware.tbox.parcelable_GetOperatorCode;
import vendor.mediatek.hardware.tbox.parcelable_getApn;
import vendor.mediatek.hardware.tbox.parcelable_getPinInfo;
import vendor.mediatek.hardware.tbox.parcelable_dataCallResultGet;
import vendor.mediatek.hardware.tbox.parcelable_getCSQ;
import vendor.mediatek.hardware.tbox.parcelable_getCardStatus;
import vendor.mediatek.hardware.tbox.parcelable_getCardFullStatus;
import vendor.mediatek.hardware.tbox.parcelable_getCurrentCall;
import vendor.mediatek.hardware.tbox.parcelable_getDataCallReason;
import vendor.mediatek.hardware.tbox.parcelable_getEcallType;
import vendor.mediatek.hardware.tbox.parcelable_getFullSignalStrength;
import vendor.mediatek.hardware.tbox.parcelable_getIccid;
import vendor.mediatek.hardware.tbox.parcelable_getIccIo;
import vendor.mediatek.hardware.tbox.parcelable_getImei;
import vendor.mediatek.hardware.tbox.parcelable_getImsRegState;
import vendor.mediatek.hardware.tbox.parcelable_getImsi;
import vendor.mediatek.hardware.tbox.parcelable_getLastDataCallfailReason;
import vendor.mediatek.hardware.tbox.parcelable_getMccMnc;
import vendor.mediatek.hardware.tbox.parcelable_getModemStat;
import vendor.mediatek.hardware.tbox.parcelable_getNetState;
import vendor.mediatek.hardware.tbox.parcelable_getPreferredNetworkType;
import vendor.mediatek.hardware.tbox.parcelable_getRadioAccessType;
import vendor.mediatek.hardware.tbox.parcelable_getSignalStrength;
import vendor.mediatek.hardware.tbox.parcelable_iccOpenLogicalChannel;
import vendor.mediatek.hardware.tbox.parcelable_sendAT;
import vendor.mediatek.hardware.tbox.parcelable_getOprtMode;
import vendor.mediatek.hardware.tbox.GsmSmsMessage;
import vendor.mediatek.hardware.tbox.parcelable_getUsageSetting;
import vendor.mediatek.hardware.tbox.UsageSetting;
import vendor.mediatek.hardware.tbox.AccessNetwork;
import vendor.mediatek.hardware.tbox.parcelable_getNetworkSelectionMode;
import vendor.mediatek.hardware.tbox.parcelable_isVoNrEnabled;
import vendor.mediatek.hardware.tbox.parcelable_getNitzTime;
import vendor.mediatek.hardware.tbox.parcelable_getImsCfg;
import vendor.mediatek.hardware.tbox.RegStateResult;
import vendor.mediatek.hardware.tbox.RegState;
import vendor.mediatek.hardware.tbox.RadioTechnology;
import vendor.mediatek.hardware.tbox.parcelable_getOperator;
import vendor.mediatek.hardware.tbox.parcelable_getCallWaiting;
/**
 * This interface is used by MtkLayer Client to talk to tele-fwk.
 * All the functions have minimum one parameter:
 * serial: which corresponds to serial no. of request. Serial numbers must only be memorized for the
 * duration of a method call. If clients provide colliding serials (including passing the same
 * serial to different methods), multiple responses (one for each method call) must still be served.
 */
// Interface inherits from vendor.mediatek.hardware.tbox@1.1::ITeleService but AIDL does not support interface inheritance (methods have been flattened).
@VintfStability
interface ITeleService {
    // FIX done
    parcelable_GetOperatorCode GetOperatorCode();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int IVSPushMSD();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int PSAPPushMSD();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int answerVcall();

    void deactiveDataCall(in int profile_id);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int deleteReconfNumber();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int deleteTestNumber();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int enableIms(in int onoff);

    int setDefaultDataSlot(in int slotId);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int enableRadio(in int onfff, in ITeleServiceResponse teleServiceResponseParam);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int endVcall();

    // FIX done
    parcelable_getApn getApn(in int profile_id);

    parcelable_dataCallResultGet getDataCallInfo(in int profile_id);

    // FIX done
    parcelable_getCSQ getCSQ();

    // FIX done
    parcelable_getCardStatus getCardStatus();

    // FIX done
    parcelable_getCardFullStatus getCardFullStatus();

    // FIX done
    parcelable_getCurrentCall getCurrentCall();

    // FIX done
    parcelable_getDataCallReason getDataCallReason(in int profile);

    // FIX done
    parcelable_getEcallType getEcallType();

    // FIX done
    parcelable_getFullSignalStrength getFullSignalStrength();

    // FIX done
    parcelable_getIccid getIccid();

    // FIX done
    parcelable_getImei getImei();

    // FIX done
    parcelable_getImsRegState getImsRegState();

    // FIX done
    parcelable_getImsi getImsi();

    // FIX done
    parcelable_getLastDataCallfailReason getLastDataCallfailReason();

    // FIX done
    parcelable_getMccMnc getMccMnc();

    // FIX done
    parcelable_getModemStat getModemStat();

    // FIX done
    parcelable_getNetState getNetState();

    // FIX done
    parcelable_getPreferredNetworkType getPreferredNetworkType();

    // FIX done
    parcelable_getRadioAccessType getRadioAccessType();

    // FIX done
    parcelable_getSignalStrength getSignalStrength();

    parcelable_getIccIo iccIoForApp(in int command, in int fileid, in String pathid, in int in_p1, in int in_p2, in int in_p3, in String data, in String pin2, in String aid_ptr);

    // Adding return type to method instead of out param int state since there is only one return value.
    int getStatusVcall();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int hangupEcall();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int holdVcall();

    parcelable_getPinInfo supplyIccPinForApp(in String pin, in String aid);

    parcelable_getPinInfo supplyIccPin2ForApp(in String pin, in String aid);


    parcelable_getPinInfo supplyIccPukForApp(in String puk, in String pin, in String aid);

    parcelable_getPinInfo supplyIccPuk2ForApp(in String puk, in String pin, in String aid);

    parcelable_getPinInfo changeIccPinForApp(in String oldPin, in String newPin, in String aid);

    parcelable_getPinInfo changeIccPin2ForApp(in String oldPin2, in String newPin2, in String aid);

    parcelable_getPinInfo setFacilityLockForApp(in String facility, in int lockState, in String password, in int serviceClass, in String appId);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int iccCloseLogicalChannel(in int channel);

    int setSimCardPower(in int CardPowerState);

    int endVcallId(in int call_id);

    // FIX done
    parcelable_iccOpenLogicalChannel iccOpenLogicalChannel(in String aid, in int p2);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int iccTransmitApduLogicalChannel(in int channel, in int cla, in int instruction, in int p1,
        in int p2, in int p3, in String data, in ITeleServiceResponse teleServiceResponseParam);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int makeEcall(in byte type);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int makeFastEcall(in ECallReqMsg msg);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int notifyIpoStatus(in int onoff);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int resetIvs();

    // Adding return type to method instead of out param int err since there is only one return value.
    int restartNetwork();

    // FIX done
    parcelable_sendAT sendAT(in String atCmd, in long timeout_ms);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int sendSms(in SmsInfo msg, in ITeleServiceResponse teleServiceResponseParam);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setApn(in ApnInfo info);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setCTRLSequence(in ECallCTRLSequence quence);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setCellInfoInd(in int onoff);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setEcallType(in byte type);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setEmsdpri(in ECallPRI pri);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setIVS(in byte state);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setMSD(in ECallSetMSD msd);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setNadDeregTime(in ECallTime time);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setNadRegState(in byte state);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setPASP(in byte state);

    int setOprtMode(in int mode);
    parcelable_getOprtMode getOprtMode();

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setPreferredNetworkType(in int mode, in ITeleServiceResponse teleServiceResponseParam);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setReconfNumber(in ECallSetNum recNum);

    oneway void setResponseFunctions(in ITeleServiceResponse teleServiceResponseParam,
        in ITeleServiceIndication teleServiceIndicationParam);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setTestNumber(in ECallSetNum testNum);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int setUnsolResponseFilter(in Mtk_IndicationFilter filter);

    void setupDataCall(in int profile_id);

    // Adding return type to method instead of out param int ril_err since there is only one return value.
    int startVcall(in String phone_number, in int cli);

    int startDtmf(in String phone_number);

    int stopDtmf();

    int controlCall(in int controlType, in int callId);
    /**
     * Scans for available networks with ACT
     *
     * Response function is IMtkRadioExResponse.getAvailableNetworksWithActResponse()
     */
    int getAvailableNetworksWithAct(in ITeleServiceResponse teleServiceResponseParam);

    /**
     * Send an SMS message. Based on the returned error, caller decides to resend if sending sms
     * fails. RadioError:SMS_SEND_FAIL_RETRY means retry (i.e. error cause is 332) and
     * RadioError:GENERIC_FAILURE means no retry (i.e. error cause is 500)
     *
     * @param serial Serial number of request.
     * @param message GsmSmsMessage to be sent
     *
     * Response function is IRadioMessagingResponse.sendSmsResponse()
     */
    void sendSmsNoEncoder(in GsmSmsMessage message, in ITeleServiceResponse teleServiceResponseParam);
    /**
     * Set the UE usage setting for data/voice centric usage.
     *
     * <p>Sets the usage setting in accordance with 3gpp 24.301 sec 4.3 and 3gpp 24.501 sec 4.3.
     * <p>This value must be independently preserved for each SIM; (setting the value is not a
     * "global" override).
     *
     * @param usageSetting the usage setting for the current SIM.
     *
     */
    int setUsageSetting(in UsageSetting usageSetting);

    /**
     * Get the UE usage setting for data/voice centric usage.
     *
     * <p>Gets the usage setting in accordance with 3gpp 24.301 sec 4.3 and 3gpp 24.501 sec 4.3.
     *
     * @param serial Serial number of request.
     *
     */
    parcelable_getUsageSetting getUsageSetting();

    /**
     * Specify that the network must be selected automatically.
     * This request must not respond until the new operator is selected and registered.
     *
     * @param serial Serial number of request.
     *
     * Response function is ITeleServiceResponse.setNetworkSelectionModeAutomaticResponse()
     *
     */
    void setNetworkSelectionModeAutomatic(in ITeleServiceResponse teleServiceResponseParam);

    /**
     * Manually select a specified network. This request must not respond until the new operator is
     * selected and registered. Per TS 23.122, the RAN is just the initial suggested value.
     * If registration fails, the RAN is not available afterwards, or the RAN is not within the
     * network types specified by IRadioNetwork::setAllowedNetworkTypeBitmap, then the modem will
     * need to select the next best RAN for network registration.
     *
     * @param serial Serial number of request.
     * @param operatorNumeric String specifying MCCMNC of network to select (eg "310170").
     * @param ran Initial suggested access network type. If value is UNKNOWN, the modem will select
     *        the next best RAN for network registration.
     *
     * Response function is ITeleServiceResponse.setNetworkSelectionModeManualResponse()
     *
     */
    void setNetworkSelectionModeManual(in String operatorNumeric, in AccessNetwork ran, in ITeleServiceResponse teleServiceResponseParam);

    /**
     * Query current network selection mode
     *
     */
    parcelable_getNetworkSelectionMode getNetworkSelectionMode();

    /**
     * Query current Voice NR enable state
     *
     */
    parcelable_isVoNrEnabled isVoNrEnabled();

	/**
     * Set Voice NR enable state
     *
     * @param enable true for "enable vonr" and false for "disable vonr"
     *
     */
    int setVoNrEnabled(in boolean enable);
	/**
     * Get NitzTime
     *
     */
    parcelable_getNitzTime getNitzTime();
	/**
     * Get ims confg
     *
     */
    parcelable_getImsCfg getImsCfg();
    /**
     * Request current voice registration state.
     *
     * Response function is ITeleServiceResponse.getVoiceRegistrationStateResponse()
     *
     * This is available when vendor.mediatek.hardware.tbox is defined.
     */
    void getVoiceRegistrationState(in ITeleServiceResponse teleServiceResponseParam);

    /**
     * Request current data registration state.
     *
     * Response function is ITeleServiceResponse.getDataRegistrationStateResponse()
     *
     * This is available when vendor.mediatek.hardware.tbox is defined.
     */
    void getDataRegistrationState(in ITeleServiceResponse teleServiceResponseParam);

    /**
     * Request all of the current cell information known to the radio. The radio must return a list
     * of all current cells, including the neighboring cells. If for a particular cell information
     * isn't known then the appropriate unknown value will be returned.
     * This does not cause or change the rate of unsolicited cellInfoList().
     *
     * @param serial Serial number of request.
     *
     * Response function is ITeleServiceResponse.getCellInfoListResponse()
     */
    void getCellInfoList(in ITeleServiceResponse teleServiceResponseParam);

    /**
     * Request current operator ONS or EONS
     *
     */
    parcelable_getOperator getOperator();

    	/**
     * Set CallWaiting
     *
     * @param enable true for "CallWaiting" and false for "disable CallWaiting"
     * @param serviceClass
     *
     */
    int setCallWaiting(in boolean enable, in int serviceClass);

    	/**
     * Get Voice NR enable state
     *
     * @param serviceClass
     *
     */
    parcelable_getCallWaiting getCallWaiting(in int serviceClass);
}
