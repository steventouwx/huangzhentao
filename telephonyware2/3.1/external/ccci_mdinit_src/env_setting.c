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
//#include <prop/properties.h>
//#include <android/log.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/mman.h>
#include "hardware/ccci_intf.h"
#include "ccci_common.h"
//#include "sysenv_utils.h"

static unsigned int sys_boot_mode = 0xFFFFFFFF;
//#define BOOT_MODE_FILE            "/sys/class/BOOT/BOOT/boot/boot_mode" // controlled by mtxxxx_boot.c
#define BOOT_MODE_FILE "/proc/device-tree/chosen/atag,boot"
#define NORMAL_MODE        0
#define META_MODE          1
#define FACTORY_MODE       4
#ifndef PROPERTY_VALUE_MAX
#define PROPERTY_VALUE_MAX 64
#endif

static unsigned int get_sys_boot_mode(void)
{
    int fd = -1, ret = 0;
    size_t s;
    volatile char data[20];

    if (sys_boot_mode == 0xFFFFFFFF) {
        fd = open(BOOT_MODE_FILE, O_RDONLY);
        if (fd < 0) {
            CCCI_LOGE("fail to open %s: err_no=%d\n", BOOT_MODE_FILE, errno);
            return 0;
        }

        s = read(fd, (void *)data, 16);
        if (s <= 0) {
            CCCI_LOGE("fail to read %s err_no=%d\n", BOOT_MODE_FILE, errno);
            sys_boot_mode = NORMAL_MODE;

        }
        else {
            sys_boot_mode = data[2];

        }
        CCCI_LOGD("s: 0x%zX; data: %08X %08X %08X %08X\n",
                  s, data[0], data[1], data[2], data[3]);
        close(fd);
    }

    CCCI_LOGD("system boot Mode: %d\n", sys_boot_mode);

    return sys_boot_mode;
}

int is_factory_mode(void)
{
    if (get_sys_boot_mode() == FACTORY_MODE)
        return 1;
    else
        return 0;
}

int double_check_meta(void)
{
    int ret = 0;
    int current_boot_mode = MD_BOOT_MODE_INVALID;

    if(ioctl(system_ch_handle, CCCI_IOC_GET_MD_BOOT_MODE, &current_boot_mode) == 0) {
        if(current_boot_mode == MD_BOOT_MODE_META)
            ret = 1;
        else
            ret = 0;
    }
    else {
        CCCI_LOGE("fail to ioctl CCCI_IOC_GET_MD_BOOT_MODE err_no=%d\n", system_ch_handle);
    }

    return ret;
}

int is_meta_mode(void)
{
    if (get_sys_boot_mode() == META_MODE)
        return 1;
    else
        return double_check_meta();
}

/* Env variable need to store support */
static int md_env_val_get(const char name[], char buf[], int size)
{
    const char *tmp_buf = NULL;
    int ret = 0;

    CCCI_LOGD("md env r\n");

    //tmp_buf = sysenv_get(name);
	tmp_buf = "";
    ret = snprintf(buf, size, "%s", tmp_buf);
    if (ret < 0 || ret >= size) {
        CCCI_LOGI("snprintf fail\n");
        return -2;
    }
    CCCI_LOGI("get %s %s\n", name, buf);
    return 1;
}

static int md_env_val_set(const char name[], const char val[])
{
    int ret = 0;

    CCCI_LOGD("md env w\n");

    //ret = sysenv_set(name, val);
    if (ret) {
        CCCI_LOGE("set %s fail\n", name);
        return -1;
    }
    return ret;
}


int get_stored_modem_type_val(int md_id)
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int retpropget = 0;
    int ret = 0;

    if (md_id != 0)
        return 0; /* Only support modem 1 */

    retpropget = md_env_val_get("md_type", value, sizeof(value));
    if (retpropget > 0)
        ret = atoi(value);
    else
        ret = 0;

    CCCI_LOGD("get_save_modem_type_val: 0x%x[%s]\n", ret, value);

    return ret;
}

int store_modem_type_val(int md_id, int new_val)
{
    char value[PROPERTY_VALUE_MAX];
    int val = 0;

    if (md_id != 0)
        return 0; /* Only support modem 1 */
    if (new_val == 0)
        return 0;

    val = snprintf(value, sizeof(value), "%d", new_val);
    if (val < 0 || val >= sizeof(value)) {
        CCCI_LOGD("%s-%d:snprintf fail,val = %d\n",
                  __func__, __LINE__, val);
        return -1;
    }
    return md_env_val_set("md_type", value);
}

