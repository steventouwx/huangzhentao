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

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxRawData.h"

#include "RfxChannelManager.h"
#include "RfxMclDispatcherThread.h"
#include "RfxMclMessage.h"
#include "RfxMessageId.h"
#include "rfx_properties.h"
#include "RmcRcsReqHandler.h"
#include "RcsStackChannel.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RFX_LOG_TAG "RmcRcsReq"

using namespace vendor::rmc::rcs;

/*****************************************************************************
 * Class RmcRcsReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcRcsReqHandler, RIL_CMD_PROXY_IMS);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxRawData, RFX_MSG_EVENT_RCS_SEND_MSG);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RCS_START_SERVICE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RCS_STOP_SERVICE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RCS_REGISTRATION_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_RCS_ENABLE_DEREG_IND);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RCS_RESUME_DEREGISTRATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_RCS_TRIGGER_RESTORATION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxVoidData, RFX_MSG_REQUEST_RCS_SEND_SIP_MESSAGE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_RCS_SWITCH_ROI_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_RCS_UPDATE_CAPABILITIES);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_RCS_UPDATE_SESSION_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxStringData, RFX_MSG_REQUEST_RCS_GET_USER_AGENT);

RFX_REGISTER_DATA_TO_URC_ID(RfxRawData, RFX_MSG_UNSOL_RCS_REG_STATE_CHANGED);
RFX_REGISTER_DATA_TO_URC_ID(RfxRawData, RFX_MSG_UNSOL_RCS_SIP_RECEIVED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_UNSOL_RCS_IMS_DEREG_START);

bool RmcRcsReqHandler::m_debug = true;
bool RmcRcsReqHandler::m_wait_cnf = true;

RmcRcsReqHandler::RmcRcsReqHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {

    logI(RFX_LOG_TAG, "RmcRcsReqHandler create (slot_id = %d)", slot_id);

    if (!RmcRcsReqHandler::isRcsUaSupport()) {
        return;
    }

    m_state_channel = 0;
    m_sip_channel = 0;

    //data to modem
    const int CommonEventList[] = {
        RFX_MSG_EVENT_RCS_SEND_MSG,
    };

    registerToHandleEvent(CommonEventList, sizeof(CommonEventList) / sizeof(int));

    const int requestList[] = {
        RFX_MSG_REQUEST_RCS_START_SERVICE,
        RFX_MSG_REQUEST_RCS_STOP_SERVICE,
        RFX_MSG_REQUEST_RCS_REGISTRATION_INFO,
        RFX_MSG_REQUEST_RCS_ENABLE_DEREG_IND,
        RFX_MSG_REQUEST_RCS_RESUME_DEREGISTRATION,
        RFX_MSG_REQUEST_RCS_TRIGGER_RESTORATION,
        RFX_MSG_REQUEST_RCS_SEND_SIP_MESSAGE,
        RFX_MSG_REQUEST_RCS_SWITCH_ROI_STATUS,
        RFX_MSG_REQUEST_RCS_UPDATE_CAPABILITIES,
        RFX_MSG_REQUEST_RCS_UPDATE_SESSION_INFO,
        RFX_MSG_REQUEST_RCS_GET_USER_AGENT
    };
    registerToHandleRequest(requestList, sizeof(requestList)/sizeof(int));

    //data from modem
    const int imsRcsEventList[] = {
        RFX_MSG_EVENT_IMS_DATA,
    };
    //RIL_IMS_Client_RCS
    registerToHandleEvent(RIL_IMS_Client_RCS, imsRcsEventList, sizeof(imsRcsEventList) / sizeof(int));

    checkAndInitImsFd();
    // Notify RIL FWK that we are ready to recv message.
    // Sometimes remote side may be ready earler than us and send message firstly
    // so we need to tell FWK when we can recv message or the earlier message will lost.
    sendUserData(RIL_IMS_Client_RCS, RIL_IMS_CLIENT_READY, NULL, 0);
}

RmcRcsReqHandler::~RmcRcsReqHandler() {
    if (m_state_channel)
        delete m_state_channel;
    if (m_sip_channel)
        delete m_sip_channel;
}

void RmcRcsReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int requestId = msg->getId();
    logI(RFX_LOG_TAG, "onHandleRequest with request[%s]", RFX_ID_TO_STR(requestId));

    switch (requestId) {
        case RFX_MSG_REQUEST_RCS_START_SERVICE:
            startRcsService(msg);
            break;

        case RFX_MSG_REQUEST_RCS_STOP_SERVICE:
            stopRcsService(msg);
            break;

        case RFX_MSG_REQUEST_RCS_REGISTRATION_INFO:
            requestRegistrationInfo(msg);
            break;

        case RFX_MSG_REQUEST_RCS_ENABLE_DEREG_IND:
            enableDeregInd(msg);
            break;

        case RFX_MSG_REQUEST_RCS_RESUME_DEREGISTRATION:
            resumeDeregistration(msg);
            break;

        case RFX_MSG_REQUEST_RCS_TRIGGER_RESTORATION:
            triggerRestoration(msg);
            break;

        case RFX_MSG_REQUEST_RCS_SEND_SIP_MESSAGE:
            sendSipMessage(msg);
            break;

        case RFX_MSG_REQUEST_RCS_SWITCH_ROI_STATUS:
            switchRcsRoiStatus(msg);
            break;

        case RFX_MSG_REQUEST_RCS_UPDATE_CAPABILITIES:
            updateRcsCapabilities(msg);
            break;

        case RFX_MSG_REQUEST_RCS_UPDATE_SESSION_INFO:
            updateRcsSessionInfo(msg);
            break;

        case RFX_MSG_REQUEST_RCS_GET_USER_AGENT:
            getUserAgent(msg);
            break;

        default:
            logI(RFX_LOG_TAG, "unknown request, ignore!");
            break;
    }
}

void RmcRcsReqHandler::onHandleEvent(const sp<RfxMclMessage> & msg) {
    logI(RFX_LOG_TAG, "onHandleEvent with event[%s]", RFX_ID_TO_STR(msg->getId()));

    switch (msg->getId()) {
        case RFX_MSG_EVENT_RCS_SEND_MSG:
            handleDataSending(msg);
            break;
        case RFX_MSG_EVENT_IMS_DATA:
            handleDataReceiving(msg);
            break;
        default:
            logW(RFX_LOG_TAG, "onHandleEvent with unkown event");
            break;
    }
}

void RmcRcsReqHandler::handleDataSending(const sp<RfxMclMessage>& msg) {
    uint8_t *data = (uint8_t *)msg->getData()->getData();
    int length = msg->getData()->getDataLength();

    if (m_debug) {
        RcsStackChannel::ChannelData *chnlData = (RcsStackChannel::ChannelData *)data;
        logI(RFX_LOG_TAG, "Send data to stack -> req[%s(%d)], account[%d], type[%d], datalen[%d]",
                RcsStackChannel::reqToString(chnlData->reqId), chnlData->reqId, chnlData->account, chnlData->type, chnlData->dataLen);
    }

    // send to RIL FWK
    sendUserData(RIL_IMS_Client_RCS, data, length);
}

void RmcRcsReqHandler::handleDataReceiving(const sp<RfxMclMessage>& msg) {
    uint8_t *data = (uint8_t *)msg->getData()->getData();
    int length = msg->getData()->getDataLength();

    if (!data || length <= 0) {
        logW(RFX_LOG_TAG, "[%d]Receive invalid data from stack");
        return;
    }
    RcsStackChannel::ChannelData *chnlData = (RcsStackChannel::ChannelData *)data;
    if (m_debug) {
        logI(RFX_LOG_TAG, "Receive data from stack with req[%s(%d)], account[%d], type[%d], datalen[%d]",
                RcsStackChannel::reqToString(chnlData->reqId), chnlData->reqId, chnlData->account, chnlData->type, chnlData->dataLen);
    }
    if (length != (sizeof(*chnlData) + chnlData->dataLen)) {
        logW(RFX_LOG_TAG, "[%d]Receive invalid data length from stack");
        return;
    }

    switch (chnlData->reqId) {
        case RcsStackChannel::IND_REG_STATE:
            handleRegistrationInfo(chnlData);
            break;

        case RcsStackChannel::RSP_IND_SIPMSG:
            handleIncomingSipMessage(chnlData);
            break;

        case RcsStackChannel::IND_REG_DEREG_START:
            handleImsDeregInd(chnlData);
            break;

        case RcsStackChannel::RSP_SEND_SIP:
            handleOutgoingSipResponse(chnlData);
            break;

        case RcsStackChannel::RSP_CREATE:
        case RcsStackChannel::RSP_DESTROY:
        case RcsStackChannel::IND_DISCONNECT:
            if (m_debug) {
                RcsStackChannel::RequestResult *result = (RcsStackChannel::RequestResult *)chnlData->data;
                logI(RFX_LOG_TAG, "Receive response[%s(%d)] from stack with result[%d]",
                        RcsStackChannel::reqToString(chnlData->reqId), chnlData->reqId, result->code);
            }
            break;
        default:
            break;
    }

    return;
}

bool RmcRcsReqHandler::isImsSubmarineSupport() {
    char feature[] = "IMS SUBMARINE";
    int support = getFeatureVersion(feature);

    // IMS SUBMARINE, 0:not support, 1:support
    logI(RFX_LOG_TAG, "isImsSubmarineSupport, %s = %d", feature, support);

    return (support == 1);
}

bool RmcRcsReqHandler::isRcsUaSupport() {
    //when mtk_rcs_ua_support set yes, ua deamon will be exist always,
    //so here use mtk_rcs_ua_support feature option to decide server running or not
    //property will be changed for cxp, so not use property here
    char property_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
    int isRcsUaProxySupport = 0;
    rfx_property_get("persist.vendor.mtk_rcs_ua_support", property_value, "0");
    isRcsUaProxySupport = atoi(property_value);
    logI(RFX_LOG_TAG, "isRcsUaProxySupport: %d", isRcsUaProxySupport);

    if (isRcsUaProxySupport == 1) {
       return true;
    }
    return false;
}

void RmcRcsReqHandler::startRcsService(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_SUCCESS;

    if (m_state_channel) {
        logW(RFX_LOG_TAG, "startRcsService state channel exist");
    } else {
        m_state_channel = new RcsStackChannel(m_slot_id, RcsStackChannel::ChannelType::STATE_CHANNEL);
        int length = m_state_channel->create(0, 0, 0);
        if (length > 0) {
            uint8_t *data = m_state_channel->getRawData();

            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, length),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);

            m_state_channel->releaseRawData();

            length = m_state_channel->bind();
            if (length > 0) {
                data = m_state_channel->getRawData();

                sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                        RFX_MSG_EVENT_RCS_SEND_MSG,
                        RfxRawData(data, length),
                        RIL_CMD_PROXY_2, m_slot_id);
                RfxMclDispatcherThread::enqueueMclMessage(event);

                m_state_channel->releaseRawData();

            } else {
                logW(RFX_LOG_TAG, "startRcsService bind fail");
                error = RIL_E_GENERIC_FAILURE;
            }
        } else {
            logW(RFX_LOG_TAG, "startRcsService create channel fail");
            error = RIL_E_GENERIC_FAILURE;
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), error, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::stopRcsService(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_SUCCESS;

    if (!m_state_channel) {
        logW(RFX_LOG_TAG, "stopRcsService no channel exist");
    } else {
        int length;
        if (m_sip_channel) {
            length = m_sip_channel->destroy();
            if (length > 0) {
                uint8_t *data = m_sip_channel->getRawData();
                sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                        RFX_MSG_EVENT_RCS_SEND_MSG,
                        RfxRawData(data, length),
                        RIL_CMD_PROXY_2, m_slot_id);
                RfxMclDispatcherThread::enqueueMclMessage(event);
                m_sip_channel->releaseRawData();
            }
            delete m_sip_channel;
            m_sip_channel = 0;
        }
        length = m_state_channel->unbind();
        if (length > 0) {
            uint8_t *data = m_state_channel->getRawData();
            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, length),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);
            m_state_channel->releaseRawData();
        }
        length = m_state_channel->destroy();
        if (length > 0) {
            uint8_t *data = m_state_channel->getRawData();
            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, length),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);
            m_state_channel->releaseRawData();
        }
        delete m_state_channel;
        m_state_channel = 0;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), error, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::requestRegistrationInfo(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_GENERIC_FAILURE;

    if (!m_state_channel) {
        logW(RFX_LOG_TAG, "requestRegistrationInfo no channel exist");
    } else {
        int length = m_state_channel->getRegState();
        if (length > 0) {
            uint8_t *data = m_state_channel->getRawData();
            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, length),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);
            m_state_channel->releaseRawData();
            error = RIL_E_SUCCESS;
        } else {
            logW(RFX_LOG_TAG, "requestRegistrationInfo compose data fail");
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), error, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::enableDeregInd(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_GENERIC_FAILURE;

    if (!m_state_channel) {
        logW(RFX_LOG_TAG, "enableDeregInd no channel exist");
    } else {
        int *data = (int *)msg->getData()->getData();
        if (!data) {
            logW(RFX_LOG_TAG, "enableDeregInd invalid data");
        } else {
            bool enable = (data[0] == 1) ? true : false;
            int length = m_state_channel->setDeregIndEnabled(enable);
            if (length > 0) {
                uint8_t *data = m_state_channel->getRawData();
                sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                        RFX_MSG_EVENT_RCS_SEND_MSG,
                        RfxRawData(data, length),
                        RIL_CMD_PROXY_2, m_slot_id);
                RfxMclDispatcherThread::enqueueMclMessage(event);
                m_state_channel->releaseRawData();
                error = RIL_E_SUCCESS;
            } else {
                logW(RFX_LOG_TAG, "enableDeregInd compose data fail");
            }
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), error, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::resumeDeregistration(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_GENERIC_FAILURE;

    if (!m_state_channel) {
        logW(RFX_LOG_TAG, "resumDeregistration no channel exist");
    } else {
        int length = m_state_channel->resumeDeregistration();
        if (length > 0) {
            uint8_t *data = m_state_channel->getRawData();
            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, length),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);
            m_state_channel->releaseRawData();
            error = RIL_E_SUCCESS;
        } else {
            logW(RFX_LOG_TAG, "resumDeregistration compose data fail");
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), error, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::triggerRestoration(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_GENERIC_FAILURE;

    if (!m_state_channel) {
        logW(RFX_LOG_TAG, "triggerRestoration no channel exist");
    } else {
        int length = m_state_channel->restore();
        if (length > 0) {
            uint8_t *data = m_state_channel->getRawData();
            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, length),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);
            m_state_channel->releaseRawData();
            error = RIL_E_SUCCESS;
        } else {
            logW(RFX_LOG_TAG, "triggerRestoration compose data fail");
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
            msg->getId(), error, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::sendSipMessage(const sp<RfxMclMessage>& msg) {
    RIL_Errno error = RIL_E_GENERIC_FAILURE;

    if (!m_sip_channel) {
        logW(RFX_LOG_TAG, "sendSipMessage no channel exist");
    } else {
        uint8_t *data = (uint8_t *)msg->getData()->getData();
        int total = msg->getData()->getDataLength();

        RIL_RcsSipMessage *sip = (RIL_RcsSipMessage *)data;
        if (total != sizeof(*sip) + sip->dataLength) {
            logW(RFX_LOG_TAG, "sendSipMessage invalid data length[%d]", total);

        } else {
            int length = m_sip_channel->sendSipMessage(
                sip->reqId, sip->connId, sip->data, sip->dataLength);
            if (length > 0) {
                if (m_debug) {
                    char * message = getDebugSipData(sip->data, sip->dataLength, 512);
                    if (message) {
                        logD(RFX_LOG_TAG, "handleOutgoingSipMessage with sipData[%s]", message);
                        free(message);
                    }
                }

                uint8_t *data = m_sip_channel->getRawData();
                sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                        RFX_MSG_EVENT_RCS_SEND_MSG,
                        RfxRawData(data, length),
                        RIL_CMD_PROXY_2, m_slot_id);
                RfxMclDispatcherThread::enqueueMclMessage(event);
                m_sip_channel->releaseRawData();
                if (m_wait_cnf) {
                    m_messages.push_back(msg);
                }
                error = RIL_E_SUCCESS;
            } else {
                logW(RFX_LOG_TAG, "sendSipMessage compose data fail");
            }
        }
    }

    if (!m_wait_cnf || error != RIL_E_SUCCESS) {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(
                msg->getId(), error, RfxVoidData(), msg);
        responseToTelCore(response);
    }
}

void RmcRcsReqHandler::switchRcsRoiStatus(const sp<RfxMclMessage>& msg) {
    int *pData = (int *)msg->getData()->getData();
    int status = pData ? pData[0] : -1;
    logI(RFX_LOG_TAG, "[%s] status:%d", __FUNCTION__, status);
    RIL_Errno ret = RIL_E_SUCCESS;
    switch (status) {
        case 0:
        case 1:
        {
            sp<RfxAtResponse> p_response =
                atSendCommand(String8::format("AT+EIMSRCS=%d", status + 3));
            if (p_response == NULL || p_response->isAtResponseFail()) {
                logE(RFX_LOG_TAG, "[%s] error!", __FUNCTION__);
                ret = RIL_E_GENERIC_FAILURE;
            }
            break;
        }

        default:
            ret = RIL_E_INVALID_ARGUMENTS;
            logE(RFX_LOG_TAG, "[%s] error! unsupport status", __FUNCTION__);
            break;
    }

    sp<RfxMclMessage> response
        = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::updateRcsCapabilities(const sp<RfxMclMessage>& msg) {
    char** pData = (char**)msg->getData()->getData();
    RIL_Errno ret = RIL_E_SUCCESS;
    if (pData) {
        char* mode = pData[0];
        char* featureTags = pData[1];
        logD(RFX_LOG_TAG, "[%s] mode:%s, featureTags:%s", __FUNCTION__, mode, featureTags);
        sp<RfxAtResponse> p_response =
            atSendCommand(String8::format("AT+EIMSRCS=%s,%s", mode, featureTags));
        if (p_response == NULL || p_response->isAtResponseFail()) {
            ret = RIL_E_GENERIC_FAILURE;
            logE(RFX_LOG_TAG, "[%s] error!", __FUNCTION__);
        } else {
            long numMode = strtol(mode, 0, 10);
            if (numMode == 0 || numMode == 1) {
                char *propMode;
                char *propFeatures;
                char valueMode[MTK_PROPERTY_VALUE_MAX] = { 0 };
                char valueFeatures[MTK_PROPERTY_VALUE_MAX] = { 0 };

                if (m_slot_id == 0) {
                    propMode = (char *)"persist.vendor.service.rcs";
                    propFeatures = (char *)"persist.vendor.service.tag.rcs";
                } else {
                    propMode = (char *)"persist.vendor.service.rcs.slot2";
                    propFeatures = (char *)"persist.vendor.service.tag.rcs.slot2";
                }
                rfx_property_get(propMode, valueMode, "");
                rfx_property_get(propFeatures, valueFeatures, "");

                if (strtol(valueMode, 0, 10) != numMode) {
                    logI(RFX_LOG_TAG, "updateRcsCapabilities mode changed");
                    rfx_property_set(propMode, mode);
                }
                if (strtol(valueFeatures, 0, 10) != strtol(featureTags, 0, 10)) {
                    logI(RFX_LOG_TAG, "updateRcsCapabilities features changed");
                    rfx_property_set(propFeatures, featureTags);
                }
            }
        }
    } else {
        ret = RIL_E_INVALID_ARGUMENTS;
        logE(RFX_LOG_TAG, "[%s] error! req data NULL", __FUNCTION__);
    }

    sp<RfxMclMessage> response
        = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::updateRcsSessionInfo(const sp<RfxMclMessage>& msg) {
    int *pData = (int *)msg->getData()->getData();
    int count = pData ? pData[0] : -1;
    logD(RFX_LOG_TAG, "[%s] count:%d", __FUNCTION__, count);
    RIL_Errno ret = RIL_E_SUCCESS;
    switch (count) {
        case 0:
        case 1:
        case 254:
        {
            sp<RfxAtResponse> p_response =
                atSendCommand(String8::format("AT+EIMSRCSCONN=%d", count));
            if (p_response == NULL || p_response->isAtResponseFail()) {
                logE(RFX_LOG_TAG, "[%s] error!", __FUNCTION__);
                ret = RIL_E_GENERIC_FAILURE;
            }
            break;
        }

        default:
            ret = RIL_E_INVALID_ARGUMENTS;
            logE(RFX_LOG_TAG, "[%s] error! unsupport count", __FUNCTION__);
            break;
    }

    sp<RfxMclMessage> response
        = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcRcsReqHandler::getUserAgent(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "[%s]", __FUNCTION__);
    RfxAtLine* line = NULL;
    char* key = NULL;
    char* value = NULL;
    int err = 0;
    sp<RfxMclMessage> response = NULL;
    sp<RfxAtResponse> p_response =
        atSendCommandSingleline(String8::format("AT+ECFGGET=\"user_agent\""), "+ECFGGET");
    if (p_response == NULL || p_response->isAtResponseFail()) {
        logE(RFX_LOG_TAG, "[%s] error!", __FUNCTION__);
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;

    key= line->atTokNextstr(&err);
    if (err < 0) goto error;
    if (strncmp("user_agent", key, 10) != 0) goto error;

    value = line->atTokNextstr(&err);
    if (err < 0 || value == NULL) goto error;

    logV(RFX_LOG_TAG, "[%s] value = %s", __FUNCTION__, value);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxStringData(value, strlen(value)), msg, false);
    responseToTelCore(response);
    return;

error:
    logE(RFX_LOG_TAG, "[%s] error!", __FUNCTION__);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
        RfxStringData(), msg, false);
    responseToTelCore(response);
}

void RmcRcsReqHandler::handleOutgoingSipResponse(const RcsStackChannel::ChannelData *chnlData) {
    if (chnlData->dataLen != sizeof(RcsStackChannel::RequestResult)) {
        logE(RFX_LOG_TAG, "handleOutgoingSipResponse invalid data format");
        return;
    }

    RcsStackChannel::RequestResult *result = (RcsStackChannel::RequestResult *)chnlData->data;
    logI(RFX_LOG_TAG, "handleOutgoingSipResponse with result[%d]", result->code);

    if (m_wait_cnf && !m_messages.empty()) {
        sp<RfxMclMessage> msg = m_messages.front();
        if (!msg.get()) {
            logW(RFX_LOG_TAG, "handleOutgoingSipResponse pending message released");
        } else {
            sp<RfxMclMessage> response = 0;
            if (result->code == RcsStackChannel::Result_Success) {
                response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                        RfxVoidData(), msg);
            } else {
                response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                        RfxVoidData(), msg);
            }
            responseToTelCore(response);
        }
        m_messages.erase(m_messages.begin());
    }
}

void RmcRcsReqHandler::handleRegistrationInfo(const RcsStackChannel::ChannelData *chnlData) {
    RcsStackChannel::RegInfo *regInfo = 0;
    RcsStackChannel::RegInfov2 *regInfov2 = 0;

    if (chnlData->dataLen == sizeof(RcsStackChannel::RegInfo)) {
        regInfo = (RcsStackChannel::RegInfo *)chnlData->data;
    } else if (chnlData->dataLen == sizeof(RcsStackChannel::RegInfov2)) {
        regInfo = (RcsStackChannel::RegInfo *)chnlData->data;
        regInfov2 = (RcsStackChannel::RegInfov2 *)chnlData->data;
    } else {
        logE(RFX_LOG_TAG, "handleRegistrationInfo invalid data: sizeof(Regv1)[%d], sizeof(Regv2)[%d]",
                sizeof(RcsStackChannel::RegInfo), sizeof(RcsStackChannel::RegInfov2));
        return;
    }
    if (!m_state_channel) {
        logE(RFX_LOG_TAG, "handleRegistrationInfo lack STATE channel to handle");
        return;
    }

    if (regInfov2)
        logI(RFX_LOG_TAG, "handleRegistrationInfo id[%d], cause[%d], state[%d] with v2 format",
                regInfo->id, regInfo->cause, regInfo->state);
    else
        logI(RFX_LOG_TAG, "handleRegistrationInfo id[%d], cause[%d], state[%d] with v1 format",
                regInfo->id, regInfo->cause, regInfo->state);

    /*
     *   0: SIM1 IMS
     *   1: SIM1 Emergency
     *   2: SIM2 IMS
     *   3: SIM2 Emergency
     */
    if (regInfo->id == 1 || regInfo->id == 3) {
        return;
    }

    if (regInfo->cause == RcsStackChannel::RegCause::CAUSE_NOT_BIND) {
        int l = m_state_channel->bind();
        if (l > 0) {
            uint8_t *data = m_state_channel->getRawData();
            sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                    RFX_MSG_EVENT_RCS_SEND_MSG,
                    RfxRawData(data, l),
                    RIL_CMD_PROXY_2, m_slot_id);
            RfxMclDispatcherThread::enqueueMclMessage(event);
            m_state_channel->releaseRawData();
        }
    }

    if (regInfo->state == RcsStackChannel::RegState::STATE_REGISTERED) {
        if (!m_sip_channel) {
            m_sip_channel = new RcsStackChannel(m_slot_id, RcsStackChannel::ChannelType::DATA_CHANNEL);
            int l = m_sip_channel->create(regInfo->conn_info,
                    (char *)regInfo->public_uid,
                    (const char *)regInfo->associated_uri);
            if (l > 0) {
                uint8_t *data = m_sip_channel->getRawData();
                sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                        RFX_MSG_EVENT_RCS_SEND_MSG,
                        RfxRawData(data, l),
                        RIL_CMD_PROXY_2, m_slot_id);
                RfxMclDispatcherThread::enqueueMclMessage(event);
                m_sip_channel->releaseRawData();
            } else {
                logW(RFX_LOG_TAG, "handleRegistrationInfo create SIP channel fail");
                delete m_sip_channel;
                m_sip_channel = 0;
            }
        }
    } else if (regInfo->state == RcsStackChannel::RegState::STATE_DISCONNECTED) {
        if (m_sip_channel) {
            int l = m_sip_channel->destroy();
            if (l > 0) {
                uint8_t *data = m_sip_channel->getRawData();
                sp<RfxMclMessage> event = RfxMclMessage::obtainEvent(
                        RFX_MSG_EVENT_RCS_SEND_MSG,
                        RfxRawData(data, l),
                        RIL_CMD_PROXY_2, m_slot_id);
                RfxMclDispatcherThread::enqueueMclMessage(event);
                m_sip_channel->releaseRawData();
            }
            delete m_sip_channel;
            m_sip_channel = 0;
        }
    }

    RIL_RcsRegistrationInfo *ind = (RIL_RcsRegistrationInfo *)malloc(sizeof(RIL_RcsRegistrationInfo));
    memset(ind, 0, sizeof(*ind));

    if (!regInfov2) {
        ind->regState = regInfo->state;
        ind->rcsMode = regInfo->rcs_state;
        ind->rcsFeatures = regInfo->rcs_feature;
        strncpy((char *)ind->localAddress, (char *)regInfo->local_address, sizeof(ind->localAddress)-1);
        ind->localPort = regInfo->local_port;
        strncpy((char *)ind->remoteAddress, (char *)regInfo->pcscf_address, sizeof(ind->remoteAddress)-1);
        ind->remotePort = regInfo->pcscf_port;
        strncpy((char *)ind->homeDomain, (char *)regInfo->home_uri, sizeof(ind->homeDomain)-1);
        ind->transport = regInfo->protocol_type; // protocol type: TCP=1, UDP=2, TCPUDP=3
        ind->ipVersion = regInfo->protocol_version; // protocol version : 0 -> IPV4  ; 1 -> IPv6
        strncpy((char *)ind->IMPU, (char *)regInfo->public_uid, sizeof(ind->IMPU)-1);
        strncpy((char *)ind->IMPI, (char *)regInfo->private_uid, sizeof(ind->IMPI)-1);
        strncpy((char *)ind->userAgent, (char *)regInfo->user_agent, sizeof(regInfo->user_agent)-1);
        ind->ipSecLocalPort = regInfo->via_rport;
        ind->ipSecRemotePort = regInfo->target_port_s;
        strncpy((char *)ind->svHeader, (char *)regInfo->security_verify, sizeof(ind->svHeader)-1);
        strncpy((char *)ind->pAssociatedUri, (char *)regInfo->associated_uri, sizeof(ind->pAssociatedUri)-1);
        strncpy((char *)ind->instanceId, (char *)regInfo->instance_id, sizeof(ind->instanceId)-1);
        strncpy((char *)ind->serviceRoute, (char *)regInfo->service_route, sizeof(ind->serviceRoute)-1);
        strncpy((char *)ind->pAccessNetworkInfo, (char *)regInfo->pani, sizeof(ind->pAccessNetworkInfo)-1);
        strncpy((char *)ind->regTimestamp, (char *)regInfo->reg_timestamp, sizeof(ind->regTimestamp));
    } else {
        ind->regState = regInfov2->state;
        ind->rcsMode = regInfov2->rcs_state;
        ind->rcsFeatures = regInfov2->rcs_feature;
        strncpy((char *)ind->localAddress, (char *)regInfov2->local_address, sizeof(ind->localAddress)-1);
        ind->localPort = regInfov2->local_port;
        strncpy((char *)ind->remoteAddress, (char *)regInfov2->pcscf_address, sizeof(ind->remoteAddress)-1);
        ind->remotePort = regInfov2->pcscf_port;
        strncpy((char *)ind->homeDomain, (char *)regInfov2->home_uri, sizeof(ind->homeDomain)-1);
        ind->transport = regInfov2->protocol_type; // protocol type: TCP=1, UDP=2, TCPUDP=3
        ind->ipVersion = regInfov2->protocol_version; // protocol version : 0 -> IPV4  ; 1 -> IPv6
        strncpy((char *)ind->IMPU, (char *)regInfov2->public_uid, sizeof(ind->IMPU)-1);
        strncpy((char *)ind->IMPI, (char *)regInfov2->private_uid, sizeof(ind->IMPI)-1);
        strncpy((char *)ind->userAgent, (char *)regInfov2->user_agent, sizeof(regInfov2->user_agent)-1);
        ind->ipSecLocalPort = regInfov2->via_rport;
        ind->ipSecRemotePort = regInfov2->target_port_s;
        strncpy((char *)ind->svHeader, (char *)regInfov2->security_verify, sizeof(ind->svHeader)-1);
        strncpy((char *)ind->pAssociatedUri, (char *)regInfov2->associated_uri, sizeof(ind->pAssociatedUri)-1);
        strncpy((char *)ind->instanceId, (char *)regInfov2->instance_id, sizeof(ind->instanceId)-1);
        strncpy((char *)ind->serviceRoute, (char *)regInfov2->service_route, sizeof(ind->serviceRoute)-1);
        strncpy((char *)ind->pAccessNetworkInfo, (char *)regInfov2->pani, sizeof(ind->pAccessNetworkInfo)-1);
        strncpy((char *)ind->regTimestamp, (char *)regInfov2->reg_timestamp, sizeof(ind->regTimestamp));
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_RCS_REG_STATE_CHANGED,
            m_slot_id, RfxRawData((void *)ind, sizeof(*ind)));
    responseToTelCore(urc);

    free(ind);
}

