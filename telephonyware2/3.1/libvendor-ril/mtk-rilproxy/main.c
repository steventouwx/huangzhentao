// SPDX-License-Identifier: MediaTekProprietary
/* //device/system/rild/rild.c
**
** Copyright 2006 The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <android/binder_process.h>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <prop/properties.h>
#include <../include/telephony/ril.h>

#include "Rfx.h"
#include "socket_ext_api.h"

#include <mtk_log.h>

#define LOG_TAG "MtkTelephonyService"
#define TELEPHONYSERVICE_VERSION "yocto-v1.0-2024/12/18"

#ifdef TELE_FWK

RilLogTagInfo teleLogTagInfo[] = {
    {"RfxMainThread", "persist.log.tag.RfxMainThread", LEVEL_INFO},
};

int main (int argc, char **argv)
{
 //   const char *rilLibPath = "libvendor-ril.so";
    char tel_dbg[PROPERTY_VALUE_MAX] = {0};
    setTagGroup(teleLogTagInfo, sizeof(teleLogTagInfo)/sizeof(teleLogTagInfo[0]));
    property_get("persist.vendor.log.dis_tel_dbg", tel_dbg, "0");
    int teleloglevel = atoi(tel_dbg);
    if(teleloglevel == 1) {
        setTeleLogLevel(LEVEL_INFO);
    }else {
        setTeleLogLevel(LEVEL_DEBUG);
    }

    char tag_dbg[PROPERTY_VALUE_MAX] = {0};
    property_get("persist.vendor.log.tel_tag_dbg", tag_dbg, "0");
    int taglevel = atoi(tag_dbg);
    if(taglevel == 1) {
        updateAllTagLogLevel(LEVEL_DEBUG);
    }else {
        updateAllTagLogLevel(LEVEL_INFO);
    }

    property_watch_async("persist.vendor.log.tel_tag_dbg" , setTagLogLevel);
    property_watch_async("persist.log.tag.RpDataController" , setTagLogLevel);

    RLOGD("**MtkTelephonyService(%s) Daemon Started**", TELEPHONYSERVICE_VERSION);

    ABinderProcess_startThreadPool();
    //ABinderProcess_setThreadPoolMaxThreadCount(4);

    rfx_init();
    RLOGD("RIL_Init completed");
    rfx_check_table_index();
    RLOGD("rfx_check_table_index completed");
    ril_socket_init();
    RLOGD("ril_socket_init completed");
    ril_aidl_thread_pool();
    RLOGD("ril_aidl_thread_pool completed!");;
    ABinderProcess_joinThreadPool();
    RLOGD("Data FWK launch done!");;
    while (1) {
        sleep(UINT32_MAX);
    }
    return 0;

}

#endif

