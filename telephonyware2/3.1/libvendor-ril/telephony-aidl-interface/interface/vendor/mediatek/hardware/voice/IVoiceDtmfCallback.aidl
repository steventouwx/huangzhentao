

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceDtmfEvent;

@VintfStability
interface IVoiceDtmfCallback {
    void onVoiceDtmfEvent(in int id, in VoiceDtmfEvent event, in String digits, in int len);
}
