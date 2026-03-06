

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceRecordArray;

@VintfStability
interface IVoiceCallCallback {
    void onVoiceCallChanged(in VoiceRecordArray records);
}
