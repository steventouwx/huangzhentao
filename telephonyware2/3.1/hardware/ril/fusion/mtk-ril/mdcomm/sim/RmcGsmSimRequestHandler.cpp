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
#include "RmcSimBaseHandler.h"
#include "RmcCommSimDefs.h"
#include "RmcGsmSimRequestHandler.h"
#include "RmcCommSimRequestHandler.h"
#include "RmcSimRequestEntryHandler.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxSimMeLockCatData.h"
#include "RfxVoidData.h"
#include <telephony/mtk_ril.h>
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RfxMessageId.h"
#include <time.h>
#include <sys/time.h>
#include <libmtkrilutils.h>
#include <prop/properties.h>

static const int ch1ReqList[] = {
    RFX_MSG_REQUEST_GET_IMSI,
    RFX_MSG_REQUEST_QUERY_FACILITY_LOCK,
    RFX_MSG_REQUEST_SET_FACILITY_LOCK,
    RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK,
    RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK,
    RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS,
    RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS,
    RFX_MSG_REQUEST_GET_SUBLOCK_VERSION,
    RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME,
};

static const int ch3ReqList[] = {
};

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxStringData, RFX_MSG_REQUEST_GET_IMSI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_QUERY_FACILITY_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxIntsData,
        RFX_MSG_REQUEST_SET_FACILITY_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringsData, RfxVoidData,
        RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxSimMeLockCatData,
        RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_GET_SUBLOCK_VERSION);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxIntsData,
        RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME);

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RmcGsmSimRequestHandler::RmcGsmSimRequestHandler(int slot_id, int channel_id) :
        RmcSimBaseHandler(slot_id, channel_id) {
    setTag(String8("RmcGsmSimRequest"));
}

RmcGsmSimRequestHandler::~RmcGsmSimRequestHandler() {
}



const int* RmcGsmSimRequestHandler::queryTable(int channel_id, int *record_num) {

    if (channel_id == RIL_CMD_PROXY_1) {
        *record_num = sizeof(ch1ReqList)/sizeof(int);
        return ch1ReqList;
    } else if (channel_id == RIL_CMD_PROXY_3) {
        *record_num = sizeof(ch3ReqList)/sizeof(int);
        return ch3ReqList;
    } else {
        // No request registered!
    }

    return NULL;
}

RmcSimBaseHandler::SIM_HANDLE_RESULT RmcGsmSimRequestHandler::needHandle(
        const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    RmcSimBaseHandler::SIM_HANDLE_RESULT result = RmcSimBaseHandler::RESULT_IGNORE;

    switch(request) {
        case RFX_MSG_REQUEST_GET_IMSI:
            {
                int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
                char** pStrings = (char**)(msg->getData()->getData());

                if (pStrings == NULL && (cardType & RFX_CARD_TYPE_SIM)) {
                    result = RmcSimBaseHandler::RESULT_NEED;
                } else if (pStrings != NULL) {
                    String8 aid((pStrings[0] != NULL)? pStrings[0] : "");
                    if (strncmp(aid.string(), "A0000000871002", 14) == 0 ||
                        (aid.isEmpty() && (cardType & RFX_CARD_TYPE_SIM))) {
                        result = RmcSimBaseHandler::RESULT_NEED;
                    }
                }
            }
            break;
        case RFX_MSG_REQUEST_QUERY_FACILITY_LOCK:
        case RFX_MSG_REQUEST_SET_FACILITY_LOCK:
            {
                int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
                char** pStrings = (char**)(msg->getData()->getData());
                char *pAid = ((request == RFX_MSG_REQUEST_QUERY_FACILITY_LOCK)?
                        pStrings[3] : pStrings[4]);
                String8 aid((pAid != NULL)? pAid : "");
                String8 facility(pStrings[0]);
                String8 facFd("FD");
                String8 facSc("SC");

                if (facility != facFd && facility != facSc) {
                    logD(mTag, "No need to check aid for call barring, cardType: %x", cardType);
                    result = RmcSimBaseHandler::RESULT_NEED;
                } else if (aid.isEmpty() && (cardType & RFX_CARD_TYPE_SIM)) {
                    result = RmcSimBaseHandler::RESULT_NEED;
                } else if (!aid.isEmpty()) {
                    if ((strncmp(aid.string(), "A0000000871002", 14) == 0)
                            || (strncmp(aid.string(), "A0000000871004", 14) == 0)) {
                        result = RmcSimBaseHandler::RESULT_NEED;
                    }
                }
            }
            break;
        case RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK:
        case RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK:
        case RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS:
        case RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS:
        case RFX_MSG_REQUEST_GET_SUBLOCK_VERSION:
        case RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME:
            result = RmcSimBaseHandler::RESULT_NEED;
            break;
        default:
            logE(mTag, "Not support the request!");
            break;
    }

    return result;
}

