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

#include "RmmRadioRequestHandler.h"
#include "RmmCapabilitySwitchRequestHandler.h"
#include "RmmCapabilitySwitchUtil.h"
#include "Rfx.h"
#include "RfxChannelManager.h"
#include "RfxIntsData.h"
#include "RfxVoidData.h"
#include "RfxStringData.h"
#include "RfxRilUtils.h"
#include <libmtkrilutils.h>
#include <RfxRilDumpInfo.h>
#include "ratconfig.h"
#include <mtkconfigutils.h>
#include "RfxDispatchThread.h"
#include "mipc_msg.h"
#include "RfxMipcData.h"

#if !defined(MTK_RIL_THIN_MD_SUPPORT)
#include "hardware/ccci_intf.h"
#endif

#define RFX_LOG_TAG "RmmRadioReq"
#define PROPERTY_AIRPLANE_MODE "persist.vendor.radio.airplane.mode.on"
#define PROPERTY_SIM_MODE "persist.vendor.radio.sim.mode"
#define MAX_RETRY_COUNT 20
#define PROPERTY_DECRYPT "vold.decrypt"

#define RADIO_POWER_CAUSE_AIRPLANE_MODE (4)
#define RADIO_POWER_CAUSE_BOOTUP        (0x7E)

typedef enum {
    RADIO_MODE_SIM1_ONLY = 1,
    RADIO_MODE_SIM2_ONLY = (RADIO_MODE_SIM1_ONLY << 1),
    RADIO_MODE_SIM3_ONLY = (RADIO_MODE_SIM1_ONLY << 2),
    RADIO_MODE_SIM4_ONLY = (RADIO_MODE_SIM1_ONLY << 3),
} RadioMode;

enum mtk_fsm_state {
    FSM_STATE_INVALID = 0,
    FSM_STATE_OFF,
    FSM_STATE_ON,
    FSM_STATE_POSTDUMP,
    FSM_STATE_DOWNLOAD,
    FSM_STATE_BOOTUP,
    FSM_STATE_READY,
    FSM_STATE_MDEE
};

// modem status reader
static int sMdStatusFd = -1;

/*
  * struct md_status_event {
  *     struct timeval time_stamp;
  *     int md_id;
  *     int event_type;
  *     char reason[32];
  * };
  * define at ccci_inft.h
  */
typedef struct md_status_event MdStatusEvent;

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmmRadioRequestHandler, RIL_CMD_PROXY_9);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_RADIO_POWER);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO);
RFX_REGISTER_DATA_TO_REQUEST_ID(RfxIntsData, RfxVoidData,
        RFX_MSG_REQUEST_ENABLE_MODEM);
RFX_REGISTER_DATA_TO_URC_ID(RfxStringData, RFX_MSG_UNSOL_MODEM_RESTART);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxVoidData, RFX_MSG_EVENT_CHANNEL_INIT_DONE);

int RmmRadioRequestHandler::sMdStatusEventType = -1;

RmmRadioRequestHandler::RmmRadioRequestHandler(int slotId, int channelId) :
        RfxBaseHandler (slotId, channelId) {
    logD(RFX_LOG_TAG, "RmmRadioRequestHandler constructor");
    const int request[] = {
        RFX_MSG_REQUEST_RADIO_POWER,
        RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO,
        RFX_MSG_REQUEST_ENABLE_MODEM,
    };

    const int event[] = {
        RFX_MSG_EVENT_CHANNEL_INIT_DONE,
    };

    registerToHandleEvent(event, sizeof(event)/sizeof(int));

    registerToHandleRequest(request, sizeof(request)/sizeof(int));

    int mainSlotId = RfxRilUtils::getMajorSim() - 1;
    if (mainSlotId == slotId) {
        rfx_property_set("vendor.ril.ipo.radiooff", "0");
        // LibDumpRildInfo::createNewDumpMessage("radioPower", 5);
        mipc_md_event_register(mdStatusEventCb, nullptr);
    }
}

RmmRadioRequestHandler::~RmmRadioRequestHandler() {
}

void RmmRadioRequestHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleRequest: %s(%d)", idToString(id), id);
    switch(id) {
        case RFX_MSG_REQUEST_RADIO_POWER:
        case RFX_MSG_REQUEST_ENABLE_MODEM:
            requestRadioPower(msg);
            break;
        case RFX_MSG_REQUEST_BOOT_TURN_ON_RADIO:
            bootupSetRadioPower(msg);
            break;
        default:
            logE(RFX_LOG_TAG, "Should not be here");
            break;
    }
}

void RmmRadioRequestHandler::onHandleTimer() {
    // do something
}

void RmmRadioRequestHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    int slotId = msg->getSlotId();
    logD(RFX_LOG_TAG, "onHandleEvent: %d, slotid: %d", id, slotId);
    switch(id) {
        case RFX_MSG_EVENT_CHANNEL_INIT_DONE:
            {
                char filghtMode[RFX_PROPERTY_VALUE_MAX] = { 0 };
                int cause = -1;
                logD(RFX_LOG_TAG, "all channels init done");
                rfx_property_set("vendor.ril.muxreport.run", "0");
                rfx_property_get(PROPERTY_AIRPLANE_MODE, filghtMode, "false");
                if (strcmp("false", filghtMode)) {
                    logD(RFX_LOG_TAG, "under airplane mode");
                    cause = RADIO_POWER_CAUSE_AIRPLANE_MODE;
                }
                sendMipcRequest(slotId, 0, cause);

                // update radio off state to rilj
                for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
                    getMclStatusManager(i)->setIntValue(
                            RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
                }
            }
            break;
        default:
            logE(RFX_LOG_TAG, "should not be here");
            break;
    }
}

mipc_result_const_enum RmmRadioRequestHandler::sendMipcRequest(int slot, int onOff, int caller) {
    sp<RfxMipcData> mipcRet = NULL;
    mipc_result_const_enum ret = mipc_result_const_NONE;
    char filghtMode[RFX_PROPERTY_VALUE_MAX] = { 0 };
    bool isFlightMode = false;

    rfx_property_get(PROPERTY_AIRPLANE_MODE, filghtMode, "false");
    if (strcmp("false", filghtMode)) {
        isFlightMode = true;
    }
    logD(RFX_LOG_TAG, "sendMipcRequest: slot%d = %d, caller = %d, isFlightMode = %d",
            slot, onOff, caller, isFlightMode);

#if !defined(__MTK_CCCI_SUPPORT__)
    if (onOff == 1 && isFlightMode == false) {
        logI(RFX_LOG_TAG, "set vote property as 1");
        rfx_property_set("vendor.mtk.wwan.vote.airplane", "1");
    }
#endif

    sp<RfxMipcData> mipcMsg = RfxMipcData::obtainMipcData(MIPC_NW_SET_RADIO_STATE_REQ, slot);
    mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_SW_STATE, onOff);
    if ((onOff == 0) && (caller >= 0)) {
        if (caller != RFOFF_CAUSE_POWER_OFF || isFlightMode == true) {
            mipcMsg->addMipcTlvUint8(MIPC_NW_SET_RADIO_STATE_REQ_T_CAUSE, caller);
        }
    }
    uint32_t timeout = 10 * 60 * 1000;
    mipcRet = callToMipcMsgSync(mipcMsg, timeout);
    ret = (mipc_result_const_enum) mipcRet->getResult();

    logD(RFX_LOG_TAG, "sendMipcRequest: ret=%d", ret);
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        return ret;
    }

    while (ret != MIPC_RESULT_SUCCESS) {
        usleep(50*1000);
        mipcRet = callToMipcMsgSync(mipcMsg, timeout);
        ret = (mipc_result_const_enum) mipcRet->getResult();
        logD(RFX_LOG_TAG, "sendMipcRequest: ret=%d", ret);
    }

#if !defined(__MTK_CCCI_SUPPORT__)
    if (onOff == 0 && isFlightMode == true) {
        logI(RFX_LOG_TAG, "set vote property as 0");
        rfx_property_set("vendor.mtk.wwan.vote.airplane", "0");
    }
#endif

    return ret;
}

