/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2018. All rights reserved.
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

#include <telephony/mtk_ril.h>
#include <ril_internal.h>

#include <aidl/android/hardware/radio/config/BnRadioConfig.h>

namespace android::hardware::radio::config {
class RadioConfig : public aidl::android::hardware::radio::config::BnRadioConfig {
public:
::ndk::ScopedAStatus getHalDeviceCapabilities(int32_t serial) override;
::ndk::ScopedAStatus getNumOfLiveModems(int32_t serial) override;
::ndk::ScopedAStatus getPhoneCapability(int32_t serial) override;
::ndk::ScopedAStatus getSimSlotsStatus(int32_t serial) override;
::ndk::ScopedAStatus setNumOfLiveModems(int32_t serial, int8_t numOfLiveModems) override;
::ndk::ScopedAStatus setPreferredDataModem(int32_t serial, int8_t modemId) override;
::ndk::ScopedAStatus setResponseFunctions(
        const std::shared_ptr<aidl::android::hardware::radio::config::IRadioConfigResponse>& radioConfigResponse,
        const std::shared_ptr<aidl::android::hardware::radio::config::IRadioConfigIndication>& radioConfigIndication) override;
::ndk::ScopedAStatus setSimSlotsMapping(
        int32_t serial,
        const std::vector<aidl::android::hardware::radio::config::SlotPortMapping>& slotMap) override;
::ndk::ScopedAStatus getSimultaneousCallingSupport(int32_t serial) override;

public:
    RadioConfig(int slotid);
    int mSlotId;
};
}

namespace aidlRadioConfig {
    void checkAidlReturnStatus(unsigned int slotId, ndk::ScopedAStatus& ret);
    void clearRadioConfigResponseAndIndications();
    int getSimSlotsStatusResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int token, RIL_Errno e, const void *response, size_t responselen);
    int setSimSlotsMappingResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int token, RIL_Errno e, const void *response, size_t responselen);
    int setPreferredDataModemResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int token, RIL_Errno e, const void *response, size_t responselen);
    int simSlotStatusChangedInd(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int getPhoneCapabilityResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int token, RIL_Errno e, const void *response, size_t responselen);
    int setModemsConfigResponse(unsigned int slotId, android::ClientId clientId, int responseType,
            int token, RIL_Errno e, const void *response, size_t responselen);
    int onSimultaneousCallingSupportChangedInd(unsigned int slotId,
            int indicationType, int token, RIL_Errno e, const void *response,
            size_t responselen);
    int getSimultaneousCallingSupportResponse(unsigned int slotId, android::ClientId clientId,
            int responseType, int serial, RIL_Errno e, const void *response, size_t responselen);
}

namespace radioConfig {
    void registerService(RIL_RadioFunctions *callbacks, android::CommandInfo *commands);
}
