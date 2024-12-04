#ifndef _I2C_API_H_
#define _I2C_API_H_

#ifndef uint8_t
#ifndef _DECA_UINT8_T_
#define _DECA_UINT8_T_
typedef unsigned char uint8_t;
#endif // !_DECA_UINT8_T_
#endif // ! uint8_t

#define IIC_BUSY_TIMEOUT_TIME		20				//	单位：ms
#define IIC_WAIT_ACK_TIMEOUT_TIME	100				//	单位：10us

#include "i2c_master_driver.h"

void I2C_Hardware_Init(uint8_t device_addr);
I2C_State_e I2C_Master_Write_Single_Byte(uint8_t device_id, uint8_t reg_addr, uint8_t data);
I2C_State_e IIC_Driver_Write_Multi_Data(uint8_t device_id, uint8_t reg_addr, uint8_t *pdata, uint8_t data_len);
I2C_State_e IIC_Driver_Read_Single_Data(uint8_t device_id, uint8_t reg_addr, uint8_t *pdata);
I2C_State_e IIC_Driver_Read_Multi_Data(uint8_t device_id, uint8_t reg_addr, uint8_t *pdata, uint8_t data_len);

#define IIC_MASTER_INIT(device_addr)									I2C_Hardware_Init(device_addr)
#define IIC_WRITE_SINGLE_DATA(device_id, reg_addr, data)				I2C_Master_Write_Single_Byte(device_id, reg_addr, data)
#define IIC_WRITE_MULTI_DATA(device_id, reg_addr, pdata, data_len) 		IIC_Driver_Write_Multi_Data(device_id, reg_addr, pdata, data_len)
#define IIC_READ_SINGLE_DATA(device_id, reg_addr, pdata)				IIC_Driver_Read_Single_Data(device_id, reg_addr, pdata)
#define IIC_READ_MULTI_DATA(device_id, reg_addr, pdata, len_data)		IIC_Driver_Read_Multi_Data(device_id, reg_addr, pdata, len_data)	

void I2C_Master_Driver_Check(void);

#endif
