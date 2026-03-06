package vendor.mediatek.hardware.mtkradioex.rcs;

@VintfStability
@Backing(type="int")

enum RegState {
    STATE_REGISTERED = 1,
    STATE_UNREGISTERED = 2,
    STATE_REGISTERING = 3,
    STATE_DEREGISTERING = 4,
    STATE_DISCONNECTED = 5,
    STATE_AUTHENTICATING = 6,
    STATE_OOS = 7,
    STATE_CONNECTING = 8,
}