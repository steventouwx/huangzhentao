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
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "Wakelockutils.h"
//#include <mtk_log.h>
//#include "RfxLog.h"

static timer_t sWatchDogTimer[2];
#define LOG_TAG "RpSmsControllerBase"

void _watch_dog(sigval_t sig) {
    RLOGI("watch_dog, slot=%d",sig.sival_int);
    switch (sig.sival_int) {
    case 0:
        mtk_release_wake_lock(LONG_SMS_WAKE_LOCK_NAME0);
    case 1:
        mtk_release_wake_lock(LONG_SMS_WAKE_LOCK_NAME1);
    }
}

void init_watch_dog(int slot) {
    RLOGI("_init_watch_dog, slot=%d",slot);
    struct sigevent sevp;
    memset(&sevp, 0, sizeof(sevp));
    sevp.sigev_value.sival_int = slot;
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_notify_function = _watch_dog;

    if(timer_create(CLOCK_MONOTONIC, &sevp, &sWatchDogTimer[slot]) == -1) {
        RLOGE("timer_create  failed reason=[%s]", strerror(errno));
        //RFX_ASSERT(0);
    }
}

void start_watch_dog(int slot, int milliseconds) {
    RLOGI("_start_watch_dog, slot=%d",slot);
    struct itimerspec expire;
    expire.it_interval.tv_sec = 0;
    expire.it_interval.tv_nsec = 0;
    expire.it_value.tv_sec = milliseconds/1000;
    expire.it_value.tv_nsec = (milliseconds%1000)*1000000;
    timer_settime(sWatchDogTimer[slot], 0, &expire, NULL);
}


void stop_watch_dog(int slot) {
    RLOGI("_stop_watch_dog, slot=%d",slot);
    start_watch_dog(slot, 0);
}
