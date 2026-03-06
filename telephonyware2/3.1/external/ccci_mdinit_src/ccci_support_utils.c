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
//#include <android/log.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
//#include <prop/properties.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/mman.h>
#include <time.h>
#include <dlfcn.h>
//#include <sys/sysinfo.h>

#include "ccci_log.h"
#include "ccci_common.h"
//#include <aee.h>

/********************************************************************************
 * Configure region
 ********************************************************************************/
#define LK_LOAD_MD_FILE             "/sys/kernel/ccci/lk_md"
#define KERNEL_SETTING_FILE         "/sys/kernel/ccci/kcfg_setting"

#define KERNEL_SETTING_BUF_SIZE     4096
#define AEE_STR_TMP_BUF_SIZE        4096

#define LIB_AED                     "libaedv.so"



/****************************************************************************/
/****  lk load status check support section   *******************************/
/****************************************************************************/
static void show_warning_for_lk_ld_fail(char warn_msg[])
{
    const char *mod = "ccci_mdinit";
    if (wait_for_property("init.svc.debuggerd", "running", 30 * 1000) == 0)
        sleep(1);
    show_aee_system_exception(mod, "external.ccci_mdinit", 0, "%s", warn_msg);
}

int check_lk_load_md_status(int md_id)
{
    int fd = -1;
    int size;
    char *lk_info_buf = NULL;
    int ret = 0;

    // Because there is no md3 only case, check MD1 enough
    if (md_id != 1) //Not MD_SYS1
        return 0;

    fd = open(LK_LOAD_MD_FILE, O_RDONLY);
    if (fd < 0) {
        CCCI_LOGD("Kernel load modem image case\n");
        return 0;
    }

    lk_info_buf = malloc(4096);
    if (lk_info_buf == NULL) {
        CCCI_LOGE("Alloc pasing memory for lk info fail\n");
        close(fd);
        return -1;
    }

    size = read(fd, lk_info_buf, 4096 - 1);
    if (size <= 0) {
        CCCI_LOGE("LK info read fail:%d\n", errno);
        ret = -1;
        goto _Exit;
    }

    lk_info_buf[size-1] = '\0'; /* Make sure string has terminate */
    CCCI_LOGD("%s(%d)", lk_info_buf, size);

    if (strstr(lk_info_buf, "LK Load MD:[Disabled]")) {
        CCCI_LOGD("LK load modem feature disable\n");
        goto _Exit;
    }

    if (strstr(lk_info_buf, "LK load MD success!")) {
        CCCI_LOGD("LK load modem success\n");
        goto _Exit;
    }

    //show_warning_for_lk_ld_fail(lk_info_buf);
    ret = -1;

_Exit:
    free(lk_info_buf);
    close(fd);

    return ret;
}


/* Common util API section */
unsigned int str2uint(const char *str)
{
    /* max 32bit integer is 4294967296, buf[16] is enough */
    char buf[16];
    int i;
    int num;
    int base = 10;
    int ret_val;
    if (NULL == str)
        return 0;

    i = 0;
    while (i < 16) {
        /* Format it */
        if ((str[i] == 'X') || (str[i] == 'x')) {
            buf[i] = 'x';
            if (i != 1)
                return 0; /* not 0[x]XXXXXXXX */
            else if (buf[0] != '0')
                return 0; /* not [0]xXXXXXXXX */
            else
                base = 16;
        }
        else if ((str[i] >= '0') && (str[i] <= '9'))
            buf[i] = str[i];
        else if ((str[i] >= 'a') && (str[i] <= 'f')) {
            if (base != 16)
                return 0;
            buf[i] = str[i];
        }
        else if ((str[i] >= 'A') && (str[i] <= 'F')) {
            if (base != 16)
                return 0;
            buf[i] = str[i] - 'A' + 'a';
        }
        else if (str[i] == 0) {
            buf[i] = 0;
            i++;
            break;
        }
        else
            return 0;

        i++;
    }

    num = i-1;
    ret_val = 0;
    if (base == 10) {
        for (i = 0; i < num; i++)
            ret_val = ret_val * 10 + buf[i] - '0';
    }
    else if (base == 16) {
        for (i = 2; i < num; i++) {
            if (buf[i] >= 'a')
                ret_val = ret_val * 16 + buf[i] - 'a' + 10;
            else
                ret_val = ret_val * 16 + buf[i] - '0';
        }
    }
    return ret_val;
}


