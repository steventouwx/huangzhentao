// SPDX-License-Identifier: MediaTekProprietary
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
#include "RpSmsController.h"
#include <cstdlib>
#include <list>
#include "PduParser.h"
#include "SmsMessage.h"
#include "ConcentratedSms.h"
#include <mtk_log.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <cutils/jstring.h>
#include "RpDataUtils.h"
#include "sms_pdu.h"
#include "RfxMainThread.h"
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include "Wakelockutils.h"
#include <prop/properties.h>

#define LOG_TAG "RpSmsController"

/*****************************************************************************
 * Class SuplMsgDispatcher
 *****************************************************************************/
bool RpSmsController::isDoing = false;
RFX_IMPLEMENT_CLASS("RpSmsController", RpSmsController, RfxController);

RpSmsController::RpSmsController() {
}
RpSmsController::~RpSmsController() {
}

void RpSmsController::onInit() {
    RfxController::onInit();
    const int request_id_list[] = {
        /* Common Request */
        RIL_REQUEST_SEND_SMS
    };
    const int urc_id_list[] = {
        RIL_UNSOL_RESPONSE_NEW_SMS,
        RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT
    };

    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(int));
    registerToHandleUrc(urc_id_list, (sizeof(urc_id_list)/sizeof(int)));
    init_watch_dog(m_slot_id);
    RLOGD("RpSmsController init!");
}

void RpSmsController::onDeinit() {
    RfxController::onDeinit();
}

void RpSmsController::smsAcknowledge(const sp<RfxMessage>& message) {
    sp<RfxMessage> rilRequest;
    rilRequest = RfxMessage::obtainRequest(message->getSlotId(),
            RADIO_TECH_GROUP_GSM, RIL_REQUEST_SMS_ACKNOWLEDGE);

    Parcel* parcel = rilRequest->getParcel();
    // make sure get the right position
    if (parcel != NULL) {
        parcel->setDataPosition(sizeof(int32_t)*2);
        android::status_t status = parcel->writeInt32(2);
        if (status != android::OK) {
            RLOGE("writeInt32 2 failed");
            return;
        }
        status  = parcel->writeInt32(1);
        if (status != android::OK) {
            RLOGE("writeInt32 1 failed");
            return;
        }
        status  = parcel->writeInt32(0);
        if (status != android::OK) {
            RLOGE("writeInt32 0 failed");
            return;
        }
    }
    RfxMainThread::enqueueMessage(rilRequest);
}

std::unordered_map<std::string,std::unordered_map<int,std::map<int,std::string>>> gMessage;

int RpSmsController::getMsg(std::string& pdu,std::map<int,std::string>& msg) {
    std::string message;
    int length;
    std::map<int,std::string>::iterator itr = msg.begin();
    for (itr; itr != msg.end(); ++itr) {
        message += itr->second;
    }
    length = message.length();
    pdu = message;
    return length;
}

void RpSmsController:: insertMsg(mapmessage_t &msg) {
    RLOGD("RpSmsController::insertMsg, msg.phoneNumber: %s, msg.referrenceNum: %d, msg.currentMesNum: %d, msg.pdu.length: %d, msg.pdu: %s", msg.phoneNumber.c_str(),
          msg.referrenceNum, msg.currentMesNum, msg.pdu.length(), msg.pdu.c_str());
    gMessage[msg.phoneNumber][msg.referrenceNum][msg.currentMesNum] = msg.pdu;
    RLOGD("insertMsg: size: %d\n",gMessage[msg.phoneNumber][msg.referrenceNum].size());
}

