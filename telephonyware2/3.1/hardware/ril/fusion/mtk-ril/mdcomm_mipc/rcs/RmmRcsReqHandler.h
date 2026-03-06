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

#ifndef __RMM_RCS_REQ_HANDLER_H__
#define __RMM_RCS_REQ_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RcsStackChannel.h"
#include <inttypes.h>

using namespace vendor::rmm::rcs;

class RmmRcsReqHandler : public RfxBaseHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmRcsReqHandler);

    public:
        typedef struct _PendingMessage {
            int m_id;
            int m_channel_id;
            int m_token;

            _PendingMessage() {
                m_id = 0;
                m_channel_id = 0;
                m_token = 0;
            }

            _PendingMessage(int id, int channel, int token)
                : m_id(id), m_channel_id(channel), m_token(token) {}

            int getId() const {
                return m_id;
            }

            int getChannelId() const {
                return m_channel_id;
            }

            int getToken() const {
                return m_token;
            }
        } PendingMessage;

        RmmRcsReqHandler(int slot_id, int channel_id);
        virtual ~RmmRcsReqHandler();
        bool isRcsUaSupport();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    private:
        void handleDataSending(const sp<RfxMclMessage>& msg);
        void handleDataReceiving(const sp<RfxMclMessage>& msg);
        void startRcsService(const sp<RfxMclMessage>& msg);
        void stopRcsService(const sp<RfxMclMessage>& msg);
        void requestRegistrationInfo(const sp<RfxMclMessage>& msg);
        void enableDeregInd(const sp<RfxMclMessage>& msg);
        void resumeDeregistration(const sp<RfxMclMessage>& msg);
        void triggerRestoration(const sp<RfxMclMessage>& msg);
        void sendSipMessage(const sp<RfxMclMessage>& msg);
        void switchRcsRoiStatus(const sp<RfxMclMessage>& msg);
        void updateRcsCapabilities(const sp<RfxMclMessage>& msg);
        void updateRcsSessionInfo(const sp<RfxMclMessage>& msg);
        void getUserAgent(const sp<RfxMclMessage>& msg);
        void handleOutgoingSipResponse(const RcsStackChannel::ChannelData *chnlData);
        void handleRegistrationInfo(const RcsStackChannel::ChannelData *chnlData);
        void handleIncomingSipMessage(const RcsStackChannel::ChannelData *chnlData);
        void handleImsDeregInd(const RcsStackChannel::ChannelData *chnlData);
        void checkAndInitImsFd();
        char *getDebugSipData(const uint8_t *rawData, int dataLen, int maxSize);

        std::vector<sp<RfxMclMessage>> m_messages;
        RcsStackChannel *m_state_channel;
        RcsStackChannel *m_sip_channel;
        static bool m_debug;
        static bool m_wait_cnf;
};

#endif /* __RMM_RCS_REQ_HANDLER_H__ */
