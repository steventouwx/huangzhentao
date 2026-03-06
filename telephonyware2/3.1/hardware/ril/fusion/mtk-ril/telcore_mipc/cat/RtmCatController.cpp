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
#include "RtmCatController.h"
#include "RfxMainThread.h"
#include <mtk_properties.h>

#include <errno.h>

#define RFX_LOG_TAG "RtmCatController"

/*****************************************************************************
 * Class RtmCatController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RtmCatController", RtmCatController, RfxController);

// register request and urc to RfxData

RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxStringData, \
    RFX_MSG_REQUEST_STK_SEND_ENVELOPE_COMMAND);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, \
    RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
    RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
    RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, \
    RFX_MSG_REQUEST_REPORT_STK_SERVICE_IS_RUNNING);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxSimIoRspData, \
    RFX_MSG_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
    RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
    RFX_MSG_REQUEST_STK_EVENT_NOTIFY);
// BIP @{
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, \
    RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO);
// BIP @}
// BTSAP @{
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxRawData, RfxRawData, \
    RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS);
// BTSAP @}
// End


RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_STK_PROACTIVE_COMMAND);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_STK_EVENT_NOTIFY);
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_STK_SESSION_END);
RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_STK_CALL_SETUP);
RFX_REGISTER_DATA_TO_URC_ID(RfxSimRefreshData, RFX_MSG_URC_SIM_REFRESH);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_URC_STK_CC_ALPHA_NOTIFY);  //maybe modify
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_STK_SETUP_MENU_RESET);  //maybe modify
RFX_REGISTER_DATA_TO_URC_ID(RfxStringsData, RFX_MSG_URC_STK_CALL_CTRL);  //maybe modify
RFX_REGISTER_DATA_TO_URC_ID(RfxVoidData, RFX_MSG_URC_STK_TRIGGER_OTASP);




RtmCatController::RtmCatController() :
    mCardType(-1) {
        isStkServiceRunning = false;
        isProaCmdQueued = false;
        isEventNotifyQueued = false;
        pProactive_cmd = NULL;
        pCachedMenu = NULL;
        mProCmdRec.cmdType = 0;
        mProCmdRec.cmdQualifier = 0;
        mProCmdRec.needResponse = false;
        mProCmdRec.cmdNum = 0;
        for (int i = 0; i< EVENT_NOTIFY_BUFFER_LEN; i++) {
            pEvent_notify[i] = NULL;
        }
}

RtmCatController::~RtmCatController() {
    freeAllStkQueuedEventNotify();
    freeStkQueuedProactivCmd();
    freeCachedMenu();
}

void RtmCatController::onInit() {
    RfxController::onInit();  // Required: invoke super class implementation

    isProaCmdQueued = false;
    isStkServiceRunning = false;
    resetProCmd();

    const int request_id_list[] = {
        RFX_MSG_REQUEST_STK_SEND_ENVELOPE_COMMAND,
        RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE,
        RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM,
        RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE,
        RFX_MSG_REQUEST_REPORT_STK_SERVICE_IS_RUNNING,
        RFX_MSG_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS,
        RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE,
        RFX_MSG_REQUEST_STK_EVENT_NOTIFY,
        // BIP @{
        RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO,
        // BIP @}
        // BTSAP @{
        RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS,
        // BTSAP @}
    };

    const int urc_id_list[] = {
        RFX_MSG_URC_STK_SESSION_END,
        RFX_MSG_URC_STK_PROACTIVE_COMMAND,
        RFX_MSG_URC_STK_EVENT_NOTIFY,
        RFX_MSG_URC_STK_CALL_SETUP,
        RFX_MSG_URC_SIM_REFRESH,
        RFX_MSG_URC_STK_CC_ALPHA_NOTIFY,
        RFX_MSG_URC_STK_SETUP_MENU_RESET,
        RFX_MSG_URC_STK_CALL_CTRL,
        RFX_MSG_URC_STK_TRIGGER_OTASP,
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));

    // Register callbacks for card type
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
        RfxStatusChangeCallback(this, &RtmCatController::onCardTypeChanged));

    // register callbacks to get required information
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
        RfxStatusChangeCallback(this, &RtmCatController::onRadioStateChanged));
    // register callbacks to get required information
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_AOSP_UICC_APPLICATIONS_ENABLE_STATE,
        RfxStatusChangeCallback(this, &RtmCatController::onSimOnOffStateChanged));
    // register callbacks to get required information
    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_SIM_ONOFF_STATE,
        RfxStatusChangeCallback(this, &RtmCatController::onSimOnOffStateChanged));
}

void RtmCatController::onCardTypeChanged(RfxStatusKeyEnum key,
    RfxVariant oldValue, RfxVariant newValue) {
    RFX_UNUSED(key);
    //bool isStkServiceRunning = false;
    //int cmdType = 0;
    int oldState = -1, newState = -1;

    oldState = oldValue.asInt();
    newState = newValue.asInt();

    mCardType = getStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE);
    if ((oldState > 0) && (newState <= 0)) {
        logD(RFX_LOG_TAG, "onCardTypeChanged  cmdType 0x%02X, cmdQual %d",
                mProCmdRec.cmdType, mProCmdRec.cmdQualifier);
        if (mProCmdRec.needResponse && mProCmdRec.cmdType != 0) {
            sendResponseForCardMissing(mProCmdRec.cmdType, mProCmdRec.cmdQualifier);
            resetProCmd();
        }

        logD(RFX_LOG_TAG, "card missing and reset the RFX_STATUS_KEY_IS_CAT_RUNNING");
        isStkServiceRunning = false;
        freeAllStkQueuedEventNotify();
        freeStkQueuedProactivCmd();
        freeCachedMenu();
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_CAT_RUNNING, false);
    }
}

void RtmCatController::onRadioStateChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant value) {
    int oldState = -1, newState = -1;

    RFX_UNUSED(key);
    oldState = old_value.asInt();
    newState = value.asInt();

    if (newState == RADIO_STATE_UNAVAILABLE) {
        //For radio unavailable, CatService will be created when sim ready
        if (mProCmdRec.needResponse && mProCmdRec.cmdType != 0) {
            sendResponseForCardMissing(mProCmdRec.cmdType, mProCmdRec.cmdQualifier);
            resetProCmd();
        }

        isStkServiceRunning = false;
        freeAllStkQueuedEventNotify();
        freeStkQueuedProactivCmd();
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_CAT_RUNNING, false);
    }
}
void RtmCatController::onSimOnOffStateChanged(RfxStatusKeyEnum key,
    RfxVariant old_value, RfxVariant value) {
    int oldState = -1, newState = -1;

    RFX_UNUSED(key);
    oldState = old_value.asInt();
    newState = value.asInt();
    logD(RFX_LOG_TAG, "onSimOnOffStateChanged  newState %d",newState);

    if (newState == UICC_APPLICATIONS_ENABLE_STATE_DISABLED
            || newState == SIM_POWER_STATE_SIM_OFF) {
        //For radio unavailable, CatService will be created when sim ready
        if (mProCmdRec.needResponse && mProCmdRec.cmdType != 0) {
            sendResponseForCardMissing(mProCmdRec.cmdType, mProCmdRec.cmdQualifier);
            resetProCmd();
        }

        isStkServiceRunning = false;
        freeAllStkQueuedEventNotify();
        freeStkQueuedProactivCmd();
        getStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_CAT_RUNNING, false);
        logD(RFX_LOG_TAG, "onSimOnOffStateChanged set isStkServiceRunning as false");
    }
}

void RtmCatController::onDeinit() {
    logD(RFX_LOG_TAG, "RtmCatController: onDeinit ");

    // Unregister callbacks for card type
    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_CARD_TYPE,
            RfxStatusChangeCallback(this, &RtmCatController::onCardTypeChanged));

    getStatusManager()->unRegisterStatusChanged(RFX_STATUS_KEY_RADIO_STATE,
            RfxStatusChangeCallback(this, &RtmCatController::onRadioStateChanged));
    // Required: invoke super class implementation
    RfxController::onDeinit();
}

bool RtmCatController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    int cmdType = 0;
    int resCode = 0;
    bool isIgnoredCmd = false;

    logD(RFX_LOG_TAG, "onHandleRequest: msg_id=%d is %s", msg_id, RFX_ID_TO_STR(msg_id));

    switch (msg_id) {
        case RFX_MSG_REQUEST_STK_SEND_ENVELOPE_COMMAND:
            //Filter and ignore the command which has handled by modem
            isIgnoredCmd = isIgnoredEnvelopeCommand(message);
            if (isIgnoredCmd) {
                logD(RFX_LOG_TAG, "onHandleRequest: ignore the cmd which is handled by modem");
                sp<RfxMessage> rsp =
                        RfxMessage::obtainResponse(RIL_E_SUCCESS, message, false);
                responseToRilj(rsp);
                break;
            }
            requestToMcl(message);
            break;
        case RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE:
            parseStkCmdType(message, &cmdType);

            logD(RFX_LOG_TAG, "mProCmdRec: cmdType:0x%02X, current response type:0x%02X ",
                mProCmdRec.cmdType, cmdType);

            if (0 != mProCmdRec.cmdType && mProCmdRec.cmdType == cmdType && mProCmdRec.needResponse) {
                requestToMcl(message);
                resetProCmd();
            //Setup Event list may cover current cmd type and cause response block
            } else if (CMD_SETUP_EVENT_LIST == cmdType) {
                //requestToMcl(message);
                //resetProCmd();
                sp<RfxMessage> rsp =
                            RfxMessage::obtainResponse(RIL_E_SUCCESS, message, false);
                responseToRilj(rsp);
            // BIP @{
            } else if (CMD_OPEN_CHAN == cmdType) {
                parseStkResultCode(message, &resCode);
                if (resCode >= 0) {
                    int bip_Data[2];
                    bip_Data[0] = mProCmdRec.cmdNum;
                    bip_Data[1] = resCode;
                    sp<RfxMessage> msg = RfxMessage::obtainRequest(m_slot_id,
                            RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO, RfxIntsData(bip_Data, 2));
                    requestToMcl(msg);

                    sp<RfxMessage> rsp =
                            RfxMessage::obtainResponse(RIL_E_SUCCESS, message, false);
                    responseToRilj(rsp);
                } else {
                    logE(RFX_LOG_TAG, "parse bip result code fail");
                    sp<RfxMessage> rsp =
                            RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, false);
                    responseToRilj(rsp);
                }
                resetProCmd();
            // BIP @}
            } else {
                logD(RFX_LOG_TAG, "Not Expected TR. Return directly!!!");
                sp<RfxMessage> rsp =
                        RfxMessage::obtainResponse(RIL_E_SUCCESS, message, false);
                responseToRilj(rsp);
            }
            break;
        case RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM:
        case RFX_MSG_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM_WITH_RESULT_CODE:
            if (CMD_SETUP_CALL == mProCmdRec.cmdType) {
                //note: For Gen93, it is no used to config audio path
                if (false == mProCmdRec.needResponse) {
                    logD(RFX_LOG_TAG, "Not need to send call request, just return!");
                    sp<RfxMessage> rsp =
                            RfxMessage::obtainResponse(RIL_E_SUCCESS, message, false);
                    responseToRilj(rsp);
                } else {
                    requestToMcl(message);
                }
            // BIP @{
            } else if (CMD_OPEN_CHAN == mProCmdRec.cmdType) {
                RfxIntsData *intsData = (RfxIntsData*)message->getData();
                int *data = NULL;
                if (intsData != NULL) {
                    data = (int*)intsData->getData();
                    logD(RFX_LOG_TAG, "parse bip result code: %d ", data[0]);
                    int bip_Data[2];
                    bip_Data[0] = mProCmdRec.cmdNum;
                    bip_Data[1] = data[0];
                    sp<RfxMessage> msg = RfxMessage::obtainRequest(m_slot_id,
                            RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO, RfxIntsData(bip_Data, 2));
                    requestToMcl(msg);

                    sp<RfxMessage> rsp =
                            RfxMessage::obtainResponse(RIL_E_SUCCESS, message, false);
                    responseToRilj(rsp);
                } else {
                    logE(RFX_LOG_TAG, "parse bip result code fail");
                    sp<RfxMessage> rsp =
                            RfxMessage::obtainResponse(RIL_E_GENERIC_FAILURE, message, false);
                    responseToRilj(rsp);
                }
            // BIP @}
            } else {
                logD(RFX_LOG_TAG, "Unexpected CALL_SETUP_REQUESTED. Warning!!!");
                requestToMcl(message);
            }
            // reset mProCmdRec
            resetProCmd();
            break;
        case RFX_MSG_REQUEST_REPORT_STK_SERVICE_IS_RUNNING:
            requestToMcl(message);
            mCardType = getStatusManager()->getIntValue(RFX_STATUS_KEY_CARD_TYPE, -1);
            getStatusManager()->setBoolValue(RFX_STATUS_KEY_IS_CAT_RUNNING, true);
            requestHandleStkServiceIsRunning();
            break;
        // BTSAP :{
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            //Currently we send the envelope cmd directly
            requestToMcl(message);
            break;
        // BTSAP :}
        default:
            logE(RFX_LOG_TAG, "Unsupport cmd. Waring!!!");
            requestToMcl(message);
            break;
    }
    return true;
}

bool RtmCatController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();

    logD(RFX_LOG_TAG,"onHandleUrc: urc id %d is %s", msg_id, RFX_ID_TO_STR(msg_id));
    switch (msg_id) {
        case RFX_MSG_URC_STK_PROACTIVE_COMMAND:
            handleProactiveCommand(message);
            break;
        case RFX_MSG_URC_STK_EVENT_NOTIFY:
            handleEventNotify(message);
            break;
        default:
            responseToRilj(message);
            break;
    }
    return true;
}

bool RtmCatController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_REQUEST_REPORT_STK_SERVICE_IS_RUNNING:
            responseToRilj(message);
            break;
        // BIP @{
        case RFX_MSG_REQUEST_BIP_SEND_CONFIRM_INFO:
            logD(RFX_LOG_TAG, "onHandleResponse: for bip response, no need to handle");
            break;
        // BIP @}
        case RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE:
            logD(RFX_LOG_TAG, "onHandleResponse: for stk abnormal response, no need to handle");
            break;
        case RFX_MSG_REQUEST_STK_EVENT_NOTIFY:
            logD(RFX_LOG_TAG, "onHandleResponse: for stk event notify response, no need to handle");
            break;
        case RFX_MSG_REQUEST_SIM_SAP_TRANSFER_CARD_READER_STATUS:
            // Send BT directly
            logD(RFX_LOG_TAG, "onHandleResponse: return response to BT");
            responseToBT(message);
            break;
        default:
            responseToRilj(message);
            break;
    }
    return true;
}

int RtmCatController::checkEventNotifyFreeBuffer() {
    int i = 0;
    int freeBufferNum = 0;

    for (i = 0 ; i < EVENT_NOTIFY_BUFFER_LEN ; i++) {
        if (NULL == pEvent_notify[i]) {
            freeBufferNum++;
        }
    }

    logD(RFX_LOG_TAG, "checkEventNotifyFreeBuffer: freeBufferNum:%d", freeBufferNum);
    return freeBufferNum;
}

int RtmCatController::checkStkCmdDisplay(char *cmd_str)
{
    int is_alpha_id_existed = 0;
    int index = 0;
    int cmd_length = 0;
    char temp_str[3] = {0};
    char *end;

    cmd_length = strlen(cmd_str);

    while (cmd_length > index) {
        if (cmd_str[index + 1] == '5' && (cmd_str[index] == '0' || cmd_str[index] == '8') ) {

            index += 2;
            if (cmd_str[index] != '0' || cmd_str[index + 1] != '0' ) {
                is_alpha_id_existed = 1;
            }
            if (cmd_str[index] <= '7') {
                memcpy(temp_str, &(cmd_str[index]), 2);
                index += (strtoul(temp_str, &end, 16) + 1) * 2;
            } else {
                memcpy(temp_str, &(cmd_str[index + 2]), 2);
                index += (strtoul(temp_str, &end, 16) + 2) * 2;
            }
        } else if((cmd_str[index + 1] == 'E' || cmd_str[index + 1] == 'e') &&
                                    (cmd_str[index] == '1' || cmd_str[index] == '9')) {
            int icon_qualifier = 0;

            index += 4;
            memset(temp_str, 0, 3);
            memcpy(temp_str, &(cmd_str[index + 1]), 1);
            icon_qualifier = strtoul(temp_str, &end, 16);

            if((icon_qualifier & 0x01) == 0x01) {
                if(is_alpha_id_existed == 0) {
                    return 1;
                }
            }
            index += 4;
        } else {
            index += 2;
            if (cmd_str[index] <= '7') {
                memcpy(temp_str, &(cmd_str[index]), 2);
                index += (strtoul(temp_str, &end, 16) + 1) * 2;
            } else {
                memcpy(temp_str, &(cmd_str[index + 2]), 2);
                index += (strtoul(temp_str, &end, 16) + 2) * 2;
            }
        }
    }
    return 0;
}

unsigned int RtmCatController::findStkCallDuration(char* str)
{
    int length = 0, offset = 0, temp = 0;
    unsigned int duration = 0;
    char temp_str[3] = {0};
    char *end;

    length = strlen(str);

    while(length > 0) {

        if(str[offset] == '8' && str[offset + 1] == '4') {
            memcpy(temp_str, &(str[offset + 6]), 2);
            if(str[offset + 5] == '0') {
                temp = strtoul(temp_str, &end, 16);
                if (temp == ULONG_MAX && errno == ERANGE) {
                    logE(RFX_LOG_TAG, "findStkCallDuration: strtoul fail !!!");
                    break;
                }
                duration = temp * 60000;
            } else if(str[offset + 5] == '1') {
                temp = strtoul(temp_str, &end, 16);
                if (temp == ULONG_MAX && errno == ERANGE) {
                    logE(RFX_LOG_TAG, "findStkCallDuration: strtoul fail !!!");
                    break;
                }
                duration = temp * 1000;
            } else if(str[offset + 5] == '2') {
                temp = strtoul(temp_str, &end, 16);
                if (temp == ULONG_MAX && errno == ERANGE) {
                    logE(RFX_LOG_TAG, "findStkCallDuration: strtoul fail !!!");
                    break;
                }
                duration = temp * 100;
            }
            break;
        } else {
            length -= 2;
            offset += 2;
            memcpy(temp_str, &(str[offset]), 2);
            temp = strtoul(temp_str, &end, 16);
            if (temp == ULONG_MAX && errno == ERANGE) {
                logE(RFX_LOG_TAG, "findStkCallDuration: strtoul fail !!!");
                break;
            }
            length -= (2 * temp + 2);
            offset += (2 * temp + 2);

        }
    }
    return duration;
}

void RtmCatController::freeStkQueuedEventNotify(int index) {
    if (index < 0 || index >= EVENT_NOTIFY_BUFFER_LEN) {
        logE(RFX_LOG_TAG, "freeStkQueuedEventNotify: Invalid index:%d !!!", index);
        return;
    }

    if (NULL != pEvent_notify[index]) {
        free(pEvent_notify[index]);
        pEvent_notify[index] = NULL;
    }
}

void RtmCatController::freeAllStkQueuedEventNotify() {
    for (int i = 0; i < EVENT_NOTIFY_BUFFER_LEN; i++) {
        if (NULL != pEvent_notify[i]) {
            free(pEvent_notify[i]);
            pEvent_notify[i] = NULL;
        }
    }
}

void RtmCatController::freeStkQueuedProactivCmd() {
    if (NULL != pProactive_cmd) {
        free(pProactive_cmd);
        pProactive_cmd = NULL;
    }
}

void RtmCatController::freeCachedMenu() {
    if (NULL != pCachedMenu) {
        free(pCachedMenu);
        pCachedMenu = NULL;
    }
}

int RtmCatController::getStkCommandType(char *cmd) {
    char temp[3] = {0};
    int cmdType = 0;

    strncpy(temp, cmd, 2);
    cmdType = strtoul(temp, NULL, 16);
    cmdType = 0xFF & cmdType;

    return cmdType;
}

char* RtmCatController::getStkQueuedEventNotify(int index) {
    if (index < 0 || index >= EVENT_NOTIFY_BUFFER_LEN) {
        logE(RFX_LOG_TAG, "getStkQueuedEventNotify: Invalid index:%d !!!", index);
        return NULL;
    }

    return pEvent_notify[index];
}

void RtmCatController::handleStkCommand(char* cmd, int cmdClass) {
    int type_pos = 0, cmdType = -1, cmdNum = 0;
    int cmdQual = 0;
    int cmd_not_understood = 0;
    int duration[1] = {0};

    if (cmd == NULL) {
        logE(RFX_LOG_TAG, "handleStkCommand error: cmd is null!");
        return;
    }
    parseStkCmdType(cmd, &cmdType);
    parseStkCmdQualifier(cmd, &cmdQual);
    mProCmdRec.cmdType = cmdType;
    mProCmdRec.cmdQualifier = cmdQual;

    if (CMD_TYPE_PROACTIVE == cmdClass) {
        switch(cmdType) {
            case CMD_SETUP_MENU:
            case CMD_LAUNCH_BROWSER:
            case CMD_PLAY_TONE:
            case CMD_DSPL_TXT:
            case CMD_GET_INKEY:
            case CMD_GET_INPUT:
            case CMD_SELECT_ITEM:
            case CMD_PROVIDE_LOCAL_INFO:
            case CMD_IDLE_MODEL_TXT:
            case CMD_LANGUAGE_NOTIFY:
                mProCmdRec.needResponse = true;
                break;
            // BIP @{
            case CMD_OPEN_CHAN:
                mProCmdRec.needResponse = false;
                parseStkCmdNum(cmd, &cmdNum);
                mProCmdRec.cmdNum = cmdNum;
                logD(RFX_LOG_TAG,"handleStkCommand: cmd num is %d", mProCmdRec.cmdNum);
                break;
            // BIP @}
            default:
                mProCmdRec.needResponse = false;
                break;
        }
        if ((CMD_CLOSE_CHAN == cmdType) || (CMD_RECEIVE_DATA == cmdType)
                                                    || (CMD_SEND_DATA == cmdType)) {
            logD(RFX_LOG_TAG,"onHandleUrc: cmd has no need to send to stk fwk currently!");
            return;
        }
        sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_STK_PROACTIVE_COMMAND,
                RfxStringData(cmd));
        responseToRilj(urc);
    } else if (CMD_TYPE_NOTIFY == cmdClass) {
        mProCmdRec.needResponse = false;
        sp<RfxMessage> msg = NULL;
        if(cmd[2] <= '7' ) {
            type_pos = 10;
        } else if(cmd[2] > '7' ) {
            type_pos = 12;
        }
        /*temp_str[type_pos -6] points to cmd_detail tag*/
        cmd_not_understood = checkStkCmdDisplay(&(cmd[type_pos - 6]));
        logD(RFX_LOG_TAG, "cmd_not_understood Cmd: %d", cmd_not_understood);
        //handle setup call
        if(CMD_SETUP_CALL == cmdType && cmd_not_understood == 0) {
            logD(RFX_LOG_TAG, "handleEventNotify : CMD_SETUP_CALL");
            /*temp_str[type_pos -6] points to cmd_detail tag*/
            duration[0] = (int)findStkCallDuration(&cmd[type_pos - 6]);
            logD(RFX_LOG_TAG, "duration: %d", duration[0]);
            mProCmdRec.cmdType = cmdType;
            mProCmdRec.needResponse = true;
        }
        // BIP @{
        //Need to compatible with the AP bip solution
        if (CMD_OPEN_CHAN == cmdType) {
            sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_STK_PROACTIVE_COMMAND,
                    RfxStringData(cmd));
            responseToRilj(urc);
            return;
        }
        // BIP @}
        sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_STK_EVENT_NOTIFY,
                RfxStringData(cmd));
        responseToRilj(urc);

        if(CMD_SETUP_CALL == cmdType) {
            sp<RfxMessage> setup_call_urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_STK_CALL_SETUP,
                RfxIntsData(duration, 1));
            responseToRilj(setup_call_urc);
        }
    }
}

