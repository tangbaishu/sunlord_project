#include "base_time_driver.h"
#include "LED_Driver/led_driver.h"
#include <stdio.h>
#include "zr_btm.h"
#include "zr_rcc.h"
#include "zr_nvic.h"

uint32_t sys_start = 0, sys_time = 0,time_finish = 0, timer_stop = 0;


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

void Base_Time_Driver_DeInit(void)
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
		sys_time++;
		if(sys_time >= 120000)
		{
			sys_time = 0;
			time_finish = 1;
		}
		Btm4_Clear_Int_Pending();
		LED_Driver_Turn(2);
	}
}