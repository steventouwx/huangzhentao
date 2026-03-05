/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_eavb.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_eavb相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_eavb.h 
  @brief eavb service API 
*/
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.
  $Header: $
  when         who          what, where, why
  ----------   ---          ----------------------------------------------------------
  2024/02/19   kemp.li      Created .
-------------------------------------------------------------------------------------------------*/

#ifndef __SV_EAVB_H__
#define __SV_EAVB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sv_eavb_common.h>

/** EAVB service error callback */
typedef void (*sv_eavb_service_error_cb_f)(int error);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Initializes eavb service.
  @return Whether the EAVB service was initialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_init(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Initializes eavb service.
  @return Whether the EAVB service was deinitialized successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Set gptp config.
  @param[in] config    gptp config, define by sv_eavb_gptp_t.
  @return Whether set GPTP config successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_gptp_conf_set(sv_eavb_gptp_t *config);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Get gptp config.
  @param[out] config    gptp config buffer, define by sv_eavb_gptp_t
  @return Whether get GPTP config successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_gptp_conf_get(sv_eavb_gptp_t *config);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set SRP config.
  @param[in] config    SRP config, define by sv_eavb_srp_t
  @return Whether set SRP config successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_srp_conf_set(sv_eavb_srp_t *config);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Get SRP config.
  @param[out] config    SRP config buffer, define by sv_eavb_srp_t
  @return Whether get SRP config successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_srp_conf_get(sv_eavb_srp_t *config);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set aaf config.
  @param[in] config    aaf stream config, define by sv_eavb_aaf_t
  @return Whether set AAF stream successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_aaf_conf_set( sv_eavb_aaf_t *config);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Get aaf config. need set config->stream_uid
  @param[in]  stream_uid     aaf stream UID
  @param[out] config        aaf stream config buffer, define by sv_eavb_aaf_t
  @return Whether get AAF stream successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_aaf_conf_get(int stream_uid, sv_eavb_aaf_t *config);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Add aaf stream config.
  @param[in] config      aaf stream config, define by sv_eavb_aaf_t
  @return Whether add aaf stream successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_add_aaf_stream(sv_eavb_aaf_t *config);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Delete aaf stream config.
  @param[in] stream_uid    aaf stream UID
  @return Whether delete aaf stream successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_del_aaf_stream(int stream_uid);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set gPTP service start/stop.
  @param[in] state    service status, define by SV_EAVB_STATUS_E
  @return Whether set GPTP service status successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_set_gptp_service_status(SV_EAVB_STATUS_E state);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get gPTP service start/stop status.
  @param[out] state    service status, define by SV_EAVB_STATUS_E
  @return Whether get GPTP service status successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_get_gptp_service_status(SV_EAVB_STATUS_E *state);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  set SRP service start/stop.
  @param[in] state    service status, define by SV_EAVB_STATUS_E
  @return Whether set SRP service status successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_set_srp_service_status(SV_EAVB_STATUS_E state);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  get SRP service start/stop status.
  @param[out] state    service status, define by SV_EAVB_STATUS_E
  @return Whether get SRP service status successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_get_srp_service_status(SV_EAVB_STATUS_E *state);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Add an AVTP application scenes.
  @param[in] scenes_id    AVTP application scenes ID
  @return Whether add application scenes successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_add_scenes(int scenes_id);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Delete an AVTP application scenes.
  @param[in] scenes_id    AVTP application scenes ID
  @return Whether delete application scenes successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_del_scenes(int scenes_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Add an exist stream to an exist scenes.
  @param[in] stream_uid    stream UID
  @param[in] scenes_id     AVTP application scenes
  @return Whether add a AAF stream to a scenes successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_add_stream_to_scenes(int stream_uid, int scenes_id);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Delete an exist stream from an exist scenes.
  @param[in] stream_uid    stream UID
  @param[in] scenes_id     AVTP application scenes
  @return Whether delete a AAF stream from a scenes successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_del_stream_from_scenes(int stream_uid, int scenes_id);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Set an exist scenes start/stop.
  @param[in] state    scenes config, defined by sv_eavb_scenes_status_t
  @return Whether set application scenes status successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_set_scenes_status(sv_eavb_scenes_status_t *state);


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Get an exist scenes status.
  @param[in] scenes_id    AVTP application scenes ID
  @param[out] state       scenes config buffer, defined by sv_eavb_scenes_status_t
  @return Whether get application scenes status successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_get_scenes_status(int scenes_id, sv_eavb_scenes_status_t *state);

/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Check the scenes is ready to play audio or not.
  @param[in] scenes_id    AVTP application scenes ID
  @param[out] ready       ready config, defined by sv_eavb_scenes_check_ready_play_t
  @return Whether get successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_check_ready_play_audio(int scenes_id, sv_eavb_scenes_check_ready_play_t *ready);
/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief  Start play audio file to stream.
  @param[in] info     play audio file info
  @return Whether get successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_start_play_audio_file(sv_eavb_play_audio_file_t *info);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Stop play file
  @param[in] stream_uid    aaf stream UID
  @return Whether delete aaf stream successfully.
  @retval SV_ERR_OK successful.
  @retval SV_ERR_INVALID_ARG invalid argument.
  @retval SV_ERR_NOT_INIT can not init the service.
  @retval SV_ERR_SERVICE_NOT_READY service is not ready, need to retry.
  @retval Other error code defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_stop_play_audio_file(int stream_uid);


/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  Registration server error callback. Currently, only if the server exits abnormally, 
  the callback function will be executed, and the error code is SV_ERR_ABORTED;
  @param[in] cb  Callback function 
  @return
  SV_ERR_OK - successful
  Other - error code defined by sv_type.h
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_eavb_set_service_error_cb(sv_eavb_service_error_cb_f cb);

#ifdef __cplusplus
} 
#endif

#endif

