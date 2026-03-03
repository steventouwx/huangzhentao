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
enum VoiceEcallEventProcess {
    IVS_NONE = 0,
    IVS_START_RECEIVED_MSD = 1,
    IVS_NACK_RECEIVED = 2,
    IVS_ACK_RECEIVED = 3,
    IVS_TX_COMPLETED = 4,
    IVS_HLACK_RECEIVED = 5,
}