void RmmRadioRequestHandler::requestRadioPower(const sp<RfxMclMessage> &msg) {
    sp<RfxAtResponse> response;
    RIL_RadioState newState;
    RIL_Errno errNo = RIL_E_SUCCESS;
    sp<RfxMclMessage> resMsg;

    int onOff = ((int *)msg->getData()->getData())[0];
    int forEcc = ((int *)msg->getData()->getData())[1];
    int preEcc = ((int *)msg->getData()->getData())[2];
    int caller =  msg->getData()->getDataLength()/sizeof(int) > 3
            ? ((int *)msg->getData()->getData())[3] : -1;
    int slotId = msg->getSlotId();

    int originMode = 0;
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        if (i != slotId) {
            originMode |=getMclStatusManager(i)->getIntValue(
                    RFX_STATUS_KEY_RADIO_POWER_MSIM_MODE, 0) << i;
        } else {
            originMode |= onOff << i;
        }
    }
    logD(RFX_LOG_TAG, "original mode=%d", originMode);

    int mainSlotId = RfxRilUtils::getMajorSim() - 1;
    int targetMode = 0;
    int slotMode = 0;
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        slotMode = (originMode & (1 << i)) >> i;
        switch(mainSlotId) {
            case RFX_SLOT_ID_0:
                slotMode = slotMode << i;
                break;
            case RFX_SLOT_ID_1:
                if (RFX_SLOT_ID_0 == i) {
                    slotMode = slotMode << 1;
                } else if (RFX_SLOT_ID_2 == i) {
                    slotMode = slotMode << 2;
                } else if (RFX_SLOT_ID_3 == i) {
                    slotMode = slotMode << 3;
                }
                break;
            case RFX_SLOT_ID_2:
                if (RFX_SLOT_ID_0 == i) {
                    slotMode = slotMode << 1;
                } else if (RFX_SLOT_ID_1 == i) {
                    slotMode = slotMode << 2;
                } else if (RFX_SLOT_ID_3 == i) {
                    slotMode = slotMode << 3;
                }
                break;
            case RFX_SLOT_ID_3:
                if (RFX_SLOT_ID_0 == i) {
                    slotMode = slotMode << 1;
                } else if (RFX_SLOT_ID_1 == i) {
                    slotMode = slotMode << 2;
                } else if (RFX_SLOT_ID_2 == i) {
                    slotMode = slotMode << 3;
                }
                break;
            default:
                break;
        }
        targetMode |= slotMode;
        slotMode = 0;
    }

    logI(RFX_LOG_TAG, "requestRadioPower, power = %d, mode = %d, forEcc %d, preEcc %d, caller: %d",
            onOff, targetMode, forEcc, preEcc, caller);

    // TODO: need new ECC MIPC interface ? move to RTM
    /// M: CC: APM ecc indication handling
    handleEccIndication(slotId, onOff, forEcc, preEcc);

    /*
    * RFOFF_CAUSE_UNSPECIFIED = 0,
    * RFOFF_CAUSE_DUPLEX_MODE = 1,
    * RFOFF_CAUSE_POWER_OFF = 2,
    * RFOFF_CAUSE_SIM_SWITCH = 3,
    */
    /*
    if (targetMode == 0) {
        if (caller >= 0) {
            response = atSendCommand(String8::format("AT+EFUN=0,%d", caller));
        } else {
            response = atSendCommand("AT+EFUN=0");
        }
        newState = RADIO_STATE_OFF;
    } else if (targetMode > 0) {
        if (!onOff) {
            response = atSendCommand(String8().format(("AT+EFUN=%d,%d"), targetMode, caller));
        } else {
            response = atSendCommand(String8().format(("AT+EFUN=%d"), targetMode));
        }
        newState = (onOff==1)? RADIO_STATE_ON: RADIO_STATE_OFF;
    } else {
        newState = curState;
        notSent = true;
    }
    */
    newState = (onOff == 1)? RADIO_STATE_ON: RADIO_STATE_OFF;
    sendMipcRequest(slotId, onOff, caller);

    // TODO: Move to RTM, response of SET RADIO POWER request
    /// M: CC: Ecc selected phone handling
    updateEccPhone(slotId, preEcc);

    /*
    // error handle
    if (!notSent && response->getSuccess() == 0) {
        cause = response->atGetCmeError();
        newState = curState;
        logD(RFX_LOG_TAG, "Get error cause: %d",cause);

        // always do EFUN retry when get response error
        errNo = RIL_E_OEM_ERROR_1;
        if (CME_MODEM_NOT_ALLOW == cause) {
            // This error indicates that EFUN conflicts with ECC flow
            errNo = RIL_E_OPERATION_NOT_ALLOWED;
        }
    }
    */

    // only update radio state when EFUN sended successfully
    if (errNo == RIL_E_SUCCESS) {
        getMclStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_POWER_MSIM_MODE, onOff);
        getMclStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, newState);
    }
    resMsg = RfxMclMessage::obtainResponse(msg->getId(), errNo, RfxVoidData(), msg,
            false);
    responseToTelCore(resMsg);
    return;
}