void RpSmsController::onHandleNewSmsReceived(const sp<RfxMessage>& message) {
    int32_t support = property_get_int32("persist.vendor.sms_codec_support", 0);
    if (!support) {
      RLOGD("onHandleNewSmsReceived: SMS codec don't supported");
      responseToRilj(message);
        sp<RfxMessage> rilRequest;
        rilRequest = RfxMessage::obtainRequest(message->getSlotId(),
                RADIO_TECH_GROUP_GSM, RIL_REQUEST_SMS_ACKNOWLEDGE);

        Parcel* parcel = rilRequest->getParcel();
        // make sure get the right position
        if (parcel != NULL) {
            parcel->setDataPosition(sizeof(int32_t)*2);
            parcel->writeInt32(2);
            parcel->writeInt32(1);
            parcel->writeInt32(0);
        }
        RfxMainThread::enqueueMessage(rilRequest);
        return;
    }
    Parcel *urcParcel = message->getParcel();
    if (urcParcel == NULL) {
        RLOGD("onHandleNewSmsReceived urcParcel null");
        return;
    }
    char *pdu = RpDataUtils::strdupReadString(urcParcel);
    RLOGD("onHandleNewSms ret: %s", pdu);
    if (pdu != NULL) {
        char smsc[512] = {0};
        char msg[512] = {0};
        char num[512] = {0};
        int charset = 0;
        int ref_num = 0;
        int cur_pack = 0;
        int total_pack = -1;
        RLOGD("len: %d, sms: %s", strlen(pdu), pdu);

        kal_int32 status = MDAPI_RET_ERROR;
        status = smsPduDecode(pdu, strlen(pdu), num, smsc, msg, &charset, &ref_num, &cur_pack , &total_pack);
        if(status != MDAPI_RET_SUCCESS) {
            RLOGE("Failed to decode pdu: %s", pdu);
            free(pdu);
            return;
        }
        if (total_pack == -1){
            urcParcel->setDataPosition(sizeof(uint32_t)*2);
            urcParcel->writeInt32((int32_t)charset);
            RpDataUtils::writeStringToParcel(urcParcel, num);
            RpDataUtils::writeStringToParcel(urcParcel, msg);
            urcParcel->writeInt32(strlen(msg));

            RLOGD("single message decoded message: %d, sms: %s", strlen(msg), msg);
            responseToRilj(message);
            }
        else {
            mapmessage_t map_msg;

            map_msg.phoneNumber = num;
            map_msg.referrenceNum = ref_num;
            map_msg.currentMesNum = cur_pack;
            map_msg.maximumNum = total_pack;
            map_msg.pdu = msg;

            int slotId = getSlotId();
            if(!isDoing) {
                mtk_acquire_wake_lock((slotId == 0) ? LONG_SMS_WAKE_LOCK_NAME0: LONG_SMS_WAKE_LOCK_NAME1);
                isDoing = true;
                start_watch_dog(slotId, LONG_SMS_WAKE_LOCK_TIMEOUT);
            }

            insertMsg(map_msg);
            std::map<int, std::string> outMsg = gMessage[map_msg.phoneNumber] [map_msg.referrenceNum];
            RLOGD("onHandleNewSmsReceived outMsg.size(): %d", outMsg.size());
            if (outMsg.size() == map_msg.maximumNum) {
                RLOGD("onHandleNewSmsReceived outMsg.size() == map_msg.maximumNum, then in the last seg");
                gMessage[map_msg.phoneNumber].erase(map_msg.referrenceNum);
                if (gMessage[map_msg.phoneNumber].empty()) {
                  gMessage.erase(map_msg.phoneNumber);
                }
                int length;
                std::string msg_all = "";
                length = getMsg(msg_all, outMsg);
                msg_all[length] = '\0';
                RLOGD("length=%d\n",length);
                urcParcel->setDataPosition(sizeof(uint32_t)*2);
                urcParcel->writeInt32((int32_t)charset);
                RpDataUtils::writeStringToParcel(urcParcel, num);
                RpDataUtils::writeStringToParcel(urcParcel, msg_all.c_str());
                urcParcel->writeInt32(msg_all.size());
                responseToRilj(message);
                if(isDoing) {
                    mtk_release_wake_lock((slotId == 0) ? LONG_SMS_WAKE_LOCK_NAME0: LONG_SMS_WAKE_LOCK_NAME1);
                    isDoing = false;
                    stop_watch_dog(slotId);
                }
            }
        }

        sp<RfxMessage> rilRequest;
        rilRequest = RfxMessage::obtainRequest(message->getSlotId(),
                RADIO_TECH_GROUP_GSM, RIL_REQUEST_SMS_ACKNOWLEDGE);

        Parcel* parcel = rilRequest->getParcel();
        // make sure get the right position
        if (parcel != NULL) {
            parcel->setDataPosition(sizeof(int32_t)*2);
            parcel->writeInt32(2);
            parcel->writeInt32(1);
            parcel->writeInt32(0);
        }
        RfxMainThread::enqueueMessage(rilRequest);
        free(pdu);
    }
}

