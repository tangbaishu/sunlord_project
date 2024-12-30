/**
 * @file ntc_temp_sample.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _NTC_TEMP_SAMPLE_
#define _NTC_TEMP_SAMPLE_

#include "system.h"

#define NTC_OVER_TEMP_THRESHOLD			250
#define NTC_RECOVER_TEMP_THRESHOLD		500

#define NTC_VOLTAGE_VALUE_SAMPLE_RATE	25	// unit: ms
#define NTC_TEMP_SAMPLE_START()			Adc_Start_Adc_Channel(ADC_CHL_2)
#define NTC_TEMP_SAMPLE_STOP()			Adc_Stop_Adc_Channel(ADC_CHL_2)

typedef enum
{
	NORMAL_WORKING,
	OVERTEMP,			// 过温
}NTC_Temp_State_e;

typedef enum
{
	UNDISPOSED,
	PROCESSED,
}OverTemp_Dispose_e;

typedef struct
{
	uint16_t			NTC_Voltage_Value;
	uint8_t				NTC_Voltage_Sample_Time_Count;
	NTC_Temp_State_e	NTC_Over_Temp_Flag;
	OverTemp_Dispose_e	OverTemp_Dispose_Flag;
}NTC_Temp_Data_t;
extern NTC_Temp_Data_t	NTC_Temp_Data;

void NTC_Driver_Init(void);
void NTC_Temp_Sample(void);

#endif

