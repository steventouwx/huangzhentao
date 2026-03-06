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

#ifndef __RMM_NW_ASYNC_REQUEST_HANDLER_H__
#define __RMM_NW_ASYNC_REQUEST_HANDLER_H__

#include "RfxBaseHandler.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include "RfxSscData.h"
#include "RmmNwHandler.h"

typedef struct _PreferredPlmnResponse {
    char *index;
    char *format;
    char *name;
    char *act;
} PreferredPlmnResponse;
#define MAX_INDEX_LEN   (12)
#define MAX_FORMAT_LEN  (4)
#define MAX_ACT_LEN     (8)

#define RESPONSE_SIZE   (MAX_INDEX_LEN + MAX_OPER_NAME_LENGTH + MAX_FORMAT_LEN + MAX_ACT_LEN)

class RmmNwAsyncRequestHandler : public RmmNwHandler {
    RFX_DECLARE_HANDLER_CLASS(RmmNwAsyncRequestHandler);

    public:
        RmmNwAsyncRequestHandler(int slot_id, int channel_id);
        virtual ~RmmNwAsyncRequestHandler();

    protected:
        virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
        virtual void onHandleResponse(const sp<RfxMclMessage>& msg);

        int m_csgListOngoing = 0;
        int m_csgListAbort =0;

    private:
        void handleSetNwSelectionModeRequest(const sp<RfxMclMessage>& msg);
        void handleSetNwSelectionModeResponse(const sp<RfxMclMessage>& msg);
        void handleGetPolCapabilityRequest(const sp<RfxMclMessage>& msg);
        void handleGetPolCapabilityResponse(const sp<RfxMclMessage>& msg);
        void handleGetPolListRequest(const sp<RfxMclMessage>& msg);
        void handleGetPolListResponse(const sp<RfxMclMessage>& msg);
        void handleSetPolEntryRequest(const sp<RfxMclMessage>& msg);
        void handleSetPolEntryResponse(const sp<RfxMclMessage>& msg);
        void handleSetSystemSelectionChannelsRequest(const sp<RfxMclMessage>& msg);
        void handleSetSystemSelectionChannelsResponse(const sp<RfxMclMessage>& msg);
        void handleGetSystemSelectionChannelsRequest(const sp<RfxMclMessage>& msg);
        void handleSearchRatRequest(const sp<RfxMclMessage>& msg);
        void handleSearchStoredFrequencyInfoRequest(const sp<RfxMclMessage>& msg);
        void handleSetRoamingEnableRequest(const sp<RfxMclMessage>& msg);
        void handleGetRoamingEnableRequest(const sp<RfxMclMessage>& msg);
        void handleGetRoamingEnableResponse(const sp<RfxMclMessage>& msg);
        void handleDeactivateNrScgCommunicationRequest(const sp<RfxMclMessage>& msg);
        void handleGetDeactivateNrScgCommunicationRequest(const sp<RfxMclMessage>& msg);
        void handleGetDeactivateNrScgCommunicationResponse(const sp<RfxMclMessage>& msg);
        void handleQueryAvailableBandModeRequest(const sp<RfxMclMessage>& msg);
        void handleQueryAvailableBandModeResponse(const sp<RfxMclMessage>& msg);
        void handleSetBandModeRequest(const sp<RfxMclMessage>& msg);
        void handleSetLteReleaseVersionRequest(const sp<RfxMclMessage>& msg);
        void handleGetLteReleaseVersionRequest(const sp<RfxMclMessage>& msg);
        void handleGetLteReleaseVersionResponse(const sp<RfxMclMessage>& msg);
        void handleSetBackgroundSearchTimerRequest(const sp<RfxMclMessage>& msg);
        void handleGetPseudoCellInfoRequest(const sp<RfxMclMessage>& msg);
        void handleGetPseudoCellInfoResponse(const sp<RfxMclMessage>& msg);
        void handleSetPseudoCellModeRequest(const sp<RfxMclMessage>& msg);
        void handleSetLocationUpdatesRequest(const sp<RfxMclMessage>& msg);
        void handleSetServiceStateRequest(const sp<RfxMclMessage>& msg);
        void handleGetFemtocellListRequest(const sp<RfxMclMessage>& msg);
        void handleGetFemtocellListResponse(const sp<RfxMclMessage>& msg);
        void handleAbortFemtocellListRequest(const sp<RfxMclMessage>& msg);
        void handleSelectFemtocellRequest(const sp<RfxMclMessage>& msg);
        void handleQueryFemtocellSystemSelectionModeRequest(const sp<RfxMclMessage>& msg);
        void handleQueryFemtocellSystemSelectionModeReponse(const sp<RfxMclMessage>& msg);
        void handleSetFemtocellSystemSelectionModeRequest(const sp<RfxMclMessage>& msg);
        void handleConfigA2OffsetRequest(const sp<RfxMclMessage>& msg);
        void handleConfigB1OffsetRequest(const sp<RfxMclMessage>& msg);
        void handleSetDisable2GRequest(const sp<RfxMclMessage>& msg);
        void handleGetDisable2GRequest(const sp<RfxMclMessage>& msg);
        void handleGetDisable2GResponse(const sp<RfxMclMessage>& msg);
        void handleNwOemHookRawRequest(const sp<RfxMclMessage>& msg);
        void setNrOptionRequest(const sp<RfxMclMessage>& msg);
        void getNrOptionRequest(const sp<RfxMclMessage>& msg);
        void setNrCaCombRequest(const sp<RfxMclMessage>& msg);
        void setNrCaCfgRequest(const sp<RfxMclMessage>& msg);
};

#endif
