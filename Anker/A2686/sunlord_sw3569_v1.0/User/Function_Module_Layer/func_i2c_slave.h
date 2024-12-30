#ifndef _FUNC_I2C_SLAVE_H_
#define _FUNC_I2C_SLAVE_H_

#include "i2c_protocol_library.h"
#include "port_data_library.h"
#include "ntc_temp_sample.h"
#include "i2c_slave_driver.h"

typedef struct
{
	Port_Out_Protocol_Data_t	*Port2_Out_Protocol_Data;
	Port_RealTime_Out_Power_t	*Port2_RealTime_Out_Power;
	NTC_Temp_Data_t				*NTC_Temp_Data;
	POWER_PDO_Mode_e			*Port2_Seting_Output_Power;
	I2C_Running_State_t			*I2C_Running_State;
}Func_I2C_Slave_Data_t;
extern Func_I2C_Slave_Data_t	Func_I2C_Slave_Data;

void Func_I2CS_Dispose(I2CS_Int_Event event);

#endif
