

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceEcallType;

@VintfStability
parcelable VoiceEcallInfo {
    int msdLen;
    byte[] msd;
    VoiceEcallType type;
    int autoTrigger;
    String testNumber;
}
