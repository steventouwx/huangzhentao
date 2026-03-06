// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

/**
 * Structure for RIL_UNSOL_CIPHER_INDICATION.
 */
@VintfStability
parcelable CipherNotification {
    String simCipherStatus;
    String sessionStatus;
    String csStatus;
    String psStatus;
}

