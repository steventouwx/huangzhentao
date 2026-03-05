/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_ms_audio_cfg.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_ms_audio_cfg相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

#ifndef __SV_MS_AUDIO_CFG_H__
#define __SV_MS_AUDIO_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sv_audio_defines.h"

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function initializes an audio service.

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_SERVICE_NOT_READY  Audio service not ready. Try again later.
  @retval  Others Failed execution.See sv_type.h for error codes
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_init(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function deinitializes audio services.

  @retval  SV_ERR_OK Successful
  @retval  SV_ERR_SERVICE_NOT_READY Service is not ready, need to retry
  @retval  Others Failed execution.See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_deinit(void);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables/disables loopback.

  @param[in] state  enable/disable the loopback to be set.
             SV_AUDIO_STATE_0: disable, SV_AUDIO_STATE_1: enable

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_loopback_enable_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the loopback state.

  @param[out] p_state  the current loopback state. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_loopback_enable_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables/disables loopback.

  @param[in] type  loopback type, defined by SV_AUDIO_LOOPBACK_STREAM_TYPE_E
  @param[in] state  enable/disable the loopback to be set.
             SV_AUDIO_STATE_0: disable, SV_AUDIO_STATE_1: enable

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_loopback_enable_state_ex(SV_AUDIO_LOOPBACK_STREAM_TYPE_E type, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the loopback state.

  @param[in] type  loopback type, defined by SV_AUDIO_LOOPBACK_STREAM_TYPE_E
  @param[out] p_state  the current loopback state. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_loopback_enable_state_ex(SV_AUDIO_LOOPBACK_STREAM_TYPE_E type, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the tone state.

  @param[in] state  the tone state to be set.
  @param[in] p_config Tone configuration parameters to be set.When enable_state is SV_AUDIO_STATE_1, this parameter must be specified.When enable_state is SV_AUDIO_STATE_0, this parameter is NULL.

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_tone_enable_state(int32_t state, sv_audio_tone_config_t *p_config);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the tone state.

  @param[out] p_state  the current tone state.
              SV_AUDIO_STATE_0: close, SV_AUDIO_STATE_1:open

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_tone_enable_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the voice call manager state. By default, voice call services such as ringtones, ringback
tones, third-party ringtones and voice stream status control are all implemented by sv_audiod program
automatically.

  @param[in] manager_state  The manager state to be set. Voice call services include ringtones, ringback tones, the third-party
ringtones and voice stream status control. range: SV_AUDIO_STATE_0 and SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The voice service, such as call ring, beep tone, will not work during a voice call
         If the manager_state is set to SV_AUDIO_STATE_1
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_voice_call_manager_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the voice call manager state.

  @param[out] p_manager_state  the current voice call manager state.
              SV_AUDIO_STATE_0: close, SV_AUDIO_STATE_1:open

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_voice_call_manager_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the voice stream state.

  @param[in] sim_id sim card id.
  @param[in] stream_state  voice stream state to be set.
             SV_AUDIO_STATE_0: close, SV_AUDIO_STATE_1:open

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  After the voice call manager state is set to SV_AUDIO_STATE_1 by calling
sv_ms_audio_set_voice_call_manager_state(), the service program sv_audiod will not enable voice stream
while establishing a voice call. In such a case, call sv_ms_audio_set_voice_stream_state() to enable voice
stream state by setting stream_state to SV_AUDIO_STATE_1, and then the voice stream can be disabled
by setting stream_state to SV_AUDIO_STATE_0.*/
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_voice_stream_state(SV_SIM_ID_E sim_id, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the voice stream state.

  @param[in] sim_id sim card id.
  @param[out] p_stream_state  the current voice stream state. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_voice_stream_state(SV_SIM_ID_E sim_id, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the microphone gain for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_tx_voice_mic_gain(SV_SIM_ID_E sim_id, int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the microphone gain for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_tx_voice_mic_gain(SV_SIM_ID_E sim_id, int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the volume for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_tx_voice_vol(SV_SIM_ID_E sim_id, int32_t volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the volume for voice call uplink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_tx_voice_vol(SV_SIM_ID_E sim_id, int32_t *p_volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the mute state of voice call uplink.

  @param[in] sim_id sim card id.
  @param[in] mute_state  the mute state to be set.
             SV_AUDIO_STATE_0: unmute, SV_AUDIO_STATE_1: mute

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_tx_voice_mute_state(SV_SIM_ID_E sim_id, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the mute state of voice call uplink.

  @param[in] sim_id sim card id.
  @param[out] p_mute_state  the current mute state of voice call uplink. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_tx_voice_mute_state(SV_SIM_ID_E sim_id, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the speaker gain for voice call downlink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_rx_voice_spkr_gain(SV_SIM_ID_E sim_id, int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gsets the speaker gain for voice call downlink.

  @note  Not support now.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_rx_voice_spkr_gain(SV_SIM_ID_E sim_id, int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the volume level of voice call downlink.

  @param[in] sim_id sim card id.
  @param[in] vol_step  the volume level to be set. range:0-10

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_rx_voice_vol_step(SV_SIM_ID_E sim_id, int32_t volume_step);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the volume level of voice call downlink.

  @param[in] sim_id sim card id.
  @param[out] p_vol_step  the current volume level of voice call downlink.

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_rx_voice_vol_step(SV_SIM_ID_E sim_id, int32_t *p_volume_step);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the mute state of voice call downlink.

  @param[in] sim_id sim card id.
  @param[in] mute_state  the mute state to be set.
             SV_AUDIO_STATE_0: unmute, SV_AUDIO_STATE_1: mute

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_rx_voice_mute_state(SV_SIM_ID_E sim_id, int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the mute state of voice call downlink.

  @param[in] sim_id sim card id.
  @param[out] p_mute_state  the current mute state of voice call downlink. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during the call
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_rx_voice_mute_state(SV_SIM_ID_E sim_id, int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the gain for audio capturing.

  @param[in] the gain to be set. range:0-100

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See sv_type.h for error codes.

  @note  The API should be called during audio capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_audio_tx_mic_gain(int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the gain for audio capturing.

  @param[out] p_mic_gain  the current gain for audio capturing

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during audio capturing.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_audio_tx_mic_gain(int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the gain for audio playback.

  @param[in] codec_gain  the gain to be set. range:0-100

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during audio playback.
  */
/*-----------------------------------------------------------------------------------------------*/

int sv_ms_audio_set_audio_rx_codec_gain(int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the gain for audio playback.

  @param[out] p_codec_gain  the current gain for audio playback.

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during audio playback.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_audio_rx_codec_gain(int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the uplink volume of a codec.

  @param[in] up_volume  the uplink volume to be set. range:0-100

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_codec_up_vol(int32_t volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the uplink volume of a codec.

  @param[out] p_up_volume  the current uplink volume of codec. range:0-100

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_codec_up_vol(int32_t *p_volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the downlink volume of a codec.

  @param[in] down_volume  the volume to be set. range:0-100

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_codec_down_vol(int32_t volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the downlink volume of a codec.

  @param[out] p_down_volume  the current downlink volume of codec. range:0-100

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_codec_down_vol(int32_t *p_volume);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the microphone mute state of a codec.

  @param[in] mute_state  the muute state to be set.
             SV_AUDIO_STATE_0: unmute, SV_AUDIO_STATE_1: mute

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during the call or audio playback
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_codec_mic_mute_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the microphone mute state of a codec.

  @param[out] p_mute_state  the current microphone mute state of codec. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_codec_mic_mute_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the speaker mute state of a codec.

  @param[in] mute_state  the mute state to be set.
             SV_AUDIO_STATE_0: unmute, SV_AUDIO_STATE_1: mute

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.

  @note  The API should be called during the call or audio playback
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_codec_spk_mute_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the speaker mute state of a codec.

  @param[out] p_mute_state  the current speaker mute state of codec. SV_AUDIO_STATE_0 or SV_AUDIO_STATE_1

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_codec_spk_mute_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the value of a mixer.

  @param[in] control    the name of the mixer
  @param[in] val_list   the value of the mixer to be set. String type value.Multiple values are separated by spaces.

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_mixer_control(const char *ctl_name, const char *ctl_values);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the value of the mixer.

  @param[in]  control       the name of the mixer
  @param[out] val_list_buf  buffer for storing mixer values
  @param[in]  buf_size      the buffer size. Unit:Byte

  @retval  SV_ERR_OK Successful execution.
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See sv_type.h for error codes.

  @note  Generally, 64 bytes is enough
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_mixer_control(const char *ctl_name, char *values, uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the ACDB device mode.

  @param[in] dev_mode  the ACDB device mode to be set. defined by SV_AUDIO_DEVICE_MODE_E

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_acdb_device_mode(SV_AUDIO_ACDB_DEV_MODE_E mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the ACDB device mode.

  @param[out] p_dev_mode  the current ACDB device mode. defined by SV_AUDIO_DEVICE_MODE_E

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others Failed execution.See sv_type.h for error codes.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_acdb_device_mode(SV_AUDIO_ACDB_DEV_MODE_E *p_mode);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the sample rate of PCM interface.

  @param[in] sample_rate  the sample rate of PCM interface to be set. support follow:
             SV_AUDIO_PCM_IF_SAMPLE_RATE_8KHZ
             SV_AUDIO_PCM_IF_SAMPLE_RATE_16KHZ

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_sample_rate(int32_t sample_rate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the sampling rate of PCM interface.

  @param[out] p_sample_rate  the current samping rate of PCM interface. Unit:Hz
              SV_AUDIO_PCM_IF_SAMPLE_RATE_8KHZ or SV_AUDIO_PCM_IF_SAMPLE_RATE_16KHZ

  @retval  SV_ERR_OK Successful execution
  @retval  SV_ERR_INVALID_ARG Failed execution.Invalid arguments
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_sample_rate(int32_t *p_sample_rate);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function enables/disables UAC.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_uac_enable_state(int32_t state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  The function gets the UAC state.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_uac_enable_state(int32_t *p_state);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the sidetone gain.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_sidetone_gain(int32_t gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function gets the sidetone gain.

  @note Not support now
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_get_sidetone_gain(int32_t *p_gain);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief  This function sets the callback function for audio service errors. 

  @param[in] cb  The callback function for audio service errors.Only when the audio service exit abnormally,the callback function is executed.

  @retval  SV_ERR_OK Successful execution
  @retval  Others errorcode defined by sv_type.h.
  */
/*-----------------------------------------------------------------------------------------------*/
int sv_ms_audio_set_service_error_cb(sv_audio_service_error_cb_f cb);

#ifdef __cplusplus
}
#endif
#endif // #ifndef __SV_MS_AUDIO_CFG_H__
