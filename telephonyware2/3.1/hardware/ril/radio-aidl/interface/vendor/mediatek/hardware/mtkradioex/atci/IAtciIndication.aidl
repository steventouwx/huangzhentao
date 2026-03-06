// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.atci;

import android.hardware.radio.RadioIndicationType;

/**
 * Interface declaring unsolicited indications to ATCI.
 */
@VintfStability
interface IAtciIndication {
    /**
     * Send raw data to ATCI.
     *
     * @param type Type of radio indication
     * @param data data passed as raw bytes
     */
    oneway void atciInd(in android.hardware.radio.RadioIndicationType type, in byte[] data);
}
