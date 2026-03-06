/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2021. All rights reserved.
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

#include "RmmEccNumberRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxMipcData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "mipc_msg_tlv_api.h"
#include "mipc_msg_tlv_const.h"
#include "RfxEmergencyNumberListData.h"
#include "EccNumberSource.h"

#define RFX_LOG_TAG "RmmEccNumberReqHdlr"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmEccNumberRequestHandler, RIL_CMD_PROXY_2);

RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_MD_ECC_CHANGE);

RmmEccNumberRequestHandler::RmmEccNumberRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int request[] = {
        RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD,
        RFX_MSG_REQUEST_LOCAL_GET_ECC_NUM_FROM_MD
    };
    logD(RFX_LOG_TAG, "init slotId %d, channel_id %d", slot_id, channel_id);
    registerToHandleRequest(request, sizeof(request) / sizeof(int));

    const int eventList[] = {
        RFX_MSG_EVENT_MD_ECC_CHANGE,
    };

    registerToHandleEvent(eventList, sizeof(eventList) / sizeof(int));
}

RmmEccNumberRequestHandler::~RmmEccNumberRequestHandler() {
}

void RmmEccNumberRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    mipc_result_const_enum ret = mipc_result_const_NONE;
    switch (request) {
        case RFX_MSG_REQUEST_LOCAL_SYNC_ECC_NUM_TO_MD: {
            // Add MIPC ECC count
            if (msg->getData()->getDataLength() > 0) {
                int eccCount = msg->getData()->getDataLength() / sizeof(RIL_EmergencyNumber);
                sp<RfxMipcData> eccMipc = RfxMipcData::obtainMipcData(MIPC_CALL_SET_ECC_LIST_REQ, msg->getSlotId());
                eccMipc->addMipcTlvUint32(MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_COUNT, eccCount);
                logI(RFX_LOG_TAG, "[%s] AP ECC count: %d", __FUNCTION__, eccCount);

                // Add MIPC ECC list
                RIL_EmergencyNumber *pEccList = (RIL_EmergencyNumber*)(msg->getData()->getData());
                int mipcEccSize = sizeof(mipc_ecc_info_struct4) * eccCount;
                mipc_ecc_info_struct4 *mipcEccList = (mipc_ecc_info_struct4*)calloc(1, mipcEccSize);
                RFX_ASSERT(mipcEccList != NULL);
                for (int i = 0; i < eccCount; i++) {
                    mipcEccList[i].is_fake_ecc = pEccList[i].isFake;
                    mipcEccList[i].category = (uint16_t)(pEccList[i].categories);
                    mipcEccList[i].type = MIPC_ECC_INFO_TYPE_HOST;
                    strncpy(mipcEccList[i].number, pEccList[i].number, MIPC_MAX_CALL_ECC_NUMBER_LEN);
                    logD(RFX_LOG_TAG, "[%s] AP ECC[%d] {%s,%d,%d}", __FUNCTION__, i,
                            mipcEccList[i].number, mipcEccList[i].category, mipcEccList[i].is_fake_ecc);
                }
                eccMipc->addMipcTlv(MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_LIST, mipcEccSize, mipcEccList);
                sp<RfxMipcData> mipcCnf = callToMipcMsgSync(eccMipc);
                ret = (mipc_result_const_enum) mipcCnf->getResult();
                FREEIF(mipcEccList);
            } else {
                // clear ECC list
                sp<RfxMipcData> eccMipc = RfxMipcData::obtainMipcData(MIPC_CALL_SET_ECC_LIST_REQ, msg->getSlotId());
                eccMipc->addMipcTlvUint32(MIPC_CALL_SET_ECC_LIST_REQ_T_INFO_COUNT, 0);
                sp<RfxMipcData> mipcCnf = callToMipcMsgSync(eccMipc);
                ret = (mipc_result_const_enum) mipcCnf->getResult();
                logD(RFX_LOG_TAG, "[%s] Clear AP ECC list", __FUNCTION__);
            }

            sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                    ret == MIPC_RESULT_SUCCESS ?
                    RIL_E_SUCCESS : RIL_E_MODEM_ERR,
                    RfxVoidData(), msg);
            responseToTelCore(response);
            break;
        }
        case RFX_MSG_REQUEST_LOCAL_GET_ECC_NUM_FROM_MD: {
            // Tigger modem update ECC list to framework
            sendEvent(RFX_MSG_EVENT_MD_ECC_CHANGE, RfxVoidData(),
                RIL_CMD_PROXY_2, m_slot_id);
            sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(),
                    RIL_E_SUCCESS, RfxVoidData(), msg);
            responseToTelCore(response);
            break;
        }
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmEccNumberRequestHandler::onHandleEvent(const sp<RfxMclMessage> & msg) {
    int event = msg->getId();
    switch (event) {
        case RFX_MSG_EVENT_MD_ECC_CHANGE:
            handleMdEccChange(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "[%s] unknown event, ignore!", __FUNCTION__);
            break;
    }
}

