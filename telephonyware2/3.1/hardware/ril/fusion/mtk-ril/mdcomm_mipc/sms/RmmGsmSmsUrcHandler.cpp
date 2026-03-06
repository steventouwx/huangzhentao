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
#include "RfxMessageId.h"
#include "RmmGsmSmsUrcHandler.h"
#include <telephony/mtk_ril.h>
#include "rfx_properties.h"
#include "RfxStatusDefs.h"
#include "RfxIntsData.h"
#include "RfxStringData.h"
#include "RfxVoidData.h"
#include "RfxRawData.h"
#include "RfxRilUtils.h"
#include "RmmGsmSmsDefs.h"

using ::android::String8;


static const int urc_list[] = {
    MIPC_SMS_NEW_CBM_IND,
    MIPC_SMS_EXT_INFO_IND,
    MIPC_SMS_STORE_STATUS_IND,
    MIPC_SMS_NEW_STATUS_REPORT_IND,
};

static const int cmd_list[] = {
    MIPC_SMS_NEW_SMS_CMD,
    MIPC_SMS_NEW_STATUS_REPORT_CMD,
};

static const int event_list[] = {
    RFX_MSG_EVENT_SMS_DUP_DELAY,
    RFX_MSG_EVENT_GSM_NEW_SMS_IND,
};

static const int DUP_SMS_DELAY_TIME_MS = 2000;

RFX_IMPLEMENT_HANDLER_CLASS(RmmGsmSmsUrcHandler, RIL_CMD_PROXY_URC);

RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_RESPONSE_NEW_SMS);
RFX_REGISTER_DATA_TO_URC_ID(RfxRawData, RFX_MSG_URC_RESPONSE_NEW_BROADCAST_SMS);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_SMS_STORAGE_FULL);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_ME_SMS_STORAGE_FULL);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_CDMA_RUIM_SMS_STORAGE_FULL);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_RESPONSE_NEW_SMS_ON_SIM);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, \
        RFX_MSG_URC_RESPONSE_NEW_SMS_STATUS_REPORT);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_SMS_EINFO_EXTENSIONS);
RFX_REGISTER_DATA_TO_URC_ID(
        RfxStringData, RFX_MSG_URC_CDMA_CARD_INITIAL_ESN_OR_MEID);


// Register event
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_SMS_DUP_DELAY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxStringData, RFX_MSG_EVENT_GSM_NEW_SMS_IND);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
RmmGsmSmsUrcHandler::RmmGsmSmsUrcHandler(int slot_id, int channel_id) :
        RmmGsmSmsBaseHandler(slot_id, channel_id) {
    setTag(String8("RmmGsmSmsUrc"));
    mLastSms = NULL;
    if (RfxRilUtils::isSmsSupport()) {
        registerToHandleMipcURC(urc_list, sizeof(urc_list)/sizeof(int));
        registerToHandleMipcCmd(cmd_list, sizeof(cmd_list)/sizeof(int));
        registerToHandleEvent(event_list, sizeof(event_list)/sizeof(int));
    }
}

RmmGsmSmsUrcHandler::~RmmGsmSmsUrcHandler() {
    if (mLastSms != NULL) {
        free(mLastSms);
        mLastSms = NULL;
    }
}

void RmmGsmSmsUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(mTag, "onHandleUrc msgId:%d MipcMsgId:%d, MipcSlotId:%d", msg->getId(),
                msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        switch (msg->getMipcData()->getMipcMsgId()) {
        case MIPC_SMS_NEW_CBM_IND:
            handleNewBroadcastSms(msg);
            break;
        case MIPC_SMS_EXT_INFO_IND:
            handleSmsEInfoExtensions(msg);
            break;
        case MIPC_SMS_STORE_STATUS_IND:
            handleSimSmsStorageStatus(msg);
            break;
        case MIPC_SMS_NEW_STATUS_REPORT_IND:
            handleNewSmsStatusReportUrc(msg);
            break;
        default:
            break;
        }
        return;
    }
}

