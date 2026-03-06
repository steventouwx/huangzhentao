/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2023. All rights reserved.
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

#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <ril_service.h>
#include <rilAidlUtils.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <radiodch_service.h>

#include <binder/ProcessState.h>
#include <android/binder_manager.h>

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#include <binder/RpcServer.h>
#include <android/binder_libbinder.h>
#endif

#undef LOG_TAG
#define LOG_TAG "RILC"

namespace aidl_radio = ::aidl::android::hardware::radio;
namespace aidl_mtk_radio_dch = ::aidl::vendor::mediatek::hardware::mtkradioex::dch;

using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::populateAidlResponseInfo;
using rilAidlUtils::convertIntToAidlRadioIndicationType;
using rilAidlUtils::copyAidlStringToRil;
using rilAidlUtils::retChecker;

extern "C" unsigned int toRealSlot(unsigned int slotId);

struct RadioDchImpl {
    // Vendor data channel solution
    std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcResponse> mtkDchResponse;
    std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcIndication> mtkDchIndication;

    // MTK data channel solution
    std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcResponse> mtkDchInternalResponse;
    std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcIndication> mtkDchInternalIndication;
};

RadioDchImpl radioDchServiceImpl[MAX_SIM_COUNT];
static RIL_ImsPendingInd *s_dch_pending_ind[MAX_SIM_COUNT] = { NULL }; // Vendor data channel solution
bool mIsBindFail[MAX_SIM_COUNT] = { false };
static RIL_dchDataPendingInd *s_dch_internal_pending_ind[MAX_SIM_COUNT] = { NULL }; // MTK data channel solution

bool checkDchDataValid(const char *str, int maxLength) {
    if (str!= NULL) {
        if (strlen(str) > maxLength) {
            mtkLogI(LOG_TAG, "vodc checkDchDataValid: invalid, maxLength: %d>%d", strlen(str), maxLength);
            return false;
        }
    }
    return true;
}

