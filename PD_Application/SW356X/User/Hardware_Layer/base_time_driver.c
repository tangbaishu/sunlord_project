#include "base_time_driver.h"
#include "LED_Driver/led_driver.h"
#include <stdio.h>
#include "zr_btm.h"
#include "zr_rcc.h"
#include "zr_nvic.h"

void Base_Time_Driver_Check(void)
{
	#include "zr_systick.h"
	unsigned int value=122;
	LED_Driver_Init();
	while(1)
	{
		printf("reload_value = %d\r\n", value);
		Base_Time_Driver_Init(value);
		value += 122;
		for(uint8_t num=0; num < 100; num++)
		{
			Systick_Delay_Ms(1000);
		}
	}
}


/**
 * @brief 计算公式：定时时间T = reload_value * 8.2us
 * 
 * @param reload_value 
 */
void Base_Time_Driver_Init(unsigned int reload_value)
{
	Rcc_Module_Rst(BTM_ID);
	Rcc_Module_Clk_Enable(BTM_ID);
	Rcc_Module_Rst_Release(BTM_ID);
	
	Btm4_Set_Reload_Value(reload_value);
	Btm4_Int_Enable();
	Btm4_Clear_Int_Pending();
	Btm4_Enable();
	Nvic_Irq_Enable(BTM4_IRQn, PRIORITY_0);
	
}

void BTM4_Handler(void)
{
	if(Btm4_Get_Int_Pending() == SET)
	{
		Btm4_Clear_Int_Pending();
		LED_Driver_Turn(2);
	}
}