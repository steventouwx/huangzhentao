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

#include "RfxBaseHandler.h"
#include "RfxSimIoRspData.h"
#include "RfxSimIoData.h"
#include "RfxSimAuthData.h"
#include "RfxSimGenAuthData.h"
#include "RfxSimOpenChannelData.h"
#include "RfxSimApduData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxIntsData.h"
#include "RfxSimStatusData.h"
#include "RfxRawData.h"
#include "RmmSimBaseHandler.h"
#include "RmmPhbSimIoRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include "RfxChannel.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <pthread.h>
#include "RfxMessageId.h"
#include "RfxRilUtils.h"
#include "RfxLog.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <hardware_legacy/mtk_power.h>
#include "base64.h"
#include "usim_fcp_parser.h"
#include <platformlib/common/libmtkrilutils/proto/sap-api.pb.h>
#include "pb_decode.h"
#include "pb_encode.h"
#ifdef __cplusplus
}
#endif

#define PHBIO_LOG_TAG "RmmPhbReq"
#define DlogD(x...) if (mIsEngLoad == 1) logD( x )

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmPhbSimIoRequestHandler, RIL_CMD_PROXY_7);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimIoData, RfxSimIoRspData, RFX_MSG_REQUEST_PHB_SIM_IO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxSimIoData, RfxSimIoRspData, RFX_MSG_REQUEST_PHB_PBR_SIM_IO);

RmmPhbSimIoRequestHandler::RmmPhbSimIoRequestHandler(int slot_id, int channel_id):RmmSimBaseHandler(slot_id, channel_id) {
    const int request[] = {
        RFX_MSG_REQUEST_PHB_SIM_IO,
        RFX_MSG_REQUEST_PHB_PBR_SIM_IO,
    };

    registerToHandleRequest(request, sizeof(request)/sizeof(int));
    mIsEngLoad = RfxRilUtils::isEngLoad();
}

RmmPhbSimIoRequestHandler::~RmmPhbSimIoRequestHandler() {
}

void RmmPhbSimIoRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    DlogD(PHBIO_LOG_TAG, "onHandlePhbSimIoRequest: %d", msg->getId());
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_PHB_SIM_IO:
        case RFX_MSG_REQUEST_PHB_PBR_SIM_IO:
            requestHandlePhbSimIo(msg);
            break;
        default:
            RFX_ASSERT(0);
            break;
    }
}

void RmmPhbSimIoRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_PHB_SIM_IO:
        case RFX_MSG_REQUEST_PHB_PBR_SIM_IO:
            handlePhbSimIoResponse(msg);
            break;
    }
}


void RmmPhbSimIoRequestHandler::handlePhbSimIoResponse(const sp<RfxMclMessage>& msg) {
    RIL_SIM_IO_v6 *pData = (RIL_SIM_IO_v6*)(msg->getData()->getData());
    String8 path("");
    String8 aid((pData->aidPtr != NULL)? pData->aidPtr : "");
    // String8 data((pData->data != NULL)? pData->data : "");
    char *simResponse = NULL;
    int appTypeId = -1;  // channelId = 0;
    // int remain = pData->p3;
    RIL_Errno result = RIL_E_SUCCESS;
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    uint16_t sw = 0;
    uint16_t apdu_len = 0;

    memset(&sr, 0, sizeof(RIL_SIM_IO_Response));
    // p_response = atSendCommandSingleline(cmd.string(), "+ECRLA:");

    RfxMipcData* mipcData = msg->getMipcData();
    int  mipc_result = mipcData->getResult();

    if (mipc_result != MIPC_RESULT_SUCCESS) {
        logD(PHBIO_LOG_TAG, "handleSimIoResponse, token:%d, mipc_result:%d",
                msg->getToken(), mipc_result);
        // if (channelId == 255) result = RIL_E_SIM_BUSY;
        switch (mipc_result) {
            case MIPC_RESULT_SIM_EXT_PIN2_REQUIRED:
                result = RIL_E_SIM_PIN2;
                break;
            case MIPC_RESULT_SIM_EXT_PUK2_REQUIRED:
                result = RIL_E_SIM_PUK2;
                break;
            case MIPC_RESULT_OPERATION_NOT_SUPPORTED:
                result = RIL_E_REQUEST_NOT_SUPPORTED;
                break;
            case MIPC_RESULT_BUSY:
                result = RIL_E_SIM_BUSY;
                break;
            default:
                result = RIL_E_SIM_ERR;
                break;
        }

        goto error;
    }


    sw = mipcData->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_SW, 0);
    apdu_len =
            mipcData->getMipcUint16Val(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_LEN, 0);
    if (apdu_len > 0) {
        uint16_t len = 0;
        simResponse =
                (char*) mipcData->getMipcVal(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF_T_RESP_APDU, &len);
        sr.simResponse = (char*)calloc(1, (sizeof(char)* (apdu_len + 1)));
        RFX_ASSERT(sr.simResponse != NULL);
        memcpy(sr.simResponse, simResponse, apdu_len);
        sr.simResponse[apdu_len] = '\0';

        // logD(PHBIO_LOG_TAG, "handleSimIoResponse, md response:%.6s", sr.simResponse);

        // Run FCP parser for USIM and CSIM when the command is GET_RESPONSE
        appTypeId = queryAppTypeId(aid);
        if ((appTypeId == UICC_APP_USIM || appTypeId == UICC_APP_CSIM ||
                appTypeId == UICC_APP_ISIM || isSimIoFcp(sr.simResponse))
                && pData->command == 192 /* GET_RESPONSE */) {
            makeSimRspFromUsimFcp((unsigned char**)&(sr.simResponse), apdu_len, pData->fileid);
        } else {
            char* temp = sr.simResponse;
            sr.simResponse = (char*)byteArrayToHexString((unsigned char*)sr.simResponse, apdu_len);
            free(temp);
        }
    }

    sr.sw2 = sw & 0xFF;
    sr.sw1 = (sw >> 8) & 0xFF;

    logD(PHBIO_LOG_TAG, "handleSimIoResponse, token:%d, result:%d, (sw:%x, sw1:%x, sw2:%x, rsp:%d,%.6s)",
            msg->getToken(), mipc_result, sw, sr.sw1, sr.sw2, apdu_len, sr.simResponse);


    if ((sr.sw1 == 0x00) && (sr.sw2 == 0x00)) {
        result = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    } else if ((sr.sw1 == 0x69) && (sr.sw2 == 0x82)) {
        result = RIL_E_OPERATION_NOT_ALLOWED;
        goto error;
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);
    responseToTelCore(response);

    if (sr.simResponse != NULL) {
        free(sr.simResponse);
    }
    return;

