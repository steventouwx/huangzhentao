// SPDX-License-Identifier: MediaTekProprietary
/*
 * Copyright (C) 2022 The Android Open Source Project
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

#pragma once

#include <aidl/android/hardware/radio/ims/BnRadioImsIndication.h>
#include <aidl/android/hardware/radio/ims/BnRadioImsResponse.h>
#include <aidl/android/hardware/radio/ims/IRadioIms.h>
#include <memory>
#include <cstdint>

#include "radio_ims_response.h"
#include "radio_ims_indication.h"
#include "../radio_service.h"


using namespace aidl::android::hardware::radio::ims;

class RadioIms : public RadioService {
public:
     RadioIms(int slot, RfxDispatchThread* dispatchThread);

  public:

    /* radio ims service handle */
    std::shared_ptr<IRadioIms> radio_ims;
    /* radio ims response handle */
    std::shared_ptr<RadioImsResponse> radioRsp_ims;
    /* radio ims indication handle */
    std::shared_ptr<RadioImsIndication> radioInd_ims;
};