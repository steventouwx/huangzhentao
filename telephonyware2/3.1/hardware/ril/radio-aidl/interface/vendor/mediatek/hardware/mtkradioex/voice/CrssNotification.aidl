// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

/**
 * Structure for RIL_UNSOL_CRSS_NOTIFICATION.
 */
@VintfStability
parcelable CrssNotification {
    int code;
    int type;
    String number;
    String alphaid;
    int cli_validity;
}

