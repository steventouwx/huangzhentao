#ifndef  QL_NTRIPADAPTER_H__
#define  QL_NTRIPADAPTER_H__
#include "ql_ntrip.h"

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
    ql_err_t (*init)(ADAPTER_NTRIP_CALLBACK callBack);
    ql_err_t (*deinit)(void);
    ql_err_t (*get_sdk_version)(const char* version);
    ql_err_t (*send_gga)(const char* gga, const int length);
    ql_err_t (*set_server_url)(const char* url,int port);
    ql_err_t (*set_mount_point)(const char* mount_point);
    ql_err_t (*set_user_info)(const char* user, const char* pwd);
    ql_err_t (*set_device_id)(const char* device_id);
    ql_err_t (*set_device_type)(const char* device_type);
    ql_err_t (*set_etpr_user_info)(const char* user, const char* pwd, const char* device_id);
    ql_err_t (*set_time_user_info)(const char* user, const char* pwd, const char* device_id);
    ql_err_t (*set_source_node)(int source_Node);
    ql_err_t (*get_expire_date)(char* pDate, int length);
    ql_err_t (*get_frequency)(char* pFreq, int length);
    ql_err_t (*set_log_threshold)(int logThrd);
    ql_err_t (*set_rtcm_freq)(int freq);
}NTRIP_IFACE;




#ifdef __cplusplus
}
#endif

#endif