void RmcGsmSimRequestHandler::handleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_GET_IMSI:
            handleGetImsi(msg);
            break;
        case RFX_MSG_REQUEST_QUERY_FACILITY_LOCK:
            handleQueryFacilityLock(msg);
            break;
        case RFX_MSG_REQUEST_SET_FACILITY_LOCK:
            handleSetFacilityLock(msg);
            break;
        case RFX_MSG_REQUEST_SIM_SET_SIM_NETWORK_LOCK:
            handleSetSimNetworkLock(msg);
            break;
        case RFX_MSG_REQUEST_SIM_QUERY_SIM_NETWORK_LOCK:
            handleQuerySimNetworkLock(msg);
            break;
        case RFX_MSG_REQUEST_UPDATE_SUBLOCK_SETTINGS:
            handleReqSubsidyLockUpdateSettings(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_MODEM_STATUS:
            handleReqSubsidyLockGetModemStatus(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_VERSION:
            handleReqSubsidyLockGetVersion(msg);
            break;
        case RFX_MSG_REQUEST_GET_SUBLOCK_UNLOCK_TIME:
            handleReqSubsidyLockGetUnlockTime(msg);
            break;
        default:
            logE(mTag, "Not support the request!");
            break;
    }
}

char* RmcGsmSimRequestHandler::getReducedLog(char* data) {
    static char dataToPrint[8];
    memset(dataToPrint, 0, sizeof(dataToPrint));
    int len = 0;
    if (data == NULL) {
        logD(mTag, "getReducedLog: dataToPrint is null");
    } else {
        len = strlen(data);
        if (len > 6) {
            strncpy(dataToPrint, data, 6);
        } else {
            strcpy(dataToPrint, data);
        }
    }
    return dataToPrint;
}

#define MAX_SUBSIDY_DATA_SIZE    1400

