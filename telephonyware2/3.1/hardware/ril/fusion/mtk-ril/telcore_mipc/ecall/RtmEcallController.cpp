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
#include <string>

#include "RtmEcallController.h"
#include "RfxRootController.h"
#include "RfxIntsData.h"
#include "RfxRilUtils.h"
#include "RfxStringData.h"
#include "RfxStringsData.h"
#include "RfxTimer.h"
#include "RfxVoidData.h"
#include "rfx_properties.h"
#include "RfxEcallIndicationData.h"
#include "RfxEcallData.h"
#include "RfxEcallMsdData.h"
#include "RfxEcallSetNumData.h"

#include <compiler/compiler_utils.h>
#include <mtkconfigutils.h>
#include <libmtkrilutils.h>

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define RFX_LOG_TAG "RtmEcall"


/*****************************************************************************
 * Class RtmEcallController
 *****************************************************************************/
RFX_IMPLEMENT_CLASS("RtmEcallController", RtmEcallController, RfxController);

// register request to RfxData
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_IVS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallSetNumData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_TEST_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallSetNumData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallMsdData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_MSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_PSAP);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_MAKE_ECALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_MSD_MODE);
//RFX_REGISTER_DATA_TO_REQUEST_ID(RfxEcallData, RfxVoidData, RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxStringData, RfxVoidData, RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxVoidData, RFX_MSG_REQUEST_ECALL_RESET_IVS);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_PRI);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxIntsData, RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_ECALL_GET_SIM_INFO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData, RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxVoidData, RfxIntsData, RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE);

RFX_REGISTER_DATA_TO_URC_ID(RfxIntsData, RFX_MSG_URC_ECALL_MSDHACK);
RFX_REGISTER_DATA_TO_URC_ID(RfxEcallIndicationData, RFX_MSG_URC_ECALL_INDICATIONS);


RtmEcallController::RtmEcallController() {
}

RtmEcallController::~RtmEcallController() {
}

void RtmEcallController::onInit() {
    // Required: invoke super class implementation
    RfxController::onInit();
    logD(RFX_LOG_TAG, "onInit !!");

    const int request_id_list[] = {
        RFX_MSG_REQUEST_ECALL_SET_IVS,
        RFX_MSG_REQUEST_ECALL_SET_TEST_NUM,
        RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM,
        RFX_MSG_REQUEST_ECALL_SET_MSD,
        RFX_MSG_REQUEST_ECALL_SET_PSAP,
        RFX_MSG_REQUEST_ECALL_MAKE_ECALL,
        RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD,
        RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD,
        RFX_MSG_REQUEST_ECALL_SET_MSD_MODE,
        //RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL,
        RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE,
        RFX_MSG_REQUEST_ECALL_RESET_IVS,
        RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM,
        RFX_MSG_REQUEST_ECALL_SET_PRI,
        RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME,
        RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE,
        RFX_MSG_REQUEST_ECALL_GET_SIM_INFO,
        RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE,
        RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE,
    };

    const int urc_id_list[] = {
        RFX_MSG_URC_ECALL_MSDHACK,
        RFX_MSG_URC_ECALL_INDICATIONS,
    };

    // register request & URC id list
    // NOTE. one id can only be registered by one controller
    registerToHandleRequest(request_id_list, sizeof(request_id_list)/sizeof(const int));
    registerToHandleUrc(urc_id_list, sizeof(urc_id_list)/sizeof(const int));
}

bool RtmEcallController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_REQUEST_ECALL_SET_IVS :
        case RFX_MSG_REQUEST_ECALL_SET_TEST_NUM :
        case RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM :
        case RFX_MSG_REQUEST_ECALL_SET_MSD :
        case RFX_MSG_REQUEST_ECALL_SET_PSAP :
        case RFX_MSG_REQUEST_ECALL_MAKE_ECALL :
        case RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD :
        case RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD :
        case RFX_MSG_REQUEST_ECALL_SET_MSD_MODE :
        //case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL :
        case RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE :
        case RFX_MSG_REQUEST_ECALL_RESET_IVS :
        case RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM :
        case RFX_MSG_REQUEST_ECALL_SET_PRI :
        case RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME :
        case RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE :
        case RFX_MSG_REQUEST_ECALL_GET_SIM_INFO :
        case RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE :
        case RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE : {
            // Send RMM directly
            requestToMcl(message);
            }
            break;
        default:
            logD(RFX_LOG_TAG, "Not Support the req %s", idToString(msg_id));
            break;
    }
    return true;
}

bool RtmEcallController::onHandleUrc(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_URC_ECALL_MSDHACK :
        case RFX_MSG_URC_ECALL_INDICATIONS : {
            // Send RILJ directly
            responseToRilj(message);
            }
            break;
        default:
            return false;
    }
    return true;
}

bool RtmEcallController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    switch (msg_id) {
        case RFX_MSG_REQUEST_ECALL_SET_IVS :
        case RFX_MSG_REQUEST_ECALL_SET_TEST_NUM :
        case RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM :
        case RFX_MSG_REQUEST_ECALL_SET_MSD :
        case RFX_MSG_REQUEST_ECALL_SET_PSAP :
        case RFX_MSG_REQUEST_ECALL_MAKE_ECALL :
        case RFX_MSG_REQUEST_ECALL_IVS_PUSH_MSD :
        case RFX_MSG_REQUEST_ECALL_PSAP_PULL_MSD :
        case RFX_MSG_REQUEST_ECALL_SET_MSD_MODE :
        //case RFX_MSG_REQUEST_ECALL_FAST_MAKE_ECALL :
        case RFX_MSG_REQUEST_ECALL_CTRL_SEQUENCE :
        case RFX_MSG_REQUEST_ECALL_RESET_IVS :
        case RFX_MSG_REQUEST_ECALL_QUERY_ECALL_NUM :
        case RFX_MSG_REQUEST_ECALL_SET_PRI :
        case RFX_MSG_REQUEST_ECALL_SET_NAD_DEREGISTRATION_TIME :
        case RFX_MSG_REQUEST_ECALL_SET_REGISTRATION_STATE :
        case RFX_MSG_REQUEST_ECALL_GET_SIM_INFO :
        case RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE :
        case RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE : {
            // Send RILJ directly
            responseToRilj(message);
            }
            break;
        default:
            logD(RFX_LOG_TAG, "Not Support the req %s", idToString(msg_id));
            break;
    }
    return true;
}

bool RtmEcallController::onCheckIfRejectMessage(
        const sp<RfxMessage>& message, bool isModemPowerOff, int radioState) {
    int msgId = message->getId();
    if (radioState == (int)RADIO_STATE_UNAVAILABLE ||
            radioState == (int)RADIO_STATE_OFF ||
            isModemPowerOff == true) {
        if (msgId == RFX_MSG_REQUEST_ECALL_SET_TEST_NUM ||
                msgId == RFX_MSG_REQUEST_ECALL_SET_RECONF_NUM ||
                msgId == RFX_MSG_REQUEST_ECALL_SET_PRI ||
                msgId == RFX_MSG_REQUEST_ECALL_SET_OPRT_MODE ||
                msgId == RFX_MSG_REQUEST_ECALL_GET_OPRT_MODE) {
            return false;
        }
    }
    return RfxController::onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

