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
#include <stdlib.h>
#include <prop/properties.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <unistd.h>
/*
#ifndef MTK_TC1_FEATURE
#include <mtk_log.h>
#endif
*/   //wait for mtkLogI ready on alps-trunk-p0.bsp.tc1
#include "hardware/ccci_intf.h"
#include "../ccci_common.h"
#include "mdinit_relate.h"

//#include <aee.h>
#include "libnvram.h"
//#include "libnvram_log.h"
// for FSD wake_lock not released
//#include <hardware_legacy/power.h>
#include "../ccci_fsd/platform/power.h"

#define FSD_WAKE_UNLOCK() release_wake_lock("ccci_fsd")

#define CCCCI_LOG_TAG "ccci_mdinit"

//----------------maro define-----------------//
// For rild common
#define RILD_PROXY_NAME              "vendor.ril-proxy"
#define RILD_PROXY_SERVICE_STATUS    "init.svc.vendor.ril-proxy"
// For MD1
#define MUXD_FOR_MD1_NAME            "vendor.gsm0710muxd"
#define RILD_FOR_MD1_NAME            "vendor.ril-daemon-mtk"
#define MD_LOG_FOR_MD1_NAME          "mdlogger"
//#define MD_LOG_FOR_MD1_NAME_E      "ecccimdlogger"
#define MD_LOG_FOR_MD1_NAME_E        "emdlogger1"
#define FSD_FOR_MD1_NAME             "vendor.ccci_fsd"

// For MD2
#define MUXD_FOR_MD2_NAME            "vendor.gsm0710muxdmd2"
#define RILD_FOR_MD2_NAME            "vendor.ril-daemon-mtkmd2"
#define MD_LOG_FOR_MD2_NAME          "dualmdlogger"
#define MD_LOG_FOR_MD2_NAME_E        "emdlogger2"
#define FSD_FOR_MD2_NAME             "vendor.ccci2_fsd"

// For MD3
#define MUXD_FOR_MD3_NAME            "invalid"
#define RILD_FOR_MD3_NAME            "vendor.viarild"
#define MD_LOG_FOR_MD3_NAME          "dualmdlogger"
#define MD_LOG_FOR_MD3_NAME_E        "emdlogger3"
#define FSD_FOR_MD3_NAME             "vendor.ccci3_fsd"

// service start/stop wait time
#define MAX_WAIT_FOR_PROPERTY        6000  // wait 6s for service status changed
#define PROPERTY_WAIT_TIME           10    // 10 ms between polls

// Phase out
#define MUX_DAEMON_SINGLE            "gsm0710muxd-s"
#define RIL_DAEMON_SINGLE            "ril-daemon-s"

//property key max length
#define PROPERTY_KEY_MAX   64

static char muxd_name[PROPERTY_KEY_MAX];
static char rild_name[PROPERTY_KEY_MAX];

static int mux_rild_no_ctrl;
static int mdlogger_cnt = 0;
static char mdlogger_name[PROPERTY_KEY_MAX];
#define MDLOGGER_CAN_RESET           "vendor.debug.md.reset"

static char fsd_name[PROPERTY_KEY_MAX];

static const char *pre_service_status = "init.svc.";
static char muxd_service_status[PROPERTY_KEY_MAX];
static char rild_service_status[PROPERTY_KEY_MAX];
static char mdlogger_service_status[PROPERTY_KEY_MAX];
static char fsd_service_status[PROPERTY_KEY_MAX];

typedef struct _wait_prop_t
{
    char *prop_name;
    char *disr_value;
} wait_prop_t;

static int md_id = -1;
/* This varialbe will set to 1 when modem exception at boot ready state */
static int  need_silent_reboot = 0; 

extern int update_inf_to_bootprof(char str[]);

int get_rsc_protol_value(int md_id, char *buf, int len)
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int retpropget = 0;

    if (md_id == MD_SYS1)
        retpropget = property_get("ro.vendor.mtk_protocol1_rat_config",
                                  value, NULL);

    if (retpropget)
        snprintf(buf, len, "%s", value);

    CCCI_LOGD("[get_rsc_protol_value] md_id: %d; value: %s\n", md_id, value);

    return retpropget;
}

