// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

@VintfStability
parcelable Dialog {
    int dialogId;
    int callState;
    int callType;
    boolean isPullable;
    boolean isCallHeld;
    boolean isMt;
    String address;
    String remoteAddress;
}