void RmmGsmSmsUrcHandler::onHandleCmd(const sp<RfxMclMessage>& msg) {
    if (msg->getMipcData() != NULL) {
        logD(mTag, "onHandleUrc msgId:%d MipcMsgId:%d, MipcSlotId:%d", msg->getId(),
                msg->getMipcData()->getMipcMsgId(), msg->getMipcData()->getMipcSlotId());
        switch (msg->getMipcData()->getMipcMsgId()) {
        case MIPC_SMS_NEW_SMS_CMD:
            handleNewSmsCmd(msg);
            break;
        case MIPC_SMS_NEW_STATUS_REPORT_CMD:
            handleNewSmsStatusReportCmd(msg);
            break;
        default:
            break;
        }
        return;
    }
}

void RmmGsmSmsUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch(event) {
        case RFX_MSG_EVENT_SMS_DUP_DELAY: {
                sp<RfxMclMessage> unsol;
                char *smsPdu = (char*)msg->getData()->getData();
                unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_SMS,
                        m_slot_id, RfxStringData(smsPdu, strlen(smsPdu)));
                responseToTelCore(unsol);
            }
            break;
        case RFX_MSG_EVENT_GSM_NEW_SMS_IND:
            handleNewSmsUrc(msg);
            break;
        default:
            logE(mTag, "Not support the event!");
            break;
    }
}

void RmmGsmSmsUrcHandler::handleNewSmsUrc(const sp<RfxMclMessage>& msg) {
    int smsType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
            SMS_INBOUND_NONE);
    showCurrIncomingSmsType();
    if (smsType == SMS_INBOUND_NONE) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
                SMS_INBOUND_3GPP_CMT);
        onNewSmsUrc(msg);
    } else {
        logE(mTag, "One 3G or IMS SMS on AP, reject");
    }
}

void RmmGsmSmsUrcHandler::onNewSmsUrc(const sp<RfxMclMessage>& msg) {
    sp<RfxMclMessage> unsol;

    // no need ack
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID, -1);

    // get the pdu
    char *smsPduHex = (char *)msg->getData()->getData();

    // if dup sms, delay 2sec to report
    int pduLen = strlen(smsPduHex);
    if (mLastSms != NULL && pduLen == strlen(mLastSms) &&
            strcmp(smsPduHex, mLastSms) == 0) {
        sendEvent(RFX_MSG_EVENT_SMS_DUP_DELAY, RfxStringData(smsPduHex, pduLen),
                RIL_CMD_PROXY_1, m_slot_id, -1, -1, ms2ns(DUP_SMS_DELAY_TIME_MS));
        return;
    }

    // cache the last sms
    if (mLastSms != NULL) {
        free(mLastSms);
        mLastSms = NULL;
    }
    mLastSms = (char*)malloc(pduLen + 1);
    RFX_ASSERT(mLastSms != NULL);
    strncpy(mLastSms, smsPduHex, pduLen);
    mLastSms[pduLen] = '\0';

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_SMS,
            m_slot_id, RfxStringData(smsPduHex, pduLen));
    responseToTelCore(unsol);
}


