// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.rsu;

import android.hardware.radio.RadioIndicationType;

/**
 * Interface declaring unsolicited indications to rsu.
 */
@VintfStability
interface IRsuRadioIndication {
    /**
     * Notification for Remote SIM Unlock event
     * @param type Type of radio indication
     * @param eventId event id
     * @param eventString event string
     */
    oneway void onRsuEvent(in android.hardware.radio.RadioIndicationType type, in int eventId,
        in String eventString);
}
