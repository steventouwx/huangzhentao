/*-----------------------------------------------------------------------------------------------*/
/**
  @file ql_ms_audio_cfg.h
  @brief ms audio config API, including audio initlization, audio configuration

  @detailes
  Quectel AG59x series module AUDIO service.

  @htmlonly
  <span style="font-weight: bold">History</span>
  @endhtmlonly

  when       |   who      |    what, where, why
  --------   |   ---      |    ----------------------------------------------------------
  2024-11-08    ewen.li      Created 

  Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------------------------*/
#ifndef __QL_MS_AUDIO_CFG_H__
#define __QL_MS_AUDIO_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_audio_defines.h"

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function initializes an audio service.

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_SERVICE_NOT_READY  Audio service not ready. Try again later.
  @retval  Others Failed execution.See ql_type.h for error codes
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function deinitializes audio services.

  @retval  QL_ERR_OK Successful
  @retval  QL_ERR_SERVICE_NOT_READY Service is not ready, need to retry
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables/disables loopback.

  @param[in] state  enable/disable the loopback to be set.
             QL_AUDIO_STATE_0: disable, QL_AUDIO_STATE_1: enable

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_loopback_enable_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the loopback state.

  @param[out] p_state  the current loopback state. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_loopback_enable_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables/disables loopback.

  @param[in] type  loopback type, defined by QL_AUDIO_LOOPBACK_STREAM_TYPE_E
  @param[in] state  enable/disable the loopback to be set.
             QL_AUDIO_STATE_0: disable, QL_AUDIO_STATE_1: enable

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_loopback_enable_state_ex(QL_AUDIO_LOOPBACK_STREAM_TYPE_E type, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the loopback state.

  @param[in] type  loopback type, defined by QL_AUDIO_LOOPBACK_STREAM_TYPE_E
  @param[out] p_state  the current loopback state. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_loopback_enable_state_ex(QL_AUDIO_LOOPBACK_STREAM_TYPE_E type, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the tone state.

  @param[in] state  the tone state to be set.
  @param[in] p_config Tone configuration parameters to be set.When enable_state is QL_AUDIO_STATE_1, this parameter must be specified.When enable_state is QL_AUDIO_STATE_0, this parameter is NULL.

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_tone_enable_state(int32_t state, ql_audio_tone_config_t *p_config);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the tone state.

  @param[out] p_state  the current tone state.
              QL_AUDIO_STATE_0: close, QL_AUDIO_STATE_1:open

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_tone_enable_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the voice call manager state. By default, voice call services such as ringtones, ringback
tones, third-party ringtones and voice stream status control are all implemented by ql_audiod program
automatically.

  @param[in] manager_state  The manager state to be set. Voice call services include ringtones, ringback tones, the third-party
ringtones and voice stream status control. range: QL_AUDIO_STATE_0 and QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The voice service, such as call ring, beep tone, will not work during a voice call
         If the manager_state is set to QL_AUDIO_STATE_1
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_voice_call_manager_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the voice call manager state.

  @param[out] p_manager_state  the current voice call manager state.
              QL_AUDIO_STATE_0: close, QL_AUDIO_STATE_1:open

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_voice_call_manager_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the voice stream state.

  @param[in] sim_id sim card id.
  @param[in] stream_state  voice stream state to be set.
             QL_AUDIO_STATE_0: close, QL_AUDIO_STATE_1:open

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  After the voice call manager state is set to QL_AUDIO_STATE_1 by calling
ql_ms_audio_set_voice_call_manager_state(), the service program ql_audiod will not enable voice stream
while establishing a voice call. In such a case, call ql_ms_audio_set_voice_stream_state() to enable voice
stream state by setting stream_state to QL_AUDIO_STATE_1, and then the voice stream can be disabled
by setting stream_state to QL_AUDIO_STATE_0.*/
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_voice_stream_state(QL_SIM_ID_E sim_id, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the voice stream state.

  @param[in] sim_id sim card id.
  @param[out] p_stream_state  the current voice stream state. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_voice_stream_state(QL_SIM_ID_E sim_id, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the microphone gain for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_tx_voice_mic_gain(QL_SIM_ID_E sim_id, int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the microphone gain for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_tx_voice_mic_gain(QL_SIM_ID_E sim_id, int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the volume for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_tx_voice_vol(QL_SIM_ID_E sim_id, int32_t volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the volume for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_tx_voice_vol(QL_SIM_ID_E sim_id, int32_t *p_volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the mute state of voice call uplink.

  @param[in] sim_id sim card id.
  @param[in] mute_state  the mute state to be set.
             QL_AUDIO_STATE_0: unmute, QL_AUDIO_STATE_1: mute

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_tx_voice_mute_state(QL_SIM_ID_E sim_id, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the mute state of voice call uplink.

  @param[in] sim_id sim card id.
  @param[out] p_mute_state  the current mute state of voice call uplink. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_tx_voice_mute_state(QL_SIM_ID_E sim_id, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the speaker gain for voice call downlink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_rx_voice_spkr_gain(QL_SIM_ID_E sim_id, int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gsets the speaker gain for voice call downlink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_rx_voice_spkr_gain(QL_SIM_ID_E sim_id, int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the volume level of voice call downlink.

  @param[in] sim_id sim card id.
  @param[in] vol_step  the volume level to be set. range:0-10

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_rx_voice_vol_step(QL_SIM_ID_E sim_id, int32_t volume_step);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the volume level of voice call downlink.

  @param[in] sim_id sim card id.
  @param[out] p_vol_step  the current volume level of voice call downlink.

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_rx_voice_vol_step(QL_SIM_ID_E sim_id, int32_t *p_volume_step);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the mute state of voice call downlink.

  @param[in] sim_id sim card id.
  @param[in] mute_state  the mute state to be set.
             QL_AUDIO_STATE_0: unmute, QL_AUDIO_STATE_1: mute

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_rx_voice_mute_state(QL_SIM_ID_E sim_id, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the mute state of voice call downlink.

  @param[in] sim_id sim card id.
  @param[out] p_mute_state  the current mute state of voice call downlink. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_rx_voice_mute_state(QL_SIM_ID_E sim_id, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the gain for audio capturing.

  @param[in] the gain to be set. range:0-100

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See ql_type.h for error codes.

  @note  The API should be called during audio capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_audio_tx_mic_gain(int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the gain for audio capturing.

  @param[out] p_mic_gain  the current gain for audio capturing

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during audio capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_audio_tx_mic_gain(int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the gain for audio playback.

  @param[in] codec_gain  the gain to be set. range:0-100

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during audio playback.
  */
/*-----------------------------------------------------------------------------------------------*/

int ql_ms_audio_set_audio_rx_codec_gain(int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the gain for audio playback.

  @param[out] p_codec_gain  the current gain for audio playback.

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during audio playback.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_audio_rx_codec_gain(int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the uplink volume of a codec.

  @param[in] up_volume  the uplink volume to be set. range:0-100

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_codec_up_vol(int32_t volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the uplink volume of a codec.

  @param[out] p_up_volume  the current uplink volume of codec. range:0-100

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_codec_up_vol(int32_t *p_volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the downlink volume of a codec.

  @param[in] down_volume  the volume to be set. range:0-100

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_codec_down_vol(int32_t volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the downlink volume of a codec.

  @param[out] p_down_volume  the current downlink volume of codec. range:0-100

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_codec_down_vol(int32_t *p_volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the microphone mute state of a codec.

  @param[in] mute_state  the muute state to be set.
             QL_AUDIO_STATE_0: unmute, QL_AUDIO_STATE_1: mute

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during the call or audio playback
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_codec_mic_mute_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the microphone mute state of a codec.

  @param[out] p_mute_state  the current microphone mute state of codec. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_codec_mic_mute_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the speaker mute state of a codec.

  @param[in] mute_state  the mute state to be set.
             QL_AUDIO_STATE_0: unmute, QL_AUDIO_STATE_1: mute

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.

  @note  The API should be called during the call or audio playback
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_codec_spk_mute_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the speaker mute state of a codec.

  @param[out] p_mute_state  the current speaker mute state of codec. QL_AUDIO_STATE_0 or QL_AUDIO_STATE_1

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_codec_spk_mute_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the value of a mixer.

  @param[in] control    the name of the mixer
  @param[in] val_list   the value of the mixer to be set. String type value.Multiple values are separated by spaces.

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_mixer_control(const char *ctl_name, const char *ctl_values);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the value of the mixer.

  @param[in]  control       the name of the mixer
  @param[out] val_list_buf  buffer for storing mixer values
  @param[in]  buf_size      the buffer size. Unit:Byte

  @retval  QL_ERR_OK Successful execution.
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See ql_type.h for error codes.

  @note  Generally, 64 bytes is enough
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_mixer_control(const char *ctl_name, char *values, uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the ACDB device mode.

  @param[in] dev_mode  the ACDB device mode to be set. defined by QL_AUDIO_DEVICE_MODE_E

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_acdb_device_mode(QL_AUDIO_ACDB_DEV_MODE_E mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the ACDB device mode.

  @param[out] p_dev_mode  the current ACDB device mode. defined by QL_AUDIO_DEVICE_MODE_E

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See ql_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_acdb_device_mode(QL_AUDIO_ACDB_DEV_MODE_E *p_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the sample rate of PCM interface.

  @param[in] sample_rate  the sample rate of PCM interface to be set. support follow:
             QL_AUDIO_PCM_IF_SAMPLE_RATE_8KHZ
             QL_AUDIO_PCM_IF_SAMPLE_RATE_16KHZ

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_sample_rate(int32_t sample_rate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the sampling rate of PCM interface.

  @param[out] p_sample_rate  the current samping rate of PCM interface. Unit:Hz
              QL_AUDIO_PCM_IF_SAMPLE_RATE_8KHZ or QL_AUDIO_PCM_IF_SAMPLE_RATE_16KHZ

  @retval  QL_ERR_OK Successful execution
  @retval  QL_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_sample_rate(int32_t *p_sample_rate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables/disables UAC.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_uac_enable_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the UAC state.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_uac_enable_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the sidetone gain.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_sidetone_gain(int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the sidetone gain.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_get_sidetone_gain(int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the callback function for audio service errors. 

  @param[in] cb  The callback function for audio service errors.Only when the audio service exit abnormally,the callback function is executed.

  @retval  QL_ERR_OK Successful execution
  @retval  Others errorcode defined by ql_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_ms_audio_set_service_error_cb(ql_audio_service_error_cb_f cb);

#ifdef __cplusplus
}
#endif
#endif // #ifndef __QL_MS_AUDIO_CFG_H__