bool dispatchDchCloseSipReq(int serial, int slotId, int request,
        uint8_t callId, uint8_t closeAllStream,
        const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcMapDelStruct>& delList) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int cfgCount = delList.size();
    if (cfgCount == 0 || cfgCount > MAX_DCMAP_NUM_PER_CONFIG) {
        mtkLogE(LOG_TAG, "dispatchDchCloseSipReq, vodc data count error, %d, request %s",
            cfgCount, android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    RIL_DchMapDelStruct *out =
        (RIL_DchMapDelStruct *) calloc(1, sizeof(RIL_DchMapDelStruct));
    if (out == NULL) {
        mtkLogE(LOG_TAG, "dispatchDchCloseSipReq, vodc Memory allocation failed for request %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    out->callId = callId;
    out->closeAllStream = closeAllStream;
    out->dcmap_count = cfgCount;
    for (int i = 0; i < cfgCount; i++) {
        out->dc_config_index[i] = delList[i].dc_config_index;
        out->dcmap_stream_id[i] = (uint16_t) delList[i].dcmap_stream_id;
    }
    CALL_ONREQUEST(request, out, sizeof(RIL_DchMapDelStruct), pRI, slotId);
    free(out);
    return true;
}

bool dispatchDchConfig(int serial, int slotId, int request, uint8_t callId,
        uint8_t result, uint16_t cause,
        const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int cfgCount = dataList.size();
    if (cfgCount == 0 || cfgCount > MAX_DC_CONFIG_NUM) {
        mtkLogE(LOG_TAG, "dispatchDchConfig, vodc data count error, %d, request %s",
            cfgCount, android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    bool success = true;
    RIL_DchConfig *configs =
        (RIL_DchConfig *) calloc(cfgCount, sizeof(RIL_DchConfig));

    if (configs == NULL) {
        mtkLogE(LOG_TAG, "dispatchDchConfig, vodc Memory allocation failed for request %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    for (int i = 0; i < cfgCount; i++) {
        RIL_DchConfig* config = &(configs[i]);
        const aidl_mtk_radio_dch::DcConfig* data = &(dataList[i]);
        int j = 0;

        config->call_id = callId;
        config->result = result;
        config->cause = cause;

        config->index = data->index;
        config->dc_type = data->dc_type;
        config->ip_addr_type = data->ip_addr_type;
        config->remote_udp_port = (uint16_t) data->remote_udp_port;
        config->remote_sctp_port = (uint16_t) data->remote_sctp_port;
        config->local_udp_port = (uint16_t) data->local_udp_port;
        config->local_sctp_port = (uint16_t) data->local_sctp_port;
        config->remote_dtls_setup = data->remote_dtls_setup;
        config->local_dtls_setup = data->local_dtls_setup;
        config->fp_hash_func = (uint16_t) data->fp_hash_func;
        config->remote_max_message_size = data->remote_max_message_size;
        config->local_max_message_size = data->local_max_message_size;
        config->data_b_as = data->data_b_as;
        config->dcmap_count = data->dcmap_count;

        int mapCount = (data->dcmap).size();
        if (mapCount < config->dcmap_count) {
            config->dcmap_count = mapCount;
            mtkLogE(LOG_TAG, "vodc dispatchDchConfig: real dcmap_count: %d", mapCount);
        }
        if (config->dcmap_count >= MAX_DCMAP_NUM_PER_CONFIG) {
            mtkLogE(LOG_TAG, "vodc dispatchDchConfig: invalid dcmap_count: %d", config->dcmap_count);
            config->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }
        for (j = 0; j < config->dcmap_count; j++) {
            config->dcmap_stream_id[j] = (uint16_t) data->dcmap[j].dcmap_stream_id;
            config->dcmap_priority[j] = (uint16_t) data->dcmap[j].priority;
            config->dcmap_ordered[j] = data->dcmap[j].ordered;
            config->dcmap_max_retr[j] = data->dcmap[j].max_retr;
            config->dcmap_max_time[j] = data->dcmap[j].max_time;
        }

        int size = data->remote_addr.size();
        for (int x = 0; x < MAX_DC_IP_ADDR_LENGTH; x++) {
            if (x < size) {
                config->remote_addr[x] = data->remote_addr[x];
            } else {
                config->remote_addr[x] = 0;
            }
        }
        size = data->local_addr.size();
        for (int y = 0; y < MAX_DC_IP_ADDR_LENGTH; y++) {
            if (y < size) {
                config->local_addr[y] = data->local_addr[y];
            } else {
                config->local_addr[y] = 0;
            }
        }

        if (!copyAidlStringToRil(&config->remote_tls_id, data->remote_tls_id, pRI)) {
            success = false;;
        }
        if (success && !copyAidlStringToRil(&config->local_tls_id, data->local_tls_id, pRI)) {
            success = false;
        }
        if (success && !copyAidlStringToRil(&config->remote_fp_str, data->remote_fp_str, pRI)) {
            success = false;
        }
        if (success && !copyAidlStringToRil(&config->local_fp_str, data->local_fp_str, pRI)) {
            success = false;
        }
        if (success && !copyAidlStringToRil(&config->qos_hint, data->qos_hint, pRI)) {
            success = false;
        }
        if (success) {
            if (!checkDchDataValid(config->remote_tls_id, MAX_DC_TLS_ID_LENGTH)
                    || !checkDchDataValid(config->local_tls_id, MAX_DC_TLS_ID_LENGTH)
                    || !checkDchDataValid(config->remote_fp_str, MAX_DC_FINGERPRINT_LENGTH)
                    || !checkDchDataValid(config->local_fp_str, MAX_DC_FINGERPRINT_LENGTH)
                    || !checkDchDataValid(config->qos_hint, MAX_DC_QOS_HINT_LENGTH)) {
                mtkLogE(LOG_TAG, "vodc dispatchDchConfig: invalid data 1");
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                success = false;
            }
        }
        for (j = 0; j < config->dcmap_count; j++) {
            if (success && !copyAidlStringToRil(&config->dcmap_label[j], data->dcmap[j].label, pRI)) {
                success = false;
            }
            if (success && !copyAidlStringToRil(&config->dcmap_subprotocol[j], data->dcmap[j].subprotocol, pRI)) {
                success = false;
            }
            if (success) {
                if (!checkDchDataValid(config->dcmap_label[j], MAX_DCMAP_LABEL_LENGTH)
                        || !checkDchDataValid(config->dcmap_subprotocol[j], MAX_DCMAP_SUBPROTOCOL_LENGTH)) {
                    mtkLogE(LOG_TAG, "vodc dispatchDchConfig: invalid data 2");
                    sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                    success = false;
                }
            }
        }
        if (!success) {
            for (int k = 0; k <= i; k++) {
                RIL_DchConfig* config = &(configs[k]);
                memsetAndFreeStrings(2, config->remote_tls_id, config->remote_fp_str);
                memsetAndFreeStrings(3, config->local_tls_id, config->local_fp_str,  config->qos_hint);
                for (j = 0; j < config->dcmap_count; j++) {
                    memsetAndFreeStrings(2, config->dcmap_label[j], config->dcmap_subprotocol[j]);
                }
            }
            free(configs);
            mtkLogE(LOG_TAG, "vodc dispatchDchConfig: invalid data 3");
            // sendErrorResponse(pRI, RIL_E_NO_MEMORY); already send when copyAidlStringToRil() fail
            return false;
        }
    }

    CALL_ONREQUEST(request, configs, cfgCount*sizeof(RIL_DchConfig), pRI, slotId);

    for (int i = 0; i < cfgCount; i++) {
        RIL_DchConfig* config = &(configs[i]);
        memsetAndFreeStrings(2, config->remote_tls_id, config->remote_fp_str);
        memsetAndFreeStrings(3, config->local_tls_id, config->local_fp_str,  config->qos_hint);
        for (int j = 0; j < config->dcmap_count; j++) {
            memsetAndFreeStrings(2, config->dcmap_label[j], config->dcmap_subprotocol[j]);
        }
    }
    free(configs);
    return true;
}

bool dispatchDchSetCapabilityConfig(int serial, int slotId, int request,
        uint8_t callId, uint8_t result,
        const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfigLocal>& dataList) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    int cfgCount = dataList.size();
    if (cfgCount == 0 || cfgCount > MAX_BDC_CONFIG_NUM) {
        mtkLogE(LOG_TAG, "vodc dispatchDchSetCapabilityConfig data count error, %d, request %s", cfgCount,
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    }

    bool success = true;
    RIL_DchConfigLocal *configs =
        (RIL_DchConfigLocal *) calloc(cfgCount, sizeof(RIL_DchConfigLocal));

    if (configs == NULL) {
        mtkLogE(LOG_TAG, "vodc dispatchDchSetCapabilityConfig Memory allocation failed for request %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    for (int i = 0; i < cfgCount; i++) {
        RIL_DchConfigLocal* config = &(configs[i]);
        const aidl_mtk_radio_dch::DcConfigLocal* data = &(dataList[i]);
        int j = 0;

        config->call_id = callId;
        config->result = result;
        config->cause = 0;

        config->local_udp_port = (uint16_t) data->local_udp_port;
        config->local_sctp_port = (uint16_t) data->local_sctp_port;
        config->index = data->index;
        config->local_dtls_setup = data->local_dtls_setup;
        config->fp_hash_func = (uint16_t) data->fp_hash_func;
        config->local_max_message_size = data->local_max_message_size;
        config->data_b_as = data->data_b_as;
        config->dcmap_count = data->dcmap_count;

        int mapCount = (data->dcmap).size();
        mtkLogD(LOG_TAG, "vodc dispatchDchSetCapabilityConfig: index:%d, dcmap_count:%d,%d",
                data->index, config->dcmap_count, mapCount);

        if (mapCount < config->dcmap_count) {
            config->dcmap_count = mapCount;
        }
        if (config->dcmap_count >= MAX_DCMAP_NUM_PER_CONFIG) {
            mtkLogE(LOG_TAG, "vodc dispatchDchSetCapabilityConfig: invalid dcmap_count: %d", config->dcmap_count);
            config->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
        }
        for (j = 0; j < config->dcmap_count; j++) {
            config->dcmap_stream_id[j] = (uint16_t) data->dcmap[j].dcmap_stream_id;
            config->dcmap_priority[j] = (uint16_t) data->dcmap[j].priority;
            config->dcmap_ordered[j] = data->dcmap[j].ordered;
            config->dcmap_max_retr[j] = data->dcmap[j].max_retr;
            config->dcmap_max_time[j] = data->dcmap[j].max_time;
        }
        if (!copyAidlStringToRil(&config->local_tls_id, data->local_tls_id, pRI)) {
            success = false;;
        }
        if (success && !copyAidlStringToRil(&config->local_fp_str, data->local_fp_str, pRI)) {
            success = false;
        }
        if (success && !copyAidlStringToRil(&config->qos_hint, data->qos_hint, pRI)) {
            success = false;
        }
        if (success) {
            if (!checkDchDataValid(config->local_tls_id, MAX_DC_TLS_ID_LENGTH)
                    || !checkDchDataValid(config->local_fp_str, MAX_DC_FINGERPRINT_LENGTH)
                    || !checkDchDataValid(config->qos_hint, MAX_DC_QOS_HINT_LENGTH)) {
                mtkLogE(LOG_TAG, "vodc dispatchDchSetCapabilityConfig: invalid data 1");
                sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                success = false;
            }
        }

        for (j = 0; j < config->dcmap_count; j++) {
            if (success && !copyAidlStringToRil(&config->dcmap_label[j], data->dcmap[j].label, pRI)) {
                success = false;
            }
            if (success && !copyAidlStringToRil(&config->dcmap_subprotocol[j], data->dcmap[j].subprotocol, pRI)) {
                success = false;
            }
            if (success) {
                if (!checkDchDataValid(config->dcmap_label[j], MAX_DCMAP_LABEL_LENGTH)
                        || !checkDchDataValid(config->dcmap_subprotocol[j], MAX_DCMAP_SUBPROTOCOL_LENGTH)) {
                    mtkLogE(LOG_TAG, "vodc dispatchDchSetCapabilityConfig: invalid data 2");
                    sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
                    success = false;
                }
            }
        }
        if (!success) {
            for (int k = 0; k <= i; k++) {
                RIL_DchConfigLocal* config = &(configs[k]);
                memsetAndFreeStrings(3, config->local_tls_id, config->local_fp_str, config->qos_hint);
                for (j = 0; j < config->dcmap_count; j++) {
                    memsetAndFreeStrings(2, config->dcmap_label[j], config->dcmap_subprotocol[j]);
                }
            }
            free(configs);
            mtkLogE(LOG_TAG, "vodc dispatchDchSetCapabilityConfig: invalid data 3");
            // sendErrorResponse(pRI, RIL_E_NO_MEMORY); already send when copyAidlStringToRil() fail
            return false;
        }
    }

    CALL_ONREQUEST(request, configs, cfgCount*sizeof(RIL_DchConfigLocal), pRI, slotId);
    for (int i = 0; i < cfgCount; i++) {
        RIL_DchConfigLocal* config = &(configs[i]);
        memsetAndFreeStrings(3, config->local_tls_id, config->local_fp_str, config->qos_hint);
        for (int j = 0; j < config->dcmap_count; j++) {
            memsetAndFreeStrings(2, config->dcmap_label[j], config->dcmap_subprotocol[j]);
        }
    }
    free(configs);
    return true;
}

void appendRequestIntoDchPendingInd(unsigned int slotId, int request, int responseType,
        const void* data, int size) {
    if (request != RIL_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD) {
        mtkLogE(LOG_TAG, "vodc appendRequestIntoDchPendingInd, no need add Request %s to pending list",
                android::requestToString(request));
        return;
    }

    bool is_addNode = true;
    unsigned int slot = toRealSlot(slotId);
    RIL_ImsPendingInd *node = s_dch_pending_ind[slot];
    bool ignore_duplicate_request = false;
    if (request == RIL_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD) {
        ignore_duplicate_request = true;
    }
    while (node != NULL) {
        if (ignore_duplicate_request && node->request == request) {
            mtkLogD(LOG_TAG,
                "vodc appendRequestIntoDchPendingInd, Request %s(%d) already exist, only copy new data",
                android::requestToString(request), slot);
            if (data != NULL) {
                if (node->data != NULL) {
                    free(node->data);
                }
                node->data = (void*) calloc(1, size);
                if (node->data == NULL) {
                    mtkLogE(LOG_TAG, "vodc appendRequestIntoDchPendingInd, node->data calloc NULL.");
                } else {
                    memcpy(node->data, data, size);
                    node->data_size = size;
                }
            }
            return;
        }

        if (node->next != NULL) {
            node = node->next;
        } else {
            break;
        }
    }

    if (node == NULL) {
        node = (RIL_ImsPendingInd  *) calloc(1, sizeof(RIL_ImsPendingInd));
        if (node == NULL) {
            mtkLogE(LOG_TAG, "vodc appendRequestIntoDchPendingInd: node calloc NULL.");
        } else {
            node->slot = slotId;
            node->request = request;
            node->responseType = responseType;
            node->next = NULL;
            if (data != NULL) {
                node->data = (void*) calloc(1, size);
                if (node->data == NULL) {
                    mtkLogE(LOG_TAG, "vodc appendRequestIntoDchPendingInd: node->data calloc NULL.");
                    is_addNode = false;
                    free(node);
                } else {
                    memcpy(node->data, data, size);
                    node->data_size = size;
                }
            }
            if (is_addNode) {
                s_dch_pending_ind[slot] = node;
            }
        }
    } else {
        RIL_ImsPendingInd *item =
                (RIL_ImsPendingInd  *) calloc(1, sizeof(RIL_ImsPendingInd));
        if (item == NULL) {
            mtkLogE(LOG_TAG, "vodc appendRequestIntoDchPendingInd: item calloc NULL.");
        } else {
            item->slot = slotId;
            item->request = request;
            item->responseType = responseType;
            item->next = NULL;
            if (data != NULL) {
                item->data = (void*) calloc(1, size);
                if (item->data == NULL) {
                    mtkLogE(LOG_TAG, "vodc item->data calloc NULL.");
                    is_addNode = false;
                    free(item);
                } else {
                    memcpy(item->data, data, size);
                    item->data_size = size;
                }
            }
            if (is_addNode) {
                node->next = item;
            }
        }
    }

    mtkLogI(LOG_TAG, "vodc appendRequestIntoDchPendingInd, Request %s(%d) append done",
                android::requestToString(request), slot);
}

bool cancelDchBootstrapCfgQueryCmd(int slotId, uint8_t callId) {
    android::RequestInfo *pRI = android::addRequestToList(-1, slotId, RIL_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP);
    if (pRI == NULL) {
        mtkLogE(LOG_TAG, "vodc cancelDchBootstrapCfgQueryCmd pRI == NULL");
        return false;
    }

    RIL_DchConfigLocal *config =
        (RIL_DchConfigLocal *) calloc(1, sizeof(RIL_DchConfigLocal));
    if (config == NULL) {
        mtkLogE(LOG_TAG, "vodc cancelDchBootstrapCfgQueryCmd Memory allocation failed");
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    config->call_id = callId;
    config->result = 1; // 1: failed
    config->cause = MAX_DC_RSP_CAUSE_CANCLED;
    mtkLogD(LOG_TAG, "vodc cancelDchBootstrapCfgQueryCmd, slotId:%d", slotId);

    CALL_ONREQUEST(RIL_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP, config, sizeof(RIL_DchConfigLocal), pRI, slotId);
    free(config);
    return true;
}

void notifyDchServiceReady(unsigned int slotId) {
    unsigned int slot = toRealSlot(slotId);
    RIL_ImsPendingInd *node = s_dch_pending_ind[slot];
    RIL_ImsPendingInd *pre_node = NULL;
    while (node != NULL) {
        // Fire pending indication
        mtkLogI(LOG_TAG, "vodc notifyDchServiceReady [%d]Fire Pending indication %s(%d)",
                slot, android::requestToString(node->request), node->slot);
        switch (node->request) {
            case RIL_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD:
                mtkRadioExDch::dcBootstrapCfgQueryCmd(
                        node->slot, node->responseType, 0, RIL_E_SUCCESS, node->data,
                        node->data_size);
                break;
            default:
                break;
        }
        pre_node = node;
        node = node->next;
        if (pre_node->data != NULL) {
            free(pre_node->data);
        }
        free(pre_node);
    }
    s_dch_pending_ind[slot] = NULL;
}

// MTK solution start @{
bool dispatchDchXmlDataReq(int serial, int slotId, int request,
        int32_t callId, const std::string& xmlStrData) {
    android::RequestInfo *pRI = android::addRequestToList(serial, slotId, request);
    if (pRI == NULL) {
        return false;
    }

    uint16_t len = 0;

    len = strlen(xmlStrData.c_str());
    if (len > MAX_DATA_CHANNEL_XML_DATA_LEN) {
        mtkLogE(LOG_TAG, "dispatchDchXmlDataReq, vodc callId%d, xmlData len:%d too large, request %s",
                callId, len, android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_INVALID_ARGUMENTS);
        return false;
    } else {
        mtkLogD(LOG_TAG, "dispatchDchXmlDataReq, vodc callId%d, xmlData len:%d, request %s",
                callId, len, android::requestToString(pRI->pCI->requestNumber));
    }

    RIL_DchXmlConfig *out =
        (RIL_DchXmlConfig *) calloc(1, sizeof(RIL_DchXmlConfig));
    if (out == NULL) {
        mtkLogE(LOG_TAG, "dispatchDchXmlDataReq, vodc Memory allocation failed for request %s",
                android::requestToString(pRI->pCI->requestNumber));
        sendErrorResponse(pRI, RIL_E_NO_MEMORY);
        return false;
    }

    out->call_id = callId;
    if (!copyAidlStringToRil(&(out->xml_data), xmlStrData, pRI)) {
        free(out);
        return false;
    }

    CALL_ONREQUEST(request, out, sizeof(RIL_DchXmlConfig), pRI, slotId);
    memsetAndFreeStrings(1, out->xml_data);
    free(out);
    return true;
}

void appendDchInternalInd(unsigned int slotId, int request, int responseType,
        int callId, const void* data) {
    if (request != RIL_UNSOL_DCH_CONNECT_DATA_CHANNEL_IND
            && request != RIL_UNSOL_DCH_CREATE_DATA_CHANNEL_IND) {
        mtkLogE(LOG_TAG, "vodc Request %s don't need to append into pending ind to DCH list",
                android::requestToString(request));
        return;
    }

    bool is_addNode = true;
    unsigned int slot = toRealSlot(slotId);
    RIL_dchDataPendingInd *node = NULL;

    node = s_dch_internal_pending_ind[slot];
    while (node != NULL) {
        if (node->next != NULL) {
            node = node->next;
        } else {
            break;
        }
    }

    if (node == NULL) {
        node = (RIL_dchDataPendingInd  *) calloc(1, sizeof(RIL_dchDataPendingInd));
        if (node == NULL) {
            mtkLogE(LOG_TAG, "vodc appendDchInternalInd: node calloc NULL.");
        } else {
            node->slot = slotId;
            node->request = request;
            node->responseType = responseType;
            node->callId = callId;
            node->next = NULL;
            if (data != NULL) {
                int dataSize = strlen((const char*)data);
                node->data = (char*) calloc(1, dataSize);
                if (node->data == NULL) {
                    mtkLogE(LOG_TAG, "vodc appendDchInternalInd: node->data calloc NULL.");
                    is_addNode = false;
                    free(node);
                } else {
                    memcpy(node->data, data, dataSize);
                    node->data_size = dataSize;
                }
            }
            if (is_addNode) {
                s_dch_internal_pending_ind[slot] = node;
            }
        }
    } else {
        RIL_dchDataPendingInd *item =
                (RIL_dchDataPendingInd  *) calloc(1, sizeof(RIL_dchDataPendingInd));
        if (item == NULL) {
            mtkLogE(LOG_TAG, "vodc appendDchInternalInd: item calloc NULL.");
        } else {
            item->slot = slotId;
            item->request = request;
            item->responseType = responseType;
            node->callId = callId;
            item->next = NULL;
            if (data != NULL) {
                int dataSize = strlen((const char*)data);
                item->data = (char*) calloc(1, dataSize);
                if (item->data == NULL) {
                    mtkLogE(LOG_TAG, "vodc item->data calloc NULL.");
                    is_addNode = false;
                    free(item);
                } else {
                    memcpy(item->data, data, dataSize);
                    item->data_size = dataSize;
                }
            }
            if (is_addNode) {
                node->next = item;
            }
        }
    }

    mtkLogI(LOG_TAG, "vodc Request %s(%d) already append into DCH pending ind list",
                android::requestToString(request), slot);
}

void notifyDchInternalServiceReady(unsigned int slotId) {
    unsigned int slot = toRealSlot(slotId);
    RIL_dchDataPendingInd *node = s_dch_internal_pending_ind[slot];
    RIL_dchDataPendingInd *pre_node = NULL;
    while (node != NULL) {
        // Fire pending indication
        mtkLogI(LOG_TAG, "vodc notifyDchInternalServiceReady [%d]Fire Pending indication %s(%d)",
                slot, android::requestToString(node->request), node->slot);
        switch (node->request) {
            case RIL_UNSOL_DCH_CONNECT_DATA_CHANNEL_IND: {
                    RIL_DchXmlConfig pIn;
                    pIn.call_id = node->callId;
                    pIn.xml_data = node->data;
                    mtkRadioExDch::dchConnectDataChannelInd(
                            node->slot, node->responseType, 0, RIL_E_SUCCESS, &pIn,
                            sizeof(RIL_DchXmlConfig));
                }
                break;
            case RIL_UNSOL_DCH_CREATE_DATA_CHANNEL_IND: {
                    RIL_DchXmlConfig pIn;
                    pIn.call_id = node->callId;
                    pIn.xml_data = node->data;
                    mtkRadioExDch::dchCreateDataChannelInd(
                            node->slot, node->responseType, 0, RIL_E_SUCCESS, &pIn,
                            sizeof(RIL_DchXmlConfig));
                }
                break;
            default:
                break;
        }
        pre_node = node;
        node = node->next;
        if (pre_node->data != NULL) {
            free(pre_node->data);
        }
        free(pre_node);
    }
    s_dch_internal_pending_ind[slot] = NULL;
}
// MTK solution end }@

int mtkRadioExDch::dcBootstrapCfgQueryCmdRspCnf(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dcBootstrapCfgQueryCmdRspCnf[%d]: serial %d", slotId, serial);
    if (serial == -1) {
        return 0;
    }

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchResponse->
                dcBootstrapCfgQueryCmdRspCnf(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dcBootstrapCfgQueryCmdRspCnf: radioDchServiceImpl[%d].mtkDchResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dcCreateSipResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dcCreateSipResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "vodc dcCreateSipResponse: Invalid responseLen %d", responseLen);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }  else {
            int32_t *pInt = (int32_t *) response;
            data.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }
        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchResponse->
                dcCreateSipResponse(responseInfo, data);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dcCreateSipResponse: radioDchServiceImpl[%d].mtkDchResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dcModifySipResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "vodc dcModifySipResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> data;
        int numInts = responselen / sizeof(int);
        if (response == NULL || responselen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "vodc dcModifySipResponse: Invalid responselen %d", responselen);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }  else {
            int32_t *pInt = (int32_t *) response;
            data.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchResponse->
                dcModifySipResponse(responseInfo, data);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dcModifySipResponse: radioDchServiceImpl[%d].mtkDchResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dcCloseSipResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dcCloseSipResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);

        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "vodc dcCloseSipResponse: Invalid responseLen:%d", responseLen);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }  else {
            int32_t *pInt = (int32_t *) response;
            data.resize(numInts);
            for (int i = 0; i < numInts; i++) {
                data[i] = (int32_t) pInt[i];
            }
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchResponse->
                dcCloseSipResponse(responseInfo, data);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dcCloseSipResponse: radioDchServiceImpl[%d].mtkDchResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dcCreateCmdRspCnf(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dcCreateCmdRspCnf[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchResponse!= NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchResponse->
                dcCreateCmdRspCnf(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dcCreateCmdRspCnf: radioDchServiceImpl[%d].mtkDchResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dcEnableRsp(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dcEnableRsp[%d]: serial %d", slotId, serial);
    // dummy API
    return 0;
}

int mtkRadioExDch::dcBootstrapCfgQueryCmd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (response == NULL || responseLen < (sizeof(int))) {
        mtkLogE(LOG_TAG, "vodc dcBootstrapCfgQueryCmd: responseLen:%d", responseLen);
        return 0;
    }

    int callId = ((int *) response)[0];

    if (radioDchServiceImpl[slotId].mtkDchIndication != NULL) {
        int opCode = ((int *) response)[1];
        mtkLogD(LOG_TAG, "vodc dcBootstrapCfgQueryCmd[%d]: callId:%d, opCode:%d",
            slotId, callId, opCode);

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchIndication->
                                 dcBootstrapCfgQueryCmd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 (uint8_t)callId, (int)opCode);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!, rsp to md", __FUNCTION__, slotId);
            // appendRequestIntoDchPendingInd(slotId, RIL_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD,
            //        indicationType, response, responseLen);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_EXT);
            cancelDchBootstrapCfgQueryCmd(slotId + android::CLIENT_DCH * MAX_SIM_COUNT, (uint8_t)callId);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dcBootstrapCfgQueryCmd: radioDchServiceImpl[%d].mtkDchIndication == NULL, mIsBindFail:%d",
            slotId, mIsBindFail[slotId]);
        if (mIsBindFail[slotId]) {
            cancelDchBootstrapCfgQueryCmd(slotId + android::CLIENT_DCH * MAX_SIM_COUNT, (uint8_t)callId);
        } else {
            appendRequestIntoDchPendingInd(slotId, RIL_UNSOL_DCH_BOOTSTRAP_CFG_QRY_CMD, indicationType, response,
                    responseLen);
        }
    }

    return 0;
}

int mtkRadioExDch::dcConfigInitInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (radioDchServiceImpl[slotId].mtkDchIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(RIL_DchImsDataConfig))) {
            mtkLogE(LOG_TAG, "vodc dcConfigInitInd: invalid responseLen:%d", responseLen);
            return 0;
        }

        RIL_DchImsDataConfig *pIn = (RIL_DchImsDataConfig *)response;
        std::vector<aidl_mtk_radio_dch::DcConfig> pOut;
        pOut.resize(pIn->dc_config_num);
        uint32_t call_id = pIn->call_id;
        std::string if_name = convertCharPtrToStdString(pIn->if_name);

        mtkLogD(LOG_TAG, "vodc dcConfigInitInd[%d]: dc_config_num:%d", slotId, pIn->dc_config_num);

        for (int i = 0; i < pIn->dc_config_num; i++) {
            RIL_DchConfig* configIn = &(pIn->dc_config_list[i]);
            aidl_mtk_radio_dch::DcConfig* configOut = &(pOut[i]);

            configOut->index = configIn->index;
            configOut->dc_type = configIn->dc_type;
            configOut->ip_addr_type = configIn->ip_addr_type;
            configOut->remote_udp_port = configIn->remote_udp_port;
            configOut->remote_sctp_port = configIn->remote_sctp_port;
            configOut->local_udp_port = configIn->local_udp_port;
            configOut->local_sctp_port = configIn->local_sctp_port;

            configOut->remote_addr.resize(MAX_DC_IP_ADDR_LENGTH);
            configOut->local_addr.resize(MAX_DC_IP_ADDR_LENGTH);
            for (int x = 0; x < MAX_DC_IP_ADDR_LENGTH; x++) {
                configOut->remote_addr[x] = configIn->remote_addr[x];
                configOut->local_addr[x] = configIn->local_addr[x];
            }

            configOut->remote_dtls_setup = configIn->remote_dtls_setup;
            configOut->local_dtls_setup = configIn->local_dtls_setup;
            configOut->remote_tls_id = convertCharPtrToStdString(configIn->remote_tls_id);
            configOut->local_tls_id = convertCharPtrToStdString(configIn->local_tls_id);
            configOut->fp_hash_func = configIn->fp_hash_func;
            configOut->remote_fp_str = convertCharPtrToStdString(configIn->remote_fp_str);
            configOut->local_fp_str = convertCharPtrToStdString(configIn->local_fp_str);
            configOut->remote_max_message_size = configIn->remote_max_message_size;
            configOut->local_max_message_size = configIn->local_max_message_size;
            configOut->data_b_as = configIn->data_b_as;
            configOut->qos_hint = convertCharPtrToStdString(configIn->qos_hint);
            // RIL_DcMapStruct dcmap;  // RFC 8864
            configOut->dcmap_count = configIn->dcmap_count;
            if (configOut->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
                mtkLogE(LOG_TAG, "vodc dcConfigInitInd: invalid dcmap_count, %d", configOut->dcmap_count);
                configOut->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
            }
            configOut->dcmap.resize(configOut->dcmap_count);
            for (int j = 0; j < configOut->dcmap_count; j++) {
                configOut->dcmap[j].dcmap_stream_id = configIn->dcmap_stream_id[j];
                configOut->dcmap[j].priority = configIn->dcmap_priority[j];
                configOut->dcmap[j].ordered = configIn->dcmap_ordered[j];
                configOut->dcmap[j].max_retr =  configIn->dcmap_max_retr[j];
                configOut->dcmap[j].max_time = configIn->dcmap_max_time[j];
                configOut->dcmap[j].label = convertCharPtrToStdString(configIn->dcmap_label[j]);
                configOut->dcmap[j].subprotocol = convertCharPtrToStdString(configIn->dcmap_subprotocol[j]);
            }
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchIndication->
                                 dcConfigInitInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 (uint8_t)call_id, if_name, pOut);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dcConfigInitInd: radioDchServiceImpl[%d].mtkDchIndication == NULL",
            slotId);
    }

    return 0;
}

int mtkRadioExDch::dcConfigUpdateInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (radioDchServiceImpl[slotId].mtkDchIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(RIL_DchImsDataConfig))) {
            mtkLogE(LOG_TAG, "vodc dcConfigUpdateInd: invalid responseLen:%d", responseLen);
            return 0;
        }

        RIL_DchImsDataConfig *pIn = (RIL_DchImsDataConfig *)response;
        std::vector<aidl_mtk_radio_dch::DcConfig> pOut;
        pOut.resize(pIn->dc_config_num);
        uint32_t call_id = pIn->call_id;
        std::string if_name = convertCharPtrToStdString(pIn->if_name);

        mtkLogD(LOG_TAG, "vodc dcConfigUpdateInd[%d]: dc_config_num:%d", slotId, pIn->dc_config_num);
        for (int i = 0; i < pIn->dc_config_num; i++) {
            RIL_DchConfig* configIn = &(pIn->dc_config_list[i]);
            aidl_mtk_radio_dch::DcConfig* configOut = &(pOut[i]);

            configOut->index = configIn->index;
            configOut->dc_type = configIn->dc_type;
            configOut->ip_addr_type = configIn->ip_addr_type;
            configOut->remote_udp_port = configIn->remote_udp_port;
            configOut->remote_sctp_port = configIn->remote_sctp_port;
            configOut->local_udp_port = configIn->local_udp_port;
            configOut->local_sctp_port = configIn->local_sctp_port;

            configOut->remote_addr.resize(MAX_DC_IP_ADDR_LENGTH);
            configOut->local_addr.resize(MAX_DC_IP_ADDR_LENGTH);
            for (int x = 0; x < MAX_DC_IP_ADDR_LENGTH; x++) {
                configOut->remote_addr[x] = configIn->remote_addr[x];
                configOut->local_addr[x] = configIn->local_addr[x];
            }

            configOut->remote_dtls_setup = configIn->remote_dtls_setup;
            configOut->local_dtls_setup = configIn->local_dtls_setup;
            configOut->remote_tls_id = convertCharPtrToStdString(configIn->remote_tls_id);
            configOut->local_tls_id = convertCharPtrToStdString(configIn->local_tls_id);
            configOut->fp_hash_func = configIn->fp_hash_func;
            configOut->remote_fp_str = convertCharPtrToStdString(configIn->remote_fp_str);
            configOut->local_fp_str = convertCharPtrToStdString(configIn->local_fp_str);
            configOut->remote_max_message_size = configIn->remote_max_message_size;
            configOut->local_max_message_size = configIn->local_max_message_size;
            configOut->data_b_as = configIn->data_b_as;
            configOut->qos_hint = convertCharPtrToStdString(configIn->qos_hint);
            // RIL_DcMapStruct dcmap;  //RFC 8864
            configOut->dcmap_count = configIn->dcmap_count;
            if (configOut->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
                mtkLogE(LOG_TAG, "vodc dcConfigUpdateInd: invalid dcmap_count, %d", configOut->dcmap_count);
                configOut->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
            }
            configOut->dcmap.resize(configOut->dcmap_count);
            for (int j = 0; j < configOut->dcmap_count; j++) {
                configOut->dcmap[j].dcmap_stream_id = configIn->dcmap_stream_id[j];
                configOut->dcmap[j].priority = configIn->dcmap_priority[j];
                configOut->dcmap[j].ordered = configIn->dcmap_ordered[j];
                configOut->dcmap[j].max_retr =  configIn->dcmap_max_retr[j];
                configOut->dcmap[j].max_time = configIn->dcmap_max_time[j];
                configOut->dcmap[j].label = convertCharPtrToStdString(configIn->dcmap_label[j]);
                configOut->dcmap[j].subprotocol = convertCharPtrToStdString(configIn->dcmap_subprotocol[j]);
            }
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchIndication->
                                 dcConfigUpdateInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 (uint8_t)call_id, if_name, pOut);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_EXT);
        } else {
            mtkLogD(LOG_TAG, "vodc dcConfigUpdateInd[%d]: end", slotId);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dcConfigUpdateInd: radioDchServiceImpl[%d].mtkDchIndication == NULL",
            slotId);
    }

    return 0;
}

