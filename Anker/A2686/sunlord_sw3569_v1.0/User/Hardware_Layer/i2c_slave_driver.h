/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file i2c_slave.h
 * @brief Functions interfaces for the I2C firmware library
 * @author David
 ****************************************************************************/
#ifndef I2C_SLAVE_DRIVER_H
#define I2C_SLAVE_DRIVER_H

#include "system.h"
#include "hardware_config.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*P_I2c_Slave_Int_Dispose) (void);

void I2c_Slave_Driver_Init(P_I2c_Slave_Int_Dispose Int_Dispose_Func, Slave_Device_Addr_e dev_addr);

#ifdef __cplusplus
}
#endif

#endif

