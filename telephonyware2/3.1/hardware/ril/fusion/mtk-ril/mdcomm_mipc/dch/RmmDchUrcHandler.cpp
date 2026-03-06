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

#include "RmmDchUrcHandler.h"

#include <mipc_msg.h>
#include "rfx_properties.h"
#include "RfxDchConfigData.h"
#include "RfxDchImsConfigData.h"
#include "RfxDchXmlConfigData.h"
#include "RfxMipcData.h"
#include <telephony/mtk_ril.h>

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmDchUrcHandler, RIL_CMD_PROXY_URC);

// register data
// Vendor solution
RFX_REGISTER_DATA_TO_URC_ID(RfxDchImsConfigData, RFX_MSG_UNSOL_DCH_CONFIG_INIT_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxDchImsConfigData, RFX_MSG_UNSOL_DCH_CONFIG_UPDATE_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_DCH_CONFIG_DEINIT_IND);
// MTK solution
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_UNSOL_DCH_IFUP_PACKET_ROUTE_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxDchXmlConfigData, RFX_MSG_UNSOL_DCH_CONNECT_DATA_CHANNEL_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxDchXmlConfigData, RFX_MSG_UNSOL_DCH_CLOSE_DATA_CHANNEL_IND);
RFX_REGISTER_DATA_TO_URC_ID(RfxDchXmlConfigData, RFX_MSG_UNSOL_DCH_CREATE_DATA_CHANNEL_IND);

RmmDchUrcHandler::RmmDchUrcHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {

    char dcSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get("persist.vendor.mtk_dc_call_capability", dcSupport, "0");

    logD(RFX_LOG_TAG, "vodc RmmDchUrcHandler constructor, dcSupport:%s", dcSupport);
    // int value = atoi(dcSupport);
    // if (value == 1 || value == 3) {
        const int urc[] = {
            MIPC_CALL_DC_CONFIG_INIT_IND,
            MIPC_CALL_DC_CONFIG_UPDATE_IND,
            MIPC_CALL_DC_CONFIG_DEINIT_IND,
        };

        registerToHandleMipcURC(urc, sizeof(urc)/sizeof(int));
    // }

    // MTK solution start @{
    // if (value == 2 || value == 3) {
        const int newUrc[] = {
            MIPC_DATA_IFUP_PACKET_ROUTE_IND,
            MIPC_CALL_CONNECT_DATA_CHANNEL_IND,
            MIPC_CALL_CLOSE_DATA_CHANNEL_IND,
            MIPC_CALL_CREATE_DATA_CHANNEL_IND,
        };
        registerToHandleMipcURC(newUrc, sizeof(newUrc)/sizeof(int));
    // }
    // MTK solution end }@
}

RmmDchUrcHandler::~RmmDchUrcHandler() {
}

void RmmDchUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    uint16_t mipcMsgId = (int) msg->getMipcData()->getMipcMsgId();
    logD(RFX_LOG_TAG, "vodc onHandleUrc, mipc message %04x", mipcMsgId);

    if (mipcMsgId == MIPC_CALL_DC_CONFIG_INIT_IND) {
        handleDchConfigInitInd(msg);
    } else if (mipcMsgId == MIPC_CALL_DC_CONFIG_UPDATE_IND) {
        handleDchConfigUpdateInd(msg);
    } else if (mipcMsgId == MIPC_CALL_DC_CONFIG_DEINIT_IND) {
        handleDchConfigDeinitInd(msg);
    } else if (mipcMsgId == MIPC_DATA_IFUP_PACKET_ROUTE_IND) {
        handleDchDataIfupPacketRouteInd(msg);
    } else if (mipcMsgId == MIPC_CALL_CONNECT_DATA_CHANNEL_IND) {
        handleDchConnectDataChannelInd(msg);
    } else if (mipcMsgId == MIPC_CALL_CLOSE_DATA_CHANNEL_IND) {
        handleDchCloseDataChannelInd(msg);
    } else if (mipcMsgId == MIPC_CALL_CREATE_DATA_CHANNEL_IND) {
        handleDchCreateDataChannelInd(msg);
    } else {
        logE(RFX_LOG_TAG, "vodc onHandleUrc, mipcMsgId %04x Should not be here", mipcMsgId);
    }
}

