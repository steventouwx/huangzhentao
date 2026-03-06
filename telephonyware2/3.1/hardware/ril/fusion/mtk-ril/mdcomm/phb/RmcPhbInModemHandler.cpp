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

#include <stdio.h>
#include <string.h>
#include <telephony/mtk_ril.h>
#include "RmcPhbInModemHandler.h"
#include "RfxPhbEntryExtData.h"
#include "RfxPhbEntriesExtData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include <libmtkrilutils.h>
#include <SimPhonebookUtils.h>
#include <Unicode.h>
using ::android::String8;


RFX_IMPLEMENT_HANDLER_CLASS(RmcPhbInModemHandler,  RIL_CMD_PROXY_10);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxPhbEntryExtData, RfxIntsData, RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_PHB_STATE_CHANGED);


RmcPhbInModemHandler::RmcPhbInModemHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int request1[] = {
        RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY,
        RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS,
        RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS,
    };

    const int event[] = {
        RFX_MSG_EVENT_PHB_STATE_CHANGED,
    };

    registerToHandleRequest(request1, sizeof(request1)/sizeof(int));
    registerToHandleEvent(event, sizeof(event)/sizeof(int));
    mPhonebookBitArray = NULL;
}

RmcPhbInModemHandler::~RmcPhbInModemHandler() {
    if (mPhonebookBitArray != NULL) {
        delete mPhonebookBitArray;
    }
}

void RmcPhbInModemHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY: {
                requestGetSimPhonebookCapacity(msg);
            }
            break;
        case RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS: {
                requestUpdateSimPhonebookRecords(msg);
                sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                        RFX_MSG_URC_SIM_PHONEBOOK_CHANGED, m_slot_id, RfxVoidData());
                responseToTelCore(urc);
            }
            break;
        case RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS: {
                requestReadSimPhonebookRecords(msg);
            }
            break;
    }
}

void RmcPhbInModemHandler::clearPhbStorageInfo(void) {
    current_phb_storage = -1;
}

void RmcPhbInModemHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int event = msg->getId();
    switch (event) {
        case RFX_MSG_EVENT_PHB_STATE_CHANGED: {
            clearPhbStorageInfo();
            sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(
                    RFX_MSG_URC_SIM_PHONEBOOK_CHANGED, m_slot_id, RfxVoidData());
            responseToTelCore(urc);
        }
        break;
    }
}

int RmcPhbInModemHandler::selectPhbStorage(int type) {
    const char *storage;
    int result = 1;
    sp<RfxAtResponse> p_response = NULL;

    if (current_phb_storage != type) {
        storage = SimPhonebookUtils::getPhbStorageString(type);
        if (storage == NULL) {
            result = 0;
        }

        p_response = atSendCommand(String8::format("AT+CPBS=\"%s\"", storage));

        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            result = 0;
        }
        else {
            current_phb_storage = type;
        }
    }
    // logD(RFX_LOG_TAG, "Select the Storage: %d", m_slot_id);
    return result;
}

void RmcPhbInModemHandler::requestClearPhbEntry(int index) {
    String8 cmd("");
    int i = 0;
    int len = 0;
    int maxNum = -1;
    sp<RfxAtResponse> p_response = NULL;
    if (m_slot_id < 4) {
        maxNum = maxGrpNum;
    } else {
        maxNum = -1;
    }

    logD(RFX_LOG_TAG, "requestClearPhbEntry - maxNum = %d", maxNum);
    if (maxNum > 0) {
        cmd.append(String8::format("AT+EPBUM=2,5,%d,0", index));
        len = cmd.length();
        logD(RFX_LOG_TAG, "requestClearPhbEntry - len = %d", len);
        for (i = 0; i < maxNum * 2; i += 2) {
            cmd.append(",");
            cmd.append("0");
        }
        logD(RFX_LOG_TAG, "cmd = %s", cmd.string());
        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->getError() < 0) {
            logE(RFX_LOG_TAG, "requestClearPhbEntry Fail");
        }
    }
}