void RmcRcsReqHandler::handleIncomingSipMessage(const RcsStackChannel::ChannelData *chnlData) {
    RcsStackChannel::SipInfo *sipInfo = 0;

    if (chnlData->dataLen < sizeof(*sipInfo)) {
        logE(RFX_LOG_TAG, "handleIncomingSipMessage invalid data format");
        return;
    }
    if (!m_sip_channel) {
        logE(RFX_LOG_TAG, "handleIncomingSipMessage lack SIP channel to handle");
        return;
    }

    sipInfo = (RcsStackChannel::SipInfo *)chnlData->data;
    if (chnlData->dataLen != sizeof(*sipInfo) + sipInfo->length) {
        logE(RFX_LOG_TAG, "handleIncomingSipMessage invalid data length, sip length[%d]", sipInfo->length);
        return;
    }

    logI(RFX_LOG_TAG, "handleIncomingSipMessage with reqId[%d], connId[%d], msgSize[%d]",
            sipInfo->reqId, sipInfo->connId, sipInfo->length);

    if (m_debug) {
        char * message = getDebugSipData(sipInfo->data, sipInfo->length, 512);
        if (message) {
            logD(RFX_LOG_TAG, "handleIncomingSipMessage with sipData[%s]", message);
            free(message);
        }
    }

    int bufSize = sizeof(RIL_RcsSipMessage) + sipInfo->length;
    RIL_RcsSipMessage *ind = (RIL_RcsSipMessage *)malloc(bufSize);
    memset(ind, 0, bufSize);

    ind->reqId = sipInfo->reqId;
    ind->connId = sipInfo->connId;
    ind->dataLength = sipInfo->length;
    memcpy(ind->data, sipInfo->data, sipInfo->length);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_RCS_SIP_RECEIVED,
            m_slot_id, RfxRawData((void *)ind, bufSize));
    responseToTelCore(urc);

    free(ind);
}

