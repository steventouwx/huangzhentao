/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2023. All rights reserved.
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

#include "RmmDchRequestHandler.h"
#include "rfx_properties.h"
#include "RfxDchCloseSipData.h"
#include "RfxDchConfigList.h"
#include "RfxDchConfigLocalList.h"
#include "RfxDchDataPathData.h"
#include "RfxDchImsConfigData.h"
#include "RfxDchXmlConfigData.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringsData.h"

#include <arpa/inet.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <sys/socket.h>

static const int requests[] = {
    RFX_MSG_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP,
    RFX_MSG_REQUEST_DCH_CREATE_SIP_REQ,
    RFX_MSG_REQUEST_DCH_MODIFY_SIP_REQ,
    RFX_MSG_REQUEST_DCH_CLOSE_SIP_REQE,
    RFX_MSG_REQUEST_DCH_CREATE_CMD_RSP,
    RFX_MSG_REQUEST_DCH_ENABLE_REQ,
};
static const int dchEventList[] = {
    RFX_MSG_EVENT_DCH_CLEAR_CMD,
    RFX_MSG_EVENT_DCH_SYNC_ROUTE_PARAMS,
};

// MTK solution start @{
static const int Mtkrequests[] = {
    RFX_MSG_REQUEST_DCH_ACT_PACKET_ROUTE_REQ,
    RFX_MSG_REQUEST_DCH_IFUP_PACKET_ROUTE_REQ,
    RFX_MSG_REQUEST_DCH_DATA_PATH_CONFIG_SYNC_REQ,
    RFX_MSG_REQUEST_DCH_CREATE_DATA_CHANNEL_REQ,
    RFX_MSG_REQUEST_DCH_CLOSE_DATA_CHANNEL_REQ,
};
// MTK solution end }@

static const int mipcCmd[] = {
    MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD,
    MIPC_CALL_DC_CREATE_CMD,
};

#define SIOCSQID        (SIOCDEVPRIVATE + 13)
#define SIOCSNETTYPE    (SIOCDEVPRIVATE + 15)

// Vendor solution
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD);
RFX_REGISTER_DATA_TO_URC_ID(RfxDchImsConfigData, RFX_MSG_UNSOL_DCH_CREATE_CMD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchConfigLocalList, RfxVoidData, RFX_MSG_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchConfigList, RfxIntsData, RFX_MSG_REQUEST_DCH_CREATE_SIP_REQ);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchConfigList, RfxIntsData, RFX_MSG_REQUEST_DCH_MODIFY_SIP_REQ);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchCloseSipData, RfxIntsData, RFX_MSG_REQUEST_DCH_CLOSE_SIP_REQE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchConfigList, RfxVoidData, RFX_MSG_REQUEST_DCH_CREATE_CMD_RSP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_DCH_ENABLE_REQ);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DCH_CLEAR_CMD);
// MTK solution
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_DCH_ACT_PACKET_ROUTE_REQ);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_DCH_IFUP_PACKET_ROUTE_REQ);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxDchDataPathData, RFX_MSG_REQUEST_DCH_DATA_PATH_CONFIG_SYNC_REQ);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchXmlConfigData, RfxDchXmlConfigData, RFX_MSG_REQUEST_DCH_CREATE_DATA_CHANNEL_REQ);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxDchXmlConfigData, RfxDchXmlConfigData, RFX_MSG_REQUEST_DCH_CLOSE_DATA_CHANNEL_REQ);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_DCH_SYNC_ROUTE_PARAMS);

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmDchRequestHandler, RIL_CMD_PROXY_1);

RmmDchRequestHandler::RmmDchRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {

    char dcSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.mtk_dc_call_capability", dcSupport, "0");

    logD(RFX_LOG_TAG, "vodc RmmDchRequestHandler constructor, dcSupport:%s", dcSupport);
    int value = atoi(dcSupport);
    // if (value == 1 || value == 3) {
        registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
        registerToHandleMipcCmd(mipcCmd, sizeof(mipcCmd) / sizeof(const int));
        registerToHandleEvent(dchEventList, sizeof(dchEventList)/sizeof(int));
    // }

    // MTK solution start @{
    // if (value == 2 || value == 3) {
        registerToHandleRequest(Mtkrequests, sizeof(Mtkrequests)/sizeof(int));
    // }
    // MTK solution end }@

    if (value == 1 || value == 2 || value == 3) {
        dchSetDcEnableReq();
    }
}

RmmDchRequestHandler::~RmmDchRequestHandler() {
}

void RmmDchRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int rildMsgId = msg->getId();

    logD(RFX_LOG_TAG, "vodc onHandleRequest, rildMsgId:%d", rildMsgId);

    switch(rildMsgId) {
        case RFX_MSG_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP:
            dchBootstrapCfgQueryCmdRsp(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CREATE_SIP_REQ:
            dchCreateSipReq(msg);
            break;
        case RFX_MSG_REQUEST_DCH_MODIFY_SIP_REQ:
            dchModifySipReq(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CLOSE_SIP_REQE:
            dchCloseSipReq(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CREATE_CMD_RSP:
            dchCreateCmdRsp(msg);
            break;
        case RFX_MSG_REQUEST_DCH_ENABLE_REQ:
            handleDchEnableReq(msg);
            break;
        case RFX_MSG_REQUEST_DCH_ACT_PACKET_ROUTE_REQ:
            dchDataActPacketRouteRequest(msg);
            break;
        case RFX_MSG_REQUEST_DCH_IFUP_PACKET_ROUTE_REQ:
            dchDataIfupPacketRouteRequest(msg);
            break;
        case RFX_MSG_REQUEST_DCH_DATA_PATH_CONFIG_SYNC_REQ:
            dchDataPathConfigSyncRequest(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CREATE_DATA_CHANNEL_REQ:
            dchCreateDataChannelRequest(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CLOSE_DATA_CHANNEL_REQ:
            dchCloseDataChannelRequest(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "vodc onHandleRequest, Should not be here");
            break;
    }
}

void RmmDchRequestHandler::onHandleResponse(const sp<RfxMclMessage>& msg) {
    int rildMsgId = msg->getId();

    logD(RFX_LOG_TAG, "vodc onHandleResponse, rildMsgId:%d", rildMsgId);

    switch(rildMsgId) {
        case RFX_MSG_REQUEST_DCH_CREATE_SIP_REQ:
            dchCreateSipResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_MODIFY_SIP_REQ:
            dchModifySipResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CLOSE_SIP_REQE:
            dchCloseSipResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_ENABLE_REQ:
            handleDchEnableResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_ACT_PACKET_ROUTE_REQ:
            dchDataActPacketRouteResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_IFUP_PACKET_ROUTE_REQ:
            dchDataIfupPacketRouteResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_DATA_PATH_CONFIG_SYNC_REQ:
            dchDataPathConfigSyncResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CREATE_DATA_CHANNEL_REQ:
            dchCreateDataChannelResponse(msg);
            break;
        case RFX_MSG_REQUEST_DCH_CLOSE_DATA_CHANNEL_REQ:
            dchCloseDataChannelResponse(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "vodc onHandleResponse, Should not be here");
            break;
    }
}

void RmmDchRequestHandler::onHandleCmd(const sp<RfxMclMessage>& msg) {
    uint16_t mipcMsgId = msg->getMipcData()->getMipcMsgId();

    logD(RFX_LOG_TAG, "vodc onHandleCmd, mipcMsgId:%d", mipcMsgId);

     switch (mipcMsgId) {
        case MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD:
            handleDchBootstrapCfgQueryCmd(msg);
            break;
        case MIPC_CALL_DC_CREATE_CMD:
            handleDchCreateCmd(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "vodc onHandleCmd, Should not be here");
            break;
    }
}

void RmmDchRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_DCH_CLEAR_CMD:
            handleEventClearCmd(msg);
            break;
        case RFX_MSG_EVENT_DCH_SYNC_ROUTE_PARAMS:
            handleEventSyncRouteParams(msg);
            break;
        default:
            logD(RFX_LOG_TAG, "vodc onHandleEvent, Not support the event %d!", event);
            break;
    }
}

void RmmDchRequestHandler::dchSetDcEnableReq() {
    sp<RfxMipcData> req_data =
            RfxMipcData::obtainMipcData(MIPC_IMS_ENABLE_DATA_CHANNEL_REQ, m_slot_id);

    //shall use property value??
    req_data->addMipcTlvUint8(MIPC_IMS_ENABLE_DATA_CHANNEL_REQ_T_ENABLE, 1);

     sp<RfxMipcData> cnfMipc = callToMipcMsgSync(req_data, 500);
     logD(RFX_LOG_TAG, "vodc dchSetDcEnableReq, enable vodc, result:%d",
                cnfMipc->getResult());
}

void RmmDchRequestHandler::handleDchEnableReq(const sp<RfxMclMessage>& msg) {
    char *pData = (char*)(msg->getData()->getData());
    int enable = 0;
    if (pData != NULL && strlen(pData) > 0) {
        enable = atoi(pData);
    }
    if (enable != 1) {
        enable = 0;
    }
    sp<RfxMipcData> req_data =
        RfxMipcData::obtainMipcData(MIPC_IMS_ENABLE_DATA_CHANNEL_REQ, m_slot_id);
    logD(RFX_LOG_TAG, "vodc handleDchEnableReq, enable:%s", pData);
    req_data->addMipcTlvUint8(MIPC_IMS_ENABLE_DATA_CHANNEL_REQ_T_ENABLE, (uint8_t)enable);
    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::handleDchEnableResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    logD(RFX_LOG_TAG, "vodc handleDchEnableResponse, result_code:%d", result_code);

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::handleDchBootstrapCfgQueryCmd(const sp<RfxMclMessage>& msg) {
    int response[2] = { 0 };
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchBootstrapCfgQueryCmd, error case, urc_data is NULL");
        return;
    }

    uint8_t callId = urc_data->getMipcUint8Val(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD_T_CALL_ID, 0);
    uint16_t opCode = urc_data->getMipcUint16Val(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD_T_OP_ID, 0);
    mBootstrapCfgQueryCallTxIds.add(callId, urc_data->getMipcMsgTxId());

    // get operator code
    response[0] = (int)callId;
    response[1] = (int)opCode;

    logI(RFX_LOG_TAG, "vodc handleDchBootstrapCfgQueryCmd, opCode:%d, callId:%d, msgTxId:%x",
            opCode, callId, urc_data->getMipcMsgTxId());

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD,
            m_slot_id, RfxIntsData(response, 2));
    responseToTelCore(urc);
}

void RmmDchRequestHandler::handleEventClearCmd(const sp<RfxMclMessage>& msg) {
    int *data = (int *)(msg->getData()->getData());
    if (data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleEventClearCreateCmd, data is NULL");
        return;
    }

    uint8_t callId = (uint8_t)data[0];
    if (!mBootstrapCfgQueryCallTxIds.isEmpty()) {
        if (mBootstrapCfgQueryCallTxIds.indexOfKey(callId) >= 0) {
            int existId = mBootstrapCfgQueryCallTxIds.valueFor(callId);
            logD(RFX_LOG_TAG, "vodc handleEventClearCmd, Bootstrap txid:%x", existId);
            sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP,
                                            m_slot_id, existId);
            mBootstrapCfgQueryCallTxIds.removeItem(callId);

            req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_CALL_ID, callId);
            req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_RESULT, 1); // 0:success, 1:failed
            req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DC_CONFIG_LOCAL, 0);
            req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DCMAP, 0);
            callToMipcMsgCmdResponse(req_data);
        }
    }

    if (!mCreateCmdCallTxIds.isEmpty()) {
        if (mCreateCmdCallTxIds.indexOfKey(callId) >= 0) {
            int existId = mCreateCmdCallTxIds.valueFor(callId);
            logD(RFX_LOG_TAG, "vodc handleEventClearCreateCmd, createcmd txid:%x", existId);
            sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_CREATE_RSP,
                                            m_slot_id, existId);
            mCreateCmdCallTxIds.removeItem(callId);

            req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_CALL_ID, callId);
            req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_RESULT, 1); // 0:success, 1:failed
            req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_NUM_DC_CONFIG, 0);
            req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_NUM_DCMAP, 0);

            callToMipcMsgCmdResponse(req_data);
        }
    }
}

