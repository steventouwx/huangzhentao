// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ims;

import android.hardware.radio.RadioIndicationType;
import vendor.mediatek.hardware.mtkradioex.voice.CallInfoType;
import vendor.mediatek.hardware.mtkradioex.voice.Dialog;
import vendor.mediatek.hardware.mtkradioex.ims.ImsConfParticipant;
import vendor.mediatek.hardware.mtkradioex.ims.ImsRegStatusInfo;

/**
 * Interface declaring unsolicited IMS indications.
 */
@VintfStability
interface IMtkRadioExImsIndication {

// M: IMS_DATA start @{
    /**
     * [IMS] IMS Bearer State Notify Activation/Deactivation
     * @param type Indication type
     * @param aid AID
     * @param capabilities Capability
     */
    oneway void imsBearerStateNotify(in android.hardware.radio.RadioIndicationType type,
        in int aid, in int action, in String capability);

    /**
     * [IMS] IMS Bearer Initialization
     * @param type Indication type
     */
    oneway void imsBearerInit(in android.hardware.radio.RadioIndicationType type);
    
    /**
     * [IMS] IMS Data event info Indication
     * @param type Indication type
     * @param capabilities Capability
     * @param event Event type
     * @param extra Extra info
     */
    oneway void imsDataInfoNotify(in android.hardware.radio.RadioIndicationType type,
        in String capability, in String event, in String extra);

    /**
     * Indicates when new CDMA SMS is received
     * Callee must subsequently confirm the receipt of the SMS with
     * acknowledgeLastIncomingCdmaSmsEx()
     * Server must not send cdmaNewSms() messages until
     * acknowledgeLastIncomingCdmaSmsEx() has been received
     *
     * @param type Type of radio indication
     * @param msg Cdma Sms Message
     */
    oneway void cdmaNewSmsEx(in android.hardware.radio.RadioIndicationType type,
        in android.hardware.radio.messaging.CdmaSmsMessage msg);
// M: IMS_DATA end @}

// M: IMS_SMS start @{
    /**
     * Indicates when new SMS Status Report is received.
     * Callee must subsequently confirm the receipt of the SMS with a
     * acknowledgeLastIncomingGsmSmsEx()
     * ImsSmsDispatcher is waitting for it.
     *
     * Server must not send newSms() nor newSmsStatusReport() messages until a
     * acknowledgeLastIncomingGsmSms() has been received
     *
     * @param type Type of radio indication
     * @param pdu PDU of SMS-STATUS-REPORT represented as byte array.
     *        The PDU starts with the SMSC address per TS 27.005 (+CMT:)
     */
    oneway void newSmsStatusReportEx(in android.hardware.radio.RadioIndicationType type,
        in byte[] pdu);

    /**
     * Indicates when new Ims SMS is received.
     * Callee must subsequently confirm the receipt of the SMS with a
     * acknowledgeLastIncomingGsmSmsEx()
     * ImsSmsDispatcher is waitting for it.
     *
     * Server must not send newSms() nor newSmsStatusReport() messages until a
     * acknowledgeLastIncomingGsmSms() has been received
     *
     * @param type Type of radio indication
     * @param pdu PDU of SMS-DELIVER represented as byte array.
     *        The PDU starts with the SMSC address per TS 27.005 (+CMT:)
     */
    oneway void newSmsEx(in android.hardware.radio.RadioIndicationType type, in byte[] pdu);
// M: IMS_SMS end @}

// M: IMS_RTT start @{
    /**
     * Indication for RTT Modify Response
     * RIL_UNSOL_RTT_MODIFY_RESPONSE
     *
     * @param type Type of radio indication
     * @param callId Call Id
     * @param result 0: Success, 1: Fail
     */
    oneway void rttModifyResponse(in android.hardware.radio.RadioIndicationType type,
        in int callId, in int result);

    /**
     * Indication for receiving RTT Text
     * RIL_UNSOL_RTT_TEXT_RECEIVE
     *
     * @param type Type of radio indication
     * @param callId Call Id
     * @param lenOfString Length of String
     * @param text Text
     */
    oneway void rttTextReceive(in android.hardware.radio.RadioIndicationType type, in int callId,
        in int lenOfString, in String text);

