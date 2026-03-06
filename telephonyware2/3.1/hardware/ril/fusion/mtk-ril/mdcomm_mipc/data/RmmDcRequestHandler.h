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

#ifndef __RMM_DC_REQUEST_HANDLER_H__
#define __RMM_DC_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RmmDataDefs.h"
#include "RmmDcEventHandler.h"
#include "RmmDcPdnManager.h"

/*****************************************************************************
 * Class RmmDcRequestHandler
 *****************************************************************************/
class RmmDcRequestHandler : public RfxBaseHandler, public NetObject {
    RFX_DECLARE_HANDLER_CLASS(RmmDcRequestHandler);

    public:
        RmmDcRequestHandler(int slot_id, int channel_id);
        virtual ~RmmDcRequestHandler();

    public:
        void handleInterfaceInfoUpdated(bool bSuccess, void* data);

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);

    private:
        void handleSetupDataCallRequest(const sp<RfxMclMessage>& msg);
        void handleDeactivateDataCallRequest(const sp<RfxMclMessage>& msg);
        void handleDataCallListRequest(const sp<RfxMclMessage>& msg);
        int deactivateDataCall(int aid);
        void handlePdnChange(const sp<RfxMclMessage>& msg);
        void handleSyncApnTableRequest(const sp<RfxMclMessage>& msg);
        void handleSetInitialAttachApnRequest(const sp<RfxMclMessage>& msg);
        void handleSyncDataSettingsToMdRequest(const sp<RfxMclMessage>& msg);
        void handleResetMdDataRetryCount(const sp<RfxMclMessage>& msg);
        void handleResendSyncDataSettingsToMd(const sp<RfxMclMessage>& msg);
        void handleResetAllConnections(const sp<RfxMclMessage>& msg);
        void handleSetPreferredDataModem(const sp<RfxMclMessage>& msg);
        bool isValidInitialAttachApn(const char* apn);
        void handleStartKeepaliveRequest(const sp<RfxMclMessage>& msg);
        void handleStopKeepaliveRequest(const sp<RfxMclMessage>& msg);
        void handleSetFdModeRequest(const sp<RfxMclMessage>& msg);
        void handleStartLceRequest(const sp<RfxMclMessage>& msg);
        void handleStopLceRequest(const sp<RfxMclMessage>& msg);
        void handlePullLceDataRequest(const sp<RfxMclMessage>& msg);
        void handleLceStatusChanged(const sp<RfxMclMessage>& msg);
        void handleSetLinkCapacityReportingCriteriaRequest(const sp<RfxMclMessage>& msg);
        void handleSendDeviceStateRequest(const sp<RfxMclMessage>& msg);
        void handleEnableDsdaIndication(const sp<RfxMclMessage>& msg);
        void handleGetDsdaStatus(const sp<RfxMclMessage>& msg);
        void handleGetSlicingConfig(const sp<RfxMclMessage>& msg);
        void handleUpdateSlicingConfig(const sp<RfxMclMessage>& msg);
        void handleUpdateDataRatRequest(const sp<RfxMclMessage>& msg);
        void hanleEnableNetworkRejectCauseIndication(const sp<RfxMclMessage>& msg);
        void handleReleaseRfOccupy(const sp<RfxMclMessage>& msg);

        // For Multi-SIM
        void handleDcAttachDetachRequest(const sp<RfxMclMessage>& msg, int type);

    private:
        RmmDcEventHandler *m_pRmmDcEventHandler;
        RmmDcPdnManager *m_pPdnManager;
        bool mIsUserBuild;
};
#endif /* __RMM_DC_REQUEST_HANDLER_H__ */

