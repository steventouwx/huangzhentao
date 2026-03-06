// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.data;

/*
 * Structure for RIL_UNSOL_PCO_DATA_AFTER_ATTACHED
 */
@VintfStability
parcelable PcoDataAttachedInfo {
    int cid;
    String apnName;
    String bearerProto;
    int pcoId;
    byte[] contents;
}

