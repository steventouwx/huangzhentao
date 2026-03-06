// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ecall;
import android.hardware.radio.RadioResponseInfo;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExEcallResponse {
    /**
     * Reset IVS response
     * Response of RIL request "RIL_REQUEST_ECALL_RESET_IVS"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void resetIvsResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Set MSD response
     * Response of RIL request "RIL_REQUEST_ECALL_SET_MSD"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setMSDResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Set test number response
     * Response of RIL request "RIL_REQUEST_ECALL_SET_TEST_NUM"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setTestNumResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Set reconf number response
     * Response of RIL request "RIL_REQUEST_ECALL_SET_RECONF_NUM"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setReconfNumResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Make fast ecall response
     * Response of RIL request "RIL_REQUEST_ECALL_FAST_MAKE_ECALL"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void makeFastEcallResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Set ecall priority response
     * Response of RIL request "RIL_REQUEST_ECALL_SET_PRI"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setEmsdPriResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Set T10 timer response
     * Response of RIL request "RIL_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setNadDeregTimeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Set ecall only mode response
     * Response of RIL request "RIL_REQUEST_ECALL_SET_OPRT_MODE"
     *
     * @param info Response info struct containing response type, serial no. and error
     */
    oneway void setOprtModeResponse(in android.hardware.radio.RadioResponseInfo info);

    /**
     * Get ecall only mode response
     * Response of RIL request "RIL_REQUEST_ECALL_GET_OPRT_MODE"
     *
     * @param info Response info struct containing response type, serial no. and error
     * @param results The result (integers) for each configurations (0: success, -1: fail)
     *
     */
    oneway void getOprtModeResponse(in android.hardware.radio.RadioResponseInfo info,
        in int mode);

}
