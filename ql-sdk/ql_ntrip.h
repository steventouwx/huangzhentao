/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_ntrip.h 
  @brief Interface for ntrip client api

  @detailes
  Quectel AG55x series module Ntrip Api.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly
  
  when     |  who      |    what, where, why
  -------- |  ---      |    ----------------------------------------------------------
  20230323 |  raigor.zhao  |    Created .
  
  Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

#ifndef  QL_NTRIP_H__
#define  QL_NTRIP_H__
#include "ql_type.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    QL_NTRIP_TYPE_UNKNOWN=-1,
    QL_NTRIP_TYPE_GENERAL=0,
    QL_NTRIP_TYPE_CMCC=1,
    QL_NTRIP_TYPE_MAX
}QL_NTRIP_TYPE_E;
typedef void (*CorrectionDataCb)(void* data, int length);
typedef void (*NtripStatusCb)(int status);

typedef struct 
{
    CorrectionDataCb correctionDataCb;
    NtripStatusCb ntripStatusCb;
}QL_NTRIP_CALLBACK;


ql_err_t ql_ntrip_init(QL_NTRIP_TYPE_E type,QL_NTRIP_CALLBACK call_back);
ql_err_t ql_ntrip_deinit(void);
ql_err_t ql_ntrip_get_sdk_version(char* version);
ql_err_t ql_ntrip_send_gga(const char* ggaRawBuf, const int length);
ql_err_t ql_ntrip_set_server_url(const char* url,int port);
ql_err_t ql_ntrip_set_mount_point(const char* mountpoint);
ql_err_t ql_ntrip_set_user_info(const char* user, const char* pwd);
ql_err_t ql_ntrip_set_device_id(const char* device_id);
ql_err_t ql_ntrip_set_device_type(const char* device_id);
ql_err_t ql_ntrip_set_etpr_user_info(const char* user, const char* pwd, const char* device_id);
ql_err_t ql_ntrip_set_time_user_info(const char* user, const char* pwd, const char* device_id);
ql_err_t ql_ntrip_set_source_node(int sourceNode);
ql_err_t ql_ntrip_get_expire_date(char* pDate, int length);
ql_err_t ql_ntrip_get_frequency(char* pFreq, int length);
ql_err_t ql_ntrip_set_log_threshold(int logThrd);
ql_err_t ql_ntrip_set_rtcm_freq(int FreqPort);
/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Inect RTCM data to RTK Engine 【Note】: Cannot be used together with ntrip interface
  @param[in]  data  RTCM data pointer.
  @param[in]  length  RTCM data length.
  @return Whether inject RTCM data to RTK engine  successfully.
  @retval QL_ERR_OK successful.
  @retval QL_ERR_INVALID_ARG invalid argument.
  @retval QL_ERR_FAILED unknown error
  @retval QL_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
ql_err_t ql_inject_rtcm(void* data, int length);


#ifdef __cplusplus
}
#endif

#endif

