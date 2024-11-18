#ifndef _SW3516P_DRIVER_H_
#define _SW3516P_DRIVER_H_

#include "IIC_Driver/iic_api.h"

typedef unsigned char uint8;
typedef unsigned int  uint16;				// 期待使用16位变量，但采用32bit时，该类型为32位

#define READ_DEVICE_VERSION


//状态指示寄存器（只读）
#define FAST_CHARGE_INDICATION		0x06	// 快充指示
#define SYSTEM_STATE_0				0x07	// 系统状态 0
#define SYSTEM_STATE_1				0x08	// 系统状态 1
#define PORT_1_CURRENT_LIMITING		0x0C	// 端口1限流值指示
#define PORT_2_CURRENT_LIMITING		0x0D	// 端口2限流值指示

//可读写状态寄存器
#define IIC_ENABLE_CONTROL			0x12	// IIC使能控制
#define ADC_VIN_ENABLE				0x13	// ADC Vin	 使能
#define PWR_COMPEL_OPERATION_ENABLE	0x15	// PWR强制操作使能
#define PWR_COMPEL_OPERATION_1		0x16	// PWR强制操作0

#define ADC_VIN_DATA				0x30	// ADC Vin	 数据
#define ADC_VOUT_DATA				0x31	// ADC Vout	 数据
#define ADC_IOUT_1_DATA				0x33	// ADC Iout1 数据
#define ADC_IOUT_2_DATA				0x34	// ADC Iout2 数据
#define ADC_NTC_VOLTAGE_DATA		0x37	// ADC NTC 	 电压数据
#define ADC_CONFIG					0x3A	// ADC 配置
#define ADC_DATA_HIGH_8BIT			0x3B	// ADC 数据高8位
#define ADC_DTAT_LOW_4BIT			0x3C	// ADC 数据低4位

#define PD_CMD_REQUEST				0x70	// PD命令请求
#define PD_MESAGE_SET				0x71	// PD消息设置
#define PD_SRC_CAP_CMD_SEND			0x73	// PD SRC CAP 命令发送
#define HARDRESET_COUNT_SET			0x75	// 硬复位 次数设置
#define CONNECT_SET					0x76	// 连接设置
#define VOLTAGE_OFFSET_SET			0xA6	// 电压偏移设置
#define FAST_CHARGE_CONFIG_0		0xAA	// 快充配置0	
#define PORT_MODE_CONFIG			0xAB	// 端口模式配置
#define FAST_CHARGE_CONFIG_1		0xAD	// 快充配置1
#define VID_CONFIG_0				0xAF	// VID配置0
#define PD_CONFIG_0					0xB0	// PD配置0
#define PD_CONFIG_1					0xB1	// PD配置1
#define PD_CONFIG_2					0xB2	// PD配置2
#define PD_CONFIG_3					0xB3	// PD配置3
#define PD_CONFIG_4					0xB4	// PD配置4
#define PD_CONFIG_5					0xB5	// PD配置5
#define PD_CONFIG_6					0xB6	// PD配置6
#define PD_CONFIG_7					0xB7	// PD配置7
#define PD_CONFIG_8					0xB8	// PD配置8
#define FAST_CHARGE_CONFIG_2		0xB9	// 快充配置2
#define FAST_CHARGE_CONFIG_3		0xBA	// 快充配置3
#define FAST_CHARGE_CONFIG_4		0xBC	// 快充配置4
#define CURRENT_LIMITING_0			0xBD	// 限流配置0
#define PD_CONFIG_9					0xBE	// PD配置9
#define VID_CONFIG_1				0xBF	// VID配置1
#define CURRENT_LIMITING_1			0xC4	// 限流配置1
#define FAST_CHARGE_CONFIG_5		0xC5	// 快充配置5
#define FAST_CHARGE_CONFIG_6		0xCF	// 快充配置6


typedef enum
{
	INPUT_OVER_VOLTAGE = 1,					// 输入过压保护
	INPUT_UNDER_VOLTAGE,					// 输入欠压保护
	OUTPUT_OVER_CURRENT,					// 输出过流保护
	OUTPUT_SHORT_CIRCUIT,					// 输出短路保护
	OVER_TEMPERATURE						// 过温保护
}Protect_Event_e;							// 保护事件

typedef struct
{
	uint8 	Fast_Charge_State;				// 快充指示状态
	uint8 	SYS_State0;						// 系统状态0
	uint8 	SYS_State1;						// 系统状态1
	uint16	Port1_Limiting_Value;			// 端口1限流值
	uint16	Port2_Limiting_Value;			// 端口1限流值
}IC_Work_State_t;							// 芯片工作状态

typedef struct
{
	uint16	Vin_Voltage;					// 输入电压
	uint16	Vout_Voltage;					// 输出电压
	uint16	Iout1_Current;					// 输出电流1
	uint16	Iout2_Current;					// 输出电流2
	uint16	Temp_Onboard;					// 板载温度
}IC_Work_Data_t;							// 芯片工作数据


typedef struct 
{
	Protect_Event_e		Protect_Event;		// 保护事件
	IC_Work_State_t		IC_Work_State;		// 芯片工作状态
	IC_Work_Data_t		IC_Work_Data;		// 芯片工作数据
}SW3516P_Driver_Info_t;
extern SW3516P_Driver_Info_t	SW3516P_Driver_Info;


#ifdef READ_DEVICE_VERSION
	void Read_Device_Version(void);
#endif // !READ_DEVICE_VERSION

void SW3516P_Get_State(void);
void SW3516P_Init(void);
/**
 * @brief SW3516P电源端口设置
 * 
 * @param param 
 * 0：单A口
 * 1：双A口
 * 2: 单C口
 * 3: C+A口
 */
void SW3516P_Power_Port_Config(uint8 param);
#endif
