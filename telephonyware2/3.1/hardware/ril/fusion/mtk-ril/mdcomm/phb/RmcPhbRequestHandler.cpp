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
#include "RmcPhbRequestHandler.h"
#include <telephony/mtk_ril.h>
#include "RfxPhbEntryData.h"
#include "RfxPhbEntriesData.h"
#include "RfxPhbEntryExtData.h"
#include "RfxPhbEntriesExtData.h"
#include "RfxPhbMemStorageData.h"
#include "RfxIntsData.h"
#include "RfxStringsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxMessageId.h"
#include <libmtkrilutils.h>
using ::android::String8;

RFX_IMPLEMENT_HANDLER_CLASS(RmcPhbRequestHandler, RIL_CMD_PROXY_1);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_QUERY_PHB_STORAGE_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxPhbEntryData, RfxIntsData, RFX_MSG_REQUEST_WRITE_PHB_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxPhbEntriesData, RFX_MSG_REQUEST_READ_PHB_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_QUERY_UPB_CAPABILITY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_EDIT_UPB_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_DELETE_UPB_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringsData, RFX_MSG_REQUEST_READ_UPB_GAS_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_READ_UPB_GRP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_WRITE_UPB_GRP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_GET_PHB_STRING_LENGTH);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxPhbMemStorageData, RFX_MSG_REQUEST_GET_PHB_MEM_STORAGE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData, RFX_MSG_REQUEST_SET_PHB_MEM_STORAGE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxPhbEntriesExtData, RFX_MSG_REQUEST_READ_PHB_ENTRY_EXT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxPhbEntryExtData, RfxVoidData, RFX_MSG_REQUEST_WRITE_PHB_ENTRY_EXT);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_QUERY_UPB_AVAILABLE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringData, RFX_MSG_REQUEST_READ_EMAIL_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringData, RFX_MSG_REQUEST_READ_SNE_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxPhbEntriesData, RFX_MSG_REQUEST_READ_ANR_ENTRY);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxStringsData, RFX_MSG_REQUEST_READ_UPB_AAS_LIST);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_PHB_CURRENT_STORAGE_RESET);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_PHB_CURRENT_STORAGE_RESET);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_SET_PHONEBOOK_READY);

#define PROPERTY_RIL_PHB_READY "vendor.gsm.sim.ril.phbready"

RmcPhbRequestHandler::RmcPhbRequestHandler(int slot_id, int channel_id) :
        RfxBaseHandler(slot_id, channel_id) {
    const int request1[] = {
        RFX_MSG_REQUEST_QUERY_PHB_STORAGE_INFO,
        RFX_MSG_REQUEST_WRITE_PHB_ENTRY,
        RFX_MSG_REQUEST_READ_PHB_ENTRY,
        RFX_MSG_REQUEST_QUERY_UPB_CAPABILITY,
        RFX_MSG_REQUEST_EDIT_UPB_ENTRY,
        RFX_MSG_REQUEST_DELETE_UPB_ENTRY,
        RFX_MSG_REQUEST_READ_UPB_GAS_LIST,
        RFX_MSG_REQUEST_READ_UPB_GRP,
        RFX_MSG_REQUEST_WRITE_UPB_GRP,
        RFX_MSG_REQUEST_GET_PHB_STRING_LENGTH,
        RFX_MSG_REQUEST_GET_PHB_MEM_STORAGE,
        RFX_MSG_REQUEST_SET_PHB_MEM_STORAGE,
        RFX_MSG_REQUEST_READ_PHB_ENTRY_EXT,
        RFX_MSG_REQUEST_WRITE_PHB_ENTRY_EXT,
        RFX_MSG_REQUEST_QUERY_UPB_AVAILABLE,
        RFX_MSG_REQUEST_READ_EMAIL_ENTRY,
        RFX_MSG_REQUEST_READ_SNE_ENTRY,
        RFX_MSG_REQUEST_READ_ANR_ENTRY,
        RFX_MSG_REQUEST_READ_UPB_AAS_LIST,
        RFX_MSG_REQUEST_PHB_CURRENT_STORAGE_RESET,
        RFX_MSG_REQUEST_SET_PHONEBOOK_READY,
    };
    const int event[] = {
        RFX_MSG_EVENT_PHB_CURRENT_STORAGE_RESET,
    };
    registerToHandleRequest(request1, sizeof(request1)/sizeof(int));
    registerToHandleEvent(event, sizeof(event)/sizeof(int));
    mIsUserLoad = RfxRilUtils::isUserLoad();
}

RmcPhbRequestHandler::~RmcPhbRequestHandler() {
}

void RmcPhbRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();

    if (RFX_MSG_REQUEST_SET_PHONEBOOK_READY == request) {
        requestSetPhonebookReady(msg);
        return;
    }

    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
    if (cardType <= 0 && request != RFX_MSG_REQUEST_PHB_CURRENT_STORAGE_RESET) {
        logE(RFX_LOG_TAG, "onHandleRequest No SIM inserted, cardType: %d, request: %d", cardType, request);
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(request, RIL_E_GENERIC_FAILURE,
                RfxVoidData(), msg, false);
        responseToTelCore(response);
        return;
    }

    switch (request) {
        // PHB Start
        case RFX_MSG_REQUEST_QUERY_PHB_STORAGE_INFO:
            requestQueryPhbInfo(msg);
            break;
        case RFX_MSG_REQUEST_WRITE_PHB_ENTRY:
            requestWritePhbEntry(msg);
            break;
        case RFX_MSG_REQUEST_READ_PHB_ENTRY:
            requestReadPhbEntry(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_UPB_CAPABILITY:
            requestQueryUPBCapability(msg);
            break;
        case RFX_MSG_REQUEST_EDIT_UPB_ENTRY:
            requestEditUPBEntry(msg);
            break;
        case RFX_MSG_REQUEST_DELETE_UPB_ENTRY:
            requestDeleteUPBEntry(msg);
            break;
        case RFX_MSG_REQUEST_READ_UPB_GAS_LIST:
            requestReadGasList(msg);
            break;
        case RFX_MSG_REQUEST_READ_UPB_GRP:
            requestReadUpbGrpEntry(msg);
            break;
        case RFX_MSG_REQUEST_WRITE_UPB_GRP:
            requestWriteUpbGrpEntry(msg);
            break;
        case RFX_MSG_REQUEST_GET_PHB_STRING_LENGTH:
            requestGetPhoneBookStringsLength(msg);
            break;
        case RFX_MSG_REQUEST_GET_PHB_MEM_STORAGE:
            requestGetPhoneBookMemStorage(msg);
            break;
        case RFX_MSG_REQUEST_SET_PHB_MEM_STORAGE:
            requestSetPhoneBookMemStorage(msg);
            break;
        case RFX_MSG_REQUEST_READ_PHB_ENTRY_EXT:
            requestReadPhoneBookEntryExt(msg);
            break;
        case RFX_MSG_REQUEST_WRITE_PHB_ENTRY_EXT:
            requestWritePhoneBookEntryExt(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_UPB_AVAILABLE:
            requestQueryUPBAvailable(msg);
            break;
        case RFX_MSG_REQUEST_READ_EMAIL_ENTRY:
            requestReadUPBEmail(msg);
            break;
        case RFX_MSG_REQUEST_READ_SNE_ENTRY:
            requestReadUPBSne(msg);
            break;
        case RFX_MSG_REQUEST_READ_ANR_ENTRY:
            requestReadUPBAnr(msg);
            break;
        case RFX_MSG_REQUEST_READ_UPB_AAS_LIST:
            requestReadAasList(msg);
            break;
        case RFX_MSG_REQUEST_PHB_CURRENT_STORAGE_RESET:
            requestResetPhbStorage(msg);
            break;
        default:  /* no match */
            break;
    }
}

void RmcPhbRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    switch (id) {
        case RFX_MSG_EVENT_PHB_CURRENT_STORAGE_RESET:
            resetPhbStorage();
            break;
        default:
            logE(RFX_LOG_TAG, "RmcPhbRequestHandler::onHandleEvent: unknown message id: %d", id);
            break;
    }
}

int RmcPhbRequestHandler::selectPhbStorage(int type) {
    const char *storage;
    int result = 1;
    sp<RfxAtResponse> p_response = NULL;

    if (current_phb_storage != type || type != RIL_PHB_ADN) {
        storage = getPhbStorageString(type);
        if (storage == NULL) {
            result = 0;
        }

        p_response = atSendCommand(String8::format("AT+CPBS=\"%s\"", storage));
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            result = 0;
        } else {
            current_phb_storage = type;
        }
    }
    // logD(RFX_LOG_TAG, "Select the Storage: %d", m_slot_id);
    return result;
}

char* RmcPhbRequestHandler::getPhbStorageString(int type) {
    char* str = NULL;
    switch (type) {
        case RIL_PHB_ADN:
            str = (char*)"SM";
            break;
        case RIL_PHB_FDN:
            str = (char*)"FD";
            break;
        case RIL_PHB_MSISDN:
            str = (char*)"ON";
            break;
        case RIL_PHB_ECC:
            str = (char*)"EN";
            break;
    }

    return str;
}

void RmcPhbRequestHandler::resetPhbStorage() {
    logD(RFX_LOG_TAG, "resetPhbStorage");
    current_phb_storage = -1;
    maxGrpNum = -1;
    maxAnrNum = -1;
    maxEmailNum = -1;
}

