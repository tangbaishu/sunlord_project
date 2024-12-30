#ifndef _FUNC_HARDWARE_INIT_H_
#define _FUNC_HARDWARE_INIT_H_

#define	HOST_DEVICE		0
#define SLAVE_DEVICE	1
#define DEVICE_TYPE		HOST_DEVICE

#include "ntc_temp_sample.h"
#include "i2c_master_driver.h"
#include "i2c_slave_driver.h"

typedef struct
{
	uint8_t					*Port1_NTC_Temp_State;
	uint8_t					*Port2_NTC_Temp_State;
	OverTemp_Dispose_e		*Port1_OverTemp_Dispose_Flag;
	OverTemp_Dispose_e		*Port2_OverTemp_Dispose_Flag;	
}Port_BSP_Data_t;
extern Port_BSP_Data_t Port_BSP_Data;

typedef struct
{
	void 		(*P_I2C_Master_Init) 		(uint8_t device_id);
	I2C_Master_State_e (*P_I2C_Master_Write_NByte)	(I2CM_Transfer_Info_t *wirte_data);
	I2C_Master_State_e (*P_I2C_Master_Read_NByte)	(I2CM_Transfer_Info_t *read_data);
}IIC_Master_Driver_t;


typedef void (*P_NTC_Driver_Init) 			(void);
typedef void (*P_Base_Timer_Driver_Init)	(unsigned int timer_ms);
typedef void (*P_I2C_Slave_Driver_Init)		(void);

typedef struct
{
	#if (DEVICE_TYPE == HOST_DEVICE)
		IIC_Master_Driver_t			IIC_Master_Driver;
	#else
		P_I2C_Slave_Driver_Init		Func_I2C_Slave_Driver_Init;
	#endif
	P_NTC_Driver_Init			Func_NTC_Driver_Init;
	P_Base_Timer_Driver_Init	Func_Base_Timer_Driver_Init;
}Func_Fast_Charge_Driver_t;
extern Func_Fast_Charge_Driver_t Func_Fast_Charge_Driver;

void Func_Fast_Charge_Drvier_Init(void);

#endif