void RtmCatController::parseStkCmdQualifier(char* cmd, int* cmdQual) {
    int cmd_len = strlen(cmd);
    int typePos = 0;

    if (cmd_len < 14) {
        logD(RFX_LOG_TAG, "parseStkCmdQualifier exception!");
        return;
    }

    if(cmd[2] <= '7' ) {
        typePos = 12;
    } else {
        typePos = 14;
    }

    // get command qualifier
    *cmdQual = getStkCommandType(&cmd[typePos]);
}

void RtmCatController::parseStkCmdType(char* cmd, int* cmdType) {
    int cmd_len = strlen(cmd);
    int typePos = 0;

    if (cmd_len < 14) {
        logD(RFX_LOG_TAG, "parseStkCmdType exception!");
        return;
    }

    if(cmd[2] <= '7' ) {
        typePos = 10;
    } else {
        typePos = 12;
    }

    // get command type
    *cmdType = getStkCommandType(&cmd[typePos]);
}

void RtmCatController::parseStkCmdType(const sp<RfxMessage>& message, int* cmdType) {
    int msgId = message->getId();
    int typePos = 0;
    RfxBaseData* stkCmd = NULL;
    char* cmd = NULL;

    // Get command from message
    stkCmd = message->getData();
    if (stkCmd != NULL) {
        cmd = (char*)stkCmd->getData();
    } else {
        logD(RFX_LOG_TAG, "parseStkCmdType cmd fail!");
        return;
    }

    if (cmd) {

        // decide typePos value
        switch (msgId) {
            case RFX_MSG_URC_STK_PROACTIVE_COMMAND:
            case RFX_MSG_URC_STK_EVENT_NOTIFY:
                if (cmd[2] <= '7') {
                    typePos = 10;
                } else {
                    typePos = 12;
                }
                break;

            case RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE:
                typePos = 6;
                break;

            default:
                //logD(RFX_LOG_TAG,"Not Support msgId:%d", msgId);
                break;
        }

        // check command type
        *cmdType = getStkCommandType(&cmd[typePos]);
        //free(cmd);
    } else {
        logE(RFX_LOG_TAG, "cmd is NULL. Error!!!");
    }
}