void RmmDchRequestHandler::dchBootstrapCfgQueryCmdRsp(const sp<RfxMclMessage>& msg) {
    if (msg->getData() == NULL || msg->getData()->getData() == NULL) {
        logE(RFX_LOG_TAG, "vodc dchBootstrapCfgQueryCmdRsp, error case, data is NULL");

        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(mclResponse);

        return;
    }

    RIL_DchConfigLocal *configs =
                (RIL_DchConfigLocal *)msg->getData()->getData();
    int count = msg->getData()->getDataLength() / sizeof(RIL_DchConfigLocal);

    if (count <= 0 || mBootstrapCfgQueryCallTxIds.isEmpty()) {
        logE(RFX_LOG_TAG, "vodc dchBootstrapCfgQueryCmdRsp, error case, data is invalid");
        sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(resp);
        return;
    }

    logD(RFX_LOG_TAG, "vodc dchBootstrapCfgQueryCmdRsp, count:%d", count);

    if (count > MAX_BDC_CONFIG_NUM) {
        count = MAX_BDC_CONFIG_NUM;
    }

    char noStr[2] = { 0 };
    RIL_DchConfigLocal *ConfigsPtrs[MAX_BDC_CONFIG_NUM];
    for (int i = 0; i < count; i++) {
        ConfigsPtrs[i] = &(configs[i]);
    }
    if (ConfigsPtrs[0]->result == 1 && ConfigsPtrs[0]->cause == MAX_DC_RSP_CAUSE_CANCLED) {
        count = 0;
    }
    if (mBootstrapCfgQueryCallTxIds.indexOfKey(ConfigsPtrs[0]->call_id) < 0) {
        logE(RFX_LOG_TAG, "vodc dchBootstrapCfgQueryCmdRsp, error case, can't find callId");
        sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(resp);
        return;
    }

    uint16_t local_udp_port[MAX_BDC_CONFIG_NUM];
    uint16_t local_sctp_port[MAX_BDC_CONFIG_NUM];
    uint8_t  config_index[MAX_BDC_CONFIG_NUM];
    uint8_t  local_dtls_setup[MAX_BDC_CONFIG_NUM];
    char*    local_tls_id[MAX_BDC_CONFIG_NUM];
    uint16_t fp_hash_func[MAX_BDC_CONFIG_NUM];
    char*    local_fp_str[MAX_BDC_CONFIG_NUM];
    uint32_t local_max_message_size[MAX_BDC_CONFIG_NUM];
    uint32_t data_b_as[MAX_BDC_CONFIG_NUM];
    char*    qos_hint[MAX_BDC_CONFIG_NUM];
    //RIL_DcMapStruct dcmap;
    uint16_t dcmap_stream_id[MAX_BDC_DCMAP_NUM];
    uint16_t dcmap_priority[MAX_BDC_DCMAP_NUM];
    uint8_t  dcmap_config_index[MAX_BDC_DCMAP_NUM];
    uint8_t  dcmap_ordered[MAX_BDC_DCMAP_NUM];
    uint32_t dcmap_max_retr[MAX_BDC_DCMAP_NUM];
    uint32_t dcmap_max_time[MAX_BDC_DCMAP_NUM];
    char*    dcmap_label[MAX_BDC_DCMAP_NUM];
    char*    dcmap_subprotocol[MAX_BDC_DCMAP_NUM];

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP, m_slot_id,
                                    mBootstrapCfgQueryCallTxIds.valueFor(ConfigsPtrs[0]->call_id));
    logD(RFX_LOG_TAG, "vodc dchBootstrapCfgQueryCmdRsp, msgTxId:%x",
                mBootstrapCfgQueryCallTxIds.valueFor(ConfigsPtrs[0]->call_id));
    mBootstrapCfgQueryCallTxIds.removeItem(ConfigsPtrs[0]->call_id);

    req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_CALL_ID, ConfigsPtrs[0]->call_id);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_RESULT, ConfigsPtrs[0]->result);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DC_CONFIG_LOCAL, (uint8_t)count);

    int dcmap_count = 0;
    for (int i = 0; i < count; i++) {
        local_udp_port[i] = ConfigsPtrs[i]->local_udp_port;
        local_sctp_port[i] = ConfigsPtrs[i]->local_sctp_port;
        config_index[i] = ConfigsPtrs[i]->index;
        logD(RFX_LOG_TAG, "vodc dchBootstrapCfgQueryCmdRsp, index[%d]:%d", i, ConfigsPtrs[i]->index);

        local_dtls_setup[i] = ConfigsPtrs[i]->local_dtls_setup;
        if ((char*)ConfigsPtrs[i]->local_tls_id != NULL) {
            local_tls_id[i] = (char*)ConfigsPtrs[i]->local_tls_id;
        } else {
            local_tls_id[i] = noStr;
        }
        fp_hash_func[i] = ConfigsPtrs[i]->fp_hash_func;
        if ((char*)ConfigsPtrs[i]->local_fp_str != NULL) {
            local_fp_str[i] = (char*)ConfigsPtrs[i]->local_fp_str;
        } else {
            local_fp_str[i] = noStr;
        }
        local_max_message_size[i] = ConfigsPtrs[i]->local_max_message_size;
        data_b_as[i] = ConfigsPtrs[i]->data_b_as;
        if ((char*)ConfigsPtrs[i]->qos_hint != NULL) {
            qos_hint[i] = (char*)ConfigsPtrs[i]->qos_hint;
        } else {
            qos_hint[i] = noStr;
        }

        if (ConfigsPtrs[i]->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
            logE(RFX_LOG_TAG,
                "vodc dchBootstrapCfgQueryCmdRsp, wrong dcmap_count:%d", ConfigsPtrs[i]->dcmap_count);
            ConfigsPtrs[i]->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }
        for (int j = 0; j < ConfigsPtrs[i]->dcmap_count; j++) {
            dcmap_stream_id[dcmap_count] = (uint16_t)ConfigsPtrs[i]->dcmap_stream_id[j];
            dcmap_priority[dcmap_count] = (uint16_t)ConfigsPtrs[i]->dcmap_priority[j];
            dcmap_config_index[dcmap_count] = (uint8_t) config_index[i];
            dcmap_ordered[dcmap_count] = (uint8_t)ConfigsPtrs[i]->dcmap_ordered[j];
            dcmap_max_retr[dcmap_count] = (uint32_t)ConfigsPtrs[i]->dcmap_max_retr[j];
            dcmap_max_time[dcmap_count] = (uint32_t)ConfigsPtrs[i]->dcmap_max_time[j];
            if ((char*)ConfigsPtrs[i]->dcmap_label[j] != NULL) {
                dcmap_label[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_label[j];
            } else {
                dcmap_label[dcmap_count] = noStr;
            }
            if ((char*)ConfigsPtrs[i]->dcmap_subprotocol[j] != NULL) {
                dcmap_subprotocol[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_subprotocol[j];
            } else {
                dcmap_subprotocol[dcmap_count] = noStr;
            }
            dcmap_count++;
        }
    }
    if (count > 0) {
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_UDP_PORT, count, local_udp_port);
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_SCTP_PORT, count, local_sctp_port);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_INDEX, count, config_index);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_DTLS_SETUP, count, local_dtls_setup);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_TLS_ID, count, local_tls_id);
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_FP_HASH_FUNC, count, fp_hash_func);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_FP_STR, count, local_fp_str);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_LOCAL_MAX_MESSAGE_SIZE, count, local_max_message_size);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DATA_B_AS, count, data_b_as);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_QOS_HINT, count, qos_hint);
    }
    req_data->addMipcTlvUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_NUM_DCMAP, (uint8_t)dcmap_count);
    if (dcmap_count > 0) {
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_STREAM_ID, dcmap_count, dcmap_stream_id);
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_PRIORITY, dcmap_count, dcmap_priority);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DC_CONFIG_INDEX, dcmap_count, dcmap_config_index);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_ORDERED, dcmap_count, dcmap_ordered);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_MAX_RETR, dcmap_count, dcmap_max_retr);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_MAX_TIME, dcmap_count, dcmap_max_time);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_LABEL, dcmap_count, dcmap_label);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP_T_DCMAP_SUBPROTOCOL, dcmap_count, dcmap_subprotocol);
    }

    callToMipcMsgCmdResponse(req_data);

    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(mclResponse);
}

