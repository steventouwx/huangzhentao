

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceForwardingDetail;

@VintfStability
parcelable VoiceForwardingStatus {
    int enabled;
    int len;
    VoiceForwardingDetail[] details;
}
