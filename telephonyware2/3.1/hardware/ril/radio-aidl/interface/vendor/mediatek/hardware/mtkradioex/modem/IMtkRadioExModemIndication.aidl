// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.modem;
import vendor.mediatek.hardware.mtkradioex.modem.DsbpState;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExModemIndication {

    /**
     * Indicates dsbp state
     * @param indicationType RadioIndicationType
     * @param dsbpState vendor.mediatek.hardware.radio.V3_0.DsbpState
     */
    oneway void dsbpStateChanged(in android.hardware.radio.RadioIndicationType indicationType,
        in DsbpState dsbpState);

    /**
     * Indicates of eMBMS AT command event
     *
     * @param info Information AT command string
     */
    oneway void eMBMSAtInfoIndication(in android.hardware.radio.RadioIndicationType type,
        in String info);

    /**
     * Indicates of eMBMS session activate status
     *
     * @param status Activated session:1, else 0
     */
    oneway void eMBMSSessionStatusIndication(in android.hardware.radio.RadioIndicationType type,
        in int status);

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
     * Notify TX power status without power info, only event(high or low Tx power)
     *
     * @param type Type of radio indication
     * @param indPower indicates TX power status information
     */
    oneway void onTxPowerStatusIndication(in android.hardware.radio.RadioIndicationType type,
        in int[] indPower);

    /**
     * Indicates when modem trigger world mode.
     *
     * @param type Type of radio indication
     */
    oneway void worldModeChangedIndication(in android.hardware.radio.RadioIndicationType type,
        in int[] modes);

    /**
     * Indicates the onCellularQualityChangedInd
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onCellularQualityChangedInd(in android.hardware.radio.RadioIndicationType type,
        in int[] indStgs);
}
