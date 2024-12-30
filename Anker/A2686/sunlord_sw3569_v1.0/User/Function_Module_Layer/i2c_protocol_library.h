/**
 * @file i2c_Protocol_library.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _I2C_PROTOCOL_LIBRARY_H_
#define _I2C_PROTOCOL_LIBRARY_H_
/*
 * I2C数据传输协议规定
 * i2c数据传输业务
 * 待传输业务：
 * 		busi_port_detection.h 文件下：
 * 			Port_Detection_Data_t	Busi_Port_Detection_Data; 内有关Port2的各项数据
 * 			 	具体如下：
 * 					Port_Out_Protocol_Data_t	Port2_Out_Protocol_Data;	--> 寄存器1
 * 						Port_Link_State_e		Port_Link_State;		// 端口连接状态
						fast_charge_protocol_e	Protocol_Type;			// 端口协议类型
						uint16_t 				Protocol_Voltage;		// 端口输出电压
						uint16_t 				Protocol_Current;		// 端口输出电流
						POWER_PDO_Mode_e		Current_Power_Grade; 	// 当前端口输出协议等级
 * 					Port_RealTime_Out_Power_t	Port2_RealTime_Out_Power;	--> 寄存器2
						uint16_t				Voltage;				// 实时电压 unit: mV
						uint16_t				Current;				// 实时电流 unit: mA
						uint16_t				Power;					// 实时功率 unit: mW
 * 		ntc_temp_sample.h 文件下：
 * 			NTC_Temp_Data_t	NTC_Temp_Data; 内有关Port2的各项数据			--> 寄存器3
 * 				具体如下：
 * 					NTC_Temp_State_e	NTC_Over_Temp_Flag;				// NTC过温标志
 * 					OverTemp_Dispose_e	OverTemp_Dispose_Flag;			// NTC过温处理标志
 * 		busi_fast_charge_config.h 文件下：
 * 			POWER_PDO_Mode_e  控制从机输出对应功率等级
 * 
 * 通讯协议如下：
 * 		从机地址：0x3c
 * 		仅读：
 * 		寄存器 PORT2_OUT_PROTOCOL: 		Port_Out_Protocol_Data_t	Port2_Out_Protocol_Data 地址：0x02
 * 		寄存器 PORT2_REALTIME_POWER:	Port_RealTime_Out_Power_t	Port2_RealTime_Out_Power  地址：0x03
 * 		读写：
 * 		寄存器 PORT2_NTC_TEMP_DATA:		NTC_Temp_Data_t	NTC_Temp_Data 地址：0x04
 * 		仅写：
 * 		寄存器 PORT2_CTL_OUT_POWER:		POWER_PDO_Mode_e 地址：0x05
 * 		寄存器 Slave_Rouse				0xFF 地址：0x01
 * 		寄存器 Slave_Sleep				0x00 地址：0xFF
 * 
 */
#include "system.h"

typedef enum
{
	LEISURE_STATE,
	BUSY_STATE
}I2C_Running_State_t;

typedef enum
{
	SLAVE_ROUSE = 0x01,
	PORT2_OUT_PROTOCOL,
	PORT2_REALTIME_POWER,
	PORT2_NTC_TEMP_DATA,
	PORT2_CTL_OUT_POWER,		// 控制端口2输出功率
	SLAVE_SLEEP = 0xFF
}I2C_Register_List_e;

#endif
