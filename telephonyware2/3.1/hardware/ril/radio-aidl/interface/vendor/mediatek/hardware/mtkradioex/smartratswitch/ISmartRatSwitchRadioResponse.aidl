// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.smartratswitch;

import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface ISmartRatSwitchRadioResponse {
    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void getSmartRatSwitchResponse(in android.hardware.radio.RadioResponseInfo info,
        in int state);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void setSmartSceneSwitchResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * @param info Response info struct containing response type, serial no. and error
     *
     * Valid errors returned:
     *   RadioError:NONE
     *   RadioError:INVALID_ARGUMENTS
     *   RadioError:OPERATION_NOT_ALLOWED
     */
    oneway void smartRatSwitchResponse(in android.hardware.radio.RadioResponseInfo info);
}
