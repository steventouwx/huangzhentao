// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

/**
 * Structure for RIL_UNSOL_INCOMING_CALL_INDICATION.
 */
@VintfStability
parcelable IncomingCallNotification {
    String callId;
    String number;
    String type;
    String callMode;
    String seqNo;
    String redirectNumber;
    String toNumber;
}

