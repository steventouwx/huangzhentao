

package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceForwardingCondition {
    UNCONDITIONALLY = 0,
    MOBILE_BUSY = 1,
    NO_REPLY = 2,
    UNREACHABLE = 3,
    ALL_FORWARDING = 4,
    ALL_CONDITIONAL = 5,
}