void RmmDchRequestHandler::dchCreateSipReq(const sp<RfxMclMessage>& msg) {
    if (msg->getData() == NULL || msg->getData()->getData() == NULL) {
        logE(RFX_LOG_TAG, "vodc dchCreateSipReq, error case, data is NULL");

        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(mclResponse);

        return;
    }

    //RIL_DcMapStruct dcmap;
    RIL_DchConfig *configs =
                (RIL_DchConfig *)msg->getData()->getData();
    int count = msg->getData()->getDataLength() / sizeof(RIL_DchConfig);

    if (count <= 0) {
         logE(RFX_LOG_TAG, "vodc dchCreateSipReq, error case, data is invalid");
         sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                 RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
         responseToTelCore(resp);
         return;
     }

    logD(RFX_LOG_TAG, "vodc dchCreateSipReq, count:%d", count);

    if (count > MAX_DC_CONFIG_NUM) {
        count = MAX_DC_CONFIG_NUM;
    }

    char noStr[2] = { 0 };
    RIL_DchConfig *ConfigsPtrs[MAX_DC_CONFIG_NUM];
    for (int i = 0; i < count; i++) {
        ConfigsPtrs[i] = &(configs[i]);
    }

    uint8_t  config_index[MAX_DC_CONFIG_NUM];
    uint8_t  dc_type[MAX_DC_CONFIG_NUM];
    uint8_t  ip_addr_type[MAX_DC_CONFIG_NUM];
    uint16_t remote_udp_port[MAX_DC_CONFIG_NUM];
    uint16_t remote_sctp_port[MAX_DC_CONFIG_NUM];
    uint16_t local_udp_port[MAX_DC_CONFIG_NUM];
    uint16_t local_sctp_port[MAX_DC_CONFIG_NUM];
    uint8_t* remote_addr[MAX_DC_CONFIG_NUM];
    uint8_t* local_addr[MAX_DC_CONFIG_NUM];
    uint8_t  remote_dtls_setup[MAX_DC_CONFIG_NUM];
    uint8_t  local_dtls_setup[MAX_DC_CONFIG_NUM];
    char*    remote_tls_id[MAX_DC_CONFIG_NUM];
    char*    local_tls_id[MAX_DC_CONFIG_NUM];
    uint16_t fp_hash_func[MAX_DC_CONFIG_NUM];
    char*    remote_fp_str[MAX_DC_CONFIG_NUM];
    char*    local_fp_str[MAX_DC_CONFIG_NUM];
    uint32_t remote_max_message_size[MAX_DC_CONFIG_NUM];
    uint32_t local_max_message_size[MAX_DC_CONFIG_NUM];
    uint32_t data_b_as[MAX_DC_CONFIG_NUM];
    char*    qos_hint[MAX_DC_CONFIG_NUM];
    //RIL_DcMapStruct dcmap;
    uint16_t dcmap_stream_id[MAX_DC_DCMAP_NUM];
    uint16_t dcmap_priority[MAX_DC_DCMAP_NUM];
    uint8_t  dcmap_config_index[MAX_DC_DCMAP_NUM];
    uint8_t  dcmap_ordered[MAX_DC_DCMAP_NUM];
    uint32_t dcmap_max_retr[MAX_DC_DCMAP_NUM];
    uint32_t dcmap_max_time[MAX_DC_DCMAP_NUM];
    char*    dcmap_label[MAX_DC_DCMAP_NUM];
    char*    dcmap_subprotocol[MAX_DC_DCMAP_NUM];
    int dcmap_count = 0;

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_CREATE_SIP_REQ, m_slot_id);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_CALL_ID, (uint8_t) ConfigsPtrs[0]->call_id);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_NUM_DC_CONFIG, (uint8_t)count);

    for (int i = 0; i < count; i++) {
        config_index[i] = ConfigsPtrs[i]->index;
        dc_type[i] = ConfigsPtrs[i]->dc_type;
        logD(RFX_LOG_TAG,
            "vodc dchCreateSipReq, index[%d]:%d, dcType:%d", i, ConfigsPtrs[i]->index, dc_type[i]);
        ip_addr_type[i] = ConfigsPtrs[i]->ip_addr_type;
        remote_udp_port[i] = ConfigsPtrs[i]->remote_udp_port;
        remote_sctp_port[i] = ConfigsPtrs[i]->remote_sctp_port;
        local_udp_port[i] = ConfigsPtrs[i]->local_udp_port;
        local_sctp_port[i] = ConfigsPtrs[i]->local_sctp_port;
        remote_addr[i] = (uint8_t*)ConfigsPtrs[i]->remote_addr;
        local_addr[i] = (uint8_t*)ConfigsPtrs[i]->local_addr;

        remote_dtls_setup[i] = ConfigsPtrs[i]->remote_dtls_setup;
        local_dtls_setup[i] = ConfigsPtrs[i]->local_dtls_setup;
        if ((char*)ConfigsPtrs[i]->remote_tls_id != NULL) {
            remote_tls_id[i] = (char*)ConfigsPtrs[i]->remote_tls_id;
        } else {
            remote_tls_id[i] = noStr;
        }
        if ((char*)ConfigsPtrs[i]->local_tls_id != NULL) {
            local_tls_id[i] = (char*)ConfigsPtrs[i]->local_tls_id;
        } else {
            local_tls_id[i] = noStr;
        }
        fp_hash_func[i] = ConfigsPtrs[i]->fp_hash_func;
        if ((char*)ConfigsPtrs[i]->remote_fp_str != NULL) {
            remote_fp_str[i] = (char*)ConfigsPtrs[i]->remote_fp_str;
        } else {
            remote_fp_str[i] = noStr;
        }
        if ((char*)ConfigsPtrs[i]->local_fp_str != NULL) {
            local_fp_str[i] = (char*)ConfigsPtrs[i]->local_fp_str;
        } else {
            local_fp_str[i] = noStr;
        }
        remote_max_message_size[i] = ConfigsPtrs[i]->remote_max_message_size;
        local_max_message_size[i] = ConfigsPtrs[i]->local_max_message_size;
        data_b_as[i] = ConfigsPtrs[i]->data_b_as;
        if ((char*)ConfigsPtrs[i]->qos_hint != NULL) {
            qos_hint[i] = (char*)ConfigsPtrs[i]->qos_hint;
        } else {
            qos_hint[i] = noStr;
        }
        if (ConfigsPtrs[i]->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
            logE(RFX_LOG_TAG,
                "vodc dchCreateSipReq, wrong dcmap_count:%d", ConfigsPtrs[i]->dcmap_count);
            ConfigsPtrs[i]->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }
        for (int j = 0; j < ConfigsPtrs[i]->dcmap_count; j++) {
            dcmap_stream_id[dcmap_count] = ConfigsPtrs[i]->dcmap_stream_id[j];
            dcmap_priority[dcmap_count] = ConfigsPtrs[i]->dcmap_priority[j];
            dcmap_config_index[dcmap_count] = config_index[i];
            dcmap_ordered[dcmap_count] = ConfigsPtrs[i]->dcmap_ordered[j];
            dcmap_max_retr[dcmap_count] = ConfigsPtrs[i]->dcmap_max_retr[j];
            dcmap_max_time[dcmap_count] = ConfigsPtrs[i]->dcmap_max_time[j];
            if ((char*)ConfigsPtrs[i]->dcmap_label[j] != NULL) {
                dcmap_label[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_label[j];
            } else {
                dcmap_label[dcmap_count] = noStr;
            }
            if ((char*)ConfigsPtrs[i]->dcmap_subprotocol[j] != NULL) {
                dcmap_subprotocol[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_subprotocol[j];
            } else {
                dcmap_subprotocol[dcmap_count] = noStr;
            }
            dcmap_count++;
        }
    }
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_INDEX, count, config_index);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_DC_TYPE, count, dc_type);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_IP_ADDR_TYPE, count, ip_addr_type);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_UDP_PORT, count, remote_udp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_SCTP_PORT, count, remote_sctp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_UDP_PORT, count, local_udp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_SCTP_PORT, count, local_sctp_port);
    req_data->addMipcTlvArrayUint8Array(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_ADDR, count, remote_addr, MAX_DC_IP_ADDR_LENGTH);
    req_data->addMipcTlvArrayUint8Array(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_ADDR, count, local_addr, MAX_DC_IP_ADDR_LENGTH);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_DTLS_SETUP, count, remote_dtls_setup);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_DTLS_SETUP, count, local_dtls_setup);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_TLS_ID, count, remote_tls_id);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_TLS_ID, count, local_tls_id);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_FP_HASH_FUNC, count, fp_hash_func);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_FP_STR, count, remote_fp_str);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_FP_STR, count, local_fp_str);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_SIP_REQ_T_REMOTE_MAX_MESSAGE_SIZE, count, remote_max_message_size);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_SIP_REQ_T_LOCAL_MAX_MESSAGE_SIZE, count, local_max_message_size);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_SIP_REQ_T_DATA_B_AS, count, data_b_as);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_QOS_HINT, count, qos_hint);

    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_NUM_DCMAP, dcmap_count);
    if (dcmap_count > 0) {
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_STREAM_ID, dcmap_count, dcmap_stream_id);
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_PRIORITY, dcmap_count, dcmap_priority);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_DC_CONFIG_INDEX, dcmap_count, dcmap_config_index);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_ORDERED, dcmap_count, dcmap_ordered);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_MAX_RETR, dcmap_count, dcmap_max_retr);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_MAX_TIME, dcmap_count, dcmap_max_time);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_LABEL, dcmap_count, dcmap_label);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_SIP_REQ_T_DCMAP_SUBPROTOCOL, dcmap_count, dcmap_subprotocol);
    }

    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchCreateSipResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    int response[3] = { 0 };

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    logD(RFX_LOG_TAG, "vodc dchCreateSipResponse, result_code:%d", result_code);

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    uint8_t callId = mipcData->getMipcUint8Val(MIPC_CALL_DC_CREATE_SIP_CNF_T_CALL_ID, 0);
    uint8_t cause = mipcData->getMipcUint16Val(MIPC_CALL_DC_CREATE_SIP_CNF_T_CAUSE, 0);
    response[0] = (int)callId;
    response[1] = (int)result_code;
    response[2] = (int)cause;

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxIntsData(response, 3), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::dchModifySipReq(const sp<RfxMclMessage>& msg) {
    if (msg->getData() == NULL || msg->getData()->getData() == NULL) {
        logE(RFX_LOG_TAG, "vodc dchModifySipReq, error case, data is NULL");

        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(mclResponse);

        return;
    }

    RIL_DchConfig *configs =
                (RIL_DchConfig *)msg->getData()->getData();
    int count = msg->getData()->getDataLength() / sizeof(RIL_DchConfig);

    if (count <= 0) {
         logE(RFX_LOG_TAG, "vodc dchModifySipReq, error case, data is invalid");
         sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                 RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
         responseToTelCore(resp);
         return;
     }

    logD(RFX_LOG_TAG, "vodc dchModifySipReq, count:%d", count);

    if (count > MAX_DC_CONFIG_NUM) {
        count = MAX_DC_CONFIG_NUM;
    }

    char noStr[2] = { 0 };
    RIL_DchConfig *ConfigsPtrs[MAX_DC_CONFIG_NUM];
    for (int i = 0; i < count; i++) {
        ConfigsPtrs[i] =  &(configs[i]);
    }

    uint8_t  config_index[MAX_DC_CONFIG_NUM];
    uint8_t  dc_type[MAX_DC_CONFIG_NUM];
    uint8_t  ip_addr_type[MAX_DC_CONFIG_NUM];
    uint16_t remote_udp_port[MAX_DC_CONFIG_NUM];
    uint16_t remote_sctp_port[MAX_DC_CONFIG_NUM];
    uint16_t local_udp_port[MAX_DC_CONFIG_NUM];
    uint16_t local_sctp_port[MAX_DC_CONFIG_NUM];
    uint8_t* remote_addr[MAX_DC_CONFIG_NUM];
    uint8_t* local_addr[MAX_DC_CONFIG_NUM];
    uint8_t  remote_dtls_setup[MAX_DC_CONFIG_NUM];
    uint8_t  local_dtls_setup[MAX_DC_CONFIG_NUM];
    char*    remote_tls_id[MAX_DC_CONFIG_NUM];
    char*    local_tls_id[MAX_DC_CONFIG_NUM];
    uint16_t fp_hash_func[MAX_DC_CONFIG_NUM];
    char*    remote_fp_str[MAX_DC_CONFIG_NUM];
    char*    local_fp_str[MAX_DC_CONFIG_NUM];
    uint32_t remote_max_message_size[MAX_DC_CONFIG_NUM];
    uint32_t local_max_message_size[MAX_DC_CONFIG_NUM];
    uint32_t data_b_as[MAX_DC_CONFIG_NUM];
    char*    qos_hint[MAX_DC_CONFIG_NUM];
    //RIL_DcMapStruct dcmap;
    uint16_t dcmap_stream_id[MAX_DC_DCMAP_NUM];
    uint16_t dcmap_priority[MAX_DC_DCMAP_NUM];
    uint8_t  dcmap_config_index[MAX_DC_DCMAP_NUM];
    uint8_t  dcmap_ordered[MAX_DC_DCMAP_NUM];
    uint32_t dcmap_max_retr[MAX_DC_DCMAP_NUM];
    uint32_t dcmap_max_time[MAX_DC_DCMAP_NUM];
    char*    dcmap_label[MAX_DC_DCMAP_NUM];
    char*    dcmap_subprotocol[MAX_DC_DCMAP_NUM];
    int dcmap_count = 0;

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_MODIFY_SIP_REQ, m_slot_id);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_CALL_ID, (uint8_t) ConfigsPtrs[0]->call_id);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_NUM_DC_CONFIG, (uint8_t)count);

    for (int i = 0; i < count; i++) {
        config_index[i] = ConfigsPtrs[i]->index;
        dc_type[i] = ConfigsPtrs[i]->dc_type;
        logD(RFX_LOG_TAG,
            "vodc dchModifySipReq, index[%d]:%d, dcType:%d", i, ConfigsPtrs[i]->index, dc_type[i]);
        ip_addr_type[i] = ConfigsPtrs[i]->ip_addr_type;
        remote_udp_port[i] = ConfigsPtrs[i]->remote_udp_port;
        remote_sctp_port[i] = ConfigsPtrs[i]->remote_sctp_port;
        local_udp_port[i] = ConfigsPtrs[i]->local_udp_port;
        local_sctp_port[i] = ConfigsPtrs[i]->local_sctp_port;

        remote_addr[i] = (uint8_t*)ConfigsPtrs[i]->remote_addr;
        local_addr[i] = (uint8_t*)ConfigsPtrs[i]->local_addr;

        remote_dtls_setup[i] = ConfigsPtrs[i]->remote_dtls_setup;
        local_dtls_setup[i] = ConfigsPtrs[i]->local_dtls_setup;
        if ((char*)ConfigsPtrs[i]->remote_tls_id != NULL) {
            remote_tls_id[i] = (char*)ConfigsPtrs[i]->remote_tls_id;
        } else {
            remote_tls_id[i] = noStr;
        }
        if ((char*)ConfigsPtrs[i]->local_tls_id != NULL) {
            local_tls_id[i] = (char*)ConfigsPtrs[i]->local_tls_id;
        } else {
            local_tls_id[i] = noStr;
        }
        fp_hash_func[i] = ConfigsPtrs[i]->fp_hash_func;
        if ((char*)ConfigsPtrs[i]->remote_fp_str != NULL) {
            remote_fp_str[i] = (char*)ConfigsPtrs[i]->remote_fp_str;
        } else {
            remote_fp_str[i] = noStr;
        }
        if ((char*)ConfigsPtrs[i]->local_fp_str != NULL) {
            local_fp_str[i] = (char*)ConfigsPtrs[i]->local_fp_str;
        } else {
            local_fp_str[i] = noStr;
        }
        remote_max_message_size[i] = ConfigsPtrs[i]->remote_max_message_size;
        local_max_message_size[i] = ConfigsPtrs[i]->local_max_message_size;
        data_b_as[i] = ConfigsPtrs[i]->data_b_as;
        if ((char*)ConfigsPtrs[i]->qos_hint != NULL) {
            qos_hint[i] = (char*)ConfigsPtrs[i]->qos_hint;
        } else {
            qos_hint[i] = noStr;
        }

        if (ConfigsPtrs[i]->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
            logE(RFX_LOG_TAG,
                "vodc dchModifySipReq, wrong dcmap_count:%d", ConfigsPtrs[i]->dcmap_count);
            ConfigsPtrs[i]->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }
        for (int j = 0; j < ConfigsPtrs[i]->dcmap_count; j++) {
            dcmap_stream_id[dcmap_count] = ConfigsPtrs[i]->dcmap_stream_id[j];
            dcmap_priority[dcmap_count] = ConfigsPtrs[i]->dcmap_priority[j];
            dcmap_config_index[dcmap_count] = config_index[i];
            dcmap_ordered[dcmap_count] = ConfigsPtrs[i]->dcmap_ordered[j];
            dcmap_max_retr[dcmap_count] = ConfigsPtrs[i]->dcmap_max_retr[j];
            dcmap_max_time[dcmap_count] = ConfigsPtrs[i]->dcmap_max_time[j];
            if ((char*)ConfigsPtrs[i]->dcmap_label[j] != NULL) {
                dcmap_label[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_label[j];
            } else {
                dcmap_label[dcmap_count] = noStr;
            }
            if ((char*)ConfigsPtrs[i]->dcmap_subprotocol[j] != NULL) {
                dcmap_subprotocol[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_subprotocol[j];
            } else {
                dcmap_subprotocol[dcmap_count] = noStr;
            }
            dcmap_count++;
        }
    }
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_INDEX, count, config_index);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DC_TYPE, count, dc_type);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_IP_ADDR_TYPE, count, ip_addr_type);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_UDP_PORT, count, remote_udp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_SCTP_PORT, count, remote_sctp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_UDP_PORT, count, local_udp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_SCTP_PORT, count, local_sctp_port);
    req_data->addMipcTlvArrayUint8Array(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_ADDR, count, remote_addr, MAX_DC_IP_ADDR_LENGTH);
    req_data->addMipcTlvArrayUint8Array(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_ADDR, count, local_addr, MAX_DC_IP_ADDR_LENGTH);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_DTLS_SETUP, count, remote_dtls_setup);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_DTLS_SETUP, count, local_dtls_setup);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_TLS_ID, count, remote_tls_id);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_TLS_ID, count, local_tls_id);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_FP_HASH_FUNC, count, fp_hash_func);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_FP_STR, count, remote_fp_str);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_FP_STR, count, local_fp_str);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_MODIFY_SIP_REQ_T_REMOTE_MAX_MESSAGE_SIZE, count, remote_max_message_size);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_MODIFY_SIP_REQ_T_LOCAL_MAX_MESSAGE_SIZE, count, local_max_message_size);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DATA_B_AS, count, data_b_as);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_QOS_HINT, count, qos_hint);

    req_data->addMipcTlvUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_NUM_DCMAP, dcmap_count);
    if (dcmap_count > 0) {
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_STREAM_ID, dcmap_count, dcmap_stream_id);
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_PRIORITY, dcmap_count, dcmap_priority);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DC_CONFIG_INDEX, dcmap_count, dcmap_config_index);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_ORDERED, dcmap_count, dcmap_ordered);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_MAX_RETR, dcmap_count, dcmap_max_retr);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_MAX_TIME, dcmap_count, dcmap_max_time);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_LABEL, dcmap_count, dcmap_label);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_MODIFY_SIP_REQ_T_DCMAP_SUBPROTOCOL, dcmap_count, dcmap_subprotocol);
    }
    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchModifySipResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    int response[3] = { 0 };

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    logD(RFX_LOG_TAG, "vodc dchModifySipResponse, result_code:%d", result_code);

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    uint8_t callId = mipcData->getMipcUint8Val(MIPC_CALL_DC_MODIFY_SIP_CNF_T_CALL_ID, 0);
    uint8_t cause = mipcData->getMipcUint16Val(MIPC_CALL_DC_MODIFY_SIP_CNF_T_CAUSE, 0);
    response[0] = (int)callId;
    response[1] = (int)result_code;
    response[2] = (int)cause;

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxIntsData(response, 3), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::dchCloseSipReq(const sp<RfxMclMessage>& msg) {

    if (msg->getData() == NULL || msg->getData()->getData() == NULL) {
        logE(RFX_LOG_TAG, "vodc dchCloseSipReq, error case, data is NULL");

        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(mclResponse);

        return;
    }

    uint8_t dcmap_count;
    uint8_t dc_config_index[MAX_DCMAP_NUM_PER_CONFIG];
    uint16_t dcmap_stream_id[MAX_DCMAP_NUM_PER_CONFIG];

    RIL_DchMapDelStruct *params = (RIL_DchMapDelStruct *)msg->getData()->getData();

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_CLOSE_SIP_REQ, m_slot_id);

    req_data->addMipcTlvUint8(MIPC_CALL_DC_CLOSE_SIP_REQ_T_CALL_ID, params->callId);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_CLOSE_SIP_REQ_T_CLOSE_ALL_STREAM,  params->closeAllStream);
    dcmap_count = params->dcmap_count;

    logD(RFX_LOG_TAG, "vodc dchCloseSipReq, dcmap_count:%d", dcmap_count);

    if (params->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
        dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
    }
    req_data->addMipcTlvUint8(MIPC_CALL_DC_CLOSE_SIP_REQ_T_NUM_DCMAP_DEL,  dcmap_count);
    for (int i = 0; i < dcmap_count; i++) {
        dc_config_index[i] = params->dc_config_index[i];
        dcmap_stream_id[i] = params->dcmap_stream_id[i];
    }
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CLOSE_SIP_REQ_T_DC_CONFIG_INDEX, dcmap_count, dc_config_index);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CLOSE_SIP_REQ_T_DCMAP_STREAM_ID, dcmap_count, dcmap_stream_id);

    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchCloseSipResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    int response[3] = { 0 };

    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    logD(RFX_LOG_TAG, "vodc dchCloseSipResponse, result_code:%d", result_code);

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    uint8_t callId = mipcData->getMipcUint8Val(MIPC_CALL_DC_CLOSE_SIP_CNF_T_CALL_ID, 0);
    uint8_t cause = mipcData->getMipcUint16Val(MIPC_CALL_DC_CLOSE_SIP_CNF_T_CAUSE, 0);
    response[0] = (int)callId;
    response[1] = (int)result_code;
    response[2] = (int)cause;

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxIntsData(response, 3), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::handleDchCreateCmd(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error case, urc_data is NULL");
        return;
    }

    mipc_msg_t *msg_ptr = urc_data->getData();
    if (msg_ptr == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error case, msg_ptr is NULL");
        return;
    }

    uint16_t str_len = 0;
    RIL_DchImsDataConfig response;
    memset(&response, 0, sizeof(RIL_DchImsDataConfig));

    response.call_id = urc_data->getMipcUint8Val(MIPC_CALL_DC_CREATE_CMD_T_CALL_ID, 0);
    response.dc_config_num = urc_data->getMipcUint8Val(MIPC_CALL_DC_CREATE_CMD_T_NUM_DC_CONFIG, 0);
    if (response.dc_config_num > MAX_DC_CONFIG_NUM) {
        logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error dc_config_num: %d", response.dc_config_num);
        response.dc_config_num = MAX_DC_CONFIG_NUM;
    }

    uint8_t dcmap_config_index[MAX_BDC_DCMAP_NUM];
    uint8_t dcmap_count = urc_data->getMipcUint8Val(MIPC_CALL_DC_CREATE_CMD_T_NUM_DCMAP, 0);
    for (uint16_t i = 0; i < dcmap_count; i++) {
        dcmap_config_index[i] = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DC_CONFIG_INDEX, i, 0);
    }

    for (uint16_t i = 0; i < response.dc_config_num; i++) {
        RIL_DchConfig* dchConfig = &(response.dc_config_list[i]);
        dchConfig->index =  urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_INDEX, i, 0);
        dchConfig->dc_type = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DC_TYPE, i, 0);
        logD(RFX_LOG_TAG,
            "vodc handleDchCreateCmd, index[%d]:%d, dcType:%d", i, dchConfig->index, dchConfig->dc_type);
        dchConfig->ip_addr_type = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_IP_ADDR_TYPE, i, 0);
        dchConfig->remote_udp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_REMOTE_UDP_PORT, i, 0);
        dchConfig->remote_sctp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_REMOTE_SCTP_PORT, i, 0);
        dchConfig->local_udp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_LOCAL_UDP_PORT, i, 0);
        dchConfig->local_sctp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_LOCAL_SCTP_PORT, i, 0);

        uint8_t* remote_addr = (uint8_t *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_ADDR, i, &str_len);
        if (str_len > MIPC_VOLTE_IPADDR_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, remote_addr str_len:%d", str_len);
            str_len = MIPC_VOLTE_IPADDR_LENGTH;
        }
        if (str_len > 0 && remote_addr != NULL) {
            // memcpy(dchConfig->remote_addr, remote_addr, str_len);
            logD(RFX_LOG_TAG,
                "vodc handleDchCreateCmd, remote_addr copy:%d, str_len:%d", remote_addr[0], str_len);
            for (int x = 0; x < str_len; x++) {
                dchConfig->remote_addr[x] = remote_addr[x];
            }
        }

        uint8_t* local_addr = (uint8_t *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_ADDR, i, &str_len);
        if (str_len > MIPC_VOLTE_IPADDR_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, local_addr str_len:%d", str_len);
            str_len = MIPC_VOLTE_IPADDR_LENGTH;
        }
        if (str_len > 0 && local_addr != NULL) {
            // memcpy(dchConfig->local_addr, local_addr, str_len);
            logD(RFX_LOG_TAG,
                "vodc handleDchCreateCmd, local_addr copy:%d, str_len:%d", local_addr[0], str_len);
            for (int x = 0; x < str_len; x++) {
                dchConfig->local_addr[x] = local_addr[x];
            }
        }

        dchConfig->remote_dtls_setup = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_REMOTE_DTLS_SETUP, i, 0);
        dchConfig->local_dtls_setup = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_LOCAL_DTLS_SETUP, i, 0);

        dchConfig->remote_tls_id = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_TLS_ID, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_TLS_ID_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error remote_tls_id str_len:%d", str_len);
        }
        dchConfig->local_tls_id = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_TLS_ID, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_TLS_ID_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error local_tls_id str_len:%d", str_len);
        }
        dchConfig->fp_hash_func =  urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_FP_HASH_FUNC, i, 0);

        dchConfig->remote_fp_str = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_REMOTE_FP_STR, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_FINGERPRINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error remote_fp_str str_len:%d", str_len);
        }
        dchConfig->local_fp_str = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_LOCAL_FP_STR, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_FINGERPRINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error local_fp_str str_len:%d", str_len);
        }
        dchConfig->remote_max_message_size = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CREATE_CMD_T_REMOTE_MAX_MESSAGE_SIZE, i, 0);
        dchConfig->local_max_message_size = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CREATE_CMD_T_LOCAL_MAX_MESSAGE_SIZE, i, 0);
        dchConfig->data_b_as = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DATA_B_AS, i, 0);
        dchConfig->qos_hint = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_QOS_HINT, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_QOS_HINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error qos_hint str_len:%d", str_len);
        }

        //RIL_DcMapStruct dcmap;  //RFC 8864
        uint16_t j = 0;
        for (uint16_t k = 0; k < dcmap_count; k++) {
            if (dcmap_config_index[k] != dchConfig->index) {
                continue;
            }
            dchConfig->dcmap_stream_id[j] = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DCMAP_STREAM_ID, k, 0);
            dchConfig->dcmap_priority[j] = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DCMAP_PRIORITY, k, 0);
            dchConfig->dcmap_ordered[j] = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DCMAP_ORDERED, k, 0);
            dchConfig->dcmap_max_retr[j] =  urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_RETR, k, 0);
            dchConfig->dcmap_max_time[j] = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CREATE_CMD_T_DCMAP_MAX_TIME, k, 0);
            dchConfig->dcmap_label[j] = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_LABEL, k, &str_len);
            if (str_len > MIPC_VOLTE_MAX_DCMAP_LABEL_LENGTH) {
                logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error dcmap_label str_len:%d", str_len);
            }
            dchConfig->dcmap_subprotocol[j] = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CREATE_CMD_T_DCMAP_SUBPROTOCOL, k, &str_len);
            if (str_len > MIPC_VOLTE_MAX_DCMAP_SUBPROTOCOL_LENGTH) {
                logE(RFX_LOG_TAG, "vodc handleDchCreateCmd, error dcmap_subprotocol str_len:%d", str_len);
            }
            j++;
        }
        dchConfig->dcmap_count = j;
        logI(RFX_LOG_TAG, "vodc handleDchCreateCmd, callId:%d, i:%d, index:%d, dcmap_count:%d", response.call_id, i, dchConfig->index, j);
    }

    mCreateCmdCallTxIds.add((uint8_t)response.call_id, urc_data->getMipcMsgTxId());

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CREATE_CMD,
            m_slot_id, RfxDchImsConfigData(&response, sizeof(RIL_DchImsDataConfig)));
    responseToTelCore(urc);
}