void RmcPhbRequestHandler::requestResetPhbStorage(const sp<RfxMclMessage>& msg) {
    resetPhbStorage();
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

// rilprotect start from here.

#define RIL_E_DIAL_STRING_TOO_LONG RIL_E_OEM_ERROR_1
#define RIL_E_TEXT_STRING_TOO_LONG RIL_E_OEM_ERROR_2
#define RIL_E_SIM_MEM_FULL RIL_E_OEM_ERROR_3
#define DlogD(x...) if (mIsUserLoad != 1) logD( x )

#ifdef ESC_SUPPORT
#define SPEC_ENCODING_ARR_LEN 54
static unsigned short specialEncoding[SPEC_ENCODING_ARR_LEN][2] = {
#else
#define SPEC_ENCODING_ARR_LEN 44
static unsigned short specialEncoding[SPEC_ENCODING_ARR_LEN][2] = {
#endif
    {0x0040, 0x0000},  //@
    {0x00A3, 0x0001},
    {0x0024, 0x0002},
    {0x00A5, 0x0003},
    {0x00E8, 0x0004},
    {0x00E9, 0x0005},
    {0x00F9, 0x0006},
    {0x00EC, 0x0007},
    {0x00F2, 0x0008},
    {0x00C7, 0x0009},
    {0x0020, 0x0020},
    {0x00D8, 0x000B},
    {0x00F8, 0x000C},
    {0x0020, 0x0020},
    {0x00C5, 0x000E},
    {0x00E5, 0x000F},
    {0x0394, 0x0010},
    {0x005F, 0x0011},
    {0x03A6, 0x0012},
    {0x0393, 0x0013},
    {0x039B, 0x0014},
    {0x03A9, 0x0015},
    {0x03A0, 0x0016},
    {0x03A8, 0x0017},
    {0x03A3, 0x0018},
    {0x0398, 0x0019},
    {0x039E, 0x001A},
    {0x00C6, 0x001C},
    {0x00E6, 0x001D},
    {0x00DF, 0x001E},
    {0x00C9, 0x001F},
    {0x00A4, 0x0024},
    {0x00A1, 0x0040},
    {0x00C4, 0x005B},
    {0x00D6, 0x005C},
    {0x00D1, 0x005D},
    {0x00DC, 0x005E},
    {0x00A7, 0x005F},
    {0x00BF, 0x0060},
    {0x00E4, 0x007B},
    {0x00F6, 0x007C},
    {0x00F1, 0x007D},
    {0x00FC, 0x007E},
    {0x00E0, 0x007F},
#ifdef ESC_SUPPORT
    {0x0020, 0x0020},
    {0x005E, 0x1B14},
    {0x007B, 0x1B28},
    {0x007D, 0x1B29},
    {0x005C, 0x1B2F},
    {0x005B, 0x1B3C},
    {0x007E, 0x1B3D},
    {0x005D, 0x1B3E},
    {0x007C, 0x1B40},
    {0x20AC, 0x1B65}
#endif
};

// PHB Start
int RmcPhbRequestHandler::hexCharToDecInt(char *hex, int length) {
    int i = 0;
    int value, digit;

    for (i = 0, value = 0; i < length && hex[i] != '\0'; i++) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            digit = hex[i] - '0';
        }
        else if (hex[i] >= 'A' && hex[i] <= 'F') {
            digit = hex[i] - 'A' + 10;
        }
        else if (hex[i] >= 'a' && hex[i] <= 'f') {
            digit = hex[i] - 'a' + 10;
        }
        else {
            return -1;
        }
        value = value*16 + digit;
    }

    return value;
}

int RmcPhbRequestHandler::encodeUCS2_0x81(char *src, char *des, int maxLen) {
    int i, j, k, len = 0;
    unsigned int base = 0xFF000000;
    unsigned short tmpAlphaId[RIL_MAX_PHB_NAME_LEN+3+1];

    len = (int)strlen(src);
    for (i=0, j=0; (i < len && j < RIL_MAX_PHB_NAME_LEN+3); i+=4, j++) {
        tmpAlphaId[j] = (unsigned short) hexCharToDecInt(src+i, 4);
    }
    tmpAlphaId[j] = '\0';
    len = j;

    logD(RFX_LOG_TAG, "0x81: len: %d", len);

    if (len <= 3)   // at least 3 characters
        return 0;
    if ( ((len+3)*2+1) > maxLen)  // the destinaiton buffer is not enough(include '\0')
        return 0;

    for (i = 0; i < len; i++) {
        int needSpecialEncoding = 0;

        if (tmpAlphaId[i] & 0x8000)  // Because base pointer of 0x81 is 0hhh hhhh h000 0000
            return 0;

        for (k = 0; k < SPEC_ENCODING_ARR_LEN; k++) {
            if (tmpAlphaId[i] == specialEncoding[k][0]) {
                DlogD(RFX_LOG_TAG, "Haman 0x81 found: i: %d, tmpAlphaId: %d", i, tmpAlphaId[i]);
                tmpAlphaId[i] = specialEncoding[k][1];
                needSpecialEncoding = 1;
                break;
            }
        }

        if (needSpecialEncoding != 1) {
            if (tmpAlphaId[i] < 0x80) {
#ifndef ESC_SUPPORT
                if (tmpAlphaId[i] == 0x0020 ||
                    tmpAlphaId[i] == 0x005E ||
                    tmpAlphaId[i] == 0x007B ||
                    tmpAlphaId[i] == 0x007D ||
                    tmpAlphaId[i] == 0x005B ||
                    tmpAlphaId[i] == 0x007E ||
                    tmpAlphaId[i] == 0x005D ||
                    tmpAlphaId[i] == 0x005C ||
                    tmpAlphaId[i] == 0x007C )
                    return 0;
                else
#endif
                {
                    if (tmpAlphaId[i] == 0x0060) {
                        if (base == 0xFF000000) {
                            base = 0;
                            tmpAlphaId[i] = 0x00E0;
                        } else {
                            return 0;
                        }
                    }

                    continue;
                }
            }

            if (base == 0xFF000000) {
                base = tmpAlphaId[i] & RIL_PHB_UCS2_81_MASK;
            }
            tmpAlphaId[i] ^= base;
            DlogD(RFX_LOG_TAG, "0x81: alpha: %x", tmpAlphaId[i]);
            if ( tmpAlphaId[i] >= 0x80)
                break;
            tmpAlphaId[i] |= 0x80;
        }
    }

    if (i != len) {
        return 0;
    }
    // Make up UCS2 0x81 String
    if (sprintf(des, "81%02X%02X", len, base>>7) < 0) {
        return 0;
    }
    int realLen = 0;
    char* pLen = des;
    des += 6;

    for (i=0; i < len; i++, des+=2) {
        if ((tmpAlphaId[i] & 0xFF00) != 0x1B00) {
            DlogD(RFX_LOG_TAG, "Haman1B 0x81: i: %d, tmpAlphaId: %x", i, tmpAlphaId[i]);
            if (sprintf(des, "%02X", tmpAlphaId[i]) < 0) {
                return 0;
            }
            DlogD(RFX_LOG_TAG, "Haman pleni: %s", pLen);
        }
        else {
            if (sprintf(des, "%04X", tmpAlphaId[i]) < 0) {
                return 0;
            }
            DlogD(RFX_LOG_TAG, "Haman 0x81: i: %d, tmpAlphaId: %x", i, tmpAlphaId[i]);
            DlogD(RFX_LOG_TAG, "Haman plenx: %s", pLen);
            des+=2;
            realLen++;
        }
    }

    realLen += len;
    if (realLen > len) {
        *(pLen + 2) = (char)((realLen/16) > 9 ? ((realLen/16) - 10 + 'A') : (realLen/16) + '0');
        *(pLen + 3) = (char)((realLen%16) > 9 ? ((realLen%16) - 10 + 'A') : (realLen%16) + '0');
    }
    *des = '\0';

    return realLen;
}

int RmcPhbRequestHandler::encodeUCS2_0x82(char *src, char *des, int maxLen) {
    int i, j, k, len;
    int min, max, base;
    int needSpecialEncoding = 0;
    unsigned short tmpAlphaId[RIL_MAX_PHB_NAME_LEN+4+1];

    int realLen = 0;
    char* pLen = des;

    min = max = base = -1;

    len = (int)strlen(src);
    for (i=0, j=0; (i < len && j < RIL_MAX_PHB_NAME_LEN+4); i+=4, j++) {
        tmpAlphaId[j] = (unsigned short) hexCharToDecInt(src+i, 4);
    }
    tmpAlphaId[j] = '\0';
    len = j;

    logD(RFX_LOG_TAG, "0x82: len: %d", len);

    if (len <= 4)   // at least 4 characters
        return 0;
    if (((len+4)*2+1) > maxLen)  // the destinaiton buffer is not enough(include '\0')
        return 0;

    for (i=0; i < len; i++) {
        for (k = 0; k < SPEC_ENCODING_ARR_LEN; k++) {
            if (tmpAlphaId[i] == specialEncoding[k][0]) {
                needSpecialEncoding = 1;
                break;
            }
        }

        if (needSpecialEncoding == 0) {
            if ((tmpAlphaId[i] < 0x0080) && (tmpAlphaId[i] != 0x0060)) {
#ifndef ESC_SUPPORT
                 if (tmpAlphaId[i] == 0x0020 ||
                    tmpAlphaId[i] == 0x005E ||
                    tmpAlphaId[i] == 0x007B ||
                    tmpAlphaId[i] == 0x007D ||
                    tmpAlphaId[i] == 0x005B ||
                    tmpAlphaId[i] == 0x007E ||
                    tmpAlphaId[i] == 0x005D ||
                    tmpAlphaId[i] == 0x005C ||
                    tmpAlphaId[i] == 0x007C )
                    return 0;
#endif
            } else {    // (tmpAlphaId[i] >= 0x80) || (tmpAlphaId[i] == 0x0060)
                if (min < 0) {
                    min = max = tmpAlphaId[i];
                } else if (tmpAlphaId[i] < min) {
                    min = tmpAlphaId[i];
                } else if (tmpAlphaId[i] > max) {
                    max = tmpAlphaId[i];
                }
            }
        } else {
            needSpecialEncoding = 0;
        }
    }

    if (min < 0)
        min = max = 0;

    DlogD(RFX_LOG_TAG, "0x82: min: 0x%x, max: 0x%x", min, max);

    if ((max - min) >= 128)
        return 0;

    base = min;

    // encode the String
    for (i=0; i < len; i++) {
        for (k = 0; k < SPEC_ENCODING_ARR_LEN; k++) {
            if (tmpAlphaId[i] == specialEncoding[k][0]) {
                tmpAlphaId[i] = specialEncoding[k][1];
                needSpecialEncoding = 1;
                break;
            }
        }

        if (needSpecialEncoding == 0) {
            if ((tmpAlphaId[i] < 0x0080) && (tmpAlphaId[i] != 0x0060))
                continue;

            tmpAlphaId[i] -= base;
            tmpAlphaId[i] |= 0x80;
        } else {
            needSpecialEncoding = 0;
        }
    }

    // Make up UCS2 0x82 String
    if (sprintf(des, "82%02X%04X", len, base) < 0) {
        return 0;
    }
    des += 8;

    for (i=0; i < len; i++, des+=2) {
        if ((tmpAlphaId[i] & 0xFF00) != 0x1B00) {
            if (sprintf(des, "%02X", tmpAlphaId[i]) < 0) {
                return 0;
            }
        } else {
            if (sprintf(des, "%04X", tmpAlphaId[i]) < 0) {
                return 0;
            }
            des+=2;
            realLen++;
        }
    }

    realLen += len;

    if (realLen > len) {
        *(pLen + 2) = (char)((realLen/16) > 9 ? ((realLen/16) - 10 + 'A') : (realLen/16) + '0');
        *(pLen + 3) = (char)((realLen%16) > 9 ? ((realLen%16) - 10 + 'A') : (realLen%16) + '0');
    }

    *des = '\0';

    return realLen;
}