/* so support section */
static int (*aee_system_exception_fun_ptr)(const char*, const char*, unsigned int, const char*,...);
typedef int (*aee_system_exception_t)(const char*, const char*, unsigned int, const char*,...);
static void *lib_handle_aed = NULL;


int show_aee_system_exception(const char *module, const char *path, unsigned int flag,
                              const char *msg, ...)
{
    va_list args;
    char *temp_log;
    int ret;
    int val = 0;

    temp_log = (char *)malloc(AEE_STR_TMP_BUF_SIZE);
    if (temp_log == NULL)
        return -2;

    //va_start(args, msg);
    val = vsnprintf(temp_log, AEE_STR_TMP_BUF_SIZE, msg, args);
    if (val < 0 || val >= AEE_STR_TMP_BUF_SIZE)
        CCCI_LOGD("vsnprintf fail\n");
    //va_end(args);

    if (aee_system_exception_fun_ptr)
        ret = aee_system_exception_fun_ptr(module, path, flag, temp_log);
    else
        ret = -1;
    free(temp_log);

    return ret;
}


void depends_so_prepare(void)
{
    void *func = NULL;

    /* AEE */
    aee_system_exception_fun_ptr = NULL;
    lib_handle_aed = dlopen(LIB_AED, RTLD_NOW);
    if (lib_handle_aed) {
        func = dlsym(lib_handle_aed, "aee_system_exception");
        if (func == NULL)
            CCCI_LOGI("load sym:aee_system_exception fail\n");
        else
            aee_system_exception_fun_ptr = (aee_system_exception_t)func;
    }
    else
        CCCI_LOGI("disable aee\n");
}


void depends_so_free(void)
{
    /* Free AED */
    if (lib_handle_aed)
        dlclose(lib_handle_aed);
}

/*Kernel setting parse section */
static int k_setting_md_en[5];
static int k_setting_ccci_drv_ver;

int is_current_md_en(int md_id)
{
    switch(md_id) {
    case 1:
        return k_setting_md_en[0];
    case 2:
        return k_setting_md_en[1];
    case 3:
        return k_setting_md_en[2];
    default:
        return 0;
    }
}

int get_ccci_drv_ver(void)
{
    return k_setting_ccci_drv_ver;
}

static int parse_k_setting(const char ref_data[], int raw_size, char name[], char val[], int size)
{
    int i, j = 0;
    char tmp_name[64] = {0};
    char tmp_val[64] = {0};
    int state = 0;
    int ret = 0;

    CCCI_LOGV("parse_info name:%s\n", name);

    for(i = 0; i < raw_size; i++) {
        if (j < 0 || j >= 64) {
            CCCI_LOGE("invalid name\n");
            return -1;
        }
        switch(state) {
        case 0: // Init
            if(ref_data[i] == '[') {
                j=0;
                state = 1;
            }
            break;

        case 1: // Read name
            if(ref_data[i] == ']') {
                state =2; // name parse done
                tmp_name[j] = '\0';
                j = 0;
            }
            else if((ref_data[i] == '\r')||(ref_data[i] == '\n')) {
                j = 0;
                state = 0;
            }
            else {
                tmp_name[j] = ref_data[i];
                j++;
            }
            break;

        case 2: // Get ':'
            if(ref_data[i] == ':') {
                state = 3; //divider found
                tmp_val[0] = 0;
            }
            else
                state = 0; // Invalid format
            break;

        case 3: // Read value
            if((ref_data[i] == '\r')||(ref_data[i] == '\n')) {
                state =4; // value parse done
                tmp_val[j]='\0';
                j = 0;
            }
            else {
                tmp_val[j] = ref_data[i];
                j++;
                break;
            }

        case 4: // Check if name is match
            if(strcmp(tmp_name, name)==0) {
                // Copy value
                ret = snprintf(val, size, "%s", tmp_val);
                if (ret < 0 || ret >= size) {
                    CCCI_LOGE("%s-%d:snprintf fail, ret = %d\n",
                              __func__, __LINE__, ret);
                    return -1;
                }
                return 0;
            }
            state = 0;
            break;
        default:
            break;
        }
    }
    return -1;
}

