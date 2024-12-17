#include "BSP/PD_Protocol_IC/sw_iic_ctl_api.h"

SW_IIC_Ctl_Func_t SW_IIC_Ctl_Func;		// 需制作实际执行该功能的函数，并将函数地址辅助给该变量

void SW_IIC_Ctl_Init(SW_IIC_Ctl_Func_t *sw_iic_ctl_func)
{

}

void SW_IIC_Write_Enable(void)
{
	SW_IIC_Ctl_Func.IIC_Write_Enable();
}

void SW_IIC_PD_Current_Limiting(Output_Voltage_e v_grade, uint16_t current_limiting)
{
	SW_IIC_Ctl_Func.PD_Current_Limiting(v_grade, current_limiting);
}

void SW_IIC_Output_Power(uint16_t  *output_ma, uint16_t *output_mv)
{
	SW_IIC_Ctl_Func.Get_Output_Power(output_ma, output_mv);
}