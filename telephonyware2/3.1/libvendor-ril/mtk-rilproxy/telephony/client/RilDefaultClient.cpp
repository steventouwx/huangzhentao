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
#include "RilDefaultClient.h"

/*****************************************************************************
 * Class RfxController
 *****************************************************************************/

#define LOG_TAG "RilDefaultClient"
#include <mtk_log.h>

#define ARRAY_LENGTH(array) (sizeof(array)/sizeof(array[0]))

using namespace std;
using namespace android;

RilDefaultClient::RilDefaultClient(int identity, char* socketName) : RilClient(identity, socketName){
    this -> clientState = CLIENT_STATE_UNKNOWN;
    this -> stream = NULL;
    setClientState(CLIENT_INITIALIZING);
    RLOGD("init done");
}

RilDefaultClient::~RilDefaultClient() {

}

void RilDefaultClient::clientStateCallback() {
    RLOGD("Enter callback %s", clientStateToString(clientState));
    switch(clientState) {
        case CLIENT_INITIALIZING:
            handleStateInitializing();
            break;
        case CLIENT_ACTIVE:
            handleStateActive();
            break;
        case CLIENT_DEACTIVE:
            handleStateDeactive();
            break;
        case CLIENT_CLOSED:
            handleStateClosed();
            break;
        default:
            break;
    }
}

void RilDefaultClient::handleStateInitializing() {

    int ret;
    char* socketName = this -> socketName;
    struct sockaddr_un my_addr;
    struct sockaddr_un peer_addr;

    if (listenFd < 0) {
        listenFd = android_get_control_socket(socketName);
    }

    // some trial to manually create socket, will work if permission is added
    if (listenFd < 0) {
        RLOGD("init.rc didnt define, create socket manually");
        //do retry if init.rc didn't define socket
        memset(&my_addr, 0, sizeof(struct sockaddr_un));
        my_addr.sun_family = AF_UNIX;

        char path[256];
        int len = snprintf (path, sizeof(path), "/data/%s", socketName);
        if (len < 0) {
            RLOGW("snprintf return error!!!should never go to here");
        }
        RLOGD("socketName is %s", path);
        strncpy(my_addr.sun_path, path,
            sizeof(my_addr.sun_path) - 1);

        listenFd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (listenFd < 0) {
            RLOGD("manually listen fail, closed");
            setClientState(CLIENT_CLOSED);
            return;
        }

        int ret = ::bind(listenFd, (struct sockaddr *) &my_addr,
                sizeof(struct sockaddr_un));
        if (ret < 0) {
            RLOGD("bind fail, ret = %d, errno = %d, set state to close", ret, errno);
            listenFd = -1;
            setClientState(CLIENT_CLOSED);
            return;
        }
    }

    if (listenFd < 0) {
        RLOGD("Failed to get socket %s", socketName);
        setClientState(CLIENT_CLOSED);
        return;
    }

    RLOGD("start listen on fd: %d", listenFd);
    ret = listen(listenFd, 4);
    if (ret < 0) {
        RLOGD("Failed to listen on control socket '%d': %s",
             listenFd, strerror(errno));
        setClientState(CLIENT_CLOSED);
        return;
    }

    socklen_t socklen = sizeof (peer_addr);
    commandFd = accept (listenFd,  (sockaddr *) &peer_addr, &socklen);
    RLOGD("initialize: commandFd is %d", commandFd);

    if (commandFd < 0 ) {
        RLOGD("Error on accept() errno:%d", errno);
        setClientState(CLIENT_CLOSED);
        return;
    }
    RLOGD("set client state active");
    setClientState(CLIENT_ACTIVE);
}

#define MAX_COMMAND_BYTES (8 * 1024)
void RilDefaultClient::handleStateActive() {
    RecordStream *p_rs;
    void *p_record;
    size_t recordlen;
    int ret;

    if (stream == NULL) {
        RLOGD("Create new stream first time enter active");
        p_rs = record_stream_new(commandFd, MAX_COMMAND_BYTES);
        stream = p_rs;
    } else {
        RLOGD("Already have a stream");
        p_rs = stream;
    }
    RLOGD("command Fd active is %d", commandFd);
    while(clientState == CLIENT_ACTIVE) {
        /* loop until EAGAIN/EINTR, end of stream, or other error */
        if(p_rs != NULL) {
            ret = record_stream_get_next(p_rs, &p_record, &recordlen);
        } else {
            RLOGD("p_rs is NULL, break the loop");
            break;
        }

        if (ret == 0 && p_record == NULL) {
            /* end-of-stream */
            break;
        } else if (ret < 0) {
            break;
        } else if (ret == 0) { /* && p_record != NULL */
            processCommands(p_record, recordlen, 0);
        }
    }

    if (ret == 0 || !(errno == EAGAIN || errno == EINTR)) {
        RLOGD("socket read: ret: %d, errno: %d", ret, errno);
        /* fatal error or end-of-stream */
        if (ret != 0) {
            RLOGD("error on reading command socket errno:%d\n", errno);
        } else {
            RLOGD("EOS.  Closing command socket.");
        }
        setClientState(CLIENT_DEACTIVE);
    }  else {
        RLOGD("socket read: ret: %d, errno: %d, keep reading", ret, errno);
        setClientState(CLIENT_ACTIVE);
    }
}

void RilDefaultClient::handleStateDeactive() {
    if (commandFd != -1) {
        RLOGD("clear Fd Command %d", commandFd);
        close(commandFd);
        commandFd = -1;
    } else {
        RLOGD("commandFd alread -1");
    }

    if (stream != NULL) {
        RLOGD("clear stream because stream exists");
        record_stream_free(stream);
        stream = NULL;
    } else {
        RLOGD("stream null here");
    }

    setClientState(CLIENT_INITIALIZING);
}

void RilDefaultClient::handleStateClosed() {
    if (commandFd != -1) {
        RLOGD("closed: clear Fd Command %d", commandFd);
        close(commandFd);
        commandFd = -1;
    } else {
        RLOGD("closed: commandFd alread -1");
    }

    if (stream != NULL) {
        RLOGD("closed: clear stream because stream exists");
        record_stream_free(stream);
        stream = NULL;
    } else {
        RLOGD("closed: stream null here");
    }
}

void RilDefaultClient::setClientState(RilClientState state) {
    if (clientState != state) {
        RLOGD("set client state %s with old state %s",
            clientStateToString(state), clientStateToString(clientState));
        clientState = state;
    } else {
        RLOGD("client state is already %s", clientStateToString(state));
    }
    activityThread = new StateActivityThread(this);
    activityThread -> run("RilDefaultClient");
}

char* RilDefaultClient::clientStateToString(RilClientState state) {
    char* ret;
    switch(state) {
        case CLIENT_INITIALIZING:
            ret =  "CLIENT_INITIALIZING";
            break;
        case CLIENT_ACTIVE:
            ret =  "CLIENT_ACTIVE";
            break;
        case CLIENT_DEACTIVE:
            ret =  "CLIENT_DEACTIVE";
            break;
        case CLIENT_CLOSED:
            ret = "CLIENT_CLOSED";
            break;
        default:
            ret = "NO_SUCH_STATE";
            break;
    }
    return ret;
}

RilDefaultClient::StateActivityThread::StateActivityThread (RilDefaultClient* client){
    this -> client = client;
    RLOGD("Consctruct Activity thread");
}

RilDefaultClient::StateActivityThread::~StateActivityThread() {
    RLOGD("Desctruct Activity thread");
}

bool RilDefaultClient::StateActivityThread::threadLoop() {
    client -> clientStateCallback();
    return false;
}
