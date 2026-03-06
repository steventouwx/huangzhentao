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

#ifndef __RMM_SML_REQUEST_HANDLER_H__
#define __RMM_SML_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RmmSimBaseHandler.h"
#include "RmmCommSimDefs.h"

/*****************************************************************************
 * Class RmmSmlRequestHandler
 * SIM ME Lock request handler
 *****************************************************************************/

class RmmSmlRequestHandler : public RmmSimBaseHandler {
RFX_DECLARE_HANDLER_CLASS(RmmSmlRequestHandler);

public:
    RmmSmlRequestHandler(int slot_id, int channel_id);
    virtual ~RmmSmlRequestHandler();

    // Process Request here
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

private:
    //utils
    int convertPersoStateToCat(int perso_substate);
    int convertCatToPersoState(int cat);
    mipc_sim_carrier_struct_struct4 *allocMipcCarriers(const RIL_Carrier *pRilCarriers,
            int count, RIL_Errno &error);
    void freeMipcCarriers(mipc_sim_carrier_struct_struct4 *pMipcCarriers);
    RIL_Carrier *allocRilCarriers(const mipc_sim_carrier_struct_struct4 *pMipcCarriers,
            int count, RIL_Errno &error);
    void freeRilCarriers(RIL_Carrier *pMipcCarriers);
    unsigned int htoi(char* szHex);
    void logMipcData(sp<RfxMipcData> mipcData);
    void setSimSlotLockDeviceLockInfoProp(int remaining);
    char* getReducedLog(char* data);
    int32_t getOpIdForCommon();
    int32_t getRequestIdForCommon(int32_t commRequestId);
    int32_t convertRequestIdToL5(int32_t opId, int32_t requestId);
    int32_t convertRequestIdfromL5(int32_t opId, int32_t requestId);
    int smlGetLockRule();
    int smlIsEnabled();

    // Request handler
    void handleEnterNetworkDepersonalization(const sp<RfxMclMessage>& msg);
    void handleEnterDepersonalization(const sp<RfxMclMessage>& msg);
    void handleEnterSimDepersonalization(const sp<RfxMclMessage>& msg);
    void handleSetAllowedCarriers(const sp < RfxMclMessage > & msg);
    void handleGetAllowedCarriers(const sp < RfxMclMessage > & msg);
    void handleQuerySimNetworkLock(const sp < RfxMclMessage > & msg);
    void handleSetSimNetworkLock(const sp < RfxMclMessage > & msg);
    void handleEnterDeviceNetworkDepersonalization(const sp<RfxMclMessage>& msg);
    //RSU
    void handleRsuRequest(const sp<RfxMclMessage>& msg);
    void handleRsuClientRequest(const sp<RfxMclMessage>& msg);
    void handleCommonRsuGetLockInfo(const sp<RfxMclMessage>& msg);
    void handleCommonRsuGetCatInfo(const sp<RfxMclMessage>& msg);
    //SubsidyLock
    void handleReqSubsidyLockUpdateSettings(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGetModemStatus(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGetVersion(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGetUnlockTime(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGet(const sp<RfxMclMessage>& msg, uint32_t reqId);
    void handleEventResetSim(const sp<RfxMclMessage>& msg);


    // Response handler
    void handleEnterNetworkDepersonalizationResp(const sp<RfxMclMessage>& msg);
    void handleEnterDepersonalizationResp(const sp<RfxMclMessage>& msg);
    void handleEnterSimDepersonalizationResp(const sp<RfxMclMessage>& msg);
    void handleSetAllowedCarriersResp(const sp < RfxMclMessage > & msg);
    void handleGetAllowedCarriersResp(const sp < RfxMclMessage > & msg);
    void handleQuerySimNetworkLockResp(const sp < RfxMclMessage > & msg);
    void handleSetSimNetworkLockResp(const sp < RfxMclMessage > & msg);
    void handleEnterDeviceNetworkDepersonalizationResp(const sp<RfxMclMessage>& msg);
    void handleRsuRequestResp(const sp<RfxMclMessage>& msg);
    void handleRsuClientRequestResp(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockUpdateSettingsResp(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGetModemStatusResp(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGetVersionResp(const sp<RfxMclMessage>& msg);
    void handleReqSubsidyLockGetUnlockTimeResp(const sp<RfxMclMessage>& msg);
    void handleCommonRsuGetLockInfoResp(const sp<RfxMclMessage>& msg);
    void handleCommonRsuGetCatInfoResp(const sp<RfxMclMessage>& msg);

};
#endif /* __RMM_SML_REQUEST_HANDLER_H__ */

