// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

// import android.hardware.radio.CallForwardInfoStatus;

/**
 * Structure for RIL_REQUEST_SET_CALL_FORWARD_IN_TIME_SLOT
 */
@VintfStability
parcelable CallForwardInfoEx {
    int status;
    int reason;
    int serviceClass;
    int toa;
    String number;
    int timeSeconds;
    String timeSlotBegin;
    String timeSlotEnd;
}

