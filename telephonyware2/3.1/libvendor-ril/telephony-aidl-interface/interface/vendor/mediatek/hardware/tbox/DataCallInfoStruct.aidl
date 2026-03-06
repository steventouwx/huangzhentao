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
parcelable DataCallInfoStruct {
    int ProfileIdx;
    int IpFamily;
    String V4InterfaceName;
    int V4State;
    boolean V4Reconnect;
    String V4Ip;
    String V4Gateway;
    String V4PriDns;
    String V4SecDns;
    int V4PktsTx;
    int V4PktsRx;
    long V4BytesTx;
    long V4BytesRx;
    int V4PktsDroppedTx;
    int V4PktsDroppedRx;
    String V6InterfaceName;
    int V6State;
    boolean V6Reconnect;
    String V6Ip;
    String V6Gateway;
    String V6PriDns;
    String V6SecDns;
    int V6PktsTx;
    int V6PktsRx;
    long V6BytesTx;
    long V6BytesRx;
    int V6PktsDroppedTx;
    int V6PktsDroppedRx;
}
