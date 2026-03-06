// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

/**
 * Structure for RIL_UNSOL_CALL_FORWARDING
 */
@VintfStability
parcelable CfuStatusNotification {
    int status;
    int lineId;
}