void RmmGsmSmsUrcHandler::onNewSmsCmd(const sp<RfxMclMessage>& msg) {
    unsigned int tpduLen, pduLen;
    int i;
    uint8_t *smsPdu = NULL;
    char smsPduHex[513];
    int err;
    sp<RfxMclMessage> unsol;
    mipc_sms_pdu_struct4 *smsPdulist;
    int txid = msg->getMipcData()->getMipcMsgTxId();
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID, txid);

    smsPdulist = (mipc_sms_pdu_struct4 *)msg->getMipcData()->getMipcVal(
            MIPC_SMS_NEW_SMS_CMD_T_PDU_LIST, NULL);

    // get <tpdu length> without sca
    tpduLen = smsPdulist->pdu_len;
    // pdu length with sca
    pduLen = smsPdulist->pdu[0] + tpduLen + 1;

    smsPdu = smsPdulist->pdu;

    //convert bytes to hex string
    for (i = 0; i < pduLen; i++) {
        int ret = sprintf(smsPduHex + i * 2, "%02x", smsPdu[i]);
        if (ret < 0) goto error;
    }
    smsPduHex[i*2] = '\0';

    err = smsCheckReceivedPdu(tpduLen, smsPduHex, true);
    if (err < 0) goto error;

    // if dup sms, delay 2sec to report
    pduLen = strlen(smsPduHex);
    if (mLastSms != NULL && pduLen == strlen(mLastSms) &&
            strcmp(smsPduHex, mLastSms) == 0) {
        sendEvent(RFX_MSG_EVENT_SMS_DUP_DELAY, RfxStringData(smsPduHex, pduLen),
                RIL_CMD_PROXY_1, m_slot_id, -1, -1, ms2ns(DUP_SMS_DELAY_TIME_MS));
        return;
    }

    // cache the last sms
    if (mLastSms != NULL) {
        free(mLastSms);
        mLastSms = NULL;
    }
    mLastSms = (char*)malloc(pduLen + 1);
    RFX_ASSERT(mLastSms != NULL);
    strncpy(mLastSms, smsPduHex, pduLen);
    mLastSms[pduLen] = '\0';

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_SMS,
            m_slot_id, RfxStringData(smsPduHex, pduLen));
    responseToTelCore(unsol);

    return;

error:
    // Use the timer callback to send CNMA fail to modem and will reset the
    // sms_type value while the CNMA is sent
    int ackData[2] = {ERR_ACK_CMT_ACK_RESET, txid};
    sendEvent(RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK, RfxIntsData(ackData, 2),
            RIL_CMD_PROXY_1, m_slot_id);

    logE(mTag, "onNewSms check fail.");
}

void RmmGsmSmsUrcHandler::handleNewSmsCmd(const sp<RfxMclMessage>& msg) {
    uint8_t format;
    format = msg->getMipcData()->getMipcUint8Val(MIPC_SMS_NEW_SMS_CMD_T_FORMAT,
            MIPC_SMS_FORMAT_PDU_3GPP);
    if (format == MIPC_SMS_FORMAT_PDU_3GPP) {
        int smsType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
                SMS_INBOUND_NONE);
        showCurrIncomingSmsType();
        if (smsType == SMS_INBOUND_NONE) {
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
                    SMS_INBOUND_3GPP_CMT);
            onNewSmsCmd(msg);
        } else {
            logE(mTag, "One 3G or IMS SMS on AP, reject");
            int txid = msg->getMipcData()->getMipcMsgTxId();
            // Use the timer callback to send CNMA fail to modem and will reset the
            // sms_type value while the CNMA is sent
            int ackData[2] = {ERR_ACK_CMT_ACK_ONLY, txid};
            sendEvent(RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK, RfxIntsData(ackData, 2),
                    RIL_CMD_PROXY_1, m_slot_id);
        }
    } else if (format == MIPC_SMS_FORMAT_PDU_3GPP2) {
        int txid = msg->getMipcData()->getMipcMsgTxId();
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_CDMA_INBOUND_SMS_PRE_TXID, txid);

        uint16_t length = 0;
        uint8_t *pdu = (uint8_t *)msg->getMipcData()->getMipcVal(MIPC_SMS_NEW_SMS_CMD_T_PDU_C2K,
                &length);
        //convert bytes to hex string
        String8 pduHex("");
        for (int i = 0; i < length; i++) {
            pduHex.appendFormat("%02x", pdu[i]);
        }

        sendEvent(RFX_MSG_EVENT_CDMA_NEW_SMS_CMD, RfxStringData((char*)pduHex.string(),
                 2 * length), RIL_CMD_PROXY_URC, m_slot_id);
    }
}

