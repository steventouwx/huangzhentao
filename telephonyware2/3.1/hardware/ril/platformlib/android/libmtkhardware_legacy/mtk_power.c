//SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2020. All rights reserved.
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

#include <stdbool.h>

#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mtk_power.h"
#define LOG_TAG "rild_power"

// This library is built-in, and we will surely not initialized in threads meanwhile
// Hence not bother pthread_once etc
static bool s_initialized;
static int s_wake_lock_fd = -1;
static int s_wake_unlock_fd = -1;
static int s_errno;

#define WAKE_LOCK_PATH "/sys/power/wake_lock"
#define WAKE_UNLOCK_PATH "/sys/power/wake_unlock"

static int open_fds(void) {
    do {
        s_wake_lock_fd = open(WAKE_LOCK_PATH, O_RDWR);
    } while (s_wake_lock_fd == -1 && errno == EINTR);

    if (s_wake_lock_fd == -1)
        goto fail;

    do {
        s_wake_unlock_fd = open(WAKE_UNLOCK_PATH, O_RDWR);
    } while (s_wake_unlock_fd == -1 && errno == EINTR);

    if (s_wake_unlock_fd == -1)
        goto fail;

    s_errno = 0; //err
    return 0;

fail:
    s_errno = errno;

    if (s_wake_lock_fd >= 0)
        close(s_wake_lock_fd);

    if (s_wake_unlock_fd >= 0)
        close(s_wake_unlock_fd);

    s_wake_lock_fd = s_wake_unlock_fd = -1;

    return -s_errno;
}

static inline int ensure_initialized(void) {
    // XXX: should be this:
    //pthread_once(&g_initialized, open_file_descriptors);
    // XXX: not this:

    if (!s_errno && !s_initialized) {
        if (open_fds() < 0) {
            goto out;
        }

        s_initialized = true;
    }

out:
    return -s_errno;
}

int mtk_acquire_wake_lock(const char* id) {
    int r = ensure_initialized();

    if (r)
        return r;

again:
    // all written or none
    r = write(s_wake_lock_fd, id, strlen(id));

    if (r < 0) {
        if (errno == EINTR)
            goto again;

        return -errno;
    }

    return r;
}

int mtk_release_wake_lock(const char* id) {
    int r = ensure_initialized();
    if (r)
        return r;

again:
    // all written or none
    r = write(s_wake_unlock_fd, id, strlen(id));

    if (r < 0) {
        if (errno == EINTR)
            goto again;

        return -errno;
    }

    return r;
}


