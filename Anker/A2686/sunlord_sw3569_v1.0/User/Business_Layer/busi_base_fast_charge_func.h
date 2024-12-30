/**
 * @file busi_base_fast_charge_func.h
 * @author your name (you@domain.com)
 * @brief 
 * void (*P_GPIO_Driver_Init_Config_API)	(void);	// 基于客户需求，对GPIO进行初始化配置 API接口
 * void Busi_Vin_Voltage_Control	(void);			// 母线电压控制
 * void Busi_LowPower_OnBefore		(void);			// 进入低功耗之前的处理函数
 * void Busi_LowPower_OnAfter 		(void);			// 从低功耗唤醒后的处理函数
 * @version 0.1
 * @date 2024-12-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _BUSI_BASE_FAST_CHARGE_FUNC_H_
#define _BUSI_BASE_FAST_CHARGE_FUNC_H_


void Busi_Fast_Charge_Driver_Init	(void);		
void Busi_Vin_Voltage_Control		(void);			// 母线电压控制
void Busi_LowPower_OnBefore			(void);			// 进入低功耗之前的处理函数
void Busi_LowPower_OnAfter 			(void);			// 从低功耗唤醒后的处理函数

#endif