void RmmDchUrcHandler::handleDchConfigInitInd(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> urc;
    RIL_DchImsDataConfig response;
    memset(&response, 0, sizeof(RIL_DchImsDataConfig));

    uint16_t str_len = 0;
    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error case, urc_data is NULL");
        return;
    }

    mipc_msg_t *msg_ptr = urc_data->getData();
    if (msg_ptr == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error case, msg_ptr is NULL");
        return;
    }


    response.call_id = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_INIT_IND_T_CALL_ID, 0);
    response.if_name = urc_data->getMipcStr(MIPC_CALL_DC_CONFIG_INIT_IND_T_IF_NAME);
    response.dc_config_num = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_INIT_IND_T_NUM_DC_CONFIG, 0);

    logD(RFX_LOG_TAG, "vodc handleDchConfigInitInd, dc_config_num:%d", response.dc_config_num);
    if (response.dc_config_num > MAX_DC_CONFIG_NUM) {
        response.dc_config_num = MAX_DC_CONFIG_NUM;
    }

    uint8_t dcmap_config_index[MAX_BDC_DCMAP_NUM];
    uint8_t dcmap_count = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_INIT_IND_T_NUM_DCMAP, 0);
    for (uint16_t i = 0; i < dcmap_count; i++) {
        dcmap_config_index[i] = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_CONFIG_INDEX, i, 0);
    }

    for (uint16_t i = 0; i < response.dc_config_num; i++) {
        RIL_DchConfig* dchConfig = &(response.dc_config_list[i]);
        dchConfig->call_id = response.call_id;
        dchConfig->index =  urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_INDEX, i, 0);
        dchConfig->dc_type= urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DC_TYPE, i, 0);
        dchConfig->ip_addr_type = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_IP_ADDR_TYPE, i, 0);
        dchConfig->remote_udp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_UDP_PORT, i, 0);
        dchConfig->remote_sctp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_SCTP_PORT, i, 0);
        dchConfig->local_udp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_UDP_PORT, i, 0);
        dchConfig->local_sctp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_SCTP_PORT, i, 0);

        uint8_t* remote_addr = (uint8_t *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_ADDR, i, &str_len);
        if (str_len > MIPC_VOLTE_IPADDR_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, remote_addr str_len:%d", str_len);
            str_len = MIPC_VOLTE_IPADDR_LENGTH;
        }
        if (str_len > 0 && remote_addr != NULL) {
            //memcpy(dchConfig->remote_addr, remote_addr, str_len);
            logD(RFX_LOG_TAG, "vodc handleDchConfigInitInd, remote_addr copy:%d, str_len:%d", remote_addr[0], str_len);
            for (int x = 0; x < str_len; x++) {
                dchConfig->remote_addr[x] = remote_addr[x];
            }
        }

        uint8_t* local_addr = (uint8_t *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_ADDR, i, &str_len);
        if (str_len > MIPC_VOLTE_IPADDR_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, local_addr str_len:%d", str_len);
            str_len = MIPC_VOLTE_IPADDR_LENGTH;
        }
        if (str_len > 0 && local_addr != NULL) {
            //memcpy(dchConfig->local_addr, local_addr, str_len);
            logD(RFX_LOG_TAG, "vodc handleDchConfigInitInd, local_addr copy:%d, str_len:%d", local_addr[0], str_len);
            for (int x = 0; x < str_len; x++) {
                dchConfig->local_addr[x] = local_addr[x];
            }
        }

        dchConfig->remote_dtls_setup = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_DTLS_SETUP, i, 0);
        dchConfig->local_dtls_setup = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_DTLS_SETUP, i, 0);

        dchConfig->remote_tls_id = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_TLS_ID, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_TLS_ID_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error remote_tls_id str_len:%d", str_len);
        }
        dchConfig->local_tls_id = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_TLS_ID, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_TLS_ID_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error local_tls_id str_len:%d", str_len);
        }
        dchConfig->fp_hash_func =  urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_FP_HASH_FUNC, i, 0);

        dchConfig->remote_fp_str = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_FP_STR, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_FINGERPRINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error remote_fp_str str_len:%d", str_len);
        }
        dchConfig->local_fp_str = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_FP_STR, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_FINGERPRINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error local_fp_str str_len:%d", str_len);
        }
        dchConfig->remote_max_message_size = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_REMOTE_MAX_MESSAGE_SIZE, i, 0);
        dchConfig->local_max_message_size = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_LOCAL_MAX_MESSAGE_SIZE, i, 0);
        dchConfig->data_b_as = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DATA_B_AS, i, 0);
        dchConfig->qos_hint = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_QOS_HINT, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_QOS_HINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error qos_hint str_len:%d", str_len);
        }

        //RIL_DcMapStruct dcmap;  //RFC 8864
        uint16_t j = 0;
        for (uint16_t k = 0; k < dcmap_count; k++) {
            if (dcmap_config_index[k] != dchConfig->index) {
                continue;
            }
            dchConfig->dcmap_stream_id[j] = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_STREAM_ID, k, 0);
            dchConfig->dcmap_priority[j] = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_PRIORITY, k, 0);
            dchConfig->dcmap_ordered[j] = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_ORDERED, k, 0);
            dchConfig->dcmap_max_retr[j] =  urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_RETR, k, 0);
            dchConfig->dcmap_max_time[j] = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_MAX_TIME, k, 0);
            dchConfig->dcmap_label[j] = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_LABEL, k, &str_len);
            if (str_len > MIPC_VOLTE_MAX_DCMAP_LABEL_LENGTH) {
                logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error dcmap_label str_len:%d", str_len);
            }
            dchConfig->dcmap_subprotocol[j] = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_INIT_IND_T_DCMAP_SUBPROTOCOL, k, &str_len);
            if (str_len > MIPC_VOLTE_MAX_DCMAP_SUBPROTOCOL_LENGTH) {
                logE(RFX_LOG_TAG, "vodc handleDchConfigInitInd, error dcmap_subprotocol str_len:%d", str_len);
            }
            j++;
        }
        dchConfig->dcmap_count = j;
        logI(RFX_LOG_TAG, "vodc handleDchConfigInitInd, callId:%d, i:%d, index:%d, dcmap_count:%d",
                response.call_id, i, dchConfig->index, j);
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CONFIG_INIT_IND,
            m_slot_id, RfxDchImsConfigData(&response, sizeof(RIL_DchImsDataConfig)));
    responseToTelCore(urc);
}