///// M: CC: Ecc power on emergency indication related function @{
void RmmRadioRequestHandler::handleEccIndication(int slotId, int onOff, int forEcc, int preEcc) {
    RFX_UNUSED(preEcc);

    if (slotId < 0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "handleEccIndication invalid slotId (%d), skip.", slotId);
        return;
    }

    /// M: CC: indication to modem if enter emergency call mode (EMCI)
    bool currEccPowerOnStaus = getEccPowerOnStatus();
    if ((!currEccPowerOnStaus && onOff == 1 && forEcc == 1)
            || (currEccPowerOnStaus && forEcc == 0)) {
        sp<RfxMipcData> data = RfxMipcData::obtainMipcData(
                MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ, getSlotId());
        data->addMipcTlvUint8(MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ_T_IS_FLIGHT_MODE, forEcc);
        logI(RFX_LOG_TAG, "MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ %d", forEcc);
        sp<RfxMipcData> response = callToMipcMsgSync(data);
        setEccPowerOnStatus(forEcc == 1);
        if (response->getResult() != MIPC_RESULT_SUCCESS) {
            logE(RFX_LOG_TAG, "ecc indication failed (slot: %d, cause: %d)",
                    slotId, response->getResult());
        }
    }

}

/** M: CC: Ecc power on status update function */
void RmmRadioRequestHandler::setEccPowerOnStatus(bool isOnForEcc) {
    getNonSlotMclStatusManager()->setBoolValue(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, isOnForEcc);
}

bool RmmRadioRequestHandler::getEccPowerOnStatus() {
    return getNonSlotMclStatusManager()->getBoolValue(
            RFX_STATUS_KEY_RADIO_POWER_ON_FOR_ECC, false);
}
/// @}

/** M: CC: Ecc selected phone update function */
void RmmRadioRequestHandler::updateEccPhone(int slotId, int preEcc) {
    if (slotId < 0 || slotId >= RfxRilUtils::rfxGetSimCount()) {
        logE(RFX_LOG_TAG, "updateEccPhone invalid slotId (%d)", slotId);
    }
    int currEccSelectedSlot = getNonSlotMclStatusManager()->getIntValue(
            RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, -1);
    if (preEcc == 1) {
        if (currEccSelectedSlot == slotId) {
            logD(RFX_LOG_TAG, "[%d] duplicate phone selection request (%d)",
                    slotId, currEccSelectedSlot);
        } else if (currEccSelectedSlot != -1) {
            /// M: CC: to protect case if need to turn on radio for ECC retry.
            logI(RFX_LOG_TAG, "[%d] ECC phone selection already set (%d), skip.",
                    slotId, currEccSelectedSlot);
        } else {
            getNonSlotMclStatusManager()->setIntValue(
                    RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, slotId);
            logI(RFX_LOG_TAG, "[%d] update ECC phone selection (%d -> %d)",
                    m_slot_id, currEccSelectedSlot, slotId);
        }
    } else if (preEcc == 0 && currEccSelectedSlot == slotId) {
        getNonSlotMclStatusManager()->setIntValue(
                RFX_STATUS_KEY_BSP_EMERGENCY_SELECTED_SLOT, -1);
        logI(RFX_LOG_TAG, "[%d] reset ECC phone selection (%d -> %d)",
                m_slot_id, currEccSelectedSlot, -1);
    }
}