    /**
     * Indication for RTT Capability
     * RIL_UNSOL_RTT_CAPABILITY_INDICATION
     *
     * @param type Type of radio indication
     * @param callId Call Id
     * @param localCap Local text capability
     * @param remoteCap Remote text capability
     * @param localStatus Local Text Status
     * @param remoteStatus Real Remote Text Capability
     */
    oneway void rttCapabilityIndication(in android.hardware.radio.RadioIndicationType type,
        in int callId, in int localCap, in int remoteCap, in int localStatus,
        in int remoteStatus);

    /**
     * Indication for Pear Upgrade/Downgrade Request
     * RIL_UNSOL_RTT_MODIFY_REQUEST_RECEIVE
     *
     * @param type Type of radio indication
     * @param callId Call Id
     * @param rttType 0: Peer Request RTT downgrade
     *             1: Peer Request RTT upgrade
     * @param text Text
     */
    oneway void rttModifyRequestReceive(in android.hardware.radio.RadioIndicationType type,
        in int callId, in int rttType);

    /**
     * Indication for RTT Audio
     * RIL_UNSOL_RTT_AUDIO_INDICATION
     *
     * @param type Type of radio indication
     * @param callId Call Id
     * @param audio 0: Silence, 1: Speech
     */
    oneway void audioIndication(in android.hardware.radio.RadioIndicationType type, in int callId,
        in int audio);
// M: IMS_RTT end @}

// M: IMS_BASE start @{
    /**
     * Indication for IMS VoPS
     * RIL_UNSOL_IMSVOPS_INDICATION
     *
     * @param type Type of radio indication
     * @param vops IMS VoPS value
     */
    oneway void sendVopsIndication(in android.hardware.radio.RadioIndicationType type,
        in int vops);

    /**
     * [IMS] VoLTE Setting Indiciation
     * @param Type Indication type
     * @param isEnable Is enable
     */
    oneway void volteSetting(in android.hardware.radio.RadioIndicationType type,
        in boolean isEnable);

    /**
     * [IMS] IMS Registration Info Indiciation
     * @param type Indication type
     * @param registerState If IMS is registered or not
     * @param capability IMS capabilities
     */
    oneway void imsRegistrationInfo(in android.hardware.radio.RadioIndicationType type,
        in int registerState, in int capability);

    /**
     * [IMS] network support LTE emergency call or not
     * @param type Indication type
     * @param 1 for support ecc, 0 not
     */
    oneway void imsSupportEcc(in android.hardware.radio.RadioIndicationType type,
        in int supportLteEcc);

    /**
     * [IMS] SIP registration info Indication
     * @param type Indication type
     * @param account_id account id
     * @param response_code registration response code
     * @param info 1. direction
     *             2. SIP_msg_type
     *             3. method
     *             4. reason_phrase
     *             5. warn_text
     */
    oneway void sipRegInfoInd(in android.hardware.radio.RadioIndicationType type,
        in int account_id, in int response_code, in String[] info);

    /**
     * Report IMS registration status
     * @param report IMS registration status
     */
    oneway void imsRegStatusReport(in android.hardware.radio.RadioIndicationType type,
        in ImsRegStatusInfo report);

    /**
     * [IMS] Detail IMS registration info Indication
     * @param type Indication type
     * @param info 1. reg_state
     *             2. reg_type
     *             3. ext_info
     *             4. dereg_cause
     *             5. ims_retry
     *             6. rat
     *             7. sip_uri_type
     */
    oneway void imsRegInfoInd(in android.hardware.radio.RadioIndicationType type, in int[] info);

    /**
     * [IMS] Re-register/Re-Sub info Indication
     * @param type Indication type
     * @param info 1. rereg_flag
     *             2. rereg_remain_time
     *             3. resub_flag
     *             4. resub_remain_time
     */
    oneway void eregrtInfoInd(in android.hardware.radio.RadioIndicationType type, in int[] info);

    /**
     * [IMS] IMS register flag Indiciation
     * @param Type Indication type
     * @param flag for ims register
     */
    oneway void imsRegFlagInd(in android.hardware.radio.RadioIndicationType type, in int flag);

    /**
     * [IMS] IMS RTP Indiciation Report
     * @param type Indication type
     * @param pdnId PDN Id
     * @param networkId Network Id
     * @param timer Timer
     * @param sendPktLost Send packet lost
     * @param recvPktLost Receive packet lost
     * @param jetter Jitter in ms
     * @param delay Delay in ms
     */
    oneway void imsRtpInfo(in android.hardware.radio.RadioIndicationType type, in String pdnId,
        in String networkId, in String timer, in String sendPktLost, in String recvPktLost,
        in String jitter, in String delay);

