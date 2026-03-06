// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.em;

import android.hardware.radio.RadioResponseInfo;

/*
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface IEmRadioResponse {
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
}
