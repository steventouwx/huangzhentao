/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  Copyright (c) [2020], MediaTek Inc. All rights reserved.
*  This software/firmware and related documentation ("MediaTek Software") are
*  protected under relevant copyright laws.
*
*  The information contained herein is confidential and proprietary to
*  MediaTek Inc. and/or its licensors. Except as otherwise provided in the
*  applicable licensing terms with MediaTek Inc. and/or its licensors, any
*  reproduction, modification, use or disclosure of MediaTek Software, and
*  information contained herein, in whole or in part, shall be strictly
*  prohibited.
*****************************************************************************/

#include "platform.h"
#include "mipc_hashmap.h"
#include "mipc_msg.h"
#include "mipc_msg_host.h"
#include "mipc_internal.h"
#include <sys/prctl.h>
#include <dlfcn.h>

#ifdef TRM_SUPPORT
void *libtrm_handle;

void (*LIBTRM_INIT)() = NULL;
void (*LIBTRM_DEINIT)() = NULL;
int (*LIBTRM_RESET)(int ccci_fd) = NULL;
int (*LIBTRM_POWER_OFF_MD)(int ccci_fd) = NULL;
int (*LIBTRM_POWER_ON_MD)(int ccci_fd) = NULL;
void (*LIBTRM_MD_EVENT_REGISTER)(MIPC_MD_EVENT_CB cb, void *cb_priv_ptr) = NULL;
#endif

const char* mipcIdToString(int mipcId);

#define RECV_BUF_LEN (65000)
static unsigned char *recv_buf_ptr;
static uint32_t inited = 0;
static mipc_hashmap_t *transaction_hashmap_ptr;
static mipc_hashmap_t *ind_hashmap_ptr;
static mipc_hashmap_t *cmd_hashmap_ptr;
static MUTEX *transaction_mutex_ptr;
static MUTEX *ind_mutex_ptr;
static MUTEX *cmd_mutex_ptr;
static MUTEX *write_mutex_ptr; //for write access
static THREAD *thread_ptr = 0;

static uint32_t default_timeout_ms = 300000;

static mipc_hashmap_t *tls_timeout_hashmap_ptr;
static MUTEX *tls_mutex_ptr;

static const uint16_t sMipcDumpMsgWithoutContent[] = {
    // data
    MIPC_DATA_GET_TFT_CNF,
    MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF,
    MIPC_DATA_GET_URSP_UE_POLICY_CNF,
    // network
    MIPC_NW_GET_EHRPD_INFO_CNF,
    MIPC_NW_GET_PSEUDO_CELL_INFO_CNF,
    MIPC_NW_PSEUDO_CELL_IND,
    MIPC_NW_CS_IND,
    MIPC_NW_GET_CS_CNF,
    MIPC_NW_PS_IND,
    MIPC_NW_GET_PS_CNF,
    MIPC_NW_GET_BASE_STATIONS_CNF,
    MIPC_NW_ECELL_IND,
    // MWIS(mobile wifi service/wifi calling service)
    MIPC_WFC_WIFI_INFO_NTF,
    MIPC_WFC_NATT_KEEP_ALIVE_RSP,
    MIPC_WFC_NATT_KEEP_ALIVE_CMD,
    MIPC_WFC_GET_MAC_CMD,
    MIPC_WFC_GET_MAC_RSP,
    MIPC_SYS_GEO_LOCATION_IND,
    MIPC_SYS_SET_GEO_LOCATION_REQ,
    // phone book
    MIPC_PHB_SET_PHB_ENTRY_REQ,
    MIPC_PHB_GET_PHB_ENTRY_CNF,
    MIPC_PHB_SET_UPB_ENTRY_REQ,
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF,
    // device identiy
    MIPC_SYS_GET_INFO_CNF
};

static const uint16_t sMipcLogReductionNotOutput[] = {
    MIPC_INTERNAL_REGISTER_IND_REQ,
    MIPC_INTERNAL_REGISTER_IND_CNF
};

#define NUM_ELEMS(a)    (sizeof (a) / sizeof (a)[0])

int needHideMipcMsgContent(uint16_t msgId) {
    int ret = 0;
    unsigned int num = NUM_ELEMS(sMipcDumpMsgWithoutContent);
    for (unsigned int i = 0; i < num; i++) {
        if (msgId == sMipcDumpMsgWithoutContent[i]) {
            ret = 1;
            break;
        }
    }
    return ret;
}

int isLogReductionMipcMsgId(uint16_t msgId) {
    int ret = 0;
    unsigned int num = NUM_ELEMS(sMipcLogReductionNotOutput);
    for (unsigned int i = 0; i < num; i++) {
        if (msgId == sMipcLogReductionNotOutput[i]) {
            ret = 1;
            break;
        }
    }
    return ret;
}

#ifdef UNIT_TEST
static mipc_msg_t *unit_test_set_msg_ptr = NULL;
void UNIT_TEST_SET_MSG(mipc_msg_t *msg_ptr)
{
    unit_test_set_msg_ptr = msg_ptr;
}

void UNIT_TEST_SEND_MSG(mipc_msg_t *msg_ptr)
{
    uint8_t *buf_ptr;
    uint16_t buf_len;

    buf_ptr = mipc_msg_serialize(msg_ptr, &buf_len);

    UNIT_TEST_READ_BUF_ADD(buf_ptr, buf_len);

    FREE(buf_ptr);
}
#endif

/* 0 only output msgid
 * 1 output dump msg
 * 2 output dump msg and tlv info
 */
#define DEBUG_MIPC_FULL_DUMP (1)
static void dump_msg(uint16_t is_send, uint16_t msg_id, uint16_t msg_txid, uint8_t *msg_raw, uint16_t msg_raw_len)
{
    if (isLogReductionMipcMsgId(msg_id) == 1) {
        return;
    }
#if (DEBUG_MIPC_FULL_DUMP != 0)
    uint8_t *p;
    uint8_t *p_cur;
    uint8_t *p_tail;
    uint16_t i;
    uint16_t buf_left_size = 4096;
    char *log_buf;
    char *log_cur;

    p_cur = p = ALLOC(msg_raw_len);
    if (p == NULL) {
        ErrMsg("Debug Error 0, ALLOC failed MSGID=%04x, TXID=%04x", msg_id, msg_txid);
        return;
    }

    MEMCPY(p, msg_raw, msg_raw_len);

    p_tail = p + msg_raw_len;
    //skip header
    p_cur += sizeof(mipc_msg_hdr_t);
    int need_hide = needHideMipcMsgContent(msg_id);
    while (p_cur < p_tail) {
        mipc_msg_tlv_t *tlv_ptr;
        uint16_t tlv_len;
        if ((p_tail - p_cur) < sizeof(mipc_msg_tlv_t)) { //something error
            ErrMsg("Debug Error 1 [%ld][%zu]", p_tail - p_cur, sizeof(mipc_msg_tlv_t));
            break;
        }
        tlv_ptr = (mipc_msg_tlv_t *)p_cur;
        if (tlv_ptr->len == 0) { //it's an array
            tlv_len = sizeof(mipc_msg_tlv_array_t) - sizeof(mipc_msg_tlv_t);
        } else {
            tlv_len = tlv_ptr->len;
        }
        p_cur += sizeof(mipc_msg_tlv_t);
        if ((p_tail - p_cur) < tlv_len) { //something error
            ErrMsg("Debug Error 2 [%ld][%u]", (p_tail - p_cur), tlv_len);
            break;
        }
        if (tlv_ptr->type >= 0x8100 || need_hide == 1) { //it's a sensitive tag, replace value to ***
            memset(p_cur, '*', tlv_len);
        }
        p_cur = ((uint8_t *)tlv_ptr) + ALIGN(sizeof(mipc_msg_tlv_t) + tlv_len);
    }

    log_cur = log_buf = ALLOC(buf_left_size);
    if (log_buf == NULL) {
        FREE(p);
        ErrMsg("Debug Error 3, ALLOC failed MSGID=%04x, TXID=%04x", msg_id, msg_txid);
        return;
    }
    for (i = 0; (i < msg_raw_len && buf_left_size > 32);) {
        int num_read = 0;
        if ((msg_raw_len - i) >= 16) {
            num_read = snprintf(log_cur, buf_left_size, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", p[i], p[i + 1], p[i + 2], p[i + 3], p[i + 4], p[i + 5], p[i + 6], p[i + 7], p[i + 8], p[i + 9], p[i + 10], p[i + 11], p[i + 12], p[i + 13], p[i + 14], p[i + 15]);
            if (num_read < 0) {
                FREE(log_buf);
                FREE(p);
                ErrMsg("Debug Error 4, snprintf failed MSGID=%04x, TXID=%04x", msg_id, msg_txid);
                return;
            }
            i += 16;
            log_cur += 32;
            buf_left_size -= 32;
        } else {
            num_read = snprintf(log_cur, buf_left_size, "%02X", p[i]);
            if (num_read < 0) {
                FREE(log_buf);
                FREE(p);
                ErrMsg("Debug Error 5, snprintf failed MSGID=%04x, TXID=%04x", msg_id, msg_txid);
                return;
            }
            i += 1;
            log_cur += 2;
            buf_left_size -= 2;
        }

    }

    if (is_send == 1) {
        char buf[30] = {0};
        prctl(PR_GET_NAME, buf);
        DbgLogInfo("SEND MSGID=%04x [%s], TXID=%04x [%s], DUMP=%s", msg_id, mipcIdToString(msg_id), msg_txid, buf, log_buf);
    } else {
        DbgLogInfo("RECV MSGID=%04x [%s], TXID=%04x, DUMP=%s", msg_id, mipcIdToString(msg_id), msg_txid, log_buf);
    }
    FREE(log_buf);
    FREE(p);
#else
    if (is_send == 1) {
        char buf[30] = {0};
        prctl(PR_GET_NAME, buf);
        DbgLogInfo("SEND MSGID=%04x [%s], TXID=%04x [%s]", msg_id, mipcIdToString(msg_id), msg_txid, buf);
    } else {
        DbgLogInfo("RECV MSGID=%04x [%s], TXID=%04x", msg_id, mipcIdToString(msg_id), msg_txid);
    }

#endif
}


static uint32_t rx_thread(void *priv_ptr)
{
    mipc_msg_t *msg_ptr;
    mipc_msg_hdr_t *msg_hdr_ptr;
    mipc_hashmap_element_t *hashmap_element_ptr;
    uint16_t recv_buf_ptr_len;
    uint16_t total_read_bytes;
    uint16_t msg_id;
    uint16_t msg_txid;
    uint32_t key;
    MIPC_UNUSED(priv_ptr);

    for (;;) {
        memset(recv_buf_ptr, 0, RECV_BUF_LEN);

        VdbgMsg("Start to READCOM");

        //read header
        total_read_bytes = READCOM(recv_buf_ptr, sizeof(mipc_msg_hdr_t));
#ifdef UNIT_TEST
        if (total_read_bytes == 0) continue;
#endif
        if (total_read_bytes != sizeof(mipc_msg_hdr_t)) {
            ErrMsg("read error, rx_thread stop");
            return -1;
        }

        msg_hdr_ptr = (mipc_msg_hdr_t*)&recv_buf_ptr[0];
        if (msg_hdr_ptr->msg_len) {
            recv_buf_ptr_len = total_read_bytes + msg_hdr_ptr->msg_len;
            //read payload
            total_read_bytes += READCOM(recv_buf_ptr + total_read_bytes, recv_buf_ptr_len - total_read_bytes);
#ifdef UNIT_TEST
            if (total_read_bytes == 0) continue;
#endif
        } else {
            recv_buf_ptr_len = total_read_bytes;
        }
        if (total_read_bytes != recv_buf_ptr_len) {
            ErrMsg("read error, rx_thread stop");
            return -1;
        }
        if ((msg_ptr = mipc_msg_deserialize(recv_buf_ptr, recv_buf_ptr_len)) == NULL) {
            //error handling
            ErrMsg("MSG deserialize failed:");
            continue;
        }

        msg_id = MTOHS(msg_ptr->hdr.msg_id);
        msg_txid = MTOHS(msg_ptr->hdr.msg_txid);

        //dump recv message
        if (recv_buf_ptr != NULL) {
            dump_msg(0, msg_id, msg_txid, recv_buf_ptr, recv_buf_ptr_len);
        }

        switch (msg_id & MIPC_MSG_TYPE_MASK) {
        case MIPC_MSG_TYPE_REQ_CNF: {//receive CNF
            LOCK_MUTEX(transaction_mutex_ptr);
            if ((hashmap_element_ptr = mipc_hashmap_get(transaction_hashmap_ptr, &msg_txid, sizeof(msg_txid)))) {
                mipc_transaction_t tx;
                //copy first and free hashmap_element_ptr
                MEMCPY(&tx, MIPC_HASHMAP_GET_VAL_PTR(hashmap_element_ptr), sizeof(mipc_transaction_t));
                mipc_hashmap_del(hashmap_element_ptr);
                UNLOCK_MUTEX(transaction_mutex_ptr);

                switch (tx.transaction_type) {
                case MIPC_TRANSACTION_TYPE_SYNC:
                    WAKE_EVENT(tx.event_ptr, msg_ptr);
                    break;
                case MIPC_TRANSACTION_TYPE_ASYNC:
                    if (tx.cb_mode == 1) {
                        if (tx.a.cb1.msg_cb) {
                            DbgMsg("async CNF callback start");
                            tx.a.cb1.msg_cb(msg_ptr, tx.a.cb1.cb_priv_ptr);
                            DbgMsg("async CNF callback end");
                        }
                    } else {
                        if (tx.a.cb2.msg_cb) {
                            tx.a.cb2.msg_cb(msg_ptr, tx.a.cb2.api_cb, tx.a.cb2.cb_priv_ptr);
                        }
                    }
                    //free
                    mipc_msg_deinit(msg_ptr);
                    break;
                }
            } else {
                //NOTICE
                UNLOCK_MUTEX(transaction_mutex_ptr);

                //no found, just free
                mipc_msg_deinit(msg_ptr);
            }
            break;
        }
        case MIPC_MSG_TYPE_IND: {//receive IND
            int32_t i;
            uint8_t sim_ps_id = msg_ptr->hdr.msg_sim_ps_id;
            for (i = 0; i < 8; i++) {
                if (sim_ps_id & (1 << i)) {
                    key = ((1 << i) << 16) | msg_id;
                    LOCK_MUTEX(ind_mutex_ptr);
                    if (msg_ptr == NULL) {
                        //create new one
                        if ((msg_ptr = mipc_msg_deserialize(recv_buf_ptr, recv_buf_ptr_len)) == NULL) {
                            //NOTICE
                            UNLOCK_MUTEX(ind_mutex_ptr);
                            continue;
                        }
                    }
                    msg_ptr->hdr.msg_sim_ps_id = (1 << i);
                    if ((hashmap_element_ptr = mipc_hashmap_get(ind_hashmap_ptr, &key, sizeof(key)))) {
                        mipc_ind_t *ind_ptr;

                        ind_ptr = (mipc_ind_t *)MIPC_HASHMAP_GET_VAL_PTR(hashmap_element_ptr);
                        //ind_ptr will not be NULL if hashmap_element_ptr is not NULL
                        //whether hashmap_element_ptr is NULL had been checked in the previous if condition

                        if (ind_ptr->cb_mode == 1) {
                            if (ind_ptr->a.cb1.msg_cb) {
                                DbgMsg("IND callback start");
                                ind_ptr->a.cb1.msg_cb(msg_ptr, ind_ptr->a.cb1.cb_priv_ptr);
                                DbgMsg("IND callback end");
                            }
                        } else {
                            if (ind_ptr->a.cb2.msg_cb) {
                                ind_ptr->a.cb2.msg_cb(msg_ptr, ind_ptr->a.cb2.api_cb, ind_ptr->a.cb2.cb_priv_ptr);
                            }
                        }
                        //  NOTICE
                        UNLOCK_MUTEX(ind_mutex_ptr);
                    } else {
                        //NOTICE
                        UNLOCK_MUTEX(ind_mutex_ptr);
                    }
                    //always free
                    mipc_msg_deinit(msg_ptr);
                    msg_ptr = NULL;
                }
            }
            if (msg_ptr) {
                //no one register it, just free
                mipc_msg_deinit(msg_ptr);
            }
            break;
        }
        case MIPC_MSG_TYPE_CMD_RSP: {//receive CMD
            key = msg_id;
            LOCK_MUTEX(cmd_mutex_ptr);
            if ((hashmap_element_ptr = mipc_hashmap_get(cmd_hashmap_ptr, &key, sizeof(key)))) {
                mipc_cmd_t *cmd_ptr;

                //NOTICE
                UNLOCK_MUTEX(cmd_mutex_ptr);

                cmd_ptr = (mipc_cmd_t *)MIPC_HASHMAP_GET_VAL_PTR(hashmap_element_ptr);
                //cmd_ptr will not be NULL if hashmap_element_ptr is not NULL
                //whether hashmap_element_ptr is NULL had been checked in the previous if condition

                if (cmd_ptr->cb_mode == 1) {
                    if (cmd_ptr->a.cb1.msg_cb) {
                        DbgMsg("CMD callback start");
                        cmd_ptr->a.cb1.msg_cb(msg_ptr, cmd_ptr->a.cb1.cb_priv_ptr);
                        DbgMsg("CMD callback end");
                    }
                } else {
                    if (cmd_ptr->a.cb2.msg_cb) {
                        cmd_ptr->a.cb2.msg_cb(msg_ptr, cmd_ptr->a.cb2.api_cb, cmd_ptr->a.cb2.cb_priv_ptr);
                    }
                }
            } else {
                //NOTICE
                UNLOCK_MUTEX(cmd_mutex_ptr);
            }
            //always free
            mipc_msg_deinit(msg_ptr);
            break;
        }
        default:
            //free
            mipc_msg_deinit(msg_ptr);
            break;
        }
        //port will be closed
        if (msg_id == MIPC_INTERNAL_CLOSE_CNF) {
            break;
        }
    }
    return 0;
}

int32_t mipc_init_debug(const char *process_name_ptr)
{
    if (process_name_ptr == NULL) {
        return -1;
    }

    return mipc_init(process_name_ptr);
}


int32_t mipc_init(const char *process_name_ptr)
{
    static int32_t singleton = 0;
#ifndef UNIT_TEST
    mipc_msg_t* msg_req_ptr;
    mipc_msg_t* msg_cnf_ptr;
    mipc_msg_t* msg_test_req_ptr;
    mipc_msg_t* msg_test_cnf_ptr;
#endif

    if (process_name_ptr == NULL) {
        return -1;
    }


    if (OPENCOM(FINDCOM(process_name_ptr)) < 0) {
        return -2;
    }

    //only allocate once
    if (singleton == 0) {
        INITIALIZE();
        recv_buf_ptr = ALLOC(RECV_BUF_LEN);
        if (recv_buf_ptr == NULL) {
            return -3;
        }
        transaction_hashmap_ptr = mipc_hashmap_init(32, NULL);
        ind_hashmap_ptr = mipc_hashmap_init(32, NULL);
        cmd_hashmap_ptr = mipc_hashmap_init(32, NULL);
        transaction_mutex_ptr = CREATE_MUTEX();
        ind_mutex_ptr = CREATE_MUTEX();
        cmd_mutex_ptr = CREATE_MUTEX();
        write_mutex_ptr = CREATE_MUTEX();
        //
        tls_timeout_hashmap_ptr = mipc_hashmap_init(32, NULL);
        tls_mutex_ptr = CREATE_MUTEX();
        mipc_create_txid_mutex();

#ifdef TRM_SUPPORT
        //init libtrm
        libtrm_handle = dlopen("libtrm.so", RTLD_LAZY);

        if (libtrm_handle) {
            LIBTRM_INIT = (void(*)(void))dlsym(libtrm_handle, "libtrm_init");
            LIBTRM_DEINIT = (void(*)(void))dlsym(libtrm_handle, "libtrm_deinit");
            LIBTRM_RESET = (int(*)(int))dlsym(libtrm_handle, "libtrm_reset");
            LIBTRM_POWER_OFF_MD = (int(*)(int))dlsym(libtrm_handle, "libtrm_power_off_md");
            LIBTRM_POWER_ON_MD = (int(*)(int))dlsym(libtrm_handle, "libtrm_power_on_md");
            LIBTRM_MD_EVENT_REGISTER = (void(*)(MIPC_MD_EVENT_CB, void *))dlsym(libtrm_handle, "libtrm_md_event_register");
            ErrMsg("open libtrm success");
        } else {
            ErrMsg("open libtrm fail");
        }
#endif

        singleton = 1;
        inited = 1;
    }

#ifdef OPENWRT
    //init libtrm
    if (LIBTRM_INIT) {
        LIBTRM_INIT();
    }
#endif

    //create rx thread
    if (thread_ptr){
        ErrMsg("create thread failed");
        return -1;
    }else{
        thread_ptr = CREATE_THREAD(rx_thread, NULL);
    }

#ifndef UNIT_TEST
    //sync to modem
    // never change MIPC_MSG_ALL to other value
    msg_test_req_ptr = mipc_msg_init(MIPC_INTERNAL_TEST_REQ, MIPC_MSG_ALL);
    mipc_ret_cause_enum causeValue = ERR_NO_ERR;
    mipc_msg_set_timeout_once(100);
    causeValue = mipc_msg_sync_timeout_with_cause(msg_test_req_ptr, &msg_test_cnf_ptr);
    while (causeValue != ERR_NO_ERR) {
        ErrMsg("MIPC_INTERNAL_TEST_REQ sync fail %d", causeValue);
        mipc_msg_set_timeout_once(100);
        causeValue = mipc_msg_sync_timeout_with_cause(msg_test_req_ptr, &msg_test_cnf_ptr);
    }
    mipc_msg_deinit(msg_test_req_ptr);
    if (msg_test_cnf_ptr) {
        mipc_msg_deinit(msg_test_cnf_ptr);
    }

    msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_OPEN_REQ, MIPC_MSG_ALL);
    mipc_msg_add_tlv_uint32(msg_req_ptr, MIPC_INTERNAL_OPEN_REQ_T_VERSION, MIPC_VERSION);
    mipc_msg_add_tlv(msg_req_ptr, MIPC_INTERNAL_OPEN_REQ_T_CLIENT_NAME, strlen(process_name_ptr) + 1, process_name_ptr);
    mipc_msg_add_tlv_uint8(msg_req_ptr, MIPC_INTERNAL_OPEN_REQ_T_USIR_SUPPORT, MIPC_BOOLEAN_TRUE);
    msg_cnf_ptr = mipc_msg_sync_timeout(msg_req_ptr);
    mipc_msg_deinit(msg_req_ptr);

    if (msg_cnf_ptr) {
        //get version
        DbgMsg("Target version: %x", mipc_msg_get_val_uint32(msg_cnf_ptr, MIPC_INTERNAL_OPEN_CNF_T_VERSION, 0xffffffff));
        //get timeout (default is 5 minutes)
        DbgMsg("Default timeout: %u ms", mipc_msg_get_val_uint32(msg_cnf_ptr, MIPC_INTERNAL_OPEN_CNF_T_TIMEOUT, 300000));
        mipc_msg_deinit(msg_cnf_ptr);
    } else {
        return -1;
    }
#endif

    return 0;
}

