#include "ntc_temp_sample.h"
#include "zr_gpio.h"
#include "zr_adc.h"

NTC_Temp_Data_t	NTC_Temp_Data;

void NTC_Driver_Init(void)
{
	static adc_config_t	 NTC_ADC_Config;
	static gpio_config_t ntc_gpio_config;

	ntc_gpio_config.direction = GPIO_DIRECTION_INPUT;
	ntc_gpio_config.gpioFunc = FUNCTION_SELF;
	Gpio_Init(GPIO7, &ntc_gpio_config);

	NTC_ADC_Config.adc2Pullup = ADC_PULLUP_CURRENT_10UA;
	NTC_ADC_Config.adc2SampleRate = ADC_SAMPLE_RATE_250H;
	NTC_ADC_Config.adc2Source = ADC2_SIG_SRC_GPIO7;
	Adc_Set_Config(&NTC_ADC_Config);
	NTC_TEMP_SAMPLE_START();
}

void NTC_Temp_Sample(void)
{
	NTC_Temp_Data.NTC_Voltage_Value = Adc_Get_Channel_Data(ADC_CHL_2);
	if(NTC_Temp_Data.NTC_Voltage_Value > NTC_OVER_TEMP_THRESHOLD)
	{
		NTC_Temp_Data.NTC_Over_Temp_Flag = OVERTEMP;
	}
	else if(NTC_Temp_Data.NTC_Voltage_Value < NTC_RECOVER_TEMP_THRESHOLD)
	{
		NTC_Temp_Data.NTC_Over_Temp_Flag = NORMAL_WORKING;
	}
}