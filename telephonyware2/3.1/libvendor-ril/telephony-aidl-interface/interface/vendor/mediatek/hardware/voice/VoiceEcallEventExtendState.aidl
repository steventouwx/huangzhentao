

package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceEcallEventExtendState {
    NONE = -1,
    START_RECV_SYN = 0,
    T9_TIMEOUT = 1,
    T10_TIMEOUT = 2,
    IVS_ALACK_RECEIVED = 3,
    IVS_LLACK_RECEIVED = 4,
    STOPPED = 5,
    ANSWERING_INCOMING_PSAP_ECALL = 6,
    CLEARDOWN_RECEIVED = 7,
    CALLBACK_TIMEOUT = 8,
    IVS_NORMAL_CLEARING = 9,
    IVS_ABNORMAL_CLEARING = 10,
    T1_TIMEOUT = 11,
    REDIAL_DURATION_TIMEOUT = 12,
}