void RmmRadioRequestHandler::bootupSetRadioPower(const sp<RfxMclMessage> &msg) {
    logD(RFX_LOG_TAG, "bootupSetRadioPower");
    RIL_Errno errNo = RIL_E_GENERIC_FAILURE;

    char filghtMode[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_AIRPLANE_MODE, filghtMode, "false");
    if (strcmp("false", filghtMode) || isUnderCryptKeeper()) {
        logE(RFX_LOG_TAG, "under airplane mode or in cryptKeeper, return");
        for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        }
        errNo = RIL_E_SUCCESS;
        sp<RfxMclMessage> resMsg = RfxMclMessage::obtainResponse(msg->getId(), errNo, RfxVoidData(),
                msg, false);
        responseToTelCore(resMsg);
        return;
    }

    char simMode_prop[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_SIM_MODE, simMode_prop,
            (String8::format("%d", (1 << getSimCount()) - 1)).string());
    int simMode = atoi(simMode_prop);
    int originMode = simMode & ((int *)msg->getData()->getData())[0];

    // EFUN affected by SIM switch
    int mainSlotId = RfxRilUtils::getMajorSim() - 1;
    int slotMode = 0;
    int targetMode = 0;
    logI(RFX_LOG_TAG, "bootupSetRadioPower, origin mode = %d", originMode);
    for (int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
        slotMode = (originMode & (1 << i)) >> i;

        sendMipcRequest(i, slotMode, -1);

        switch(mainSlotId) {
            case RFX_SLOT_ID_0:
                slotMode = slotMode << i;
                break;
            case RFX_SLOT_ID_1:
                if (RFX_SLOT_ID_0 == i) {
                    slotMode = slotMode << 1;
                } else if (RFX_SLOT_ID_2 == i) {
                    slotMode = slotMode << 2;
                } else if (RFX_SLOT_ID_3 == i) {
                    slotMode = slotMode << 3;
                }
                break;
            case RFX_SLOT_ID_2:
                if (RFX_SLOT_ID_0 == i) {
                    slotMode = slotMode << 1;
                } else if (RFX_SLOT_ID_1 == i) {
                    slotMode = slotMode << 2;
                } else if (RFX_SLOT_ID_3 == i) {
                    slotMode = slotMode << 3;
                }
                break;
            case RFX_SLOT_ID_3:
                if (RFX_SLOT_ID_0 == i) {
                    slotMode = slotMode << 1;
                } else if (RFX_SLOT_ID_1 == i) {
                    slotMode = slotMode << 2;
                } else if (RFX_SLOT_ID_2 == i) {
                    slotMode = slotMode << 3;
                }
                break;
            default:
                break;
        }
        targetMode |= slotMode;
        slotMode = 0;
    }
    logI(RFX_LOG_TAG, "bootupSetRadioPower, target mode = %d", targetMode);

    errNo = RIL_E_SUCCESS;
    if (targetMode == 0) {
        for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE, RADIO_STATE_OFF);
        }
    } else {
        for(int i = 0; i < RfxRilUtils::rfxGetSimCount(); i++) {
            if (originMode & (RADIO_MODE_SIM1_ONLY << i)) {
                getMclStatusManager(i)->setBoolValue(RFX_STATUS_KEY_REQUEST_RADIO_POWER, true);
            }
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_STATE,
                    (originMode & (RADIO_MODE_SIM1_ONLY << i)) ? RADIO_STATE_ON :
                     RADIO_STATE_OFF);
            getMclStatusManager(i)->setIntValue(RFX_STATUS_KEY_RADIO_POWER_MSIM_MODE,
                    (originMode & (RADIO_MODE_SIM1_ONLY << i)) ? 1 : 0);
        }
    }
    sp<RfxMclMessage> resMsg = RfxMclMessage::obtainResponse(msg->getId(), errNo, RfxVoidData(),
            msg, false);
    responseToTelCore(resMsg);
}