void mipc_deinit()
{
    mipc_msg_t* msg_req_ptr;
    mipc_msg_t* msg_cnf_ptr;

    if (inited == 0) return;

#ifdef OPENWRT
    //deinit libtrm
    if (LIBTRM_DEINIT) {
        LIBTRM_DEINIT();
    }
#endif
    DbgLogInfo("mipc_deinit");
    //sync to modem
    msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_CLOSE_REQ, MIPC_MSG_ALL);
    mipc_msg_set_timeout_once(2000);
    msg_cnf_ptr = mipc_msg_sync_timeout(msg_req_ptr);
    mipc_msg_deinit(msg_req_ptr);
    mipc_msg_deinit(msg_cnf_ptr);

    DbgLogInfo("Waiting for Terminate ");
    WAIT_FOR_KILL_THREAD(thread_ptr);

    //close com port
    CLOSECOM();

    //clean all hashmap
    LOCK_MUTEX(transaction_mutex_ptr);
    mipc_hashmap_clean(transaction_hashmap_ptr);
    UNLOCK_MUTEX(transaction_mutex_ptr);

    LOCK_MUTEX(ind_mutex_ptr);
    mipc_hashmap_clean(ind_hashmap_ptr);
    UNLOCK_MUTEX(ind_mutex_ptr);

    LOCK_MUTEX(cmd_mutex_ptr);
    mipc_hashmap_clean(cmd_hashmap_ptr);
    UNLOCK_MUTEX(cmd_mutex_ptr);

    LOCK_MUTEX(tls_mutex_ptr);
    mipc_hashmap_clean(tls_timeout_hashmap_ptr);
    UNLOCK_MUTEX(tls_mutex_ptr);

    DELETE_THREAD(thread_ptr);
    thread_ptr = 0;
}


void mipc_set_host_api_timeout(uint32_t timeout_ms)
{
    default_timeout_ms = timeout_ms;
}

void mipc_msg_set_timeout_once(uint32_t timeout_ms)
{
    //
    uint64_t tid;
    mipc_hashmap_element_t *hashmap_element_ptr;

    //get thread speific event
    tid = GETTID();
    LOCK_MUTEX(tls_mutex_ptr);
    if ((hashmap_element_ptr = mipc_hashmap_get(tls_timeout_hashmap_ptr, &tid, sizeof(tid))) != NULL) {
        //update value if existence
        uint32_t *timeout_ptr = (uint32_t *)MIPC_HASHMAP_GET_VAL_PTR(hashmap_element_ptr);
        *timeout_ptr = timeout_ms;
    } else {
        //add it if non-existence
        mipc_hashmap_add(tls_timeout_hashmap_ptr, &tid, sizeof(tid), &timeout_ms, sizeof(timeout_ms));
    }
    UNLOCK_MUTEX(tls_mutex_ptr);
}

static int _mipc_msg_send(mipc_msg_t *msg_ptr)
{
    uint8_t* send_buf_ptr;
    uint16_t send_buf_len;
    uint16_t total_write_bytes;

    send_buf_ptr = mipc_msg_serialize(msg_ptr, &send_buf_len);

    if (send_buf_ptr == NULL) {
        ErrMsg("mipc_msg_serialize error");
        return -1;
    }

    // dump send message
    dump_msg(1, msg_ptr->hdr.msg_id, msg_ptr->hdr.msg_txid, send_buf_ptr, send_buf_len);

    LOCK_MUTEX(write_mutex_ptr);
    total_write_bytes = WRITECOM(send_buf_ptr, send_buf_len);
    UNLOCK_MUTEX(write_mutex_ptr);

    FREE(send_buf_ptr);
    send_buf_ptr = NULL;
    if (total_write_bytes != send_buf_len) {
        ErrMsg("write error");
        return -1;
    }
#ifdef UNIT_TEST
    if (unit_test_set_msg_ptr) {
        UNIT_TEST_SEND_MSG(unit_test_set_msg_ptr);
        unit_test_set_msg_ptr = NULL;
    }
#endif
    return 0;
}

mipc_msg_t *_mipc_msg_sync_timeout(mipc_msg_t *msg_ptr, int32_t *ret_cause_ptr)
{
    EVENT *event_ptr;
    mipc_transaction_t tx;
    mipc_msg_t *msg_cnf_ptr;
    //
    uint64_t tid;
    mipc_hashmap_element_t *hashmap_element_ptr;
    uint32_t timeout_ms;
    int32_t ret_cause = 0;

    if ((MTOHS(msg_ptr->hdr.msg_id) & MIPC_MSG_TYPE_MASK) != MIPC_MSG_TYPE_REQ_CNF) {
        if (ret_cause_ptr) {
            *ret_cause_ptr = ERR_UNEXPECTED_MSG_TYPE;
        }
        return NULL;
    }

    //get thread speific event
    tid = GETTID();
    LOCK_MUTEX(tls_mutex_ptr);
    if ((hashmap_element_ptr = mipc_hashmap_get(tls_timeout_hashmap_ptr, &tid, sizeof(tid))) != NULL) {
        uint32_t *timeout_ptr = (uint32_t *)MIPC_HASHMAP_GET_VAL_PTR(hashmap_element_ptr);
        timeout_ms = *timeout_ptr;
        mipc_hashmap_del(hashmap_element_ptr); //thread specific timeout only active once
    } else {
        timeout_ms = default_timeout_ms;
    }
    UNLOCK_MUTEX(tls_mutex_ptr);

    //create event
    event_ptr = CREATE_EVENT(timeout_ms);

    //fill
    memset(&tx, 0, sizeof(tx));
    tx.msg_txid = MTOHS(msg_ptr->hdr.msg_txid);
    tx.transaction_type = MIPC_TRANSACTION_TYPE_SYNC;
    tx.event_ptr = event_ptr;
    LOCK_MUTEX(transaction_mutex_ptr);
    //add to hashmap
    mipc_hashmap_add(transaction_hashmap_ptr, &tx.msg_txid, sizeof(tx.msg_txid), &tx, sizeof(tx));
    UNLOCK_MUTEX(transaction_mutex_ptr);

    if (_mipc_msg_send(msg_ptr) != 0) {
        ret_cause = ERR_WRITE_ERROR;
    }

    //send and wait complete for synchronous operation
    if ( ret_cause == 0 && WAIT_EVENT(event_ptr) == 0) {
        //get result
        msg_cnf_ptr = (mipc_msg_t *)event_ptr->result_ptr;
    } else {
        //timeout
        msg_cnf_ptr = NULL;
        if (ret_cause == 0) {
            ret_cause = ERR_TIME_OUT;
        }

        //remove hashmap element
        LOCK_MUTEX(transaction_mutex_ptr);
        if ((hashmap_element_ptr = mipc_hashmap_get(transaction_hashmap_ptr, &tx.msg_txid, sizeof(tx.msg_txid)))) {
            mipc_hashmap_del(hashmap_element_ptr);
        }
        UNLOCK_MUTEX(transaction_mutex_ptr);
    }
    //free event
    DELETE_EVENT(event_ptr);
    event_ptr = 0;

    if (ret_cause_ptr) {
        *ret_cause_ptr = ret_cause;
    }

    return msg_cnf_ptr;
}

mipc_msg_t *mipc_msg_sync_timeout(mipc_msg_t *msg_ptr) {
    return _mipc_msg_sync_timeout(msg_ptr, 0);
}

mipc_ret_cause_enum mipc_msg_sync_timeout_with_cause(mipc_msg_t *msg_ptr, mipc_msg_t **cnf_ptr)
{
    mipc_msg_t *cnf = 0;
    int32_t ret_cause = 0;

    cnf = _mipc_msg_sync_timeout(msg_ptr, &ret_cause);

    if (cnf_ptr) {
        *cnf_ptr = cnf;
    } else {
        mipc_msg_deinit(cnf);
    }

    return ret_cause;
}

mipc_msg_t *mipc_msg_sync(mipc_msg_t *msg_ptr)
{
    mipc_msg_set_timeout_once(0);
    return mipc_msg_sync_timeout(msg_ptr);
}

mipc_ret_cause_enum mipc_msg_sync_with_cause(mipc_msg_t *msg_ptr, mipc_msg_t **cnf_ptr)
{
    mipc_msg_t *cnf = 0;
    int32_t ret_cause = 0;

    mipc_msg_set_timeout_once(0);
    cnf = _mipc_msg_sync_timeout(msg_ptr, &ret_cause);

    if (cnf_ptr) {
        *cnf_ptr = cnf;
    } else {
        mipc_msg_deinit(cnf);
    }

    return ret_cause;
}

mipc_ret_cause_enum mipc_msg_async_api(mipc_msg_t *msg_ptr, void *msg_cb, MIPC_API_CB api_cb, void *cb_priv_ptr)
{
    mipc_transaction_t tx;

    if ((MTOHS(msg_ptr->hdr.msg_id) & MIPC_MSG_TYPE_MASK) != MIPC_MSG_TYPE_REQ_CNF) {
        return ERR_UNEXPECTED_MSG_TYPE;
    }

    //fill
    memset(&tx, 0, sizeof(tx));
    tx.msg_txid = MTOHS(msg_ptr->hdr.msg_txid);
    tx.transaction_type = MIPC_TRANSACTION_TYPE_ASYNC;
    if (api_cb == NULL) {
        tx.cb_mode = 1;
        tx.a.cb1.msg_cb = (MIPC_MSG_CB)msg_cb;
        tx.a.cb1.cb_priv_ptr = cb_priv_ptr;
    } else {
        tx.cb_mode = 2;
        tx.a.cb2.msg_cb = (MIPC_MSG_CB2)msg_cb;
        tx.a.cb2.api_cb = api_cb;
        tx.a.cb2.cb_priv_ptr = cb_priv_ptr;
    }
    LOCK_MUTEX(transaction_mutex_ptr);
    //add to hashmap
    mipc_hashmap_add(transaction_hashmap_ptr, &tx.msg_txid, sizeof(tx.msg_txid), &tx, sizeof(tx));
    UNLOCK_MUTEX(transaction_mutex_ptr);

    //send
    if (_mipc_msg_send(msg_ptr) != 0) {
        //remove hashmap element
        LOCK_MUTEX(transaction_mutex_ptr);
        mipc_hashmap_element_t *hashmap_element_ptr;
        if ((hashmap_element_ptr = mipc_hashmap_get(transaction_hashmap_ptr, &tx.msg_txid, sizeof(tx.msg_txid)))) {
            mipc_hashmap_del(hashmap_element_ptr);
        }
        UNLOCK_MUTEX(transaction_mutex_ptr);
        return ERR_WRITE_ERROR;
    }

    return 0;
}

int32_t mipc_msg_unregister_ind_api(mipc_msg_sim_ps_id_enum sim_ps_id, uint16_t msg_id)
{
    mipc_msg_t* msg_req_ptr;
    mipc_msg_t* msg_cnf_ptr;
    mipc_hashmap_element_t *hashmap_element_ptr;
    uint32_t key = sim_ps_id << 16 | msg_id;

    LOCK_MUTEX(ind_mutex_ptr);
    // check exist
    if ((hashmap_element_ptr = mipc_hashmap_get(ind_hashmap_ptr, &key, sizeof(key)))) {
        mipc_hashmap_del(hashmap_element_ptr);

        msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_UNREGISTER_IND_REQ, sim_ps_id);
        mipc_msg_add_tlv_uint16(msg_req_ptr, MIPC_INTERNAL_UNREGISTER_IND_REQ_T_MSG_ID, msg_id);
    } else {
        UNLOCK_MUTEX(ind_mutex_ptr);
        return -1;
    }

    UNLOCK_MUTEX(ind_mutex_ptr);
#ifndef UNIT_TEST
    // send to modem
    msg_cnf_ptr = mipc_msg_sync(msg_req_ptr);
    if (msg_cnf_ptr != NULL) {
        DbgMsg("Unregister IND MSGID=%04x Result:%x",
                msg_req_ptr->hdr.msg_id, MTOHL(*(uint32_t *)mipc_msg_get_val_ptr(msg_cnf_ptr, MIPC_T_RESULT, NULL)));

        mipc_msg_deinit(msg_cnf_ptr);
    }
#endif
    mipc_msg_deinit(msg_req_ptr);

    return 0;
}

int32_t mipc_msg_register_ind_api(mipc_msg_sim_ps_id_enum sim_ps_id, uint16_t msg_id, void *msg_cb, MIPC_API_CB api_cb, void *cb_priv_ptr)
{
    mipc_msg_t* msg_req_ptr;
    mipc_msg_t* msg_cnf_ptr;
    mipc_ind_t ind;
    mipc_hashmap_element_t *hashmap_element_ptr;
    uint32_t key = sim_ps_id << 16 | msg_id;

    if ((msg_id & MIPC_MSG_TYPE_MASK) != MIPC_MSG_TYPE_IND) {
        return ERR_UNEXPECTED_MSG_TYPE;
    }

    LOCK_MUTEX(ind_mutex_ptr);
    // check exist
    if ((hashmap_element_ptr = mipc_hashmap_get(ind_hashmap_ptr, &key, sizeof(key)))) {
        if (msg_cb == NULL) {
            mipc_hashmap_del(hashmap_element_ptr);

            msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_UNREGISTER_IND_REQ, sim_ps_id);
            mipc_msg_add_tlv_uint16(msg_req_ptr, MIPC_INTERNAL_UNREGISTER_IND_REQ_T_MSG_ID, msg_id);
        } else {
            UNLOCK_MUTEX(ind_mutex_ptr);
            return ERR_CB_IS_NULL;
        }
    } else {
        // register callback
        memset(&ind, 0, sizeof(ind));
        ind.key = key;
        if (api_cb == NULL) {
            ind.cb_mode = 1;
            ind.a.cb1.msg_cb = (MIPC_MSG_CB)msg_cb;
            ind.a.cb1.cb_priv_ptr = cb_priv_ptr;
        } else {
            ind.cb_mode = 2;
            ind.a.cb2.msg_cb = (MIPC_MSG_CB2)msg_cb;
            ind.a.cb2.api_cb = api_cb;
            ind.a.cb2.cb_priv_ptr = cb_priv_ptr;
        }
        // add to hashmap
        mipc_hashmap_add(ind_hashmap_ptr, &ind.key, sizeof(ind.key), &ind, sizeof(ind));

        msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_REGISTER_IND_REQ, sim_ps_id);
        mipc_msg_add_tlv_uint16(msg_req_ptr, MIPC_INTERNAL_REGISTER_IND_REQ_T_MSG_ID, msg_id);
    }
    UNLOCK_MUTEX(ind_mutex_ptr);

#ifndef UNIT_TEST
    // send to modem
    msg_cnf_ptr = mipc_msg_sync(msg_req_ptr);
    if (msg_cnf_ptr != NULL) {
        VdbgMsg("Register IND MSGID=%04x Result:%x",
                msg_req_ptr->hdr.msg_id, MTOHL(*(uint32_t *)mipc_msg_get_val_ptr(msg_cnf_ptr, MIPC_T_RESULT, NULL)));
    }

    mipc_msg_deinit(msg_cnf_ptr);
#endif
    mipc_msg_deinit(msg_req_ptr);

    return 0;
}

int32_t mipc_msg_unregister_cmd_api(uint16_t msg_id)
{
    mipc_msg_t* msg_req_ptr;
    mipc_msg_t* msg_cnf_ptr;
    mipc_hashmap_element_t *hashmap_element_ptr;
    uint32_t key = msg_id;

    LOCK_MUTEX(cmd_mutex_ptr);
    // check exist
    if ((hashmap_element_ptr = mipc_hashmap_get(cmd_hashmap_ptr, &key, sizeof(key)))) {
        mipc_hashmap_del(hashmap_element_ptr);
        msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_UNREGISTER_CMD_REQ, MIPC_MSG_ALL);
        mipc_msg_add_tlv_uint16(msg_req_ptr, MIPC_INTERNAL_UNREGISTER_CMD_REQ_T_MSG_ID, msg_id);

    } else {
        UNLOCK_MUTEX(cmd_mutex_ptr);
        return -1;
    }
    UNLOCK_MUTEX(cmd_mutex_ptr);

#ifndef UNIT_TEST
    // send to modem
    msg_cnf_ptr = mipc_msg_sync(msg_req_ptr);
    if (msg_cnf_ptr != NULL) {
        DbgMsg("Register CMD MSGID=%04x Result:%x",
                msg_req_ptr->hdr.msg_id, MTOHL(*(uint32_t *)mipc_msg_get_val_ptr(msg_cnf_ptr, MIPC_T_RESULT, NULL)));

        mipc_msg_deinit(msg_cnf_ptr);
    }
#endif
    mipc_msg_deinit(msg_req_ptr);

    return 0;
}

int32_t mipc_msg_register_cmd_api(uint16_t msg_id, void *msg_cb, MIPC_API_CB api_cb, void *cb_priv_ptr)
{
    mipc_msg_t* msg_req_ptr;
    mipc_msg_t* msg_cnf_ptr;
    mipc_cmd_t cmd;
    mipc_hashmap_element_t *hashmap_element_ptr;
    uint32_t key = msg_id;

    if ((msg_id & MIPC_MSG_TYPE_MASK) != MIPC_MSG_TYPE_CMD_RSP) {
        return ERR_UNEXPECTED_MSG_TYPE;
    }

    LOCK_MUTEX(cmd_mutex_ptr);
    // check exist
    if ((hashmap_element_ptr = mipc_hashmap_get(cmd_hashmap_ptr, &key, sizeof(key)))) {
        if (msg_cb == NULL) {
            mipc_hashmap_del(hashmap_element_ptr);

            msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_UNREGISTER_CMD_REQ, MIPC_MSG_ALL);
            mipc_msg_add_tlv_uint16(msg_req_ptr, MIPC_INTERNAL_UNREGISTER_CMD_REQ_T_MSG_ID, msg_id);
        } else {
            UNLOCK_MUTEX(cmd_mutex_ptr);
            return ERR_CB_IS_NULL;
        }
    } else {
        // register callback
        memset(&cmd, 0, sizeof(cmd));
        cmd.key = key;
        if (api_cb == NULL) {
            cmd.cb_mode = 1;
            cmd.a.cb1.msg_cb = (MIPC_MSG_CB)msg_cb;
            cmd.a.cb1.cb_priv_ptr = cb_priv_ptr;
        } else {
            cmd.cb_mode = 2;
            cmd.a.cb2.msg_cb = (MIPC_MSG_CB2) msg_cb;
            cmd.a.cb2.api_cb = api_cb;
            cmd.a.cb2.cb_priv_ptr = cb_priv_ptr;
        }
        // add to hashmap
        mipc_hashmap_add(cmd_hashmap_ptr, &cmd.key, sizeof(cmd.key), &cmd, sizeof(cmd));

        msg_req_ptr = mipc_msg_init(MIPC_INTERNAL_REGISTER_CMD_REQ, MIPC_MSG_ALL);
        mipc_msg_add_tlv_uint16(msg_req_ptr, MIPC_INTERNAL_REGISTER_CMD_REQ_T_MSG_ID, msg_id);
    }
    UNLOCK_MUTEX(cmd_mutex_ptr);

#ifndef UNIT_TEST
    // send to modem
    msg_cnf_ptr = mipc_msg_sync(msg_req_ptr);

    if (msg_cnf_ptr != NULL) {
        DbgMsg("Register CMD MSGID=%04x Result:%x",
                msg_req_ptr->hdr.msg_id, MTOHL(*(uint32_t *)mipc_msg_get_val_ptr(msg_cnf_ptr, MIPC_T_RESULT, NULL)));
    }

    mipc_msg_deinit(msg_cnf_ptr);
#endif
    mipc_msg_deinit(msg_req_ptr);

    return 0;
}

int32_t mipc_msg_rsp(mipc_msg_t *msg_rsp_ptr)
{
    if (!msg_rsp_ptr) {
        return ERR_UNEXPECTED_MSG;
    }

    if ((MTOHS(msg_rsp_ptr->hdr.msg_id) & MIPC_MSG_TYPE_MASK) != MIPC_MSG_TYPE_CMD_RSP) {
        return ERR_UNEXPECTED_MSG_TYPE;
    }

    //send
    _mipc_msg_send(msg_rsp_ptr);

    return 0;
}

int32_t mipc_msg_ntf(mipc_msg_t *msg_ntf_ptr)
{
    if (!msg_ntf_ptr) {
        return ERR_UNEXPECTED_MSG;
    }

    if ((MTOHS(msg_ntf_ptr->hdr.msg_id) & MIPC_MSG_TYPE_MASK) != MIPC_MSG_TYPE_NTF) {
        return ERR_UNEXPECTED_MSG_TYPE;
    }

    //send
    _mipc_msg_send(msg_ntf_ptr);

    return 0;
}