void RpSmsController::onHandleSendSms(const sp<RfxMessage>& request) {
    int32_t support = property_get_int32("persist.vendor.sms_codec_support", 0);
    if (!support) {
        RLOGD("onHandleSendSms: SMS codec don't supported");
        requestToRild(request);
        return;
    }
    Parcel *reqParcel = request->getParcel();
    reqParcel->setDataPosition(sizeof(uint32_t) * 2);
    RLOGD("onHandleSendSms");

    int charset = reqParcel->readInt32();
    char *phoneNum = RpDataUtils::strdupReadString(reqParcel);
    if (phoneNum == nullptr) {
        RLOGE("onHandleSendSms phone number is null");
        return;
    }

    char *msg = RpDataUtils::strdupReadString(reqParcel);
    if (msg == nullptr) {
        RLOGE("onHandleSendSms message is null");
        free(phoneNum);
        return;
    }

    int msgLen = reqParcel->readInt32();
    int msg_num = 0, ret;

    char smscPDU[30] = {0};
    char **pdu = nullptr;
    char smsc[16] = {0};

    RLOGD("onHandleSendSms message: %d, %s", msgLen, msg);
    RLOGD("onHandleSendSms phone_number: %s", phoneNum);

    ret = _mdapi_sms_get_msg_num(msg, charset, &msg_num, &msgLen);
    if (ret != MDAPI_RET_SUCCESS) {
        RLOGD("get message number failed");
        free(phoneNum);
        free(msg);
        return;
    }

    pdu = (char **)malloc(sizeof(char *) * msg_num);
    if (pdu == nullptr) {
        RLOGW("%s, %d, allocate memory for pdu failed", __FUNCTION__, __LINE__);
        free(phoneNum);
        free(msg);
        return;
    }

    bool allocationFailed = false;
    for (int index = 0; index < msg_num; ++index) {
        pdu[index] = (char *)malloc(MAX_PDU_SIZE);
        if (pdu[index] == nullptr) {
            allocationFailed = true;
            msg_num = index;  // Adjust msg_num to correctly free already allocated memory
            break;
        }
    }

    if (allocationFailed) {
        for (int i = 0; i < msg_num; ++i) {
            free(pdu[i]);
        }
        free(pdu);
        free(phoneNum);
        free(msg);
        RLOGW("%s, %d, allocate memory for pdu failed", __FUNCTION__, __LINE__);
        return;
    }

    smsPduEncode(smsc, phoneNum, msg, charset, smscPDU, pdu);
    RLOGD("encoded smscPdu: %s", smscPDU);

    for (int index = 0; index < msg_num; ++index) {
        RLOGD("encoded pdu len: %zu, pdu[%d]: %s", strlen(pdu[index]), index, pdu[index]);
        reqParcel->setDataPosition(sizeof(uint32_t) * 2);
        reqParcel->writeInt32(2);
        RpDataUtils::writeStringToParcel(reqParcel, smscPDU);
        RpDataUtils::writeStringToParcel(reqParcel, pdu[index]);
        requestToRild(request);
        free(pdu[index]);  // Freeing each PDU as soon as it's processed
    }

    free(pdu);
    free(phoneNum);
    free(msg);
}

bool RpSmsController::onHandleRequest(const sp<RfxMessage>& request) {
    int msg_id = request->getId();
    RLOGD("[onHandleRequest] handle request %d %s (slot %d)", msg_id, requestToString(msg_id),
        getSlotId());

    Parcel *reqParcel = request->getParcel();
    switch (msg_id) {
        case RIL_REQUEST_SEND_SMS:
            onHandleSendSms(request);
            break;
        default:
            break;
    }
    return true;
}

bool RpSmsController::onHandleResponse(const sp<RfxMessage>& response) {
    int msg_id = response->getId();
    sp<RfxMessage> rilResponse = response;

    RLOGD("[onHandleResponse] handle resp %d %s (slot %d)", msg_id, requestToString(msg_id),
            getSlotId());

    responseToRilj(response);
    return true;
}

bool RpSmsController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    RLOGD("[onHandleUrc] handle urc %d %s (slot %d)", msg_id, requestToString(msg_id),
            getSlotId());

    switch (msg_id) {
        case RIL_UNSOL_RESPONSE_NEW_SMS:
            onHandleNewSmsReceived(message);
            break;
        case RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT:
            smsAcknowledge(message);
            break;
        default:
            responseToRilj(message);
            break;
    }
    return true;
}

