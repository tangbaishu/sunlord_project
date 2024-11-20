#ifndef _SOFTWARE_IIC_DRIVER_H_
#define _SOFTWARE_IIC_DRIVER_H_

#include "gd32e50x.h"
#include "systick.h"

//-----------------OLED IIC端口定义----------------
#define SOFTWARE_IIC_PORT		GPIOB			// 软件模拟IIC 引脚组	
#define SOFTWARE_IIC_COLCK		RCU_GPIOB		// 软件模拟IIC 引脚组时钟
#define SOFTWARE_IIC_SCL_PIN	GPIO_PIN_10		// 软件模拟IIC SCL引脚端口
#define SOFTWARE_IIC_SDA_PIN	GPIO_PIN_11		// 软件模拟IIC SDA引脚端口

#define IIC_SCL_PIN_OUTPUT()	gpio_init(SOFTWARE_IIC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, SOFTWARE_IIC_SCL_PIN)
#define IIC_SCL_PIN_INPUT()		gpio_init(SOFTWARE_IIC_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, SOFTWARE_IIC_SCL_PIN)

#define IIC_SDA_PIN_OUTPUT()	gpio_init(SOFTWARE_IIC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, SOFTWARE_IIC_SDA_PIN)
#define IIC_SDA_PIN_INPUT()		gpio_init(SOFTWARE_IIC_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, SOFTWARE_IIC_SDA_PIN)

#define IIC_SCL_PIN_LOW() 		gpio_bit_write(SOFTWARE_IIC_PORT, SOFTWARE_IIC_SCL_PIN,RESET) 	// SCL
#define IIC_SCL_PIN_HIGH() 		gpio_bit_write(SOFTWARE_IIC_PORT, SOFTWARE_IIC_SCL_PIN,SET)

#define IIC_SDA_PIN_LOW() 		gpio_bit_write(SOFTWARE_IIC_PORT, SOFTWARE_IIC_SDA_PIN,RESET) 	// SDA
#define IIC_SDA_PIN_HIGH() 		gpio_bit_write(SOFTWARE_IIC_PORT, SOFTWARE_IIC_SDA_PIN,SET)
#define IIC_SDA_PIN_READ()		gpio_input_bit_get(SOFTWARE_IIC_PORT, SOFTWARE_IIC_SDA_PIN)

// void IIC_Start(void);
// void IIC_Stop(void);
// void IIC_Wait_Ack(void);
// void Write_IIC_Byte(unsigned char IIC_Byte);
// void Read_IIC_Byte(unsigned char *IIC_Byte);

void software_iic_Init(void);
void delay_check(void);

/**
 * @brief
 * IIC单数据写入
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Write_Single_Data(unsigned char device_id, unsigned char reg_adr, unsigned char data);

/**
 * @brief
 * IIC多数据写入
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Write_Multi_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *pdata, unsigned char data_len);

/**
 * @brief
 * IIC单数据读取
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Read_Single_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *pdata);

/**
 * @brief
 * IIC多数据读取
 * 返回值 1:PASS、0:ERROR
 */
unsigned char IIC_Driver_Read_Multi_Data(unsigned char device_id, unsigned char reg_adr, unsigned char *data, unsigned char data_len);

void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);

#endif