void RmmDchUrcHandler::handleDchConfigUpdateInd(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> urc;
    RIL_DchImsDataConfig response;
    memset(&response, 0, sizeof(RIL_DchImsDataConfig));

    uint16_t str_len = 0;
    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error case, urc_data is NULL");
        return;
    }

    mipc_msg_t *msg_ptr = urc_data->getData();
    if (msg_ptr == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error case, msg_ptr is NULL");
        return;
    }

    response.call_id = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_CALL_ID, 0);
    response.if_name = urc_data->getMipcStr(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IF_NAME);
    if (response.if_name != NULL && strlen(response.if_name) > MIPC_VOLTE_MAX_IF_NAME_LENGTH) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error if_name str_len:%lu", strlen(response.if_name));
    }
    response.dc_config_num = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_NUM_DC_CONFIG, 0);
    if (response.dc_config_num > MAX_DC_CONFIG_NUM) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error dc_config_num:%d", response.dc_config_num);
        response.dc_config_num = MAX_DC_CONFIG_NUM;
    }

    uint8_t dcmap_config_index[MAX_BDC_DCMAP_NUM];
    uint8_t dcmap_count = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_NUM_DCMAP, 0);
    for (uint16_t i = 0; i < dcmap_count; i++) {
        dcmap_config_index[i] = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_CONFIG_INDEX, i, 0);
    }

    for (uint16_t i = 0; i < response.dc_config_num; i++) {
        RIL_DchConfig* dchConfig = &(response.dc_config_list[i]);
        dchConfig->index =  urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_INDEX, i, 0);
        dchConfig->dc_type= urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DC_TYPE, i, 0);
        dchConfig->ip_addr_type = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_IP_ADDR_TYPE, i, 0);
        dchConfig->remote_udp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_UDP_PORT, i, 0);
        dchConfig->remote_sctp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_SCTP_PORT, i, 0);
        dchConfig->local_udp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_UDP_PORT, i, 0);
        dchConfig->local_sctp_port = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_SCTP_PORT, i, 0);

        uint8_t* remote_addr = (uint8_t *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_ADDR, i, &str_len);
        if (str_len > MIPC_VOLTE_IPADDR_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, remote_addr str_len:%d", str_len);
            str_len = MIPC_VOLTE_IPADDR_LENGTH;
        }
        if (str_len > 0 && remote_addr != NULL) {
            //memcpy(dchConfig->remote_addr, remote_addr, str_len);
            logD(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, remote_addr copy:%d, str_len:%d", remote_addr[0], str_len);
            for (int x = 0; x < str_len; x++) {
                dchConfig->remote_addr[x] = remote_addr[x];
            }
        }

        uint8_t* local_addr = (uint8_t *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_ADDR, i, &str_len);
        if (str_len > MIPC_VOLTE_IPADDR_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, local_addr str_len:%d", str_len);
            str_len = MIPC_VOLTE_IPADDR_LENGTH;
        }
        if (str_len > 0 && local_addr != NULL) {
            //memcpy(dchConfig->local_addr, local_addr, str_len);
            logD(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, local_addr copy:%d, str_len:%d", local_addr[0], str_len);
            for (int x = 0; x < str_len; x++) {
                dchConfig->local_addr[x] = local_addr[x];
            }
        }

        dchConfig->remote_dtls_setup = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_DTLS_SETUP, i, 0);
        dchConfig->local_dtls_setup = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_DTLS_SETUP, i, 0);

        dchConfig->remote_tls_id = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_TLS_ID, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_TLS_ID_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error remote_tls_id str_len:%d", str_len);
        }
        dchConfig->local_tls_id = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_TLS_ID, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_TLS_ID_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error local_tls_id str_len:%d", str_len);
        }
        dchConfig->fp_hash_func =  urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_FP_HASH_FUNC, i, 0);

        dchConfig->remote_fp_str = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_FP_STR, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_FINGERPRINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error remote_fp_str str_len:%d", str_len);
        }
        dchConfig->local_fp_str = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_FP_STR, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_FINGERPRINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error local_fp_str str_len:%d", str_len);
        }
        dchConfig->remote_max_message_size = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_REMOTE_MAX_MESSAGE_SIZE, i, 0);
        dchConfig->local_max_message_size = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_LOCAL_MAX_MESSAGE_SIZE, i, 0);
        dchConfig->data_b_as = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DATA_B_AS, i, 0);
        dchConfig->qos_hint = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_QOS_HINT, i, &str_len);
        if (str_len > MIPC_VOLTE_MAX_QOS_HINT_LENGTH) {
            logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error qos_hint str_len:%d", str_len);
        }

        //RIL_DcMapStruct dcmap;  //RFC 8864
        uint16_t j = 0;
        for (uint16_t k = 0; k < dcmap_count; k++) {
            if (dcmap_config_index[k] != dchConfig->index) {
                continue;
            }
            dchConfig->dcmap_stream_id[j] = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_STREAM_ID, k, 0);
            dchConfig->dcmap_priority[j] = urc_data->getMipcUint16IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_PRIORITY, k, 0);
            dchConfig->dcmap_ordered[j] = urc_data->getMipcUint8IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_ORDERED, k, 0);
            dchConfig->dcmap_max_retr[j] =  urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_RETR, k, 0);
            dchConfig->dcmap_max_time[j] = urc_data->getMipcUint32IdxVal(MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_MAX_TIME, k, 0);
            dchConfig->dcmap_label[j] = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_LABEL, k, &str_len);
            if (str_len > MIPC_VOLTE_MAX_DCMAP_LABEL_LENGTH) {
                logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error dcmap_label str_len:%d", str_len);
            }
            dchConfig->dcmap_subprotocol[j] = (char *)mipc_msg_get_idx_ptr(msg_ptr, MIPC_CALL_DC_CONFIG_UPDATE_IND_T_DCMAP_SUBPROTOCOL, k, &str_len);
            if (str_len > MIPC_VOLTE_MAX_DCMAP_SUBPROTOCOL_LENGTH) {
                logE(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, error dcmap_subprotocol str_len:%d", str_len);
            }
            j++;
        }
        dchConfig->dcmap_count = j;
        logI(RFX_LOG_TAG, "vodc handleDchConfigUpdateInd, callId: %d, i:%d, index: %d, dcmap_count: %d",
                response.call_id, i, dchConfig->index, j);
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CONFIG_UPDATE_IND,
            m_slot_id, RfxDchImsConfigData(&response, sizeof(RIL_DchImsDataConfig)));
    responseToTelCore(urc);
}

