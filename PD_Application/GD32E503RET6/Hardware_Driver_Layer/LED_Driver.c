#include "LED_Driver.h"

#define LED1_ON_LEVEL()		gpio_bit_set(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED2_ON_LEVEL()		gpio_bit_set(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED3_ON_LEVEL()		gpio_bit_set(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED4_ON_LEVEL()		gpio_bit_set(LED4_GPIO_PORT, LED4_GPIO_PIN)

#define LED1_OFF_LEVEL()	gpio_bit_reset(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED2_OFF_LEVEL()	gpio_bit_reset(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED3_OFF_LEVEL()	gpio_bit_reset(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED4_OFF_LEVEL()	gpio_bit_reset(LED4_GPIO_PORT, LED4_GPIO_PIN)

#define LED1_TURN()			do{if(gpio_output_bit_get(LED1_GPIO_PORT, LED1_GPIO_PIN)){LED1_OFF_LEVEL();}else{LED1_ON_LEVEL();}}while(0)
#define LED2_TURN()			do{if(gpio_output_bit_get(LED2_GPIO_PORT, LED2_GPIO_PIN)){LED2_OFF_LEVEL();}else{LED2_ON_LEVEL();}}while(0)
#define LED3_TURN()			do{if(gpio_output_bit_get(LED3_GPIO_PORT, LED3_GPIO_PIN)){LED3_OFF_LEVEL();}else{LED3_ON_LEVEL();}}while(0)
#define LED4_TURN()			do{if(gpio_output_bit_get(LED4_GPIO_PORT, LED4_GPIO_PIN)){LED4_OFF_LEVEL();}else{LED4_ON_LEVEL();}}while(0)

void LED_Driver_Init(void)
{
	gpio_init(LED1_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX, LED1_GPIO_PIN);
	gpio_init(LED2_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX, LED2_GPIO_PIN);
	gpio_init(LED3_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX, LED3_GPIO_PIN);
	gpio_init(LED4_GPIO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_MAX, LED4_GPIO_PIN);
}

/**
 * @brief LED_Driver_ON
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_ON(uint8 led_num)
{
	if(LED1 == led_num)
	{
		LED1_ON_LEVEL();
	}
	else if (LED2 == led_num)
	{
		LED2_ON_LEVEL();
	}
	else if (LED3 == led_num)
	{
		LED3_ON_LEVEL();
	}
	else if (LED4 == led_num)
	{
		LED4_ON_LEVEL();
	}
}

/**
 * @brief LED_Driver_OFF
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_OFF(uint8 led_num)
{
	if(LED1 == led_num)
	{
		LED1_OFF_LEVEL();
	}
	else if (LED2 == led_num)
	{
		LED2_OFF_LEVEL();
	}
	else if (LED3 == led_num)
	{
		LED3_OFF_LEVEL();
	}
	else if (LED4 == led_num)
	{
		LED4_OFF_LEVEL();
	}
}

/**
 * @brief LED_Driver_Turn
 * 
 * @param led_num:	LED_BSP_Num_e
 */
void LED_Driver_Turn(uint8 led_num)
{
	if(LED1 == led_num)
	{
		LED1_TURN();
	}
	else if (LED2 == led_num)
	{
		LED2_TURN();
	}
	else if (LED3 == led_num)
	{
		LED3_TURN();
	}
	else if (LED4 == led_num)
	{
		LED4_TURN();
	}
}