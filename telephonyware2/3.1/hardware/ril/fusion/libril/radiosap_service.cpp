/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2022. All rights reserved.
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

#include <ril_service.h>
#include <mtk_log.h>
#include <android/binder_manager.h>
#include <libmtkrilutils.h>
#include <radiosap_service.h>
#include <rilAidlUtils.h>
#include <stdlib.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RIL_SAP"
//using ::android::hardware::configureRpcThreadpool;

namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_radio_sap = ::aidl::android::hardware::radio::sap;
using rilAidlUtils::retChecker;

struct RadioSapImpl {
    std::shared_ptr<aidl_radio_sap::ISapCallback> sapCallback;
};

RadioSapImpl radioSapServiceImpl[MAX_SIM_COUNT];

void *sapDecodeMessage(MsgId msgId, MsgType msgType, uint8_t *payloadPtr, size_t payloadLen) {
    void *responsePtr = NULL;
    pb_istream_t stream;

    /* Create the stream */
    stream = pb_istream_from_buffer((uint8_t *)payloadPtr, payloadLen);

    /* Decode based on the message id */
    switch (msgId)
    {
        case MsgId_RIL_SIM_SAP_CONNECT:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_CONNECT_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_CONNECT_RSP_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_CONNECT_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_DISCONNECT:
            if (msgType == MsgType_RESPONSE) {
                responsePtr = malloc(sizeof(RIL_SIM_SAP_DISCONNECT_RSP));
                if (responsePtr) {
                    if (!pb_decode(&stream, RIL_SIM_SAP_DISCONNECT_RSP_fields, responsePtr)) {
                        mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_DISCONNECT_RSP");
                        free(responsePtr);
                        responsePtr = nullptr;
                        return NULL;
                    }
                }
            } else {
                responsePtr = malloc(sizeof(RIL_SIM_SAP_DISCONNECT_IND));
                if (responsePtr) {
                    if (!pb_decode(&stream, RIL_SIM_SAP_DISCONNECT_IND_fields, responsePtr)) {
                        mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_DISCONNECT_IND");
                        free(responsePtr);
                        responsePtr = nullptr;
                        return NULL;
                    }
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_APDU:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_APDU_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_APDU_RSP_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_APDU_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_TRANSFER_ATR:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_TRANSFER_ATR_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_TRANSFER_ATR_RSP_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_TRANSFER_ATR_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_POWER:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_POWER_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_POWER_RSP_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_POWER_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_RESET_SIM:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_RESET_SIM_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_RESET_SIM_RSP_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_RESET_SIM_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_STATUS:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_STATUS_IND));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_STATUS_IND_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_STATUS_IND");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_fields,
                        responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_ERROR_RESP:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_ERROR_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_ERROR_RSP_fields, responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_ERROR_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        case MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL:
            responsePtr = malloc(sizeof(RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP));
            if (responsePtr) {
                if (!pb_decode(&stream, RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP_fields,
                        responsePtr)) {
                    mtkLogE(LOG_TAG, "Error decoding RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP");
                    free(responsePtr);
                    responsePtr = nullptr;
                    return NULL;
                }
            }
            break;

        default:
            break;
    }
    return responsePtr;
} /* sapDecodeMessage */

int getSapSlot(RilSapSocket *sapSocket) {
    switch (sapSocket->getSocketId()) {
        case RIL_SOCKET_1:
            mtkLogD(LOG_TAG, "getSapSlot: returning slot0");
            return 0;
        case RIL_SOCKET_2:
            mtkLogD(LOG_TAG, "getSapSlot: returning slot1");
            return 1;
        case RIL_SOCKET_3:
            mtkLogD(LOG_TAG, "getSapSlot: returning slot2");
            return 2;
        case RIL_SOCKET_4:
            mtkLogD(LOG_TAG, "getSapSlot: returning slot3");
            return 3;
        default:
            mtkLogD(LOG_TAG, "default:    returning slot0");
            return 0;
    }
}

RadioSapImpl getSapImpl(RilSapSocket *sapSocket) {
    switch (sapSocket->getSocketId()) {
        case RIL_SOCKET_1:
            mtkLogD(LOG_TAG, "getSapImpl: returning sapService[0]");
            return radioSapServiceImpl[0];
        case RIL_SOCKET_2:
            mtkLogD(LOG_TAG, "getSapImpl: returning sapService[1]");
            return radioSapServiceImpl[1];
        case RIL_SOCKET_3:
            mtkLogD(LOG_TAG, "getSapImpl: returning sapService[2]");
            return radioSapServiceImpl[2];
        case RIL_SOCKET_4:
            mtkLogD(LOG_TAG, "getSapImpl: returning sapService[3]");
            return radioSapServiceImpl[3];
        default:
            mtkLogD(LOG_TAG, "default:    returning sapService[0]");
            return radioSapServiceImpl[0];
    }
}