void RmcGsmSimRequestHandler::handleReqSubsidyLockUpdateSettings(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    int err = -1;
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxAtResponse> p_response1 = NULL;
    sp<RfxAtResponse> p_response2 = NULL;

    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_GENERIC_FAILURE;
    int result[2] = {0};
    result[0] = 1;    // SUBSIDYLOCK COMMAND = 1

    char *current_data = (char*)(msg->getData()->getData());
    char* lock_data = NULL;
    int length = 0, is_last = 1;
    int data_size = 0;
    int blob_status = 0;
    int simCount = 0;

    int data_length = msg->getData()->getDataLength();
    logI(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings data_length=%d", data_length);
    if (data_length > 0 && current_data != NULL) {
        if (current_data[data_length - 1] != '\0') {
          logE(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings blob is not null-terminated\n");
          goto done;
        }
    } else if (current_data != NULL) {
          logE(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings invalid data_length\n");
          goto done;
    }

    data_size = (current_data != NULL ? strlen(current_data) : 0);
    if (data_size < 8) {
        logE(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings blob size < 8\n");
        goto done;
    }

    // parse blob status to get lock status bit. 0 : SUBSIDYLOCK, 1: PERMANENT_UNLOCK
    blob_status = current_data[7] - '0';
    logD(mTag, "[SUBSIDY_LOCK]handleUpdateSimLockSettings update blob_status to: %d, strings %s***",
            blob_status, getReducedLog(current_data));

    if (blob_status == 0) {
        logD(mTag, "[SUBSIDY_LOCK]blob update request to SUBSIDYLOCK");
    } else {
        logD(mTag, "[SUBSIDY_LOCK]blob update request to PERMANENT_UNLOCK");
    }

    do {
        ril_error = RIL_E_GENERIC_FAILURE;
        current_data = current_data + length;
        if (data_size >= MAX_SUBSIDY_DATA_SIZE) {
            is_last = 0;
            length = MAX_SUBSIDY_DATA_SIZE;
        } else {
            is_last = 1;
            length = data_size;
        }
        logD(mTag, "[SUBSIDY_LOCK] data_size %d, length %d, is_last %d",
                data_size, length, is_last);
        lock_data = (char*)calloc(1, length + 1);
        RFX_ASSERT(lock_data != NULL);
        strncpy(lock_data, current_data, length);

        cmd.clear();
        cmd.append(String8::format("AT+ESLBLOB=1,%d,\"%s\",%d",
                (unsigned int)length+1, lock_data, is_last));
        logD(mTag,"AT+ESLBLOB=1,%d,\"%s***\",%d", (unsigned int)length+1,
                getReducedLog(lock_data), is_last);

        p_response = atSendCommand(cmd);
        free(lock_data);

        if (p_response == NULL) {
            logE(mTag, "[RSU-SIMLOCK] atResponse is null");
            goto done;
        }

        err = p_response->getError();
        if (err < 0) {
            logD(mTag, "handleReqSubsidyLockUpdateSettings fail.\n");
            goto done;
        }

        if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                logD(mTag, "p_response %d\n", p_response->atGetCmeError());
                case CME_BLOB_VALIDATION_FAILED:
                    ril_error = RIL_E_VALIDATION_FAILED;
                    logD(mTag, "p_response: CME_BLOB_VALIDATION_FAILED\n");
                    goto done;
                case CME_BLOB_CONFIG_DATA_INVALID:
                    ril_error = RIL_E_CONFIG_DATA_INVALID;
                    logD(mTag, "p_response: CME_BLOB_CONFIG_DATA_INVALID\n");
                    goto done;
                case CME_BLOB_IMEI_MISMATCH:
                    ril_error = RIL_E_IMEI_MISMATCH;
                    logD(mTag, "p_response: CME_BLOB_IMEI_MISMATCH\n");
                    goto done;
                case CME_BLOB_LENGTH_MISMATCH:
                    ril_error = RIL_E_LENGTH_MISMATCH;
                    logD(mTag, "p_response: CME_BLOB_LENGTH_MISMATCH\n");
                    goto done;
                case CME_UNKNOWN_ERROR:
                    ril_error = RIL_E_UNKNOWN_ERROR;
                    logD(mTag, "p_response: CME_UNKNOWN_ERROR\n");
                    goto done;
                case CME_REBOOT_REQUIRED:
                    ril_error = RIL_E_SUCCESS;
                    logD(mTag, "p_response: CME_REBOOT_REQUIRED\n");
                    //reset all the sims
                    simCount = RfxRilUtils::rfxGetSimCount();
                    for (int i=0; i<simCount; i++) {
                        logD(mTag, "handleReqSubsidyLockUpdateSettings: sendEvent to %d", i);
                        sendEvent(RFX_MSG_EVENT_SIM_RESET, RfxVoidData(), RIL_CMD_PROXY_7, i);
                    }
                    goto done;
                default:
                    goto done;
            }
        } else {
            logE(mTag, "handleReqSubsidyLockUpdateSettings success, E_REBOOT_NOT_REQUIRED\n");
            ril_error = RIL_E_SUCCESS;
        }
        data_size = data_size - length;
        logD(mTag, "data_size %d", data_size);
    } while (data_size > 0);

    if (ril_error == RIL_E_SUCCESS) {
        logE(mTag, "update blob_status property");
                if (blob_status == 1) {
                    // PERMANENT UNLOCK CASE:
            // property_set("persist.subsidylock.lockstatus_ril", "0");
                } else {
                    // SUBSIDYLOCK CASE:
            // property_set("persist.subsidylock.lockstatus_ril", "1");
        }
    }

done:
    result[1] = (int) ril_error;
    logD(mTag, "reqType = %d , result[1] = %d, ril_error = %d", result[0], result[1], ril_error);
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                            RfxIntsData((void*)result, 2*sizeof(int)), msg, false);
    responseToTelCore(response);
}

void RmcGsmSimRequestHandler::handleReqSubsidyLockGetModemStatus(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    RfxAtLine *line = NULL;
    int err = -1;
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_GENERIC_FAILURE;
    int result[2] = {0};
    result[0] = 2;  // SUBSIDYLOCK COMMAND = 2
    result[1] = RIL_E_GENERIC_FAILURE;
    logD(mTag, "[SUBSIDY_LOCK]handleReqSubsidyLockGetModemStatus\n");

    cmd.append(String8::format("AT+ESLBLOB=2"));
    logD(mTag,  "[SUBSIDY_LOCK] AT+ESLBLOB=2");
    p_response = atSendCommandSingleline(cmd, "+ESLBLOB:");
    cmd.clear();
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logD(mTag, "handleReqSubsidyLockGetModemStatus fail.\n");
        goto done;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        goto done;
    }

    // Get status
    result[1] = line->atTokNextint(&err);
    if (err < 0) {
        result[1] = (int) ril_error;
        goto done;
    }

    logD(mTag, "modem status err = %d , status = %d", err, result[1]);

    ril_error = RIL_E_SUCCESS;
done:
    logD(mTag, "[SUBSIDY_LOCK]done.\n");
    if (p_response != NULL) {
        // logD(mTag, "[SUBSIDY_LOCK]status %s.\n", result[1]);
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData((void*)result, 2*sizeof(int)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(), msg, false);
    }
    responseToTelCore(response);
}

/**
  * AT+ESLBLOB=6
  *
  * +ESLBLOB:<major_ver>,<minor_ver>
  *
  *  In response:
  *    result[0]: SUBSIDYLOCK COMMAND
  *    result[1]: return status: 0 -- success, 1 -- fail
  *    result[2]: version
  */