    /**
     * [IMS] On Xui Indiciation
     * @param type Indication type
     * @param accountId Account Id
     * @param broadcastFlag Broadcast Flag
     * @param xuiInfo XUI Infomation
     */
    oneway void onXui(in android.hardware.radio.RadioIndicationType type, in String accountId,
        in String broadcastFlag, in String xuiInfo);
// M: IMS_BASE end @}

// M: IMS_SS start @{
    /**
     * [IMS] On USSI Indiciation
     * Indicates when a new USSD message is received.
     * The USSD session is assumed to persist if the type code is REQUEST, otherwise
     * the current session (if any) is assumed to have terminated.
     *
     * @param type Type of radio indication
     * @param modeType USSD type code
     * @param msg Message string in UTF-8, if applicable
     */
    oneway void onUssi(in android.hardware.radio.RadioIndicationType type, in int modeType,
        in String msg);

    /**
     * [IMS] On VoLTE Subscription
     * @param type Indication type
     * @param status 0: Unknown, 1: is VoLTE card, 2: non VoLTE card
     */
    oneway void onVolteSubscription(in android.hardware.radio.RadioIndicationType type,
        in int status);
// M: IMS_SS end @}

// M: IMS_ViLTE start @{
    /**
     * [IMS] Video Capability Indiciation
     * @param type Indication type
     * @param callId Call id
     * @param localVideoCap Local video capabilities
     * @param remoteVideoCap Remote video capabilities
     */
    oneway void videoCapabilityIndicator(in android.hardware.radio.RadioIndicationType type,
        in String callId, in String localVideoCap, in String remoteVideoCap);

    /**
     * Interface declaring unsolicited radio indications.
     * @param type Indication type
     * @param event the event to notify to UI
     */
    oneway void videoRingtoneEventInd(in android.hardware.radio.RadioIndicationType type,
        in String[] event);

    /**
     * MD internet interface usage info Indication
     * Tx bytes will be reset once internet interface is up
     * After every 2MB memory consumption, it will update the statistics using same URC
     * The statistics will be reset on interface down / interface up
     * @param type Indication type
     * @param info 1. <sim_id>: Sim1~4 = {0|1|2|3}
     *             2. <tx bytes>: Total tx bytes on internet interface for SIM (bytes) since interface up
     *             3. <rx bytes>: Total rx bytes on internet interface for SIM (bytes) since interface up
     *             4. <tx pkt>: Total tx pkts on internet interface for SIM since interface up
     *             5. <rx pkt>: Total rx pkts on internet interface for SIM since interface up
     */
    oneway void onMDInternetUsageInd(in android.hardware.radio.RadioIndicationType type,
        in int[] info);
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
    /**
     * [IMS] ECT Indiciation
     * @param type Indication type
     * @param call_id Call Id
     * @param ectResult ECT result
     * @param cause Cause
     */
    oneway void ectIndication(in android.hardware.radio.RadioIndicationType type, in int call_id,
        in int ectResult, in int cause);

    /**
     * [IMS] Call info Indiciation
     * @param type Indication Type
     * @param Indication data
     */
    oneway void callInfoIndication(in android.hardware.radio.RadioIndicationType type,
        in String[] data);

    /**
     * [IMS] Call Mod Change Indiciation
     * @param type Indication type
     * @param callId Call id
     * @param callMode Call mode
     * @param videoState Video State
     * @param audioDirection Audio direction
     * @param pau PAU
     */
    oneway void callmodChangeIndicator(in android.hardware.radio.RadioIndicationType type,
        in String callId, in String callMode, in String videoState, in String audioDirection,
        in String pau);

    /**
     * [IMS] ECONF Result Indiciation
     * @param type Indication Type
     * @param confCallId Conference call id
     * @param op Operator
     * @param num Number
     * @param result Result
     * @param cause Cause
     * @param joinedCallId
     */
    oneway void econfResultIndication(in android.hardware.radio.RadioIndicationType type,
        in String confCallId, in String op, in String num, in String result, in String cause,
        in String joinedCallId);

    /**
     * [IMS] SIP Call Progress Indiciation
     * @param type Indication type
     * @param callid Call Id
     * @param dir Dir
     * @param sipMsgType SIP message type
     * @param method Method
     * @param responseCode Response code
     * @param reasonText Reason text
     */
    oneway void sipCallProgressIndicator(in android.hardware.radio.RadioIndicationType type,
        in String callId, in String dir, in String sipMsgType, in String method,
        in String responseCode, in String reasonText);