char * RmcPhbRequestHandler::ascii2UCS2(char * input) {
    if (input == 0) {
        return NULL;
    }

    int len = (int)strlen(input);
    char * ret = (char *)calloc(1, len*4+1);
    if (ret == NULL) {
        logE(RFX_LOG_TAG, "ascii2UCS2 ret calloc fail");
        return NULL;
    }
    int i = 0;
    for (i=0; i < len; ++i) {
        if (sprintf(ret+i*4, "%04x", input[i]) < 0) {
            return NULL;
        }
    }

    return ret;
}


void RmcPhbRequestHandler::requestQueryPhbInfo(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err, type;
    int query_info[4];
    char *tmp;
    RfxAtLine *line = NULL;
    int *data = (int *)msg->getData()->getData();
    type = ((int *)data)[0];
    sp<RfxMclMessage> response;

    if (selectPhbStorage(type)) {
        p_response = atSendCommandSingleline("AT+CPBS?", "+CPBS:");
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            goto error;
        }

        // +CPBS: <STORAGE>, <used>, <total>
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        if (err < 0) goto error;
        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        query_info[0] = line->atTokNextint(&err);
        if (err < 0) goto error;

        query_info[1] = line->atTokNextint(&err);
        if (err < 0) goto error;

        if (RIL_PHB_ADN == type) {
            maxAdnRecords = query_info[1];
        }

        p_response = atSendCommandSingleline("AT+CPBR=?", "+CPBR:");
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            goto error;
        }
        // +CPBR: (<bIndex>-<eIndex>), <max_num_len>, <max_alpha_len>
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        if (err < 0) goto error;

        tmp = line->atTokNextstr(&err);
        if (err < 0) goto error;

        query_info[2] = line->atTokNextint(&err);
        if (err < 0) goto error;

        query_info[3] = line->atTokNextint(&err);
        if (err < 0) goto error;
        logD(RFX_LOG_TAG, "PhbQueryInformation: %d %d %d %d", query_info[0],
                query_info[1], query_info[2], query_info[3]);

        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(query_info, 4), msg, false);
        responseToTelCore(response);
        return;
    }
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler::requestClearPhbEntry(int index) {
    String8 cmd("");
    int i = 0;
    int len = 0;
    int maxNum = -1;
    sp<RfxAtResponse> p_response = NULL;
    if (m_slot_id >= 0 && m_slot_id < 4) {
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

void RmcPhbRequestHandler::requestWritePhbEntry(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int tmp;
    size_t i, j, len;
    int asciiNotSupported = 0;
    RIL_PhbEntryStructure *entry;
    RIL_PhbEntryStructure *data = (RIL_PhbEntryStructure *)msg->getData()->getData();
    char *line, *number;
    String8 cmd("");
    char alphaId[RIL_MAX_PHB_NAME_LEN*4+4+1]={0};
    // If using UCS2, each character use 4 bytes,
    // if using 0x82 coding, we need extra 4 bytes,
    // we also need '\0'

    char temp[RIL_MAX_PHB_NAME_LEN*4+4+1] = {0};

    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    RilPhbCpbwEncode encodeMethod;

    entry = (RIL_PhbEntryStructure *)data;
    if (selectPhbStorage(entry->type)) {
        if (entry->alphaId != NULL || entry->number != NULL) {
            if (entry->alphaId == NULL) {
                line = (char*)"";
            } else {
                line = entry->alphaId;
            }
            if (entry->number == NULL) {
                number = (char*)"";
            } else {
                number = entry->number;
            }
            len = strlen(line);
            if (len == 0 && strlen(number) == 0) {
                logE(RFX_LOG_TAG, "The number and alphaid can't be empty String at the same time: %s %s",
                        line, number);
                goto error;
            }

            if ((entry->index > maxAdnRecords) && (entry->type == RIL_PHB_ADN)) {
                logE(RFX_LOG_TAG, "entry index more than max record: %d, %d", entry->index, maxAdnRecords);
                goto error;
            }

            // logD(RFX_LOG_TAG, "The number and alphaid: %s %s", line, number);

            // pack Alpha Id
            if ((len%4) != 0) {
                logE(RFX_LOG_TAG, "The alphaId should encode using Hexdecimal: %s", line);
                goto error;
            } else if (len > (RIL_MAX_PHB_NAME_LEN*4)) {
                logE(RFX_LOG_TAG, "The alphaId shouldn't longer than RIL_MAX_PHB_NAME_LEN");
                ret = RIL_E_TEXT_STRING_TOO_LONG;
                goto error;
            }

            for (i=0, j=0; i < len; i+=4, j++) {
                tmp = hexCharToDecInt(line+i, 4);

                if (tmp >= 128) {
                    break;
                }

                // Skip '\r' & '\n'
                if (tmp == 0x000A || tmp == 0x000D) continue;

                if (tmp == '`') {
                    // Special character, ascii not support it
                    asciiNotSupported = 1;
                    logD(RFX_LOG_TAG, "Ascii not supported, there is a '`'.");
                }

                alphaId[j] = static_cast<char>(tmp);
                DlogD(RFX_LOG_TAG, "requestWritePhbEntry tmp=%d, alphaId=%d", tmp, alphaId[j]);

                // alphaId[ril_max_phb_name_len] = '\0';
            }
            alphaId[j] = static_cast<char>('\0');

            // logD(RFX_LOG_TAG, "requestWritePhbEntry alphaId = %s", alphaId); // pii

            if (i != len || asciiNotSupported) {
                len /= 4;

                if (encodeUCS2_0x81(line, alphaId, sizeof(alphaId))) {  // try UCS2_0x81 coding
                    // UCS2 0x81 coding
                    encodeMethod = CPBW_ENCODE_UCS2_81;
                    // alphaId[ril_max_phb_name_len * 2] = '\0';
                } else if (encodeUCS2_0x82(line, alphaId, sizeof(alphaId))) {
                    // try UCS2_0x82 coding
                    // UCS2 0x82 coding
                    encodeMethod = CPBW_ENCODE_UCS2_82;
                } else {
                    // UCS2 coding
                    encodeMethod = CPBW_ENCODE_UCS2;
                    logD(RFX_LOG_TAG, "alphaId length=%zu, line length=%zu",
                            sizeof(alphaId), strlen(line));
                    memcpy(alphaId, line,
                            ((strlen(line) < sizeof(alphaId)) ? strlen(line) : sizeof(alphaId)));
                    // alphaId[ril_max_phb_name_len * 2] = '\0';
                }
            }
            else {
                encodeMethod = CPBW_ENCODE_IRA;
                // ASCII coding

                // Replace \ to \5c and replace " to \22 for MTK modem
                // the order is very important! for "\\" is substring of "\\22"
                memcpy(temp, alphaId, sizeof(alphaId));
                temp[sizeof(temp) - 1] = static_cast<char>('\0');
                j = 0;
                for (i=0; i < strlen(temp); i++) {
                    if (temp[i] == '\\') {
                        logD(RFX_LOG_TAG, "Use Ascii encoding, there is a '\\', index=%zu", i);
                        if ((j+2) < (sizeof(alphaId)-1)) {
                            alphaId[j++] = static_cast<char>('\\');
                            alphaId[j++] = static_cast<char>('5');
                            alphaId[j++] = static_cast<char>('c');
                        } else {
                            break;
                        }
                    } else {
                        alphaId[j++] = temp[i];
                    }

                    if (j == (sizeof(alphaId)-1)) {
                        break;
                    }
                }
                alphaId[j] = static_cast<char>('\0');

                memcpy(temp, alphaId, sizeof(alphaId));
                temp[sizeof(temp) - 1] = static_cast<char>('\0');
                j = 0;
                for (i=0; i < strlen(temp); i++) {
                    if (temp[i] == '\"') {
                        logD(RFX_LOG_TAG, "Use Ascii encoding, there is a '\"', index=%zu", i);
                        if ((j+2) < (sizeof(alphaId)-1)) {
                            alphaId[j++] = static_cast<char>('\\');
                            alphaId[j++] = static_cast<char>('2');
                            alphaId[j++] = static_cast<char>('2');
                        } else {
                            break;
                        }
                    } else {
                        alphaId[j++] = temp[i];
                    }

                    if (j == (sizeof(alphaId)-1)) {
                        break;
                    }
                }
                alphaId[j] = static_cast<char>('\0');
            }
            // move this action to FW
            // requestClearPhbEntry(entry->index);
            cmd.append(String8::format("AT+EPBW=%d, \"%s\", %d, \"%s\", 0, %d",
                        entry->index, number, entry->ton, alphaId, encodeMethod));
        }
        else {
            // delete
            cmd.append(String8::format("AT+EPBW=%d", entry->index));
        }

        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->getError() < 0) {
            logE(RFX_LOG_TAG, "EPBW Error!!!!");
            goto error;
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                case CME_SUCCESS:
                    /* While p_response->getSuccess() is 0, the CME_SUCCESS means CME ERROR:0
                    => it is phone failure */
                    ret = RIL_E_GENERIC_FAILURE;
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
    }
error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestReadPhbEntry(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err, type, bIndex, eIndex, count, i;
    String8 cmd("");
    char *out;
    RfxAtLine *p_cur = NULL;
    RIL_PhbEntryStructure *entry = NULL;
    RIL_PhbEntryStructure **entries = NULL;
    int *data = (int *)msg->getData()->getData();
    sp<RfxMclMessage> response;

    type = ((int *)data)[0];
    if (selectPhbStorage(type)) {
        bIndex = ((int *)data)[1];
        eIndex = ((int *)data)[2];

        if ((eIndex - bIndex + 1) <= 0 || (eIndex - bIndex + 1) > RIL_MAX_PHB_ENTRY) {
            logE(RFX_LOG_TAG, "Begin index or End Index is invalid: %d %d", bIndex, eIndex);
            goto error;
        }

        cmd.append(String8::format("AT+CPBR=%d, %d", bIndex, eIndex));
        p_response = atSendCommandMultiline(cmd, "+CPBR:");
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            goto error;
        }

        /* count the calls */
        for (count = 0, p_cur = p_response->getIntermediates()
                ; p_cur != NULL
                ; p_cur = p_cur->getNext()) {
            count++;
        }

        /* allocate memory and parse +CPBR*/
        entries = (RIL_PhbEntryStructure**)calloc(1, sizeof(RIL_PhbEntryStructure*)*count);
        if (entries == NULL) {
            logE(RFX_LOG_TAG, "entries calloc fail");
            goto error;
        }
        // +CPBR: <index>, <number>, <TON>, <alphaId>
        for (i = 0, p_cur = p_response->getIntermediates()
                ; p_cur != NULL
                ; p_cur = p_cur->getNext(), i++) {
            entries[i] = entry = (RIL_PhbEntryStructure*)calloc(1, sizeof(RIL_PhbEntryStructure));
            if (entry == NULL) {
                logE(RFX_LOG_TAG, "entry calloc fail");
                goto error;
            }
            entry->type = type;

            p_cur->atTokStart(&err);
            if (err < 0) goto error;

            entry->index = p_cur->atTokNextint(&err);
            if (err < 0) goto error;

            out = p_cur->atTokNextstr(&err);
            if (err < 0) goto error;
            entry->number = (char*)alloca(strlen(out) + 1);
            strncpy(entry->number, out, strlen(out) + 1);

            entry->ton = p_cur->atTokNextint(&err);
            if (err < 0) goto error;

            out = p_cur->atTokNextstr(&err);
            if (err < 0) goto error;
            entry->alphaId = (char*)alloca(strlen(out) + 1);
            strncpy(entry->alphaId, out, strlen(out) + 1);

            // logD(RFX_LOG_TAG, "PhbEntry %d: %s, %d, %s", entry->index,
            //    entry->number, entry->ton, entry->alphaId); // pii
        }

        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxPhbEntriesData(entries, count), msg, false);
        responseToTelCore(response);

        for (i=0; i < count; i++) {
            if (entries[i] != NULL) {
                free(entries[i]);
            }
        }
        free(entries);
        return;
    }
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);

    if (entries != NULL) {
        for (i=0; i < count; i++) {
            if (entries[i] != NULL) {
                free(entries[i]);
            }
        }
        free(entries);
    }
}

