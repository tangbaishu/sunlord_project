/**
 * @file 	LED_Driver.h
 * @author 	TLB
 * @brief 	智融SW356x 驱动GPIO外设,实现LED控制
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "zr_gpio.h"
#include "zr_rcc.h"

#define LED0_GPIO_CLOCK	(GPIO_ID)
#define LED0_GPIO_PIN	(GPIO1)

#define LED1_GPIO_CLOCK	(GPIO_ID)
#define LED1_GPIO_PIN	(GPIO2)

#define LED2_GPIO_CLOCK	(GPIO_ID)
#define LED2_GPIO_PIN	(GPIO3)

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

typedef enum
{
	LED0 = 0,
	LED1,
	LED2
}LED_BSP_Num_e;


void LED_Driver_Check(void);

void LED_Driver_Init(void);

/**
 * @brief LED_Driver_ON
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_ON(uint8 led_num);

/**
 * @brief LED_Driver_OFF
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_OFF(uint8 led_num);

/**
 * @brief LED_Driver_Turn
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_Turn(uint8 led_num);

#endif
