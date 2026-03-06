
package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceEcallState {
    NONE = -1,
    INACTIVE = 0,
    ORIGINATING_CALL = 1,
    IN_CALL_TRANSMITTING = 2,
    WAITING_FOR_AL_ACK = 3,
    IN_CALL = 4,
    IDLE_ALLOW_MT_ECALL = 5,
}
