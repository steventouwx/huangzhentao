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
#ifndef __RMM_DATA_H__
#define __RMM_DATA_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseData.h"
#include "RfxBaseHandler.h"
#include "RmmAtInfo.h"

/*****************************************************************************
 * Class RmmBaseReqData
 *****************************************************************************/
class RmmBaseReqData: public RfxBaseData {
// External Method
public:
    // Set the RIL error number
    //
    // RETURNS: void
    void setError(
        RIL_Errno err       // [IN] Set RIL error number
    ) {
        m_error = err;
    }

    // Get the RIL error number
    //
    // RETURNS: the RIL error number
    RIL_Errno getError() const {
        return m_error;
    }

// Constructor / Destructor
public:
    RmmBaseReqData(void *data, int length)
            :RfxBaseData(data, length), m_error(RIL_E_SUCCESS) {
    }

    virtual ~RmmBaseReqData() {
    }

// Overridable
public:
    // Get the first AT send Info
    //
    // RETURNS: the first AT send info
    virtual RmmAtSendInfo* getFirstAt(
        RfxBaseHandler *h           // [IN] The pointer of the RfxBaseHandler
    ) = 0;

    // Get the next AT send info
    //
    // RETURNS: the next AT send info
    virtual RmmAtSendInfo* getNextAt(
        RmmAtRspInfo *cur,         // [IN] the current AT response info
        RfxBaseHandler *h          // [IN] the pointer of the RfxBaseHandler
    ) = 0;

// Implementation
private:
    RIL_Errno m_error;
};

/*****************************************************************************
 * Class RmmMultiAtReq
 *****************************************************************************/
class RmmMultiAtReq: public RmmBaseReqData {
// Constructor / Destructor
public:
    RmmMultiAtReq(void *data, int length) : RmmBaseReqData(data, length) {
    }
    virtual ~RmmMultiAtReq() {
    }

// Override
public:
    virtual RmmAtSendInfo* getFirstAt(RfxBaseHandler *h) {
         return onGetFirstAtInfo(h);
    }

    virtual RmmAtSendInfo* getNextAt(RmmAtRspInfo *cur, RfxBaseHandler *h);

// Overridable
protected:
    // Callback when get the first AT send info
    //
    // RETURNS: the AT send info
    virtual RmmAtSendInfo* onGetFirstAtInfo(
        RfxBaseHandler *h        // [IN] The pointer of the RfxBaseHandler
    ) = 0;

    // Callback when get the next AT send info
    //
    // RETURNS: the next AT send info
    virtual RmmAtSendInfo* onGetNextAtInfo(
        const String8 & curCmd,    // [IN] The current AT command
        RfxBaseHandler *h          // [IN] The pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(curCmd);
        RFX_UNUSED(h);
        return NULL;
    };

    // Callback when handle the final response
    //
    // RETURNS: void
    virtual void onHandleFinalResponse(
        const String8 &curCmd,  // [IN] the AT command
        RfxAtLine *line,        // [IN] the AT line after atToStart is invoked
        RfxBaseHandler *h       // [IN] the pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(curCmd);
        RFX_UNUSED(line);
        RFX_UNUSED(h);
    }

    // Callback when handle the final response in error case
    //
    // RETURNS: void
    virtual void onHandleFinalResponseForError(
        const String8 &curCmd,  // [IN] the AT command
        RfxAtLine *line,        // [IN] the AT line after atToStart is invoked
        RfxBaseHandler *h       // [IN] the pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(curCmd);
        RFX_UNUSED(line);
        RFX_UNUSED(h);
    }

    // Callback when handle the intermediate
    //
    // RETURNS: return true to handle the next intermediates if there is
    virtual bool onHandleIntermediates(
        const String8 &curCmd,  // [IN] the AT command
        RfxAtLine *line,        // [IN] the AT line after atToStart is invoked
        RfxBaseHandler *h       // [IN] the pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(curCmd);
        RFX_UNUSED(line);
        RFX_UNUSED(h);
        return true;
    }
};


/*****************************************************************************
 * Class RmmSingleAtReq
 *****************************************************************************/
class RmmSingleAtReq: public RmmMultiAtReq {
// Constructor / Destructor
public:
    RmmSingleAtReq(void *data, int length)
            : RmmMultiAtReq(data, length) {
    }

    virtual ~RmmSingleAtReq() {
    }

// Override
protected:
    virtual RmmAtSendInfo* onGetFirstAtInfo(RfxBaseHandler *h);
    virtual RmmAtSendInfo* onGetNextAtInfo(const String8 & cmd, RfxBaseHandler *h);
    virtual void onHandleFinalResponse(const String8 &cmd, RfxAtLine *line, RfxBaseHandler *h);
    virtual void onHandleFinalResponseForError(
        const String8 &cmd,
        RfxAtLine *line,
        RfxBaseHandler *h
    );
    virtual bool onHandleIntermediates(const String8 &cmd, RfxAtLine *line, RfxBaseHandler *h);

// Overridable
protected:
    // Get the AT send info
    //
    // RETURNS: the AT send info
    virtual RmmAtSendInfo* onGetAtInfo(
        RfxBaseHandler *h   // [IN] the pointer of the RfxBaseHandler
    ) = 0;


