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
enum VoiceEcallEventExtendState {
    NONE = -1,
    START_RECV_SYN = 0,
    T9_TIMEOUT = 1,
    T10_TIMEOUT = 2,
    IVS_ALACK_RECEIVED = 3,
    IVS_LLACK_RECEIVED = 4,
    STOPPED = 5,
    ANSWERING_INCOMING_PSAP_ECALL = 6,
    CLEARDOWN_RECEIVED = 7,
    CALLBACK_TIMEOUT = 8,
    IVS_NORMAL_CLEARING = 9,
    IVS_ABNORMAL_CLEARING = 10,
    T1_TIMEOUT = 11,
    REDIAL_DURATION_TIMEOUT = 12,
}