void RmcPhbRequestHandler:: requestQueryUPBCapability(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    RfxAtLine *line = NULL;
    int upbCap[8]= {0};
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+EPBUM=?", "+EPBUM:");
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestQueryUPBCapability Fail");
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
        logE(RFX_LOG_TAG, "requestQueryUPBCapability: Error!");
        goto error;
    }
    line = p_response->getIntermediates();
    line->atTokStart(&err);
    // logD(RFX_LOG_TAG, "requestQueryUPBCapability: strlen of response is %d", strlen(line) );
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

    maxGrpNum = upbCap[7];
    maxAnrNum = upbCap[0];
    maxEmailNum = upbCap[1];

    logD(RFX_LOG_TAG, "requestQueryUPBCapability: %d, %d, %d, %d, %d, %d, %d, %d",
        upbCap[0], upbCap[1], upbCap[2], upbCap[3], upbCap[4], upbCap[5],
        upbCap[6], upbCap[7]);

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(upbCap, 8), msg, false);
    responseToTelCore(response);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestEditUPBEntry(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int tmp;
    size_t len, i, j;
    int asciiNotSupported = 0;
    char** params = (char**)msg->getData()->getData();
    char *line;
    String8 cmd("");
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    RilPhbCpbwEncode encodeMethod;
    int datalen = msg->getData()->getDataLength();
    int paramCount = datalen/sizeof(char*);


    // GAS, AAS, SNE
    if (0 == strcmp(params[0], "4") || 0 == strcmp(params[0], "3") || 0 == strcmp(params[0], "2")) {
        char temp[RIL_MAX_PHB_NAME_LEN*4+4+1] = {0};
        char str[RIL_MAX_PHB_NAME_LEN*4+4+1] = {0};  // If using UCS2, each character use 4 bytes,
                                                     // if using 0x82 coding, we need extra 4 bytes, we also need '\0'
        line = params[3];
        if (line == NULL) {
            line = (char*)"";
        }
        len = strlen(line);
        DlogD(RFX_LOG_TAG, "requestEditUPBEntry: line is %s, len=%zu", line, len);

        if (len == 0) {
            logE(RFX_LOG_TAG, "The group name can't be empty String: %s", line);
            goto error;
        }

        // pack gas
        if ((len % 4) != 0) {
            logE(RFX_LOG_TAG, "The gas should encode using Hexdecimal: %s", line);
            goto error;
        } else if (len > (RIL_MAX_PHB_NAME_LEN*4)) {
            logE(RFX_LOG_TAG, "The gas shouldn't longer than RIL_MAX_PHB_NAME_LEN");
            ret = RIL_E_TEXT_STRING_TOO_LONG;
            goto error;
        }

        for (i=0, j=0; i < len; i+=4, j++) {
            tmp = hexCharToDecInt(line+i, 4);

            if (tmp >= 128) {
                break;
            }

            if (tmp == '`') {
                // Special character, ascii not support it
                asciiNotSupported = 1;
                logD(RFX_LOG_TAG, "Ascii not supported, there is a '`'.");
            }

            str[j] = static_cast<char>(tmp);
        }
        str[j] = static_cast<char>('\0');

        DlogD(RFX_LOG_TAG, "requestEditUPBEntry str = %s", str);

        if (i != len || asciiNotSupported) {
            len /= 4;

            if (encodeUCS2_0x81(line, str, sizeof(str))) {    // try UCS2_0x81 coding
                // UCS2 0x81 coding
                encodeMethod = CPBW_ENCODE_UCS2_81;
            }
            else {
                // UCS2 coding
                encodeMethod = CPBW_ENCODE_UCS2;
                memcpy(str, line, ((sizeof(str) < strlen(line)) ? sizeof(str) : strlen(line)));
            }
        }
        else {
            // ASCII coding
            encodeMethod = CPBW_ENCODE_IRA;

            // Replace \ to \5c and replace " to \22 for MTK modem
            // the order is very important! for "\\" is substring of "\\22"
            memcpy(temp, str, sizeof(str));
            temp[sizeof(temp) - 1] = static_cast<char>('\0');
            j = 0;
            for (i=0; i < strlen(temp); i++) {
                if (temp[i] == '\\') {
                    logD(RFX_LOG_TAG, "Use Ascii encoding, there is a '\\', index=%zu", i);
                    if ((j+2) < (sizeof(str)-1)) {
                        str[j++] = static_cast<char>('\\');
                        str[j++] = static_cast<char>('5');
                        str[j++] = static_cast<char>('c');
                    } else {
                        break;
                    }
                } else {
                    str[j++] = temp[i];
                }

                if (j == (sizeof(str)-1)) {
                    break;
                }
            }
            str[j] = static_cast<char>('\0');

            memcpy(temp, str, sizeof(str));
            temp[sizeof(temp) - 1] = static_cast<char>('\0');
            j = 0;
            for (i=0; i < strlen(temp); i++) {
                if (temp[i] == '\"') {
                    logD(RFX_LOG_TAG, "Use Ascii encoding, there is a '\"', index=%zu", i);
                    if ((j + 2) < (sizeof(str)-1)) {
                        str[j++] = static_cast<char>('\\');
                        str[j++] = static_cast<char>('2');
                        str[j++] = static_cast<char>('2');
                    } else {
                        break;
                    }
                } else {
                    str[j++] = temp[i];
                }

                if (j == (sizeof(str)-1)) {
                    break;
                }
            }
            str[j] = static_cast<char>('\0');
        }

        cmd.append(String8::format("AT+EPBUM=2,%s,%s,%s,\"%s\",%d",
                params[0], params[2], params[1], str, encodeMethod));
    }
    else if ( 0 == strcmp(params[0], "1") ) {  // Email
        char temp[RIL_MAX_PHB_EMAIL_LEN*4+4+1] = {0};
        char str[RIL_MAX_PHB_EMAIL_LEN*4+4+1] = {0};  // If using UCS2, each character use 4 bytes,
                                                      // if using 0x82 coding, we need extra 4 bytes, we also need '\0'
        line = params[3];
        if (line == NULL) {
            line = (char*)"";;
        }
        len = strlen(line);
        // logD(RFX_LOG_TAG, "requestEditUPBEntry: line is %s, len=%d", line, len); // pii
        if (len == 0) {
            logE(RFX_LOG_TAG, "The email name can't be empty String: %s", line);
            goto error;
        }

        // pack email
        if ((len % 4) != 0) {
            logE(RFX_LOG_TAG, "The email should encode using Hexdecimal: %s", line);
            goto error;
        } else if (len > (RIL_MAX_PHB_EMAIL_LEN*4)) {
            logE(RFX_LOG_TAG, "The email shouldn't longer than RIL_MAX_PHB_EMAIL_LEN");
            ret = RIL_E_TEXT_STRING_TOO_LONG;
            goto error;
        }

        for (i=0, j=0; i < len; i+=4, j++) {
            tmp = hexCharToDecInt(line + i, 4);

            if (tmp >= 128) {
                break;
            }

            if (tmp == '`') {
                // Special character, ascii not support it
                asciiNotSupported = 1;
                logD(RFX_LOG_TAG, "Ascii not supported, there is a '`'.");
            }

            str[j] = static_cast<char>(tmp);
        }
        str[j] = static_cast<char>('\0');

        // logD(RFX_LOG_TAG, "requestEditUPBEntry str = %s", str); // pii

        if (i != len || asciiNotSupported) {  // we think email doesn't get into here.
            len /= 4;

            if (encodeUCS2_0x81(line, str, sizeof(str))) {    // try UCS2_0x81 coding
                // UCS2 0x81 coding
                encodeMethod = CPBW_ENCODE_UCS2_81;
            }
            else {
                // UCS2 coding
                encodeMethod = CPBW_ENCODE_UCS2;
                memcpy(str, line, ((sizeof(str) < strlen(line)) ? sizeof(str) : strlen(line)));
            }
        }
        else {
            // ASCII coding
            encodeMethod = CPBW_ENCODE_IRA;

            // Replace \ to \5c and replace " to \22 for MTK modem
            // the order is very important! for "\\" is substring of "\\22"
            memcpy(temp, str, sizeof(str));
            temp[sizeof(temp) - 1] = static_cast<char>('\0');
            j = 0;
            for (i=0; i < strlen(temp); i++) {
                if (temp[i] == '\\') {
                    logD(RFX_LOG_TAG, "Use Ascii encoding, there is a '\\', index=%zu", i);
                    if ((j + 2) < (sizeof(str)-1)) {
                        str[j++] = static_cast<char>('\\');
                        str[j++] = static_cast<char>('5');
                        str[j++] = static_cast<char>('c');
                    } else {
                        break;
                    }
                } else {
                    str[j++] = temp[i];
                }

                if (j == (sizeof(str)-1)) {
                    break;
                }
            }
            str[j] = static_cast<char>('\0');

            memcpy(temp, str, sizeof(str));
            temp[sizeof(temp) - 1] = static_cast<char>('\0');
            j = 0;
            for (i=0; i < strlen(temp); i++) {
                if (temp[i] == '\"') {
                    logD(RFX_LOG_TAG, "Use Ascii encoding, there is a '\"', index=%zu", i);
                    if ((j + 2) < (sizeof(str)-1)) {
                        str[j++] = static_cast<char>('\\');
                        str[j++] = static_cast<char>('2');
                        str[j++] = static_cast<char>('2');
                    } else {
                        break;
                    }
                } else {
                    str[j++] = temp[i];
                }

                if (j == (sizeof(str)-1)) {
                    break;
                }
            }
            str[j] = static_cast<char>('\0');
        }

        cmd.append(String8::format("AT+EPBUM=2,%s,%s,%s,\"%s\"", params[0], params[2], params[1], str));
    }
    else if ( paramCount > 4 && 0 == strcmp(params[0], "0") ) {  // Anr
        cmd.append(String8::format("AT+EPBUM=2,%s,%s,%s,\"%s\",%s,%s", params[0], params[2],
                params[1], params[3], params[4], params[5]));
    }
    p_response = atSendCommand(cmd);
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "EPBUM write Error!!!!");
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
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

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestDeleteUPBEntry(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    String8 cmd("");
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int* params = (int *) msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();
    if (datalen/sizeof(int) < 3) {
        logE(RFX_LOG_TAG, "requestDeleteUPBEntry param is not enough. datalen is %d", datalen);
        goto error;
    }

    cmd.append(String8::format("AT+EPBUM=3,%d,%d,%d", params[0], params[2], params[1]));
    p_response = atSendCommand(cmd);
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestDeleteUPBEntry Fail");
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
        logD(RFX_LOG_TAG, "requestDeleteUPBEntry: Error!");
        goto error;
    }

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestReadGasList(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    String8 cmd("");
    RfxAtLine* line = NULL;
    int tmp, i, entryIndex;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int* params = (int *) msg->getData()->getData();
    char **responses = NULL;
    char * out;
    int datalen = msg->getData()->getDataLength();
    sp<RfxMclMessage> response;

    if (datalen/sizeof(int) < 2 || params[0] < 0 || params[1] < params[0]) {
        logE(RFX_LOG_TAG, "requestReadGasList param is not enough. datalen is %d", datalen);
        goto error;
    }
    responses = (char**)calloc(1, sizeof(char*)*(params[1] - params[0] + 1));
    if (responses == NULL) {
        logE(RFX_LOG_TAG, "responses calloc fail");
        ret = RIL_E_NO_MEMORY;
        goto error;
    }

    for (i = params[0]; i <= params[1]; i++) {
        cmd.clear();
        cmd.append(String8::format("AT+EPBUM=1,4,%d,0", i));

        p_response = atSendCommandSingleline(cmd, "+EPBUM:");
        if (p_response == NULL || p_response->getError() < 0) {
            logD(RFX_LOG_TAG, "requestReadGasList: the %d th gas entry is null", i);
            ret = RIL_E_SUCCESS;
            continue;  // it means the index is not used.
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError());
                case CME_SUCCESS:
                    ret = RIL_E_GENERIC_FAILURE;
                    break;
                case CME_UNKNOWN:
                    ret = RIL_E_GENERIC_FAILURE;
                    logE(RFX_LOG_TAG, "p_response: CME_UNKNOWN");
                    break;
                default:
                    ret = RIL_E_GENERIC_FAILURE;
                    logE(RFX_LOG_TAG, "p_response: Not success");
                    break;
            }
        } else {
            ret = RIL_E_SUCCESS;
        }

        if (ret != RIL_E_SUCCESS) {
            logE(RFX_LOG_TAG, "requestReadGasList: Error!");
            // goto error;
            continue;
        }

        if (p_response == NULL || p_response->getIntermediates() == NULL) continue;
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        // logD(RFX_LOG_TAG, "requestReadGasList: strlen of response is %d", strlen(line) );

        tmp = line->atTokNextint(&err);  // type
        if (err < 0 || tmp != 4) continue;

        entryIndex = line->atTokNextint(&err);  // entry_index
        if (err < 0 || entryIndex < 1) continue;;
        // logD(RFX_LOG_TAG, "requestReadGasList: (entryIndex-1)=%d", entryIndex-1);

        tmp = line->atTokNextint(&err);   // adn_index

        out = line->atTokNextstr(&err);  // gas string
        if (err < 0) continue;

        tmp = line->atTokNextint(&err);
        if (err < 0) continue;
        logD(RFX_LOG_TAG, "requestReadGasList: encode type is %d", tmp);

        if (tmp == CPBW_ENCODE_IRA) {
            out = ascii2UCS2(out);
            if (out == NULL) {
                free(responses);
                ret = RIL_E_NO_MEMORY;
                goto error;
            }
            responses[entryIndex-1] = (char*)alloca(strlen(out) + 1);
            strncpy(responses[entryIndex-1], out, strlen(out) + 1);
            free(out);  // calloc in ascii2UCS2
        } else {
            responses[entryIndex-1] = (char*)alloca(strlen(out) + 1);
            strncpy(responses[entryIndex-1], out, strlen(out) + 1);
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxStringsData((char **)responses, (params[1] - params[0] + 1)), msg, false);
    responseToTelCore(response);
    free(responses);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestReadUpbGrpEntry(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    String8 cmd("");
    RfxAtLine* line = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int* params = (int *) msg->getData()->getData();
    int grpIds[10] = {0};
    int grpCount = 0;
    int tmp = 0;
    int datalen = msg->getData()->getDataLength();
    int paramCount = datalen/sizeof(int);
    sp<RfxMclMessage> response;

    if (paramCount < 1) {
        logE(RFX_LOG_TAG, "requestReadUpbGrpEntry param is not enough. paramCount is %d", paramCount);
        goto error;
    }

    cmd.append(String8::format("AT+EPBUM=1,5,%d,1", params[0]));
    p_response = atSendCommandSingleline(cmd, "+EPBUM:");
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestReadUpbGrpEntry Fail");
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
        logE(RFX_LOG_TAG, "requestReadUpbGrpEntry: Error!");
        goto error;
    }

    if (p_response->getIntermediates() != NULL) {
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        // logD(RFX_LOG_TAG, "requestReadUpbGrpEntry: strlen of response is %d", strlen(line) );
        tmp = line->atTokNextint(&err);  // field type , must be 5
        if (err < 0) goto error;

        tmp = line->atTokNextint(&err);  // index1
        if (err < 0) goto error;

        tmp = line->atTokNextint(&err);  // maximum number of supported entries in the file
        if (err < 0) {
            // logD(RFX_LOG_TAG, "requestReadUpbGrpEntry: index2 is ignored!");
        }
        grpIds[grpCount++] = line->atTokNextint(&err);  // group id
        if (err < 0) goto error;
        // logD(RFX_LOG_TAG, "requestReadUpbGrpEntry: grpCount is %d, grpIds[%d] is %d!",
        //        grpCount, grpCount-1, grpIds[grpCount-1]);
        while (line->atTokHasmore()) {
            grpIds[grpCount++] = line->atTokNextint(&err);  // group id
            if (err < 0) goto error;
            // logD(RFX_LOG_TAG, "requestReadUpbGrpEntry: grpCount is %d, grpIds[%d] is %d!", grpCount, grpCount-1,
            //        grpIds[grpCount-1]);
            if (grpCount >= 10) {
                break;
            }
        }
    }
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(grpIds, grpCount), msg, false);
    responseToTelCore(response);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestWriteUpbGrpEntry(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int* params = (int *) msg->getData()->getData();
    sp<RfxAtResponse> p_response = NULL;
    int i = 0;
    int datalen = msg->getData()->getDataLength();
    int paramCount = datalen/sizeof(int);
    logD(RFX_LOG_TAG, "requestWriteUpbGrpEntry . datalen is %d, size of int is %zu",
            datalen, sizeof(int));
    if (paramCount < 1) {
        logE(RFX_LOG_TAG, "requestWriteUpbGrpEntry param is not enough. paramCount is %d", paramCount);
        goto error;
    }

    if (paramCount == 1) {
        // delete
        cmd.append(String8::format("AT+EPBUM=3,5,%d,1", params[0]));
    } else {
        // write
        cmd.append(String8::format("AT+EPBUM=2,5,%d,1", params[0]));
        for (i = 1; i < paramCount; i ++) {
            cmd.append(String8::format(",%d", params[i]));
        }
    }

    p_response = atSendCommand(cmd);
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestWriteUpbGrpEntry Fail");
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
        logE(RFX_LOG_TAG, "requestWriteUpbGrpEntry: Error!");
    }

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: requestGetPhoneBookStringsLength(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: entering");
    sp<RfxAtResponse> p_response = NULL;
    int err;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    // <nlength>,<tlength>,<glength>,<slength>,<elength>
    int response[5] = {0};
    int tem;
    RfxAtLine *line = NULL;
    char * temStr;
    sp<RfxMclMessage> res;

    p_response = atSendCommandSingleline("AT+CPBR=?", "+CPBR:");
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength Fail");
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError() );
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
        logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: Error!");
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;
    // logD(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: strlen of response is %d", strlen(line));
    // +CPBR: (<bIndex>-<eIndex>), <max_num_len>, <max_alpha_len>
    temStr = line->atTokNextstr(&err);
    if (err < 0) goto error;
    response[0] = line->atTokNextint(&err);
    if (err < 0) goto error;
    response[1] = line->atTokNextint(&err);
    if (err < 0) goto error;
    logD(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: end AT+CPBR=?");
    p_response = atSendCommandSingleline("AT+EPBUM=?", "+EPBUM:");
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength epbum Fail");
        goto error;
    }

    if (p_response == NULL || p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError());
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
                break;
            case CME_UNKNOWN:
                logE(RFX_LOG_TAG, "p_response epbum: CME_UNKNOWN");
                break;
            default:
                break;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }

    if (ret != RIL_E_SUCCESS) {
        logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength:epbum  Error!");
        goto error;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    // logD(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: epbum strlen of response is %d", strlen(line));
    // +CPBR: <n_anr>, <n_email>, <n_sne>, <n_aas>, <l_aas>, <n_gas>, <l_gas>, <n_grp>
    tem = line->atTokNextint(&err);  // number of supported ANR for a ADN entry
    if (err < 0) goto error;

    tem = line->atTokNextint(&err);  // number of supported EMAIL for a ADN entry
    if (err < 0) goto error;

    tem = line->atTokNextint(&err);  // number of supported SNE for a ADN entry
    if (err < 0) goto error;

    tem = line->atTokNextint(&err);  // maximum number of AAS entries
    if (err < 0) goto error;

    tem = line->atTokNextint(&err);  // maximum length of the AAS in the entries
    if (err < 0) goto error;

    tem = line->atTokNextint(&err);  // maximum number of GAS entries
    if (err < 0) goto error;

    response[2] = line->atTokNextint(&err);  // maximum length of the GAS in the entries
    if (err < 0) goto error;
    logD(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: end AT+EPBUM=?");

    // query the SNE info +EPBUM:<type>,<Index1>,<M_NUM>,<A_NUM>,<L_SNE>
    p_response = atSendCommandSingleline("AT+EPBUM=0,2,1", "+EPBUM:");
    if (p_response != NULL && p_response->getError() >= 0 && p_response->getSuccess() > 0) {
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        response[3] = line->atTokNextint(&err);
    }
    // query the email info +EPBUM:<type>,<Index1>,<M_NUM>,<A_NUM>,<L_EMAIL>
    p_response = atSendCommandSingleline("AT+EPBUM=0,1,1", "+EPBUM:");
    if (p_response != NULL && p_response->getError() >= 0 && p_response->getSuccess() > 0) {
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        response[4] = line->atTokNextint(&err);
    }
    res = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(response, 5), msg, false);
    responseToTelCore(res);
    return;
error:
    logE(RFX_LOG_TAG, "requestGetPhoneBookStringsLength: process error");
    res = RfxMclMessage::obtainResponse(msg->getId(), ret, RfxIntsData(response, 5), msg, false);
    responseToTelCore(res);
}

