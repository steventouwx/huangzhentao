/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_eavb_common.h
 * 编写者: AutoAgent
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_eavb_common相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_eavb_common.h 
  @brief eavb API Struct
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

#ifndef __SV_EAVB_COMMON_H__
#define __SV_EAVB_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_AUDIO_FILE_NAME 100

/** AVTP work role */
typedef enum {
    SV_EAVB_ROLE_TALKER,   /**< Talker only */
    SV_EAVB_ROLE_LISTENER  /**< Listener only */
}SV_EAVB_AVTP_E;

/** Audio device */
typedef enum {
    SV_EAVB_FE_PCM_DEV_E_MIN_ENUM_VAL = -1,
    SV_EAVB_FE_PCM_DEV_MULTIMEDIA1,        /**< The second PCM for Voice Call */
    SV_EAVB_FE_PCM_DEV_MULTIMEDIA2,        /**< The primary I2S  Voice Call*/
    SV_EAVB_FE_PCM_DEV_MULTIMEDIA3,        /**< The second PCM for local codec*/
    SV_EAVB_FE_PCM_DEV_MULTIMEDIA4,        /**< The primary I2S for local codec*/
    SV_EAVB_FE_PCM_DEV_MULTIMEDIA5,        /**< Not support */
    SV_EAVB_FE_PCM_DEV_AFE_PCM_RX,         /**< Not support */
    SV_EAVB_FE_PCM_DEV_AFE_PCM_TX,         /**< Not support */
    SV_EAVB_FE_PCM_DEV_E_MAX_ENUM_VAL
}SV_EAVB_FE_PCM_DEV_E;

/** GPTP mode */
typedef enum {
    SV_EAVB_GPTP_SLAVE,   /**< GPTP SLAVE MODE */
    SV_EAVB_GPTP_MASTER   /**< GPTP MASTER MODE */
}SV_EAVB_GPTP_E;

/** Daemon ON/OFF */
typedef enum {
    SV_EAVB_STATUS_OFF,   /**< Status OFF */
    SV_EAVB_STATUS_ON     /**< Status ON */
}SV_EAVB_STATUS_E;

/** Ready to play audio */
typedef enum {
    SV_EAVB_IS_READY_PLAY,   /**< Is ready to play audio */
    SV_EAVB_NOT_READY_PLAY   /**< not ready to play audio */
}SV_EAVB_READY_PLAY_E;

/** endpoint config */
typedef struct {
    char ifname[20];      /**< Etherner name */
    int srp_disable;      /**< disable SRP or not */
}sv_eavb_srp_t;

/** GPTP config */
typedef struct {
    char ifname[20];      /**< GPTP on the ethernet */
    SV_EAVB_GPTP_E mode;  /**< GPTP mode */
    int automotive;       /**< gPTP vehicle profile disable/enable item */
}sv_eavb_gptp_t;

/** AVTP AAF stream config */
typedef struct {
    SV_EAVB_AVTP_E role;                 /**< aaf stream role */
    int tx_rate;                         /**< Number of audio packets send per second */
    int rate;                            /**< Audio sampling rate (8000, 16000, 44100, 48000) */
    int stream_uid;                      /**< AVTP stream ID */
    int channel;                         /**< Audio channel */
    int bit;                             /**< Audio sampling bit */
    int pack_factor;                     /**< Sets number of items to send or receive on each Tx and Rx, respectively.If there is no special requirement, please keep it as 1 */
    SV_EAVB_FE_PCM_DEV_E audio_device;   /**< Audio device number,see: SV_EAVB_FE_PCM_DEV_E */
    char stream_addr[18];                /**< Stream addr, this should be set same as Talker macaddr */
    char dest_macaddr[18];               /**< 1722 protocol multicast MAC address (91:E0:F0:00:FE:00-91:E0:F0:00:FE:FF),different stream should be set differently */
    char sr_class[2];                    /**< Stream type,only support the value:A-classA, B-classB, C-classAAF */
}sv_eavb_aaf_t;

/** EAVB functional scenarios config */
typedef struct{
    SV_EAVB_STATUS_E status;   /**< scene status */
    int scenes_id;             /**< scene ID */
}sv_eavb_scenes_status_t;

/** check EAVB is ready to play audio*/
typedef struct{
    SV_EAVB_READY_PLAY_E is_ready;   /**< Ready to play audio */
    int scenes_id;                 /**< scene ID */
}sv_eavb_scenes_check_ready_play_t;

/** play audio file*/
typedef struct{
    int stream_uid;                        /**< aaf stream uid */
    int cycle;                            /**< Cycle play */
    int auto_stop;                        /**< 0:auto stop when voice call start 1:auto stop when answer voice call*/
    char filename[MAX_AUDIO_FILE_NAME];   /**< Audio play file name*/
}sv_eavb_play_audio_file_t;

#ifdef __cplusplus
}
#endif
#endif

