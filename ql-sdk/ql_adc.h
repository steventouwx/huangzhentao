/**
 * @file ql_adc.h
 * @brief ADC API 
 *
 * @details
 * QuecOpen AG5xx series ADC API
 *
 * @htmlonly 
 * <span style="font-weight: bold">History</span> 
 * @endhtmlonly
 *
 * when|who|why
 * ----------|--------------|-------------------------------------------
 * 20180627  |  QUECTEL     | Create
 * 20181224  |  juson       | add adc 2
 * 20191211  |  larry.zhang | Modify the code format
 * 20210918  |  ron.zhang   | Modify the code comment for doxygen
 *
 * @copyright Copyright (c) 2019 Quectel Wireless Solution, Co., Ltd. All Rights Reserved.
 * Quectel Wireless Solution Proprietary and Confidential.
 */  
#ifndef _QL_ADC_H_
#define _QL_ADC_H_


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

int ql_adc_show(ADC_CHANNEL_E qadc);
char* ql_get_line_from_file(const char* file_name, int line, char* buf, int len);

#ifdef __cplusplus
}
#endif


#endif