// return 1: ready,  0, not ready
static int wait_property_ready(wait_prop_t *wp, const int count, const int waitmsec)
{
    int i;
    int ready = 0;
    int watiflag = 0;
    int bitflag = 0;
    int maxtimes = waitmsec / PROPERTY_WAIT_TIME;
    int needtry = 1;
    wait_prop_t *curwt = NULL;
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int needgetprop = 0;

    if (count > (int)(sizeof(watiflag) * 8)) {
        CCCI_LOGI("%s: count > %lu, return 0\n", __func__, (8 * sizeof(int)));
        return ready;
    }

    if (maxtimes < 1)
        maxtimes = 1;

    CCCI_LOGI("%s: count:%d, waitmsec:%d, loop:%d\n", __func__, count, waitmsec, maxtimes);
    while ((maxtimes-- > 0) && needtry) {
        curwt = wp;
        needtry = 0;

        //CCCI_LOGI("%s watiflag:0x%x\n", __func__, watiflag);
        for (i = 0; i < count; i++) {
            bitflag = (1 << i);

            needgetprop = (!(watiflag & bitflag));
            if (needgetprop) {
                if (wait_for_property(curwt->prop_name, curwt->disr_value, 0) >= 0)
                    watiflag |= bitflag;
                else if (needtry == 0) 
                    needtry = 1;
            }

            if (needgetprop && (maxtimes % 10 == 0)) {
                CCCI_LOGI("%s: retry name:%s, disrvalue:%s, loop:%d\n",
                          __func__, curwt->prop_name, curwt->disr_value, maxtimes);
            }
            curwt++;
        }

        // if need to retry, sleep about 100ms
        if (needtry)
            usleep(PROPERTY_WAIT_TIME * 1000);
    }

    if (!needtry) {
        CCCI_LOGI("%s:Succeeded! count:%d, waitmsec:%d, loop:%d\n",
                  __func__, count, waitmsec, maxtimes);
        ready = 1;
    }
    else {
        CCCI_LOGI("%s:Failed! count:%d, waitmsec:%d, loop:%d\n",
                  __func__, count, waitmsec, maxtimes);
    }

    return ready;
}

int start_service_verified(const char *service_name, const char *service_status_name, int waitmsec)
{
    int succeeded = -1;
    char value[PROPERTY_VALUE_MAX] = {'\0'};

    succeeded = property_get(service_status_name, value, NULL);
    if (succeeded > 0)
        CCCI_LOGI("start_service %s, current state:%s, returned:%d\n",
                  service_status_name, value, succeeded);
    else if (succeeded == 0)
        CCCI_LOGI("start_service %s, but returned 0, maybe has no this property\n",
                  service_status_name);
    else
        CCCI_LOGI("start_service %s, returned:%d\n", service_status_name, succeeded);
    property_set("ctl.start", service_name);
    succeeded = wait_for_property(service_status_name, "running", waitmsec);
    return succeeded;
}

int stop_service_verified(const char *service_name, const char *service_status_name, int waitmsec)
{
    int succeeded = -1;

    property_set("ctl.stop", service_name);
    succeeded = wait_for_property(service_status_name, "stopped", waitmsec);
    return succeeded;
}

