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

#ifndef __RMC_NETWORK_REQUEST_HANDLER_H__
#define __RMC_NETWORK_REQUEST_HANDLER_H__

#include "RmcNetworkHandler.h"
#include "RfxCellInfoData.h"
#include "RfxNetworkScanData.h"
#include "RfxNetworkScanResultData.h"
#include "RfxSsrcData.h"
#include "RfxSscData.h"
#include "RfxBarringInfoData.h"
#include "RfxRegistrationFailedData.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcNwReqHdlr"

/* RIL Network Enumeration */
typedef enum
{
    GSM_BAND_900    = 0x02,
    GSM_BAND_1800   = 0x08,
    GSM_BAND_1900   = 0x10,
    GSM_BAND_850    = 0x80
} GSM_BAND_ENUM;

typedef enum
{
    UMTS_BAND_I     = 0x0001,
    UMTS_BAND_II    = 0x0002,
    UMTS_BAND_III   = 0x0004,
    UMTS_BAND_IV    = 0x0008,
    UMTS_BAND_V     = 0x0010,
    UMTS_BAND_VI    = 0x0020,
    UMTS_BAND_VII   = 0x0040,
    UMTS_BAND_VIII  = 0x0080,
    UMTS_BAND_IX    = 0x0100,
    UMTS_BAND_X     = 0x0200
} UMTS_BAND_ENUM;

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

typedef enum
{
    UPDATE_CONNECTION_STATE_REASON_NONE = 0,
    UPDATE_CONNECTION_STATE_REASON_SCREEN_ON = 1,
    UPDATE_CONNECTION_STATE_REASON_NSA_RULE_CHANGED = 2,
} UPDATE_CONNECTION_STATE_REASON;

#define RIL_NW_INIT_INT         {0, 0, 0, 0}

class RmcNetworkRequestHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcNetworkRequestHandler);

    public:
        RmcNetworkRequestHandler(int slot_id, int channel_id);
        virtual ~RmcNetworkRequestHandler();

        static bool isInService(int regState) {
            if (regState == 1 || regState == 5) {
                return true;
            }
            return false;
        }

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

        virtual void onHandleTimer();

        virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    private:
        void requestSignalStrength(const sp<RfxMclMessage>& msg, bool forceUpdate);
        void requestSetNetworkSelectionAutomatic(const sp<RfxMclMessage>& msg);
        void requestSetNetworkSelectionManual(const sp<RfxMclMessage>& msg);
        void requestSetNetworkSelectionManualWithRan(const sp<RfxMclMessage>& msg);
        void requestSetNetworkSelectionManualWithAct(const sp<RfxMclMessage>& msg);
        void requestSetBandMode(const sp<RfxMclMessage>& msg);
        void requestQueryAvailableBandMode(const sp<RfxMclMessage>& msg);
        void requestSetLocationUpdates(const sp<RfxMclMessage>& msg);
        void requestGetCellInfoList(const sp<RfxMclMessage>& msg);
        void requestSetCellInfoListRate(const sp<RfxMclMessage>& msg);
        void requestGetPOLCapability(const sp<RfxMclMessage>& msg);
        void requestGetPOLList(const sp<RfxMclMessage>& msg);
        void requestSetPOLEntry(const sp<RfxMclMessage>& msg);
        void requestSetCdmaRoamingPreference(const sp<RfxMclMessage>& msg);
        void requestQueryCdmaRoamingPreference(const sp<RfxMclMessage>& msg);
        void requestAbortFemtocellList(const sp<RfxMclMessage>& msg);
        void requestSelectFemtocell(const sp<RfxMclMessage>& msg);
        void requestSetUnsolicitedResponseFilter(const sp<RfxMclMessage>& msg);
        void requestQueryFemtoCellSystemSelectionMode(const sp<RfxMclMessage>& msg);
        void requestSetFemtoCellSystemSelectionMode(const sp<RfxMclMessage>& msg);
        void requestSetServiceState(const sp<RfxMclMessage>& msg);
        void requestSetPseudoCellMode(const sp<RfxMclMessage>& msg);
        void requestGetPseudoCellInfo(const sp<RfxMclMessage>& msg);
        void updatePseudoCellMode();
        void setUnsolResponseFilterSignalStrength(bool enable);
        void setUnsolResponseFilterNetworkState(bool enable);
        void setUnsolResponseFilterLinkCapacityEstimate(bool enable);
        void triggerPollNetworkState();
        int isEnableModulationReport();
        void handleConfirmRatBegin(const sp<RfxMclMessage>& msg);
        void handleCsNetworkStateEvent(const sp<RfxMclMessage>& msg);
        void handlePsNetworkStateEvent(const sp<RfxMclMessage>& msg);
        void setRoamingEnable(const sp<RfxMclMessage>& msg);
        void getRoamingEnable(const sp<RfxMclMessage>& msg);
        void requestSetLteReleaseVersion(const sp<RfxMclMessage>& msg);
        void requestGetLteReleaseVersion(const sp<RfxMclMessage>& msg);
        void requestSetSignalStrengthReportingCriteria(const sp<RfxMclMessage>& msg);
        int setSignalStrengthReportingCriteriaSingle(RIL_SignalStrength_Reporting_Criteria * ssrc);
        void requestSetSystemSelectionChannels(const sp<RfxMclMessage>& msg);
        void requestGetSystemSelectionChannels(const sp<RfxMclMessage>& msg);
        void requestGetTs25Name(const sp<RfxMclMessage>& msg);
        bool handleGetDataContextIds(const sp<RfxMclMessage>& msg);
        void registerCellularQualityReport(const sp<RfxMclMessage>& msg); // MUSE WFC requirement
        void requestGetSuggestedPlmnList(const sp<RfxMclMessage>& msg);
        void requestSmartRatSwitch(const sp<RfxMclMessage>& msg);
        void requestGetSmartRatSwitch(const sp<RfxMclMessage>& msg);
        void smartRatSwitchBack();

        // NR request
        void requestConfigA2Offset(const sp<RfxMclMessage>& msg);
        void requestConfigB1Offset(const sp<RfxMclMessage>& msg);
        void requestDeactivateNrScgCommunication(const sp<RfxMclMessage>& msg);
        void requestGetDeactivateNrScgCommunication(const sp<RfxMclMessage>& msg);
        void requestSetNrOption(const sp<RfxMclMessage>& msg);
        void requestSetNrdcState(const sp<RfxMclMessage>& msg);
        void requestGetNrdcState(const sp<RfxMclMessage>& msg);
        void requestSearchStoreFrenquencyInfo(const sp<RfxMclMessage>& msg);
        void requestSearchRat(const sp<RfxMclMessage>& msg);
        void requestSetBackgroundSearchTimer(const sp<RfxMclMessage>& msg);
        void onListenAtFromOemHook(const sp<RfxMclMessage>& msg);
        void updateConnectionStateReport(int reason);
        void requestGetBarringInfo(const sp<RfxMclMessage>& msg);
        void handleRegistrationFailedEvent(const sp<RfxMclMessage>& msg);

        void updateEndcCapability();
        void updateNrDisplayRule(bool isRuleBySlot, int setRule, int setJpTimer1, int setJpTimer2);
        void onSync5GIconShowRuleWithOtherSlot(const sp<RfxMclMessage>& msg);
        int setNrOption(int option);
        int setEndcDeactivate(bool deactivate);
        int getNrOption();
        int isEndcDeactivate();

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
        void requestGetAllBandModeWithoutNr(const sp<RfxMclMessage>& msg);
        void requestSetUsageSetting(const sp<RfxMclMessage>& msg);
        void requestGetUsageSetting(const sp<RfxMclMessage>& msg);
        void requestSetN1ModeEnabled(const sp<RfxMclMessage>& msg);
        void requestGetN1ModeEnabled(const sp<RfxMclMessage>& msg);
        void requestSetNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg);
        void requestIsNullCipherAndIntegrityEnabled(const sp<RfxMclMessage>& msg);
        void requestIsCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg);
        void requestSetCellularIdentifierTransparencyEnabled(const sp<RfxMclMessage>& msg);
        void requestSetSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg);
        void requestIsSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg);

        void currentPhysicalChannelConfigs(bool forceQuery);
        void fillCidToPhysicalChannelConfig(RIL_PhysicalChannelConfig* pcc);
        bool getPhyChlCfgInfoV1(RIL_RadioTechnology curRat, RIL_PhysicalChannelConfig * pInfo, int* count);
        bool getPhyChlCfgInfoV2ByQuery(RIL_RadioTechnology curRat, RIL_PhysicalChannelConfig * pInfo, int* count);
        bool getPhyChlCfgInfoV2ByUrc(RIL_RadioTechnology curRat, RIL_PhysicalChannelConfig * pInfo, int* count);
        int getPhyChlCfgMode();
        bool needReportPccEmptyList();
        void setUnsolResponseFilterPhyChlCfg(bool enable);
        void updatePhyChlCfgUrcByQuery();

    protected:
        unsigned int m_slot_id;
        int m_channel_id;
        int m_emergency_only;

        //Add for band de-sense feature.
        int bands[4] = RIL_NW_INIT_INT;

        int enableCaInfoUrc; // 0: disable ecainfo; 1: enable ecainfo; 2: enable ecainfo depend on dmftc02
        bool support_eops_urc = true;

        //mode:1, all info need nw fill in
        //mode:2, md reprot both pcell and scell info to NW by at command
        int mPhyChlCfgMode = -1;
        int mCurrentLceMode = -1;
};

#endif
