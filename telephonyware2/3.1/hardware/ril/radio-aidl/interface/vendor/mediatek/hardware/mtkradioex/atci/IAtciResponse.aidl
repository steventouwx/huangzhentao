// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.atci;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to ATCI requests.
 */
@VintfStability
interface IAtciResponse {
    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param data data returned
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:RADIO_NOT_AVAILABLE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OEM_ERROR_X
     */
    oneway void sendAtciResponse(in android.hardware.radio.RadioResponseInfo info,
        in byte[] data);
}
