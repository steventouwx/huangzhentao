// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.rsu;

@VintfStability
@Backing(type="int")
enum RsuRequest {
    RSU_REQUEST_INIT_REQUEST = 0,
    RSU_REQUEST_GET_SHARED_KEY,
    RSU_REQUEST_UPDATE_LOCK_DATA,
    RSU_REQUEST_GET_LOCK_VERSION,
    RSU_REQUEST_RESET_LOCK_DATA,
    RSU_REQUEST_GET_LOCK_STATUS,
    RSU_REQUEST_UNLOCK_TIMER = 50,
}