aidl_radio_sap::SapResultCode convertApduResponseProtoToHal(RIL_SIM_SAP_APDU_RSP_Response responseProto) {
    switch(responseProto) {
        case RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SUCCESS:
            return aidl_radio_sap::SapResultCode::SUCCESS;
        case RIL_SIM_SAP_APDU_RSP_Response_RIL_E_GENERIC_FAILURE:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
        case RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_NOT_READY:
            return aidl_radio_sap::SapResultCode::CARD_NOT_ACCESSSIBLE;
        case RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF:
            return aidl_radio_sap::SapResultCode::CARD_ALREADY_POWERED_OFF;
        case RIL_SIM_SAP_APDU_RSP_Response_RIL_E_SIM_ABSENT:
            return aidl_radio_sap::SapResultCode::CARD_REMOVED;
        default:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
    }
}

aidl_radio_sap::SapResultCode convertTransferAtrResponseProtoToHal(
        RIL_SIM_SAP_TRANSFER_ATR_RSP_Response responseProto) {
    switch(responseProto) {
        case RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SUCCESS:
            return aidl_radio_sap::SapResultCode::SUCCESS;
        case RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_GENERIC_FAILURE:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
        case RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF:
            return aidl_radio_sap::SapResultCode::CARD_ALREADY_POWERED_OFF;
        case RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SIM_ABSENT:
            return aidl_radio_sap::SapResultCode::CARD_REMOVED;
        case RIL_SIM_SAP_TRANSFER_ATR_RSP_Response_RIL_E_SIM_DATA_NOT_AVAILABLE:
            return aidl_radio_sap::SapResultCode::DATA_NOT_AVAILABLE;
        default:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
    }
}

aidl_radio_sap::SapResultCode convertPowerResponseProtoToHal(RIL_SIM_SAP_POWER_RSP_Response responseProto) {
    switch(responseProto) {
        case RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SUCCESS:
            return aidl_radio_sap::SapResultCode::SUCCESS;
        case RIL_SIM_SAP_POWER_RSP_Response_RIL_E_GENERIC_FAILURE:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
        case RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ABSENT:
            return aidl_radio_sap::SapResultCode::CARD_REMOVED;
        case RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF:
            return aidl_radio_sap::SapResultCode::CARD_ALREADY_POWERED_OFF;
        case RIL_SIM_SAP_POWER_RSP_Response_RIL_E_SIM_ALREADY_POWERED_ON:
            return aidl_radio_sap::SapResultCode::CARD_ALREADY_POWERED_ON;
        default:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
    }
}

aidl_radio_sap::SapResultCode convertResetSimResponseProtoToHal(RIL_SIM_SAP_RESET_SIM_RSP_Response responseProto) {
    switch(responseProto) {
        case RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SUCCESS:
            return aidl_radio_sap::SapResultCode::SUCCESS;
        case RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_GENERIC_FAILURE:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
        case RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_ABSENT:
            return aidl_radio_sap::SapResultCode::CARD_REMOVED;
        case RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_NOT_READY:
            return aidl_radio_sap::SapResultCode::CARD_NOT_ACCESSSIBLE;
        case RIL_SIM_SAP_RESET_SIM_RSP_Response_RIL_E_SIM_ALREADY_POWERED_OFF:
            return aidl_radio_sap::SapResultCode::CARD_ALREADY_POWERED_OFF;
    }
    return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
}

aidl_radio_sap::SapResultCode convertTransferCardReaderStatusResponseProtoToHal(
        RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response responseProto) {
    switch(responseProto) {
        case RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response_RIL_E_SUCCESS:
            return aidl_radio_sap::SapResultCode::SUCCESS;
        case RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response_RIL_E_GENERIC_FAILURE:
            return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
        case RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP_Response_RIL_E_SIM_DATA_NOT_AVAILABLE:
            return aidl_radio_sap::SapResultCode::DATA_NOT_AVAILABLE;
    }
    return aidl_radio_sap::SapResultCode::GENERIC_FAILURE;
}

