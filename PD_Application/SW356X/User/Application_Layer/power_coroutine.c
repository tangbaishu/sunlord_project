#include "power_coroutine.h"
#include "power_business.h"
#include "power_function_api.h"
#include "base_time_driver.h"
#include "zr_abn.h"
#include "zr_systick.h"
#include <stdio.h>

#define LOG	"power_coroution.c: "

/**
 * @brief 电源供应改变例程
 * 1、基于端口插入逻辑&事件，设定/切换输出功率值
 * 2、周期检测输出功率值
 * 3、异常触发&释放事件打印
 */
void Power_Supply_Charge_Routine(void)
{
	static uint8_t 	protocol_clock = 0,printf_en=0;
    static uint32_t test = 0, refresh_sensor=0, abnoraml_release_check_t=0;
	static bool		initialize_flag=0;
	static abn_config_t abn_config;
	if(initialize_flag == 0)
	{
		initialize_flag = 1;
		Abn_Get_Config(&abn_config);
		printf(LOG"abn_config = %#x\r\n", abn_config.value);
		abn_config.value |= 0x01<<7;
		Abn_Set_Config(&abn_config);
		Power_Abnormal_Init_Hook(&Abnormal_Display_Hook_Func);
	}
	/*
	if(Port_Module_Data.Port2_Insert_State.Current_State == PORT_ATTACH)
	{
		if(sys_start == 0)
		{
			sys_start = 1;
			Start_Noload_Timer(1);
			timer_stop = 0;
			printf("start timer\r\n");
			// Systick_Delay_Ms(100);
			// printf("resume time = %d\r\n", sys_time);
		}
		if(time_finish)
		{
			time_finish = 0;
			if(protocol_clock == 0)
			{
				protocol_clock = 1;
				Port_Module_Data.Power_Supply_Select(POWER_SUPPLY_MODE_B, TRUE);
				printf("POWER_SUPPLY_MODE_B\r\n");
			}
			else
			{
				protocol_clock = 0;
				Port_Module_Data.Power_Supply_Select(POWER_SUPPLY_MODE_A, TRUE);
				printf("POWER_SUPPLY_MODE_A\r\n");
			}
		}
		// if((sys_time - test) >= 1000)
		// {
		// 	printf("resume time = %d\r\n", sys_time);
		// 	test = sys_time;
		// } 
		// if(sys_time - refresh_sensor >= 1000)
		// {
		// 	refresh_sensor = sys_time;
		// 	Power_Output_Gather(PORT_1,TRUE);
		// 	Power_Output_Gather(PORT_2,TRUE);
		// 	printf(LOG"Port_1 Output_P=%d W, O_I=%d mA, O_V=%d mV\r\n",  Power_Output_Info[PORT_1].Out_Power, \
		// 									Power_Output_Info[PORT_1].Out_Current, Power_Output_Info[PORT_1].Out_Voltage);
		// 	printf(LOG"Port_2 Output_P=%d W, O_I=%d mA, O_V=%d mV\r\n",  Power_Output_Info[PORT_2].Out_Power, \
		// 									Power_Output_Info[PORT_2].Out_Current, Power_Output_Info[PORT_2].Out_Voltage);
		// }
		// if((sys_time -abnoraml_release_check_t) >= 1000)
		// {
			
		// }
	}
	else
	{
		if(timer_stop == 0)
		{
			timer_stop = 1;
			sys_start = 0;
			printf("Stop_Noload_Timer()\r\n");
			Stop_Noload_Timer();
		}
	}
	*/
}