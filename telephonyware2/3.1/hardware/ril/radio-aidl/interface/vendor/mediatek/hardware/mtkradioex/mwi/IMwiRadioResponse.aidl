// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.mwi;

import android.hardware.radio.RadioResponseInfo;

/*
 * Interface declaring response functions to solicited radio requests.
 * Response functions defined in this interface are as per following convention:
 * <xyz>Response is response to IRadio.<xyz>
 */
@VintfStability
interface IMwiRadioResponse {
    /**
     * Response for getWfcConfig request.
     *
     * @param responseInfo struct containing response type, serial no, and error.
     * @param state The config state.
     */
    oneway void getWfcConfigResponse(in android.hardware.radio.RadioResponseInfo responseInfo,
        in int state);

    /**
     * The response of notifyEPDGScreenState
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void notifyEPDGScreenStateResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setEmergencyAddressId request.
     */
    oneway void setEmergencyAddressIdResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for setLocationInfo request.
     */
    oneway void setLocationInfoResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for setNattKeepAliveStatus request.
     */
    oneway void setNattKeepAliveStatusResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for setWfcConfig request.
     */
    oneway void setWfcConfigResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for setWifiAssociated request.
     */
    oneway void setWifiAssociatedResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for setWifiEnabled request.
     */
    oneway void setWifiEnabledResponse(in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * Response for setWifiIpAddress request.
     */
    oneway void setWifiIpAddressResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);

    /**
     * The response of setWifiPingResult
     *
     * @param info Response info struct containing response type, serial no. and error.
     */
    oneway void setWifiPingResultResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for setWifiSignalLevel request.
     */
    oneway void setWifiSignalLevelResponse(
        in android.hardware.radio.RadioResponseInfo responseInfo);
}
