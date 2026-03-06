#include "libtrm.h"
#include "log.h"
#include <pthread.h>
#include <mtk_properties.h>

static int init = 0;
static int mon_thread_init = 0;
static int fd = -1;
static pthread_t tid;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static LIBTRM_MD_EVENT_CB md_event_cb;
static void *md_event_cb_priv_ptr;

static const char *deli = ", ";
static char *event_id = "0001:01:00.0:event_id=";
static char *info_state = "info=state=";
static char *fsm_flag = "fsm_flag=";
#define UEVENT_MSG_LEN 2048
#define PCIE_DEV_NODE_FSM       "/sys/bus/pci/devices/0001:01:00.0/fsm_state"
#define PCIE_DEV_NODE_CTRL      "/sys/bus/pci/devices/0001:01:00.0/mtk_device_ctrl"
#define KEY_FSM_STATE "state="
#define KEY_DEVICE_CONTROL "trm"
#define KEY_FSM_FLAG "fsm_flag="
#define STR_MODEM_RESET "modem reset"
#define LIBTRM_UNUSED(x)                  ((void)(x))

struct event_info {
    int event_id;
    int fsm_state;
    int fsm_flag;
};

static int open_uevent_socket(void);
static int parse_uevent(char *msg, struct event_info *info);
static void dump_uevent(struct event_info *info, int dumpUevent);
static int read_fsm_state(struct event_info *p_info);
static int write_mtk_device_ctrl(void);
static int wwan_pcie_pwrctl(int request);
#define WWAM_PWR_IOC_CHANNEL "/dev/wwan_pwrctl"

static int isThindModemSupport() {
    static int sIsThinModeSupport = -1;
    if (sIsThinModeSupport == -1) {
        char thinModem[10] = { 0 };
        mtk_property_get("ro.vendor.mtk_thin_md_telephony_support_mode", thinModem, "0");
        if (strcmp("0", thinModem) == 0) {
            sIsThinModeSupport = 0;
        } else {
            sIsThinModeSupport = 1;
        }
    }
    return sIsThinModeSupport;
}

void libtrm_init()
{
    if (init == 1) {
        LOGD("libtrm_init already called.\n");
    } else {
        LOGD("libtrm_init.\n");
        //openlog("libtrm", LOG_PID, LOG_DAEMON);
        init = 1;
    }
}

void libtrm_deinit()
{
    if (init == 0) {
        LOGD("libtrm_deinit - not init yet.\n");
    } else {
        LOGD("libtrm_deinit.\n");
        //closelog();
        init = 0;
    }
}

int libtrm_reset(int ccci_fd)
{
    int reset_parm = -1;

    LOGD("libtrm_reset: reset modem\n");
    if (isThindModemSupport() == 0) {
        if (ioctl(ccci_fd, CCCI_IOC_MD_RESET, &reset_parm) == 0) {
            return 0;
        } else {
            LOGD("libtrm_reset: reset control error (%s)\n", strerror(errno));
            return -1;
        }
    } else {
        return wwan_pcie_pwrctl(PWRCTL_CMD_COLD_RESET);
    }
}

int libtrm_power_off_md(int ccci_fd)
{
    int parm = -1;

    LOGD("libtrm_power_off_md\n");
    if (isThindModemSupport() == 0) {
        if (ioctl(ccci_fd, CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED, &parm) == 0) {
            return 0;
        } else {
            LOGD("libtrm_reset: reset control error (%s)\n", strerror(errno));
            return -1;
        }
    } else {
        return wwan_pcie_pwrctl(PWRCTL_CMD_POWER_OFF);
    }
}

int libtrm_silent_reset(void){
    int rval = 0;

    rval = write_mtk_device_ctrl();

    return rval;
}

int libtrm_power_on_md(int ccci_fd)
{
    int parm = -1;

    LOGD("libtrm_power_off_md\n");
    if (isThindModemSupport() == 0) {
        if (ioctl(ccci_fd, CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED, &parm) == 0) {
            return 0;
        } else {
            LOGD("libtrm_reset: reset control error (%s)\n", strerror(errno));
            return -1;
        }
    } else {
        return wwan_pcie_pwrctl(PWRCTL_CMD_POWER_ON);
    }
}

