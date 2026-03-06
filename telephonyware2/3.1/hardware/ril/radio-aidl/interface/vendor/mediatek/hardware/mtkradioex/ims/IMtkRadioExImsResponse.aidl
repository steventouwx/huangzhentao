// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ims;

import android.hardware.radio.RadioResponseInfo;
import android.hardware.radio.messaging.SendSmsResult;
import vendor.mediatek.hardware.mtkradioex.ims.ImsBarringCall;
import vendor.mediatek.hardware.mtkradioex.voice.CallForwardInfoEx;

/**
 * Interface declaring response functions to IMS requests.
 * Response functions defined in this interface are as per following convention:
 * <xyz>Response is response to IMtkRadioExIms.<xyz>
 */
@VintfStability
interface IMtkRadioExImsResponse {

// M: IMS_DATA start @{
    /**
     * Response for 'IMS Bearer State Confirm Activiation/Deactivation'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void imsBearerStateConfirmResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Response message of 'set Ims Bearer Notificatioin'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setImsBearerNotificationResponse(
        in android.hardware.radio.RadioResponseInfo info);
// M: IMS_DATA end @}

// M: IMS_SMS start @{
    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INTERNAL_ERR
     *   RadioError:NO_MEMORY
     *   RadioError:NO_RESOURCES
     *   RadioError:CANCELLED
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void acknowledgeLastIncomingGsmSmsExResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:NO_SMS_TO_ACK
     *   RadioError:INVALID_STATE
     *   RadioError:NO_MEMORY
     *   RadioError:REQUEST_RATE_LIMITED
     *   RadioError:SYSTEM_ERR
     *   RadioError:MODEM_ERR
     *   RadioError:NETWORK_NOT_READY
     *   RadioError:INVALID_MODEM_STATE
     *   RadioError:INTERNAL_ERR
     *   RadioError:REQUEST_NOT_SUPPORTED
     *   RadioError:OPERATION_NOT_ALLOWED
     *   RadioError:NO_RESOURCES
     *   RadioError:CANCELLED
     */
    oneway void acknowledgeLastIncomingCdmaSmsExResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param sms Response to sms sent as defined by SendSmsResult in types.hal
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SMS_SEND_FAIL_RETRY
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:NETWORK_REJECT
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:INVALID_STATE
     *   RadioError:NO_MEMORY
     *   RadioError:INVALID_SMS_FORMAT
     *   RadioError:SYSTEM_ERR
     *   RadioError:REQUEST_RATE_LIMITED
     *   RadioError:MODEM_ERR
     *   RadioError:NETWORK_ERR
     *   RadioError:ENCODING_ERR
     *   RadioError:OPERATION_NOT_ALLOWED
     *   RadioError:INTERNAL_ERR
     *   RadioError:REQUEST_NOT_SUPPORTED
     *   RadioError:NETWORK_NOT_READY
     *   RadioError:NO_RESOURCES
     *   RadioError:CANCELLED
     */
    oneway void sendImsSmsExResponse(in android.hardware.radio.RadioResponseInfo info,
        in android.hardware.radio.messaging.SendSmsResult sms);
// M: IMS_SMS end @}

// M: IMS_RTT start @{
    /**
     * The response of setRttMode
     * Command: RIL_REQUEST_SET_RTT_MODE
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setRttModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of sendRttModifyRequest
     * Command: RIL_REQUEST_SEND_RTT_MODIFY_REQUEST
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void sendRttModifyRequestResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of sendRttText
     * Command: RIL_REQUEST_SEND_RTT_TEXT
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void sendRttTextResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of rttModifyRequestResponse
     * Command: RIL_REQUEST_RTT_MODIFY_REQUEST
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void rttModifyRequestResponseResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of toggleRttAudioIndicationResponse
     * Command: RIL_REQUEST_TOGGLE_RTT_AUDIO_INDICATION
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void toggleRttAudioIndicationResponse(
        in android.hardware.radio.RadioResponseInfo info);
// M: IMS_RTT end @}

// M: IMS_BASE start @{
    /**
     * Response for 'setImsRegistrationReport'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setImsRegistrationReportResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * The result of querying Voice over PS status
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param vops The result of querying Voice over PS status
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void queryVopsStatusResponse(in android.hardware.radio.RadioResponseInfo info,
        in int vops);
// M: IMS_BASE end @}

// M: IMS_SS start @{
    /**
     * Response for 'Send USSI'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void sendUssiResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Cancel Pending USSI'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void cancelUssiResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of Get Xcap status
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:SYSTEM_ERR
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getXcapStatusResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * Response for "Setup customized XCAP user agent string".
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void setupXcapUserAgentStringResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for "Get call barring specific calls".
     * @param info Response info struct containing response type, serial no. and error
     * @param calls ImsBarringCall
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void getBarringCallsResponse(in android.hardware.radio.RadioResponseInfo info,
        in ImsBarringCall[] calls);

    /**
     * Response for "Set call barring specific calls".
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:GENERIC_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void setBarringCallsResponse(in android.hardware.radio.RadioResponseInfo info);
// M: IMS_SS end @}

// M: IMS_ViLTE start @{

    /**
     * Response for 'VT Dial'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void vtDialResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'VT Dial with SIP URI'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void vtDialWithSipUriResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Accept a video call'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void videoCallAcceptResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of imsVideoRingtoneEventRequest
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void videoRingtoneEventResponse(in android.hardware.radio.RadioResponseInfo info);
// M: IMS_ViLTE end @}

// M: IMS_CC start @{
    /**
     * Response for 'Dial with SIP URI'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void dialWithSipUriResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'IMS ECT Command'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void imsEctCommandResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Pull Call'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void pullCallResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for eccRedialApprove
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void eccRedialApproveResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Conference Dial'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void conferenceDialResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Force Hangup'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void forceReleaseCallResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Control a call'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void controlCallResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Control Conference Member'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void controlImsConferenceCallMemberResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of Set Sip Header
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setSipHeaderResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of Set Sip Header Report
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setSipHeaderReportResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of Set IMS call mode
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setImsCallModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of setCallAdditionalInfo
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setCallAdditionalInfoResponse(in android.hardware.radio.RadioResponseInfo info);
// M: IMS_CC end @}

// M: IMS_CONFIG  start @{
    /**
     * Response for 'Turn On/Off IMS Features'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setImscfgResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * MTK SS Feature : End
     *
     *
     * Response of IMtkRadioExIms.setModemImsCfg()
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param results The result (integers) for each configurations (0: success, -1: fail)
     *
     */
    oneway void setModemImsCfgResponse(in android.hardware.radio.RadioResponseInfo info,
        in String results);

