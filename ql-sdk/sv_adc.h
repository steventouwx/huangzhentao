/*******************************************
 * Copyright (c) 2026，德赛西威汽车电子有限公司
 * All rights reserved
 * 文件名: sv_adc.h
 * 编写者:
 * 编写日期: 2026-03-05
 * 简要描述: 定义sv_adc相关接口、数据结构及功能能力。
 * 修改记录:
 *******************************************/

  
#ifndef _SV_ADC_H_
#define _SV_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
    enum ADC channel
*/
typedef enum  ADC_CHANNEL_ENUM{
	QADC_NONE = 0,				/**< Invalid  channel*/
	ADC0 = 1,				/**< amux_5 for ADC0 channel  */
	ADC1 = 2,				/**< amux_6 for ADC1 channel  */
	ADC2 = 3,				/**< pmu_gpio01 for ADC2 channel  */
	ADC3 = 4,				/**< pmu_gpio05 for ADC3 channel  */
	ADC4 = 5,				/**< pmu_gpio15 for ADC4 channel  */
	QADC_END				/**< Invalid  channel*/
}ADC_CHANNEL_E;

/*========================================================================*/
/**
  @brief This function retrieves the ADC voltage value with the specified
  @param[in]  adc channel definitions, one value of Enum_QADC
  @param  ADC0  0.3V-VBAT_BB(3.8V)
  @param  ADC1  0.3V-VBAT_BB
  @return adc value(mv) on success,on failure, the return value is -1;
  */
/*=======================================================================*/

int sv_adc_show(ADC_CHANNEL_E qadc);
char* sv_get_line_from_file(const char* file_name, int line, char* buf, int len);

#ifdef __cplusplus
}
#endif

#endif