void RmcGsmSimRequestHandler::handleReqSubsidyLockGetVersion(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    RfxAtLine *line = NULL;
    int err = -1;
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_GENERIC_FAILURE;
    int result[3] = {0};
    result[0] = 3;  // SUBSIDYLOCK COMMAND = 3
    logD(mTag, "[SUBSIDY_LOCK]handleReqSubsidyLockGetVersion\n");

    cmd.append(String8::format("AT+ESLBLOB=6"));
    logD(mTag, "[SUBSIDY_LOCK] AT+ESLBLOB=6");
    p_response = atSendCommandSingleline(cmd, "+ESLBLOB:");
    cmd.clear();
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        logD(mTag, "handleReqSubsidyLockGetVersion fail.\n");
        goto done;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        goto done;
    }

    // Get major_ver
    result[2] = line->atTokNextint(&err);
    if (err < 0) {
        goto done;
    }

    ril_error = RIL_E_SUCCESS;
done:
    if (ril_error == RIL_E_SUCCESS) {
        result[1] = 0;
    } else {
        result[1] = 1;
    }
    logD(mTag, "SubsidyLockGetVersion err = %d , status = %d, version = %d",
            err, result[1], result[2]);
    if (p_response != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData((void*)result, sizeof(result)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(), msg, false);
    }
    responseToTelCore(response);
}

#define INVALID_HEX_NUM 255

uint8_t RmcGsmSimRequestHandler::hexCharToInt(uint8_t c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    return INVALID_HEX_NUM;
}

/**
 * Convert hexStr[start, end) to unsigned 32 bit int value
 *
 * return:
 *   0 -- success
 *   other -- fail
 */
int RmcGsmSimRequestHandler::convertHexStringToUint32(const uint8_t *hexStr, int start, int end,
        uint32_t *pValue) {
    uint32_t value = 0;

    if (hexStr == NULL || pValue == NULL) {
        logE(mTag, "convertHexStringToUint32: hexStr/pValue is null!");
        return -1;
    }

    if (((end - start) > 8) || (end <= start)) {
        logE(mTag, "convertHexStringToUint32: invalid parameter start=%d, end=%d!",
               start, end);
        return -1;
    }

    for (int i = start; i < end; i++) {
        uint8_t hexChar = hexCharToInt(hexStr[i]);
        if (hexChar == INVALID_HEX_NUM) {
            logE(mTag, "convertHexStringToUint32: invalid hex char %d", hexStr[i]);
            return -1;
        }
        value = (value << 4) | hexChar;
    }
    *pValue = value;
    return 0;
}

/**
  * AT+ESLBLOB=7
  *
  * +ESLBLOB:<unlock_time_left>
  *
  *  In response:
  *    result[0]: SUBSIDYLOCK COMMAND
  *    result[1]: return status: 0 -- success, 1 -- fail
  *    result[2]: High word value
  *    result[3]: Low word value
  */
void RmcGsmSimRequestHandler::handleReqSubsidyLockGetUnlockTime(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    size_t hexLen = 0;
    RfxAtLine *line = NULL;
    const uint8_t *tempStr = NULL;
    int err = -1;
    sp<RfxAtResponse> p_response = NULL;
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_GENERIC_FAILURE;
    int result[4] = {0};

    result[0] = 4;  // SUBSIDYLOCK COMMAND = 4
    logD(mTag, "[SUBSIDY_LOCK]handleReqSubsidyLockGetUnlockTime\n");

    cmd.append(String8::format("AT+ESLBLOB=7"));
    logD(mTag, "[SUBSIDY_LOCK] AT+ESLBLOB=7");
    p_response = atSendCommandSingleline(cmd, "+ESLBLOB:");
    cmd.clear();
    err = p_response->getError();

    if (err < 0 || p_response->getSuccess() == 0) {
        logD(mTag, "handleReqSubsidyLockGetUnlockTime fail.\n");
        goto done;
    }

    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if (err < 0) {
        goto done;
    }

    tempStr = (uint8_t *)line->atTokNextstr(&err);
    if (err < 0 || tempStr == NULL) {
        goto done;
    }

    hexLen = strlen((const char *)tempStr);
    if (hexLen > 16 || hexLen == 0) {
        logE(mTag, "Invalid unlock time string 1: %s", tempStr);
        goto done;
    }

    if (hexLen <= 8) {
        if (convertHexStringToUint32(tempStr, 0, hexLen, (uint32_t *)&result[3]) !=  0) {
            logE(mTag, "Invalid unlock time string 2: %s", tempStr);
            goto done;
        }
    } else {
        if (convertHexStringToUint32(tempStr, 0, hexLen - 8, (uint32_t *)&result[2]) !=  0) {
            logE(mTag, "Invalid unlock time string 3: %s", tempStr);
            goto done;
        }

        if (convertHexStringToUint32(tempStr, hexLen - 8, hexLen, (uint32_t *)&result[3]) !=  0) {
            logE(mTag, "Invalid unlock time string 4: %s", tempStr);
            goto done;
        }
    }

    ril_error = RIL_E_SUCCESS;

done:
    if (ril_error == RIL_E_SUCCESS) {
        result[1] = 0;
    } else {
        result[1] = 1;
    }
    logD(mTag, "SubsidyLockGetUnlockTime err = %d , status = %d, hword = %X, lword = %X",
            err, result[1], result[2], result[3]);

    if (p_response != NULL) {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData((void*)result, sizeof(result)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(), msg, false);
    }
    responseToTelCore(response);
}