static void *libtrm_monitor_thread(void *priv_ptr)
{
    int count;
    int ret = 0;
    struct md_status_event status_buf;
    LIBTRM_UNUSED(priv_ptr);
    if (isThindModemSupport() == 0) {
        do {
            count = read(fd, &status_buf, sizeof(struct md_status_event));
            if (count < 0) {
                if (errno == EINTR) {
                    continue;
                } else {
                    LOGD("libtrm_monitor_thread: read error (%s)\n", strerror(errno));
                    break;
                }
            }

            if (count > 0) {
                LOGD("libtrm_monitor_thread: receive status event (%d)\n", status_buf.event_type);
                if (md_event_cb) {
                    md_event_cb(md_event_cb_priv_ptr, &status_buf);
                }
            }
        } while(1);
    } else {
        char msg[UEVENT_MSG_LEN + 2];
        int n = 0;
        int result = 0;
        struct md_status_event status_buf;
        memset(&status_buf, 0x0, sizeof(status_buf));
        LOGD("libtrm_monitor_thread: start\n");
        while (1) {
            while ((n = recv(fd, msg, UEVENT_MSG_LEN, 0)) > 0) {
                LOGD("libtrm_monitor_thread: receive status event (%s)[num=%d]\n", msg, n);
                struct event_info info;
                memset(&info, 0x0, sizeof(info));
                if (n == UEVENT_MSG_LEN) {
                    continue;
                }
                msg[n] = '\0';
                msg[n+1] = '\0';
                result = parse_uevent(msg, &info);
                status_buf.event_type = info.fsm_state;
#if defined(MTK_RIL_THIN_MD_SUPPORT)
                status_buf.event_flag = info.fsm_flag;
                LOGI("[%s][%d] event_type:%d, event_flag:%d \n", __FUNCTION__, __LINE__, status_buf.event_type, status_buf.event_flag);
#endif
                if (strlen(STR_MODEM_RESET) < sizeof(status_buf.reason)) {
                    strncpy(status_buf.reason, STR_MODEM_RESET, strlen(STR_MODEM_RESET));
                }
                if (0 == result) {
                    if (md_event_cb) {
                        md_event_cb(md_event_cb_priv_ptr, &status_buf);
                    }
                }
            }
        }
    }
    LOGD("libtrm_monitor_thread: exit.\n");

    ret = pthread_mutex_lock(&mutex);
    if (ret != 0) {
        LOGE("%s:pthread_mutex_lock fail(%d)\n", __FUNCTION__, ret);
    }

    close(fd);
    fd = -1;
    mon_thread_init = 0;
    ret = pthread_mutex_unlock(&mutex);
    if (ret != 0) {
        LOGE("%s:pthread_mutex_unlock fail(%d)\n", __FUNCTION__, ret);
    }
    return NULL;
}

void libtrm_md_event_register(LIBTRM_MD_EVENT_CB cb, void *cb_priv_ptr)
{

    int current_state = 0;
    int result = 0;
    int ret = 0;
    struct md_status_event status_buf;
    memset(&status_buf, 0x0, sizeof(status_buf));

    md_event_cb = cb;
    md_event_cb_priv_ptr = cb_priv_ptr;

    ret = pthread_mutex_lock(&mutex);
    if (ret != 0) {
        LOGE("%s:pthread_mutex_lock fail(%d)\n", __FUNCTION__, ret);
    }
    if (cb) {
        if (mon_thread_init == 0) {
            if (isThindModemSupport() == 0) {
                fd = open("/dev/ccci_md1_sta", O_RDWR);
            } else {
                fd = open_uevent_socket();
            }
            if (fd >= 0) {
                pthread_attr_t attr;
                int ret = 0;
                ret = pthread_attr_init(&attr);
                if (ret != 0) {
                    LOGE("%s:pthread_attr_init fail(%d)\n", __FUNCTION__, ret);
                }
                ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
                if (ret != 0) {
                    LOGE("%s:pthread_attr_setdetachstate fail(%d)\n", __FUNCTION__, ret);
                }
                if (isThindModemSupport() == 1) {
                    struct event_info info;
                    memset(&info, 0x0, sizeof(info));
                    result = read_fsm_state(&info);
                    status_buf.event_type = info.fsm_state;
#if defined(MTK_RIL_THIN_MD_SUPPORT)
                    status_buf.event_flag = info.fsm_flag;
                    LOGI("[%s][%d] event_type:%d, event_flag:%d \n", __FUNCTION__, __LINE__, status_buf.event_type, status_buf.event_flag);
#endif
                    if (0 == result) {
                        if (md_event_cb) {
                            md_event_cb(md_event_cb_priv_ptr, &status_buf);
                        }
                    }
                }
                int err = pthread_create(&tid, &attr, libtrm_monitor_thread, NULL);
                if (err == 0) {
                    mon_thread_init = 1;
                    LOGI("libtrm_md_event_register: mon. thread started.\n");
                } else {
                    LOGE("libtrm_md_event_register: mon. thread create fail (%s)\n",
                            strerror(err));
                    close(fd);
                    fd = -1;
                }
            } else {
                LOGE("libtrm_md_event_register open fail(%d) (%s)\n", fd, strerror(errno));
            }
        } else {
            LOGI("libtrm_md_event_register: mon. thread already exist.\n");
        }
    } else {
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }
    ret = pthread_mutex_unlock(&mutex);
    if (ret != 0) {
        LOGE("%s:pthread_mutex_unlock fail(%d)\n", __FUNCTION__, ret);
    }
}

