// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.em;

import android.hardware.radio.RadioIndicationType;

/*
 * Interface declaring response functions to solicited radio requests.
 */
@VintfStability
interface IEmRadioIndication {
    /**
     * Indicates of network info
     *
     * @param type Type of radio indication
     * @param networkinfo network info
     */
    oneway void networkInfoInd(in android.hardware.radio.RadioIndicationType type,
        in String[] networkinfo);

    /**
     * This is for OEM specific use.
     * It's used to send unsolicited AT command from modem to framework for internal testing.
     *
     * @param type Type of radio indication
     * @param data data passed as raw bytes
     */
    oneway void oemHookRaw(in android.hardware.radio.RadioIndicationType type, in byte[] data);

    /**
     * Notify TX power status
     *
     * @param type Type of radio indication
     * @param indPower indicates TX power information
     */
    oneway void onTxPowerIndication(in android.hardware.radio.RadioIndicationType type,
        in int[] indPower);

    /**
     * Indicates when radio state changes.
     *
     * @param type Type of radio indication
     * @param radioState Current radio state
     */
    oneway void radioStateChanged(in android.hardware.radio.RadioIndicationType type, in android.hardware.radio.modem.RadioState radioState);
}
