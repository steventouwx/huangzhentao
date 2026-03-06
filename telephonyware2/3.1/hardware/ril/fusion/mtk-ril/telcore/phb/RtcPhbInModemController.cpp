/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
#include "RtcPhbInModemController.h"
#include "RfxPhbEntriesExtData.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"
#include "RfxIntsData.h"
#include "rfx_properties.h"
#include <libmtkrilutils.h>

#define RFX_LOG_TAG "RtcPhbInModem"

using ::android::String8;

/*****************************************************************************
 * Class RtcPhbInModemController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtcPhbInModemController", RtcPhbInModemController, RfxController);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_RECORDS);

RFX_REGISTER_DATA_TO_URC_ID(RfxPhbEntriesExtData, RFX_MSG_URC_SIM_PHONEBOOK_RECORDS_RECEIVED);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_SIM_PHONEBOOK_CHANGED);

RtcPhbInModemController::RtcPhbInModemController() {
    mPhonebookCapacity.maxAdnRecords = 0;
    mPhonebookCapacity.usedAdnRecords = 0;
    mPhonebookCapacity.maxEmailRecords = 0;
    mPhonebookCapacity.usedEmailRecords = 0;
    mPhonebookCapacity.maxAdditionalNumberRecords = 0;
    mPhonebookCapacity.usedAdditionalNumberRecords = 0;
    mPhonebookCapacity.maxNameLen = 0;
    mPhonebookCapacity.maxNumberLen = 0;
    mPhonebookCapacity.maxEmailLen = 0;
    mPhonebookCapacity.maxAdditionalNumberLen = 0;
}

RtcPhbInModemController::~RtcPhbInModemController() {
}

void RtcPhbInModemController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "[%s]", __FUNCTION__);

    const int request1[] = {
        RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_RECORDS,
        RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY,
        RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS,
        RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS,
    };
    registerToHandleRequest(request1, sizeof(request1)/sizeof(int));

    const int urc_id_list[] = {
        RFX_MSG_URC_SIM_PHONEBOOK_RECORDS_RECEIVED,
        RFX_MSG_URC_SIM_PHONEBOOK_CHANGED,
    };
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));
}

bool RtcPhbInModemController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff, int radioState) {
    int msgId = message->getId();
    char phbState[RFX_PROPERTY_VALUE_MAX] = {0};
    getMSimProperty(m_slot_id, (char*)"vendor.gsm.sim.ril.phbready", phbState);
    logD(RFX_LOG_TAG, "[%s][%s][%s]", __FUNCTION__, phbState, RFX_ID_TO_STR(msgId));
    // wait phb state ready
    if (msgId == RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY && 0 != strcmp(phbState, "true")) {
        return true;
    }

    if ((radioState == (int)RADIO_STATE_OFF) &&
            (msgId == RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_RECORDS ||
            msgId == RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY ||
            msgId == RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS ||
            msgId == RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS)) {
        return false;
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

bool RtcPhbInModemController::onHandleUrc(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    logD(RFX_LOG_TAG, "onHandleUrc, handle %s", RFX_ID_TO_STR(msgId));
    responseToRilj(message);
    return true;
}

void RtcPhbInModemController::createNewReadSimPhonebookMessageToRmc(
        int messageId, int slotId, int begineIndex, int endIndex) {
    int reserved[2];
    reserved[0] = begineIndex;
    if (endIndex > mPhonebookCapacity.maxAdnRecords) {
        reserved[1] = mPhonebookCapacity.maxAdnRecords;
    } else {
        reserved[1] = endIndex;
    }
    sp<RfxMessage> request = RfxMessage::obtainRequest(
            slotId, messageId, RfxIntsData(reserved, 2));
    requestToMcl(request);
}

bool RtcPhbInModemController::onHandleResponse(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    switch (msgId) {
        case RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS: {
                responseToRilj(message);
            }
            break;
        case RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS: {
                if (message->getError() == RIL_E_SUCCESS) {
                    int *pInt = (int *)message->getData()->getData();
                    int lastIndex = pInt[0];
                    int readNumber = pInt[1];
                    readedAdnRecords = readedAdnRecords + readNumber;
                    if (readedAdnRecords >= mPhonebookCapacity.usedAdnRecords) {
                        logI(RFX_LOG_TAG, "read simphonebookdone");
                    } else {
                        createNewReadSimPhonebookMessageToRmc(
                                RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS,
                                m_slot_id, lastIndex + 1, lastIndex + 10);
                    }
                } else {
                    logI(RFX_LOG_TAG, "read simphonebook fail");
                }
            }
            break;
        case RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY: {
                int *pInt = (int *)message->getData()->getData();
                mPhonebookCapacity.maxAdnRecords = (int32_t)pInt[0];
                mPhonebookCapacity.usedAdnRecords = (int32_t)pInt[1];
                mPhonebookCapacity.maxEmailRecords = (int32_t)pInt[2];
                mPhonebookCapacity.usedEmailRecords = (int32_t)pInt[3];
                mPhonebookCapacity.maxAdditionalNumberRecords = (int32_t)pInt[4];
                mPhonebookCapacity.usedAdditionalNumberRecords = (int32_t)pInt[5];
                mPhonebookCapacity.maxNameLen = (int32_t)pInt[6];
                mPhonebookCapacity.maxNumberLen = (int32_t)pInt[7];
                mPhonebookCapacity.maxEmailLen = (int32_t)pInt[8];
                mPhonebookCapacity.maxAdditionalNumberLen = (int32_t)pInt[9];
                responseToRilj(message);
            }
            break;
        default: {
                responseToRilj(message);
            }
            break;
    }
    logD(RFX_LOG_TAG, "onHandleResponse, handle %s", RFX_ID_TO_STR(msgId));
    return true;
}

int RtcPhbInModemController::beginToReadSimPhoneBookRecordsFromModem(void) {
    int ret = 1;
    readedAdnRecords = 0;
    if (mPhonebookCapacity.usedAdnRecords > 0) {
        createNewReadSimPhonebookMessageToRmc(
            RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS, m_slot_id, 1, 10);
    } else {
        RIL_PHB_ENTRY *entry = NULL;
        RIL_PHB_ENTRY **entries = NULL;
        sp<RfxMessage> urcIndication = NULL;
        entries = (RIL_PHB_ENTRY**)calloc(1, sizeof(RIL_PHB_ENTRY*) * 1);
        if (entries == NULL) {
            logE(RFX_LOG_TAG, "entries calloc fail");
            return ret;
        }
        entry = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
        if (entry == NULL) {
            free(entries);
            logE(RFX_LOG_TAG, "entry calloc fail");
            return ret;
        }
        entries[0] = entry;
        // this magic number means used adn is 0
        entry->hidden = 0x1234;

        urcIndication = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_SIM_PHONEBOOK_RECORDS_RECEIVED,
                RfxPhbEntriesExtData(entries, 1));
        responseToRilj(urcIndication);
        free(entries[0]);
        free(entries);
    }
    return ret;
}

bool RtcPhbInModemController::onHandleRequest(const sp<RfxMessage>& message) {
    int msgId = message->getId();
    switch (msgId) {
        case RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_RECORDS: {
                sp<RfxMessage> response = RfxMessage::obtainResponse(message->getSlotId(), message->getPId(),
                        RIL_E_SUCCESS, RfxVoidData(), message);
                responseToRilj(response);
                beginToReadSimPhoneBookRecordsFromModem();
            }
            break;
        case RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY:
        case RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS: {
                requestToMcl(message);
            }
            break;
        default: {
                requestToMcl(message);
            }
            break;
    }
    logD(RFX_LOG_TAG, "onHandleRequest, handle %s", RFX_ID_TO_STR(msgId));
    return true;
}