static int open_uevent_socket(void) {
    struct sockaddr_nl addr;
    int sz = 64*1024;
    int socket_fd = 0;
    int ret = 0;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0xffffffff;
    socket_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (socket_fd < 0) {
        LOGE("create socket fail. s = %d, error (%s)\n", socket_fd, strerror(errno));
        return -1;
    }
    ret = setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &sz, sizeof(sz));
    if (ret < 0) {
        LOGE("setsockopt socket fail. s = %d, error (%s)\n", ret, strerror(errno));
        close(socket_fd);
        return -1;
    }
    if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        LOGE("bind socket fail. error (%s)\n", strerror(errno));
        close(socket_fd);
        return -1;
    }
    return socket_fd;
}

static int parse_uevent(char *msg, struct event_info *info) {
    char *token = NULL;
    char *savePtr = NULL;
    int dumpUevent = 0;
    if (msg ==  NULL || info == NULL) {
        LOGE("NULL parameter.\n");
        return -1;
    }
    while (*msg) {
#ifdef TRM_LOG_DEBUG
        LOGD("msg= %s\n", msg);
#endif
        token = strtok_r(msg, deli, &savePtr);
        while (token != NULL) {
//            LOGD("%s\n",token);
            if (strncmp(token, event_id, strlen(event_id)) == 0) {
                dumpUevent |= 0x01;
                info->event_id = atoi(token + strlen(event_id));
            } else if (strncmp(token, info_state, strlen(info_state)) == 0) {
                dumpUevent |= 0x02;
                info->fsm_state = atoi(token + strlen(info_state));
            } else if (strncmp(token, fsm_flag, strlen(fsm_flag)) == 0) {
                dumpUevent |= 0x04;
                info->fsm_flag = strtol(token + strlen(fsm_flag), NULL, 16);
                if (info->fsm_flag < 0) {
                    LOGE("p_info->fsm_state %d is invalid\n", info->fsm_flag);
                    return -1;
                }
            } else {
//                LOGD("No match fsm_state %s\n", token);
            }
            token = strtok_r(NULL, deli, &savePtr);
        }
        while (*msg++) {};
    }
    if (dumpUevent != 0) {
        dump_uevent(info, dumpUevent);
        return 0;
    } else {
        return -1;
    }
}

static void dump_uevent(struct event_info *info, int dumpUevent) {
    if (info != NULL) {
         LOGD("dump_uevent[event_id:%d],[fsm_state:%d],[fsm_flag:0x%x]",
             (dumpUevent & 0x01) ? info->event_id : -1,
             (dumpUevent & 0x02) ? info->fsm_state : -1,
             (dumpUevent & 0x04) ? info->fsm_flag : -1);
    }
}

#define READ_FSM_STATE_LEN 64

static int read_fsm_state(struct event_info *p_info) {
    int fsm_stat_fd = -1;
    char prop1[MTK_PROPERTY_VALUE_MAX] = {0};
    char prop2[MTK_PROPERTY_VALUE_MAX] = {0};
    char prop3[MTK_PROPERTY_VALUE_MAX] = {0};
    char prop4[MTK_PROPERTY_VALUE_MAX] = {0};
    char fsm_state_path[256] = {0};
    char fsm_state_ready_key[256] = {0};
    char fsm_flag_ready_key[256] = {0};
    char buf[READ_FSM_STATE_LEN+1] = {0};
    int rval = 0;
    int ret = 0;
    int count = 0;
    char *token = NULL;
    char *savePtr = NULL;

    mtk_property_get("ro.vendor.md.state.path1", prop1, "");
    mtk_property_get("ro.vendor.md.state.path2", prop2, "");
    mtk_property_get("ro.vendor.md.state.ready.key", prop3, "");
    mtk_property_get("ro.vendor.md.flag.ready.key", prop4, "");

    LOGD("read_fsm_state p1:%s, p2:%s, p3:%s, p4:%s\n", prop1, prop2, prop3, prop4);

    if (strlen(prop1) == 0 && strlen(prop2) == 0) {
        ret |= sprintf(fsm_state_path, "%s", PCIE_DEV_NODE_FSM);
    } else {
        ret |= sprintf(fsm_state_path, "%s%s", prop1, prop2);
    }
    if (strlen(prop3) == 0) {
        ret |= sprintf(fsm_state_ready_key, "%s", KEY_FSM_STATE);
    } else {
        ret |= sprintf(fsm_state_ready_key, "%s", prop3);
    }
    if (strlen(prop4) == 0) {
        ret |= sprintf(fsm_flag_ready_key, "%s", KEY_FSM_FLAG);
    } else {
        ret |= sprintf(fsm_flag_ready_key, "%s", prop4);
    }
    fsm_stat_fd = open(fsm_state_path, O_RDONLY);
    if ((fsm_stat_fd >= 0) && (ret > 0)) {
        count = read(fsm_stat_fd, buf, READ_FSM_STATE_LEN);
        if (count < 0) {
            close(fsm_stat_fd);
            LOGE("read fsm_stat_fd fail\n");
            rval = -1;
        } else{
            buf[count] = '\0';
            close(fsm_stat_fd);
            LOGI("read_fsm_state %s\n", buf);
            token = strtok_r(buf, deli, &savePtr);
            while (token != NULL) {
//              LOGD("%s\n",token);
                if (strncmp(token, fsm_state_ready_key, strlen(fsm_state_ready_key)) == 0) {
                    p_info->fsm_state = atoi(token + strlen(fsm_state_ready_key));
                    if (p_info->fsm_state < 0) {
                        LOGE("p_info->fsm_state %d is invalid\n", p_info->fsm_state);
                        rval = -1;
                    }
                } else if (strncmp(token, fsm_flag_ready_key, strlen(fsm_flag_ready_key)) == 0) {
                    p_info->fsm_flag = strtol(token + strlen(fsm_flag_ready_key), NULL, 16);
                    if (p_info->fsm_flag < 0) {
                        LOGE("p_info->fsm_state %d is invalid\n", p_info->fsm_flag);
                        rval = -1;
                    }
                } else {
//                  LOGD("No match fsm_state %s\n", token);
                }
                token = strtok_r(NULL, deli, &savePtr);
            }
        }
    } else {
        if (fsm_stat_fd >= 0) {
            close(fsm_stat_fd);
        }
        LOGI("read_fsm_state fails\n");
        rval = -1;
    }
    return rval;
}

