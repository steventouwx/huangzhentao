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
 * File name:  RfxController.h
 * Author: Jun Liu (MTK80064)
 * Description:
 *  Define the prototypes of base controller class.
 */

#ifndef __RFX_CONTROLLER_H__
#define __RFX_CONTROLLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <sys/types.h>
#include "utils/String8.h"
#include "RfxDefs.h"
#include "RfxMessage.h"
#include "RfxObject.h"
#include "RfxStatusManager.h"
#include "RfxTimer.h"
#include "RfxSignal.h"
#include "RfxMessageId.h"

using ::android::String8;

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/
class RfxController;

/*
 * Class RfxController,
 *  prototype of base controller class, define the virtual
 *  functions which sub-class need to override, implement
 *  the default behaviors of controllers
 */
class RfxController : public RfxObject
{
    RFX_DECLARE_CLASS(RfxController);
// Constructor / Destructor
public:
    // Constructor
    RfxController();

    virtual ~RfxController();

public:

    enum HANDLER_PRIORITY{
        NORMAL,
        DEFAULT = NORMAL,
        MEDIUM,
        HIGH,
        HIGHEST
    };

// Overridable
protected:

    virtual bool onPreviewMessage(const sp<RfxMessage>& message);

    virtual bool onHandleRequest(const sp<RfxMessage>& message);

    virtual bool onHandleUrc(const sp<RfxMessage>& message);

    virtual bool onHandleResponse(const sp<RfxMessage>& message);

    virtual bool onCheckIfResumeMessage(const sp<RfxMessage>& message);

    virtual bool onCheckIfRemoveSuspendedMessage(const sp<RfxMessage>& message);

    virtual bool onCheckIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState);

    virtual bool onHandleAtciRequest(const sp<RfxMessage>& message);

    virtual bool onHandleAtciResponse(const sp<RfxMessage>& message);

// Methods
public:

    RfxController *getSlotRoot(int slot_id) const;

    RfxController *getSlotRoot() const;

    RfxController *findController(int slot_id, const RfxClassInfo *class_info);

    RfxController *findController(const RfxClassInfo *class_info);

    void requestToMcl(const sp<RfxMessage>& message, bool sendToMainProtocol = false);
    void requestToMcl(const sp<RfxMessage>& message, bool sendToMainProtocol, nsecs_t nsec);

    void requestAckToRilj(const sp<RfxMessage>& message);

    bool responseToRilj(const sp<RfxMessage>& message);

    bool responseToBT(const sp<RfxMessage>& message);

    void registerToHandleRequest(const int *request_id_list,
            size_t length,
            HANDLER_PRIORITY priority = DEFAULT);

    void unregisterToHandleRequest(const int *request_id_list,
            size_t length,
            HANDLER_PRIORITY priority = DEFAULT);

    void registerToHandleUrc(const int *urc_id_list, size_t length);

    void unregisterToHandleUrc(const int *urc_id_list, size_t length);

    void registerToHandleRequest(int slot_id, const int *request_id_list,
            size_t length,
            HANDLER_PRIORITY priority = DEFAULT);

    void unregisterToHandleRequest(int slot_id, const int *request_id_list,
            size_t length,
            HANDLER_PRIORITY priority = DEFAULT);

    void registerToHandleUrc(int slot_id, const int *urc_id_list, size_t length);

    void unregisterToHandleUrc(int slot_id, const int *urc_id_list, size_t length);

    int getSlotId() const;

    RfxStatusManager *getStatusManager(int slot_id) const;

    RfxStatusManager *getStatusManager() const;

    RfxStatusManager *getNonSlotScopeStatusManager() const;

    void logD(const char *tag, const char *fmt, ...) const;
    void logI(const char *tag, const char *fmt, ...) const;
    void logV(const char *tag, const char *fmt, ...) const;
    void logE(const char *tag, const char *fmt, ...) const;
    void logW(const char *tag, const char *fmt, ...) const;

    const char* idToString(int id);

    int getFeatureVersion(char *feature, int defaultVaule);

    int getFeatureVersion(char *feature);

// Override
public:

    virtual void onInit();

    virtual void onDeinit();

public:

    // framework internal use
    virtual bool processMessage(const sp<RfxMessage>& message);

    bool checkIfResumeMessage(const sp<RfxMessage>& message);

    bool checkIfRemoveSuspendedMessage(const sp<RfxMessage>& message);

    bool checkIfRejectMessage(const sp<RfxMessage>& message,
            bool isModemPowerOff, int radioState);

    virtual bool processAtciMessage(const sp<RfxMessage>& message);

    virtual void clearMessages();

private:
    String8 toString();

protected:

    int m_slot_id;

    int m_last_token;
};

inline
RfxController *RfxController::getSlotRoot() const {
    return getSlotRoot(m_slot_id);
}

inline
RfxController *RfxController::findController(const RfxClassInfo *class_info) {
    return findController(m_slot_id, class_info);
}

inline
RfxStatusManager *RfxController::getStatusManager() const {
    return getStatusManager(m_slot_id);
}

inline
RfxStatusManager *RfxController::getNonSlotScopeStatusManager() const {
    return getStatusManager(RFX_SLOT_ID_UNKNOWN);
}

inline
void RfxController::registerToHandleRequest(const int *request_id_list,
            size_t length, HANDLER_PRIORITY priority) {
    registerToHandleRequest(m_slot_id, request_id_list, length, priority);
}

inline
void RfxController::unregisterToHandleRequest(const int *request_id_list,
            size_t length, HANDLER_PRIORITY priority) {
    unregisterToHandleRequest(m_slot_id, request_id_list, length, priority);
}

inline
void RfxController::registerToHandleUrc(const int *urc_id_list, size_t length) {
    registerToHandleUrc(m_slot_id, urc_id_list, length);
}

inline
void RfxController::unregisterToHandleUrc(const int *urc_id_list, size_t length) {
    unregisterToHandleUrc(m_slot_id, urc_id_list, length);
}

#endif /* __RFX_CONTROLLER_H__ */

