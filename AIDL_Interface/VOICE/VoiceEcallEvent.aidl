/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
