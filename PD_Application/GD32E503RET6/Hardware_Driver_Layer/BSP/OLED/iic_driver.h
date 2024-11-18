#ifndef _IIC_DRIVER_H_
#define _IIC_DRIVER_H_

#include "stm32f4xx_hal.h"

#define HARDWARE_IIC_EN

#pragma pack(push,8)
typedef struct
{
	uint16_t	Dev_Addr;
	uint8_t 	Reg_Addr;
	uint8_t		P_Data[128];
	uint16_t 	Data_Len;
}IIC_Transmit_t;
#pragma pack(pop)

extern I2C_HandleTypeDef I2C_Config;

void IIC_Driver_Init(void);
void IIC_Driver_Write(IIC_Transmit_t transmit_data);
void IIC_Driver_Read(uint8_t device_addr, uint8_t reg_addr, uint8_t *get_data);



#endif