void stop_all_ccci_up_layer_services(void)
{
    int retry = 0;
    char buf[PROPERTY_VALUE_MAX];
    wait_prop_t wp[3];
    unsigned int wpcount = 0;
    int stopped = 0;
    int meta_mode = is_meta_mode();

    CCCI_LOGD("stop all up layer service, meta_mode: %d\n", meta_mode);

    //stop_service_verified(fsd_name, fsd_service_status, MAX_WAIT_FOR_PROPERTY);
    if (executeCmd("systemctl stop tele-fwk.service" , 0) == -1) {
        CCCI_LOGD("stop tele-fwk.service fail\n");
        stopped = 0;
    } else {
        CCCI_LOGD("stop tele-fwk.service succeeded \n");
        stopped = stopped + 1;
    }

    if (executeCmd("systemctl stop ccci_fsd.service" , 0) == -1) {
        CCCI_LOGD("stop ccci_fsd.service fail\n");
        stopped = 0;
    } else {
        CCCI_LOGD("stop ccci_fsd.service succeeded \n");
        stopped = stopped + 1;
    }

    FSD_WAKE_UNLOCK();

    if (meta_mode)
        return;

    if (!mux_rild_no_ctrl && !is_factory_mode()) {
        //stop_service_verified(rild_name, rild_service_status, MAX_WAIT_FOR_PROPERTY);
        if (executeCmd("systemctl stop tele-fwk.service" , 0) == -1) {
            CCCI_LOGD("stop tele-fwk.service fail\n");
            stopped = 0;
        } else {
            CCCI_LOGD("stop tele-fwk.service succeeded \n");
            stopped = stopped + 1;
        }

        if (executeCmd("systemctl stop mtkfusionrild.service" , 0) == -1) {
            CCCI_LOGD("stop mtkfusionrild.service fail\n");
            stopped = 0;
        } else {
            CCCI_LOGD("stop mtkfusionrild.service succeeded \n");
            stopped = stopped + 1;
        }

        // if(md_id != MD_SYS3) {
        //     //property_set("ctl.stop", muxd_name);
        //     //wp[wpcount].prop_name = muxd_service_status;
        //     //wp[wpcount].disr_value ="stopped";
        //     //wpcount++;
        //     if (executeCmd("systemctl stop gsm0710muxd.service" , 0) == -1) {
        //         CCCI_LOGD("stop gsm0710muxd.service fail\n");
        //         stopped = 0;
        //     } else {
        //         CCCI_LOGD("stop gsm0710muxd.service succeeded \n");
        //         stopped = stopped + 1;
        //     }

        // }
    }
    if(mdlogger_cnt) {
        while(retry < 6) {
            retry++;
            property_get(MDLOGGER_CAN_RESET, buf, "0");
            if (0 != strcmp(buf, "0"))
                break;
            usleep(50 * 1000);
        }
        if (retry >= 6)
            CCCI_LOGD("MDlogger not set %s\n",MDLOGGER_CAN_RESET);
        //property_set("ctl.stop", mdlogger_name);
        //wp[wpcount].prop_name = mdlogger_service_status;
        //wp[wpcount].disr_value ="stopped";
        //wpcount++;
        if (executeCmd("systemctl stop emdlogger1.service" , 0) == -1) {
            CCCI_LOGD("stop emdlogger1.service fail\n");
            stopped = 0;
        } else {
            CCCI_LOGD("stop emdlogger1.service succeeded \n");
            stopped = stopped + 1;
        }

        mdlogger_cnt = 0;
    }

    //stopped = wait_property_ready(wp, wpcount, MAX_WAIT_FOR_PROPERTY);
    if (stopped > 0)
        CCCI_LOGD("stop all up layer service succeeded!\n");
    else
        CCCI_LOGE("stop all up layer service failed\n");
}

void start_all_ccci_up_layer_services(void)
{
    CCCI_LOGD("start all ccci up layer services\n");

    if (need_silent_reboot) {
        CCCI_LOGD("set vendor.ril.mux.report.case 2\n");
        property_set("vendor.ril.mux.report.case", "2"); /* set mux flag here, should before muxd */
    }

    if(mdlogger_cnt == 0) {
        // CCCI_LOGD("start mdlogger\n");
        //start_service_verified(mdlogger_name, mdlogger_service_status, 0);
        if (executeCmd("systemctl start emdlogger1.service" , 0) == -1) {
            CCCI_LOGD("start emdlogger1.service fail\n");
        } else {
            CCCI_LOGD("start emdlogger1.service succeeded \n");
        }
        mdlogger_cnt = 1;
    }

    //update_service_name();
    if (!mux_rild_no_ctrl && !is_factory_mode()) {
        if(md_id != MD_SYS3) {
            //start_service_verified(muxd_name, muxd_service_status, MAX_WAIT_FOR_PROPERTY);
            if (executeCmd("systemctl start mtkfusionrild.service" , 0) == -1) {
                CCCI_LOGD("start mtkfusionrild.service fail\n");
            } else {
                CCCI_LOGD("start mtkfusionrild.service succeeded \n");
            }
        }
        else {
            // CCCI_LOGD("start c2k rild\n");
            //start_service_verified(rild_name, rild_service_status, MAX_WAIT_FOR_PROPERTY);
        }
    }
}

int rild_generation(void)
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int retpropget = 0;
    int ril_gen = 0;

    retpropget = property_get("ro.vendor.mtk_ril_mode", value, "legacy chip");
    if ((retpropget > 0) && (strcmp(value, "c6m_1rild") == 0))
        ril_gen = 1;

    CCCI_LOGI("rild_generation value: %s [%d]\n", value, ril_gen);
    return ril_gen;
}

/* Wait decrypt and NVRAM */
#define MAX_NVRAM_RETRY_TIMES       240
#define MAX_CRYPT_RETRY_TIMES       120

/*return: 0: succeeded,
 *return: 1: maybe has no this property, negative:failed
 */
