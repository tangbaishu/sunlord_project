#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "gd32e50x.h"

#define LED_NUMBER		4

#define LED1_GPIO_CLOCK	RCU_GPIOA
#define LED1_GPIO_PORT	GPIOA
#define LED1_GPIO_PIN	GPIO_PIN_7

#define LED2_GPIO_CLOCK	RCU_GPIOA
#define LED2_GPIO_PORT	GPIOA
#define LED2_GPIO_PIN	GPIO_PIN_8

#define LED3_GPIO_CLOCK	RCU_GPIOA
#define LED3_GPIO_PORT	GPIOA
#define LED3_GPIO_PIN	GPIO_PIN_9

#define LED4_GPIO_CLOCK	RCU_GPIOA
#define LED4_GPIO_PORT	GPIOA
#define LED4_GPIO_PIN	GPIO_PIN_10

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

typedef enum
{
	LED1 = 1,
	LED2,
	LED3,
	LED4,
}LED_BSP_Num_e;


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
