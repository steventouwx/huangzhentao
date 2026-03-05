/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_eavb_common.h 
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

#ifndef __QL_EAVB_COMMON_H__
#define __QL_EAVB_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_AUDIO_FILE_NAME 100

/** AVTP work role */
typedef enum {
    QL_EAVB_ROLE_TALKER,   /**< Talker only */
    QL_EAVB_ROLE_LISTENER  /**< Listener only */
}QL_EAVB_AVTP_E;

/** Audio device */
typedef enum {
    QL_EAVB_FE_PCM_DEV_E_MIN_ENUM_VAL = -1,
    QL_EAVB_FE_PCM_DEV_MULTIMEDIA1,        /**< The second PCM for Voice Call */
    QL_EAVB_FE_PCM_DEV_MULTIMEDIA2,        /**< The primary I2S  Voice Call*/
    QL_EAVB_FE_PCM_DEV_MULTIMEDIA3,        /**< The second PCM for local codec*/
    QL_EAVB_FE_PCM_DEV_MULTIMEDIA4,        /**< The primary I2S for local codec*/
    QL_EAVB_FE_PCM_DEV_MULTIMEDIA5,        /**< Not support */
    QL_EAVB_FE_PCM_DEV_AFE_PCM_RX,         /**< Not support */
    QL_EAVB_FE_PCM_DEV_AFE_PCM_TX,         /**< Not support */
    QL_EAVB_FE_PCM_DEV_E_MAX_ENUM_VAL
}QL_EAVB_FE_PCM_DEV_E;

/** GPTP mode */
typedef enum {
    QL_EAVB_GPTP_SLAVE,   /**< GPTP SLAVE MODE */
    QL_EAVB_GPTP_MASTER   /**< GPTP MASTER MODE */
}QL_EAVB_GPTP_E;

/** Daemon ON/OFF */
typedef enum {
    QL_EAVB_STATUS_OFF,   /**< Status OFF */
    QL_EAVB_STATUS_ON     /**< Status ON */
}QL_EAVB_STATUS_E;

/** Ready to play audio */
typedef enum {
    QL_EAVB_IS_READY_PLAY,   /**< Is ready to play audio */
    QL_EAVB_NOT_READY_PLAY   /**< not ready to play audio */
}QL_EAVB_READY_PLAY_E;

/** endpoint config */
typedef struct {
    char ifname[20];      /**< Etherner name */
    int srp_disable;      /**< disable SRP or not */
}ql_eavb_srp_t;

/** GPTP config */
typedef struct {
    char ifname[20];      /**< GPTP on the ethernet */
    QL_EAVB_GPTP_E mode;  /**< GPTP mode */
    int automotive;       /**< gPTP vehicle profile disable/enable item */
}ql_eavb_gptp_t;

/** AVTP AAF stream config */
typedef struct {
    QL_EAVB_AVTP_E role;                 /**< aaf stream role */
    int tx_rate;                         /**< Number of audio packets send per second */
    int rate;                            /**< Audio sampling rate (8000, 16000, 44100, 48000) */
    int stream_uid;                      /**< AVTP stream ID */
    int channel;                         /**< Audio channel */
    int bit;                             /**< Audio sampling bit */
    int pack_factor;                     /**< Sets number of items to send or receive on each Tx and Rx, respectively.If there is no special requirement, please keep it as 1 */
    QL_EAVB_FE_PCM_DEV_E audio_device;   /**< Audio device number,see: QL_EAVB_FE_PCM_DEV_E */
    char stream_addr[18];                /**< Stream addr, this should be set same as Talker macaddr */
    char dest_macaddr[18];               /**< 1722 protocol multicast MAC address (91:E0:F0:00:FE:00-91:E0:F0:00:FE:FF),different stream should be set differently */
    char sr_class[2];                    /**< Stream type,only support the value:A-classA, B-classB, C-classAAF */
}ql_eavb_aaf_t;

/** EAVB functional scenarios config */
typedef struct{
    QL_EAVB_STATUS_E status;   /**< scene status */
    int scenes_id;             /**< scene ID */
}ql_eavb_scenes_status_t;

/** check EAVB is ready to play audio*/
typedef struct{
    QL_EAVB_READY_PLAY_E is_ready;   /**< Ready to play audio */
    int scenes_id;                 /**< scene ID */
}ql_eavb_scenes_check_ready_play_t;

/** play audio file*/
typedef struct{
    int stream_uid;                        /**< aaf stream uid */
    int cycle;                            /**< Cycle play */
    int auto_stop;                        /**< 0:auto stop when voice call start 1:auto stop when answer voice call*/
    char filename[MAX_AUDIO_FILE_NAME];   /**< Audio play file name*/
}ql_eavb_play_audio_file_t;

#ifdef __cplusplus
}
#endif
#endif

