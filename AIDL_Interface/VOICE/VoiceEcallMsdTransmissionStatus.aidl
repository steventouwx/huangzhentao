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
enum VoiceEcallMsdTransmissionStatus {
    NONE = -1,
    SUCCESS = 0,
    FAILURE = 1,
    STARTED = 2,
    NACK_OUT_OF_ORDER = 3,
    ACK_OUT_OF_ORDER = 4,
    START_RECEIVED = 5,
    LL_ACK_RECEIVED = 6,
    TX_USER_DEFINED_1 = 7,
    TX_USER_DEFINED_2 = 8,
    TX_USER_DEFINED_3 = 9,
    OUTBAND_STARTED = 10,
    OUTBAND_SUCCESS = 11,
    OUTBAND_FAILURE = 12,
}
