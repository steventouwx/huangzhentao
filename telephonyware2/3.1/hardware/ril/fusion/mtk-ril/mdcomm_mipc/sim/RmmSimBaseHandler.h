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

#ifndef __RMM_SIM_BASE_HANDLER_H__
#define __RMM_SIM_BASE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxBaseHandler.h"
#include "RfxMipcData.h"
#include "RmmCommSimDefs.h"
#include "ratconfig.h"

#define SINGLE_MODE_SIM_CARD  10
#define SINGLE_MODE_USIM_CARD  20
#define SINGLE_MODE_RUIM_CARD  30
#define DUAL_MODE_CG_CARD  40
#define DUAL_MODE_UG_CARD  50
#define CT_NATIONAL_ROAMING_CARD  41
#define CU_DUAL_MODE_CARD  42
#define DUAL_MODE_TELECOM_LTE_CARD  43
#define UNKNOWN_CARD  -1

#define MAX_RETRY_COUNT  30

/*****************************************************************************
 * Class RpSimController
 *****************************************************************************/

class RmmSimBaseHandler : public RfxBaseHandler {
RFX_DECLARE_HANDLER_CLASS(RmmSimBaseHandler);


protected:
    enum SIM_HANDLE_RESULT {
        RESULT_NEED,
        RESULT_IGNORE,

        RESULT_END
    };

    String8 mTag;

public:
    RmmSimBaseHandler(int slot_id, int channel_id);
    virtual ~RmmSimBaseHandler();

    // Process URC here
    //virtual void handleUrc(const sp<RfxMclMessage>& msg, RfxAtLine *urc);

    // Process Request here
    //virtual void handleRequest(const sp<RfxMclMessage>& msg);

    //virtual void handleEvent(const sp<RfxMclMessage>& msg) {RFX_UNUSED(msg);}

// Override
protected:
    int queryAppTypeId(String8 aid);

    void setTag(String8 s);
    bool isCommontSlotSupport();
    bool isSimInserted();
    bool isSimSlotLockSupport();
    bool getIccId(char* value);
    RmmSimPinPukCount* getPinPukRetryCount(void);
    void setPinPukRetryCountProp(RmmSimPinPukCount *retry);
    void setPin1RetryCountProp(int retry_count);
    void setPin2RetryCountProp(int retry_count);
    void setPuk1RetryCountProp(int retry_count);
    void setPuk2RetryCountProp(int retry_count);
    UICC_Status getSimStatus();
    // Convert all the lower case characters in string to upper case
    char* stringToUpper(char *str);
    void sendSimStatusChanged();
    bool isAOSPPropSupport();
    bool isSimIoFcp(char* response);
    void updateCdmaCardType(const char *iccid);
    bool isOp09Card(const char *iccid);
    //int isApplicationIdExist(const char *aid);
    void makeSimRspFromUsimFcp(unsigned char ** simResponse, unsigned short fcpLen, int fileId = 0);
    void resetSimPropertyAndStatusKey();
    //RIL_SimSlotStatus** decodeEslotsTlvInfo(sp<RfxMipcData> spData, RfxMipcData *mipc_data, int *decodeerr, int *slotnum);
    RIL_SimSlotStatus** decodeEslotsinfo(sp<RfxMipcData> spData, RfxMipcData *mipc_data, int *decodeerr, int *slotnum);
    RIL_SimSlotStatus** decodeMepSlotsinfo(sp<RfxMipcData> spData, RfxMipcData *mipc_data, int *decodeerr, int *slotnum);
    void sendIccidChanged(const char *iccid);
    RIL_AppType getRilAppTypeFromMipc(uint8_t mipc_app_type);
    uint8_t getMipcAppTypeFromRil(int appTypeId);
    RIL_AppState getRilAppStateFromMipc(uint8_t mipc_app_state);
    RIL_PersoSubstate getPersoSubstateFromMipc(uint8_t mipc_sub_state);
    UICC_Status getUiccStatusFromMipc(uint8_t mipc_status);
    int getSimStatebyUiccStatus(int uicc_status);
};
#endif /* __RMM_SIM_BASE_HANDLER_H__ */

