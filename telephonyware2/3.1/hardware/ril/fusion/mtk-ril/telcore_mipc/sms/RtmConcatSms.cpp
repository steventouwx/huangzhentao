/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2018. All rights reserved.
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
#include "RtmConcatSms.h"
#include "RtmSmsMessage.h"

/*****************************************************************************
 * Class RtmConCatSmsRoot
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtmConCatSmsRoot", RtmConCatSmsRoot, RfxObject);

RtmConCatSmsSender* RtmConCatSmsRoot::getSmsSender(const String8 & address) {
    if (address.isEmpty()) {
       return NULL;
    }
    RfxObject *obj = getFirstChildObj();
    RtmConCatSmsSender *sender = NULL;
    while (obj != NULL) {
        sender = (RtmConCatSmsSender *)obj;
        if (sender->isSameAs(address)) {
            return sender;
        }
        obj = obj->getNextObj();
    }
    RFX_OBJ_CREATE_EX(sender, RtmConCatSmsSender, this, (address));
    return sender;
}

void RtmConCatSmsRoot::cleanUpObj() {
    RfxObject *obj = getFirstChildObj();
    RtmConCatSmsSender *sender = NULL;
    Vector<RtmConCatSmsSender *> cleanObj;
    while (obj != NULL) {
        sender = (RtmConCatSmsSender *)obj;
        sender->cleanUpObj();
        if (sender->getChildCount() == 0) {
            cleanObj.push(sender);
        }
        obj = obj->getNextObj();
    }
    Vector<RtmConCatSmsSender *>::iterator it;
    for (it = cleanObj.begin(); it != NULL && it != cleanObj.end(); it++) {
        RFX_OBJ_CLOSE(*it);
    }
}

/*****************************************************************************
 * RtmConCatSmsSender
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtmConCatSmsSender", RtmConCatSmsSender, RfxObject);

RtmConCatSmsGroup *RtmConCatSmsSender::getSmsGroup(int refNumber, int messageCount) {
    if (refNumber < 0 || messageCount < 0) {
        return NULL;
    }
    RfxObject *obj = getFirstChildObj();
    RtmConCatSmsGroup *group = NULL;
    while (obj != NULL) {
        group = (RtmConCatSmsGroup *)obj;
        if (group->isSameAs(refNumber, messageCount)) {
            return group;
        }
        obj = obj->getNextObj();
    }
    RFX_OBJ_CREATE_EX(group, RtmConCatSmsGroup, this, (refNumber, messageCount));
    return group;
}

void RtmConCatSmsSender::cleanUpObj() {
    RfxObject *obj = getFirstChildObj();
    RtmConCatSmsGroup *group = NULL;
    Vector<RtmConCatSmsGroup *> cleanObj;
    while (obj != NULL) {
        group = (RtmConCatSmsGroup *)obj;
        if (group->allPartsReady() || group->isExpire()) {
            cleanObj.push(group);
        }
        obj = obj->getNextObj();
    }
    Vector<RtmConCatSmsGroup *>::iterator it;
    for (it = cleanObj.begin(); it != NULL && it != cleanObj.end(); it++) {
        RFX_OBJ_CLOSE(*it);
    }
}

/*****************************************************************************
 * RtmConCatSmsGroup
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtmConCatSmsGroup", RtmConCatSmsGroup, RfxObject);

RtmConCatSmsPart *RtmConCatSmsGroup::getSmsPart(int seqNumber) {
    if (seqNumber <= 0 || seqNumber > mMessageCount) {
        return NULL;
    }
    RfxObject *obj = getFirstChildObj();
    RtmConCatSmsPart *part = NULL;
    while (obj != NULL) {
        part = (RtmConCatSmsPart *)obj;
        if (part->isSameAs(seqNumber)) {
            return part;
        }
        obj = obj->getNextObj();
    }
    RFX_OBJ_CREATE_EX(part, RtmConCatSmsPart, this, (seqNumber));
    return part;
}

bool RtmConCatSmsGroup::isExpire() {
    nsecs_t now = systemTime(SYSTEM_TIME_MONOTONIC);
    if ((now - mTimeStamp) > seconds_to_nanoseconds(60 * 3)) {
        return true;
    }
    return false;
}

/*****************************************************************************
 * RtmConCatSmsPart
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtmConCatSmsPart", RtmConCatSmsPart, RfxObject);

RtmConCatSmsPart::~RtmConCatSmsPart() {
    if (mConvertedMessage != NULL) {
        delete mConvertedMessage;
    }
    if (mMessage != NULL) {
        delete mMessage;
    }
}

const RtmSmsMessage &RtmConCatSmsPart::getMessage()  const {
    if (mIs3Gpp) {
        return *((RtmGsmSmsMessage *)mMessage);
    } else  {
        return *((RtmCdmaSmsMessage *)mMessage);
    }
}

RtmSmsMessage *RtmConCatSmsPart::getConvertedMessage() {
    if (mConvertedMessage != NULL) {
        delete mConvertedMessage;
        mConvertedMessage = NULL;
    }

    RtmConCatSmsGroup *group = (RtmConCatSmsGroup *)getParent();
    if (group != NULL) {
        RtmConCatSmsPart *part = (RtmConCatSmsPart *)group->getFirstChildObj();
        if ((part != NULL) && (part != this) && (!part->isSameFormatAs(this))) {
            if (part->isFormat3Gpp()) {
                mConvertedMessage = new RtmGsmSmsMessage(
                        (const RtmGsmSmsMessage &)part->getMessage(),
                        getMessage().getEncodingType(),
                        getMessage().getNumField(),
                        getMessage().getUserDataPayload());
            } else {
                mConvertedMessage = new RtmCdmaSmsMessage(
                        (const RtmCdmaSmsMessage &)part->getMessage(),
                        getMessage().getEncodingType(),
                        getMessage().getNumField(),
                        getMessage().getUserDataPayload());
            }
        }
    }
    return mConvertedMessage;
}
