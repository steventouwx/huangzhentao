

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceEcallMsdTransmissionStatus;

@VintfStability
interface IVoiceEcallStatusCallback {
    void onVoiceEcallStatus(in int id, in VoiceEcallMsdTransmissionStatus status);
}
