#ifndef _PORT_DATA_LIBRARY_H_
#define _PORT_DATA_LIBRARY_H_

#include "api.h"

#define SYS_PORT_MAX_NUMBER		2	// 端口数量 （从1开始计数）
#define	CHIP_TYPE_C_PORT_NUM	1	// 芯片Type-c 映射端口编号

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

typedef struct
{
	fast_charge_protocol_e	Protocol_Type;			// 端口协议类型	
	uint16_t 				Protocol_Voltage;		// 端口输出电压
	uint16_t 				Protocol_Current;		// 端口输出电流
	POWER_PDO_Mode_e		Current_Power_Grade; 	// 当前端口输出协议等级
}Port_Out_Protocol_Info_t;										// 端口输出信息

typedef struct
{
	uint16_t				Voltage;		//  unit: mV
	uint16_t				Current;		//  unit: mA
	uint16_t				Power;			//  unit: mW
}RealTime_Out_Power_t;

/**
 * 端口连接状态
 * 
 */
typedef struct
{
	uint8_t						Port_Link_State;					// 端口连接状态	端口插入：bitx==1、端口拔出：bitx=0; bit0:对应自身芯片映射端口1, bit1:对应从机1的映射端口
	Port_Out_Protocol_Info_t	Port_Info;				// 端口输出协议信息
	RealTime_Out_Power_t		RealTime_Out_Powe;		// 端口实时输出功率
	uint8_t						Port_NTC_Res;			// 端口NTC阻值
}Port_Out_Info_t;
extern Port_Out_Info_t Port_Out_Info[SYS_PORT_MAX_NUMBER];	// 各个端口输出信息

#endif
