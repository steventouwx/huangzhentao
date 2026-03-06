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

#include "RmcEcallUrcHandler.h"
#include "rfx_properties.h"
#include "RfxStringData.h"
#include "rfx_properties.h"
#include "RfxEcallIndicationData.h"
#include <string.h>

#define RFX_LOG_TAG "RmcEcallUrcHandler"

#define MAX_PROP_CHARS   50

/*****************************************************************************
 * Class RmcEcallUrcHandler
 *****************************************************************************/


RFX_IMPLEMENT_HANDLER_CLASS(RmcEcallUrcHandler, RIL_CMD_PROXY_URC);

// register data
//RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_CC_GSM_SIM_ECC);


RmcEcallUrcHandler::RmcEcallUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {
        "+EMSDHACK",
        "+CECN",
        "+EECREGS",
    };

    registerToHandleURC(urc, sizeof(urc)/sizeof(char *));
}

RmcEcallUrcHandler::~RmcEcallUrcHandler() {
}

void RmcEcallUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    String8 ss(msg->getRawUrc()->getLine());

    logV(RFX_LOG_TAG, "[%s] URC: %s", __FUNCTION__, ss.string());
    if (ss.find("+EMSDHACK") == 0) {
        handleMsdHack(msg);
    } else if (ss.find("+CECN") == 0) {
        handleImsEcallIndication(msg);
    } else if (ss.find("+EECREGS") == 0) {
        handleEcallDeregistrationIndication(msg);
    }
}

void RmcEcallUrcHandler::handleMsdHack(const sp<RfxMclMessage>& msg) {
    /*
    * +EMSDHACK: <status>, ...
    * <status>: 1/0 to report msd send status
    */
    const int maxLen = 1;
    int rfxMsg = RFX_MSG_URC_ECALL_MSDHACK;
    // int rfxMsg = RFX_MSG_UNSOL_IMS_SUPPORT_ECC;
    // get <s1_support> and send Urc
    notifyIntsDataToTcl(msg, rfxMsg, maxLen);
}

void RmcEcallUrcHandler::handleImsEcallIndication(const sp<RfxMclMessage>& msg) {
    /*
    * +CECN: <domain>
    * <domain>: 0 = success; 1 = resend; 100 = fallback to CS
    */

    int err;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) return;

    int indAT = line->atTokNextint(&err);
    if (err < 0) return;

    logD(RFX_LOG_TAG, "CECN:%d", indAT);

    RIL_ECall_Indication imsInd;

    switch (indAT) {
    case 0:
        imsInd = RIL_UNSOL_ECALL_IMS_MSD_ACK;
        break;
    case 1:
        imsInd = RIL_UNSOL_ECALL_IMS_UPDATE_MSD;
        break;
    case 100:
        imsInd = RIL_UNSOL_ECALL_IMS_IN_BAND_TRANSFER;
        break;
    case 101:
        imsInd = RIL_UNSOL_ECALL_IMS_SRVCC;
        break;
    case 102:
        imsInd = RIL_UNSOL_ECALL_IMS_MSD_NACK;
        break;
    default:
        imsInd = RIL_UNSOL_ECALL_UNSPECIFIED;
        break;
    }

    RIL_Ecall_Unsol_Indications* indication = (RIL_Ecall_Unsol_Indications*)calloc(1, sizeof(RIL_Ecall_Unsol_Indications));
    indication->ind = imsInd;
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_ECALL_INDICATIONS, m_slot_id,
            RfxEcallIndicationData(indication, sizeof(RIL_Ecall_Unsol_Indications)));
    responseToTelCore(urc);
    free(indication);
}

void RmcEcallUrcHandler::handleEcallDeregistrationIndication(const sp<RfxMclMessage>& msg) {
    /*
     * +EECREGS: <dereg_state>
     * 0: deregister from NW, enter the eCall inactivity procedure
     * 1: register to NW, leave the eCall inactivity procedure
     */

    int err;
    RfxAtLine *line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) return;

    int value = line->atTokNextint(&err);
    if (err < 0) return;
    logD(RFX_LOG_TAG, "EECREGS:%d", value);

    RIL_ECall_Indication eCallInd = RIL_UNSOL_ECALL_UNSPECIFIED;
    if (value == 0) {
        eCallInd = RIL_UNSOL_ECALL_ONLY_DEREGISTRATION;
    }

    RIL_Ecall_Unsol_Indications* indication = (RIL_Ecall_Unsol_Indications*)
            calloc(1, sizeof(RIL_Ecall_Unsol_Indications));
    indication->ind = eCallInd;
    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_ECALL_INDICATIONS, m_slot_id,
            RfxEcallIndicationData(indication, sizeof(RIL_Ecall_Unsol_Indications)));
    responseToTelCore(urc);
    free(indication);
}