static int write_mtk_device_ctrl(void) {
    int mtk_device_ctrl_fd = -1;
    char prop1[MTK_PROPERTY_VALUE_MAX] = {0};
    char prop2[MTK_PROPERTY_VALUE_MAX] = {0};
    char prop3[MTK_PROPERTY_VALUE_MAX] = {0};
    char mtk_device_ctrl_path[256] = {0};
    char mtk_device_ctrl_key[256] = {0};
    int rval = 0;
    int ret = 0;
    int write_count=0;

    mtk_property_get("ro.vendor.md.ctrl.path1", prop1, "");
    mtk_property_get("ro.vendor.md.ctrl.path2", prop2, "");
    mtk_property_get("ro.vendor.md.ctrl.ready.key", prop3, "");

    LOGI("write_mtk_device_ctrl p1:%s, p2:%s, p3:%s\n", prop1, prop2, prop3);

    if (strlen(prop1) == 0 && strlen(prop2) == 0) {
        ret |= sprintf(mtk_device_ctrl_path, "%s", PCIE_DEV_NODE_CTRL);
    } else {
        ret |= sprintf(mtk_device_ctrl_path, "%s%s", prop1, prop2);
    }
    if (strlen(prop3) == 0) {
        ret |= sprintf(mtk_device_ctrl_key, "%s", KEY_DEVICE_CONTROL);
    } else {
        ret |= sprintf(mtk_device_ctrl_key, "%s", prop3);
    }

    mtk_device_ctrl_fd = open(mtk_device_ctrl_path, O_WRONLY);
    if ((mtk_device_ctrl_fd >= 0) && (ret > 0)) {
        write_count = write(mtk_device_ctrl_fd, mtk_device_ctrl_key, sizeof(mtk_device_ctrl_key));
        LOGI("write_count %d\n", write_count);
        close(mtk_device_ctrl_fd);
        LOGI("write_mtk_device_ctrl %s\n", mtk_device_ctrl_key);
    } else {
        if (mtk_device_ctrl_fd >= 0) {
            close(mtk_device_ctrl_fd);
        }
        LOGI("write_mtk_device_ctrl fails\n");
        rval = -1;
    }
    return rval;
}

static int wwan_pcie_pwrctl(int request) {
    int ccci_sys_fd = -1;
    int reset_parm = -1;
    LOGD("wwan_pcie_pwrctl %d\n", request);
    ccci_sys_fd = open(WWAM_PWR_IOC_CHANNEL, O_RDWR | O_NONBLOCK);
    if (ccci_sys_fd < 0) {
        LOGE("open %s, errno: %d", WWAM_PWR_IOC_CHANNEL, errno);
        return -1;
    }
    if (ioctl(ccci_sys_fd, request, &reset_parm) == 0) {
        close(ccci_sys_fd);
        return 0;
    } else {
        LOGE("ioctl %d, errno: %d", request, errno);
        close(ccci_sys_fd);
        return -1;
    }
}
