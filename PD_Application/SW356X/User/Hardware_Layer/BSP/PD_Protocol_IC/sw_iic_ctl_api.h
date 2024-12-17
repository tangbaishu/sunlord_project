/**
 * @file sw_iic_cTL_api.h
 * @author your name (you@domain.com)
 * @brief	智融SW IIC寄存器系列芯片，常规功能函数映射
 * 具体功能如下：
 * 配置输出端口			typedef void (*P_Config_Port_Mode)	(Output_Port_Mode_e Output_Port_Mode);
 * 配置最大输出功率		typedef void (*P_Config_Max_Power)	(uint8_t port1, uint8_t port2);
 * 配置PD限流			typedef void (*P_PD_Current_Limiting)	(Output_Voltage_e voltage_grade, uint16_t current_limiting);
 * 配置快充协议使能		typedef void (*P_Config_Protocol_En)	(SW_Protocol_Ctl_t protocol_config);
 * 
 * 获取插入端口			typedef Port_Info_e (*P_Get_Insert_Port) (void);
 * 获取输出功率			typedef void (*P_Get_Output_Power)	(uint16_t  *output_ma, uint16_t *output_mv);
 * 获取NTC温度			typedef uint16_t (*P_Get_NTC_Temp) (void);
 * 
 * 异常状态回调			typedef void (*P_Abnormal_Hook)(SW_Protection_Event_t *protection_event);
 * 
 * 强制打开/关闭通路	typedef void (Compel_Open_VBUS)(bool on_off);
 * 强制打开/关闭Buck	typedef void (Compel_Open_Buck)(bool on_off);
 * 强制设置DAC电压&电流	typedef void (Compel_Set_DAC)(uint16_t dac_voltage, uint16_t dac_current);
 * 
 * 变量名称缩写说明：
 * Ctl 	-> Control
 * func	-> function
 * En	-> Enable
 * @version 0.1
 * @date 2024-12-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _POWER_SW_IIC_CTL_API_H_
#define _POWER_SW_IIC_CTL_API_H_

#include "system.h"

typedef enum
{
	OUTPUT_SINGLE_A = 0,
	OUTPUT_SINGLE_C,
	OUTPUT_DUAL_AA,
	OUTPUT_DUAL_CA,
	OUTPUT_DUAL_CC
}Output_Port_Mode_e;

typedef enum
{
	ALL_PULL_OUT	= 0x00,		// 全部拔出
	INSERT_PORT_1 	= 0x01,		// 若未插入，该位清零
	INSERT_PORT_2 	= 0x02,
	INSERT_PORT_1_2	= 0x03
}Port_Info_e;

typedef enum
{
	OUTPUT_5V = 0x01,
	OUTPUT_9V = 0x02,
	OUTPUT_12V = 0x04,
	OUTPUT_15V = 0x08,
	OUTPUT_20V = 0x10
}Output_Voltage_e;

typedef enum
{
	MAX_18W,
	MAX_27W,
	MAX_40W,
	MAX_45W
}MAX_Output_Power_e;

typedef union
{
	uint32_t Data;
	struct
	{
		bool	BC_DCP_Enable;		// bit0
		bool	Apple_Enable;
		bool	Samsung_Enable;
		bool	FCP_Enable;
		bool	LOW_V_SCP_Enable;	// 低压SCP协议使能
		bool	High_V_SCP_Enable;	// 高压SCP协议使能
		bool	PE_Enable;
		bool	SFCP_Enable;

		bool	AFC_Enable;
		bool	QC2_Enable;
		bool	QC3_Enable;
		bool	QC3_Plus_Enable;	// QC3+
		bool	QC4_Plus_Enable;	// QC4+
		bool	TFCP_Enable;		// 传音TFCP


		bool	PD_Enable;
		bool 	PPS0_Enable;
		bool 	PPS1_Enable;
		bool	PPS2_Enable;
		bool	PPS3_Enable;
	};

}SW_Protocol_Ctl_t;

typedef union
{
	uint16_t Data;
	struct
	{
		bool Protection_Enable;		// FALSE:不使能、 TRUE：使能
		bool NTC_OTP;
		bool DIE_OTP;
		bool VIN_UVLO;				// VIN under voltage lock out
		bool VIN_OVP;				// OVP over voltage protection
		bool VOUT_UVP;				// UVP under voltage protection	
		bool VOUT_SCP;				// SCP	
		bool VOUT_Fast_OVP;
		bool VOUT_Slow_OVP;			// CS
	};
}SW_Protection_Event_t;

typedef enum
{
	PD2=0,
	PD3
}PD_Version_e;


typedef union
{
	PD_Version_e	PD_Version : 1;		// 选择PD协议版本 PD2: PD2.0 PD3: PD3.0
	uint8_t			PD_Out_Valtage;		// 参数列表：Output_Valtage_e 支持位操作
}SW_IIC_PD_Ctl_t;

typedef void (*P_IIC_Write_Enable)	(void);
typedef void (*P_PD_Current_Limiting)	(Output_Voltage_e voltage_grade, uint16_t current_limiting);

typedef void (*P_Config_Port_Mode)	(Output_Port_Mode_e Output_Port_Mode);
typedef void (*P_Config_Max_Power)	(uint8_t port1, uint8_t port2);
typedef void (*P_Config_Protocol_En)	(SW_Protocol_Ctl_t protocol_config);

typedef Port_Info_e (*P_Get_Insert_Port) (void);
typedef void (*P_Get_Output_Power)	(uint16_t  *output_ma, uint16_t *output_mv);
typedef uint16_t (*P_Get_NTC_Temp) (void);

typedef void (*P_Abnormal_Hook)(SW_Protection_Event_t *protection_event);

typedef void (*P_Compel_Open_VBUS)(bool on_off);
typedef void (*P_Compel_Open_Buck)(bool on_off);
typedef void (*P_Compel_Set_DAC)(uint16_t dac_voltage, uint16_t dac_current);


typedef struct
{
	P_IIC_Write_Enable 		IIC_Write_Enable;
	P_PD_Current_Limiting 	PD_Current_Limiting;

	P_Config_Port_Mode		Config_Port_Mode;
	P_Config_Max_Power		Config_Max_Power;
	P_Config_Protocol_En	Config_Protocol_En;

	P_Get_Insert_Port		Get_Insert_Port;
	P_Get_Output_Power		Get_Output_Power;
	P_Get_NTC_Temp			Get_NTC_Temp;

	P_Abnormal_Hook			Abnormal_Hook;
	
	P_Compel_Open_VBUS		Compel_Open_VBUS;
	P_Compel_Open_Buck		Compel_Open_Buck;
	P_Compel_Set_DAC		Compel_Set_DAC;
}SW_IIC_Ctl_Func_t;
extern SW_IIC_Ctl_Func_t SW_IIC_Ctl_Func;		// 需制作实际执行该功能的函数，并将函数地址辅助给该变量
void SW_IIC_Ctl_Init(SW_IIC_Ctl_Func_t *sw_iic_ctl_func);	// 在该函数中，将实现具体功能的函数主体的函数地址赋值给 SW_IIC_Ctl_Func

/**
 * @brief 智融外挂芯片IIC写使能控制
 * 
 */
