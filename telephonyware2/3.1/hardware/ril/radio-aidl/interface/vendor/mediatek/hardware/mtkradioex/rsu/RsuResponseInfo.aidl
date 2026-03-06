// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.rsu;

@VintfStability
parcelable RsuResponseInfo {
    int opId;
    int requestId;
    int errCode;
    String data;
    long time;
    int version;
    int status;
    int reserveInt1;
    int reserveInt2;
    String reserveString1;
}

