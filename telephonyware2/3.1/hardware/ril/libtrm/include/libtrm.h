#ifndef _LIBTRM_H_
#define _LIBTRM_H_

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define CCCI_IOC_MAGIC 'C'
#define CCCI_IOC_MD_RESET _IO(CCCI_IOC_MAGIC, 0)
#define CCCI_IOC_ENTER_DEEP_FLIGHT_ENHANCED _IO(CCCI_IOC_MAGIC,  123)
#define CCCI_IOC_LEAVE_DEEP_FLIGHT_ENHANCED _IO(CCCI_IOC_MAGIC,  124)
#define CCCI_IOC_RILD_POWER_OFF_MD _IO(CCCI_IOC_MAGIC,  125)

/* Power control command magic number */
#define PWRCTL_CMD_MAGIC        'G'
/* Power control command through ioctl of /dev/wwan_pwrctl */
#define PWRCTL_CMD_POWER_ON         _IOW(PWRCTL_CMD_MAGIC, 0, int)
#define PWRCTL_CMD_POWER_OFF        _IOW(PWRCTL_CMD_MAGIC, 1, int)
#define PWRCTL_CMD_WARM_RESET       _IOW(PWRCTL_CMD_MAGIC, 2, int)
#define PWRCTL_CMD_COLD_RESET       _IOW(PWRCTL_CMD_MAGIC, 3, int)
#define PWRCTL_CMD_SOFT_ON          _IOW(PWRCTL_CMD_MAGIC, 4, int)
#define PWRCTL_CMD_TRIGGER_EXCEPTION    _IOW(PWRCTL_CMD_MAGIC, 5, int)

enum md_bc_event {
    MD_STA_EV_INVALID = 0,
    MD_STA_EV_RESET_REQUEST,
    MD_STA_EV_F_ASSERT_REQUEST,
    MD_STA_EV_STOP_REQUEST,
    MD_STA_EV_START_REQUEST,
    MD_STA_EV_ENTER_FLIGHT_REQUEST,
    MD_STA_EV_LEAVE_FLIGHT_REQUEST,
    MD_STA_EV_ENTER_FLIGHT_E_REQUEST,
    MD_STA_EV_LEAVE_FLIGHT_E_REQUEST,
    MD_STA_EV_HS1,
    MD_STA_EV_READY,
    MD_STA_EV_EXCEPTION,
    MD_STA_EV_STOP,
};

enum mtk_fsm_state {
    FSM_STATE_INVALID = 0,
    FSM_STATE_OFF,
    FSM_STATE_ON,
    FSM_STATE_POSTDUMP,
    FSM_STATE_DOWNLOAD,
    FSM_STATE_BOOTUP,
    FSM_STATE_READY,
    FSM_STATE_MDEE
};

enum mtk_fsm_flag {
    FSM_F_DFLT = 0,
    FSM_F_DL_PORT_CREATE   = 0x000001,
    FSM_F_DL_DA            = 0x000002,
    FSM_F_DL_JUMPBL        = 0x000004,
    FSM_F_DL_TIMEOUT       = 0x000008,
    FSM_F_SAP_HS_START     = 0x000010,
    FSM_F_SAP_HS2_DONE     = 0x000020,
    FSM_F_MD_HS_START      = 0x000040,
    FSM_F_MD_HS2_DONE      = 0x000080,
    FSM_F_MDEE_INIT        = 0x000100,
    FSM_F_MDEE_INIT_DONE   = 0x000200,
    FSM_F_MDEE_CLEARQ_DONE = 0x000400,
    FSM_F_MDEE_ALLQ_RESET  = 0x000800,
    FSM_F_MDEE_MSG         = 0x001000,
    FSM_F_MDEE_RECV_OK     = 0x002000,
    FSM_F_MDEE_PASS        = 0x004000,
    FSM_F_FULL_REINIT      = 0x008000,
    FSM_F_DL_PL            = 0x010000,
    FSM_F_DL_FB            = 0x020000,
    FSM_F_MD_HS4_DONE      = 0x040000,
    FSM_F_DL_JUMPLK        = 0x080000,
    FSM_F_MD_SILENT_REBOOT = 0x100000
};

struct md_status_event {
    struct timeval time_stamp;
    int md_id;
    int event_type;
    char reason[32];
#if defined(MTK_RIL_THIN_MD_SUPPORT)
    int event_flag;
#endif
};

typedef void (*LIBTRM_MD_EVENT_CB)(void *priv_ptr, struct md_status_event*);

void libtrm_init();
void libtrm_deinit();
int libtrm_power_off_md(int ccci_fd);
int libtrm_power_on_md(int ccci_fd);
int libtrm_reset(int ccci_fd);
int libtrm_silent_reset(void);
void libtrm_md_event_register(LIBTRM_MD_EVENT_CB cb, void *cb_priv_ptr);

#endif
