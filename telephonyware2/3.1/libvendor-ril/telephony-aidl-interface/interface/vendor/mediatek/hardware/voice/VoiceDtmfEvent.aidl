

package vendor.mediatek.hardware.voice;

@VintfStability
enum VoiceDtmfEvent {
    REV_BURST = 0,
    REV_START_CONT = 1,
    REV_STOP_CONT = 2,
    FWD_BURST = 3,
    FWD_START_CONT = 4,
    FWD_STOP_CONT = 5,
    IP_INCOMING_DTMF_START = 6,
    IP_INCOMING_DTMF_STOP = 7,
}