int mtkRadioExDch::dcConfigDeinitInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (radioDchServiceImpl[slotId].mtkDchIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(int))) {
            mtkLogE(LOG_TAG, "vodc dcConfigDeinitInd: invalid responseLen:%d", responseLen);
            return 0;
        }

        int callId = ((int *) response)[0];
        mtkLogD(LOG_TAG, "vodc dcConfigDeinitInd[%d]: indicationType:%d, callId:%d",
                slotId, indicationType, callId);

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchIndication->
                                 dcConfigDeinitInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 (uint8_t)callId);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dcConfigDeinitInd: radioDchServiceImpl[%d].mtkDchIndication == NULL",
            slotId);
    }

    return 0;
}

int mtkRadioExDch::dcCreateCmd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {

    if (radioDchServiceImpl[slotId].mtkDchIndication != NULL) {
        if (response == NULL || responseLen < (sizeof(RIL_DchImsDataConfig))) {
            mtkLogE(LOG_TAG, "vodc dcConfigUpdateInd: invalid responseLen:%d", responseLen);
            return 0;
        }

        RIL_DchImsDataConfig *pIn = (RIL_DchImsDataConfig *)response;
        std::vector<aidl_mtk_radio_dch::DcConfig> pOut;
        pOut.resize(pIn->dc_config_num);
        uint32_t call_id = pIn->call_id;

        mtkLogD(LOG_TAG, "vodc dcCreateCmd[%d]: dc_config_num:%d", slotId, pIn->dc_config_num);

        for (int i = 0; i < pIn->dc_config_num; i++) {
            RIL_DchConfig* configIn = &(pIn->dc_config_list[i]);
            aidl_mtk_radio_dch::DcConfig* configOut = &(pOut[i]);

            configOut->index = configIn->index;
            configOut->dc_type = configIn->dc_type;
            configOut->ip_addr_type = configIn->ip_addr_type;
            configOut->remote_udp_port = configIn->remote_udp_port;
            configOut->remote_sctp_port = configIn->remote_sctp_port;
            configOut->local_udp_port = configIn->local_udp_port;
            configOut->local_sctp_port = configIn->local_sctp_port;

            configOut->remote_addr.resize(MAX_DC_IP_ADDR_LENGTH);
            configOut->local_addr.resize(MAX_DC_IP_ADDR_LENGTH);
            for (int x = 0; x < MAX_DC_IP_ADDR_LENGTH; x++) {
                configOut->remote_addr[x] = configIn->remote_addr[x];
                configOut->local_addr[x] = configIn->local_addr[x];
            }

            configOut->remote_dtls_setup = configIn->remote_dtls_setup;
            configOut->local_dtls_setup = configIn->local_dtls_setup;
            configOut->remote_tls_id = convertCharPtrToStdString(configIn->remote_tls_id);
            configOut->local_tls_id = convertCharPtrToStdString(configIn->local_tls_id);
            configOut->fp_hash_func = configIn->fp_hash_func;
            configOut->remote_fp_str = convertCharPtrToStdString(configIn->remote_fp_str);
            configOut->local_fp_str = convertCharPtrToStdString(configIn->local_fp_str);
            configOut->remote_max_message_size = configIn->remote_max_message_size;
            configOut->local_max_message_size = configIn->local_max_message_size;
            configOut->data_b_as = configIn->data_b_as;
            configOut->qos_hint = convertCharPtrToStdString(configIn->qos_hint);
            // RIL_DcMapStruct dcmap;  // RFC 8864
            configOut->dcmap_count = configIn->dcmap_count;
            if (configOut->dcmap_count > MAX_DCMAP_NUM_PER_CONFIG) {
                mtkLogE(LOG_TAG, "vodc dcConfigUpdateInd: invalid dcmap_count, %d", configOut->dcmap_count);
                configOut->dcmap_count = MAX_DCMAP_NUM_PER_CONFIG;
            }
            configOut->dcmap.resize(configOut->dcmap_count);
            for (int j = 0; j < configOut->dcmap_count; j++) {
                configOut->dcmap[j].dcmap_stream_id = configIn->dcmap_stream_id[j];
                configOut->dcmap[j].priority = configIn->dcmap_priority[j];
                configOut->dcmap[j].ordered = configIn->dcmap_ordered[j];
                configOut->dcmap[j].max_retr =  configIn->dcmap_max_retr[j];
                configOut->dcmap[j].max_time = configIn->dcmap_max_time[j];
                configOut->dcmap[j].label = convertCharPtrToStdString(configIn->dcmap_label[j]);
                configOut->dcmap[j].subprotocol = convertCharPtrToStdString(configIn->dcmap_subprotocol[j]);
            }
        }

        ndk::ScopedAStatus ret =  radioDchServiceImpl[slotId].mtkDchIndication->
                                dcCreateCmd(
                                rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                (uint8_t)call_id, pOut);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_EXT);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dcCreateCmd: radioDchServiceImpl[%d].mtkDchIndication == NULL", slotId);
    }

    return 0;
}