void sendFailedResponse(RadioSapImpl radioSapImpl, int32_t sapSlot, MsgId msgId, int32_t token) {
    if (radioSapImpl.sapCallback == NULL) {
        mtkLogE(LOG_TAG, "processResponse: radioSapImpl.sapCallback == NULL; msgId = %d, token = %d",
                msgId, token);
        return;
    }

    ndk::ScopedAStatus ret = ::ndk::ScopedAStatus::ok();

    switch(msgId) {
        case MsgId_RIL_SIM_SAP_CONNECT:
            ret = radioSapImpl.sapCallback->connectResponse(token,
                    aidl_radio_sap::SapConnectRsp::CONNECT_FAILURE, 0);
            break;

        case MsgId_RIL_SIM_SAP_DISCONNECT:
            ret = radioSapImpl.sapCallback->disconnectResponse(token);
            break;

        case MsgId_RIL_SIM_SAP_APDU: {
            std::vector<uint8_t> apduRsp;
            ret = radioSapImpl.sapCallback->apduResponse(token,
                    aidl_radio_sap::SapResultCode::GENERIC_FAILURE, apduRsp);
            break;
        }

        case MsgId_RIL_SIM_SAP_TRANSFER_ATR: {
            std::vector<uint8_t> atr;
            ret = radioSapImpl.sapCallback->transferAtrResponse(token,
                    aidl_radio_sap::SapResultCode::GENERIC_FAILURE, atr);
            break;
        }

        case MsgId_RIL_SIM_SAP_POWER:
            ret = radioSapImpl.sapCallback->powerResponse(token,
                    aidl_radio_sap::SapResultCode::GENERIC_FAILURE);
            break;

        case MsgId_RIL_SIM_SAP_RESET_SIM:
            ret = radioSapImpl.sapCallback->resetSimResponse(token,
                    aidl_radio_sap::SapResultCode::GENERIC_FAILURE);
            break;

        case MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            ret = radioSapImpl.sapCallback->transferCardReaderStatusResponse(token,
                    aidl_radio_sap::SapResultCode::GENERIC_FAILURE, 0);
            break;

        case MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL:
            ret = radioSapImpl.sapCallback->transferProtocolResponse(token,
                    aidl_radio_sap::SapResultCode::NOT_SUPPORTED);
            break;

        default:
            return;
    }

    if (!ret.isOk()) {
        retChecker->aospAidlReturnStatusChecker(sapSlot, ret);
        mtkLogE(LOG_TAG, "checkReturnStatus: unable to call response/indication callback.");
        radioSapImpl.sapCallback = NULL;
    }
}

