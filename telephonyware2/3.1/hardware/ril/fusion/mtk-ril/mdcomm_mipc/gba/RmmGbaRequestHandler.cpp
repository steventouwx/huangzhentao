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

#include "RmmGbaRequestHandler.h"
#include "RfxMessageId.h"
#include "RfxMipcData.h"
#include "RfxRilUtils.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include <telephony/mtk_ril.h>
#include <string.h>
#include <dlfcn.h>

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxStringsData, RFX_MSG_REQUEST_RUN_GBA);

static const int requests[] = {
    RFX_MSG_REQUEST_RUN_GBA
};

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmGbaRequestHandler, RIL_CMD_PROXY_6);

RmmGbaRequestHandler::RmmGbaRequestHandler(int slot_id,
        int channel_id):RfxBaseHandler(slot_id, channel_id){
    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
}

RmmGbaRequestHandler::~RmmGbaRequestHandler() {
}

void RmmGbaRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(TAG, "onHandleRequest: %s", RFX_ID_TO_STR(msg->getId()));
    }

    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_RUN_GBA:
            handleGbaRequest(msg);
            break;
        default:
            logE(TAG, "Should not be here");
            break;
    }
}

void RmmGbaRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    if (RfxRilUtils::isUserLoad() != 1) {
        logD(TAG, "onHandleResponse: %s", RFX_ID_TO_STR(msg->getId()));
    }

    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_RUN_GBA:
            handleGbaResponse(msg);
            break;
        default:
            logE(TAG, "Should not be here");
            break;
    }
}

void RmmGbaRequestHandler::handleGbaRequest(const sp<RfxMclMessage>& msg) {
    const char** strings = (const char**) (msg->getData()->getData());
    char *nafqdn = NULL;
    char *pch = NULL;
    int nafProtocolLen = 0;
    int i;
    sp<RfxMipcData> data;
    uint8_t bforcerun = (uint8_t)0;

    if (RfxRilUtils::isUserLoad() != 1) {
        logD(TAG, "strings[0]: %s", strings[0]);
        logD(TAG, "strings[1]: %s", strings[1]);
        logD(TAG, "strings[2]: %s", strings[2]);
        logD(TAG, "strings[3]: %s", strings[3]);
        logD(TAG, "length: %d", msg->getData()->getDataLength());
    } else {
        logD(TAG, "Start GBA request");
    }

    /**
     * AT+EGBA=Nafqdn,nafSecureProtocolId,bforcerun,netid
     * <Nafqdn>: Nafqdn is a string to indicate GBA key
     * <nafSecureProtocolId>: is a string for GBA protocol
     * <bforcerun>: bforcerun is a string to indicate to force run GBA or using cache.
     *                      0: no need
     *                      1: force run
     * <netid>: is an string for network access
     */

    /* check gba parameters are good or not*/
    if (msg->getData()->getDataLength() != 4 * sizeof(char*)) {
        logE(TAG, "msg data length is ilegal");
        goto error;
    }

    // check Nafqdn is legal.
    asprintf(&nafqdn, "%s", strings[0]);
    if (nafqdn == NULL) {
        logE(TAG, "allocate Nafqdn fail");
        goto error;
    }
    pch = strchr(nafqdn, ',');
    if (pch != NULL) {
        logE(TAG, "Nafqdn is ilegal");
        free(nafqdn);
        goto error;
    }
    pch = strchr(nafqdn, '.');
    if (pch == NULL) {
        logE(TAG, "Nafqdn is ilegal");
        free(nafqdn);
        goto error;
    }
    free(nafqdn);

    // check naf secure protocol id is legal.
    nafProtocolLen = strlen(strings[1]);
    if (nafProtocolLen != 10) {
        logE(TAG, "Naf secure protocol id is ilegal (length is incorrect)");
        goto error;
    }
    for (i = 0; i < nafProtocolLen; i++) {
        if (!((*(strings[1] + i) >= 'A' && *(strings[1] + i) <= 'F')
           || (*(strings[1] + i) >= 'a' && *(strings[1] + i) <= 'f')
           || (*(strings[1] + i) >= '0' && *(strings[1] + i) <= '9'))) {
            logE(TAG, "Naf secure protocol id is ilegal");
            goto error;
        }
    }

    // check bforcerun is 0 or 1
    if (strcmp(strings[2], "0") != 0 && strcmp(strings[2], "1") != 0) {
        logE(TAG, "bforcerun is ilegal");
        goto error;
    }

    bforcerun = (uint8_t)atoi(strings[2]);
    data = RfxMipcData::obtainMipcData(MIPC_SS_RUN_GBA_REQ,
            msg->getSlotId());
    data->addMipcTlvStr(MIPC_SS_RUN_GBA_REQ_T_NAF_FQDN, strings[0]);
    data->addMipcTlvStr(MIPC_SS_RUN_GBA_REQ_T_NAF_SECURE_PROTOCOL_ID, strings[1]);
    data->addMipcTlvUint8(MIPC_SS_RUN_GBA_REQ_T_FORCE_RUN, bforcerun);
    data->addMipcTlvUint32(MIPC_SS_RUN_GBA_REQ_T_NET_ID, atoi(strings[3]));
    callToMipcMsgAsync(msg, data);
    return;

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
            RIL_E_GENERIC_FAILURE, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}

void RmmGbaRequestHandler::handleGbaResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    char *responses[4] = {NULL, NULL, NULL, NULL};

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result = mipcData->getResult();
    if (result != MIPC_RESULT_SUCCESS) {
        logE(TAG, "handleGbaResponse Fail, %d", result);
        goto error;
    }

    // <key>GBA key
    responses[0] = mipcData->getMipcStr(MIPC_SS_RUN_GBA_CNF_T_KEY);
    // <key_length> key length
    responses[1] = mipcData->getMipcStr(MIPC_SS_RUN_GBA_CNF_T_KEY_LEN);
    // <btid> bitId
    responses[2] = mipcData->getMipcStr(MIPC_SS_RUN_GBA_CNF_T_BIT_ID);
    // <keylifetime> the life time of key
    responses[3] = mipcData->getMipcStr(MIPC_SS_RUN_GBA_CNF_T_KEY_LIFETIME);

    if (RfxRilUtils::isUserLoad() != 1) {
        logD(TAG, "handleGbaResponse: key=%s, key_length=%s, btid=%s, keylifetime=%s",
                responses[0], responses[1],responses[2],responses[3]);
    } else {
        logD(TAG, "handleGbaResponse finished");
    }
    ret = RIL_E_SUCCESS;

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxStringsData((void*)responses, sizeof(responses)), msg, false);
    // response to TeleCore
    responseToTelCore(response);
}