int mtkRadioExDch::clearMtkDchResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioDchServiceImpl[realSlotId].mtkDchResponse = NULL;
    radioDchServiceImpl[realSlotId].mtkDchIndication = NULL;
    mIsBindFail[realSlotId] = true;
    return 0;
}

// MTK solution start @{
int mtkRadioExDch::clearMtkDchInternalResponseAndIndications(unsigned int slotId) {
    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    radioDchServiceImpl[realSlotId].mtkDchInternalResponse = NULL;
    radioDchServiceImpl[realSlotId].mtkDchInternalIndication = NULL;
    return 0;
}

int mtkRadioExDch::dchDataActPacketRouteResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responselen) {
    mtkLogD(LOG_TAG, "vodc dchDataActPacketRouteResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchInternalResponse!= NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchInternalResponse->
                dchDataActPacketRouteResponse(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dchDataActPacketRouteResponse: radioDchServiceImpl[%d].mtkDchInternalResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dchDataIfupPacketRouteResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dchDataIfupPacketRouteResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchInternalResponse!= NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchInternalResponse->
                dchDataIfupPacketRouteResponse(responseInfo);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dchDataIfupPacketRouteResponse: radioDchServiceImpl[%d].mtkDchInternalResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dchDataPathConfigSyncResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dchDataPathConfigSyncResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchInternalResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        std::vector<uint8_t> clientAddr;
        std::vector<uint8_t> serverAddr;
        int dcMss = 0;
        if (response == NULL || responseLen != sizeof(RIL_DchDataPathConfig)) {
            mtkLogE(LOG_TAG, "vodc dchDataPathConfigSyncResponse: Invalid responseLen:%d", responseLen);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }  else {
            RIL_DchDataPathConfig *pIn = (RIL_DchDataPathConfig *)response;
            clientAddr.resize(MAX_DC_IP_ADDR_LENGTH);
            serverAddr.resize(MAX_DC_IP_ADDR_LENGTH);
            for (int i = 0; i < MAX_DC_IP_ADDR_LENGTH; i++) {
                clientAddr[i] = pIn->client_ip[i];
                serverAddr[i] = pIn->server_ip[i];
            }
            dcMss = pIn->dc_mss;
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchInternalResponse->
                dchDataPathConfigSyncResponse(responseInfo, clientAddr, serverAddr, dcMss);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dchDataPathConfigSyncResponse: radioDchServiceImpl[%d].mtkDchInternalResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dchCreateDataChannelResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dchCreateDataChannelResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchInternalResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int callId = 0;
        std::string xmlData;
        if (response == NULL || responseLen != sizeof(RIL_DchXmlConfig)) {
            mtkLogE(LOG_TAG, "vodc dchCreateDataChannelResponse: Invalid responseLen:%d", responseLen);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }  else {
            RIL_DchXmlConfig *pIn = (RIL_DchXmlConfig *) response;
            xmlData = convertCharPtrToStdString(pIn->xml_data);
            callId = pIn->call_id;
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchInternalResponse->
                dchCreateDataChannelResponse(responseInfo, callId, xmlData);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dchCreateDataChannelResponse: radioDchServiceImpl[%d].mtkDchInternalResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dchCloseDataChannelResponse(unsigned int slotId, android::ClientId clientId,
        int responseType, int serial, RIL_Errno e, const void *response, size_t responseLen) {
    mtkLogD(LOG_TAG, "vodc dchCloseDataChannelResponse[%d]: serial %d", slotId, serial);

    unsigned int realSlotId = slotId % MAX_SIM_COUNT;
    if (radioDchServiceImpl[realSlotId].mtkDchInternalResponse != NULL) {
        aidl_radio::RadioResponseInfo responseInfo = {};
        rilAidlUtils::populateAidlResponseInfo(responseInfo, serial, responseType, e);
        int callId = 0;
        std::string xmlData;
        if (response == NULL || responseLen != sizeof(RIL_DchXmlConfig)) {
            mtkLogE(LOG_TAG, "vodc dchCloseDataChannelResponse: Invalid responseLen:%d", responseLen);
            if (e == RIL_E_SUCCESS) responseInfo.error = aidl_radio::RadioError::INVALID_RESPONSE;
        }  else {
            RIL_DchXmlConfig *pIn = (RIL_DchXmlConfig *) response;
            xmlData = convertCharPtrToStdString(pIn->xml_data);
            callId = pIn->call_id;
        }

        ndk::ScopedAStatus ret = radioDchServiceImpl[realSlotId].mtkDchInternalResponse->
                dchCloseDataChannelResponse(responseInfo, callId, xmlData);

        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binder fail!", __func__, realSlotId);
            retChecker->mtkAidlReturnStatusChecker(slotId, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
                "vodc dchCloseDataChannelResponse: radioDchServiceImpl[%d].mtkDchInternalResponse == NULL",
                realSlotId);
    }

    return 0;
}

int mtkRadioExDch::dchDataIfupPacketRouteInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (radioDchServiceImpl[slotId].mtkDchInternalIndication != NULL) {
        if (response == NULL || responseLen % sizeof(int) != 0) {
            mtkLogE(LOG_TAG, "vodc dchDataIfupPacketRouteInd: Invalid responseLen:%d", responseLen);
            return 0;
        }

        std::vector<int32_t> data;
        int numInts = responseLen / sizeof(int);
        int32_t *pInt = (int32_t *) response;
        data.resize(numInts);
        for (int i = 0; i < numInts; i++) {
            data[i] = pInt[i];
        }

        mtkLogD(LOG_TAG, "vodc dchDataIfupPacketRouteInd[%d]: netType:%d, nwItfId:%d, hwUploadQ:%d",
            slotId, data[0], data[1], data[2]);

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchInternalIndication->
                                 dchDataIfupPacketRouteInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 data);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dchDataIfupPacketRouteInd: radioDchServiceImpl[%d].mtkDchInternalIndication == NULL",
            slotId);

    }

    return 0;
}

int mtkRadioExDch::dchConnectDataChannelInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (response == NULL || responseLen != sizeof(RIL_DchXmlConfig)) {
        mtkLogE(LOG_TAG, "vodc dchConnectDataChannelInd: Invalid responseLen:%d", responseLen);
        return 0;
    }

    RIL_DchXmlConfig *pIn = (RIL_DchXmlConfig *) response;

    if (radioDchServiceImpl[slotId].mtkDchInternalIndication != NULL) {
        std::string xmlData = convertCharPtrToStdString(pIn->xml_data);
        int callId = pIn->call_id;
        mtkLogD(LOG_TAG, "vodc dchConnectDataChannelInd[%d]: callId:%d, xml_data:%.5s",
            slotId, callId, pIn->xml_data);

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchInternalIndication->
                                 dchConnectDataChannelInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId, xmlData);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!, pending", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_MTK);
            // appendRequestIntoDchPendingInd(slotId, RIL_UNSOL_DCH_CONNECT_DATA_CHANNEL_IND, indicationType,
            //             pIn->call_id, pIn->xml_data);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dchConnectDataChannelInd: radioDchServiceImpl[%d].mtkDchInternalIndication == NULL, pending",
            slotId);
        // appendRequestIntoDchPendingInd(slotId, RIL_UNSOL_DCH_CONNECT_DATA_CHANNEL_IND, indicationType,
        //            pIn->call_id, pIn->xml_data);
    }

    return 0;
}

