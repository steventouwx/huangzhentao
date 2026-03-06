

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceEcallMode;

@VintfStability
parcelable VoiceEcallConfig {
    int enable;
    int voiceMute;
    VoiceEcallMode mode;
    int reportEvent;
    int startTimer;
    int hackTimer;
    int transTimer;
    int failRedial;
    int dropRedial;
    int cleardownTimer;
    int initTimer;
    int nadRegTimer;
    int nadDeregTimer;
    int standard;
    int eraGlonassRedial;
    int autoAnswer;
    int ecallUseSimNum;
    int t1Timer;
    int redialDuration;
}