void RmmDchRequestHandler::dchCreateCmdRsp(const sp<RfxMclMessage>& msg) {
    RIL_DchConfig *configs =
                (RIL_DchConfig *)msg->getData()->getData();
    RIL_DchConfig *ConfigsPtrs[MAX_DC_CONFIG_NUM];
    int count = msg->getData()->getDataLength() / sizeof(RIL_DchConfig);
    logD(RFX_LOG_TAG, "vodc dchCreateCmdRsp, count:%d", count);

    if (count <= 0 || mCreateCmdCallTxIds.isEmpty()) {
        logE(RFX_LOG_TAG, "vodc dchCreateCmdRsp, error case, data is invalid");
        sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(resp);
        return;
    }

    if (count > MAX_DC_CONFIG_NUM) {
        count = MAX_DC_CONFIG_NUM;
    }

    char noStr[2] = { 0 };
    for (int i = 0; i < count; i++) {
        ConfigsPtrs[i] = &(configs[i]);
    }

    if (mCreateCmdCallTxIds.indexOfKey(ConfigsPtrs[0]->call_id) < 0) {
        logE(RFX_LOG_TAG, "vodc dchCreateCmdRsp, error case, can't find callId");
        sp<RfxMclMessage> resp = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(resp);
        return;
    }

    uint8_t  config_index[MAX_DC_CONFIG_NUM];
    uint8_t  dc_type[MAX_DC_CONFIG_NUM];
    uint8_t  ip_addr_type[MAX_DC_CONFIG_NUM];
    uint16_t remote_udp_port[MAX_DC_CONFIG_NUM];
    uint16_t remote_sctp_port[MAX_DC_CONFIG_NUM];
    uint16_t local_udp_port[MAX_DC_CONFIG_NUM];
    uint16_t local_sctp_port[MAX_DC_CONFIG_NUM];
    uint8_t* remote_addr[MAX_DC_CONFIG_NUM];
    uint8_t* local_addr[MAX_DC_CONFIG_NUM];
    uint8_t  remote_dtls_setup[MAX_DC_CONFIG_NUM];
    uint8_t  local_dtls_setup[MAX_DC_CONFIG_NUM];
    char*    remote_tls_id[MAX_DC_CONFIG_NUM];
    char*    local_tls_id[MAX_DC_CONFIG_NUM];
    uint16_t fp_hash_func[MAX_DC_CONFIG_NUM];
    char*    remote_fp_str[MAX_DC_CONFIG_NUM];
    char*    local_fp_str[MAX_DC_CONFIG_NUM];
    uint32_t remote_max_message_size[MAX_DC_CONFIG_NUM];
    uint32_t local_max_message_size[MAX_DC_CONFIG_NUM];
    uint32_t data_b_as[MAX_DC_CONFIG_NUM];
    char*    qos_hint[MAX_DC_CONFIG_NUM];
    //RIL_DcMapStruct dcmap;
    uint16_t dcmap_stream_id[MAX_DC_DCMAP_NUM];
    uint16_t dcmap_priority[MAX_DC_DCMAP_NUM];
    uint8_t  dcmap_config_index[MAX_DC_DCMAP_NUM];
    uint8_t  dcmap_ordered[MAX_DC_DCMAP_NUM];
    uint32_t dcmap_max_retr[MAX_DC_DCMAP_NUM];
    uint32_t dcmap_max_time[MAX_DC_DCMAP_NUM];
    char*    dcmap_label[MAX_DC_DCMAP_NUM];
    char*    dcmap_subprotocol[MAX_DC_DCMAP_NUM];
    int dcmap_count = 0;

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_DC_CREATE_RSP, m_slot_id,
        mCreateCmdCallTxIds.valueFor(ConfigsPtrs[0]->call_id));
    mCreateCmdCallTxIds.removeItem(ConfigsPtrs[0]->call_id);

    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_CALL_ID, ConfigsPtrs[0]->call_id);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_RESULT, ConfigsPtrs[0]->result);
    req_data->addMipcTlvUint16(MIPC_CALL_DC_CREATE_RSP_T_CAUSE, ConfigsPtrs[0]->cause);
    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_NUM_DC_CONFIG, (uint8_t)count);

    for (int i = 0; i < count; i++) {
        config_index[i] = ConfigsPtrs[i]->index;
        dc_type[i] = ConfigsPtrs[i]->dc_type;
        ip_addr_type[i] = ConfigsPtrs[i]->ip_addr_type;
        remote_udp_port[i] = ConfigsPtrs[i]->remote_udp_port;
        remote_sctp_port[i] = ConfigsPtrs[i]->remote_sctp_port;
        local_udp_port[i] = ConfigsPtrs[i]->local_udp_port;
        local_sctp_port[i] = ConfigsPtrs[i]->local_sctp_port;

        remote_addr[i] = (uint8_t*)ConfigsPtrs[i]->remote_addr;
        local_addr[i] = (uint8_t*)ConfigsPtrs[i]->local_addr;

        remote_dtls_setup[i] = ConfigsPtrs[i]->remote_dtls_setup;
        local_dtls_setup[i] = ConfigsPtrs[i]->local_dtls_setup;
        if ((char*)ConfigsPtrs[i]->remote_tls_id != NULL) {
            remote_tls_id[i] = (char*)ConfigsPtrs[i]->remote_tls_id;
        } else {
            remote_tls_id[i] = noStr;
        }
        if ((char*)ConfigsPtrs[i]->local_tls_id != NULL) {
            local_tls_id[i] = (char*)ConfigsPtrs[i]->local_tls_id;
        } else {
            local_tls_id[i] = noStr;
        }
        fp_hash_func[i] = ConfigsPtrs[i]->fp_hash_func;
        if ((char*)ConfigsPtrs[i]->remote_fp_str != NULL) {
            remote_fp_str[i] = (char*)ConfigsPtrs[i]->remote_fp_str;
        } else {
            remote_fp_str[i] = noStr;
        }
        if ((char*)ConfigsPtrs[i]->local_fp_str != NULL) {
            local_fp_str[i] = (char*)ConfigsPtrs[i]->local_fp_str;
        } else {
            local_fp_str[i] = noStr;
        }
        remote_max_message_size[i] = ConfigsPtrs[i]->remote_max_message_size;
        local_max_message_size[i] = ConfigsPtrs[i]->local_max_message_size;
        data_b_as[i] = ConfigsPtrs[i]->data_b_as;
        if ((char*)ConfigsPtrs[i]->qos_hint != NULL) {
            qos_hint[i] = (char*)ConfigsPtrs[i]->qos_hint;
        } else {
            qos_hint[i] = noStr;
        }

        if (ConfigsPtrs[i]->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
            logE(RFX_LOG_TAG,
                "vodc dchCreateCmdRsp, wrong dcmap_count:%d", ConfigsPtrs[i]->dcmap_count);
            ConfigsPtrs[i]->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }
        for (int j = 0; j < ConfigsPtrs[i]->dcmap_count; j++) {
            dcmap_stream_id[dcmap_count] = ConfigsPtrs[i]->dcmap_stream_id[j];
            dcmap_priority[dcmap_count] = ConfigsPtrs[i]->dcmap_priority[j];
            dcmap_config_index[dcmap_count] = config_index[i];
            dcmap_ordered[dcmap_count] = ConfigsPtrs[i]->dcmap_ordered[j];
            dcmap_max_retr[dcmap_count] = ConfigsPtrs[i]->dcmap_max_retr[j];
            dcmap_max_time[dcmap_count] = ConfigsPtrs[i]->dcmap_max_time[j];
            if ((char*)ConfigsPtrs[i]->dcmap_label[j] != NULL) {
                dcmap_label[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_label[j];
            } else {
                dcmap_label[dcmap_count] = noStr;
            }
            if ((char*)ConfigsPtrs[i]->dcmap_subprotocol[j] != NULL) {
                dcmap_subprotocol[dcmap_count] = (char*)ConfigsPtrs[i]->dcmap_subprotocol[j];
            } else {
                dcmap_subprotocol[dcmap_count] = noStr;
            }
            dcmap_count++;
        }
    }
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_INDEX, count, config_index);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_DC_TYPE, count, dc_type);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_IP_ADDR_TYPE, count, ip_addr_type);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_UDP_PORT, count, remote_udp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_SCTP_PORT, count, remote_sctp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_UDP_PORT, count, local_udp_port);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_SCTP_PORT, count, local_sctp_port);
    req_data->addMipcTlvArrayUint8Array(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_ADDR, count, remote_addr, MAX_DC_IP_ADDR_LENGTH);
    req_data->addMipcTlvArrayUint8Array(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_ADDR, count, local_addr, MAX_DC_IP_ADDR_LENGTH);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_DTLS_SETUP, count, remote_dtls_setup);
    req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_DTLS_SETUP, count, local_dtls_setup);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_TLS_ID, count, remote_tls_id);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_TLS_ID, count, local_tls_id);
    req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_FP_HASH_FUNC, count, fp_hash_func);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_FP_STR, count, remote_fp_str);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_FP_STR, count, local_fp_str);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_RSP_T_REMOTE_MAX_MESSAGE_SIZE, count, remote_max_message_size);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_RSP_T_LOCAL_MAX_MESSAGE_SIZE, count, local_max_message_size);
    req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_RSP_T_DATA_B_AS, count, data_b_as);
    req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_QOS_HINT, count, qos_hint);

    req_data->addMipcTlvUint8(MIPC_CALL_DC_CREATE_RSP_T_NUM_DCMAP, dcmap_count);
    if (dcmap_count > 0) {
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_STREAM_ID, dcmap_count, dcmap_stream_id);
        req_data->addMipcTlvArrayUint16(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_PRIORITY, dcmap_count, dcmap_priority);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_DC_CONFIG_INDEX, dcmap_count, dcmap_config_index);
        req_data->addMipcTlvArrayUint8(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_ORDERED, dcmap_count, dcmap_ordered);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_MAX_RETR, dcmap_count, dcmap_max_retr);
        req_data->addMipcTlvArrayUint32(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_MAX_TIME, dcmap_count, dcmap_max_time);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_LABEL, dcmap_count, dcmap_label);
        req_data->addMipcTlvArrayStr(MIPC_CALL_DC_CREATE_RSP_T_DCMAP_SUBPROTOCOL, dcmap_count, dcmap_subprotocol);
    }
    callToMipcMsgCmdResponse(req_data);

    sp<RfxMclMessage> mclResponse =
            RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    responseToTelCore(mclResponse);
}

