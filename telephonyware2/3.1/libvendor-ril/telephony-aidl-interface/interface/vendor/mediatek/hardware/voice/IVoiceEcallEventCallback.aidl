

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceEcallEvent;

@VintfStability
interface IVoiceEcallEventCallback {
    void onVoiceEcallEvent(in VoiceEcallEvent event);
}
