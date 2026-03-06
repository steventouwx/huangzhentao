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

#ifndef __RMM_ECALL_REQUEST_HANDLER_H__
#define __RMM_ECALL_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "telephony/ril.h"
#include "RfxRawData.h"
#include <telephony/mtk_ril_ivt.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "mipc_msg_tlv_const.h"
#ifdef __cplusplus
}
#endif

//#define RFX_LOG_TAG "RmmEcallReqHandler"

class RmmEcallRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmEcallRequestHandler);

    public:
        RmmEcallRequestHandler(int slot_id, int channel_id);
        virtual ~RmmEcallRequestHandler();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
        void responseVoidDataToTcl(const sp<RfxMclMessage>& msg);
        void responseVoidDataToTcl(const sp<RfxMclMessage>& msg, RIL_Errno err);

    private:
        //std::string mLog_tag;
        String8 mEccListWithCard;
        String8 mEccListNoCard;
        bool mIsSimInsert;
        String8 mGsmEcc;

    private:
        int handleEcallFastMakeEcall(const sp<RfxMclMessage>& msg);
        int handleEcallUpdateMsd(const sp<RfxMclMessage>& msg);
        int handleEcallResetIvs(const sp<RfxMclMessage>& msg);
        int handleEcallSetRegState(const sp<RfxMclMessage>& msg);
        int handleEcallSetOprtMode(const sp<RfxMclMessage>& msg);
        int handleEcallGetOprtMode(const sp<RfxMclMessage>& msg);
        int handleEcallGetSimInfo(int slotId);
        int handleEcallSetTestAddr(const sp<RfxMclMessage>& msg);
        int handleEcallSetReconfAddr(const sp<RfxMclMessage>& msg);
        int handleEcallSetAddrPri(const sp<RfxMclMessage>& msg);

        void requestGetOprtModeResponse(const sp<RfxMclMessage>& msg);

        int checkEccNumberAndServiceCategory(char* number, const sp<RfxMclMessage>& msg, RIL_ECall_Variant ecall_variant, int is_number);
        bool isEmergencyNumber(String8 number);
        int getServiceCategory(String8 number);
        bool isEccMatchInList(String8 number, String8 eccList);

        std::string IdToString(int request);

};
#endif
