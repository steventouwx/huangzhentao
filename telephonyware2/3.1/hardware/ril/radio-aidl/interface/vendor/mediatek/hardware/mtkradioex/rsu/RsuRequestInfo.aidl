// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.rsu;

@VintfStability
parcelable RsuRequestInfo {
    int opId;
    int requestId;
    int requestType;
    String data;
    int reserveInt1;
    int reserveInt2;
    String reserveString1;
}

