#ifndef _PORT_DATA_LIBRARY_H_
#define _PORT_DATA_LIBRARY_H_

#include "api.h"

/**
 * 端口可广播功率列表
 */
typedef enum
{
	POWER_PDO_70W = 0,	// 70W
	POWER_PDO_65W,		// 65W
	POWER_PDO_45W,		// 45W
	POWER_PDO_35W,		// 35W
	POWER_PDO_25W,		// 25W
	POWER_PDO_5W,		// 5W
}POWER_PDO_Mode_e;

/**
 * 端口连接状态
 * 
 */
typedef enum
{
	Port_ALL_Pull_Out,					// 所有端口全部拔出(为变量初始化保留，上电后在系统初始化阶段，相应变量会更新)
	Port_1_Pull_Out,					// 端口1拔出
	Port_2_Pull_Out,					// 端口2拔出
	Port_1_Insert,						// 端口1插入	
	Port_2_Insert						// 端口2插入	
}Port_Link_State_e;

typedef struct
{
	Port_Link_State_e		Port_Link_State;		// 端口连接状态
	fast_charge_protocol_e	Protocol_Type;			// 端口协议类型
	uint16_t 				Protocol_Voltage;		// 端口输出电压
	uint16_t 				Protocol_Current;		// 端口输出电流
	POWER_PDO_Mode_e		Current_Power_Grade; // 当前端口输出协议等级
	// uint16_t 				Portocol_Power;		// 功率
}Port_Out_Protocol_Data_t;

typedef struct
{
	uint16_t				Voltage;		//  unit: mV
	uint16_t				Current;		//  unit: mA
	uint16_t				Power;			//  unit: mW
}Port_RealTime_Out_Power_t;

#endif