int RtmCatController::getStkCommandNumber(char *cmd) {
    char temp[3] = {0};
    int cmdNum = 0;

    strncpy(temp, cmd, 2);
    cmdNum = strtoul(temp, NULL, 16);
    cmdNum = 0xFF & cmdNum;

    return cmdNum;
}

void RtmCatController::parseStkCmdNum(char* cmd, int* cmdNum) {
    int cmd_len = strlen(cmd);
    int typePos = 0;

    if (cmd_len < 12) {
        logD(RFX_LOG_TAG, "parseStkCmdNum exception!");
        return;
    }

    if (cmd[2] <= '7') {
        typePos = 8;
    } else {
        typePos = 10;
    }

    // check command num
    *cmdNum = getStkCommandNumber(&cmd[typePos]);
    logD(RFX_LOG_TAG,"parseStkCmdNum cmdNum:%d", *cmdNum);
}

int RtmCatController::getStkResultCode(char *cmd) {
    char temp[3] = {0};
    int resCode = 0;

    strncpy(temp, cmd, 2);
    resCode = strtoul(temp, NULL, 16);
    resCode = 0xFF & resCode;

    return resCode;
}

void RtmCatController::parseStkResultCode(const sp<RfxMessage>& message, int* resCode) {
    int msgId = message->getId();
    int typePos = 0;
    int cmd_len = 0;
    RfxBaseData* stkCmd = NULL;
    char* cmd = NULL;
    *resCode = -1;

    // Get command from message
    stkCmd = message->getData();
    if (stkCmd != NULL) {
        cmd = (char*)stkCmd->getData();
    } else {
        logD(RFX_LOG_TAG, "parseStkResultCode cmd:%s fail", cmd);
        return;
    }
    if (cmd != NULL) {
        cmd_len = strlen(cmd);
        logD(RFX_LOG_TAG, "parseStkResultCode cmd:%s ", cmd);
        // decide typePos value
        switch (msgId) {
            case RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE:
                //For the TR from CatService, result code is at the 12th Bytes
                typePos = 22;
                break;
            default:
                break;
        }
        if (cmd_len < typePos) {
            logE(RFX_LOG_TAG, "parseStkResultCode exception!");
            return;
        }
        // check command type
        *resCode = getStkResultCode(&cmd[typePos]);
        logD(RFX_LOG_TAG,"parseStkResultCode cmdNum:%d", *resCode);
    } else {
        logE(RFX_LOG_TAG, "cmd is NULL. Error!!!");
    }
}

