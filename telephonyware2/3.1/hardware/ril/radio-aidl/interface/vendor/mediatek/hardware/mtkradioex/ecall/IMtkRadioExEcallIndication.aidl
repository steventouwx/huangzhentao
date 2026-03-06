// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ecall;

import android.hardware.radio.RadioResponseInfo;
import vendor.mediatek.hardware.mtkradioex.ecall.EcallMsg;

/**
 * Interface declaring response functions to subsidylock requests.
 */
@VintfStability
interface IMtkRadioExEcallIndication {
    /**
     * Indicates when ecall state changes.
     *
     * @param type Type of radio indication
     * @param Info ecall indication type
     * @param Info call id
     */
    oneway void ecallIndicationInd(in android.hardware.radio.RadioIndicationType type,
        in EcallMsg ecallMsg);
}
