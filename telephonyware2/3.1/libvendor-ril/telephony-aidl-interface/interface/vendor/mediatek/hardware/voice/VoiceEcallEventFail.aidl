

package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceEcallEventFail {
    NONE = 0,
    TIMEOUT = 1,
    HACK_TIMEOUT = 2,
    MSD_TRANS_TIMEOUT = 3,
    IVS_RESET_TIMEOUT = 4,
    CLEAR_DOWN_FALLBACK_TIMEOUT = 5,
    IVS_INITIATION_TIMEOUT = 6,
}