void RtmCatController::handleEventNotify(const sp<RfxMessage>& message) {
    int urc_len = 0;
    char* cmd = NULL;
    char* pEventNotify = NULL;
    RfxBaseData* stkCmd = NULL;

    // Get command from message
    stkCmd = message->getData();
    if (stkCmd != NULL) {
        cmd = (char*)stkCmd->getData();
    } else {
        logE(RFX_LOG_TAG, "handleEventNotify fail!");
        return;
    }

    if (cmd != NULL && strstr(cmd, "BIP")) {
        logD(RFX_LOG_TAG, "report BIP: START urc");
        sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_STK_EVENT_NOTIFY,
                RfxStringData(cmd));
        responseToRilj(urc);
        return;
    }

    if (false == isStkServiceRunning) {
        if (cmd != NULL) {
            urc_len = strlen(cmd);
        } else {
            logE(RFX_LOG_TAG,"handleEventNotify: urc_len error!");
            return;
        }
        if (checkEventNotifyFreeBuffer() > 0) {
            pEventNotify = (char*)calloc(1, urc_len + 1);
            if (NULL == pEventNotify) {
                logE(RFX_LOG_TAG,"handleEventNotify: pEventNotify is NULL !!!");
                return;
            }
            memset(pEventNotify, 0x0, urc_len + 1);
            memcpy(pEventNotify, cmd, urc_len);
            setStkCachedEventNotify(pEventNotify);
            isEventNotifyQueued = true;
            logD(RFX_LOG_TAG, "STK service is not running yet.[%p]", pEventNotify);
        }
        return;
    }

    handleStkCommand(cmd, CMD_TYPE_NOTIFY);
}

