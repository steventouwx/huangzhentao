

package vendor.mediatek.hardware.voice;

import vendor.mediatek.hardware.voice.VoiceCallAlertingType;
import vendor.mediatek.hardware.voice.VoiceCallDirection;
import vendor.mediatek.hardware.voice.VoiceCallState;
import vendor.mediatek.hardware.voice.VoiceCallTech;
import vendor.mediatek.hardware.voice.VoiceCallType;

@VintfStability
parcelable VoiceRecord {
    int id;
    String number;
    VoiceCallState state;
    VoiceCallTech tech;
    VoiceCallDirection dir;
    int endReason;
    VoiceCallType callType;
    VoiceCallAlertingType callAlertingType;
    int sipErrorCode;
}
