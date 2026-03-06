// SPDX-License-Identifier: MediaTekProprietary
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
#include "RpOemController.h"
#include "RfxTimer.h"
#include <prop/properties.h>
#include <string>
#include <stdlib.h>
#include <utils/String16.h>
#include <stdio.h>
#include "RfxLog.h"

using ::android::String16;
using ::android::String8;
/*****************************************************************************
 * Class RpOemController
 *****************************************************************************/
#define RP_OEM_TAG "RpOemController"

RFX_IMPLEMENT_CLASS("RpOemController", RpOemController, RfxController);
RFX_OBJ_IMPLEMENT_SINGLETON_CLASS(RpOemController);

RpOemController::RpOemController() {
}

RpOemController::~RpOemController() {
}

void RpOemController::onInit() {
    RfxController::onInit(); // Required: invoke super class implementation
    RFX_LOG_D(RP_OEM_TAG, "[RpOemController] onInit()");

#if (defined(TELEPHONYWARE))
const int request_id_list[] = {
    };

#else
    const int request_id_list[] = {
        RIL_REQUEST_DEVICE_IDENTITY,
        RIL_REQUEST_GET_IMEISV,
    };
#endif

    const int urc_id_list[] = {

    };

    registerToHandleRequest(request_id_list,
            (sizeof(request_id_list) / sizeof(int)));
    registerToHandleUrc(urc_id_list, 0);
}

bool RpOemController::onHandleRequest(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    RFX_LOG_D(RP_OEM_TAG, "[RpOemController] xonHandleRequest req %d", msg_id);

    switch (msg_id) {
    case RIL_REQUEST_DEVICE_IDENTITY:
        requestToRild(message);
        break;
    case RIL_REQUEST_GET_IMEISV:
        requestToRild(message);
        break;
    default:
        break;
    }
    return true;
}

bool RpOemController::onHandleUrc(const sp<RfxMessage>& message) {
    return true;
}

bool RpOemController::onHandleResponse(const sp<RfxMessage>& message) {
    int msg_id = message->getId();
    RFX_LOG_D(RP_OEM_TAG, "[RpOemController] onHandleResponse req %d", msg_id);
    switch (msg_id) {
    case RIL_REQUEST_DEVICE_IDENTITY:
        responseToRilj(message);
        break;
    case RIL_REQUEST_GET_IMEISV:
        responseToRilj(message);
        break;
    default:
        break;
    }
    return true;
}