int wait_for_property(const char *name, const char *desired_value, int waitmsec)
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int maxtimes = waitmsec / PROPERTY_WAIT_TIME;
    int needretry = 1;
    int retpropget = 0;
    int is_stop_wait = 0;

    if (desired_value == NULL) {
        CCCI_LOGI("%s:%d : desired_value is null\n", __func__, __LINE__);
        return 0;
    }

    if (strcmp(desired_value, "stopped") == 0)
        is_stop_wait = 1;

    CCCI_LOGI("%s: is_stop_wait: %d\n", __func__, is_stop_wait);

    do {
        retpropget = property_get(name, value, NULL);
        if (retpropget > 0) {
            if (desired_value == NULL || strcmp(value, desired_value) == 0) {
                CCCI_LOGI("%s:success(%s=%s), loop:%d\n", __func__, name, desired_value, maxtimes);
                return 0;
            }
        }
        else if (retpropget == 0) { /* has no this property, needn't try again */
            if (is_stop_wait) {
                CCCI_LOGI("%s: no property of :%s, loop:%d\n", __func__, name, maxtimes);
                return 1; // when phone boot up, service's property is just not created yet, no need to stop
            }
        }
        else {
            CCCI_LOGI("%s: error returned:%d, errno:%d\n", __func__, retpropget, errno);
        }
        usleep(PROPERTY_WAIT_TIME * 1000);
        maxtimes--;
    } while (maxtimes > 0);
    if (waitmsec > 0)
        CCCI_LOGE("%s:failed, name:%s, desired_value:%s, waitmsec:%d, value:%s\n",
                  __func__, name, desired_value, waitmsec, value);

    return -1;
}

/* Wait_level
 * 1: only check whether service.nvram_init = "Ready"
 * 2: check service.nvram_init is both "Ready" and "Pre_ready
 */
static int wait_nvram_ready(int wait_level)
{
    int retry = 0;
    int ret = -1;
    char property_val[PROPERTY_VALUE_MAX] = {0};
    char mod_name[32];
    int val = 0;

    CCCI_LOGD("%s begin\n", __func__);
    while(1) {
        property_get("vendor.service.nvram_restore", property_val, NULL);
        if(wait_level == 1 || wait_level == 2) {
            if(strcmp(property_val, "Ready") == 0) {
                ret = 0;
                break;
            }
        }
        if (wait_level == 2) {
            if (strcmp(property_val, "Pre_Ready") == 0) {
                ret = 0;
                break;
            }
        }
        retry++;
        if ((retry % MAX_NVRAM_RETRY_TIMES) == 0) {
            ret = -1;
            CCCI_LOGD("wait service.nvram_init=%s... timeout\n", property_val);
            break;
        }
        usleep(500 * 1000);
    }
    /* when factory restore runs on emmc, modem start to run before nvram
     * restore complete,and then modem exception happens, so need wait for
     * nvram is ready
     */
    CCCI_LOGI("Gotten ret=%d,nvram_init=%s!\n", ret, property_val);
    if (ret < 0) {
        CCCI_LOGE("Get nvram restore ready fail! Warning for nvram!\n");
        if (md_id == MD_SYS1)
            val = snprintf(mod_name, 32, "md1");
        else if (md_id == MD_SYS2)
            val = snprintf(mod_name, 32, "md2");
        else if (md_id == MD_SYS3)
            val = snprintf(mod_name, 32, "md3");
        else
            val = snprintf(mod_name, 32, "mdx");
        if (val < 0 || val >= 32)
            CCCI_LOGD("%s-%d:snprintf fail,val = %d\n", __func__, __LINE__, val);
        show_aee_system_exception("ccci_mdinit", mod_name, DB_OPT_FTRACE,
                                  "Wait service.nvram_init ready timeout, please ask nvram owner to check!");
        exit(-0xF1);
    }
    return ret;
}