void RmmGsmSmsUrcHandler::onNewSmsStatusReportCmd(const sp<RfxMclMessage>& msg) {
    int i, err;
    char pduHex[513];
    uint16_t length = 0;
    uint8_t *pdu = (uint8_t *)msg->getMipcData()->getMipcVal(MIPC_SMS_NEW_STATUS_REPORT_CMD_T_PDU,
            &length);

    int txid = msg->getMipcData()->getMipcMsgTxId();
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID, txid);

    sp<RfxMclMessage> unsol;

    //convert bytes to hex string
    for (i = 0; i < length; i++) {
        int ret = sprintf(pduHex + i * 2, "%02x", pdu[i]);
        if (ret < 0) goto error;
    }
    pduHex[i*2] = '\0';

    err = smsCheckReceivedPdu(length, pduHex, false);
    if (err < 0) goto error;

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_SMS_STATUS_REPORT,
            m_slot_id, RfxStringData(pduHex, length * 2));
    responseToTelCore(unsol);

    return;

error:
    // Use the timer callback to send CNMA fail to modem and will reset the
    // sms_type value while the CNMA is sent
    int ackData[2] = {ERR_ACK_CDS_ACK_RESET, txid};
    sendEvent(RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK, RfxIntsData(ackData, 2),
            RIL_CMD_PROXY_1, m_slot_id);

    logE(mTag, "onNewSmsStatusReport check fail.");
}

void RmmGsmSmsUrcHandler::handleNewSmsStatusReportCmd(const sp<RfxMclMessage>& msg) {
    int smsType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
            SMS_INBOUND_NONE);

    showCurrIncomingSmsType();
    if (smsType == SMS_INBOUND_NONE) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
                SMS_INBOUND_3GPP_CDS);
        onNewSmsStatusReportCmd(msg);
    } else {
        logE(mTag, "One 3G or IMS SMS report on AP, reject");
        int txid = msg->getMipcData()->getMipcMsgTxId();
        // Use the timer callback to send CNMA fail to modem and will reset the
        // sms_type value while the CNMA is sent
        int ackData[2] = {ERR_ACK_CDS_ACK_ONLY, txid};
        sendEvent(RFX_MSG_EVENT_SMS_NEW_SMS_ERR_ACK, RfxIntsData(ackData, 2),
                RIL_CMD_PROXY_1, m_slot_id);
    }
}

void RmmGsmSmsUrcHandler::onNewSmsStatusReportUrc(const sp<RfxMclMessage>& msg) {
    int i, err;
    char pduHex[513];
    uint16_t length = 0;
    uint8_t *pdu = (uint8_t *)msg->getMipcData()->getMipcVal(MIPC_SMS_NEW_STATUS_REPORT_IND_T_PDU,
            &length);

    // no need ack
    getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TXID, -1);

    sp<RfxMclMessage> unsol;

    //convert bytes to hex string
    for (i = 0; i < length; i++) {
        int ret = sprintf(pduHex + i * 2, "%02x", pdu[i]);
        if (ret < 0) return;
    }
    pduHex[i*2] = '\0';

    err = smsCheckReceivedPdu(length, pduHex, false);
    if (err < 0) {
        logE(mTag, "onNewSmsStatusReportUrc check fail.");
        return;
    }

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_SMS_STATUS_REPORT,
            m_slot_id, RfxStringData(pduHex, length * 2));
    responseToTelCore(unsol);
}

void RmmGsmSmsUrcHandler::handleNewSmsStatusReportUrc(const sp<RfxMclMessage>& msg) {
    int smsType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
            SMS_INBOUND_NONE);

    showCurrIncomingSmsType();
    if (smsType == SMS_INBOUND_NONE) {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_GSM_INBOUND_SMS_TYPE,
                SMS_INBOUND_3GPP_CDS);
        onNewSmsStatusReportUrc(msg);
    } else {
        logE(mTag, "One 3G or IMS SMS report on AP, reject");
    }
}


void RmmGsmSmsUrcHandler::handleNewSmsOnSim(const sp<RfxMclMessage>& msg) {
    int index[1];
    sp<RfxMclMessage> unsol;

    index[0] = msg->getMipcData()->getMipcUint16Val(
            MIPC_SMS_STORE_STATUS_IND_T_MESSAGE_INDEX, 0);

    unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_SMS_ON_SIM,
            m_slot_id, RfxIntsData(index, 1));
    responseToTelCore(unsol);
}

