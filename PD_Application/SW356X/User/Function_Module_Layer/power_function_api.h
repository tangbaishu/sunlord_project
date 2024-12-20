/**
 * @file power_function.h
 * @author tong.libo@sunlord.com.cn
 * @brief SW356X 电源相关的API函数，基于SW356X提供的 api.h 文件二次开发
 * 功能清单：
 * 实现输出功率采集（输出功率W、输出电流mA、输出电压mV）
 * 触发异常保护配置&回调
 * 
 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _POWER_FUNCTION_API_H_
#define _POWER_FUNCTION_API_H_

#include "system.h"

#define PRINTF_ENABLE			ENABLE
#define POWER_OUTPUT_PORT_NUM	2

typedef enum
{
	PORT_1 = 0,
	#if POWER_OUTPUT_PORT_NUM == 2
		PORT_2
	#endif
}Power_Output_Port_e;

typedef struct
{
	uint16_t	Out_Current;	// unit :mA
	uint16_t	Out_Voltage;	// unit :mV
	uint16_t	Out_Power;		// unit :W (A*V)
}Power_Output_Info_t;
extern Power_Output_Info_t Power_Output_Info[POWER_OUTPUT_PORT_NUM];
void Power_Output_Gather(Power_Output_Port_e port_num, bool updata_i_v);

void PD_Request_Info_func(u8 objIndex, u16 volInMv, u16 currInMa);
bool PD_Policy_Hook(void);
/**
 * @brief 异常发生显示
 * 
 * @param pending 异常发生中断挂起标志
 * @note  for abnormalOccurredHook, 
 *        bit0  : LPS
 *        bit1  : DM OVP 
 *        bit2  : DP OVP
 *        bit3  : CC2 OVP
 *        bit4  : CC1 OVP
 *        bit5  : VCONN2 OCP
 *        bit6  : VCONN1 OCP
 *        bit7  : DIE OTP
 *        bit8  : VOUT SCP
 *        bit9  : VOUT Slow OVP
 *        bit10 : VOUT Fast OVP 
 *        bit11 : VIN UVLO
 *        bit12 : VIN OVP
 *        bit13 : VOUT UVP
 *        bit14 : DIE Warn
 *        bit15 : DM2 Weak Short
 *        bit16 : DP2 Weak Short
 *        bit17 : DM1 Weak Short,
 *        bit18 : DP1 Weak Short
 *        bit19~bit31  reserved
 * 
 *        for abnormalReleaseHook
 *        bit0  : DM OVP Release
 *        bit1  : DP OVP Release
 *        bit2  : CC2 OVP Release
 *        bit3  : CC1 OVP Release
 *        bit4  : DIE OTP Release
 *        bit5  : VOUT SLOW OVP Release
 *        bit6  : VOUT FAST OVP  Release
 *        bit7  ：VIN UVLO Release
 *        bit8  ：VIN OVP Release
 *        bit9  ：DIE WARN Release
 *        bit10 ：DM2 Weak Short Release
 *        bit11 ：DP2 Weak Short Release
 *        bit12 ：DM1 Weak Short Release
 *        bit13 ：DP1 Weak Short Release
 *        bit14~bit31  reserved
 */
typedef void (*Abnormal_Display_Hook) (u32 pending);

typedef struct
{
	Abnormal_Display_Hook Abnormal_Occurred;
	Abnormal_Display_Hook Abnormal_Release;
}Abnormal_Display_Hook_t;
extern Abnormal_Display_Hook_t	Abnormal_Display_Hook_Func;

typedef struct
{
	bool Abnormal_Occurred_Flag;	// 1: Abnormal Occurred 	0: normal
	bool Abnormal_Release_Flag;		// 1: Abnormal Release 		0: normal / Abnormal Occurred
	uint32_t Abnormal_Value;
}Abnormal_Dispose_Data_t;
extern Abnormal_Dispose_Data_t	Abnormal_Dispose_Data;


void Power_Abnormal_Init_Hook(Abnormal_Display_Hook_t *Abnormal_Display);


#endif
