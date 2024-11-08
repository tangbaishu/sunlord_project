#ifndef _SW3516P_DRIVER_H_
#define _SW3516P_DRIVER_H_

typedef unsigned char uint8;
typedef unsigned int  uint16;				// 期待使用16位变量，但采用32bit时，该类型为32位

//状态指示寄存器（只读）
#define FAST_CHARGE_INDICATION_REG	0X06	// 快充指示
#define SYSTEM_STATE_0				0X07	// 系统状态 0
#define SYSTEM_STATE_1				0X08	// 系统状态 1
#define PORT_1_CURRENT_LIMITING		0X0C	// 端口1限流值指示
#define PORT_2_CURRENT_LIMITING		0X0D	// 端口2限流值指示

//可读写状态寄存器
#define IIC_ENABLE_CONTROL			0X12	// IIC使能控制
#define ADC_VIN_ENABLE				0X13	// ADC Vin	 使能
#define PWR_COMPEL_OPERATION_ENABLE	0X15	// PWR强制操作使能
#define PWR_COMPEL_OPERATION_1		0X16	// PWR强制操作0
#define ADC_VIN_DATA				0X30	// ADC Vin	 数据
#define ADC_VOUT_DATA				0X31	// ADC Vout	 数据
#define ADC_IOUT_1_DATA				0X33	// ADC Iout1 数据
#define ADC_IOUT_2_DATA				0X34	// ADC Iout2 数据
#define ADC_NTC_VOLTAGE_DATA		0X37	// ADC NTC 	 电压数据
#define ADC_CONFIG					0X3A	// ADC 配置
#define ADC_DATA_HIGH_8BIT			0X3B	// ADC 数据高8位
#define ADC_DTAT_LOW_4BIT			0X3C	// ADC 数据低4位
#define PD_CMD_REQUEST				0X70	// PD命令请求
#define PD_MESAGE_SET				0X71	// PD消息设置
#define PD_SRC_CAP_CMD_SEND			0X73	// PD SRC CAP 命令发送
#define HARDRESET_COUNT_SET			0X75	// 硬复位 次数设置
#define CONNECT_SET					0X76	// 连接设置
#define VOLTAGE_OFFSET_SET			0XA6	// 电压偏移设置
#define FAST_CHARGE_CONFIG_0		0XAA	// 快充配置0	
#define PORT_MODE_CONFIG			0XAB	// 端口模式配置
#define FAST_CHARGE_CONFIG_1		0XAD	// 快充配置1
#define VID_CONFIG_0				0XAF	// VID配置0
#define PD_CONFIG_0					0XB0	// PD配置0
#define PD_CONFIG_1					0XB1	// PD配置1
#define PD_CONFIG_2					0XB2	// PD配置2
#define PD_CONFIG_3					0XB3	// PD配置3
#define PD_CONFIG_4					0XB4	// PD配置4
#define PD_CONFIG_5					0XB5	// PD配置5
#define PD_CONFIG_6					0XB6	// PD配置6
#define PD_CONFIG_7					0XB7	// PD配置7
#define PD_CONFIG_8					0XB8	// PD配置8
#define FAST_CHARGE_CONFIG_2		0XB9	// 快充配置2
#define FAST_CHARGE_CONFIG_3		0XBA	// 快充配置3
#define FAST_CHARGE_CONFIG_4		0XBC	// 快充配置4
#define CURRENT_LIMITING_0			0XBD	// 限流配置0
#define PD_CONFIG_9					0XBE	// PD配置9
#define VID_CONFIG_1				0XBF	// VID配置1
#define CURRENT_LIMITING_1			0XC4	// 限流配置1
#define FAST_CHARGE_CONFIG_5		0XC5	// 快充配置5
#define FAST_CHARGE_CONFIG_6		0XCF	// 快充配置6

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
	Protect_Event_e		Protect_Event;		// 保护事件
	
}SW3516P_Work_State_t;



#endif
