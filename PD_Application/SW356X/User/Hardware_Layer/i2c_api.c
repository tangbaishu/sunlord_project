#include "i2c_api.h"
#include "i2c_master_driver.h"
#include "zr_i2c.h"
#include "zr_systick.h"
#include "stdio.h"

extern void I2C_Master_Driver_Init(uint8_t device_id);

/**
 * @brief  * @brief I2C主机写入 N数据
 * 
 * @param wirte_data
	struct Transfer_Info_t
	{
		uint8_t Reg_Addr;
		uint8_t *P_Wdata[IIC_MAX_WDATA_LEN];
		uint8_t Wdata_Len;
	}wirte_data;
 *
 * @return I2C_State_e 
 */
extern I2C_State_e I2C_Master_Write_NByte(Transfer_Info_t *wirte_data);

extern I2C_State_e I2C_Master_Read_NByte(Transfer_Info_t *read_data);

/**
 * @brief 驱动测试
 * 
 */
void I2C_Master_Driver_Check(void)
{
	I2C_Master_Driver_Init(0x3c);
	printf("I2C_Master_Init\r\n");
	I2C_M_Driver.Transfer_Data.P_Wdata[0] = 0x01;

	I2C_M_Driver.Transfer_Data.Reg_Addr = 0x01;
	I2C_M_Driver.Transfer_Data.P_Wdata[0]++;
	I2C_M_Driver.Transfer_Data.P_Wdata[1] = 0xFF;
	I2C_M_Driver.Transfer_Data.P_Wdata[2] = 0x00;
	I2C_M_Driver.Transfer_Data.P_Wdata[3] = 0x55;
	I2C_M_Driver.Transfer_Data.P_Wdata[4] = 0xAA;
	I2C_M_Driver.Transfer_Data.Wdata_Len = 5;
	
	printf("i2c_master return:%#x\r\n", (uint8_t)I2C_Master_Write_NByte(&I2C_M_Driver.Transfer_Data));
	Systick_Delay_Ms(100);

	I2C_M_Driver.Transfer_Data.Reg_Addr = 0x01;
	I2C_M_Driver.Transfer_Data.Wdata_Len = 0;
	I2C_M_Driver.Transfer_Data.Rdata_Len = 1;
	I2C_Master_Read_NByte(&I2C_M_Driver.Transfer_Data);
	printf("i2c_read %#x\r\n", I2C_M_Driver.Transfer_Data.P_Rdata[0]);
	
}

void I2C_Hardware_Init(uint8_t device_addr)
{
	I2C_Master_Driver_Init(device_addr);
}

I2C_State_e I2C_Master_Write_Single_Byte(uint8_t device_id, uint8_t reg_addr, uint8_t data)
{
	uint8_t wait=0;
	while(1)	// 等待总线空闲
	{
		if(I2c_Master_Busy_Flag() == SET) 		// 若忙碌
		{
			Systick_Delay_Ms(1);
			wait++;
			if(wait >= IIC_BUSY_TIMEOUT_TIME)
			{
				return BUSY_TIMEOUT;
			}
		}
		else
		{
			wait = 0;
			break;
		}
	}
	I2C_M_Driver.Runing_State = BUSY;
	I2C_M_Driver.Transfer_Data.Reg_Addr = reg_addr;
	I2C_M_Driver.Transfer_Data.P_Wdata[0] = data;
	I2C_M_Driver.Transfer_Data.Wdata_Len = 1;
	I2C_Master_Write_NByte(&I2C_M_Driver.Transfer_Data);
	return I2C_M_Driver.Runing_State;
}

I2C_State_e IIC_Driver_Write_Multi_Data(uint8_t device_id, uint8_t reg_addr, uint8_t *pdata, uint8_t data_len)
{
	uint8_t num=0, wait=0;
	if(data_len >= IIC_MAX_WDATA_LEN)
	{
		printf("Error!!!\t IIC Write Data Length OverFlow\r\n");
		return I2C_M_Driver.Runing_State = DATA_OVERFLOW;
	}

	while(1)
	{
		if(I2c_Master_Busy_Flag() == SET) 		// 若忙碌
		{
			Systick_Delay_Ms(1);
			wait++;
			if(wait >= IIC_BUSY_TIMEOUT_TIME)
			{
				return BUSY_TIMEOUT;
			}
		}
		else
		{
			wait = 0;
			break;
		}
	}
	I2C_M_Driver.Runing_State = BUSY;
	I2C_M_Driver.Transfer_Data.Reg_Addr = reg_addr;
	for(; num < data_len; num++)
	{
		I2C_M_Driver.Transfer_Data.P_Wdata[num] = pdata[num];
	}
	I2C_M_Driver.Transfer_Data.Wdata_Len = data_len;
	I2C_Master_Write_NByte(&I2C_M_Driver.Transfer_Data);
	return I2C_M_Driver.Runing_State;
}

I2C_State_e IIC_Driver_Read_Single_Data(uint8_t device_id, uint8_t reg_addr, uint8_t *pdata)
{
	uint8_t wait=0;
	while(1)
	{
		if(I2c_Master_Busy_Flag() == SET) 		// 若忙碌
		{
			Systick_Delay_Ms(1);
			wait++;
			if(wait >= IIC_BUSY_TIMEOUT_TIME)
			{
				return BUSY_TIMEOUT;
			}
		}
		else
		{
			wait = 0;
			break;
		}
	}
	I2C_M_Driver.Runing_State = BUSY;
	I2C_M_Driver.Transfer_Data.Reg_Addr = reg_addr;
	I2C_M_Driver.Transfer_Data.Wdata_Len = 0;
	I2C_M_Driver.Transfer_Data.Rdata_Len = 1;
	I2C_Master_Read_NByte(&I2C_M_Driver.Transfer_Data);
	*pdata = I2C_M_Driver.Transfer_Data.P_Rdata[0];
	return I2C_M_Driver.Runing_State;
}

I2C_State_e IIC_Driver_Read_Multi_Data(uint8_t device_id, uint8_t reg_addr, uint8_t *pdata, uint8_t data_len)
{
	uint8_t num=0, wait=0;
	if(data_len >= IIC_MAX_RDATA_LEN)
	{
		printf("Error!!!\t IIC Read Data Length OverFlow\r\n");
		return I2C_M_Driver.Runing_State = DATA_OVERFLOW;
	}
	while(1)
	{
		if(I2c_Master_Busy_Flag() == SET) 		// 若忙碌
		{
			Systick_Delay_Ms(1);
			wait++;
			if(wait >= IIC_BUSY_TIMEOUT_TIME)
			{
				return BUSY_TIMEOUT;
			}
		}
		else
		{
			wait = 0;
			break;
		}
	}
	I2C_M_Driver.Runing_State = BUSY;
	I2C_M_Driver.Transfer_Data.Reg_Addr = reg_addr;
	I2C_M_Driver.Transfer_Data.Wdata_Len = 0;
	I2C_Master_Read_NByte(&I2C_M_Driver.Transfer_Data);
	for(; num < data_len; num++)
	{
		pdata[num] = I2C_M_Driver.Transfer_Data.P_Rdata[num];
	}
	return I2C_M_Driver.Runing_State;
}