    /**
     * [IMS] Udate IMS Conference participants info
     * @param type Indication type
     * @param arrays of ims conference participant info
     */
    oneway void imsConferenceInfoIndication(in android.hardware.radio.RadioIndicationType type,
        in ImsConfParticipant[] participants);

    /**
     * [IMS] Lte Message waiting Indiciation
     * @param type Indication type
     * @param callId Call Id
     * @param ptype Package Type
     * @param urcIdx URC Index
     * @param totalUrCount Total URC count
     * @param rawData
     */
    oneway void lteMessageWaitingIndication(in android.hardware.radio.RadioIndicationType type,
        in String callId, in String ptype, in String urcIdx, in String totalUrcCount,
        in String rawData);

    /**
     * [IMS] IMS Dialog Event Package Indiciation
     *
     * @param type Type of radio indication
     * @param dialogList the dialog info list
     */
    oneway void imsDialogIndication(in android.hardware.radio.RadioIndicationType type,
        in Dialog[] dialogList);

    /**
     * Indicates that the radio system selection module
     * won't enter emergency callback mode when the emergency call end.
     *
     * @param type Type of radio indication
     */
    oneway void noEmergencyCallbackMode(in android.hardware.radio.RadioIndicationType type);

    /**
     * [IMS] Ims Redial As ECC Indication
     * @param type Indication type
     * @param callId Call Id
     */
    oneway void imsRedialEmergencyIndication(in android.hardware.radio.RadioIndicationType type,
        in String callId);

    /**
     * Reports speech codec information
     *
     * @param type Type of radio indication
     * @param info integer type speech codec info
     */
    oneway void speechCodecInfoIndication(in android.hardware.radio.RadioIndicationType type,
        in int info);

    /**
     * [IMS] IMS Event Package Indiciation
     * @param type Indication type
     * @param callId Call Id
     * @param ptype Package Type
     * @param urcIdx URC Index
     * @param totalUrCount Total URC count
     * @param rawData
     */
    oneway void imsEventPackageIndication(in android.hardware.radio.RadioIndicationType type,
        in String callId, in String ptype, in String urcIdx, in String totalUrcCount,
        in String rawData);

    /**
     * Report Sip header info
     * @param type Indication Type
     * @param data Indication data
     */
    oneway void sipHeaderReport(in android.hardware.radio.RadioIndicationType type,
        in String[] data);

    /**
     * Report Voice call domain select result
     * @param type Indication Type
     * @param domain 0 = CS; 1 = IMS
     * @param rat The rat of IMS call, 1 = LTE; 2 = Wifi
     */
    oneway void callRatIndication(in android.hardware.radio.RadioIndicationType type,
        in int domain, in int rat);
// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
    /**
     * [IMS] Ims Config Dynamic IMS Switch Complete
     * @param type Indication type
     */
    oneway void imsCfgDynamicImsSwitchComplete(
        in android.hardware.radio.RadioIndicationType type);

    /**
     * [IMS] Ims Config Config Changed
     * @param type Indication type
     * @param phoneId Phone Id
     * @param configId config Id
     * @param value Config value
     */
    oneway void imsCfgConfigChanged(in android.hardware.radio.RadioIndicationType type,
        in int phoneId, in String configId, in String value);

    /**
     * [IMS] Ims Config Feature Changed
     * @param type Indication type
     * @param phoneId Phone Id
     * @param featureId Feature Id
     * @param value Feature value
     */
    oneway void imsCfgFeatureChanged(in android.hardware.radio.RadioIndicationType type,
        in int phoneId, in int featureId, in int value);

    /**
     * [IMS] Ims Config Data Loaded
     * @param type Indication type
     */
    oneway void imsCfgConfigLoaded(in android.hardware.radio.RadioIndicationType type);
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
    /**
     * Indicates the onSsacStatus
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onSsacStatus(in android.hardware.radio.RadioIndicationType type, in int[] status);
// M: IMS_WFC end @}

// M: IMS_CC start @{
    /**
     * Report SIP invite info of MT
     * @param type Type of radio indication
     * @param number MT number
     * @param text Content of SIP invite
     */
    oneway void incomingCallSipInviteIndication(in android.hardware.radio.RadioIndicationType type,
        in String number, in String text);
// M: IMS_CC end @}
}
