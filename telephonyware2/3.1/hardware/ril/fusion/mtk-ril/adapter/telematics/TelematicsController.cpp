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
//#include "RtcHelloController.h"
#include "TelematicsController.h"
#include "RfxMessage.h"
#include <telephony/mtk_ril.h>
#include "log/mtk_log.h"
#include "RfxVoidData.h"

#undef LOG_TAG
#define LOG_TAG "TelematicsController"
/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

RFX_IMPLEMENT_CLASS("TelematicsController", TelematicsController, RfxController);


void TelematicsController::onInit() {
    mtkLogD(LOG_TAG, "onInit TelematicsController");

    RfxController::onInit();  // Required: invoke super class implementation

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_GSM_NUMERIC,
        RfxStatusChangeCallback(this, &TelematicsController::onUiccMccMncChanged));

    getStatusManager()->registerStatusChanged(RFX_STATUS_KEY_UICC_CDMA_NUMERIC,
            RfxStatusChangeCallback(this, &TelematicsController::onUiccMccMncChanged));

    // default set RFX_STATUS_KEY_PREFERRED_DATA_SIM as 0 for default data call can be setup
    // as tbox do not call SET_PREFERRED_DATA_SIM ril request.
    getNonSlotScopeStatusManager()->setIntValue(RFX_STATUS_KEY_PREFERRED_DATA_SIM,
              0, true);

    // use wp<> to retain other object's pointer,
    // if use *, it would be dangling pointer if the object is deleted by others
    // if use sp<>, it will impact the object's life cycel principle which is,
    // who create the object, who delete it
    //m_hello_controller = (RtcHelloController *)findController(RFX_OBJ_CLASS_INFO(RtcHelloController));
}

void TelematicsController::onDeinit() {
    mtkLogD(LOG_TAG, "onDeinit");
    RfxController::onDeinit();
}

void TelematicsController::onUiccMccMncChanged(RfxStatusKeyEnum /*key*/,
    RfxVariant /*old_value*/, RfxVariant /*value*/) {
    mtkLogD(LOG_TAG, "onUiccMccMncChanged");

    sp<RfxMessage> urc = RfxMessage::obtainUrc(m_slot_id, RFX_MSG_URC_RESPONSE_SIM_STATUS_CHANGED, RfxVoidData());
    responseToRilj(urc);
}


