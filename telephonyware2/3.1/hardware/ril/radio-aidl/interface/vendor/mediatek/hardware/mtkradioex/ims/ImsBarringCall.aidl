// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ims;

@VintfStability
parcelable ImsBarringCall {
    int status; // 0: disabled, 1: enabled
    int serviceClass;
    int toa;
    String number; // the ICB number for IMS call barring
}

