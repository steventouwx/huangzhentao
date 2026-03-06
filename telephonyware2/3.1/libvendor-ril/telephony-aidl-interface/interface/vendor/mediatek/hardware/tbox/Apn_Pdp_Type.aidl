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
@Backing(type="int")
enum Apn_Pdp_Type {
    APN_PDP_TYPE_IPV4 = 0,
    APN_PDP_TYPE_PPP,
    APN_PDP_TYPE_IPV6,
    APN_PDP_TYPE_IPV4V6,
}