void RmcGsmSimRequestHandler::handleGetImsi(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err;
    sp<RfxMclMessage> response;

    p_response = atSendCommandNumeric("AT+CIMI");

    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
                RfxStringData(NULL, 0), msg, false);
    } else {
        String8 imsi(p_response->getIntermediates()->getLine());
        getMclStatusManager()->setString8Value(RFX_STATUS_KEY_GSM_IMSI, imsi);

        response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxStringData((void*)imsi.string(), imsi.length()), msg, false);
    }
    responseToTelCore(response);
}

void RmcGsmSimRequestHandler::handleQueryFacilityLock(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err, count = 0, enable = -1;
    int appTypeId = -1;
    int fdnServiceResult = -1;
    int cardType = getMclStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
    char** pStrings = (char**)(msg->getData()->getData());
    String8 facFd("FD");
    String8 facSc("SC");
    String8 facility(pStrings[0]);
    String8 aid((pStrings[3] != NULL)? pStrings[3] : "");
    String8 cmd("");
    RfxAtLine *line = NULL;
    sp<RfxMclMessage> response;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    if (facility.isEmpty()) {
        logE(mTag, "The facility string is empty.");
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    // Get app type id
    appTypeId = queryAppTypeId(aid);

    if (appTypeId == -1) {
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    do {
        /* No network related query. CLASS is unnecessary */
        if (facility == facFd) {
            // Use AT+ESSTQ=<app>,<service> to query service table
            // 0:  Service not supported
            // 1:  Service supported
            // 2:  Service allocated but not activated
            if (cardType & RFX_CARD_TYPE_USIM) {
                cmd.append(String8::format("AT+ESSTQ=%d,%d", 1, 2));
            } else {
                cmd.append(String8::format("AT+ESSTQ=%d,%d", 0, 3));
            }
            p_response = atSendCommandSingleline(cmd, "+ESSTQ:");
            cmd.clear();

            err = p_response->getError();
            // The same as AOSP. 0 - Available & Disabled, 1-Available & Enabled, 2-Unavailable.
            if (err < 0 || p_response->getSuccess() == 0) {
                logE(mTag, "Fail to query service table");
            } else {
                line = p_response->getIntermediates();

                line->atTokStart(&err);
                fdnServiceResult = line->atTokNextint(&err);
                if (err < 0) goto error;

                if (fdnServiceResult == 0) {
                    fdnServiceResult = 2;
                } else if (fdnServiceResult == 2) {
                    fdnServiceResult = 0;
                }
                logD(mTag, "FDN available: %d", fdnServiceResult);
            }
            p_response = NULL;
        }
        cmd.append(String8::format("AT+ECLCK=%d,\"%s\",2", appTypeId, facility.string()));

        p_response = atSendCommandSingleline(cmd, "+ECLCK:");
        err = p_response->getError();

        cmd.clear();

        if (err < 0) {
            logE(mTag, "getFacilityLock Fail");
            goto error;
        } else if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                case CME_SIM_BUSY:
                    logD(mTag, "simFacilityLock: CME_SIM_BUSY");
                    sleepMsec(200);
                    count++;
                    p_response = NULL;
                    ril_error = RIL_E_SIM_BUSY;
                    break;
                case CME_SIM_PIN_REQUIRED:
                case CME_SIM_PUK_REQUIRED:
                    ril_error = RIL_E_PASSWORD_INCORRECT;
                    goto error;
                case CME_SIM_PIN2_REQUIRED:
                    ril_error = RIL_E_SIM_PIN2;
                    goto error;
                case CME_SIM_PUK2_REQUIRED:
                    ril_error = RIL_E_SIM_PUK2;
                    goto error;
                case CME_INCORRECT_PASSWORD:
                    ril_error = RIL_E_PASSWORD_INCORRECT;
                    goto error;
                case CME_PHB_FDN_BLOCKED:
                    ril_error = RIL_E_FDN_CHECK_FAILURE;
                    goto error;
                default:
                    logD(mTag, "simFacilityLock: default");
                    goto error;
            }
        } else {
            // Success
            line = p_response->getIntermediates();

            line->atTokStart(&err);
            if (err < 0) goto error;

            /* 0 disable 1 enable */
            enable = line->atTokNextint(&err);
            if (err < 0) goto error;

            ril_error = RIL_E_SUCCESS;
            if (fdnServiceResult == -1) {
                response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                        RfxIntsData((void*)&enable, sizeof(int)), msg, false);
            } else {
                if (fdnServiceResult == 1 && enable == 0) {
                    fdnServiceResult = 0;
                }
                logD(mTag, "final FDN result: %d", fdnServiceResult);
                response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                        RfxIntsData((void*)&fdnServiceResult, sizeof(int)), msg, false);
            }
            responseToTelCore(response);
        }

        if(count == 13) {
            logE(mTag, "Set Facility Lock: CME_SIM_BUSY and time out.");
            goto error;
        }
    } while (ril_error == RIL_E_SIM_BUSY);

    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
            RfxIntsData(), msg, false);
    responseToTelCore(response);
}

