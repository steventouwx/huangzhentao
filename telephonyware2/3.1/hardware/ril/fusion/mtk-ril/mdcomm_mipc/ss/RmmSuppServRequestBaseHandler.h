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

#ifndef __RMM_SUPP_SERV_REQUEST_BASE_HANDLER_H__
#define __RMM_SUPP_SERV_REQUEST_BASE_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "SuppServDef.h"
#include "RfxCallForwardInfoData.h"
#include "RfxCallForwardInfosData.h"
#include "RfxCallForwardInfoExData.h"
#include "RfxCallForwardInfosExData.h"

#ifdef TAG
#undef TAG
#endif
#define TAG "RmmSSBaseHandler"

class RmmSuppServRequestBaseHandler : public RfxBaseHandler {
    public:
        RmmSuppServRequestBaseHandler(int slot_id, int channel_id);
        virtual ~RmmSuppServRequestBaseHandler();

    protected:
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);

        int requestMipcVoid(const sp<RfxMclMessage>& msg, uint16_t mipcMsgId);
        void handleMipcVoidCnf(const sp<RfxMclMessage>& msg, const char *logTag, bool copyData = false);

        int requestSendUssd(const sp<RfxMclMessage>& msg);
        void handleSendUssdCnf(const sp<RfxMclMessage>& msg);
        void sendUssdFailureReport(const sp<RfxMclMessage>& msg, RIL_Errno ret);

        int requestCancelUssd(const sp<RfxMclMessage>& msg);

        int requestClirOperation(const sp<RfxMclMessage>& msg);
        void handleClirOperationCnf(const sp<RfxMclMessage>& msg);

        int requestCallForwardOperation(const sp<RfxMclMessage>& msg, CallForwardOperationE op);
        void handleCallForwardOperationCnf(const sp<RfxMclMessage>& msg, CallForwardOperationE op);

        int requestCallForwardExOperation(const sp<RfxMclMessage>& msg, CallForwardOperationE op);
        void handleCallForwardExOperationCnf(
                const sp<RfxMclMessage>& msg, CallForwardOperationE op);

        int requestCallWaitingOperation(const sp<RfxMclMessage>& msg, CallWaitingOperationE op);
        void handleCallWaitingOperationCnf(const sp<RfxMclMessage>& msg, CallWaitingOperationE op);

        int requestCallBarring(const sp<RfxMclMessage>& msg, CallBarringOperationE op);
        void handleCallBarringCnf(const sp<RfxMclMessage>& msg, CallBarringOperationE op);

        int requestChangeBarringPassword(const sp<RfxMclMessage>& msg);
        void handleChangeBarringPasswordCnf(const sp<RfxMclMessage>& msg);

        int requestQueryClip(const sp<RfxMclMessage>& msg);
        void handleQueryClipCnf(const sp<RfxMclMessage>& msg);

        int requestSetClip(const sp<RfxMclMessage>& msg);
        void handleSetClipCnf(const sp<RfxMclMessage>& msg);

        int requestColpOperation(const sp<RfxMclMessage>& msg);
        void handleColpOperationCnf(const sp<RfxMclMessage>& msg);

        int requestColrOperation(const sp<RfxMclMessage>& msg);
        void handleColrOperationCnf(const sp<RfxMclMessage>& msg);

        int requestSendCnap(const sp<RfxMclMessage>& msg);
        void handleSendCnapCnf(const sp<RfxMclMessage>& msg);

        int requestSetSuppSvcNotification(const sp<RfxMclMessage>& msg);

        int requestSetupXcapUserAgentString(const sp<RfxMclMessage>& msg);

        int requestSetXcapConfig(const sp<RfxMclMessage>& msg);

        int requestGetXcapStatus(const sp<RfxMclMessage>& msg);

        int requestGetBarringCalls(const sp<RfxMclMessage>& msg);
        void handleGetBarringCallsCnf(const sp<RfxMclMessage>& msg);

        int requestSetBarringCalls(const sp<RfxMclMessage>& msg);
        void handleSetBarringCallsCnf(const sp<RfxMclMessage>& msg);

        void handleErrorMessage(RfxMipcData* mipcData, uint16_t errNo);
        void sleepMsec(long long msec);

    private:
        /**
         * When EM wants to reset XCAP config, it should be trigger by oemhook using
         * reset and resetdone command. And except the reset cmd, all properties only need
         * to set the property for UI to get status.
         */
        bool isResetSession;
};

#endif