void RmcRcsReqHandler::handleImsDeregInd(const RcsStackChannel::ChannelData *chnlData) {
    RcsStackChannel::DeregStartNotify *notify = 0;

    if (chnlData->dataLen != sizeof(RcsStackChannel::DeregStartNotify)) {
        logE(RFX_LOG_TAG, "handleImsDeregInd invalid data format");
        return;
    }
    if (!m_state_channel) {
        logE(RFX_LOG_TAG, "handleImsDeregInd lack STATE channel to handle");
        return;
    }

    notify = (RcsStackChannel::DeregStartNotify *)chnlData->data;
    logI(RFX_LOG_TAG, "handleImsDeregInd with notifyId[%d]", notify->id);

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
            RFX_MSG_UNSOL_RCS_IMS_DEREG_START, m_slot_id, RfxVoidData());
    responseToTelCore(urc);
}

void RmcRcsReqHandler::checkAndInitImsFd() {
    if (m_slot_id) {
        logI(RFX_LOG_TAG, "[%s][%d] not slot 0, skip", __FUNCTION__, m_slot_id);
        return;
    }

    char dev[RIL_SUPPORT_CHANNELS_MAX_NAME_LEN] = {0};
    strncpy(dev, RIL_TTY_CHANNEL_IMS, RIL_SUPPORT_CHANNELS_MAX_NAME_LEN - 1);
    int count = 0;
    RfxChannel *channel = RfxChannelManager::getChannel(m_channel_id);
    while (channel->getSender()->getFd() < 0) {
        int fd = open(dev, O_RDWR);
        if (fd < 0) {
            logE(RFX_LOG_TAG, "could not connect to %s: %s, count:%d", dev, strerror(errno), count);
            if (count == 10) {
                fd = -1;
                logE(RFX_LOG_TAG, "giveup connectting to %s", dev);
                break;
            }
            count++;
            usleep(100 * 1000);
            /* never returns */
        } else {
            logI(RFX_LOG_TAG, "open dev successfully %s", dev);
            struct termios ios;
            tcgetattr(fd, &ios);
            ios.c_lflag = 0; /* disable ECHO, ICANON, etc... */
            ios.c_iflag = 0;
            tcsetattr(fd, TCSANOW, &ios);
            RfxChannelManager::initImsFd(m_channel_id, fd);
        }
    }
}