    // Callback when handle the final response
    //
    // RETURNS: void
    virtual void onHandleFinalResponse(
        RfxAtLine *line,    // [IN] the AT line after atToStart is invoked
        RfxBaseHandler *h   // [IN] the pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(line);
        RFX_UNUSED(h);
    }

    // Callback when handle the final response in error case
    //
    // RETURNS: void
    virtual void onHandleFinalResponseForError(
        RfxAtLine *line,    // [IN] the AT line after atToStart is invoked
        RfxBaseHandler *h   // [IN] the pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(line);
        RFX_UNUSED(h);
    }

    // Callback when handle the intermediate
    //
    // RETURNS: return true to handle the next intermediates if there is
    virtual bool onHandleIntermediates(
        RfxAtLine *line,    // [IN] the AT line after atToStart is invoked
        RfxBaseHandler *h   // [IN] the pointer of the RfxBaseHandler
    ) {
        RFX_UNUSED(line);
        RFX_UNUSED(h);
        return true;
    }
};

/*****************************************************************************
 * Class RmmBaseRspData
 *****************************************************************************/
class RmmBaseRspData : public RfxBaseData {
// External Method
public:
    // Generate the response message to telcore
    //
    // RETURNS: the response message
    sp<RfxMclMessage> toMessage(
        const sp<RfxMclMessage> &msg  // [IN] the request message from telcore
    );

// Constructor / Destructor
public:
    RmmBaseRspData(void *data, int length)
            : RfxBaseData(data, length) {
    }
    virtual ~RmmBaseRspData() {
    }

// Overriable
protected:
    // Get the RIL error number
    //
    // RETURNS: the RIL error number
    virtual RIL_Errno getError() const= 0;

protected:
    // Get the reference of the data
    //
    // RETURNS: the reference of the data
    const RfxBaseData & getData() const {
        return *this;
    }
};

/*****************************************************************************
 * Class RmmVoidRsp
 *****************************************************************************/
class RmmVoidRsp : public RmmBaseRspData {
    RFX_DECLARE_DATA_CLASS(RmmVoidRsp);

// Constructor / Destructor
public:
    RmmVoidRsp(RIL_Errno e) :
        RmmBaseRspData(NULL, 0), m_error(e) {
    }

// Override
protected:
    virtual RIL_Errno getError() const {
        return m_error;
    }

// Implementation
private:
    RIL_Errno m_error;
};

/*****************************************************************************
 * Class RmmBaseUrspData
 *****************************************************************************/
class RmmBaseUrspData : public RfxBaseData {
// External Method
public:
    // Generate the URC message to telcore
    //
    // RETURNS: the URC message
    sp<RfxMclMessage> toMessage();

// Constructor / Destructor
public:
    RmmBaseUrspData(void *data, int length)
            :RfxBaseData(data, length) {
    }
    virtual ~RmmBaseUrspData() {
    }

// Overridable
protected:
    // Get the URC ID
    //
    // RETURNS: the URC ID
    virtual int getUrcId() const = 0;

    // Get the Slot ID
    //
    // RETURNS The slot ID
    virtual int getSlotId() const = 0;

protected:
    // Get the reference of the data
    //
    // The reference of the data
    const RfxBaseData & getData() const {
        return *this;
    }
};


/*****************************************************************************
 * Class RmmVoidUrsp
 *****************************************************************************/
class RmmVoidUrsp : public RmmBaseUrspData {
    RFX_DECLARE_DATA_CLASS(RmmVoidUrsp);

// Constructor / Destructor
public:
    RmmVoidUrsp(int urc, int slotId) :
        RmmBaseUrspData(NULL, 0), m_urc(urc), m_slotId(slotId) {
    }

// Override
protected:
    virtual int getUrcId() const {
        return m_urc;
    }

    virtual int getSlotId() const {
        return m_slotId;
    }

// Implementation
private:
    int m_urc;
    int m_slotId;
};

/*****************************************************************************
 * Class RmmStringUrsp
 *****************************************************************************/
class RmmStringUrsp: public RmmVoidUrsp {
    RFX_DECLARE_DATA_CLASS(RmmStringUrsp);

// Constructor / Destructor
public:
    RmmStringUrsp(int urc, int slotId, String8 str) :
            RmmVoidUrsp(urc, slotId), m_string(str) {
        m_data = (void *)m_string.string();
        m_length = m_string.length();
    }

// Implementation
private:
    String8 m_string;
};

#endif /* __RMM_DATA_H__ */
