/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_ms_audio_pcm.h
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
#ifndef __QL_MS_AUDIO_PCM_H__
#define __QL_MS_AUDIO_PCM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_audio_defines.h"

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function sets the callback for playback state.

  @param[in] cb  Callback function to report the current playback state.
  @param[in] params  Parameters carried by the callback function.

  */
/*-----------------------------------------------------------------------------------------------*/
void ql_ms_audio_playback_state_ind_cb_set(ql_audio_playback_state_cb_f cb, void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function opens the audio context for playback.

  @param[in] fe_pcm  PCM device type. defined by QL_AUDIO_FE_PCM_DEV_E
  @param[in] be_dai_mask  Backend digit audio interface mask, support follow:
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_PRI_PCM  Play audio to the primary PCM interface
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_SEC_PCM  Play audio to the secondary PCM interface
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_TER_PCM  Play audio to the tertiary PCM interface
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_PRI_I2S  Play audio to the primary I2S interface
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_SEC_I2S  Play audio to the secondary I2S interface
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_TER_I2S  Play audio to the tertiary PCM interface
             QL_AUDIO_BE_DAI_MASK_PLAYBACK_VOICE_TX Play audio to the voice call uplink

  @retval  A_valid_handle   Successful execution.
  @retval  QL_AUDIO_INVALID_HANDLE  Failed execution.Invalid handle
  */
/*-----------------------------------------------------------------------------------------------*/
ql_audio_handle_t ql_ms_audio_playback_open(QL_AUDIO_FE_PCM_DEV_E fe_pcm, uint32_t be_dai_mask);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function prepares for audio file playback.

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().
  @param[in] file_name  The name of the file to be played back.
  @param[in] pcm_config  Pcm config, including sample rate, channel nums,
             defined by ql_audio_pcm_config_t. Generally, it is NULL.
  @param[in] state_cb  Callback function to report the current playback state 
             The states defined by QL_AUDIO_PLAYBACK_STATE_E
  @param[in] params Parameters carried by the callback function.

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others  Failed execution. See ql_type.h for error codes.

  @note  Before calling this function, call ql_ms_audio_playback_open() first to obtain a handle.
         If an audio file is expected to be played back, call this function first to prepare for
         the playback and then ql_ms_audio_playback_play() to start playback.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_file_prepare(ql_audio_handle_t hdl,
                                   const char *file_name,
                                   ql_audio_pcm_config_t *p_config,
                                   ql_audio_playback_state_cb_f state_cb,
                                   void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function prepares for audio stream playback.

  @param[in] hdl  The API ql_ms_audio_playback_open return results
  @param[in] pcm_config  Pcm config, including sample rate, channel nums,
             defined by ql_audio_pcm_config_t. If it is NULL, the API use defaule value.
  @param[in] state_cb  Callback function to report the current playback state.
  @param[in] params  Parameters carried by the callback function.

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  @note  Before calling this function, call ql_ms_audio_playback_open() first to obtain a handle.
         If an audio stream is expected to be played back, call this function first to prepare
         for the audio stream playback and then ql_ms_audio_playback_push_stream() to start playback.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_stream_prepare(ql_audio_handle_t hdl,
                                     ql_audio_pcm_config_t *p_config,
                                     ql_audio_playback_state_cb_f state_cb,
                                     void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function starts playback of the audio data. 

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  @note  Before calling this function, call ql_ms_audio_playback_file_prepare() first to prepare the audio file
         to be played back, otherwise the audio data cannot be played back successfully.This function also supports
         playback of audio stream data. In this case, call ql_ms_audio_playback_stream_prepare() first to prepare
         the audio stream to be played back, then this function to start playback, and finally
         ql_ms_audio_playback_push_stream() to play back the audio stream in buffer.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_play(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the stream buffer size.

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().
  @param[out] p_size  Buffer size. Unit: Byte.

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_buffer_size_get(ql_audio_handle_t hdl, uint32_t *p_size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function plays back the audio stream in buffer.

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().
  @param[in] stream_buf  The buffer that stores the audio stream to be played back.
  @param[in] buf_size  The size of the audio stream to be played back. Unit: Byte.

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_push_stream(ql_audio_handle_t hdl, void *buffer, uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function stops the audio playback. 

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_stop(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function closes the audio context for playback. 

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  @Note  After an audio playback ends, you must call this function to close the audio context,
         otherwise subsequent call of ql_ms_audio_playback_open() will fail.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_close(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function pauses the audio playback. 

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_pause(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function resumes the audio playback. 

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others   Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_resume(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function gets the audio playback state.

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().
  @param[out] p_state  the current audio playback state, defined by QL_AUDIO_PLAYBACK_STATE_E
  */
/*-----------------------------------------------------------------------------------------------*/
void ql_ms_audio_playback_get_state(ql_audio_handle_t hdl, QL_AUDIO_PLAYBACK_STATE_E *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function sets the block flag for audio playback.

  @param[in] hdl  The handle returned by ql_ms_audio_playback_open().
  @param[in] flags  block flag, including QL_AUDIO_PLAYBACK_NONBLOCK and QL_AUDIO_PLAYBACK_BLOCK

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_ARG  Illegal argument.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_playback_set_block_flag(ql_audio_handle_t hdl, uint8_t flags);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function sets the callback for capture state.

  @param[in] cb  Callback function to report the current capture state.
  @param[in] params  Parameters carried by the callback function.

  */
/*-----------------------------------------------------------------------------------------------*/
void ql_ms_audio_capture_state_ind_cb_set(ql_audio_capture_state_cb_f cb, void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function opens the audio context for capturing.

  @param[in] fe_pcm_dev  Front end PCM device type.
  @param[in] be_dai_mask  Back end DAI mask,support follow:
             QL_AUDIO_BE_DAI_MASK_CAPTURE_PRI_PCM  record audio from the primary PCM interface
             QL_AUDIO_BE_DAI_MASK_CAPTURE_SEC_PCM  record audio from the secondary PCM interface
             QL_AUDIO_BE_DAI_MASK_CAPTURE_TER_PCM  record audio from the tertiary PCM interface
             QL_AUDIO_BE_DAI_MASK_CAPTURE_PRI_I2S  record audio from the primary I2S interface
             QL_AUDIO_BE_DAI_MASK_CAPTURE_SEC_I2S  record audio from the secondary I2S interface
             QL_AUDIO_BE_DAI_MASK_CAPTURE_TER_I2S  record audio from the tertiary I2S interface
             QL_AUDIO_BE_DAI_MASK_CAPTURE_VOICE_UL  record audio from the voice downlink
             QL_AUDIO_BE_DAI_MASK_CAPTURE_VOICE_DL  record audio from the voice uplink

  @retval  A_valid_handle  Successful execution
  @retval  QL_AUDIO_INVALID_HANDLE  Failed execution.Invalid handle
  */
/*-----------------------------------------------------------------------------------------------*/
ql_audio_handle_t ql_ms_audio_capture_open(QL_AUDIO_FE_PCM_DEV_E fe_pcm, uint32_t be_dai_mask);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function prepares for audio file capturing.

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().
  @param[in] file_name  The name of the audio file to be captured.
  @param[in] type  The format of the audio data in the audio file.
  @param[in] pcm_config  Pcm config, including sample rate, channel nums,
             defined by ql_audio_pcm_config_t
  @param[in] state_cb  Callback function to report the current audio capturing state.
  @param[in] params  Parameters carried by the callback function.

  @retval  QL_ERR_OK    Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  @note  Before calling this function, call ql_ms_audio_capture_open() first to obtain a handle.
         If an audio file is expected to be captured, call this function first to prepare for the audio file
         capturing and then ql_ms_audio_capture_record() to start capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_file_prepare(ql_audio_handle_t hdl,
                                  const char *file_name,
                                  QL_AUDIO_STREAM_FORMAT_E type,
                                  ql_audio_pcm_config_t *p_config,
                                  ql_audio_capture_state_cb_f state_cb,
                                  void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function prepares for audio stream capturing. 

  @param[in] hdl   The handle returned by ql_ms_audio_capture_open().
  @param[in] pcm_config  PCM configuration parameters. 
  @param[in] state_cb  Callback function to report the current audio capturing state.
  @param[in] params  Parameters carried by the callback function. 

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others  Failed execution. See ql_type.h for error codes.

  @note  Before calling this function, call ql_ms_audio_capture_open() first to obtain a handle.
         If an audio stream is expected to be captured, call this function first to prepare for
         the audio stream capturing and then ql_ms_audio_capture_push_stream() to start capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_stream_prepare(ql_audio_handle_t hdl,
                                    ql_audio_pcm_config_t *p_config,
                                    ql_audio_capture_state_cb_f state_cb,
                                    void *params);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function starts to capture the audio data. 

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  QL_ERR_INVALID_STATE  Failed execution. Invalid state.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  @note Before calling this function, call ql_ms_audio_capture_file_prepare() first to prepare the audio
        file to be captured, otherwise the audio data cannot be captured successfully.This function also
        supports capturing of audio stream data. In this case, call ql_ms_audio_capture_stream_prepare()
        first to prepare the audio stream to be captured, then this function to start capturing,
        and finally ql_ms_audio_capture_pull_stream() to capture the audio stream in buffer.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_record(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function gets the stream buffer size.

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().
  @param[out] p_size  Buffer size. Unit: Byte.

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE    Failed execution. Invalid handle.
  @retval  Others   Failed execution. See ql_type.h for error codes.

  @note  This function should be called after ql_ms_audio_capture_stream_prepare
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_buffer_size_get(ql_audio_handle_t hdl, uint32_t *p_size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function captures the audio stream data to the buffer.

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().
  @param[out] buffer  The buffer that stores the audio stream data to be captured.
  @param[in] size  Buffer size. Unit: Byte.

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_pull_stream(ql_audio_handle_t hdl, void *buffer, uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function stops the audio capturing. 

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See ql_type.h for error codes.

  @note  Calling this function will stop audio capturing regardless of whether the capturing is in
         progress or paused
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_stop(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function closes the audio context for capturing.

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others  Failed execution. See ql_type.h for error codes.
  @note  After audio capturing ends, you must call this function to close the audio context,
         otherwise subsequent call of ql_ms_audio_capture_open() will fail.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_close(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function pauses the audio capturing. 

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().

  @retval  QL_ERR_OK  Successful execution.
  @retval  QL_ERR_INVALID_HANDLE  Failed execution. Invalid handle.
  @retval  Others   Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_pause(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function resumes the audio capturing. 

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().

  @retval  QL_ERR_OK    Successful execution.
  @retval  QL_ERR_INVALID_HANDLE    Failed execution. Invalid handle.
  @retval  Others   Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_capture_resume(ql_audio_handle_t hdl);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function gets the current audio capturing state.

  @param[in] hdl  The handle returned by ql_ms_audio_capture_open().
  @param[out] capture_state  The current audio capturing state.
  */
/*-----------------------------------------------------------------------------------------------*/
void ql_ms_audio_capture_get_state(ql_audio_handle_t hdl, QL_AUDIO_CAPTURE_STATE_E *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief This function convert amr format audio file to wav format file.

  @param[in] amr_file_name  The path of amr file.
  @param[in] wav_file_name  The path of wav file.

  @retval  QL_ERR_OK  Successful execution.
  @retval  Others  Failed execution. See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_decode_amrFile_to_wavFile(const char *amr_file_name, const char *wav_file_name);

#ifdef __cplusplus
}
#endif
#endif


