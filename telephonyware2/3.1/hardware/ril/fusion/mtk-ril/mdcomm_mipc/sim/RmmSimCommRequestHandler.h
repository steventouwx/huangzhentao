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

#ifndef __RMM_SIM_COMM_REQUEST_HANDLER_H__
#define __RMM_SIM_COMM_REQUEST_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxMipcData.h"
#include "RmmSimBaseHandler.h"
#include "RmmCommSimDefs.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include <platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "pb_decode.h"
#include "pb_encode.h"
#ifdef __cplusplus
}
#endif


/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmmSimCommRequestHandler : public RmmSimBaseHandler {
RFX_DECLARE_HANDLER_CLASS(RmmSimCommRequestHandler);

public:
    RmmSimCommRequestHandler(int slot_id, int channel_id);
    virtual ~RmmSimCommRequestHandler();

    // Process Request here
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);
    virtual void onHandleResponse(const sp<RfxMclMessage>& msg);
    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

    // BTSAP - START
    void notifyBtSapStatusInd(RIL_SIM_SAP_STATUS_IND_Status message);
    // BTSAP - END

private:
    static int mRetryGetSlotsInfoTimes;
    int mRetryGetSimStatusTimes;
    int mRetrySetFacilityTimes;

    void enableSilentReboot();
    int isNormalBootUp();
    // Utility functions
    int checkRetryFCI(int sw1, int sw2);
    int setChannelToClassByte(int cla, int channelNumber);
    bool getSimAppInfo(mipc_app_status_desc_struct4 *src_app_info, RIL_AppStatus *p_app_info,
            UICC_Status uStatus, RfxMipcData* mipcData);
    void sleepMsec(long long msec);
    //unsigned int htoi(char* szHex);
    //void setSimInsertedStatus(int slotId, int isInserted);
    void decodeBtSapPayload(int msgId, const void *src, size_t srclen, void *dst);
    void resetBtSapContext();
    char* convertBtSapIntToHexString(uint8_t *data, size_t datalen);
    int toByte(char c);
    int hexStringToByteArrayEx(unsigned char* hexString, int hexStringLen,
            unsigned char ** byte);
    void sendBtSapResponseComplete(const sp<RfxMclMessage>& msg,
        RIL_Errno ret, int msgId, void *data);
    void querySimOnOffActivation();

    // Request handler
    void handleGetSimStatus(const sp<RfxMclMessage>& msg);
    void handleGetSimStatusResponse(const sp<RfxMclMessage>& msg);
    void handleSimIo(const sp<RfxMclMessage>& msg);
    void handleSimIoResponse(const sp<RfxMclMessage>& msg);
    void handleIsimAuth(const sp<RfxMclMessage>& msg);
    void handleIsimAuthResponse(const sp<RfxMclMessage>& msg);
    void handleSimAuth(const sp<RfxMclMessage>& msg);
    void handleSimAuthResponse(const sp<RfxMclMessage>& msg);
    void handleGeneralSimAuth(const sp<RfxMclMessage>& msg);
    void handleGeneralSimAuthResponse(const sp<RfxMclMessage>& msg);
    void handleSimOpenChannel(const sp < RfxMclMessage > & msg);
    void handleSimOpenChannelResponse(const sp < RfxMclMessage > & msg);
    void handleSimCloseChannel(const sp < RfxMclMessage > & msg);
    void handleSimCloseChannelResponse(const sp < RfxMclMessage > & msg);
    void handleDetectSim(const sp < RfxMclMessage > & msg);
    void handleDetectSimResponse(const sp < RfxMclMessage > & msg);
    void handleSimTransmitApdu(const sp < RfxMclMessage > & msg, bool isBasicChannel);
    void handleSimTransmitApduResponse(const sp < RfxMclMessage > & msg);
    void handleSimGetAtr(const sp < RfxMclMessage > & msg);
    void handleSimGetAtrResponse(const sp < RfxMclMessage > & msg);
    void handleSimGetIccid(const sp < RfxMclMessage > & msg);
    void handleEventQueryIccid(const sp<RfxMclMessage>& msg);
    void handleEventQueryIccidResponse(const sp<RfxMclMessage>& msg);
    void handleSecurityOperation(const sp<RfxMclMessage>& msg, UICC_Security_Operation op);
    void handleSecurityOperationResponse(const sp<RfxMclMessage>& msg, UICC_Security_Operation op);
    // BTSAP - START
    void handleLocalBtSapReset(const sp<RfxMclMessage>& msg);
    void handleBtSapConnect(const sp<RfxMclMessage>& msg);
    void handleBtSapConnectResponse(const sp<RfxMclMessage>& msg);
    void handleBtSapDisconnect(const sp<RfxMclMessage>& msg);
    void handleBtSapDisconnectResponse(const sp<RfxMclMessage>& msg);
    void handleBtSapTransferApdu(const sp<RfxMclMessage>& msg);
    void handleBtSapTransferApduResponse(const sp<RfxMclMessage>& msg);
    void handleBtSapTransferAtr(const sp<RfxMclMessage>& msg);
    void handleBtSapPower(const sp<RfxMclMessage>& msg);
    void handleBtSapPowerResponse(const sp<RfxMclMessage>& msg);
    void handleBtSapResetSim(const sp<RfxMclMessage>& msg);
    void handleBtSapResetSimResponse(const sp<RfxMclMessage>& msg);
    void handleBtSapSetTransferProtocol(const sp<RfxMclMessage>& msg);
    // BTSAP - END
    bool isOp09Card(const char *iccid);
    void handleSetSimCardPower(const sp < RfxMclMessage > & msg);
    void handleSetSimCardPowerResponse(const sp < RfxMclMessage > & msg);
    bool isEnableUiccApplicationsSupported();
    void handleEnableUiccApplications(const sp < RfxMclMessage > & msg);
    void handleEnableUiccApplicationsResponse(const sp < RfxMclMessage > & msg);
    void handleAreUiccApplicationsEnabled(const sp < RfxMclMessage > & msg);
    void handleAreUiccApplicationsEnabledResponse(const sp < RfxMclMessage > & msg);
    void handleSetSimPower(const sp < RfxMclMessage > & msg);
    void handleSetSimPowerResponse(const sp < RfxMclMessage > & msg);
    void handleEventGetSlotStatus(const sp<RfxMclMessage>& msg);
    void handleEventGetSlotStatusResponse(const sp<RfxMclMessage>& msg);
    void handleGetSlotStatus(const sp<RfxMclMessage>& msg);
    void handleGetSlotStatusResponse(const sp<RfxMclMessage>& msg);
    void handleSetSimSlotsMapping(const sp<RfxMclMessage>& msg);
    void handleSetSimSlotsMappingResponse(const sp<RfxMclMessage>& msg);
    void handleActiveUiccCard(const sp<RfxMclMessage>& msg);
    void handleActiveUiccCardResponse(const sp<RfxMclMessage>& msg);
    void handleDeactivateUiccCard(const sp<RfxMclMessage>& msg);
    void handleDeactivateUiccCardResponse(const sp<RfxMclMessage>& msg);
    void handleGetImsi(const sp<RfxMclMessage>& msg);
    void handleGetImsiResponse(const sp<RfxMclMessage>& msg);
    void handleGetImsiGsmResponse(const sp<RfxMclMessage>& msg);
    void handleGetImsiCdmaResponse(const sp<RfxMclMessage>& msg);
    void handleQueryFacilityLock(const sp<RfxMclMessage>& msg);
    void handleQueryFacilityLockResponse(const sp<RfxMclMessage>& msg);
    void handleSetFacilityLock(const sp<RfxMclMessage>& msg);
    void handleSetFacilityLockResponse(const sp<RfxMclMessage>& msg);

    void querySimTypeInfo();
    void handleEventGetSimTypeInfo(const sp<RfxMclMessage>& msg);
    void handleEventGetSimTypeInfoResponse(const sp<RfxMclMessage>& msg);
    void handleGetSimTypeInfo(const sp<RfxMclMessage>& msg);
    void handleGetSimTypeInfoResponse(const sp<RfxMclMessage>& msg);
    void handleSwitchSimType(const sp<RfxMclMessage>& msg);
    void handleSwitchSimTypeResponse(const sp<RfxMclMessage>& msg);

    void handleCdmaSubscription(const sp<RfxMclMessage>& msg);
    void handleCdmaGetSubSource(const sp<RfxMclMessage>& msg);
    void handleCdmaGetSubSourceResponse(const sp<RfxMclMessage>& msg);
    void queryCdma3gDualActivation();
    void queryCdmaMlpl(const sp<RfxMclMessage>& msg);
    void queryCdmaMspl(const sp<RfxMclMessage>& msg);
    /// void enableEuiccSupport();

    void querySimMepSupport();
    void handleGetMepSlotStatus(const sp<RfxMclMessage>& msg);
    void handleGetMepSlotStatusResponse(const sp<RfxMclMessage>& msg);
    void handleEventGetMepSlotStatus(const sp<RfxMclMessage>& msg);
    void handleEventGetMepSlotStatusResponse(const sp<RfxMclMessage>& msg);
    void handleSetMepSimSlotsMapping(const sp<RfxMclMessage>& msg);

};
#endif /* __RMM_SIM_COMM_REQUEST_HANDLER_H__ */

