#include "iic_driver.h"
#include "debug_driver.h"

#ifdef HARDWARE_IIC_EN

	I2C_HandleTypeDef I2C_Config;

	void IIC_Driver_Init(void)
	{
		I2C_Config.Instance = I2C1;
		I2C_Config.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		I2C_Config.Init.ClockSpeed = 400000;
		I2C_Config.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;	// 双重地址模式
		I2C_Config.Init.DutyCycle = I2C_DUTYCYCLE_2;
		I2C_Config.Init.GeneralCallMode = I2C_GENERALCALL_ENABLE;	// 常规调用
		I2C_Config.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
		I2C_Config.Init.OwnAddress1 = 0;
		I2C_Config.Init.OwnAddress2 = 0;
		HAL_I2C_Init(&I2C_Config);
		
	}

	void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
	{
		GPIO_InitTypeDef i2c_gpio_config;
		__HAL_RCC_GPIOB_CLK_ENABLE();
		i2c_gpio_config.Alternate = GPIO_AF4_I2C1;
		i2c_gpio_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		
		i2c_gpio_config.Mode = GPIO_MODE_AF_OD;
		i2c_gpio_config.Pin = GPIO_PIN_6;	// I2C1_SCL
		i2c_gpio_config.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &i2c_gpio_config);
		
		i2c_gpio_config.Mode = GPIO_MODE_AF_OD;
		i2c_gpio_config.Pin = GPIO_PIN_7;	// I2C1_SDA
		i2c_gpio_config.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &i2c_gpio_config);	

		__HAL_RCC_I2C1_CLK_ENABLE();	// 提前设置外设各项寄存器，最后使能IIC外设时钟。防止外设时钟提前使能，配置寄存器时外设误工作
	}

	void IIC_Driver_Write(IIC_Transmit_t transmit_data)
	{
		HAL_I2C_Master_Transmit(&I2C_Config, transmit_data.Dev_Addr, &transmit_data.Reg_Addr, transmit_data.Data_Len + 1, 0xffffff);
	}

	void IIC_Driver_Read(uint8_t device_addr, uint8_t reg_addr, uint8_t *get_data)
	{
		HAL_I2C_Master_Transmit(&I2C_Config, device_addr, &reg_addr, 1, 0xff);
		HAL_I2C_Master_Receive(&I2C_Config, device_addr, get_data, 1, 0xff);
	}

#else
	// 	GPIO_InitTypeDef  GPIO_InitStructure;
	//	__HAL_RCC_GPIOB_CLK_ENABLE();	//使能A端口时钟

	//	GPIO_InitStructure.Pin = GPIO_PIN_6|GPIO_PIN_7;	 
	// 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 //推挽输出
	//	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;//速度50MHz 
	//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);			//初始化GPIOD3,6
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
#endif