static int wait_decrypt_done(void)
{
    int retry = 0;
    char crypto_state[PROPERTY_VALUE_MAX];
    char decrypt_state[PROPERTY_VALUE_MAX] = {0};
    bool isEncryptWithPasswd = false;
    bool toCheckIsEncryptWithPasswd = true;
    long becomeEncryptedTimestamp = 0;
    struct sysinfo info; /* #include <sys/sysinfo.h> */
    info.uptime = 0;

    /* AOSP adds a new encrypting flow, encrypting with password:
     *     1. The device doesn't enable "default encryption"
     *     2. Set the screen lock with pin/password/pattern
     *     3. Go to Settings and try to encrypt the device manually
     *     4. The device will reboot
     *     5. Vold encrypts the device
     *     6. After finish encrypting, the device will show the UI to let the end use decrypt the device
     *        (The property, vold.decrypt, is 'trigger_restart_min_framework')
     */

    CCCI_LOGD("waiting vold.decrypt=trigger_restart_framework or trigger_restart_min_framework");
    property_get("ro.crypto.state", crypto_state, "");
    property_get("vold.decrypt", decrypt_state, NULL);
    while (true) {
        if (!strcmp(crypto_state, "encrypted")) {
            if (becomeEncryptedTimestamp == 0) {
                sysinfo(&info);
                becomeEncryptedTimestamp = info.uptime;
            }
            if (toCheckIsEncryptWithPasswd == true && isEncryptWithPasswd == false) {
                sysinfo(&info);
                if ( info.uptime - becomeEncryptedTimestamp > 10 ) {
                    if (!strcmp(decrypt_state, "trigger_restart_min_framework")) {
                        isEncryptWithPasswd = true;
                        CCCI_LOGD("Password is NOT default because vold.decrypt is still trigger_restart_min_framework\n");
                    }
                    toCheckIsEncryptWithPasswd = false;
                }
            }

            if(isEncryptWithPasswd) { /* Password is NOT default */
                if (!strcmp(decrypt_state, "trigger_restart_min_framework")) {
                    break;
                }
            }
            else { /* Password is default */
                if (!strcmp(decrypt_state, "trigger_restart_framework")) {
                    break;
                }
            }
        }
        else if (!strcmp(crypto_state, "unencrypted")) { /* fake encrypting in meta/factory mode */
            break;
        }

        retry++;
        if ((retry % MAX_CRYPT_RETRY_TIMES) == 0)
            CCCI_LOGD("wait vold.decrypt...,%s\n", decrypt_state);
        usleep(500 * 1000);
        property_get("ro.crypto.state", crypto_state, "");
        property_get("vold.decrypt", decrypt_state, NULL);
    }
    CCCI_LOGD("wait vold.decrypt=%s done success!\n", decrypt_state);
    return 0;
}



/*
 *Return value:
 * 0: unencrypted, unsupported
 * 1: auto encrypt and decrpty on first boot
 * 2: vold trigger_restart_framework after decrpty, fsis norma
 * 3: vold trigger_restart_min_framework and wait decrpty, fs is tmpfs
 *   error value: <0
 */
int check_decrypt_ready(void)
{
    int ret;
    char property_val[PROPERTY_VALUE_MAX] = {0};
    // Check whether is at decrypt state
    property_get("ro.crypto.state", property_val, NULL);
    CCCI_LOGD("ro.crypto.state=%s\n",property_val);
    if (strcmp(property_val, "") == 0) {
        CCCI_LOGD("auto encrypt & decrypt\n");
        wait_decrypt_done();
        wait_nvram_ready(2);
        return 1;
    }
    else if (strcmp(property_val, "unencrypted") == 0) {
        wait_nvram_ready(1);
        CCCI_LOGD("unencrypted!!\n");
        return 0;
    }
    else if (strcmp(property_val, "unsupported") == 0) {
        wait_nvram_ready(1);
        CCCI_LOGD("unsupported!!\n");
        return 0;
    }
    else if (strcmp(property_val, "encrypted") == 0) {
        property_get("ro.crypto.type", property_val, NULL);
        if (strcmp(property_val, "file") == 0) {
            wait_nvram_ready(1);
            CCCI_LOGD("file/FBE!!\n");
            return 0;
        }
        while(1) {
            property_get("vold.decrypt", property_val, NULL);
            if (strcmp(property_val, "trigger_restart_framework") == 0) {
                CCCI_LOGD("vold.decrypt:trigger_restart_framework\n");
                wait_nvram_ready(1);
                return 2;
            }
            else if (strcmp(property_val, "trigger_restart_min_framework") == 0) {
                CCCI_LOGD("vold.decrypt:trigger_restart_min_framework!!\n");
                wait_nvram_ready(2);
                return 3;
            }
            usleep(100 * 1000);
        }
    }
    else {
        CCCI_LOGE("crypto state error %s!!\n", property_val);
        ret = -1;
    }
    return ret;
}

