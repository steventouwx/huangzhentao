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
 
package vendor.mediatek.hardware.tbox;

@VintfStability
parcelable MTK_LTE_SignalStrength {
    int signalStrength;
    /*
     * Valid values are (0-31, 99) as defined in TS 27.007 8.5
     */
    int rsrp;
    /*
     * The current Reference Signal Receive Power in dBm multipled by -1.
     * Range: 44 to 140 dBm
     * INT_MAX: 0x7FFFFFFF denotes invalid value.
     * Reference: 3GPP TS 36.133 9.1.4
     */
    int rsrq;
    /*
     * The current Reference Signal Receive Quality in dB multiplied by -1.
     * Range: 20 to 3 dB.
     * INT_MAX: 0x7FFFFFFF denotes invalid value.
     * Reference: 3GPP TS 36.133 9.1.7
     */
    int rssnr;
    /*
     * The current reference signal signal-to-noise ratio in 0.1 dB units.
     * Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).
     * INT_MAX : 0x7FFFFFFF denotes invalid value.
     * Reference: 3GPP TS 36.101 8.1.1
     */
    int cqi;
    /*
     * The current Channel Quality Indicator.
     * Range: 0 to 15.
     * INT_MAX : 0x7FFFFFFF denotes invalid value.
     * Reference: 3GPP TS 36.101 9.2, 9.3, A.4
     */
    int timingAdvance;
}