// Use sync way to get ecc list
void RmmEccNumberRequestHandler::handleMdEccChange(const sp<RfxMclMessage> & msg) {
    logD(RFX_LOG_TAG, "[%s]", __FUNCTION__);
    sp<RfxMipcData> mipc = RfxMipcData::obtainMipcData(MIPC_CALL_GET_ECC_LIST_REQ,
            msg->getSlotId());
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(mipc);
    if ((mipc_result_const_enum) cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "[%d][%s] MIPC_CALL_GET_ECC_LIST_REQ return error",
                m_slot_id, __FUNCTION__);
        return;
    }
    int count = cnfMipc->getMipcUint32Val(MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_COUNT, 0);
    if (count <= 0) {
        // Should never goes here
        logE(RFX_LOG_TAG, "[%d][%s] ECC count invalid, count: %d",
                m_slot_id, __FUNCTION__, count);
        return;
    }
    uint16_t eccListSize = 0;
    mipc_ecc_info_struct4 *pMipcEccList = (mipc_ecc_info_struct4*)cnfMipc->getMipcVal(
            MIPC_CALL_GET_ECC_LIST_CNF_T_INFO_LIST, &eccListSize);
    RIL_EmergencyNumber *pData = (RIL_EmergencyNumber*)calloc(1,
            count * sizeof(RIL_EmergencyNumber));
    RFX_ASSERT(pData != NULL);
    logD(RFX_LOG_TAG, "[%s] MD ECC count: %d, eccListSize: %d", __FUNCTION__, count, eccListSize);
    for (int i = 0; i < count; i++) {
        asprintf(&pData[i].number, "%s", pMipcEccList[i].number);
        pData[i].isFake = pMipcEccList[i].is_fake_ecc;
        pData[i].categories = (uint32_t)(pMipcEccList[i].category);
        pData[i].sources = mipcEccSourceTypeToRILType(pMipcEccList[i].type);
        logD(RFX_LOG_TAG, "[%s] MD ECC[%d] {%s,isFake:%d,category:%d,type:%d}", __FUNCTION__, i,
                pMipcEccList[i].number, pMipcEccList[i].is_fake_ecc,
                pMipcEccList[i].category, pMipcEccList[i].type);
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_EMERGENCY_NUMBER_LIST,
            m_slot_id, RfxEmergencyNumberListData(pData,
            count * sizeof(RIL_EmergencyNumber)));
    responseToTelCore(urc);

    // free
    for (int i = 0; i < count; i++) {
        FREEIF(pData[i].number);
    }
    FREEIF(pData);
}

uint32_t RmmEccNumberRequestHandler::mipcEccSourceTypeToRILType(uint16_t type) {
    switch (type) {
        case MIPC_ECC_INFO_TYPE_HOST:
            return SOURCE_CONFIG;
        case MIPC_ECC_INFO_TYPE_SIM:
            return SOURCE_SIM;
        case MIPC_ECC_INFO_TYPE_MCF:
            return SOURCE_MCF;
        case MIPC_ECC_INFO_TYPE_NW:
            return SOURCE_NETWORK;
        case MIPC_ECC_INFO_TYPE_SPEC:
            return SOURCE_DEFAULT;
        default:
            break;
    }
    return 0;
}
