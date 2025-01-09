#include "base_timer_driver.h"
#include "ntc_temp_sample.h"
#include "busi_power_policy_engine.h"
#include <stdio.h>
#include "zr_btm.h"
#include "zr_rcc.h"
#include "zr_nvic.h"

uint32_t Base_Timer_1ms_Count=0;

// void Base_Timer_Driver_Check(void)
// {
// 	#include "zr_systick.h"
// 	unsigned int value=122;
// 	LED_Driver_Init();
// 	while(1)
// 	{
// 		printf("reload_value = %d\r\n", value);
// 		Base_Timer_Driver_Init(value);
// 		value += 122;
// 		for(uint8_t num=0; num < 100; num++)
// 		{
// 			Systick_Delay_Ms(1000);
// 		}
// 	}
// }

void Start_Noload_Timer(uint16_t reload_value)
{
	// start a btm every NOLOAD_CHECKING_INTERVAL ms
	Btm4_Set_Reload_Value(reload_value*1000/8 - 1);
	Nvic_Irq_Enable(BTM4_IRQn,PRIORITY_2);
	Btm4_Int_Enable();
	Btm4_Enable();   
}

void Stop_Noload_Timer(void)
{
	Nvic_Irq_Disable(BTM4_IRQn);
	Btm4_Int_Disable();
	Btm4_Disable();   
}


void Base_Timer_Driver_Init(unsigned int timer_ms)
{
	timer_ms *= 121;	// 计算公式：定时时间T = reload_value * 8.2us，即 timer_ms = xms * 1000 / 8.2 = xms * 121
	Rcc_Module_Rst(BTM_ID);
	Rcc_Module_Clk_Enable(BTM_ID);
	Rcc_Module_Rst_Release(BTM_ID);
	
	Btm4_Set_Reload_Value(timer_ms);
	Btm4_Int_Enable();
	Btm4_Clear_Int_Pending();
	Btm4_Enable();
	Nvic_Irq_Enable(BTM4_IRQn, PRIORITY_0);
	
}

void Base_Timer_Driver_DeInit(void)
{
	Nvic_Irq_Disable(BTM4_IRQn);
	Btm4_Clear_Int_Pending();
	Btm4_Int_Disable();
	Btm4_Disable();  
}

void BTM4_Handler(void)
{
	if(Btm4_Get_Int_Pending() == SET)
	{
		Base_Timer_1ms_Count++;

//		if(Busi_Power_Policy_Data.Software_Timer_EN)
//		{
//			Busi_Power_Policy_Data.Output_Voltage_Sample_Timer++;
//		}

		NTC_Temp_Data.NTC_Voltage_Sample_Time_Count++;
		if(NTC_Temp_Data.NTC_Voltage_Sample_Time_Count > NTC_VOLTAGE_VALUE_SAMPLE_RATE)
		{
			NTC_Temp_Sample();
		}

		Btm4_Clear_Int_Pending();
	}
}