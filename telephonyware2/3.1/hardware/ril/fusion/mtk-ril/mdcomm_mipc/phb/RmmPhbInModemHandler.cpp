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
#include "RmmPhbInModemHandler.h"
#include "RfxPhbEntryExtData.h"
#include "RfxPhbEntriesExtData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include <libmtkrilutils.h>
#include <SimPhonebookUtils.h>
#include <utils/Unicode.h>
using ::android::String8;


RFX_IMPLEMENT_HANDLER_CLASS(RmmPhbInModemHandler,  RIL_CMD_PROXY_10);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_SIM_PHONEBOOK_CAPACITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxPhbEntryExtData, RfxIntsData, RFX_MSG_REQUEST_UPDATE_SIM_PHONEBOOK_RECORDS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_LOCAL_REQUEST_GET_SIM_PHONEBOOK_RECORDS);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_PHB_STATE_CHANGED);


RmmPhbInModemHandler::RmmPhbInModemHandler(int slot_id, int channel_id) :
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

RmmPhbInModemHandler::~RmmPhbInModemHandler() {
    if (mPhonebookBitArray != NULL) {
        delete mPhonebookBitArray;
    }
}

void RmmPhbInModemHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
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

void RmmPhbInModemHandler::clearPhbStorageInfo(void) {
    current_phb_storage = -1;
}

void RmmPhbInModemHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
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

mipc_phb_stroage_type_const_enum RmmPhbInModemHandler::getPhbStorageType(int type) {
    mipc_phb_stroage_type_const_enum mipcType = MIPC_PHB_STROAGE_TYPE_SM;
    switch (type) {
        case RIL_PHB_ADN:
            mipcType = MIPC_PHB_STROAGE_TYPE_SM;
            break;
        case RIL_PHB_FDN:
            mipcType = MIPC_PHB_STROAGE_TYPE_FD;
            break;
        case RIL_PHB_MSISDN:
            mipcType = MIPC_PHB_STROAGE_TYPE_ON;
            break;
    }

    return mipcType;
}

int RmmPhbInModemHandler::selectPhbStorage(int type) {
    mipc_phb_stroage_type_const_enum storage;
    int result = 1;

    if (current_phb_storage != type) {
        storage = getPhbStorageType(type);
        sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_PHB_SET_PHB_MEM_STORAGE_REQ, m_slot_id);
        initMipc->addMipcTlvUint32(MIPC_PHB_SET_PHB_MEM_STORAGE_REQ_T_STORAGE_TYPE, storage);
        sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);

        if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
            result = 0;
            if (cnfMipc != NULL) {
                logD(RFX_LOG_TAG, "Select the Storage: error:%d", cnfMipc->getResult());
            }
        } else {
            current_phb_storage = type;
        }
    }
    logD(RFX_LOG_TAG, "Select the Storage: %d, %d, %d", m_slot_id, result, type);
    return result;
}

void RmmPhbInModemHandler::requestClearPhbEntry(int index) {
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

void RmmPhbInModemHandler:: loadUPBCapability() {
    int upbCap[8]= {0};

    sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_PHB_GET_UPB_CAPABILITY_REQ, m_slot_id);
    sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
    if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "loadUPBCapability Fail:%d", cnfMipc->getResult());
        goto error;
    }

    // number of supported ANR for a ADN entry
    upbCap[0] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_ANR, 0);
    // number of supported EMAIL for a ADN entry
    upbCap[1] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_EMAIL, 0);
    // number of supported SNE for a ADN entry
    upbCap[2] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_SNE, 0);
    // maximum number of AAS entries
    upbCap[3] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_AAS, 0);
    // maximum length of the AAS in the entries
    upbCap[4] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_LEN_AAS, 0);
    // maximum number of GAS entries
    upbCap[5] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_GAS, 0);
    // maximum length of the GAS in the entries
    upbCap[6] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_LEN_GAS, 0);
    // maximum number of GRP entries
    upbCap[7] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_GRP, 0);

    if (m_slot_id >= 0 && m_slot_id < 4) {
        maxGrpNum = upbCap[7];
        maxAnrNum = upbCap[0];
        maxEmailNum = upbCap[1];
    }

    logD(RFX_LOG_TAG, "loadUPBCapability: %d, %d, %d, %d, %d, %d, %d, %d",
            upbCap[0], upbCap[1], upbCap[2], upbCap[3], upbCap[4], upbCap[5],
            upbCap[6], upbCap[7]);
    return;