void RmmDchUrcHandler::handleDchConfigDeinitInd(const sp<RfxMclMessage>& msg) {
    int response[1] = { 0 };
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchConfigDeinitInd, error case, urc_data is NULL");
        return;
    }

    uint8_t callId = urc_data->getMipcUint8Val(MIPC_CALL_DC_CONFIG_DEINIT_IND_T_CALL_ID, 0);

    // get operator code
    response[0] = (int)callId;

    logI(RFX_LOG_TAG, "vodc handleDchConfigDeinitInd, callId:%d", callId);

    sendEvent(RFX_MSG_EVENT_DCH_CLEAR_CMD,
            RfxIntsData(response, 1), RIL_CMD_PROXY_1, m_slot_id);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CONFIG_DEINIT_IND,
            m_slot_id, RfxIntsData(response, 1));
    responseToTelCore(urc);
}

// MTK solution start @{
void RmmDchUrcHandler::handleDchDataIfupPacketRouteInd(const sp<RfxMclMessage>& msg) {
    int response[3] = { 0 };
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchDataIfupPacketRouteInd, error case, urc_data is NULL");
        return;
    }

    uint8_t app_Id = urc_data->getMipcUint8Val(MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_APP_ID, 0);
    if (app_Id != MIPC_IPC_PACKET_ROUTE_APP_ID_ENUM_IPC_PACKET_ROUTE_APP_ID_EAP_IMS) {
        logI(RFX_LOG_TAG, "vodc handleDchDataIfupPacketRouteInd, app_id:%d", app_Id);
        return;
    }
    uint8_t net_type = urc_data->getMipcUint8Val(MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_NET_TYPE, 0);
    uint8_t nw_itf_id = urc_data->getMipcUint8Val(MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_NW_ITF_ID, 0);
    uint8_t hw_ul_q = urc_data->getMipcUint8Val(MIPC_DATA_IFUP_PACKET_ROUTE_IND_T_HW_UL_Q, 0);

    response[0] = (int)net_type;
    response[1] = (int)nw_itf_id;
    response[2] = (int)hw_ul_q;

    logI(RFX_LOG_TAG, "vodc handleDchDataIfupPacketRouteInd, data:%d,%d,%d", net_type, nw_itf_id, hw_ul_q);

    sendEvent(RFX_MSG_EVENT_DCH_SYNC_ROUTE_PARAMS,
            RfxIntsData(response, 3), RIL_CMD_PROXY_1, m_slot_id);

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_IFUP_PACKET_ROUTE_IND,
            m_slot_id, RfxIntsData(response, 3));
    responseToTelCore(urc);
}

