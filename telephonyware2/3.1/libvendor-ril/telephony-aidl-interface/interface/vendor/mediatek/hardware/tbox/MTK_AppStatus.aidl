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

import vendor.mediatek.hardware.tbox.MTK_AppType;
import vendor.mediatek.hardware.tbox.MTK_AppState;
import vendor.mediatek.hardware.tbox.MTK_PersoSubstate;
import vendor.mediatek.hardware.tbox.MTK_PinState;

@VintfStability
parcelable MTK_AppStatus {
  MTK_AppType      app_type;
  MTK_AppState     app_state;
  MTK_PersoSubstate perso_substate; /* applicable only if app_state ==
                                       MTK_APPSTATE_SUBSCRIPTION_PERSO */
  String             aid_ptr;        /* null terminated string, e.g., from 0xA0, 0x00 -> 0x41,
                                       0x30, 0x30, 0x30 */
  String             app_label_ptr;  /* null terminated string */
  int              pin1_replaced;   /* applicable to USIM, CSIM & ISIM */
  MTK_PinState     pin1;
  MTK_PinState     pin2;
}