#if defined(MTK_RIL_THIN_MD_SUPPORT)
void RmmRadioRequestHandler::mdStatusEventCb(void *priv_ptr  __unused, struct md_status_event_t *event_ptr) {
    RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG),
            String8::format("mdStatusEventCb: event_type: %d, event_flag: %x",
            event_ptr->event_type, event_ptr->event_flag), RFX_SLOT_ID_0);

    // ThinModem reset fsm state change flow.
    // MDEE:
    //   [T800] MDEE-> OFF-> ON->DOWNLOAD->BOOTUP->READY
    //   [T900 with slient reboot] MDEE-> BOOTUP bit20=1-> READY
    // Reset:
    //   [T800] READY-> OFF-> ON-> DOWNLOAD-> BOOTUP-> READY
    //   [T900 with slient reboot] READY-> BOOTUP bit20=1-> READY
    if ((FSM_STATE_ON == event_ptr->event_type || FSM_STATE_READY == event_ptr->event_type)
            && sMdStatusEventType == FSM_STATE_OFF) {
        RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN)
                ->setBoolValue(RFX_STATUS_KEY_MODEM_RESET, true);
        sendUrcToTelCor(event_ptr->reason);
        sMdStatusEventType = FSM_STATE_ON;
        // For BIP restart {
        if (FSM_STATE_ON == event_ptr->event_type) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "reset");
        } else {
            rfx_property_set("vendor.ril.md_status_from_ccci", "ready");
        }
        // For BIP restart }
        rfx_property_set("vendor.ril.mtk.restart", "1");
    } else if (FSM_STATE_OFF == event_ptr->event_type
            || FSM_STATE_MDEE == event_ptr->event_type
            // Modem reset with sAP non reset will receive FSM_STATE_BOOTUP with bit20 set
            || ((FSM_STATE_BOOTUP == event_ptr->event_type) && (0x100000 & event_ptr->event_flag))) {
        bool mdOff = RfxMclStatusManager::getNonSlotMclStatusManager()->getBoolValue(
                RFX_STATUS_KEY_MODEM_POWER_OFF, false);
        if (false == mdOff) {
            RfxMclStatusManager::getNonSlotMclStatusManager()->setBoolValue(
                    RFX_STATUS_KEY_MODEM_POWER_OFF, true);
        }
        // Recording there is a modem off event
        sMdStatusEventType = FSM_STATE_OFF;
        // For BIP restart {
        if (FSM_STATE_OFF == event_ptr->event_type
                || ((FSM_STATE_BOOTUP == event_ptr->event_type) && (0x100000 & event_ptr->event_flag))) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "stop");
        } else if (FSM_STATE_MDEE == event_ptr->event_type) {
            rfx_property_set("vendor.ril.md_status_from_ccci", "exception");
        }
        // For BIP restart }
    } else if (FSM_STATE_READY == event_ptr->event_type) {
        // For BIP restart {
        rfx_property_set("vendor.ril.md_status_from_ccci", "ready");
        // For BIP restart }
    }
}

// get reset lock at bootup
void RmmRadioRequestHandler::getResetLock() {
    RfxRilUtils::printLog(DEBUG, String8::format("%s", RFX_LOG_TAG),
            String8::format("getResetLock"), RFX_SLOT_ID_0);
}

// release reset lock at following case
// 1. Leave flight mode
// 2. Receive MD_STA_EV_RESET_REQUEST
// 3. Receive MD_STA_EV_STOP_REQUEST
void RmmRadioRequestHandler::releaseResetLock() {
    RfxRilUtils::printLog(DEBUG, String8::format("%s", RFX_LOG_TAG),
            String8::format("releaseResetLock"), RFX_SLOT_ID_0);
}

