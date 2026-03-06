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

#ifndef _PLATFORM_MTK_H_
#define _PLATFORM_MTK_H_

#include "pthread.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <mtk_log.h>

#define DLL_EXPORT

#define HTOML(x) (x)
#define HTOMS(x) (x)
#define MTOHL(x) (x)
#define MTOHS(x) (x)
#define ALLOC mtkAllocAssert
#undef FREE
#define FREE free
#define SLEEP(x) sleep(x)
#define MEMCPY memcpy
#define MEMCMP memcmp
#define MEMSET memset

extern const char *LOG_TAG_MIPC;
#define SRC_FILE_OFFSET (0)
#define ErrMsg(...) ((void)mtkLogE(LOG_TAG_MIPC, __VA_ARGS__))
#define DbgMsg(...) ((void)mtkLogD(LOG_TAG_MIPC, __VA_ARGS__))
#define InfoMsg(...) ((void)mtkLogD(LOG_TAG_MIPC, __VA_ARGS__))
#define Info2Msg(...) ((void)mtkLogD(LOG_TAG_MIPC, __VA_ARGS__))
#define DbgLogInfo(...) ((void)mtkLogI(LOG_TAG_MIPC, __VA_ARGS__))
#define VdbgMsg(...) ((void)mtkLogV(LOG_TAG_MIPC, __VA_ARGS__))
#define MIPC_UNUSED(x) ((void)(x))

typedef uint32_t (*THREAD_FUNC)(void *priv_ptr);

typedef struct {
    pthread_t tid;
    THREAD_FUNC func;
    void *func_priv_ptr;
} THREAD;

typedef struct {
    pthread_cond_t cond;
    pthread_mutex_t lock;
    int32_t ready;
    void *result_ptr;
    uint32_t timeout_ms;
} EVENT;

typedef struct {
    pthread_mutex_t hMutex;
} MUTEX;


#ifdef __cplusplus
extern "C" {
#endif

THREAD *CREATE_THREAD(THREAD_FUNC func, void *func_priv_ptr);
void DELETE_THREAD(THREAD *thread_ptr);
void WAIT_FOR_KILL_THREAD(THREAD *thread_ptr);

//
MUTEX *CREATE_MUTEX();
void DELETE_MUTEX(MUTEX *mutex_ptr);
void LOCK_MUTEX(MUTEX *mutex_ptr);
void UNLOCK_MUTEX(MUTEX *mutex_ptr);
//
int32_t CHECK_MIPC_SUPPORT();
//
void INITIALIZE();
//
void SETLOGTAG(const char *logTag);
void SETCOM(const char *port_name_ptr);
int32_t GETCOM();
const char *FINDCOM(const char *process_name_ptr);
int32_t OPENCOM(const char *port_name_ptr);
void CLOSECOM();
int32_t WRITECOM(uint8_t *buf_ptr, const uint32_t buf_len);
int32_t READCOM(uint8_t *buf_ptr, const uint32_t buf_len);

uint64_t GETTID();

#ifdef __cplusplus
}
#endif


extern FILE *debug_fp;

#ifdef DEBUG
#define SRC_FILE_OFFSET (0)
#define ErrMsg(format, ...) do {char dt[64];time_t t;struct tm tt;t=time(NULL);localtime_r(&t, &tt);strftime(dt,sizeof(dt),"%F %T",&tt);printf("[%s] (%s:%d) "format"\n",dt,&__FILE__[SRC_FILE_OFFSET],__LINE__,##__VA_ARGS__);}while(0);
#define DbgMsg(format, ...) do {char dt[64];time_t t;struct tm tt;t=time(NULL);localtime_r(&t, &tt);strftime(dt,sizeof(dt),"%F %T",&tt);printf("[%s] (%s:%d) "format"\n",dt,&__FILE__[SRC_FILE_OFFSET],__LINE__,##__VA_ARGS__);}while(0);
#define InfoMsg(format, ...) do {printf("  "format"\n",##__VA_ARGS__);}while(0);
#define Info2Msg(format, ...) do {printf("    "format"\n",##__VA_ARGS__);}while(0);
#endif

#ifdef __cplusplus
extern "C" {
#endif

//
DLL_EXPORT void UNIT_TEST_READ_BUF_ADD(uint8_t *ptr, uint32_t ptr_len);
DLL_EXPORT uint8_t *UNIT_TEST_WRITE_BUF_GET(uint32_t *len_ptr);

//
DLL_EXPORT EVENT *CREATE_EVENT(uint32_t timeout_ms);
DLL_EXPORT void DELETE_EVENT(EVENT *event_ptr);
DLL_EXPORT int32_t WAIT_EVENT(EVENT *event_ptr);
DLL_EXPORT void WAKE_EVENT(EVENT *event_ptr, void *result_ptr);
//
DLL_EXPORT void UNIT_TEST_RESET();


#ifdef TRM_SUPPORT

typedef struct {
    char* process_name;
    char* port_path;
} MIPC_PORT_MAP;

struct md_status_event_t {
    struct timeval time_stamp;
    int md_id;
    int event_type;
    char reason[32];
#if defined(MTK_RIL_THIN_MD_SUPPORT)
    int event_flag;
#endif
};

//LIBTRM
typedef void (*MIPC_MD_EVENT_CB)(void *priv_ptr, struct md_status_event_t *event_ptr);

extern void (*LIBTRM_INIT)();
extern void (*LIBTRM_DEINIT)();
extern int (*LIBTRM_RESET)(int ccci_fd);
extern int (*LIBTRM_POWER_OFF_MD)(int ccci_fd);
extern int (*LIBTRM_POWER_ON_MD)(int ccci_fd);

extern void (*LIBTRM_MD_EVENT_REGISTER)(MIPC_MD_EVENT_CB cb, void *cb_priv_ptr);

DLL_EXPORT int mipc_reset();
DLL_EXPORT int mipc_power_off();
DLL_EXPORT int mipc_power_on();
DLL_EXPORT void mipc_md_event_register(MIPC_MD_EVENT_CB cb, void *cb_priv_ptr);
#endif

extern void* mtkAllocAssert(size_t size);

#ifdef __cplusplus
}
#endif

#endif