void processResponse(MsgHeader *rsp, RilSapSocket *sapSocket, MsgType msgType) {
    MsgId msgId = rsp->id;
    uint8_t *data = rsp->payload->bytes;
    size_t dataLen = rsp->payload->size;

    void *messagePtr = sapDecodeMessage(msgId, msgType, data, dataLen);

    RadioSapImpl sapImpl = getSapImpl(sapSocket);
    int sapSlot = getSapSlot(sapSocket);
    if (sapImpl.sapCallback == NULL) {
        mtkLogE(LOG_TAG, "processResponse: sapCallback == NULL; msgId = %d; msgType = %d",
                msgId, msgType);
        return;
    }

    if (messagePtr == NULL) {
        mtkLogE(LOG_TAG, "processResponse: *messagePtr == NULL; msgId = %d; msgType = %d",
                msgId, msgType);
        sendFailedResponse(sapImpl, sapSlot, msgId, rsp->token);
        return;
    }

    mtkLogD(LOG_TAG, "processResponse: sapCallback != NULL; msgId = %d; msgType = %d",
            msgId, msgType);

    ndk::ScopedAStatus ret = ::ndk::ScopedAStatus::ok();
    switch (msgId) {
        case MsgId_RIL_SIM_SAP_CONNECT: {
            RIL_SIM_SAP_CONNECT_RSP *connectRsp = (RIL_SIM_SAP_CONNECT_RSP *)messagePtr;
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->connectResponse %d %d %d",
                    rsp->token,
                    connectRsp->response,
                    connectRsp->max_message_size);
            ret = sapImpl.sapCallback->connectResponse(rsp->token,
                    (aidl_radio_sap::SapConnectRsp)connectRsp->response,
                    connectRsp->max_message_size);
            break;
        }

        case MsgId_RIL_SIM_SAP_DISCONNECT:
            if (msgType == MsgType_RESPONSE) {
                mtkLogD(LOG_TAG, "processResponse: calling sapCallback->disconnectResponse %d", rsp->token);
                ret = sapImpl.sapCallback->disconnectResponse(rsp->token);
            } else {
                RIL_SIM_SAP_DISCONNECT_IND *disconnectInd =
                        (RIL_SIM_SAP_DISCONNECT_IND *)messagePtr;
                mtkLogD(LOG_TAG, "processResponse: calling sapCallback->disconnectIndication %d %d",
                        rsp->token, disconnectInd->disconnectType);
                ret = sapImpl.sapCallback->disconnectIndication(rsp->token,
                        (aidl_radio_sap::SapDisconnectType)disconnectInd->disconnectType);
            }
            break;

        case MsgId_RIL_SIM_SAP_APDU: {
            RIL_SIM_SAP_APDU_RSP *apduRsp = (RIL_SIM_SAP_APDU_RSP *)messagePtr;
            aidl_radio_sap::SapResultCode apduResponse = convertApduResponseProtoToHal(apduRsp->response);
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->apduResponse %d %d",
                    rsp->token, apduResponse);
            std::vector<uint8_t> apduRspVec;
            if (apduRsp->apduResponse != NULL && apduRsp->apduResponse->size > 0) {
                apduRspVec.resize(apduRsp->apduResponse->size);
                for (int i = 0; i < apduRsp->apduResponse->size; i++) {
                    apduRspVec[i] = apduRsp->apduResponse->bytes[i];
                }
            }
            ret = sapImpl.sapCallback->apduResponse(rsp->token, apduResponse, apduRspVec);
            break;
        }

        case MsgId_RIL_SIM_SAP_TRANSFER_ATR: {
            RIL_SIM_SAP_TRANSFER_ATR_RSP *transferAtrRsp =
                (RIL_SIM_SAP_TRANSFER_ATR_RSP *)messagePtr;
            aidl_radio_sap::SapResultCode transferAtrResponse =
                convertTransferAtrResponseProtoToHal(transferAtrRsp->response);
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->transferAtrResponse %d %d",
                    rsp->token, transferAtrResponse);
            std::vector<uint8_t> transferAtrRspVec;
            if (transferAtrRsp->atr != NULL && transferAtrRsp->atr->size > 0) {
                transferAtrRspVec.resize(transferAtrRsp->atr->size);
                for (int i = 0; i < transferAtrRsp->atr->size; i++) {
                    transferAtrRspVec[i] = transferAtrRsp->atr->bytes[i];
                }
            }
            ret = sapImpl.sapCallback->transferAtrResponse(rsp->token, transferAtrResponse,
                    transferAtrRspVec);
            break;
        }

        case MsgId_RIL_SIM_SAP_POWER: {
            aidl_radio_sap::SapResultCode powerResponse = convertPowerResponseProtoToHal(
                    ((RIL_SIM_SAP_POWER_RSP *)messagePtr)->response);
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->powerResponse %d %d",
                    rsp->token, powerResponse);
            ret = sapImpl.sapCallback->powerResponse(rsp->token, powerResponse);
            break;
        }

        case MsgId_RIL_SIM_SAP_RESET_SIM: {
            aidl_radio_sap::SapResultCode resetSimResponse = convertResetSimResponseProtoToHal(
                    ((RIL_SIM_SAP_RESET_SIM_RSP *)messagePtr)->response);
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->resetSimResponse %d %d",
                    rsp->token, resetSimResponse);
            ret = sapImpl.sapCallback->resetSimResponse(rsp->token, resetSimResponse);
            break;
        }

        case MsgId_RIL_SIM_SAP_STATUS: {
            RIL_SIM_SAP_STATUS_IND *statusInd = (RIL_SIM_SAP_STATUS_IND *)messagePtr;
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->statusIndication %d %d",
                    rsp->token, statusInd->statusChange);
            ret = sapImpl.sapCallback->statusIndication(rsp->token,
                    (aidl_radio_sap::SapStatus)statusInd->statusChange);
            break;
        }

        case MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS: {
            RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP *transferStatusRsp =
                    (RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_RSP *)messagePtr;
            aidl_radio_sap::SapResultCode transferCardReaderStatusResponse =
                    convertTransferCardReaderStatusResponseProtoToHal(
                    transferStatusRsp->response);
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->transferCardReaderStatusResponse %d %d %d",
                    rsp->token,
                    transferCardReaderStatusResponse,
                    transferStatusRsp->CardReaderStatus);
            ret = sapImpl.sapCallback->transferCardReaderStatusResponse(rsp->token,
                    transferCardReaderStatusResponse,
                    transferStatusRsp->CardReaderStatus);
            break;
        }

        case MsgId_RIL_SIM_SAP_ERROR_RESP: {
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->errorResponse %d", rsp->token);
            ret = sapImpl.sapCallback->errorResponse(rsp->token);
            break;
        }

        case MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL: {
            aidl_radio_sap::SapResultCode setTransferProtocolResponse;
            if (((RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP *)messagePtr)->response ==
                    RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_RSP_Response_RIL_E_SUCCESS) {
                setTransferProtocolResponse = aidl_radio_sap::SapResultCode::SUCCESS;
            } else {
                setTransferProtocolResponse = aidl_radio_sap::SapResultCode::NOT_SUPPORTED;
            }
            mtkLogD(LOG_TAG, "processResponse: calling sapCallback->transferProtocolResponse %d %d",
                    rsp->token, setTransferProtocolResponse);
            ret = sapImpl.sapCallback->transferProtocolResponse(rsp->token,
                    setTransferProtocolResponse);
            break;
        }

        default:
            return;
    }

    if (!ret.isOk()) {
        retChecker->aospAidlReturnStatusChecker(sapSlot, ret);
        mtkLogE(LOG_TAG, "checkReturnStatus: unable to call response/indication callback.");
        sapImpl.sapCallback = NULL;
    }

    if (messagePtr != NULL) {
        free(messagePtr);
        messagePtr = NULL;
        mtkLogD(LOG_TAG, "processResponse: free malloc buffer.");
    }
}


