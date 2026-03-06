/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ccci_lib.c
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 ****************************************************************************/

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
#include <prop/properties.h>
//#include <android/log.h>
//#include <sys/ioctl.h>
//#include <errno.h>
#include <stdlib.h>
//#include <sys/mman.h>
//#include <stdint.h>
//#include <pthread.h>
//#include "hardware/ccci_intf.h"
#include "ccci_lib_platform.h"


#define AB_PROPERTY_NAME        "ro.boot.slot_suffix"
#define LIB_SYSENV_32_PATH      "/usr/lib/libsysenv.so"
#define LIB_SYSENV_64_PATH      "/usr/lib64/libsysenv.so"
#define SLOT_SUFFIX_KEY         "slot_suffix"
#define PARTITION_SUFFIX_LEN    10
typedef const char *(*SYSENV_GET)(const char *);
typedef const char *(*SYSENV_CLEANUP)();

void *my_dlopen() {
  void *handle = NULL;
  handle = dlopen(LIB_SYSENV_64_PATH, RTLD_LAZY);
  if (!handle) {
    handle = dlopen(LIB_SYSENV_32_PATH, RTLD_LAZY);
    if (!handle) {
      return NULL;
    }
    dlerror();
  }
  return handle;
}

void sysenv_get_slot_suffix(char *buf)
{
    void *handle = NULL;
    char *error = NULL;
    SYSENV_GET get_func = NULL;
    SYSENV_CLEANUP sysenv_clean = NULL;
    const char * constValue = NULL;

    dlerror();
    handle = my_dlopen();
    if (!handle) {
        return;
    }

    get_func = (SYSENV_GET)dlsym(handle, "sysenv_get");
    sysenv_clean = (SYSENV_GET)dlsym(handle, "sysenv_cleanup");
    if ((error = dlerror()) != NULL)  {
        dlclose(handle);
        handle = NULL;
        return;
    }

    if (get_func != NULL) {
        constValue = get_func(SLOT_SUFFIX_KEY);
    }

    if (constValue == NULL) {
        dlclose(handle);
        handle = NULL;
        return;
    }
    strncpy(buf, constValue, PARTITION_SUFFIX_LEN-1);

    sysenv_clean();
    dlclose(handle);
    handle = NULL;
}

void AB_image_get(char *buf)
{
    //if (property_get(AB_PROPERTY_NAME, buf, NULL) == 0)
    //    buf[0] = 0;
    sysenv_get_slot_suffix(buf);
}

int query_prj_cfg_setting_platform(__attribute__((unused))char name[],
    __attribute__((unused))char val[], __attribute__((unused))int size)
{
    __attribute__((unused)) int ret;

    //-- For MTK_ECCCI_C2K
    #ifdef MTK_ECCCI_C2K
    if(strcmp(name, "MTK_ECCCI_C2K")==0) {
        ret = snprintf(val, size, "1");
        if (ret < 0 || ret >= size)
            return -1;
        return 0;
    }
    #endif

    #ifdef MD_SBP_CUSTOM_VALUE
    if(strcmp(name, "MTK_MD_SBP_CUSTOM_VALUE")==0) {
        ret = snprintf(val, size, "%d", MD_SBP_CUSTOM_VALUE);
        if (ret < 0 || ret >= size)
            return -1;
        return 0;
    }
    #endif

    #ifdef MD2_SBP_CUSTOM_VALUE
    if(strcmp(name, "MTK_MD2_SBP_CUSTOM_VALUE")==0) {
        ret = snprintf(val, size, "%d", MD2_SBP_CUSTOM_VALUE);
        if (ret < 0 || ret >= size)
            return -1;
        return 0;
    }
    #endif

    return -1;
}