void RmcPhbRequestHandler:: requestGetPhoneBookMemStorage(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    RIL_PHB_MEM_STORAGE_RESPONSE* entry = NULL;
    RfxAtLine* line = NULL;
    char *out;
    sp<RfxMclMessage> response;

    p_response = atSendCommandSingleline("AT+CPBS?", "+CPBS:");
    if (p_response == NULL || p_response->getError() < 0) {
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError());
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
                goto error;
                break;
            default:
                goto error;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }

    entry = (RIL_PHB_MEM_STORAGE_RESPONSE*)calloc(1, sizeof(RIL_PHB_MEM_STORAGE_RESPONSE));
    if (entry == NULL) {
        logE(RFX_LOG_TAG, "entry calloc fail");
        ret = RIL_E_NO_MEMORY;
        goto error;
    }
    // +CPBS: <STORAGE>, <used>, <total>
    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) goto error;

    out = line->atTokNextstr(&err);
    if (err < 0) goto error;
    entry->storage = (char*)alloca(strlen(out) + 1);
    strncpy(entry->storage, out, strlen(out) + 1);

    entry->used = line->atTokNextint(&err);
    if (err < 0) goto error;

    entry->total = line->atTokNextint(&err);
    if (err < 0) goto error;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxPhbMemStorageData((void*)entry, sizeof(RIL_PHB_MEM_STORAGE_RESPONSE)), msg, false);
    responseToTelCore(response);
    free(entry);
    return;
