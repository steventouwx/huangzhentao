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

// External SIM [Start]
#ifndef __RMM_VSIM_REQUEST_HANDLER_H__
#define __RMM_VSIM_REQUEST_HANDLER_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RfxMclMessage.h"

#define VSIM_TRIGGER_RESET      0
#define VSIM_TRIGGER_PLUG_IN    1
#define VSIM_TRIGGER_PLUG_OUT   2

#define VSIM_MD_WAITING_RESET   -1
#define VSIM_MD_WAITING_ATR     1
#define VSIM_MD_WAITING_APDU    0

/*****************************************************************************
 * Class RmmVsimRequestHandler
 *****************************************************************************/

class RmmVsimRequestHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmVsimRequestHandler);

public:
    RmmVsimRequestHandler(int slot_id, int channel_id);
    virtual ~RmmVsimRequestHandler();

    // Process Request here
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    static int isSingeleRfRemoteSimSupport();
    static int getAkaSimSlot();
    static void setMdWaitingResponse(int slot_id, int waiting);
    static int getMdWaitingResponse(int slot_id);
    static void setVsimPlugInOutEvent(int slot_id, int flag);
    static int getVsimPlugInOutEvent(int slot_id);

private:
    void requestSwitchExternalSim();
    void initVsimConfiguration();
    void requestSetAkaSim();
    void handleVsimNotification(const sp<RfxMclMessage>& msg);
    void handleVsimOperation(const sp<RfxMclMessage>& msg);
    // Handle vsim disconnected event, send necessary responses to modem
    void handleVsimDisconnected(const sp<RfxMclMessage>& msg);
    void sendVsimErrorResponse(const sp<RfxMclMessage>& msg);
    void handleVsimAuthRequestEvent(const sp<RfxMclMessage>& msg);
    void handleReleaseAkaSim(const sp<RfxMclMessage>& msg);
    void handleReleaseRf(const sp<RfxMclMessage>& msg);
    void setVsimAuthTimeoutDuration(int slot_id);

private:
    // VSIM modem waiting response value: -1: N/A, 0: APDU, 1: ATR
    static int sVsimMdWaiting[MAX_SIM_COUNT];
    static int sVsimPlugInOut[MAX_SIM_COUNT];
};
#endif /* __RMM_VSIM_REQUEST_HANDLER_H__ */
// External SIM [End]
