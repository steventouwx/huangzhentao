// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.assist;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface IAssistModemResponse {
    /**
     * Response for 'RIL_REQUEST_SYNC_APP_EVENT_STATUS'.
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *     RadioError:RIL_E_SUCCESS
     */
    oneway void syncAppEventStatusResponse(in android.hardware.radio.RadioResponseInfo info);
}
