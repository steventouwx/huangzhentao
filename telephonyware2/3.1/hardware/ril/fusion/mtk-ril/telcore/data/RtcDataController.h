/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2016. All rights reserved.
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

#ifndef __RTC_DATA_CONTROLLER_H__
#define __RTC_DATA_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxAction.h"
#include "RfxController.h"
#include "RfxDataCallResponseData.h"
#include "RfxFdModeData.h"
#include "RfxIaApnData.h"
#include "RfxIntsData.h"
#include "RfxLceDataResponseData.h"
#include "RfxLceStatusResponseData.h"
#include "RfxLinkCapacityEstimateData.h"
#include "RfxLinkCapacityReportingCriteriaData.h"
#include "RfxKeepaliveRequestData.h"
#include "RfxKeepaliveStatusData.h"
#include "RfxPlmnMvnoData.h"
#include "RfxSetDataProfileData.h"
#include "RfxSetupDataCallData.h"
#include "RfxSlicingConfigResponseData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include <list>
#include <map>


/*****************************************************************************
 * Class RtcDataController
 *************************************************
 ****************************/

#define DATA_SETTING_NUMBERS   5
#define SKIP_DATA_SETTINGS    -2
#define INVALID_VALUE         -1
#define DEACTIVATE_REASON_HANDOVER 3

typedef enum {
    MOBILE_DATA      = 0,
    ROAMING_DATA     = 1,
    DEFAULT_DATA_SIM = 2,
    DOMESTIC_ROAMING_DATA = 3,
    INTERNATIONAL_ROAMING_DATA = 4
} DATA_SETTING_ITEM;

class RtcDataController : public RfxController {
    RFX_DECLARE_CLASS(RtcDataController);  // Required: declare this class

public:
    RtcDataController();

    virtual ~RtcDataController();

// Override
protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState);
    virtual bool onPreviewMessage(const sp<RfxMessage>& message);
    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);
    virtual void preCheckIfNeedDisableIms(const sp<RfxMessage>& message);
    virtual void handleSyncDataSettingsToMD(const sp<RfxMessage>& response);
    virtual void handleSetupDataRequest(const sp<RfxMessage>& message);
    virtual void handleSetupDataResponse(const sp<RfxMessage>& response);
    virtual void handleDeactivateDataRequest(const sp<RfxMessage>& message);
    virtual void handleSetPreferredDataModem(const sp<RfxMessage>& message);
    virtual void onImsConfirmed(const sp<RfxMessage> message);
    virtual void requestResumeIms(const sp<RfxMessage> message);
private:
    // Private functions
    void registerForStatusChange();
    void onWorldModeStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    bool canHandleRequest(const sp<RfxMessage>& message);
    void onModemOffStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onUiccMccMncChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onImsiChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onSpnChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onGid1Changed(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onPnnChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onImpiChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onCardTypeChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);

    void onConnectionStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onMtkRadioConnectionChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onDataSettingStatusChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onServiceStateChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void onAllowedChanged(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    bool isPreferredDataMode();
    void enqueueForPreferredDataMode(const sp<RfxMessage>& message);
    void dequeueForPreferredDataMode(const sp<RfxMessage>& message);
    int getProfileID(int apnTypeId);
    void notifyPlmnMvnoData(String8& strGsmPlmn, String8& strCdmaPlmn);
    void notifyEmptyPlmnMvnoData();
    void resendPreferData();
    void updateQualifiedNetworks(RfxVariant& new_value);

private:
    bool isUnderCapabilitySwitch;
    int requestTokenIdForDisableIms;
    int transIdForDisableIms;
    int mIsPreferredDataMode;
    int mPreferredTransport;
    std::map<int, int> m_mapProfileIdToken; // <ProfileId, Token>
    std::map<int, int> m_mapCidProfileId; // <Cid, ProfileId>
};

#endif /* __RTC_DATA_CONTROLLER_H__ */