error:
    logE(RFX_LOG_TAG, "loadUPBCapability: Error!");
}


RIL_Errno RmmPhbInModemHandler::mipcResultToRilResult(unsigned int result) {
    RIL_Errno ret = RIL_E_INTERNAL_ERR;
    switch (result) {
      case MIPC_RESULT_PHB_EXT_PUK2_REQUIRED:
            ret = RIL_E_SIM_PUK2;
            break;
        case MIPC_RESULT_PHB_EXT_PIN2_REQUIRED:
            ret = RIL_E_SIM_PIN2;
            break;
        case MIPC_RESULT_MEMORY_FULL:
            ret = RIL_E_SIM_MEM_FULL;
            break;
        case MIPC_RESULT_PHB_EXT_DIAL_STRING_TOO_LONG:
            ret = RIL_E_DIAL_STRING_TOO_LONG;
            break;
      case MIPC_RESULT_PHB_EXT_TEXT_STRING_TOO_LONG:
            ret = RIL_E_TEXT_STRING_TOO_LONG;
            break;
        default:
            break;
    }
    return ret;
}

void RmmPhbInModemHandler::requestUpdateSimPhonebookRecords(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxMclMessage> res = NULL;
    int responseValue[1] = {0};
    int tmp;
    size_t len, i, j;
    RIL_PHB_ENTRY *entry;
    char *line, *number;
    String8 cmd("");
    char* name = NULL;
    sp<RfxMipcData> mipcMsg;
    sp<RfxMipcData> mipcCnf;
    mipc_phb_entry_struct4* phbEntry = NULL;
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

        mipcMsg = RfxMipcData::obtainMipcData(MIPC_PHB_SET_PHB_ENTRY_REQ, m_slot_id);
        mipcMsg->addMipcTlvUint8(MIPC_PHB_SET_PHB_ENTRY_REQ_T_EXT, 0);
        phbEntry = (mipc_phb_entry_struct4*)calloc(1, sizeof(mipc_phb_entry_struct4));
        if (phbEntry == NULL) {
            logE(RFX_LOG_TAG, "calloc phbEntry memory fail!");
            goto error;
        }
        phbEntry->index = entry->index;

        if (strlen(number) > 0) {
            char* adnLocalNumber = (char *)alloca(strlen(number) + 1);
            entry->type = SimPhonebookUtils::buildAdnAnrNumber(number, adnLocalNumber, strlen(number));
            adnLocalNumber[strlen(number)] = '\0';
            memcpy((void*)phbEntry->number, (void*)adnLocalNumber,
                    strlen(adnLocalNumber) > 40 ? 40 : strlen(adnLocalNumber));
            phbEntry->ton = entry->type;
        } else {
            phbEntry->ton = 0x81;
            phbEntry->number[0] = '\0';
        }

        alphaId[RIL_MAX_PHB_NAME_LEN * 4 + 4]= '\0';
        memcpy((void*)phbEntry->alphaID, (void*)alphaId, strlen(alphaId) > 168 ? 168 : strlen(alphaId));
        phbEntry->encode_method = encodeMethod;

        // cmd.append(String8::format("AT+EPBW=%d, \"%s\", %d, \"%s\", 0, %d",
        //            entry->index, number, entry->ton, alphaId, encodeMethod));
        logD(RFX_LOG_TAG, "requestWritePhbEntry index:%d, number:%s, ton:%d, alphaId:%s, encodeMethod:%d",
                phbEntry->index, phbEntry->number, phbEntry->ton, phbEntry->alphaID, phbEntry->encode_method);

        mipcMsg->addMipcTlv(
                MIPC_PHB_SET_PHB_ENTRY_REQ_T_ENTRY, sizeof(mipc_phb_entry_struct4), (void*)phbEntry);
        mPhonebookBitArray -> setBit(entry->index, 1);
    } else {
        // delete
        // cmd.append(String8::format("AT+EPBW=%d", entry->index));
        mipcMsg = RfxMipcData::obtainMipcData(MIPC_PHB_SET_PHB_ENTRY_REQ, m_slot_id);
        mipcMsg->addMipcTlvUint8(MIPC_PHB_SET_PHB_ENTRY_REQ_T_EXT, 0);
        phbEntry = (mipc_phb_entry_struct4*)calloc(1, sizeof(mipc_phb_entry_struct4));
        if (phbEntry == NULL) {
            logE(RFX_LOG_TAG, "calloc phbEntry memory fail!!");
            goto error;
        }
        phbEntry->index = entry->index;
        logD(RFX_LOG_TAG, "requestDeletePhbEntry index:%d", phbEntry->index);
        mipcMsg->addMipcTlv(
                MIPC_PHB_SET_PHB_ENTRY_REQ_T_ENTRY, sizeof(mipc_phb_entry_struct4), (void*)phbEntry);
        mPhonebookBitArray -> setBit(entry->index, 0);
    }

    mipcCnf = callToMipcMsgSync(mipcMsg);
    free(phbEntry);

    if (mipcCnf == NULL || mipcCnf->getResult() != MIPC_RESULT_SUCCESS) {
        logE(RFX_LOG_TAG, "EPBW Error!!!!");
        if (mipcCnf != NULL) {
            ret = mipcResultToRilResult(mipcCnf->getResult());
        }
        goto error;
    } else {
        ret = RIL_E_SUCCESS;
    }

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