int get_mdini_killed_state(unsigned int monitor_fd)
{
    int ret;
    unsigned int mdinit_killed = 0;

    ret = ioctl(monitor_fd, CCCI_IOC_GET_MDINIT_KILLED, &mdinit_killed);
    if(ret) {
        if (ret == -ENOTTY)
            CCCI_LOGE("[%s] not support: CCCI_IOC_GET_MDINIT_KILLED\n",
                      __func__);
        else
            CCCI_LOGE("[%s] error: get mdinit killed: %d(%d)\n",
                      __func__, errno, ret);

        return 0;
    }

    return (int)mdinit_killed;
}

void update_vendor_ril_active_md(unsigned int monitor_fd)
{
    int ret, val;
    char md_type_str[16];
    int curr_md_type;

    ret = ioctl(monitor_fd, CCCI_IOC_GET_MD_TYPE, &curr_md_type);
    if(0 == ret) {
        val = snprintf(md_type_str, 16, "%d", curr_md_type);
        if (val < 0 || val >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail,val = %d\n",
                      __func__, __LINE__, val);
            return;
        }
        char* key = "vendor.ril.active.md";
        CCCI_LOGE("[%s] only set %s=%d yocto local", __func__, key, curr_md_type);
        int ret = property_set("vendor.ril.active.md", md_type_str);
        CCCI_LOGE("[%s] only set %s=%d yocto local(%s) ", __func__, key, curr_md_type, (ret < 0 ? "failed" : "successed"));
    }
    else
        CCCI_LOGD("[Active MD]get md type fail: %d(%d)\n", errno, ret);
}

void check_to_restart_md_v2(unsigned int monitor_fd, int first_boot)
{
    char this_md_status_key[32] = {0};
    char the_other_md_status_key[32] = {0};
    char buf[PROPERTY_VALUE_MAX];
    int ret;
    int cnt=0;
    int ccif_on = 1;
    int mdinit_killed;
    int val = 0;

    if(md_id == MD_SYS1) {
        val = snprintf(this_md_status_key, 32,
                       "%s", "vendor.mtk.md1.status");
        if (val < 0 || val >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail,val = %d\n",
                      __func__, __LINE__, val);
            return;
        }
        val = snprintf(the_other_md_status_key, 32,
                       "%s", "vendor.mtk.md3.status");
        if (val < 0 || val >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail,val = %d\n",
                      __func__, __LINE__, val);
            return;
        }
    }
    else if (md_id == MD_SYS3) {
        val = snprintf(this_md_status_key, 32,
                       "%s", "vendor.mtk.md3.status");
        if (val < 0 || val >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail,val = %d\n",
                      __func__, __LINE__, val);
            return;
        }
        val = snprintf(the_other_md_status_key, 32,
                       "%s", "vendor.mtk.md1.status");
        if (val < 0 || val >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail,val = %d\n",
                      __func__, __LINE__, val);
            return;
        }
    }

    mdinit_killed = get_mdini_killed_state(monitor_fd);
    CCCI_LOGD("mdinit_killed: %d; first_boot: %d\n", mdinit_killed, first_boot);

    if (!first_boot || mdinit_killed) {
        do {
            ret = property_get(the_other_md_status_key, buf, NULL);
            if (ret == 0) {
                if (md_id == MD_SYS1 && !is_factory_mode()) {
                    if (rild_generation() == 0) { /* c2k in one case */
                        if (!mux_rild_no_ctrl)
                            stop_service_verified(RILD_PROXY_NAME, RILD_PROXY_SERVICE_STATUS,
                                                  MAX_WAIT_FOR_PROPERTY);
                        CCCI_LOGD("check_to_restart_md kill ril-proxy, md1 only\n");
                    }
                    else
                        CCCI_LOGD("ignore kill ril-proxy, fusion\n");
                }
                CCCI_LOGI("check_to_restart_md:%s not exist\n", the_other_md_status_key);
                goto OTHER_CHECK;
            }
            if (strcmp(buf, "reset") == 0) {
                if (md_id == MD_SYS3) {
                    if (0 != ioctl(monitor_fd, CCCI_IOC_RESET_MD1_MD3_PCCIF, NULL)) {
                        CCCI_LOGE("reset pccif fail\n");
                    }
                    CCCI_LOGD("reset pccif/ccirq done\n");
                }
                property_set(the_other_md_status_key, "reset_start");
                wait_for_property(the_other_md_status_key, "reset_start", MAX_WAIT_FOR_PROPERTY);
                CCCI_LOGI("check_to_restart_md:set %s reset_start done\n", the_other_md_status_key);
                break;
            }
            cnt++;
            if ((cnt % 100) == 0)
                CCCI_LOGI("check_to_restart_md:waiting %s=%s...\n", the_other_md_status_key, buf);
            usleep(30 * 1000);
        } while(1);
        CCCI_LOGI("md%d check_to_restart_md:waiting %s done\n", md_id+1, the_other_md_status_key);

        do {
            ret = property_get(this_md_status_key, buf, NULL);
            if (ret == 0) {
                CCCI_LOGI("check_to_restart_md:%s not exist\n", this_md_status_key);
                goto OTHER_CHECK;
            }
            if (strcmp(buf,"reset_start") == 0) {
                CCCI_LOGI("check_to_restart_md: %s reset_start\n", this_md_status_key);
                break;
            }
            cnt++;
            if ((cnt % 100) == 0)
                CCCI_LOGI("check_to_restart_md:waiting %s =%s...\n", this_md_status_key, buf);
            usleep(30 * 1000);
        } while(1);

        if (md_id == MD_SYS1 && !is_factory_mode()) {
            if (rild_generation() == 0) { /* c2k in one case */
                if (!mux_rild_no_ctrl)
                    stop_service_verified(RILD_PROXY_NAME, RILD_PROXY_SERVICE_STATUS,
                                          MAX_WAIT_FOR_PROPERTY);
                CCCI_LOGD("check_to_restart_md kill ril-proxy\n");
            }
            else
                CCCI_LOGD("ignore kill ril-proxy, fusion\n");
        }
    }