namespace android::hardware::radio::sap {

using ::ndk::ScopedAStatus;
namespace aidl_sap = ::aidl::android::hardware::radio::sap;
constexpr auto ok = &ScopedAStatus::ok;

MsgHeader* RadioSap::createMsgHeader(MsgId msgId, int32_t serial) {
    // Memory for msg will be freed by RilSapSocket::onRequestComplete()
    MsgHeader *msg = (MsgHeader *)calloc(1, sizeof(MsgHeader));
    if (msg == NULL) {
        return NULL;
    }
    msg->token = serial;
    msg->type = MsgType_REQUEST;
    msg->id = msgId;
    msg->error = Error_RIL_E_SUCCESS;
    return msg;
}

ScopedAStatus RadioSap::addPayloadAndDispatchRequest(MsgHeader *msg, uint16_t reqLen,
        uint8_t *reqPtr) {
    pb_bytes_array_t *payload = (pb_bytes_array_t *) malloc(sizeof(pb_bytes_array_t) - 1 + reqLen);
    if (payload == NULL) {
        sendFailedResponse(msg->id, msg->token, 2, reqPtr, msg);
        free(reqPtr);
        return ok();
    }

    msg->payload = payload;
    msg->payload->size = reqLen;
    memcpy(msg->payload->bytes, reqPtr, reqLen);

    RilSapSocket *sapSocket = RilSapSocket::getSocketById(mSocketId);
    if (sapSocket) {
        mtkLogD(LOG_TAG, "SapImpl::addPayloadAndDispatchRequest: calling dispatchRequest");
        sapSocket->dispatchRequest(msg);
    } else {
        mtkLogE(LOG_TAG, "SapImpl::addPayloadAndDispatchRequest: sapSocket is null");
        sendFailedResponse(msg->id, msg->token, 3, payload, reqPtr, msg);
        free(payload);
        free(reqPtr);
        return ok();
    }
    free(payload);
    free(reqPtr);
    return ok();
}

void RadioSap::sendFailedResponse(MsgId msgId, int32_t token, int numPointers, ...) {
    va_list ap;
    va_start(ap, numPointers);
    for (int i = 0; i < numPointers; i++) {
        void *ptr = va_arg(ap, void *);
        if (ptr) free(ptr);
    }
    va_end(ap);

    if (radioSapServiceImpl[mSlotId].sapCallback == NULL) {
        mtkLogE(LOG_TAG, "processResponse: radioSapServiceImpl[mSlotId].sapCallback == NULL; msgId = %d, slot = %d",
                msgId, mSlotId);
        return;
    }

    ndk::ScopedAStatus ret = ::ndk::ScopedAStatus::ok();

    switch(msgId) {
        case MsgId_RIL_SIM_SAP_CONNECT:
            ret = radioSapServiceImpl[mSlotId].sapCallback->connectResponse(token,
                    aidl_sap::SapConnectRsp::CONNECT_FAILURE, 0);
            break;

        case MsgId_RIL_SIM_SAP_DISCONNECT:
            ret = radioSapServiceImpl[mSlotId].sapCallback->disconnectResponse(token);
            break;

        case MsgId_RIL_SIM_SAP_APDU: {
            std::vector<uint8_t> apduRsp;
            ret = radioSapServiceImpl[mSlotId].sapCallback->apduResponse(token,
                    aidl_sap::SapResultCode::GENERIC_FAILURE, apduRsp);
            break;
        }

        case MsgId_RIL_SIM_SAP_TRANSFER_ATR: {
            std::vector<uint8_t> atr;
            ret = radioSapServiceImpl[mSlotId].sapCallback->transferAtrResponse(token,
                    aidl_sap::SapResultCode::GENERIC_FAILURE, atr);
            break;
        }

        case MsgId_RIL_SIM_SAP_POWER:
            ret = radioSapServiceImpl[mSlotId].sapCallback->powerResponse(token,
                    aidl_sap::SapResultCode::GENERIC_FAILURE);
            break;

        case MsgId_RIL_SIM_SAP_RESET_SIM:
            ret = radioSapServiceImpl[mSlotId].sapCallback->resetSimResponse(token,
                    aidl_sap::SapResultCode::GENERIC_FAILURE);
            break;

        case MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            ret = radioSapServiceImpl[mSlotId].sapCallback->transferCardReaderStatusResponse(token,
                    aidl_sap::SapResultCode::GENERIC_FAILURE, 0);
            break;

        case MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL:
            ret = radioSapServiceImpl[mSlotId].sapCallback->transferProtocolResponse(token,
                    aidl_sap::SapResultCode::NOT_SUPPORTED);
            break;

        default:
            return;
    }

    if (!ret.isOk()) {
        retChecker->aospAidlReturnStatusChecker(mSlotId, ret);
        mtkLogE(LOG_TAG, "checkReturnStatus: unable to call response/indication callback.");
        radioSapServiceImpl[mSlotId].sapCallback = NULL;
    }
}

RadioSap::RadioSap(int slotid, RIL_SOCKET_ID socketid) {
    mSlotId = slotid;
    mSocketId = socketid;
}

ScopedAStatus RadioSap::apduReq(int32_t serial,
        aidl_sap::SapApduType type,
        const std::vector<uint8_t>& command) {
    mtkLogD(LOG_TAG, "RadioSap::apduReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_APDU, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::apduReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_APDU, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_APDU_REQ *****/
    RIL_SIM_SAP_APDU_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_APDU_REQ));
    req.type = (RIL_SIM_SAP_APDU_REQ_Type)type;