void RmmPhbInModemHandler::requestReadSimPhonebookRecords(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxAtResponse> p_res = NULL;
    int count, i, tmp;
    int bIndex = -1;
    int eIndex = -1;
    String8 cmd("");
    RIL_PHB_ENTRY *entry = NULL;
    RIL_PHB_ENTRY **entries = NULL;
    int responseData[2] = {0};
    int *data = (int *)msg->getData()->getData();
    sp<RfxMclMessage> response;
    sp<RfxMclMessage> urcIndication = NULL;
    mipc_phb_entry_struct4* phbEntry;
    mipc_phb_entry_struct4* phbEntrys;
    uint16_t system_len = 0;
    sp<RfxMipcData> initMipc;
    sp<RfxMipcData> cnfMipc;

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

    initMipc = RfxMipcData::obtainMipcData(MIPC_PHB_GET_PHB_ENTRY_REQ, m_slot_id);
    initMipc->addMipcTlvUint32(MIPC_PHB_GET_PHB_ENTRY_REQ_T_BINDEX, bIndex);
    initMipc->addMipcTlvUint32(MIPC_PHB_GET_PHB_ENTRY_REQ_T_EINDEX, eIndex);
    initMipc->addMipcTlvUint8(MIPC_PHB_GET_PHB_ENTRY_REQ_T_EXT, 0);
    cnfMipc = callToMipcMsgSync(initMipc);

    if (cnfMipc == NULL || cnfMipc->getResult() != MIPC_RESULT_SUCCESS) {
        goto error;
    }

    /* count the calls */
    count = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_COUNT, 0);
    logD(RFX_LOG_TAG, "requestReadSimPhonebookRecords count: begin index:%d, end index:%d, count:%d",
            bIndex, eIndex, count);

    readAdnRecordsNum = readAdnRecordsNum + count;
    logD(RFX_LOG_TAG, "requestReadSimPhonebookRecords end count=%d, readcount=%d", count, readAdnRecordsNum);

    // /* allocate memory and parse +CPBR*/
    entries = (RIL_PHB_ENTRY**)calloc(1, sizeof(RIL_PHB_ENTRY*) * count);
    if (entries == NULL) {
        logE(RFX_LOG_TAG, "entries calloc fail");
        goto error;
    }

    phbEntrys = (mipc_phb_entry_struct4*)cnfMipc->getMipcVal(
            MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_LIST, &system_len);

    // +CPBR: <index>, <number>, <TON>, <alphaId>
    for (i = 0; i < count; i++) {
        char* localNumber;
        char* localNumberAlphaId;
        entries[i] = entry = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
        phbEntry = &(phbEntrys[i]);

        if (entry == NULL) {
            logE(RFX_LOG_TAG, "entry calloc fail");
            goto error;
        }

        if (readAdnRecordsNum >= usedAdnRecords) {
            entry->hidden = 1;
        } else {
            entry->hidden = 0;
        }

        entry->type = phbEntry->ton;
        entry->index = phbEntry->index;

        localNumber = (char*)alloca(40 + 1);
        SimPhonebookUtils::transferSpecialAlpaToReal(phbEntry->number, localNumber, 40);
        localNumber[40] = '\0';

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

        logD(RFX_LOG_TAG, "requestReadPhbEntry index:%d, number:%s, name:%s",
                i, entry->number, phbEntry->alphaID);

        if (strlen(phbEntry->alphaID) > 0) {
            localNumberAlphaId = (char*)alloca(168 + 1);
            strncpy(localNumberAlphaId, phbEntry->alphaID, 168);
            localNumberAlphaId[168] = '\0';

            if (strlen(localNumberAlphaId) % 4 != 0) {
                goto error;
            }
            char32_t* nameUtf32 = (char32_t*) malloc((strlen(localNumberAlphaId) / 4) * sizeof(char32_t));
            int resultLen = strlen(localNumberAlphaId) / 4;
            if (nameUtf32 == NULL) {
                goto error;
            }

            for (int i = 0; i < resultLen; i++) {
                nameUtf32[i] = SimPhonebookUtils::hexCharToInt(localNumberAlphaId[i * 4 + 0]) * 4096 +
                        SimPhonebookUtils::hexCharToInt(localNumberAlphaId[i * 4 + 1]) * 256 +
                        SimPhonebookUtils::hexCharToInt(localNumberAlphaId[i * 4 + 2]) * 16 +
                        SimPhonebookUtils::hexCharToInt(localNumberAlphaId[i * 4 + 3]);
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
            // +epbum:<type>,<adn_index>,<ef_index>,<anr>,<type>,<aas>
            uint16_t t_system_len = 0;
            char* localAnrNum;
            char* localAdNumber;
            sp<RfxMipcData> mipcMsg;
            sp<RfxMipcData> mipcCnf;
             mipc_phb_anr_entry_struct4* anrEntry;

            mipcMsg = RfxMipcData::obtainMipcData(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ, m_slot_id);
            mipcMsg->addMipcTlvUint32(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_TYPE, MIPC_PHB_EF_FILE_TYPE_EF_ANR);
            mipcMsg->addMipcTlvUint32(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_ADN_INDEX, entry->index);
            mipcMsg->addMipcTlvUint32(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_EF_ENTRY_INDEX, 1);
            mipcCnf = callToMipcMsgSync(mipcMsg);
            if (mipcCnf == NULL || mipcCnf->getResult() != MIPC_RESULT_SUCCESS) {
                goto error;
            }

            anrEntry = (mipc_phb_anr_entry_struct4*)mipcCnf->getMipcVal(
                    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_PHB_ENTRY, &t_system_len);

            localAnrNum = (char*)alloca(40 + 1);
            strncpy(localAnrNum, anrEntry->number, 40);
            localAnrNum[40] = '\0';
            entry->adtype = anrEntry->ton;

            localAdNumber = (char*)alloca(strlen(localAnrNum) + 1);
            SimPhonebookUtils::transferSpecialAlpaToReal(localAnrNum, localAdNumber, strlen(localAnrNum));
            localAdNumber[strlen(localAnrNum)] = '\0';

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
        // email
        if (maxEmailNum > 0 && (strlen(entry->text) > 0 || strlen(entry->number) > 0)) {
            sp<RfxMipcData> mipcMsg;
            sp<RfxMipcData> mipcCnf;
            mipc_phb_email_str_struct4* emailEntry;
            uint16_t t_system_len = 0;

            mipcMsg = RfxMipcData::obtainMipcData(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ, m_slot_id);
            mipcMsg->addMipcTlvUint32(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_TYPE, MIPC_PHB_EF_FILE_TYPE_EF_EMAIL);
            mipcMsg->addMipcTlvUint32(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_ADN_INDEX, entry->index);
            mipcMsg->addMipcTlvUint32(MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_EF_ENTRY_INDEX, 1);

            mipcCnf = callToMipcMsgSync(mipcMsg);
            if (mipcCnf == NULL || mipcCnf->getResult() != MIPC_RESULT_SUCCESS) {
                goto error;
            }

            emailEntry = (mipc_phb_email_str_struct4*)mipcCnf->getMipcVal(
                    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_EMAIL, &t_system_len);
            tmp = emailEntry->encode_method;
            entry->email = (char*)alloca(248 + 1);
            strncpy(entry->email, emailEntry->name_str, 248);
            entry->email[248] = '\0';
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

mipc_phb_ef_file_type_const_enum RmmPhbInModemHandler::convertToMipcPhbEfFileType(int type) {
    mipc_phb_ef_file_type_const_enum mipcType = mipc_phb_ef_file_type_const_NONE;
    switch (type) {
        case 0:
            mipcType = MIPC_PHB_EF_FILE_TYPE_EF_ANR;
            break;
        case 1:
            mipcType = MIPC_PHB_EF_FILE_TYPE_EF_EMAIL;
            break;
        case 2:
            mipcType = MIPC_PHB_EF_FILE_TYPE_EF_SNE;
            break;
        case 3:
            mipcType = MIPC_PHB_EF_FILE_TYPE_EF_AAS;
            break;
        case 4:
            mipcType = MIPC_PHB_EF_FILE_TYPE_EF_GAS;
            break;
        case 5:
            mipcType = MIPC_PHB_EF_FILE_TYPE_EF_GRP;
        break;
        default:
            break;
    }

    return mipcType;
}


void RmmPhbInModemHandler::requestGetSimPhonebookCapacity(const sp<RfxMclMessage>& msg) {
    int query_info[10] = {0};
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> response;

    if (selectPhbStorage(RIL_PHB_ADN)) {
        sp<RfxMipcData> initMipc = RfxMipcData::obtainMipcData(MIPC_PHB_GET_PHB_STORAGE_INFO_REQ, m_slot_id);
        sp<RfxMipcData> cnfMipc = callToMipcMsgSync(initMipc);
        if (cnfMipc != NULL && cnfMipc->getResult() == MIPC_RESULT_SUCCESS) {
            ret = RIL_E_SUCCESS;
        } else {
            logE(RFX_LOG_TAG, "requestGetSimPhonebookCapacity Fail:%d", cnfMipc->getResult());
            goto error;
        }
        // +CPBS: <STORAGE>, <used>, <total>
        // usedAdnRecords
        query_info[1] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_USED, 0);
        usedAdnRecords = query_info[1];
        // maxAdnRecords
        query_info[0] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_TOTAL, 0);
        maxAdnRecords = query_info[0];
        // +CPBR: (<bIndex>-<eIndex>), <max_num_len>, <max_alpha_len>
        query_info[7] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_NLENGTH, 0);
        query_info[6] = cnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_TLENGTH, 0);

        // email
        // query the info +EPBUM:<type>, <INDEX1>, <M_NUM>, <A_NUM>, <L_XXX>
        mipc_phb_ef_file_type_const_enum emailFileType = convertToMipcPhbEfFileType(1);

        sp<RfxMipcData> emailInitMipc = RfxMipcData::obtainMipcData(MIPC_PHB_GET_PHB_AVAILABLE_REQ, m_slot_id);
        emailInitMipc->addMipcTlvUint32(MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_TYPE, emailFileType);
        emailInitMipc->addMipcTlvUint32(MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_INDEX, 1);
        sp<RfxMipcData> emailCnfMipc = callToMipcMsgSync(emailInitMipc);
        if (cnfMipc != NULL && emailCnfMipc->getResult() == MIPC_RESULT_SUCCESS) {
            query_info[2] = emailCnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_NUM, 0);
            query_info[3] = emailCnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_AVAILABLE_NUM, 0);
            query_info[8] = emailCnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_LEN, 0);
        } else {
            goto error;
        }

        // anr
        mipc_phb_ef_file_type_const_enum anrFileType = convertToMipcPhbEfFileType(0);

        sp<RfxMipcData> anrInitMipc = RfxMipcData::obtainMipcData(MIPC_PHB_GET_PHB_AVAILABLE_REQ, m_slot_id);
        anrInitMipc->addMipcTlvUint32(MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_TYPE, anrFileType);
        anrInitMipc->addMipcTlvUint32(MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_INDEX, 1);
        sp<RfxMipcData> anrCnfMipc = callToMipcMsgSync(emailInitMipc);
        if (cnfMipc != NULL && cnfMipc->getResult() == MIPC_RESULT_SUCCESS) {
            query_info[4] = anrCnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_NUM, 0);
            query_info[5] = anrCnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_AVAILABLE_NUM, 0);
            query_info[9] = anrCnfMipc->getMipcUint32Val(MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_LEN, 0);
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
