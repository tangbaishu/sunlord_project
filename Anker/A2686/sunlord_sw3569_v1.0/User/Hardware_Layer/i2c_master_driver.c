#include "i2c_master_driver.h"
#include "zr_systick.h"
#include "zr_rcc.h"
#include "zr_gpio.h"
#include "zr_i2c.h"
#include "zr_nvic.h"
#include <stdio.h>

I2C_M_Driver_Info_t I2C_M_Driver;
i2cm_config_t 		i2cm_config;

void I2C_Master_Driver_Init(uint8_t device_id)
{
	gpio_config_t gpio_config;

	Rcc_Module_Rst(I2C_ID);
	Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID);
	Rcc_Module_Rst_Release(I2C_ID);

	Gpio_Deinit(I2C_M_SCK_GPIO_PIN);
	Gpio_Deinit(I2C_M_SDA_GPIO_PIN);
	Gpio_Function_Select(I2C_M_SCK_GPIO_PIN, I2C_M_SCK_AF_MODE);
	Gpio_Function_Select(I2C_M_SDA_GPIO_PIN, I2C_M_SDA_AF_MODE);
	Gpio_Mos_Opendrain_Cfg(I2C_M_SCK_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
	Gpio_Mos_Opendrain_Cfg(I2C_M_SDA_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
	Gpio_Pulldown_Pullup_Cfg(I2C_M_SCK_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
	Gpio_Pulldown_Pullup_Cfg(I2C_M_SDA_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
	Gpio_Pullup_Resistance_Cfg(I2C_M_SCK_GPIO_PIN, PULLUP_RES_10K);
	Gpio_Pullup_Resistance_Cfg(I2C_M_SDA_GPIO_PIN, PULLUP_RES_10K);
	

	i2cm_config.devAddr = device_id;
	i2cm_config.clockSpeed = 50;
	I2c_Master_Init(&i2cm_config);
	I2c_Master_Int_Enable(I2C_M_FINISH_INT_EN | I2C_M_ERROR_INT_EN | I2C_M_BUS_LOST_INT_EN | I2C_M_SCL_TIMEOUT_INT_EN);
	I2c_Master_Clear_Pending(I2C_M_FINISH_PENGING | I2C_M_NACK_PENGING | I2C_M_BUS_LOST_PENGING | I2C_M_SCL_TIMEOUT_PENGING);
	Nvic_Irq_Enable(I2CM_IRQn, PRIORITY_0);
	I2c_Master_Enable();
}

/**
 * @brief  * @brief I2C主机写入 N数据
 * 
 * @param wirte_data
	struct I2CM_Transfer_Info_t
	{
		uint8_t Reg_Addr;
		uint8_t *P_Wdata[IIC_MAX_WDATA_LEN];
		uint8_t Wdata_Len;
	}wirte_data;
 *
 * @return I2C_Master_State_e 
 */
I2C_Master_State_e I2C_Master_Write_NByte(I2CM_Transfer_Info_t *wirte_data)
{
	I2c_Master_Write_Data((uint8_t *)wirte_data, wirte_data->Wdata_Len + 1);
	return I2C_M_Driver.Runing_State;
}

I2C_Master_State_e I2C_Master_Read_NByte(I2CM_Transfer_Info_t *read_data)
{
	I2C_M_Driver.Runing_State = BUSY;
	I2c_Master_Read_Data((u8*)&read_data->Reg_Addr, read_data->Wdata_Len + 1, read_data->P_Rdata, read_data->Rdata_Len);
	return I2C_M_Driver.Runing_State;
}

void I2CM_Handler(void)
{
	if(I2c_Master_Get_Pending(I2C_M_FINISH_PENGING) == SET)
	{
		I2C_M_Driver.Runing_State = FINISH;
		I2c_Master_Clear_Pending(I2C_M_FINISH_PENGING);
		I2C_M_Driver.Runing_State = IDLE;
	}

	if(I2c_Master_Get_Pending(I2C_M_NACK_PENGING) == SET)
	{
		I2C_M_Driver.Runing_State = WAIT_FINISH_OVER_TIME;
		I2c_Master_Clear_Pending(I2C_M_NACK_PENGING);
	}

	if(I2c_Master_Get_Pending(I2C_M_BUS_LOST_PENGING))
	{
		I2C_M_Driver.Runing_State = BUS_LOST;
		I2c_Master_Clear_Pending(I2C_M_BUS_LOST_PENGING);
	}

	if(I2c_Master_Get_Pending(I2C_M_SCL_TIMEOUT_PENGING))
	{
		I2C_M_Driver.Runing_State = SCL_TIMEOUT;
		I2c_Master_Clear_Pending(I2C_M_SCL_TIMEOUT_PENGING);
	}
}

void I2C_Master_Driver_Test(uint8_t device_id)
{
	I2CM_Transfer_Info_t i2cm_test_data;
	I2C_Master_Driver_Init(device_id);
	Systick_Delay_Ms(200);
	i2cm_test_data.Reg_Addr = 0X01;
	i2cm_test_data.P_Wdata[0] = 0xAF;
	i2cm_test_data.P_Wdata[1] = 0xFA;
	i2cm_test_data.Wdata_Len = 2;
	I2C_Master_Write_NByte(&i2cm_test_data);
}