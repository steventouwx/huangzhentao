/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2019. All rights reserved.
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

#include "RmmSuppServRequestNRTHandler.h"
#include "RfxMessageId.h"

#include <telephony/mtk_ril.h>
#include <mtkconfigutils.h>

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_GET_XCAP_STATUS);

static const int requests[] = {
    RFX_MSG_REQUEST_GET_XCAP_STATUS
};

// register handler to non real time channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmSuppServRequestNRTHandler, RIL_CMD_PROXY_8);

RmmSuppServRequestNRTHandler::RmmSuppServRequestNRTHandler(int slot_id, int channel_id) :
        RmmSuppServRequestBaseHandler(slot_id, channel_id) {
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
}

RmmSuppServRequestNRTHandler::~RmmSuppServRequestNRTHandler() {
}

void RmmSuppServRequestNRTHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_GET_XCAP_STATUS:
            requestGetXcapStatus(msg);
            break;

        default:
            logE(TAG, "Should not be here");
            break;
    }
}

