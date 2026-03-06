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
parcelable DataCallState {
    int profile_idx;
    /*
     * !< UMTS/CMDA profile ID.
     */
    String name;
    /*
     * !< interface name.
     */
    int ipFamily;
    /*
     * !< IP version.
     */
    int state;
    /*
     * !< PDN status.
     */
    int online;
    /*
     * !< online status.
     */
    int err;
    String v4Ip;
    String v4Gateway;
    String v4PriDns;
    String v4SecDns;
    String v6Ip;
    String v6Gateway;
    String v6PriDns;
    String v6SecDns;
}
