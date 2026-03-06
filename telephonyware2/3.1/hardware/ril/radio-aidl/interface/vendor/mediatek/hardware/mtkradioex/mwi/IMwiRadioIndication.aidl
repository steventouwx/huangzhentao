// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.mwi;

import android.hardware.radio.RadioIndicationType;

/*
 * Interface declaring Digit unsolicited radio indications.
 */
@VintfStability
interface IMwiRadioIndication {
    /**
     * Indicates the LocationRequest
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onLocationRequest(in android.hardware.radio.RadioIndicationType type,
        in String[] indStgs);

    /**
     * Indicates the NattKeepAliveChanged
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onNattKeepAliveChanged(in android.hardware.radio.RadioIndicationType type,
        in String[] indStgs);

    /**
     * Indicates the PdnHandover
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onPdnHandover(in android.hardware.radio.RadioIndicationType type,
        in int[] indStgs);

    /**
     * Indicates the WfcPdnError
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWfcPdnError(in android.hardware.radio.RadioIndicationType type,
        in int[] indStgs);

    /**
     * Indicates the WfcPdnStateChanged
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWfcPdnStateChanged(in android.hardware.radio.RadioIndicationType type,
        in int[] indStgs);

    /**
     * Indicates the onWifiLock
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWifiLock(in android.hardware.radio.RadioIndicationType type,
        in String[] indStgs);

    /**
     * Indicates the WifiMonitoringThreshouldChanged
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWifiMonitoringThreshouldChanged(
        in android.hardware.radio.RadioIndicationType type, in int[] indStgs);

    /**
     * Indicates the WifiPdnActivate
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWifiPdnActivate(in android.hardware.radio.RadioIndicationType type,
        in int[] indStgs);

    /**
     * Indicates the onWifiPdnOOS
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWifiPdnOOS(in android.hardware.radio.RadioIndicationType type,
        in String[] indStgs);

    /**
     * Indicates the WifiPingReques
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWifiPingRequest(in android.hardware.radio.RadioIndicationType type,
        in int[] indStgs);

    /**
     * Indicates the WifiRoveout
     *
     * @param type Type of radio indication
     * @param indStgs detail info
     */
    oneway void onWifiRoveout(in android.hardware.radio.RadioIndicationType type,
        in String[] indStgs);
}
