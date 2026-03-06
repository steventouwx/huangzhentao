/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2017. All rights reserved.
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
#ifndef __RMM_CDMA_SMS_HANDLER__
#define __RMM_CDMA_SMS_HANDLER__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "common/RmmChannelHandler.h"


/*****************************************************************************
 * Class RmmCdmaSmsReqHandler
 *****************************************************************************/
class RmmCdmaSmsReqHandler : public RmmBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmCdmaSmsReqHandler);

// Constructor / Destructor
public:
    RmmCdmaSmsReqHandler(int slotId, int channelId);
    virtual ~RmmCdmaSmsReqHandler();

// Override
protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleResponse(const sp<RfxMclMessage>& msg);

private:
    void handleWriteSmsToRuim(const sp<RfxMclMessage>& msg);
    void handleWriteSmsToRuimRsp(const sp<RfxMclMessage>& msg);
    void handleDeleteSmsOnRuim(const sp<RfxMclMessage>& msg);
    void handleDeleteSmsOnRuimRsp(const sp<RfxMclMessage>& msg);
    void handleGetSmsRuimMemStatus(const sp<RfxMclMessage>& msg);
    void handleGetSmsRuimMemStatusRsp(const sp<RfxMclMessage>& msg);
    void handleCdmaCbSmsActivation(const sp<RfxMclMessage>& msg);
    void handleCdmaCbSmsCfgRsp(const sp<RfxMclMessage>& msg);
    void handleSetCdmaCbSmsCfg(const sp<RfxMclMessage>& msg);
    void handleGetCdmaCbSmsCfg(const sp<RfxMclMessage>& msg);
    void handleGetCdmaCbSmsCfgRsp(const sp<RfxMclMessage>& msg);

    void setTag(String8 tag) {mTag = tag;}

private:
    String8 mTag;
};

/*****************************************************************************
 * Class RmmCdmaMoSmsReqHandler
 *****************************************************************************/
class RmmCdmaMoSmsReqHandler : public RmmBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmCdmaMoSmsReqHandler);

// Constructor / Destructor
public:
    RmmCdmaMoSmsReqHandler(int slotId, int channelId);
    virtual ~RmmCdmaMoSmsReqHandler();

// Override
protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

private:
    bool isCtSimCard();
    void handleSendCdmaSms(const sp<RfxMclMessage>& msg, bool needBearerReplyOpt);
    void handleSendCdmaSmsRsp(const sp<RfxMclMessage>& msg, const sp<RfxMipcData> mipcData);

private:
    const int TELESERVICE_WMT = 0x1002;
    const int TELESERVICE_WEMT = 0x1005;
};

/*****************************************************************************
 * Class RmmCdmaMtSmsAckReqHandler
 *****************************************************************************/
class RmmCdmaMtSmsAckReqHandler : public RmmBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmCdmaMtSmsAckReqHandler);

// Constructor / Destructor
public:
    RmmCdmaMtSmsAckReqHandler(int slotId, int channelId);
    virtual ~RmmCdmaMtSmsAckReqHandler();

// Override
protected:
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

private:
    void setTag(String8 tag) {mTag = tag;}
    void handleCdmaSmsAcknowledge(const sp<RfxMclMessage>& msg);
    void sendCdmaSmsErrorAck(const sp<RfxMclMessage>& msg);

private:
    String8 mTag;
};

/*****************************************************************************
 * Class RmmCdmaSmsUrcHandler
 *****************************************************************************/
class RmmCdmaSmsUrcHandler : public RmmBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmCdmaSmsUrcHandler);

// Constructor / Destructor
public:
    RmmCdmaSmsUrcHandler(int slotId, int channelId);
    virtual ~RmmCdmaSmsUrcHandler();

// Override
protected:
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);
    virtual void onHandleUrc(const sp<RfxMclMessage>& msg);

private:
    void setTag(String8 tag) {mTag = tag;}
    void handleNewCdmaSmsCmd(const sp<RfxMclMessage>& msg);
    void handleNewCdmaSmsUrc(const sp<RfxMclMessage>& msg);
    void handleCdmaPendingVMI(const sp<RfxMclMessage>& msg);

private:
    const int TELESERVICE_VMI = 0x1003;
    String8 mTag;
};
#endif /* __RMM_CDMA_SMS_HANDLER__ */
