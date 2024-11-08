#ifndef _SOFTWARE_IIC_DRIVER_H_
#define _SOFTWARE_IIC_DRIVER_H_

#include "gd32e50x.h"
#include "systick.h"

//-----------------OLED IIC端口定义----------------  					   
#define OLED_SCLK_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_10)//SCL
#define OLED_SCLK_Set() gpio_bit_set(GPIOB,GPIO_PIN_10)

#define OLED_SDIN_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_11)//SDA
#define OLED_SDIN_Set() gpio_bit_set(GPIOB,GPIO_PIN_11)

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Byte(unsigned char IIC_Byte);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);


#endif