char *RmcRcsReqHandler::getDebugSipData(const uint8_t *rawData, int dataLen, int maxSize) {
    const int MIN_DIGIT_COUNT = 7;
    const int MAX_DIGIT_COUNT = 16;
    const int RESERVED_HEAD = 1;
    const int RESERVED_TAIL = 2;
    const char MASK_CHAR = '*';

    char buff[MAX_DIGIT_COUNT];

    int size = (dataLen < maxSize ? dataLen : maxSize);
    char *out = (char *)malloc(size+1);

    char* src = (char *)rawData;
    char *dst = out;
    int count = 0;
    bool nonDigitMode = false;

    for (int i = 0; i < size; i++) {
        if (src[i] >= '0' && src[i] <= '9' && !nonDigitMode) {
            if (count < MAX_DIGIT_COUNT) {
                buff[count++] = src[i];
            } else {
                // exceed maxium digit count, consider as non-digit for following digits
                nonDigitMode = true;
                memcpy(dst, buff, count);
                dst += count;
                count = 0;
                *dst++ = src[i];
            }
        } else {
            if (count > 0) {
                if (count >= MIN_DIGIT_COUNT) {
                    int offset = 0;
                    memcpy(dst, buff, RESERVED_HEAD);
                    dst += RESERVED_HEAD;
                    offset += RESERVED_HEAD;
                    while (offset < count-RESERVED_TAIL) {
                        *dst++ = MASK_CHAR;
                        offset++;
                    }
                    memcpy(dst, &buff[offset], RESERVED_TAIL);
                    dst += RESERVED_TAIL;
                } else {
                    memcpy(dst, buff, count);
                    dst += count;
                }
                count = 0;
            }
            *dst++ = src[i];
            if (nonDigitMode && !(src[i] >= '0' && src[i] <= '9')) {
                nonDigitMode = false;
            }
        }
    }

    if (count > 0) {
        if (count >= MIN_DIGIT_COUNT) {
            int offset = 0;
            memcpy(dst, buff, RESERVED_HEAD);
            dst += RESERVED_HEAD;
            offset += RESERVED_HEAD;
            while (offset < count-RESERVED_TAIL) {
                *dst++ = MASK_CHAR;
                offset++;
            }
            memcpy(dst, &buff[offset], RESERVED_TAIL);
            dst += RESERVED_TAIL;
        } else {
            memcpy(dst, buff, count);
            dst += count;
        }
        count = 0;
    }
    *dst = '\0';

    return out;
}


