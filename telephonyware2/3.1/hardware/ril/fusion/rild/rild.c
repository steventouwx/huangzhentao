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

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <telephony/mtk_ril.h>
#include <cutils/sockets.h>
#ifdef __ANDROID__
    #include <sys/capability.h>
    #include <cutils/android_filesystem_config.h>
#else
    #include <linux/capability.h>
#endif

#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <librilmtk/ril_ex.h>
//#include <cutils/android_filesystem_config.h>
#include <mtk_log.h>
#include <mtk_properties.h>
#include <prop/properties.h>
#include <mtkconfigutils.h>
#include <libmtkrilutils.h>
#include <execinfo.h>
#include "mipc_api.h"
#include "mipc_msg.h"
#include "mipc_msg_host.h"
#include <execinfo.h>


#define LOG_TAG "RILD"

#define LIB_PATH_PROPERTY   "vendor.rild.libpath"
#define LIB_ARGS_PROPERTY   "vendor.rild.libargs"
#define MAX_LIB_ARGS        16

static void usage(const char *argv0) {
    fprintf(stderr, "Usage: %s -l <ril impl library> [-- <args for impl library>]\n", argv0);
    exit(EXIT_FAILURE);
}


extern char ril_service_name_base[MAX_SERVICE_NAME_LENGTH];
extern char ril_service_name[MAX_SERVICE_NAME_LENGTH];


extern void RIL_register (const RIL_RadioFunctions *callbacks);
extern void rilc_thread_pool ();

extern void RIL_register_socket (const RIL_RadioFunctions *(*rilUimInit)
        (const struct RIL_Env *, int, char **), RIL_SOCKET_TYPE socketType, int argc, char **argv);

extern void RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
        void *response, size_t responselen);

extern void RIL_onRequestAck(RIL_Token t);

#if defined(ANDROID_MULTI_SIM)
extern void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
        size_t datalen, RIL_SOCKET_ID socket_id);
#else
extern void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
        size_t datalen);
#endif

extern void RIL_requestTimedCallback (RIL_TimedCallback callback,
        void *param, const struct timeval *relativeTime);

extern void RIL_initReadAndWriteLockOwnerListMutex(void);
#ifdef TBOX
extern void RIL_onIssueLocalRequest(int request, void *data, int len, RIL_SOCKET_ID socket_id);
#endif


static struct RIL_Env s_rilEnv = {
    RIL_onRequestComplete,
    RIL_onUnsolicitedResponse,
    RIL_requestTimedCallback,
    RIL_onRequestAck,
    NULL,
    NULL,
#ifndef C2K_RIL
    NULL,
#endif
    NULL,
#ifdef TBOX
    /// MTK_RIL_ADAPTER @{
    RIL_onIssueLocalRequest
    /// @}
#endif
};

extern void RIL_startEventLoop();
extern void RIL_ExceptionCallMbrainIndex(int param, char* trace, char* extra);



