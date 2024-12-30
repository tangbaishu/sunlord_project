#ifndef _BUSI_POWER_POLICY_ENGINE_H_
#define _BUSI_POWER_POLICY_ENGINE_H_
#include "config.h"
#include "busi_fast_charge_config.h"
#include "busi_port_detection.h"
#include "busi_i2c_bus.h"
#include "zr_adc.h"

#define PORT1_OUTPUT_POWER_POLICY(config_pointer, power_garde)		Busi_Port_Detection_Data.Port1_Out_Protocol_Data.Current_Power_Grade = power_garde;\
																				Busi_Power_PDO_Mode_Select(config_pointer, power_garde)
#define PORT2_OUTPUT_POWER_POLICY(config_pointer, power_garde)		Busi_Port_Detection_Data.Port2_Out_Protocol_Data->Current_Power_Grade = power_garde
																				

#define GET_PORT1_OUTPUT_VOLTAGE()									Adc_Vout_Get()
#define GET_PORT2_OUTPUT_VOLTAGE()									0

#define	PORT_POWER_SETING_RIGHT_AWAY() 								Busi_Right_Away_Power_PDO_Mode()	\
																	//;Busi_I2CM_Write_Slave_Register()


#define ONE_STAGE_CHECK_TIME										6000	// 一阶段 设备类型检测时间,单位: ms
#define TWO_STAGE_CHECK_TIME										21000	// 二阶段 设备类型检测时间,单位: ms
#define OUTPUT_VOLTAGE_SAMPLE_RATE									50		// 输出电压采样速率		

typedef struct
{
	uint16_t	Device_Type_Check_Timer;
	uint16_t	Output_Voltage_Sample_Timer;
	uint16_t	Port_1_Output_Voltage_Peak;
	uint16_t	Port_2_Output_Voltage_Peak;
	bool		Software_Timer_EN : 1;
	bool		One_Stage_Check_Finish_Flag : 1;
	bool		Two_Stage_Check_Finish_Flag : 1;
}Busi_Power_Policy_Data_t;
extern Busi_Power_Policy_Data_t	Busi_Power_Policy_Data;

void Real_Time_Allocation_Power(config_data_t *config_data);

#endif
