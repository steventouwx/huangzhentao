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
#ifndef __RTM_CONCAT_SMS_H__
#define __RTM_CONCAT_SMS_H__
/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxObject.h"
#include "RtmSmsAddress.h"
#include "RtmSmsHeader.h"
#include "utils/Timers.h"

/*****************************************************************************
 * Class Declaration
 *****************************************************************************/
class RtmConCatSmsSender;
class RtmConCatSmsGroup;
class RtmConCatSmsPart;
class RtmSmsMessage;

/*****************************************************************************
 * RtmConCatSmsRoot
 *****************************************************************************/
class RtmConCatSmsRoot : public RfxObject {
    RFX_DECLARE_CLASS(RtmConCatSmsRoot);
// External Method
public:
    // Get the SMS sender object.
    //
    // RETURNS: SMS sender object
    RtmConCatSmsSender* getSmsSender(
        const String8 &address   // [IN] the SMS address string
    );

    // Close the object that should be closed.
    //
    // RETURNS: None
    void cleanUpObj();
};

/*****************************************************************************
 * RtmConCatSmsSender
 *****************************************************************************/
class RtmConCatSmsSender : public RfxObject {
    RFX_DECLARE_CLASS(RtmConCatSmsSender);
// Constructor / Destructor
public:
    // Default Constructor
    RtmConCatSmsSender() {}

    // Construct with a SMS address
    RtmConCatSmsSender(
        const String8 &address  // [IN] the SMS address
    ) :mAddress(address) {}

    // Destructor
    virtual ~RtmConCatSmsSender() {}

// External Method
public:
    // Get the SMS address.
    //
    // RETURNS: SMS address string.
    const String8 &getAddress() const {
        return mAddress;
    }

    // Check if the input address is the same address
    //
    // RETURNS: true if address same
    bool isSameAs(
        const String8 &address // [IN] the address to compare with
    ) {
        return mAddress == address;
    }

    // Get the SMS group object.
    //
    // RETURNS: the SMS group object
    RtmConCatSmsGroup *getSmsGroup(
        int refNumber,    // [IN] the reference number of this concatenated SMS.
        int messageCount  // [IN] the message count
    );

    // Close the object that should be closed.
    //
    // RETURNS: None
    void cleanUpObj();

// Implement
private:
    String8 mAddress;
};

/*****************************************************************************
 * RtmConCatSmsGroup
 *****************************************************************************/
class RtmConCatSmsGroup : public RfxObject {
    RFX_DECLARE_CLASS(RtmConCatSmsGroup);
// Constructor / Destructor
public:
    // Default constructor
    RtmConCatSmsGroup()
        : RfxObject(), mTimeStamp(0), mRefNumber(-1), mMessageCount(-1) {
    }

    // Construct SMS group with reference number and message count.
    RtmConCatSmsGroup(
        int refNumber,   // [IN] the reference number of this concatenated SMS.
        int messageCount // [IN] the message count
    ) : RfxObject(), mRefNumber(refNumber), mMessageCount(messageCount) {
        mTimeStamp = systemTime(SYSTEM_TIME_MONOTONIC);
    }

    // Destructor
    virtual ~RtmConCatSmsGroup() {}

// External Method
public:
    // Check if same concatenated SMS group
    //
    // RETURNS: true if belong to same concatenated SMS group
    bool isSameAs(
        int refNumber,     // [IN] the reference number of this concatenated SMS.
        int messageCount   // [IN] the message count
    ) {
        return (refNumber == mRefNumber) && (mMessageCount == messageCount);
    }

    // Get the SMS part object
    //
    // RETURNS: SMS part object
    RtmConCatSmsPart *getSmsPart(
        int seqNumber  // [IN] the sequence number of this concatenated SMS
    );

    // Check if all parts are received of this concatenated SMS
    //
    // RETURNS: true if all parts received
    bool allPartsReady() {
        return mMessageCount == getChildCount();
    }

    // Get the reference number of this concatenated SMS.
    //
    // RETURNS: reference number
    int getRefNumber() const {
        return mRefNumber;
    }

    // Get the message count of this concatenated SMS.
    //
    // RETURNS: message count
    int getMsgCount() const {
        return mMessageCount;
    }

    // Update the time stamp of this SMS group.
    //
    // RETURNS: None
    void updateTimeStamp() {
        mTimeStamp = systemTime(SYSTEM_TIME_MONOTONIC);
    }

    // Check if this SMS group is expired
    //
    // RETURNS: true if expired
    bool isExpire();

// Implement
private:
    nsecs_t mTimeStamp;
    int mRefNumber;
    int mMessageCount;
};

/*****************************************************************************
 * RtmConCatSmsPart
 *****************************************************************************/
class RtmConCatSmsPart : public RfxObject {
    RFX_DECLARE_CLASS(RtmConCatSmsPart);
// Constructor / Destructor
public:
    // Default constructor
    RtmConCatSmsPart()
        : mSeqNumber(-1), mIs3Gpp(true), mMessage(NULL), mConvertedMessage(NULL) {
    }

    // Construct a SMM part with sequency number
    RtmConCatSmsPart(
        int seqNumber   // [IN] the sequency number of this concatenated SMS
    ): mSeqNumber(seqNumber), mIs3Gpp(true), mMessage(NULL), mConvertedMessage(NULL) {
    }

    // Destructor
    virtual ~RtmConCatSmsPart();

// External Method
public:
    // Check if same SMS part.
    //
    // RETURNS: None
    bool isSameAs(
        int seqNumber // [IN] the sequency number of this concatenated SMS
    ) {
        return seqNumber == mSeqNumber;
    }

    // Set the SMS format
    //
    // RETURNS: None
    void setFormat3Gpp(
        bool is3Gpp    // [IN] true if 3GPP format
    ) {
        mIs3Gpp = is3Gpp;
    }

    // Check SMS format.
    //
    // RETURNS: true if 3GPP format
    bool isFormat3Gpp() const {
        return mIs3Gpp;
    }

    // Check if two SMS parts have same SMS format.
    //
    // RETURNS: true if having same format
    bool isSameFormatAs(
        RtmConCatSmsPart* other  // [IN] another SMS part to compare with
    ) {
        return mIs3Gpp == other->isFormat3Gpp();
    }

    // Get the sequency number of this concatenated SMS.
    //
    // RETURNS: sequency number
    int getSeqNumber() const {
        return mSeqNumber;
    }

    // Set the message object of this SMS part
    //
    // RETURNS: None
    void setMessage(
        RtmSmsMessage *message    // [IN] the message object
    ) {
         mMessage = message;
    }

    // Get the reference of message object.
    //
    // RETURNS: message object
    const RtmSmsMessage &getMessage()  const;

    // Get the format converted message.
    //
    // RETURNS; the converted message object
    RtmSmsMessage *getConvertedMessage();

// Implement
private:
    int mSeqNumber;
    bool mIs3Gpp;
    RtmSmsMessage *mMessage;
    RtmSmsMessage *mConvertedMessage;
};

#endif /* __RTM_CONCAT_SMS_H__ */