// MTK solution start @{
void RmmDchRequestHandler::dchDataActPacketRouteRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_DATA_ACT_PACKET_ROUTE_REQ, m_slot_id);
    logD(RFX_LOG_TAG, "vodc dchDataActPacketRouteRequest");
    req_data->addMipcTlvUint8(MIPC_DATA_ACT_PACKET_ROUTE_REQ_T_APP_ID,
            MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_IMS);
    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchDataActPacketRouteResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    uint8_t app_Id = mipcData->getMipcUint8Val(MIPC_DATA_ACT_PACKET_ROUTE_CNF_T_APP_ID, 0);
    if (app_Id != MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_IMS) {
        logE(RFX_LOG_TAG, "vodc dchDataActPacketRouteResponse, error app_id:%d", app_Id);
    }

    logD(RFX_LOG_TAG, "vodc dchDataActPacketRouteResponse, result_code:%d", result_code);

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::dchDataIfupPacketRouteRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_DATA_IFUP_PACKET_ROUTE_REQ, m_slot_id);
    logD(RFX_LOG_TAG, "vodc dchDataIfupPacketRouteRequest");
    req_data->addMipcTlvUint8(MIPC_DATA_IFUP_PACKET_ROUTE_REQ_T_APP_ID,
            MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_IMS);
    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchDataIfupPacketRouteResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    uint8_t app_Id = mipcData->getMipcUint8Val(MIPC_DATA_IFUP_PACKET_ROUTE_CNF_T_APP_ID, 0);
    if (app_Id != MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_IMS) {
        logE(RFX_LOG_TAG, "vodc dchDataIfupPacketRouteResponse, error app_id:%d", app_Id);
    }

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxVoidData(), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::dchDataPathConfigSyncRequest(const sp<RfxMclMessage>& msg) {
    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_REQ, m_slot_id);
    logD(RFX_LOG_TAG, "vodc dchDataPathConfigSyncRequest");
    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchDataPathConfigSyncResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    RIL_DchDataPathConfig response;
    memset(&response, 0, sizeof(RIL_DchDataPathConfig));
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    logD(RFX_LOG_TAG, "vodc dchDataPathConfigSyncResponse, result_code:%d", result_code);

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    response.dc_mss = (uint32_t)mipcData->getMipcUint16Val(MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_DC_MSS, 0);

    uint16_t len = 0;
    uint8_t* client_ip = (uint8_t *)mipcData->getMipcVal(MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_CLIENT_IP, &len);
    if (len > MIPC_VOLTE_IPADDR_LENGTH) {
        logE(RFX_LOG_TAG, "vodc dchDataPathConfigSyncResponse, client_ip len:%d", len);
        len = MIPC_VOLTE_IPADDR_LENGTH;
    }
    if (len > 0 && client_ip != NULL) {
        logD(RFX_LOG_TAG, "vodc dchDataPathConfigSyncResponse, client_ip copy:%d, len:%d", client_ip[0], len);
        for (int x = 0; x < len; x++) {
           response.client_ip[x] = client_ip[x];
        }
    }

    uint8_t* server_ip = (uint8_t*)mipcData->getMipcVal(MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF_T_SERVER_IP, &len);
    if (len > MIPC_VOLTE_IPADDR_LENGTH) {
        logE(RFX_LOG_TAG, "vodc dchDataPathConfigSyncResponse, server_ip len:%d", len);
        len = MIPC_VOLTE_IPADDR_LENGTH;
    }
    if (len > 0 && server_ip != NULL) {
        logD(RFX_LOG_TAG, "vodc dchDataPathConfigSyncResponse, server_ip copy:%d, len:%d", server_ip[0], len);
        for (int x = 0; x < len; x++) {
            response.server_ip[x] = server_ip[x];
        }
    }

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxDchDataPathData(&response, sizeof(RIL_DchDataPathConfig)), msg, false);
    responseToTelCore(rsp);

    /* Set network interface up and address */
    dchSetNetwork(client_ip);
}