void RmcPhbInModemHandler:: loadUPBCapability() {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    RfxAtLine *line = NULL;
    int upbCap[8]= {0};
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;

    p_response = atSendCommandSingleline("AT+EPBUM=?", "+EPBUM:");
    logD(RFX_LOG_TAG, "loadUPBCapability slot %d", m_slot_id);
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "loadUPBCapability Fail");
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError());
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
                break;
            case CME_UNKNOWN:
                logE(RFX_LOG_TAG, "p_response: CME_UNKNOWN");
                break;
            default:
                break;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }

    if (ret != RIL_E_SUCCESS) {
        logE(RFX_LOG_TAG, "loadUPBCapability: Error!");
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    // logD(RFX_LOG_TAG, "loadUPBCapability: strlen of response is %d", strlen(line));
    upbCap[0] = line->atTokNextint(&err);  // number of supported ANR for a ADN entry
    if (err < 0) goto error;

    upbCap[1] = line->atTokNextint(&err);  // number of supported EMAIL for a ADN entry
    if (err < 0) goto error;

    upbCap[2] = line->atTokNextint(&err);  // number of supported SNE for a ADN entry
    if (err < 0) goto error;

    upbCap[3] = line->atTokNextint(&err);  // maximum number of AAS entries
    if (err < 0) goto error;

    upbCap[4] = line->atTokNextint(&err);  // maximum length of the AAS in the entries
    if (err < 0) goto error;

    upbCap[5] = line->atTokNextint(&err);  // maximum number of GAS entries
    if (err < 0) goto error;

    upbCap[6] = line->atTokNextint(&err);  // maximum length of the GAS in the entries
    if (err < 0) goto error;

    upbCap[7] = line->atTokNextint(&err);  // maximum number of GRP entries
    if (err < 0) goto error;

    if (m_slot_id < 4) {
        maxGrpNum = upbCap[7];
        maxAnrNum = upbCap[0];
        maxEmailNum = upbCap[1];
    }

    logD(RFX_LOG_TAG, "loadUPBCapability: %d, %d, %d, %d, %d, %d, %d, %d",
        upbCap[0], upbCap[1], upbCap[2], upbCap[3], upbCap[4], upbCap[5],
        upbCap[6], upbCap[7]);
error:
    logE(RFX_LOG_TAG, "loadUPBCapability: Error!");
}

