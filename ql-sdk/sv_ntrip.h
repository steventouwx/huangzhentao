/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ntrip.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ntrip相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef  SV_NTRIP_H__
#define  SV_NTRIP_H__
#include "sv_type.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    SV_NTRIP_TYPE_UNKNOWN=-1,
    SV_NTRIP_TYPE_GENERAL=0,
    SV_NTRIP_TYPE_CMCC=1,
    SV_NTRIP_TYPE_MAX
}SV_NTRIP_TYPE_E;
typedef void (*CorrectionDataCb)(void* data, int length);
typedef void (*NtripStatusCb)(int status);

typedef struct 
{
    CorrectionDataCb correctionDataCb;
    NtripStatusCb ntripStatusCb;
}SV_NTRIP_CALLBACK;

sv_err_t sv_ntrip_init(SV_NTRIP_TYPE_E type,SV_NTRIP_CALLBACK call_back);
sv_err_t sv_ntrip_deinit(void);
sv_err_t sv_ntrip_get_sdk_version(char* version);
sv_err_t sv_ntrip_send_gga(const char* ggaRawBuf, const int length);
sv_err_t sv_ntrip_set_server_url(const char* url,int port);
sv_err_t sv_ntrip_set_mount_point(const char* mountpoint);
sv_err_t sv_ntrip_set_user_info(const char* user, const char* pwd);
sv_err_t sv_ntrip_set_device_id(const char* device_id);
sv_err_t sv_ntrip_set_device_type(const char* device_id);
sv_err_t sv_ntrip_set_etpr_user_info(const char* user, const char* pwd, const char* device_id);
sv_err_t sv_ntrip_set_time_user_info(const char* user, const char* pwd, const char* device_id);
sv_err_t sv_ntrip_set_source_node(int sourceNode);
sv_err_t sv_ntrip_get_expire_date(char* pDate, int length);
sv_err_t sv_ntrip_get_frequency(char* pFreq, int length);
sv_err_t sv_ntrip_set_log_threshold(int logThrd);
sv_err_t sv_ntrip_set_rtcm_freq(int FreqPort);
/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Inect RTCM data to RTK Engine 【Note】: Cannot be used together with ntrip interface
  @param[in]  data  RTCM data pointer.
  @param[in]  length  RTCM data length.
  @return Whether inject RTCM data to RTK engine  successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_FAILED unknown error
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
sv_err_t sv_inject_rtcm(void* data, int length);

#ifdef __cplusplus
}
#endif

#endif