error:
    logE(RFX_LOG_TAG, "requestGetPhoneBookMemStorage: process error");
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (entry != NULL) {
        free(entry);
    }
}

void RmcPhbRequestHandler:: requestSetPhoneBookMemStorage(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    String8 cmd("");
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    const char** strings = (const char**) msg->getData()->getData();
    const char *storage = strings[0];
    const char *password = strings[1];

    logD(RFX_LOG_TAG, "Select the Storage: %s", storage);
    if ((password != NULL) && (strlen(password) > 0)) {
        cmd.append(String8::format("AT+EPIN2=\"%s\"", password));
        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->getError() < 0) {
            logE(RFX_LOG_TAG, "requestSetPhoneBookMemStorage set pin2 Fail");
            goto error;
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
                goto error;
                break;
            default:
                goto error;
            }
        } else {
            ret = RIL_E_SUCCESS;
        }
        p_response = NULL;
    }
    cmd.clear();
    cmd.append(String8::format("AT+CPBS=\"%s\"", storage));
    p_response = atSendCommand(cmd);

    if (p_response == NULL || p_response->getError() < 0) {
        ret = RIL_E_GENERIC_FAILURE;
        goto error;
    } else {
        int type = -1;
        if (0 == strcmp("SM", storage))
            type = RIL_PHB_ADN;
        if (0 == strcmp("FD", storage))
            type = RIL_PHB_FDN;
        if (0 == strcmp("ON", storage))
            type = RIL_PHB_MSISDN;
        if (0 == strcmp("EN", storage))
            type = RIL_PHB_ECC;
        current_phb_storage = type;
    }
    if (p_response == NULL || p_response->getSuccess() == 0) {
        ret = RIL_E_GENERIC_FAILURE;
    } else {
        ret = RIL_E_SUCCESS;
    }
error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