    if (command.size() > 0) {
        req.command = (pb_bytes_array_t *)malloc(sizeof(pb_bytes_array_t) - 1 + command.size());
        if (req.command == NULL) {
            mtkLogE(LOG_TAG, "RadioSap::apduReq: Error allocating memory for req.command");
            sendFailedResponse(MsgId_RIL_SIM_SAP_APDU, serial, 1, msg);
            return ok();
        }
        req.command->size = command.size();
        memcpy(req.command->bytes, command.data(), command.size());
    }

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_APDU_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::apduReq: Error getting encoded size for RIL_SIM_SAP_APDU_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_APDU, serial, 2, req.command, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::apduReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_APDU, serial, 2, req.command, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::apduReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_APDU_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::apduReq: Error encoding RIL_SIM_SAP_APDU_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_APDU, serial, 3, req.command, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_APDU_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);

}

ScopedAStatus RadioSap::connectReq(int32_t serial, int32_t maxMsgSize) {
    mtkLogD(LOG_TAG, "RadioSap::connectReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_CONNECT, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::connectReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_CONNECT, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_CONNECT_REQ *****/
    RIL_SIM_SAP_CONNECT_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_CONNECT_REQ));
    req.max_message_size = maxMsgSize;

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_CONNECT_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::connectReq: Error getting encoded size for RIL_SIM_SAP_CONNECT_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_CONNECT, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::connectReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_CONNECT, serial, 1, msg);
        return ok();
    }
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::connectReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_CONNECT_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::connectReq: Error encoding RIL_SIM_SAP_CONNECT_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_CONNECT, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_CONNECT_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}