void RmcPhbInModemHandler::requestUpdateSimPhonebookRecords(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxMclMessage> res = NULL;
    int responseValue[1] = {0};
    int tmp;
    size_t len, i, j;
    RIL_PHB_ENTRY *entry;
    char *line, *number;
    String8 cmd("");
    char* name = NULL;
    char alphaId[RIL_MAX_PHB_NAME_LEN*4+4+1]={0, };  // If using UCS2, each character use 4 bytes,
                                                     // if using 0x82 coding, we need extra 4 bytes,
                                                     // we also need '\0'

    RIL_Errno ret = RIL_E_INTERNAL_ERR;
    RilPhbCpbwEncode encodeMethod;
    int *data = (int *)msg->getData()->getData();

    entry = (RIL_PHB_ENTRY *)data;
    if (entry == NULL) {
        logE(RFX_LOG_TAG, "phb entry data is null");
        goto error;
    }
    selectPhbStorage(RIL_PHB_ADN);
    if (entry != NULL && (entry->text != NULL || entry->number != NULL)) {
        if (entry->text == NULL) {
            line = (char*)"";
        } else {
            line = entry->text;
        }

        int dest_length = utf8_to_utf32_length(line, strlen(line));
        char32_t * dest = (char32_t *)malloc((dest_length + 1) * sizeof(char32_t));
        if (dest == NULL) {
            goto error;
        }
        utf8_to_utf32(line, strlen(line), dest);
        logE(RFX_LOG_TAG, "The dest alphaid length:%d", dest_length);
        char *nameToUcs = (char*)alloca(dest_length * 4 + 1);;
        for (i = 0; i < dest_length; i++) {
            logE(RFX_LOG_TAG, "The dest alphaid[%zu]=%04X", i, dest[i]);
            int ret = sprintf(nameToUcs + i * 4, "%04X", dest[i]);
            if (ret < 0) {
                free(dest);
                goto error;
            }
        }
        nameToUcs[i * 4] = '\0';
        line = nameToUcs;
        free(dest);

        if (entry->number == NULL) {
            number = (char*)"";
        } else {
            number = entry->number;
        }

        len = strlen(line);
        if (len == 0 && strlen(number) == 0) {
            logE(RFX_LOG_TAG, "The number and alphaid not be empty String at the same time: %s %s",
                    line, number);
            goto error;
        }

        logE(RFX_LOG_TAG, "The number and alphaid:%s,%s", line, number);

        // pack Alpha Id
        if ((len % 4) != 0) {
            logE(RFX_LOG_TAG, "The alphaId should encode using Hexdecimal: %s", line);
            goto error;
        } else if (len > (RIL_MAX_PHB_NAME_LEN*4)) {
            logE(RFX_LOG_TAG, "The alphaId shouldn't longer than RIL_MAX_PHB_NAME_LEN");
            ret = RIL_E_TEXT_STRING_TOO_LONG;
            goto error;
        }

        for (i=0, j=0; i < len; i+=4, j++) {
            tmp = SimPhonebookUtils::hexCharToDecInt(line + i, 4);
            if (tmp >= 128) {
                break;
            }
            alphaId[j] = static_cast<char>(tmp);
        }
        alphaId[j] = static_cast<char>('\0');

        logD(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords - i = %zu, len = %zu", i, len);
        if (i != len) {
            len /= 4;

            if (SimPhonebookUtils::encodeUCS2_0x81(line, alphaId, sizeof(alphaId))) {  // try UCS2_0x81 coding
                // UCS2 0x81 coding
                encodeMethod = CPBW_ENCODE_UCS2_81;
                // alphaId[ril_max_phb_name_len * 2] = '\0';
            } else if (SimPhonebookUtils::encodeUCS2_0x82(line, alphaId, sizeof(alphaId))) {
                // try UCS2_0x82 coding
                // UCS2 0x82 coding
                encodeMethod = CPBW_ENCODE_UCS2_82;
            } else {
                // UCS2 coding
                encodeMethod = CPBW_ENCODE_UCS2;
                logD(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords-alphaIdlen = %zu,linelength=%zu",
                        sizeof(alphaId), strlen(line));
                memcpy(alphaId, line,
                        ((sizeof(alphaId) < strlen(line)) ? sizeof(alphaId) : strlen(line)));
                // alphaId[ril_max_phb_name_len * 2] = '\0';
            }
        } else {
            encodeMethod = CPBW_ENCODE_IRA;
                // ASCII coding
        }

        // for add new contacts
        if (entry->index <= 0) {
            unsigned int firstIndex = 0;

            if (mPhonebookBitArray == NULL) {
                if (maxAdnRecords > 0) {
                    mPhonebookBitArray = new PhonebookBitArray(maxAdnRecords);
                } else {
                    mPhonebookBitArray = new PhonebookBitArray(1000);
                }
            }

            firstIndex = mPhonebookBitArray -> getFirstNotSetBitIndex();
            if (firstIndex > mPhonebookBitArray -> getLength()) {
                logE(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords %d > %d",
                        firstIndex, mPhonebookBitArray -> getLength());
                goto error;
            } else {
                entry->index = firstIndex;
            }
        }

        requestClearPhbEntry(entry->index);
        if (strlen(number) > 0) {
            char* adnLocalNumber = (char *)alloca(strlen(number) + 1);
            entry->type = SimPhonebookUtils::buildAdnAnrNumber(number, adnLocalNumber, strlen(number));
            adnLocalNumber[strlen(number)] = '\0';
            cmd.append(String8::format("AT+EPBW=%d, \"%s\", %d, \"%s\", 0, %d",
                        entry->index, adnLocalNumber, entry->type, alphaId, encodeMethod));
        } else {
            cmd.append(String8::format("AT+EPBW=%d, \"%s\", %d, \"%s\", 0, %d",
                        entry->index, number, 0x81, alphaId, encodeMethod));
        }
        mPhonebookBitArray -> setBit(entry->index, 1);
    } else {
        // delete
        cmd.append(String8::format("AT+EPBW=%d", entry->index));
        mPhonebookBitArray -> setBit(entry->index, 0);
    }

    p_response = atSendCommand(cmd);
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords EPBW Error!!!!");
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            case CME_SUCCESS:
                ret = RIL_E_INTERNAL_ERR;
                break;
            case CME_SIM_PIN2_REQUIRED:
                ret = RIL_E_SIM_PIN2;
                break;
            case CME_SIM_PUK2_REQUIRED:
                ret = RIL_E_SIM_PUK2;
                break;
            case CME_DIAL_STRING_TOO_LONG:
                ret = RIL_E_DIAL_STRING_TOO_LONG;
                break;
            case CME_TEXT_STRING_TOO_LONG:
                ret = RIL_E_TEXT_STRING_TOO_LONG;
                break;
            case CME_MEMORY_FULL:
                ret = RIL_E_SIM_MEM_FULL;
                break;
            default:
                break;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }
    logD(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords ret %d", ret);
    p_response = NULL;

    if (maxAnrNum == -1) {
        loadUPBCapability();
    }

    // ANR
    if (ret == RIL_E_SUCCESS && maxAnrNum > 0) {
        cmd.clear();
        // at+epbum=<op>,<type>,<adn_index>,<ef_index>,<anr>,<type>,<aas>
        if (entry->adnumber != NULL && strlen(entry->adnumber) > 0) {
            char* anrLocalNumber = (char *)alloca(strlen(entry->adnumber) + 1);
            entry->adtype = SimPhonebookUtils::buildAdnAnrNumber(
                    entry->adnumber, anrLocalNumber, strlen(entry->adnumber));
            anrLocalNumber[strlen(entry->adnumber)] = '\0';
            cmd.append(String8::format("AT+EPBUM=2,0,%d,%d,\"%s\",%d,0", entry->index,
                    maxAnrNum, anrLocalNumber, entry->adtype));
        } else {
            cmd.append(String8::format("AT+EPBUM=3,0,%d,%d", entry->index, maxAnrNum));
        }

        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            logE(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords EPBUM ANR Error!!!!");
        }
        p_response = NULL;
    }

    // EMAIL
    if (ret == RIL_E_SUCCESS && maxEmailNum > 0) {
        cmd.clear();
        if (entry->email != NULL && strlen(entry->email) > 0) {
            // at+epbum=<op>,<type>,<adn_index>,<ef_index>,<email>
            cmd.append(String8::format("AT+EPBUM=2,1,%d,%d,\"%s\"", entry->index,
                    maxEmailNum, entry->email));
        } else {
            cmd.append(String8::format("AT+EPBUM=3,1,%d,%d", entry->index, maxEmailNum));
        }
        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            logE(RFX_LOG_TAG, "requestUpdateSimPhonebookRecords EPBUM Email Error!!!!");
        }
    }

    responseValue[0] = entry->index;
    res = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(responseValue, 1), msg, false);
    responseToTelCore(res);
    if (name != NULL) {
        free(name);
    }
    return;