int mtkRadioExDch::dchCloseDataChannelInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (radioDchServiceImpl[slotId].mtkDchInternalIndication != NULL) {
        if (response == NULL || responseLen != sizeof(RIL_DchXmlConfig)) {
            mtkLogE(LOG_TAG, "vodc dchCloseDataChannelInd: Invalid responseLen:%d", responseLen);
            return 0;
        }

        RIL_DchXmlConfig *pIn = (RIL_DchXmlConfig *) response;
        std::string xmlData = convertCharPtrToStdString(pIn->xml_data);
        int callId = pIn->call_id;

        mtkLogD(LOG_TAG, "vodc dchCloseDataChannelInd[%d]: callId:%d, xml_data:%.5s",
            slotId, callId, pIn->xml_data);

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchInternalIndication->
                                 dchCloseDataChannelInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId, xmlData);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_MTK);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dchCloseDataChannelInd: radioDchServiceImpl[%d].mtkDchInternalIndication == NULL",
            slotId);
    }

    return 0;
}

int mtkRadioExDch::dchCreateDataChannelInd(unsigned int slotId, int indicationType, int token, RIL_Errno e,
        const void *response, size_t responseLen) {
    if (response == NULL || responseLen != sizeof(RIL_DchXmlConfig)) {
        mtkLogE(LOG_TAG, "vodc dchCreateDataChannelInd: Invalid responseLen:%d", responseLen);
        return 0;
    }

    RIL_DchXmlConfig *pIn = (RIL_DchXmlConfig *) response;

    if (radioDchServiceImpl[slotId].mtkDchInternalIndication != NULL) {
        std::string xmlData = convertCharPtrToStdString(pIn->xml_data);
        int callId = pIn->call_id;

        mtkLogD(LOG_TAG, "vodc dchCreateDataChannelInd[%d]: callId:%d, xml_data:%.5s",
            slotId, callId, pIn->xml_data);

        ndk::ScopedAStatus ret = radioDchServiceImpl[slotId].mtkDchInternalIndication->
                                 dchCreateDataChannelInd(
                                 rilAidlUtils::convertIntToAidlRadioIndicationType(indicationType),
                                 callId, xmlData);
        if (!ret.isOk()) {
            mtkLogE(LOG_TAG, "vodc %s: slotId = %d, binderfail!, pending", __FUNCTION__, slotId);
            retChecker->mtkAidlReturnStatusChecker(
                slotId + android::CLIENT_DCH * MAX_SIM_COUNT, ret, android::DCH_MTK);
            // appendRequestIntoDchPendingInd(slotId, RIL_UNSOL_DCH_CREATE_DATA_CHANNEL_IND, indicationType,
            //            pIn->call_id, pIn->xml_data);
        }
    } else {
        mtkLogE(LOG_TAG,
            "vodc dchCreateDataChannelInd: radioDchServiceImpl[%d].mtkDchInternalIndication == NULL, pending",
            slotId);
        // appendRequestIntoDchPendingInd(slotId, RIL_UNSOL_DCH_CREATE_DATA_CHANNEL_IND, indicationType,
        //            pIn->call_id, pIn->xml_data);
    }

    return 0;
}
// MTK solution end }@