ScopedAStatus RadioSap::disconnectReq(int32_t serial) {
    mtkLogD(LOG_TAG, "RadioSap::disconnectReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_DISCONNECT, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::disconnectReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_DISCONNECT, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_DISCONNECT_REQ *****/
    RIL_SIM_SAP_DISCONNECT_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_DISCONNECT_REQ));

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_DISCONNECT_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::disconnectReq: Error getting encoded size for RIL_SIM_SAP_DISCONNECT_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_DISCONNECT, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::disconnectReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_DISCONNECT, serial, 1, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::disconnectReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_DISCONNECT_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::disconnectReq: Error encoding RIL_SIM_SAP_DISCONNECT_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_DISCONNECT, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_DISCONNECT_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}

ScopedAStatus RadioSap::powerReq(int32_t serial, bool state) {
    mtkLogD(LOG_TAG, "RadioSap::powerReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_POWER, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::powerReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_POWER, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_POWER_REQ *****/
    RIL_SIM_SAP_POWER_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_POWER_REQ));
    req.state = state;

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_POWER_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::powerReq: Error getting encoded size for RIL_SIM_SAP_POWER_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_POWER, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::powerReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_POWER, serial, 1, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::powerReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_POWER_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::powerReq: Error encoding RIL_SIM_SAP_POWER_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_POWER, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_POWER_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}

ScopedAStatus RadioSap::resetSimReq(int32_t serial) {
    mtkLogD(LOG_TAG, "RadioSap::resetSimReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_RESET_SIM, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::resetSimReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_RESET_SIM, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_RESET_SIM_REQ *****/
    RIL_SIM_SAP_RESET_SIM_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_RESET_SIM_REQ));

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_RESET_SIM_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::resetSimReq: Error getting encoded size for RIL_SIM_SAP_RESET_SIM_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_RESET_SIM, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::resetSimReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_RESET_SIM, serial, 1, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::resetSimReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_RESET_SIM_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::resetSimReq: Error encoding RIL_SIM_SAP_RESET_SIM_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_RESET_SIM, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_RESET_SIM_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}

ScopedAStatus RadioSap::setCallback(
        const std::shared_ptr<aidl_sap::ISapCallback>& sapCallback) {
    mtkLogD(LOG_TAG, "aosp sap setCallback, slotId:%d", mSlotId);

    pthread_rwlock_t *radioServiceRwlockPtr = ::radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp sap setCallback: got lock %d", mSlotId);


    radioSapServiceImpl[mSlotId].sapCallback = sapCallback;

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "aosp sap setCallback: release lock %d", mSlotId);
    return ok();
}

ScopedAStatus RadioSap::setTransferProtocolReq(
        int32_t serial,
        aidl_sap::SapTransferProtocol transferProtocol) {
    mtkLogD(LOG_TAG, "RadioSap::setTransferProtocolReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::setTransferProtocolReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ *****/
    RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ));
    req.protocol = (RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ_Protocol)transferProtocol;

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::setTransferProtocolReq: Error getting encoded size for "
                "RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::setTransferProtocolReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL, serial, 1, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::setTransferProtocolReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::setTransferProtocolReq: Error encoding "
                "RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_SET_TRANSFER_PROTOCOL, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_SET_TRANSFER_PROTOCOL_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}

ScopedAStatus RadioSap::transferAtrReq(int32_t serial) {
    mtkLogD(LOG_TAG, "RadioSap::transferAtrReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_TRANSFER_ATR, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::transferAtrReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_ATR, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_TRANSFER_ATR_REQ *****/
    RIL_SIM_SAP_TRANSFER_ATR_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_TRANSFER_ATR_REQ));

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_TRANSFER_ATR_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::transferAtrReq: Error getting encoded size for "
                "RIL_SIM_SAP_TRANSFER_ATR_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_ATR, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::transferAtrReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_ATR, serial, 1, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::transferAtrReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_TRANSFER_ATR_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::transferAtrReq: Error encoding RIL_SIM_SAP_TRANSFER_ATR_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_ATR, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_TRANSFER_ATR_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}

