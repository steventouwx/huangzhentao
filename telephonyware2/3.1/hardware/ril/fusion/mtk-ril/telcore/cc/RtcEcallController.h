/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2020. All rights reserved.
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

#ifdef TBOX_ECALL
#ifndef __RTC_ECALL_CONTROLLER_H__
#define __RTC_ECALL_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxDefs.h"
#include "RfxRootController.h"
#include "RfxStatusDefs.h"
#include "RfxTimer.h"
#include "telephony/mtk_ril_ivt.h"
#include "RtcCallController.h"

/*****************************************************************************
 * MACRO
 *****************************************************************************/
#define LOG_ECALL_TAG "RtcEcallController"


/*****************************************************************************
 * Const
 *****************************************************************************/
static const bool ECALL_DBG = true;

/*****************************************************************************
 * TYPE
 *****************************************************************************/

#define ECALL_CTRLSEQ_LEN 8000

#define ECALL_MAX_SEQ_LEN 8192

#define ECALL_IPC_FOR_UPLINK    "/tmp/ecall/ecall_flow_recv_from_lower"

#define ECALL_IPC_FOR_INDICATION "/tmp/ecall/ecall_flow_report_indication"

#define ECALL_DEFAULT_T9_TIMER "3600"

extern "C" void *ecallasst_report_unsol_handle(void *arg);

/*****************************************************************************
 * Class RtcEcallController
 *****************************************************************************/
class RtcEcallController : public RfxController {
    RFX_DECLARE_CLASS(RtcEcallController);

public:
    RtcEcallController();
    virtual ~RtcEcallController();

protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(
        const sp<RfxMessage>& message, bool isModemPowerOff, int radioState);

public:
    int getCallCount();
    RtcEcallController* getActiveController();
    int getFastEcallIndex();
    int getSendMSDCount();
    void setSendMSDCount(int count);
    RIL_Ecall_Unsol_Indications* getImsIndications();
    int getStartCount();
    int getIvsMode();

private:
    ecall_ctrl_sequence_tmp_t ctrl_sequence_tmp;
    int WaitFastEcallResp;
    int isInFastEcall;         // if isInFastEall==1, no more eCall ril request is allowed.
    bool isInNormalEcall;
    int mFastEcallHangupCall;
    int ivsMode;
    int isMSDSetDone;
    int Start_cnt;
    int isMSDReady;
    int mIsImsUpdateMsd;
    int SendMSD_cnt;
    int mCallCount;
    bool mIsInImsEcall;
    bool mWaitImsEcallConnected;

    unsigned char msdData[140];
    char address[40];
    int FastEcallType;
    int CtrlSeqFlag;
    int HasResetTnum;
    sp<RfxMessage> mFastEcallMessage;
    pthread_t ecall_report_fd;
    pthread_t ecall_report_unsol_fd;
    RIL_Ecall_Unsol_Indications *mImsInd;
    TimerHandle mECallTimerHandle;
    int mEcallSimType;
    bool mWaitingDeregistrationNotification;

    enum {
        SIM_NORMAL = 0,
        SIM_ECALL_ONLY = 1,
        SIM_ECALL_AND_NORMAL = 2,
    };

    void onCallCountChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    int16_t *tokstr(char *str, int16_t *data, int *n);
    void handleEcallSetIvsRequest(const sp<RfxMessage>& message);
    void handleEcallSetMsdRequest(const sp<RfxMessage>& message);
    void handleEcallSetPsapRequest(const sp<RfxMessage>& message);
    void handleEcallIvsPushMsdRequest(const sp<RfxMessage>& message);
    void handleEcallPsapPullMsdRequest(const sp<RfxMessage>& message);
    void handleFastMakeEcallRequest(const sp<RfxMessage>& message);
    void handleEcallCtrlSequence(const sp<RfxMessage>& message);
    void handleEcallResetIvs(const sp<RfxMessage>& message);

    friend void *::ecallasst_report_unsol_handle(void *arg);
    void SendEcallIndication(RIL_Ecall_Unsol_Indications* resp_data, int resp_data_size);
    int handleEcallStartMessage();
    void handleFastEcall();
    void onCallIdChanged(int slotId, RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void handleImsInBandMsdSuccess();
    void handleFastEcallHangupAll();
    void handleEcallError();
    void sendECallConnectedStatus(bool isConnected, int cause);
    void sendECallConnectedStatus(bool isConnected);
    void setTestNumber(char* number);
    void onCallDisconnected(RfxStatusKeyEnum key, RfxVariant old_value, RfxVariant value);
    void startT9Timer();
    void onTimeOut();
    void dialFastEcall();
    int resetIvs();
};

#endif /* __RTC_REDIAL_CONTROLLER_H__ */
#endif