void RmmGsmSmsUrcHandler::handleSimSmsStorageStatus(const sp<RfxMclMessage>& msg) {
    int status;
    int urc_id = -1;
    sp<RfxMclMessage> unsol;

    status = msg->getMipcData()->getMipcUint32Val(MIPC_SMS_STORE_STATUS_IND_T_FLAG, 0);

    // for +CMTI
    if ((status & MIPC_SMS_STORE_FLAG_NEW_MESSAGE) != 0) {
        handleNewSmsOnSim(msg);
    }

    if ((status & MIPC_SMS_STORE_FLAG_STORE_FULL_SIM) != 0) {
        // full, SIM exceed
        urc_id = RFX_MSG_URC_SIM_SMS_STORAGE_FULL;
    } else if ((status & MIPC_SMS_STORE_FLAG_STORE_FULL_UIM) != 0) {
        // c2k ruim exceed
        urc_id = RFX_MSG_URC_CDMA_RUIM_SMS_STORAGE_FULL;
    } else if ((status & MIPC_SMS_STORE_FLAG_STORE_FULL_ME) != 0) {
        // ME exceed
        urc_id = RFX_MSG_URC_ME_SMS_STORAGE_FULL;
    } else {
        logW(mTag, "The received <status> is not full: %d", status);
    }

    if (urc_id >= 0) {
        sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(urc_id, m_slot_id, RfxVoidData());
        responseToTelCore(unsol);
    }
}
void RmmGsmSmsUrcHandler::handleNewBroadcastSms(const sp<RfxMclMessage>& msg) {
    RfxMipcData* mipcData = msg->getMipcData();
    uint16_t length = 0;
    sp<RfxMclMessage> unsol;
    uint16_t msgId = mipcData->getMipcUint16Val(MIPC_SMS_NEW_CBM_IND_T_MESSAGE_ID, 0);
    uint16_t wacLen = 0;
    uint8_t *wacInfo = (uint8_t *)mipcData->getMipcVal(MIPC_SMS_NEW_CBM_IND_T_WAC_INFO, &wacLen);
    uint32_t cbmType = mipcData->getMipcUint32Val(MIPC_SMS_NEW_CBM_IND_T_CBM_TYPE, 0);

    if (MIPC_SMS_CBM_TYPE_ETWS_PRIMARY == cbmType) {
        // GSM format (currently only ETWS primary uses this format)
        uint16_t segNum = mipcData->getMipcArrayTlvNum(MIPC_SMS_NEW_CBM_IND_T_PDU_3GPP_SEG);
        for (int i = 0; i < segNum; i++) {
            char *byteSmsPdu = (char *)mipcData->getMipcIdxVal(MIPC_SMS_NEW_CBM_IND_T_PDU_3GPP_SEG,
                    i, &length);
            logD(mTag, "New CB bytes len %d", length);

            unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_BROADCAST_SMS, m_slot_id,
                    RfxRawData(byteSmsPdu, sizeof(char)*length));
            responseToTelCore(unsol);
        }
    } else if (0 == wacLen) {
        // UMTS format without wac info
        uint16_t sn = mipcData->getMipcUint16Val(MIPC_SMS_NEW_CBM_IND_T_SERIAL_NUMBER, 0);
        uint8_t dcs = mipcData->getMipcUint8Val(MIPC_SMS_NEW_CBM_IND_T_DCS, 0);
        uint16_t cbDataLen = 0;
        uint8_t *cbData = (uint8_t *)mipcData->getMipcVal(MIPC_SMS_NEW_CBM_IND_T_DATA, &cbDataLen);
        uint16_t finalPduLen = 1 + 2 + 2 + 1 + cbDataLen;
        uint8_t *finalPdu = (uint8_t *)malloc(finalPduLen * sizeof(int8_t));
        RFX_ASSERT(finalPdu != NULL);

        uint16_t idx = 0;
        finalPdu[idx++] = 1;
        finalPdu[idx++] = (uint8_t)(msgId >> 8);
        finalPdu[idx++] = (uint8_t)(msgId & 0xff);
        finalPdu[idx++] = (uint8_t)(sn >> 8);
        finalPdu[idx++] = (uint8_t)(sn & 0xff);
        finalPdu[idx++] = dcs;
        memcpy(&finalPdu[idx], cbData, cbDataLen);
        idx += cbDataLen;
        RFX_ASSERT(idx == finalPduLen);

        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_BROADCAST_SMS, m_slot_id,
                RfxRawData(finalPdu, sizeof(uint8_t)*finalPduLen));
        responseToTelCore(unsol);

        free(finalPdu);
    } else {
        // WEA3.0 message which is UMTS format with wac info
        uint16_t sn = mipcData->getMipcUint16Val(MIPC_SMS_NEW_CBM_IND_T_SERIAL_NUMBER, 0);
        uint8_t dcs = mipcData->getMipcUint8Val(MIPC_SMS_NEW_CBM_IND_T_DCS, 0);
        uint16_t cbDataLen = 0;
        uint8_t *cbData = (uint8_t *)mipcData->getMipcVal(MIPC_SMS_NEW_CBM_IND_T_DATA, &cbDataLen);
        uint16_t finalPduLen = 1 + 2 + 2 + 1 + cbDataLen + 2 + wacLen;
        uint8_t *finalPdu = (uint8_t *)malloc(finalPduLen * sizeof(int8_t));
        RFX_ASSERT(finalPdu != NULL);

        uint16_t idx = 0;
        finalPdu[idx++] = 1;
        finalPdu[idx++] = (uint8_t)(msgId >> 8);
        finalPdu[idx++] = (uint8_t)(msgId & 0xff);
        finalPdu[idx++] = (uint8_t)(sn >> 8);
        finalPdu[idx++] = (uint8_t)(sn & 0xff);
        finalPdu[idx++] = dcs;
        memcpy(&finalPdu[idx], cbData, cbDataLen);
        idx += cbDataLen;
        finalPdu[idx++] = (uint8_t)(wacLen & 0xff);
        finalPdu[idx++] = (uint8_t)(wacLen >> 8);
        memcpy(&finalPdu[idx], wacInfo, wacLen);
        idx += wacLen;
        RFX_ASSERT(idx == finalPduLen);

        unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_NEW_BROADCAST_SMS, m_slot_id,
                RfxRawData(finalPdu, sizeof(uint8_t)*finalPduLen));
        responseToTelCore(unsol);

        free(finalPdu);
    }
}