ScopedAStatus RadioSap::transferCardReaderStatusReq(int32_t serial) {
    mtkLogD(LOG_TAG, "RadioSap::transferCardReaderStatusReq");
    MsgHeader *msg = createMsgHeader(MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS, serial);
    if (msg == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::transferCardReaderStatusReq: Error allocating memory for msg");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS, serial, 0);
        return ok();
    }

    /***** Encode RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ *****/
    RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ req;
    memset(&req, 0, sizeof(RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ));

    size_t encodedSize = 0;
    if (!pb_get_encoded_size(&encodedSize, RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ_fields,
            &req)) {
        mtkLogE(LOG_TAG, "RadioSap::transferCardReaderStatusReq: Error getting encoded size for "
                "RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS, serial, 1, msg);
        return ok();
    }

    uint8_t *buffer = (uint8_t *)calloc(1, encodedSize);
    if (buffer == NULL) {
        mtkLogE(LOG_TAG, "RadioSap::transferCardReaderStatusReq: Error allocating memory for buffer");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS, serial, 1, msg);
        return ok();
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, encodedSize);

    mtkLogD(LOG_TAG, "RadioSap::transferCardReaderStatusReq calling pb_encode");
    if (!pb_encode(&stream, RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ_fields, &req)) {
        mtkLogE(LOG_TAG, "RadioSap::transferCardReaderStatusReq: Error encoding "
                "RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ");
        sendFailedResponse(MsgId_RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS, serial, 2, buffer, msg);
        return ok();
    }
    /***** Encode RIL_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ done *****/

    /* encoded req is payload */
    return addPayloadAndDispatchRequest(msg, stream.bytes_written, buffer);
}
}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addSapLock(std::shared_ptr<android::hardware::radio::sap::RadioSap> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_SAP_PORT_BASE 58000

static pthread_t gThreadIds[MAX_SIM_COUNT];


void addRcpVsockRadioSapAidlService(unsigned int sloid, int socketId) {
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_SAP_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_SAP_PORT_BASE + sloid, sloid);

    std::shared_ptr<android::hardware::radio::sap::RadioSap> radioSapAidl =
            ndk::SharedRefBase::make<android::hardware::radio::sap::RadioSap>(sloid, socketId);
    addSapLock(radioSapAidl);
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(radioSapAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
}

struct ThreadArg {
    unsigned int i;
    int          socketId;
};

extern "C" {
    void* threadFunc(void* arg) {
        ThreadArg* threadArg = static_cast<ThreadArg*>(arg);
        addRcpVsockRadioSapAidlService(threadArg->i, threadArg->socketId);

        delete threadArg;  // Don't forget to free the memory allocated for args
        return nullptr;
    }
}
#endif

void publicRadioSapAidlService() {
    mtkLogD(LOG_TAG, "publicRadioSapAidlService");

    const std::string instances[] = {
        std::string() + android::hardware::radio::sap::RadioSap::descriptor + "/slot1",
        std::string() + android::hardware::radio::sap::RadioSap::descriptor + "/slot2",
        std::string() + android::hardware::radio::sap::RadioSap::descriptor + "/slot3",
        std::string() + android::hardware::radio::sap::RadioSap::descriptor + "/slot4"
    };

    RIL_SOCKET_ID socketIds[] = {
        RIL_SOCKET_1,
        RIL_SOCKET_2,
        RIL_SOCKET_3,
        RIL_SOCKET_4
    };

    int simCount = 1;
    simCount = getSimCount();

    for (unsigned int i = 0; i < simCount; i++) {
        /*std::shared_ptr<android::hardware::radio::sap::RadioSap> radioSapAidl =
                ndk::SharedRefBase::make<android::hardware::radio::sap::RadioSap>(i, socketIds[i]);
        gPublishedHals.push_back(radioSapAidl);
        binder_status_t status =
                AServiceManager_addService(radioSapAidl->asBinder().get(), instances[i].c_str());
        mtkLogD(LOG_TAG, "aosp sap addservice, slotId:%d, status:%d", i, status);*/
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
        // add service for android client
        ThreadArg* arg = new ThreadArg{i, static_cast<int>(socketIds[i])};
        int result = pthread_create(&gThreadIds[i], nullptr, threadFunc, arg);

        if (result != 0) {
            mtkLogE(LOG_TAG, "pthread createSap failed");
        }
#endif
    }
}

void sap::registerService(const RIL_RadioFunctions * callbacks) {
    using namespace android::hardware;

#ifdef MTK_USE_AIDL
    /// configureRpcThreadpool(1, false);
#endif

    publicRadioSapAidlService();
#ifdef MTK_USE_AIDL
    /// joinRpcThreadpool();
#endif
}

void sap::processResponse(MsgHeader * rsp,RilSapSocket * sapSocket) {
    processResponse(rsp, sapSocket, MsgType_RESPONSE);
}

void sap::processUnsolResponse(MsgHeader * rsp,RilSapSocket * sapSocket) {
    processResponse(rsp, sapSocket, MsgType_UNSOL_RESPONSE);
}
