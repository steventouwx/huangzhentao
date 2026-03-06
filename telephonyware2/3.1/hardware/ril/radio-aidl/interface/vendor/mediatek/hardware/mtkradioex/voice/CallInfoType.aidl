// FIXME: license file, or use the -l option to generate the files with the header.

package vendor.mediatek.hardware.mtkradioex.voice;

@VintfStability
@Backing(type="int")
enum CallInfoType {
    MT_CALL_NONE = 0,
    MT_CALL_REJECTED = 1,
    MT_CALL_MISSED = 2,
    MT_CALL_NUMREDIRECT = 3,
    MT_CALL_RQ = 4,
    MT_CALL_GWSD = 10,
    MT_CALL_DIAL_IMS_STK = 100,
}
