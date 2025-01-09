#ifndef _FUNC_HARDWARE_API_H_
#define _FUNC_HARDWARE_API_H_

#include "hardware_config.h"
#include "ntc_temp_sample.h"
#include "func_i2c_api.h"

typedef struct
{
	uint16_t	*NTC_Voltage_Value;
	uint8_t		*NTC_Pullup_Current;				// 单位：uA
	uint8_t		*NTC_Over_Temp_Flag;
	uint8_t		*OverTemp_Dispose_Flag;
}NTC_Driver_Data_t;


typedef struct
{
	Device_Type_e		Device_Type;
	NTC_Driver_Data_t	NTC_Data;

}Hardware_Driver_Data_t;
extern Hardware_Driver_Data_t Hardware_Driver_Data;


typedef void (*P_NTC_Driver_Init) 			(void);
typedef void (*P_Base_Timer_Driver_Init)	(unsigned int timer_ms);

/**
 * @brief 	角色识别函数指针
 * @param	device_type
 */
typedef void (*P_Role_Identify)		(Device_Type_e *device_type);

typedef struct
{			
	I2C_Driver_API_t				I2C_Driver_API;
	P_NTC_Driver_Init				Func_NTC_Driver_Init;
	P_Base_Timer_Driver_Init		Func_Base_Timer_Driver_Init;
	P_Role_Identify					Func_Role_Identify;
}Hardware_Driver_API_t;
extern Hardware_Driver_API_t 	Hardware_Driver_API;

void Func_Hardware_Drvier_Init(void);

#endif