error:
    if (name != NULL) {
        free(name);
    }
    if (entry != NULL) {
        responseValue[0] = entry->index;
    } else {
        responseValue[0] = -1;
    }
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxIntsData(responseValue, 1), msg, false);
    responseToTelCore(response);
}

void RmcPhbInModemHandler::requestReadSimPhonebookRecords(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxAtResponse> p_res = NULL;
    int err, count, i, tmp;
    int bIndex = -1;
    int eIndex = -1;
    char *out;
    String8 cmd("");
    RfxAtLine *p_cur = NULL;
    RfxAtLine *eline = NULL;
    RIL_PHB_ENTRY *entry = NULL;
    RIL_PHB_ENTRY **entries = NULL;
    int responseData[2] = {0};
    int *data = (int *)msg->getData()->getData();
    sp<RfxMclMessage> response;
    sp<RfxMclMessage> urcIndication = NULL;

    selectPhbStorage(RIL_PHB_ADN);
    if (maxAnrNum == -1) {
        loadUPBCapability();
    }
    logD(RFX_LOG_TAG, "maxAnrNum[%d]=%d", m_slot_id, maxAnrNum);
    if (data != NULL) {
        bIndex = ((int *)data)[0];
        eIndex = ((int *)data)[1];
    }
    if ((eIndex - bIndex + 1) <= 0) {
        logE(RFX_LOG_TAG, "Begin index or End Index is invalid: %d %d", bIndex, eIndex);
        goto error;
    }

    if (bIndex == 1) {
        readAdnRecordsNum = 0;
        if (mPhonebookBitArray == NULL) {
            mPhonebookBitArray = new PhonebookBitArray(maxAdnRecords);
        } else {
            delete mPhonebookBitArray;
            mPhonebookBitArray = new PhonebookBitArray(maxAdnRecords);
        }
    }

    cmd.append(String8::format("AT+CPBR=%d, %d", bIndex, eIndex));
    p_response = atSendCommandMultiline(cmd, "+CPBR:");

    if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
        logE(RFX_LOG_TAG, "requestReadPhoneBookEntryExt CPBR error.");
        goto error;
    }
    /* count the calls */
    for (count = 0, p_cur = p_response->getIntermediates()
            ; p_cur != NULL
            ; p_cur = p_cur->getNext()) {
        count++;
    }
    readAdnRecordsNum = readAdnRecordsNum + count;
    logD(RFX_LOG_TAG, "requestReadPhoneBookEntryExt end count=%d, readcount=%d", count, readAdnRecordsNum);

    // /* allocate memory and parse +CPBR*/
    entries = (RIL_PHB_ENTRY**)calloc(1, sizeof(RIL_PHB_ENTRY*) * count);
    if (entries == NULL) {
        logE(RFX_LOG_TAG, "entries calloc fail");
        goto error;
    }

    // +CPBR: <index>, <number>, <TON>, <alphaId>
    for (i = 0, p_cur = p_response->getIntermediates()
            ; p_cur != NULL
            ; p_cur = p_cur->getNext(), i++) {
        char* localNumber;
        entry = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
        if (entry == NULL) {
            logE(RFX_LOG_TAG, "entry calloc fail");
            goto error;
        }
        entries[i] = entry;
        if (readAdnRecordsNum >= usedAdnRecords) {
            entry->hidden = 1;
        } else {
            entry->hidden = 0;
        }
        entry->index = 0;
        entry->type = 0;
        entry->number = (char*)"";
        entry->text = (char*)"";
        entry->group = (char*)"";
        entry->adnumber = (char*)"";
        entry->adtype = 0;
        entry->secondtext = (char*)"";
        entry->email = (char*)"";

        p_cur->atTokStart(&err);
        if (err < 0) goto error;

        entry->index = p_cur->atTokNextint(&err);
        if (err < 0) goto error;

        out = p_cur->atTokNextstr(&err);
        if (err < 0) goto error;
        localNumber = (char*)alloca(strlen(out) + 1);
        SimPhonebookUtils::transferSpecialAlpaToReal(out, localNumber, strlen(out));
        localNumber[strlen(out)] = '\0';

        entry->type = p_cur->atTokNextint(&err);
        if (err < 0) goto error;
        if (entry->type == 0x91 && strlen(localNumber) > 0) {
            string localStringNumber = localNumber;
            if (!localStringNumber.empty()) {
                string plusNumber = SimPhonebookUtils::prependPlusToNumber(localStringNumber);
                if (!plusNumber.empty()) {
                    entry->number =  (char*)alloca(plusNumber.length() + 1);
                    strncpy(entry->number, plusNumber.c_str(), plusNumber.length());
                    entry->number[plusNumber.length()] = '\0';
                } else {
                    entry->number = (char*)alloca(1);
                    entry->number[0] = '\0';
                }
            } else {
                entry->number = (char*)alloca(1);
                entry->number[0] = '\0';
            }
        } else {
            entry->number = localNumber;
        }

        out = p_cur->atTokNextstr(&err);
        if (err < 0) {
            goto error;
        }

        if (strlen(out) > 0) {
            if (strlen(out) % 4 != 0) {
                goto error;
            }
            char32_t* nameUtf32 = (char32_t*) malloc((strlen(out) / 4) * sizeof(char32_t));
            int resultLen = strlen(out) / 4;
            if (nameUtf32 == NULL) {
                goto error;
            }

            for (int i = 0; i < resultLen; i++) {
                nameUtf32[i] = SimPhonebookUtils::hexCharToInt(out[i * 4 + 0]) * 4096 +
                        SimPhonebookUtils::hexCharToInt(out[i * 4 + 1]) * 256 +
                        SimPhonebookUtils::hexCharToInt(out[i * 4 + 2]) * 16 +
                        SimPhonebookUtils::hexCharToInt(out[i * 4 + 3]);
                logD(RFX_LOG_TAG, "PhbEntry nameUtf32[%d]:%d", i, nameUtf32[i]);
            }

            int utf8Length = utf32_to_utf8_length(nameUtf32, resultLen) + 1;
            if (utf8Length <= 0) {
                free(nameUtf32);
                goto error;
            }
            char* utf8String = (char *)alloca(utf8Length);
            utf32_to_utf8(nameUtf32, resultLen, utf8String, utf8Length);
            utf8String[utf8Length -1] = '\0';
            for (int i = 0; i < strlen(utf8String); i++) {
                logD(RFX_LOG_TAG, "PhbEntry utf8String[%d]:%d", i, utf8String[i]);
            }
            entry->text = (char*)alloca(utf8Length);
            strncpy(entry->text, utf8String, utf8Length - 1);
            entry->text[utf8Length - 1] = '\0';
            free(nameUtf32);
        } else {
            entry->text = (char*)alloca(1);
            entry->text[0] = '\0';
        }

        logD(RFX_LOG_TAG, "PhbEntry %d: %s, %d, %s", entry->index,
                entry->number, entry->type, entry->text);

        mPhonebookBitArray -> setBit(entry->index, 1);

        // ANR
        if (maxAnrNum > 0 && (strlen(entry->text) > 0 || strlen(entry->number) > 0)) {
            cmd.clear();
            cmd.append(String8::format("AT+EPBUM=1,0,%d,%d", entry->index, maxAnrNum));
            // +epbum:<type>,<adn_index>,<ef_index>,<anr>,<type>,<aas>
            p_res = atSendCommandSingleline(cmd, "+EPBUM:");
            if (p_res == NULL || p_res->getError() < 0) {
                logE(RFX_LOG_TAG, "requestReadPhoneBookEntryExt anr epbum error.");
            } else if (p_res->getSuccess() != 0 && p_res->getIntermediates() != NULL) {
                char *localAdNumber;
                eline = p_res->getIntermediates();
                eline->atTokStart(&err);
                tmp = eline->atTokNextint(&err);
                tmp = eline->atTokNextint(&err);
                tmp = eline->atTokNextint(&err);
                out = eline->atTokNextstr(&err);

                localAdNumber = (char*)alloca(strlen(out) + 1);
                SimPhonebookUtils::transferSpecialAlpaToReal(out, localAdNumber, strlen(out));
                localAdNumber[strlen(out)] = '\0';

                entry->adtype = eline->atTokNextint(&err);
                if (entry->adtype == 0x91 && strlen(localAdNumber) > 0) {
                    string localStringAnrNumber = localAdNumber;
                    if (!localStringAnrNumber.empty()) {
                        string plusAnrNumber = SimPhonebookUtils::prependPlusToNumber(localStringAnrNumber);
                        if (!plusAnrNumber.empty()) {
                            entry->adnumber = (char*)alloca(plusAnrNumber.length() + 1);
                            strncpy(entry->adnumber, plusAnrNumber.c_str(), plusAnrNumber.length());
                            entry->adnumber[plusAnrNumber.length()] = '\0';
                        } else {
                            entry->adnumber = (char*)alloca(1);
                            entry->adnumber[0] = '\0';
                        }
                    } else {
                        entry->adnumber = (char*)alloca(1);
                        entry->adnumber[0] = '\0';
                    }
                } else {
                    entry->adnumber = localAdNumber;
                }
                logD(RFX_LOG_TAG, "PhbEntry anr %d: %s, %d", entry->index,
                              entry->adnumber, entry->adtype);
            }
        }
        // email
        if (maxEmailNum > 0 && (strlen(entry->text) > 0 || strlen(entry->number) > 0)) {
            cmd.clear();
            cmd.append(String8::format("AT+EPBUM=1,1,%d,%d", entry->index, maxEmailNum));
            p_res = NULL;

            // +epbum:<type>,<adn_index>,<ef_index>,<email>
            p_res = atSendCommandSingleline(cmd, "+EPBUM:");
            if (p_res == NULL || p_res->getError() < 0) {
                logE(RFX_LOG_TAG, "requestReadPhoneBookEntryExt email epbum err");
            } else if (p_res->getSuccess() != 0 && p_res->getIntermediates() != NULL) {
                eline = p_res->getIntermediates();
                eline->atTokStart(&err);
                tmp = eline->atTokNextint(&err);
                tmp = eline->atTokNextint(&err);
                tmp = eline->atTokNextint(&err);
                out = eline->atTokNextstr(&err);
                entry->email = (char*)alloca(strlen(out) + 1);
                strncpy(entry->email, out, strlen(out) + 1);
                logD(RFX_LOG_TAG, "PhbEntry email %d: %s", entry->index, entry->email);
            }
        }
    }
    responseData[0] = eIndex;
    responseData[1] = count;
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
          RfxIntsData(responseData, 2), msg, false);
    responseToTelCore(response);

    if (count > 0) {
        urcIndication = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_PHONEBOOK_RECORDS_RECEIVED, m_slot_id,
                RfxPhbEntriesExtData(entries, count));
        responseToTelCore(urcIndication);
    }

    for (i = 0; i < count; i++) {
        if (entries[i] != NULL) {
            free(entries[i]);
        }
    }
    free(entries);
    return;

