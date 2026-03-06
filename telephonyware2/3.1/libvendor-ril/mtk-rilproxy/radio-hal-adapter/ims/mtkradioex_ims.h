// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2021. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#pragma once

#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/BnMtkRadioExImsIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/BnMtkRadioExImsResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/mwi/BnMwiRadioIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/mwi/BnMwiRadioResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/ims/IMtkRadioExIms.h>
#include <cstdint>

#include "RfxDispatchThread.h"
#include "mtkradioex_ims_response.h"
#include "mtkradioex_ims_indication.h"
#include "../radio_service.h"
#include "../radio_hal_utils.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::ims;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::mwi;

// The main test class for MtkRadioEx AIDL Ims.
class MtkRadioExIms : public RadioService {
public:
     MtkRadioExIms(int slot, RfxDispatchThread* dispatchThread);

  public:
    /* mtkradioex ims service handle */
    std::shared_ptr<IMtkRadioExIms> mtkradioex_ims;
    /* mtkradioex ims response handle */
    std::shared_ptr<MtkRadioExImsResponse> mtkradioexRsp_ims;
    /* mtkradioex ims indication handle */
    std::shared_ptr<MtkRadioExImsIndication> mtkradioexInd_ims;
public:
    int32_t setImscfg(int32_t in_serial, bool in_volteEnable, bool in_vilteEnable, bool in_vowifiEnable, bool in_viwifiEnable, bool in_smsEnable, bool in_eimsEnable, int32_t in_clientId = CLIENT_TBOX_IMS);
    int32_t controlCall(int32_t in_serial, int32_t controlType, int32_t callId, int32_t clientId);
    int32_t getImscfg(int serial, int32_t in_clientId = CLIENT_TBOX_IMS);
};