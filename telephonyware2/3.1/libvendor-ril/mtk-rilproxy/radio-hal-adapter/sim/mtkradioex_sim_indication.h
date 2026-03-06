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

#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSimIndication.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/BnMtkRadioExSimResponse.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/sim/IMtkRadioExSim.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuRequest.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuRequestInfo.h>
#include <aidl/vendor/mediatek/hardware/mtkradioex/rsu/RsuResponseInfo.h>
#include <aidl/android/hardware/radio/RadioError.h>
#include <aidl/android/hardware/radio/config/IRadioConfig.h>
#include <aidl/android/hardware/radio/config/SimSlotStatus.h>
#include <aidl/android/hardware/radio/sim/CardStatus.h>
#include <aidl/android/hardware/radio/sim/IRadioSim.h>
#include <aidl/android/hardware/radio/RadioIndicationType.h>

#include "RfxDispatchThread.h"

using namespace aidl::vendor::mediatek::hardware::mtkradioex::sim;
using namespace aidl::vendor::mediatek::hardware::mtkradioex::rsu;
using namespace aidl::android::hardware::radio;

/* Callback class for radio SIM indication */
class MtkRadioExSimIndication : public BnMtkRadioExSimIndication {
public:
    MtkRadioExSimIndication(int slot, RfxDispatchThread* dispatchThread);
 private:
    RfxDispatchThread* mDispatchThread;
    int mSlot;

  public:

    virtual ndk::ScopedAStatus simTypeInfoChangedInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::SimTypeInfo& in_typeInfo) override;
    virtual ndk::ScopedAStatus bipProactiveCommand(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_cmd) override;
    virtual ndk::ScopedAStatus iccidChanged(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::string& in_iccid) override;
    virtual ndk::ScopedAStatus onCardDetectedInd(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus onImeiLock(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus onImsiRefreshDone(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus onRsuEvent(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_eventId, const std::string& in_eventString) override;
    virtual ndk::ScopedAStatus onRsuSimLockEvent(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_eventId) override;
    virtual ndk::ScopedAStatus onSimHotSwapInd(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_event, const std::string& in_info) override;
    virtual ndk::ScopedAStatus onSimPowerChangedInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_info) override;
    virtual ndk::ScopedAStatus onStkMenuReset(::aidl::android::hardware::radio::RadioIndicationType in_type) override;
    virtual ndk::ScopedAStatus onVirtualSimStatusChanged(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_simInserted) override;
    virtual ndk::ScopedAStatus onVsimEventIndication(::aidl::android::hardware::radio::RadioIndicationType in_type, const ::aidl::vendor::mediatek::hardware::mtkradioex::sim::VsimOperationEvent& in_event) override;
    virtual ndk::ScopedAStatus phbReadyNotification(::aidl::android::hardware::radio::RadioIndicationType in_type, int32_t in_isPhbReady) override;
    virtual ndk::ScopedAStatus smlSlotLockInfoChangedInd(::aidl::android::hardware::radio::RadioIndicationType in_type, const std::vector<int32_t>& in_info) override;

};