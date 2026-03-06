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
/*
 * File name:  rfx_controller.cpp
 * Author: Jun Liu (MTK80064)
 * Description:
 *  Implementation of base controller class.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include "RfxController.h"
#include "RfxRootController.h"
#include "RfxSlotRootController.h"
#include "RfxStatusManager.h"
#include "RfxLog.h"
#include <inttypes.h>
#include "RfxMclMessage.h"
#include "RfxMclDispatcherThread.h"
#include "RfxIdToStringUtils.h"
#include "RfxMessageId.h"
#include "RfxVersionManager.h"

#define RFX_LOG_TAG "RfxController"
#define LOG_BUF_SIZE 1024

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("RfxController", RfxController, RfxObject);

RfxController::RfxController() :
    m_slot_id(RFX_SLOT_ID_UNKNOWN),
    m_last_token(-1) {
}

RfxController::~RfxController() {
}

RfxController *RfxController::findController(int slot_id, const RfxClassInfo *class_info) {

    RfxController *ret = NULL;

    if (!class_info->isKindOf(RFX_OBJ_CLASS_INFO(RfxController))) {
        return ret;
    }

    RfxController *root = getSlotRoot(slot_id);
    RfxObject *parent;

    if (root == NULL) {
        parent = RFX_OBJ_GET_INSTANCE(RfxRootController);
    } else {
        parent = static_cast<RfxObject *>(root);
    }

    RfxObject *obj = findObject(parent, class_info);

    ret = RFX_OBJ_DYNAMIC_CAST(obj, RfxController);

    // if can't find a controller in slot tree,
    // find in non-slot based tree
    if (ret == NULL && slot_id >= 0 && slot_id < RFX_SLOT_COUNT) {
        root = getSlotRoot(RFX_SLOT_ID_UNKNOWN);
        RFX_ASSERT(root != NULL);
        parent = static_cast<RfxObject *>(root);
        RfxObject *obj = findObject(parent, class_info);
        ret = RFX_OBJ_DYNAMIC_CAST(obj, RfxController);
    }

    return ret;
}

RfxController *RfxController::getSlotRoot(int slot_id) const {

    return RFX_OBJ_GET_INSTANCE(RfxRootController)->getSlotRootController(slot_id);
}

void RfxController::registerToHandleRequest(int slot_id, const int *request_id_list,
                        size_t length, HANDLER_PRIORITY priority) {
    RfxRootController *root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    root->registerToHandleRequest(this, slot_id, request_id_list, length, priority);
}

void RfxController::registerToHandleUrc(int slot_id, const int *urc_id_list, size_t length) {
    RfxRootController *root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    root->registerToHandleUrc(this, slot_id, urc_id_list, length);
}

void RfxController::unregisterToHandleRequest(int slot_id, const int *request_id_list,
            size_t length, HANDLER_PRIORITY priority) {
    RfxRootController *root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    root->unregisterToHandleRequest(slot_id, request_id_list, length, priority);
}

void RfxController::unregisterToHandleUrc(int slot_id,
                                     const int *urc_id_list, size_t length) {
    RfxRootController *root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    root->unregisterToHandleUrc(slot_id, urc_id_list, length);
}

void RfxController::requestToMcl(const sp<RfxMessage>& message, bool sendToMainProtocol) {
    int id = message->getId();
    if (id == RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL) {
        RFX_LOG_D(RFX_LOG_TAG, "Register ATCI response, %s, msg = %s",
                               toString().string(), message->toString().string());
        RFX_OBJ_GET_INSTANCE(RfxRootController)->registerToHandleResponse(this,
            message->getSlotId(), &id, 1, message->getToken());
    }
    if (sendToMainProtocol) {
        message->setSendToMainProtocol(true);
    }
    message->setMainProtocolSlotId(getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0));
    return RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->requestToMcl(message);
}

void RfxController::requestToMcl(const sp<RfxMessage>& message, bool sendToMainProtocol,
        nsecs_t nsec) {
    int id = message->getId();
    if (id == RFX_MSG_REQUEST_OEM_HOOK_ATCI_INTERNAL) {
        RFX_LOG_D(RFX_LOG_TAG, "Register ATCI response, %s, msg = %s",
                               toString().string(), message->toString().string());
        RFX_OBJ_GET_INSTANCE(RfxRootController)->registerToHandleResponse(this,
            message->getSlotId(), &id, 1, message->getToken());
    }
    if (sendToMainProtocol) {
        message->setSendToMainProtocol(true);
    }
    message->setMainProtocolSlotId(getNonSlotScopeStatusManager()->getIntValue(
                    RFX_STATUS_KEY_MAIN_CAPABILITY_SLOT, 0));
    return RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->requestToMclWithDelay(message, nsec);
}

void RfxController::requestAckToRilj(const sp<RfxMessage>& message) {
    return RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->requestAckToRilj(message);
}

bool RfxController::responseToRilj(const sp<RfxMessage>& message) {
    return RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->responseToRilj(message);
}

bool RfxController::responseToBT(const sp<RfxMessage>& message) {
    return RFX_OBJ_GET_INSTANCE(RfxRilAdapter)->responseToBT(message);
}

int RfxController::getSlotId() const {
    return m_slot_id;
}

RfxStatusManager *RfxController::getStatusManager(int slot_id) const {
    return RFX_OBJ_GET_INSTANCE(RfxRootController)->getStatusManager(slot_id);
}

void RfxController::onInit() {
    RfxObject::onInit();
    if (m_slot_id == RFX_SLOT_ID_UNKNOWN) {
        RfxObject *parent = getParent();
        RfxController *controller = RFX_OBJ_DYNAMIC_CAST(parent, RfxController);
        if (controller != NULL) {
            m_slot_id = controller->getSlotId();
        }
    }
}

void RfxController::onDeinit() {
    RFX_LOG_D(RFX_LOG_TAG, "onDeinit() %p, %s", this, getClassInfo()->getClassName());
    RFX_OBJ_GET_INSTANCE(RfxRootController)->clearRegistry(this);
    RfxObject::onDeinit();
}

bool RfxController::processMessage(const sp<RfxMessage>& message) {
    if (!onPreviewMessage(message)) {
        RFX_OBJ_GET_INSTANCE(RfxRootController)->suspendMessage(this, message);
        return true;
    }
    if (message->getType() == REQUEST || message->getType() == SAP_REQUEST) {
        return onHandleRequest(message);
    } else if (message->getType() == URC || message->getType() == SAP_URC) {
        return onHandleUrc(message);
    } else if (message->getType() == RESPONSE || message->getType() == SAP_RESPONSE) {
        return onHandleResponse(message);
    }
    return false;
}

bool RfxController::processAtciMessage(const sp<RfxMessage>& message) {
    if (message->getType() == REQUEST) {
        return onHandleAtciRequest(message);
    } else if (message->getType() == RESPONSE) {
        return onHandleAtciResponse(message);
    }
    return false;
}

bool RfxController::checkIfResumeMessage(const sp<RfxMessage>& message) {
    return onCheckIfResumeMessage(message);
}

bool RfxController::checkIfRemoveSuspendedMessage(const sp<RfxMessage>& message) {
    return onCheckIfRemoveSuspendedMessage(message);
}

bool RfxController::checkIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff, int radioState) {
    return onCheckIfRejectMessage(message, isModemPowerOff, radioState);
}

bool RfxController::onPreviewMessage(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return true;
}

bool RfxController::onHandleRequest(const sp<RfxMessage>& message) {
    requestToMcl(message);
    return true;
}

bool RfxController::onHandleUrc(const sp<RfxMessage>& message) {
    return responseToRilj(message);

}
bool RfxController::onHandleResponse(const sp<RfxMessage>& message) {
    return responseToRilj(message);
}

bool RfxController::onCheckIfResumeMessage(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return false;
}

bool RfxController::onCheckIfRemoveSuspendedMessage(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return false;
}

bool RfxController::onCheckIfRejectMessage(const sp<RfxMessage>& message,
        bool isModemPowerOff, int radioState) {
    RFX_UNUSED(message);
    bool ret = false;
    if (radioState == (int)RADIO_STATE_UNAVAILABLE ||
            radioState == (int)RADIO_STATE_OFF ||
            isModemPowerOff == true) {
        ret = true;
    }
    RFX_LOG_D(RFX_LOG_TAG, "onCheckIfRejectMessage isModemPowerOff %d, radioState: %d",
            (isModemPowerOff == false) ? 0 : 1, radioState);
    return ret;
}

bool RfxController::onHandleAtciRequest(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return false;
}

bool RfxController::onHandleAtciResponse(const sp<RfxMessage>& message) {
    RFX_UNUSED(message);
    return false;
}

String8 RfxController::toString() {
    return String8::format("%p, %s", this, getClassInfo()->getClassName());
}

void RfxController::clearMessages() {
    for (RfxObject *i = getFirstChildObj(); i != NULL; i = i->getNextObj()) {
        RfxController *controller = RFX_OBJ_DYNAMIC_CAST(i, RfxController);
        if (controller != NULL) {
            controller->clearMessages();
        }
    }
}

const char* RfxController::idToString(int id) {
    return RFX_ID_TO_STR(id);
}

int RfxController::getFeatureVersion(char *feature, int defaultVaule) {
    return RfxVersionManager::getInstance()->getFeatureVersion(feature, defaultVaule);
}

int RfxController::getFeatureVersion(char *feature) {
    return getFeatureVersion(feature, 0);
}

void RfxController::logD(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    if (ret < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "out of memory!");
    }
    va_end(ap);

    RfxRilUtils::printLog(DEBUG, tagString,
            String8::format("%s", buf), m_slot_id);
}

void RfxController::logI(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    if (ret < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "out of memory!");
    }
    va_end(ap);

    RfxRilUtils::printLog(INFO, tagString,
            String8::format("%s", buf), m_slot_id);
}

void RfxController::logV(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    if (ret < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "out of memory!");
    }
    va_end(ap);

    RfxRilUtils::printLog(VERBOSE, tagString,
            String8::format("%s", buf), m_slot_id);
}

void RfxController::logE(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    if (ret < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "out of memory!");
    }
    va_end(ap);

    RfxRilUtils::printLog(ERROR, tagString,
            String8::format("%s", buf), m_slot_id);
}

void RfxController::logW(const char *tag, const char *fmt, ...) const {
    va_list ap;
    char buf[LOG_BUF_SIZE] = {0};
    int ret = 0;
    String8 tagString;
    if (RfxRilUtils::getRilRunMode() == RilRunMode::RIL_RUN_MODE_MOCK) {
        tagString = String8::format("%s%s", "[GT]", tag);
    } else {
        tagString = String8::format("%s", tag);
    }
    va_start(ap, fmt);
    ret = vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    if (ret < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "out of memory!");
    }
    va_end(ap);

    RfxRilUtils::printLog(WARN, tagString,
            String8::format("%s", buf), m_slot_id);
}

