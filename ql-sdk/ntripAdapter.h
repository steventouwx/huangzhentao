/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: ntripAdapter.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义ntripAdapter相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef  SV_NTRIPADAPTER_H__
#define  SV_NTRIPADAPTER_H__
#include "sv_ntrip.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*AdapterCorrectionDataCb)(void* data, int length);
typedef void (*AdapterNtripStatusCb)(int status);

typedef struct 
{
    AdapterCorrectionDataCb on_Correction_Data_Cb;
    AdapterNtripStatusCb on_Ntrip_Status_Cb;
}ADAPTER_NTRIP_CALLBACK;

typedef struct 
{
    sv_err_t (*init)(ADAPTER_NTRIP_CALLBACK callBack);
    sv_err_t (*deinit)(void);
    sv_err_t (*get_sdk_version)(const char* version);
    sv_err_t (*send_gga)(const char* gga, const int length);
    sv_err_t (*set_server_url)(const char* url,int port);
    sv_err_t (*set_mount_point)(const char* mount_point);
    sv_err_t (*set_user_info)(const char* user, const char* pwd);
    sv_err_t (*set_device_id)(const char* device_id);
    sv_err_t (*set_device_type)(const char* device_type);
    sv_err_t (*set_etpr_user_info)(const char* user, const char* pwd, const char* device_id);
    sv_err_t (*set_time_user_info)(const char* user, const char* pwd, const char* device_id);
    sv_err_t (*set_source_node)(int source_Node);
    sv_err_t (*get_expire_date)(char* pDate, int length);
    sv_err_t (*get_frequency)(char* pFreq, int length);
    sv_err_t (*set_log_threshold)(int logThrd);
    sv_err_t (*set_rtcm_freq)(int freq);
}NTRIP_IFACE;




#ifdef __cplusplus
}
#endif

#endif


