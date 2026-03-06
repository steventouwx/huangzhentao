// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

import vendor.mediatek.hardware.mtkradioex.data.SetupDataCallResultSlice;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExDataResponse {
    /**
     * Response for 'RIL_REQUEST_DATA_CONNECTION_ATTACH'.
     *
     * @param info Response info contains error if have.
     */
   oneway void dataConnectionAttachResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'RIL_REQUEST_DATA_CONNECTION_DETACH'.
     *
     * @param info Response info contains error if have.
     */
    oneway void dataConnectionDetachResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'RIL_REQUEST_ENABLE_DSDA_INDICATION'.
     *
     * @param info Response info contains error if have.
     */
    oneway void enableDsdaIndicationResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for set ignore same phone number interval.
     * @param info Response info struct containing response type, serial no. and error
     * @param mode The DSDA mode
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:REQUEST_NOT_SUPPORTED
     */
   oneway void getDsdaStatusResponse(in android.hardware.radio.RadioResponseInfo info,
        in int mode);

    /**
     * Response for 'RIL_REQUEST_RESET_ALL_CONNECTIONS'.
     *
     * @param info Response info contains error if have.
     */
   oneway void resetAllConnectionsResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * M: Data Framework - Data Retry enhancement
     * The response of reset modem data retry count
     *
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
   oneway void resetMdDataRetryCountResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * customized version, return customized SetupDataCallResultSlice
     */
   oneway void setupDataCallResponseSlice(in android.hardware.radio.RadioResponseInfo info,
        in SetupDataCallResultSlice dcResponse);
}