namespace vendor::mediatek::hardware::mtkradioex::dch {

using ::ndk::ScopedAStatus;
namespace aidl = ::aidl::vendor::mediatek::hardware::mtkradioex::dch;
constexpr auto ok = &ScopedAStatus::ok;
MtkRadioDch::MtkRadioDch(unsigned int slotId) {
    mSlotId = slotId;

    char dcSupport[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("persist.vendor.mtk_dc_call_capability", dcSupport, "0");

    int value = atoi(dcSupport);
    if (value == 1) {
        mIsDcSupport = true;
    } else if (value == 2) {
        mIsDcInternalSupport = true;
    } else if (value == 3) {
        mIsDcSupport = true;
        mIsDcInternalSupport = true;
    }

    mtkLogD(LOG_TAG, "vodc MtkRadioDch, mSlotId:%d, dcSupport:%s",
        mSlotId, dcSupport);
}

ScopedAStatus MtkRadioDch::dcBootstrapCfgQueryCmdRsp(int32_t serial, int8_t callId, int8_t result,
            const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfigLocal>& dataList,
            int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dcBootstrapCfgQueryCmdRsp: serial %d", serial);

    char dcSupport[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("persist.vendor.mtk_dc_call_capability", dcSupport, "0");

    int value = atoi(dcSupport);
    if (value == 1) {
        mIsDcSupport = true;
    } else if (value == 2) {
        mIsDcInternalSupport = true;
    } else if (value == 3) {
        mIsDcSupport = true;
        mIsDcInternalSupport = true;
    }
    mtkLogD(LOG_TAG, "vodc dcBootstrapCfgQueryCmdRsp, mSlotId:%d, dcSupport:%s",
        mSlotId, dcSupport);

    if (!checkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial,
            RIL_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP)) {
        return ok();
    }

    dispatchDchSetCapabilityConfig(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_DCH_BOOTSTRAP_CFG_QRY_CMD_RSP,
                (uint8_t)callId, (uint8_t)result, dataList);

    return ok();
}

ScopedAStatus MtkRadioDch::dcCreateSipRequest(int32_t serial, int8_t callId,
            const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList,
            int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dcCreateSipRequest: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_CREATE_SIP_REQ)) {
        return ok();
    }

