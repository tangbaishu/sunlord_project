/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file i2c_slave.h
 * @brief Functions interfaces for the I2C firmware library
 * @author David
 ****************************************************************************/
#ifndef I2C_SLAVE_DRIVER_H
#define I2C_SLAVE_DRIVER_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// This demo demostarates how to use i2c slave reading in sleep mode
void I2c_Slave_Driver_Init(void);
// void I2c_Slave_Policy_Run();

typedef enum
{
	MASTER_READ,
	MASTER_WRITE
}I2CS_Int_Event;	// I2C从机中断事件

typedef void (*P_I2c_Slave_Busi) (I2CS_Int_Event event);

extern P_I2c_Slave_Busi Pointer_I2C_Slave_Busi;

#ifdef __cplusplus
}
#endif

#endif

