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

#ifndef __RMM_NW_REAL_TIME_REQUEST_HANDLER_H__
#define __RMM_NW_REAL_TIME_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include "RmmNwHandler.h"

class RmmNwRealTimeRequestHandler : public RmmNwHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmNwRealTimeRequestHandler);

    public:
        RmmNwRealTimeRequestHandler(int slot_id, int channel_id);
        virtual ~RmmNwRealTimeRequestHandler();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        /*
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
        */

    private:
        void requestOperator(const sp<RfxMclMessage>& msg);
        void requestVoiceRegistrationState(const sp<RfxMclMessage>& msg);
        void requestDataRegistrationState(const sp<RfxMclMessage>& msg);
        void combineVoiceRegState(const sp<RfxMclMessage>& msg);
        void updateServiceStateValue();
        void sendVoiceRegResponse(const sp<RfxMclMessage>& msg);
        int convertToAndroidRegState(unsigned int uiRegState, bool isVoice);
        void combineDataRegState(const sp<RfxMclMessage>& msg);
        void sendDataRegResponse(const sp<RfxMclMessage>& msg);
        void requestQueryNetworkSelectionMode(const sp<RfxMclMessage>& msg);
        void requestIWlanRegistrationState(const sp<RfxMclMessage>& msg);
        int requestVoiceRegistrationStateCdma(const sp<RfxMclMessage>& msg);
        void resetVoiceRegStateCache(RIL_CS_REG_STATE_CACHE *pCache, RIL_CACHE_GROUP source);
        void resetDataRegStateCache(RIL_PS_REG_STATE_CACHE *dataCache, RIL_CACHE_GROUP source);
#ifdef  MTK_HYP_DUAL_TELE_SUPPORT
        bool skipDCNRRestrictedState(const char *mccmnc);
#endif //MTK_HYP_DUAL_TELE_SUPPORT
        int m_emergency_only = 0;
        int m_cdma_emergency_only = 0;
        int m_ims_ecc_only = 0;
};

#endif