error:
    logE(RFX_LOG_TAG, "requestReadPhoneBookEntryExt error");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_INTERNAL_ERR,
            RfxVoidData(), msg, false);
    responseToTelCore(response);

    urcIndication = RfxMclMessage::obtainUrc(RFX_MSG_URC_SIM_PHONEBOOK_RECORDS_RECEIVED, m_slot_id,
            RfxVoidData());
    responseToTelCore(urcIndication);

    if (entries != NULL) {
      for (i=0; i < count; i++) {
          if (entries[i] != NULL) {
              free(entries[i]);
          }
      }
      free(entries);
    }
}

void RmcPhbInModemHandler::requestGetSimPhonebookCapacity(const sp<RfxMclMessage>& msg) {
    int query_info[10] = {0};
    sp<RfxAtResponse> p_response = NULL;
    int err;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    RfxAtLine* line = NULL;
    char *out;
    char * temStr;
    sp<RfxMclMessage> response;
    int tem;

    if (selectPhbStorage(RIL_PHB_ADN)) {
        p_response = atSendCommandSingleline("AT+CPBS?", "+CPBS:");
        if (p_response == NULL || p_response->getError() < 0) {
            goto error;
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError());
                case CME_SUCCESS:
                    ret = RIL_E_INTERNAL_ERR;
                    goto error;
                    break;
                default:
                    goto error;
            }
        } else {
            ret = RIL_E_SUCCESS;
        }

        // +CPBS: <STORAGE>, <used>, <total>
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        if (err < 0) goto error;

        out = line->atTokNextstr(&err);
        if (err < 0) goto error;
        // usedAdnRecords
        query_info[1] = line->atTokNextint(&err);
        usedAdnRecords = query_info[1];
        if (err < 0) goto error;
        // maxAdnRecords
        query_info[0] = line->atTokNextint(&err);
        maxAdnRecords = query_info[0];
        if (err < 0) goto error;

        p_response = atSendCommandSingleline("AT+CPBR=?", "+CPBR:");
        if (p_response == NULL || p_response->getError() < 0) {
            logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength Fail");
            goto error;
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError() );
                case CME_SUCCESS:
                    ret = RIL_E_INTERNAL_ERR;
                    break;
                case CME_UNKNOWN:
                    logE(RFX_LOG_TAG, "p_response: CME_UNKNOWN");
                    break;
                default:
                    break;
            }
        } else {
            ret = RIL_E_SUCCESS;
        }

        if (ret != RIL_E_SUCCESS) {
            logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: Error!");
            goto error;
        }

        line = p_response->getIntermediates();
        line->atTokStart(&err);
        if (err < 0) goto error;
        // +CPBR: (<bIndex>-<eIndex>), <max_num_len>, <max_alpha_len>
        temStr = line->atTokNextstr(&err);
        if (err < 0) goto error;
        // maxNumberLen
        query_info[7] = line->atTokNextint(&err);
        if (err < 0) goto error;
        // maxNameLen
        query_info[6] = line->atTokNextint(&err);
        if (err < 0) goto error;

        // email
        // query the info +EPBUM:<type>, <INDEX1>, <M_NUM>, <A_NUM>, <L_XXX>
        p_response = atSendCommandSingleline("AT+EPBUM=0,1,1", "+EPBUM:");
        if (p_response != NULL && p_response->getError() >= 0 && p_response->getSuccess() > 0) {
            line = p_response->getIntermediates();
            line->atTokStart(&err);
            tem = line->atTokNextint(&err);
            tem = line->atTokNextint(&err);
            query_info[2] = line->atTokNextint(&err);
            query_info[3] = line->atTokNextint(&err);
            query_info[8] = line->atTokNextint(&err);
        } else {
            goto error;
        }

        // anr
        p_response = atSendCommandSingleline("AT+EPBUM=0,0,1", "+EPBUM:");
        if (p_response != NULL && p_response->getError() >= 0 && p_response->getSuccess() > 0) {
            line = p_response->getIntermediates();
            line->atTokStart(&err);
            tem = line->atTokNextint(&err);
            tem = line->atTokNextint(&err);
            query_info[4] = line->atTokNextint(&err);
            query_info[5] = line->atTokNextint(&err);
            query_info[9] = line->atTokNextint(&err);
        } else {
            goto error;
        }

        logE(RFX_LOG_TAG, "requestGetSimPhonebookCapacity:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                query_info[0], query_info[1], query_info[2], query_info[3], query_info[4],
                query_info[5], query_info[6], query_info[7], query_info[8], query_info[9]);

        sp<RfxMclMessage> res = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
               RfxIntsData(query_info, 10), msg, false);
        responseToTelCore(res);
    }
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_INTERNAL_ERR,
            RfxIntsData(query_info, 10), msg, false);
    responseToTelCore(response);
}
