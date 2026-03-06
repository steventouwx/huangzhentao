// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ims;

import android.hardware.radio.RadioResponseInfo;
import android.hardware.radio.messaging.ImsSmsMessage;
import android.hardware.radio.messaging.CdmaSmsAck;
import android.hardware.radio.messaging.SmsAcknowledgeFailCause;
import android.hardware.radio.voice.Dial;

import vendor.mediatek.hardware.mtkradioex.ims.IMtkRadioExImsResponse;
import vendor.mediatek.hardware.mtkradioex.ims.IMtkRadioExImsIndication;
import vendor.mediatek.hardware.mtkradioex.ims.ImsBarringCall;
import vendor.mediatek.hardware.mtkradioex.mwi.IMwiRadioResponse;
import vendor.mediatek.hardware.mtkradioex.mwi.IMwiRadioIndication;
import vendor.mediatek.hardware.mtkradioex.voice.ConferenceDial;
import vendor.mediatek.hardware.mtkradioex.voice.CallForwardInfoEx;


/**
 * Interface declaring response functions to IMS requests.
 */
@VintfStability
oneway interface IMtkRadioExIms {

// M: IMS_DATA start @{
    /**
     * Response message of 'Bearer Activiation Done' - Void
     *
     * @param serial Serail Number
     * @param action Action
     * @param ussiString USSI String
     */
    void imsBearerStateConfirm(in int serial, in int aid, in int action, in int status, in int clientId);

   /**
     * config bearer notification.
     *
     * @param serial Serail Number
     * @param enable
     */
    void setImsBearerNotification(in int serial, in int enable, in int clientId);
// M: IMS_DATA end @}

// M: IMS_SMS start @{
    /**
     * Acknowledge the success or failure in the receipt of SMS
     * previously indicated via cdmaNewSmsEx()
     *
     * @param serial Serial number of request.
     * @param smsAck Cdma Sms ack to be sent described by CdmaSmsAck in types.hal
     *
     * Response callback is IImsRadioResponse.acknowledgeLastIncomingCdmaSmsResponseEx()
     */
    void acknowledgeLastIncomingCdmaSmsEx(in int serial,
        in android.hardware.radio.messaging.CdmaSmsAck smsAck, in int clientId);

    /**
     * Acknowledge successful or failed receipt of SMS previously indicated
     * via unsolResponseNewSms
     *
     * @param serial Serial number of request.
     * @param success is true on successful receipt
     *        (basically, AT+CNMA=1 from TS 27.005 is 0 on failed receipt
     *        (basically, AT+CNMA=2 from TS 27.005)
     * @param cause: if success is false, this contains the failure cause as defined
     *        in TS 23.040, 9.2.3.22.
     *
     * Response function is IMtkRadioExResponse.acknowledgeLastIncomingGsmSmsExResponse()
     */
    void acknowledgeLastIncomingGsmSmsEx(in int serial, in boolean success,
        in android.hardware.radio.messaging.SmsAcknowledgeFailCause cause, in int clientId);

    /**
     * Send a SMS message over IMS.
     * Based on the return error, caller decides to resend if sending sms
     * fails. SMS_SEND_FAIL_RETRY means retry, and other errors means no retry.
     * In case of retry, data is encoded based on Voice Technology available.
     *
     * @param serial Serial number of request.
     * @param message ImsSmsMessage as defined in types.hal to be sent
     *
     * Response callback is IMtkRadioExResponse.sendImsSmsExResponse()
     */
    void sendImsSmsEx(in int serial, in android.hardware.radio.messaging.ImsSmsMessage message, in int clientId);
// M: IMS_SMS end @}

// M: IMS_RTT start @{
    /**
     * Request RTT Modify Response
     * RIL_REQUEST_RTT_MODIFY_REQUEST_RESPONSE
     *
     * @param serial Serial number of request
     * @param callId Call Id
     * @param result 0: Accept the request, 1:Reject the request
     */
    void rttModifyRequestResponse(in int serial, in int callId, in int result, in int clientId);

    /**
     * Send RTT Modify Request, IMS RTT Call Upgrade/Downgrade
     * RIL_REQUEST_SEND_RTT_MODIFY_REQUEST
     *
     * @param serial Serial number of request
     * @param callid Call Id
     * @param newMode 0: Downgrade, 1: Upgrade
     */
    void sendRttModifyRequest(in int serial, in int callId, in int newMode, in int clientId);

    /**
     * Send RTT Text, Send or Receive RTT String
     * RIL_REQUEST_SEND_RTT_TEXT
     *
     * @param serial Serial number of request
     * @param callId Call Id
     * @param lenOfString Length of RTT text String
     * @param text RTT Text String
     */
    void sendRttText(in int serial, in int callId, in int lenOfString, in String text, in int clientId);

    /**
     * Set RTT Mode, Indicate RTT Call type
     * RIL_REQUEST_SET_RTT_MODE
     *
     * @param serial Serial number of request
     * @param mode 0: Not a RTT Call (RTT OFF)
     *             1: RTT Call (RTT Auto)
     *             2: Enable IMS RTT Capability with upon Request RTT Operation Mode
     */
    void setRttMode(in int serial, in int mode, in int clientId);

    /**
     * Request Toggle RTT Audio Indication
     * RIL_REQUEST_TOGGLE_RTT_AUDIO_INDICATION
     *
     * @param serial Serial number of request
     * @param callId Call Id
     * @param audio 0: Audio off, 1:Audio on
     */
    void toggleRttAudioIndication(in int serial, in int callId, in int audio, in int clientId);
// M: IMS_RTT end @}

// M: IMS_BASE start @{
    /**
     * Query Voice over PS status
     *
     * @param serial Serial number of request.
     *
     * Response function is IImsRadioResponse.queryVopsStatusResponse
     */
    void queryVopsStatus(in int serial, in int clientId);

    /**
     * Set IMS Registration Report
     *
     * @param serial Serail Number
     */
    void setImsRegistrationReport(in int serial, in int clientId);
// M: IMS_BASE end @}

// M: IMS_SS start @{
    /**
     * Cancel Pending USSI
     *
     * @param serial Serail Number
     */
    void cancelUssi(in int serial, in int clientId);

    /**
     * Get xcap status
     *
     * @param serial Serail Number
     */
    void getXcapStatus(in int serial, in int clientId);

    /**
     * Send USSI
     *
     * @param serial Serail Number
     * @param ussiString USSI String
     */
    void sendUssi(in int serial, in String ussiString, in int clientId);

    /**
     * Setup customized XCAP user agent string
     *
     * @param serial Serail Number
     * @param serial Customized XCAP user agent string
     */
    void setupXcapUserAgentString(in int serial, in String userAgent, in int clientId);

    /**
     * Get call barring specific users
     *
     * @param serial Serail Number
     * @param serviceClass Service class
     */
    void getBarringCalls(in int serial, in int serviceClass, in int clientId);

    /**
     * Set call barring specific users
     *
     * @param serial Serail Number
     * @param calls Barring specific call
     */
    void setBarringCalls(in int serial, in ImsBarringCall[] calls, in int clientId);
// M: IMS_SS end @}

// M: IMS_ViLTE start @{
    /**
     * Accept a video call
     *
     * @param serial Serail Number
     * @param videoMode Video Mode
     * @param callId Call ID
     */
    void videoCallAccept(in int serial, in int videoMode, in int callId, in int clientId);

    /**
     * send video ringtone event to modem.
     *
     * @param serial Serial number of request.
     * @param info video ringtone event
     *
     * Response function is IImsRadioResponse.videoRingtoneEventResponse()
     *
     */
    void videoRingtoneEventRequest(in int serial, in String[] event, in int clientId);

    /**
     * VT Dial
     * RIL Request: RIL_REQUEST_VT_DIAL
     *
     * @param serial Serail Number
     * @param dialInfo DialInfo
     */
    void vtDial(in int serial, in android.hardware.radio.voice.Dial dialInfo, in int clientId);

    /**
     * VT Dial with SIP URI
     *
     * @param serial Serail Number
     * @param address Address
     */
    void vtDialWithSipUri(in int serial, in String address, in int clientId);
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
    /**
     * Conference Dial
     *
     * @param serial Serail Number
     * @param dailInfo Conference Dial Information
     */
    void conferenceDial(in int serial, in ConferenceDial dailInfo, in int clientId);

    /**
     * Control a call
     *
     * @param serial Serail Number
     * @param controlType 0: Hold call, 1: Resume call
     * @param callId Call ID
     */
    void controlCall(in int serial, in int controlType, in int callId, in int clientId);

    /**
     * Control Conference Member
     *
     * @param serial Serail Number
     * @param controlType 0: Remove, 1: Add
     * @param confCallId Conference call ID
     * @param address Address
     * @param callToAdd Call ID to be controlled
     */
    void controlImsConferenceCallMember(in int serial, in int controlType,
        in int confCallId, in String address, in int callId, in int clientId);

    /**
     * Dial with SIP URI
     *
     * @param serial Serail Number
     * @param address Address
     */
    void dialWithSipUri(in int serial, in String address, in int clientId);

    /**
     * Approve ECC redial or not
     *
     * @param serial Serial number of request
     * @param approve 0: disapprove, 1: approve
     * @param callId Call ID
     */
    void eccRedialApprove(in int serial, in int approve, in int callId, in int clientId);

    /**
     * @param serial Serail Number
     * @param callId Call ID
     */
    void forceReleaseCall(in int serial, in int callId, in int clientId);

    /**
     * IMS ECT Command
     *
     * @param serial Serail Number
     * @param number Number
     * @param type Type
     */
    void imsEctCommand(in int serial, in String number, in int type, in int clientId);

    /**
     * Request Pull Call
     *
     * @param serial Serail Number
     * @param target Target
     * @param isVideoCall isVideoCall
     */
    void pullCall(in int serial, in String target, in boolean isVideoCall, in int clientId);

    /**
     * Set additional call information to modem.
     *
     * @param serial Serial number of request.
     * @param info additional call information
     *
     * Response function is IImsRadioResponse.setCallAdditionalInfoResponse()
     *
     */
    void setCallAdditionalInfo(in int serial, in String[] info, in int clientId);

    /**
     * Set IMS call mode
     *
     * @param serial Serail Number
     * @param mode 1: original mode, 2: client API mode
     */
    void setImsCallMode(in int serial, in int mode, in int clientId);

    /**
     * Set Sip Header
     *
     * @param serial Serail Number
     * @param data data pass as strings
     */
    void setSipHeader(in int serial, in String[] data, in int clientId);

    /**
     * Set Sip Header report
     *
     * @param serial Serail Number
     * @param data data pass as strings
     */
    void setSipHeaderReport(in int serial, in String[] data, in int clientId);
// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
    /**
     * Get IMS feature value for enabling or disabling feature
     *
     * @param serial Serial number of socket command
     * @param featureId Feature ID
     * @param network Network type
     */
    void getImsCfgFeatureValue(in int serial, in int featureId, in int network, in int clientId);

    /**
     * Get IMS Config provision value
     *
     * @param serial Serial number of socket command
     * @param configId Provision Id
     */
    void getImsCfgProvisionValue(in int serial, in int configId, in int clientId);

    /**
     * Get IMS Config Resource Value
     *
     * @param serial Serial number of socket command
     * @param featureId Feature Id
     */
    void getImsCfgResourceCapValue(in int serial, in int featureId, in int clientId);

    /**
     * Request get voice domain preference from modem.
     *
     * @param serial Serial number of request.
     *
     * Response function is IImsRadioResponse.setVoiceDomainPreferenceResponse()
     *
     */
    void getVoiceDomainPreference(in int serial, in int clientId);

    /**
     * Set IMS feature value for enabling or disabling feature
     *
     * @param serial Serial number of socket command
     * @param featureId Feature ID
     * @param network Network type
     * @param value Enable(1) or disable(0)
     * @param isLast is last command of set multiple feature values,
     *        ISLAST_NULL(-1), ISLAST_FALSE(0) or ISLAST_TRUE(1) that defined in ImsConfigAdapter
     *
     * Response function is IMtkRadioExResponse.setImsCfgFeatureValueResponse
     */
    void setImsCfgFeatureValue(in int serial, in int featureId, in int network,
        in int value, in int isLast, in int clientId);

    /**
     * Set IMS Config provision value
     *
     * @param serial Serial number of socket command
     * @param configId Provision Id
     * @param value Provision Value
     */
    void setImsCfgProvisionValue(in int serial, in int configId, in String value, in int clientId);

    /**
     * Turn On/Off IMS Features
     *
     * @param serial Serail Number
     * @param volteEnable Is VoLTE turn ON/OFF
     * @param vilteEnable Is ViLTE turn ON/OFF
     * @param vowifiEnable Is VoWifi turn ON/OFF
     * @param viwifiEnable ViWifi VoLTE turn ON/OFF
     * @param smsEnable Is SMS turn ON/OFF
     * @param eimsEnable Is EIMS turn ON/OFF
     */
    void setImscfg(in int serial, in boolean volteEnable, in boolean vilteEnable,
        in boolean vowifiEnable, in boolean viwifiEnable, in boolean smsEnable,
        in boolean eimsEnable, in int clientId);

    /**
     * Request set voice domain preference to modem.
     *
     * @param vdp Voice domain preference
     *
     */
    void setVoiceDomainPreference(in int serial, in int vdp, in int clientId);

    /**
     * Set WFC Profile Information
     * RIL_REQUEST_SET_WFC_PROFILE
     *
     * @param serial Serail Number
     * @param wfcPreference WFC Perference
     */
    void setWfcProfile(in int serial, in int wfcPreference, in int clientId);

    /**
     * To set multiple IMS configurations to modem in a RIL request
     * RIL Request: RIL_REQUEST_SET_MD_IMSCFG
     *
     * @param serial Serail Number
     * @param keys Defined in ImsConfig.ConfigConstants, entries divided by ",".
     * @param values The values for each keys, divided by ",".
     * @param type The type of the config, maps to AT+ECFGSET, AT+EWOCFGSET and AT+EIWLCFGSET AT commands.
     *
     * Response callback is IMtkRadioExImsResponse.setModemImsCfgResponse()
     */
    void setModemImsCfg(in int serial, in String keys, in String values, in int type, in int clientId);
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
    /**
     * Query SSAC status from MD.
     *
     * @param serial Serial number of socket command
     */
    void querySsacStatus(in int serial, in int clientId);

    /**
     * Query WFC related config setting from MD.
     *
     * @param serial Serial number of socket command
     * @param setting index for WFC setting
     */
    void getWfcConfig(in int serial, in int setting, in int clientId);

    /**
     * Notify ePDG the screen state
     * @param serial Serial number of socket command
     * @param state Screen state.
     */
    void notifyEPDGScreenState(in int serial, in int state, in int clientId);

    /**
     * Notify Emergency address ID to MD.
     *
     * @param serial Serial number of socket command
     * @param phoneId SIM index
     * @param aid Address ID for emergency
     */
    void setEmergencyAddressId(in int serial, in String aid, in int clientId);

    /**
     * Notify location information and UE Wi-FI interface MAC to MD.
     *
     * @param serial Serial number of socket command
     * @param data Data set
     *        which contains:
     *        accountId Account ID
     *        broadcastFlag Broadcast Flag
     *        latitude Latitude value
     *        longitude Longitude value
     *        accuracy Accuracy
     *        method Positioning method
     *        city City
     *        state State
     *        zip Zip code
     *        countryCode Two-letter country code defined by ISO 3166-1
     *        ueWlanMac UE Wi-FI interface MAC
     *        confidence Confidence level
     *        altitude Altitude
     *        majorAxisAccuracy Major axis accuracy
     *        minorAxisAccuracy Minor axis accuracy
     *        vericalAxisAccuracy Verical axis accuracy
     */
    void setLocationInfo(in int serial, in String[] data, in int clientId);

    /**
     * Notify Wi-Fi Keepalive Status to MD.
     *
     * @param serial Serial number of socket command
     * @param phoneId SIM index
     * @param ifName Wi-Fi interface name
     * @param enable enabled or not
     * @param srcIp source IP
     * @param srcPort source port
     * @param dstIp destination IP
     * @param dstPort destination port
     */
    void setNattKeepAliveStatus(in int serial, in String ifName, in boolean enable,
        in String srcIp, in int srcPort, in String dstIp, in int dstPort, in int clientId);

    /**
     * Notify WFC related config setting to MD.
     *
     * @param serial Serial number of socket command
     * @param setting index for WFC setting
     * @param ifName Wi-Fi interface name
     * @param value for WFC setting
     */
    void setWfcConfig(in int serial, in int setting, in String ifName, in String value, in int clientId);

    /**
     * Notify Wi-Fi association information to MD.
     *
     * @param serial Serial number of socket command
     * @param data String data set
     *        which contains:
     *        ifName Wi-Fi interface name
     * 		  associated Wi-Fi association status
     *        ssid Wi-Fi AP SSID
     *        apMac Wi-Fi AP MAC address
     *        mtuSize mtu size
     *        ueMac UE MAC address
     *        wifiConnectState Wi-Fi connect state
     */
    void setWifiAssociated(in int serial, in String[] data, in int clientId);

    /**
     * Notify Wi-Fi and flight mode enable/disable status to MD.
     *
     * @param serial Serial number of socket command
     * @param phoneId SIM index
     * @param ifName Wi-Fi interface name
     * @param isEnabled Wi-Fi enable/disable status
     * @param isFlightModeOn Flight mode enable/disable status
     */
    void setWifiEnabled(in int serial, in String ifName, in int isWifiEnabled,
        in int isFlightModeOn, in int clientId);

    /**
     * Notify Wi-Fi IP address information to MD.
     *
     * @param serial Serial number of socket command
     * @param data String data set
     *        which contains:
     *  	  ifName Wi-Fi interface name
     *        ipv4Addr IPV4 address
     *        ipv6Addr IPV6 address
     *        ipv4PrefixLen IPV4 prefix length
     *        ipv6PrefixLen IPV6 prefix length
     *        ipv4Gateway IPV4 gateway address
     *        ipv6Gateway IPV6 gateway address
     *        dnsCount DNS server count
     *        dnsServers DNS server addresses string
     */
    void setWifiIpAddress(in int serial, in String[] data, in int clientId);

    /**
     * Notify DNS ping-on-wifi result to MD.
     *
     * @param serial Serial number of socket command
     * @param phoneId SIM index
     * @param rat radio bearer ie. cellular/wifi
     * @param latency ping packet latency
     * @param pktloss ping packet loss
     */
    void setWifiPingResult(in int serial, in int rat, in int latency, in int pktloss, in int clientId);

    /**
     * Notify Wi-Fi signal infromation to MD.
     *
     * @param serial Serial number of socket command
     * @param phoneId SIM index
     * @param rssi Wi-Fi RSSI strength
     * @param snr Wi-Fi SNR value
     */
    void setWifiSignalLevel(in int serial, in int rssi, in int snr, in int clientId);
// M: IMS_WFC end @}

    /**
     * When response type received from a radio indication or radio response is
     * RadioIndicationType:UNSOLICITED_ACK_EXP or RadioResponseType:SOLICITED_ACK_EXP respectively,
     * acknowledge the receipt of those messages by sending responseAcknowledgement().
     *
     */
    void responseAcknowledgementMtk();

    /**
     * Set response functions for mtk radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMtk(in IMtkRadioExImsResponse radioResponse,
        in IMtkRadioExImsIndication radioIndication);

   /**
     * Set response functions for MWI radio requests & radio indications.
     *
     * @param radioResponse Object containing response functions
     * @param radioIndication Object containing radio indications
     */
    void setResponseFunctionsMwi(in IMwiRadioResponse radioResponse,
		in IMwiRadioIndication radioIndication);

    /**
     * Get On/Off IMS Features
     *
     * @param serial Serail Number
     */
    void getImscfg(in int serial, in int clientId);

}
