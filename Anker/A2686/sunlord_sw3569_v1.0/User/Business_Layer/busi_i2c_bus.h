/**
 * @file busi_i2c_bus.h
 * @author tong.libo@sunlord.com.cn
 * @brief 
 * @version 0.1
 * @date 2024-12-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _BUSI_I2C_BUS_H_
#define _BUSI_I2C_BUS_H_

#include "busi_port_detection.h"
#include "i2c_protocol_library.h"
#include "port_data_library.h"
#include "ntc_temp_sample.h"

typedef struct 
{
	Port_Out_Protocol_Data_t	Port2_Out_Protocol_Data;
	Port_RealTime_Out_Power_t	Port2_RealTime_Out_Power;
	NTC_Temp_Data_t				NTC_Temp_Data;
	POWER_PDO_Mode_e			Port2_Seting_Output_Power;
	I2C_Running_State_t			*I2C_Running_State;
}I2C_Transfer_Data_t;
extern I2C_Transfer_Data_t Busi_I2C_Bus_Data;

void Busi_I2C_Init(void);
void Busi_I2C_Bus(void);

void Busi_I2CM_Read_Slave_Register(I2C_Register_List_e register_adr);
void Busi_I2CM_Write_Slave_Register(void);





#endif // !_BUSI_I2C_BUS_H_