void RmmGsmSmsUrcHandler::handleSmsEInfoExtensions(const sp<RfxMclMessage>& msg) {
    char *strPara = NULL;
    String8 info("");
    int urc_id = -1;
    sp<RfxMclMessage> unsol;

    strPara = (char *)msg->getMipcData()->getMipcVal(MIPC_SMS_EXT_INFO_IND_T_EPSI, NULL);

    if (strPara != NULL) {
        info.append(strPara);
        urc_id = RFX_MSG_URC_SMS_EINFO_EXTENSIONS;
    } else {
        uint16_t length = 0;
        uint8_t *oldEsnMe = (uint8_t *)msg->getMipcData()->getMipcVal(
                MIPC_SMS_EXT_INFO_IND_T_ESN_OLD, &length);
        for (int i = 0; i < length; i++) {
            info.appendFormat("%02x", oldEsnMe[i]);
        }
        info.append(",");
        uint8_t *newEsnMe = (uint8_t *)msg->getMipcData()->getMipcVal(
                MIPC_SMS_EXT_INFO_IND_T_ESN_NEW, &length);
        for (int i = 0; i < length; i++) {
            info.appendFormat("%02x", newEsnMe[i]);
        }
        urc_id = RFX_MSG_URC_CDMA_CARD_INITIAL_ESN_OR_MEID;
    }

    if (urc_id >= 0) {
        unsol = RfxMclMessage::obtainUrc(urc_id,
                m_slot_id, RfxStringData((void*)info.string(), info.length()));
        responseToTelCore(unsol);
    }
}

