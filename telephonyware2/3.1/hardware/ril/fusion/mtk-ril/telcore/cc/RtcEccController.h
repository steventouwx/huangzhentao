/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2020. All rights reserved.
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

#ifndef __RTC_ECC_CONTROLLER_H__
#define __RTC_ECC_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/

#include "RfxController.h"

#include "nw/RtcNwDefs.h"
#include "RtcEccNumberController.h"

using namespace android;

class RtcEccController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtcEccController);

public:
    RtcEccController();
    virtual ~RtcEccController();

    void onPreLocalEmergencyDial(const sp<RfxMessage>& message);
    void onPostLocalEmergencyDial(const sp<RfxMessage>& message);
    void onEccActive();
    void onEccHangupLocal();

// Override
protected:
    virtual void onInit();
    virtual void onDeinit();
    RIL_Dial *getDialData(const sp<RfxMessage>& message);

    RtcEccNumberController *getEccNumberController();
    RtcEccNumberController *getEccNumberController(int slotId);

    static bool isBspFeatureSupported();

    int getEccPreferredRat();
    void setEccPreferredRat(int eccPreferRat);

    /// M: CC: ECC prefer RAT related utils @{
    int getMainCapabilitySlot();
    int getCPhoneSlotId();
    bool hasC2kRaf();
    bool hasC2kRaf(int slotId);
    bool isGCDualPhoneExist();
    bool isCPhone();
    bool isCPhone(int slotId);
    bool isGPhone();
    bool isGPhone(int slotId);
    NwsMode getNWSelectionMode();
    NwsMode getNWSelectionMode(int slotId);
    bool isAllSimInsert();
    bool isAllNoSimInsert();
    bool hasNoSim();
    bool hasNoSim(int slotId);
    bool hasLockedSim();
    bool hasLockedSim(int slotId);
    bool hasUnlockedSim();
    bool hasUnlockedSim(int slotId);
    bool hasDisabledSim();
    bool hasLockedOrDisabledSim();
    bool hasNonCdmaSim();
    bool hasNonCdmaSim(int slotId);
    bool hasCdmaSim();
    bool hasCdmaSim(int slotId);
    int getSimState();
    int getSimState(int slotId);

    bool isGsmOnlyNumber(String8 number);
    bool isGsmOnlyNumber(int slotId, String8 number);
    bool isGsmPreferredNumber(String8 number);
    bool isGsmPreferredNumber(int slotId, String8 number);
    bool isCdmaPreferredNumber(String8 number) ;
    bool isCdmaPreferredNumber(int slotId, String8 number) ;

    bool isCTAMccExist();
    int getCtVolteSupportType();
    bool isVolteEnabled();

    bool isAllSlotInService();
    bool isGPhoneInService();
    bool isInService();
    bool isInService(int slotId);

    bool isOneSlotEmergencyNumber(String8 number);
    bool isSlotEmergencyNumber(String8 number);
    bool isSlotEmergencyNumber(int slotId, String8 number);
    /// @}

private:
    // add one more status manage for no-slot controllers
    RfxStatusManager *m_status_managers[MAX_RFX_SLOT_ID + 1];
    RtcEccNumberController *m_ecc_num_controller[MAX_RFX_SLOT_ID];

    static bool mIsBspProject;

    bool checkRequestMsgParam(const sp<RfxMessage>& message);



    /***** [ BSP project only control below ] *****/

    /// M: CC: BSP project ECC retry status @{
private:
    static bool mIsBspEccDialed;
protected:
    bool isInBspEccRetry();
    void setBspEccDialed(bool isDialed);
    /// @}

    /// M: CC: radio power for ecc status indication @{
private:
    static bool mIsRadioOnForEcc;
    TimerHandle mRadioOnForEccTimeoutHandle;
public:
    bool isInRadioOnForEcc();
    void setRadioOnForEcc(bool forEcc);
    void setRadioOnForEcc(bool forEcc, const RfxCallback0& timeoutHandler, long int timeout);
protected:
private:
    void onDummyCallback();
    /// @}

    /// M: CC: BSP project ECC select phone control @{
protected:
    int calculateBspEccPreferRat(const sp<RfxMessage>& message, bool isAuto);
private:
    void updateBspEccSelectPhone(int slot);
    void resetBspEccSelectPhone();
    int getBspEccSelectPhone();
    void onBspEccSelectPhoneChanged(RfxStatusKeyEnum key,
            RfxVariant oldValue, RfxVariant newValue);
    /// @}

};


#endif // __RTC_ECC_CONTROLLER_H__