void RmmDchRequestHandler::dchCreateDataChannelRequest(const sp<RfxMclMessage>& msg) {
    if (msg->getData() == NULL || msg->getData()->getData() == NULL) {
        logE(RFX_LOG_TAG, "vodc dchCreateDataChannelRequest, error case, data is NULL");
        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(mclResponse);
        return;
    }

    RIL_DchXmlConfig *params = (RIL_DchXmlConfig *)msg->getData()->getData();

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_CREATE_DATA_CHANNEL_REQ, m_slot_id);

    req_data->addMipcTlvUint32(MIPC_CALL_CREATE_DATA_CHANNEL_REQ_T_CALL_ID, params->call_id);

    if (params->xml_data == NULL) {
        logI(RFX_LOG_TAG, "vodc dchCreateDataChannelRequest, call_id:%d xml_data is null", params->call_id);
    } else {
         uint16_t len = strlen(params->xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc dchCreateDataChannelRequest, error, call_id:%d xml_data len:%d too large",
                    params->call_id, len);
        } else {
            req_data->addMipcTlvStr(MIPC_CALL_CREATE_DATA_CHANNEL_REQ_T_DATA, params->xml_data);
            logD(RFX_LOG_TAG, "vodc dchCreateDataChannelRequest, call_id:%d, xml_data len:%d",
                    params->call_id, len);
        }
    }

    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchCreateDataChannelResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    RIL_DchXmlConfig response;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    response.call_id = mipcData->getMipcUint32Val(MIPC_CALL_CREATE_DATA_CHANNEL_CNF_T_CALL_ID, 0);

    response.xml_data = mipcData->getMipcStr(MIPC_CALL_CREATE_DATA_CHANNEL_CNF_T_DATA);
    if (response.xml_data != NULL) {
        uint16_t len = strlen(response.xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc dchCreateDataChannelResponse, mipcResult:%d, xml_data len:%d too large",
                    result_code, len);
            response.xml_data = NULL;
        } else {
            logD(RFX_LOG_TAG, "vodc dchCreateDataChannelResponse, mipcResult:%d, xml_data len:%d",
                    result_code, len);
        }
    } else {
        logD(RFX_LOG_TAG, "vodc dchCreateDataChannelResponse, mipcResult:%d, xml_data is empty", result_code);
    }

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxDchXmlConfigData(&response, sizeof(RIL_DchXmlConfig)), msg, false);
    responseToTelCore(rsp);
}

