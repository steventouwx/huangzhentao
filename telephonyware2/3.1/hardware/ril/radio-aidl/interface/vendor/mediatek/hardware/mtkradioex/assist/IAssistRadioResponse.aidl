// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.assist;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface IAssistRadioResponse {
    /**
     * Response for 'RIL_REQUEST_SET_FD_MODE'.
     * @param info Response info contains error if have.
     *
     * Valid errors returned:
     *     RadioError:RIL_E_SUCCESS
     */
    oneway void setFdModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Response for 'RIL_REQUEST_SYNC_DATA_SETTINGS_TO_MODE'.
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_GENERIC_FAILURE
     */
    oneway void syncDataSettingsToMdResponse(in android.hardware.radio.RadioResponseInfo info);
}
