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

import vendor.mediatek.hardware.tbox.MTK_TimeStampType;
import vendor.mediatek.hardware.tbox.MTK_CellInfoRatSpecificInfo;

@VintfStability
parcelable MTK_CellInfo {
    int cellInfoType;
    /**
     * True if this cell is registered false if not registered.
     */
    boolean registered;
    /**
     * Type of time stamp represented by timeStamp.
     */
    MTK_TimeStampType timeStampType;
    /**
     * Time in nanos as returned by ril_nano_time.
     */
    long timeStamp;
    MTK_CellInfoRatSpecificInfo ratSpecificInfo;
}
