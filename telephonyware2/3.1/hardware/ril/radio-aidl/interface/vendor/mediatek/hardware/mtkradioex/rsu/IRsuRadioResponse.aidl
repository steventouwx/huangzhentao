// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.rsu;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.rsu.RsuResponseInfo;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IRsuRadioResponse {
    /**
     * @param info Response info struct containing response type, serial no. and error
     * @param data data returned
     */
    oneway void sendRsuResponse(in android.hardware.radio.RadioResponseInfo info,
        in RsuResponseInfo rri);
}
