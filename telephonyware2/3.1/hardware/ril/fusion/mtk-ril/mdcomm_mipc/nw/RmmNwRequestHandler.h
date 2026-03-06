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

#ifndef __RMM_NW_REQUEST_HANDLER_H__
#define __RMM_NW_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include "RmmNwHandler.h"
#include "RfxBarringInfoData.h"

/* RIL Network Enumeration */
typedef enum {
    CDMA_ROAMING_MODE_RADIO_DEFAULT = -1,
    CDMA_ROAMING_MODE_HOME = 0,
    CDMA_ROAMING_MODE_AFFILIATED = 1,
    CDMA_ROAMING_MODE_ANY = 2
} CDMA_ROAMING_MODE;

typedef enum
{
    RAT_SWITCH_MODE_NONE = 0,
    RAT_SWITCH_MODE_NSA_SCG,
    RAT_SWITCH_MODE_NSA_SCG_ALLOW,
    RAT_SWITCH_MODE_NSA_TAU,
    RAT_SWITCH_MODE_SA_ONLY,
    RAT_SWITCH_MODE_SA_TAU,
    RAT_SWITCH_MODE_BOTH,
    RAT_SWITCH_MODE_BOTH_TAU,
    RAT_SWITCH_MODE_ALL,
    RAT_SWITCH_MODE_RRC_STATE
} SMART_RAT_SWITCH_MODE;

class RmmNwRequestHandler : public RmmNwHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmNwRequestHandler);

    public:
        RmmNwRequestHandler(int slot_id, int channel_id);
        virtual ~RmmNwRequestHandler();

    protected:
        virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        /*
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
        */
    private:
        void handleRegistrationFailedEvent(const sp<RfxMclMessage>& msg);
        void handleWfcStateChanged(const sp<RfxMclMessage>& msg);
        void requestGetCellInfoList(const sp<RfxMclMessage>& msg);
        int updatePlmnInfo();
        bool handleGetDataContextIds(const sp<RfxMclMessage>& msg);
        void fillCidToPhysicalChannelConfig(RIL_PhysicalChannelConfig* pcc);
        void sendPhysicalChannelConfigs(bool forceUpdate = false);
        void handleCsNetworkStateEvent(const sp<RfxMclMessage>& msg);
        void handlePsNetworkStateEvent(const sp<RfxMclMessage>& msg);
        void requestSmartRatSwitch(const sp<RfxMclMessage>& msg);
        void requestGetSmartRatSwitch(const sp<RfxMclMessage>& msg);
        void requestGetBarringInfo(const sp<RfxMclMessage>& msg);
        void requestSetCdmaRoamingPreference(const sp<RfxMclMessage>& msg);
        void requestQueryCdmaRoamingPreference(const sp<RfxMclMessage>& msg);
        void requestSetUnsolCellInfoListRate(const sp<RfxMclMessage>& msg);
        void updateFemtoCellInfo(const sp<RfxMclMessage>& msg);
        void requestGetTs25Name(const sp<RfxMclMessage>& msg);
        void requestSetNrOption(const sp<RfxMclMessage>& msg);
        void requestSetNrdcState(const sp<RfxMclMessage>& msg);
        void requestGetNrdcState(const sp<RfxMclMessage>& msg);
        void triggerPollNetworkState();
        void onListenAtFromOemHook(const sp<RfxMclMessage>& msg);
        void updateNrDisplayRule(bool isRuleBySlot, int setRule, int setJpTimer1, int setJpTimer2);
        void onSync5GIconShowRuleWithOtherSlot(const sp<RfxMclMessage>& msg);
        int setNrOption(int option, int slot);
        int setEndcDeactivate(bool deactivate, int slot);
        int getNrOption(int slot);
        int isEndcDeactivate(int slot);

        //NA requirement
        void requestGetTOEInfo(const sp<RfxMclMessage>& msg);
        void requestSetCarrierAggregationMode(const sp<RfxMclMessage>& msg);
        void requestGetCALinkEnableStatus(const sp<RfxMclMessage>& msg);
        void requestSetCALinkEnableStatus(const sp<RfxMclMessage>& msg);
        void requestGetCALinkCapabilityList(const sp<RfxMclMessage>& msg);
        void requestGetLteData(const sp<RfxMclMessage>& msg);
        void requestGetLteRRCState(const sp<RfxMclMessage>& msg);
        void requestSetLteBandEnableStatus(const sp<RfxMclMessage>& msg);
        void requestGetBandPriorityList(const sp<RfxMclMessage>& msg);
        void requestSetBandPriorityList(const sp<RfxMclMessage>& msg);
        void requestSet4x4MimoEnabled(const sp<RfxMclMessage>& msg);
        void requestGet4x4MimoEnabled(const sp<RfxMclMessage>& msg);
        void requestGetLteBsrTimer(const sp<RfxMclMessage>& msg);
        void requestSetLteBsrTimer(const sp<RfxMclMessage>& msg);
        void requestGetLte1xRttCellList(const sp<RfxMclMessage>& msg);
        void requestClearLteAvailableFile(const sp<RfxMclMessage>& msg);
        void requestGetBandMode(const sp<RfxMclMessage>& msg);
        void requestGetCaBandMode(const sp<RfxMclMessage>& msg);
        void requestSetQamEnabled(const sp<RfxMclMessage>& msg);
        void requestGetQamEnabled(const sp<RfxMclMessage>& msg);
        void requestSetTm9Enabled(const sp<RfxMclMessage>& msg);
        void requestGetTm9Enabled(const sp<RfxMclMessage>& msg);
        void requestSetLteScanDuration(const sp<RfxMclMessage>& msg);
        void requestGetLteScanDuration(const sp<RfxMclMessage>& msg);
        void requestGetAllBandMode(const sp<RfxMclMessage>& msg);
        void requestSetNrBandMode(const sp<RfxMclMessage>& msg);
        void handleRefreshCellInfoListEvent(const sp<RfxMclMessage>& msg);
        void requestSetUsageSetting(const sp<RfxMclMessage>& msg);
        void requestGetUsageSetting(const sp<RfxMclMessage>& msg);
        void handleSetSignalStrengthReportingCriteriaRequest(const sp<RfxMclMessage>& msg);
        void requestSetN1ModeEnabled(const sp<RfxMclMessage>& msg);
        void requestGetN1ModeEnabled(const sp<RfxMclMessage>& msg);
        void requestSetNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg);
        void requestIsNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg);
        void requestIsCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg);
        void requestSetCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg);
        void requestGetNitz(const sp<RfxMclMessage>& msg);

        int mDataContextIds[16] = {-1};
        int mCellInfoListRetryCountLeft;
};

#endif