void RmmDchUrcHandler::handleDchConnectDataChannelInd(const sp<RfxMclMessage>& msg) {
    RIL_DchXmlConfig response;
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchConnectDataChannelInd, error case, urc_data is NULL");
        return;
    }

    uint8_t call_id = urc_data->getMipcUint32Val(MIPC_CALL_CONNECT_DATA_CHANNEL_IND_T_CALL_ID, 0);
    response.call_id = (uint32_t)call_id;
    response.xml_data = urc_data->getMipcStr(MIPC_CALL_CONNECT_DATA_CHANNEL_IND_T_DATA);
    if (response.xml_data != NULL) {
        uint16_t len = strlen(response.xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc handleDchConnectDataChannelInd, callId:%d xmlData len:%d too large",
                    call_id, len);
            return;
        } else {
            logD(RFX_LOG_TAG, "vodc handleDchConnectDataChannelInd, callId:%d, xmlData len:%d",
                    call_id, len);
        }
    } else {
        logD(RFX_LOG_TAG, "vodc handleDchConnectDataChannelInd, callId:%d xmlData is empty", call_id);
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CONNECT_DATA_CHANNEL_IND,
            m_slot_id, RfxDchXmlConfigData(&response, sizeof(RIL_DchXmlConfig)));
    responseToTelCore(urc);
}

