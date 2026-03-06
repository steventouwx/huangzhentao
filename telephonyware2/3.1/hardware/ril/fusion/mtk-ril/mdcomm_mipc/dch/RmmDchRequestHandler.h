/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2023. All rights reserved.
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

#ifndef __RMM_DCH_REQUEST_HANDLER_H__
#define __RMM_DCH_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxMessageId.h"
#include <utils/KeyedVector.h>

#define RFX_LOG_TAG "RmmDch"

class RmmDchRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmDchRequestHandler);
    public:
        RmmDchRequestHandler(int slot_id, int channel_id);
        virtual ~RmmDchRequestHandler();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
        virtual void onHandleCmd(const sp<RfxMclMessage>& msg);
        virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    private:
        void dchSetDcEnableReq();
        void handleDchEnableReq(const sp<RfxMclMessage>& msg);
        void handleDchEnableResponse(const sp<RfxMclMessage>& msg);
        void handleDchBootstrapCfgQueryCmd(const sp<RfxMclMessage>& msg);
        void handleEventClearCmd(const sp<RfxMclMessage>& msg);
        void dchBootstrapCfgQueryCmdRsp(const sp<RfxMclMessage>& msg);
        void handleDchCreateCmd(const sp<RfxMclMessage>& msg);
        void dchCreateCmdRsp(const sp<RfxMclMessage>& msg);
        void dchCreateSipReq(const sp<RfxMclMessage>& msg);
        void dchCreateSipResponse(const sp<RfxMclMessage>& msg);
        void dchModifySipReq(const sp<RfxMclMessage>& msg);
        void dchModifySipResponse(const sp<RfxMclMessage>& msg);
        void dchCloseSipReq(const sp<RfxMclMessage>& msg);
        void dchCloseSipResponse(const sp<RfxMclMessage>& msg);
        // MTK solution start @{
        void dchDataActPacketRouteRequest(const sp<RfxMclMessage>& msg);
        void dchDataActPacketRouteResponse(const sp<RfxMclMessage>& msg);
        void dchDataIfupPacketRouteRequest(const sp<RfxMclMessage>& msg);
        void dchDataIfupPacketRouteResponse(const sp<RfxMclMessage>& msg);
        void dchDataPathConfigSyncRequest(const sp<RfxMclMessage>& msg);
        void dchDataPathConfigSyncResponse(const sp<RfxMclMessage>& msg);
        void dchCreateDataChannelRequest(const sp<RfxMclMessage>& msg);
        void dchCreateDataChannelResponse(const sp<RfxMclMessage>& msg);
        void dchCloseDataChannelRequest(const sp<RfxMclMessage>& msg);
        void dchCloseDataChannelResponse(const sp<RfxMclMessage>& msg);
        /* Set network interface up and address */
        void handleEventSyncRouteParams(const sp<RfxMclMessage>& msg);
        void dchSetNetwork(uint8_t* client_ip);
        void ril_data_setaddr(int s, struct ifreq *ifr, const char *addr);
        void ril_data_setflags(int s, struct ifreq *ifr, int set, int clr);
        // MTK solution end }@

        android::KeyedVector<uint8_t, uint16_t> mBootstrapCfgQueryCallTxIds;
        android::KeyedVector<uint8_t, uint16_t> mCreateCmdCallTxIds;
        int mNetType = 0;
        int mNetIf = 0;
        int mQueue = 0;
};

#endif
