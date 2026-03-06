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
enum MTK_PersoSubstate {
    UNKNOWN                   = 0, /* initial state */
    IN_PROGRESS               = 1, /* in between each lock transition */
    READY                     = 2, /* when either SIM or RUIM Perso is finished
                                                        since each app can only have 1 active perso
                                                        involved */
    SIM_NETWORK               = 3,
    SIM_NETWORK_SUBSET        = 4,
    SIM_CORPORATE             = 5,
    SIM_SERVICE_PROVIDER      = 6,
    SIM_SIM                   = 7,
    SIM_NETWORK_PUK           = 8, /* The corresponding perso lock is blocked */
    SIM_NETWORK_SUBSET_PUK    = 9,
    SIM_CORPORATE_PUK         = 10,
    SIM_SERVICE_PROVIDER_PUK  = 11,
    SIM_SIM_PUK               = 12,
    RUIM_NETWORK1             = 13,
    RUIM_NETWORK2             = 14,
    RUIM_HRPD                 = 15,
    RUIM_CORPORATE            = 16,
    RUIM_SERVICE_PROVIDER     = 17,
    RUIM_RUIM                 = 18,
    RUIM_NETWORK1_PUK         = 19, /* The corresponding perso lock is blocked */
    RUIM_NETWORK2_PUK         = 20,
    RUIM_HRPD_PUK             = 21,
    RUIM_CORPORATE_PUK        = 22,
    RUIM_SERVICE_PROVIDER_PUK = 23,
    RUIM_RUIM_PUK             = 24,
    SIM_SPN                   = 25,
    SIM_SPN_PUK               = 26,
    SIM_SP_EHPLMN             = 27,
    SIM_SP_EHPLMN_PUK         = 28,
    SIM_ICCID                 = 29,
    SIM_ICCID_PUK             = 30,
    SIM_IMPI                  = 31,
    SIM_IMPI_PUK              = 32,
    SIM_NS_SP                 = 33,
    SIM_NS_SP_PUK             = 34,
    // MTK added
    SIM_SIM_C                 = 100,
    SIM_SIM_C_PUK             = 101,
}