void RtmCatController::handleProactiveCommand(const sp<RfxMessage>& message) {
    int urc_len = 0;
    int cmdType = 0;
    int cmdQual = 0;
    char* cmd = NULL;
    char* pProCmd = NULL;
    char* cachedMenu = NULL;
    RfxBaseData* stkCmd = NULL;

    // Get command from message
    stkCmd = message->getData();
    if (stkCmd != NULL) {
        cmd = (char*)stkCmd->getData();
    } else {
        logE(RFX_LOG_TAG, "handleProactiveCommand fail!");
        return;
    }

    if (cmd != NULL) {
        urc_len = strlen(cmd);
    } else {
        logE(RFX_LOG_TAG, "handleProactiveCommand: urc_len error!");
        return;
    }

    parseStkCmdType(cmd, &cmdType);
    mProCmdRec.cmdType = cmdType;
    parseStkCmdQualifier(cmd, &cmdQual);
    mProCmdRec.cmdQualifier= cmdQual;
    logD(RFX_LOG_TAG, " mProCmdRec.cmdType : 0x%02X, mProCmdRec.cmdQualifier : %d",
            mProCmdRec.cmdType, mProCmdRec.cmdQualifier);

    switch(cmdType) {
        case CMD_SETUP_MENU:
            cachedMenu = (char*)calloc(1, urc_len + 1);
            if (NULL == cachedMenu) {
                logE(RFX_LOG_TAG, "handleProactiveCommand: cachedMenu is NULL !!!");
                return;
            }
            memset(cachedMenu, 0x0, urc_len + 1);
            memcpy(cachedMenu, cmd, urc_len);
            logD(RFX_LOG_TAG, "cachedMenu:[%s] ", cachedMenu);
            freeCachedMenu();
            pCachedMenu = cachedMenu;
            mProCmdRec.needResponse = true;
            break;
        case CMD_LAUNCH_BROWSER:
        case CMD_PLAY_TONE:
        case CMD_DSPL_TXT:
        case CMD_GET_INKEY:
        case CMD_GET_INPUT:
        case CMD_SELECT_ITEM:
        case CMD_PROVIDE_LOCAL_INFO:
        case CMD_IDLE_MODEL_TXT:
        case CMD_LANGUAGE_NOTIFY:
            mProCmdRec.needResponse = true;
            break;
        default:
            mProCmdRec.needResponse = false;
            break;
    }

    if (false == isStkServiceRunning) {
        pProCmd = (char*)calloc(1, urc_len + 1);
        if (NULL == pProCmd) {
            logE(RFX_LOG_TAG, "handleProactiveCommand: pProCmd is NULL !!!");
            return;
        }

        memset(pProCmd, 0x0, urc_len + 1);
        memcpy(pProCmd, cmd, urc_len);
        freeStkQueuedProactivCmd();
        pProactive_cmd = pProCmd;
        isProaCmdQueued = true;
        logD(RFX_LOG_TAG, "STK service is not running yet. cmd:[%p]", pProactive_cmd);
        return;
    }

    handleStkCommand(cmd, CMD_TYPE_PROACTIVE);
}