void RmmDchRequestHandler::dchCloseDataChannelRequest(const sp<RfxMclMessage>& msg) {
    if (msg->getData() == NULL || msg->getData()->getData() == NULL) {
        logE(RFX_LOG_TAG, "vodc dchCloseDataChannelRequest, error case, data is NULL");
        sp<RfxMclMessage> mclResponse = RfxMclMessage::obtainResponse(msg->getId(),
                RIL_E_INVALID_ARGUMENTS, RfxVoidData(), msg, false);
        responseToTelCore(mclResponse);
        return;
    }

    RIL_DchXmlConfig *params = (RIL_DchXmlConfig *)msg->getData()->getData();

    sp<RfxMipcData> req_data = RfxMipcData::obtainMipcData(MIPC_CALL_CLOSE_DATA_CHANNEL_REQ, m_slot_id);

    req_data->addMipcTlvUint32(MIPC_CALL_CLOSE_DATA_CHANNEL_REQ_T_CALL_ID, params->call_id);

    if (params->xml_data == NULL) {
        logI(RFX_LOG_TAG, "vodc dchCloseDataChannelRequest, call_id:%d xml_data is null", params->call_id);
    } else {
         uint16_t len = strlen(params->xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc dchCloseDataChannelRequest, error, call_id:%d xml_data len:%d too large",
                    params->call_id, len);
        } else {
            req_data->addMipcTlvStr(MIPC_CALL_CLOSE_DATA_CHANNEL_REQ_T_DATA, params->xml_data);
            logD(RFX_LOG_TAG, "vodc dchCloseDataChannelRequest, call_id:%d, xml_data len:%d",
                    params->call_id, len);
        }
    }

    callToMipcMsgAsync(msg, req_data);
}