void RmcPhbRequestHandler:: loadUPBCapability() {
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

void RmcPhbRequestHandler:: requestReadPhoneBookEntryExt(const sp<RfxMclMessage>& msg) {
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
    int *data = (int *)msg->getData()->getData();
    sp<RfxMclMessage> response;

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
    logD(RFX_LOG_TAG, "requestReadPhoneBookEntryExt end count=%d", count);
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
        entry = (RIL_PHB_ENTRY*)calloc(1, sizeof(RIL_PHB_ENTRY));
        if (entry == NULL) {
            logE(RFX_LOG_TAG, "entry calloc fail");
            goto error;
        }
        entries[i] = entry;
        entry->hidden = 0;
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
        entry->number = (char*)alloca(strlen(out) + 1);
        strncpy(entry->number, out, strlen(out) + 1);
        entry->number[strlen(out)] = static_cast<char>('\0');
        entry->type = p_cur->atTokNextint(&err);
        if (err < 0) goto error;
        out = p_cur->atTokNextstr(&err);
        if (err < 0) goto error;
        entry->text = (char*)alloca(strlen(out) + 1);
        strncpy(entry->text, out, strlen(out) + 1);
        entry->text[strlen(out)] = static_cast<char>('\0');
        entry->hidden = 0;
        DlogD(RFX_LOG_TAG, "PhbEntry %d: %s, %d, %s", entry->index,
            entry->number, entry->type, entry->text);
        // ANR
        if (maxAnrNum > 0 && (strlen(entry->text) > 0 || strlen(entry->number) > 0)) {
            cmd.clear();
            cmd.append(String8::format("AT+EPBUM=1,0,%d,%d", entry->index, maxAnrNum));

            // +epbum:<type>,<adn_index>,<ef_index>,<anr>,<type>,<aas>
            p_res = atSendCommandSingleline(cmd, "+EPBUM:");
            if (p_res == NULL || p_res->getError() < 0) {
                logE(RFX_LOG_TAG, "requestReadPhoneBookEntryExt anr epbum error.");
            } else if (p_res->getSuccess() != 0 && p_res->getIntermediates() != NULL) {
                eline = p_res->getIntermediates();
                eline->atTokStart(&err);
                tmp = eline->atTokNextint(&err);
                tmp = eline->atTokNextint(&err);
                tmp = eline->atTokNextint(&err);
                out = eline->atTokNextstr(&err);
                entry->adnumber = (char*)alloca(strlen(out) + 1);
                strncpy(entry->adnumber, out, strlen(out) + 1);
                entry->adtype = eline->atTokNextint(&err);
                DlogD(RFX_LOG_TAG, "PhbEntry anr %d: %s, %d", entry->index,
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
                DlogD(RFX_LOG_TAG, "PhbEntry email %d: %s", entry->index, entry->email);
            }
        }
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
          RfxPhbEntriesExtData(entries, count), msg, false);
    responseToTelCore(response);
    for (i=0; i < count; i++) {
        if (entries[i] != NULL) {
            free(entries[i]);
        }
    }
    free(entries);
    return;

error:
    logE(RFX_LOG_TAG, "requestReadPhoneBookEntryExt error");
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (entries != NULL) {
      for (i=0; i < count; i++) {
          if (entries[i] != NULL) {
              free(entries[i]);
          }
      }
      free(entries);
    }
}

void RmcPhbRequestHandler:: requestWritePhoneBookEntryExt(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int tmp;
    size_t len, i, j;
    RIL_PHB_ENTRY *entry;
    char *line, *number;
    String8 cmd("");
    char alphaId[RIL_MAX_PHB_NAME_LEN*4+4+1]={0, };  // If using UCS2, each character use 4 bytes,
                                                     // if using 0x82 coding, we need extra 4 bytes,
                                                     // we also need '\0'
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
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
            tmp = hexCharToDecInt(line + i, 4);
            if (tmp >= 128) {
                break;
            }
            alphaId[j] = static_cast<char>(tmp);
        }
        alphaId[j] = static_cast<char>('\0');

        logD(RFX_LOG_TAG, "requestWritePhoneBookEntryExt - i = %zu, len = %zu", i, len);
        if (i != len) {
            len /= 4;

            if (encodeUCS2_0x81(line, alphaId, sizeof(alphaId))) {  // try UCS2_0x81 coding
                // UCS2 0x81 coding
                encodeMethod = CPBW_ENCODE_UCS2_81;
                // alphaId[ril_max_phb_name_len * 2] = '\0';
            } else if (encodeUCS2_0x82(line, alphaId, sizeof(alphaId))) {
                // try UCS2_0x82 coding
                // UCS2 0x82 coding
                encodeMethod = CPBW_ENCODE_UCS2_82;
            } else {
                // UCS2 coding
                encodeMethod = CPBW_ENCODE_UCS2;
                logD(RFX_LOG_TAG, "requestWritePhoneBookEntryExt-alphaIdlen = %zu,linelength=%zu",
                        sizeof(alphaId), strlen(line));
                memcpy(alphaId, line,
                        ((sizeof(alphaId) < strlen(line)) ? sizeof(alphaId) : strlen(line)));
                // alphaId[ril_max_phb_name_len * 2] = '\0';
            }
        } else {
            encodeMethod = CPBW_ENCODE_IRA;
             // ASCII coding
        }
        requestClearPhbEntry(entry->index);
        cmd.append(String8::format("AT+EPBW=%d, \"%s\", %d, \"%s\", 0, %d",
                    entry->index, number, entry->type, alphaId, encodeMethod));
    } else {
        // delete
        cmd.append(String8::format("AT+EPBW=%d", entry->index));
    }

    p_response = atSendCommand(cmd);
    if (p_response == NULL || p_response->getError() < 0) {
        logE(RFX_LOG_TAG, "requestWritePhoneBookEntryExt EPBW Error!!!!");
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        switch (p_response->atGetCmeError()) {
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
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
    logD(RFX_LOG_TAG, "requestWritePhoneBookEntryExt ret %d", ret);
    p_response = NULL;

    if (maxAnrNum == -1) {
        loadUPBCapability();
    }

    // ANR
    if (ret == RIL_E_SUCCESS && maxAnrNum > 0) {
        cmd.clear();
        // at+epbum=<op>,<type>,<adn_index>,<ef_index>,<anr>,<type>,<aas>
        if (entry->adnumber != NULL && strlen(entry->adnumber) > 0) {
            cmd.append(String8::format("AT+EPBUM=2,0,%d,%d,\"%s\",%d,0", entry->index,
                    maxAnrNum, entry->adnumber, entry->adtype));
        } else {
            cmd.append(String8::format("AT+EPBUM=3,0,%d,%d", entry->index, maxAnrNum));
        }

        p_response = atSendCommand(cmd);
        if (p_response == NULL || p_response->getError() < 0 || p_response->getSuccess() == 0) {
            logE(RFX_LOG_TAG, "requestWritePhoneBookEntryExt EPBUM ANR Error!!!!");
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
            logE(RFX_LOG_TAG, "requestWritePhoneBookEntryExt EPBUM Email Error!!!!");
        }
    }

error:
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
}

// PHB Enhance
void RmcPhbRequestHandler:: requestQueryUPBAvailable(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "requestQueryUPBAvailable: entering");
    sp<RfxAtResponse> p_response = NULL;
    int err;
    int response[3] = {0};
    int tem;
    RfxAtLine *line;
    String8 cmd("");
    int* params = (int *) msg->getData()->getData();

    // query the info +EPBUM:<type>, <INDEX1>, <M_NUM>, <A_NUM>, <L_XXX>
    cmd.append(String8::format("AT+EPBUM=0,%d,%d", params[0], params[1]));
    p_response = atSendCommandSingleline(cmd, "+EPBUM:");
    if (p_response != NULL && p_response->getError() >= 0 && p_response->getSuccess() > 0) {
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        tem = line->atTokNextint(&err);
        tem = line->atTokNextint(&err);
        response[0] = line->atTokNextint(&err);
        response[1] = line->atTokNextint(&err);
        response[2] = line->atTokNextint(&err);
    }

    sp<RfxMclMessage> res = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(response, 3), msg, false);
    responseToTelCore(res);
    return;
}

void RmcPhbRequestHandler:: requestReadUPBEmail(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int err = 0;
    int tmp, strLen;
    char *out = NULL;
    RfxAtLine *line = NULL;
    String8 cmd("");
    int* params = (int *) msg->getData()->getData();
    char* response = NULL;
    sp<RfxMclMessage> res;

    if (maxEmailNum == -1) {
        loadUPBCapability();
    }

    // email
    if (maxEmailNum > 0) {
        cmd.append(String8::format("AT+EPBUM=1,1,%d,%d", params[0], params[1]));

        // +epbum:<type>,<adn_index>,<ef_index>,<email>
        p_response = atSendCommandSingleline(cmd, "+EPBUM:");
        if (p_response == NULL || p_response->getError() < 0) {
            // logE(RFX_LOG_TAG, "requestReadUPBEmail email epbum err: %d", err);
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
            goto error;
        } else {
              ret = RIL_E_SUCCESS;
        }

        if (p_response->getIntermediates() != NULL) {
            line = p_response->getIntermediates();
            line->atTokStart(&err);
            if (err < 0) goto error;
            tmp = line->atTokNextint(&err);
            if (err < 0) goto error;
            tmp = line->atTokNextint(&err);
            if (err < 0) goto error;
            tmp = line->atTokNextint(&err);
            if (err < 0) goto error;
            out = line->atTokNextstr(&err);
            if (err < 0) goto error;
            response = (char*)alloca(strlen(out) + 1);
            strncpy(response, out, strlen(out) + 1);
            response[strlen(out)] = static_cast<char>('\0');
            // logD(RFX_LOG_TAG, "requestReadUPBEmail email %s",  response); // pii
            // response = ascii2UCS2(response);
        }
        strLen = response == NULL ? 0 : strlen(response);

        res = RfxMclMessage::obtainResponse(msg->getId(), ret,
                RfxStringData(response, strLen), msg, false);
        responseToTelCore(res);
        return;
    }
error:
    // logE(RFX_LOG_TAG, "requestReadUPBEmail error: %d", err);
    res = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(res);
}