#else
void RmmRadioRequestHandler::mdStatusEventCb(void *priv_ptr  __unused, struct md_status_event_t *event_ptr) {
    RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG),
            String8::format("mdStatusEventCb: event_type: %d",
            event_ptr->event_type), RFX_SLOT_ID_0);

    // LibDumpRildInfo::AddDumpMessage("radioPower",
    //        String8::format("MODEM State:%d", event_ptr->event_type));

    if (MD_STA_EV_RESET_REQUEST == event_ptr->event_type) {
        RfxMclStatusManager::getMclStatusManager(RFX_SLOT_ID_UNKNOWN)
                ->setBoolValue(RFX_STATUS_KEY_MODEM_RESET, true);
        sendUrcToTelCor(event_ptr->reason);
    } else if (MD_STA_EV_STOP == event_ptr->event_type
            && sMdStatusEventType == MD_STA_EV_LEAVE_FLIGHT_E_REQUEST) {
        rfx_property_set("vendor.ril.mtk.restart", "1");
        if (RfxRilUtils::executeCmdForRild("systemctl restart mtkfusionrild.service" , 0) == -1) {
            RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG), String8::format("restart mtkfusionrild.service fail"), RFX_SLOT_ID_0);
        } else {
            RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG), String8::format("restart mtkfusionrild.service succeeded"), RFX_SLOT_ID_0);
        }
    } else if (MD_STA_EV_LEAVE_FLIGHT_REQUEST == event_ptr->event_type
            || MD_STA_EV_START_REQUEST == event_ptr->event_type
            || MD_STA_EV_HS1 == event_ptr->event_type) {
        rfx_property_set("vendor.ril.mtk.restart", "1");
        if (RfxRilUtils::executeCmdForRild("systemctl restart mtkfusionrild.service" , 0) == -1) {
            RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG), String8::format("restart mtkfusionrild.service fail"), RFX_SLOT_ID_0);
        } else {
            RfxRilUtils::printLog(ERROR, String8::format("%s", RFX_LOG_TAG), String8::format("restart mtkfusionrild.service succeeded"), RFX_SLOT_ID_0);
        }
    } else if (MD_STA_EV_STOP_REQUEST == event_ptr->event_type
            || MD_STA_EV_STOP == event_ptr->event_type
            || MD_STA_EV_LEAVE_FLIGHT_E_REQUEST == event_ptr->event_type
            || MD_STA_EV_F_ASSERT_REQUEST == event_ptr->event_type
            || MD_STA_EV_EXCEPTION == event_ptr->event_type) {
        bool mdOff = RfxMclStatusManager::getNonSlotMclStatusManager()->getBoolValue(
                RFX_STATUS_KEY_MODEM_POWER_OFF, false);
        if (false == mdOff) {
            RfxMclStatusManager::getNonSlotMclStatusManager()->setBoolValue(
                    RFX_STATUS_KEY_MODEM_POWER_OFF, true);
        }

        if (MD_STA_EV_LEAVE_FLIGHT_E_REQUEST == event_ptr->event_type) {
            sMdStatusEventType = MD_STA_EV_LEAVE_FLIGHT_E_REQUEST;
        }
    }
}

// get reset lock at bootup
void RmmRadioRequestHandler::getResetLock() {
    RfxRilUtils::printLog(DEBUG, String8::format("%s", RFX_LOG_TAG),
            String8::format("getResetLock"), RFX_SLOT_ID_0);
    ioctl(sMdStatusFd, CCCI_IOC_HOLD_RST_LOCK, -1);
}

// release reset lock at following case
// 1. Leave flight mode
// 2. Receive MD_STA_EV_RESET_REQUEST
// 3. Receive MD_STA_EV_STOP_REQUEST
void RmmRadioRequestHandler::releaseResetLock() {
    RfxRilUtils::printLog(DEBUG, String8::format("%s", RFX_LOG_TAG),
            String8::format("releaseResetLock"), RFX_SLOT_ID_0);

    ioctl(sMdStatusFd, CCCI_IOC_FREE_RST_LOCK, -1);
}
#endif

void RmmRadioRequestHandler::sendUrcToTelCor(char* buffer) {
    sp<RfxMclMessage> urc;
    for (int i=0; i<getSimCount(); i++) {
        RfxRilUtils::printLog(DEBUG, String8::format("%s", RFX_LOG_TAG),
                String8::format(
                "[RmmRadioRequestHandler] send modem reset urc"), i);
        urc = RfxMclMessage::obtainUrc(
                RFX_MSG_UNSOL_MODEM_RESTART, i,
                RfxStringData((void*)buffer, strlen(buffer)));

        RfxDispatchThread::enqueueUrcMessage(urc);
    }
}

/*
 *A special paragraph, not to trun off modem power under cryptkeeper
 */
bool RmmRadioRequestHandler::isUnderCryptKeeper() {
    char vold_decrypt[RFX_PROPERTY_VALUE_MAX] = { 0 };
    rfx_property_get(PROPERTY_DECRYPT, vold_decrypt, "false");

    if (!strcmp("trigger_restart_min_framework", vold_decrypt)) {
        logI(RFX_LOG_TAG, "UnderCryptKeeper");
        return true;
    }
    return false;
}
