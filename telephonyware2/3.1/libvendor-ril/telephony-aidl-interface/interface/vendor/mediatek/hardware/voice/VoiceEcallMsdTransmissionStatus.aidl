

package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceEcallMsdTransmissionStatus {
    NONE = -1,
    SUCCESS = 0,
    FAILURE = 1,
    STARTED = 2,
    NACK_OUT_OF_ORDER = 3,
    ACK_OUT_OF_ORDER = 4,
    START_RECEIVED = 5,
    LL_ACK_RECEIVED = 6,
    TX_USER_DEFINED_1 = 7,
    TX_USER_DEFINED_2 = 8,
    TX_USER_DEFINED_3 = 9,
    OUTBAND_STARTED = 10,
    OUTBAND_SUCCESS = 11,
    OUTBAND_FAILURE = 12,
}
