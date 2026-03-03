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

@VintfStability
enum VoiceEcallEventFail {
    NONE = 0,
    TIMEOUT = 1,
    HACK_TIMEOUT = 2,
    MSD_TRANS_TIMEOUT = 3,
    IVS_RESET_TIMEOUT = 4,
    CLEAR_DOWN_FALLBACK_TIMEOUT = 5,
    IVS_INITIATION_TIMEOUT = 6,
}