void RtmCatController::requestHandleStkServiceIsRunning() {
    isStkServiceRunning = true;
    sendStkQueuedCmd(0);
}

void RtmCatController::sendStkQueuedCmd(int flag) {

    if (true == isEventNotifyQueued) {
        logD(RFX_LOG_TAG, "sendStkQueuedCmd: isEventNotifyQueued: true");
        int index = 0;
        while (index < EVENT_NOTIFY_BUFFER_LEN) {
            char *cmd = getStkQueuedEventNotify(index);
            if (NULL != cmd) {
                logD(RFX_LOG_TAG, "send queued event to framework.");
                handleStkCommand(cmd, CMD_TYPE_NOTIFY);
                freeStkQueuedEventNotify(index);
            }
            index++ ;
        }
        isEventNotifyQueued = false;
    }

    if (true == isProaCmdQueued) {
        logD(RFX_LOG_TAG, "sendStkQueuedCmd: isProaCmdQueued: true");
        char *cmd = pProactive_cmd;
        if (NULL != cmd) {
            logD(RFX_LOG_TAG, "Proactive Cmd:[%s]", cmd);
            handleStkCommand(cmd, CMD_TYPE_PROACTIVE);
            freeStkQueuedProactivCmd();
            isProaCmdQueued = false;
        }
    } else {
       // For STK service running, need report set up menu to framework
        if (flag == 0) {
            if (mProCmdRec.needResponse && mProCmdRec.cmdType != 0) {
                sendResponseForSpecial(mProCmdRec.cmdType, mProCmdRec.cmdQualifier);
            }
            char *cachedMenu = pCachedMenu;
            logD(RFX_LOG_TAG, "sendStkQueuedCmd, will report menu, cachedMenu :[%s]" , cachedMenu);
            if (NULL != cachedMenu) {
                sp<RfxMessage> urc = RfxMessage::obtainUrc(getSlotId(), RFX_MSG_URC_STK_PROACTIVE_COMMAND,
                        RfxStringData(cachedMenu));
                responseToRilj(urc);
            }
        }
    }
}

