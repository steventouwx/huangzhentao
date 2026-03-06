/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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

#ifndef __RADIOECALL_SERVICE__H__
#define __RADIOECALL_SERVICE__H__

#include <telephony/mtk_ril.h>
#include <telephony/mtk_ril_ivt.h>
#include <ril_internal.h>

using ::android::ClientId;

namespace mtkRadioExEcall {
// Response
int resetIvsResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int setMSDResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int setTestNumResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int setReconfNumResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int makeFastEcallResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int setEmsdPriResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int setNadDeregTimeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int setOprtModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);
int getOprtModeResponse(unsigned int slotId, ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response,
        size_t responselen);

//indication
int ecallIndicationInd(unsigned int slotId, int indicationType,
        int token, RIL_Errno e, const void *response, size_t responselen);
int clearMtkEcallResponseAndIndications(unsigned int slotId);
}


#include <aidl/vendor/mediatek/hardware/mtkradioex/ecall/BnMtkRadioExEcall.h>

namespace aidl_mtk_radio_ecall = ::aidl::vendor::mediatek::hardware::mtkradioex::ecall;

namespace vendor::mediatek::hardware::mtkradioex::ecall {

class MtkRadioEcall : public aidl::vendor::mediatek::hardware::mtkradioex::ecall::BnMtkRadioExEcall {
public:
    ::ndk::ScopedAStatus resetIvs(int32_t in_serial, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setMSD(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetMSD& in_setMSD, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setTestNum(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_testNum, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setReconfNum(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallSetNum& in_reconfNum, int32_t in_clientId) override;
    ::ndk::ScopedAStatus makeFastEcall(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallReqMsg& in_reqMsg, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setEmsdPri(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallPRI& in_pri, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setNadDeregTime(int32_t in_serial, const ::aidl::vendor::mediatek::hardware::mtkradioex::ecall::ECallTime& in_time, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setOprtMode(int32_t in_serial, int32_t in_mode, int32_t in_clientId) override;
    ::ndk::ScopedAStatus getOprtMode(int32_t in_serial, int32_t in_clientId) override;
    ::ndk::ScopedAStatus setResponseFunctionsMtk(
            const std::shared_ptr<aidl_mtk_radio_ecall::IMtkRadioExEcallResponse>& radioResponse,
            const std::shared_ptr<aidl_mtk_radio_ecall::IMtkRadioExEcallIndication>& radioIndication) override;

  public:
    MtkRadioEcall(int slotid);
    int mSlotId;

  public:
    bool checkRequestClientIdSupport(int32_t client, android::ClientId clientId) {
        if (client < 0) {
            return false;
        }
        return clientId == ((unsigned int)client);
    }
};
}

#define FREEIF(data)    \
if (data != NULL) {     \
    free(data);         \
    data = NULL;        \
}

#endif
