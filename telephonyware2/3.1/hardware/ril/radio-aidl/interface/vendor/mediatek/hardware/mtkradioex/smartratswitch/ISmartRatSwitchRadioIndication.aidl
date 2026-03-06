// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.smartratswitch;

import android.hardware.radio.RadioIndicationType;

/**
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface ISmartRatSwitchRadioIndication {
    /**
     * Indicates of codec active information
     *
     * @param instanceId codec instance id
     * @use true: codec init, false: codec de-init
     */
    oneway void codecActiveInd(in android.hardware.radio.RadioIndicationType type,
        in int instanceId, in boolean use);

    /**
     * Indicates of buffer empty information
     *
     * @param instanceId: codec instance id
     * @param empty: true: buffer empty, false: buffer non-empty
     */
    oneway void codecEmptyInd(in android.hardware.radio.RadioIndicationType type,
        in int instanceId, in boolean empty);

    /**
     * Indicates of codec fps information
     *
     * @param instanceId: codec instance id
     * @param fps: fps
     */
    oneway void codecFpsInd(in android.hardware.radio.RadioIndicationType type, in int instanceId,
        in int fps);

    /**
     * Indicates of codec resolution information
     *
     * @param instanceId: codec instance id
     * @param w: wide
     * @param h: height
     */
    oneway void codecResolutionInd(in android.hardware.radio.RadioIndicationType type,
        in int instanceId, in int w, in int h);

    /**
     * Indicates of smart rat switch information
     *
     * @param info Information AT command information
     */
    oneway void smartRatSwitchInd(in android.hardware.radio.RadioIndicationType type,
        in int info);
}
