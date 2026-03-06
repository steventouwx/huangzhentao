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

#include <prop/properties.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <cutils/sockets.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <binder/Parcel.h>
#include <string.h>
#include "RilClient.h"
#include "Rfx.h"
#include "../include/telephony/ril.h"
#include "RpRilClientController.h"
#include "RfxDefaultDestUtils.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define LOG_TAG "RilClient"
#include <mtk_log.h>

#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(array[0]))

using namespace std;
using namespace android;

RilClient::RilClient(int identity, char* clientName) {
    this -> identity = identity;
    this -> socketName = clientName;
    this -> commandFd = -1;
    this -> listenFd = -1;

    RLOGD("init done");
}

RilClient::~RilClient() {

}

void RilClient::processCommands(void *buffer, size_t buflen, int slotId) {
    Parcel* p =  new Parcel();
    status_t status;
    int32_t request = 0;;
    int32_t token = 0;

    p -> setData((uint8_t *) buffer, buflen);
    status = p -> readInt32(&request);
    status = p -> readInt32 (&token);
    p->setDataPosition(0);

    int clientId = this->identity;

    //TODO: get 3g capacity protocol
    RLOGD("enqueue request id %d with token %d for client %d", request, token, clientId);

    RILD_RadioTechnology_Group dest = RfxDefaultDestUtils::getDefaultDest(request);

    if (dest == RADIO_TECH_GROUP_C2K) {
        int cdmaSlotId = RpRilClientController::getCdmaSlotId();
        RLOGD("Default destination is cdma, enqueue to slot %d", cdmaSlotId);
        rfx_enqueue_client_request_message(request, cdmaSlotId, dest, token, p, clientId);
    } else {
        RLOGD("Default destination is gsm, enqueue to slot %d", slotId);
        rfx_enqueue_client_request_message(request, slotId, dest, token, p, clientId);
    }
}

void RilClient::sendRequest(Parcel& dataParcel, int slotId) {
    int clientId = this->identity;
    int32_t request = 0;
    int32_t token = 0;
    Parcel* p =  new Parcel();
    p->setData(dataParcel.data(), dataParcel.dataSize());
    dataParcel.setDataPosition(0);
    dataParcel.readInt32(&request);
    dataParcel.readInt32(&token);

    RLOGD("enqueue request id %d with slot %d token %d for client %d datasize %d ", request, slotId, token,
            clientId, dataParcel.dataSize());

    RILD_RadioTechnology_Group dest = RfxDefaultDestUtils::getDefaultDest(request);

    if (dest == RADIO_TECH_GROUP_C2K) {
        int cdmaSlotId = RpRilClientController::getCdmaSlotId();
        RLOGD("Default destination is cdma, enqueue to slot %d", cdmaSlotId);
        rfx_enqueue_client_request_message(request, cdmaSlotId, dest, token, (void *)p, clientId);
    } else {
        RLOGD("Default destination is gsm, enqueue to slot %d", slotId);
        rfx_enqueue_client_request_message(request, slotId, dest, token, (void *)p, clientId);
    }
}

void RilClient::sendRequest(int request, int slotId, int token, void *data, size_t datalen) {
    int clientId = this->identity;

    Parcel p;
    p.writeInt32(request);
    p.writeInt32(token);
    if (data != NULL) {
        p.write(data, datalen);
    }

    RLOGD("enqueue request id %d with slot %d token %d for client %d", request, slotId, token,
            clientId);

    RILD_RadioTechnology_Group dest = RfxDefaultDestUtils::getDefaultDest(request);

    if (dest == RADIO_TECH_GROUP_C2K) {
        int cdmaSlotId = RpRilClientController::getCdmaSlotId();
        RLOGD("Default destination is cdma, enqueue to slot %d", cdmaSlotId);
        rfx_enqueue_client_request_message(request, cdmaSlotId, dest, token, (void *)&p, clientId);
    } else {
        RLOGD("Default destination is gsm, enqueue to slot %d", slotId);
        rfx_enqueue_client_request_message(request, slotId, dest, token, (void *)&p, clientId);
    }
}

void RilClient::onRequestComplete(int slotId, int token, RIL_Errno e, void *response,
                size_t responselen) {
    if (commandFd < 0) {
        RFX_LOG_D(LOG_TAG, "command Fd not ready here");
        return;
    }

    int ret;
    size_t errorOffset;

    Parcel p;
    p.writeInt32(RESPONSE_SOLICITED);
    p.writeInt32(token);
    errorOffset = p.dataPosition();
    p.writeInt32 (e);

    if (response != NULL) {
        ret = p.write(response, responselen);

        if (ret != 0) {
            RFX_LOG_D(LOG_TAG, "responseFunction error, ret %d", ret);
            p.setDataPosition(errorOffset);
            p.writeInt32 (ret);
        }
    }

    if (e != RIL_E_SUCCESS) {
        RFX_LOG_D(LOG_TAG, "fails by %d", e);
    }

    RpRilClientController::sendResponse(p, commandFd);
}

void RilClient::onUnsolicitedResponse(int slotId, int unsolResponse, const void *data,
                                size_t datalen)
{
    int ret;

    if (commandFd == -1) {
        RLOGD("command Fd not ready here");
        return;
    }

    Parcel p;

    p.writeInt32 (RESPONSE_UNSOLICITED);
    p.writeInt32 (unsolResponse);
    ret = p.write(data, datalen);

    if (ret != 0) {
        RLOGD("ret = %d, just return", ret);
    }

    RpRilClientController::sendResponse(p, commandFd);
    return;
}
