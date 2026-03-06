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



#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <prop/properties.h>
//#include <android/log.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/mman.h>
#include <time.h>

#include "hardware/ccci_intf.h"
#include "ccci_common.h"

int curr_md_id;
int  system_ch_handle = 0;

#define BOOT_PROF_FILE      "/proc/bootprof"
static int boot_prof_fd;
int update_inf_to_bootprof(const char str[])
{
    int ret = -1, size;
    char msg[64];

    if (boot_prof_fd >= 0) {
        size = snprintf(msg, 64, "ccci_md%d: %s", curr_md_id, str);
        if (size < 0 || size >= 64) {
            CCCI_LOGV("%s-%d:snprintf fail,size = %d\n",
                      __func__, __LINE__, size);
            return -1;
        }
        ret = write(boot_prof_fd, msg, size);
    }

    return ret;
}

#define CCCI_DUMP_FILE      "/proc/ccci_dump"
#define MAX_INFOR_LEN       128
static int ccci_dump_fd;

void write_ccci_dump(const char *cmd, ...)
{
    va_list args;
    int res;
    char *inforBuf;

    if (ccci_dump_fd < 0)
        return;

    inforBuf = (char *)malloc(MAX_INFOR_LEN);
    if (inforBuf == NULL)
        return;

    memset(inforBuf, 0, MAX_INFOR_LEN);

    //va_start(args, cmd);
    res = vsnprintf(inforBuf, MAX_INFOR_LEN, cmd, args);

    if (res < 0 || res >= MAX_INFOR_LEN) {
        //va_end(args);
        CCCI_LOGE("[%s]fail to vsnprintf cmd(%s), err:%s, res=%d\n",
                  __FUNCTION__, cmd, strerror(errno), res);
        goto _Exit;
    }

    //va_end(args);

    //res = TEMP_FAILURE_RETRY(write(ccci_dump_fd, inforBuf, strlen(inforBuf)));
	res = 0;
    if (res < 0) {
        CCCI_LOGE("[%s]fail to write cmd(%s), err:%s, res=%d\n",
                  __FUNCTION__, cmd, strerror(errno), res);
        goto _Exit;
    }

_Exit:
    free(inforBuf);
}

extern void ccci_create_main_thread(int argc, int md_id, int is_v1);
extern void ccci_create_md_status_listen_thread();
extern int first_open_fs_device(void);

int main(int argc, char **argv)
{
    int drv_ver;

    curr_md_id = 0;

    if (argc < 2) {
        CCCI_LOGV("argc value invalid(%d), exit", argc);
        return 0;
    }
    if (strcmp(argv[1], "0") == 0)
        curr_md_id = 1;
    else if (strcmp(argv[1], "1") == 0)
        curr_md_id = 2;
    else if (strcmp(argv[1], "2") == 0)
        curr_md_id = 3;
    else {
        CCCI_LOGV("invalid md_id(%s), exit\n", argv[1]);
        return 0;
    }

    boot_prof_fd = open(BOOT_PROF_FILE, O_RDWR);
    update_inf_to_bootprof("md_init srv start");

    //ccci_dump_fd = TEMP_FAILURE_RETRY(open(CCCI_DUMP_FILE, O_WRONLY | O_CLOEXEC));
    //if (ccci_dump_fd < 0) {
    //    CCCI_LOGE("%s, fail to open ccci_dump, err(%s)\n",
    //              __FUNCTION__, strerror(errno));
    //} else {
    //    write_ccci_dump("md_init srv start, md_id=%d", curr_md_id);
    //}

    //depends_so_prepare();

    if (check_lk_load_md_status(curr_md_id) < 0) {
        CCCI_LOGV("detect lk info fail, exit\n");
        return 0;
    }

    if (kernel_setting_prepare() < 0) {
        CCCI_LOGV("get kernel setting fail, exit\n");
        return 0;
    }

    if (is_current_md_en(curr_md_id) == 0) {
        CCCI_LOGV("md not enabled, exit\n");
        return 0;
    }

    drv_ver = get_ccci_drv_ver();

    CCCI_LOGI("[main] drv_ver: %d\n", drv_ver);

    first_open_fs_device();
    CCCI_LOGI("[main] ccci_create_md_status_listen_thread\n");
    ccci_create_md_status_listen_thread();

    if (drv_ver == 2) {
        ccci_create_main_thread(argc, curr_md_id - 1, 0);
        main_v2(curr_md_id, 0);
    } else if(drv_ver == 1) {
        ccci_create_main_thread(argc, curr_md_id - 1, 1);
        main_v1(curr_md_id, 1);
    } else {
        ccci_create_main_thread(argc, curr_md_id - 1, 1);
        main_v1(curr_md_id, 0);
    }

    //depends_so_free();
    ccci_ccb_exit();

    if (boot_prof_fd >= 0)
        close(boot_prof_fd);

    if (ccci_dump_fd >= 0)
        close(ccci_dump_fd);

    return 0;
}