int executeCmd(char * cmd, bool print)
{
    mtkLogD(LOG_TAG, "%s, cmd:%s\n", __FUNCTION__, cmd);
    FILE *fp = NULL;
    char buf[512] = {0};
    int status;
    fp = popen(cmd, "r");
    if (!fp) {
        mtkLogD(LOG_TAG, "popen  %s Fail!\n", cmd);
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

/*
* Purpose:  Function responsible by all signal handlers treatment any new signal must be added here
* Input:      param - signal ID
* Return:    -
*/

#define LIB_AED "libaedv.so"
typedef int (*AEE_SYSTEM_EXCEPTION_FUNC)(const char *, const char *, unsigned int, const char *, ...);

static int isReseted = 0;
static void resetWhenReceiveFaultSignal(void) {
    if (!isReseted) {
        mtkLogE(LOG_TAG, "trigger TRM");
        char prop_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
        mtk_property_get("persist.log.tag.tel_dbg", prop_value, "0");
        if (prop_value[0] == '1') {
            void *dlHandle;
            int (*aee_system_exception_func)(const char *, const char *, unsigned int, const char *, ...);
            dlHandle = dlopen(LIB_AED, RTLD_NOW);
            if (dlHandle != NULL) {
                aee_system_exception_func = (AEE_SYSTEM_EXCEPTION_FUNC)dlsym(dlHandle, "aee_system_exception");
                if (aee_system_exception_func != NULL) {
                    dlerror();
                    // DB_OPT_DEFAULT is 0
                    aee_system_exception_func("mtkrild", NULL, 0, "fusion rild NE");
                } else {
                    mtkLogE(LOG_TAG, "libaedv.so aee_system_exception_func not exsist");
                }
                dlclose(dlHandle);
            } else {
                mtkLogE(LOG_TAG, "libaedv.so dlopen failed: %s", dlerror());
            }
        } // prop_value[0] == '1'

        mtk_property_set("vendor.ril.mux.report.case","2");
        mtk_property_set("vendor.ril.muxreport", "1");
        isReseted = 1;
        if (true == isMipcSupport()) {
            mtk_property_set("vendor.ril.mtk.restart", "1");
        }
    } else {
        mtkLogE(LOG_TAG, "already reset");
    }

}

typedef int (*MBNotifyRILDException)(int signalNo, const char* backtrace, const char* extraInfo);

void signal_treatment(int param)
{
    mtkLogD(LOG_TAG, "signal_no=%d", param);
    if (param == SIGSEGV || param == SIGBUS) {
        void* callstack[128];
        char trace[1024] = {0};
        char extra[1024] = {0};
        int frames = backtrace(callstack, 128);
        mtkLogE(LOG_TAG, "signal_treatment frames(%d)", frames);

        for (int i = 1; i < frames; i++) {
            Dl_info info;
            if (dladdr(callstack[i], &info) && info.dli_sname) {
                mtkLogE(LOG_TAG, "symbols:(%s)(%s)", info.dli_sname, info.dli_fname);

                size_t trace_len = strlen(trace);
                size_t extra_len = strlen(extra);

                if (snprintf(trace + trace_len, sizeof(trace) - trace_len, "(%s)", info.dli_sname) < 0) {
                    mtkLogE(LOG_TAG, "append trace fail (%s)", info.dli_sname);
                }
                if (snprintf(extra + extra_len, sizeof(extra) - extra_len, "(%s)", info.dli_fname) < 0) {
                    mtkLogE(LOG_TAG, "append extra fail (%s)", info.dli_fname);
                }
            } else {
                mtkLogE(LOG_TAG, "symbol not found");
            }
        }
        RIL_ExceptionCallMbrainIndex(param, trace, extra);
    }

    switch (param) {
        case SIGUSR1:
            mtkLogD(LOG_TAG, "SIGUSR1");
            break;
        case SIGUSR2:
            mtkLogD(LOG_TAG, "SIGUSR2");
            break;
        case SIGSEGV:
        case SIGABRT:
        case SIGPIPE:
        case SIGILL:
        case SIGBUS:
        case SIGFPE:
            resetWhenReceiveFaultSignal();
            break;
        default:
            exit(2);
            break;
    }
}

void mtkInit() {
    //signals treatment
    if (signal(SIGUSR1, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGUSR1 failed");
    }
    if (signal(SIGUSR2, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGUSR2 failed");
    }
    mtkLogD(LOG_TAG, "Setup SIGSEGV signal handling, %d, %d", isInternalLoad(), isUserLoad());
    if (signal(SIGSEGV, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGSEGV failed");
    }
    if (signal(SIGABRT, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGABRT failed");
    }
    if (signal(SIGILL, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGILL failed");
    }
    if (signal(SIGPIPE, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGPIPE failed");
    }
    if (signal(SIGBUS, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGBUS failed");
    }
    if (signal(SIGFPE, signal_treatment) == SIG_ERR) {
        mtkLogE(LOG_TAG, "signal SIGFPE failed");
    }
}

static int make_argv(char * args, char ** argv) {
    // Note: reserve argv[0]
    int count = 1;
    char * tok;
    char * s = args;
    char *saveptr = NULL;
    while ((tok = strtok_r(s, " \0", &saveptr))) {
        argv[count] = tok;
        s = NULL;
        count++;
    }
    return count;
}

void setDynamicMsimConfig() {
    char prop_value[MTK_PROPERTY_VALUE_MAX] = { 0 };
    // update dynamic multisim config
    // sync persist.radio.multisim.config and persist.vendor.radio.msimmode
    FeatureValue featureValueOld = {0};
    FeatureValue featureValueNew = {0};
    mtkGetFeature(CONFIG_SIM_MODE, &featureValueOld);
    mtk_property_set("persist.vendor.radio.msimmode", featureValueOld.value);
    {
        // for EM
        if (strcmp(featureValueOld.value, "dsda") != 0) {
            // get ccci setting first, if not set then get EM setting.
            mtk_property_get("ro.boot.opt_sim_count", prop_value, "0");
            if (prop_value[0] == '0') {
                mtk_property_get("persist.vendor.radio.multisimslot", prop_value, "0");
            }
            if (prop_value[0] == '1') {
                strncpy(featureValueNew.value, "ss", 10);
                mtkSetFeature(CONFIG_SIM_MODE, &featureValueNew);
                mtk_property_set("persist.vendor.radio.msimmode", "ss");
            } else if (prop_value[0] == '2') {
                strncpy(featureValueNew.value, "dsds", 10);
                mtkSetFeature(CONFIG_SIM_MODE, &featureValueNew);
                mtk_property_set("persist.vendor.radio.msimmode", "dsds");
            } else if (prop_value[0] == '3') {
                strncpy(featureValueNew.value, "tsts", 10);
                mtkSetFeature(CONFIG_SIM_MODE, &featureValueNew);
                mtk_property_set("persist.vendor.radio.msimmode", "tsts");
            } else if (prop_value[0] == '4') {
                strncpy(featureValueNew.value, "qsqs", 10);
                mtkSetFeature(CONFIG_SIM_MODE, &featureValueNew);
                mtk_property_set("persist.vendor.radio.msimmode", "qsqs");
            }
        }
    }
}


mipc_msg_sim_ps_id_enum slotIdChangeToMipcSlotId(int slotId) {
    switch (slotId) {
        case 0: return MIPC_MSG_SIM0;
        case 1: return MIPC_MSG_SIM1;
        case 2: return MIPC_MSG_SIM2;
        case 3: return MIPC_MSG_SIM3;
    default:
        return MIPC_MSG_SIM0;
    }
}

int main(int argc, char **argv) {
    // vendor ril lib path either passed in as -l parameter, or read from rild.libpath property
    const char *rilLibPath = NULL;
    // ril arguments either passed in as -- parameter, or read from rild.libargs property
    char **rilArgv;
    // handle for vendor ril lib
    void *dlHandle;
    // Pointer to ril init function in vendor ril
    const RIL_RadioFunctions *(*rilInit)(const struct RIL_Env *, int, char **);
    // Pointer to sap init function in vendor ril
    const RIL_RadioFunctions *(*rilUimInit)(const struct RIL_Env *, int, char **);
    const char *err_str = NULL;

    // functions returned by ril init function in vendor ril
    const RIL_RadioFunctions *funcs;
    // lib path from rild.libpath property (if it's read)
    char libPath[MTK_PROPERTY_VALUE_MAX] = {0};
    char rilMode[MTK_PROPERTY_VALUE_MAX] = {0};
    // flat to indicate if -- parameters are present
    unsigned char hasLibArgs = 0;

    int i;
    // ril/socket id received as -c parameter, otherwise set to 0
    const char *clientId = NULL;

    char tel_dbg[MTK_PROPERTY_VALUE_MAX] = {0};
    mtk_property_get("persist.vendor.log.tel_dbg", tel_dbg, "0");
    int teleloglevel = atoi(tel_dbg);
    if(teleloglevel == 1) {
        updateAllTagLogLevel(LEVEL_DEBUG);
    }else {
        updateAllTagLogLevel(LEVEL_INFO);
    }
    //property_watch("persist.log.tag.RfxHandlerMgr" , setTagLogLevel);
    property_watch_async("persist.vendor.log.tel_dbg" , setTagLogLevel);
    property_watch_async("persist.log.tag.RILC" , setTagLogLevel);

    mtkLogD(LOG_TAG, "**RIL Proxy Started**");
    mtkLogD(LOG_TAG, "**RILd param count=%d**", argc);
    // check if rild is started by muxd
    mtk_property_get("ro.vendor.mtk_mipc_support", rilMode, "0");
    if (strcmp(rilMode, "1") != 0) {
        mtk_property_get("vendor.ril.mux.start", libPath, "0");
        if (libPath[0] == '0') {
            mtkLogE(LOG_TAG, "RILD is not started by muxd, TRM");
            mtk_property_set("vendor.ril.disable.eboot", "1");
            mtk_property_set("vendor.ril.mux.report.case", "2");
            mtk_property_set("vendor.ril.muxreport", "1");
            exit(0);
        }
        mtk_property_set("vendor.ril.mux.start", "0");
    }
#ifdef MTK_HYP_DUAL_TELE_SUPPORT
    #ifdef MTK_USE_AIDL
        if(!ABinderProcess_isThreadPoolStarted()) {
            ABinderProcess_setThreadPoolMaxThreadCount(0);
        } else {
            mtkLogE(LOG_TAG, "**binder pool already started, please chekch reason**");
        }
    #endif
#endif
    setDynamicMsimConfig();
    mtkInit();

    umask(S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    for (i = 1; i < argc ;) {
        if (0 == strcmp(argv[i], "-l") && (argc - i > 1)) {
            if (strcmp(rilMode, "1") == 0) {
                mtkLogD(LOG_TAG, "**fusion mipc ril Started**");
                rilLibPath = "libmtkmipc-ril.so";
            } else {
                rilLibPath = argv[i + 1];
            }
            i += 2;
        } else if (0 == strcmp(argv[i], "--")) {
            i++;
            hasLibArgs = 1;
            break;
        } else if (0 == strcmp(argv[i], "-c") &&  (argc - i > 1)) {
            clientId = argv[i+1];
            i += 2;
        } else {
            usage(argv[0]);
        }
    }
    if (clientId == NULL) {
        clientId = "0";
    } else if (atoi(clientId) >= MAX_RILDS) {
        mtkLogE(LOG_TAG, "Max Number of rild's supported is: %d", MAX_RILDS);
        exit(0);
    }

    if (strncmp(clientId, "0", MAX_CLIENT_ID_LENGTH)) {
        snprintf(ril_service_name, sizeof(ril_service_name), "%s%s", ril_service_name_base,
                 clientId);
    }


    if (rilLibPath == NULL) {
        if ( 0 == mtk_property_get(LIB_PATH_PROPERTY, libPath, NULL)) {
            // No lib sepcified on the command line, and nothing set in props.
            // Assume "no-ril" case.
            goto done;
        } else {
            rilLibPath = libPath;
        }
    }
    mtkLogD(LOG_TAG, "rilLibPath:%s", rilLibPath);
    /* special override when in the emulator */

    dlHandle = dlopen(rilLibPath, RTLD_NOW);

    if (dlHandle == NULL) {
        mtkLogE(LOG_TAG, "dlopen failed: %s", dlerror());
        exit(EXIT_FAILURE);
    }

    RIL_startEventLoop();

    rilInit =
        (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))
        dlsym(dlHandle, "RIL_Init");

    if (rilInit == NULL) {
        mtkLogE(LOG_TAG, "RIL_Init not defined or exported in %s\n", rilLibPath);
        exit(EXIT_FAILURE);
    }

    dlerror(); // Clear any previous dlerror
    rilUimInit =
        (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))
        dlsym(dlHandle, "RIL_SAP_Init");
    err_str = dlerror();
    if (err_str) {
        mtkLogW(LOG_TAG, "RIL_SAP_Init not defined or exported in %s: %s\n", rilLibPath, err_str);
    } else if (!rilUimInit) {
        mtkLogW(LOG_TAG, "RIL_SAP_Init defined as null in %s. SAP Not usable\n", rilLibPath);
    }

    if (hasLibArgs) {
        rilArgv = argv + i - 1;
        argc = argc -i + 1;
    } else {
        static char * newArgv[MAX_LIB_ARGS];
        static char args[MTK_PROPERTY_VALUE_MAX];
        rilArgv = newArgv;
        mtk_property_get(LIB_ARGS_PROPERTY, args, "");
        argc = make_argv(args, rilArgv);
    }


    RIL_initReadAndWriteLockOwnerListMutex();


    rilArgv[argc++] = "-c";
    rilArgv[argc++] = (char*)clientId;
    mtkLogD(LOG_TAG, "RIL_Init argc = %d clientId = %s", argc, rilArgv[argc-1]);

    // Make sure there's a reasonable argv[0]
    rilArgv[0] = argv[0];

    if (strcmp(rilMode, "1") == 0) {
        mtkLogD(LOG_TAG, "mipc init start");
        mtk_property_set("vendor.ril.muxreport.run", "2");
        SETLOGTAG("AT");
#if defined(MTK_RIL_THIN_MD_SUPPORT)
        SETCOM("/dev/wwanD0MIPC2");
        mipc_init_debug("/dev/wwanD0MIPC2");
#else
        SETCOM("/dev/ttyCMIPC1");
        mipc_init_debug("/dev/ttyCMIPC1");
#endif
        mtkLogD(LOG_TAG, "mipc init end");

        // disable modem notify URC from ttyC0
        const char* disableUrc = "AT+EURCRPT=2";
        mipc_msg_t* msg_req_ptr = mipc_msg_init(MIPC_SYS_AT_REQ, slotIdChangeToMipcSlotId(0));
        mipc_msg_add_tlv(msg_req_ptr, MIPC_SYS_AT_REQ_T_ATCMD, strlen(disableUrc) + 1, disableUrc);
        mipc_msg_t* msg_cnf_ptr = mipc_msg_sync(msg_req_ptr);
        mipc_msg_deinit(msg_req_ptr);
        mipc_msg_deinit(msg_cnf_ptr);

        // read data from port to clear for ccci queue
        int fd;
        fd = open("/dev/ttyC0", O_RDWR | O_NONBLOCK);
        if (fd >= 0) {
            ssize_t count = 0;
            do {
                char buffer[40 + 1];
                count = read(fd, buffer, 40);
                buffer[40] = '\0';
                mtkLogD(LOG_TAG, "read from ttyC0 count:%zd,%s\n", count, buffer);
            } while (count < 0 && errno == EINTR);
            close(fd);
        } else {
            mtkLogD(LOG_TAG, "Fail to open ttyC0: %s\n", strerror(errno));
        }
    }

    funcs = rilInit(&s_rilEnv, argc, rilArgv);
    mtkLogI(LOG_TAG, "RIL_Init rilInit completed");

    RIL_register(funcs);

    mtkLogI(LOG_TAG, "RIL_Init RIL_register completed");

    if (rilUimInit) {
        mtkLogD(LOG_TAG, "RIL_register_socket started");
        RIL_register_socket(rilUimInit, RIL_SAP_SOCKET, argc, rilArgv);
    }

    mtkLogI(LOG_TAG, "RIL_register_socket completed");

    if (executeCmd("systemctl restart muxreport.service", 0) == -1) {
        mtkLogE(LOG_TAG, "start muxreport.service fail\n");
    } else {
        mtkLogI(LOG_TAG, "start muxreport.service succeeded \n");
    }

    if (executeCmd("systemctl restart mtktelephonyservice.service" , 0) == -1) {
        mtkLogE(LOG_TAG, "start mtktelephonyservice.service fail\n");
    } else {
        mtkLogI(LOG_TAG, "start mtktelephonyservice.service succeeded \n");
    }

    if (executeCmd("systemctl start tboxdemoservice.service" , 0) == -1) {
        mtkLogE(LOG_TAG, "start tboxdemoservice.service fail\n");
    } else {
        mtkLogI(LOG_TAG, "start tboxdemoservice.service succeeded \n");
    }
done:

    rilc_thread_pool();

    mtkLogD(LOG_TAG, "RIL_Init starting sleep loop");
    while (true) {
        sleep(UINT32_MAX);
    }
}
