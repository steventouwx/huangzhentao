

package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceEcallEventProcess {
    IVS_NONE = 0,
    IVS_START_RECEIVED_MSD = 1,
    IVS_NACK_RECEIVED = 2,
    IVS_ACK_RECEIVED = 3,
    IVS_TX_COMPLETED = 4,
    IVS_HLACK_RECEIVED = 5,
}
