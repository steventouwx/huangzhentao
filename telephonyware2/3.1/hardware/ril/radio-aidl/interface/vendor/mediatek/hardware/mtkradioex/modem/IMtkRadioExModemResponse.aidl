// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.modem;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IMtkRadioExModemResponse {

    /**
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void getEngineeringModeInfoResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in String[] result);


    /**
     * Response for modifyModemType request.
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param applyType which is store/reload type
     */
    oneway void modifyModemTypeResponse(in android.hardware.radio.RadioResponseInfo info,
        in int applyType);

    /**
     * The response of restartRILD
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void restartRILDResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'runGbaAuthentication'
     * @param resList A response string list
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:SS_MODIFIED_TO_DIAL
     *   RadioError:SS_MODIFIED_TO_USSD
     *   RadioError:SS_MODIFIED_TO_SS
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:NO_MEMORY
     *   RadioError:SYSTEM_ERR
     *   RadioError:MODEM_ERR
     *   RadioError:INTERNAL_ERR
     *   RadioError:FDN_CHECK_FAILURE
     *   RadioError:REQUEST_NOT_SUPPORTED
     *   RadioError:SYSTEM_ERR
     */
    oneway void runGbaAuthenticationResponse(in android.hardware.radio.RadioResponseInfo info,
        in String[] resList);


    /**
     * The response of sendEmbmsAtCommand
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param data response string
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:GENERIC_FAILURE
     */
    oneway void sendEmbmsAtCommandResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo, in String data);

    /**
     * Response for sendRequestRaw.
     * It's for internal testing to get response of AT command.
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param data data returned by oem
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OEM_ERROR_X
     */
    oneway void sendRequestRawResponse(in android.hardware.radio.RadioResponseInfo info,
        in byte[] data);

    /**
     * Response for sendRequestStrings.
     * It's for internal testing to get response of AT command.
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param data data returned by oem
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OEM_ERROR_X
     */
    oneway void sendRequestStringsResponse(in android.hardware.radio.RadioResponseInfo info,
        in String[] data);

    /**
     * Response of RIL request "RIL_REQUEST_SEND_SAR_IND"
     *
     * @param responseInfo Response info struct containing response type, serial no. and error
     */
    oneway void sendSarIndicatorResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setModemPower request.
     */
    oneway void setModemPowerResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setTrm request.
     */
    oneway void setTrmResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * Returns RadioError:GENERIC_FAILURE for all other causes that might be fixed by retries.
     */
    oneway void setTxPowerResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for set TX power status.
     */
    oneway void setTxPowerStatusResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setVendorSetting.
     *
     * @param responseInfo Response info struct containing response type, serial no. and error
     */
    oneway void setVendorSettingResponse(in android.hardware.radio.RadioResponseInfo info);


    /**
     * Response for trigger mode switch request by ecc
     */
    oneway void triggerModeSwitchByEccResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for sendWifiAssociated request.
     */
    oneway void sendWifiAssociatedResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for sendWifiEnabled request.
     */
    oneway void sendWifiEnabledResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for sendWifiIpAddress request.
     */
    oneway void sendWifiIpAddressResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of registerCellQltyReport
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void registerCellQltyReportResponse(in android.hardware.radio.RadioResponseInfo info);
}
