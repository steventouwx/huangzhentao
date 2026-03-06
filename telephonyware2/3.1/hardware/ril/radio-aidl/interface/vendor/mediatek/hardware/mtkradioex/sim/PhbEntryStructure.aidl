// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.sim;

@VintfStability
parcelable PhbEntryStructure {
    int type;
    /*
     * type of the entry, refer to RIL_PhbStorageType
     */
    int index;
    /*
     * the stored index of the entry
     */
    String number;
    /*
     * the phone number
     */
    int ton;
    /*
     * type of the number
     */
    String alphaId;
}

