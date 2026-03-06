// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.se;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to solicited radio requests.
 * Response functions defined in this interface are as per following convention:
 * <xyz>Response is response to ISecureElement.<xyz>
 */
@VintfStability
interface ISERadioResponse {
    /**
     * The response of getATRResponse
     *
     * @param info Response info struct containing response type, serial no. and error.
     * @param response Response string of getATRResponse.
     */
    oneway void getATRResponse(in android.hardware.radio.RadioResponseInfo info,
        in String response);
}
