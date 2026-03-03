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
enum VoiceDtmfEvent {
    REV_BURST = 0,
    REV_START_CONT = 1,
    REV_STOP_CONT = 2,
    FWD_BURST = 3,
    FWD_START_CONT = 4,
    FWD_STOP_CONT = 5,
    IP_INCOMING_DTMF_START = 6,
    IP_INCOMING_DTMF_STOP = 7,
}
