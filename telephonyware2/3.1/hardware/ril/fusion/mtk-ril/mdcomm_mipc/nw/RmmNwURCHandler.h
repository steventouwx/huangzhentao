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

#ifndef __RMM_NW_URC_HANDLER_H__
#define __RMM_NW_URC_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RmmNwHandler.h"
#include "RfxBarringInfoData.h"

class RmmNwURCHandler : public RmmNwHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmNwURCHandler);

    public:
        RmmNwURCHandler(int slot_id, int channel_id);
        virtual ~RmmNwURCHandler();

    protected:
        virtual void onHandleUrc(const sp<RfxMclMessage>& msg);
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    private:
        void handleNitzTzInd(const sp<RfxMclMessage>& msg);
        void handleCsInd(const sp<RfxMclMessage>& msg);
        void handlePsInd(const sp<RfxMclMessage>& msg);
        void handleEcampuiChanged(const sp<RfxMclMessage>& msg);
        void handleCurrentRatInd(const sp<RfxMclMessage>& msg);
        void handleEgmssInd(const sp<RfxMclMessage>& msg);
        void sendDisableEcampuiChanged();
        void sendRegistrationFailedEvent(RIL_Domain domain);
        void handleEcellInd(const sp<RfxMclMessage>& msg);
        void requestSetUnsolCellInfoListRate(const sp<RfxMclMessage>& msg);
        void requestSetUnsolicitedResponseFilter(const sp<RfxMclMessage>& msg);
        void handlePseudoCellInd(const sp<RfxMclMessage>& msg);
        void handleImsEmergencySupportR9(const sp<RfxMclMessage>& msg);
        void handlePhysicalChannelConfigs(const sp<RfxMclMessage>& msg);
        void setUnsolResponseFilterPhyChlCfg(bool enable);
        void handleSignalStrength(const sp<RfxMclMessage>& msg);
        void handleLteNetworkInfoInd(const sp<RfxMclMessage>& msg);
        void handleModulationInfoInd(const sp<RfxMclMessage>& msg);
        void handleNetworkEventInd(const sp<RfxMclMessage>& msg);
        void handleNetworkInfoInd(const sp<RfxMclMessage>& msg);
        void handleCaInfoInd(const sp<RfxMclMessage>& msg);
        void handleConnectionStateReportInd(const sp<RfxMclMessage>& msg);
        void handleNrSwitchInd(const sp<RfxMclMessage>& msg);
        void handleOtaProvisionStatusInd(const sp<RfxMclMessage>& msg);
        void handleBarringInfoInd(const sp<RfxMclMessage>& msg);
        int convertOtaProvisionStatus(int rawState);
        void setUnsolResponseFilterSignalStrength(bool enable);
        void setUnsolResponseFilterNetworkState(bool enable);
        void setUnsolResponseFilterDataDomain(RIL_UnsolicitedResponseFilter filter);
        void setUnsolResponseFilterBarringInfo(bool enable);
        void setUnsolResponseFilterForAll(bool enable);
        void handleCievInd(const sp<RfxMclMessage>& msg);
        void handleFemtocellInfoInd(const sp<RfxMclMessage>& msg);
        void handleMccMncChanged(const sp<RfxMclMessage>& msg);
        void handleEmergencyBearerSupportInd(const sp<RfxMclMessage>& msg);
        void handleChannelInfoInd(const sp<RfxMclMessage>& msg);
        void handleNrCaBandInfoInd(const sp<RfxMclMessage>& msg);
        void handleNrSysInfoChanged(const sp<RfxMclMessage>& msg);
        void handleIWlanRegStateChanged(const sp<RfxMclMessage>& msg);
        void handleNetworkScanInd(const sp<RfxMclMessage>& msg);
        void handle5GUWChanged(const sp<RfxMclMessage>& msg);
        void handle5GUCChanged(const sp<RfxMclMessage>& msg);
        void handleFirstPlmnChanged(const sp<RfxMclMessage>& msg);
        void handleNwCfgInfo(const sp<RfxMclMessage>& msg);
        void handleCellularIdentifierDisclosure(const sp<RfxMclMessage>& msg);
        void handleSecurityAlgorithmsUpdated(const sp<RfxMclMessage>& msg);
        void updateNwCfgInfo();
        void sendNwCfgInfo(RIL_NwCfgInfo nw_cfg_info);
        void handleConnectionStateReportInternal(uint8_t mode, uint8_t state, uint8_t access);
        void updateConnectionStateReport();
        void requestSetSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg);
        void requestIsSecurityAlgorithmsUpdatedEnabled(const sp<RfxMclMessage>& msg);
#ifndef  MTK_HYP_DUAL_TELE_SUPPORT
        void handlePsIndMisc(int oldRegState, int old_rat);
        void handleCsIndMisc(int oldRegState, int old_rat);
        void updataPsIndicaton();
        void updataCsIndicaton();
#endif //MTK_HYP_DUAL_TELE_SUPPORT
        bool mReceivedEcampUI = false;
        bool enableCaInfoUrc = false;
        bool supportMccMncUrc = true;
        bool keep_rrc_urc = false;
};

#endif