OTHER_CHECK:
    if (first_boot)
        ccci_ccb_init_users(md_id);
}

void mux_rild_ctrl_method_init(void)
{
    int retpropget = 0;
    char value[PROPERTY_VALUE_MAX] = {'\0'};

    retpropget = property_get("ro.vendor.mtk_mipc_support", value, NULL);
    if (retpropget > 0) {
        if (strcmp(value, "1") == 0) {
            mux_rild_no_ctrl = 1;
            CCCI_LOGW("mux_rild_no_ctrl == 1\n");
        }
    }
    else
        CCCI_LOGE("get property fail: ro.vendor.mtk_mipc_support\n");
}

void md_init_related_init(int md_id)
{
    int ret = 0;

    mux_rild_ctrl_method_init();

    switch(md_id) {
    case 0:
        if (!mux_rild_no_ctrl) {
            ret = snprintf(muxd_name, 32, "%s", MUXD_FOR_MD1_NAME);
            if (ret < 0 || ret >= 32) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
            ret = snprintf(rild_name, 32, "%s", RILD_FOR_MD1_NAME);
            if (ret < 0 || ret >= 32) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
        }
        ret = snprintf(mdlogger_name, 32, "%s", MD_LOG_FOR_MD1_NAME_E);
        if (ret < 0 || ret >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        ret = snprintf(fsd_name, 32, "%s", FSD_FOR_MD1_NAME);
        if (ret < 0 || ret >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        break;
    case 1:
        if (!mux_rild_no_ctrl) {
            ret = snprintf(muxd_name, 32, "%s", MUXD_FOR_MD2_NAME);
            if (ret < 0 || ret >= 32) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
            ret = snprintf(rild_name, 32, "%s", RILD_FOR_MD2_NAME);
            if (ret < 0 || ret >= 32) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
        }
        ret = snprintf(mdlogger_name, 32, "%s", MD_LOG_FOR_MD2_NAME_E);
        if (ret < 0 || ret >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        ret = snprintf(fsd_name, 32, "%s", FSD_FOR_MD2_NAME);
        if (ret < 0 || ret >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        break;
    case 2:
        if (!mux_rild_no_ctrl) {
            ret = snprintf(muxd_name, 32, "%s", MUXD_FOR_MD3_NAME);
            if (ret < 0 || ret >= 32) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
            ret = snprintf(rild_name, 32, "%s", RILD_FOR_MD3_NAME);
            if (ret < 0 || ret >= 32) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
        }
        ret = snprintf(mdlogger_name, 32, "%s", MD_LOG_FOR_MD3_NAME_E);
        if (ret < 0 || ret >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        ret = snprintf(fsd_name, 32, "%s", FSD_FOR_MD3_NAME);
        if (ret < 0 || ret >= 32) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        break;
    default:
        CCCI_LOGE("[Error]Invalid md sys id: %d\n", md_id+1);
        return;
    }

    if (!mux_rild_no_ctrl) {
        ret = snprintf(muxd_service_status, PROPERTY_KEY_MAX, "%s%s",
                       pre_service_status, muxd_name);
        if (ret < 0 || ret >= PROPERTY_KEY_MAX) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
        ret = snprintf(rild_service_status, PROPERTY_KEY_MAX, "%s%s",
                       pre_service_status, rild_name);
        if (ret < 0 || ret >= PROPERTY_KEY_MAX) {
            CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
            return;
        }
    }
    ret = snprintf(mdlogger_service_status, PROPERTY_KEY_MAX, "%s%s",
                   pre_service_status, mdlogger_name);
    if (ret < 0 || ret >= PROPERTY_KEY_MAX) {
        CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
        return;
    }
    ret = snprintf(fsd_service_status, PROPERTY_KEY_MAX, "%s%s", pre_service_status, fsd_name);
    if (ret < 0 || ret >= PROPERTY_KEY_MAX) {
        CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
        return;
    }
    if (mux_rild_no_ctrl)
        CCCI_LOGI("service names: [%s][%s]\n",
                  mdlogger_service_status,
                  fsd_service_status);
    else
        CCCI_LOGI("service names: [%s][%s][%s][%s]\n",
                  muxd_service_status,
                  rild_service_status,
                  mdlogger_service_status,
                  fsd_service_status);
}

enum {
    CCCI_MD_STA_INIT = -1,
    CCCI_MD_STA_BOOT_READY = 0,
    CCCI_MD_STA_BOOT_UP,
    CCCI_MD_STA_RESET,
    CCCI_MD_STA_STOP,
    CCCI_MD_STA_FLIGHT_MODE,
    CCCI_MD_STA_EXCEPTION,
};

void notify_md_status(int status, int flight_mode, char *buf)
{
    char property_name[50];
    int ret = 0;
    int current_boot_mode = MD_BOOT_MODE_INVALID;

    CCCI_LOGI("MD%d set status: mtk.md%d.status=%s\n", md_id+1, md_id+1, buf); /* #include "libnvram.h" */

    switch(status) {
    case CCCI_MD_STA_INIT:
        md_id = curr_md_id - 1;
        md_init_related_init(md_id);
        update_inf_to_bootprof("decpyt ready");
        break;
    case CCCI_MD_STA_BOOT_READY:
        ccci_ccb_check_users(md_id); // must before we start MDlogger
        if (!is_meta_mode()) {
            if (ioctl(system_ch_handle, CCCI_IOC_GET_MD_BOOT_MODE, &current_boot_mode) == 0) {
                if (current_boot_mode != MD_BOOT_MODE_META)
                    start_all_ccci_up_layer_services();
                else
                    CCCI_LOGD("boot MD into META mode when system is in normal mode\n");
            }
            else {
                start_all_ccci_up_layer_services();
            }
        }
        need_silent_reboot = 0;
        break;
    case CCCI_MD_STA_BOOT_UP:
        break;
    case CCCI_MD_STA_RESET:
        if (flight_mode) {
            wait_for_property("vendor.ril.getccci.response", "1", 500);
            stop_all_ccci_up_layer_services();
        }
        break;
    case CCCI_MD_STA_STOP:
        break;
    case CCCI_MD_STA_FLIGHT_MODE:
        break;
    case CCCI_MD_STA_EXCEPTION:
        if(mdlogger_cnt == 0) {
            CCCI_LOGD("start mdlogger when MD exception happens early\n");

            //start_service_verified(mdlogger_name, mdlogger_service_status, MAX_WAIT_FOR_PROPERTY);
            if (executeCmd("systemctl start emdlogger1.service" , 0) == -1) {
                CCCI_LOGD("start emdlogger1.service fail\n");
            } else {
                CCCI_LOGD("start emdlogger1.service succeeded \n");
            }
            mdlogger_cnt = 1;
        }
        need_silent_reboot = 1;
        if(md_id == MD_SYS3) {
            /*c2krild will use this "exception" state to decide how to triger NE*/
            ret = snprintf(property_name, sizeof(property_name), "vendor.net.cdma.mdmstat");
            if (ret < 0 || ret >= sizeof(property_name)) {
                CCCI_LOGD("%s-%d:snprintf fail.ret = %d\n", __func__, __LINE__, ret);
                return;
            }
            CCCI_LOGD("set md3 status:%s=exception\n",property_name);
            property_set(property_name,"exception");
        }
        break;
    default:
        break;
    }
}

