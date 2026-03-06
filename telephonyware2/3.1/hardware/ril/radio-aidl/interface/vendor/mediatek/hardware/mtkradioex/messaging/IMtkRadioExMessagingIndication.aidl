// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.messaging;

/**
 * Interface declaring unsolicited indications to subsidylock.
 */
@VintfStability
interface IMtkRadioExMessagingIndication {
    /**
     * Report ESN/MEID changes when modem changes the value in NVRAM.
     *
     * @param type Type of radio indication
     * @param esnMeid ESN or MEID string, the format is "old value, new value"
     */
    oneway void esnMeidChangeInd(in android.hardware.radio.RadioIndicationType type,
        in String esnMeid);

    /**
     * Indicate ME SMS storage full
     * The API should be called while receiving the URC which means
     * device storage is full from modem
     *
     * @param type Type of radio indication
     */
    oneway void meSmsStorageFullInd(in android.hardware.radio.RadioIndicationType type);

    /**
     * Sms Extent Information
     *
     * @param type Type of radio indication
     * @param info sms information
     */
    oneway void smsInfoExtInd(in android.hardware.radio.RadioIndicationType type, in String info);

    /**
     * Indicate the module of SMS in modem is ready
     *
     * @param type Type of radio indication
     */
    oneway void smsReadyInd(in android.hardware.radio.RadioIndicationType type);

    /**
     * Indicate when SCBM ends.
     *
     * @param type Type of radio indication
     */
    oneway void exitSCBMInd(in android.hardware.radio.RadioIndicationType type);

    /**
     * Indicate when SCBM starts.
     *
     * @param type Type of radio indication
     */
    oneway void enterSCBMInd(in android.hardware.radio.RadioIndicationType type);
}
