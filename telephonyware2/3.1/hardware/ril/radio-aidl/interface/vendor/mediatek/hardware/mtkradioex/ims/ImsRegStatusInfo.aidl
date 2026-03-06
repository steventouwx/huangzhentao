// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.ims;

/*
 * Structure for IMS registration status report
 */
@VintfStability
parcelable ImsRegStatusInfo {
    int report_type;
    int account_id;
    int expire_time;
    int error_code;
    String uri;
    String error_msg;
}