    /**
     * Response of IMtkRadioExIms.setImsCfgFeatureValue()
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param results The result (integers) for each configurations (0: success, -1: fail)
     *
     */
    oneway void setImsCfgFeatureValueResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of IMtkRadioExIms.getImsCfgFeatureValue()
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param results The result (integers) for each configurations (0: success, -1: fail)
     *
     */
    oneway void getImsCfgFeatureValueResponse(in android.hardware.radio.RadioResponseInfo info,
        in int value);

    /**
     * Response of Set IMS Config provision value
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setImsCfgProvisionValueResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response of Get IMS Config provision value
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getImsCfgProvisionValueResponse(in android.hardware.radio.RadioResponseInfo info,
        in String value);

    /**
     * Response if Get IMS Config Resource Value
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getImsCfgResourceCapValueResponse(in android.hardware.radio.RadioResponseInfo info,
        in int value);

    /**
     * Response for 'Set WFC Profile Information'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setWfcProfileResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'Set IMS Voice domain preference'
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:GENERIC_FAILURE
     */
    oneway void setVoiceDomainPreferenceResponse(
        in android.hardware.radio.RadioResponseInfo info);

    /**
     * The response of getVoiceDomainPreference
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param vdp Voice domain preference
     */
    oneway void getVoiceDomainPreferenceResponse(in android.hardware.radio.RadioResponseInfo info,
        in int vdp);
// M: IMS_CONFIG end @}

// M: IMS_WFC start @{
    /**
     * The response of query SSAC
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
    oneway void querySsacStatusResponse(in android.hardware.radio.RadioResponseInfo info,
        in int[] status);
// M: IMS_WFC end @}

    /**
     * Response for IMtkRadioExIms.getModemImsCfg()
     * @param info Response info struct containing response type, serial no. and error
     * @param results The result (integers) for each configurations
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void getImscfgResponse(in android.hardware.radio.RadioResponseInfo info, in String results);
}
