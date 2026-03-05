/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ms_audio_pcm.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ms_audio_pcm相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
  @file sv_ms_audio_pcm.h
  @brief MS playback or capture API
*/
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.
  $Header: $
  when          who          what, where, why
  --------      ---          ----------------------------------------------------------
  2024-11-08    ewen.li      Created .
-------------------------------------------------------------------------------------------------*/
#ifndef __SV_MS_AUDIO_PCM_H__
#define __SV_MS_AUDIO_PCM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sv_audio_defines.h"

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function sets the callback for playback state.

  @param[in] cb  Callback function to report the current playback state.
  @param[in] params  Parameters carried by the callback function.

  */
/*-----------------------------------------------------------------------------------------------*/
void sv_ms_audio_playback_state_ind_cb_set(sv_audio_playback_state_cb_f cb, void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function opens the audio context for playback.

  @param[in] fe_pcm  PCM device type. defined by SV_AUDIO_FE_PCM_DEV_E
  @param[in] be_dai_mask  Backend digit audio interface mask, support follow:
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_PRI_PCM  Play audio to the primary PCM interface
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_SEC_PCM  Play audio to the secondary PCM interface
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_TER_PCM  Play audio to the tertiary PCM interface
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_PRI_I2S  Play audio to the primary I2S interface
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_SEC_I2S  Play audio to the secondary I2S interface
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_TER_I2S  Play audio to the tertiary PCM interface
             SV_AUDIO_BE_DAI_MASK_PLAYBACK_VOICE_TX Play audio to the voice call uplink

  @retval  A_valid_handle   Successful execution.
  @retval  SV_AUDIO_INVALID_HANDLE  Failed execution.Invalid handle
  */
/*-----------------------------------------------------------------------------------------------*/
sv_audio_handle_t sv_ms_audio_playback_open(SV_AUDIO_FE_PCM_DEV_E fe_pcm, uint32_t be_dai_mask);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function prepares for audio file playback.

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().
  @param[in] file_name  The name of the file to be played back.
  @param[in] pcm_config  Pcm config, including sample rate, channel nums,
             defined by sv_audio_pcm_config_t. Generally, it is NULL.
  @param[in] state_cb  Callback function to report the current playback state 
             The states defined by SV_AUDIO_PLAYBACK_STATE_E
  @param[in] params Parameters carried by the callback function.

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others  Failed execution. See sv_type.h for error codes.

  @note  Before calling this function, call sv_ms_audio_playback_open() first to obtain a handle.
         If an audio file is expected to be played back, call this function first to prepare for
         the playback and then sv_ms_audio_playback_play() to start playback.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_file_prepare(sv_audio_handle_t hdl,
                                   const char *file_name,
                                   sv_audio_pcm_config_t *p_config,
                                   sv_audio_playback_state_cb_f state_cb,
                                   void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function prepares for audio stream playback.

  @param[in] hdl  The API sv_ms_audio_playback_open return results
  @param[in] pcm_config  Pcm config, including sample rate, channel nums,
             defined by sv_audio_pcm_config_t. If it is NULL, the API use defaule value.
  @param[in] state_cb  Callback function to report the current playback state.
  @param[in] params  Parameters carried by the callback function.

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  @note  Before calling this function, call sv_ms_audio_playback_open() first to obtain a handle.
         If an audio stream is expected to be played back, call this function first to prepare
         for the audio stream playback and then sv_ms_audio_playback_push_stream() to start playback.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_stream_prepare(sv_audio_handle_t hdl,
                                     sv_audio_pcm_config_t *p_config,
                                     sv_audio_playback_state_cb_f state_cb,
                                     void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function starts playback of the audio data. 

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  @note  Before calling this function, call sv_ms_audio_playback_file_prepare() first to prepare the audio file
         to be played back, otherwise the audio data cannot be played back successfully.This function also supports
         playback of audio stream data. In this case, call sv_ms_audio_playback_stream_prepare() first to prepare
         the audio stream to be played back, then this function to start playback, and finally
         sv_ms_audio_playback_push_stream() to play back the audio stream in buffer.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_play(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the stream buffer size.

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().
  @param[out] p_size  Buffer size. Unit: Byte.

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_buffer_size_get(sv_audio_handle_t hdl, uint32_t *p_size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function plays back the audio stream in buffer.

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().
  @param[in] stream_buf  The buffer that stores the audio stream to be played back.
  @param[in] buf_size  The size of the audio stream to be played back. Unit: Byte.

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_push_stream(sv_audio_handle_t hdl, void *buffer, uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function stops the audio playback. 

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_stop(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function closes the audio context for playback. 

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  @Note  After an audio playback ends, you must call this function to close the audio context,
         otherwise subsequent call of sv_ms_audio_playback_open() will fail.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_close(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function pauses the audio playback. 

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_pause(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function resumes the audio playback. 

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others   Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_resume(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function gets the audio playback state.

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().
  @param[out] p_state  the current audio playback state, defined by SV_AUDIO_PLAYBACK_STATE_E
  */
/*-----------------------------------------------------------------------------------------------*/
void sv_ms_audio_playback_get_state(sv_audio_handle_t hdl, SV_AUDIO_PLAYBACK_STATE_E *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function sets the block flag for audio playback.

  @param[in] hdl  The handle returned by sv_ms_audio_playback_open().
  @param[in] flags  block flag, including SV_AUDIO_PLAYBACK_NONBLOCK and SV_AUDIO_PLAYBACK_BLOCK

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_ARG  Illegal argument.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_playback_set_block_flag(sv_audio_handle_t hdl, uint8_t flags);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function sets the callback for capture state.

  @param[in] cb  Callback function to report the current capture state.
  @param[in] params  Parameters carried by the callback function.

  */
/*-----------------------------------------------------------------------------------------------*/
void sv_ms_audio_capture_state_ind_cb_set(sv_audio_capture_state_cb_f cb, void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function opens the audio context for capturing.

  @param[in] fe_pcm_dev  Front end PCM device type.
  @param[in] be_dai_mask  Back end DAI mask,support follow:
             SV_AUDIO_BE_DAI_MASK_CAPTURE_PRI_PCM  record audio from the primary PCM interface
             SV_AUDIO_BE_DAI_MASK_CAPTURE_SEC_PCM  record audio from the secondary PCM interface
             SV_AUDIO_BE_DAI_MASK_CAPTURE_TER_PCM  record audio from the tertiary PCM interface
             SV_AUDIO_BE_DAI_MASK_CAPTURE_PRI_I2S  record audio from the primary I2S interface
             SV_AUDIO_BE_DAI_MASK_CAPTURE_SEC_I2S  record audio from the secondary I2S interface
             SV_AUDIO_BE_DAI_MASK_CAPTURE_TER_I2S  record audio from the tertiary I2S interface
             SV_AUDIO_BE_DAI_MASK_CAPTURE_VOICE_UL  record audio from the voice downlink
             SV_AUDIO_BE_DAI_MASK_CAPTURE_VOICE_DL  record audio from the voice uplink

  @retval  A_valid_handle  Successful execution
  @retval  SV_AUDIO_INVALID_HANDLE  Failed execution.Invalid handle
  */
/*-----------------------------------------------------------------------------------------------*/
sv_audio_handle_t sv_ms_audio_capture_open(SV_AUDIO_FE_PCM_DEV_E fe_pcm, uint32_t be_dai_mask);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function prepares for audio file capturing.

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().
  @param[in] file_name  The name of the audio file to be captured.
  @param[in] type  The format of the audio data in the audio file.
  @param[in] pcm_config  Pcm config, including sample rate, channel nums,
             defined by sv_audio_pcm_config_t
  @param[in] state_cb  Callback function to report the current audio capturing state.
  @param[in] params  Parameters carried by the callback function.

  @retval  SV_ERR_OK    Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  @note  Before calling this function, call sv_ms_audio_capture_open() first to obtain a handle.
         If an audio file is expected to be captured, call this function first to prepare for the audio file
         capturing and then sv_ms_audio_capture_record() to start capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_file_prepare(sv_audio_handle_t hdl,
                                  const char *file_name,
                                  SV_AUDIO_STREAM_FORMAT_E type,
                                  sv_audio_pcm_config_t *p_config,
                                  sv_audio_capture_state_cb_f state_cb,
                                  void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function prepares for audio stream capturing. 

  @param[in] hdl   The handle returned by sv_ms_audio_capture_open().
  @param[in] pcm_config  PCM configuration parameters. 
  @param[in] state_cb  Callback function to report the current audio capturing state.
  @param[in] params  Parameters carried by the callback function. 

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others  Failed execution. See sv_type.h for error codes.

  @note  Before calling this function, call sv_ms_audio_capture_open() first to obtain a handle.
         If an audio stream is expected to be captured, call this function first to prepare for
         the audio stream capturing and then sv_ms_audio_capture_push_stream() to start capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_stream_prepare(sv_audio_handle_t hdl,
                                    sv_audio_pcm_config_t *p_config,
                                    sv_audio_capture_state_cb_f state_cb,
                                    void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function starts to capture the audio data. 

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  SV_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  @note Before calling this function, call sv_ms_audio_capture_file_prepare() first to prepare the audio
        file to be captured, otherwise the audio data cannot be captured successfully.This function also
        supports capturing of audio stream data. In this case, call sv_ms_audio_capture_stream_prepare()
        first to prepare the audio stream to be captured, then this function to start capturing,
        and finally sv_ms_audio_capture_pull_stream() to capture the audio stream in buffer.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_record(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function gets the stream buffer size.

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().
  @param[out] p_size  Buffer size. Unit: Byte.

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE    Failed execution. Invalid handle.
  @retval  Others   Failed execution. See sv_type.h for error codes.

  @note  This function should be called after sv_ms_audio_capture_stream_prepare
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_buffer_size_get(sv_audio_handle_t hdl, uint32_t *p_size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function captures the audio stream data to the buffer.

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().
  @param[out] buffer  The buffer that stores the audio stream data to be captured.
  @param[in] size  Buffer size. Unit: Byte.

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_pull_stream(sv_audio_handle_t hdl, void *buffer, uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function stops the audio capturing. 

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See sv_type.h for error codes.

  @note  Calling this function will stop audio capturing regardless of whether the capturing is in
         progress or paused
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_stop(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function closes the audio context for capturing.

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See sv_type.h for error codes.
  @note  After audio capturing ends, you must call this function to close the audio context,
         otherwise subsequent call of sv_ms_audio_capture_open() will fail.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_close(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function pauses the audio capturing. 

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().

  @retval  SV_ERR_OK  Successful execution.
  @retval  SV_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others   Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_pause(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function resumes the audio capturing. 

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().

  @retval  SV_ERR_OK    Successful execution.
  @retval  SV_ERR_INVALID_HANDLE    Failed execution. Invalid handle.
  @retval  Others   Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_capture_resume(sv_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function gets the current audio capturing state.

  @param[in] hdl  The handle returned by sv_ms_audio_capture_open().
  @param[out] capture_state  The current audio capturing state.
  */
/*-----------------------------------------------------------------------------------------------*/
void sv_ms_audio_capture_get_state(sv_audio_handle_t hdl, SV_AUDIO_CAPTURE_STATE_E *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function convert amr format audio file to wav format file.

  @param[in] amr_file_name  The path of amr file.
  @param[in] wav_file_name  The path of wav file.

  @retval  SV_ERR_OK  Successful execution.
  @retval  Others  Failed execution. See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_decode_amrFile_to_wavFile(const char *amr_file_name, const char *wav_file_name);

#ifdef __cplusplus
}
#endif
#endif


