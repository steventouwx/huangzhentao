// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

// import android.hardware.radio.Clir;

@VintfStability
parcelable ConferenceDial {
    int clir; // android.hardware.radio.Clir
    boolean isVideoCall;
    String[] dialNumbers;
}

