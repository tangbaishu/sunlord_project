#include "power_function_api.h"
#include "api.h"
#include "zr_adc.h"
#include "zr_abn.h"
#include "stdio.h"

#define LOG		"power_function_api.c :"

Power_Output_Info_t Power_Output_Info[POWER_OUTPUT_PORT_NUM];
Abnormal_Dispose_Data_t	Abnormal_Dispose_Data;

Abnormal_Display_Hook_t	Abnormal_Display_Hook_Func;


/**
 * @brief 采集功率输出
 * 
 * @param port_num 		待采集的输出功率端口 Power_Output_Port_e
 * @param updata_i_v 	TRUE: 同步更新电流、电压变量值、 FALSE: 不更新
 */
void Power_Output_Gather(Power_Output_Port_e port_num, bool updata_i_v)
{
	if(updata_i_v == TRUE)
	{
		Power_Output_Info[port_num].Out_Current = Adc_Iout_Get(port_num);
		Power_Output_Info[port_num].Out_Voltage = Adc_Vout_Get();
		Power_Output_Info[port_num].Out_Power = Power_Output_Info[port_num].Out_Current * Power_Output_Info[port_num].Out_Voltage / 1000000;
	}
	else
	{
		Power_Output_Info[port_num].Out_Power = Adc_Iout_Get(port_num) * Adc_Vout_Get() / 1000000;
	}
}
#include "log_function.h"
void Abnormal_Occurred_Display_Func(u32 pending_flag)
{
	LOG_Abnormal_Occurred_Event(pending_flag);
	#if PRINTF_ENABLE == ENABLE
		printf(LOG"Abnormal_Occurred_display = %#x\r\n", pending_flag);
	#endif
	
}

void Abnormal_Release_Display_Func(u32 pending_flag)
{
	LOG_Abnormal_Release_Event(pending_flag);
	#if PRINTF_ENABLE == ENABLE
		printf(LOG"Abnormal_Release_display = %#x\r\n", pending_flag);
	#endif
}

/**
 * @brief 已发生的异常处理函数
 * 
 * @param pending 异常中断挂起标志
 */
void Abnormal_Occurred_Dispose(u32 pending)
{
	#if PRINTF_ENABLE == ENABLE
		printf(LOG"Abnormal_Occurred = %#x but Display Func Null\r\n", pending);
	#endif
	Abnormal_Dispose_Data.Abnormal_Occurred_Flag = TRUE;
	Abnormal_Dispose_Data.Abnormal_Release_Flag = FALSE;
	Abnormal_Dispose_Data.Abnormal_Value = pending;
	if(Abnormal_Display_Hook_Func.Abnormal_Occurred != NULL)
	{
		Abnormal_Display_Hook_Func.Abnormal_Occurred(pending);
		return;
	}
}

/**
 * @brief 异常释放处理函数
 * 实测异常恢复时，未调用该函数
 * @param pending 异常中断挂起标志
 */
void Abnormal_Release_Dispose(u32 pending)
{
	#if PRINTF_ENABLE == ENABLE
		printf(LOG"Abnormal_Release = %#x but Display Func Null\r\n", pending);
	#endif
	Abnormal_Dispose_Data.Abnormal_Occurred_Flag = FALSE;
	Abnormal_Dispose_Data.Abnormal_Release_Flag = TRUE;
	Abnormal_Dispose_Data.Abnormal_Value = pending;
	if(Abnormal_Display_Hook_Func.Abnormal_Release != NULL)
	{
		Abnormal_Display_Hook_Func.Abnormal_Release(pending);
		return;
	}
}

void Power_Abnormal_Init_Hook(Abnormal_Display_Hook_t *Abnormal_Display)
{
	// 指定 触发异常 和 异常恢复 时的显示函数，若无需异常显示仅处理，可注释下面两条语句
	Abnormal_Display->Abnormal_Occurred = Abnormal_Occurred_Display_Func;
	Abnormal_Display->Abnormal_Release = Abnormal_Release_Display_Func;
	Abnormal_Init_Hook_Func(Abnormal_Occurred_Dispose, Abnormal_Release_Dispose);
}
