/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

#ifndef __RTM_ECC_NUMBER_CONTROLLER_H__
#define __RTM_ECC_NUMBER_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include "RfxAtLine.h"
#include "rfx_properties.h"
#include <utils/String8.h>
#include <utils/Vector.h>
#include "EccNumberSource.h"
#include "EccNumberPreference.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RtmEccNumberController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtmEccNumberController);

public:
    RtmEccNumberController();
    virtual ~RtmEccNumberController();

// Override
protected:
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual void onInit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(
            const sp<RfxMessage>& message, bool isModemPowerOff, int radioState);

// External Method
public:

    // Get the emergency call routing for the corresponding emergency number
    //
    // RETURNS:
    //     UNKNOWN if the number is not recognized as emergency emergency number in RILD
    //     EMERGENCY if the number should dial using emergency call routing
    //     NORMAL if the number should dial using normal call routing
    EmergencyCallRouting getEmergencyCallRouting(String8 number);

    // Check if given number is a emergency number or not based on SIM/Network/XML/Default
    // emergency number list.
    // NOTE: For speical emergency number which should dial using normal call
    //       This API will return true
    //
    // RETURNS: true if number is emergency number
    bool isEmergencyNumber(String8 number);

    // Get service category of emergency number
    //
    // RETURNS: service category value defined 3gpp 22.101, Section 10 - Emergency Calls
    int getServiceCategory(String8 number);

    // Check if the given number is GSM only number
    //
    // RETURNS:
    bool isGsmOnlyNumber(String8 number);

    // Check if the given number is GSM preferred number
    //
    // RETURNS:
    bool isGsmPreferredNumber(String8 number);

    // Check if the given number is CDMA preferred number
    //
    // RETURNS:
    bool isCdmaPreferredNumber(String8 number);

    // Set ECC preference List for test cases
    //
    // RETURNS:
    void setPreferenceList(int preferenceType, String8 preferenceList);

    // Get ECC list(Test case only)
    Vector<EmergencyNumber> * getEccList();

    String8 getPlmn(int slot);

    bool isSimInserted();
    void updateEmergencySourcesForAllSimStateChanges(bool isSimInserted) ;
    void syncEccToModem();
    void updateEccToFramework();

private:
    void onCardTypeChanged(RfxStatusKeyEnum key,
            RfxVariant oldValue, RfxVariant newValue);
    void onPlmnChanged(RfxStatusKeyEnum key,
            RfxVariant old_value, RfxVariant value);
    void onSimStateChanged(RfxStatusKeyEnum key,
            RfxVariant old_value, RfxVariant value);
    void onConnectionStateChanged(RfxStatusKeyEnum key,
            RfxVariant old_value, RfxVariant value);
    void onGsmUiccMccMncChanged(RfxStatusKeyEnum key,
            RfxVariant old_value, RfxVariant value);
    void onCdmaUiccMccMncChanged(RfxStatusKeyEnum key,
            RfxVariant old_value, RfxVariant value);
    void onCapabilitySwitchStateChanged(RfxStatusKeyEnum key,
            RfxVariant old_value, RfxVariant value);
    void onScreenStateChanged(RfxStatusKeyEnum key,
            RfxVariant oldValue, RfxVariant newValue);
    void handleSetEccNum(const sp<RfxMessage>& message);
    void handleGetEccNum(const sp<RfxMessage>& message);
    void handleModemEccChange(const sp<RfxMessage>& message);
    void initEmergencyNumberSource();
    bool isEccMatchInList(String8 number, String8 eccList);
    void createEmergencyNumberListResponse(RIL_EmergencyNumber *data);
    void freeEmergencyNumberListResponse(RIL_EmergencyNumber *data);
    String8 getSourcesString(int sources);
    void updateSpecialEmergencyNumber();
    void delaySetFwkReady();
    String8 convertPlmnForRoaming(String8 plmn);
    bool isCtCard();
    void dumpEccList();
    void dumpApEccList();
    void testEcc();
    bool updateEmergencySourcesForPlmnChange(bool isSimInsert);
    bool updateEmergencySourcesFromPeer(String8 plmn);
    void removeNonEmergencyNumbers();
    bool hasSimInAnySlot();
    bool updateForSimStateChanges(bool isSimInserted);
    void createApEmergencyNumberList(RIL_EmergencyNumber *data);
    void freeApEmergencyNumberList(RIL_EmergencyNumber *data);
    void handleGetEccResponse(const sp<RfxMessage>& message);

private:
    bool mIsSimInsert;
    Vector<EccNumberSource *> mApEccNumberSourceList;
    XmlEccNumberSource *mXmlEccSource;
    FrameworkEccNumberSource *mFrameworkEccSource;
    OemPropertyEccNumberSource *mPropertyEccSource;
    TestEccNumberSource *mTestEccSource;
    ModemEccNumberSource *mModemEccSource;
    Vector<EmergencyNumber> mEccList; // Ecc list including AP and modem
    Vector<EmergencyNumber> mApEccList; // Ecc list including AP and will sync to modem
    bool mIsFwkReady; // Indicate if framework is ready to receive emergency number
    bool mIsPendingUpdate;
    TimerHandle mTimerHandle;
    EccNumberPreference *mEccPreference;
};

#endif /* __RTM_ECC_NUMBER_CONTROLLER_H__ */
