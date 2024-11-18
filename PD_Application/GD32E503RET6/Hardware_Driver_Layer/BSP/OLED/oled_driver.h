#ifndef _OLED_DRIVER_H_
#define _OLED_DRIVER_H_

#include "iic_driver.h"

#define FREERTOS_EN						//系统使能FreeRTOS
#ifdef FREERTOS_EN
	#include "FreeRTOS.h"
	#include "semphr.h"
#endif // FREERTOS_EN


#define OLED_DEVICE_ADDR		0x78	//oled设备地址
#define OLED_WRITE_DATA_REG		0x40	//oled写入数据寄存器地址
#define OLED_WRITE_CMD_REG		0x00	//oled写入指令寄存器地址

typedef struct
{
	void (*IIC_Init) (void);
	void (*IIC_Write) (IIC_Transmit_t transmit_data);
	void (*IIC_Read) (uint8_t device_addr, uint8_t reg_addr, uint8_t *get_data);
	#ifdef FREERTOS_EN
	QueueHandle_t IIC_Semaphore_Binary;
	#endif //FREETROS_EN
}IIC_Interface_t;
extern IIC_Interface_t IIC_Interface;

extern IIC_Transmit_t IIC_Transmit;

#ifdef FREERTOS_EN
	extern TaskHandle_t OLED_Driver_Handle;
#endif //FREETROS_EN

void OLED_Driver_Task(void *param);
void OLED_Init(void);
void OLED_IIC_Byte(uint8_t cmd, uint8_t data);

#endif
