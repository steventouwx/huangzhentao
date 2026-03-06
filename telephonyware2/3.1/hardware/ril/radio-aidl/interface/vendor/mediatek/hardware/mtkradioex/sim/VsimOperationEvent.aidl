// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.sim;

@VintfStability
parcelable VsimOperationEvent {
    int transactionId;
    int eventId;
    int result;
    int dataLength;
    String data;
}