void RmmDchRequestHandler::dchCloseDataChannelResponse(const sp<RfxMclMessage>& msg) {
    RIL_Errno rilErrNo = RIL_E_SUCCESS;
    RIL_DchXmlConfig response;
    RfxMipcData* mipcData = msg->getMipcData();
    unsigned int result_code = mipcData->getResult();

    if (MIPC_RESULT_SUCCESS != result_code) {
        rilErrNo = RIL_E_GENERIC_FAILURE;
    }

    response.call_id = mipcData->getMipcUint32Val(MIPC_CALL_CLOSE_DATA_CHANNEL_CNF_T_CALL_ID, 0);
    response.xml_data = mipcData->getMipcStr(MIPC_CALL_CLOSE_DATA_CHANNEL_CNF_T_DATA);
    if (response.xml_data != NULL) {
        uint16_t len = strlen(response.xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc dchCloseDataChannelResponse, mipcResult:%d, xml_data len:%d too large",
                    result_code, len);
            response.xml_data = NULL;
        } else {
            logD(RFX_LOG_TAG, "vodc dchCloseDataChannelResponse, mipcResult:%d, xml_data len:%d",
                    result_code, len);
        }
    } else {
        logD(RFX_LOG_TAG, "vodc dchCloseDataChannelResponse, mipcResult:%d, xml_data is empty", result_code);
    }

    sp<RfxMclMessage> rsp = RfxMclMessage::obtainResponse(msg->getId(), rilErrNo,
            RfxDchXmlConfigData(&response, sizeof(RIL_DchXmlConfig)), msg, false);
    responseToTelCore(rsp);
}

/* Set network interface up and address */
void RmmDchRequestHandler::handleEventSyncRouteParams(const sp<RfxMclMessage>& msg) {
    int *data = (int *)(msg->getData()->getData());
    if (data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleEventClearCreateCmd, data is NULL");
        return;
    }
    mNetType = data[0];
    mNetIf = data[1];
    mQueue = data[2];
    logD(RFX_LOG_TAG, "vodc handleEventSyncRouteParams, data:%d,%d,%d", mNetType, mNetIf, mQueue);
}

void RmmDchRequestHandler::dchSetNetwork(uint8_t* client_ip)
{
    /* Set network interface up and address */
    struct ifreq ifr;
    // The value is in RmmDchUrcHandler.cpp handleDchDataIfupPacketRouteInd
    // int queue = 3, net_type = 5, net_if = 19;
    int soc_fd, result;

    memset(&ifr, 0, sizeof(struct ifreq));
    result = snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "ccmni%d", mNetIf);
    if (result < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "vodc dchSetNetwork result error");
        return;
    }

    soc_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ril_data_setflags(soc_fd, &ifr, IFF_UP, 0);
    ril_data_setaddr(soc_fd, &ifr, (const char *)client_ip);

    ifr.ifr_flags = 0;
    ifr.ifr_ifru.ifru_ivalue = mQueue;
    logI(RFX_LOG_TAG, "vodc dchSetNetwork, mNetIf = %d, mQueue = %d, mNetType = %d", mNetIf, mQueue, mNetType);
    result = ioctl(soc_fd, SIOCSQID, &ifr);
    if (result < 0) {
        logE(RFX_LOG_TAG, "vodc dchSetNetwork, mNetIf = %d, error = %d", mNetIf, errno);
    }

    ifr.ifr_ifru.ifru_ivalue = mNetType;
    result = ioctl(soc_fd, SIOCSNETTYPE, &ifr);
    if (result < 0) {
        logE(RFX_LOG_TAG, "vodc dchSetNetwork, error network type = %d", mNetType);
    }

    close(soc_fd);
}

void RmmDchRequestHandler::ril_data_setflags(int s, struct ifreq *ifr, int set, int clr)
{
    if (ioctl(s, SIOCGIFFLAGS, ifr) < 0) {
        goto error_SIOCGIFFLAGS;
    }

    ifr->ifr_flags = (ifr->ifr_flags & (~clr)) | set;
    if (ioctl(s, SIOCSIFFLAGS, ifr) < 0) {
        goto error_SIOCSIFFLAGS;
    }

    return;

error_SIOCGIFFLAGS:
    logE(RFX_LOG_TAG, "vodc ril_data_setflags: set SIOCGIFFLAGS Error!");
    return;

error_SIOCSIFFLAGS:
    logE(RFX_LOG_TAG, "vodc ril_data_setflags: set SIOCSIFFLAGS Error!");
    return;
}

void RmmDchRequestHandler::ril_data_setaddr(int s, struct ifreq *ifr, const char *addr)
{
    int ret = 0;

    struct sockaddr_in *sin = (struct sockaddr_in *) &ifr->ifr_addr;
    sin->sin_family = AF_INET;
    sin->sin_port = 0;
    sin->sin_addr.s_addr = *(int *)addr;

    ret = ioctl(s, SIOCSIFADDR, ifr);
    if (ret < 0)
        goto terminate;
    return;
terminate:
    logE(RFX_LOG_TAG, "vodc ril_data_setaddr: error in set SIOCSIFADDR:ret = %d, error = %d",
            ret, errno);
    return;
}
// MTK solution end }@