error:
    response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);

    responseToTelCore(response);

    if (sr.simResponse != NULL) {
        free(sr.simResponse);
    }
}

void RmmPhbSimIoRequestHandler::requestHandlePhbSimIo(const sp<RfxMclMessage>& msg) {
    RIL_SIM_IO_v6 *pData = (RIL_SIM_IO_v6*)(msg->getData()->getData());
    String8 aid((pData->aidPtr != NULL)? pData->aidPtr : "");
    int appTypeId = -1;
    RIL_Errno result = RIL_E_SIM_ERR;
    sp<RfxMipcData> mipc_data = NULL;
    uint8_t sim_app_id = 0;
    unsigned char *byteData = NULL;

    appTypeId = queryAppTypeId(aid);
    if (appTypeId == -1) {
        result = RIL_E_REQUEST_NOT_SUPPORTED;
        goto error;
    }

    if (pData->pin2 != NULL && strlen(pData->pin2) > 0) {
        sp<RfxMipcData> mipc_pin2 = NULL;
        sp<RfxMipcData> pin2_cnf = NULL;
        logE(PHBIO_LOG_TAG, "requestHandlePhbSimIo verify PIN2!");
        mipc_pin2 =  RfxMipcData::obtainMipcData(MIPC_SIM_VERIFY_PIN_REQ, m_slot_id);
        mipc_pin2->addMipcTlvUint8(MIPC_SIM_VERIFY_PIN_REQ_T_PIN_TYPE, MIPC_SIM_PIN_TYPE_PIN2);
        mipc_pin2->addMipcTlvStr(MIPC_SIM_VERIFY_PIN_REQ_T_PIN_CODE, pData->pin2);
        pin2_cnf = callToMipcMsgSync(mipc_pin2);
        if (pin2_cnf == NULL) {
            logE(PHBIO_LOG_TAG, "requestHandlePhbSimIo verify PIN2 fail");
            goto error;
        } else {
            int  mipc_result = pin2_cnf->getResult();
            if (mipc_result != MIPC_RESULT_SUCCESS) {
                logE(PHBIO_LOG_TAG, "requestHandlePhbSimIo PIN2 fail result:%d", mipc_result);
                if (mipc_result == MIPC_RESULT_SIM_EXT_INCORRECT_PASS_WORD) {
                    result = RIL_E_PASSWORD_INCORRECT;
                }
                goto error;
            }
        }
    }

    // modem will check if service active, if not, return not support.

    // if (pData->pin2 != NULL && strlen(pData->pin2) > 0) {
    // may be need enter pin2
    // cmd.append(String8::format("AT+EPIN2=\"%s\"", pData->pin2));

    // 1.SIM access
    // Remove 3F00 or 3f00 for path
    // if path has 7f25 or 7F25 and the CDMA 3G card is switched into SIM, go to error

    // 2. Query channel id
    // if channelId == 255,  result = RIL_E_SIM_BUSY, go to error
    // cmd.append(String8::format("AT+ESIMAPP=%d,0", appTypeId));
    // p_response = atSendCommandSingleline(cmd.string(), "+ESIMAPP:");

    logD(PHBIO_LOG_TAG, "handleSimIo, token:%d, SIM_IO(%d, %x, %d, %d, %d, %s)", msg->getToken(), pData->command,
            pData->fileid, pData->p1, pData->p2, pData->p3, pData->path);

    // fileid is ADN, command is COMMAND_READ_RECORD or COMMAND_UPDATE_RECORD
    if ((pData->command == 0xb2 || pData->command == 0xdc) && (pData->fileid == 0x6F3A)
            && (msg->getId() == RFX_MSG_REQUEST_PHB_SIM_IO)) {
        int phbVersion = getNonSlotMclStatusManager()->getIntValue(
                RFX_STATUS_KEY_SIM_PHONEBOOK_VERSION, PHB_VERSION_UNKNOWN);

        // check phonebook version
        if (phbVersion == PHB_VERSION_UNKNOWN) {
            getNonSlotMclStatusManager()->setIntValue(RFX_STATUS_KEY_SIM_PHONEBOOK_VERSION,
                    PHB_VERSION_SIMIO);
        } else if (phbVersion == PHB_VERSION_MTK) {
            logE(PHBIO_LOG_TAG, "Should not here, token:%d, SIM_IO(%d, %x, %d, %d, %d, %s)",
                    msg->getToken(), pData->command, pData->fileid, pData->p1, pData->p2,
                    pData->p3, pData->path);
            RFX_ASSERT(0);
        }
    }

    mipc_data = RfxMipcData::obtainMipcData(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ, m_slot_id);
    sim_app_id = getMipcAppTypeFromRil(appTypeId);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_APP_ID, sim_app_id);
    // mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_SESSION_ID, (uint8_t)channelId);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_CMD, (uint8_t)pData->command);
    mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_FILE_ID, (uint16_t)pData->fileid);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P1, (uint8_t)pData->p1);
    mipc_data->addMipcTlvUint8(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P2, (uint8_t)pData->p2);
    if ((pData->command == 192) || (pData->command == 242)) {
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, 0);
    } else {
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_P3_EX, (uint16_t)pData->p3);
    }
    if ((pData->data != NULL)) {
        uint16_t bLen = 0;
        uint16_t len = strlen(pData->data);
        bLen = hexStringToByteArrayEx((unsigned char*)pData->data, len, &byteData);
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, bLen);
        mipc_data->addMipcTlv(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA, bLen, byteData);  // Byte_array
    } else {
        mipc_data->addMipcTlvUint16(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_DATA_LEN, 0);
    }
    if (pData->path != NULL) {
        if ((strstr(pData->path, "3F00") || strstr(pData->path, "3f00")) && (strlen(pData->path) > 4)) {
            mipc_data->addMipcTlvStr(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, pData->path + 4);
        } else {
            mipc_data->addMipcTlvStr(MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ_T_PATH, pData->path);
        }
    }

    callToMipcMsgAsync(msg, mipc_data);

    if (byteData != NULL) {
        free(byteData);
    }

    // if (((pData->command == 192) || (pData->command == 242)) && (remain <= 0)) {
        // Set remain value same with AOSP define for get_response command.
    //    remain = 15; ///??? for what TODO
    // }
    /*while (remain > 0) {
        // 3. Send AT+ECRLA
        result = RIL_E_SIM_ERR;

        if ((pData->command == 192) || (pData->command == 242)) {
            // GET RESPONSE or STATUS
            cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d", appTypeId, channelId,
                    pData->command, pData->fileid, pData->p1, pData->p2, 0 ));
        } else if (data.isEmpty()) {
            // READ COMMAND, P3: Max value is 256 and use 0 to represent 256
            len = ((remain < 256) ? remain : 256);
            cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d", appTypeId, channelId,
                    pData->command, pData->fileid, (0xFF & (offset >> 8)),  ??????
                    (0xFF & offset),
                    ((remain < 256) ? remain : 0)));
        } else {
            // WRITE COMMAND, P3: Max value is 255
            len = (remain > 255) ? 255 : remain;

            cmd.append(String8::format("AT+ECRLA=%d,%d,%d,%d,%d,%d,%d", appTypeId, channelId,
                    pData->command, pData->fileid, (0xFF & (offset >> 8)), ??????
                    (0xFF & offset),
                    len));
        }
    } */

    return;
error:
    sp<RfxMclMessage> response;
    RIL_SIM_IO_Response sr;
    memset(&sr, 0, sizeof(RIL_SIM_IO_Response));
    response = RfxMclMessage::obtainResponse(msg->getId(), result,
            RfxSimIoRspData((void*)&sr, sizeof(sr)), msg, false);

    responseToTelCore(response);
}

int RmmPhbSimIoRequestHandler::toByte(char c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    logD(PHBIO_LOG_TAG, "toByte, Error: %c", c);
    return 0;
}

int RmmPhbSimIoRequestHandler::hexStringToByteArrayEx(unsigned char* hexString,
        int hexStringLen, unsigned char ** byte) {
    int length = hexStringLen/2;
    unsigned char* buffer = (unsigned char*)malloc((length + 1)*sizeof(char));
    RFX_ASSERT(buffer != NULL);
    int i = 0;

    memset(buffer, 0, ((length + 1)*sizeof(char)));
    for (i = 0 ; i < hexStringLen ; i += 2) {
        buffer[i / 2] = (unsigned char)((toByte(hexString[i]) << 4) | toByte(hexString[i+1]));
    }

    *byte = buffer;

    return (hexStringLen/2);
}