uint8_t is_valid_mipc_msg_txid(uint16_t txid) {
    // if exist in transaction_hashmap_ptr, means duplicated txid.
    return (mipc_hashmap_get(transaction_hashmap_ptr, &txid, sizeof(txid)) ? 0 : 1);
}

const char* mipcIdToString(int mipcId) {
    switch (mipcId) {
        case MIPC_SYS_GET_INFO_REQ: return "SYS_GET_INFO_REQ";
        case MIPC_SYS_GET_INFO_CNF: return "SYS_GET_INFO_CNF";
        case MIPC_SYS_AT_REQ: return "SYS_AT_REQ";
        case MIPC_SYS_AT_CNF: return "SYS_AT_CNF";
        case MIPC_SYS_REBOOT_REQ: return "SYS_REBOOT_REQ";
        case MIPC_SYS_REBOOT_CNF: return "SYS_REBOOT_CNF";
        case MIPC_SYS_GET_MAPPING_REQ: return "SYS_GET_MAPPING_REQ";
        case MIPC_SYS_GET_MAPPING_CNF: return "SYS_GET_MAPPING_CNF";
        case MIPC_SYS_SET_MAPPING_REQ: return "SYS_SET_MAPPING_REQ";
        case MIPC_SYS_SET_MAPPING_CNF: return "SYS_SET_MAPPING_CNF";
        case MIPC_SYS_GET_THERMAL_SENSOR_NUM_REQ: return "SYS_GET_THERMAL_SENSOR_NUM_REQ";
        case MIPC_SYS_GET_THERMAL_SENSOR_NUM_CNF: return "SYS_GET_THERMAL_SENSOR_NUM_CNF";
        case MIPC_SYS_GET_THERMAL_SENSOR_INFO_REQ: return "SYS_GET_THERMAL_SENSOR_INFO_REQ";
        case MIPC_SYS_GET_THERMAL_SENSOR_INFO_CNF: return "SYS_GET_THERMAL_SENSOR_INFO_CNF";
        case MIPC_SYS_SET_THERMAL_SENSOR_REQ: return "SYS_SET_THERMAL_SENSOR_REQ";
        case MIPC_SYS_SET_THERMAL_SENSOR_CNF: return "SYS_SET_THERMAL_SENSOR_CNF";
        case MIPC_SYS_GET_THERMAL_SENSOR_REQ: return "SYS_GET_THERMAL_SENSOR_REQ";
        case MIPC_SYS_GET_THERMAL_SENSOR_CNF: return "SYS_GET_THERMAL_SENSOR_CNF";
        case MIPC_SYS_GET_THERMAL_ACTUATOR_NUM_REQ: return "SYS_GET_THERMAL_ACTUATOR_NUM_REQ";
        case MIPC_SYS_GET_THERMAL_ACTUATOR_NUM_CNF: return "SYS_GET_THERMAL_ACTUATOR_NUM_CNF";
        case MIPC_SYS_SET_THERMAL_ACTUATOR_REQ: return "SYS_SET_THERMAL_ACTUATOR_REQ";
        case MIPC_SYS_SET_THERMAL_ACTUATOR_CNF: return "SYS_SET_THERMAL_ACTUATOR_CNF";
        case MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_REQ: return "SYS_GET_THERMAL_ACTUATOR_INFO_REQ";
        case MIPC_SYS_GET_THERMAL_ACTUATOR_INFO_CNF: return "SYS_GET_THERMAL_ACTUATOR_INFO_CNF";
        case MIPC_SYS_SET_CONFIG_REQ: return "SYS_SET_CONFIG_REQ";
        case MIPC_SYS_SET_CONFIG_CNF: return "SYS_SET_CONFIG_CNF";
        case MIPC_SYS_GET_CONFIG_REQ: return "SYS_GET_CONFIG_REQ";
        case MIPC_SYS_GET_CONFIG_CNF: return "SYS_GET_CONFIG_CNF";
        case MIPC_SYS_REG_CONFIG_REQ: return "SYS_REG_CONFIG_REQ";
        case MIPC_SYS_REG_CONFIG_CNF: return "SYS_REG_CONFIG_CNF";
        case MIPC_SYS_SET_ADPCLK_REQ: return "SYS_SET_ADPCLK_REQ";
        case MIPC_SYS_SET_ADPCLK_CNF: return "SYS_SET_ADPCLK_CNF";
        case MIPC_SYS_GET_ADPCLK_REQ: return "SYS_GET_ADPCLK_REQ";
        case MIPC_SYS_GET_ADPCLK_CNF: return "SYS_GET_ADPCLK_CNF";
        case MIPC_SYS_WRITE_NVRAM_REQ: return "SYS_WRITE_NVRAM_REQ";
        case MIPC_SYS_WRITE_NVRAM_CNF: return "SYS_WRITE_NVRAM_CNF";
        case MIPC_SYS_READ_NVRAM_REQ: return "SYS_READ_NVRAM_REQ";
        case MIPC_SYS_READ_NVRAM_CNF: return "SYS_READ_NVRAM_CNF";
        case MIPC_SYS_AUTH_REQ: return "SYS_AUTH_REQ";
        case MIPC_SYS_AUTH_CNF: return "SYS_AUTH_CNF";
        case MIPC_SYS_SET_DAT_REQ: return "SYS_SET_DAT_REQ";
        case MIPC_SYS_SET_DAT_CNF: return "SYS_SET_DAT_CNF";
        case MIPC_SYS_GET_DAT_REQ: return "SYS_GET_DAT_REQ";
        case MIPC_SYS_GET_DAT_CNF: return "SYS_GET_DAT_CNF";
        case MIPC_SYS_MCF_REQ: return "SYS_MCF_REQ";
        case MIPC_SYS_MCF_CNF: return "SYS_MCF_CNF";
        case MIPC_SYS_SET_FCC_LOCK_REQ: return "SYS_SET_FCC_LOCK_REQ";
        case MIPC_SYS_SET_FCC_LOCK_CNF: return "SYS_SET_FCC_LOCK_CNF";
        case MIPC_SYS_SET_TIME_REQ: return "SYS_SET_TIME_REQ";
        case MIPC_SYS_SET_TIME_CNF: return "SYS_SET_TIME_CNF";
        case MIPC_SYS_GET_TIME_REQ: return "SYS_GET_TIME_REQ";
        case MIPC_SYS_GET_TIME_CNF: return "SYS_GET_TIME_CNF";
        case MIPC_SYS_SET_SAR_REQ: return "SYS_SET_SAR_REQ";
        case MIPC_SYS_SET_SAR_CNF: return "SYS_SET_SAR_CNF";
        case MIPC_SYS_GET_SAR_REQ: return "SYS_GET_SAR_REQ";
        case MIPC_SYS_GET_SAR_CNF: return "SYS_GET_SAR_CNF";
        case MIPC_SYS_SET_POWER_SAVING_REQ: return "SYS_SET_POWER_SAVING_REQ";
        case MIPC_SYS_SET_POWER_SAVING_CNF: return "SYS_SET_POWER_SAVING_CNF";
        case MIPC_SYS_CONNECTIVITY_STATISTICS_REQ: return "SYS_CONNECTIVITY_STATISTICS_REQ";
        case MIPC_SYS_CONNECTIVITY_STATISTICS_CNF: return "SYS_CONNECTIVITY_STATISTICS_CNF";
        case MIPC_SYS_QUERY_SBP_REQ: return "SYS_QUERY_SBP_REQ";
        case MIPC_SYS_QUERY_SBP_CNF: return "SYS_QUERY_SBP_CNF";
        case MIPC_SYS_SET_GEO_LOCATION_REQ: return "SYS_SET_GEO_LOCATION_REQ";
        case MIPC_SYS_SET_GEO_LOCATION_CNF: return "SYS_SET_GEO_LOCATION_CNF";
        case MIPC_SYS_SET_DSBP_REQ: return "SYS_SET_DSBP_REQ";
        case MIPC_SYS_SET_DSBP_CNF: return "SYS_SET_DSBP_CNF";
        case MIPC_SYS_SEND_SAR_IND_REQ: return "SYS_SEND_SAR_IND_REQ";
        case MIPC_SYS_SEND_SAR_IND_CNF: return "SYS_SEND_SAR_IND_CNF";
        case MIPC_SYS_SILENT_REBOOT_REQ: return "SYS_SILENT_REBOOT_REQ";
        case MIPC_SYS_SILENT_REBOOT_CNF: return "SYS_SILENT_REBOOT_CNF";
        case MIPC_SYS_MULTI_SIM_CONFIG_REQ: return "SYS_MULTI_SIM_CONFIG_REQ";
        case MIPC_SYS_MULTI_SIM_CONFIG_CNF: return "SYS_MULTI_SIM_CONFIG_CNF";
        case MIPC_SYS_REBOOT_SET_REQ: return "SYS_REBOOT_SET_REQ";
        case MIPC_SYS_REBOOT_SET_CNF: return "SYS_REBOOT_SET_CNF";
        case MIPC_SYS_GET_THERMAL_ACTUATOR_REQ: return "SYS_GET_THERMAL_ACTUATOR_REQ";
        case MIPC_SYS_GET_THERMAL_ACTUATOR_CNF: return "SYS_GET_THERMAL_ACTUATOR_CNF";
        case MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_REQ: return "SYS_GET_THERMAL_SENSOR_RUNTIME_REQ";
        case MIPC_SYS_GET_THERMAL_SENSOR_RUNTIME_CNF: return "SYS_GET_THERMAL_SENSOR_RUNTIME_CNF";
        case MIPC_SYS_SET_THERMAL_RUNTIME_REQ: return "SYS_SET_THERMAL_RUNTIME_REQ";
        case MIPC_SYS_SET_THERMAL_RUNTIME_CNF: return "SYS_SET_THERMAL_RUNTIME_CNF";
        case MIPC_SYS_SET_OR_GET_SBP_INFO_REQ: return "SYS_SET_OR_GET_SBP_INFO_REQ";
        case MIPC_SYS_SET_OR_GET_SBP_INFO_CNF: return "SYS_SET_OR_GET_SBP_INFO_CNF";
        case MIPC_SYS_GET_ALL_THERMAL_INFO_REQ: return "SYS_GET_ALL_THERMAL_INFO_REQ";
        case MIPC_SYS_GET_ALL_THERMAL_INFO_CNF: return "SYS_GET_ALL_THERMAL_INFO_CNF";
        case MIPC_SYS_SET_MD_LOG_FLUSH_INTERVAL_REQ: return "SYS_SET_MD_LOG_FLUSH_INTERVAL_REQ";
        case MIPC_SYS_SET_MD_LOG_FLUSH_INTERVAL_CNF: return "SYS_SET_MD_LOG_FLUSH_INTERVAL_CNF";
        case MIPC_SYS_GET_MD_LOG_FLUSH_INTERVAL_REQ: return "SYS_GET_MD_LOG_FLUSH_INTERVAL_REQ";
        case MIPC_SYS_GET_MD_LOG_FLUSH_INTERVAL_CNF: return "SYS_GET_MD_LOG_FLUSH_INTERVAL_CNF";
        case MIPC_SYS_TRIGGER_MD_LOG_FLUSH_REQ: return "SYS_TRIGGER_MD_LOG_FLUSH_REQ";
        case MIPC_SYS_TRIGGER_MD_LOG_FLUSH_CNF: return "SYS_TRIGGER_MD_LOG_FLUSH_CNF";
        case MIPC_SYS_GET_MD_LOG_FLUSH_STATUS_REQ: return "SYS_GET_MD_LOG_FLUSH_STATUS_REQ";
        case MIPC_SYS_GET_MD_LOG_FLUSH_STATUS_CNF: return "SYS_GET_MD_LOG_FLUSH_STATUS_CNF";
        case MIPC_APN_SET_IA_REQ: return "APN_SET_IA_REQ";
        case MIPC_APN_SET_IA_CNF: return "APN_SET_IA_CNF";
        case MIPC_APN_GET_IA_REQ: return "APN_GET_IA_REQ";
        case MIPC_APN_GET_IA_CNF: return "APN_GET_IA_CNF";
        case MIPC_APN_ADD_PROFILE_REQ: return "APN_ADD_PROFILE_REQ";
        case MIPC_APN_ADD_PROFILE_CNF: return "APN_ADD_PROFILE_CNF";
        case MIPC_APN_LIST_PROFILE_REQ: return "APN_LIST_PROFILE_REQ";
        case MIPC_APN_LIST_PROFILE_CNF: return "APN_LIST_PROFILE_CNF";
        case MIPC_APN_DEL_PROFILE_REQ: return "APN_DEL_PROFILE_REQ";
        case MIPC_APN_DEL_PROFILE_CNF: return "APN_DEL_PROFILE_CNF";
        case MIPC_APN_SET_PROFILE_STATUS_REQ: return "APN_SET_PROFILE_STATUS_REQ";
        case MIPC_APN_SET_PROFILE_STATUS_CNF: return "APN_SET_PROFILE_STATUS_CNF";
        case MIPC_APN_LIST_MD_PROFILE_REQ: return "APN_LIST_MD_PROFILE_REQ";
        case MIPC_APN_LIST_MD_PROFILE_CNF: return "APN_LIST_MD_PROFILE_CNF";
        case MIPC_APN_SET_VZW_APN_REQ: return "APN_SET_VZW_APN_REQ";
        case MIPC_APN_SET_VZW_APN_CNF: return "APN_SET_VZW_APN_CNF";
        case MIPC_APN_SET_VZW_APN_TIMER_REQ: return "APN_SET_VZW_APN_TIMER_REQ";
        case MIPC_APN_SET_VZW_APN_TIMER_CNF: return "APN_SET_VZW_APN_TIMER_CNF";
        case MIPC_APN_ADD_PROFILE_LIST_REQ: return "APN_ADD_PROFILE_LIST_REQ";
        case MIPC_APN_ADD_PROFILE_LIST_CNF: return "APN_ADD_PROFILE_LIST_CNF";
        case MIPC_APN_SET_IA_MD_PREFER_REQ: return "APN_SET_IA_MD_PREFER_REQ";
        case MIPC_APN_SET_IA_MD_PREFER_CNF: return "APN_SET_IA_MD_PREFER_CNF";
        case MIPC_DATA_ACT_CALL_REQ: return "DATA_ACT_CALL_REQ";
        case MIPC_DATA_ACT_CALL_CNF: return "DATA_ACT_CALL_CNF";
        case MIPC_DATA_DEACT_CALL_REQ: return "DATA_DEACT_CALL_REQ";
        case MIPC_DATA_DEACT_CALL_CNF: return "DATA_DEACT_CALL_CNF";
        case MIPC_DATA_GET_CALL_REQ: return "DATA_GET_CALL_REQ";
        case MIPC_DATA_GET_CALL_CNF: return "DATA_GET_CALL_CNF";
        case MIPC_DATA_SET_PACKET_FILTER_REQ: return "DATA_SET_PACKET_FILTER_REQ";
        case MIPC_DATA_SET_PACKET_FILTER_CNF: return "DATA_SET_PACKET_FILTER_CNF";
        case MIPC_DATA_GET_PACKET_FILTER_REQ: return "DATA_GET_PACKET_FILTER_REQ";
        case MIPC_DATA_GET_PACKET_FILTER_CNF: return "DATA_GET_PACKET_FILTER_CNF";
        case MIPC_DATA_GET_PCO_REQ: return "DATA_GET_PCO_REQ";
        case MIPC_DATA_GET_PCO_CNF: return "DATA_GET_PCO_CNF";
        case MIPC_DATA_SET_DATA_ALLOW_REQ: return "DATA_SET_DATA_ALLOW_REQ";
        case MIPC_DATA_SET_DATA_ALLOW_CNF: return "DATA_SET_DATA_ALLOW_CNF";
        case MIPC_DATA_GET_MD_DATA_CALL_LIST_REQ: return "DATA_GET_MD_DATA_CALL_LIST_REQ";
        case MIPC_DATA_GET_MD_DATA_CALL_LIST_CNF: return "DATA_GET_MD_DATA_CALL_LIST_CNF";
        case MIPC_DATA_SET_CONFIG_REQ: return "DATA_SET_CONFIG_REQ";
        case MIPC_DATA_SET_CONFIG_CNF: return "DATA_SET_CONFIG_CNF";
        case MIPC_DATA_GET_CONFIG_REQ: return "DATA_GET_CONFIG_REQ";
        case MIPC_DATA_GET_CONFIG_CNF: return "DATA_GET_CONFIG_CNF";
        case MIPC_DATA_ABORT_CALL_REQ: return "DATA_ABORT_CALL_REQ";
        case MIPC_DATA_ABORT_CALL_CNF: return "DATA_ABORT_CALL_CNF";
        case MIPC_DATA_GET_CALL_INFO_REQ: return "DATA_GET_CALL_INFO_REQ";
        case MIPC_DATA_GET_CALL_INFO_CNF: return "DATA_GET_CALL_INFO_CNF";
        case MIPC_DATA_GET_PDP_CID_REQ: return "DATA_GET_PDP_CID_REQ";
        case MIPC_DATA_GET_PDP_CID_CNF: return "DATA_GET_PDP_CID_CNF";
        case MIPC_DATA_RETRY_TIMER_REQ: return "DATA_RETRY_TIMER_REQ";
        case MIPC_DATA_RETRY_TIMER_CNF: return "DATA_RETRY_TIMER_CNF";
        case MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ: return "DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_REQ";
        case MIPC_DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_CNF: return "DATA_SET_LINK_CAPACITY_REPORTING_CRITERIA_CNF";
        case MIPC_DATA_GET_DEDICATE_BEARER_INFO_REQ: return "DATA_GET_DEDICATE_BEARER_INFO_REQ";
        case MIPC_DATA_GET_DEDICATE_BEARER_INFO_CNF: return "DATA_GET_DEDICATE_BEARER_INFO_CNF";
        case MIPC_DATA_GET_QOS_REQ: return "DATA_GET_QOS_REQ";
        case MIPC_DATA_GET_QOS_CNF: return "DATA_GET_QOS_CNF";
        case MIPC_DATA_GET_TFT_REQ: return "DATA_GET_TFT_REQ";
        case MIPC_DATA_GET_TFT_CNF: return "DATA_GET_TFT_CNF";
        case MIPC_DATA_SET_LGDCONT_REQ: return "DATA_SET_LGDCONT_REQ";
        case MIPC_DATA_SET_LGDCONT_CNF: return "DATA_SET_LGDCONT_CNF";
        case MIPC_DATA_SET_NSSAI_REQ: return "DATA_SET_NSSAI_REQ";
        case MIPC_DATA_SET_NSSAI_CNF: return "DATA_SET_NSSAI_CNF";
        case MIPC_DATA_KEEPALIVE_REQ: return "DATA_KEEPALIVE_REQ";
        case MIPC_DATA_KEEPALIVE_CNF: return "DATA_KEEPALIVE_CNF";
        case MIPC_DATA_GET_DSDA_STATE_REQ: return "DATA_GET_DSDA_STATE_REQ";
        case MIPC_DATA_GET_DSDA_STATE_CNF: return "DATA_GET_DSDA_STATE_CNF";
        case MIPC_DATA_GET_5GQOS_REQ: return "DATA_GET_5GQOS_REQ";
        case MIPC_DATA_GET_5GQOS_CNF: return "DATA_GET_5GQOS_CNF";
        case MIPC_DATA_SET_PSI_REQ: return "DATA_SET_PSI_REQ";
        case MIPC_DATA_SET_PSI_CNF: return "DATA_SET_PSI_CNF";
        case MIPC_DATA_GET_NSSAI_REQ: return "DATA_GET_NSSAI_REQ";
        case MIPC_DATA_GET_NSSAI_CNF: return "DATA_GET_NSSAI_CNF";
        case MIPC_DATA_GET_URSP_ROUTE_PROFILE_REQ: return "DATA_GET_URSP_ROUTE_PROFILE_REQ";
        case MIPC_DATA_GET_URSP_ROUTE_PROFILE_CNF: return "DATA_GET_URSP_ROUTE_PROFILE_CNF";
        case MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_REQ: return "DATA_SET_URSP_PRECONF_UE_POLICY_REQ";
        case MIPC_DATA_SET_URSP_PRECONF_UE_POLICY_CNF: return "DATA_SET_URSP_PRECONF_UE_POLICY_CNF";
        case MIPC_DATA_GET_URSP_UE_POLICY_REQ: return "DATA_GET_URSP_UE_POLICY_REQ";
        case MIPC_DATA_GET_URSP_UE_POLICY_CNF: return "DATA_GET_URSP_UE_POLICY_CNF";
        case MIPC_INTERNAL_OPEN_REQ: return "INTERNAL_OPEN_REQ";
        case MIPC_INTERNAL_OPEN_CNF: return "INTERNAL_OPEN_CNF";
        case MIPC_INTERNAL_CLOSE_REQ: return "INTERNAL_CLOSE_REQ";
        case MIPC_INTERNAL_CLOSE_CNF: return "INTERNAL_CLOSE_CNF";
        case MIPC_INTERNAL_TEST_REQ: return "INTERNAL_TEST_REQ";
        case MIPC_INTERNAL_TEST_CNF: return "INTERNAL_TEST_CNF";
        case MIPC_INTERNAL_REGISTER_IND_REQ: return "INTERNAL_REGISTER_IND_REQ";
        case MIPC_INTERNAL_REGISTER_IND_CNF: return "INTERNAL_REGISTER_IND_CNF";
        case MIPC_INTERNAL_UNREGISTER_IND_REQ: return "INTERNAL_UNREGISTER_IND_REQ";
        case MIPC_INTERNAL_UNREGISTER_IND_CNF: return "INTERNAL_UNREGISTER_IND_CNF";
        case MIPC_INTERNAL_REGISTER_CMD_REQ: return "INTERNAL_REGISTER_CMD_REQ";
        case MIPC_INTERNAL_REGISTER_CMD_CNF: return "INTERNAL_REGISTER_CMD_CNF";
        case MIPC_INTERNAL_UNREGISTER_CMD_REQ: return "INTERNAL_UNREGISTER_CMD_REQ";
        case MIPC_INTERNAL_UNREGISTER_CMD_CNF: return "INTERNAL_UNREGISTER_CMD_CNF";
        case MIPC_INTERNAL_SET_FILTER_REQ: return "INTERNAL_SET_FILTER_REQ";
        case MIPC_INTERNAL_SET_FILTER_CNF: return "INTERNAL_SET_FILTER_CNF";
        case MIPC_INTERNAL_RESET_FILTER_REQ: return "INTERNAL_RESET_FILTER_REQ";
        case MIPC_INTERNAL_RESET_FILTER_CNF: return "INTERNAL_RESET_FILTER_CNF";
        case MIPC_INTERNAL_EIF_REQ: return "INTERNAL_EIF_REQ";
        case MIPC_INTERNAL_EIF_CNF: return "INTERNAL_EIF_CNF";
        case MIPC_NW_GET_RADIO_STATE_REQ: return "NW_GET_RADIO_STATE_REQ";
        case MIPC_NW_GET_RADIO_STATE_CNF: return "NW_GET_RADIO_STATE_CNF";
        case MIPC_NW_SET_RADIO_STATE_REQ: return "NW_SET_RADIO_STATE_REQ";
        case MIPC_NW_SET_RADIO_STATE_CNF: return "NW_SET_RADIO_STATE_CNF";
        case MIPC_NW_SET_REGISTER_STATE_REQ: return "NW_SET_REGISTER_STATE_REQ";
        case MIPC_NW_SET_REGISTER_STATE_CNF: return "NW_SET_REGISTER_STATE_CNF";
        case MIPC_NW_GET_REGISTER_STATE_REQ: return "NW_GET_REGISTER_STATE_REQ";
        case MIPC_NW_GET_REGISTER_STATE_CNF: return "NW_GET_REGISTER_STATE_CNF";
        case MIPC_NW_GET_PLMN_LIST_REQ: return "NW_GET_PLMN_LIST_REQ";
        case MIPC_NW_GET_PLMN_LIST_CNF: return "NW_GET_PLMN_LIST_CNF";
        case MIPC_NW_SET_PS_REQ: return "NW_SET_PS_REQ";
        case MIPC_NW_SET_PS_CNF: return "NW_SET_PS_CNF";
        case MIPC_NW_GET_PS_REQ: return "NW_GET_PS_REQ";
        case MIPC_NW_GET_PS_CNF: return "NW_GET_PS_CNF";
        case MIPC_NW_SET_SIGNAL_REQ: return "NW_SET_SIGNAL_REQ";
        case MIPC_NW_SET_SIGNAL_CNF: return "NW_SET_SIGNAL_CNF";
        case MIPC_NW_GET_SIGNAL_REQ: return "NW_GET_SIGNAL_REQ";
        case MIPC_NW_GET_SIGNAL_CNF: return "NW_GET_SIGNAL_CNF";
        case MIPC_NW_GET_PREFERRED_PROVIDER_REQ: return "NW_GET_PREFERRED_PROVIDER_REQ";
        case MIPC_NW_GET_PREFERRED_PROVIDER_CNF: return "NW_GET_PREFERRED_PROVIDER_CNF";
        case MIPC_NW_SET_PREFERRED_PROVIDER_REQ: return "NW_SET_PREFERRED_PROVIDER_REQ";
        case MIPC_NW_SET_PREFERRED_PROVIDER_CNF: return "NW_SET_PREFERRED_PROVIDER_CNF";
        case MIPC_NW_SET_HOME_PROVIDER_REQ: return "NW_SET_HOME_PROVIDER_REQ";
        case MIPC_NW_SET_HOME_PROVIDER_CNF: return "NW_SET_HOME_PROVIDER_CNF";
        case MIPC_NW_GET_HOME_PROVIDER_REQ: return "NW_GET_HOME_PROVIDER_REQ";
        case MIPC_NW_GET_HOME_PROVIDER_CNF: return "NW_GET_HOME_PROVIDER_CNF";
        case MIPC_NW_GET_IA_STATUS_REQ: return "NW_GET_IA_STATUS_REQ";
        case MIPC_NW_GET_IA_STATUS_CNF: return "NW_GET_IA_STATUS_CNF";
        case MIPC_NW_GET_NITZ_REQ: return "NW_GET_NITZ_REQ";
        case MIPC_NW_GET_NITZ_CNF: return "NW_GET_NITZ_CNF";
        case MIPC_NW_SET_IDLE_HINT_REQ: return "NW_SET_IDLE_HINT_REQ";
        case MIPC_NW_SET_IDLE_HINT_CNF: return "NW_SET_IDLE_HINT_CNF";
        case MIPC_NW_GET_IDLE_HINT_REQ: return "NW_GET_IDLE_HINT_REQ";
        case MIPC_NW_GET_IDLE_HINT_CNF: return "NW_GET_IDLE_HINT_CNF";
        case MIPC_NW_GET_BASE_STATIONS_REQ: return "NW_GET_BASE_STATIONS_REQ";
        case MIPC_NW_GET_BASE_STATIONS_CNF: return "NW_GET_BASE_STATIONS_CNF";
        case MIPC_NW_GET_LOCATION_INFO_REQ: return "NW_GET_LOCATION_INFO_REQ";
        case MIPC_NW_GET_LOCATION_INFO_CNF: return "NW_GET_LOCATION_INFO_CNF";
        case MIPC_NW_SET_RAT_REQ: return "NW_SET_RAT_REQ";
        case MIPC_NW_SET_RAT_CNF: return "NW_SET_RAT_CNF";
        case MIPC_NW_GET_PROVIDER_NAME_REQ: return "NW_GET_PROVIDER_NAME_REQ";
        case MIPC_NW_GET_PROVIDER_NAME_CNF: return "NW_GET_PROVIDER_NAME_CNF";
        case MIPC_NW_GET_RAT_REQ: return "NW_GET_RAT_REQ";
        case MIPC_NW_GET_RAT_CNF: return "NW_GET_RAT_CNF";
        case MIPC_NW_SET_NR_REQ: return "NW_SET_NR_REQ";
        case MIPC_NW_SET_NR_CNF: return "NW_SET_NR_CNF";
        case MIPC_NW_GET_CS_REQ: return "NW_GET_CS_REQ";
        case MIPC_NW_GET_CS_CNF: return "NW_GET_CS_CNF";
        case MIPC_NW_GET_BAND_MODE_REQ: return "NW_GET_BAND_MODE_REQ";
        case MIPC_NW_GET_BAND_MODE_CNF: return "NW_GET_BAND_MODE_CNF";
        case MIPC_NW_SET_BAND_MODE_REQ: return "NW_SET_BAND_MODE_REQ";
        case MIPC_NW_SET_BAND_MODE_CNF: return "NW_SET_BAND_MODE_CNF";
        case MIPC_NW_SET_CHANNEL_LOCK_REQ: return "NW_SET_CHANNEL_LOCK_REQ";
        case MIPC_NW_SET_CHANNEL_LOCK_CNF: return "NW_SET_CHANNEL_LOCK_CNF";
        case MIPC_NW_GET_CHANNEL_LOCK_REQ: return "NW_GET_CHANNEL_LOCK_REQ";
        case MIPC_NW_GET_CHANNEL_LOCK_CNF: return "NW_GET_CHANNEL_LOCK_CNF";
        case MIPC_NW_SET_ABORT_PLMN_LIST_SEARCH_REQ: return "NW_SET_ABORT_PLMN_LIST_SEARCH_REQ";
        case MIPC_NW_SET_ABORT_PLMN_LIST_SEARCH_CNF: return "NW_SET_ABORT_PLMN_LIST_SEARCH_CNF";
        case MIPC_NW_GET_POL_CAPABILITY_REQ: return "NW_GET_POL_CAPABILITY_REQ";
        case MIPC_NW_GET_POL_CAPABILITY_CNF: return "NW_GET_POL_CAPABILITY_CNF";
        case MIPC_NW_SET_PREFER_RAT_REQ: return "NW_SET_PREFER_RAT_REQ";
        case MIPC_NW_SET_PREFER_RAT_CNF: return "NW_SET_PREFER_RAT_CNF";
        case MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ: return "NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_REQ";
        case MIPC_NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_CNF: return "NW_SET_LTE_CARRIER_AGGREGATION_SWITCH_CNF";
        case MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ: return "NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_REQ";
        case MIPC_NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_CNF: return "NW_SET_PS_CS_REGISTRATION_STATE_ROAMING_TYPE_CNF";
        case MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_REQ: return "NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_REQ";
        case MIPC_NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF: return "NW_GET_LTE_CARRIER_AGGREGATION_SWITCH_CNF";
        case MIPC_NW_CELL_MEASUREMENT_REQ: return "NW_CELL_MEASUREMENT_REQ";
        case MIPC_NW_CELL_MEASUREMENT_CNF: return "NW_CELL_MEASUREMENT_CNF";
        case MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_REQ: return "NW_CELL_BAND_WHITE_LIST_LOCK_REQ";
        case MIPC_NW_CELL_BAND_WHITE_LIST_LOCK_CNF: return "NW_CELL_BAND_WHITE_LIST_LOCK_CNF";
        case MIPC_NW_GET_CELL_BAND_BANDWIDTH_REQ: return "NW_GET_CELL_BAND_BANDWIDTH_REQ";
        case MIPC_NW_GET_CELL_BAND_BANDWIDTH_CNF: return "NW_GET_CELL_BAND_BANDWIDTH_CNF";
        case MIPC_NW_GET_NR_REQ: return "NW_GET_NR_REQ";
        case MIPC_NW_GET_NR_CNF: return "NW_GET_NR_CNF";
        case MIPC_NW_GET_SRXLEV_REQ: return "NW_GET_SRXLEV_REQ";
        case MIPC_NW_GET_SRXLEV_CNF: return "NW_GET_SRXLEV_CNF";
        case MIPC_NW_SET_ROAMING_MODE_REQ: return "NW_SET_ROAMING_MODE_REQ";
        case MIPC_NW_SET_ROAMING_MODE_CNF: return "NW_SET_ROAMING_MODE_CNF";
        case MIPC_NW_GET_ROAMING_MODE_REQ: return "NW_GET_ROAMING_MODE_REQ";
        case MIPC_NW_GET_ROAMING_MODE_CNF: return "NW_GET_ROAMING_MODE_CNF";
        case MIPC_NW_SET_URC_ENABLE_REQ: return "NW_SET_URC_ENABLE_REQ";
        case MIPC_NW_SET_URC_ENABLE_CNF: return "NW_SET_URC_ENABLE_CNF";
        case MIPC_NW_GET_SUGGESTED_PLMN_LIST_REQ: return "NW_GET_SUGGESTED_PLMN_LIST_REQ";
        case MIPC_NW_GET_SUGGESTED_PLMN_LIST_CNF: return "NW_GET_SUGGESTED_PLMN_LIST_CNF";
        case MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ: return "NW_SET_SEARCH_STORE_FREQUENCY_INFO_REQ";
        case MIPC_NW_SET_SEARCH_STORE_FREQUENCY_INFO_CNF: return "NW_SET_SEARCH_STORE_FREQUENCY_INFO_CNF";
        case MIPC_NW_SET_SELECT_FEMTOCELL_REQ: return "NW_SET_SELECT_FEMTOCELL_REQ";
        case MIPC_NW_SET_SELECT_FEMTOCELL_CNF: return "NW_SET_SELECT_FEMTOCELL_CNF";
        case MIPC_NW_SET_CONFIG_A2_OFFSET_REQ: return "NW_SET_CONFIG_A2_OFFSET_REQ";
        case MIPC_NW_SET_CONFIG_A2_OFFSET_CNF: return "NW_SET_CONFIG_A2_OFFSET_CNF";
        case MIPC_NW_SET_CONFIG_B1_OFFSET_REQ: return "NW_SET_CONFIG_B1_OFFSET_REQ";
        case MIPC_NW_SET_CONFIG_B1_OFFSET_CNF: return "NW_SET_CONFIG_B1_OFFSET_CNF";
        case MIPC_NW_SET_REPORT_ANBR_REQ: return "NW_SET_REPORT_ANBR_REQ";
        case MIPC_NW_SET_REPORT_ANBR_CNF: return "NW_SET_REPORT_ANBR_CNF";
        case MIPC_NW_SET_NETWORK_EVENT_REQ: return "NW_SET_NETWORK_EVENT_REQ";
        case MIPC_NW_SET_NETWORK_EVENT_CNF: return "NW_SET_NETWORK_EVENT_CNF";
        case MIPC_NW_SET_ENABLE_CA_PLUS_FILTER_REQ: return "NW_SET_ENABLE_CA_PLUS_FILTER_REQ";
        case MIPC_NW_SET_ENABLE_CA_PLUS_FILTER_CNF: return "NW_SET_ENABLE_CA_PLUS_FILTER_CNF";
        case MIPC_NW_ABORT_FEMTOCELL_LIST_REQ: return "NW_ABORT_FEMTOCELL_LIST_REQ";
        case MIPC_NW_ABORT_FEMTOCELL_LIST_CNF: return "NW_ABORT_FEMTOCELL_LIST_CNF";
        case MIPC_NW_GET_FEMTOCELL_LIST_REQ: return "NW_GET_FEMTOCELL_LIST_REQ";
        case MIPC_NW_GET_FEMTOCELL_LIST_CNF: return "NW_GET_FEMTOCELL_LIST_CNF";
        case MIPC_NW_SET_PSEUDO_CELL_MODE_REQ: return "NW_SET_PSEUDO_CELL_MODE_REQ";
        case MIPC_NW_SET_PSEUDO_CELL_MODE_CNF: return "NW_SET_PSEUDO_CELL_MODE_CNF";
        case MIPC_NW_GET_PSEUDO_CELL_INFO_REQ: return "NW_GET_PSEUDO_CELL_INFO_REQ";
        case MIPC_NW_GET_PSEUDO_CELL_INFO_CNF: return "NW_GET_PSEUDO_CELL_INFO_CNF";
        case MIPC_NW_SET_ROAMING_ENABLE_REQ: return "NW_SET_ROAMING_ENABLE_REQ";
        case MIPC_NW_SET_ROAMING_ENABLE_CNF: return "NW_SET_ROAMING_ENABLE_CNF";
        case MIPC_NW_GET_ROAMING_ENABLE_REQ: return "NW_GET_ROAMING_ENABLE_REQ";
        case MIPC_NW_GET_ROAMING_ENABLE_CNF: return "NW_GET_ROAMING_ENABLE_CNF";
        case MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ: return "NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ";
        case MIPC_NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF: return "NW_SET_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF";
        case MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ: return "NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_REQ";
        case MIPC_NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF: return "NW_QUERY_FEMTOCELL_SYSTEM_SELECTION_MODE_CNF";
        case MIPC_NW_SET_NW_IND_REPORT_LEVEL_REQ: return "NW_SET_NW_IND_REPORT_LEVEL_REQ";
        case MIPC_NW_SET_NW_IND_REPORT_LEVEL_CNF: return "NW_SET_NW_IND_REPORT_LEVEL_CNF";
        case MIPC_NW_SET_DISABLE_2G_REQ: return "NW_SET_DISABLE_2G_REQ";
        case MIPC_NW_SET_DISABLE_2G_CNF: return "NW_SET_DISABLE_2G_CNF";
        case MIPC_NW_GET_DISABLE_2G_REQ: return "NW_GET_DISABLE_2G_REQ";
        case MIPC_NW_GET_DISABLE_2G_CNF: return "NW_GET_DISABLE_2G_CNF";
        case MIPC_NW_SET_SMART_RAT_SWITCH_REQ: return "NW_SET_SMART_RAT_SWITCH_REQ";
        case MIPC_NW_SET_SMART_RAT_SWITCH_CNF: return "NW_SET_SMART_RAT_SWITCH_CNF";
        case MIPC_NW_GET_SMART_RAT_SWITCH_REQ: return "NW_GET_SMART_RAT_SWITCH_REQ";
        case MIPC_NW_GET_SMART_RAT_SWITCH_CNF: return "NW_GET_SMART_RAT_SWITCH_CNF";
        case MIPC_NW_VSS_ANTENNA_CONF_REQ: return "NW_VSS_ANTENNA_CONF_REQ";
        case MIPC_NW_VSS_ANTENNA_CONF_CNF: return "NW_VSS_ANTENNA_CONF_CNF";
        case MIPC_NW_VSS_ANTENNA_INFO_REQ: return "NW_VSS_ANTENNA_INFO_REQ";
        case MIPC_NW_VSS_ANTENNA_INFO_CNF: return "NW_VSS_ANTENNA_INFO_CNF";
        case MIPC_NW_SET_RADIO_CAPABILITY_REQ: return "NW_SET_RADIO_CAPABILITY_REQ";
        case MIPC_NW_SET_RADIO_CAPABILITY_CNF: return "NW_SET_RADIO_CAPABILITY_CNF";
        case MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_REQ: return "NW_SET_CDMA_ROAMING_PREFERENCE_REQ";
        case MIPC_NW_SET_CDMA_ROAMING_PREFERENCE_CNF: return "NW_SET_CDMA_ROAMING_PREFERENCE_CNF";
        case MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_REQ: return "NW_GET_CDMA_ROAMING_PREFERENCE_REQ";
        case MIPC_NW_GET_CDMA_ROAMING_PREFERENCE_CNF: return "NW_GET_CDMA_ROAMING_PREFERENCE_CNF";
        case MIPC_NW_GET_BARRING_INFO_REQ: return "NW_GET_BARRING_INFO_REQ";
        case MIPC_NW_GET_BARRING_INFO_CNF: return "NW_GET_BARRING_INFO_CNF";
        case MIPC_NW_GET_EHRPD_INFO_REQ: return "NW_GET_EHRPD_INFO_REQ";
        case MIPC_NW_GET_EHRPD_INFO_CNF: return "NW_GET_EHRPD_INFO_CNF";
        case MIPC_NW_GET_EGMSS_REQ: return "NW_GET_EGMSS_REQ";
        case MIPC_NW_GET_EGMSS_CNF: return "NW_GET_EGMSS_CNF";
        case MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_REQ: return "NW_SET_CACHE_ENDC_CONNECT_MODE_REQ";
        case MIPC_NW_SET_CACHE_ENDC_CONNECT_MODE_CNF: return "NW_SET_CACHE_ENDC_CONNECT_MODE_CNF";
        case MIPC_NW_SET_PS_TEST_MODE_REQ: return "NW_SET_PS_TEST_MODE_REQ";
        case MIPC_NW_SET_PS_TEST_MODE_CNF: return "NW_SET_PS_TEST_MODE_CNF";
        case MIPC_NW_GET_PS_TEST_MODE_REQ: return "NW_GET_PS_TEST_MODE_REQ";
        case MIPC_NW_GET_PS_TEST_MODE_CNF: return "NW_GET_PS_TEST_MODE_CNF";
        case MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_REQ: return "NW_SET_SIGNAL_REPORT_CRITERIA_REQ";
        case MIPC_NW_SET_SIGNAL_REPORT_CRITERIA_CNF: return "NW_SET_SIGNAL_REPORT_CRITERIA_CNF";
        case MIPC_NW_GET_ECAINFO_REQ: return "NW_GET_ECAINFO_REQ";
        case MIPC_NW_GET_ECAINFO_CNF: return "NW_GET_ECAINFO_CNF";
        case MIPC_NW_GET_ACTIVITY_INFO_REQ: return "NW_GET_ACTIVITY_INFO_REQ";
        case MIPC_NW_GET_ACTIVITY_INFO_CNF: return "NW_GET_ACTIVITY_INFO_CNF";
        case MIPC_NW_SET_CA_REQ: return "NW_SET_CA_REQ";
        case MIPC_NW_SET_CA_CNF: return "NW_SET_CA_CNF";
        case MIPC_NW_GET_LTE_RRC_STATE_REQ: return "NW_GET_LTE_RRC_STATE_REQ";
        case MIPC_NW_GET_LTE_RRC_STATE_CNF: return "NW_GET_LTE_RRC_STATE_CNF";
        case MIPC_NW_GET_LTE_1XRTT_CELL_LIST_REQ: return "NW_GET_LTE_1XRTT_CELL_LIST_REQ";
        case MIPC_NW_GET_LTE_1XRTT_CELL_LIST_CNF: return "NW_GET_LTE_1XRTT_CELL_LIST_CNF";
        case MIPC_NW_CLEAR_LTE_AVAILABLE_FILE_REQ: return "NW_CLEAR_LTE_AVAILABLE_FILE_REQ";
        case MIPC_NW_CLEAR_LTE_AVAILABLE_FILE_CNF: return "NW_CLEAR_LTE_AVAILABLE_FILE_CNF";
        case MIPC_NW_GET_CA_LINK_CAPABILITY_REQ: return "NW_GET_CA_LINK_CAPABILITY_REQ";
        case MIPC_NW_GET_CA_LINK_CAPABILITY_CNF: return "NW_GET_CA_LINK_CAPABILITY_CNF";
        case MIPC_NW_GET_CA_LINK_ENABLE_STATUS_REQ: return "NW_GET_CA_LINK_ENABLE_STATUS_REQ";
        case MIPC_NW_GET_CA_LINK_ENABLE_STATUS_CNF: return "NW_GET_CA_LINK_ENABLE_STATUS_CNF";
        case MIPC_NW_GET_TM9_ENABLE_STATUS_REQ: return "NW_GET_TM9_ENABLE_STATUS_REQ";
        case MIPC_NW_GET_TM9_ENABLE_STATUS_CNF: return "NW_GET_TM9_ENABLE_STATUS_CNF";
        case MIPC_NW_SET_TM9_ENABLE_STATUS_REQ: return "NW_SET_TM9_ENABLE_STATUS_REQ";
        case MIPC_NW_SET_TM9_ENABLE_STATUS_CNF: return "NW_SET_TM9_ENABLE_STATUS_CNF";
        case MIPC_NW_GET_OMADM_CONF_REQ: return "NW_GET_OMADM_CONF_REQ";
        case MIPC_NW_GET_OMADM_CONF_CNF: return "NW_GET_OMADM_CONF_CNF";
        case MIPC_NW_SET_OMADM_CONF_REQ: return "NW_SET_OMADM_CONF_REQ";
        case MIPC_NW_SET_OMADM_CONF_CNF: return "NW_SET_OMADM_CONF_CNF";
        case MIPC_NW_GET_CA_BAND_MODE_REQ: return "NW_GET_CA_BAND_MODE_REQ";
        case MIPC_NW_GET_CA_BAND_MODE_CNF: return "NW_GET_CA_BAND_MODE_CNF";
        case MIPC_NW_SET_CA_LINK_ENABLE_STATUS_REQ: return "NW_SET_CA_LINK_ENABLE_STATUS_REQ";
        case MIPC_NW_SET_CA_LINK_ENABLE_STATUS_CNF: return "NW_SET_CA_LINK_ENABLE_STATUS_CNF";
        case MIPC_NW_GET_LTE_DATA_REQ: return "NW_GET_LTE_DATA_REQ";
        case MIPC_NW_GET_LTE_DATA_CNF: return "NW_GET_LTE_DATA_CNF";
        case MIPC_NW_SET_TUW_TIMER_LENGTH_REQ: return "NW_SET_TUW_TIMER_LENGTH_REQ";
        case MIPC_NW_SET_TUW_TIMER_LENGTH_CNF: return "NW_SET_TUW_TIMER_LENGTH_CNF";
        case MIPC_NW_GET_TUW_TIMER_LENGTH_REQ: return "NW_GET_TUW_TIMER_LENGTH_REQ";
        case MIPC_NW_GET_TUW_TIMER_LENGTH_CNF: return "NW_GET_TUW_TIMER_LENGTH_CNF";
        case MIPC_NW_GET_5GUW_INFO_REQ: return "NW_GET_5GUW_INFO_REQ";
        case MIPC_NW_GET_5GUW_INFO_CNF: return "NW_GET_5GUW_INFO_CNF";
        case MIPC_NW_GET_NR_CA_BAND_REQ: return "NW_GET_NR_CA_BAND_REQ";
        case MIPC_NW_GET_NR_CA_BAND_CNF: return "NW_GET_NR_CA_BAND_CNF";
        case MIPC_NW_GET_NR_SCS_REQ: return "NW_GET_NR_SCS_REQ";
        case MIPC_NW_GET_NR_SCS_CNF: return "NW_GET_NR_SCS_CNF";
        case MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_REQ: return "NW_GET_PHYSICAL_CHANNEL_CONFIGS_REQ";
        case MIPC_NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF: return "NW_GET_PHYSICAL_CHANNEL_CONFIGS_CNF";
        case MIPC_NW_OS_ID_UPDATE_REQ: return "NW_OS_ID_UPDATE_REQ";
        case MIPC_NW_OS_ID_UPDATE_CNF: return "NW_OS_ID_UPDATE_CNF";
        case MIPC_NW_START_NETWORK_SCAN_REQ: return "NW_START_NETWORK_SCAN_REQ";
        case MIPC_NW_START_NETWORK_SCAN_CNF: return "NW_START_NETWORK_SCAN_CNF";
        case MIPC_NW_STOP_NETWORK_SCAN_REQ: return "NW_STOP_NETWORK_SCAN_REQ";
        case MIPC_NW_STOP_NETWORK_SCAN_CNF: return "NW_STOP_NETWORK_SCAN_CNF";
        case MIPC_NW_GET_NETWORK_SCAN_SUPPORT_STATUS_REQ: return "NW_GET_NETWORK_SCAN_SUPPORT_STATUS_REQ";
        case MIPC_NW_GET_NETWORK_SCAN_SUPPORT_STATUS_CNF: return "NW_GET_NETWORK_SCAN_SUPPORT_STATUS_CNF";
        case MIPC_NW_SET_5GUC_REQ: return "NW_SET_5GUC_REQ";
        case MIPC_NW_SET_5GUC_CNF: return "NW_SET_5GUC_CNF";
        case MIPC_NW_GET_5GUC_SETTING_REQ: return "NW_GET_5GUC_SETTING_REQ";
        case MIPC_NW_GET_5GUC_SETTING_CNF: return "NW_GET_5GUC_SETTING_CNF";
        case MIPC_NW_GET_5GUC_INFO_REQ: return "NW_GET_5GUC_INFO_REQ";
        case MIPC_NW_GET_5GUC_INFO_CNF: return "NW_GET_5GUC_INFO_CNF";
        case MIPC_NW_GET_NSSAI_REQ: return "NW_GET_NSSAI_REQ";
        case MIPC_NW_GET_NSSAI_CNF: return "NW_GET_NSSAI_CNF";
        case MIPC_NW_GET_FIRST_PLMN_REQ: return "NW_GET_FIRST_PLMN_REQ";
        case MIPC_NW_GET_FIRST_PLMN_CNF: return "NW_GET_FIRST_PLMN_CNF";
        case MIPC_NW_SET_UE_USAGE_SETTING_REQ: return "NW_SET_UE_USAGE_SETTING_REQ";
        case MIPC_NW_SET_UE_USAGE_SETTING_CNF: return "NW_SET_UE_USAGE_SETTING_CNF";
        case MIPC_NW_GET_UE_USAGE_SETTING_REQ: return "NW_GET_UE_USAGE_SETTING_REQ";
        case MIPC_NW_GET_UE_USAGE_SETTING_CNF: return "NW_GET_UE_USAGE_SETTING_CNF";
        case MIPC_NW_SET_CAP_NRCA_OPTION_REQ: return "NW_SET_CAP_NRCA_OPTION_REQ";
        case MIPC_NW_SET_CAP_NRCA_OPTION_CNF: return "NW_SET_CAP_NRCA_OPTION_CNF";
        case MIPC_NW_SET_CAP_BC_LIST_REQ: return "NW_SET_CAP_BC_LIST_REQ";
        case MIPC_NW_SET_CAP_BC_LIST_CNF: return "NW_SET_CAP_BC_LIST_CNF";
        case MIPC_NW_GET_ENWCFGINFO_REQ: return "NW_GET_ENWCFGINFO_REQ";
        case MIPC_NW_GET_ENWCFGINFO_CNF: return "NW_GET_ENWCFGINFO_CNF";
        case MIPC_NW_GET_CSCON_STATE_REQ: return "NW_GET_CSCON_STATE_REQ";
        case MIPC_NW_GET_CSCON_STATE_CNF: return "NW_GET_CSCON_STATE_CNF";
        case MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ: return "NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_REQ";
        case MIPC_NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_CNF: return "NW_SET_ALLOWED_SECURITY_ALGO_CONFIG_CNF";
        case MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_REQ: return "NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_REQ";
        case MIPC_NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF: return "NW_GET_ALLOWED_SECURITY_ALGO_CONFIG_CNF";
        case MIPC_NW_SET_CONFIG_REQ: return "NW_SET_CONFIG_REQ";
        case MIPC_NW_SET_CONFIG_CNF: return "NW_SET_CONFIG_CNF";
        case MIPC_NW_GET_CONFIG_REQ: return "NW_GET_CONFIG_REQ";
        case MIPC_NW_GET_CONFIG_CNF: return "NW_GET_CONFIG_CNF";
        case MIPC_SIM_PIN_PROTECT_REQ: return "SIM_PIN_PROTECT_REQ";
        case MIPC_SIM_PIN_PROTECT_CNF: return "SIM_PIN_PROTECT_CNF";
        case MIPC_SIM_CHANGE_PIN_REQ: return "SIM_CHANGE_PIN_REQ";
        case MIPC_SIM_CHANGE_PIN_CNF: return "SIM_CHANGE_PIN_CNF";
        case MIPC_SIM_VERIFY_PIN_REQ: return "SIM_VERIFY_PIN_REQ";
        case MIPC_SIM_VERIFY_PIN_CNF: return "SIM_VERIFY_PIN_CNF";
        case MIPC_SIM_UNBLOCK_PIN_REQ: return "SIM_UNBLOCK_PIN_REQ";
        case MIPC_SIM_UNBLOCK_PIN_CNF: return "SIM_UNBLOCK_PIN_CNF";
        case MIPC_SIM_GET_PIN_INFO_REQ: return "SIM_GET_PIN_INFO_REQ";
        case MIPC_SIM_GET_PIN_INFO_CNF: return "SIM_GET_PIN_INFO_CNF";
        case MIPC_SIM_GET_PIN_LIST_REQ: return "SIM_GET_PIN_LIST_REQ";
        case MIPC_SIM_GET_PIN_LIST_CNF: return "SIM_GET_PIN_LIST_CNF";
        case MIPC_SIM_STATE_REQ: return "SIM_STATE_REQ";
        case MIPC_SIM_STATE_CNF: return "SIM_STATE_CNF";
        case MIPC_SIM_STATUS_REQ: return "SIM_STATUS_REQ";
        case MIPC_SIM_STATUS_CNF: return "SIM_STATUS_CNF";
        case MIPC_SIM_ICCID_REQ: return "SIM_ICCID_REQ";
        case MIPC_SIM_ICCID_CNF: return "SIM_ICCID_CNF";
        case MIPC_SIM_IMSI_REQ: return "SIM_IMSI_REQ";
        case MIPC_SIM_IMSI_CNF: return "SIM_IMSI_CNF";
        case MIPC_SIM_MSISDN_REQ: return "SIM_MSISDN_REQ";
        case MIPC_SIM_MSISDN_CNF: return "SIM_MSISDN_CNF";
        case MIPC_SIM_GET_ATR_INFO_REQ: return "SIM_GET_ATR_INFO_REQ";
        case MIPC_SIM_GET_ATR_INFO_CNF: return "SIM_GET_ATR_INFO_CNF";
        case MIPC_SIM_OPEN_CHANNEL_REQ: return "SIM_OPEN_CHANNEL_REQ";
        case MIPC_SIM_OPEN_CHANNEL_CNF: return "SIM_OPEN_CHANNEL_CNF";
        case MIPC_SIM_CLOSE_CHANNEL_REQ: return "SIM_CLOSE_CHANNEL_REQ";
        case MIPC_SIM_CLOSE_CHANNEL_CNF: return "SIM_CLOSE_CHANNEL_CNF";
        case MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_REQ: return "SIM_CHANNEL_RESTRICTED_ACCESS_REQ";
        case MIPC_SIM_CHANNEL_RESTRICTED_ACCESS_CNF: return "SIM_CHANNEL_RESTRICTED_ACCESS_CNF";
        case MIPC_SIM_CHANNEL_GENERIC_ACCESS_REQ: return "SIM_CHANNEL_GENERIC_ACCESS_REQ";
        case MIPC_SIM_CHANNEL_GENERIC_ACCESS_CNF: return "SIM_CHANNEL_GENERIC_ACCESS_CNF";
        case MIPC_SIM_LONG_APDU_ACCESS_REQ: return "SIM_LONG_APDU_ACCESS_REQ";
        case MIPC_SIM_LONG_APDU_ACCESS_CNF: return "SIM_LONG_APDU_ACCESS_CNF";
        case MIPC_SIM_APP_LIST_REQ: return "SIM_APP_LIST_REQ";
        case MIPC_SIM_APP_LIST_CNF: return "SIM_APP_LIST_CNF";
        case MIPC_SIM_FILE_STATUS_REQ: return "SIM_FILE_STATUS_REQ";
        case MIPC_SIM_FILE_STATUS_CNF: return "SIM_FILE_STATUS_CNF";
        case MIPC_SIM_GET_RESET_REQ: return "SIM_GET_RESET_REQ";
        case MIPC_SIM_GET_RESET_CNF: return "SIM_GET_RESET_CNF";
        case MIPC_SIM_SET_RESET_REQ: return "SIM_SET_RESET_REQ";
        case MIPC_SIM_SET_RESET_CNF: return "SIM_SET_RESET_CNF";
        case MIPC_SIM_GET_TERMINAL_CAPABILITY_REQ: return "SIM_GET_TERMINAL_CAPABILITY_REQ";
        case MIPC_SIM_GET_TERMINAL_CAPABILITY_CNF: return "SIM_GET_TERMINAL_CAPABILITY_CNF";
        case MIPC_SIM_SET_TERMINAL_CAPABILITY_REQ: return "SIM_SET_TERMINAL_CAPABILITY_REQ";
        case MIPC_SIM_SET_TERMINAL_CAPABILITY_CNF: return "SIM_SET_TERMINAL_CAPABILITY_CNF";
        case MIPC_SIM_SET_PIN_EX_REQ: return "SIM_SET_PIN_EX_REQ";
        case MIPC_SIM_SET_PIN_EX_CNF: return "SIM_SET_PIN_EX_CNF";
        case MIPC_SIM_GET_PIN_EX_REQ: return "SIM_GET_PIN_EX_REQ";
        case MIPC_SIM_GET_PIN_EX_CNF: return "SIM_GET_PIN_EX_CNF";
        case MIPC_SIM_GET_GSM_AUTH_REQ: return "SIM_GET_GSM_AUTH_REQ";
        case MIPC_SIM_GET_GSM_AUTH_CNF: return "SIM_GET_GSM_AUTH_CNF";
        case MIPC_SIM_GET_EXT_AUTH_REQ: return "SIM_GET_EXT_AUTH_REQ";
        case MIPC_SIM_GET_EXT_AUTH_CNF: return "SIM_GET_EXT_AUTH_CNF";
        case MIPC_SIM_GET_FACILITY_REQ: return "SIM_GET_FACILITY_REQ";
        case MIPC_SIM_GET_FACILITY_CNF: return "SIM_GET_FACILITY_CNF";
        case MIPC_SIM_SET_FACILITY_REQ: return "SIM_SET_FACILITY_REQ";
        case MIPC_SIM_SET_FACILITY_CNF: return "SIM_SET_FACILITY_CNF";
        case MIPC_SIM_GET_EUICC_SLOTS_STATUS_REQ: return "SIM_GET_EUICC_SLOTS_STATUS_REQ";
        case MIPC_SIM_GET_EUICC_SLOTS_STATUS_CNF: return "SIM_GET_EUICC_SLOTS_STATUS_CNF";
        case MIPC_SIM_ACCESS_PROFILE_CONNECT_REQ: return "SIM_ACCESS_PROFILE_CONNECT_REQ";
        case MIPC_SIM_ACCESS_PROFILE_CONNECT_CNF: return "SIM_ACCESS_PROFILE_CONNECT_CNF";
        case MIPC_SIM_ACCESS_PROFILE_DISCONNECT_REQ: return "SIM_ACCESS_PROFILE_DISCONNECT_REQ";
        case MIPC_SIM_ACCESS_PROFILE_DISCONNECT_CNF: return "SIM_ACCESS_PROFILE_DISCONNECT_CNF";
        case MIPC_SIM_ACCESS_PROFILE_POWER_ON_REQ: return "SIM_ACCESS_PROFILE_POWER_ON_REQ";
        case MIPC_SIM_ACCESS_PROFILE_POWER_ON_CNF: return "SIM_ACCESS_PROFILE_POWER_ON_CNF";
        case MIPC_SIM_ACCESS_PROFILE_POWER_OFF_REQ: return "SIM_ACCESS_PROFILE_POWER_OFF_REQ";
        case MIPC_SIM_ACCESS_PROFILE_POWER_OFF_CNF: return "SIM_ACCESS_PROFILE_POWER_OFF_CNF";
        case MIPC_SIM_ACCESS_PROFILE_RESET_REQ: return "SIM_ACCESS_PROFILE_RESET_REQ";
        case MIPC_SIM_ACCESS_PROFILE_RESET_CNF: return "SIM_ACCESS_PROFILE_RESET_CNF";
        case MIPC_SIM_ACCESS_PROFILE_APDU_REQ: return "SIM_ACCESS_PROFILE_APDU_REQ";
        case MIPC_SIM_ACCESS_PROFILE_APDU_CNF: return "SIM_ACCESS_PROFILE_APDU_CNF";
        case MIPC_SIM_SET_SIM_POWER_REQ: return "SIM_SET_SIM_POWER_REQ";
        case MIPC_SIM_SET_SIM_POWER_CNF: return "SIM_SET_SIM_POWER_CNF";
        case MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ: return "SIM_SET_PHYSICAL_SLOTS_MAPPING_REQ";
        case MIPC_SIM_SET_PHYSICAL_SLOTS_MAPPING_CNF: return "SIM_SET_PHYSICAL_SLOTS_MAPPING_CNF";
        case MIPC_SIM_MEP_SLOTS_INFO_GET_REQ: return "MIPC_SIM_MEP_SLOTS_INFO_GET_REQ";
        case MIPC_SIM_MEP_SLOTS_INFO_GET_CNF: return "MIPC_SIM_MEP_SLOTS_INFO_GET_CNF";
        case MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ: return "MIPC_SIM_MEP_SLOTS_MAPPING_SET_REQ";
        case MIPC_SIM_MEP_SLOTS_MAPPING_SET_CNF: return "MIPC_SIM_MEP_SLOTS_MAPPING_SET_CNF";
        case MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ: return "SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_REQ";
        case MIPC_SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF: return "SIM_EXTENDED_CHANNEL_GENERIC_ACCESS_CNF";
        case MIPC_SIM_UICC_FILE_ACCESS_RECORD_REQ: return "SIM_UICC_FILE_ACCESS_RECORD_REQ";
        case MIPC_SIM_UICC_FILE_ACCESS_RECORD_CNF: return "SIM_UICC_FILE_ACCESS_RECORD_CNF";
        case MIPC_SIM_UICC_FILE_ACCESS_BINARY_REQ: return "SIM_UICC_FILE_ACCESS_BINARY_REQ";
        case MIPC_SIM_UICC_FILE_ACCESS_BINARY_CNF: return "SIM_UICC_FILE_ACCESS_BINARY_CNF";
        case MIPC_SIM_GET_PHYSICAL_SLOTS_MAPPING_REQ: return "SIM_GET_PHYSICAL_SLOTS_MAPPING_REQ";
        case MIPC_SIM_GET_PHYSICAL_SLOTS_MAPPING_CNF: return "SIM_GET_PHYSICAL_SLOTS_MAPPING_CNF";
        case MIPC_SIM_GET_SIM_AUTH_REQ: return "SIM_GET_SIM_AUTH_REQ";
        case MIPC_SIM_GET_SIM_AUTH_CNF: return "SIM_GET_SIM_AUTH_CNF";
        case MIPC_SIM_ESIM_SWITCH_SET_REQ: return "MIPC_SIM_ESIM_SWITCH_SET_REQ";
        case MIPC_SIM_ESIM_SWITCH_SET_CNF: return "MIPC_SIM_ESIM_SWITCH_SET_CNF";
        case MIPC_SIM_ESIM_SWITCH_GET_REQ: return "MIPC_SIM_ESIM_SWITCH_GET_REQ";
        case MIPC_SIM_ESIM_SWITCH_GET_CNF: return "MIPC_SIM_ESIM_SWITCH_GET_CNF";
        case MIPC_SIM_ESIM_EID_QUERY_REQ: return "MIPC_SIM_ESIM_EID_QUERY_REQ";
        case MIPC_SIM_ESIM_EID_QUERY_CNF: return "MIPC_SIM_ESIM_EID_QUERY_CNF";
        case MIPC_SIM_HOTSWAP_DISABLE_GET_REQ: return "MIPC_SIM_HOTSWAP_DISABLE_GET_REQ";
        case MIPC_SIM_HOTSWAP_DISABLE_GET_CNF: return "MIPC_SIM_HOTSWAP_DISABLE_GET_CNF";
        case MIPC_SIM_HOTSWAP_DISABLE_SET_REQ: return "MIPC_SIM_HOTSWAP_DISABLE_SET_REQ";
        case MIPC_SIM_HOTSWAP_DISABLE_SET_CNF: return "MIPC_SIM_HOTSWAP_DISABLE_SET_CNF";
        case MIPC_SIM_RECOVERY_DISABLE_GET_REQ: return "MIPC_SIM_RECOVERY_DISABLE_GET_REQ";
        case MIPC_SIM_RECOVERY_DISABLE_GET_CNF: return "MIPC_SIM_RECOVERY_DISABLE_GET_CNF";
        case MIPC_SIM_RECOVERY_DISABLE_SET_REQ: return "MIPC_SIM_RECOVERY_DISABLE_SET_REQ";
        case MIPC_SIM_RECOVERY_DISABLE_SET_CNF: return "MIPC_SIM_RECOVERY_DISABLE_SET_CNF";
        case MIPC_SIM_SML_GET_ALLOWED_CARRIERS_REQ: return "SIM_SML_GET_ALLOWED_CARRIERS_REQ";
        case MIPC_SIM_SML_GET_ALLOWED_CARRIERS_CNF: return "SIM_SML_GET_ALLOWED_CARRIERS_CNF";
        case MIPC_SIM_SML_SET_ALLOWED_CARRIERS_REQ: return "SIM_SML_SET_ALLOWED_CARRIERS_REQ";
        case MIPC_SIM_SML_SET_ALLOWED_CARRIERS_CNF: return "SIM_SML_SET_ALLOWED_CARRIERS_CNF";
        case MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ: return "SIM_SML_ENTER_SIM_DEPERSONALIZATION_REQ";
        case MIPC_SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF: return "SIM_SML_ENTER_SIM_DEPERSONALIZATION_CNF";
        case MIPC_SIM_SML_GET_LOCK_REQ: return "SIM_SML_GET_LOCK_REQ";
        case MIPC_SIM_SML_GET_LOCK_CNF: return "SIM_SML_GET_LOCK_CNF";
        case MIPC_SIM_SML_SET_LOCK_REQ: return "SIM_SML_SET_LOCK_REQ";
        case MIPC_SIM_SML_SET_LOCK_CNF: return "SIM_SML_SET_LOCK_CNF";
        case MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ: return "SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_REQ";
        case MIPC_SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF: return "SIM_SML_ENTER_DEVICE_DEPERSONALIZATION_CNF";
        case MIPC_SIM_SML_GET_DEV_LOCK_REQ: return "SIM_SML_GET_DEV_LOCK_REQ";
        case MIPC_SIM_SML_GET_DEV_LOCK_CNF: return "SIM_SML_GET_DEV_LOCK_CNF";
        case MIPC_SIM_SML_RSU_REQ: return "SIM_SML_RSU_REQ";
        case MIPC_SIM_SML_RSU_CNF: return "SIM_SML_RSU_CNF";
        case MIPC_SIM_VSIM_SET_AKA_REQ: return "SIM_VSIM_SET_AKA_REQ";
        case MIPC_SIM_VSIM_SET_AKA_CNF: return "SIM_VSIM_SET_AKA_CNF";
        case MIPC_SIM_VSIM_ENABLE_REQ: return "SIM_VSIM_ENABLE_REQ";
        case MIPC_SIM_VSIM_ENABLE_CNF: return "SIM_VSIM_ENABLE_CNF";
        case MIPC_SIM_VSIM_DISABLE_REQ: return "SIM_VSIM_DISABLE_REQ";
        case MIPC_SIM_VSIM_DISABLE_CNF: return "SIM_VSIM_DISABLE_CNF";
        case MIPC_SIM_VSIM_PLUG_REQ: return "SIM_VSIM_PLUG_REQ";
        case MIPC_SIM_VSIM_PLUG_CNF: return "SIM_VSIM_PLUG_CNF";
        case MIPC_SIM_VSIM_SET_TIMER_REQ: return "SIM_VSIM_SET_TIMER_REQ";
        case MIPC_SIM_VSIM_SET_TIMER_CNF: return "SIM_VSIM_SET_TIMER_CNF";
        case MIPC_SIM_VSIM_RESET_REQ: return "SIM_VSIM_RESET_REQ";
        case MIPC_SIM_VSIM_RESET_CNF: return "SIM_VSIM_RESET_CNF";
        case MIPC_SIM_VSIM_APDU_REQ: return "SIM_VSIM_APDU_REQ";
        case MIPC_SIM_VSIM_APDU_CNF: return "SIM_VSIM_APDU_CNF";
        case MIPC_SIM_VSIM_AUTH_REQ: return "SIM_VSIM_AUTH_REQ";
        case MIPC_SIM_VSIM_AUTH_CNF: return "SIM_VSIM_AUTH_CNF";
        case MIPC_SIM_CDMA_SUBSCRIPTION_REQ: return "SIM_CDMA_SUBSCRIPTION_REQ";
        case MIPC_SIM_CDMA_SUBSCRIPTION_CNF: return "SIM_CDMA_SUBSCRIPTION_CNF";
        case MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_REQ: return "SIM_CDMA_GET_SUBSCRIPTION_SOURCE_REQ";
        case MIPC_SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF: return "SIM_CDMA_GET_SUBSCRIPTION_SOURCE_CNF";
        case MIPC_SIM_PIN_COUNT_QUERY_REQ: return "SIM_PIN_COUNT_QUERY_REQ";
        case MIPC_SIM_PIN_COUNT_QUERY_CNF: return "SIM_PIN_COUNT_QUERY_CNF";
        case MIPC_SIM_SML_GET_NETWORK_LOCK_REQ: return "SIM_SML_GET_NETWORK_LOCK_REQ";
        case MIPC_SIM_SML_GET_NETWORK_LOCK_CNF: return "SIM_SML_GET_NETWORK_LOCK_CNFSIM_SML_GET_NETWORK_LOCK_CNF";
        case MIPC_SMS_CFG_REQ: return "SMS_CFG_REQ";
        case MIPC_SMS_CFG_CNF: return "SMS_CFG_CNF";
        case MIPC_SMS_SEND_REQ: return "SMS_SEND_REQ";
        case MIPC_SMS_SEND_CNF: return "SMS_SEND_CNF";
        case MIPC_SMS_READ_REQ: return "SMS_READ_REQ";
        case MIPC_SMS_READ_CNF: return "SMS_READ_CNF";
        case MIPC_SMS_DELETE_REQ: return "SMS_DELETE_REQ";
        case MIPC_SMS_DELETE_CNF: return "SMS_DELETE_CNF";
        case MIPC_SMS_GET_STORE_STATUS_REQ: return "SMS_GET_STORE_STATUS_REQ";
        case MIPC_SMS_GET_STORE_STATUS_CNF: return "SMS_GET_STORE_STATUS_CNF";
        case MIPC_SMS_WRITE_REQ: return "SMS_WRITE_REQ";
        case MIPC_SMS_WRITE_CNF: return "SMS_WRITE_CNF";
        case MIPC_SMS_CBM_CFG_REQ: return "SMS_CBM_CFG_REQ";
        case MIPC_SMS_CBM_CFG_CNF: return "SMS_CBM_CFG_CNF";
        case MIPC_SMS_SCBM_REQ: return "SMS_SCBM_REQ";
        case MIPC_SMS_SCBM_CNF: return "SMS_SCBM_CNF";
        case MIPC_SS_SEND_USSD_REQ: return "SS_SEND_USSD_REQ";
        case MIPC_SS_SEND_USSD_CNF: return "SS_SEND_USSD_CNF";
        case MIPC_SS_CANCEL_USSD_REQ: return "SS_CANCEL_USSD_REQ";
        case MIPC_SS_CANCEL_USSD_CNF: return "SS_CANCEL_USSD_CNF";
        case MIPC_SS_SET_CLIR_REQ: return "SS_SET_CLIR_REQ";
        case MIPC_SS_SET_CLIR_CNF: return "SS_SET_CLIR_CNF";
        case MIPC_SS_GET_CLIR_REQ: return "SS_GET_CLIR_REQ";
        case MIPC_SS_GET_CLIR_CNF: return "SS_GET_CLIR_CNF";
        case MIPC_SS_SET_CALL_WAITING_REQ: return "SS_SET_CALL_WAITING_REQ";
        case MIPC_SS_SET_CALL_WAITING_CNF: return "SS_SET_CALL_WAITING_CNF";
        case MIPC_SS_QUERY_CALL_WAITING_REQ: return "SS_QUERY_CALL_WAITING_REQ";
        case MIPC_SS_QUERY_CALL_WAITING_CNF: return "SS_QUERY_CALL_WAITING_CNF";
        case MIPC_SS_SET_CALL_FORWARD_REQ: return "SS_SET_CALL_FORWARD_REQ";
        case MIPC_SS_SET_CALL_FORWARD_CNF: return "SS_SET_CALL_FORWARD_CNF";
        case MIPC_SS_QUERY_CALL_FORWARD_REQ: return "SS_QUERY_CALL_FORWARD_REQ";
        case MIPC_SS_QUERY_CALL_FORWARD_CNF: return "SS_QUERY_CALL_FORWARD_CNF";
        case MIPC_SS_SET_CALL_BARRING_REQ: return "SS_SET_CALL_BARRING_REQ";
        case MIPC_SS_SET_CALL_BARRING_CNF: return "SS_SET_CALL_BARRING_CNF";
        case MIPC_SS_QUERY_CALL_BARRING_REQ: return "SS_QUERY_CALL_BARRING_REQ";
        case MIPC_SS_QUERY_CALL_BARRING_CNF: return "SS_QUERY_CALL_BARRING_CNF";
        case MIPC_SS_CHANGE_BARRING_PASSWORD_REQ: return "SS_CHANGE_BARRING_PASSWORD_REQ";
        case MIPC_SS_CHANGE_BARRING_PASSWORD_CNF: return "SS_CHANGE_BARRING_PASSWORD_CNF";
        case MIPC_SS_SET_SUPP_SVC_NOTIFICATION_REQ: return "SS_SET_SUPP_SVC_NOTIFICATION_REQ";
        case MIPC_SS_SET_SUPP_SVC_NOTIFICATION_CNF: return "SS_SET_SUPP_SVC_NOTIFICATION_CNF";
        case MIPC_SS_QUERY_CLIP_REQ: return "SS_QUERY_CLIP_REQ";
        case MIPC_SS_QUERY_CLIP_CNF: return "SS_QUERY_CLIP_CNF";
        case MIPC_SS_SET_CLIP_REQ: return "SS_SET_CLIP_REQ";
        case MIPC_SS_SET_CLIP_CNF: return "SS_SET_CLIP_CNF";
        case MIPC_SS_RUN_GBA_REQ: return "SS_RUN_GBA_REQ";
        case MIPC_SS_RUN_GBA_CNF: return "SS_RUN_GBA_CNF";
        case MIPC_SS_GET_COLP_REQ: return "SS_GET_COLP_REQ";
        case MIPC_SS_GET_COLP_CNF: return "SS_GET_COLP_CNF";
        case MIPC_SS_SET_COLP_REQ: return "SS_SET_COLP_REQ";
        case MIPC_SS_SET_COLP_CNF: return "SS_SET_COLP_CNF";
        case MIPC_SS_GET_COLR_REQ: return "SS_GET_COLR_REQ";
        case MIPC_SS_GET_COLR_CNF: return "SS_GET_COLR_CNF";
        case MIPC_SS_SEND_CNAP_REQ: return "SS_SEND_CNAP_REQ";
        case MIPC_SS_SEND_CNAP_CNF: return "SS_SEND_CNAP_CNF";
        case MIPC_SS_SET_COLR_REQ: return "SS_SET_COLR_REQ";
        case MIPC_SS_SET_COLR_CNF: return "SS_SET_COLR_CNF";
        case MIPC_SS_SETUP_XCAP_USER_AGENT_REQ: return "SS_SETUP_XCAP_USER_AGENT_REQ";
        case MIPC_SS_SETUP_XCAP_USER_AGENT_CNF: return "SS_SETUP_XCAP_USER_AGENT_CNF";
        case MIPC_SS_SET_XCAP_CFG_REQ: return "SS_SET_XCAP_CFG_REQ";
        case MIPC_SS_SET_XCAP_CFG_CNF: return "SS_SET_XCAP_CFG_CNF";
        case MIPC_STK_SET_PAC_REQ: return "STK_SET_PAC_REQ";
        case MIPC_STK_SET_PAC_CNF: return "STK_SET_PAC_CNF";
        case MIPC_STK_GET_PAC_REQ: return "STK_GET_PAC_REQ";
        case MIPC_STK_GET_PAC_CNF: return "STK_GET_PAC_CNF";
        case MIPC_STK_SEND_TERMINAL_RESPONSE_REQ: return "STK_SEND_TERMINAL_RESPONSE_REQ";
        case MIPC_STK_SEND_TERMINAL_RESPONSE_CNF: return "STK_SEND_TERMINAL_RESPONSE_CNF";
        case MIPC_STK_SEND_ENVELOPE_REQ: return "STK_SEND_ENVELOPE_REQ";
        case MIPC_STK_SEND_ENVELOPE_CNF: return "STK_SEND_ENVELOPE_CNF";
        case MIPC_STK_GET_ENVELOPE_INFO_REQ: return "STK_GET_ENVELOPE_INFO_REQ";
        case MIPC_STK_GET_ENVELOPE_INFO_CNF: return "STK_GET_ENVELOPE_INFO_CNF";
        case MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_REQ: return "STK_HANDLE_CALL_SETUP_FROM_SIM_REQ";
        case MIPC_STK_HANDLE_CALL_SETUP_FROM_SIM_CNF: return "STK_HANDLE_CALL_SETUP_FROM_SIM_CNF";
        case MIPC_STK_SEND_BIPCONF_REQ: return "STK_SEND_BIPCONF_REQ";
        case MIPC_STK_SEND_BIPCONF_CNF: return "STK_SEND_BIPCONF_CNF";
        case MIPC_ECALL_IVS_UPDATE_MSD_REQ: return "ECALL_IVS_UPDATE_MSD_REQ";
        case MIPC_ECALL_IVS_UPDATE_MSD_CNF: return "ECALL_IVS_UPDATE_MSD_CNF";
        case MIPC_ECALL_IVS_RESET_REQ: return "ECALL_IVS_RESET_REQ";
        case MIPC_ECALL_IVS_RESET_CNF: return "ECALL_IVS_RESET_CNF";
        case MIPC_ECALL_IVS_SET_ECALL_MODE_REQ: return "ECALL_IVS_SET_ECALL_MODE_REQ";
        case MIPC_ECALL_IVS_SET_ECALL_MODE_CNF: return "ECALL_IVS_SET_ECALL_MODE_CNF";
        case MIPC_ECALL_IVS_GET_ECALL_MODE_REQ: return "ECALL_IVS_GET_ECALL_MODE_REQ";
        case MIPC_ECALL_IVS_GET_ECALL_MODE_CNF: return "ECALL_IVS_GET_ECALL_MODE_CNF";
        case MIPC_ECALL_IVS_GET_SIM_INFO_REQ: return "ECALL_IVS_GET_SIM_INFO_REQ";
        case MIPC_ECALL_IVS_GET_SIM_INFO_CNF: return "ECALL_IVS_GET_SIM_INFO_CNF";
        case MIPC_ECALL_IVS_SET_TEST_ADDR_REQ : return "ECALL_IVS_SET_TEST_ADDR_REQ";
        case MIPC_ECALL_IVS_SET_TEST_ADDR_CNF : return "ECALL_IVS_SET_TEST_ADDR_CNF";
        case MIPC_ECALL_IVS_SET_RECONF_ADDR_REQ: return "ECALL_IVS_SET_RECONF_ADDR_REQ";
        case MIPC_ECALL_IVS_SET_RECONF_ADDR_CNF: return "ECALL_IVS_SET_RECONF_ADDR_CNF";
        case MIPC_ECALL_IVS_SET_ADDR_PRI_REQ: return "ECALL_IVS_SET_ADDR_PRI_REQ";
        case MIPC_ECALL_IVS_SET_ADDR_PRI_CNF: return "ECALL_IVS_SET_ADDR_PRI_CNF";
        case MIPC_ECALL_STATUS_IND: return "ECALL_STATUS_IND";
        case MIPC_CALL_DIAL_REQ: return "CALL_DIAL_REQ";
        case MIPC_CALL_DIAL_CNF: return "CALL_DIAL_CNF";
        case MIPC_CALL_SS_REQ: return "CALL_SS_REQ";
        case MIPC_CALL_SS_CNF: return "CALL_SS_CNF";
        case MIPC_CALL_HANGUP_REQ: return "CALL_HANGUP_REQ";
        case MIPC_CALL_HANGUP_CNF: return "CALL_HANGUP_CNF";
        case MIPC_CALL_ANSWER_REQ: return "CALL_ANSWER_REQ";
        case MIPC_CALL_ANSWER_CNF: return "CALL_ANSWER_CNF";
        case MIPC_CALL_GET_CALL_STATUS_REQ: return "CALL_GET_CALL_STATUS_REQ";
        case MIPC_CALL_GET_CALL_STATUS_CNF: return "CALL_GET_CALL_STATUS_CNF";
        case MIPC_CALL_CONFERENCE_REQ: return "CALL_CONFERENCE_REQ";
        case MIPC_CALL_CONFERENCE_CNF: return "CALL_CONFERENCE_CNF";
        case MIPC_CALL_GET_CONFERENCE_INFO_REQ: return "CALL_GET_CONFERENCE_INFO_REQ";
        case MIPC_CALL_GET_CONFERENCE_INFO_CNF: return "CALL_GET_CONFERENCE_INFO_CNF";
        case MIPC_CALL_GET_FINISH_REASON_REQ: return "CALL_GET_FINISH_REASON_REQ";
        case MIPC_CALL_GET_FINISH_REASON_CNF: return "CALL_GET_FINISH_REASON_CNF";
        case MIPC_CALL_DTMF_REQ: return "CALL_DTMF_REQ";
        case MIPC_CALL_DTMF_CNF: return "CALL_DTMF_CNF";
        case MIPC_CALL_GET_ECC_LIST_REQ: return "CALL_GET_ECC_LIST_REQ";
        case MIPC_CALL_GET_ECC_LIST_CNF: return "CALL_GET_ECC_LIST_CNF";
        case MIPC_CALL_SET_ECC_LIST_REQ: return "CALL_SET_ECC_LIST_REQ";
        case MIPC_CALL_SET_ECC_LIST_CNF: return "CALL_SET_ECC_LIST_CNF";
        case MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ: return "CALL_SET_FLIGHT_MODE_ECC_SESSION_REQ";
        case MIPC_CALL_SET_FLIGHT_MODE_ECC_SESSION_CNF: return "CALL_SET_FLIGHT_MODE_ECC_SESSION_CNF";
        case MIPC_CALL_SET_EXIT_ECBM_MODE_REQ: return "CALL_SET_EXIT_ECBM_MODE_REQ";
        case MIPC_CALL_SET_EXIT_ECBM_MODE_CNF: return "CALL_SET_EXIT_ECBM_MODE_CNF";
        case MIPC_CALL_IVS_ONEKEY_ECALL_REQ: return "CALL_IVS_ONEKEY_ECALL_REQ";
        case MIPC_CALL_IVS_ONEKEY_ECALL_CNF: return "CALL_IVS_ONEKEY_ECALL_CNF";
        case MIPC_CALL_SET_SIP_HEADER_REQ: return "CALL_SET_SIP_HEADER_REQ";
        case MIPC_CALL_SET_SIP_HEADER_CNF: return "CALL_SET_SIP_HEADER_CNF";
        case MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ: return "CALL_ENABLE_IMS_SIP_HEADER_REPORT_REQ";
        case MIPC_CALL_ENABLE_IMS_SIP_HEADER_REPORT_CNF: return "CALL_ENABLE_IMS_SIP_HEADER_REPORT_CNF";
        case MIPC_CALL_SET_CALL_ADDITIONAL_INFO_REQ: return "CALL_SET_CALL_ADDITIONAL_INFO_REQ";
        case MIPC_CALL_SET_CALL_ADDITIONAL_INFO_CNF: return "CALL_SET_CALL_ADDITIONAL_INFO_CNF";
        case MIPC_CALL_PEER_RTT_MODIFY_REQ: return "CALL_PEER_RTT_MODIFY_REQ";
        case MIPC_CALL_PEER_RTT_MODIFY_CNF: return "CALL_PEER_RTT_MODIFY_CNF";
        case MIPC_CALL_LOCAL_RTT_MODIFY_REQ: return "CALL_LOCAL_RTT_MODIFY_REQ";
        case MIPC_CALL_LOCAL_RTT_MODIFY_CNF: return "CALL_LOCAL_RTT_MODIFY_CNF";
        case MIPC_CALL_RTT_TEXT_REQ: return "CALL_RTT_TEXT_REQ";
        case MIPC_CALL_RTT_TEXT_CNF: return "CALL_RTT_TEXT_CNF";
        case MIPC_CALL_RTT_MODE_REQ: return "CALL_RTT_MODE_REQ";
        case MIPC_CALL_RTT_MODE_CNF: return "CALL_RTT_MODE_CNF";
        case MIPC_CALL_RTT_AUDIO_REQ: return "CALL_RTT_AUDIO_REQ";
        case MIPC_CALL_RTT_AUDIO_CNF: return "CALL_RTT_AUDIO_CNF";
        case MIPC_CALL_SET_RCS_STATE_AND_FEATURE_REQ: return "CALL_SET_RCS_STATE_AND_FEATURE_REQ";
        case MIPC_CALL_SET_RCS_STATE_AND_FEATURE_CNF: return "CALL_SET_RCS_STATE_AND_FEATURE_CNF";
        case MIPC_CALL_UPDATE_RCS_SESSION_INFO_REQ: return "CALL_UPDATE_RCS_SESSION_INFO_REQ";
        case MIPC_CALL_UPDATE_RCS_SESSION_INFO_CNF: return "CALL_UPDATE_RCS_SESSION_INFO_CNF";
        case MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_REQ: return "CALL_GET_VOICE_DOMAIN_PREFERENCE_REQ";
        case MIPC_CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF: return "CALL_GET_VOICE_DOMAIN_PREFERENCE_CNF";
        case MIPC_CALL_PULL_REQ: return "CALL_PULL_REQ";
        case MIPC_CALL_PULL_CNF: return "CALL_PULL_CNF";
        case MIPC_CALL_GET_TTY_MODE_REQ: return "CALL_GET_TTY_MODE_REQ";
        case MIPC_CALL_GET_TTY_MODE_CNF: return "CALL_GET_TTY_MODE_CNF";
        case MIPC_CALL_SET_IMS_CALL_MODE_REQ: return "CALL_SET_IMS_CALL_MODE_REQ";
        case MIPC_CALL_SET_IMS_CALL_MODE_CNF: return "CALL_SET_IMS_CALL_MODE_CNF";
        case MIPC_CALL_SET_TTY_MODE_REQ: return "CALL_SET_TTY_MODE_REQ";
        case MIPC_CALL_SET_TTY_MODE_CNF: return "CALL_SET_TTY_MODE_CNF";
        case MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ: return "CALL_SET_VOICE_DOMAIN_PREFERENCE_REQ";
        case MIPC_CALL_SET_VOICE_DOMAIN_PREFERENCE_CNF: return "CALL_SET_VOICE_DOMAIN_PREFERENCE_CNF";
        case MIPC_CALL_CONFERENCE_DIAL_REQ: return "CALL_CONFERENCE_DIAL_REQ";
        case MIPC_CALL_CONFERENCE_DIAL_CNF: return "CALL_CONFERENCE_DIAL_CNF";
        case MIPC_CALL_SET_GWSD_MODE_REQ: return "CALL_SET_GWSD_MODE_REQ";
        case MIPC_CALL_SET_GWSD_MODE_CNF: return "CALL_SET_GWSD_MODE_CNF";
        case MIPC_CALL_APPROVE_CSFB_REQ: return "CALL_APPROVE_CSFB_REQ";
        case MIPC_CALL_APPROVE_CSFB_CNF: return "CALL_APPROVE_CSFB_CNF";
        case MIPC_CALL_SET_GWSD_CALL_VALID_REQ: return "CALL_SET_GWSD_CALL_VALID_REQ";
        case MIPC_CALL_SET_GWSD_CALL_VALID_CNF: return "CALL_SET_GWSD_CALL_VALID_CNF";
        case MIPC_CALL_SET_GWSD_IGNORE_CALL_INTERVAL_REQ: return "CALL_SET_GWSD_IGNORE_CALL_INTERVAL_REQ";
        case MIPC_CALL_SET_GWSD_IGNORE_CALL_INTERVAL_CNF: return "CALL_SET_GWSD_IGNORE_CALL_INTERVAL_CNF";
        case MIPC_CALL_SET_GWSD_KA_PDCP_REQ: return "CALL_SET_GWSD_KA_PDCP_REQ";
        case MIPC_CALL_SET_GWSD_KA_PDCP_CNF: return "CALL_SET_GWSD_KA_PDCP_CNF";
        case MIPC_CALL_SET_GWSD_KA_IPDATA_REQ: return "CALL_SET_GWSD_KA_IPDATA_REQ";
        case MIPC_CALL_SET_GWSD_KA_IPDATA_CNF: return "CALL_SET_GWSD_KA_IPDATA_CNF";
        case MIPC_CALL_UIS_INFO_REQ: return "CALL_UIS_INFO_REQ";
        case MIPC_CALL_UIS_INFO_CNF: return "CALL_UIS_INFO_CNF";
        case MIPC_CALL_AUTO_ANSWER_REQ: return "CALL_AUTO_ANSWER_REQ";
        case MIPC_CALL_AUTO_ANSWER_CNF: return "CALL_AUTO_ANSWER_CNF";
        case MIPC_CALL_DATA_PREFER_SET_REQ: return "CALL_DATA_PREFER_SET_REQ";
        case MIPC_CALL_DATA_PREFER_SET_CNF: return "CALL_DATA_PREFER_SET_CNF";
        case MIPC_CALL_ECC_REDIAL_APPROVE_REQ: return "CALL_ECC_REDIAL_APPROVE_REQ";
        case MIPC_CALL_ECC_REDIAL_APPROVE_CNF: return "CALL_ECC_REDIAL_APPROVE_CNF";
        case MIPC_CALL_DC_CREATE_SIP_REQ: return "CALL_DC_CREATE_SIP_REQ";
        case MIPC_CALL_DC_CREATE_SIP_CNF: return "CALL_DC_CREATE_SIP_CNF";
        case MIPC_CALL_DC_MODIFY_SIP_REQ: return "CALL_DC_MODIFY_SIP_REQ";
        case MIPC_CALL_DC_MODIFY_SIP_CNF: return "CALL_DC_MODIFY_SIP_CNF";
        case MIPC_CALL_DC_CLOSE_SIP_REQ: return "CALL_DC_CLOSE_SIP_REQ";
        case MIPC_CALL_DC_CLOSE_SIP_CNF: return "CALL_DC_CLOSE_SIP_CNF";
        case MIPC_DATA_ACT_PACKET_ROUTE_REQ: return "DATA_ACT_PACKET_ROUTE_REQ";
        case MIPC_DATA_ACT_PACKET_ROUTE_CNF: return "DATA_ACT_PACKET_ROUTE_CNF";
        case MIPC_DATA_IFUP_PACKET_ROUTE_REQ: return "DATA_IFUP_PACKET_ROUTE_REQ";
        case MIPC_DATA_IFUP_PACKET_ROUTE_CNF: return "DATA_IFUP_PACKET_ROUTE_CNF";
        case MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_REQ: return "DATA_DC_DATA_PATH_CONFIG_SYNC_REQ";
        case MIPC_DATA_DC_DATA_PATH_CONFIG_SYNC_CNF: return "DATA_DC_DATA_PATH_CONFIG_SYNC_CNF";
        case MIPC_CALL_CREATE_DATA_CHANNEL_REQ: return "CALL_CREATE_DATA_CHANNEL_REQ";
        case MIPC_CALL_CREATE_DATA_CHANNEL_CNF: return "CALL_CREATE_DATA_CHANNEL_CNF";
        case MIPC_CALL_CLOSE_DATA_CHANNEL_REQ: return "CALL_CLOSE_DATA_CHANNEL_REQ";
        case MIPC_CALL_CLOSE_DATA_CHANNEL_CNF: return "CALL_CLOSE_DATA_CHANNEL_CNF";
        case MIPC_IMS_SET_CONFIG_REQ: return "IMS_SET_CONFIG_REQ";
        case MIPC_IMS_SET_CONFIG_CNF: return "IMS_SET_CONFIG_CNF";
        case MIPC_IMS_SET_EVODATA_REQ: return "IMS_SET_EVODATA_REQ";
        case MIPC_IMS_SET_EVODATA_CNF: return "IMS_SET_EVODATA_CNF";
        case MIPC_IMS_GET_CONFIG_REQ: return "IMS_GET_CONFIG_REQ";
        case MIPC_IMS_GET_CONFIG_CNF: return "IMS_GET_CONFIG_CNF";
        case MIPC_IMS_GET_STATE_REQ: return "IMS_GET_STATE_REQ";
        case MIPC_IMS_GET_STATE_CNF: return "IMS_GET_STATE_CNF";
        case MIPC_IMS_SET_PDIS_REQ: return "IMS_SET_PDIS_REQ";
        case MIPC_IMS_SET_PDIS_CNF: return "IMS_SET_PDIS_CNF";
        case MIPC_IMS_SET_NAPTR_REQ: return "IMS_SET_NAPTR_REQ";
        case MIPC_IMS_SET_NAPTR_CNF: return "IMS_SET_NAPTR_CNF";
        case MIPC_IMS_GET_NW_RPT_REQ: return "IMS_GET_NW_RPT_REQ";
        case MIPC_IMS_GET_NW_RPT_CNF: return "IMS_GET_NW_RPT_CNF";
        case MIPC_IMS_SET_TEST_MODE_REQ: return "IMS_SET_TEST_MODE_REQ";
        case MIPC_IMS_SET_TEST_MODE_CNF: return "IMS_SET_TEST_MODE_CNF";
        case MIPC_IMS_SET_EIREG_REQ: return "IMS_SET_EIREG_REQ";
        case MIPC_IMS_SET_EIREG_CNF: return "IMS_SET_EIREG_CNF";
        case MIPC_IMS_SET_SCM_REQ: return "IMS_SET_SCM_REQ";
        case MIPC_IMS_SET_SCM_CNF: return "IMS_SET_SCM_CNF";
        case MIPC_IMS_SET_SERVICE_SESSION_REQ: return "IMS_SET_SERVICE_SESSION_REQ";
        case MIPC_IMS_SET_SERVICE_SESSION_CNF: return "IMS_SET_SERVICE_SESSION_CNF";
        case MIPC_IMS_SET_UAC_REQ: return "IMS_SET_UAC_REQ";
        case MIPC_IMS_SET_UAC_CNF: return "IMS_SET_UAC_CNF";
        case MIPC_IMS_ENABLE_DATA_CHANNEL_REQ: return "IMS_ENABLE_DATA_CHANNEL_REQ";
        case MIPC_IMS_ENABLE_DATA_CHANNEL_CNF: return "IMS_ENABLE_DATA_CHANNEL_CNF";
        case MIPC_IMS_CFG_ADDITION_SERVICE_REQ: return "IMS_CFG_ADDITION_SERVICE_REQ";
        case MIPC_IMS_CFG_ADDITION_SERVICE_CNF: return "IMS_CFG_ADDITION_SERVICE_CNF";
        case MIPC_WFC_REGISTER_CELL_SIGNAL_IND_REQ: return "WFC_REGISTER_CELL_SIGNAL_IND_REQ";
        case MIPC_WFC_REGISTER_CELL_SIGNAL_IND_CNF: return "WFC_REGISTER_CELL_SIGNAL_IND_CNF";
        case MIPC_WFC_CSSAC_REQ: return "WFC_CSSAC_REQ";
        case MIPC_WFC_CSSAC_CNF: return "WFC_CSSAC_CNF";
        case MIPC_WFC_SET_EMC_AID_REQ: return "WFC_SET_EMC_AID_REQ";
        case MIPC_WFC_SET_EMC_AID_CNF: return "WFC_SET_EMC_AID_CNF";
        case MIPC_WFC_CFG_REQ: return "WFC_CFG_REQ";
        case MIPC_WFC_CFG_CNF: return "WFC_CFG_CNF";
        case MIPC_PHB_SET_UPB_ENTRY_REQ: return "PHB_SET_UPB_ENTRY_REQ";
        case MIPC_PHB_SET_UPB_ENTRY_CNF: return "PHB_SET_UPB_ENTRY_CNF";
        case MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ: return "PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ";
        case MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF: return "PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF";
        case MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ: return "PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ";
        case MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF: return "PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF";
        case MIPC_PHB_GET_PHB_STORAGE_INFO_REQ: return "PHB_GET_PHB_STORAGE_INFO_REQ";
        case MIPC_PHB_GET_PHB_STORAGE_INFO_CNF: return "PHB_GET_PHB_STORAGE_INFO_CNF";
        case MIPC_PHB_SET_PHB_MEM_STORAGE_REQ: return "PHB_SET_PHB_MEM_STORAGE_REQ";
        case MIPC_PHB_SET_PHB_MEM_STORAGE_CNF: return "PHB_SET_PHB_MEM_STORAGE_CNF";
        case MIPC_PHB_GET_PHB_ENTRY_REQ: return "PHB_GET_PHB_ENTRY_REQ";
        case MIPC_PHB_GET_PHB_ENTRY_CNF: return "PHB_GET_PHB_ENTRY_CNF";
        case MIPC_PHB_SET_PHB_ENTRY_REQ: return "PHB_SET_PHB_ENTRY_REQ";
        case MIPC_PHB_SET_PHB_ENTRY_CNF: return "PHB_SET_PHB_ENTRY_CNF";
        case MIPC_PHB_GET_PHB_STRINGSLENGTH_REQ: return "PHB_GET_PHB_STRINGSLENGTH_REQ";
        case MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF: return "PHB_GET_PHB_STRINGSLENGTH_CNF";
        case MIPC_PHB_GET_UPB_CAPABILITY_REQ: return "PHB_GET_UPB_CAPABILITY_REQ";
        case MIPC_PHB_GET_UPB_CAPABILITY_CNF: return "PHB_GET_UPB_CAPABILITY_CNF";
        case MIPC_PHB_GET_PHB_AVAILABLE_REQ: return "PHB_GET_PHB_AVAILABLE_REQ";
        case MIPC_PHB_GET_PHB_AVAILABLE_CNF: return "PHB_GET_PHB_AVAILABLE_CNF";
        case MIPC_EMBMS_EMSLU_REQ: return "EMBMS_EMSLU_REQ";
        case MIPC_EMBMS_EMSLU_CNF: return "EMBMS_EMSLU_CNF";
        case MIPC_EMBMS_GET_SAI_LIST_REQ: return "EMBMS_GET_SAI_LIST_REQ";
        case MIPC_EMBMS_GET_SAI_LIST_CNF: return "EMBMS_GET_SAI_LIST_CNF";
        case MIPC_EMBMS_NOTIFY_HVOLTE_STATUS_REQ: return "EMBMS_NOTIFY_HVOLTE_STATUS_REQ";
        case MIPC_EMBMS_NOTIFY_HVOLTE_STATUS_CNF: return "EMBMS_NOTIFY_HVOLTE_STATUS_CNF";
        case MIPC_SYS_AT_IND: return "SYS_AT_IND";
        case MIPC_SYS_THERMAL_SENSOR_IND: return "SYS_THERMAL_SENSOR_IND";
        case MIPC_SYS_CONFIG_IND: return "SYS_CONFIG_IND";
        case MIPC_SYS_ADPCLK_IND: return "SYS_ADPCLK_IND";
        case MIPC_SYS_MCF_IND: return "SYS_MCF_IND";
        case MIPC_SYS_SBP_IND: return "SYS_SBP_IND";
        case MIPC_SYS_GEO_LOCATION_IND: return "SYS_GEO_LOCATION_IND";
        case MIPC_SYS_MD_INIT_IND: return "SYS_MD_INIT_IND";
        case MIPC_SYS_WARNING_IND: return "SYS_WARNING_IND";
        case MIPC_SYS_NV_SIG_ERR_IND: return "SYS_NV_SIG_ERR_IND";
        case MIPC_SYS_VODATA_STATISTICS_IND: return "SYS_VODATA_STATISTICS_IND";
        case MIPC_SYS_THERMAL_ACTUATOR_IND: return "SYS_THERMAL_ACTUATOR_IND";
        case MIPC_APN_IA_CFG_IND: return "APN_IA_CFG_IND";
        case MIPC_APN_PROFILE_CFG_IND: return "APN_PROFILE_CFG_IND";
        case MIPC_APN_VZW_CHG_IND: return "APN_VZW_CHG_IND";
        case MIPC_DATA_ACT_CALL_IND: return "DATA_ACT_CALL_IND";
        case MIPC_DATA_DEACT_CALL_IND: return "DATA_DEACT_CALL_IND";
        case MIPC_DATA_MOD_CALL_IND: return "DATA_MOD_CALL_IND";
        case MIPC_DATA_MOD_PCO_IND: return "DATA_MOD_PCO_IND";
        case MIPC_DATA_WWAN_ACT_CALL_IND: return "DATA_WWAN_ACT_CALL_IND";
        case MIPC_DATA_WWAN_DEACT_CALL_IND: return "DATA_WWAN_DEACT_CALL_IND";
        case MIPC_DATA_CALL_ACT_REG_IND: return "DATA_CALL_ACT_REG_IND";
        case MIPC_DATA_CALL_DEACT_REG_IND: return "DATA_CALL_DEACT_REG_IND";
        case MIPC_DATA_CALL_MOD_REG_IND: return "DATA_CALL_MOD_REG_IND";
        case MIPC_DATA_MD_ACT_CALL_IND: return "DATA_MD_ACT_CALL_IND";
        case MIPC_DATA_MD_DEACT_CALL_IND: return "DATA_MD_DEACT_CALL_IND";
        case MIPC_DATA_IWLAN_PRIORITY_LIST_IND: return "DATA_IWLAN_PRIORITY_LIST_IND";
        case MIPC_DATA_LINK_CAPACITY_ESTIMATE_IND: return "DATA_LINK_CAPACITY_ESTIMATE_IND";
        case MIPC_DATA_NW_LIMIT_IND: return "DATA_NW_LIMIT_IND";
        case MIPC_DATA_TIMER_IND: return "DATA_TIMER_IND";
        case MIPC_DATA_KEEPALIVE_STATUS_IND: return "DATA_KEEPALIVE_STATUS_IND";
        case MIPC_DATA_MOBILE_DATA_USAGE_IND: return "DATA_MOBILE_DATA_USAGE_IND";
        case MIPC_DATA_NETWORK_REJECT_CAUSE_IND: return "DATA_NETWORK_REJECT_CAUSE_IND";
        case MIPC_DATA_DSDA_STATE_IND: return "DATA_DSDA_STATE_IND";
        case MIPC_DATA_UMTS_PS_STATE_IND: return "DATA_UMTS_PS_STATE_IND";
        case MIPC_DATA_RETRY_TIMER_IND: return "DATA_RETRY_TIMER_IND";
        case MIPC_DATA_URSP_REEVAL_IND: return "DATA_URSP_REEVAL_IND";
        case MIPC_DATA_URSP_UE_POLICY_CHG_IND: return "DATA_URSP_UE_POLICY_CHG_IND";
        case MIPC_INTERNAL_TEST_IND: return "INTERNAL_TEST_IND";
        case MIPC_INTERNAL_EIF_IND: return "INTERNAL_EIF_IND";
        case MIPC_INTERNAL_HO_IND: return "INTERNAL_HO_IND";
        case MIPC_NW_REGISTER_IND: return "NW_REGISTER_IND";
        case MIPC_NW_SIGNAL_IND: return "NW_SIGNAL_IND";
        case MIPC_NW_PS_IND: return "NW_PS_IND";
        case MIPC_NW_RADIO_IND: return "NW_RADIO_IND";
        case MIPC_NW_IA_IND: return "NW_IA_IND";
        case MIPC_NW_NITZ_IND: return "NW_NITZ_IND";
        case MIPC_NW_LOCATION_INFO_IND: return "NW_LOCATION_INFO_IND";
        case MIPC_NW_CS_IND: return "NW_CS_IND";
        case MIPC_NW_CSCON_IND: return "NW_CSCON_IND";
        case MIPC_NW_PREFERRED_PROVIDER_IND: return "NW_PREFERRED_PROVIDER_IND";
        case MIPC_NW_CAINFO_IND: return "NW_CAINFO_IND";
        case MIPC_NW_EONS_IND: return "NW_EONS_IND";
        case MIPC_NW_CIEV_IND: return "NW_CIEV_IND";
        case MIPC_NW_EGMSS_IND: return "NW_EGMSS_IND";
        case MIPC_NW_PSBEARER_IND: return "NW_PSBEARER_IND";
        case MIPC_NW_ECELL_IND: return "NW_ECELL_IND";
        case MIPC_NW_ANBR_IND: return "NW_ANBR_IND";
        case MIPC_NW_IRAT_IND: return "NW_IRAT_IND";
        case MIPC_NW_EREGINFO_IND: return "NW_EREGINFO_IND";
        case MIPC_NW_EMODCFG_IND: return "NW_EMODCFG_IND";
        case MIPC_NW_EPCELLINFO_IND: return "NW_EPCELLINFO_IND";
        case MIPC_NW_PSEUDO_CELL_IND: return "NW_PSEUDO_CELL_IND";
        case MIPC_NW_NETWORK_INFO_IND: return "NW_NETWORK_INFO_IND";
        case MIPC_NW_MCCMNC_IND: return "NW_MCCMNC_IND";
        case MIPC_NW_PHYSICAL_CHANNEL_CONFIGS_IND: return "NW_PHYSICAL_CHANNEL_CONFIGS_IND";
        case MIPC_NW_OTACMSG_IND: return "NW_OTACMSG_IND";
        case MIPC_NW_BARRING_INFO_IND: return "NW_BARRING_INFO_IND";
        case MIPC_NW_RADIO_CAPABILITY_IND: return "NW_RADIO_CAPABILITY_IND";
        case MIPC_NW_CURRENT_RAT_IND: return "NW_CURRENT_RAT_IND";
        case MIPC_NW_CAMP_STATE_IND: return "NW_CAMP_STATE_IND";
        case MIPC_NW_NR_SWITCH_IND: return "NW_NR_SWITCH_IND";
        case MIPC_NW_FEMTOCELL_INFO_IND: return "NW_FEMTOCELL_INFO_IND";
        case MIPC_NW_ETXPWR_IND: return "NW_ETXPWR_IND";
        case MIPC_NW_ETXPWRSTUS_IND: return "NW_ETXPWRSTUS_IND";
        case MIPC_NW_IWLAN_IND: return "NW_IWLAN_IND";
        case MIPC_NW_CH_INFO_IND: return "NW_CH_INFO_IND";
        case MIPC_NW_NRUW_INFO_IND: return "NW_NRUW_INFO_IND";
        case MIPC_NW_NR_CA_BAND_IND: return "NW_NR_CA_BAND_IND";
        case MIPC_NW_NR_SCS_IND: return "NW_NR_SCS_IND";
        case MIPC_NW_NETWORK_SCAN_IND: return "NW_NETWORK_SCAN_IND";
        case MIPC_NW_CA_INFO_IND: return "NW_CA_INFO_IND";
        case MIPC_NW_NRUC_INFO_IND: return "NW_NRUC_INFO_IND";
        case MIPC_NW_FIRST_PLMN_IND: return "NW_FIRST_PLMN_IND";
        case MIPC_NW_ENWCFGINFO_IND: return "NW_ENWCFGINFO_IND";
        case MIPC_NW_LCM_HOPPING_IND: return "NW_LCM_HOPPING_IND";
        case MIPC_NW_CELLULAR_ID_DISCLOSURE_IND: return "NW_CELLULAR_ID_DISCLOSURE_IND";
        case MIPC_NW_SECURITY_ALGOS_IND: "NW_SECURITY_ALGOS_IND";
        case MIPC_SIM_STATE_IND: return "SIM_STATE_IND";
        case MIPC_SIM_STATUS_IND: return "SIM_STATUS_IND";
        case MIPC_SIM_EUICC_SLOTS_STATUS_IND: return "SIM_EUICC_SLOTS_STATUS_IND";
        case MIPC_SIM_MEP_SLOTS_INFO_IND: return "MIPC_SIM_MEP_SLOTS_INFO_IND";
        case MIPC_SIM_ICCID_IND: return "SIM_ICCID_IND";
        case MIPC_SIM_STATUS_CHANGE_WITH_CAUSE_IND: return "SIM_STATUS_CHANGE_WITH_CAUSE_IND";
        case MIPC_SIM_PHYSICAL_SLOTS_MAPPING_DONE_IND: return "SIM_PHYSICAL_SLOTS_MAPPING_DONE_IND";
        case MIPC_SIM_CSIM_IMSI_CHANGE_IND: return "SIM_CSIM_IMSI_CHANGE_IND";
        case MIPC_SIM_SML_STATUS_IND: return "SIM_SML_STATUS_IND";
        case MIPC_SIM_SML_RSU_IND: return "SIM_SML_RSU_IND";
        case MIPC_SIM_VSIM_RESET_IND: return "SIM_VSIM_RESET_IND";
        case MIPC_SIM_VSIM_APDU_IND: return "SIM_VSIM_APDU_IND";
        case MIPC_SIM_VSIM_EVENT_IND: return "SIM_VSIM_EVENT_IND";
        case MIPC_SIM_SIMAPP_IND: return "SIM_SIMAPP_IND";
        case MIPC_SIM_TEST_SIM_IND: return "SIM_TEST_SIM_IND";
        case MIPC_SIM_CT3G_IND: return "SIM_CT3G_IND";
        case MIPC_SIM_CARD_TYPE_IND: return "SIM_CARD_TYPE_IND";
        case MIPC_SIM_SIMIND_IND: return "SIM_SIMIND_IND";
        case MIPC_SIM_STATUS_CHANGED_IND: return "SIM_STATUS_CHANGED_IND";
        case MIPC_SIM_ESLOTESIM_STATE_IND: return "MIPC_SIM_ESLOTESIM_STATE_IND";
        case MIPC_SMS_CFG_IND: return "SMS_CFG_IND";
        case MIPC_SMS_NEW_SMS_IND: return "SMS_NEW_SMS_IND";
        case MIPC_SMS_STORE_STATUS_IND: return "SMS_STORE_STATUS_IND";
        case MIPC_SMS_NEW_STATUS_REPORT_IND: return "SMS_NEW_STATUS_REPORT_IND";
        case MIPC_SMS_NEW_CBM_IND: return "SMS_NEW_CBM_IND";
        case MIPC_SMS_SCBM_IND: return "SMS_SCBM_IND";
        case MIPC_SMS_EXT_INFO_IND: return "SMS_EXT_INFO_IND";
        case MIPC_SS_USSD_IND: return "SS_USSD_IND";
        case MIPC_SS_ECMCCSS_IND: return "SS_ECMCCSS_IND";
        case MIPC_SS_CFU_IND: return "SS_CFU_IND";
        case MIPC_SS_XCAP_RCN_IND: return "SS_XCAP_RCN_IND";
        case MIPC_SS_IMS_XUI_IND: return "SS_IMS_XUI_IND";
        case MIPC_STK_PAC_IND: return "STK_PAC_IND";
        case MIPC_STK_SIM_REFRESH_IND: return "STK_SIM_REFRESH_IND";
        case MIPC_STK_BIP_EVENT_NOTIFY_IND: return "STK_BIP_EVENT_NOTIFY_IND";
        case MIPC_CALL_STATUS_IND: return "CALL_STATUS_IND";
        case MIPC_CALL_EVENT_IND: return "CALL_EVENT_IND";
        case MIPC_CALL_MODE_IND: return "CALL_MODE_IND";
        case MIPC_CALL_SIP_IND: return "CALL_SIP_IND";
        case MIPC_CALL_CONFERENCE_IND: return "CALL_CONFERENCE_IND";
        case MIPC_CALL_IMS_EVENT_PACKAGE_IND: return "CALL_IMS_EVENT_PACKAGE_IND";
        case MIPC_CALL_ECC_LIST_CHANGE_IND: return "CALL_ECC_LIST_CHANGE_IND";
        case MIPC_CALL_SS_IND: return "CALL_SS_IND";
        case MIPC_CALL_ECBM_CHANGE_IND: return "CALL_ECBM_CHANGE_IND";
        case MIPC_CALL_CRSS_IND: return "CALL_CRSS_IND";
        case MIPC_CALL_ECT_IND: return "CALL_ECT_IND";
        case MIPC_CALL_CIPHER_IND: return "CALL_CIPHER_IND";
        case MIPC_CALL_RTT_AUDIO_IND: return "CALL_RTT_AUDIO_IND";
        case MIPC_CALL_RTT_CAPABILITY_IND: return "CALL_RTT_CAPABILITY_IND";
        case MIPC_CALL_LOCAL_RTT_MODIFY_RESULT_IND: return "CALL_LOCAL_RTT_MODIFY_RESULT_IND";
        case MIPC_CALL_PEER_RTT_MODIFY_RESULT_IND: return "CALL_PEER_RTT_MODIFY_RESULT_IND";
        case MIPC_CALL_RTT_TEXT_RECEIVE_IND: return "CALL_RTT_TEXT_RECEIVE_IND";
        case MIPC_CALL_RCS_DIGITS_LINE_IND: return "CALL_RCS_DIGITS_LINE_IND";
        case MIPC_CALL_DISPLAY_AND_SIGNALS_INFO_IND: return "CALL_DISPLAY_AND_SIGNALS_INFO_IND";
        case MIPC_CALL_EXTENDED_DISPLAY_INFO_IND: return "CALL_EXTENDED_DISPLAY_INFO_IND";
        case MIPC_CALL_LINE_CONTROL_INFO_IND: return "CALL_LINE_CONTROL_INFO_IND";
        case MIPC_CALL_REDIRECTING_NUMBER_INFO_IND: return "CALL_REDIRECTING_NUMBER_INFO_IND";
        case MIPC_CALL_GWSD_EVENT_IND: return "CALL_GWSD_EVENT_IND";
        case MIPC_CALL_ECONF_IND: return "CALL_ECONF_IND";
        case MIPC_CALL_IMS_SIP_HEADER_IND: return "CALL_IMS_SIP_HEADER_IND";
        case MIPC_CALL_ECC_REDIAL_IND: return "CALL_ECC_REDIAL_IND";
        case MIPC_CALL_EMERGENCY_BEARER_SUPPORT_IND: return "CALL_EMERGENCY_BEARER_SUPPORT_IND";
        case MIPC_CALL_UIS_INFO_IND: return "CALL_UIS_INFO_IND";
        case MIPC_CALL_CALL_ADDITIONAL_INFO_IND: return "CALL_CALL_ADDITIONAL_INFO_IND";
        case MIPC_CALL_MT_SIP_INVITE_IND: return "CALL_MT_SIP_INVITE_IND";
        case MIPC_CALL_DC_CONFIG_INIT_IND: return "CALL_DC_CONFIG_INIT_IND";
        case MIPC_CALL_DC_CONFIG_UPDATE_IND: return "CALL_DC_CONFIG_UPDATE_IND";
        case MIPC_CALL_DC_CONFIG_DEINIT_IND: return "CALL_DC_CONFIG_DEINIT_IND";
        case MIPC_DATA_IFUP_PACKET_ROUTE_IND: return "DATA_IFUP_PACKET_ROUTE_IND";
        case MIPC_CALL_CONNECT_DATA_CHANNEL_IND: return "CALL_CONNECT_DATA_CHANNEL_IND";
        case MIPC_CALL_CLOSE_DATA_CHANNEL_IND: return "CALL_CLOSE_DATA_CHANNEL_IND";
        case MIPC_CALL_CREATE_DATA_CHANNEL_IND: return "CALL_CREATE_DATA_CHANNEL_IND";
        case MIPC_IMS_CONFIG_IND: return "IMS_CONFIG_IND";
        case MIPC_IMS_STATE_IND: return "IMS_STATE_IND";
        case MIPC_IMS_SUPPORT_ECC_IND: return "IMS_SUPPORT_ECC_IND";
        case MIPC_IMS_PDN_IND: return "IMS_PDN_IND";
        case MIPC_IMS_NAPTR_IND: return "IMS_NAPTR_IND";
        case MIPC_IMS_REG_IND: return "IMS_REG_IND";
        case MIPC_IMS_SIP_REG_INFO_IND: return "IMS_SIP_REG_INFO_IND";
        case MIPC_IMS_VOPS_IND: return "IMS_VOPS_IND";
        case MIPC_IMS_REG_REMAIN_TIME_IND: return "IMS_REG_REMAIN_TIME_IND";
        case MIPC_IMS_UI_IND: return "IMS_UI_IND";
        case MIPC_WFC_CELL_SIGNAL_IND: return "WFC_CELL_SIGNAL_IND";
        case MIPC_WFC_WIFI_PDN_COUNT_IND: return "WFC_WIFI_PDN_COUNT_IND";
        case MIPC_WFC_PDN_HO_IND: return "WFC_PDN_HO_IND";
        case MIPC_WFC_ROVE_OUT_IND: return "WFC_ROVE_OUT_IND";
        case MIPC_WFC_SSAC_IND: return "WFC_SSAC_IND";
        case MIPC_WFC_WIFI_PDN_ERR_IND: return "WFC_WIFI_PDN_ERR_IND";
        case MIPC_WFC_WIFI_PDN_OOS_IND: return "WFC_WIFI_PDN_OOS_IND";
        case MIPC_WFC_WFC_IND: return "WFC_WFC_IND";
        case MIPC_PHB_READY_STATE_IND: return "PHB_READY_STATE_IND";
        case MIPC_EMBMS_EMSRV_IND: return "EMBMS_EMSRV_IND";
        case MIPC_EMBMS_EMSLUI_IND: return "EMBMS_EMSLUI_IND";
        case MIPC_EMBMS_EMSAILNF_IND: return "EMBMS_EMSAILNF_IND";
        case MIPC_EMBMS_EMSESS_IND: return "EMBMS_EMSESS_IND";
        case MIPC_EMBMS_EHVOLTE_IND: return "EMBMS_EHVOLTE_IND";
        case MIPC_SYS_REBOOT_CMD: return "SYS_REBOOT_CMD";
        case MIPC_SYS_REBOOT_RSP: return "SYS_REBOOT_RSP";
        case MIPC_SYS_SET_CONFIG_DIPC_CMD: return "SYS_SET_CONFIG_DIPC_CMD";
        case MIPC_SYS_SET_CONFIG_DIPC_RSP: return "SYS_SET_CONFIG_DIPC_RSP";
        case MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_CMD: return "SYS_CONFIG_NEEDED_TO_UPDATE_CMD";
        case MIPC_SYS_CONFIG_NEEDED_TO_UPDATE_RSP: return "SYS_CONFIG_NEEDED_TO_UPDATE_RSP";
        case MIPC_SYS_GET_CONFIG_DIPC_CMD: return "SYS_GET_CONFIG_DIPC_CMD";
        case MIPC_SYS_GET_CONFIG_DIPC_RSP: return "SYS_GET_CONFIG_DIPC_RSP";
        case MIPC_SYS_SET_TIME_CMD: return "SYS_SET_TIME_CMD";
        case MIPC_SYS_SET_TIME_RSP: return "SYS_SET_TIME_RSP";
        case MIPC_INTERNAL_TEST_CMD: return "INTERNAL_TEST_CMD";
        case MIPC_INTERNAL_TEST_RSP: return "INTERNAL_TEST_RSP";
        case MIPC_INTERNAL_EIPPORT_CMD: return "INTERNAL_EIPPORT_CMD";
        case MIPC_INTERNAL_EIPPORT_RSP: return "INTERNAL_EIPPORT_RSP";
        case MIPC_INTERNAL_EIPSPI_CMD: return "INTERNAL_EIPSPI_CMD";
        case MIPC_INTERNAL_EIPSPI_RSP: return "INTERNAL_EIPSPI_RSP";
        case MIPC_INTERNAL_MULTI_EIPSPI_FREE_CMD: return "INTERNAL_MULTI_EIPSPI_FREE_CMD";
        case MIPC_INTERNAL_MULTI_EIPSPI_FREE_RSP: return "INTERNAL_MULTI_EIPSPI_FREE_RSP";
        case MIPC_SMS_NEW_SMS_CMD: return "SMS_NEW_SMS_CMD";
        case MIPC_SMS_NEW_SMS_RSP: return "SMS_NEW_SMS_RSP";
        case MIPC_SMS_NEW_STATUS_REPORT_CMD: return "SMS_NEW_STATUS_REPORT_CMD";
        case MIPC_SMS_NEW_STATUS_REPORT_RSP: return "SMS_NEW_STATUS_REPORT_RSP";
        case MIPC_CALL_APPROVE_INCOMING_CMD: return "CALL_APPROVE_INCOMING_CMD";
        case MIPC_CALL_APPROVE_INCOMING_RSP: return "CALL_APPROVE_INCOMING_RSP";
        case MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_CMD: return "CALL_DC_BOOTSTRAP_CFG_QRY_CMD";
        case MIPC_CALL_DC_BOOTSTRAP_CFG_QRY_RSP: return "CALL_DC_BOOTSTRAP_CFG_QRY_RSP";
        case MIPC_CALL_DC_CREATE_CMD: return "CALL_DC_CREATE_CMD";
        case MIPC_CALL_DC_CREATE_RSP: return "CALL_DC_CREATE_RSP";
        case MIPC_IMS_PDIS_CMD: return "IMS_PDIS_CMD";
        case MIPC_IMS_PDIS_RSP: return "IMS_PDIS_RSP";
        case MIPC_WFC_PING_CMD: return "WFC_PING_CMD";
        case MIPC_WFC_PING_RSP: return "WFC_PING_RSP";
        case MIPC_WFC_GET_MAC_CMD: return "WFC_GET_MAC_CMD";
        case MIPC_WFC_GET_MAC_RSP: return "WFC_GET_MAC_RSP";
        case MIPC_WFC_NATT_KEEP_ALIVE_CMD: return "WFC_NATT_KEEP_ALIVE_CMD";
        case MIPC_WFC_NATT_KEEP_ALIVE_RSP: return "WFC_NATT_KEEP_ALIVE_RSP";
        case MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_CMD: return "WFC_REGISTER_WIFI_SIGNAL_NTF_CMD";
        case MIPC_WFC_REGISTER_WIFI_SIGNAL_NTF_RSP: return "WFC_REGISTER_WIFI_SIGNAL_NTF_RSP";
        case MIPC_DATA_ACT_CALL_NTF: return "DATA_ACT_CALL_NTF";
        case MIPC_INTERNAL_TEST_NTF: return "INTERNAL_TEST_NTF";
        case MIPC_INTERNAL_INJECT_TST_NTF: return "INTERNAL_INJECT_TST_NTF";
        case MIPC_WFC_WIFI_SIGNAL_NTF: return "WFC_WIFI_SIGNAL_NTF";
        case MIPC_WFC_EPDG_SCREEN_STATE_NTF: return "WFC_EPDG_SCREEN_STATE_NTF";
        case MIPC_WFC_WIFI_INFO_NTF: return "WFC_WIFI_INFO_NTF";
        case MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_REQ: return "SYS_GET_GNSS_COCLOCK_NVDATA_REQ";
        case MIPC_SYS_GET_GNSS_COCLOCK_NVDATA_CNF: return "SYS_GET_GNSS_COCLOCK_NVDATA_CNF";
        case MIPC_DATA_ACT_PACKET_ROUTE_NTF: return "DATA_ACT_PACKET_ROUTE_NTF";
        case MIPC_DATA_IFUP_PACKET_ROUTE_CMD: return "DATA_IFUP_PACKET_ROUTE_CMD";
        case MIPC_DATA_IFUP_PACKET_ROUTE_RSP: return "DATA_IFUP_PACKET_ROUTE_RSP";
        case MIPC_WFC_SET_EAP_VERSION_REQ: return "WFC_SET_EAP_VERSION_REQ";
        case MIPC_WFC_SET_EAP_VERSION_CNF: return "WFC_SET_EAP_VERSION_CNF";
        case MIPC_WFC_SET_DL_FILTER_CMD: return "WFC_SET_DL_FILTER_CMD";
        case MIPC_WFC_SET_DL_FILTER_RSP: return "WFC_SET_DL_FILTER_RSP";
        case MIPC_WFC_DEL_DL_FILTER_CMD: return "WFC_DEL_DL_FILTER_CMD";
        case MIPC_WFC_DEL_DL_FILTER_RSP: return "WFC_DEL_DL_FILTER_RSP";
        case MIPC_SYS_SET_MSPM_SESSION_REQ: return "SYS_SET_MSPM_SESSION_REQ";
        case MIPC_SYS_SET_MSPM_SESSION_CNF: return "SYS_SET_MSPM_SESSION_CNF";
        case MIPC_SYS_SEND_APP_EVENT_REQ: return "MIPC_SYS_SEND_APP_EVENT_REQ";
        case MIPC_SYS_SEND_APP_EVENT_CNF: return "MIPC_SYS_SEND_APP_EVENT_CNF";
        case MIPC_LOC_AGPS_SYNC_STATUS_REQ: return "MIPC_LOC_AGPS_SYNC_STATUS_REQ";
        case MIPC_LOC_AGPS_SYNC_STATUS_CNF: return "MIPC_LOC_AGPS_SYNC_STATUS_CNF";
        case MIPC_LOC_AGPS_SEND_PMTK_REQ: return "MIPC_LOC_AGPS_SEND_PMTK_REQ";
        case MIPC_LOC_AGPS_SEND_PMTK_CNF: return "MIPC_LOC_AGPS_SEND_PMTK_CNF";
        case MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ: return "MIPC_LOC_AGPS_SYNC_CAPABILITIES_REQ";
        case MIPC_LOC_AGPS_SYNC_CAPABILITIES_CNF: return "MIPC_LOC_AGPS_SYNC_CAPABILITIES_CNF";
        case MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ: return "MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_REQ";
        case MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_CNF: return "MIPC_LOC_AGPS_QUERY_MD_TIME_CMD_CNF";
        case MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ: return "MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_REQ";
        case MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF: return "MIPC_LOC_LBS_SYNC_LOCATION_EM_CONFIG_CNF";
        case MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_REQ: return "MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_REQ";
        case MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_CNF: return "MIPC_LOC_LBS_SET_LOCATION_EM_MSG_REPORT_CNF";
        case MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ: return "MIPC_LOC_LBS_GET_CUSTOM_DEBUG_REQ";
        case MIPC_LOC_LBS_GET_CUSTOM_DEBUG_CNF: return "MIPC_LOC_LBS_GET_CUSTOM_DEBUG_CNF";
        case MIPC_LOC_LBS_LPPE_SERVICE_REQ: return "MIPC_LOC_LBS_LPPE_SERVICE_REQ";
        case MIPC_LOC_LBS_LPPE_SERVICE_CNF: return "MIPC_LOC_LBS_LPPE_SERVICE_CNF";
        case MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ: return "MIPC_LOC_LBS_LPPE_SERVICE_FWK_REQ";
        case MIPC_LOC_LBS_LPPE_SERVICE_FWK_CNF: return "MIPC_LOC_LBS_LPPE_SERVICE_FWK_CNF";
        case MIPC_LOC_LBS_SYNC_AIDL_REQ: return "MIPC_LOC_LBS_SYNC_AIDL_REQ";
        case MIPC_LOC_LBS_SYNC_AIDL_CNF: return "MIPC_LOC_LBS_SYNC_AIDL_CNF";
        case MIPC_LOC_AGPS_NI_REQ: return "MIPC_LOC_AGPS_NI_REQ";
        case MIPC_LOC_AGPS_NI_CNF: return "MIPC_LOC_AGPS_NI_CNF";
        case MIPC_LOC_AGPS_QUERY_REAIDING_REQ: return "MIPC_LOC_AGPS_QUERY_REAIDING_REQ";
        case MIPC_LOC_AGPS_QUERY_REAIDING_CNF: return "MIPC_LOC_AGPS_QUERY_REAIDING_CNF";
        case MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_REQ: return "MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_REQ";
        case MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_CNF: return "MIPC_LOC_AGPS_GET_HA_GNSS_ASSIST_DATA_CNF";
        case MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ: return "MIPC_LOC_AGPS_SET_ASN_INFORMATION_REQ";
        case MIPC_LOC_AGPS_SET_ASN_INFORMATION_CNF: return "MIPC_LOC_AGPS_SET_ASN_INFORMATION_CNF";
        case MIPC_LOC_AGPS_GET_TSX_DATA_REQ: return "MIPC_LOC_AGPS_GET_TSX_DATA_REQ";
        case MIPC_LOC_AGPS_GET_TSX_DATA_CNF: return "MIPC_LOC_AGPS_GET_TSX_DATA_CNF";
        case MIPC_LOC_AGPS_SYNC_MNLD_VER_REQ: return "MIPC_LOC_AGPS_SYNC_MNLD_VER_REQ";
        case MIPC_LOC_AGPS_SYNC_MNLD_VER_CNF: return "MIPC_LOC_AGPS_SYNC_MNLD_VER_CNF";
        case MIPC_LOC_LBS_LOCATION_EM_MSG_IND: return "MIPC_LOC_LBS_LOCATION_EM_MSG_IND";
        case MIPC_LOC_LBS_CUSTOM_DEBUG_IND: return "MIPC_LOC_LBS_CUSTOM_DEBUG_IND";
        case MIPC_LOC_LBS_LPPE_SERVICE_IND: return "MIPC_LOC_LBS_LPPE_SERVICE_IND";
        case MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND: return "MIPC_LOC_LBS_LPPE_SERVICE_FWK_IND";
        case MIPC_LOC_LBS_AIDL_IND: return "MIPC_LOC_LBS_AIDL_IND";
        case MIPC_LOC_AGPS_NI_IND: return "MIPC_LOC_AGPS_NI_IND";
        case MIPC_LOC_AGPS_LOCATION_IND: return "MIPC_LOC_AGPS_LOCATION_IND";
        case MIPC_LOC_AGPS_MD_TIME_CMD_IND: return "MIPC_LOC_AGPS_MD_TIME_CMD_IND";
        case MIPC_LOC_AGPS_HA_GNSS_ASSIST_DATA_IND: return "MIPC_LOC_AGPS_HA_GNSS_ASSIST_DATA_IND";
        case MIPC_LOC_AGPS_TSX_IND: return "MIPC_LOC_AGPS_TSX_IND";
        case MIPC_LOC_AGPS_ASN_INFORMATION_IND: return "MIPC_LOC_AGPS_ASN_INFORMATION_IND";
        case MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND: return "MIPC_LOC_AGPS_CAPABILITIES_SYNC_IND";
        case MIPC_LOC_AGPS_PMTK_IND: return "MIPC_LOC_AGPS_PMTK_IND";
        case MIPC_SYS_GET_PRIMARY_IMEI_INFO_REQ: return "MIPC_SYS_GET_PRIMARY_IMEI_INFO_REQ";
        case MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF: return "MIPC_SYS_GET_PRIMARY_IMEI_INFO_CNF";
        case MIPC_SYS_PRIMARY_IMEI_SWITCH_IND: return "MIPC_SYS_PRIMARY_IMEI_SWITCH_IND";
        case MIPC_SYS_SET_SBP_INFO_REQ: return "MIPC_SYS_SET_SBP_INFO_REQ";
        case MIPC_SYS_SET_SBP_INFO_CNF: return "MIPC_SYS_SET_SBP_INFO_CNF";
        default: return "<unknown MIPC>";
    }
}