void RmcGsmSimRequestHandler::handleSetSimNetworkLock(const sp<RfxMclMessage>& msg) {
    String8 cmd("");
    int err = -1;
    RIL_Errno ret = RIL_E_GENERIC_FAILURE;
    sp<RfxMclMessage> response;
    sp<RfxAtResponse> p_response = NULL;
    UICC_Status sim_status;
    char** strings = (char**)(msg->getData()->getData());
    char* key = NULL;
    char* imsi = NULL;
    char* gid1 = NULL;
    char* gid2 = NULL;

    if (strings == NULL || strings[0] == NULL ||
            strings[1] == NULL) {
         ret = RIL_E_INVALID_ARGUMENTS;
         logD(mTag, "handleSetSimNetworkLock invalid arguments.");
         goto error;
    }

    // strings[0]: cat
    // strings[1]: op
    key = (strings[2] == NULL) ? (char*)"" : (char*)strings[2];
    imsi = (strings[3] == NULL) ? (char*)"" : (char*)strings[3];
    gid1 = (strings[4] == NULL) ? (char*)"" : (char*)strings[4];
    gid2 = (strings[5] == NULL) ? (char*)"" : (char*)strings[5];

    logD(mTag, "simNetworkLock strings %s,%s ...\n", strings[0], strings[1]);
    if(0 == strcmp (strings[1],"2")) { //add data
        if ((0 == strcmp(strings[0], "2")) || (0 == strcmp(strings[0], "5"))) {
            cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\",\"%s\",\"%s\"",
                    strings[0], strings[1], key, imsi, gid1));
        } else if ((0 == strcmp(strings[0], "3")) || (0 == strcmp(strings[0], "6"))) {
            cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\",\"%s\",\"%s\",\"%s\"",
                    strings[0], strings[1], key, imsi, gid1, gid2));
        } else {
            logD(mTag, "add data.");
            cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\",\"%s\"",
                    strings[0], strings[1], key, imsi));
        }
    } else if (0 == strcmp (strings[1],"3") || //remove data
             0 == strcmp (strings[1],"4")) { //disable data
        cmd.append(String8::format("AT+ESMLCK=%s,%s", strings[0], strings[1]));
    } else if (0 == strcmp (strings[1],"0") ) { //unlock
        sim_status = getSimStatus();//getSIMLockStatus(rid);
        logD(mTag, "unlock.");
        if ((UICC_NP == sim_status && (0 == strcmp(strings[0], "0"))) ||
                (UICC_NSP == sim_status && (0 == strcmp(strings[0], "1")))  ||
                (UICC_SP == sim_status && (0 == strcmp(strings[0], "2"))) ||
                (UICC_CP == sim_status && (0 == strcmp(strings[0], "3"))) ||
                (UICC_SIMP == sim_status && (0 == strcmp(strings[0], "4"))) ||
                (UICC_NS_SP == sim_status && (0 == strcmp(strings[0], "5"))) ||
                (UICC_SIM_C == sim_status && (0 == strcmp(strings[0], "6")))) {
            logE(mTag, "simsatus = %d, category = %s", sim_status, strings[0]);
            cmd.append(String8::format("AT+CPIN=\"%s\"", key));
            p_response = atSendCommand(cmd.string());
            err = p_response->getError();
            cmd.clear();
            if (err < 0) {
                logE(mTag, "err = %d", err);
                goto error;
            }
            if (p_response->getSuccess() == 0) {
                logE(mTag, "err = %d", p_response->atGetCmeError());
                switch (p_response->atGetCmeError()) {
                case CME_INCORRECT_PASSWORD:
                    goto error;
                    break;
                case CME_SUCCESS:
                    /* While p_response->success is 0, the CME_SUCCESS means CME ERROR:0 => it is phone failure */
                    goto error;
                    break;
                default:
                    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                            m_slot_id, RfxVoidData());
                    responseToTelCore(unsol);
                    cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\"", strings[0], strings[1], key));
                    break;
                }
            } else {
                sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                        m_slot_id, RfxVoidData());
                responseToTelCore(unsol);
                cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\"", strings[0], strings[1], key));
            }
        } else {
            cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\"", strings[0], strings[1], key));
        }
    } else if (0 == strcmp (strings[1],"1")) { //lock
        cmd.append(String8::format("AT+ESMLCK=%s,%s,\"%s\"", strings[0], strings[1], key));
    }
    p_response = atSendCommand(cmd.string());
    cmd.clear();
    logD(mTag, "network lock command sent.");
    err = p_response->getError();
    if (err < 0) {
        logE(mTag, "err = %d", err);
        goto error;
    }

    if (p_response->getSuccess() == 0) {
        logE(mTag, "p_response err = %d", p_response->atGetCmeError());
        switch (p_response->atGetCmeError()) {
            case CME_SUCCESS:
                ret = RIL_E_GENERIC_FAILURE;
                break;
            case CME_UNKNOWN:
                break;
            default:
                goto error;
        }
    } else {
        ret = RIL_E_SUCCESS;
    }
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), ret,
                        RfxVoidData(), msg, false);
    responseToTelCore(response);
}
void RmcGsmSimRequestHandler::handleQuerySimNetworkLock(const sp<RfxMclMessage>& msg) {
    int i = 0;
    int err = -1;
    String8 cmd("");
    RfxAtLine *line = NULL;
    char *value, *tmpStr;
    RIL_SimMeLockInfo result;
    RIL_SimMeLockCatInfo lockstate;
    sp<RfxAtResponse> p_response = NULL;
    int* cat = (int*)(msg->getData()->getData());
    sp<RfxMclMessage> response;

    logD(mTag, "handleQuerySimNetworkLock cat: %d", cat[0]);

    if (cat[0] < 0 || cat[0] >= MAX_SIM_ME_LOCK_CAT_NUM) {
        goto error;
    }
    cmd.append(String8::format("AT+ESMLCK"));
    p_response = atSendCommandSingleline(cmd, "+ESMLCK:");
    cmd.clear();
    err = p_response->getError();
    if (err < 0 || p_response->getSuccess() == 0) {
        goto error;
    }
    line = p_response->getIntermediates();
    line->atTokStart(&err);
    if(err < 0) goto error;

    for (i = 0; i < MAX_SIM_ME_LOCK_CAT_NUM; i++) {
        value = line->atTokChar(&err);
        if(err < 0) goto error;
        result.catagory[i].catagory = line->atTokNextint(&err);
        if(err < 0) goto error;
        result.catagory[i].state = line->atTokNextint(&err);
        if(err < 0) goto error;
        result.catagory[i].retry_cnt = line->atTokNextint(&err);
        if(err < 0) goto error;
        result.catagory[i].autolock_cnt = line->atTokNextint(&err);
        if(err < 0) goto error;
        result.catagory[i].num_set = line->atTokNextint(&err);
        if(err < 0) goto error;
        result.catagory[i].total_set = line->atTokNextint(&err);
        if(err < 0) goto error;
        result.catagory[i].key_state = line->atTokNextint(&err);
        if(err < 0) goto error;
    }
    tmpStr = line->atTokNextstr(&err);
    if(err < 0) goto error;
    strncpy(result.imsi, tmpStr, 15);

    result.isgid1 = line->atTokNextint(&err);
    if(err < 0) goto error;

    tmpStr = line->atTokNextstr(&err);
    if(err < 0) goto error;
    strncpy(result.gid1, tmpStr, 15);

    result.isgid2 = line->atTokNextint(&err);
    if(err < 0) goto error;

    tmpStr = line->atTokNextstr(&err);
    if(err < 0) goto error;

    strncpy(result.gid2, tmpStr, 15);
    result.mnclength = line->atTokNextint(&err);
    if(err < 0) goto error;

    lockstate.catagory= result.catagory[cat[0]].catagory;
    lockstate.state = result.catagory[cat[0]].state;
    lockstate.retry_cnt= result.catagory[cat[0]].retry_cnt;
    lockstate.autolock_cnt = result.catagory[cat[0]].autolock_cnt;
    lockstate.num_set = result.catagory[cat[0]].num_set;
    lockstate.total_set = result.catagory[cat[0]].total_set;
    lockstate.key_state = result.catagory[cat[0]].key_state;

    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxSimMeLockCatData((RIL_SimMeLockCatInfo*)&lockstate, sizeof(RIL_SimMeLockCatInfo)), msg, false);
    responseToTelCore(response);
    return;
