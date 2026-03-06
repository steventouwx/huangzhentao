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

import vendor.mediatek.hardware.tbox.MTK_CellInfoGsm;
import vendor.mediatek.hardware.tbox.MTK_CellInfoWcdma;
import vendor.mediatek.hardware.tbox.MTK_CellInfoTdscdma;
import vendor.mediatek.hardware.tbox.MTK_CellInfoLte;
import vendor.mediatek.hardware.tbox.MTK_CellInfoNr;
import vendor.mediatek.hardware.tbox.MTK_CellInfoCdma;
	  
@VintfStability
parcelable MTK_CellInfoRatSpecificInfo {
        /**
         * 3gpp CellInfo types.
         */
        MTK_CellInfoGsm gsm;
        MTK_CellInfoWcdma wcdma;
        MTK_CellInfoTdscdma tdscdma;
        MTK_CellInfoLte lte;
        MTK_CellInfoNr nr;
        /**
         * 3gpp2 CellInfo types;
         */
        MTK_CellInfoCdma cdma;
}
