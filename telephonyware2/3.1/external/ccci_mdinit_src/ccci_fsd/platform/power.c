/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "fsd_platform.h"
#define PARTITION_PATH_LEN	128

enum {
    ACQUIRE_PARTIAL_WAKE_LOCK = 0,
    RELEASE_WAKE_LOCK,
    OUR_FD_COUNT
};

const char * const OLD_PATHS[] = {
    "/sys/android_power/acquire_partial_wake_lock",
    "/sys/android_power/release_wake_lock",
};

const char * const NEW_PATHS[] = {
    "/sys/power/wake_lock",
    "/sys/power/wake_unlock",
};

//XXX static pthread_once_t g_initialized = THREAD_ONCE_INIT;
static int g_initialized;
static int g_fds[OUR_FD_COUNT];
static int g_error = -1;

static int open_file_descriptors(const char * const paths[])
{
    unsigned int i;
    int ret = 0;

    for (i = 0; i < OUR_FD_COUNT; i++) {
        int fd = open(paths[i], O_RDWR | O_CLOEXEC);
        if (fd < 0) {
            g_error = -errno;
            LOGE("[%s] errno : %d paths: %s", __func__, errno, paths[i]);
            ret = fprintf(stderr, "fatal error opening \"%s\": %s\n", paths[i],
                strerror(errno));
            if (ret <= 0 || ret >= PARTITION_PATH_LEN)
                return -errno;
            return -1;
        }
        g_fds[i] = fd;
    }

    g_error = 0;
    return 0;
}

static void initialize_fds(void)
{
    if (g_initialized == 0) {
        if(open_file_descriptors(NEW_PATHS) < 0) {
            LOGE("open new paths fail! use old path\n");
            open_file_descriptors(OLD_PATHS);
        }
        g_initialized = 1;
    }
}

int acquire_wake_lock(int lock, const char* id)
{
    int fd;
    ssize_t ret;

    initialize_fds();
    if (g_error)
        return g_error;

    if (lock != PARTIAL_WAKE_LOCK) {
        return -EINVAL;
    }

    fd = g_fds[ACQUIRE_PARTIAL_WAKE_LOCK];

    ret = write(fd, id, strlen(id));
    if (ret < 0) {
        return -errno;
    }

    return ret;
}

int release_wake_lock(const char* id)
{
    initialize_fds();

    if (g_error)
        return g_error;

    ssize_t len = write(g_fds[RELEASE_WAKE_LOCK], id, strlen(id));
    if (len < 0) {
        return -errno;
    }
    return len;
}
