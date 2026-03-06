/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2023. All rights reserved.
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
#include "RaEventHandler.h"

#define NA_LOG_TAG "RaEventHandler"

RaEventHandler::RaEventHandler(NetAgentService *na, int listenerSocket) :
        SocketListener(listenerSocket, false) {
    mNetAgent = na;
}

RaEventHandler::~RaEventHandler() {
}

int RaEventHandler::start() {
    return startListener();
}

int RaEventHandler::stop() {
    return stopListener();
}

bool RaEventHandler::onDataAvailable(SocketClient *cli) {
    char buffer[MAX_RA_PAYLOAD_LENGTH / 2] = {0};
    char bufferHex[MAX_RA_PAYLOAD_LENGTH] = {0};
    struct icmp6_hdr *recv_icmphdr;
    struct sockaddr from;
    socklen_t fromlen = sizeof (from);
    memset(&from, 0, sizeof (from));

    int socket = cli->getSocket();
    int bytes = TEMP_FAILURE_RETRY(recvfrom(socket, buffer, MAX_RA_PAYLOAD_LENGTH / 2, 0,
            (struct sockaddr *) &from, &fromlen));
    if (bytes <= 0 || bytes >= MAX_RA_PAYLOAD_LENGTH / 2) {
        NA_LOG_E("[%s] recv message bytes invalid", __FUNCTION__);
        return false;
    }

    NA_LOG_I("[%s] recv ra message bytes=%d", __FUNCTION__, bytes);

    recv_icmphdr = (struct icmp6_hdr*)(buffer);
    if (recv_icmphdr->icmp6_type != ND_ROUTER_ADVERT) {
        NA_LOG_D("[%s] type %d is not ra message", __FUNCTION__, recv_icmphdr->icmp6_type);
        return false;
    }

    int n = 0, offset = 0;
    for (int i = 0; i < bytes; i++) {
        n = snprintf(bufferHex + offset, sizeof(bufferHex) - offset, "%02X", buffer[i]);
        if (n < 0) return false;
        offset += n;
    }

    if (mNetAgent == NULL) {
        NA_LOG_E("[%s] mNetAgent is NULL", __FUNCTION__);
        return false;
    }

    RaEventReqInfo *pRaEventObj = (RaEventReqInfo *)calloc(1, sizeof(RaEventReqInfo));
    if (pRaEventObj == NULL) {
        NA_LOG_E("[%s] can't allocate ra event obj", __FUNCTION__);
        return false;
    }

    pRaEventObj->cmd = NETAGENT_IO_CMD_RAPAYLOAD;
    memcpy(pRaEventObj->raPayload, bufferHex, MAX_RA_PAYLOAD_LENGTH);
    mNetAgent->enqueueReqInfo(pRaEventObj, REQUEST_TYPE_RAEVENT);
    return true;
}

int RaEventHandler::getRaPayload(void *obj, char *raPayload) {
    RaEventReqInfo *pReqInfo = reinterpret_cast<RaEventReqInfo*>(obj);

    if (pReqInfo == NULL) {
        NA_LOG_E("[%s] pReqInfo is NULL", __FUNCTION__);
        return RA_GENERIC_FAILURE;
    }

    memcpy(raPayload, pReqInfo->raPayload, MAX_RA_PAYLOAD_LENGTH);
    return RA_RET_SUCCESS;
}

int RaEventHandler::getCommandType(void *obj, NA_CMD *cmd) {
    RaEventReqInfo *pReqInfo = reinterpret_cast<RaEventReqInfo*>(obj);

    if (pReqInfo == NULL) {
        NA_LOG_E("[%s] pReqInfo is NULL", __FUNCTION__);
        return RA_GENERIC_FAILURE;
    }

    *cmd = pReqInfo->cmd;
    return RA_RET_SUCCESS;
}

void RaEventHandler::freeRaEventObj(void *obj) {
    RaEventReqInfo *pReqInfo = reinterpret_cast<RaEventReqInfo *>(obj);
    if (pReqInfo == NULL) {
        NA_LOG_E("[%s] pReqInfo is NULL", __FUNCTION__);
        return;
    }

    FREEIF(pReqInfo);
}