int kernel_setting_prepare(void)
{
    char *ref_data_buf;
    int ref_data_len;
    char tmp_buf[64];
    unsigned int i = 0,j = 0;
    int ret = 0;
    int fd = -1;

    ref_data_buf = (char *)malloc(KERNEL_SETTING_BUF_SIZE);

    if (ref_data_buf == NULL) {
        CCCI_LOGE("Allock kernel setting reference buffer fail, exit\n");
        return -1;
    }

    fd = open(KERNEL_SETTING_FILE, O_RDONLY);
    if (fd < 0) {
        CCCI_LOGE("Open ccci kernel setting file fail, exit\n");
        free(ref_data_buf);
        return -1;
    }

    ref_data_len = read(fd, ref_data_buf, KERNEL_SETTING_BUF_SIZE - 1);
    if(ref_data_len <= 0) {
        CCCI_LOGE("Read kernel setting info fail ret%d(%d)\n", ref_data_len, errno);
        ret = -1;
        goto _Exit;
    }
    ref_data_buf[ref_data_len - 1] = 0;

    if (parse_k_setting(ref_data_buf, ref_data_len, "modem en", tmp_buf, 64) < 0) {
        CCCI_LOGV("parsing modem en fail, exit\n");
        ret = -1;
        goto _Exit;
    }
    for (i = 0; i < 5; i++)
        k_setting_md_en[i] = 0;
    i = 0;
    while (1) {
        switch(tmp_buf[j]) {
        case '\0':
            break;
        case '-':
            break;
        case '1':
            k_setting_md_en[i] = 1;
            i++;
            break;
        case '0':
            i++;
            break;
        default:
            break;
        }
        j++;
        if (i >= 5 || j >= 64)
            break;
    }

    if (parse_k_setting(ref_data_buf, ref_data_len, "ccci_drv_ver", tmp_buf, 64) < 0) {
        CCCI_LOGV("parsing ccci_drv_ver fail, exit\n");
        ret = -1;
        goto _Exit;
    }
    if (strcmp("V2", tmp_buf) == 0)
        k_setting_ccci_drv_ver = 2; /* using v2 code */
    else
        k_setting_ccci_drv_ver = 1; /* using v1 code */

    if (parse_k_setting(ref_data_buf, ref_data_len, "MTK_MD_CAP", tmp_buf, 64) < 0) {
        CCCI_LOGV("parsing MTK_MD_CAP fail, exit\n");
    }

_Exit:
    close(fd);
    free(ref_data_buf);
    return ret;
}

int executeCmd(char * cmd, bool print)
{
    CCCI_LOGD("%s, cmd:%s\n", __FUNCTION__, cmd);
    FILE *fp = NULL;
    char buf[512] = {0};
    int status;
    fp = popen(cmd, "r");
    if (!fp) {
        CCCI_LOGD("popen  %s Fail!\n", cmd);
        return -1;
    }
    while (fgets(buf, sizeof(buf), fp)) {
        if (print)
        {
            if(fprintf(stdout, "%s", buf) < 0) {
               pclose(fp);
               return -1;
            }
        }
    }
    status = pclose(fp);
    return status;
}
