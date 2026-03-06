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

#include <aidl/vendor/mediatek/hardware/mtkradioex/ecall/BnMtkRadioExEcallIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/ecall/BnMtkRadioExEcallResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/ecall/IMtkRadioExEcall.h>

#include "RfxDispatchThread.h"
#include "mtkradioex_ecall_response.h"
#include "mtkradioex_ecall_indication.h"
#include "../radio_service.h"
#include "../radio_hal_utils.h"


using namespace aidl::vendor::mediatek::hardware::mtkradioex::ecall;


// The main test class for MtkRadioEx AIDL Ecall.
class MtkRadioExEcall : public RadioService {
public:
     MtkRadioExEcall(int slot, RfxDispatchThread* dispatchThread);
  public:
    /* mtkradioex ecall service handle */
    std::shared_ptr<IMtkRadioExEcall> mtkradioex_ecall;
    /* mtkradioex ecall response handle */
    std::shared_ptr<MtkRadioExEcallResponse> mtkradioexRsp_ecall;
    /* mtkradioex ecall indication handle */
    std::shared_ptr<MtkRadioExEcallIndication> mtkradioexInd_ecall;

    int32_t resetIvs(int32_t in_serial, int32_t in_clientId);
    int32_t setMSD(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetMSD& in_setMSD, int32_t in_clientId);
    int32_t setTestNum(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_testNum, int32_t in_clientId);
    int32_t setReconfNum(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_reconfNum, int32_t in_clientId);
    int32_t makeFastEcall(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallReqMsg& in_reqMsg, int32_t in_clientId);
    int32_t setEmsdPri(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallPRI& in_pri, int32_t in_clientId);
    int32_t setNadDeregTime(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallTime& in_time, int32_t in_clientId);
    int32_t setOprtMode(int32_t in_serial, int32_t in_mode, int32_t in_clientId);
    int32_t getOprtMode(int32_t in_serial, int32_t in_clientId);
};