    dispatchDchConfig(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_CREATE_SIP_REQ,
                (uint8_t)callId, 0, 0, dataList);

    return ok();
}

ScopedAStatus MtkRadioDch::dcModifySipRequest(int32_t serial, int8_t callId,
            const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList,
            int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dcModifySipRequest: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_MODIFY_SIP_REQ)) {
        return ok();
    }

    dispatchDchConfig(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_MODIFY_SIP_REQ,
                (uint8_t)callId, 0, 0, dataList);
    return ok();
}

ScopedAStatus MtkRadioDch::dcCloseSipRequest(int32_t serial, int8_t callId, int8_t closeAllStream,
            const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcMapDelStruct>& delList,
            int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dcCloseSipRequest: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_CLOSE_SIP_REQ)) {
        return ok();
    }

    dispatchDchCloseSipReq(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_CLOSE_SIP_REQ,
                (uint8_t)callId, (uint8_t)closeAllStream, delList);
    return ok();
}

ScopedAStatus MtkRadioDch::dcCreateCmdRsp(int32_t serial, int8_t callId, int8_t result, int32_t cause,
            const std::vector<::aidl::vendor::mediatek::hardware::mtkradioex::dch::DcConfig>& dataList,
            int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dcCreateCmdRsp: serial %d", serial);
    if (!checkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_CREATE_CMD_RSP)) {
        return ok();
    }

    dispatchDchConfig(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_CREATE_CMD_RSP,
                (uint8_t)callId, (uint8_t)result, (uint16_t)cause, dataList);
    return ok();
}

ScopedAStatus MtkRadioDch::responseAcknowledgementMtk() {
    android::releaseWakeLock();
    return ok();
}

ScopedAStatus MtkRadioDch::setResponseFunctionsDc(
        const std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcResponse>& radioResponse,
        const std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk vodc setResponseFunctionsDc, slotId:%d, mIsDcSupport:%d",
                mSlotId, mIsDcSupport);
    if (!isValidSlotId(mSlotId)) {
        return ok();
    }

    char dcSupport[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("persist.vendor.mtk_dc_call_capability", dcSupport, "0");

    int value = atoi(dcSupport);
    if (value == 1) {
        mIsDcSupport = true;
    } else if (value == 2) {
        mIsDcInternalSupport = true;
    } else if (value == 3) {
        mIsDcSupport = true;
        mIsDcInternalSupport = true;
    }

    mtkLogD(LOG_TAG, "vodc setResponseFunctionsDc, mSlotId:%d, dcSupport:%s",
        mSlotId, dcSupport);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk vodc setResponseFunctionsDc: got lock %d", mSlotId);

    radioDchServiceImpl[mSlotId].mtkDchResponse = radioResponse;
    radioDchServiceImpl[mSlotId].mtkDchIndication = radioIndication;
    mIsBindFail[mSlotId] = false;
    notifyDchServiceReady(mSlotId);
    plusCounterMtkRadioNumOne(mSlotId + android::CLIENT_DCH * MAX_SIM_COUNT);

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk vodc setResponseFunctionsDc: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    android::onNewCommandConnect((RIL_SOCKET_ID) (mSlotId + android::CLIENT_DCH * MAX_SIM_COUNT), android::DCH_EXT);

    return ok();
}

