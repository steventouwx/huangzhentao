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
#include <telephony/mtk_ril.h>
#include <telephony/mtk_ril_ivt.h>

// MTK fusion include
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxIntsData.h"


// Local include
#include "RmcCallControlInterface.h"
#include "RmcCallControlEcallRequestHandler.h"

#define RFX_LOG_TAG "RmcCCReqHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcCallControlEcallRequestHandler, RIL_CMD_PROXY_1);

RmcCallControlEcallRequestHandler::RmcCallControlEcallRequestHandler(int slot_id,
        int channel_id) : RmcCallControlBaseHandler(slot_id, channel_id) {
    const int requests[] = {
        RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,                           //AT%EMSDTNUM=
        RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM,                         //AT%EMSDTNUM=
        RFX_MSG_REQUEST_ECALL_MAKE_ECALL,                             //AT+CECALL=
        RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,                        //AT+CECALL=
        RFX_MSG_REQUEST_ECALL_SET_MSD,                                //AT%EMSDUPDATE=
        RFX_MSG_REQUEST_ECALL_SET_MSD_MODE,                           //AT%EMSDMOD=
        RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM,                        //AT+ECALLQUERY=
        RFX_MSG_REQUEST_ECALL_SET_PRI,                                //AT%EMSDPRI=
        RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME,            //AT+ESETTIMER=
        RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE,                 //AT+EECREGS=
        RFX_MSG_REQUEST_ECALL_GET_SIM_INFO,                           //AT+EECSIMINFO?
    };

    registerToHandleRequest(requests, sizeof(requests)/sizeof(int));
}

RmcCallControlEcallRequestHandler::~RmcCallControlEcallRequestHandler() {
}

void RmcCallControlEcallRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "onHandleRequest: %d", msg->getId());
    int request = msg->getId();
    switch(request) {
        case RFX_MSG_REQUEST_ECALL_SET_TEST_NUM:
            requestSetTestNum(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM:
            requestSetReconfNum(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_MAKE_ECALL:
            requestMakeEcall(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL:
            requestMakeFastEcall(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_MSD:
            requestUpdateMsd(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_MSD_MODE:
            requestSetMsdMode(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM:
            requestQueryEcallNum(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_PRI:
            requestSetPri(msg);
        case RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME:
            requestSetNadDeregistrationTime(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE:
            requestSetRegistrationState(msg);
            break;
        case RFX_MSG_REQUEST_ECALL_GET_SIM_INFO:
            requestGetSimInfo(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmcCallControlEcallRequestHandler::onHandleTimer() {
    // do something
}

void RmcCallControlEcallRequestHandler::requestSetTestNum(const sp<RfxMclMessage>& msg) {
    RIL_ECallSetNum *num = ((RIL_ECallSetNum *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;

    int argNum = num->arg_num;

    logD(RFX_LOG_TAG, "requestSetTestNum type: %d address:%s argNum=%d", num->type, num->address, num->arg_num);

    if (argNum == 0) {
        p_response = atSendCommand(String8::format("AT%%EMSDTNUM"));
    } else if (argNum == 1) {
        p_response = atSendCommand(String8::format("AT%%EMSDTNUM=%d", num->type));
    } else if (argNum == 2) {
        p_response = atSendCommand(String8::format("AT%%EMSDTNUM=%d,%s", num->type, num->address));
    }

    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestSetReconfNum(const sp<RfxMclMessage>& msg) {
    RIL_ECallSetNum *num = ((RIL_ECallSetNum *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;

    int argNum = num->arg_num;
    if (argNum == 0) {
        p_response = atSendCommand(String8::format("AT%%EMSDRNUM"));
    } else if (argNum == 1) {
        p_response = atSendCommand(String8::format("AT%%EMSDRNUM=%d", num->type));
    } else if (argNum == 2) {
        p_response = atSendCommand(String8::format("AT%%EMSDRNUM=%d,%s", num->type, num->address));
    }

    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestMakeEcall(const sp<RfxMclMessage>& msg) {
    int *type = ((int *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    sp<RfxAtResponse> setModeResponse;
    bool ret = false;

    setModeResponse = atSendCommand(String8::format("%s=%d,%d", "AT+EVADSMOD", 1, 0));
    if (setModeResponse->getError() != 0 || setModeResponse->getSuccess() != 1) {
        logE(RFX_LOG_TAG, "Setup CS only mode failed");
        ret = false;
    }

    if (type == NULL) {
        p_response = atSendCommand(String8::format("AT+CECALL="));
    } else {
        p_response = atSendCommand(String8::format("AT+CECALL=%d,%d,%d,\"\"", type[0], 0, 0));
    }
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        ret = false;
    } else {
        ret = true;
    }

    if (ret) {
        sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
                RfxVoidData(), msg);
        responseToTelCore(response);
    } else {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
    }
}

void RmcCallControlEcallRequestHandler::requestMakeFastEcall(const sp<RfxMclMessage>& msg) {
    RIL_ECallReqMsg *fastEcall = ((RIL_ECallReqMsg *)msg->getData()->getData());
    int ecall_type;
    sp<RfxAtResponse> p_response;

    if (fastEcall == NULL) {
        p_response = atSendCommand(String8::format("AT+CECALL="));
    } else {
        int ecall_variant = fastEcall->ecall_variant;
        int ecall_cat = fastEcall->ecall_cat;
        int ecall_format = 1;
        int msdLength = fastEcall->length;
        switch(ecall_variant) {
            case 1:
                ecall_type = 0;
                break;
            case 2:
                if(ecall_cat == 1) {
                    ecall_type = 2;
                    break;
                } else if (ecall_cat == 2) {
                    ecall_type = 3;
                    break;
                } else {
                    break;
                }
            case 3:
                ecall_type = 1;
                break;
            default:
                break;
        }

        char imsEcallSupport[RFX_PROPERTY_VALUE_MAX] = { 0 };
        rfx_property_get("persist.vendor.ims_ecall_support", imsEcallSupport, "0");
        logD(RFX_LOG_TAG, "requestMakeFastEcall imsEcallSupport: %s", imsEcallSupport);
        if ((atoi(imsEcallSupport) == 1)) {
            setVDSAuto(true, false);
        } else if ((atoi(imsEcallSupport) == 0)) {
            sp<RfxAtResponse> atResponse;
            atResponse = atSendCommand(String8::format("%s=%d,%d", "AT+EVADSMOD", 1, 0));
            if (atResponse->getError() != 0 || atResponse->getSuccess() != 1) {
                logE(RFX_LOG_TAG, "Setup VDS failed");
            }
        }

        String8 cmd = String8::format("AT+CECALL=%d,%d,%d,\"", ecall_type, ecall_format, msdLength);
        for (int i = 0; i < msdLength; i++) {
            cmd += String8::format("%02X", fastEcall->msd_data[i]);
        }
        cmd += "\"";

        p_response = atSendCommand(cmd);
    }

    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestUpdateMsd(const sp < RfxMclMessage > & msg) {
    RIL_ECallSetMSD *newMsd = ((RIL_ECallSetMSD *)msg->getData()->getData());
    int msdLength = newMsd->length;
    sp<RfxAtResponse> p_response;

    //AT+EMSDUPDATE=<ecall domain>,<length>,<msd_data>
    //<ecall domain> integer: 1, IMS domain for ng ecall (always 1 now, reserved for future use)
    //<length> integer: even integer range from 2 to 280, length of <msd_data>
    //<msd_data> String: Hexstring, maximum length is 280 bytes

    if (msdLength <= 0) {
        responseVoidDataToTcl(msg, RIL_E_INVALID_ARGUMENTS);
        return;
    }
    String8 cmd = String8::format("AT+EMSDUPDATE=%d,%d,", 1, 2 * msdLength);

    for (int i = 0; i < msdLength; i++) {
            String8 msdHex = String8::format("%02X", newMsd->msd_data[i]);
            cmd += msdHex;
    }
    p_response = atSendCommand(cmd);

    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestSetMsdMode(const sp<RfxMclMessage>& msg) {
    int *msdMode = ((int *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    if (msdMode == NULL) {
        p_response = atSendCommand(String8::format("AT%%EMSDMOD="));
    } else {
        p_response = atSendCommand(String8::format("AT%%EMSDMOD=%d", msdMode[0]));
    }
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestQueryEcallNum(const sp<RfxMclMessage>& msg) {
    char *number = (char *)msg->getData()->getData();
    sp<RfxAtResponse> p_response;
    RfxAtLine *intermediates;
    int ret;

    if (number == NULL || strlen(number) == 0) {
        p_response = atSendCommandSingleline(String8::format("AT+ECALLQUERY="), "+ECALLQUERY:");
    } else {
        p_response = atSendCommandSingleline(String8::format("AT+ECALLQUERY=%s", number), "+ECALLQUERY:");
    }
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    intermediates = p_response->getIntermediates();
    if (intermediates == NULL) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    intermediates->atTokStart(&ret);
    if (ret < 0) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    char *line = intermediates->getCurrentLine();
    logD(RFX_LOG_TAG, "requestQueryEcallNum response line: %s", line);

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxStringData(line), msg);
    responseToTelCore(response);
}


void RmcCallControlEcallRequestHandler::requestSetPri(const sp<RfxMclMessage>& msg) {
    int *pri = ((int *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    if (pri == NULL) {
        p_response = atSendCommand(String8::format("AT%%EMSDPRI="));
    } else {
        p_response = atSendCommand(String8::format("AT%%EMSDPRI=%d,%d,%d,%d", pri[0], pri[1], pri[2], pri[3]));
    }
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestSetNadDeregistrationTime(
        const sp<RfxMclMessage>& msg) {
    int *nadDeregTime = ((int *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    /* AT command description:
       AT+ESETTIMER= <purpose>, <mode>, <timer1>, <timer2>
       <purpose> Integer
           0 - For eCALL (currently only this is used, can be extended in future)
       <mode> Integer
           1 - Set timer
           0 - Reset Timer
       <timer1>, <timer2> Integer
           Timer values passed to EMM and MM.
           Note: In case of <mode> == 0, <timer1>, <timer2> are not needed
    */

    if (nadDeregTime != NULL) {
        p_response = atSendCommand(String8::format("AT+ESETTIMER=%d,%d,%d,%d",
            nadDeregTime[0], nadDeregTime[1], nadDeregTime[2], nadDeregTime[3]));
    }
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        responseVoidDataToTcl(msg, RIL_E_INTERNAL_ERR);
        return;
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
        RfxVoidData(), msg);
    responseToTelCore(response);
}

void RmcCallControlEcallRequestHandler::requestSetRegistrationState(
        const sp<RfxMclMessage>& msg) {
    int *state = ((int *)msg->getData()->getData());
    sp<RfxAtResponse> p_response;
    int simType = getSimInfo();
    int returnValue = -1;
    RIL_Errno rilErrorNo = RIL_E_SUCCESS;

    if ((simType == -1) || (state == NULL)) {
        rilErrorNo = RIL_E_GENERIC_FAILURE;
    } else {
        if (simType == 1) { /* SIM_ECALL_ONLY = 1 */
            /* AT command description:
               AT+EECREGS=<state>	- ECall REGistration State
               <state>:
               0: deregister from NW, enter the eCall inactivity procedure
               1: register to NW, leave the eCall inactivity procedure
               (not support, reserved for future use)
            */
            p_response = atSendCommand(String8::format("AT+EECREGS=%d", state[0]));
            if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
                rilErrorNo = RIL_E_GENERIC_FAILURE;
            } else {
                returnValue = 0;
            }
        } else {
            p_response = atSendCommand(String8::format("AT+COPS=2"));
            if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
                rilErrorNo = RIL_E_GENERIC_FAILURE;
            } else {
                returnValue = 1;
            }
        }
    }

    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), rilErrorNo,
        RfxIntsData(&returnValue, 1), msg);
    responseToTelCore(response);
}


/*
    Return Ecall SIM type.
*/
void RmcCallControlEcallRequestHandler::requestGetSimInfo(const sp<RfxMclMessage>& msg) {
    int simType = getSimInfo();
    sp<RfxMclMessage> response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS,
            RfxIntsData(&simType, 1), msg, false);
    responseToTelCore(response);
}


/*
   Get SIM Info for ECall.

   +EECSIMINFO : <eCall_SIM_TYPE>,< USIM test ecall URI>,
                 <USIM test ecall number >,< USIM reconfiguration ecall URI>,
                 <USIM reconfiguration ecall number>
*/
int RmcCallControlEcallRequestHandler::getSimInfo() {
    sp<RfxAtResponse> p_response;
    int ret;
    int simType = 0;
    RfxAtLine *pLine = NULL;

    p_response = atSendCommandMultiline(String8::format("AT+EECSIMINFO?"), "+EECSIMINFO:");
    if (p_response->getError() != 0 || p_response->getSuccess() != 1) {
        return -1;
    }

    pLine = p_response->getIntermediates();
    if (pLine == NULL) {
        return -1;
    }

    pLine->atTokStart(&ret);
    if (ret < 0) {
        return -1;
    }

    simType = pLine->atTokNextint(&ret);
    if (ret < 0) {
        return -1;
    }
    return simType;
}