error:
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_GENERIC_FAILURE,
            RfxSimMeLockCatData(NULL, 0), msg, false);
    responseToTelCore(response);
}

void RmcGsmSimRequestHandler::handleSetFacilityLock(const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response = NULL;
    int err, count = 0;
    int appTypeId = -1;
    int attemptsRemaining[4] = {0};
    char** pStrings = (char**)(msg->getData()->getData());
    String8 facFd("FD");
    String8 facSc("SC");
    String8 facility((pStrings[0] != NULL)? pStrings[0] : "");
    String8 lockStr((pStrings[1] != NULL)? pStrings[1] : "");
    String8 pwd((pStrings[2] != NULL)? pStrings[2] : "");
    String8 aid((pStrings[4] != NULL)? pStrings[4] : "");
    String8 cmd("");
    sp<RfxMclMessage> response;
    RmcSimPinPukCount *retry = NULL;
    RIL_Errno ril_error = RIL_E_SIM_ERR;

    if (facility.isEmpty()) {
        logE(mTag, "The facility string is empty.");
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    // Get app type id
    appTypeId = queryAppTypeId(aid);

    if (appTypeId == -1) {
        ril_error = RIL_E_INVALID_ARGUMENTS;
        goto error;
    }

    do {
        if (pwd.isEmpty()) {
            logE(mTag, "The password can't be empty");
            ril_error = RIL_E_PASSWORD_INCORRECT;
            goto error;
        }
        cmd.append(String8::format("AT+ECLCK=%d,\"%s\",%s,\"%s\"", appTypeId, facility.string(),
                lockStr.string(), pwd.string()));
        p_response = atSendCommand(cmd);
        err = p_response->getError();
        cmd.clear();

        if (err < 0) {
            logE(mTag, "set facility lock Fail");
            goto error;
        } else if (p_response->getSuccess() == 0) {
            switch (p_response->atGetCmeError()) {
                case CME_SIM_BUSY:
                    logD(mTag, "simFacilityLock: CME_SIM_BUSY");
                    sleepMsec(200);
                    count++;
                    p_response = NULL;
                    ril_error = RIL_E_SIM_BUSY;
                    break;
                case CME_SIM_PIN_REQUIRED:
                case CME_SIM_PUK_REQUIRED:
                    ril_error = RIL_E_PASSWORD_INCORRECT;
                    goto error;
                case CME_SIM_PIN2_REQUIRED:
                    ril_error = RIL_E_SIM_PIN2;
                    goto error;
                case CME_SIM_PUK2_REQUIRED:
                    ril_error = RIL_E_SIM_PUK2;
                    goto error;
                case CME_INCORRECT_PASSWORD:
                    ril_error = RIL_E_PASSWORD_INCORRECT;
                    goto error;
                case CME_PHB_FDN_BLOCKED:
                    ril_error = RIL_E_FDN_CHECK_FAILURE;
                    goto error;
                default:
                    logD(mTag, "simFacilityLock: default");
                    goto error;
            }
        } else {
            // Success
            logD(mTag, "Set facility lock successfully");

            ril_error = RIL_E_SUCCESS;
            /* SIM operation we shall return pin retry counts */
            retry = getPinPukRetryCount();
            RFX_ASSERT(retry != NULL);

            attemptsRemaining[0] = retry->pin1;
            attemptsRemaining[1] = retry->pin2;
            attemptsRemaining[2] = retry->puk1;
            attemptsRemaining[3] = retry->puk2;
            if (facility == facFd) {
                attemptsRemaining[0] = retry->pin2;
            }
            free(retry);
            response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                    RfxIntsData((void*)attemptsRemaining, sizeof(int)), msg, false);
            responseToTelCore(response);

            sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
                    m_slot_id, RfxVoidData());
            responseToTelCore(unsol);
        }

        if(count == 13) {
            logE(mTag, "Set Facility Lock: CME_SIM_BUSY and time out.");
            goto error;
        }
    } while (ril_error == RIL_E_SIM_BUSY);

    return;
error:
    retry = getPinPukRetryCount();

    if (retry != NULL) {
        /* SIM operation we shall return pin retry counts */
        attemptsRemaining[0] = retry->pin1;
        attemptsRemaining[1] = retry->pin2;
        attemptsRemaining[2] = retry->puk1;
        attemptsRemaining[3] = retry->puk2;
        if (facility == facFd) {
            attemptsRemaining[0] = retry->pin2;
        }
        free(retry);
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData((void*)attemptsRemaining, sizeof(int)), msg, false);
    } else {
        response = RfxMclMessage::obtainResponse(msg->getId(), ril_error,
                RfxIntsData(), msg, false);
    }
    responseToTelCore(response);

    sp<RfxMclMessage> unsol = RfxMclMessage::obtainUrc(RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED,
            m_slot_id, RfxVoidData());
    responseToTelCore(unsol);
}

void RmcGsmSimRequestHandler::sleepMsec(long long msec) {
    struct timespec ts;
    int err;

    ts.tv_sec = (msec / 1000);
    ts.tv_nsec = (msec % 1000) * 1000 * 1000;

    do {
        err = nanosleep(&ts, &ts);
    } while (err < 0 && errno == EINTR);
}