// MTK solution start @{
ScopedAStatus MtkRadioDch::setResponseFunctionsDchInternal(
        const std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcResponse>& radioResponse,
        const std::shared_ptr<aidl_mtk_radio_dch::IMtkRadioExDcIndication>& radioIndication) {
    mtkLogD(LOG_TAG, "mtk vodc setResponseFunctionsDchInternal, slotId:%d, mIsDcSupport:%d",
                mSlotId, mIsDcSupport);
    if (!isValidSlotId(mSlotId)) {
        return ok();
    }

    char dcSupport[MTK_PROPERTY_VALUE_MAX] = { 0 };
    mtk_property_get("persist.vendor.mtk_dc_call_capability", dcSupport, "0");

    int value = atoi(dcSupport);
    if (value == 1) {
        mIsDcSupport = true;
    } else if (value == 2) {
        mIsDcInternalSupport = true;
    } else if (value == 3) {
        mIsDcSupport = true;
        mIsDcInternalSupport = true;
    }

    mtkLogD(LOG_TAG, "vodc setResponseFunctionsDchInternal, mSlotId:%d, dcSupport:%s",
        mSlotId, dcSupport);

    pthread_rwlock_t *radioServiceRwlockPtr = radio::getRadioServiceRwlock(mSlotId);
    ::radio::lockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk vodc setResponseFunctionsDchInternal: got lock %d", mSlotId);

    radioDchServiceImpl[mSlotId].mtkDchInternalResponse = radioResponse;
    radioDchServiceImpl[mSlotId].mtkDchInternalIndication = radioIndication;
    notifyDchInternalServiceReady(mSlotId);
    plusCounterMtkRadioNumOne(mSlotId + android::CLIENT_DCH * MAX_SIM_COUNT);

    ::radio::unlockRadioServiceWlock(radioServiceRwlockPtr, (int)mSlotId);
    mtkLogV(LOG_TAG, "mtk vodc setResponseFunctionsDchInternal: release lock %d", mSlotId);
    // client is connected. Send initial indications.
    android::onNewCommandConnect((RIL_SOCKET_ID) (mSlotId + android::CLIENT_DCH * MAX_SIM_COUNT), android::DCH_MTK);

    return ok();
}

ScopedAStatus MtkRadioDch::dchDataActPacketRouteRequest(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dchDataActPacketRouteRequest: serial %d", serial);
    if (!checkMtkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_ACT_PACKET_ROUTE_REQ)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_ACT_PACKET_ROUTE_REQ);

    return ok();
}

ScopedAStatus MtkRadioDch::dchDataIfupPacketRouteRequest(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dchDataIfupPacketRouteRequest: serial %d", serial);
    if (!checkMtkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_IFUP_PACKET_ROUTE_REQ)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_IFUP_PACKET_ROUTE_REQ);

    return ok();
}

ScopedAStatus MtkRadioDch::dchDataPathConfigSyncRequest(int32_t serial, int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dchDataPathConfigSyncRequest: serial %d", serial);
    if (!checkMtkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_DATA_PATH_CONFIG_SYNC_REQ)) {
        return ok();
    }

    dispatchVoid(serial, mSlotId + clientId * MAX_SIM_COUNT, RIL_REQUEST_DCH_DATA_PATH_CONFIG_SYNC_REQ);

    return ok();
}

ScopedAStatus MtkRadioDch::dchCreateDataChannelRequest(int32_t serial, int32_t callId,
            const std::string& xmlStrData, int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dchCreateDataChannelRequest: serial %d", serial);
    if (!checkMtkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_CREATE_DATA_CHANNEL_REQ)) {
        return ok();
    }

    dispatchDchXmlDataReq(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_DCH_CREATE_DATA_CHANNEL_REQ, callId, xmlStrData);
    return ok();
}

ScopedAStatus MtkRadioDch::dchCloseDataChannelRequest(int32_t serial, int32_t callId,
            const std::string& xmlStrData, int32_t clientId) {
    mtkLogD(LOG_TAG, "vodc dchCloseDataChannelRequest: serial %d", serial);
    if (!checkMtkRequestClientIdSupport(clientId, android::CLIENT_DCH, serial, RIL_REQUEST_DCH_CLOSE_DATA_CHANNEL_REQ)) {
        return ok();
    }

    dispatchDchXmlDataReq(serial, mSlotId + clientId * MAX_SIM_COUNT,
                RIL_REQUEST_DCH_CLOSE_DATA_CHANNEL_REQ, callId, xmlStrData);

    return ok();
}
// MTK solution end }@

}

static std::vector<std::shared_ptr<ndk::ICInterface>> gPublishedHals;
static std::mutex  gMutex;

void addDchLock(std::shared_ptr<vendor::mediatek::hardware::mtkradioex::dch::MtkRadioDch> radioAidl) {
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
  std::lock_guard<std::mutex> lock(gMutex);
#endif
  gPublishedHals.push_back(radioAidl); // need or no need
}

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
#define VMADDR_CID_ANY (-1U)
#define RADIO_DCH_PORT_BASE 59000
static pthread_t gThreadIds[MAX_SIM_COUNT];

void* addRcpVsockRadioDchAidlService(void* arg) {
    unsigned int sloid = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(arg));
    const char* kAnyInetAddress = "0.0.0.0";
    auto rpcServer = android::RpcServer::make();
    mtkLogI(LOG_TAG, "setupVsockServer sloid:%d", sloid);
    android::status_t status = rpcServer->setupVsockServer(VMADDR_CID_ANY, RADIO_DCH_PORT_BASE + sloid);
    if(status != android::OK) {
        mtkLogE(LOG_TAG, "setupVsockServer fail sloid:%d", sloid);
        return nullptr;
    }
    mtkLogI(LOG_TAG, "VSOCK listen port: %u sloid:%d", RADIO_DCH_PORT_BASE + sloid, sloid);

    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::dch::MtkRadioDch> mtkRadioDchAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::dch::MtkRadioDch>(sloid);
    addDchLock(mtkRadioDchAidl);
    mtkLogI(LOG_TAG, "VSOCK setRootObject sloid:%d", sloid);
    rpcServer->setRootObject(AIBinder_toPlatformBinder(mtkRadioDchAidl->asBinder().get())); //Convert ABinder to IBinder
    mtkLogI(LOG_TAG, "VSOCK setRootObject ok to join sloid:%d", sloid);
    rpcServer->join();
    mtkLogI(LOG_TAG, "VSOCK setRootObject join exit sloid:%d", sloid);
    return nullptr;
}
#endif

void publicRadioDchAidlService(unsigned int slotId) {
    if (slotId >= MAX_SIM_COUNT) {
        mtkLogE(LOG_TAG, "vodc publicRadioDchAidlService, error slotId:%d", slotId);
        return;
    }
    /*const char *mtkRadioDchDescriptor = vendor::mediatek::hardware::mtkradioex::dch::MtkRadioDch::descriptor;
    std::shared_ptr<vendor::mediatek::hardware::mtkradioex::dch::MtkRadioDch> mtkRadioDchAidl =
            ndk::SharedRefBase::make<vendor::mediatek::hardware::mtkradioex::dch::MtkRadioDch>(slotId);
    gPublishedHals.push_back(mtkRadioDchAidl);
    const std::string mtkInstance = std::string(mtkRadioDchDescriptor != NULL ? mtkRadioDchDescriptor : "")
            + "/slot" + std::to_string(slotId + 1);
    binder_status_t mtkStatus =
            AServiceManager_addService(mtkRadioDchAidl->asBinder().get(), mtkInstance.c_str());
    mtkLogD(LOG_TAG, "mtk vodc addservice, slotId:%d, status:%d", slotId, mtkStatus);*/

#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    // add service for android client
    if (pthread_create(&gThreadIds[slotId], NULL,
            addRcpVsockRadioDchAidlService, reinterpret_cast<void*>(static_cast<unsigned int>(slotId))) != 0) {
        mtkLogE(LOG_TAG, "failed to create thread for VsockRadiodch");
    }
#endif
}

