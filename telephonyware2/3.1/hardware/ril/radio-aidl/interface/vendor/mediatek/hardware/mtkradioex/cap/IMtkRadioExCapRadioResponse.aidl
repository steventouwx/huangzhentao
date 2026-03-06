// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.cap;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface IMtkRadioExCapRadioResponse {
    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void abortCertificateResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void enableCapabilityResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param uid Caller process id
     * @param msg Authentication message
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void routeAuthMessageResponse(in android.hardware.radio.RadioResponseInfo info,
        in byte[] devId, in int capMask);

    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param rnd Random number
     * @param custId customer id
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void routeCertificateResponse(in android.hardware.radio.RadioResponseInfo info,
        in byte[] rnd, in int custId);
}