void SW_IIC_Write_Enable(void);

/**
 * @brief 针对PD协议不同电压挡位，设定对应的限流值
 * 
 * @param v_grade 电压挡位 参数列表: Output_Voltage_e
 * @param ma 	限流值，单位: mA
 */
void SW_IIC_PD_Current_Limiting(Output_Voltage_e v_grade, uint16_t current_limiting);


void SW_IIC_Output_Power(uint16_t  *output_ma, uint16_t *output_mv);

void SW_IIC_Config_Port_Mode(Output_Port_Mode_e output_port_mode);

void SW_IIC_Config_Max_Power(uint8_t port1, uint8_t port2);

void SW_IIC_Config_Protocol_En(SW_Protocol_Ctl_t protocol_config);

Port_Info_e SW_IIC_Get_Insert_Port();

void SW_IIC_Get_Output_Power(uint16_t *output_ma, uint16_t *output_mv);

uint16_t SW_IIC_Get_NTC_Temp();

void SW_IIC_Abnormal_Hook(SW_Protection_Event_t *protection_event);

void SW_IIC_Compel_Open_VBUS(bool on_off);

void SW_IIC_Compel_Open_Buck(bool on_off);

void SW_IIC_Compel_Set_DAC(uint16_t dac_voltage, uint16_t dac_current);






#endif