void RmcPhbRequestHandler:: requestReadUPBSne(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int err = 0;
    int tmp, strLen;
    RfxAtLine* line = NULL;
    String8 cmd("");
    char *current;
    int* params = (int *) msg->getData()->getData();
    int datalen = msg->getData()->getDataLength();
    int paramCount = datalen/sizeof(int);
    char* response = NULL;
    sp<RfxMclMessage> res;

    if (paramCount < 1) {
        logE(RFX_LOG_TAG, "requestReadUpbGrpEntry param is not enough. paramCount is %d", paramCount);
        goto error;
    }

    // SNE
    cmd.append(String8::format("AT+EPBUM=1,2,%d,%d", params[0], 1));
    // +epbum:<type>,<adn_index>,<ef_index>,<sne>,<encode_type>
    p_response = atSendCommandSingleline(cmd, "+EPBUM:");
    if (p_response == NULL || p_response->getError() < 0) {
        // logE(RFX_LOG_TAG, "requestReadUPBSne sne epbum err: %d", err);
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

      logD(RFX_LOG_TAG, "requestReadUPBSne ret: %d", ret);
      if (p_response->getSuccess() != 0 && p_response->getIntermediates() != NULL) {
          ret = RIL_E_SUCCESS;
          line = p_response->getIntermediates();
          line->atTokStart(&err);
          // logD(RFX_LOG_TAG, "requestReadUPBSne: strlen of response is %d", strlen(line) );
          tmp = line->atTokNextint(&err);
          if (err < 0) goto error;
          tmp = line->atTokNextint(&err);
          if (err < 0) goto error;
          tmp = line->atTokNextint(&err);
          if (err < 0) goto error;
          response = line->atTokNextstr(&err);
          current = line->getCurrentLine();
          if (current == NULL || *current == '\0') {
              logE(RFX_LOG_TAG, "requestReadUPBSne  SNE is NULL");
              goto error;
          }
          if (err < 0) goto error;
          tmp = line->atTokNextint(&err);
          if (err < 0) goto error;
          logD(RFX_LOG_TAG, "requestReadUPBSne: encode type is %d", tmp);

          if (tmp == CPBW_ENCODE_IRA) {
              response = ascii2UCS2(response);
              if (response == NULL) {
                  ret = RIL_E_NO_MEMORY;
                  goto error;
              }
          }
          DlogD(RFX_LOG_TAG, "requestReadUPBSne sne %s",  response);
      }

      strLen = (response == NULL ? 0 : strlen(response));

      res = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxStringData(response, strLen), msg, false);
      responseToTelCore(res);
      if (response != NULL && tmp == CPBW_ENCODE_IRA) {
          free(response);
      }
      return;
error:
    // logE(RFX_LOG_TAG, "requestReadUPBSne error: %d", err);
    res = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(res);
}

void RmcPhbRequestHandler::requestReadUPBAnr(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int err = 0;
    int tmp;
    char *out;
    RfxAtLine* line = NULL;
    String8 cmd("");
    int* params = (int *) msg->getData()->getData();
    RIL_PhbEntryStructure *entry = NULL;
    RIL_PhbEntryStructure **entries = NULL;
    sp<RfxMclMessage> response;

    if (maxAnrNum == -1) {
        loadUPBCapability();
    }
    // ANR
    if ( maxAnrNum > 0 ) {
        entries = (RIL_PhbEntryStructure**)calloc(1, sizeof(RIL_PhbEntryStructure*)*1);
        if (entries == NULL) {
            logE(RFX_LOG_TAG, "entries calloc fail");
            goto error;
        }
        entries[0] = entry = (RIL_PhbEntryStructure*)calloc(1, sizeof(RIL_PhbEntryStructure));
        if (entry == NULL) {
            logE(RFX_LOG_TAG, "entry calloc fail");
            goto error;
        }
        cmd.append(String8::format("AT+EPBUM=1,0,%d,%d", params[0], params[1]));

        // +epbum:<type>,<adn_index>,<ef_index>,<anr>,<type>,<aas>
        p_response = atSendCommandSingleline(cmd, "+EPBUM:");
        if (p_response == NULL || p_response->getError() < 0) {
            // logE(RFX_LOG_TAG, "requestReadUPBAnr anr epbum error: %d", err);
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
        logD(RFX_LOG_TAG, "requestReadUPBAnr ret: %d", ret);
        if (p_response->getSuccess() != 0 && p_response->getIntermediates() != NULL) {
            ret = RIL_E_SUCCESS;
            line = p_response->getIntermediates();
            line->atTokStart(&err);
            if (err < 0) goto error;
            tmp = line->atTokNextint(&err);
            if (err < 0) goto error;
            tmp = line->atTokNextint(&err);
            if (err < 0) goto error;
            tmp = line->atTokNextint(&err);
            if (err < 0) goto error;
            out = line->atTokNextstr(&err);
            entry->number = (char*)alloca(strlen(out) + 1);
            strncpy(entry->number, out, strlen(out) + 1);
            if (err < 0) goto error;
            entry->ton = line->atTokNextint(&err);;
            if (err < 0) goto error;
            entry->index = line->atTokNextint(&err);
            // logD(RFX_LOG_TAG, "requestReadUPBAnr PhbEntry anr %d: %s, %d", entry->index,
            //                entry->number, entry->ton); // pii
            entry->alphaId = (char*)"";
            entry->type = 0;
        }

        response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxPhbEntriesData(entries, 1), msg, false);
        responseToTelCore(response);
        if (entries[0] != NULL) {
            free(entries[0]);
        }
        free(entries);
        return;
     }
error:
    // logE(RFX_LOG_TAG, "requestReadUPBAnr error: %d", err);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    if (entries != NULL) {
        if (entries[0] != NULL) {
            free(entries[0]);
        }
        free(entries);
    }
}

void RmcPhbRequestHandler::requestSetPhonebookReady(const sp<RfxMclMessage>& msg) {
    int *pInt = (int *)msg->getData()->getData();
    int ready = pInt[0];

    if (ready == 1) {
        setMSimProperty(m_slot_id, (char*)PROPERTY_RIL_PHB_READY, (char*)"true");
    } else if (ready == 0) {
        setMSimProperty(m_slot_id, (char*)PROPERTY_RIL_PHB_READY, (char*)"false");
    }
    logD(RFX_LOG_TAG, "requestSetPhonebookReady complete state = %d", ready);
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg, false);
    // response to TeleCore
    responseToTelCore(response);

    return;
}

void RmcPhbRequestHandler::requestReadAasList(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    String8 cmd("");
    RfxAtLine* line = NULL;
    int tmp, i, entryIndex;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    int* params = (int *) msg->getData()->getData();
    char **responses = NULL;
    char * out;
    int datalen = msg->getData()->getDataLength();
    sp<RfxMclMessage> response;

    if (datalen/sizeof(int) < 2 || params[0] < 0 || params[1] < params[0]) {
        logE(RFX_LOG_TAG, "requestReadAasList param is not enough. datalen is %d", datalen);
        goto error;
    }
    responses = (char**)calloc(1, sizeof(char*)*(params[1] - params[0] + 1));
    if (responses == NULL) {
        logE(RFX_LOG_TAG, "responses calloc fail");
        ret = RIL_E_NO_MEMORY;
        goto error;
    }

    for (i = params[0]; i <= params[1]; i++) {
        cmd.clear();
        cmd.append(String8::format("AT+EPBUM=1,3,%d,0", i));

        p_response = atSendCommandSingleline(cmd, "+EPBUM:");
        if (p_response == NULL || p_response->getError() < 0) {
            logD(RFX_LOG_TAG, "requestReadAasList: the %d th aas entry is null", i);
            ret = RIL_E_SUCCESS;
            continue;  // it means the index is not used.
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                logE(RFX_LOG_TAG, "p_response = %d /n", p_response->atGetCmeError());
                case CME_SUCCESS:
                    ret = RIL_E_GENERIC_FAILURE;
                    break;
                case CME_UNKNOWN:
                    ret = RIL_E_GENERIC_FAILURE;
                    logE(RFX_LOG_TAG, "p_response: CME_UNKNOWN");
                    break;
                default:
                    ret = RIL_E_GENERIC_FAILURE;
                    logE(RFX_LOG_TAG, "p_response: Not success");
                    break;
            }
        } else {
            ret = RIL_E_SUCCESS;
        }

        if (ret != RIL_E_SUCCESS) {
            logD(RFX_LOG_TAG, "requestReadAasList: Error!");
            // goto error;
            continue;
        }

        if (p_response->getIntermediates() == NULL) continue;
        line = p_response->getIntermediates();
        line->atTokStart(&err);
        // logD(RFX_LOG_TAG, "requestReadAasList: strlen of response is %d", strlen(line));

        tmp = line->atTokNextint(&err);  // type
        if (err < 0 || tmp != 3) continue;

        entryIndex = line->atTokNextint(&err);  // entry_index
        if (err < 0 || entryIndex < 1) continue;;
        // logD(RFX_LOG_TAG, "requestReadAasList: (entryIndex-1)=%d", entryIndex-1);

        tmp = line->atTokNextint(&err);  // adn_index
        out = line->atTokNextstr(&err);  // aas string
        if (err < 0) continue;
        logD(RFX_LOG_TAG, "requestReadAasList:  %s", out);

        tmp = line->atTokNextint(&err);
        if (err < 0) continue;
        logD(RFX_LOG_TAG, "requestReadAasList: encode type is %d", tmp);

        if (tmp == CPBW_ENCODE_IRA) {
            out = ascii2UCS2(out);
            if (out == NULL) {
                free(responses);
                ret = RIL_E_NO_MEMORY;
                goto error;
            }
            responses[entryIndex-1] = (char*)alloca(strlen(out) + 1);
            strncpy(responses[entryIndex-1], out, strlen(out) + 1);
            free(out);  // calloc in ascii2UCS2
        } else {
            responses[entryIndex-1] = (char*)alloca(strlen(out) + 1);
            strncpy(responses[entryIndex-1], out, strlen(out) + 1);
        }

        logD(RFX_LOG_TAG, "requestReadAasList:  [%d], %s", entryIndex-1, responses[entryIndex-1]);
    }

    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxStringsData((char **)responses, (params[1] - params[0] + 1)), msg, false);
    responseToTelCore(response);
    free(responses);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
            RfxVoidData(), msg, false);
    responseToTelCore(response);
    /* if (responses != NULL) {  // remove due to logically dead code
        free(responses);
    }*/
}
// PHB Enhance
// PHB End
