/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <prop/properties.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/mman.h>
//#include <time.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include "hardware/ccci_intf.h"
#include "ccci_mdinit_cfg.h"
#include "ccci_common.h"

#define CCCI_TIME_UPDATE_PORT    "/dev/ccci_ipc_5"

//extern int time_monitor_thd_quit;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int ipc_fd;

static int get_west_time_zone(void)
{
    time_t raw_time;
    struct tm *local_time = NULL;

    raw_time = time(NULL);
    if(raw_time == (time_t)-1) {
        CCCI_LOGE("call time function fail\n");
        return 0;
    }

    local_time = localtime(&raw_time);
    if(local_time == NULL) {
        CCCI_LOGE("call localtime function fail\n");
        return 0;
    }

    return -((local_time->tm_gmtoff)/60);
}


int time_srv_init(void)
{
    int curr_tz;
    CCCI_LOGD("time_srv_init++\n");
    ipc_fd = open(CCCI_TIME_UPDATE_PORT, O_RDWR);
    if(ipc_fd < 0) {
        CCCI_LOGE("Open ipc port %d fail\n", errno);
        return -1;
    }

    curr_tz = get_west_time_zone();

    if(ioctl(ipc_fd, CCCI_IPC_UPDATE_TIMEZONE, curr_tz) < 0) {
        CCCI_LOGI("Set default tz by ipc port fail(%d)\n", errno);
        return -2;
    }
    return 0;
}


#define TIMER_USER_CFG		100
#define TZONE_POLL_INTERVAL	30

static struct timespec s_ts_boot_ref, s_ts_boot_curr;
static struct timeval s_tv_time_ref, s_tv_time_curr;

static void init_time_delta(void)
{
    gettimeofday(&s_tv_time_ref, NULL);
    clock_gettime(CLOCK_BOOTTIME, &s_ts_boot_ref);
}

static int is_delta_same(void)
{
    long long boot_delta, time_delta, delta;

    gettimeofday(&s_tv_time_curr, NULL);
    clock_gettime(CLOCK_BOOTTIME, &s_ts_boot_curr);
    time_delta = (long long)(s_tv_time_curr.tv_sec - s_tv_time_ref.tv_sec);
    boot_delta = (long long)(s_ts_boot_curr.tv_sec - s_ts_boot_ref.tv_sec);
    delta = time_delta - boot_delta;
    s_tv_time_ref.tv_sec = s_tv_time_curr.tv_sec;
    s_ts_boot_ref.tv_sec = s_ts_boot_curr.tv_sec;
    if (delta < 0)
        delta = -delta;

    CCCI_LOGI("%s delta t:%lld delta b:%lld delta:%lld\n",
              __func__, time_delta, boot_delta, delta);

    if (delta > 1)
        return 0;
    return 1;
}

void* monitor_time_update_thread(__attribute__((unused))void *arg)
{
    struct timeval select_to;
    int saved_tz, get_curr_tz, ret, tfd, tz_update, time_update;
    struct itimerspec new_value;
    unsigned long long expirations = 0LL;
    fd_set efds, rfds;
    int round = 0;

    CCCI_LOGD("%s begin to run\n", __func__);

    tfd = timerfd_create(CLOCK_REALTIME, O_NONBLOCK);
    if (tfd < 0) {
        CCCI_LOGE("timerfd_create fail%d\n", tfd);
        return NULL;
    }
    memset(&new_value, 0, sizeof(new_value));
    new_value.it_value.tv_sec = TIMER_USER_CFG;
    new_value.it_value.tv_nsec = 0;
    ret = timerfd_settime(tfd, TFD_TIMER_ABSTIME|TFD_TIMER_CANCEL_ON_SET,
                          &new_value, NULL);
    if (ret < 0) {
        CCCI_LOGE("%s timerfd_settime ret:%d\n", __func__, ret);
        return NULL;
    }

    saved_tz = get_west_time_zone();

    tz_update = 0;
    time_update = 0;
    init_time_delta();
    while(1) {
        pthread_mutex_lock(&mtx);
        CCCI_LOGD("%s round:%d ########\n", __func__, round);
        round++;

        FD_ZERO(&rfds);
        FD_SET(tfd, &rfds);
        FD_ZERO(&efds);
        FD_SET(tfd, &efds);
        select_to.tv_sec = TZONE_POLL_INTERVAL;
        select_to.tv_usec = 0;

        ret = select(tfd + 1, &rfds, NULL, &efds, &select_to);
        if (ret == -1)
            CCCI_LOGI("select try again\n");
        else if (ret == 0) {
            get_curr_tz = get_west_time_zone();
            if(saved_tz != get_curr_tz) {
                saved_tz = get_curr_tz;
                CCCI_LOGI("Time zone update, new val:%d\n", saved_tz);
                tz_update = 1;
            }
        } else {
            ret = read(tfd, &expirations, 8);
            if ((ret < 0) && (errno == ECANCELED)) {
                if (!is_delta_same()) {
                    time_update = 1;
                    CCCI_LOGI("Time updated, need notify!\n");
                }
            }
        }

        if (tz_update || time_update) {
            if(ioctl(ipc_fd, CCCI_IPC_UPDATE_TIME, saved_tz) < 0)
                CCCI_LOGI("Update time to md by ipc port fail(%d)\n", errno);
            else {
                CCCI_LOGI("Update time to md done\n");
                tz_update = 0;
                time_update = 0;
            }
        }

        pthread_mutex_unlock(&mtx);
    }

    close(tfd);
    close(ipc_fd);
    return NULL;
}