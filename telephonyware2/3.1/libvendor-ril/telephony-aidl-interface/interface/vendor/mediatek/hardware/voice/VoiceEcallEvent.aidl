

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceEcallDropRedial;
import vendor.mediatek.hardware.voice.VoiceEcallEventEstablish;
import vendor.mediatek.hardware.voice.VoiceEcallEventExtendState;
import vendor.mediatek.hardware.voice.VoiceEcallEventFail;
import vendor.mediatek.hardware.voice.VoiceEcallEventMsdUpdate;
import vendor.mediatek.hardware.voice.VoiceEcallEventProcess;
import vendor.mediatek.hardware.voice.VoiceEcallOriginateFailRedial;

@VintfStability
parcelable VoiceEcallEvent {
    VoiceEcallEventFail fail;
    VoiceEcallEventProcess process;
    VoiceEcallEventMsdUpdate msdUpdate;
    VoiceEcallEventEstablish establish;
    int hackCode;
    VoiceEcallOriginateFailRedial oriRedial;
    VoiceEcallDropRedial dropRedial;
    VoiceEcallEventExtendState extendState;
}