void RtmCatController::setStkCachedEventNotify(char* cmd) {
    int i = 0;
    for (i = 0 ; i < EVENT_NOTIFY_BUFFER_LEN ; i++) {
        if (NULL == pEvent_notify[i]) {
            pEvent_notify[i] = cmd;
            break;
        }
    }
    if (EVENT_NOTIFY_BUFFER_LEN == i) {
        logE(RFX_LOG_TAG, "setStkCachedEventNotify: No free buffer !!!");
    } else {
        logD(RFX_LOG_TAG, "setStkCachedEventNotify: [index:%d][%p]", i, pEvent_notify[i]);
    }
}

void RtmCatController::sendResponseForCardMissing(int cmdType, int cmdQual) {
    int cmdInfo[2];
    cmdInfo[0] = cmdType;
    cmdInfo[1] = cmdQual;

    sp<RfxMessage> msg = RfxMessage::obtainRequest(m_slot_id,
            RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE, RfxIntsData(cmdInfo, 2));
    requestToMcl(msg);
}

// For the special case, such as phone process is killed, need send TR to modem
void RtmCatController::sendResponseForSpecial(int cmdType, int cmdQual) {
    int cmdInfo[2];
    cmdInfo[0] = cmdType;
    cmdInfo[1] = cmdQual;

    sp<RfxMessage> msg = RfxMessage::obtainRequest(m_slot_id,
            RFX_MSG_REQUEST_STK_SEND_RESPONSE_BY_CMDTYPE, RfxIntsData(cmdInfo, 2));
    requestToMcl(msg);
}
void RtmCatController::resetProCmd() {
    mProCmdRec.cmdType = 0;
    mProCmdRec.cmdQualifier = 0;
    mProCmdRec.needResponse = false;
}

