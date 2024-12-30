/**
 * @file app_fast_charge.h
 * @author tong.libo@sunlord.com.cn
 * @brief 
 * 调用各个业务逻辑，实现客户提供的快充应用部分的功能需求
 * @version 0.1
 * @date 2024-12-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _APP_FAST_CHARGE_H_
#define _APP_FAST_CHARGE_H_

#include "config.h"
#include "busi_base_fast_charge_func.h"
#include "system.h"

typedef struct
{
			
}App_Fast_Charge_Data_t;
extern App_Fast_Charge_Data_t	App_Fast_Charge_Data;

extern config_data_t* Fast_Charge_Config;

typedef struct
{
	
	void (*P_Fast_Charge_Driver_Init) (void);		// 基于客户需求，对GPIO进行初始化配置
	void (*P_Vin_Voltage_Control)	(void);				// 母线电压控制
	void (*P_Real_Time_Allocation_Power) (config_data_t *config);	// 实时功率分配
	void (*P_P_Dual_Port_Insert_Dispose) (void);	// 双端口插入处理	单双口功率切换处理
	void (*P_Temperature_Adjust_Power)	(void);		// 温度调控			基于温度点，调整功率输出挡位
	void (*P_Set_Buck_Frequence) 	(uint8_t);		// 设置降压频率		按照客户需求，初始化配置Buck频率
	void (*P_LowPowerHook_OnBefore)	(void);			// 进入低功耗之前的处理函数
	void (*p_LowPowerHook_OnAfter) (void);			// 从低功耗唤醒后的处理函数
}App_Fast_Charge_Func_t;
extern App_Fast_Charge_Func_t App_Fast_Charge_Func;

void APP_Fast_Charge_Init(void);

void APP_Fast_Charge_Running(void);

#endif
