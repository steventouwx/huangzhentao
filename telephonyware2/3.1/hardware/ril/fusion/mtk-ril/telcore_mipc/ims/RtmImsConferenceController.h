/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#ifndef __RFX_IMS_EVENT_PACKAGE_CONTROLLER_H__
#define __RFX_IMS_EVENT_PACKAGE_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "RfxController.h"
#include "RtmImsDialogHandler.h"
#include <queue>
//#include "utils/String8.h"
//#include "utils/Vector.h"

//using ::android::String8;
//using ::android::Vector;
using ::android::sp;

using namespace std;
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

class RfxStringsData;

class RtmImsConferenceController : public RfxController {
    // Required: declare this class
    RFX_DECLARE_CLASS(RtmImsConferenceController);

public:
    RtmImsConferenceController();
    virtual ~RtmImsConferenceController();

// Override
protected:
    virtual void onInit();
    virtual void onDeinit();
    virtual bool onHandleRequest(const sp<RfxMessage>& message);
    virtual bool onHandleUrc(const sp<RfxMessage>& message);
    virtual bool onHandleResponse(const sp<RfxMessage>& message);
    virtual bool onCheckIfRejectMessage(
        const sp<RfxMessage>& message, bool isModemPowerOff, int radioState);
    void handleMergeConferenece(const sp<RfxMessage>& message);
    void handleImsCallInfoUpdate(const sp<RfxMessage>& message);
    void handleEconfUpdate(const sp<RfxMessage>& message);
    bool handleAddMemberResponse(const sp<RfxMessage>& response);
    void handleRemoveMemberResponse(const sp<RfxMessage>& response);
    bool tryhandleAddMemberByMerge(const sp<RfxMessage>& response);
    void handleSipCallProgressIndicator(const sp<RfxMessage>& message);

public:
    bool handleMessgae(const sp<RfxMessage>& message);
    bool handleResponse(const sp<RfxMessage>& message);
    bool handleUrc(const sp<RfxMessage>& message);
    // Fake disconnect related interface
    bool needProcessFakeDisconnect();
    void processFakeDisconnectDone();
    bool needSkipDueToFakeDisconnect(int callid);

private:
    void handleImsEventPackageIndication(const sp<RfxMessage>& msg);
    void handleImsConfInfoIndication(const sp<RfxMessage>& msg);
    void handleSpecificConferenceMode();

protected:
    RtmImsDialogHandler* mRtmImsDialogHandler = NULL;

private:
    bool mNormalCallsMerge;
    bool mInviteByNumber;
    bool mIsMerging;
    int mEconfCount;
    sp<RfxMessage> mOriginalMergeMessage = NULL;

    int mFakeDisconnectedCallProcessingCount;
    Vector<int> mFakeDisconnectedCallIdList;
};

#endif /* __RFX_IMS_EVENT_PACKAGE_CONTROLLER_H__ */