bool RtmCatController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState) {
    RFX_UNUSED(isModemPowerOff);

    int msg_id = message->getId();

    if (RADIO_STATE_UNAVAILABLE == radioState) {
        if (msg_id == RFX_MSG_REQUEST_STK_SEND_TERMINAL_RESPONSE) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void RtmCatController::parseStkEnvelopeCommandType(char* cmd, int* cmdType) {
    int typePos = 0;

    // As TS 102 223, the lenth of event download message is more than 9 Bytes
    if (cmd == NULL || strlen(cmd) < 18) {
        logE(RFX_LOG_TAG, "Envelop cmd length error!");
        return;
    }

    // Currently only check BER-TLV tag and D6 means this is an event download message
    if ((cmd[0] == 'D' || cmd[0] == 'd') && cmd[1] == '6') {
        if (cmd[2] <= '7') {
            // As TS 102 223, the length field will take 1 byte if the first value is less than 8
            // so the command type will start from the 5th byte.
            typePos = 8;
        } else {
            typePos = 10;
        }
        *cmdType = getStkCommandType(&cmd[typePos]);
        logD(RFX_LOG_TAG, "parseEnvelopeType event type:%d", *cmdType);
    } else {
        // For other envelope command, just return
        logE(RFX_LOG_TAG, "Not Event download cmd and return!");
    }
}

bool RtmCatController::isIgnoredEnvelopeCommand(const sp<RfxMessage>& message) {
    bool bIgnored = false;
    char* cmd = NULL;
    int eventType = -1;
    RfxBaseData* stkCmd = NULL;

    // Get command from message
    stkCmd = message->getData();
    if (stkCmd != NULL) {
        cmd = (char*)stkCmd->getData();
    } else {
        logD(RFX_LOG_TAG, "parse stk envelope command :%s fail", cmd);
        return false;
    }

    parseStkEnvelopeCommandType(cmd, &eventType);
    switch(eventType) {
        case EVENT_MT_CALL:
        case EVENT_CALL_CONNECTED:
        case EVENT_CALL_DISCONNECTED:
        case EVENT_LOCATION_STATUS:
        case EVENT_ACCESS_TECHNOLOGY_CHANGE:
        case EVENT_NETWORK_SEARCH_MODE_CHANGE:
        case EVENT_NETWORK_REJECTION:
            bIgnored = true;
            break;
        default:
            break;
    }
    logD(RFX_LOG_TAG, "Event type:%d need be ignored: %d", eventType, bIgnored);
    return bIgnored;
}
