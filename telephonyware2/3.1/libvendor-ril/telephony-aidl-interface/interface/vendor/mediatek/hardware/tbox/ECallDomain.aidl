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
enum ECallDomain {
    EM_DOMAIN_AUTO = 0,      /* Automatic mode - LTE(IMS), WG(CS), 1x(C2K) */
    EM_DOMAIN_CS_ONLY = 1,   /* CS domain only - WG(CS) */
    EM_DOMAIN_3GPP_ONLY = 2, /* 3GPP only - LTE(IMS), WG(CS) */
    EM_DOMAIN_3GPP2 = 3,     /* 3GPP2 only - 1x(C2K) */
    EM_DOMAIN_IMS_1xCS = 4,  /* IMS and 1x CS only - LTE(IMS), 1x(C2K) */
    EM_DOMAIN_CS_1x = 5,     /* WG CS and 1x CS only - WG(CS), 1x(C2K) */
    EM_DOMAIN_IMS_ONLY = 6,  /* only IMS call allowed */
}
