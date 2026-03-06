/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2016. All rights reserved.
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
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmmData.h"

/*****************************************************************************
 * Class RmmMultiAtReq
 *****************************************************************************/
RmmAtSendInfo* RmmMultiAtReq::getNextAt(RmmAtRspInfo *cur, RfxBaseHandler *h) {
    RfxAtLine *intermediate = cur->getResponse()->getIntermediates();
    RfxAtLine *finalResponse = cur->getResponse()->getFinalResponse();
    int err;
    bool stop = false;
    if (cur->getResponse()->isATCmdRspErr()) {
        setError(RIL_E_SYSTEM_ERR);
        if (finalResponse != NULL) {
            finalResponse->atTokStart(&err);
            onHandleFinalResponseForError(cur->getCmd(), finalResponse, h);
        }
    } else {
        while (intermediate) {
            intermediate->atTokStart(&err);
            if (onHandleIntermediates(cur->getCmd(), intermediate, h)) {
                intermediate = intermediate->getNext();
            } else {
                stop = true;
                break;
            }
        }
        if (finalResponse != NULL) {
            finalResponse->atTokStart(&err);
            onHandleFinalResponse(cur->getCmd(), finalResponse, h);
        }
        if ((getError() == RIL_E_SUCCESS) && (!stop)) {
            return onGetNextAtInfo(cur->getCmd(), h);
        }
    }
    return NULL;
}

/*****************************************************************************
 * Class RmmSingleAtReq
 *****************************************************************************/
RmmAtSendInfo* RmmSingleAtReq::onGetFirstAtInfo(RfxBaseHandler *h) {
    return onGetAtInfo(h);
}

RmmAtSendInfo* RmmSingleAtReq::onGetNextAtInfo(const String8 & curCmd, RfxBaseHandler *h) {
    RFX_UNUSED(curCmd);
    RFX_UNUSED(h);
    return NULL;
};

void RmmSingleAtReq::onHandleFinalResponse(
    const String8 &cmd,
    RfxAtLine *line,
    RfxBaseHandler *h
) {
    RFX_UNUSED(cmd);
    onHandleFinalResponse(line, h);
}

void RmmSingleAtReq::onHandleFinalResponseForError(
    const String8 &cmd,
    RfxAtLine *line,
    RfxBaseHandler *h
) {
    RFX_UNUSED(cmd);
    onHandleFinalResponseForError(line, h);
}

bool RmmSingleAtReq::onHandleIntermediates(
    const String8 &cmd,
    RfxAtLine *line,
    RfxBaseHandler *h
) {
    RFX_UNUSED(cmd);
    return onHandleIntermediates(line, h);
}

/*****************************************************************************
 * Class RmmBaseRspData
 *****************************************************************************/
sp<RfxMclMessage> RmmBaseRspData::toMessage(const sp<RfxMclMessage> &msg) {
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), getError(),
        getData(), msg);
    return response;
}

/*****************************************************************************
 * Class RmmVoidRsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmmVoidRsp);
RmmVoidRsp::RmmVoidRsp(void *data, int length) : RmmBaseRspData(data, length),
        m_error(RIL_E_SUCCESS) {
    m_data = NULL;
    m_length = 0;
}

RmmVoidRsp::~RmmVoidRsp() {
}

/*****************************************************************************
 * Class RmmBaseUrspData
 *****************************************************************************/
sp<RfxMclMessage> RmmBaseUrspData::toMessage() {
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(getUrcId(), getSlotId(),
        getData());
    return urc;
}

/*****************************************************************************
 * Class RmmVoidUrsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmmVoidUrsp);

RmmVoidUrsp::RmmVoidUrsp(void *data, int length) : RmmBaseUrspData(data, length),
        m_urc(0), m_slotId(-1){
    m_data = NULL;
    m_length = 0;
}

RmmVoidUrsp::~RmmVoidUrsp() {
}

/*****************************************************************************
 * Class RmmStringUrsp
 *****************************************************************************/
RFX_IMPLEMENT_DATA_CLASS(RmmStringUrsp);

RmmStringUrsp::RmmStringUrsp(void *data, int length) : RmmVoidUrsp(data, length) {
    m_string.setTo((const char *)data, length);
    m_data = (void *)m_string.string();
    m_length = length;
}

RmmStringUrsp::~RmmStringUrsp() {
}