void RmmDchUrcHandler::handleDchCloseDataChannelInd(const sp<RfxMclMessage>& msg) {
    RIL_DchXmlConfig response;
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchCloseDataChannelInd, error case, urc_data is NULL");
        return;
    }

    uint8_t call_id = urc_data->getMipcUint32Val(MIPC_CALL_CLOSE_DATA_CHANNEL_IND_T_CALL_ID, 0);
    response.call_id = (uint32_t)call_id;
    response.xml_data = urc_data->getMipcStr(MIPC_CALL_CLOSE_DATA_CHANNEL_IND_T_DATA);
    if (response.xml_data != NULL) {
        uint16_t len = strlen(response.xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc handleDchCloseDataChannelInd, callId:%d xmlData len:%d too large",
                    call_id, len);
            return;
        } else {
            logD(RFX_LOG_TAG, "vodc handleDchCloseDataChannelInd, callId:%d, xmlData len:%d",
                    call_id, len);
        }
    } else {
        logD(RFX_LOG_TAG, "vodc handleDchCloseDataChannelInd, callId:%d xmlData is empty", call_id);
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CLOSE_DATA_CHANNEL_IND,
            m_slot_id, RfxDchXmlConfigData(&response, sizeof(RIL_DchXmlConfig)));
    responseToTelCore(urc);
}

void RmmDchUrcHandler::handleDchCreateDataChannelInd(const sp<RfxMclMessage>& msg) {
    RIL_DchXmlConfig response;
    sp<RfxMclMessage> urc;

    RfxMipcData* urc_data = msg->getMipcData();
    if (urc_data == NULL) {
        logE(RFX_LOG_TAG, "vodc handleDchCreateDataChannelInd, error case, urc_data is NULL");
        return;
    }

    uint8_t call_id = urc_data->getMipcUint32Val(MIPC_CALL_CREATE_DATA_CHANNEL_IND_T_CALL_ID, 0);
    response.call_id = (uint32_t)call_id;
    response.xml_data = urc_data->getMipcStr(MIPC_CALL_CREATE_DATA_CHANNEL_IND_T_DATA);
    if (response.xml_data != NULL) {
        uint16_t len = strlen(response.xml_data);
        if (len > MIPC_MAX_DATA_CHANNEL_XML_DATA_LEN) {
            logE(RFX_LOG_TAG, "vodc handleDchCreateDataChannelInd, callId:%d xmlData len:%d too large",
                    call_id, len);
            return;
        } else {
            logD(RFX_LOG_TAG, "vodc handleDchCreateDataChannelInd, callId:%d, xmlData len:%d",
                    call_id, len);
        }
    } else {
        logD(RFX_LOG_TAG, "vodc handleDchCreateDataChannelInd, callId:%d xmlData is empty", call_id);
    }

    urc = RfxMclMessage::obtainUrc(RFX_MSG_UNSOL_DCH_CREATE_DATA_CHANNEL_IND,
            m_slot_id, RfxDchXmlConfigData(&response, sizeof(RIL_DchXmlConfig)));
    responseToTelCore(urc);
}
// MTK solution end }@


